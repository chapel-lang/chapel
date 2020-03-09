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
#define YYLAST   17918

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
     768,   769,   770,   771,   778,   779,   780,   781,   785,   786,
     790,   791,   795,   796,   797,   807,   807,   812,   813,   814,
     815,   816,   817,   818,   822,   823,   824,   825,   830,   829,
     845,   844,   861,   860,   876,   875,   891,   895,   900,   908,
     919,   926,   927,   928,   929,   930,   931,   932,   933,   934,
     935,   936,   937,   938,   939,   940,   941,   942,   943,   944,
     950,   956,   962,   968,   975,   982,   986,   993,   997,   998,
     999,  1000,  1001,  1003,  1005,  1007,  1012,  1015,  1016,  1017,
    1018,  1019,  1020,  1024,  1025,  1029,  1030,  1031,  1035,  1036,
    1040,  1043,  1045,  1050,  1051,  1055,  1057,  1059,  1066,  1076,
    1090,  1095,  1100,  1108,  1109,  1114,  1115,  1117,  1122,  1138,
    1145,  1154,  1162,  1166,  1173,  1174,  1179,  1184,  1178,  1211,
    1214,  1218,  1226,  1236,  1225,  1268,  1272,  1277,  1281,  1286,
    1293,  1294,  1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,  1314,  1315,
    1316,  1317,  1318,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1339,  1340,  1344,  1345,  1349,  1353,  1354,  1355,  1359,  1361,
    1363,  1365,  1367,  1372,  1373,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1389,  1390,  1391,  1392,  1393,  1394,
    1398,  1399,  1403,  1404,  1405,  1406,  1407,  1408,  1412,  1413,
    1416,  1417,  1421,  1422,  1426,  1431,  1435,  1436,  1437,  1445,
    1446,  1448,  1450,  1452,  1457,  1459,  1464,  1465,  1466,  1467,
    1468,  1469,  1470,  1474,  1476,  1481,  1483,  1485,  1490,  1503,
    1520,  1521,  1523,  1528,  1529,  1530,  1531,  1532,  1536,  1542,
    1550,  1551,  1559,  1561,  1566,  1568,  1570,  1575,  1577,  1579,
    1586,  1587,  1588,  1593,  1595,  1597,  1601,  1605,  1607,  1611,
    1619,  1620,  1621,  1622,  1623,  1628,  1629,  1630,  1631,  1632,
    1652,  1656,  1660,  1668,  1675,  1676,  1677,  1681,  1683,  1689,
    1691,  1693,  1698,  1699,  1700,  1701,  1702,  1708,  1709,  1710,
    1711,  1715,  1716,  1720,  1721,  1722,  1726,  1727,  1731,  1732,
    1736,  1737,  1741,  1742,  1743,  1744,  1748,  1749,  1760,  1762,
    1764,  1770,  1771,  1772,  1773,  1774,  1775,  1777,  1779,  1781,
    1783,  1785,  1787,  1790,  1792,  1794,  1796,  1798,  1800,  1802,
    1804,  1807,  1809,  1811,  1816,  1818,  1820,  1822,  1824,  1826,
    1828,  1830,  1832,  1834,  1836,  1838,  1840,  1847,  1853,  1859,
    1865,  1874,  1884,  1892,  1893,  1894,  1895,  1896,  1897,  1898,
    1899,  1904,  1905,  1909,  1913,  1914,  1918,  1922,  1923,  1927,
    1931,  1935,  1942,  1943,  1944,  1945,  1946,  1947,  1951,  1952,
    1957,  1959,  1963,  1967,  1971,  1979,  1984,  1990,  1996,  2003,
    2012,  2016,  2023,  2031,  2032,  2033,  2034,  2035,  2036,  2037,
    2038,  2039,  2041,  2043,  2045,  2047,  2049,  2054,  2055,  2059,
    2060,  2061,  2065,  2066,  2067,  2068,  2079,  2080,  2081,  2082,
    2086,  2087,  2088,  2092,  2093,  2094,  2095,  2096,  2104,  2105,
    2106,  2107,  2111,  2112,  2116,  2117,  2118,  2119,  2120,  2121,
    2122,  2123,  2124,  2125,  2126,  2127,  2128,  2132,  2140,  2141,
    2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,
    2155,  2156,  2157,  2158,  2159,  2160,  2161,  2162,  2163,  2164,
    2165,  2166,  2167,  2171,  2172,  2173,  2174,  2175,  2176,  2177,
    2181,  2182,  2183,  2184,  2188,  2189,  2190,  2191,  2196,  2197,
    2198,  2199,  2200,  2201,  2202
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

#define YYPACT_NINF -985

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-985)))

#define YYTABLE_NINF -575

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -985,   118,  2935,  -985,   -37,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,  4421,   102,   232,  -985, 13067,  -985, 17527,   102,
    9998,  -985,   213,    89,   232,  4421,  9998,  4421,   127, 17585,
   10161,  8030,  -985,  8193,  8849,  6716,  9998,  -985,  9998,   140,
    -985,  -985, 17622,  -985,  2626,  9012,  -985,  9998,   220,  -985,
    -985,  -985,  9998,  -985, 13067,  -985,  9998,   281,   239,   177,
   17810,  -985, 17680,  -985,  9177,  7539,  9998,  9012, 13067,  9998,
     347,  -985,   268,  4421,  -985,  9998,  -985, 10324, 10324, 17622,
    -985,  -985, 13067,  -985,  -985,  9998,  9998,  -985,  9998,  -985,
    9998,  -985,  -985, 12606,  9998,  -985,  9998,  -985,  -985,  -985,
    3433,  6881,  8358,  -985,  4258,  -985,   320,  -985,   399,  -985,
     377,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
   17622,  -985, 17622,   276,   199,  -985,  -985,  2626,  -985,   356,
    -985,   362,  -985,  -985,   370,   374,   385,  9998,   379,   380,
   17023, 12599,   -11,   381,   384,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,   319,  -985,  -985, 17023,   387,  4421,  -985,  -985,
     388,  9998,  -985,  -985,  9998,  9998,  9998,  -985,  9998,  9177,
    9177,    80,  -985,  -985,  -985,  -985,   -22,    81,  -985,  -985,
     382, 15292, 17622,  2626,  -985,   393,  -985,   129, 17023,   442,
    7704,   474, 17717, 17023,   689,   485,  -985, 17775, 17622,   689,
   17622,   402,    48, 14730,    26, 14654,   314,  -985, 15142, 13930,
   15235,  7704,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,  -985,  -985,  -985,  -985,  4421,  -985,   414,  1094,
      84,     5,  -985,  4421,  -985,  -985, 14882,   365,  -985,   415,
     421,  -985, 14882,   -22,   365,  -985,  7704,  1290,  -985,  -985,
    9340,  -985,  -985,  -985, 17622,  -985,   167, 17023,  9998,  9998,
    -985, 17023,   430, 15043,  -985, 14882,   -22, 17023,   427,  7704,
    -985, 17023, 15484,  -985,  -985, 15583, 13060,  -985,  -985, 15629,
     478,   435,   -22, 14882, 15682,   577,   577, 17361,   365,   365,
     225,  -985,  -985,  3598,   124,  -985,  9998,  -985,    59,    67,
    -985,   -76,    -3, 15728,   -35,   589,  -985,  3763,  -985,   543,
    9998,   469,   443,  -985,  -985,  -985,   227,   290,  -985,  9998,
     470,  9998,  9998,  9998,  8849,  8849,  9998,   301,  9998,  9998,
    9998,  9998,  9998,   520, 12606,  9998,  9998,  9998,  9998,  9998,
    9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,
    -985,  -985,  -985,  -985,  -985,  -985,  8521,  8521,  -985,  -985,
    -985,  -985,  8521,  -985,  -985,  8521,  8521,  7704,  7704,  8849,
    8849,  7376,  -985,  -985, 15385, 15438, 15827,    13,  3928,  -985,
    8849,    48,   454,   245,  -985,  9998,  -985,  9998,   496,  -985,
     449,   479,  -985,  -985,  -985, 17622,  -985,  2626,  -985, 17622,
     462,  -985,  2626,   575,  9177,  -985,  4584,  8849,  -985,   458,
    -985,    48,  4747,  8849,  -985,    48, 10487,  9998,  -985,  4421,
     590,  9998,    64,  -985,   463,  -985,    68,  -985,  -985,  1094,
    -985,   491,   471,  -985, 10650,   516,  9998,  2626,  -985,  -985,
    9998,  9998,  -985,   473,  -985,  -985,  9177,  -985, 17023, 17023,
    -985,    31,  -985,  7704,   477,  -985,   615,  -985,   615,  -985,
   10813,   502,  -985,  -985,  -985,  -985,  -985,  -985,  8686,  -985,
   13669,  7046,  -985,  7211,  -985,  4421,   481,  8849,  6218,  3268,
     482,  9998,  6550,  -985,  -985,   298,  -985,  4093, 17622,   252,
   15195,  9177,   483, 17490,   367,  -985, 15873, 17228, 17228,   371,
    -985,   371,  -985,   371, 17304,  2507,  1822,  2334,   -22,   577,
     480,  -985,  -985,  -985,  -985, 17361, 15765,   371, 12759, 12759,
   17228, 12759, 12759,  2063,   577, 15765, 17268,  2063,   365,   365,
     577,   489,   493,   494,   495,   498,   492,   499,  -985,   371,
    -985,   371,    55,  -985,  -985,  -985,   104,  -985, 13622, 17063,
     392, 10976,  8849, 11139,  8849,  9998,  8849, 13329,   102, 15918,
    -985,  -985,  -985, 17023, 16018,  7704,  -985,  7704, 17622,   469,
     253, 17622,   469,  -985,   277,  9998,   113,  9998, 17023,    54,
   14958,  7376,  -985,  9998, 17023,    45, 14806,  -985,   503,   523,
     506, 16063,   523,   508,   634, 16103,  4421, 14990,   517,  -985,
      62,  -985,  -985,  -985,  -985,  -985,  -985,   475,   144,  -985,
   13409,  -985,   448,   510,  1094,    84,   127,   140,  9998,  9998,
    6053,  -985,  -985,   334,  7867,  -985, 17023,  -985, 13709, 13762,
    -985,  -985, 17023,   513,    19,   512,  -985,  2498,  -985,  -985,
     284, 17622,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  4421,
     -13,  1411,  -985,  -985, 17023,  4421, 17023,  -985, 16215,  -985,
    -985,  -985,  9998,  -985,  9998, 10487,  9998,   540,  1638,   518,
     566,   130,  -985,   604,  -985,  -985,  -985,  -985,  2769,   522,
    -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  7376,
    -985,    35,  8849,  8849,  9998,   658, 16255,  9998,   659, 16295,
     525,  2139,    48,    48,  -985,  -985,  -985,  -985,   469,   532,
    -985,   469,   533,  -985, 14882,  -985, 14010,  4910,  -985,  5073,
    -985,   169,  -985, 14090,  5236,  -985,    48,  5399,  -985,    48,
    -985,  9998,  -985,  4421,  9998,  -985,  4421,   669,  -985,  -985,
    -985, 17622,   611,  -985,  1094,   568,   624,  -985,  -985,  -985,
      93,  -985,  -985,   516,   544,    61,  -985,  -985,   546,   552,
    -985,  5562,  9177,  -985,  -985,  -985, 17622,  -985,   588,   382,
    -985,  -985,  5725,   560,  5888,   563,  -985,  9998, 16407, 15537,
     576,   578,   569,  1733,  -985,  9998, 17622,  -985,  -985,   448,
     580,   305,  -985,   600,  -985,   602,   605,   618,   607,   613,
    -985,   616,   619,   617,   622,   625,   326,   623,   626,   627,
    -985,  -985, 17622,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
    -985,  -985,  9998,  -985,   632,   640,   633,   580,   580,  -985,
    -985,  -985,   516,   233,   242, 16447, 11302, 11465, 16487, 11628,
   11791, 11954, 12117,  -985,  -985,   597,  -985,   612,  -985,  -985,
    4421,  9998, 17023,  9998, 17023,  7376,  -985,  4421,  9998, 17023,
    -985,  9998, 17023,  -985, 17023,  -985, 17023,   749,  4421,   568,
    -985,   620,  9505,   168,  -985,    50,  -985,  -985,  8849, 13198,
    7704,  7704,  4421,  -985,    51,   637,  9998,  -985,  9998,  -985,
   17023,  4421,  9998,  -985, 17023,  4421, 17023,  9998, 10487,  9998,
    -985,  -985,  -985,  -985,  1290,  -985,  2083,  -985, 17023,  -985,
     101,    68,  -985,  -985, 13854,  -985, 13467,  -985,  -985,  -985,
    9998,  9998,  9998,  9998,  9998,  9998,  9998,  9998,  -985,  1978,
    -985,  2319, 16103, 14170, 14250,  -985, 16103, 14330, 14410,  4421,
    -985,   516,   568,  6384,  -985,  -985,  -985,   154,  9177,  -985,
    -985,   145,  9998,     6, 13523,  -985,   653,   638,   641,   442,
    -985,   382, 17023, 14490,  -985, 14570,  -985, 17023,   630,   642,
    -985,  -985, 12280,   686,   243,  -985,   657,   665,   580,   580,
   16599, 16639, 16679, 16791, 16831, 16871, 17199,  -985, 17213,  -985,
    4421,  4421,  4421,  4421,  -985,  -985,   168,  9670,    85,  -985,
   17023,  -985,   117,  -985,   -15,  -985,   734, 16983,  -985,  -985,
    -985, 12117,   651,   655,  -985,  4421,  4421,  -985,  -985,  3100,
    -985,  -985,   205,  -985,    50,  -985,  -985,  -985,  9998,  9998,
    9998,  9998,  9998,  9998,  -985,  -985, 16103, 16103, 16103, 16103,
    -985,  -985,  -985,  -985,  -985,   390,  8849, 12771,  -985,  9998,
     145,   117,   117,   117,   117,   117,   117,   145,   862,  -985,
    -985, 16103, 16103,   631, 12443,    97,    86, 13612,  -985,  -985,
   17023,  -985,  -985,  -985,  -985,  -985,  -985,  -985,   652,  -985,
    -985,   302, 12936,  -985,  -985,  -985,  9835,  -985,   432,  -985
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    92,   565,   566,   567,   568,
     569,   570,     0,   491,    78,   113,   459,   119,   461,   491,
       0,   118,     0,   364,    78,     0,     0,     0,     0,   463,
     537,   537,   562,     0,     0,     0,     0,   117,     0,     0,
     240,   114,     0,   236,     0,     0,   121,     0,   510,   482,
     571,   122,     0,   241,   453,   363,     0,     0,     0,   135,
      45,   116,   462,   365,     0,     0,     0,     0,   457,     0,
       0,   120,     0,     0,    93,     0,   563,     0,     0,     0,
     115,   222,   455,   367,   123,     0,     0,   620,     0,   622,
       0,   623,   624,   536,     0,   621,   618,   440,   132,   619,
       0,     0,     0,     4,     0,     5,     0,     9,    47,    10,
       0,    11,    12,    13,    15,   436,   437,    23,    14,   133,
     142,   143,    16,    18,    17,    20,    21,    22,    19,   141,
       0,   139,     0,   528,     0,   137,   140,     0,   138,   542,
     524,   438,   525,   443,   441,     0,     0,     0,   529,   530,
       0,   442,     0,   543,   544,   545,   564,   523,   445,   444,
     526,   527,     0,    39,    25,   451,     0,     0,   492,    79,
       0,     0,   461,   463,     0,     0,     0,   462,     0,     0,
       0,   528,   542,   441,   529,   530,   460,   442,   543,   544,
       0,   491,     0,     0,   366,     0,   196,     0,   421,     0,
     428,   568,   462,   538,   239,   568,   160,   462,     0,   239,
       0,     0,     0,     0,     0,     0,     0,   147,     0,     0,
       0,   428,    86,    94,   106,   100,    99,   108,    89,    98,
     109,    95,   110,    87,   111,   104,    97,   105,   103,   101,
     102,    88,    90,    96,   107,   112,     0,    91,     0,     0,
       0,     0,   370,     0,   129,    33,     0,   605,   514,   511,
     512,   513,     0,   454,   606,     7,   428,   239,   220,   230,
     537,   221,   134,   335,   440,   418,     0,   417,     0,     0,
     130,   541,     0,     0,    36,     0,   458,   446,     0,   428,
      37,   452,     0,   203,   199,     0,   442,   203,   200,     0,
     360,     0,   456,     0,     0,   607,   609,   535,   604,   603,
       0,    49,    52,     0,     0,   423,     0,   425,     0,     0,
     424,     0,     0,   417,     0,     0,     6,     0,    48,     0,
       0,   223,     0,   321,   320,   242,     0,   439,    24,     0,
     515,     0,     0,     0,     0,     0,     0,   608,     0,     0,
       0,     0,     0,     0,   534,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,   287,   288,   289,   284,   286,     0,     0,   282,   285,
     283,   281,     0,   291,   290,     0,     0,   428,   428,     0,
       0,     0,    26,    27,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,    29,     0,    30,     0,   436,   434,
       0,   429,   430,   435,   154,     0,   157,     0,   150,     0,
       0,   156,     0,     0,     0,   169,     0,     0,   168,     0,
     177,     0,     0,     0,   175,     0,     0,    58,   144,     0,
     189,     0,     0,    70,     0,    31,   303,   237,   374,     0,
     375,   377,     0,   399,     0,   380,     0,     0,   128,    32,
       0,     0,    34,     0,   136,   334,     0,    72,   539,   540,
     131,     0,    35,   428,     0,   210,   201,   197,   202,   198,
       0,   358,   355,   162,    38,    51,    50,    53,     0,   572,
       0,     0,   558,     0,   560,     0,     0,     0,     0,     0,
       0,     0,     0,   576,     8,     0,    41,     0,     0,     0,
      54,     0,     0,     0,   314,   369,   490,   600,   599,   602,
     611,   610,   615,   614,   596,   593,   594,   595,   532,   583,
       0,   555,   556,   554,   553,   533,   587,   598,   592,   590,
     601,   591,   589,   581,   586,   588,   597,   580,   584,   585,
     582,     0,     0,     0,     0,     0,     0,     0,   613,   612,
     617,   616,   502,   503,   505,   507,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   574,   491,   491,
     165,   356,   368,   422,     0,     0,   448,     0,     0,   223,
       0,     0,   223,   357,     0,     0,     0,     0,   466,     0,
       0,     0,   178,     0,   472,     0,     0,   176,   619,    61,
       0,    54,    59,     0,   188,     0,     0,     0,     0,   447,
     308,   305,   306,   307,   311,   312,   313,   303,     0,   296,
       0,   304,   322,     0,   378,     0,   126,   127,   125,   124,
       0,   398,   397,   524,     0,   372,   522,   371,     0,     0,
     552,   420,   419,     0,     0,     0,   449,     0,   204,   362,
     524,     0,   573,   531,   559,   426,   561,   427,   185,     0,
       0,     0,   575,   183,   476,     0,   578,   577,     0,    43,
      42,    40,     0,    65,     0,     0,    58,   224,     0,     0,
     234,     0,   231,   318,   315,   316,   319,   243,     0,     0,
      76,    77,    75,    74,    73,   550,   551,   504,   506,     0,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   432,   433,   431,   223,     0,
     159,   223,     0,   158,     0,   187,     0,     0,   167,     0,
     166,     0,   497,     0,     0,   173,     0,     0,   171,     0,
     145,     0,   146,     0,     0,   191,     0,   193,    71,   309,
     310,     0,   303,   294,     0,   412,   323,   326,   325,   327,
       0,   376,   379,   380,     0,     0,   381,   382,   520,   521,
     212,     0,     0,   211,   214,   450,     0,   205,   208,     0,
     359,   186,     0,     0,     0,     0,   184,     0,    56,    55,
       0,     0,     0,   239,   229,     0,   232,   228,   317,   322,
     292,    80,   275,    94,   273,   100,    99,    83,    98,    95,
     278,   110,    81,   111,    97,   101,    82,    84,    96,   112,
     272,   254,   257,   255,   256,   267,   258,   271,   263,   261,
     274,   277,   262,   260,   265,   270,   259,   264,   268,   269,
     266,   276,     0,   252,     0,    85,     0,   292,   292,   250,
     557,   495,   380,   542,   542,     0,     0,     0,     0,     0,
       0,     0,     0,   164,   163,     0,   225,     0,   225,   170,
       0,     0,   465,     0,   464,     0,   496,     0,     0,   471,
     174,     0,   470,   172,    55,   190,   481,   192,     0,   412,
     297,     0,     0,   380,   324,   339,   373,   403,     0,   574,
     428,   428,     0,   216,     0,     0,     0,   206,     0,   181,
     478,     0,     0,   179,   477,     0,   579,     0,     0,    58,
      66,    68,   219,   135,   239,   218,   239,   226,   235,   233,
       0,   303,   249,   253,     0,   279,     0,   245,   246,   499,
       0,     0,     0,     0,     0,     0,     0,     0,   225,   239,
     225,   239,   469,     0,     0,   498,   475,     0,     0,     0,
     195,   380,   412,     0,   415,   414,   416,   524,   336,   300,
     298,     0,     0,     0,     0,   401,   524,     0,     0,   217,
     215,     0,   209,     0,   182,     0,   180,    57,     0,     0,
     227,   394,     0,   328,     0,   251,    80,    82,   292,   292,
       0,     0,     0,     0,     0,     0,   239,   153,   239,   149,
       0,     0,     0,     0,   194,   299,   380,   404,     0,   338,
     337,   353,     0,   354,   341,   344,     0,   340,   332,   333,
     238,     0,   516,   517,   207,     0,     0,    67,    69,     0,
     393,   392,   524,   329,   339,   293,   247,   248,     0,     0,
       0,     0,     0,     0,   155,   151,   468,   467,   474,   473,
     302,   301,   406,   407,   409,   524,     0,   574,   352,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   518,
     519,   480,   479,     0,   384,     0,     0,     0,   408,   410,
     343,   345,   346,   349,   350,   351,   347,   348,   342,   389,
     387,   524,   574,   330,   244,   331,   404,   388,   524,   411
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -985,  -985,  -985,     4,  -434,  2012,  -985,  -985,  -985,  -985,
     386,   486,  -325,  -658,  -654,  -985,  -985,  -985,  -985,   340,
     786,  -985,   -17,  -161,  -671,  -985,  -823,   564,  -919,  -756,
    -985,   -57,  -985,  -985,  -985,  -985,  -985,  -985,  -985,  -985,
     265,  -985,  -985,  -985,   521,  -985,    28,  -985,  -985,  -985,
    -985,  -985,  -985,  -544,  -598,  -985,  -985,  -985,    16,   988,
    -985,  -985,  -985,    98,  -985,  -985,  -985,  -985,  -123,  -150,
    -789,  -985,  -116,    69,   203,  -985,  -985,  -985,    24,  -985,
    -985,  -261,   336,  -190,  -216,  -246,  -230,  -238,  -985,  -179,
    -985,    12,   807,  -122,   394,  -985,  -440,  -753,  -978,  -985,
    -585,  -473,  -984,  -930,  -796,   -53,  -985,    22,  -985,  -191,
    -985,   266,   468,  -408,  -985,  -985,  -985,  1078,  -985,    -9,
    -985,  -985,  -199,  -985,  -584,  -985,  -985,  -985,  1192,  1439,
     -12,   826,   -63,   728,  -985,  1634,  1821,  -985,  -985,  -985,
    -985,  -985,  -985,  -985,  -985,  -350
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   312,   104,   614,   106,   107,   108,   329,
     109,   313,   609,   613,   610,   110,   111,   112,   113,   114,
     170,   853,   250,   115,   247,   116,   641,   255,   117,   118,
     267,   119,   120,   121,   419,   591,   415,   588,   122,   123,
     720,   124,   125,   126,   476,   658,   789,   127,   128,   654,
     784,   129,   130,   512,   803,   131,   132,   691,   692,   181,
     248,   632,   134,   135,   514,   809,   697,   856,   857,   441,
     942,   447,   628,   629,   630,   631,   698,   335,   770,  1054,
    1114,  1040,   275,   979,   983,  1034,  1035,  1036,   136,   301,
     481,   137,   138,   251,   252,   451,   452,   645,  1051,  1003,
     455,   642,  1073,   976,   903,   314,   197,   318,   319,   410,
     411,   412,   182,   140,   141,   142,   143,   183,   145,   167,
     168,   566,   431,   741,   567,   568,   146,   147,   184,   185,
     150,   209,   413,   187,   152,   188,   189,   155,   156,   157,
     324,   158,   159,   160,   161,   162
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     165,   386,   282,   272,   186,   509,   103,   659,   191,   633,
     190,   276,   627,   402,   198,   336,   435,   742,   203,   203,
     906,   213,   215,   218,   219,   246,   220,   855,   801,   420,
     444,   800,   653,   256,   193,   257,   453,   429,   320,   408,
     262,   570,   263,   497,   264,   729,   643,   937,   732,   322,
     773,   456,   277,   281,   283,   285,   286,   287,   253,   576,
     408,   291,   300,   292,  1039,   295,   299,     5,   947,   948,
     302,   403,   660,   303,   304,   463,   305,   744,   306,   974,
     253,   307,   308,   253,   309,   453,   737,   781,   277,   281,
     323,    65,   495,  1098,   453,   620,  1079,  1074,   474,  -213,
     429,   707,  1001,   971,   502,   408,   981,   908,   759,   949,
     429,   347,   612,   331,   621,   332,  1110,   622,     3,   353,
    1031,   163,   496,   500,  1080,   861,   862,   397,   408,   782,
    -395,  1076,  1119,   503,  1117,   340,   498,   623,   708,   429,
     624,  -213,    57,   576,   457,   760,   386,  1099,  1031,  -395,
     980,   625,   498,  -395,   387,   792,   388,  -390,   429,   165,
      74,   982,   394,   395,   396,   499,   291,   277,   323,   398,
     626,    65,   194,   454,  -395,   300,  1026,  1039,  -395,  1050,
    1000,   577,   401,   783,   875,   905,  -390,   877,   281,   398,
     466,   300,   534,   423,   772,  -213,   556,   557,   491,  -390,
     498,  -395,  -395,   937,  -395,   937,   493,  -295,   398,   281,
     464,   398,  -390,  1033,   398,   166,   417,   398,  1025,  1056,
    1057,   422,   454,  -395,   498,   492,   408,   408,   193,   909,
    1032,   454,   450,   494,  -295,   169,   498,   -46,   258,  1002,
      23,  1033,  -395,   709,  1113,  -549,  -546,  -549,  -546,   398,
     333,   570,   466,  1077,   281,   -46,  -395,   465,   203,  -390,
     937,  -391,   937,   488,  -390,  1112,   468,   469,   405,   806,
     710,   999,  -413,  1071,   998,   855,   334,   281,   959,   735,
     961,   618,   655,   762,   -64,    55,   644,   406,   489,   265,
    -391,   259,   200,  -413,   807,   590,    63,  -413,   212,   214,
     594,   965,   416,  -391,   490,   221,   466,   421,   885,   260,
     763,   978,   408,   551,   552,   192,  -391,   487,   510,   553,
    -413,    83,   554,   555,   901,   467,   261,   516,   627,   517,
     518,   519,   521,   523,   524,   886,   525,   526,   527,   528,
     529,    23,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,  -383,   570,
    1016,   612,  1018,  -391,   281,   281,   457,   321,  -391,   936,
     281,   596,  -501,   281,   281,   281,   281,   559,   561,   569,
    -396,  -500,   762,   163,   457,   515,    55,  -383,   579,   485,
    -396,   682,   457,   583,   693,   584,   343,    63,   589,  -501,
    -383,   389,   592,   582,   266,   390,   746,   749,  -500,  1055,
     683,   730,   277,  -383,   598,   600,   457,   294,   298,  -396,
     604,   606,    83,  -361,   611,   611,   408,   615,   320,   617,
     320,   254,   450,   289,  -487,   733,   985,   317,   -86,   694,
     288,  -549,  -361,  -549,   646,  -396,   -92,   344,   648,   649,
     695,   345,  -396,   254,   652,  -548,   163,  -548,   687,   -88,
    -383,   281,   679,   293,   297,  -383,  -547,   -93,  -547,   696,
     139,   793,   795,  -396,   713,   766,   652,   328,   390,   281,
     139,   281,   790,   327,   330,   671,   652,   674,  -385,   676,
     678,   681,  -396,   139,   977,   139,   690,  -396,   347,   277,
    -396,   986,   620,   351,   347,   352,   353,   627,  -405,   351,
     356,   487,   353,   665,  -484,   667,   356,  -385,   363,   337,
     767,   621,   726,     5,   622,   936,   369,   936,  -483,  -405,
    -385,   768,   338,  -405,   339,   570,   409,  -488,  -489,  -486,
     530,   139,  -485,  -385,   623,   398,   393,   624,   858,   325,
     769,   404,   391,   531,   407,   414,  -405,   409,   625,   598,
     716,   604,   719,   615,   721,   674,   418,   424,   139,   722,
     723,   728,   139,   281,   731,   281,   399,   626,   532,   446,
     460,   777,   936,   734,   936,   736,   461,   775,   470,   569,
    -385,   743,   473,   482,  1052,  -385,   480,   504,   254,   254,
     254,   254,   409,   508,   612,   440,   513,   511,   343,   520,
     522,   353,   581,   765,   585,   586,    74,   450,   587,  1075,
     593,   595,   533,   601,   616,   409,   287,   291,   323,   619,
     634,   284,   281,  1088,   644,   139,   657,   635,   620,   650,
     788,   661,   254,   656,   300,   699,   688,   700,   254,   669,
     675,   701,   702,   703,   558,   560,   704,   621,   705,   344,
     622,   -60,   682,   345,   750,   578,   752,   706,   753,  1075,
     798,   254,   799,   611,   611,   758,   771,   780,   785,   466,
     623,   854,   804,   624,   805,    57,  1111,   808,   860,   254,
     866,   869,   599,   871,   625,   876,   878,   569,   605,  -400,
     521,   559,   865,   898,  1118,   868,   902,   904,  1075,  -400,
     347,   910,   907,   626,   139,   351,    23,   911,   353,   987,
     988,   139,   356,   409,   409,   882,   916,   884,   921,   914,
     151,   925,   889,   932,   930,   892,   931,    40,  -400,   894,
     151,  -113,   896,  -119,   899,   941,  -118,   450,  -117,   408,
     408,   -89,   -87,   151,  -114,   151,   -90,  -121,  -122,    53,
     958,    55,   670,  -116,  -400,   945,  -120,  -115,  -123,   788,
     277,  -400,    63,   -91,   946,   960,   425,   428,   430,   434,
     920,   139,   924,   969,  1053,   926,   972,   254,  1047,   690,
     -86,  1080,  -400,   938,  1078,   139,  1009,    83,   -88,  1109,
    1048,   151,   651,   991,  1042,   296,   296,  1043,  1089,   409,
     195,  -400,  1090,   507,   915,   943,  -400,   254,   478,  -400,
     459,   254,   939,  1008,   651,  1004,   462,   317,   151,   317,
     761,   900,   151,   940,   651,  1115,  1070,   715,  1096,   718,
     944,  1108,   217,  1102,  1103,  1104,  1105,  1106,  1107,   472,
    1101,   647,  1081,   727,   882,   884,   204,   889,   892,   920,
     924,     0,     0,     0,     0,     0,   139,   483,   962,   963,
       0,   964,     0,   569,     0,   966,   967,   272,  1082,   968,
    1083,  1084,     0,     0,  1085,  1086,     0,     0,     0,     0,
       0,     0,     0,     0,   139,   151,   984,   674,   281,   281,
     139,     0,     0,     0,   992,     0,   993,   139,  -402,     0,
     995,     0,     0,     0,     0,   997,   611,   611,  -402,     0,
    1028,   725,     0,   409,     0,     0,     0,     0,     0,   854,
       0,     0,     0,     0,     0,     0,     0,     0,   962,  1010,
    1011,   966,  1012,  1013,  1014,  1015,     0,  -402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   323,     0,   139,     0,   580,  1030,   139,     0,     0,
    1037,     0,     0,  -402,   151,   139,     0,   520,   558,     0,
    -402,   151,     0,     0,     0,   254,   254,     0,     0,     0,
     133,   254,   254,     0,     0,   602,  1095,     0,     0,   607,
     133,  -402,     0,   757,     0,     0,     0,     0,  1066,  1067,
    1068,  1069,     0,   133,     0,   133,     0,     0,     0,     0,
    -402,     0,     0,     0,     0,  -402,     0,     0,  -402,   924,
       0,     0,     0,  1091,  1092,     0,   711,   323,     0,     0,
       0,   151,     0,   787,     0,     0,  1066,  1067,  1068,  1069,
    1091,  1092,     0,     0,     0,   151,     0,     0,     0,     0,
       0,   133,     0,     0,  1097,   674,     0,  1100,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,     0,     0,     0,   139,     0,     0,     0,   133,     0,
     144,     0,   133,     0,     0,     0,     0,   222,     0,     0,
     674,     0,     0,   144,   924,   144,     0,     0,   254,   254,
       0,   223,   224,     0,   225,     0,     0,     0,     0,   226,
     254,     0,   440,     0,     0,     0,   151,   227,     0,   440,
       0,     0,   254,   228,     0,   254,     0,   139,   229,     0,
       0,   230,     0,   139,   231,     0,     0,     0,     0,     0,
       0,   144,   232,     0,   151,   133,     0,     0,   233,   234,
     151,     0,     0,   738,   740,   235,   859,   151,     0,   745,
     748,     0,     0,     0,   236,   917,     0,     0,   144,     0,
     863,   864,   144,   237,   238,     0,   239,     0,   240,     0,
     241,     0,     0,   242,   148,     0,     0,   243,   448,     0,
     244,     0,     0,   245,   148,   139,     0,   139,     0,     0,
       0,     0,   139,     0,     0,   139,     0,   148,     0,   148,
       0,   139,     0,   151,   139,     0,     0,   151,     0,     0,
       0,     0,     0,     0,   133,   151,     0,     0,   975,     0,
       0,   133,     0,     0,     0,   144,   409,   409,     0,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   449,
     139,     0,   139,     0,     0,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   873,   874,     0,     0,
       0,     0,   148,     0,     0,     0,   148,     0,   879,     0,
     254,   133,     0,     0,     0,     0,     0,     0,     0,     0,
     890,     0,   268,   893,  1029,   133,    22,    23,     0,     0,
       0,     0,     0,     0,   144,   269,     0,    30,   270,     0,
       0,   144,    35,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,   151,     0,     0,     0,   139,   440,
     440,     0,     0,   440,   440,   139,     0,     0,     0,   148,
      53,     0,    55,  1072,     0,     0,   139,     0,     0,  1038,
       0,   271,     0,    63,     0,     0,     0,  1044,     0,   440,
     139,   440,     0,     0,     0,     0,   133,     0,     0,   139,
       0,   144,    79,   139,     0,    81,     0,   151,    83,     0,
       0,     0,     0,   151,     0,   144,     0,     0,     0,     0,
       0,     0,     0,  1072,   133,     0,     0,     0,     0,     0,
     133,     0,     0,   341,     0,     0,     0,   133,     0,     0,
     342,     0,     0,     0,     0,     0,     0,   139,   148,     0,
       0,   149,   343,     0,     0,   148,     0,     0,     0,     0,
       0,   149,  1072,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   149,   151,   149,   151,     0,     0,
       0,     0,   151,     0,     0,   151,   144,     0,   990,     0,
       0,   151,  1038,   133,   151,     0,     0,   133,   139,   139,
     139,   139,     0,   344,     0,   133,     0,   345,     0,     0,
       0,     0,     0,     0,   144,   148,     0,     0,     0,   151,
     144,     0,   149,   139,   139,     0,     0,   144,     0,   148,
     151,     0,   151,     0,   429,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
       0,     0,     0,   149,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,     0,
     367,   368,   369,   144,     0,     0,     0,   144,     0,   794,
       0,     0,     0,     0,     0,   144,     0,     0,     0,     0,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   133,     0,   149,     0,   151,     0,
       0,     0,     0,     0,     0,   151,     0,     0,   148,     0,
       0,     0,     0,     0,   148,     0,   151,     0,     0,     0,
       0,   148,     0,     0,     0,     0,   153,     0,     0,   802,
     151,     0,     0,     0,     0,     0,   153,     0,     0,   151,
       0,     0,     0,   151,     0,     0,     0,   133,     0,   153,
    -225,   153,     0,   133,  -225,  -225,     0,     0,     0,     0,
       0,     0,     0,  -225,     0,  -225,  -225,     0,     0,     0,
    -225,     0,     0,     0,     0,   149,  -225,   148,     0,  -225,
       0,   148,   149,     0,   144,     0,     0,   151,     0,   148,
       0,     0,     0,     0,     0,     0,     0,   153,  -225,     0,
    -225,     0,  -225,     0,  -225,  -225,     0,  -225,     0,  -225,
       0,  -225,     0,     0,     0,   133,     0,   133,     0,     0,
       0,     0,   133,     0,   153,   133,     0,     0,   153,     0,
    -225,   133,     0,  -225,   133,     0,  -225,   144,   151,   151,
     151,   151,   149,   144,     0,   268,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,   149,     0,   269,   133,
      30,   270,     0,   151,   151,    35,     0,     0,     0,     0,
     133,    40,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -225,     0,     0,     0,
       0,   153,  -225,    53,     0,    55,     0,    57,   148,   933,
       0,     0,   934,     0,   271,   144,    63,   144,     0,     0,
       0,     0,   144,   154,     0,   144,     0,     0,     0,     0,
       0,   144,     0,   154,   144,    79,     0,   149,    81,     0,
       0,    83,     0,     0,     0,     0,   154,     0,   154,     0,
       0,     0,     0,   343,     0,     0,     0,     0,     0,   144,
       0,   148,     0,     0,     0,   149,     0,   148,   133,     0,
     144,   149,   144,     0,     0,   133,     0,     0,   149,     0,
     153,     0,     0,     0,     0,     0,   133,   153,     0,     0,
       0,    98,     0,     0,   154,     0,     0,   935,     0,     0,
     133,     0,     0,     0,   344,     0,     0,     0,   345,   133,
       0,     0,     0,   133,     0,     0,     0,     0,     0,     0,
       0,   154,     0,     0,     0,   154,     0,     0,     0,   148,
       0,   148,     0,     0,   149,     0,   148,     0,   149,   148,
       0,     0,     0,     0,     0,   148,   149,   153,   148,     0,
       0,     0,     0,     0,     0,   347,   348,   133,   144,   350,
     351,   153,   352,   353,     0,   144,     0,   356,     0,     0,
       0,     0,     0,   148,     0,   363,   144,     0,     0,     0,
       0,   367,   368,   369,   148,     0,   148,     0,   154,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,   144,
     268,     0,     0,   144,    22,    23,     0,     0,   133,   133,
     133,   133,     0,   269,   105,    30,   270,     0,     0,     0,
      35,     0,     0,     0,   164,     0,    40,     0,     0,     0,
       0,     0,   153,   133,   133,     0,     0,   196,     0,   199,
       0,     0,     0,     0,     0,     0,     0,   144,    53,     0,
      55,     0,    57,     0,   933,   149,     0,   934,     0,   271,
     153,    63,     0,     0,     0,     0,   153,   154,     0,     0,
       0,     0,   148,   153,   154,     0,     0,     0,     0,   148,
      79,     0,     0,    81,     0,   290,    83,     0,     0,     0,
     148,     0,     0,     0,   343,     0,     0,     0,   144,   144,
     144,   144,     0,     0,   148,   268,     0,     0,   149,    22,
      23,     0,   105,   148,   149,     0,   326,   148,   269,     0,
      30,   270,     0,   144,   144,    35,     0,     0,     0,   153,
       0,    40,     0,   153,   154,     0,    98,     0,     0,     0,
       0,   153,  1017,     0,     0,   344,     0,     0,   154,   345,
       0,   341,     0,    53,     0,    55,     0,   325,   342,   933,
       0,   148,   934,     0,   271,     0,    63,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   149,     0,   149,   392,
       0,     0,     0,   149,     0,    79,   149,     0,    81,     0,
       0,    83,   149,     0,     0,   149,   347,   348,     0,   349,
     350,   351,     0,   352,   353,     0,     0,     0,   356,     0,
       0,     0,   148,   148,   148,   148,   363,     0,     0,   154,
     149,   344,   367,   368,   369,   345,     0,     0,     0,     0,
       0,   149,     0,   149,     0,     0,     0,   148,   148,     0,
       0,    98,     0,     0,     0,     0,     0,   154,     0,     0,
     153,     0,     0,   154,     0,     0,   346,     0,   445,     0,
     154,     0,     0,     0,     0,   458,     0,     0,     0,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,     0,   367,   368,
     369,     0,     0,   153,     0,     0,     0,   872,     0,   153,
       0,     0,     0,     0,     0,     0,   154,     0,     0,   149,
     154,     0,     0,     0,     0,   105,   149,     0,   154,     0,
       0,     0,     0,     0,     0,     0,     0,   149,     0,   105,
       0,   268,     0,     0,     0,    22,    23,     0,     0,     0,
       0,   149,     0,     0,   269,     0,    30,   270,     0,     0,
     149,    35,     0,     0,   149,   343,     0,    40,     0,     0,
       0,   153,     0,   153,     0,     0,     0,     0,   153,     0,
       0,   153,     0,     0,     0,     0,     0,   153,     0,    53,
     153,    55,     0,    57,     0,   933,     0,     0,   934,     0,
     271,     0,    63,     0,     0,     0,     0,     0,   149,     0,
     105,     0,     0,     0,     0,   153,   344,     0,     0,     0,
     345,    79,     0,     0,    81,     0,   153,    83,   153,     0,
       0,     0,     0,     0,     0,     0,     0,   154,   458,     0,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   149,
     149,   149,   149,     0,     0,     0,     0,   347,   348,     0,
       0,     0,   351,     0,   352,   353,     0,    98,     0,   356,
       0,     0,     0,  1019,   149,   149,     0,   363,     0,     0,
     154,     0,     0,   367,   368,   369,   154,     0,     0,     0,
       0,   222,     0,     0,     0,     0,     0,   668,     0,     0,
       0,   673,     0,     0,   153,   223,   224,     0,   225,   105,
       0,   153,     0,   226,     0,     0,     0,     0,     0,     0,
       0,   227,   153,     0,     0,     0,     0,   228,   343,     0,
       0,     0,   229,     0,     0,   230,   153,     0,   231,     0,
       0,     0,     0,     0,     0,   153,   232,     0,   154,   153,
     154,     0,   233,   234,     0,   154,     0,     0,   154,   235,
       0,     0,     0,     0,   154,     0,     0,   154,   236,     0,
       0,     0,     0,     0,     0,     0,     0,   237,   238,   344,
     239,     0,   240,   345,   241,     0,     0,   242,     0,     0,
       0,   243,   154,   153,   244,     0,     0,   245,     0,     0,
       0,     0,     0,   154,     0,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   755,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,   223,   224,   351,   225,   352,   353,     0,
       0,   226,   356,     0,   153,   153,   153,   153,     0,   227,
     363,   398,     0,   786,     0,   228,   367,   368,   369,     0,
     229,     0,     0,   230,     0,     0,   231,     0,     0,   153,
     153,   791,     0,     0,   232,     0,     0,   796,     0,     0,
     233,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   154,     0,     0,     0,     0,   236,     0,   154,     0,
       0,     0,     0,     0,     0,   237,   238,     0,   239,   154,
     240,     0,   241,     0,     0,   242,     0,     0,     0,   243,
       0,     0,   244,   154,     0,   245,     0,     0,     0,     0,
       0,     0,   154,     0,     0,     0,   154,     0,     0,   458,
       0,   458,     0,     0,     0,     0,   458,     0,     0,   458,
       0,     0,     0,     0,     0,   895,     0,     0,   897,     0,
     810,     0,   811,     0,     0,     0,     0,     0,     0,     0,
       0,   812,     0,     0,     0,     0,   813,   224,   814,   815,
     154,   249,     0,   913,   816,     0,     0,     0,     0,     0,
       0,     0,   227,     0,   919,     0,   923,     0,   817,     0,
       0,     0,     0,   818,     0,     0,   230,     0,     0,   819,
       0,   820,     0,     0,     0,     0,     0,   821,     0,     0,
       0,     0,     0,   822,   823,     0,     0,     0,     0,     0,
     235,   154,   154,   154,   154,     0,     0,     0,     0,   824,
       0,     0,     0,     0,     0,     0,     0,     0,   237,   238,
       0,   825,     0,   240,     0,   826,   154,   154,   827,     0,
       0,     0,   828,     0,     0,   244,     0,     0,   829,     0,
       0,     0,     0,     0,     0,     0,     0,   370,   371,   372,
     373,   374,   375,     0,     0,   378,   379,   380,   381,     0,
     383,   384,   830,   831,   832,   833,   834,     0,     0,   835,
     970,     0,     0,   836,   837,   838,   839,   840,   841,   842,
     843,   844,   845,   846,   989,   847,     0,     0,   848,   849,
     850,   851,     0,   994,   852,    -2,     4,   996,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,  1024,    39,    40,     0,    41,  -239,     0,    42,    43,
      44,     0,    45,    46,    47,   -44,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -239,   -44,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -62,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,  1093,   102,     5,   273,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
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
       0,     0,    94,     0,     0,    95,    96,   274,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   180,  1094,     4,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -574,     0,    12,    13,    14,    15,    16,  -574,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,  -574,
      27,    28,  -574,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -239,   -44,    60,    61,    62,
    -574,    63,    64,    65,  -574,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,  -574,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -574,  -574,    90,  -574,  -574,  -574,  -574,  -574,  -574,
    -574,     0,  -574,  -574,  -574,  -574,  -574,     0,  -574,  -574,
    -574,  -574,  -574,  -574,  -574,  -574,    98,  -574,  -574,  -574,
       0,   100,  -574,   101,   310,   102,     5,   273,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     274,    98,     0,     0,    99,     0,   100,   311,   101,     4,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   486,   101,   505,   102,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,   506,   101,   310,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   311,   101,     4,   102,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,    57,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -62,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,   680,   101,     4,
     102,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,   325,    58,    59,  -239,   -44,    60,    61,    62,
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
      32,    33,    34,    35,    36,    37,    38,     0,    39,    40,
       0,    41,  -239,     0,    42,    43,    44,     0,    45,    46,
      47,   -44,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -239,   -44,
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
       0,    30,    31,    32,    33,    34,    35,   597,    37,    38,
       0,    39,    40,     0,    41,  -239,     0,    42,    43,    44,
       0,    45,    46,    47,   -44,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -239,   -44,    60,    61,    62,     0,    63,    64,    65,
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
     603,    37,    38,     0,    39,    40,     0,    41,  -239,     0,
      42,    43,    44,     0,    45,    46,    47,   -44,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -239,   -44,    60,    61,    62,     0,
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
      33,    34,    35,   881,    37,    38,     0,    39,    40,     0,
      41,  -239,     0,    42,    43,    44,     0,    45,    46,    47,
     -44,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -239,   -44,    60,
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
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   883,    37,    38,     0,
      39,    40,     0,    41,  -239,     0,    42,    43,    44,     0,
      45,    46,    47,   -44,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -239,   -44,    60,    61,    62,     0,    63,    64,    65,     0,
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
       0,    29,     0,    30,    31,    32,    33,    34,    35,   888,
      37,    38,     0,    39,    40,     0,    41,  -239,     0,    42,
      43,    44,     0,    45,    46,    47,   -44,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -239,   -44,    60,    61,    62,     0,    63,
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
      34,    35,   891,    37,    38,     0,    39,    40,     0,    41,
    -239,     0,    42,    43,    44,     0,    45,    46,    47,   -44,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -239,   -44,    60,    61,
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
      24,    25,    26,     0,   912,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,     0,    39,
      40,     0,    41,  -239,     0,    42,    43,    44,     0,    45,
      46,    47,   -44,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -239,
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
      29,     0,    30,    31,    32,    33,    34,    35,   918,    37,
      38,     0,    39,    40,     0,    41,  -239,     0,    42,    43,
      44,     0,    45,    46,    47,   -44,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -239,   -44,    60,    61,    62,     0,    63,    64,
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
      35,   922,    37,    38,     0,    39,    40,     0,    41,  -239,
       0,    42,    43,    44,     0,    45,    46,    47,   -44,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -239,   -44,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -62,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     0,   101,   774,   102,     5,   273,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
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
     274,     0,     0,     0,    99,     0,   179,     0,   101,     0,
     180,     5,   273,     6,     7,     8,     9,    10,    11,     0,
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
       0,   179,     0,   101,     0,   180,   672,     5,   273,     6,
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
       0,   180,  1027,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
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
       0,    99,     0,   179,     0,   101,     0,   180,   677,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,   216,    23,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,    63,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,    83,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     0,   180,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,   278,   279,     0,    80,   315,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,   316,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,   274,     0,
       0,     0,    99,     0,   179,     0,   101,     0,   180,     5,
     273,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,   278,   279,     0,    80,
     315,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,   274,     0,     0,     0,    99,     0,   179,
       0,   101,   664,   180,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,   278,   279,     0,    80,   315,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,   274,     0,
       0,     0,    99,     0,   179,     0,   101,   666,   180,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,   562,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,   563,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,   564,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,   565,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   179,
       0,   101,     5,   180,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   173,     0,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,     0,     0,    76,   278,
     279,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,   280,     0,     0,
      99,     0,   179,     0,   101,     0,   180,     5,   273,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   176,    37,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,   278,   279,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,   274,     0,     0,     0,    99,     0,   179,     0,   101,
       5,   180,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   173,     0,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
     776,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,   278,   279,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,   205,    10,
      11,   206,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   207,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,   208,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,   180,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,   210,    56,     0,    58,     0,
       0,     0,     0,    61,   177,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   178,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,   211,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   179,     0,   101,     0,
     180,     5,   273,     6,     7,     8,     9,    10,    11,     0,
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
       0,     0,     0,   211,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,   274,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,   278,   279,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   179,     0,   101,     0,   180,     5,
     273,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
       0,    95,    96,   274,     0,     0,     0,    99,     0,   179,
     662,   101,     5,   180,     6,     7,     8,     9,    10,    11,
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
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   179,     0,   101,     5,   180,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   171,     0,     0,    15,
      16,     0,    17,     0,   172,     0,     0,    21,     0,     0,
       0,     0,     0,     0,   253,    28,     0,   173,     0,     0,
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
       0,     0,     0,    99,     0,   100,     0,   101,     0,   180,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   274,     0,     0,     0,    99,     0,
     179,     0,   101,     5,   180,     6,     7,     8,   205,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   207,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   178,     0,    74,     0,     0,    76,
       0,     0,   208,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   180,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   636,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   176,    37,
       0,     0,   637,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   177,     0,     0,     0,
       0,     0,     0,     0,    68,   638,    70,    71,    72,   639,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   274,     0,     0,     0,    99,     0,   179,     0,
     101,     0,   973,     5,   273,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   173,     0,     0,     0,    32,
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
       0,     0,    94,     0,     0,    95,    96,   274,     0,     0,
       0,    99,     0,   179,     0,   101,     0,   973,     5,   273,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   171,
       0,     0,    15,    16,     0,    17,     0,   172,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     173,     0,     0,     0,    32,   174,   175,     0,   957,    37,
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
     101,     5,   180,     6,     7,     8,     9,    10,    11,     0,
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
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,   201,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   202,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
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
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   177,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   178,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   100,     0,   101,
       5,   180,     6,     7,     8,     9,    10,    11,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,   608,     0,
     179,     0,   101,     5,   180,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   171,     0,     0,    15,    16,     0,
      17,     0,   172,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,   636,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,   637,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
     638,    70,    71,    72,   639,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,   640,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
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
     640,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   714,
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
     175,     0,   717,    37,     0,     0,    39,     0,     0,    41,
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
       0,    32,   174,   175,     0,   951,    37,     0,     0,    39,
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
     173,     0,     0,     0,    32,   174,   175,     0,   952,    37,
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
       0,    28,     0,   173,     0,     0,     0,    32,   174,   175,
       0,   954,    37,     0,     0,    39,     0,     0,    41,     0,
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
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   179,     0,   101,     5,   180,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
       0,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   955,    37,     0,     0,    39,     0,
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
       0,     0,    99,     0,   179,     0,   101,     5,   180,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   171,     0,
       0,    15,    16,     0,    17,     0,   172,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   173,
       0,     0,     0,    32,   174,   175,     0,   956,    37,     0,
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
     957,    37,     0,     0,    39,     0,     0,    41,     0,     0,
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
       0,     0,     0,   636,     0,   173,     0,     0,     0,    32,
     174,   175,     0,   176,    37,     0,     0,   637,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   177,     0,     0,     0,     0,     0,     0,     0,    68,
     638,    70,    71,    72,   639,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   179,     0,   101,     5,  1049,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   171,     0,     0,
      15,    16,     0,    17,     0,   172,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   173,     0,
       0,     0,    32,   174,   175,     0,   176,    37,     0,     0,
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
      97,     0,     0,     0,    99,     0,     0,     0,   101,     5,
    1049,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,  -508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,   385,    96,    97,  -546,     0,  -546,    99,     0,   179,
       0,   101,     0,   180,     5,   273,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   171,     0,     0,    15,    16,
     343,    17,     0,   172,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   173,     0,     0,     0,
      32,   174,   175,     0,   176,    37,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,   344,    54,     0,    56,   345,    58,     0,     0,     0,
       0,    61,   177,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   178,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,     0,  -404,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,     0,   356,     0,    90,     0,     0,     0,
    -404,   362,   363,     0,  -404,   366,     0,     0,   367,   368,
     369,     0,     0,     0,     0,     0,     0,     0,   274,     0,
       0,     0,     0,     0,   179,     0,   101,  -404,   973,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     171,     0,     0,    15,    16,     0,    17,     0,   172,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   173,     0,     0,     0,    32,   174,   175,     0,   176,
      37,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,  -386,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   177,     0,     0,
       0,  -386,     0,     0,     0,    68,    69,    70,    71,    72,
     178,     0,    74,     0,  -386,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,  -386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    90,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   171,     0,     0,    15,    16,     0,    17,     0,   172,
       0,     0,    21,    97,  -386,     0,     0,     0,     0,  -386,
      28,   101,   173,  1049,     0,     0,    32,   174,   175,     0,
     176,    37,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   177,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   178,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,     0,     0,     0,     0,     0,     0,     0,
      88,     5,    90,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   171,     0,     0,    15,    16,     0,    17,     0,
     172,     0,   385,    21,    97,  -546,     0,  -546,     0,     0,
     179,    28,   101,   173,   180,     0,     0,    32,   174,   175,
       0,   176,    37,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   177,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   178,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    90,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   171,     0,     0,    15,    16,     0,    17,
       0,   172,     0,     0,    21,    97,     0,     0,     0,     0,
       0,   179,    28,   101,   173,   640,     0,     0,    32,   174,
     175,     0,   176,    37,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     177,     0,   222,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   178,     0,    74,   223,   224,    76,   225,
       0,     0,    80,     0,   226,    82,     0,     0,    84,     0,
       0,     0,   227,     0,     0,     0,     0,     0,   228,     0,
       0,     0,     0,   229,     0,     0,   230,     0,     0,   231,
       0,     0,     0,     0,    90,     0,     0,   232,     0,     0,
    1006,     0,     0,   233,   234,     0,     0,     0,     0,   812,
     235,     0,     0,     0,   223,   224,   814,   225,     0,   236,
       0,     0,   226,     0,   101,     0,   180,     0,   237,   238,
     227,   239,     0,   240,     0,   241,   817,     0,   242,     0,
       0,   229,   243,     0,   230,   244,     0,   231,   245,   820,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   822,   234,     0,     0,   341,     0,     0,   235,     0,
       0,     0,   342,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,     0,   343,     0,   237,   238,     0,   239,
       0,   240,     0,  1007,     0,     0,   827,     0,     0,     0,
     243,     0,     0,   244,   764,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,   370,   371,   372,   373,   374,
     375,     0,     0,   378,   379,   380,   381,     0,   383,   384,
     830,   831,   832,   833,   834,   344,     0,   835,     0,   345,
       0,   836,   837,   838,   839,   840,   841,   842,   843,   844,
     845,   846,     0,   847,   341,     5,   848,   849,   850,   851,
       0,   342,     0,     0,     0,     0,     0,     0,     0,    15,
     346,     0,    17,   343,     0,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,    37,   355,   356,   357,
     358,   359,    41,   360,   361,   362,   363,   364,   365,   366,
      46,   341,   367,   368,   369,     0,     0,    51,   342,     0,
       0,  1041,     0,     0,   344,     0,     0,     0,   345,     0,
     343,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,     0,     0,    74,     0,
       0,   341,     0,     0,     0,    80,     0,     0,   342,   346,
       0,    84,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,   344,   352,   353,   354,   345,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
       0,   367,   368,   369,   341,     0,     0,     0,     0,     0,
    1116,   342,     0,     0,     0,     0,   346,     0,     0,     0,
       0,   344,     0,   343,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,     0,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,     0,     0,     0,     0,   663,     0,     0,     0,     0,
       0,     0,   347,   348,   344,   349,   350,   351,   345,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   341,     0,   367,   368,
     369,     0,     0,   342,     0,   778,     0,     0,     0,   346,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
       0,   367,   368,   369,     0,     0,     0,     0,   779,     0,
       0,     0,     0,     0,     0,     0,   344,     0,     0,     0,
     345,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,     0,   344,   367,   368,   369,   345,     0,     0,     0,
    1005,     0,   341,     0,     0,   439,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,   880,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,   887,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1020,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1021,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1022,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1023,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,   341,     0,     0,  1045,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
       0,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,   344,   398,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,     0,  1046,   341,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,   432,   346,   370,   371,
     372,   373,   374,   375,     0,     0,   378,   379,   380,   381,
     433,   383,   384,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,     0,   398,     0,     0,   344,     0,     0,     0,
     345,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,   426,     0,     0,     0,     0,   429,     0,     0,
       0,   346,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,     0,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,     0,   344,   367,   368,   369,   345,   398,   341,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   747,     0,
       0,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   348,     0,   349,   350,   351,     0,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,   344,   367,
     368,   369,   345,   398,   341,     0,     0,     0,     0,     0,
       0,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   253,     0,     0,     0,     0,   429,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     348,     0,   349,   350,   351,     0,   352,   353,   354,     0,
     355,   356,   357,   358,   359,     0,   360,   361,   362,   363,
     364,   365,   366,     0,   344,   367,   368,   369,   345,   398,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
     739,     0,     0,     0,     0,     0,     0,     0,     0,   346,
       0,     0,   341,     0,     0,     0,     0,     0,     0,   342,
       0,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,   343,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     344,   367,   368,   369,   345,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   344,     0,   343,   346,   345,     0,     0,     0,
       0,     0,     0,     0,     0,   756,     0,     0,     0,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,   346,   360,   361,
     362,   363,   364,   365,   366,     0,     0,   367,   368,   369,
       0,   398,     0,   347,   348,   344,   349,   350,   351,   345,
     352,   353,   354,     0,   355,   356,   357,   358,   359,     0,
     360,   361,   362,   363,   364,   365,   366,     0,     0,   367,
     368,   369,     0,   398,   341,     0,     0,     0,     0,     0,
     346,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,     0,     0,   347,   348,   436,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
       0,     0,   367,   368,   369,     0,   471,   341,   684,   437,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,     0,   343,     0,   345,     0,
       0,   685,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,   442,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,   346,
       0,     0,   686,     0,     0,     0,   343,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   348,   344,   349,   350,
     351,   345,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     438,   367,   368,   369,   341,     0,     0,     0,     0,     0,
       0,   342,   346,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,   343,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,   400,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,   344,   352,   353,   354,   345,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,     0,   443,   367,   368,   369,   341,     0,     0,
       0,     0,     0,     0,   342,   166,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,   343,   571,     0,     0,
       0,     0,     0,     0,     0,   347,   348,     0,   349,   350,
     351,   572,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,     0,
     341,   367,   368,   369,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,   343,
     573,   345,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   574,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   341,     0,     0,     0,
       0,     0,   346,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,   347,   348,
     344,   349,   350,   351,   345,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,     0,     0,   367,   368,   369,     0,     0,   341,
       0,     0,     0,     0,     0,   346,   342,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   344,     0,   343,     0,
     345,   347,   348,   928,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,   341,     0,   367,   368,   369,
       0,   346,   342,     0,   929,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,   347,   348,   344,
     349,   350,   351,   345,   352,   353,   354,     0,   355,   356,
     357,   358,   359,     0,   360,   361,   362,   363,   364,   365,
     366,   341,   475,   367,   368,   369,     0,     0,   342,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,   344,     0,     0,     0,   345,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,   341,     0,   367,   368,   369,     0,
     346,   342,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   344,     0,   343,     0,   345,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,     0,   360,   361,   362,   363,   364,   365,   366,
     341,   477,   367,   368,   369,     0,   346,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,     0,   347,   348,   344,   349,   350,   351,   345,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,     0,   479,   367,   368,
     369,     0,     0,     0,     0,     0,   343,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     344,     0,     0,     0,   345,   347,   348,     0,   349,   350,
     351,     0,   352,   353,   354,     0,   355,   356,   357,   358,
     359,     0,   360,   361,   362,   363,   364,   365,   366,   341,
     484,   367,   368,   369,   501,   346,   342,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   343,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,   341,     0,   367,   368,   369,
       0,     0,   342,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,   343,   352,   353,   354,     0,   344,
     356,   357,   358,   345,     0,   360,   361,   362,   363,     0,
       0,   366,   575,     0,   367,   368,   369,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,   342,     0,     0,
       0,     0,     0,     0,   346,     0,     0,     0,     0,   343,
       0,     0,     0,     0,     0,   344,     0,     0,     0,   345,
     347,   348,     0,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,     0,     0,   367,   368,   369,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     344,     0,     0,     0,   345,     0,   347,   348,     0,   349,
     350,   351,     0,   352,   353,   354,     0,   355,   356,   357,
     358,   359,  -509,   360,   361,   362,   363,   364,   365,   366,
     341,   166,   367,   368,   369,   346,     0,   342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   343,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,   341,   751,   367,   368,   369,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,     0,     0,     0,     0,     0,
     344,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   343,   346,     0,   754,     0,     0,
       0,     0,     0,     0,     0,   344,     0,     0,     0,   345,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,   359,     0,   360,   361,
     362,   363,   364,   365,   366,     0,   724,   367,   368,   369,
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
       0,     0,     0,     0,     0,     0,   343,   867,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,   342,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   870,     0,     0,
       0,   797,   346,     0,     0,     0,     0,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,   344,     0,     0,
       0,   345,     0,     0,     0,     0,     0,     0,   347,   348,
       0,   349,   350,   351,     0,   352,   353,   354,     0,   355,
     356,   357,   358,   359,     0,   360,   361,   362,   363,   364,
     365,   366,   346,     0,   367,   368,   369,     0,     0,   341,
     927,     0,     0,     0,     0,     0,   342,     0,   347,   348,
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
     347,   348,   950,   349,   350,   351,     0,   352,   353,   354,
       0,   355,   356,   357,   358,   359,     0,   360,   361,   362,
     363,   364,   365,   366,   346,     0,   367,   368,   369,   344,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     347,   348,   953,   349,   350,   351,     0,   352,   353,   354,
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
       0,   341,     0,     0,  1058,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,     0,     0,   346,     0,     0,     0,
       0,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,  1059,   349,   350,   351,     0,   352,
     353,   354,     0,   355,   356,   357,   358,   359,     0,   360,
     361,   362,   363,   364,   365,   366,   346,     0,   367,   368,
     369,   344,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   347,   348,  1060,   349,   350,   351,     0,   352,
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
       0,     0,     0,   341,     0,     0,  1061,     0,     0,     0,
     342,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   343,     0,     0,     0,     0,     0,   346,     0,
       0,     0,     0,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,   347,   348,  1062,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   346,     0,
     367,   368,   369,   344,     0,     0,     0,   345,     0,     0,
       0,     0,     0,     0,   347,   348,  1063,   349,   350,   351,
       0,   352,   353,   354,     0,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,   346,     0,
     367,   368,   369,     0,     0,   341,     0,     0,     0,     0,
       0,     0,   342,     0,   347,   348,     0,   349,   350,   351,
       0,   352,   353,   354,   343,   355,   356,   357,   358,   359,
       0,   360,   361,   362,   363,   364,   365,   366,     0,     0,
     367,   368,   369,     0,     0,   341,     0,     0,     0,  1087,
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
     346,     0,   367,   368,   369,   712,     0,     0,     0,   345,
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
      30,   270,     0,     0,     0,    35,     0,     0,     0,   343,
       0,    40,     0,     0,     0,     0,     0,     0,     0,    53,
       0,    55,     0,    57,     0,   933,     0,     0,   934,     0,
     271,     0,    63,    53,     0,    55,     0,    57,     0,   933,
       0,     0,   934,     0,   271,     0,    63,     0,     0,   343,
       0,    79,     0,     0,    81,     0,     0,    83,     0,     0,
     344,     0,     0,     0,   345,    79,     0,     0,    81,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   343,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
     344,     0,     0,     0,   345,     0,     0,    98,     0,     0,
       0,   347,   348,  1064,   349,   350,   351,     0,   352,   353,
     354,    98,   355,   356,   357,   358,     0,  1065,   360,   361,
     362,   363,   364,   365,   366,   346,   344,   367,   368,   369,
     345,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   348,     0,   349,   350,   351,     0,   352,   353,
     354,     0,   355,   356,   357,   358,     0,     0,   360,   361,
     362,   363,   364,     0,   366,     0,     0,   367,   368,   369,
       0,     0,     0,     0,     0,     0,     0,   347,   348,     0,
     349,   350,   351,   344,   352,   353,   354,   345,   355,   356,
     357,   358,     0,     0,   360,   361,   362,   363,   364,     0,
     366,     0,     0,   367,   368,   369,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   689,     0,   222,   347,   348,     0,   349,   350,   351,
       0,   352,   353,     0,     0,     0,   356,   223,   224,     0,
     225,     0,     0,   362,   363,   226,     0,   366,     0,     0,
     367,   368,   369,   227,     0,     0,     0,     0,     0,   228,
    -220,     0,     0,     0,   229,     0,     0,   230,     0,     0,
     231,     0,     0,     0,  -220,  -220,     0,  -220,   232,     0,
       0,     0,  -220,     0,   233,   234,     0,     0,     0,     0,
    -220,   235,     0,     0,     0,     0,  -220,     0,     0,     0,
     236,  -220,     0,     0,  -220,     0,     0,  -220,     0,   237,
     238,     0,   239,     0,   240,  -220,   241,     0,  -230,   242,
       0,  -220,  -220,   243,     0,     0,   244,     0,  -220,   245,
       0,     0,  -230,  -230,     0,  -230,     0,  -220,     0,     0,
    -230,     0,     0,     0,     0,     0,  -220,  -220,  -230,  -220,
       0,  -220,     0,  -220,  -230,   222,  -220,     0,     0,  -230,
    -220,     0,  -230,  -220,     0,  -230,  -220,     0,     0,   223,
     224,     0,   225,  -230,     0,     0,     0,   226,     0,  -230,
    -230,     0,     0,     0,     0,   227,  -230,     0,     0,     0,
       0,   228,     0,     0,     0,  -230,   229,     0,     0,   230,
       0,     0,   231,     0,  -230,  -230,     0,  -230,     0,  -230,
     232,  -230,     0,  -221,  -230,     0,   233,   234,  -230,     0,
       0,  -230,     0,   235,  -230,     0,     0,  -221,  -221,     0,
    -221,     0,   236,     0,     0,  -221,     0,     0,     0,     0,
       0,   237,   238,  -221,   239,     0,   240,     0,   241,  -221,
    -152,   242,     0,     0,  -221,   243,     0,  -221,   244,     0,
    -221,   245,     0,     0,  -152,  -152,     0,  -152,  -221,     0,
       0,     0,  -152,     0,  -221,  -221,     0,     0,     0,     0,
    -152,  -221,     0,     0,     0,     0,  -152,     0,     0,     0,
    -221,  -152,     0,     0,  -152,     0,     0,  -152,     0,  -221,
    -221,     0,  -221,     0,  -221,  -152,  -221,     0,  -148,  -221,
       0,  -152,  -152,  -221,     0,     0,  -221,     0,  -152,  -221,
       0,     0,  -148,  -148,     0,  -148,     0,  -152,     0,     0,
    -148,     0,     0,     0,     0,     0,  -152,  -152,  -148,  -152,
       0,  -152,     0,  -152,  -148,     0,  -152,     0,     0,  -148,
    -152,     0,  -148,  -152,     0,  -148,  -152,     0,     0,     0,
       0,     0,   268,  -148,     0,     0,    22,    23,     0,  -148,
    -148,     0,     0,     0,     0,   269,  -148,    30,   270,     0,
       0,     0,    35,     0,     0,  -148,     0,     0,    40,     0,
       0,  -239,     0,     0,  -148,  -148,     0,  -148,     0,  -148,
       0,  -148,     0,     0,  -148,     0,     0,     0,  -148,     0,
      53,  -148,    55,     0,  -148,     0,     0,  -239,     0,     0,
       0,   271,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,    81,     0,   -63,    83
};

static const yytype_int16 yycheck[] =
{
      12,   151,    65,    60,    16,   330,     2,   480,    20,   449,
      19,    64,   446,   192,    26,   137,   215,   601,    30,    31,
     773,    33,    34,    35,    36,    42,    38,   698,   686,   208,
     221,   685,     1,    45,    22,    47,     1,   113,   101,   200,
      52,   391,    54,    46,    56,   589,   454,   803,   592,   102,
     635,    46,    64,    65,    66,    67,    68,    69,    32,    46,
     221,    73,    79,    75,   983,    77,    78,     3,   857,   858,
      82,   193,   480,    85,    86,   266,    88,    32,    90,   902,
      32,    93,    94,    32,    96,     1,    32,    68,   100,   101,
     102,    85,   168,  1077,     1,    27,   111,  1027,   289,    68,
     113,    46,     1,   899,   139,   266,    56,    46,    46,   862,
     113,   133,   437,   130,    46,   132,  1094,    49,     0,   141,
       3,   158,   321,   322,   139,   709,   711,   180,   289,   110,
      46,    46,  1116,   168,  1112,   147,   139,    69,    83,   113,
      72,   110,    74,    46,   139,    83,   296,  1077,     3,    56,
     903,    83,   139,   118,   165,   168,   167,    56,   113,   171,
      96,   111,   174,   175,   176,   168,   178,   179,   180,   163,
     102,    85,    83,   138,   139,   192,   972,  1096,    85,  1002,
     936,   168,   191,   164,   728,   770,    85,   731,   200,   163,
     139,   208,   353,   210,   634,   164,   387,   388,   139,    98,
     139,   166,   118,   959,   111,   961,   139,   139,   163,   221,
     267,   163,   111,    96,   163,   113,   204,   163,   971,  1008,
    1009,   209,   138,   139,   139,   166,   387,   388,   216,   168,
      85,   138,   249,   166,   166,     3,   139,    60,    18,   138,
      27,    96,   158,   139,   158,   165,   165,   167,   167,   163,
      51,   601,   139,   168,   266,    78,   163,   274,   270,   158,
    1016,    56,  1018,   139,   163,   168,   278,   279,   139,   139,
     166,   929,   118,  1026,   928,   946,    77,   289,   876,   166,
     878,   442,   473,   139,   107,    72,   118,   158,   164,     8,
      85,    71,   165,   139,   164,   417,    83,   143,    33,    34,
     422,   885,   204,    98,   316,   165,   139,   209,   139,    89,
     166,   143,   473,   376,   377,   102,   111,   313,   330,   382,
     166,   108,   385,   386,   764,   158,   106,   339,   762,   341,
     342,   343,   344,   345,   346,   166,   348,   349,   350,   351,
     352,    27,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,    56,   709,
     958,   686,   960,   158,   376,   377,   139,   102,   163,   803,
     382,   424,   139,   385,   386,   387,   388,   389,   390,   391,
      46,   139,   139,   158,   139,   158,    72,    85,   400,   164,
      56,   139,   139,   405,    27,   407,    31,    83,   415,   166,
      98,    82,   419,   158,   165,    86,   605,   606,   166,   166,
     158,   158,   424,   111,   426,   427,   139,    77,    78,    85,
     432,   433,   108,   139,   436,   437,   587,   439,   491,   441,
     493,    45,   449,   165,   158,   158,   909,   101,   133,    72,
      93,   165,   158,   167,   456,   111,   141,    82,   460,   461,
      83,    86,   118,    67,   466,   165,   158,   167,   511,   133,
     158,   473,   164,    77,    78,   163,   165,   141,   167,   102,
       2,   670,   671,   139,    82,    27,   488,    78,    86,   491,
      12,   493,   661,   163,   107,   497,   498,   499,    56,   501,
     502,   508,   158,    25,   902,    27,   513,   163,   133,   511,
     166,   909,    27,   138,   133,   140,   141,   941,   118,   138,
     145,   507,   141,   491,   158,   493,   145,    85,   153,   157,
      72,    46,   585,     3,    49,   959,   161,   961,   158,   139,
      98,    83,   158,   143,   149,   885,   200,   158,   158,   158,
      20,    73,   158,   111,    69,   163,   158,    72,   698,    74,
     102,   158,   165,    33,   112,    81,   166,   221,    83,   571,
     572,   573,   574,   575,   576,   577,    81,   165,   100,   578,
     579,   588,   104,   585,   591,   587,   190,   102,    58,   165,
     165,   644,  1016,   595,  1018,   597,   165,   640,   158,   601,
     158,   603,   165,   158,  1002,   163,   118,     8,   212,   213,
     214,   215,   266,    60,   929,   219,   163,   138,    31,   344,
     345,   141,   158,   630,   118,   166,    96,   634,   139,  1027,
     158,    46,   102,   165,    34,   289,   638,   639,   640,   166,
     139,    67,   644,  1041,   118,   167,    21,   166,    27,   166,
     657,   139,   256,   166,   661,   165,   163,   158,   262,   168,
     168,   158,   158,   158,   389,   390,   158,    46,   166,    82,
      49,   158,   139,    86,   158,   400,   158,   168,    34,  1077,
     682,   285,   684,   685,   686,   158,   166,   164,   166,   139,
      69,   698,   164,    72,   118,    74,  1094,    83,   166,   303,
      32,    32,   427,   168,    83,   163,   163,   709,   433,    46,
     712,   713,   714,    34,  1112,   717,   138,    83,  1116,    56,
     133,   165,   168,   102,   246,   138,    27,   165,   141,   910,
     911,   253,   145,   387,   388,   737,   138,   739,   168,   782,
       2,   168,   744,   164,   158,   747,   158,    48,    85,   751,
      12,   141,   754,   141,   761,   165,   141,   764,   141,   910,
     911,   133,   133,    25,   141,    27,   133,   141,   141,    70,
     163,    72,   497,   141,   111,   133,   141,   141,   141,   786,
     782,   118,    83,   133,   141,   163,   212,   213,   214,   215,
     792,   313,   794,    34,    98,   797,   166,   401,   158,   806,
     133,   139,   139,   805,  1032,   327,   946,   108,   133,   168,
     158,    73,   466,   166,   166,    77,    78,   166,   157,   473,
      24,   158,   157,   327,   786,   832,   163,   431,   297,   166,
     256,   435,   806,   946,   488,   941,   262,   491,   100,   493,
     627,   762,   104,   809,   498,  1096,  1026,   572,  1054,   574,
     852,  1087,    35,  1081,  1082,  1083,  1084,  1085,  1086,   285,
    1080,   457,   118,   587,   866,   867,    30,   869,   870,   871,
     872,    -1,    -1,    -1,    -1,    -1,   398,   303,   880,   881,
      -1,   883,    -1,   885,    -1,   887,   888,   934,   144,   891,
     146,   147,    -1,    -1,   150,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   426,   167,   908,   909,   910,   911,
     432,    -1,    -1,    -1,   916,    -1,   918,   439,    46,    -1,
     922,    -1,    -1,    -1,    -1,   927,   928,   929,    56,    -1,
     973,   585,    -1,   587,    -1,    -1,    -1,    -1,    -1,   946,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   950,   951,
     952,   953,   954,   955,   956,   957,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   973,    -1,   495,    -1,   401,   978,   499,    -1,    -1,
     982,    -1,    -1,   111,   246,   507,    -1,   712,   713,    -1,
     118,   253,    -1,    -1,    -1,   599,   600,    -1,    -1,    -1,
       2,   605,   606,    -1,    -1,   431,  1049,    -1,    -1,   435,
      12,   139,    -1,   617,    -1,    -1,    -1,    -1,  1020,  1021,
    1022,  1023,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,    -1,   163,    -1,    -1,   166,  1041,
      -1,    -1,    -1,  1045,  1046,    -1,   568,  1049,    -1,    -1,
      -1,   313,    -1,   657,    -1,    -1,  1058,  1059,  1060,  1061,
    1062,  1063,    -1,    -1,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,  1076,  1077,    -1,  1079,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,   616,    -1,    -1,    -1,   100,    -1,
      12,    -1,   104,    -1,    -1,    -1,    -1,     3,    -1,    -1,
    1112,    -1,    -1,    25,  1116,    27,    -1,    -1,   722,   723,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
     734,    -1,   736,    -1,    -1,    -1,   398,    33,    -1,   743,
      -1,    -1,   746,    39,    -1,   749,    -1,   669,    44,    -1,
      -1,    47,    -1,   675,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    58,    -1,   426,   167,    -1,    -1,    64,    65,
     432,    -1,    -1,   599,   600,    71,   698,   439,    -1,   605,
     606,    -1,    -1,    -1,    80,   789,    -1,    -1,   100,    -1,
     712,   713,   104,    89,    90,    -1,    92,    -1,    94,    -1,
      96,    -1,    -1,    99,     2,    -1,    -1,   103,   104,    -1,
     106,    -1,    -1,   109,    12,   737,    -1,   739,    -1,    -1,
      -1,    -1,   744,    -1,    -1,   747,    -1,    25,    -1,    27,
      -1,   753,    -1,   495,   756,    -1,    -1,   499,    -1,    -1,
      -1,    -1,    -1,    -1,   246,   507,    -1,    -1,   902,    -1,
      -1,   253,    -1,    -1,    -1,   167,   910,   911,    -1,   781,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
     792,    -1,   794,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   722,   723,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,   734,    -1,
     914,   313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     746,    -1,    22,   749,   978,   327,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   246,    35,    -1,    37,    38,    -1,
      -1,   253,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,   616,    -1,    -1,    -1,   880,   963,
     964,    -1,    -1,   967,   968,   887,    -1,    -1,    -1,   167,
      70,    -1,    72,  1027,    -1,    -1,   898,    -1,    -1,   983,
      -1,    81,    -1,    83,    -1,    -1,    -1,   991,    -1,   993,
     912,   995,    -1,    -1,    -1,    -1,   398,    -1,    -1,   921,
      -1,   313,   102,   925,    -1,   105,    -1,   669,   108,    -1,
      -1,    -1,    -1,   675,    -1,   327,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1077,   426,    -1,    -1,    -1,    -1,    -1,
     432,    -1,    -1,    12,    -1,    -1,    -1,   439,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   969,   246,    -1,
      -1,     2,    31,    -1,    -1,   253,    -1,    -1,    -1,    -1,
      -1,    12,  1116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   737,    27,   739,    -1,    -1,
      -1,    -1,   744,    -1,    -1,   747,   398,    -1,   914,    -1,
      -1,   753,  1096,   495,   756,    -1,    -1,   499,  1020,  1021,
    1022,  1023,    -1,    82,    -1,   507,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,   426,   313,    -1,    -1,    -1,   781,
     432,    -1,    73,  1045,  1046,    -1,    -1,   439,    -1,   327,
     792,    -1,   794,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,   104,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,   155,   156,    -1,    -1,
     159,   160,   161,   495,    -1,    -1,    -1,   499,    -1,   168,
      -1,    -1,    -1,    -1,    -1,   507,    -1,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   616,    -1,   167,    -1,   880,    -1,
      -1,    -1,    -1,    -1,    -1,   887,    -1,    -1,   426,    -1,
      -1,    -1,    -1,    -1,   432,    -1,   898,    -1,    -1,    -1,
      -1,   439,    -1,    -1,    -1,    -1,     2,    -1,    -1,     1,
     912,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,   921,
      -1,    -1,    -1,   925,    -1,    -1,    -1,   669,    -1,    25,
      22,    27,    -1,   675,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,   246,    48,   495,    -1,    51,
      -1,   499,   253,    -1,   616,    -1,    -1,   969,    -1,   507,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    70,    -1,
      72,    -1,    74,    -1,    76,    77,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    -1,   737,    -1,   739,    -1,    -1,
      -1,    -1,   744,    -1,   100,   747,    -1,    -1,   104,    -1,
     102,   753,    -1,   105,   756,    -1,   108,   669,  1020,  1021,
    1022,  1023,   313,   675,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,   327,    -1,    35,   781,
      37,    38,    -1,  1045,  1046,    42,    -1,    -1,    -1,    -1,
     792,    48,   794,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
      -1,   167,   164,    70,    -1,    72,    -1,    74,   616,    76,
      -1,    -1,    79,    -1,    81,   737,    83,   739,    -1,    -1,
      -1,    -1,   744,     2,    -1,   747,    -1,    -1,    -1,    -1,
      -1,   753,    -1,    12,   756,   102,    -1,   398,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   781,
      -1,   669,    -1,    -1,    -1,   426,    -1,   675,   880,    -1,
     792,   432,   794,    -1,    -1,   887,    -1,    -1,   439,    -1,
     246,    -1,    -1,    -1,    -1,    -1,   898,   253,    -1,    -1,
      -1,   158,    -1,    -1,    73,    -1,    -1,   164,    -1,    -1,
     912,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,   921,
      -1,    -1,    -1,   925,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,    -1,   737,
      -1,   739,    -1,    -1,   495,    -1,   744,    -1,   499,   747,
      -1,    -1,    -1,    -1,    -1,   753,   507,   313,   756,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,   969,   880,   137,
     138,   327,   140,   141,    -1,   887,    -1,   145,    -1,    -1,
      -1,    -1,    -1,   781,    -1,   153,   898,    -1,    -1,    -1,
      -1,   159,   160,   161,   792,    -1,   794,    -1,   167,    -1,
     912,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   921,
      22,    -1,    -1,   925,    26,    27,    -1,    -1,  1020,  1021,
    1022,  1023,    -1,    35,     2,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    12,    -1,    48,    -1,    -1,    -1,
      -1,    -1,   398,  1045,  1046,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   969,    70,    -1,
      72,    -1,    74,    -1,    76,   616,    -1,    79,    -1,    81,
     426,    83,    -1,    -1,    -1,    -1,   432,   246,    -1,    -1,
      -1,    -1,   880,   439,   253,    -1,    -1,    -1,    -1,   887,
     102,    -1,    -1,   105,    -1,    73,   108,    -1,    -1,    -1,
     898,    -1,    -1,    -1,    31,    -1,    -1,    -1,  1020,  1021,
    1022,  1023,    -1,    -1,   912,    22,    -1,    -1,   669,    26,
      27,    -1,   100,   921,   675,    -1,   104,   925,    35,    -1,
      37,    38,    -1,  1045,  1046,    42,    -1,    -1,    -1,   495,
      -1,    48,    -1,   499,   313,    -1,   158,    -1,    -1,    -1,
      -1,   507,   164,    -1,    -1,    82,    -1,    -1,   327,    86,
      -1,    12,    -1,    70,    -1,    72,    -1,    74,    19,    76,
      -1,   969,    79,    -1,    81,    -1,    83,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   737,    -1,   739,   167,
      -1,    -1,    -1,   744,    -1,   102,   747,    -1,   105,    -1,
      -1,   108,   753,    -1,    -1,   756,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,    -1,    -1,    -1,   145,    -1,
      -1,    -1,  1020,  1021,  1022,  1023,   153,    -1,    -1,   398,
     781,    82,   159,   160,   161,    86,    -1,    -1,    -1,    -1,
      -1,   792,    -1,   794,    -1,    -1,    -1,  1045,  1046,    -1,
      -1,   158,    -1,    -1,    -1,    -1,    -1,   426,    -1,    -1,
     616,    -1,    -1,   432,    -1,    -1,   117,    -1,   246,    -1,
     439,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,   669,    -1,    -1,    -1,   168,    -1,   675,
      -1,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,   880,
     499,    -1,    -1,    -1,    -1,   313,   887,    -1,   507,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   898,    -1,   327,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,   912,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,
     921,    42,    -1,    -1,   925,    31,    -1,    48,    -1,    -1,
      -1,   737,    -1,   739,    -1,    -1,    -1,    -1,   744,    -1,
      -1,   747,    -1,    -1,    -1,    -1,    -1,   753,    -1,    70,
     756,    72,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      81,    -1,    83,    -1,    -1,    -1,    -1,    -1,   969,    -1,
     398,    -1,    -1,    -1,    -1,   781,    82,    -1,    -1,    -1,
      86,   102,    -1,    -1,   105,    -1,   792,   108,   794,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   616,   426,    -1,
      -1,    -1,    -1,    -1,   432,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1020,
    1021,  1022,  1023,    -1,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,   138,    -1,   140,   141,    -1,   158,    -1,   145,
      -1,    -1,    -1,   164,  1045,  1046,    -1,   153,    -1,    -1,
     669,    -1,    -1,   159,   160,   161,   675,    -1,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,   495,    -1,    -1,
      -1,   499,    -1,    -1,   880,    17,    18,    -1,    20,   507,
      -1,   887,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,   898,    -1,    -1,    -1,    -1,    39,    31,    -1,
      -1,    -1,    44,    -1,    -1,    47,   912,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,   921,    58,    -1,   737,   925,
     739,    -1,    64,    65,    -1,   744,    -1,    -1,   747,    71,
      -1,    -1,    -1,    -1,   753,    -1,    -1,   756,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    82,
      92,    -1,    94,    86,    96,    -1,    -1,    99,    -1,    -1,
      -1,   103,   781,   969,   106,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,   792,    -1,   794,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   616,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    17,    18,   138,    20,   140,   141,    -1,
      -1,    25,   145,    -1,  1020,  1021,  1022,  1023,    -1,    33,
     153,   163,    -1,   165,    -1,    39,   159,   160,   161,    -1,
      44,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,  1045,
    1046,   669,    -1,    -1,    58,    -1,    -1,   675,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,   880,    -1,    -1,    -1,    -1,    80,    -1,   887,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    92,   898,
      94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,
      -1,    -1,   106,   912,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   921,    -1,    -1,    -1,   925,    -1,    -1,   737,
      -1,   739,    -1,    -1,    -1,    -1,   744,    -1,    -1,   747,
      -1,    -1,    -1,    -1,    -1,   753,    -1,    -1,   756,    -1,
       1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
     969,   165,    -1,   781,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,   792,    -1,   794,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    -1,
      71,  1020,  1021,  1022,  1023,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      -1,    92,    -1,    94,    -1,    96,  1045,  1046,    99,    -1,
      -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,
     121,   122,   123,    -1,    -1,   126,   127,   128,   129,    -1,
     131,   132,   133,   134,   135,   136,   137,    -1,    -1,   140,
     898,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   912,   156,    -1,    -1,   159,   160,
     161,   162,    -1,   921,   165,     0,     1,   925,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,   969,    47,    48,    -1,    50,    51,    -1,    53,    54,
      55,    -1,    57,    58,    59,    60,    61,    62,    -1,    64,
      65,    66,    -1,    -1,    -1,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    -1,    83,    84,
      85,    -1,    87,    88,    89,    90,    91,    92,    93,    94,
      -1,    96,    97,    -1,    99,   100,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,    -1,   112,    -1,   114,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
     135,   136,   137,    -1,    -1,    -1,    -1,   142,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
     155,   156,   157,   158,    -1,    -1,   161,    -1,   163,    -1,
     165,     1,   167,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,   134,   135,   136,   137,    -1,    -1,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,   155,   156,   157,    -1,    -1,    -1,   161,    -1,
     163,    -1,   165,     3,   167,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,    59,
      -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    96,    -1,    -1,    99,
      -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,   109,
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
      -1,    -1,    -1,    -1,    71,    72,    73,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    96,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,   115,    -1,
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
      -1,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,
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
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
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
     164,   165,     3,   167,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,    -1,    -1,    32,    33,    -1,    35,    -1,    -1,
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
      -1,    -1,   102,   103,    -1,    -1,   106,    -1,    -1,   109,
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
     157,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,     3,
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
      -1,    -1,   106,    -1,    -1,   109,    -1,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,   162,   156,   157,   165,    -1,   167,   161,    -1,   163,
      -1,   165,    -1,   167,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      31,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    -1,    64,    65,    -1,    -1,    -1,
      -1,    82,    71,    -1,    73,    86,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    96,    -1,    -1,
      99,    -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,    -1,   145,    -1,   135,    -1,    -1,    -1,
     139,   152,   153,    -1,   143,   156,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,   166,   167,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
       3,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    12,
      71,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    80,
      -1,    -1,    25,    -1,   165,    -1,   167,    -1,    89,    90,
      33,    92,    -1,    94,    -1,    96,    39,    -1,    99,    -1,
      -1,    44,   103,    -1,    47,   106,    -1,    50,   109,    52,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    12,    -1,    -1,    71,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    89,    90,    -1,    92,
      -1,    94,    -1,    96,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,   106,   165,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,
     123,    -1,    -1,   126,   127,   128,   129,    -1,   131,   132,
     133,   134,   135,   136,   137,    82,    -1,   140,    -1,    86,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,    -1,   156,    12,     3,   159,   160,   161,   162,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
     117,    -1,    20,    31,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    44,   144,   145,   146,
     147,   148,    50,   150,   151,   152,   153,   154,   155,   156,
      58,    12,   159,   160,   161,    -1,    -1,    65,    19,    -1,
      -1,   168,    -1,    -1,    82,    -1,    -1,    -1,    86,    -1,
      31,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    96,    -1,
      -1,    12,    -1,    -1,    -1,   103,    -1,    -1,    19,   117,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    82,   140,   141,   142,    86,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    12,    -1,    -1,    -1,    -1,    -1,
     168,    19,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    82,    -1,    31,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   117,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    82,   136,   137,   138,    86,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    12,    -1,   159,   160,
     161,    -1,    -1,    19,    -1,   166,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,    -1,    -1,    -1,
     166,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    95,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      -1,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    82,   163,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,   129,
      46,   131,   132,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    -1,   163,    -1,    -1,    82,    -1,    -1,    -1,
      86,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    -1,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    -1,    82,   159,   160,   161,    86,   163,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,   134,    -1,   136,   137,   138,    -1,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    82,   159,
     160,   161,    86,   163,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,   136,   137,   138,    -1,   140,   141,   142,    -1,
     144,   145,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,   155,   156,    -1,    82,   159,   160,   161,    86,   163,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    31,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      82,   159,   160,   161,    86,   163,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    31,   117,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,   117,   150,   151,
     152,   153,   154,   155,   156,    -1,    -1,   159,   160,   161,
      -1,   163,    -1,   133,   134,    82,   136,   137,   138,    86,
     140,   141,   142,    -1,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   155,   156,    -1,    -1,   159,
     160,   161,    -1,   163,    12,    -1,    -1,    -1,    -1,    -1,
     117,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,   133,   134,    36,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      -1,    -1,   159,   160,   161,    -1,   163,    12,    13,    67,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    31,    -1,    86,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    67,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    82,   136,   137,
     138,    86,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,   117,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      -1,    86,    -1,    31,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    -1,   140,   141,   142,    46,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,   117,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    82,   140,   141,   142,    86,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   133,   134,    -1,   136,   137,
     138,    46,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    -1,
      12,   159,   160,   161,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    31,
      32,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   117,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   133,   134,
      82,   136,   137,   138,    86,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
     155,   156,    -1,    -1,   159,   160,   161,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   117,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    31,    -1,
      86,   133,   134,    36,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    12,    -1,   159,   160,   161,
      -1,   117,    19,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,   133,   134,    82,
     136,   137,   138,    86,   140,   141,   142,    -1,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,    12,   158,   159,   160,   161,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    12,    -1,   159,   160,   161,    -1,
     117,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    31,    -1,    86,   133,   134,    -1,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,   155,   156,
      12,   158,   159,   160,   161,    -1,   117,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   133,   134,    82,   136,   137,   138,    86,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    86,   133,   134,    -1,   136,   137,
     138,    -1,   140,   141,   142,    -1,   144,   145,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,    12,
     158,   159,   160,   161,   116,   117,    19,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   155,   156,    12,    -1,   159,   160,   161,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   133,   134,
      -1,   136,   137,   138,    31,   140,   141,   142,    -1,    82,
     145,   146,   147,    86,    -1,   150,   151,   152,   153,    -1,
      -1,   156,    95,    -1,   159,   160,   161,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    86,
     133,   134,    -1,   136,   137,   138,    -1,   140,   141,   142,
      -1,   144,   145,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,    -1,    -1,   159,   160,   161,    -1,
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
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    31,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      81,    -1,    83,    70,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    31,
      -1,   102,    -1,    -1,   105,    -1,    -1,   108,    -1,    -1,
      82,    -1,    -1,    -1,    86,   102,    -1,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    -1,    86,    -1,    -1,   158,    -1,    -1,
      -1,   133,   134,   164,   136,   137,   138,    -1,   140,   141,
     142,   158,   144,   145,   146,   147,    -1,   164,   150,   151,
     152,   153,   154,   155,   156,   117,    82,   159,   160,   161,
      86,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   133,   134,    -1,   136,   137,   138,    -1,   140,   141,
     142,    -1,   144,   145,   146,   147,    -1,    -1,   150,   151,
     152,   153,   154,    -1,   156,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,    -1,
     136,   137,   138,    82,   140,   141,   142,    86,   144,   145,
     146,   147,    -1,    -1,   150,   151,   152,   153,   154,    -1,
     156,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,   133,   134,    -1,   136,   137,   138,
      -1,   140,   141,    -1,    -1,    -1,   145,    17,    18,    -1,
      20,    -1,    -1,   152,   153,    25,    -1,   156,    -1,    -1,
     159,   160,   161,    33,    -1,    -1,    -1,    -1,    -1,    39,
       3,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    17,    18,    -1,    20,    58,    -1,
      -1,    -1,    25,    -1,    64,    65,    -1,    -1,    -1,    -1,
      33,    71,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      80,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    89,
      90,    -1,    92,    -1,    94,    58,    96,    -1,     3,    99,
      -1,    64,    65,   103,    -1,    -1,   106,    -1,    71,   109,
      -1,    -1,    17,    18,    -1,    20,    -1,    80,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    89,    90,    33,    92,
      -1,    94,    -1,    96,    39,     3,    99,    -1,    -1,    44,
     103,    -1,    47,   106,    -1,    50,   109,    -1,    -1,    17,
      18,    -1,    20,    58,    -1,    -1,    -1,    25,    -1,    64,
      65,    -1,    -1,    -1,    -1,    33,    71,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    80,    44,    -1,    -1,    47,
      -1,    -1,    50,    -1,    89,    90,    -1,    92,    -1,    94,
      58,    96,    -1,     3,    99,    -1,    64,    65,   103,    -1,
      -1,   106,    -1,    71,   109,    -1,    -1,    17,    18,    -1,
      20,    -1,    80,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    89,    90,    33,    92,    -1,    94,    -1,    96,    39,
       3,    99,    -1,    -1,    44,   103,    -1,    47,   106,    -1,
      50,   109,    -1,    -1,    17,    18,    -1,    20,    58,    -1,
      -1,    -1,    25,    -1,    64,    65,    -1,    -1,    -1,    -1,
      33,    71,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      80,    44,    -1,    -1,    47,    -1,    -1,    50,    -1,    89,
      90,    -1,    92,    -1,    94,    58,    96,    -1,     3,    99,
      -1,    64,    65,   103,    -1,    -1,   106,    -1,    71,   109,
      -1,    -1,    17,    18,    -1,    20,    -1,    80,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    89,    90,    33,    92,
      -1,    94,    -1,    96,    39,    -1,    99,    -1,    -1,    44,
     103,    -1,    47,   106,    -1,    50,   109,    -1,    -1,    -1,
      -1,    -1,    22,    58,    -1,    -1,    26,    27,    -1,    64,
      65,    -1,    -1,    -1,    -1,    35,    71,    37,    38,    -1,
      -1,    -1,    42,    -1,    -1,    80,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    89,    90,    -1,    92,    -1,    94,
      -1,    96,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      70,   106,    72,    -1,   109,    -1,    -1,    77,    -1,    -1,
      -1,    81,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,   105,    -1,   107,   108
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
     303,   165,     3,    17,    18,    20,    25,    33,    39,    44,
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
     183,   242,    13,   158,   282,   178,   165,   244,   104,   165,
     195,   268,   269,     1,   138,   273,    46,   139,   178,   200,
     165,   165,   200,   282,   204,   195,   139,   158,   303,   303,
     158,   163,   200,   165,   282,   158,   217,   158,   217,   158,
     118,   263,   158,   200,   158,   164,   164,   176,   139,   164,
     303,   139,   166,   139,   166,   168,   295,    46,   139,   168,
     295,   116,   139,   168,     8,     1,   164,   184,    60,   185,
     303,   138,   226,   163,   237,   158,   303,   303,   303,   303,
     213,   303,   213,   303,   303,   303,   303,   303,   303,   303,
      20,    33,    58,   102,   196,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   305,   305,   305,   305,   305,   282,   282,   213,   303,
     213,   303,    27,    46,    83,   108,   294,   297,   298,   303,
     318,    32,    46,    32,    46,    95,    46,   168,   213,   303,
     200,   158,   158,   303,   303,   118,   166,   139,   210,   195,
     266,   208,   195,   158,   266,    46,   278,    43,   303,   213,
     303,   165,   200,    43,   303,   213,   303,   200,   161,   185,
     187,   303,   185,   186,   178,   303,    34,   303,   196,   166,
      27,    46,    49,    69,    72,    83,   102,   177,   245,   246,
     247,   248,   234,   269,   139,   166,    33,    47,    90,    94,
     167,   199,   274,   286,   118,   270,   303,   267,   303,   303,
     166,   255,   303,     1,   222,   282,   166,    21,   218,   274,
     286,   139,   164,   166,   166,   280,   166,   280,   178,   168,
     213,   303,   168,   178,   303,   168,   303,   168,   303,   164,
     164,   195,   139,   158,    13,    36,    67,   278,   163,     1,
     195,   230,   231,    27,    72,    83,   102,   239,   249,   165,
     158,   158,   158,   158,   158,   166,   168,    46,    83,   139,
     166,   285,    82,    82,    43,   213,   303,    43,   213,   303,
     213,   303,   292,   292,   158,   255,   305,   284,   195,   226,
     158,   195,   226,   158,   303,   166,   303,    32,   200,    32,
     200,   296,   297,   303,    32,   200,   295,    32,   200,   295,
     158,    13,   158,    34,    34,   178,    95,   183,   158,    46,
      83,   247,   139,   166,   165,   195,    27,    72,    83,   102,
     251,   166,   269,   273,     1,   278,    63,   305,   166,   166,
     164,    68,   110,   164,   223,   166,   165,   183,   195,   219,
     262,   178,   168,   295,   168,   295,   178,   116,   303,   303,
     187,   186,     1,   227,   164,   118,   139,   164,    83,   238,
       1,     3,    12,    17,    19,    20,    25,    39,    44,    50,
      52,    58,    64,    65,    80,    92,    96,    99,   103,   109,
     133,   134,   135,   136,   137,   140,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   156,   159,   160,
     161,   162,   165,   194,   195,   197,   240,   241,   242,   285,
     166,   297,   273,   285,   285,   303,    32,    32,   303,    32,
      32,   168,   168,   200,   200,   226,   163,   226,   163,   200,
      95,    43,   303,    43,   303,   139,   166,    95,    43,   303,
     200,    43,   303,   200,   303,   178,   303,   178,    34,   195,
     246,   269,   138,   277,    83,   273,   270,   168,    46,   168,
     165,   165,    32,   178,   278,   219,   138,   183,    43,   178,
     303,   168,    43,   178,   303,   168,   303,    13,    36,    67,
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
     198,   198,   198,   198,   199,   199,   199,   199,   200,   200,
     201,   201,   202,   202,   202,   203,   202,   204,   204,   204,
     204,   204,   204,   204,   205,   205,   205,   205,   207,   206,
     208,   206,   209,   206,   210,   206,   206,   206,   206,   206,
     211,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   213,   214,   214,
     214,   214,   214,   214,   214,   214,   215,   216,   216,   216,
     216,   216,   216,   217,   217,   218,   218,   218,   219,   219,
     220,   221,   221,   222,   222,   223,   223,   223,   224,   224,
     225,   225,   225,   226,   226,   227,   227,   227,   228,   228,
     229,   230,   230,   230,   231,   231,   233,   234,   232,   235,
     235,   235,   237,   238,   236,   239,   239,   239,   239,   239,
     240,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   243,   243,   244,   245,   245,   245,   246,   246,
     246,   246,   246,   247,   247,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   249,   249,   249,   249,   249,   249,
     250,   250,   251,   251,   251,   251,   251,   251,   252,   252,
     253,   253,   254,   254,   255,   255,   256,   256,   256,   257,
     257,   257,   257,   257,   258,   258,   259,   259,   259,   259,
     259,   259,   259,   260,   260,   261,   261,   261,   262,   262,
     263,   263,   263,   264,   264,   264,   264,   264,   265,   265,
     266,   266,   267,   267,   268,   268,   268,   269,   269,   269,
     270,   270,   270,   271,   271,   271,   271,   271,   271,   271,
     272,   272,   272,   272,   272,   273,   273,   273,   273,   273,
     274,   274,   274,   274,   275,   275,   275,   276,   276,   276,
     276,   276,   277,   277,   277,   277,   277,   278,   278,   278,
     278,   279,   279,   280,   280,   280,   281,   281,   282,   282,
     283,   283,   284,   284,   284,   284,   285,   285,   286,   286,
     286,   287,   287,   287,   287,   287,   287,   287,   287,   287,
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
       3,     5,     3,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     2,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     5,     2,     0,     8,
       0,     9,     0,     8,     0,     9,     3,     3,     5,     5,
       2,     5,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     6,     5,     8,     7,     2,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     6,     6,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     5,
       1,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     1,     0,     0,    10,     3,     3,     5,     5,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     5,
       4,     6,     6,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     2,     1,     1,     2,     2,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     4,     4,     2,     4,
       0,     2,     2,     1,     1,     1,     2,     1,     4,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     3,     2,     4,     3,     3,     4,     3,
       0,     2,     2,     2,     1,     0,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     1,     1,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     1,     6,     6,     4,     9,     9,     7,
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
#line 5971 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 502 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 503 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5983 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 510 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 515 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5995 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 516 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6001 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 538 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 543 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 544 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6085 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6097 "bison-chapel.cpp" /* yacc.c:1663  */
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
#line 6111 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6120 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 574 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6129 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6138 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6144 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6150 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6156 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6162 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 593 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6168 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6180 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 613 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6208 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 629 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6227 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 633 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 640 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 649 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6256 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
#line 6262 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6268 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6274 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6280 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 665 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6286 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6292 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6298 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6304 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-1].pexpr)); }
#line 6310 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 674 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildImportStmt((yyvsp[-3].pexpr), (yyvsp[-1].pch)); }
#line 6316 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 678 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6322 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 683 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6328 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6346 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 704 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 706 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 711 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 713 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6496 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6502 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6508 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6520 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6526 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6532 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 755 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 757 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6586 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 767 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6604 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 768 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6610 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 769 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6634 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6640 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6646 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6652 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6658 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6664 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 790 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6670 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6676 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6682 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 797 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6688 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 807 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 6694 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 808 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6700 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6706 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6712 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6718 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6724 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 830 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 835 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 6747 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 845 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 850 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 6771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 861 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 866 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 6794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 876 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6803 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 881 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 6817 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 892 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 896 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 6833 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 901 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6845 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 909 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 6857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 920 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 926 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 927 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 928 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 6883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 929 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 930 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 6895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 932 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 933 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 934 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 6919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 936 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 938 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 939 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6949 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 6967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 6973 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 945 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6983 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 951 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 6993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 957 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7003 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 963 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 969 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7024 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 976 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7035 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 983 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 987 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 993 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 998 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 999 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 1000 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1001 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1003 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7095 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1005 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7102 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1007 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1015 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1020 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1024 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1025 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1029 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1030 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1031 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1035 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1040 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1044 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1046 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1050 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1051 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1056 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1058 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1060 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1067 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7255 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1077 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1090 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7279 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1095 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1100 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7299 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1108 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1109 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7311 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1114 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7317 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1116 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7323 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1118 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7329 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1123 "chapel.ypp" /* yacc.c:1663  */
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
#line 7349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1139 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7357 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1146 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1155 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1163 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7387 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1167 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1179 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1184 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7425 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1189 "chapel.ypp" /* yacc.c:1663  */
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
#line 7447 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1211 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 7455 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1214 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 7464 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1218 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 7473 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1226 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1236 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1243 "chapel.ypp" /* yacc.c:1663  */
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
#line 7525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1269 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7533 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1273 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7542 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1278 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1282 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7559 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1287 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7567 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1294 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7573 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1298 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7579 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1299 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7585 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1300 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7591 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1301 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7597 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1302 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7603 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1303 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7609 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1304 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7615 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1305 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1307 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1309 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1310 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1311 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1313 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 7669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1314 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 7675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1315 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 7681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1316 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 7687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 7693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1318 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 7699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 7705 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1320 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 7711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 7717 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1322 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 7723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1323 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 7729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1324 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 7735 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1325 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 7741 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1329 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 7747 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1330 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 7753 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1331 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 7759 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 7765 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 7771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1334 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 7777 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1335 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 7783 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1336 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 7789 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 7795 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 7801 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1339 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 7807 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 7813 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1344 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1345 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1349 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7831 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1353 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7837 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7843 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1360 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1362 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7861 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1364 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7867 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1366 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7873 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1368 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7879 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1372 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7885 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1373 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1377 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 7897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1379 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 7909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 7915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7921 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 7939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1385 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7957 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 7963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1398 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1403 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 7999 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8005 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 8011 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 8017 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 8023 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1408 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 8029 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1412 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 8035 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1413 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 8041 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1416 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 8047 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1422 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8053 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1426 "chapel.ypp" /* yacc.c:1663  */
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[0].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
#line 8063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1431 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1435 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1436 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1437 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1445 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1447 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1449 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1451 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1453 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1458 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1460 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1464 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1466 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1468 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1469 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1470 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1475 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1477 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1482 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1484 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1486 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1491 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8222 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1504 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1520 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1522 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8258 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8264 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8270 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8276 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1531 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8282 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1532 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1537 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8298 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1543 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1552 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8316 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8322 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1562 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8328 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1571 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8346 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1576 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1578 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1580 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1586 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1594 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1602 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8410 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1612 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1621 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1623 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1628 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8468 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1629 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1631 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1653 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8500 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1657 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8508 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1661 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8520 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1669 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1675 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1676 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1682 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8552 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1684 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8558 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1690 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1694 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1708 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1710 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1711 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1720 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1722 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1727 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 8678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1736 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1737 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 8702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1763 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 8738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8768 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1784 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8774 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8786 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 8792 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 8804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1797 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1799 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 8816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1801 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 8828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1805 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 8834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 8840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1810 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 8846 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1812 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 8852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8876 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8900 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1833 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1835 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1837 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1839 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1841 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8935 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1848 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1854 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1860 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1866 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1875 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1884 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8987 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1900 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1904 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 8999 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1909 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9005 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1913 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9011 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1914 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9017 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1918 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9023 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1922 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9029 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1923 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9035 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1928 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1932 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1936 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9059 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1942 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9065 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9071 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1944 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9077 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1945 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9083 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1946 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9089 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1947 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9095 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1953 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9101 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1958 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9107 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1960 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1964 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9123 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1968 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9131 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1972 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1980 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1985 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9156 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1991 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9166 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 1997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 2004 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 2013 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new owned(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'owned.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[-1].pexpr))); }
#line 9196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 2017 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new shared(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'shared.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[-1].pexpr))); }
#line 9204 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 2024 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9210 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2040 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9216 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2042 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9222 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2044 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9228 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2046 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 9234 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2048 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 9240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2050 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 9246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2054 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 538:
#line 2055 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9258 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 539:
#line 2059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9264 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 540:
#line 2060 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9270 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 2061 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9276 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9282 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2080 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2081 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2082 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2086 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2088 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 2093 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 2094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 2095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9342 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2096 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2104 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2105 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2107 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2112 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2117 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2118 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2119 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2120 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2121 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 9414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2122 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2123 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2124 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 9432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2125 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 9438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2126 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2127 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2129 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9458 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2133 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2140 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2141 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2145 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9496 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9502 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9508 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2150 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2151 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9520 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9526 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9532 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2154 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2155 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2156 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2157 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2159 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2160 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2161 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2162 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9586 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2163 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2164 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2165 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9604 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9610 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2172 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9634 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9640 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9646 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9652 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2177 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9658 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2181 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9664 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2182 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9670 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2183 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9676 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2184 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9682 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2188 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9688 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2189 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9694 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9700 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2191 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 9706 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2196 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 9712 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 9718 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 9724 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 9730 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 9736 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 9742 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 624:
#line 2202 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 9748 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 9752 "bison-chapel.cpp" /* yacc.c:1663  */
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
