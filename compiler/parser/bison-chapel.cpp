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
    ImportStmt*               pimportstmt;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<PotentialRename*>* ponlylist;
    std::set<Flag>*           pflagset;
    WhereAndLifetime          lifetimeAndWhere;
  };

  #endif
#line 151 "chapel.ypp" /* yacc.c:355  */

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
#line 174 "chapel.ypp" /* yacc.c:355  */

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

#line 242 "bison-chapel.cpp" /* yacc.c:355  */

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
    TINCLUDE = 302,
    TINDEX = 303,
    TINLINE = 304,
    TINOUT = 305,
    TINT = 306,
    TITER = 307,
    TINITEQUALS = 308,
    TLABEL = 309,
    TLAMBDA = 310,
    TLET = 311,
    TLIFETIME = 312,
    TLOCAL = 313,
    TLOCALE = 314,
    TMINUSMINUS = 315,
    TMODULE = 316,
    TNEW = 317,
    TNIL = 318,
    TNOINIT = 319,
    TNONE = 320,
    TNOTHING = 321,
    TON = 322,
    TONLY = 323,
    TOTHERWISE = 324,
    TOUT = 325,
    TOVERRIDE = 326,
    TOWNED = 327,
    TPARAM = 328,
    TPLUSPLUS = 329,
    TPRAGMA = 330,
    TPRIMITIVE = 331,
    TPRIVATE = 332,
    TPROC = 333,
    TPROTOTYPE = 334,
    TPUBLIC = 335,
    TREAL = 336,
    TRECORD = 337,
    TREDUCE = 338,
    TREF = 339,
    TREQUIRE = 340,
    TRETURN = 341,
    TSCAN = 342,
    TSELECT = 343,
    TSERIAL = 344,
    TSHARED = 345,
    TSINGLE = 346,
    TSPARSE = 347,
    TSTRING = 348,
    TSUBDOMAIN = 349,
    TSYNC = 350,
    TTHEN = 351,
    TTHIS = 352,
    TTHROW = 353,
    TTHROWS = 354,
    TTRUE = 355,
    TTRY = 356,
    TTRYBANG = 357,
    TTYPE = 358,
    TUINT = 359,
    TUNDERSCORE = 360,
    TUNION = 361,
    TUNMANAGED = 362,
    TUSE = 363,
    TVAR = 364,
    TVOID = 365,
    TWHEN = 366,
    TWHERE = 367,
    TWHILE = 368,
    TWITH = 369,
    TYIELD = 370,
    TZIP = 371,
    TALIAS = 372,
    TAND = 373,
    TASSIGN = 374,
    TASSIGNBAND = 375,
    TASSIGNBOR = 376,
    TASSIGNBXOR = 377,
    TASSIGNDIVIDE = 378,
    TASSIGNEXP = 379,
    TASSIGNLAND = 380,
    TASSIGNLOR = 381,
    TASSIGNMINUS = 382,
    TASSIGNMOD = 383,
    TASSIGNMULTIPLY = 384,
    TASSIGNPLUS = 385,
    TASSIGNREDUCE = 386,
    TASSIGNSL = 387,
    TASSIGNSR = 388,
    TBANG = 389,
    TBAND = 390,
    TBNOT = 391,
    TBOR = 392,
    TBXOR = 393,
    TCOLON = 394,
    TCOMMA = 395,
    TDIVIDE = 396,
    TDOT = 397,
    TDOTDOT = 398,
    TDOTDOTDOT = 399,
    TEQUAL = 400,
    TEXP = 401,
    TGREATER = 402,
    TGREATEREQUAL = 403,
    THASH = 404,
    TIO = 405,
    TLESS = 406,
    TLESSEQUAL = 407,
    TMINUS = 408,
    TMOD = 409,
    TNOTEQUAL = 410,
    TOR = 411,
    TPLUS = 412,
    TQUESTION = 413,
    TSEMI = 414,
    TSHIFTLEFT = 415,
    TSHIFTRIGHT = 416,
    TSTAR = 417,
    TSWAP = 418,
    TLCBR = 419,
    TRCBR = 420,
    TLP = 421,
    TRP = 422,
    TLSBR = 423,
    TRSBR = 424,
    TNOELSE = 425,
    TDOTDOTOPENHIGH = 426,
    TUPLUS = 427,
    TUMINUS = 428,
    TLNOT = 429
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
#line 205 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 462 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 468 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 39 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
  bool        parsingPrivate=false;
#line 213 "chapel.ypp" /* yacc.c:359  */

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

#line 541 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   19130

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  151
/* YYNRULES -- Number of rules.  */
#define YYNRULES  636
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1141

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   429

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
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   506,   506,   511,   512,   518,   519,   524,   525,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   565,   578,   583,   588,   596,   597,   598,   602,   603,
     607,   608,   609,   614,   613,   634,   635,   636,   641,   642,
     647,   652,   657,   661,   668,   673,   677,   682,   686,   687,
     688,   692,   693,   694,   695,   696,   700,   705,   706,   707,
     711,   712,   716,   720,   722,   724,   726,   728,   735,   736,
     740,   741,   742,   743,   744,   745,   748,   749,   750,   751,
     752,   753,   765,   766,   777,   778,   779,   780,   781,   782,
     783,   784,   785,   786,   787,   788,   789,   790,   791,   792,
     793,   794,   795,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   817,   818,   819,   820,   824,
     825,   829,   830,   834,   835,   836,   846,   846,   851,   852,
     853,   854,   855,   856,   857,   861,   862,   863,   864,   869,
     868,   884,   883,   900,   899,   915,   914,   930,   934,   939,
     947,   958,   965,   966,   967,   968,   969,   970,   971,   972,
     973,   974,   975,   976,   977,   978,   979,   980,   981,   982,
     983,   989,   995,  1001,  1007,  1014,  1021,  1025,  1032,  1036,
    1037,  1038,  1039,  1040,  1042,  1044,  1046,  1051,  1054,  1055,
    1056,  1057,  1058,  1059,  1063,  1064,  1068,  1069,  1070,  1074,
    1075,  1079,  1082,  1084,  1089,  1090,  1094,  1096,  1098,  1105,
    1115,  1129,  1134,  1139,  1147,  1148,  1153,  1154,  1156,  1161,
    1177,  1184,  1193,  1201,  1205,  1212,  1213,  1218,  1223,  1217,
    1250,  1253,  1257,  1265,  1275,  1264,  1307,  1311,  1316,  1320,
    1325,  1332,  1333,  1337,  1338,  1339,  1340,  1341,  1342,  1343,
    1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,
    1354,  1355,  1356,  1357,  1358,  1359,  1360,  1361,  1362,  1363,
    1364,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,
    1377,  1378,  1379,  1383,  1384,  1388,  1392,  1393,  1394,  1398,
    1400,  1402,  1404,  1406,  1411,  1412,  1416,  1417,  1418,  1419,
    1420,  1421,  1422,  1423,  1424,  1428,  1429,  1430,  1431,  1432,
    1433,  1437,  1438,  1442,  1443,  1444,  1445,  1446,  1447,  1451,
    1452,  1455,  1456,  1460,  1461,  1465,  1470,  1474,  1475,  1476,
    1484,  1485,  1487,  1489,  1491,  1496,  1498,  1503,  1504,  1505,
    1506,  1507,  1508,  1509,  1513,  1515,  1520,  1522,  1524,  1529,
    1542,  1559,  1560,  1562,  1567,  1568,  1569,  1570,  1571,  1575,
    1581,  1589,  1590,  1598,  1600,  1605,  1607,  1609,  1614,  1616,
    1618,  1625,  1626,  1627,  1632,  1634,  1636,  1640,  1644,  1646,
    1650,  1658,  1659,  1660,  1661,  1662,  1667,  1668,  1669,  1670,
    1671,  1691,  1695,  1699,  1707,  1714,  1715,  1716,  1720,  1722,
    1728,  1730,  1732,  1737,  1738,  1739,  1740,  1741,  1747,  1748,
    1749,  1750,  1754,  1755,  1759,  1760,  1761,  1765,  1766,  1770,
    1771,  1775,  1776,  1780,  1781,  1782,  1783,  1787,  1788,  1799,
    1801,  1803,  1809,  1810,  1811,  1812,  1813,  1814,  1816,  1818,
    1820,  1822,  1824,  1826,  1829,  1831,  1833,  1835,  1837,  1839,
    1841,  1843,  1846,  1848,  1853,  1855,  1857,  1859,  1861,  1863,
    1865,  1867,  1869,  1871,  1873,  1875,  1877,  1884,  1890,  1896,
    1902,  1911,  1921,  1929,  1930,  1931,  1932,  1933,  1934,  1935,
    1936,  1941,  1942,  1946,  1950,  1951,  1955,  1959,  1960,  1964,
    1968,  1972,  1979,  1980,  1981,  1982,  1983,  1984,  1988,  1989,
    1994,  1996,  2000,  2004,  2008,  2016,  2021,  2027,  2033,  2040,
    2049,  2053,  2060,  2068,  2069,  2070,  2071,  2072,  2073,  2074,
    2075,  2076,  2078,  2080,  2082,  2097,  2099,  2101,  2103,  2108,
    2109,  2113,  2114,  2115,  2119,  2120,  2121,  2122,  2133,  2134,
    2135,  2136,  2140,  2141,  2142,  2146,  2147,  2148,  2149,  2150,
    2158,  2159,  2160,  2161,  2165,  2166,  2170,  2171,  2172,  2173,
    2174,  2175,  2176,  2177,  2178,  2179,  2180,  2181,  2182,  2186,
    2194,  2195,  2199,  2200,  2201,  2202,  2203,  2204,  2205,  2206,
    2207,  2208,  2209,  2210,  2211,  2212,  2213,  2214,  2215,  2216,
    2217,  2218,  2219,  2220,  2221,  2225,  2226,  2227,  2228,  2229,
    2230,  2231,  2235,  2236,  2237,  2238,  2242,  2243,  2244,  2245,
    2250,  2251,  2252,  2253,  2254,  2255,  2256
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
  "TFORWARDING", "TIF", "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX",
  "TINLINE", "TINOUT", "TINT", "TITER", "TINITEQUALS", "TLABEL", "TLAMBDA",
  "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
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
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_start",
  "module_decl_stmt", "access_control", "opt_prototype",
  "include_access_control", "include_module_stmt", "$@1", "block_stmt",
  "stmt_ls", "renames_ls", "opt_only_ls", "except_ls",
  "use_access_control", "use_stmt", "import_stmt", "import_expr",
  "import_ls", "require_stmt", "assignment_stmt", "opt_label_ident",
  "ident_fn_def", "ident_def", "ident_use", "internal_type_ident_def",
  "scalar_type", "reserved_type_ident_use", "do_stmt", "return_stmt",
  "class_level_stmt", "@2", "private_decl", "forwarding_stmt",
  "extern_export_decl_stmt", "$@3", "$@4", "$@5", "$@6",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt", "catch_expr",
  "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_header", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@7", "$@8", "linkage_spec", "fn_decl_stmt", "$@9",
  "$@10", "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "var_arg_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_maybe_decorated", "new_expr", "let_expr", "expr", "opt_expr",
  "opt_try_expr", "lhs_expr", "fun_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "literal_expr", "assoc_expr_ls",
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
     425,   426,   427,   428,   429
};
# endif

#define YYPACT_NINF -1003

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1003)))

#define YYTABLE_NINF -587

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1003,    58,  3041, -1003,   -84, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003,  4393,    -9,   117, -1003, 13545, -1003,  2722,    -9,
   10284, -1003,   221,   107,   117,  4393, 10284,  4393,    60, 18820,
   10451,  7763, -1003,  7930,  8940,  6419, 10284, -1003, -1003,   112,
   -1003, -1003, 18870, -1003,  2485,  9107, -1003, 10284,   185, -1003,
   -1003, -1003, 10284, -1003, 13545, -1003, 10284,   188,   179,   328,
    2927, -1003, 18920, -1003,  9276,  7260, 10284,  9107, 13545, 10284,
     167, -1003,   205,  4393, -1003, 10284, -1003, 10618, 10618, 18870,
   -1003, -1003, 13545, -1003, -1003, 10284, 10284, -1003, 10284, -1003,
   10284, -1003, -1003, 13081, 10284, -1003, 10284, -1003, -1003, -1003,
    3379,  6588,  8099, 10284, -1003,  4224, -1003,   164, -1003,   325,
   -1003, -1003,    47, -1003, -1003, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003, 18870, -1003, 18870,   130,    16, -1003, -1003,  2485,
   -1003,   287, -1003,   306, -1003, -1003,   324,   330,   338, 10284,
     335,   341, 18232,  2491,   347,   345,   350, -1003, -1003, -1003,
   -1003, -1003, -1003, -1003,   393, -1003, -1003, 18232,   356,  4393,
   -1003, -1003,   369, 10284, -1003, -1003, 10284, 10284, 10284, -1003,
   10284,  9276,  9276,   353, -1003, -1003, -1003, -1003,   267,   397,
   -1003, -1003,   359, 15026, 18870,  2485, -1003,   370, -1003,   -30,
   18232,   421,  7429,   455, 18970, 18232,   816,   456, -1003, 19020,
   18870,   816, 18870,   375,    28, 14690,    -5, 14614,   290, -1003,
   14780,  2780,   385,  7429, -1003, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003,  4393, -1003,
     378,  2195,    94,   -16, -1003,  4393, -1003, -1003, 15102,   414,
   -1003,   384,   400, -1003, 15102,   267,   414, -1003,  7429,  6162,
   -1003, -1003,  9443, -1003, -1003, -1003, 18870, -1003,    66, 18232,
   10284, 10284, -1003, 18232,   399, 15650, -1003, 15102,   267, 18232,
     401,  7429, -1003, 18232, 15726, -1003, -1003, 15806, 13074, -1003,
   -1003, 15886,   432,   419,   267, 15102, 15966,   571,   571,  1813,
     414,   414,   257, -1003, -1003,  3548,    11, -1003, 10284, -1003,
      76,   137, -1003,   -71,    18, 16046,   132,  1813,   551, -1003,
    3717, -1003,   526, 10284, 10284,   452,   430, -1003, -1003, -1003,
      68,   424, -1003, 10284,   458, 10284, 10284, 10284,  8940,  8940,
   10284,   448, 10284, 10284, 10284, 10284, 10284,   529, 13081, 10284,
   10284, 10284, 10284, 10284, 10284, 10284, 10284, 10284, 10284, 10284,
   10284, 10284, 10284, 10284, 10284, -1003, -1003, -1003, -1003, -1003,
   -1003,  8266,  8266, -1003, -1003, -1003, -1003,  8266, -1003, -1003,
    8266,  8266,  7429,  7429,  8940,  8940,  7093, -1003, -1003, 15182,
   15258, 16122,    69,  3886, -1003,  8940,    28,   438,   123, -1003,
   10284, -1003, 10284,   485, -1003,   444,   477, -1003, -1003, -1003,
   18870, -1003,  2485, -1003, 18870,   462, -1003,  2485,   576,  9276,
   -1003,  4562,  8940, -1003,   461, -1003,    28,  4731,  8940, -1003,
      28, 10785, 10284, -1003,  4393,   595, 10284, -1003, -1003,   325,
     463, -1003,   578, -1003, -1003,  2195, -1003,   494,   479, -1003,
   10952,   528, 10284,  2485, -1003, -1003, 10284, 10284, -1003,   482,
   -1003, -1003,  9276, -1003, 18232, 18232, -1003,    24, -1003,  7429,
     488, -1003,   644, -1003,   644, -1003, 11119,   542, -1003, -1003,
   -1003, -1003, -1003, -1003,  8435, -1003, 16202,  6757, -1003,  6926,
   -1003,  4393,   497,  8940,  8604,  3210,   499, 10284,  9610, -1003,
   -1003,   307, -1003,  4055, 18870, -1003,   141, 16278,   228, 14870,
    9276,   525, 18770,   224, -1003, 16358,   699,   699,   310, -1003,
     310, -1003,   310, 18615,  1082,   364,  1048,   267,   571,   524,
   -1003, -1003, -1003, -1003,  1813, 17494,   310,  1692,  1692,   699,
    1692,  1692,   149,   571, 17494, 18576,   149,   414,   414,   571,
    1813,   534,   536,   539,   541,   543,   537,   553, -1003,   310,
   -1003,   310,   136, -1003, -1003, -1003,   153, -1003,  1988, 18312,
     394, 11286,  8940, 11453,  8940, 10284,  8940, 13809,    -9, 16434,
   -1003, -1003, -1003, 18232, 16514,  7429, -1003,  7429, 18870,   452,
     284, 18870,   452, -1003,   298, 10284,   159, 10284, 18232,    30,
   15338,  7093, -1003, 10284, 18232,    19, 14946, -1003,   544,   561,
     547, 16594,   561,   564,   678, 16670,  4393, 15414,   664, -1003,
     138, -1003, -1003, -1003, -1003, -1003, -1003,  1131,   162, -1003,
    2672, -1003,   339,   559,  2195,    94,    60,   112, 10284, 10284,
    6252, -1003, -1003,   500,  7596, -1003, 18232, -1003, 16750, 16826,
   -1003, -1003, 18232,   562,    56,   565, -1003,  1646, -1003, -1003,
     299, 18870, -1003, -1003, -1003, -1003, -1003, -1003, -1003,  4393,
     -35, 15494, -1003, -1003, 18232,  4393, 18232, -1003, 16907, -1003,
   -1003, -1003, 10284, -1003,   103,    98, 10284, -1003, 10284, 10785,
   10284,   589,  1451,   569,   618,   225, -1003,   656, -1003, -1003,
   -1003, -1003, 12919,   575, -1003, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003, -1003,  7093, -1003,    35,  8940,  8940, 10284,   711,
   16983, 10284,   715, 17063,   579, 17139,    28,    28, -1003, -1003,
   -1003, -1003,   452,   586, -1003,   452,   587, -1003, 15102, -1003,
   13990,  4900, -1003,  5069, -1003,   236, -1003, 14066,  5238, -1003,
      28,  5407, -1003,    28, -1003, 10284, -1003,  4393, 10284, -1003,
    4393,   718, 18870, -1003, -1003, 18870,  1242, -1003,  2195,   615,
     675, -1003, -1003, -1003,    85, -1003, -1003,   528,   591,    77,
   -1003, -1003,   596,   600, -1003,  5576,  9276, -1003, -1003, -1003,
   18870, -1003,   616,   359, -1003, -1003,  5745,   592,  5914,   598,
   -1003, 10284, -1003, -1003, 10284, 17219, 15570,   609,   610,   606,
    1224, -1003, 10284, 18870, -1003, -1003,   339,   607,   319, -1003,
     630, -1003,   633,   634,   643,   636,   637, -1003,   639,   653,
     646,   647,   648,   329,   659,   650,   655, -1003, -1003, 18870,
   -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003,
   -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003, 10284,
   -1003,   661,   667,   662,   607,   607, -1003, -1003, -1003,   528,
     251,   254, 17295, 11620, 11787, 17375, 11954, 12121, 12288, 12455,
   -1003, -1003,   651, -1003,   652, -1003, -1003,  4393, 10284, 18232,
   10284, 18232,  7093, -1003,  4393, 10284, 18232, -1003, 10284, 18232,
   -1003, 18232, -1003, 18232,   769,  4393,   649,   615, -1003,   645,
    9779,   266, -1003,    26, -1003, -1003,  8940, 13677,  7429,  7429,
    4393, -1003,    13,   657, 10284, -1003, 10284, -1003, 18232,  4393,
   10284, -1003, 18232,  4393, 18232,   246, 10284, 10785, 10284, -1003,
   -1003, -1003, -1003,  6162, -1003,  2330, -1003, 18232, -1003,    40,
     578, -1003, -1003, 17451, -1003, 13908, -1003, -1003, -1003, 10284,
   10284, 10284, 10284, 10284, 10284, 10284, 10284, -1003, 18449, -1003,
   18466, 16670, 14146, 14222, -1003, 16670, 14302, 14378,  4393, -1003,
   -1003,   528,   615,  8773, -1003, -1003, -1003,   100,  9276, -1003,
   -1003,   145, 10284,   -19, 17532, -1003,   853,   672,   681,   421,
   -1003,   359, 18232, 14458, -1003, 14534, -1003, -1003, 18232,   663,
     666, -1003, -1003, 12622,   712,   268, -1003,   689,   692,   607,
     607, 17608, 17688, 17764, 17844, 17920, 18000, 18544, -1003, 18561,
   -1003,  4393,  4393,  4393,  4393, -1003, -1003,   266,  9948,    95,
   -1003, 18232, -1003,   108, -1003,   -63, -1003,   388, 18076, -1003,
   -1003, -1003, 12455,   673,   691, -1003,  4393,  4393, -1003, -1003,
    6083, -1003, -1003,   532, -1003,    26, -1003, -1003, -1003, 10284,
   10284, 10284, 10284, 10284, 10284, -1003, -1003, 16670, 16670, 16670,
   16670, -1003, -1003, -1003, -1003, -1003,   248,  8940, 13247, -1003,
   10284,   145,   108,   108,   108,   108,   108,   108,   145,   923,
   -1003, -1003, 16670, 16670,   660, 12789,    97,   121, 18156, -1003,
   -1003, 18232, -1003, -1003, -1003, -1003, -1003, -1003, -1003,   723,
   -1003, -1003,   621, 13413, -1003, -1003, -1003, 10117, -1003,   671,
   -1003
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   102,   577,   578,   579,   580,
     581,   582,     0,   501,    88,   123,   470,   130,   472,   501,
       0,   129,     0,   375,    88,     0,     0,     0,     0,   124,
     549,   549,   574,     0,     0,     0,     0,   128,    53,     0,
     251,   125,     0,   247,     0,     0,   132,     0,   520,   492,
     583,   133,     0,   252,   464,   374,     0,     0,     0,   146,
      46,   127,   473,   376,     0,     0,     0,     0,   468,     0,
       0,   131,     0,     0,   103,     0,   575,     0,     0,     0,
     126,   233,   466,   378,   134,     0,     0,   632,     0,   634,
       0,   635,   636,   548,     0,   633,   630,   451,   143,   631,
       0,     0,     0,     0,     4,     0,     5,     0,     9,    48,
      10,    11,     0,    12,    13,    14,    16,   447,   448,    24,
      15,   144,   153,   154,    17,    19,    18,    21,    22,    23,
      20,   152,     0,   150,     0,   538,     0,   148,   151,     0,
     149,   554,   534,   449,   535,   454,   452,     0,     0,     0,
     539,   540,     0,   453,     0,   555,   556,   557,   576,   533,
     456,   455,   536,   537,     0,    40,    26,   462,     0,     0,
     502,    89,     0,     0,   472,   124,     0,     0,     0,   473,
       0,     0,     0,   538,   554,   452,   539,   540,   471,   453,
     555,   556,     0,   501,     0,     0,   377,     0,   207,     0,
     432,     0,   439,   580,   473,   550,   250,   580,   171,   473,
       0,   250,     0,     0,     0,     0,     0,     0,     0,   158,
       0,     0,    50,   439,    96,   104,   116,   110,   109,   118,
      99,   108,   119,   105,   120,    97,   121,   114,   107,   115,
     113,   111,   112,    98,   100,   106,   117,   122,     0,   101,
       0,     0,     0,     0,   381,     0,   140,    34,     0,   617,
     524,   521,   522,   523,     0,   465,   618,     7,   439,   250,
     231,   241,   549,   232,   145,   346,   451,   429,     0,   428,
       0,     0,   141,   553,     0,     0,    37,     0,   469,   457,
       0,   439,    38,   463,     0,   214,   210,     0,   453,   214,
     211,     0,   371,     0,   467,     0,     0,   619,   621,   546,
     616,   615,     0,    55,    58,     0,     0,   434,     0,   436,
       0,     0,   435,     0,     0,   428,     0,   547,     0,     6,
       0,    49,     0,     0,     0,   234,     0,   332,   331,   253,
       0,   450,    25,     0,   525,     0,     0,     0,     0,     0,
       0,   620,     0,     0,     0,     0,     0,     0,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   298,   299,   300,   295,
     297,     0,     0,   293,   296,   294,   292,     0,   302,   301,
       0,     0,   439,   439,     0,     0,     0,    27,    28,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     0,    30,
       0,    31,     0,   447,   445,     0,   440,   441,   446,   165,
       0,   168,     0,   161,     0,     0,   167,     0,     0,     0,
     180,     0,     0,   179,     0,   188,     0,     0,     0,   186,
       0,     0,    64,   155,     0,   200,     0,    52,    51,    48,
       0,    32,   314,   248,   385,     0,   386,   388,     0,   410,
       0,   391,     0,     0,   139,    33,     0,     0,    35,     0,
     147,   345,     0,    82,   551,   552,   142,     0,    36,   439,
       0,   221,   212,   208,   213,   209,     0,   369,   366,   173,
      39,    57,    56,    59,     0,   584,     0,     0,   570,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       8,     0,    42,     0,     0,    80,     0,    77,     0,    60,
       0,     0,     0,   325,   380,   500,   612,   611,   614,   623,
     622,   627,   626,   608,   605,   606,   607,   542,   595,     0,
     567,   568,   566,   565,   543,   599,   610,   604,   602,   613,
     603,   601,   593,   598,   600,   609,   592,   596,   597,   594,
     544,     0,     0,     0,     0,     0,     0,     0,   625,   624,
     629,   628,   512,   513,   515,   517,     0,   504,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   586,   501,   501,
     176,   367,   379,   433,     0,     0,   459,     0,     0,   234,
       0,     0,   234,   368,     0,     0,     0,     0,   476,     0,
       0,     0,   189,     0,   482,     0,     0,   187,   631,    67,
       0,    60,    65,     0,   199,     0,     0,     0,     0,   458,
     319,   316,   317,   318,   322,   323,   324,   314,     0,   307,
       0,   315,   333,     0,   389,     0,   137,   138,   136,   135,
       0,   409,   408,   534,     0,   383,   532,   382,     0,     0,
     564,   431,   430,     0,     0,     0,   460,     0,   215,   373,
     534,     0,   585,   541,   571,   437,   573,   438,   196,     0,
       0,     0,   587,   194,   486,     0,   590,   589,     0,    44,
      43,    41,     0,    76,     0,     0,     0,    71,     0,     0,
      64,   235,     0,     0,   245,     0,   242,   329,   326,   327,
     330,   254,     0,     0,    86,    87,    85,    84,    83,   562,
     563,   514,   516,     0,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,   443,
     444,   442,   234,     0,   170,   234,     0,   169,     0,   198,
       0,     0,   178,     0,   177,     0,   507,     0,     0,   184,
       0,     0,   182,     0,   156,     0,   157,     0,     0,   202,
       0,   204,     0,   320,   321,     0,   314,   305,     0,   423,
     334,   337,   336,   338,     0,   387,   390,   391,     0,     0,
     392,   393,   530,   531,   223,     0,     0,   222,   225,   461,
       0,   216,   219,     0,   370,   197,     0,     0,     0,     0,
     195,     0,    81,    78,     0,    62,    61,     0,     0,     0,
     250,   240,     0,   243,   239,   328,   333,   303,    90,   286,
     104,   284,   110,   109,    93,   108,   105,   289,   120,    91,
     121,   107,   111,    92,    94,   106,   122,   283,   265,   268,
     266,   267,   278,   269,   282,   274,   272,   285,   288,   273,
     271,   276,   281,   270,   275,   279,   280,   277,   287,     0,
     263,     0,    95,     0,   303,   303,   261,   569,   505,   391,
     554,   554,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   174,     0,   236,     0,   236,   181,     0,     0,   475,
       0,   474,     0,   506,     0,     0,   481,   185,     0,   480,
     183,    61,   201,   491,   203,     0,     0,   423,   308,     0,
       0,   391,   335,   350,   384,   414,     0,   586,   439,   439,
       0,   227,     0,     0,     0,   217,     0,   192,   488,     0,
       0,   190,   487,     0,   591,     0,     0,     0,    64,    72,
      74,   230,   146,   250,   229,   250,   237,   246,   244,     0,
     314,   260,   264,     0,   290,     0,   256,   257,   509,     0,
       0,     0,     0,     0,     0,     0,     0,   236,   250,   236,
     250,   479,     0,     0,   508,   485,     0,     0,     0,   206,
      54,   391,   423,     0,   426,   425,   427,   534,   347,   311,
     309,     0,     0,     0,     0,   412,   534,     0,     0,   228,
     226,     0,   220,     0,   193,     0,   191,    79,    63,     0,
       0,   238,   405,     0,   339,     0,   262,    90,    92,   303,
     303,     0,     0,     0,     0,     0,     0,   250,   164,   250,
     160,     0,     0,     0,     0,   205,   310,   391,   415,     0,
     349,   348,   364,     0,   365,   352,   355,     0,   351,   343,
     344,   249,     0,   526,   527,   218,     0,     0,    73,    75,
       0,   404,   403,   534,   340,   350,   304,   258,   259,     0,
       0,     0,     0,     0,     0,   166,   162,   478,   477,   484,
     483,   313,   312,   417,   418,   420,   534,     0,   586,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     528,   529,   490,   489,     0,   395,     0,     0,     0,   419,
     421,   354,   356,   357,   360,   361,   362,   358,   359,   353,
     400,   398,   534,   586,   341,   255,   342,   415,   399,   534,
     422
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1003, -1003, -1003,     3,  -345,  2089, -1003, -1003, -1003,   409,
   -1003, -1003, -1003,   392,   548,  -328,  -683,  -673, -1003, -1003,
   -1003,   172, -1003, -1003,   235,   842, -1003,  1709,  -164,  -692,
   -1003,  -873,   604,  -971,  -792, -1003,   -59, -1003, -1003, -1003,
   -1003, -1003, -1003, -1003, -1003,    79, -1003, -1003, -1003,   570,
   -1003,    73, -1003, -1003, -1003, -1003, -1003, -1003,  -530,  -654,
   -1003, -1003, -1003,    57,  1001, -1003, -1003, -1003,  -103, -1003,
   -1003, -1003, -1003,   -86,  -151,  -775, -1003,   -79,   119,   247,
   -1003, -1003, -1003,    71, -1003, -1003,  -234,    48,  -149,  -174,
    -206,  -198,  -430, -1003,  -181, -1003,    12,   869,  -108,   442,
   -1003,  -446,  -777,  -857, -1003,  -606,  -479,  -981, -1002,  -832,
     -52, -1003,   142, -1003,  -220, -1003,   311,   552,  -324, -1003,
   -1003, -1003,  1107, -1003,    -8, -1003, -1003,  -202, -1003,  -595,
   -1003, -1003, -1003,  1374,  1565,   -12,   877,    15,   658, -1003,
    1764,  1951, -1003, -1003, -1003, -1003, -1003, -1003, -1003, -1003,
    -359
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   314,   105,   624,   107,   108,   109,   332,
     449,   110,   222,   111,   315,   619,   623,   620,   112,   113,
     114,   515,   516,   115,   116,   172,   870,   252,   117,   249,
     118,   651,   257,   119,   120,   269,   121,   122,   123,   424,
     601,   420,   598,   124,   125,   734,   126,   127,   128,   482,
     668,   803,   129,   130,   664,   798,   131,   132,   521,   820,
     133,   134,   705,   706,   183,   250,   642,   136,   137,   523,
     826,   711,   873,   874,   446,   961,   453,   638,   639,   640,
     641,   712,   339,   784,  1075,  1135,  1061,   277,   999,  1003,
    1055,  1056,  1057,   138,   303,   487,   139,   140,   253,   254,
     457,   458,   655,  1072,  1024,   461,   652,  1094,   996,   921,
     316,   199,   320,   321,   415,   416,   417,   184,   142,   143,
     144,   145,   185,   147,   169,   170,   576,   436,   755,   577,
     578,   148,   149,   186,   187,   152,   211,   418,   189,   154,
     190,   191,   157,   158,   159,   326,   160,   161,   162,   163,
     164
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     167,   274,   391,   450,   188,   104,   518,   669,   193,   643,
     924,   192,   278,   407,   200,   440,   756,   818,   205,   205,
     872,   215,   217,   220,   221,   663,   817,   255,   956,   425,
     462,   340,  1060,   258,   195,   259,   459,   580,   413,   787,
     264,  1022,   265,   434,   266,   255,  1095,   994,   469,  1100,
     324,   758,   279,   283,   285,   287,   288,   289,     3,   413,
     255,   293,   751,   294,   503,   297,   301,    65,   337,   743,
     304,   480,   746,   305,   306,   165,   307,  1101,   308,   434,
     284,   309,   310,  1001,   311,   991,   459,   408,   279,   283,
     325,   327,   333,  -224,   338,   459,  1120,  -401,   501,   966,
     967,     5,   968,   421,   413,   168,     5,   637,   426,   434,
     410,  1052,   214,   216,   622,   586,   322,  1119,   539,   879,
     171,   502,   506,   926,   463,   795,  -401,   413,   878,   411,
     402,   540,   434,   434,   806,  -224,   653,   344,  1002,  -401,
    -406,  1097,  -406,   586,  1000,   403,  1060,   391,  1052,   319,
    1071,   494,  -401,   472,  -406,   334,  1140,   541,   504,   403,
    1047,   167,   670,  1021,   399,   400,   401,   796,   293,   279,
     325,  -406,   566,   567,   460,  -406,   495,   403,   923,  1023,
     347,   323,   721,   403,   773,   406,   956,   505,   956,  -224,
     283,   196,   403,   543,   403,    74,   267,  -406,   786,  -401,
      74,   542,  -406,   260,  -401,  1054,   472,    65,   463,   504,
     470,   283,   892,  -406,  1046,   894,   497,   504,   422,  -424,
     722,   797,   774,   427,   460,   473,   202,   524,   413,   413,
     195,  1053,   348,   460,  -406,   504,   349,   504,   587,   978,
    -424,   980,  1054,   498,  -424,   956,   927,   956,    23,  -406,
     414,   707,   580,  -406,  1077,  1078,   283,   261,  1131,   665,
     205,   290,   814,   463,  1098,  1020,  1133,  -424,   474,   475,
    1092,   414,   508,   872,  1019,   262,  1138,   499,   223,   283,
    1134,   692,   592,   351,   352,   403,   353,   354,   355,  -497,
     356,   357,   263,   723,    55,   360,  -561,   708,  -561,   472,
     693,   509,   776,   367,   500,    63,   496,   984,   709,   371,
     372,   373,   296,   300,   600,   413,   414,    23,   493,   604,
     724,   517,   519,  1037,   194,  1039,   749,   710,   330,   777,
      83,   525,   919,   526,   527,   528,   530,   532,   533,   414,
     534,   535,   536,   537,   538,   268,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,    55,   580,   823,   780,  -416,   696,   283,
     283,   291,   622,   -70,    63,   283,   902,   606,   283,   283,
     283,   283,   569,   571,   579,   654,   696,   697,  -416,   -47,
     824,  -511,  -416,   589,  -510,   347,   561,   562,   593,    83,
     594,   351,   563,   903,   331,   564,   565,   -47,   776,   357,
     998,  1017,   781,   760,   763,  -416,   165,   279,  -511,   608,
     610,  -510,   491,   782,   463,   614,   616,   529,   531,   621,
     621,   637,   625,   413,   627,  1076,   -70,   256,   463,  -372,
     414,   414,   783,   744,   351,   347,  -494,   348,  1005,   355,
     656,   349,   357,   -96,   658,   659,   360,   747,  -372,   256,
     662,  -102,   447,   -98,   341,   448,   165,   283,   701,   295,
     299,  -103,   689,   568,   570,   955,   394,   727,   807,   809,
     395,   395,   662,  -493,   588,   283,   945,   283,   343,   342,
     804,   681,   662,   684,  -498,   686,   688,   348,   351,   352,
    -499,   349,   354,   355,  -496,   356,   357,  1102,   279,  -495,
     360,   609,   322,   392,   322,   393,   493,   615,   367,  -561,
     661,  -561,   396,   403,   371,   372,   373,   414,   398,   409,
     813,   543,     5,  1103,   412,  1104,  1105,   419,   423,  1106,
    1107,   429,   661,   580,   452,   319,  -407,   319,   351,   539,
     466,   486,   661,   355,   141,   356,   357,  -407,   476,   510,
     360,   875,   540,  -558,   141,  -558,   467,   479,   367,   608,
     730,   614,   733,   625,   735,   684,   373,   141,   488,   141,
     736,   737,   680,   283,   404,   283,  -407,   514,   541,  -402,
    -560,   520,  -560,   748,   522,   750,   997,   591,   789,   579,
     357,   757,   347,  1006,   595,   630,   256,   256,   256,   256,
     740,   596,  -407,   445,  -559,   637,  -559,   597,  -402,  -407,
     622,   603,   605,  1099,   631,   141,    74,   611,   632,   626,
     629,  -402,   542,   955,   644,   955,   289,   293,   325,   675,
    -407,   677,   283,   739,  -402,   414,   645,   654,   633,   660,
     256,   634,   141,    57,   348,   666,   256,   141,   349,  -407,
     153,   729,   635,   732,  -407,   667,   679,  -407,   685,   791,
     153,   286,  1123,  1124,  1125,  1126,  1127,  1128,  -394,   256,
     517,   636,   671,   153,   815,   153,   816,   621,   621,   702,
     713,  -402,   955,   714,   955,   715,  -402,   256,   716,  1073,
     717,   696,   718,   -66,   719,   351,   764,  -394,  1007,  1008,
     355,   579,   767,   357,   530,   569,   882,   360,  -306,   885,
    -394,   141,   720,   766,  1096,   772,   785,   794,  -396,   472,
     347,   153,   799,  -394,   821,   298,   298,   822,  1109,   899,
     825,   901,   877,   883,   932,  -306,   906,   886,   888,   909,
     893,   895,   915,   911,   920,   934,   913,  -396,   153,   922,
     925,   939,   928,   153,   413,   413,   929,   943,   949,   950,
    -396,   951,  -123,   960,  1096,  -130,  -129,   -99,  -128,  -125,
    -394,  -132,   348,  -396,   279,  -394,   349,   -97,  -133,  -127,
    -131,  1132,  -126,  -100,   938,   964,   942,  -134,   256,   944,
     141,  -101,   621,   988,   965,   529,   568,   141,   990,  1139,
     957,  1074,   992,  1096,  1030,   977,   979,   350,   430,   433,
     435,   439,  1068,   -96,  1011,  1069,   -98,   153,   256,  1130,
    -396,  1110,   256,   351,   352,  -396,   353,   354,   355,  1063,
     356,   357,   358,    23,   359,   360,   361,   362,  1064,  1111,
     364,   365,   366,   367,   368,   369,   370,   963,   628,   371,
     372,   373,   465,  1101,   812,    40,   197,   141,   468,   484,
     374,   899,   901,   933,   906,   909,   938,   942,   513,  1029,
     958,  1025,   141,  1136,   775,   981,   982,    53,   983,    55,
     579,   478,   985,   986,   274,   918,   987,   959,  1091,  -411,
      63,  1117,  1129,  1122,   219,   657,   153,   206,   741,   489,
    -411,     0,     0,   153,  1004,   684,   283,   283,     0,     0,
       0,     0,  1012,     0,  1013,    83,     0,     0,  1015,     0,
       0,     0,     0,     0,  1018,   621,   621,     0,     0,  -411,
       0,  1049,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141,     0,   981,  1031,  1032,
     985,  1033,  1034,  1035,  1036,  -411,     0,     0,   995,  -413,
       0,     0,  -411,   153,     0,     0,   414,   414,     0,     0,
    -413,   325,     0,   141,     0,     0,  1051,     0,   153,   141,
    1058,     0,     0,  -411,     0,     0,   141,     0,     0,     0,
       0,   256,   256,   135,     0,     0,     0,   256,   256,  -413,
     590,     0,  -411,   135,     0,     0,     0,  -411,  1116,   771,
    -411,     0,     0,     0,     0,     0,   135,     0,   135,  1087,
    1088,  1089,  1090,     0,     0,  -413,     0,     0,     0,     0,
     612,     0,  -413,     0,   617,     0,  1050,     0,     0,     0,
     942,     0,     0,   141,  1112,  1113,     0,   141,   325,   801,
       0,   153,     0,  -413,     0,   141,     0,  1087,  1088,  1089,
    1090,  1112,  1113,     0,   135,     0,     0,     0,     0,   347,
       0,     0,  -413,     0,     0,  1118,   684,  -413,  1121,   153,
    -413,     0,     0,     0,     0,   153,  1093,     0,     0,     0,
       0,   135,   153,     0,     0,     0,   135,     0,     0,   146,
       0,     0,     0,   347,     0,     0,     0,     0,     0,   146,
       0,   684,     0,     0,     0,   942,     0,     0,   256,   256,
     725,   348,   146,     0,   146,   349,     0,     0,     0,     0,
     256,     0,   445,     0,     0,     0,  1093,     0,     0,   445,
       0,     0,   256,     0,     0,   256,     0,     0,   630,   153,
       0,     0,     0,   153,     0,   348,     0,     0,     0,   349,
     135,   153,     0,     0,     0,     0,     0,   631,   141,     0,
     146,   632,   351,   352,     0,  1093,     0,   355,     0,   356,
     357,     0,     0,     0,   360,   935,     0,     0,     0,     0,
       0,   633,   367,     0,   634,     0,   328,   146,   371,   372,
     373,     0,   146,   752,   754,   635,   351,     0,     0,   759,
     762,   355,     0,   356,   357,     0,     0,     0,   360,     0,
       0,   141,     0,     0,   636,     0,   367,   141,     0,     0,
       0,     0,   371,   372,   373,     0,   270,     0,     0,   135,
      22,    23,     0,     0,     0,     0,   135,     0,     0,   271,
       0,    30,   272,     0,   876,     0,    35,     0,     0,   630,
       0,     0,     0,    40,     0,     0,   146,     0,   880,   881,
       0,     0,     0,     0,   153,     0,     0,     0,   631,     0,
       0,     0,   632,     0,     0,    53,     0,    55,     0,    57,
       0,   952,     0,   141,   953,   141,   273,     0,    63,     0,
     141,     0,   633,   141,     0,   634,   135,    57,     0,   141,
       0,     0,   141,     0,   256,     0,   635,    79,     0,     0,
      81,   135,     0,    83,     0,     0,     0,   153,     0,     0,
     890,   891,     0,   153,     0,   636,     0,   141,     0,     0,
       0,     0,   896,     0,     0,   146,     0,     0,   141,     0,
     141,     0,   146,     0,   907,     0,     0,   910,     0,     0,
       0,     0,     0,     0,   445,   445,   150,     0,   445,   445,
       0,     0,     0,    98,     0,     0,   150,     0,     0,   954,
       0,     0,     0,     0,     0,  1059,     0,     0,     0,   150,
       0,   150,     0,  1065,   135,   445,     0,   445,     0,   153,
       0,   153,     0,     0,     0,     0,   153,     0,     0,   153,
       0,     0,   146,     0,     0,   153,     0,     0,   153,     0,
       0,     0,   135,     0,     0,     0,     0,   146,   135,     0,
       0,     0,     0,     0,     0,   135,     0,   150,     0,   141,
       0,     0,   819,   153,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,   153,     0,   153,   141,     0,     0,
       0,     0,     0,  -236,   150,     0,     0,  -236,  -236,   150,
       0,     0,   141,     0,     0,     0,  -236,     0,  -236,  -236,
       0,   141,     0,  -236,     0,   141,     0,     0,     0,     0,
    -236,     0,   135,  -236,     0,     0,   135,     0,     0,  1059,
     146,     0,     0,     0,   135,     0,     0,     0,     0,     0,
       0,     0,  -236,     0,  -236,     0,  -236,     0,  -236,  -236,
       0,  -236,     0,  -236,     0,  -236,  1010,     0,   146,     0,
     141,     0,     0,   150,   146,     0,     0,     0,     0,     0,
       0,   146,     0,     0,  -236,   153,     0,  -236,     0,     0,
    -236,     0,   153,     0,     0,     0,     0,   151,     0,     0,
       0,     0,     0,   153,     0,     0,     0,   151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     151,     0,   151,   141,   141,   141,   141,   153,     0,     0,
       0,   153,     0,     0,     0,     0,     0,     0,   146,     0,
    -236,     0,   146,     0,     0,     0,  -236,     0,   141,   141,
     146,     0,   150,     0,     0,     0,     0,   135,     0,   150,
       0,     0,     0,     0,     0,     0,     0,     0,   151,     0,
       0,     0,     0,     0,     0,     0,   153,     0,     0,   224,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   225,   226,   151,   227,     0,     0,     0,
     151,   228,     0,     0,     0,     0,     0,     0,     0,   229,
     135,     0,     0,     0,     0,   230,   135,     0,     0,   150,
     231,     0,     0,     0,   232,     0,     0,   233,     0,   153,
     153,   153,   153,     0,   150,   234,     0,     0,     0,     0,
       0,   235,   236,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,   347,   153,   153,     0,   238,     0,     0,
       0,     0,     0,   146,   151,     0,   239,   240,     0,   241,
       0,   242,     0,   243,     0,     0,   244,     0,     0,     0,
     245,   248,   135,   246,   135,     0,   247,     0,     0,   135,
       0,     0,   135,     0,     0,     0,   155,     0,   135,     0,
       0,   135,     0,     0,     0,   348,   155,   150,     0,   349,
       0,     0,     0,     0,     0,     0,   146,     0,   302,   155,
       0,   155,   146,     0,     0,     0,   135,     0,     0,     0,
       0,     0,     0,     0,     0,   150,     0,   135,     0,   135,
     403,   150,   800,   151,     0,     0,     0,     0,   150,     0,
     151,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   155,   360,     0,
       0,   335,     0,   336,   347,   366,   367,     0,     0,   370,
       0,     0,   371,   372,   373,     0,     0,     0,   146,     0,
     146,     0,     0,   374,   155,   146,     0,     0,   146,   155,
       0,     0,     0,     0,   146,   150,     0,   146,     0,   150,
     151,     0,     0,     0,     0,     0,     0,   150,     0,     0,
       0,     0,     0,     0,     0,   151,   348,     0,   135,     0,
     349,     0,   146,   302,     0,   135,     0,     0,     0,     0,
       0,     0,     0,   146,     0,   146,   135,     0,     0,   302,
       0,   428,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   135,     0,   155,     0,     0,     0,     0,     0,     0,
     135,     0,     0,     0,   135,     0,     0,   351,   352,     0,
     353,   354,   355,   156,   356,   357,     0,     0,     0,   360,
     456,     0,     0,   156,     0,     0,   366,   367,   151,     0,
     370,     0,     0,   371,   372,   373,   156,     0,   156,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,   135,
       0,     5,     0,     0,     0,     0,   151,     0,     0,     0,
     150,     0,   151,     0,   146,    15,     0,     0,    17,   151,
       0,   146,   155,    21,     0,     0,     0,     0,     0,   155,
       0,     0,   146,   175,   156,     0,     0,     0,     0,     0,
       0,     0,    37,     0,     0,     0,     0,   146,     0,    41,
       0,     0,   135,   135,   135,   135,   146,    46,     0,     0,
     146,   156,     0,   150,    51,     0,   156,     0,     0,   150,
       0,     0,     0,     0,     0,     0,   151,   135,   135,    61,
     151,     0,     0,     0,     0,     0,     0,     0,   151,   155,
       0,    71,     0,     0,     0,    74,     0,     0,     0,     0,
       0,   106,    80,     0,   155,   146,     0,     0,    84,     0,
       0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   198,     0,   201,     0,     0,     0,
     156,     0,     0,     0,     0,   150,     0,   150,     0,   599,
       0,     0,   150,   602,     0,   150,     0,     0,     0,     0,
       0,   150,     0,     0,   150,     0,     0,     0,   146,   146,
     146,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,   456,     0,     0,   155,     0,   150,
       0,     0,     0,   146,   146,     0,     0,     0,     0,     0,
     150,     0,   150,     0,     0,     0,     0,     0,     0,   106,
       0,   151,     0,     0,   329,   155,     0,     0,   224,   156,
       0,   155,     0,     0,     0,     0,   156,     0,   155,     0,
       0,     0,   225,   226,     0,   227,     0,     0,     0,     0,
     228,     0,     0,   691,     0,     0,     0,     0,   229,     0,
       0,   704,     0,     0,   230,     0,     0,     0,     0,   231,
       0,     0,     0,   232,   151,     0,   233,     0,     0,     0,
     151,     0,     0,     0,   234,     0,     0,     0,   397,     0,
     235,   236,     0,     0,     0,   155,   156,   237,     0,   155,
       0,   150,     0,     0,     0,     0,   238,   155,   150,     0,
       0,   156,     0,     0,     0,   239,   240,     0,   241,   150,
     242,     0,   243,     0,     0,   244,     0,     0,     0,   245,
     454,     0,   246,     0,   150,   247,     0,   742,     0,     0,
     745,     0,     0,   150,     0,     0,   151,   150,   151,     0,
       0,     0,     0,   151,     0,     0,   151,     0,     0,     0,
       0,     0,   151,     0,     0,   151,     0,   451,     0,     0,
       0,     0,     0,     0,   464,     0,     0,     0,     0,   779,
       0,     0,   270,   456,   156,     0,    22,    23,     0,     0,
     151,   455,   150,     0,     0,   271,     0,    30,   272,     0,
       0,   151,    35,   151,     0,     0,   802,     0,     0,    40,
     302,     0,   156,     0,     0,     0,     0,     0,   156,     0,
     155,     0,     0,     0,     0,   156,     0,     0,     0,     0,
       0,    53,     0,    55,   106,   328,     0,   952,     0,     0,
     953,     0,   273,     0,    63,   150,   150,   150,   150,   106,
       0,   871,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,     0,     0,    81,     0,     0,    83,
     150,   150,     0,   155,     0,     0,     0,     0,     0,   155,
       0,     0,   156,     0,     0,     0,   156,     0,     0,     0,
       0,     0,   151,     0,   156,     0,     0,     0,     0,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     151,   916,     0,     0,   917,     0,     0,   456,   224,    98,
       0,     0,   106,     0,     0,   151,     0,     0,     0,     0,
       0,     0,   225,   226,   151,   227,     0,     0,   151,   802,
     228,     0,     0,     0,     0,   155,     0,   155,   229,     0,
     464,     0,   155,     0,   230,   155,   464,     0,     0,   231,
       0,   155,   704,   232,   155,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     235,   236,     0,   151,     0,     0,     0,   237,   962,   155,
       0,     0,     0,     0,     0,     0,   238,     0,     0,     0,
     155,     0,   155,     0,     0,   239,   240,   156,   241,     0,
     242,     0,   243,     0,     0,   244,     0,     0,     0,   245,
     678,     0,   246,     0,   683,   247,     0,     0,     0,     0,
       0,     0,   106,     0,     0,     0,   151,   151,   151,   151,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,     0,     0,     0,     0,     0,
     156,   151,   151,     0,     0,     0,   156,     0,     0,     0,
       0,  -518,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,   390,     0,     0,  -558,     0,  -558,
       0,   155,     0,     0,     0,     0,     0,     0,   155,     0,
       0,     0,     0,     0,   871,   224,     0,     0,     0,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   225,
     226,     0,   227,     0,   155,     0,     0,   228,     0,     0,
       0,     0,   156,   155,   156,   229,     0,   155,     0,   156,
       0,   230,   156,     0,     0,   769,   231,     0,   156,     0,
     232,   156,     0,   233,     0,  -231,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,   235,   236,  -231,
    -231,     0,  -231,     0,   237,     0,   156,  -231,     0,     0,
       0,     0,   155,   238,     0,  -231,     0,   156,     0,   156,
       0,  -231,   239,   240,     0,   241,  -231,   242,   805,   243,
    -231,     0,   244,  -231,   810,     0,   245,     0,     0,   246,
       0,  -231,   247,     0,     0,     0,     0,  -231,  -231,     0,
       0,     0,   345,     0,  -231,     0,     0,     0,     0,   346,
       0,     0,     0,  -231,     0,   155,   155,   155,   155,     0,
       0,   347,  -231,  -231,     0,  -231,     0,  -231,     0,  -231,
       0,     0,  -231,     0,     0,     0,  -231,     0,     0,  -231,
     155,   155,  -231,     0,     0,     0,     0,     0,   778,     0,
     464,     0,   464,     0,     0,     0,     0,   464,   156,     0,
     464,     0,     0,     0,     0,   156,   912,     0,     0,   914,
       0,     0,     0,   348,     0,     0,   156,   349,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,   156,     0,     0,   931,     0,     0,     0,     0,     0,
     156,     0,     0,     0,   156,   937,     0,   941,   350,   375,
     376,   377,   378,   379,   380,     0,     0,   383,   384,   385,
     386,     0,   388,   389,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   156,
     371,   372,   373,     0,   403,     0,     0,     0,     0,   270,
       0,   374,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,   271,     0,    30,   272,     0,     0,     0,    35,
       0,     0,   -69,     0,     0,     0,    40,     0,     0,  -250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   156,   156,   156,   156,     0,     0,    53,     0,
      55,     0,     0,     0,   989,  -250,     0,     0,     0,   273,
       0,    63,     0,     0,     0,     0,     0,   156,   156,  1009,
       0,     0,     0,     0,     0,     0,     0,     0,  1014,     0,
      79,     0,  1016,    81,     0,   -69,    83,     0,     0,     0,
       0,    -2,     4,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,  1045,    30,    31,
      32,    33,    34,    35,    36,    37,   -68,     0,    38,    39,
      40,     0,    41,  -250,     0,    42,    43,    44,     0,    45,
      46,    47,   -45,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,    57,    58,    59,  -250,
     -45,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -68,
      83,    84,     0,     0,    85,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     0,   101,     0,   102,
       0,     4,   103,     5,     0,     6,     7,     8,     9,    10,
      11,     0,  -586,     0,    12,    13,    14,    15,    16,  -586,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,  -586,    27,    28,  -586,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,   -68,     0,    38,    39,    40,
       0,    41,  -250,     0,    42,    43,    44,     0,    45,    46,
      47,   -45,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -250,   -45,
      60,    61,    62,  -586,    63,    64,    65,  -586,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -68,    83,
      84,     0,     0,    85,     0,    86,     0,     0,  -586,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -586,  -586,    90,  -586,  -586,  -586,
    -586,  -586,  -586,  -586,     0,  -586,  -586,  -586,  -586,  -586,
       0,  -586,  -586,  -586,  -586,  -586,  -586,  -586,  -586,    98,
    -586,  -586,  -586,     0,   100,  -586,   101,     0,   102,     0,
     312,  -586,     5,   275,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -68,     0,    38,    39,    40,     0,
      41,  -250,     0,    42,    43,    44,     0,    45,    46,    47,
     -45,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -250,   -45,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -68,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,   276,    98,     0,
       0,    99,     0,   100,   313,   101,     0,   102,     0,     4,
     103,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -68,     0,    38,    39,    40,     0,    41,
    -250,     0,    42,    43,    44,     0,    45,    46,    47,   -45,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,    57,    58,    59,  -250,   -45,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -68,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,   492,   101,     0,   102,     0,   511,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -68,     0,    38,    39,    40,     0,    41,  -250,
       0,    42,    43,    44,     0,    45,    46,    47,   -45,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -250,   -45,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -68,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   512,   101,     0,   102,     0,   312,   103,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -68,     0,    38,    39,    40,     0,    41,  -250,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -250,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -68,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,   313,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -68,     0,    38,    39,    40,     0,    41,  -250,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -250,   -45,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -68,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
     690,   101,     0,   102,     0,     4,   103,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -68,
       0,    38,    39,    40,     0,    41,  -250,     0,    42,    43,
      44,     0,    45,    46,    47,   -45,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,   328,
      58,    59,  -250,   -45,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -68,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -68,     0,
      38,    39,    40,     0,    41,  -250,     0,    42,    43,    44,
       0,    45,    46,    47,   -45,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -250,   -45,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -68,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     0,   101,
       0,   102,     0,     4,   103,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   607,    37,   -68,     0,    38,
      39,    40,     0,    41,  -250,     0,    42,    43,    44,     0,
      45,    46,    47,   -45,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -250,   -45,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -68,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,   613,    37,   -68,     0,    38,    39,
      40,     0,    41,  -250,     0,    42,    43,    44,     0,    45,
      46,    47,   -45,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -250,
     -45,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -68,
      83,    84,     0,     0,    85,     0,    86,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
      98,     0,     0,    99,     0,   100,     0,   101,     0,   102,
       0,     4,   103,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,    29,     0,    30,    31,    32,
      33,    34,    35,   898,    37,   -68,     0,    38,    39,    40,
       0,    41,  -250,     0,    42,    43,    44,     0,    45,    46,
      47,   -45,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -250,   -45,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -68,    83,
      84,     0,     0,    85,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,    98,
       0,     0,    99,     0,   100,     0,   101,     0,   102,     0,
       4,   103,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,   900,    37,   -68,     0,    38,    39,    40,     0,
      41,  -250,     0,    42,    43,    44,     0,    45,    46,    47,
     -45,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -250,   -45,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -68,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     0,   101,     0,   102,     0,     4,
     103,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,    29,     0,    30,    31,    32,    33,    34,
      35,   905,    37,   -68,     0,    38,    39,    40,     0,    41,
    -250,     0,    42,    43,    44,     0,    45,    46,    47,   -45,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -250,   -45,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -68,    83,    84,     0,
       0,    85,     0,    86,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,    98,     0,     0,
      99,     0,   100,     0,   101,     0,   102,     0,     4,   103,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
     908,    37,   -68,     0,    38,    39,    40,     0,    41,  -250,
       0,    42,    43,    44,     0,    45,    46,    47,   -45,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -250,   -45,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -68,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     0,   101,     0,   102,     0,     4,   103,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,   930,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -68,     0,    38,    39,    40,     0,    41,  -250,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -250,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -68,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   936,    37,
     -68,     0,    38,    39,    40,     0,    41,  -250,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -250,   -45,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -68,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,     0,   102,     0,     4,   103,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,   940,    37,   -68,
       0,    38,    39,    40,     0,    41,  -250,     0,    42,    43,
      44,     0,    45,    46,    47,   -45,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -250,   -45,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -68,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,   102,     0,  1114,   103,     5,   275,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,   270,     0,     0,    80,    22,    23,
      82,     0,     0,    84,     0,     0,     0,   271,     0,    30,
     272,    87,     0,     0,    35,     0,     0,     0,     0,     0,
       0,    40,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,    53,     0,    55,    94,     0,     0,    95,
      96,   276,     0,     0,   273,    99,    63,   181,     0,   101,
       0,   182,  1115,   788,   103,     5,   275,     6,     7,     8,
       9,    10,    11,     0,     0,    79,   173,     0,    81,    15,
      16,    83,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   180,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
     276,     0,     0,     0,    99,     0,   181,     0,   101,     0,
     182,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,   218,    23,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,    63,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,    83,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   182,     0,     0,
     103,     5,   275,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,   280,
     281,     0,    80,   317,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,   318,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   276,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,     0,     0,   103,
       5,   275,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,   280,   281,
       0,    80,   317,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,   276,     0,     0,     0,    99,
       0,   181,     0,   101,   674,   182,     0,     0,   103,     5,
     275,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   178,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,   280,   281,     0,
      80,   317,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   276,     0,     0,     0,    99,     0,
     181,     0,   101,   676,   182,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
     572,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,   573,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,   574,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,   575,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,   280,   281,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,   282,
       0,     0,    99,     0,   181,     0,   101,     0,   182,     0,
       0,   103,     5,   275,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
     280,   281,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,   276,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   182,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   178,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
     790,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,   280,   281,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     181,     0,   101,     0,   182,     0,     5,   103,     6,     7,
       8,   207,    10,    11,   208,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   209,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,   210,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,   212,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,   213,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,     0,   101,     0,   182,     0,
       0,   103,     5,   275,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,   213,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,   276,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   182,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   178,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,   280,   281,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     181,     0,   101,     0,   182,     0,     0,   103,     5,   275,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,   276,     0,     0,     0,    99,     0,   181,
     672,   101,     0,   182,     0,     0,   103,     5,   275,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,   276,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,   682,     0,   103,     5,   275,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,   276,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,  1048,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,   213,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,     0,   101,     0,   182,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,   255,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   100,     0,   101,     0,   182,     0,     0,   103,     5,
     275,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   178,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,   276,     0,     0,     0,    99,     0,
     181,     0,   101,     0,   182,     0,     5,   103,     6,     7,
       8,   207,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   209,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,   210,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   178,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,     0,   101,     0,   182,   687,
       0,   103,     5,   275,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   646,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,   647,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
     648,    70,    71,    72,   649,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,   276,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   993,     0,     0,
     103,     5,   275,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,   276,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   993,     0,     0,   103,
       5,   275,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     976,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,   276,     0,     0,     0,    99,
       0,   181,     0,   101,     0,   182,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   178,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,     0,     5,   103,     6,     7,     8,   203,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   204,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   182,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   178,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   100,     0,   101,     0,   182,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,   618,     0,   181,
       0,   101,     0,   182,     0,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   646,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
     647,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,   648,    70,    71,    72,   649,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   181,     0,   101,     0,
     650,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   181,     0,   101,     0,   650,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     173,     0,     0,    15,    16,     0,    17,     0,   174,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   175,     0,     0,     0,    32,   176,   177,     0,   728,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   179,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   180,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     181,     0,   101,     0,   182,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   731,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   181,     0,   101,
       0,   182,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   173,     0,     0,    15,    16,     0,
      17,     0,   174,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   175,     0,     0,     0,    32,
     176,   177,     0,   970,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   179,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   180,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   181,     0,   101,     0,   182,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     971,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   181,     0,   101,     0,   182,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   173,     0,
       0,    15,    16,     0,    17,     0,   174,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   175,
       0,     0,     0,    32,   176,   177,     0,   973,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   179,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   180,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   181,     0,
     101,     0,   182,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   974,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   181,     0,   101,     0,   182,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   173,     0,     0,    15,    16,     0,    17,     0,
     174,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   175,     0,     0,     0,    32,   176,   177,
       0,   975,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     179,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   180,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   181,     0,   101,     0,   182,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     175,     0,     0,     0,    32,   176,   177,     0,   976,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   181,
       0,   101,     0,   182,     0,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   173,     0,     0,    15,
      16,     0,    17,     0,   174,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   646,     0,   175,     0,     0,
       0,    32,   176,   177,     0,   178,    37,     0,     0,     0,
     647,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   179,     0,     0,     0,     0,     0,
       0,     0,    68,   648,    70,    71,    72,   649,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   181,     0,   101,     0,
    1070,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   175,     0,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     827,     0,   828,    88,    89,    90,    91,    92,     0,     0,
       0,   829,    93,     0,     0,     0,   830,   226,   831,   832,
       0,     0,    94,     0,   833,    95,    96,    97,     0,     0,
       0,    99,   229,     0,   175,   101,     0,  1070,   834,     0,
     103,     0,     0,   835,     0,     0,     0,   232,     0,     0,
     836,     0,   837,     0,     0,     0,     0,     0,   838,     0,
       0,     0,     0,     0,   839,   840,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
     841,     0,     0,     0,     0,     0,     0,     0,     0,   239,
     240,     0,   842,     0,   242,     0,   843,     0,     0,   844,
       0,     0,     0,   845,     0,     0,   246,     0,     0,   846,
       0,     0,     0,     0,     0,     0,     0,     0,   375,   376,
     377,   378,   379,   380,     0,     0,   383,   384,   385,   386,
       0,   388,   389,   847,   848,   849,   850,   851,     0,     0,
     852,     0,     0,     0,   853,   854,   855,   856,   857,   858,
     859,   860,   861,   862,   863,     0,   864,     0,     0,   865,
     866,   867,   868,     0,     5,   869,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   173,     0,     0,    15,    16,
       0,    17,     0,   174,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   175,     0,     0,     0,
      32,   176,   177,     0,   178,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   179,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   180,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,   390,    96,    97,
    -558,     0,  -558,    99,     0,   181,     0,   101,     0,   182,
       5,   275,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   175,     0,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,  -415,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,     0,     0,     0,  -415,     0,     0,
       0,  -415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,   181,     0,   101,  -415,   993,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   173,     0,     0,
      15,    16,     0,    17,     0,   174,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   175,     0,
       0,     0,    32,   176,   177,     0,   178,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
    -397,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   179,     0,     0,     0,  -397,
       0,     0,     0,    68,    69,    70,    71,    72,   180,     0,
      74,     0,  -397,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,  -397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,    90,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   173,
       0,     0,    15,    16,     0,    17,     0,   174,     0,     0,
      21,    97,  -397,     0,     0,     0,     0,  -397,    28,   101,
     175,  1070,     0,     0,    32,   176,   177,     0,   178,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   179,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     180,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       5,    90,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   173,     0,     0,    15,    16,     0,    17,     0,   174,
       0,     0,    21,    97,     0,     0,     0,     0,     0,   181,
      28,   101,   175,   182,     0,     0,    32,   176,   177,     0,
     178,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   179,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   180,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,    90,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   173,     0,     0,    15,    16,     0,    17,
       0,   174,     0,     0,    21,    97,     0,     0,     0,     0,
       0,   181,    28,   101,   175,   650,     0,     0,    32,   176,
     177,     0,   178,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   179,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   180,     0,    74,     0,     0,    76,
       0,  1027,     0,    80,     0,     0,    82,     0,     0,    84,
     829,     0,     0,     0,     0,   225,   226,   831,   227,     0,
       0,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,     0,    90,     0,   834,     0,     0,
       0,     0,   231,     0,     0,     0,   232,     0,     0,   233,
       0,   837,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,   839,   236,   101,     0,   182,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,   239,   240,
       0,   241,   345,   242,     0,  1028,     0,     0,   844,   346,
       0,     0,   245,     0,     0,   246,     0,     0,   247,     0,
       0,   347,     0,     0,     0,     0,     0,   375,   376,   377,
     378,   379,   380,     0,     0,   383,   384,   385,   386,     0,
     388,   389,   847,   848,   849,   850,   851,     0,     0,   852,
       0,     0,     0,   853,   854,   855,   856,   857,   858,   859,
     860,   861,   862,   863,     0,   864,     0,     0,   865,   866,
     867,   868,     0,   348,     0,     0,     0,   349,   345,     0,
       0,     0,     0,     0,     0,   346,   897,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,   375,
     376,   377,   378,   379,   380,     0,     0,   383,   384,   385,
     386,     0,   388,   389,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,   403,     0,     0,     0,   345,     0,
       0,   374,   904,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,   375,   376,   377,   378,   379,
     380,     0,     0,   383,   384,   385,   386,     0,   388,   389,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   348,
     403,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,  1041,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,   375,   376,   377,   378,   379,
     380,     0,     0,   383,   384,   385,   386,     0,   388,   389,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
     403,     0,     0,     0,   345,     0,     0,   374,  1042,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
     350,   375,   376,   377,   378,   379,   380,     0,     0,   383,
     384,   385,   386,     0,   388,   389,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   348,   403,     0,     0,   349,
     345,     0,     0,   374,     0,     0,     0,   346,  1043,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,   375,   376,   377,   378,   379,   380,     0,     0,   383,
     384,   385,   386,     0,   388,   389,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,   403,     0,     0,     0,
     345,     0,     0,   374,  1044,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,   350,   375,   376,   377,
     378,   379,   380,     0,     0,   383,   384,   385,   386,     0,
     388,   389,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,   403,     0,     0,   349,   345,     0,     0,   374,
       0,     0,     0,   346,  1066,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,   375,   376,   377,
     378,   379,   380,     0,     0,   383,   384,   385,   386,     0,
     388,   389,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,   403,     0,     0,     0,   345,     0,     0,   374,
    1067,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   437,     0,     0,     0,
       0,     0,   350,   375,   376,   377,   378,   379,   380,     0,
     438,   383,   384,   385,   386,     0,   388,   389,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,   403,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   431,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   350,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   348,   371,   372,   373,   349,   403,     0,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,     0,   345,     0,     0,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,   347,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,   442,     0,
     371,   372,   373,     0,   403,     0,     0,     0,     0,     0,
       0,   374,     0,   348,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   345,   698,     0,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,   347,     0,     0,     0,     0,   699,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,   700,   443,
     371,   372,   373,     0,     0,     0,     0,     0,     0,     0,
       0,   374,     0,   348,     0,     0,     0,   349,   345,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   761,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,     0,     0,     0,     0,   345,     0,
       0,   374,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
     434,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   348,
     403,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   255,     0,     0,     0,     0,     0,
     168,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
       0,     0,     0,     0,   345,     0,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   581,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,   582,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   348,   403,     0,     0,   349,
     345,     0,     0,   374,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     583,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,   584,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,     0,     0,     0,     0,
     345,     0,     0,   374,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     753,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,     0,     0,     0,   349,   345,     0,     0,   374,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,   403,     0,     0,     0,   345,     0,     0,   374,
     770,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,   403,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,   947,     0,   434,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,   948,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   348,   371,   372,   373,   349,     0,     0,
       0,     0,   345,   808,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,   348,     0,     0,     0,   349,   345,     0,
       0,   374,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,   477,     0,     0,     0,   345,     0,
       0,   374,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   481,   371,   372,   373,   348,
       0,     0,     0,   349,     0,     0,     0,   374,   345,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   483,   371,   372,   373,   348,
       0,     0,     0,   349,     0,     0,     0,   374,   345,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   485,   371,   372,   373,   348,
       0,     0,     0,   349,     0,     0,     0,   374,   345,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   490,   371,   372,   373,   348,
       0,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
       0,     0,     0,     0,   345,     0,     0,   374,   585,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   348,     0,     0,     0,   349,
     345,   694,     0,   374,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,     0,     0,     0,   673,
     345,     0,     0,   374,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     695,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,     0,     0,     0,   349,   345,     0,     0,   374,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,  -519,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,     0,     0,     0,     0,   345,     0,     0,   374,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,   168,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,     0,     0,
       0,   349,     0,     0,     0,   374,   345,   765,     0,     0,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   738,   371,   372,   373,   348,     0,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,   768,     0,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   348,   371,   372,   373,   349,     0,     0,
       0,     0,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,   348,     0,     0,     0,   349,   345,     0,
       0,   374,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,     0,     0,     0,   792,     0,   345,
       0,   374,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,     0,
     348,     0,     0,   793,   349,   345,     0,   374,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,   884,     0,     0,     0,     0,
       0,     0,     0,     0,   811,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,   348,   371,   372,   373,
     349,     0,     0,     0,     0,   345,     0,     0,   374,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,   887,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,     0,   371,   372,   373,   348,     0,     0,     0,
     349,   345,     0,     0,   374,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,   348,   371,   372,   373,   349,     0,     0,     0,
       0,   345,   946,     0,   374,     0,     0,     0,   346,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,     0,   371,
     372,   373,   348,     0,     0,     0,   349,   345,   889,     0,
     374,     0,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   348,   371,
     372,   373,   349,     0,     0,     0,     0,   345,     0,     0,
     374,   969,     0,     0,   346,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   347,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,     0,     0,   371,   372,   373,   348,     0,
       0,     0,   349,   345,     0,     0,   374,     0,     0,     0,
     346,   972,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,   347,   364,   365,   366,   367,
     368,   369,   370,     0,   348,   371,   372,   373,   349,     0,
       0,     0,     0,     0,   345,     0,   374,     0,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,     0,   348,     0,     0,
       0,   349,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
       0,   371,   372,   373,     0,   348,     0,     0,  1026,   349,
     345,     0,   374,     0,     0,     0,     0,   346,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   347,
     360,   361,   362,     0,     0,   364,   365,   366,   367,     0,
     350,   370,     0,     0,   371,   372,   373,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,     0,     0,     0,     0,
     345,  1062,     0,   374,  1079,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,     0,     0,     0,   349,   345,     0,     0,   374,
       0,     0,     0,   346,  1080,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,     0,     0,     0,     0,   345,     0,     0,   374,
    1081,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,     0,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
    1082,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   348,   371,   372,   373,   349,     0,     0,
       0,     0,   345,     0,     0,   374,  1083,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,   348,     0,     0,     0,   349,   345,     0,
       0,   374,     0,     0,     0,   346,  1084,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1108,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,     0,     0,     0,     0,   345,     0,
       0,   374,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   348,
       0,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
       0,     0,     0,     0,   345,  1137,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,     0,   371,   372,   373,   726,     0,     0,     0,   349,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   270,   371,   372,   373,    22,    23,     0,     0,     0,
       0,     0,     0,   374,   271,     0,    30,   272,   270,     0,
       0,    35,    22,    23,     0,     0,     0,     0,    40,     0,
       0,   271,     0,    30,   272,     0,     0,     0,    35,     0,
       0,     0,     0,     0,     0,    40,     0,     0,     0,     0,
      53,     0,    55,     0,    57,     0,   952,     0,     0,   953,
       0,   273,     0,    63,     0,     0,     0,    53,     0,    55,
       0,    57,     0,   952,     0,     0,   953,     0,   273,     0,
      63,     0,    79,     0,     0,    81,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,   270,     0,     0,    79,
      22,    23,    81,     0,     0,    83,     0,     0,     0,   271,
       0,    30,   272,   270,     0,     0,    35,    22,    23,     0,
       0,     0,     0,    40,     0,     0,   271,     0,    30,   272,
       0,     0,     0,    35,     0,     0,     0,   347,    98,     0,
      40,     0,     0,     0,  1038,    53,     0,    55,     0,    57,
       0,   952,     0,     0,   953,    98,   273,     0,    63,     0,
       0,  1040,    53,     0,    55,     0,    57,     0,   952,     0,
       0,   953,     0,   273,     0,    63,   347,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,   348,
       0,     0,     0,   349,    79,     0,     0,    81,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,   348,     0,
       0,     0,   349,    98,     0,     0,     0,     0,     0,  1085,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
      98,   359,   360,   361,   362,     0,  1086,   364,   365,   366,
     367,   368,     0,   370,     0,     0,   371,   372,   373,     0,
       0,     0,     0,     0,     0,     0,     0,   374,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,     0,     0,   364,   365,   366,   367,
     368,   703,   370,   224,     0,   371,   372,   373,     0,     0,
       0,     0,     0,     0,     0,     0,   374,   225,   226,     0,
     227,     0,     0,     0,     0,   228,     0,     0,     0,     0,
       0,     0,     0,   229,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
       0,   233,     0,  -241,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,   235,   236,  -241,  -241,     0,
    -241,     0,   237,     0,     0,  -241,     0,     0,     0,     0,
       0,   238,     0,  -241,     0,     0,     0,     0,     0,  -241,
     239,   240,     0,   241,  -241,   242,     0,   243,  -241,     0,
     244,  -241,     0,   224,   245,     0,     0,   246,     0,  -241,
     247,     0,     0,     0,     0,  -241,  -241,   225,   226,     0,
     227,     0,  -241,     0,     0,   228,     0,     0,     0,     0,
       0,  -241,     0,   229,     0,     0,     0,     0,     0,   230,
    -241,  -241,     0,  -241,   231,  -241,     0,  -241,   232,     0,
    -241,   233,     0,  -232,  -241,     0,     0,  -241,     0,   234,
    -241,     0,     0,     0,     0,   235,   236,  -232,  -232,     0,
    -232,     0,   237,     0,     0,  -232,     0,     0,     0,     0,
       0,   238,     0,  -232,     0,     0,     0,     0,     0,  -232,
     239,   240,     0,   241,  -232,   242,     0,   243,  -232,     0,
     244,  -232,     0,  -163,   245,     0,     0,   246,     0,  -232,
     247,     0,     0,     0,     0,  -232,  -232,  -163,  -163,     0,
    -163,     0,  -232,     0,     0,  -163,     0,     0,     0,     0,
       0,  -232,     0,  -163,     0,     0,     0,     0,     0,  -163,
    -232,  -232,     0,  -232,  -163,  -232,     0,  -232,  -163,     0,
    -232,  -163,     0,  -159,  -232,     0,     0,  -232,     0,  -163,
    -232,     0,     0,     0,     0,  -163,  -163,  -159,  -159,     0,
    -159,     0,  -163,     0,     0,  -159,     0,     0,     0,     0,
       0,  -163,     0,  -159,     0,     0,     0,     0,     0,  -159,
    -163,  -163,     0,  -163,  -159,  -163,     0,  -163,  -159,     0,
    -163,  -159,     0,     0,  -163,     0,     0,  -163,     0,  -159,
    -163,     0,     0,     0,     0,  -159,  -159,     0,     0,     0,
       0,     0,  -159,     0,     0,     0,     0,     0,     0,     0,
       0,  -159,     0,     0,     0,     0,     0,     0,     0,     0,
    -159,  -159,     0,  -159,     0,  -159,     0,  -159,     0,     0,
    -159,     0,     0,     0,  -159,     0,     0,  -159,     0,     0,
    -159
};

static const yytype_int16 yycheck[] =
{
      12,    60,   153,   223,    16,     2,   334,   486,    20,   455,
     787,    19,    64,   194,    26,   217,   611,   700,    30,    31,
     712,    33,    34,    35,    36,     1,   699,    32,   820,   210,
      46,   139,  1003,    45,    22,    47,     1,   396,   202,   645,
      52,     1,    54,   114,    56,    32,  1048,   920,   268,   112,
     102,    32,    64,    65,    66,    67,    68,    69,     0,   223,
      32,    73,    32,    75,    46,    77,    78,    86,    52,   599,
      82,   291,   602,    85,    86,   159,    88,   140,    90,   114,
      65,    93,    94,    57,    96,   917,     1,   195,   100,   101,
     102,   103,    45,    69,    78,     1,  1098,    57,   169,   874,
     875,     3,   879,   206,   268,   114,     3,   452,   211,   114,
     140,     3,    33,    34,   442,    46,   101,  1098,    20,   725,
       3,   323,   324,    46,   140,    69,    86,   291,   723,   159,
     182,    33,   114,   114,   169,   111,   460,   149,   112,    99,
      46,    46,    57,    46,   921,   164,  1117,   298,     3,   101,
    1023,   140,   112,   140,   119,   108,  1137,    59,   140,   164,
     992,   173,   486,   955,   176,   177,   178,   111,   180,   181,
     182,    86,   392,   393,   139,   140,   165,   164,   784,   139,
      31,   102,    46,   164,    46,   193,   978,   169,   980,   165,
     202,    84,   164,   357,   164,    97,     8,   112,   644,   159,
      97,   103,   167,    18,   164,    97,   140,    86,   140,   140,
     269,   223,   742,   119,   991,   745,   140,   140,   206,   119,
      84,   165,    84,   211,   139,   159,   166,   159,   392,   393,
     218,    86,    83,   139,   140,   140,    87,   140,   169,   893,
     140,   895,    97,   167,   144,  1037,   169,  1039,    27,   164,
     202,    27,   611,   159,  1029,  1030,   268,    72,  1115,   479,
     272,    94,   164,   140,   169,   948,   169,   167,   280,   281,
    1047,   223,   140,   965,   947,    90,  1133,   140,   166,   291,
     159,   140,   159,   134,   135,   164,   137,   138,   139,   159,
     141,   142,   107,   140,    73,   146,   166,    73,   168,   140,
     159,   169,   140,   154,   167,    84,   318,   902,    84,   160,
     161,   162,    77,    78,   422,   479,   268,    27,   315,   427,
     167,   333,   334,   977,   103,   979,   167,   103,   164,   167,
     109,   343,   778,   345,   346,   347,   348,   349,   350,   291,
     352,   353,   354,   355,   356,   166,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,    73,   723,   140,    27,   119,   140,   381,
     382,   166,   700,    45,    84,   387,   140,   429,   390,   391,
     392,   393,   394,   395,   396,   119,   140,   159,   140,    61,
     165,   140,   144,   405,   140,    31,   381,   382,   410,   109,
     412,   134,   387,   167,    79,   390,   391,    79,   140,   142,
     144,   165,    73,   615,   616,   167,   159,   429,   167,   431,
     432,   167,   165,    84,   140,   437,   438,   348,   349,   441,
     442,   776,   444,   597,   446,   167,   108,    45,   140,   140,
     392,   393,   103,   159,   134,    31,   159,    83,   927,   139,
     462,    87,   142,   134,   466,   467,   146,   159,   159,    67,
     472,   142,    77,   134,   158,    80,   159,   479,   520,    77,
      78,   142,   165,   394,   395,   820,    83,    83,   680,   681,
      87,    87,   494,   159,   405,   497,   814,   499,   150,   159,
     671,   503,   504,   505,   159,   507,   508,    83,   134,   135,
     159,    87,   138,   139,   159,   141,   142,   119,   520,   159,
     146,   432,   497,   166,   499,   168,   513,   438,   154,   166,
     472,   168,   166,   164,   160,   161,   162,   479,   159,   159,
     694,   695,     3,   145,   113,   147,   148,    82,    82,   151,
     152,   166,   494,   902,   166,   497,    46,   499,   134,    20,
     166,   119,   504,   139,     2,   141,   142,    57,   159,     8,
     146,   712,    33,   166,    12,   168,   166,   166,   154,   581,
     582,   583,   584,   585,   586,   587,   162,    25,   159,    27,
     588,   589,   503,   595,   192,   597,    86,    61,    59,    57,
     166,   139,   168,   605,   164,   607,   920,   159,   650,   611,
     142,   613,    31,   927,   119,    27,   214,   215,   216,   217,
     595,   167,   112,   221,   166,   960,   168,   140,    86,   119,
     948,   159,    46,  1053,    46,    73,    97,   166,    50,    34,
     167,    99,   103,   978,   140,   980,   648,   649,   650,   497,
     140,   499,   654,   595,   112,   597,   167,   119,    70,   167,
     258,    73,   100,    75,    83,   167,   264,   105,    87,   159,
       2,   582,    84,   584,   164,    21,   169,   167,   169,   654,
      12,    67,  1102,  1103,  1104,  1105,  1106,  1107,    57,   287,
     692,   103,   140,    25,   696,    27,   698,   699,   700,   164,
     166,   159,  1037,   159,  1039,   159,   164,   305,   159,  1023,
     159,   140,   159,   159,   167,   134,   159,    86,   928,   929,
     139,   723,    34,   142,   726,   727,   728,   146,   140,   731,
      99,   169,   169,   159,  1048,    61,   167,   165,    57,   140,
      31,    73,   167,   112,   165,    77,    78,   119,  1062,   751,
      84,   753,   167,    32,   796,   167,   758,    32,   169,   761,
     164,   164,    34,   765,   139,   139,   768,    86,   100,    84,
     169,   169,   166,   105,   928,   929,   166,   169,   159,   159,
      99,   165,   142,   166,  1098,   142,   142,   134,   142,   142,
     159,   142,    83,   112,   796,   164,    87,   134,   142,   142,
     142,  1115,   142,   134,   806,   134,   808,   142,   406,   811,
     248,   134,   814,    34,   142,   726,   727,   255,   159,  1133,
     822,    99,   167,  1137,   965,   164,   164,   118,   214,   215,
     216,   217,   159,   134,   167,   159,   134,   169,   436,   169,
     159,   158,   440,   134,   135,   164,   137,   138,   139,   167,
     141,   142,   143,    27,   145,   146,   147,   148,   167,   158,
     151,   152,   153,   154,   155,   156,   157,   869,   449,   160,
     161,   162,   258,   140,   692,    49,    24,   315,   264,   299,
     171,   883,   884,   800,   886,   887,   888,   889,   330,   965,
     823,   960,   330,  1117,   637,   897,   898,    71,   900,    73,
     902,   287,   904,   905,   953,   776,   908,   826,  1047,    46,
      84,  1075,  1108,  1101,    35,   463,   248,    30,   597,   305,
      57,    -1,    -1,   255,   926,   927,   928,   929,    -1,    -1,
      -1,    -1,   934,    -1,   936,   109,    -1,    -1,   940,    -1,
      -1,    -1,    -1,    -1,   946,   947,   948,    -1,    -1,    86,
      -1,   993,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   403,    -1,   969,   970,   971,
     972,   973,   974,   975,   976,   112,    -1,    -1,   920,    46,
      -1,    -1,   119,   315,    -1,    -1,   928,   929,    -1,    -1,
      57,   993,    -1,   431,    -1,    -1,   998,    -1,   330,   437,
    1002,    -1,    -1,   140,    -1,    -1,   444,    -1,    -1,    -1,
      -1,   609,   610,     2,    -1,    -1,    -1,   615,   616,    86,
     406,    -1,   159,    12,    -1,    -1,    -1,   164,  1070,   627,
     167,    -1,    -1,    -1,    -1,    -1,    25,    -1,    27,  1041,
    1042,  1043,  1044,    -1,    -1,   112,    -1,    -1,    -1,    -1,
     436,    -1,   119,    -1,   440,    -1,   998,    -1,    -1,    -1,
    1062,    -1,    -1,   501,  1066,  1067,    -1,   505,  1070,   667,
      -1,   403,    -1,   140,    -1,   513,    -1,  1079,  1080,  1081,
    1082,  1083,  1084,    -1,    73,    -1,    -1,    -1,    -1,    31,
      -1,    -1,   159,    -1,    -1,  1097,  1098,   164,  1100,   431,
     167,    -1,    -1,    -1,    -1,   437,  1048,    -1,    -1,    -1,
      -1,   100,   444,    -1,    -1,    -1,   105,    -1,    -1,     2,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    12,
      -1,  1133,    -1,    -1,    -1,  1137,    -1,    -1,   736,   737,
     578,    83,    25,    -1,    27,    87,    -1,    -1,    -1,    -1,
     748,    -1,   750,    -1,    -1,    -1,  1098,    -1,    -1,   757,
      -1,    -1,   760,    -1,    -1,   763,    -1,    -1,    27,   501,
      -1,    -1,    -1,   505,    -1,    83,    -1,    -1,    -1,    87,
     169,   513,    -1,    -1,    -1,    -1,    -1,    46,   626,    -1,
      73,    50,   134,   135,    -1,  1137,    -1,   139,    -1,   141,
     142,    -1,    -1,    -1,   146,   803,    -1,    -1,    -1,    -1,
      -1,    70,   154,    -1,    73,    -1,    75,   100,   160,   161,
     162,    -1,   105,   609,   610,    84,   134,    -1,    -1,   615,
     616,   139,    -1,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,   679,    -1,    -1,   103,    -1,   154,   685,    -1,    -1,
      -1,    -1,   160,   161,   162,    -1,    22,    -1,    -1,   248,
      26,    27,    -1,    -1,    -1,    -1,   255,    -1,    -1,    35,
      -1,    37,    38,    -1,   712,    -1,    42,    -1,    -1,    27,
      -1,    -1,    -1,    49,    -1,    -1,   169,    -1,   726,   727,
      -1,    -1,    -1,    -1,   626,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    77,    -1,   751,    80,   753,    82,    -1,    84,    -1,
     758,    -1,    70,   761,    -1,    73,   315,    75,    -1,   767,
      -1,    -1,   770,    -1,   932,    -1,    84,   103,    -1,    -1,
     106,   330,    -1,   109,    -1,    -1,    -1,   679,    -1,    -1,
     736,   737,    -1,   685,    -1,   103,    -1,   795,    -1,    -1,
      -1,    -1,   748,    -1,    -1,   248,    -1,    -1,   806,    -1,
     808,    -1,   255,    -1,   760,    -1,    -1,   763,    -1,    -1,
      -1,    -1,    -1,    -1,   982,   983,     2,    -1,   986,   987,
      -1,    -1,    -1,   159,    -1,    -1,    12,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,  1003,    -1,    -1,    -1,    25,
      -1,    27,    -1,  1011,   403,  1013,    -1,  1015,    -1,   751,
      -1,   753,    -1,    -1,    -1,    -1,   758,    -1,    -1,   761,
      -1,    -1,   315,    -1,    -1,   767,    -1,    -1,   770,    -1,
      -1,    -1,   431,    -1,    -1,    -1,    -1,   330,   437,    -1,
      -1,    -1,    -1,    -1,    -1,   444,    -1,    73,    -1,   897,
      -1,    -1,     1,   795,    -1,    -1,   904,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   806,    -1,   808,   915,    -1,    -1,
      -1,    -1,    -1,    22,   100,    -1,    -1,    26,    27,   105,
      -1,    -1,   930,    -1,    -1,    -1,    35,    -1,    37,    38,
      -1,   939,    -1,    42,    -1,   943,    -1,    -1,    -1,    -1,
      49,    -1,   501,    52,    -1,    -1,   505,    -1,    -1,  1117,
     403,    -1,    -1,    -1,   513,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    77,    78,
      -1,    80,    -1,    82,    -1,    84,   932,    -1,   431,    -1,
     988,    -1,    -1,   169,   437,    -1,    -1,    -1,    -1,    -1,
      -1,   444,    -1,    -1,   103,   897,    -1,   106,    -1,    -1,
     109,    -1,   904,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    -1,   915,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   930,    -1,
      25,    -1,    27,  1041,  1042,  1043,  1044,   939,    -1,    -1,
      -1,   943,    -1,    -1,    -1,    -1,    -1,    -1,   501,    -1,
     159,    -1,   505,    -1,    -1,    -1,   165,    -1,  1066,  1067,
     513,    -1,   248,    -1,    -1,    -1,    -1,   626,    -1,   255,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   988,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,   100,    20,    -1,    -1,    -1,
     105,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     679,    -1,    -1,    -1,    -1,    39,   685,    -1,    -1,   315,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,  1041,
    1042,  1043,  1044,    -1,   330,    59,    -1,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    31,  1066,  1067,    -1,    81,    -1,    -1,
      -1,    -1,    -1,   626,   169,    -1,    90,    91,    -1,    93,
      -1,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    42,   751,   107,   753,    -1,   110,    -1,    -1,   758,
      -1,    -1,   761,    -1,    -1,    -1,     2,    -1,   767,    -1,
      -1,   770,    -1,    -1,    -1,    83,    12,   403,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,   679,    -1,    79,    25,
      -1,    27,   685,    -1,    -1,    -1,   795,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   431,    -1,   806,    -1,   808,
     164,   437,   166,   248,    -1,    -1,    -1,    -1,   444,    -1,
     255,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,    73,   146,    -1,
      -1,   132,    -1,   134,    31,   153,   154,    -1,    -1,   157,
      -1,    -1,   160,   161,   162,    -1,    -1,    -1,   751,    -1,
     753,    -1,    -1,   171,   100,   758,    -1,    -1,   761,   105,
      -1,    -1,    -1,    -1,   767,   501,    -1,   770,    -1,   505,
     315,    -1,    -1,    -1,    -1,    -1,    -1,   513,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   330,    83,    -1,   897,    -1,
      87,    -1,   795,   194,    -1,   904,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   806,    -1,   808,   915,    -1,    -1,   210,
      -1,   212,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   930,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
     939,    -1,    -1,    -1,   943,    -1,    -1,   134,   135,    -1,
     137,   138,   139,     2,   141,   142,    -1,    -1,    -1,   146,
     251,    -1,    -1,    12,    -1,    -1,   153,   154,   403,    -1,
     157,    -1,    -1,   160,   161,   162,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,   988,
      -1,     3,    -1,    -1,    -1,    -1,   431,    -1,    -1,    -1,
     626,    -1,   437,    -1,   897,    17,    -1,    -1,    20,   444,
      -1,   904,   248,    25,    -1,    -1,    -1,    -1,    -1,   255,
      -1,    -1,   915,    35,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,   930,    -1,    51,
      -1,    -1,  1041,  1042,  1043,  1044,   939,    59,    -1,    -1,
     943,   100,    -1,   679,    66,    -1,   105,    -1,    -1,   685,
      -1,    -1,    -1,    -1,    -1,    -1,   501,  1066,  1067,    81,
     505,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   513,   315,
      -1,    93,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,     2,   104,    -1,   330,   988,    -1,    -1,   110,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,   751,    -1,   753,    -1,   420,
      -1,    -1,   758,   424,    -1,   761,    -1,    -1,    -1,    -1,
      -1,   767,    -1,    -1,   770,    -1,    -1,    -1,  1041,  1042,
    1043,  1044,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,   455,    -1,    -1,   403,    -1,   795,
      -1,    -1,    -1,  1066,  1067,    -1,    -1,    -1,    -1,    -1,
     806,    -1,   808,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,   626,    -1,    -1,   105,   431,    -1,    -1,     3,   248,
      -1,   437,    -1,    -1,    -1,    -1,   255,    -1,   444,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,   514,    -1,    -1,    -1,    -1,    33,    -1,
      -1,   522,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    48,   679,    -1,    51,    -1,    -1,    -1,
     685,    -1,    -1,    -1,    59,    -1,    -1,    -1,   169,    -1,
      65,    66,    -1,    -1,    -1,   501,   315,    72,    -1,   505,
      -1,   897,    -1,    -1,    -1,    -1,    81,   513,   904,    -1,
      -1,   330,    -1,    -1,    -1,    90,    91,    -1,    93,   915,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
     105,    -1,   107,    -1,   930,   110,    -1,   598,    -1,    -1,
     601,    -1,    -1,   939,    -1,    -1,   751,   943,   753,    -1,
      -1,    -1,    -1,   758,    -1,    -1,   761,    -1,    -1,    -1,
      -1,    -1,   767,    -1,    -1,   770,    -1,   248,    -1,    -1,
      -1,    -1,    -1,    -1,   255,    -1,    -1,    -1,    -1,   640,
      -1,    -1,    22,   644,   403,    -1,    26,    27,    -1,    -1,
     795,   166,   988,    -1,    -1,    35,    -1,    37,    38,    -1,
      -1,   806,    42,   808,    -1,    -1,   667,    -1,    -1,    49,
     671,    -1,   431,    -1,    -1,    -1,    -1,    -1,   437,    -1,
     626,    -1,    -1,    -1,    -1,   444,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    73,   315,    75,    -1,    77,    -1,    -1,
      80,    -1,    82,    -1,    84,  1041,  1042,  1043,  1044,   330,
      -1,   712,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,
    1066,  1067,    -1,   679,    -1,    -1,    -1,    -1,    -1,   685,
      -1,    -1,   501,    -1,    -1,    -1,   505,    -1,    -1,    -1,
      -1,    -1,   897,    -1,   513,    -1,    -1,    -1,    -1,   904,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     915,   772,    -1,    -1,   775,    -1,    -1,   778,     3,   159,
      -1,    -1,   403,    -1,    -1,   930,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,   939,    20,    -1,    -1,   943,   800,
      25,    -1,    -1,    -1,    -1,   751,    -1,   753,    33,    -1,
     431,    -1,   758,    -1,    39,   761,   437,    -1,    -1,    44,
      -1,   767,   823,    48,   770,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      65,    66,    -1,   988,    -1,    -1,    -1,    72,   849,   795,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
     806,    -1,   808,    -1,    -1,    90,    91,   626,    93,    -1,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
     501,    -1,   107,    -1,   505,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   513,    -1,    -1,    -1,  1041,  1042,  1043,  1044,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
     679,  1066,  1067,    -1,    -1,    -1,   685,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,   163,    -1,    -1,   166,    -1,   168,
      -1,   897,    -1,    -1,    -1,    -1,    -1,    -1,   904,    -1,
      -1,    -1,    -1,    -1,   965,     3,    -1,    -1,    -1,   915,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,   930,    -1,    -1,    25,    -1,    -1,
      -1,    -1,   751,   939,   753,    33,    -1,   943,    -1,   758,
      -1,    39,   761,    -1,    -1,   626,    44,    -1,   767,    -1,
      48,   770,    -1,    51,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    65,    66,    17,
      18,    -1,    20,    -1,    72,    -1,   795,    25,    -1,    -1,
      -1,    -1,   988,    81,    -1,    33,    -1,   806,    -1,   808,
      -1,    39,    90,    91,    -1,    93,    44,    95,   679,    97,
      48,    -1,   100,    51,   685,    -1,   104,    -1,    -1,   107,
      -1,    59,   110,    -1,    -1,    -1,    -1,    65,    66,    -1,
      -1,    -1,    12,    -1,    72,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    81,    -1,  1041,  1042,  1043,  1044,    -1,
      -1,    31,    90,    91,    -1,    93,    -1,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
    1066,  1067,   110,    -1,    -1,    -1,    -1,    -1,   166,    -1,
     751,    -1,   753,    -1,    -1,    -1,    -1,   758,   897,    -1,
     761,    -1,    -1,    -1,    -1,   904,   767,    -1,    -1,   770,
      -1,    -1,    -1,    83,    -1,    -1,   915,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,
      -1,   930,    -1,    -1,   795,    -1,    -1,    -1,    -1,    -1,
     939,    -1,    -1,    -1,   943,   806,    -1,   808,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,   988,
     160,   161,   162,    -1,   164,    -1,    -1,    -1,    -1,    22,
      -1,   171,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1041,  1042,  1043,  1044,    -1,    -1,    71,    -1,
      73,    -1,    -1,    -1,   915,    78,    -1,    -1,    -1,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,  1066,  1067,   930,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   939,    -1,
     103,    -1,   943,   106,    -1,   108,   109,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,   988,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    97,    98,
      -1,   100,   101,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
     159,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     1,   171,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    97,    98,    -1,
     100,   101,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,    -1,   168,    -1,
       1,   171,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    97,    98,    -1,   100,
     101,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,
      -1,   162,    -1,   164,   165,   166,    -1,   168,    -1,     1,
     171,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    97,    98,    -1,   100,   101,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,
     162,    -1,   164,   165,   166,    -1,   168,    -1,     1,   171,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    97,    98,    -1,   100,   101,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,
      -1,   164,   165,   166,    -1,   168,    -1,     1,   171,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    -1,   100,   101,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,
     164,   165,   166,    -1,   168,    -1,     1,   171,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    98,    -1,   100,   101,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,   164,
     165,   166,    -1,   168,    -1,     1,   171,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    -1,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    -1,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    97,    98,    -1,   100,   101,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,   159,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     1,   171,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      97,    98,    -1,   100,   101,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     1,   171,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    -1,    84,    85,    86,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    97,
      98,    -1,   100,   101,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,   159,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,     1,   171,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    -1,    51,    52,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    67,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    81,    82,    -1,    84,    85,    86,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    97,    98,
      -1,   100,   101,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
     159,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     1,   171,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    -1,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    47,    48,    49,
      -1,    51,    52,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    -1,    84,    85,    86,    -1,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    97,    98,    -1,
     100,   101,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
       1,   171,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    -1,
      51,    52,    -1,    54,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      81,    82,    -1,    84,    85,    86,    -1,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    97,    98,    -1,   100,
     101,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,    -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,
     171,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    -1,    84,    85,    86,    -1,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    97,    98,    -1,   100,   101,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,   171,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    -1,    51,    52,
      -1,    54,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      -1,    84,    85,    86,    -1,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    97,    98,    -1,   100,   101,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,    -1,
     113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     1,   171,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    47,    48,    49,    -1,    51,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    62,    63,
      -1,    65,    66,    67,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    81,    82,    -1,
      84,    85,    86,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    98,    -1,   100,   101,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,    -1,   113,
      -1,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     1,   171,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    -1,    51,    52,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    -1,    84,
      85,    86,    -1,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    97,    98,    -1,   100,   101,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,    -1,   113,    -1,
     115,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,   159,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,     1,   171,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      -1,    47,    48,    49,    -1,    51,    52,    -1,    54,    55,
      56,    -1,    58,    59,    60,    61,    62,    63,    -1,    65,
      66,    67,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    -1,    84,    85,
      86,    -1,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    97,    98,    -1,   100,   101,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,    -1,   113,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,   159,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     1,   171,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    22,    -1,    -1,   104,    26,    27,
     107,    -1,    -1,   110,    -1,    -1,    -1,    35,    -1,    37,
      38,   118,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    73,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    82,   162,    84,   164,    -1,   166,
      -1,   168,   169,     1,   171,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,   103,    14,    -1,   106,    17,
      18,   109,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,   109,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,    -1,
     171,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,   101,
     102,    -1,   104,   105,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,    -1,   171,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,   101,   102,
      -1,   104,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,   167,   168,    -1,    -1,   171,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,
     104,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,   167,   168,    -1,     3,   171,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    46,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,    -1,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,   159,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
      -1,   171,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,
     171,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     3,   171,     5,     6,
       7,     8,     9,    10,    11,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,   103,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
      -1,   171,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,
     171,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,    -1,   171,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
     165,   166,    -1,   168,    -1,    -1,   171,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,   169,    -1,   171,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,   169,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
       3,   171,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,    -1,   171,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     3,   171,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,   103,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,   169,
      -1,   171,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,    -1,
     171,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,    -1,   171,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,     3,   171,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,
     171,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,
      44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,
     104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,
     164,    -1,   166,    -1,   168,    -1,     3,   171,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,    -1,     3,   171,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,
      40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    94,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,    -1,
      -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,
       3,   171,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,   162,
      -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,     3,   171,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,
      -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,
     158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,
     168,    -1,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,   134,   135,   136,   137,   138,    -1,    -1,
      -1,    12,   143,    -1,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,   153,    -1,    25,   156,   157,   158,    -1,    -1,
      -1,   162,    33,    -1,    35,   166,    -1,   168,    39,    -1,
     171,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    93,    -1,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,   123,   124,    -1,    -1,   127,   128,   129,   130,
      -1,   132,   133,   134,   135,   136,   137,   138,    -1,    -1,
     141,    -1,    -1,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    -1,   157,    -1,    -1,   160,
     161,   162,   163,    -1,     3,   166,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   163,   157,   158,
     166,    -1,   168,   162,    -1,   164,    -1,   166,    -1,   168,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    -1,    -1,   140,    -1,    -1,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,    -1,
      -1,   164,    -1,   166,   167,   168,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    76,
      -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    90,    91,    92,    93,    94,    95,    -1,
      97,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,    -1,
     107,    -1,    -1,   110,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,   136,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,   158,   159,    -1,    -1,    -1,    -1,   164,    33,   166,
      35,   168,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
       3,   136,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   158,    -1,    -1,    -1,    -1,    -1,   164,
      33,   166,    35,   168,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   136,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   158,    -1,    -1,    -1,    -1,
      -1,   164,    33,   166,    35,   168,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,     3,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,   136,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    65,    66,   166,    -1,   168,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    93,    12,    95,    -1,    97,    -1,    -1,   100,    19,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,   123,   124,    -1,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,   141,
      -1,    -1,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,   157,    -1,    -1,   160,   161,
     162,   163,    -1,    83,    -1,    -1,    -1,    87,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,   164,    -1,    -1,    -1,    12,    -1,
      -1,   171,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,   127,   128,   129,   130,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    83,
     164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,    -1,    -1,   127,   128,   129,   130,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
     164,    -1,    -1,    -1,    12,    -1,    -1,   171,    96,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,    -1,    -1,   127,
     128,   129,   130,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    83,   164,    -1,    -1,    87,
      12,    -1,    -1,   171,    -1,    -1,    -1,    19,    96,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,   119,   120,   121,   122,   123,   124,    -1,    -1,   127,
     128,   129,   130,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,   164,    -1,    -1,    -1,
      12,    -1,    -1,   171,    96,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    83,   164,    -1,    -1,    87,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,   127,   128,   129,   130,    -1,
     132,   133,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    83,   160,   161,
     162,    87,   164,    -1,    -1,    -1,    12,    -1,    -1,   171,
      96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,    -1,
      46,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,   164,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    83,   160,   161,   162,    87,   164,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    68,    -1,
     160,   161,   162,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    68,   159,
     160,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    83,    -1,    -1,    -1,    87,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    83,
     164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
      -1,    -1,    -1,    -1,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    83,   164,    -1,    -1,    87,
      12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,
      12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    83,    -1,    -1,    -1,    87,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    83,   160,   161,
     162,    87,   164,    -1,    -1,    -1,    12,    -1,    -1,   171,
      96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,   164,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    68,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    83,   160,   161,   162,    87,    -1,    -1,
      -1,    -1,    12,   169,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,   164,    -1,    -1,    -1,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,   159,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   171,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,   159,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   171,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,   159,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   171,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,   159,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
      -1,    -1,    -1,    -1,    12,    -1,    -1,   171,    96,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,
      12,    13,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,   167,
      12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    83,    -1,    -1,    -1,    87,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,    83,   160,   161,
     162,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,   171,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,   159,   160,   161,   162,    83,    -1,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    83,   160,   161,   162,    87,    -1,    -1,
      -1,    -1,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,    -1,    -1,    -1,   167,    -1,    12,
      -1,   171,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    -1,
      83,    -1,    -1,   167,    87,    12,    -1,   171,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    83,   160,   161,   162,
      87,    -1,    -1,    -1,    -1,    12,    -1,    -1,   171,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,
      87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,
      -1,    12,    13,    -1,   171,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    -1,   160,
     161,   162,    83,    -1,    -1,    -1,    87,    12,   169,    -1,
     171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    83,   160,
     161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,
     171,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    -1,    -1,   160,   161,   162,    83,    -1,
      -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,
      19,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    31,   151,   152,   153,   154,
     155,   156,   157,    -1,    83,   160,   161,   162,    87,    -1,
      -1,    -1,    -1,    -1,    12,    -1,   171,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    -1,    83,    -1,    -1,   167,    87,
      12,    -1,   171,    -1,    -1,    -1,    -1,    19,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,    31,
     146,   147,   148,    -1,    -1,   151,   152,   153,   154,    -1,
     118,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,
      12,   169,    -1,   171,    96,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    83,    -1,    -1,    -1,    87,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    83,   160,   161,
     162,    87,    -1,    -1,    -1,    -1,    12,    -1,    -1,   171,
      96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    83,   160,   161,   162,    87,    -1,    -1,
      -1,    -1,    12,    -1,    -1,   171,    96,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,    -1,    -1,    -1,    -1,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
      -1,    -1,    -1,    -1,    12,   169,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    22,   160,   161,   162,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    35,    -1,    37,    38,    22,    -1,
      -1,    42,    26,    27,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      71,    -1,    73,    -1,    75,    -1,    77,    -1,    -1,    80,
      -1,    82,    -1,    84,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    77,    -1,    -1,    80,    -1,    82,    -1,
      84,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,   103,
      26,    27,   106,    -1,    -1,   109,    -1,    -1,    -1,    35,
      -1,    37,    38,    22,    -1,    -1,    42,    26,    27,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    31,   159,    -1,
      49,    -1,    -1,    -1,   165,    71,    -1,    73,    -1,    75,
      -1,    77,    -1,    -1,    80,   159,    82,    -1,    84,    -1,
      -1,   165,    71,    -1,    73,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    82,    -1,    84,    31,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,   103,    -1,    -1,   106,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    83,    -1,
      -1,    -1,    87,   159,    -1,    -1,    -1,    -1,    -1,   165,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
     159,   145,   146,   147,   148,    -1,   165,   151,   152,   153,
     154,   155,    -1,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,    -1,    -1,   151,   152,   153,   154,
     155,     1,   157,     3,    -1,   160,   161,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      -1,    51,    -1,     3,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,
      20,    -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      90,    91,    -1,    93,    44,    95,    -1,    97,    48,    -1,
     100,    51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,
     110,    -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,
      20,    -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      90,    91,    -1,    93,    44,    95,    -1,    97,    48,    -1,
     100,    51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,
     110,    -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,
      20,    -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      90,    91,    -1,    93,    44,    95,    -1,    97,    48,    -1,
     100,    51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,
     110,    -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,
      20,    -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      90,    91,    -1,    93,    44,    95,    -1,    97,    48,    -1,
     100,    51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,
     110,    -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,
      20,    -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,
      90,    91,    -1,    93,    44,    95,    -1,    97,    48,    -1,
     100,    51,    -1,    -1,   104,    -1,    -1,   107,    -1,    59,
     110,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    93,    -1,    95,    -1,    97,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,
     110
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   176,   177,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    32,    33,    35,
      37,    38,    39,    40,    41,    42,    43,    44,    47,    48,
      49,    51,    54,    55,    56,    58,    59,    60,    62,    63,
      65,    66,    67,    71,    72,    73,    74,    75,    76,    77,
      80,    81,    82,    84,    85,    86,    88,    89,    90,    91,
      92,    93,    94,    95,    97,    98,   100,   101,   102,   103,
     104,   106,   107,   109,   110,   113,   115,   118,   134,   135,
     136,   137,   138,   143,   153,   156,   157,   158,   159,   162,
     164,   166,   168,   171,   178,   179,   180,   181,   182,   183,
     186,   188,   193,   194,   195,   198,   199,   203,   205,   208,
     209,   211,   212,   213,   218,   219,   221,   222,   223,   227,
     228,   231,   232,   235,   236,   239,   242,   243,   268,   271,
     272,   292,   293,   294,   295,   296,   297,   298,   306,   307,
     308,   309,   310,   313,   314,   315,   316,   317,   318,   319,
     321,   322,   323,   324,   325,   159,   180,   310,   114,   299,
     300,     3,   200,    14,    22,    35,    40,    41,    43,    82,
      95,   164,   168,   239,   292,   297,   308,   309,   310,   313,
     315,   316,   299,   310,   103,   271,    84,   200,   180,   286,
     310,   180,   166,     8,    82,   310,   311,     8,    11,    82,
     103,   311,    73,   116,   220,   310,   220,   310,    26,   272,
     310,   310,   187,   166,     3,    17,    18,    20,    25,    33,
      39,    44,    48,    51,    59,    65,    66,    72,    81,    90,
      91,    93,    95,    97,   100,   104,   107,   110,   202,   204,
     240,   166,   202,   273,   274,    32,   188,   207,   310,   310,
      18,    72,    90,   107,   310,   310,   310,     8,   166,   210,
      22,    35,    38,    82,   211,     4,   158,   262,   285,   310,
     101,   102,   159,   310,   312,   310,   207,   310,   310,   310,
      94,   166,   180,   310,   310,   188,   199,   310,   313,   188,
     199,   310,   202,   269,   310,   310,   310,   310,   310,   310,
     310,   310,     1,   165,   178,   189,   285,   105,   144,   262,
     287,   288,   312,   220,   285,   310,   320,   310,    75,   180,
     164,    79,   184,    45,   108,   202,   202,    52,    78,   257,
     273,   158,   159,   150,   310,    12,    19,    31,    83,    87,
     118,   134,   135,   137,   138,   139,   141,   142,   143,   145,
     146,   147,   148,   149,   151,   152,   153,   154,   155,   156,
     157,   160,   161,   162,   171,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     163,   249,   166,   168,    83,    87,   166,   180,   159,   310,
     310,   310,   285,   164,   188,    46,   299,   269,   273,   159,
     140,   159,   113,   203,   262,   289,   290,   291,   312,    82,
     216,   243,   271,    82,   214,   269,   243,   271,   202,   166,
     207,    32,    46,   207,   114,   207,   302,    32,    46,   207,
     302,    36,    68,   159,    96,   188,   249,    77,    80,   185,
     289,   180,   166,   251,   105,   166,   202,   275,   276,     1,
     139,   280,    46,   140,   180,   207,   166,   166,   207,   289,
     211,   202,   140,   159,   310,   310,   159,   164,   207,   166,
     289,   159,   224,   159,   224,   159,   119,   270,   159,   207,
     159,   165,   165,   178,   140,   165,   310,   140,   167,   140,
     167,   169,   302,    46,   140,   169,   302,   117,   140,   169,
       8,     1,   165,   189,    61,   196,   197,   310,   190,   310,
     139,   233,   164,   244,   159,   310,   310,   310,   310,   220,
     310,   220,   310,   310,   310,   310,   310,   310,   310,    20,
      33,    59,   103,   203,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     310,   312,   312,   312,   312,   312,   289,   289,   220,   310,
     220,   310,    27,    46,    84,   109,   301,   304,   305,   310,
     325,    32,    46,    32,    46,    96,    46,   169,   220,   310,
     207,   159,   159,   310,   310,   119,   167,   140,   217,   202,
     273,   215,   202,   159,   273,    46,   285,    43,   310,   220,
     310,   166,   207,    43,   310,   220,   310,   207,   162,   190,
     192,   310,   190,   191,   180,   310,    34,   310,   184,   167,
      27,    46,    50,    70,    73,    84,   103,   179,   252,   253,
     254,   255,   241,   276,   140,   167,    33,    48,    91,    95,
     168,   206,   281,   293,   119,   277,   310,   274,   310,   310,
     167,   262,   310,     1,   229,   289,   167,    21,   225,   281,
     293,   140,   165,   167,   167,   287,   167,   287,   180,   169,
     220,   310,   169,   180,   310,   169,   310,   169,   310,   165,
     165,   202,   140,   159,    13,   142,   140,   159,    13,    36,
      68,   285,   164,     1,   202,   237,   238,    27,    73,    84,
     103,   246,   256,   166,   159,   159,   159,   159,   159,   167,
     169,    46,    84,   140,   167,   292,    83,    83,    43,   220,
     310,    43,   220,   310,   220,   310,   299,   299,   159,   262,
     312,   291,   202,   233,   159,   202,   233,   159,   310,   167,
     310,    32,   207,    32,   207,   303,   304,   310,    32,   207,
     302,    32,   207,   302,   159,    13,   159,    34,    34,   180,
      96,   188,    61,    46,    84,   254,   140,   167,   166,   202,
      27,    73,    84,   103,   258,   167,   276,   280,     1,   285,
      64,   312,   167,   167,   165,    69,   111,   165,   230,   167,
     166,   188,   202,   226,   269,   180,   169,   302,   169,   302,
     180,   117,   196,   203,   164,   310,   310,   192,   191,     1,
     234,   165,   119,   140,   165,    84,   245,     1,     3,    12,
      17,    19,    20,    25,    39,    44,    51,    53,    59,    65,
      66,    81,    93,    97,   100,   104,   110,   134,   135,   136,
     137,   138,   141,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   157,   160,   161,   162,   163,   166,
     201,   202,   204,   247,   248,   249,   292,   167,   304,   280,
     292,   292,   310,    32,    32,   310,    32,    32,   169,   169,
     207,   207,   233,   164,   233,   164,   207,    96,    43,   310,
      43,   310,   140,   167,    96,    43,   310,   207,    43,   310,
     207,   310,   180,   310,   180,    34,   202,   202,   253,   276,
     139,   284,    84,   280,   277,   169,    46,   169,   166,   166,
      32,   180,   285,   226,   139,   188,    43,   180,   310,   169,
      43,   180,   310,   169,   310,   190,    13,    36,    68,   159,
     159,   165,    77,    80,   165,   179,   209,   310,   238,   258,
     166,   250,   202,   310,   134,   142,   250,   250,   277,    96,
      43,    43,    96,    43,    43,    43,    43,   164,   234,   164,
     234,   310,   310,   310,   304,   310,   310,   310,    34,   180,
     159,   284,   167,   168,   206,   262,   283,   293,   144,   263,
     277,    57,   112,   264,   310,   281,   293,   289,   289,   180,
     207,   167,   310,   310,   180,   310,   180,   165,   310,   192,
     191,   209,     1,   139,   279,   252,   167,     3,    97,   248,
     249,   310,   310,   310,   310,   310,   310,   234,   165,   234,
     165,    96,    96,    96,    96,   180,   277,   284,   169,   285,
     262,   310,     3,    86,    97,   265,   266,   267,   310,   188,
     208,   261,   169,   167,   167,   188,    96,    96,   159,   159,
     168,   206,   278,   293,    99,   259,   167,   250,   250,    96,
      96,    96,    96,    96,    96,   165,   165,   310,   310,   310,
     310,   263,   277,   262,   282,   283,   293,    46,   169,   267,
     112,   140,   119,   145,   147,   148,   151,   152,    57,   293,
     158,   158,   310,   310,     1,   169,   285,   264,   310,   282,
     283,   310,   266,   267,   267,   267,   267,   267,   267,   265,
     169,   278,   293,   169,   159,   260,   261,   169,   278,   293,
     282
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   175,   176,   177,   177,   178,   178,   179,   179,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   181,   182,   182,   182,   183,   183,   183,   184,   184,
     185,   185,   185,   187,   186,   188,   188,   188,   189,   189,
     190,   190,   190,   190,   191,   191,   192,   192,   193,   193,
     193,   194,   194,   194,   194,   194,   195,   196,   196,   196,
     197,   197,   198,   199,   199,   199,   199,   199,   200,   200,
     201,   201,   201,   201,   201,   201,   202,   202,   202,   202,
     202,   202,   203,   203,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   206,   206,   206,   206,   207,
     207,   208,   208,   209,   209,   209,   210,   209,   211,   211,
     211,   211,   211,   211,   211,   212,   212,   212,   212,   214,
     213,   215,   213,   216,   213,   217,   213,   213,   213,   213,
     213,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   220,   221,
     221,   221,   221,   221,   221,   221,   221,   222,   223,   223,
     223,   223,   223,   223,   224,   224,   225,   225,   225,   226,
     226,   227,   228,   228,   229,   229,   230,   230,   230,   231,
     231,   232,   232,   232,   233,   233,   234,   234,   234,   235,
     235,   236,   237,   237,   237,   238,   238,   240,   241,   239,
     242,   242,   242,   244,   245,   243,   246,   246,   246,   246,
     246,   247,   247,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   250,   250,   251,   252,   252,   252,   253,
     253,   253,   253,   253,   254,   254,   255,   255,   255,   255,
     255,   255,   255,   255,   255,   256,   256,   256,   256,   256,
     256,   257,   257,   258,   258,   258,   258,   258,   258,   259,
     259,   260,   260,   261,   261,   262,   262,   263,   263,   263,
     264,   264,   264,   264,   264,   265,   265,   266,   266,   266,
     266,   266,   266,   266,   267,   267,   268,   268,   268,   269,
     269,   270,   270,   270,   271,   271,   271,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   275,   275,   276,   276,
     276,   277,   277,   277,   278,   278,   278,   278,   278,   278,
     278,   279,   279,   279,   279,   279,   280,   280,   280,   280,
     280,   281,   281,   281,   281,   282,   282,   282,   283,   283,
     283,   283,   283,   284,   284,   284,   284,   284,   285,   285,
     285,   285,   286,   286,   287,   287,   287,   288,   288,   289,
     289,   290,   290,   291,   291,   291,   291,   292,   292,   293,
     293,   293,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   296,   297,   298,   298,   298,   298,   298,   298,   298,
     298,   299,   299,   300,   301,   301,   302,   303,   303,   304,
     304,   304,   305,   305,   305,   305,   305,   305,   306,   306,
     307,   307,   307,   307,   307,   308,   308,   308,   308,   308,
     308,   308,   309,   310,   310,   310,   310,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   311,
     311,   312,   312,   312,   313,   313,   313,   313,   314,   314,
     314,   314,   315,   315,   315,   316,   316,   316,   316,   316,
     317,   317,   317,   317,   318,   318,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     320,   320,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   322,   322,   322,   322,   322,
     322,   322,   323,   323,   323,   323,   324,   324,   324,   324,
     325,   325,   325,   325,   325,   325,   325
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     3,     2,     2,     3,
       2,     4,     3,     4,     4,     0,     1,     1,     0,     1,
       0,     1,     1,     0,     7,     2,     3,     3,     1,     2,
       1,     3,     3,     5,     0,     1,     1,     1,     0,     1,
       1,     4,     6,     8,     6,     8,     4,     1,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     0,     1,
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
       1,     4,     3,     3,     3,     2,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
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
#line 506 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 6237 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 511 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6243 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 512 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6249 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 519 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6255 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 525 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6267 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6273 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6279 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6285 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6291 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6297 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6303 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 552 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6309 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6315 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6321 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 555 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6327 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 556 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6333 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 557 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 558 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 559 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6351 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6357 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6363 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 566 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pmodsymbol) = buildModule((yyvsp[0].pch), currentModuleType, NULL, yyfilename, (yyvsp[-3].b), (yyvsp[-2].b), (yylsp[-3]).comment);
      // store previous module name in order to restore it once we're
      // done with this module in module_decl_stmt below.  Ultimately,
      // we will need to store a stack of module names in order to
      // support full module path resolution of -s config flags.
      (yyloc).prevModule = currentModuleName;
      currentModuleName = (yyvsp[0].pch);
    }
#line 6377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 584 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 589 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6404 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6410 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6422 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 602 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6428 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6434 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6440 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6446 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6452 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6461 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 634 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 635 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6481 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 636 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6493 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 59:
#line 642 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6499 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 647 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6509 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 652 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 657 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 661 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6547 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 677 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6557 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 686 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = (currentModuleType != MOD_INTERNAL ? true : false); }
#line 6563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 687 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6569 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 688 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6575 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6581 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 694 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6593 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6599 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 706 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 6630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 711 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 712 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 725 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 736 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 743 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 744 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6732 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 749 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 753 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6762 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6768 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6774 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6780 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6786 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6792 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 784 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6846 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 790 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6876 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 794 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 799 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 800 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 6900 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 801 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 805 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 807 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 809 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 810 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 6978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 6984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 847 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 861 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 863 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 869 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7065 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 874 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 884 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 889 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 900 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7112 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 905 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7126 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 915 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7135 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 920 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7149 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 931 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 935 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7165 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7177 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 948 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7189 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 959 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7197 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 965 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7203 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 966 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 967 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7215 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 968 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7221 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 969 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7227 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 970 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7233 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 971 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7239 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 972 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7245 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 973 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7251 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 974 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7257 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 975 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7263 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 976 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7269 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 977 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7275 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 978 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7281 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 979 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7287 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 980 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7293 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 981 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7299 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 982 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 984 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7315 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 990 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 996 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7335 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1002 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1008 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7356 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1015 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1022 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7375 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7383 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7389 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1037 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1038 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1039 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7413 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1040 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1042 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1044 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7434 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1046 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7441 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1051 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7447 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1054 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7453 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1055 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7459 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1056 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1057 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7471 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1058 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7477 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7483 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1063 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7489 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1064 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7495 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1068 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7501 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1069 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1070 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7513 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1074 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1075 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7531 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1083 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1085 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7543 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7549 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1090 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7555 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7561 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1097 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7567 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1099 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7573 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1106 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1116 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7601 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1129 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7611 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1134 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1139 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7631 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7637 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7643 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7649 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1155 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7655 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1157 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1162 "chapel.ypp" /* yacc.c:1663  */
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
#line 7681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1178 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7689 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1185 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1194 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1202 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1206 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1212 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1213 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1218 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7748 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1223 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7757 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1228 "chapel.ypp" /* yacc.c:1663  */
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
#line 7779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1250 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 7787 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1253 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 7796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1257 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 7805 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1265 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1275 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1282 "chapel.ypp" /* yacc.c:1663  */
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
#line 7857 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1308 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7874 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1317 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1321 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1326 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7899 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1333 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7905 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1337 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7911 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1338 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 7917 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1339 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7923 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7929 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1341 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7935 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1342 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7941 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1343 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7947 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1344 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7953 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1345 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7959 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1346 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7971 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1348 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1349 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 7983 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1350 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 7989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1351 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 7995 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1352 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 8001 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1353 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 8007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 8013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1355 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 8019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 8025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1357 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 8031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1358 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 8037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1359 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 8043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1360 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 8049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1361 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 8055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1362 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 8061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1363 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 8067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1364 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1368 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 8079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1369 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 8085 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1370 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 8091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1371 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 8097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1372 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 8103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1373 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 8109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 8115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1375 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 8121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 8127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1377 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 8133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 8139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1379 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 8145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1401 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1403 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1411 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1412 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 8223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1416 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 8229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1417 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 8235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1418 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 8241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1419 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 8247 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1420 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 8253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1421 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 8259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1422 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1423 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 8271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1424 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 8277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1428 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8283 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1429 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1430 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 8295 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1431 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 8301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1432 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 8307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1433 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 8313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1437 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 8319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1438 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 8325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1442 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1443 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1444 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 8343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1445 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 8349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1446 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 8355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1447 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 8361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1451 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 8367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1452 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 8373 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1455 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 8379 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1461 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    { 
      DefExpr * de = new DefExpr(new VarSymbol((yyvsp[0].pch)));
      USR_WARN(de, "support for space in query expressions is deprecated");
      (yyval.pexpr) = de;
  }
#line 8395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1470 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1474 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1475 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8413 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1476 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8423 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1484 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8429 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1486 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8435 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1488 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8441 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1490 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8447 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1492 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8453 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1497 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8459 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1499 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1503 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8471 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1504 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8477 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1505 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8483 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1506 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8489 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1507 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8495 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1508 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8501 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1509 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1514 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1516 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8519 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1521 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8525 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1523 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8531 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1525 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8554 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1543 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1559 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8584 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8590 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1568 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1570 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1571 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1576 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8630 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1582 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1591 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8654 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1601 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8660 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8666 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8672 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1610 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8678 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1615 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8684 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1617 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8690 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1619 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8696 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1625 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8702 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1626 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8708 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1627 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1633 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8720 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1635 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8726 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1637 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8742 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1645 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8748 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1647 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8756 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1651 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1659 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1660 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1661 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1662 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8794 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1667 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8800 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8806 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1669 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1670 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8818 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1671 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8824 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1700 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1708 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8860 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8866 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8872 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8878 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8884 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8896 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8902 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1733 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8908 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1737 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8914 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1738 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8920 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1739 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8926 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8932 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8938 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8944 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1748 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1749 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1750 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8962 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8968 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1755 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8974 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1759 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8980 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8986 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1761 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8992 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8998 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1766 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9004 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9010 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1775 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9016 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1776 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9022 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 443:
#line 1780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9034 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9040 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1783 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9046 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 447:
#line 1787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9052 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9058 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1800 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9064 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9070 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9082 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9088 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9094 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9100 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9106 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9112 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9118 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9124 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9130 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1834 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9136 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9142 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1838 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9148 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1840 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9154 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1842 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9160 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9166 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1847 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9172 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1849 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9184 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9190 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1860 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9202 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9208 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9214 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1866 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9220 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1868 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9226 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1870 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9232 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1872 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9238 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1874 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9244 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1876 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9250 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1878 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9261 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1885 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1891 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9281 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1897 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9291 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1903 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1912 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1921 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 9313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1937 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1941 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 9325 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1946 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9331 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1950 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9337 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1951 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9343 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1955 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9349 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1959 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9355 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1960 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9361 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1965 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9369 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1969 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1973 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9385 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1979 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9391 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1980 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9397 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1981 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9403 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1982 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9409 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1983 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9415 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1984 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9421 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 1990 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9427 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 1995 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9433 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 1997 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9441 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 2001 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9449 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 2005 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9457 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 524:
#line 2009 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9465 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 525:
#line 2017 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 2022 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9482 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 2028 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 2034 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9503 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 2041 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 2050 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new owned(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'owned.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtOwned->symbol, (yyvsp[-1].pexpr))); }
#line 9522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2054 "chapel.ypp" /* yacc.c:1663  */
    { USR_WARN((yyvsp[-1].pexpr), "the syntax 'new shared(an expression)' is deprecated");
      USR_PRINT((yyvsp[-1].pexpr), "instead use 'shared.create(an expression)'");
      (yyval.pexpr) = new CallExpr(PRIM_NEW, new CallExpr(dtShared->symbol, (yyvsp[-1].pexpr))); }
#line 9530 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2061 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9536 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 541:
#line 2077 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9542 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 542:
#line 2079 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9548 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 543:
#line 2081 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9554 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 544:
#line 2083 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 9560 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 545:
#line 2098 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 9566 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2100 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 9572 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2102 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 9578 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2104 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 9584 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2108 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9590 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2109 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9596 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2113 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9602 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2114 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9608 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2115 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9614 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2133 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9620 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 559:
#line 2134 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 560:
#line 2135 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9632 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 561:
#line 2136 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9638 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 562:
#line 2140 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9644 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2141 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9650 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2142 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2147 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 567:
#line 2148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2149 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2150 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9692 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2159 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9698 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2160 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9704 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2161 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9710 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2165 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9716 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9722 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9728 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2172 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9734 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2173 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9740 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2174 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9746 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 9752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2177 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 584:
#line 2178 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 9770 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 585:
#line 2179 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 9776 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2180 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2181 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9788 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2183 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2187 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2194 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9810 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2195 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9816 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9822 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9828 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9834 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2202 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9840 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2203 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9846 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2204 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9852 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2205 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9858 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2206 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9864 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2207 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9870 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2208 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9876 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2209 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2210 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9888 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2211 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9894 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2212 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9900 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2213 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9906 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2214 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9912 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2215 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9918 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2216 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2217 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2218 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2219 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2220 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2221 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2225 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2226 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2227 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2228 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2229 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2230 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2231 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2235 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2236 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 624:
#line 2237 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 625:
#line 2238 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 626:
#line 2242 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 627:
#line 2243 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 628:
#line 2244 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 629:
#line 2245 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 630:
#line 2250 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 631:
#line 2251 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 632:
#line 2252 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 633:
#line 2253 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 634:
#line 2254 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 635:
#line 2255 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 636:
#line 2256 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 10090 "bison-chapel.cpp" /* yacc.c:1663  */
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
