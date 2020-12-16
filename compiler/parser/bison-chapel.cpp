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
#define YYLAST   18859

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  153
/* YYNRULES -- Number of rules.  */
#define YYNRULES  643
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1154

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
     647,   652,   657,   661,   670,   675,   680,   685,   689,   693,
     701,   706,   710,   715,   719,   720,   721,   725,   726,   727,
     728,   729,   730,   731,   735,   740,   741,   742,   746,   747,
     751,   755,   757,   759,   761,   763,   765,   772,   773,   777,
     778,   779,   780,   781,   782,   785,   786,   787,   788,   789,
     790,   802,   803,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   846,   847,   854,   855,   856,   857,   861,   862,
     866,   867,   871,   872,   873,   883,   883,   888,   889,   890,
     891,   892,   893,   894,   898,   899,   900,   901,   906,   905,
     921,   920,   937,   936,   952,   951,   967,   971,   976,   984,
     995,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1026,  1032,  1038,  1044,  1051,  1058,  1062,  1069,  1073,  1074,
    1075,  1076,  1077,  1079,  1081,  1083,  1088,  1091,  1092,  1093,
    1094,  1095,  1096,  1100,  1101,  1105,  1106,  1107,  1111,  1112,
    1116,  1119,  1121,  1126,  1127,  1131,  1133,  1135,  1142,  1152,
    1166,  1171,  1176,  1184,  1185,  1190,  1191,  1193,  1198,  1214,
    1221,  1230,  1238,  1242,  1249,  1250,  1255,  1260,  1254,  1287,
    1290,  1294,  1302,  1312,  1301,  1344,  1348,  1353,  1357,  1362,
    1369,  1370,  1374,  1375,  1376,  1377,  1378,  1379,  1380,  1381,
    1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1404,
    1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,
    1415,  1419,  1420,  1424,  1428,  1429,  1430,  1434,  1436,  1438,
    1440,  1442,  1447,  1448,  1452,  1453,  1454,  1455,  1456,  1457,
    1458,  1459,  1460,  1464,  1465,  1466,  1467,  1468,  1469,  1473,
    1474,  1478,  1479,  1480,  1481,  1482,  1483,  1487,  1488,  1491,
    1492,  1496,  1497,  1501,  1505,  1506,  1507,  1515,  1516,  1518,
    1520,  1522,  1527,  1529,  1534,  1535,  1536,  1537,  1538,  1539,
    1540,  1544,  1546,  1551,  1553,  1555,  1560,  1573,  1590,  1591,
    1593,  1598,  1599,  1600,  1601,  1602,  1606,  1612,  1620,  1621,
    1629,  1631,  1636,  1638,  1640,  1645,  1647,  1649,  1656,  1657,
    1658,  1663,  1665,  1667,  1671,  1675,  1677,  1681,  1689,  1690,
    1691,  1692,  1693,  1698,  1699,  1700,  1701,  1702,  1722,  1726,
    1730,  1738,  1745,  1746,  1747,  1751,  1753,  1759,  1761,  1763,
    1768,  1769,  1770,  1771,  1772,  1778,  1779,  1780,  1781,  1785,
    1786,  1790,  1791,  1792,  1796,  1797,  1801,  1802,  1806,  1807,
    1811,  1812,  1813,  1814,  1818,  1819,  1830,  1832,  1834,  1840,
    1841,  1842,  1843,  1844,  1845,  1847,  1849,  1851,  1853,  1855,
    1857,  1860,  1862,  1864,  1866,  1868,  1870,  1872,  1874,  1877,
    1879,  1884,  1886,  1888,  1890,  1892,  1894,  1896,  1898,  1900,
    1902,  1904,  1906,  1908,  1915,  1921,  1927,  1933,  1942,  1952,
    1960,  1961,  1962,  1963,  1964,  1965,  1966,  1967,  1972,  1973,
    1977,  1981,  1982,  1986,  1990,  1991,  1995,  1999,  2003,  2010,
    2011,  2012,  2013,  2014,  2015,  2019,  2020,  2025,  2027,  2031,
    2035,  2039,  2047,  2052,  2058,  2064,  2071,  2081,  2089,  2090,
    2091,  2092,  2093,  2094,  2095,  2096,  2097,  2099,  2101,  2103,
    2118,  2120,  2122,  2124,  2129,  2130,  2134,  2135,  2136,  2140,
    2141,  2142,  2143,  2152,  2153,  2154,  2155,  2156,  2160,  2161,
    2162,  2166,  2167,  2168,  2169,  2170,  2178,  2179,  2180,  2181,
    2185,  2186,  2190,  2191,  2195,  2196,  2197,  2198,  2199,  2200,
    2201,  2202,  2204,  2206,  2207,  2208,  2212,  2219,  2220,  2224,
    2225,  2226,  2227,  2228,  2229,  2230,  2231,  2232,  2233,  2234,
    2235,  2236,  2237,  2238,  2239,  2240,  2241,  2242,  2243,  2244,
    2245,  2246,  2251,  2252,  2253,  2254,  2255,  2256,  2257,  2261,
    2262,  2263,  2264,  2268,  2269,  2270,  2271,  2276,  2277,  2278,
    2279,  2280,  2281,  2282
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
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "except_ls",
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
  "opt_try_expr", "lhs_expr", "call_base_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "str_bytes_literal",
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
     425,   426,   427,   428,   429
};
# endif

#define YYPACT_NINF -1006

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1006)))

#define YYTABLE_NINF -594

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1006,    93,  3059, -1006,   -54, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006,  4411,    46,   143, -1006, 13897, -1006, 17031,    46,
   10636, -1006,   209,   167,   143,  4411, 10636,  4411,   -52, 18549,
   10803,  7781, -1006,  7948,  8958,  6437, 10636, -1006, -1006,    10,
   -1006, -1006, 18599, -1006,  2545,  9125, -1006, 10636,   198, -1006,
   -1006, -1006, 10636, -1006, 13897, -1006, 10636,   242,   120,   361,
   16818, -1006, 18649, -1006,  9294,  7278, 10636,  9125, 13897, 10636,
     170, -1006,   138,  4411, -1006, 10636, -1006, 10970, 10970, 18599,
   -1006, -1006, 13897, -1006, -1006, 10636, 10636, -1006, 10636, -1006,
   10636, -1006, -1006, 13433, 10636, -1006, 10636, -1006, -1006, -1006,
    3397,  6606,  8117, 10636, -1006,  4242, -1006,   109, -1006,   235,
   -1006, -1006,    35, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, 18599, -1006, 18599,   384,   211, -1006, -1006,  2545,
   -1006,   168, -1006,   207, -1006, -1006,   218,   229,   253, 10636,
     249,   262, 18026,  2053,   131,   284,   306, -1006, -1006,   310,
   -1006, -1006, -1006, -1006, -1006,    30, -1006, -1006, 18026,   332,
    4411, -1006, -1006,   378, 10636, -1006, -1006, 10636, 10636, 10636,
   -1006, 10636,  9294,  9294,   417, -1006, -1006, -1006, -1006,   111,
     429, -1006, -1006,   368, 15208, 18599,  2545, -1006,   407, -1006,
     156, 18026,   446,  7447,   485, 18699, 18026,   410,   508, -1006,
   18749, 18599,   410, 18599,   433,    36, 15042,    13, 14966,   400,
   -1006,  2380,  2873,   540,  7447, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,  4411,
   -1006,   456,  1622,    95,     0, -1006,  4411, -1006, -1006, 15288,
     482, -1006,   461,   467, -1006, 15288,   111,   482, -1006,  7447,
    1800, -1006, -1006,  9461, -1006, -1006, -1006, -1006,   370, 18026,
   10636, 10636, -1006, 18026,   454, 15823, -1006, 15288,   111, 18026,
     474,  7447, -1006, 18026, 15863, -1006, -1006, 15912,  2537, -1006,
   -1006, 16009,   506,   483,   111, 15288, 16058,   459,   459,  1071,
     482,   482,   240, -1006, -1006,  3566,   -42, -1006, 10636, -1006,
     -29,   161, -1006,   -67,    83, 16104,   -53,  1071,   633, -1006,
    3735, -1006,   586, 10636, 10636,   510,   486, -1006, -1006, -1006,
     381,   441, -1006, 10636,   509, 10636, 10636, 10636,  8958,  8958,
   10636,   464, 10636, 10636, 10636, 10636, 10636,   663, 13433, 10636,
   10636, 10636, 10636, 10636, 10636, 10636, 10636, 10636, 10636, 10636,
   10636, 10636, 10636, 10636, 10636,   589, -1006, -1006, -1006, -1006,
   -1006,  8284,  8284, -1006, -1006, -1006, -1006,  8284, -1006, -1006,
    8284,  8284,  7447,  7447,  8958,  8958,  7111, -1006, -1006, 15364,
   15444, 16204,   -19,  3904, -1006,  8958,    36,   495,   395, -1006,
   10636, -1006, 10636,   536, -1006,   490,   519, -1006, -1006, -1006,
   18599, -1006,  2545, -1006, 18599,   501, -1006,  2545,   615,  9294,
   -1006,  4580,  8958, -1006,   502, -1006,    36,  4749,  8958, -1006,
      36, 11137, 10636, -1006,  4411, -1006,   635, 10636, -1006, -1006,
     235,   500, -1006,   363, -1006, -1006,  1622, -1006,   530,   504,
   -1006, 11304,   553, 10636,  2545, -1006, -1006, 10636, 10636, -1006,
     511, -1006,  9294, -1006, 18026, 18026, -1006,    50, -1006,  7447,
     512, -1006,   655, -1006,   655, -1006, 11471,   537, -1006, -1006,
   -1006, -1006, -1006, -1006,  8453, -1006, 16250,  6775, -1006,  6944,
   -1006,  4411,   521,  8958,  8622,  3228,   523, 10636,  9628, -1006,
   -1006,   447, -1006,  4073, 18599, -1006,   404, 16290,   405, 15132,
    9294,   516, 18499,    80, -1006, 16403,  1606,  1606,   469, -1006,
     469, -1006,   469, 18309,   662,   693,  1257,   111,   459,   528,
   -1006, -1006, -1006, -1006,  1071, 18344,   469, 13587, 13587,  1606,
   13587, 13587,  2267,   459, 18344,  2210,  2267,   482,   482,   459,
    1071,   538,   539,   542,   543,   546,   551,   532,   550, -1006,
     469, -1006,   469,   193, -1006, -1006, -1006,   162, -1006, 13236,
   18139,   367, 11638,  8958, 11805,  8958, 10636,  8958, 14161,    46,
   16448, -1006, -1006, -1006, 18026, 16488,  7447, -1006,  7447, 18599,
     510,   409, 18599,   510, -1006,   418, 10636,   228, 10636, 18026,
      54, 15520,  7111, -1006, 10636, 18026,    28,  2701, -1006,   561,
     572,   564, 16601,   572,   566,   692, 16641,  4411, 15600,   666,
   -1006,   203, -1006, -1006, -1006, -1006, -1006, -1006,   960,   234,
   -1006,  2679, -1006,   507,   562,  1622,    95,   -52,    10, 10636,
   10636,  6270, -1006, -1006,   136,  7614, -1006, 18026, -1006, 16681,
   16794, -1006, -1006, 18026,   567,    -7,   568, -1006,  2222, -1006,
   -1006,   420, 18599, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
    4411,   -37, 15676, -1006, -1006, 18026,  4411, 18026, -1006, 16835,
   -1006, -1006, -1006, 10636, -1006,   105,    92, 10636, -1006,  9795,
   11137, 10636,   591,  1279,   569,   614,   152, -1006,   654, -1006,
   -1006, -1006, -1006, 13271,   575, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006,  7111, -1006,    40,  8958,  8958,
   10636,   712, 16875, 10636,   714, 17021,   581, 17061,    36,    36,
   -1006, -1006, -1006, -1006,   510,   593, -1006,   510,   597, -1006,
   15288, -1006, 14342,  4918, -1006,  5087, -1006,   251, -1006, 14418,
    5256, -1006,    36,  5425, -1006,    36, 10636, -1006, 10636, -1006,
    4411, 10636, -1006,  4411,   719, 18599, -1006, -1006, 18599,  1040,
   -1006,  1622,   625,   683, -1006, -1006, -1006,    98, -1006, -1006,
     553,   600,    48, -1006, -1006,   606,   607, -1006,  5594,  9294,
   -1006, -1006, -1006, 18599, -1006,   636,   368, -1006, -1006,  5763,
     605,  5932,   609, -1006, 10636, -1006, -1006, 10636, 17101,    65,
   15766,   620,   624,   619,  1405, -1006, 10636, 18599, -1006, -1006,
     507,   622,   373, -1006,   644, -1006,   647,   648,   657,   650,
     652, -1006,   653,   664,   658,   665,   667,   380,   672,   668,
     669, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006, -1006, 10636, -1006,   678,   681,   676,   622,   622,
   -1006, -1006, -1006,   553,   291,   312, 17214, 11972, 12139, 17254,
   12306, 12473, 12640, 12807, -1006, -1006,   661, -1006,   673, -1006,
   -1006,  4411, 10636, 18026, 10636, 18026,  7111, -1006,  4411, 10636,
   18026, -1006, 10636, 18026, -1006, 17294, 18026, -1006, 18026,   785,
    4411,   674,   625, -1006,   659,  9964,   355, -1006,    22, -1006,
   -1006,  8958, 14029,  7447,  7447,  4411, -1006,    32,   671, 10636,
   -1006, 10636, -1006, 18026,  4411, 10636, -1006, 18026,  4411, 18026,
     366, 10131, 11137, 10636, 11137, 10636, -1006, -1006, -1006, -1006,
    1800, -1006, 14265, -1006, 18026, -1006,    82,   363, -1006, 17407,
   -1006, 14260, -1006, -1006, -1006, 10636, 10636, 10636, 10636, 10636,
   10636, 10636, 10636, -1006,  2049, -1006, 18163, 16641, 14498, 14574,
   -1006, 16641, 14654, 14730, 10636,  4411, -1006, -1006,   553,   625,
    8791, -1006, -1006, -1006,   245,  9294, -1006, -1006,   146, 10636,
     -38, 17447, -1006,   588,   675,   679,   446, -1006,   368, 18026,
   14810, -1006, 14886, -1006, -1006, -1006, 18026,   677,   682,   684,
     686, -1006, -1006, 12974,   741,   360, -1006,   715,   716,   622,
     622, 17560, 17600, 17640, 17753, 17793, 17833, 18276, -1006, 18294,
   -1006,  4411,  4411,  4411,  4411, 18026, -1006, -1006,   355, 10300,
      89, -1006, 18026, -1006,   107, -1006,   126, -1006,   297, 17946,
   -1006, -1006, -1006, 12807,   690,   700, -1006,  4411,  4411, -1006,
   -1006, -1006, -1006,  6101, -1006, -1006,   208, -1006,    22, -1006,
   -1006, -1006, 10636, 10636, 10636, 10636, 10636, 10636, -1006, -1006,
   16641, 16641, 16641, 16641, -1006, -1006, -1006, -1006, -1006,   267,
    8958, 13599, -1006, 10636,   146,   107,   107,   107,   107,   107,
     107,   146,   618, -1006, -1006, 16641, 16641,   691, 13141,    90,
     212, 17986, -1006, -1006, 18026, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006,   730, -1006, -1006,   233, 13765, -1006, -1006, -1006,
   10469, -1006,   316, -1006
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   586,   587,   588,   582,
     583,   589,     0,   508,    97,   132,   477,   139,   479,   508,
       0,   138,     0,   382,    97,     0,     0,     0,     0,   133,
     554,   554,   580,     0,     0,     0,     0,   137,    53,     0,
     260,   134,     0,   256,     0,     0,   141,     0,   527,   499,
     590,   142,     0,   261,   471,   381,     0,     0,     0,   155,
      46,   136,   480,   383,     0,     0,     0,     0,   475,     0,
       0,   140,     0,     0,   112,     0,   581,     0,     0,     0,
     135,   242,   473,   385,   143,     0,     0,   639,     0,   641,
       0,   642,   643,   553,     0,   640,   637,   458,   152,   638,
       0,     0,     0,     0,     4,     0,     5,     0,     9,    48,
      10,    11,     0,    12,    13,    14,    16,   454,   455,    24,
      15,   153,   162,   163,    17,    19,    18,    21,    22,    23,
      20,   161,     0,   159,     0,   543,     0,   157,   160,     0,
     158,   559,   539,   456,   540,   461,   459,     0,     0,     0,
     544,   545,     0,   460,     0,   560,   561,   562,   584,   585,
     538,   463,   462,   541,   542,     0,    40,    26,   469,     0,
       0,   509,    98,     0,     0,   479,   133,     0,     0,     0,
     480,     0,     0,     0,   543,   559,   459,   544,   545,   478,
     460,   560,   561,     0,   508,     0,     0,   384,     0,   216,
       0,   439,     0,   446,   582,   480,   555,   259,   582,   180,
     480,     0,   259,     0,     0,     0,     0,     0,     0,     0,
     167,     0,     0,    50,   446,   105,   113,   125,   119,   118,
     127,   108,   117,   128,   114,   129,   106,   130,   123,   116,
     124,   122,   120,   121,   107,   109,   115,   126,   131,     0,
     110,     0,     0,     0,     0,   388,     0,   149,    34,     0,
     624,   531,   528,   529,   530,     0,   472,   625,     7,   446,
     259,   240,   250,   554,   241,   154,   353,   436,     0,   435,
       0,     0,   150,   558,     0,     0,    37,     0,   476,   464,
       0,   446,    38,   470,     0,   223,   219,     0,   460,   223,
     220,     0,   378,     0,   474,     0,     0,   626,   628,   551,
     623,   622,     0,    55,    58,     0,     0,   441,     0,   443,
       0,     0,   442,     0,     0,   435,     0,   552,     0,     6,
       0,    49,     0,     0,     0,   243,     0,   340,   339,   262,
       0,   457,    25,     0,   532,     0,     0,     0,     0,     0,
       0,   627,     0,     0,     0,     0,     0,     0,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   299,   306,   307,   308,   303,
     305,     0,     0,   301,   304,   302,   300,     0,   310,   309,
       0,     0,   446,   446,     0,     0,     0,    27,    28,     0,
       0,     0,     0,     0,    29,     0,     0,     0,     0,    30,
       0,    31,     0,   454,   452,     0,   447,   448,   453,   174,
       0,   177,     0,   170,     0,     0,   176,     0,     0,     0,
     189,     0,     0,   188,     0,   197,     0,     0,     0,   195,
       0,     0,    70,   164,     0,   299,   209,     0,    52,    51,
      48,     0,    32,   322,   257,   392,     0,   393,   395,     0,
     417,     0,   398,     0,     0,   148,    33,     0,     0,    35,
       0,   156,     0,    90,   556,   557,   151,     0,    36,   446,
       0,   230,   221,   217,   222,   218,     0,   376,   373,   182,
      39,    57,    56,    59,     0,   591,     0,     0,   576,     0,
     578,     0,     0,     0,     0,     0,     0,     0,     0,   595,
       8,     0,    42,     0,     0,    88,     0,    85,     0,    64,
       0,     0,     0,   333,   387,   507,   619,   618,   621,   630,
     629,   634,   633,   615,   612,   613,   614,   547,   602,     0,
     573,   574,   572,   571,   548,   606,   617,   611,   609,   620,
     610,   608,   600,   605,   607,   616,   599,   603,   604,   601,
     549,     0,     0,     0,     0,     0,     0,     0,     0,   632,
     631,   636,   635,   519,   520,   522,   524,     0,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   593,   508,
     508,   185,   374,   386,   440,     0,     0,   466,     0,     0,
     243,     0,     0,   243,   375,     0,     0,     0,     0,   483,
       0,     0,     0,   198,     0,   489,     0,     0,   196,   638,
      73,     0,    60,    71,     0,   208,     0,     0,     0,     0,
     465,   327,   324,   325,   326,   330,   331,   332,   322,     0,
     315,     0,   323,   341,     0,   396,     0,   146,   147,   145,
     144,     0,   416,   415,   539,     0,   390,   537,   389,     0,
       0,   570,   438,   437,     0,     0,     0,   467,     0,   224,
     380,   539,     0,   592,   546,   577,   444,   579,   445,   205,
       0,     0,     0,   594,   203,   493,     0,   597,   596,     0,
      44,    43,    41,     0,    84,     0,     0,     0,    77,     0,
       0,    70,   244,     0,     0,   254,     0,   251,   337,   334,
     335,   338,   263,     0,     0,    96,    94,    95,    93,    92,
      91,   568,   569,   521,   523,     0,   510,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     181,   450,   451,   449,   243,     0,   179,   243,     0,   178,
       0,   207,     0,     0,   187,     0,   186,     0,   514,     0,
       0,   193,     0,     0,   191,     0,     0,   165,     0,   166,
       0,     0,   211,     0,   213,     0,   328,   329,     0,   322,
     313,     0,   430,   342,   345,   344,   346,     0,   394,   397,
     398,     0,     0,   399,   400,     0,     0,   232,     0,     0,
     231,   234,   468,     0,   225,   228,     0,   377,   206,     0,
       0,     0,     0,   204,     0,    89,    86,     0,    67,    66,
      65,     0,     0,     0,   259,   249,     0,   252,   248,   336,
     341,   311,    99,   294,   113,   292,   119,   118,   102,   117,
     114,   297,   129,   100,   130,   116,   120,   101,   103,   115,
     131,   291,   273,   276,   274,   275,   286,   277,   290,   282,
     280,   293,   296,   281,   279,   284,   289,   278,   283,   287,
     288,   285,   295,     0,   272,     0,   104,     0,   311,   311,
     270,   575,   512,   398,   559,   559,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   183,     0,   245,     0,   245,
     190,     0,     0,   482,     0,   481,     0,   513,     0,     0,
     488,   194,     0,   487,   192,    62,    61,   210,   498,   212,
       0,     0,   430,   316,     0,     0,   398,   343,   357,   391,
     421,     0,   593,   446,   446,     0,   236,     0,     0,     0,
     226,     0,   201,   495,     0,     0,   199,   494,     0,   598,
       0,     0,     0,    70,     0,    70,    78,    81,   239,   155,
     259,   238,   259,   246,   255,   253,     0,   322,   269,     0,
     298,     0,   265,   266,   516,     0,     0,     0,     0,     0,
       0,     0,     0,   245,   259,   245,   259,   486,     0,     0,
     515,   492,     0,     0,     0,     0,   215,    54,   398,   430,
       0,   433,   432,   434,   539,   354,   319,   317,     0,     0,
       0,     0,   419,   539,     0,     0,   237,   235,     0,   229,
       0,   202,     0,   200,    87,    69,    68,     0,     0,     0,
       0,   247,   412,     0,   347,     0,   271,    99,   101,   311,
     311,     0,     0,     0,     0,     0,     0,   259,   173,   259,
     169,     0,     0,     0,     0,    63,   214,   318,   398,   422,
       0,   356,   355,   371,     0,   372,   359,   362,     0,   358,
     351,   352,   258,     0,   533,   534,   227,     0,     0,    80,
      83,    79,    82,     0,   411,   410,   539,   348,   357,   312,
     267,   268,     0,     0,     0,     0,     0,     0,   175,   171,
     485,   484,   491,   490,   321,   320,   424,   425,   427,   539,
       0,   593,   370,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   539,   535,   536,   497,   496,     0,   402,     0,
       0,     0,   426,   428,   361,   363,   364,   367,   368,   369,
     365,   366,   360,   407,   405,   539,   593,   349,   264,   350,
     422,   406,   539,   429
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1006, -1006, -1006,    11,  -338,  1995, -1006, -1006, -1006,   421,
   -1006, -1006, -1006,   246,   529,  -430, -1006,  -675,  -671, -1006,
   -1006, -1006,   179, -1006, -1006,   128,   849, -1006,   -10,  -194,
    -696, -1006,  -858,   763,  -976,  -775, -1006,   -57, -1006, -1006,
   -1006, -1006, -1006, -1006, -1006, -1006,   123, -1006, -1006, -1006,
     578, -1006,    79, -1006, -1006, -1006, -1006, -1006, -1006,  -500,
    -589, -1006, -1006, -1006,    57,  1093, -1006, -1006, -1006,   118,
   -1006, -1006, -1006, -1006,   -86,  -151,  -820, -1006,   -81,   108,
     250, -1006, -1006, -1006,    61, -1006, -1006,  -237,   680,  -163,
    -190,  -222,  -212,  -251, -1006,  -175, -1006,    21,   870,  -111,
     442, -1006,  -418,  -774,  -938, -1006,  -609,  -479, -1005,  -967,
    -851,   -63, -1006,    39, -1006,  -219, -1006,   309,   514,  -455,
   -1006, -1006, -1006,  1115, -1006,    -8, -1006, -1006,  -193, -1006,
    -597, -1006, -1006, -1006,  1145,  1529,   -12,   878,   166,   899,
   -1006,  1874,  1994, -1006, -1006, -1006, -1006, -1006, -1006, -1006,
   -1006, -1006,  -386
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   314,   105,   625,   107,   108,   109,   332,
     450,   110,   223,   111,   315,   620,   518,   624,   621,   112,
     113,   114,   515,   516,   115,   116,   173,   874,   253,   117,
     250,   118,   652,   258,   119,   120,   270,   121,   122,   123,
     424,   602,   420,   599,   124,   125,   736,   126,   127,   128,
     482,   669,   806,   129,   130,   665,   801,   131,   132,   521,
     824,   133,   134,   706,   707,   184,   251,   643,   136,   137,
     523,   830,   712,   877,   878,   447,   968,   454,   639,   640,
     641,   642,   713,   339,   787,  1088,  1148,  1072,   277,  1006,
    1010,  1066,  1067,  1068,   138,   303,   487,   139,   140,   254,
     255,   458,   459,   656,  1085,  1034,   462,   653,  1107,  1003,
     926,   316,   200,   320,   321,   415,   416,   417,   185,   142,
     143,   144,   145,   186,   147,   170,   171,   577,   436,   757,
     578,   579,   148,   149,   187,   188,   152,   212,   418,   190,
     154,   191,   192,   157,   158,   159,   160,   326,   161,   162,
     163,   164,   165
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,   278,   391,   275,   189,   451,   654,   670,   194,   413,
     581,   193,   623,   104,   201,   758,   929,   876,   206,   206,
     407,   216,   218,   221,   222,   440,   822,   587,   340,   821,
     413,   671,   249,   259,  1071,   260,   425,   790,   644,   324,
     265,   460,   266,   196,   267,   256,   463,   434,    65,   963,
     470,   664,   279,   283,   285,   287,   288,   289,   972,   973,
     760,   293,   798,   294,   256,   297,   301,  1001,   256,   302,
     304,   998,   480,   305,   306,   413,   307,   434,   308,  1008,
     333,   309,   310,  1032,   311,   408,   753,   508,   279,   283,
     325,   327,  1108,     3,   931,     5,   460,   413,   494,   460,
     745,   952,   501,   748,   799,   166,  1132,   708,     5,   974,
    1063,   497,   539,   394,   203,   638,   509,   395,   883,  -233,
     402,   504,   335,   495,   336,   540,   403,   434,   882,   503,
     502,   506,   809,   953,  1009,  1110,   587,   344,   498,  -408,
     464,  -413,   434,   334,  1133,  1153,   172,   391,  1058,  1063,
     588,   541,  1007,   709,  1071,  -413,   215,   217,   800,  -413,
     169,  -233,   168,   543,   710,   399,   400,   401,  -408,   293,
     279,   325,   472,   567,   568,  1084,   224,   403,   928,   461,
    -413,  -408,  -414,   711,  -413,   302,   406,  1031,   504,    74,
    1144,   283,   403,  -414,  -408,   542,   403,   434,   413,   413,
     403,   302,    74,   428,  1065,   296,   300,  -413,  1151,   963,
    -413,   963,   283,   471,  -413,  -233,   261,   932,   403,  1090,
    1091,  1033,  -414,   504,  1057,   323,   581,   789,   422,   504,
     504,   284,  1064,   427,   461,  -413,    23,   461,  1113,   723,
     196,  -408,   457,  1065,   896,   351,  -408,   898,  -414,   776,
     268,   197,   505,   357,  -413,  -414,   817,   283,  1111,  1146,
     666,   206,  -413,   337,   290,  -409,  1114,   322,   474,   475,
     262,   623,   963,   330,   963,   876,  -414,   724,  1028,   283,
    1030,  1027,    55,  1029,  1105,   413,   269,   777,   263,   338,
    -401,   257,   827,    63,  -409,  -414,   410,   392,    65,   393,
    -414,   499,   725,  -414,   291,   264,   496,  -409,   984,   990,
     986,   601,   195,   257,   331,   411,   605,   828,    83,  -401,
    -409,   517,   519,   295,   299,   421,   493,  -501,   500,   726,
     426,   525,  -401,   526,   527,   528,   530,   532,   533,   581,
     534,   535,   536,   537,   538,  -401,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   924,  -431,   341,   607,  -409,   472,   283,
     283,  1147,  -409,  -403,   779,   283,   403,  -500,   283,   283,
     283,   283,   570,   572,   580,  -431,  -423,   950,   342,  -431,
     631,   906,  -401,   590,  1047,   751,  1049,  -401,   594,   166,
     595,   780,  -403,   343,   413,   491,   -76,  -423,  -505,   632,
     600,  -423,  -431,   633,   603,  -403,  1115,   279,   907,   609,
     611,  -506,   -47,   762,   765,   615,   617,    23,  -403,   622,
     622,  -518,   626,   634,  -423,   628,   635,    23,    57,   404,
     -47,   638,  1116,  -503,  1117,  1118,   457,   636,  1119,  1120,
     729,   657,  -517,  1012,   395,   659,   660,   702,  -518,    40,
     663,   257,   257,   257,   257,  -502,   637,   283,   446,   -76,
    1004,   529,   531,    55,   655,  -403,  -567,  1013,  -567,  -517,
    -403,    53,   663,    55,    63,   283,   962,   283,   810,   812,
     347,   682,   663,   685,    63,   687,   689,   807,   396,  1005,
     779,   816,   543,  -314,   692,   257,   766,  -105,   279,    83,
     472,   257,   705,   347,  -107,  -111,   141,   569,   571,    83,
     581,   464,  -112,   623,   493,   623,   141,  1089,   589,   473,
    -314,  1024,   403,   257,   783,   464,   676,   398,   678,   141,
     524,   141,   348,  -504,   693,   697,   349,   562,   563,   464,
    -566,   257,  -566,   564,   593,   610,   565,   566,   464,   412,
    -379,   616,   879,   694,   698,   348,   409,   419,   746,   349,
     609,   732,   615,   735,   626,   737,   685,   749,  1086,  -379,
     784,   738,   739,  -566,   283,  -566,   283,   141,   792,   744,
     423,   785,   747,   351,   750,  -563,   752,  -563,   355,   429,
     580,   357,   759,   351,  1109,   360,   166,  -565,   355,  -565,
     786,   357,   690,   476,   141,   360,   351,   448,  1122,   141,
     449,   355,   453,   356,   357,   486,   681,   467,   360,   638,
    -564,   782,  -564,   468,  -418,   457,   367,   289,   293,   325,
     479,   510,   488,   283,   373,  -418,   962,   514,   962,   520,
     522,   357,   257,   561,   592,   596,  1109,   597,   805,   598,
     604,   606,   302,   322,  -420,   322,     5,   630,   612,   627,
     645,   646,   655,  1145,  -418,  -420,   668,   672,   661,   667,
     703,   517,   257,   539,   141,   818,   257,   820,   622,   622,
     680,  1152,   686,   347,   714,  1109,   540,   715,   716,   721,
    -418,   717,   718,   875,  -420,   719,   731,  -418,   734,   962,
     720,   962,   766,   580,  1014,  1015,   530,   570,   886,   722,
     -72,   889,   541,   767,   347,   769,   770,   775,  -418,   788,
    -420,   472,   797,   826,   825,   802,   937,  -420,   829,   413,
     413,   903,   881,   905,   887,   348,   890,  -418,   910,   349,
     892,   913,  -418,   920,   915,  -418,   916,   897,  -420,   918,
      74,   899,   742,   141,   925,   921,   542,   927,   922,   930,
     141,   457,   933,   934,   944,   939,   348,  -420,   948,   956,
     349,   319,  -420,   957,   958,  -420,  -132,   279,   967,  -139,
    -138,  -108,  -137,   805,  -134,  -141,   351,   943,  -106,   947,
    -142,   355,   949,   356,   357,   622,  -109,  -136,   360,  -140,
    -135,  -143,   970,  1112,   964,  -110,   367,   705,   971,   995,
    1040,   794,   371,   372,   373,   983,   999,   351,   352,   141,
     286,   354,   355,   997,   356,   357,  1079,   985,  1018,   360,
    1087,  1080,  1074,  1081,   141,  1082,  1075,   367,  1123,  -105,
    -107,   529,   569,   371,   372,   373,   257,   257,  1124,   513,
    1143,   969,   257,   257,  1136,  1137,  1138,  1139,  1140,  1141,
    1114,   629,   815,   198,   774,   903,   905,   484,   910,   913,
     943,   947,   938,   414,   965,  1039,  1035,   923,   778,   987,
     988,   966,   989,  1149,   580,  1104,   991,   992,  1130,  1142,
     993,   153,  1135,   275,   414,   220,   658,   743,   207,     0,
       0,   153,     0,     0,   804,     0,     0,   141,     0,  1011,
     685,   283,   283,     0,   153,     0,   153,  1019,     0,  1020,
       0,     0,     0,  1022,     0,     0,     0,  1060,     0,  1026,
     622,   622,   622,   622,     0,   141,     0,     0,     0,   414,
       0,   141,     0,     0,     0,     0,     0,     0,   141,     0,
       0,   875,     0,   987,  1041,  1042,   991,  1043,  1044,  1045,
    1046,   414,   153,     0,     0,     0,   298,   298,   430,   433,
     435,   439,  1055,     0,   257,   257,     0,   631,   325,     0,
       0,     0,     0,  1062,     0,     0,   257,  1069,   446,   153,
       0,     0,     0,     0,   153,   446,   632,     0,   257,     0,
     633,   257,     0,     0,     0,   141,     0,     0,     0,   141,
    1129,     0,   466,     0,     0,     0,     0,   141,   469,     0,
     634,     0,     0,   635,     0,   328,     0,     0,     0,  1100,
    1101,  1102,  1103,     0,   636,     0,     0,     0,     0,     0,
     478,     0,   940,     0,     0,     0,     0,     0,     0,     0,
       0,   947,     0,   637,     0,  1125,  1126,   631,   489,   153,
       0,   325,   414,   414,     0,     0,     0,     0,     0,     0,
    1100,  1101,  1102,  1103,  1125,  1126,   632,     0,     0,     0,
     633,     0,     0,   727,     0,   135,     0,     0,  1131,   685,
       0,  1134,   347,     0,     0,   135,     0,     0,     0,     0,
     634,     0,     0,   635,     0,    57,     0,   146,   135,     0,
     135,     0,     0,     0,   636,     0,     0,   146,     0,     0,
       0,     0,     0,     0,   685,     0,     0,     0,   947,     0,
     146,   141,   146,   637,     0,     0,     0,   150,   153,     0,
       0,     0,   662,     0,   348,   153,     0,   150,   349,   414,
       0,     0,     0,     0,     0,     0,   135,     0,     0,   591,
     150,     0,   150,     0,   662,     0,     0,   319,     0,   319,
       0,     0,     0,   257,   662,     0,     0,     0,   146,     0,
       0,     0,     0,   135,   141,     0,     0,     0,   135,   613,
     141,     0,     0,   618,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   153,   146,     0,   360,   150,     0,
     146,     0,     0,     0,   366,   367,     0,   880,   370,   153,
       0,   371,   372,   373,   446,   446,     0,     0,   446,   446,
       0,     0,   884,   885,     0,   150,     0,     0,     0,     0,
     150,     0,     0,     0,     0,     0,  1070,     0,     0,     0,
       0,     0,     0,   135,  1076,     0,   446,   141,   446,   141,
       0,     0,     0,     0,   141,     0,   741,   141,   414,     0,
     823,     0,     0,     0,   141,   146,     0,   141,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -245,   153,     0,     0,  -245,  -245,     0,     0,     0,
       0,     0,   141,     0,  -245,   150,  -245,  -245,     0,     0,
       0,  -245,     0,   141,     0,   141,     0,     0,  -245,     0,
     153,  -245,     0,     0,     0,     0,   153,     0,     0,     0,
     348,     0,   135,   153,   349,     0,     0,     0,     0,   135,
    -245,     0,  -245,     0,  -245,     0,  -245,  -245,     0,  -245,
       0,  -245,     0,  -245,   146,     0,     0,     0,     0,     0,
       0,   146,     0,   754,   756,     0,  1070,     0,     0,   761,
     764,     0,  -245,     0,     0,  -245,     0,     0,  -245,     0,
       0,   351,   352,     0,   150,     0,   355,     0,   356,   357,
     153,   150,     0,   360,   153,     0,     0,     0,   135,     0,
       0,   367,   153,     0,     0,   141,     0,   371,   372,   373,
       0,     0,   141,   135,     0,     0,     0,   271,     0,     0,
     146,    22,    23,     0,   141,     0,     0,     0,  -245,     0,
     272,     0,    30,   273,  -245,   146,     0,    35,     0,   141,
       0,     0,     0,     0,    40,     0,     0,     0,   141,     0,
     150,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   150,    53,     0,    55,     0,
      57,     0,   959,     0,     0,   960,     0,   274,     0,    63,
       0,     0,     0,     0,     0,     0,   135,     0,     0,     0,
       0,   894,   895,     0,     0,     0,     0,     0,    79,   141,
       0,    81,     0,   900,    83,     0,     0,     0,   146,     0,
       0,     0,     0,     0,   135,   911,   153,     0,   914,     0,
     135,   151,     0,     0,     0,     0,     0,   135,     0,     0,
       0,   151,     0,     0,     0,     0,   146,     0,   150,     0,
       0,     0,   146,     0,   151,     0,   151,     0,     0,   146,
       0,     0,     0,     0,    98,   141,   141,   141,   141,     0,
     961,     0,     0,     0,     0,     0,   150,     0,     0,   153,
       0,     0,   150,     0,     0,   153,     0,     0,     0,   150,
       0,   141,   141,     0,   135,     0,     0,     0,   135,     0,
       0,     0,   151,     0,     0,  1002,   135,     0,     0,     0,
       0,     0,     0,   414,   414,     0,   146,     0,     0,     0,
     146,     0,     0,     0,     0,   225,     0,     0,   146,   151,
       0,     0,     0,     0,   151,     0,     0,   347,     0,   226,
     227,     0,   228,     0,     0,     0,   150,   229,     0,     0,
     150,     0,   153,     0,   153,   230,     0,     0,   150,   153,
       0,   231,   153,     0,     0,     0,   232,     0,     0,   153,
     233,     0,   153,   234,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,  1061,     0,   236,   237,   348,
       0,     0,     0,   349,   238,     0,     0,   153,     0,   151,
    1017,     0,     0,   239,     0,     0,     0,     0,   153,     0,
     153,     0,   240,   241,     0,   242,     0,   243,     0,   244,
     135,     0,   245,     0,   350,     0,   246,   455,     0,   247,
       0,     0,   248,     0,     0,     0,     0,     0,     0,  1106,
     351,   352,   146,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,     0,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,     0,
       0,     0,   150,   135,     0,     0,     0,   374,   151,   135,
       0,     0,     0,     0,     0,   151,     0,     0,   456,     0,
       0,  1106,     0,     0,     0,   146,     0,     0,     0,     0,
     153,   146,     0,     0,     0,     0,     0,   153,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   153,
       0,     0,   271,     0,     0,   150,    22,    23,     0,     0,
    1106,   150,     0,     0,   153,   272,     0,    30,   273,     0,
       0,     0,    35,   153,   151,     0,   135,   153,   135,    40,
       0,     0,     0,   135,     0,     0,   135,     0,     0,   151,
       0,     0,     0,   135,     0,     0,   135,     0,   146,     0,
     146,    53,     0,    55,     0,   146,   155,     0,   146,     0,
       0,     0,   274,     0,    63,   146,   155,     0,   146,     0,
       0,   135,     0,     0,   153,     0,     0,     0,   150,   155,
     150,   155,   135,    79,   135,   150,    81,     0,   150,    83,
       0,     0,     0,   146,     0,   150,     0,     0,   150,     0,
       0,     0,     0,     0,   146,     0,   146,     0,     0,     0,
       0,     0,   151,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   150,     0,     0,     0,   155,     0,     0,
     153,   153,   153,   153,   150,     0,   150,     0,     0,     0,
     151,     0,     0,     0,     0,     0,   151,     0,     0,     0,
       0,     0,     0,   151,   155,     0,   153,   153,     0,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,     0,   156,   106,     0,     0,
       0,   135,     0,     0,     0,     0,   156,   167,     0,     0,
       0,     0,     0,   135,     0,     0,   146,     0,     0,   156,
     199,   156,   202,   146,     0,     0,     0,     0,   135,     0,
     151,     0,     0,     0,   151,   146,     0,   135,     0,     0,
       0,   135,   151,     0,   155,     0,   150,     0,     0,     0,
     146,     0,     0,   150,     0,     0,     0,     0,     0,   146,
       0,     0,     0,   146,     0,   150,     0,   156,   292,     0,
       0,   271,     0,     0,     0,    22,    23,     0,     0,     0,
     150,     0,     0,     0,   272,     0,    30,   273,   135,   150,
       0,    35,     0,   150,   156,   106,     0,     0,    40,   156,
     329,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,    55,   155,    57,     0,   959,     0,     0,   960,
     155,   274,     0,    63,     0,     0,     0,     0,     0,     0,
     150,     0,     0,     0,   135,   135,   135,   135,     0,     0,
       0,     0,    79,     0,     0,    81,   151,     0,    83,     0,
       0,     0,     0,     0,   156,   397,   146,   146,   146,   146,
     135,   135,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,     0,     0,   155,
       0,     0,   146,   146,     0,     0,   150,   150,   150,   150,
       0,     0,     0,  -525,   155,     0,     0,     0,    98,   151,
       0,     0,     0,     0,  1048,   151,   390,     0,     0,  -563,
       0,  -563,   150,   150,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   226,
     227,   347,   228,   156,   452,     0,     0,   229,     0,     0,
     156,   465,     0,     0,     0,   230,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,   232,     0,     0,     0,
     233,     0,     0,   234,     0,     0,     0,   155,     0,     0,
       0,   235,   151,     0,   151,     0,     0,   236,   237,   151,
       0,     0,   151,   348,   238,     0,     0,   349,   347,   151,
       0,     0,   151,   239,     0,   155,     0,     0,     0,   156,
     106,   155,   240,   241,     0,   242,     0,   243,   155,   244,
       0,     0,   245,     0,   156,   106,   246,   151,   350,   247,
       0,     0,   248,     0,     0,     0,     0,     0,   151,     0,
     151,     0,     0,     0,   351,   352,     0,   353,   354,   355,
     348,   356,   357,   358,   349,   359,   360,   361,   362,     0,
       0,   364,   365,   366,   367,   368,     0,   370,     0,     0,
     371,   372,   373,     0,     0,   155,     0,     0,     0,   155,
       0,   374,     0,     0,     0,     0,   403,   155,   803,     0,
       0,     0,   345,     0,     0,     0,     0,   156,   106,   346,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
       0,   347,     0,   360,     0,     0,   441,     0,     0,     0,
       0,   367,     0,     0,     0,   156,   465,   371,   372,   373,
     151,   156,   465,     0,     0,     0,     0,   151,   156,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   442,   151,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,   151,     0,     0,   349,     0,     0,
       0,     0,     0,   151,     0,     0,     0,   151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,   679,     0,   350,   156,
     684,   155,     0,     0,     0,     0,     0,   156,   106,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,   151,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   443,
     371,   372,   373,     0,     0,     0,     0,     0,   225,     0,
       0,   374,     0,     0,   155,     0,     0,     0,     0,     0,
     155,     0,   226,   227,     0,   228,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,     0,     0,   230,     0,
     151,   151,   151,   151,   231,     0,     0,     0,     0,   232,
       0,     0,     0,   233,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,     0,   151,   151,     0,     0,
     236,   237,     0,     0,     0,     0,     0,   238,     0,     0,
       0,   156,   772,     0,     0,     0,   239,   155,     0,   155,
       0,     0,     0,     0,   155,   240,   241,   155,   242,     0,
     243,     0,   244,     0,   155,   245,     0,   155,     0,   246,
       0,     0,   247,     0,     0,   248,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,     0,   155,     0,   156,   808,     0,     0,     0,     0,
     156,   813,   225,   155,     0,   155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,   227,     0,   228,
     390,     0,     0,  -563,   229,  -563,     0,     0,     0,     0,
       0,   252,   230,   345,     0,     0,     0,     0,   231,     0,
     346,     0,     0,   232,     0,     0,     0,   233,     0,     0,
     234,     0,   347,   763,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,   237,     0,   156,   465,   156,
     465,   238,     0,     0,   156,   465,     0,   156,   465,     0,
     239,     0,     0,     0,   156,   917,     0,   156,   919,   240,
     241,     0,   242,     0,   243,   155,   244,     0,     0,   245,
       0,     0,   155,   246,   348,     0,   247,     0,   349,   248,
       0,     0,   156,   936,   155,     0,     0,     0,     0,     0,
       0,     0,     0,   156,   942,   156,   946,     0,     0,   155,
       0,     0,     0,     0,     0,   434,     0,     0,   155,   350,
       0,     0,   155,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   781,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
       0,   371,   372,   373,     0,   403,     0,     0,     0,   155,
       0,     0,   374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   346,     0,     0,   156,     0,     0,     0,     0,
       0,     0,   156,     0,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   156,   996,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   155,   155,   155,   156,
    1016,     0,     0,     0,     0,     0,     0,     0,   156,  1021,
       0,     0,   156,  1023,     0,     0,     0,     0,     0,     0,
       0,   155,   155,     0,     0,     0,   348,     0,     0,     0,
     349,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   156,
    1056,   350,   445,   376,   377,   378,   379,   380,     0,     0,
     383,   384,   385,   386,     0,   388,   389,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,     0,   371,   372,   373,     0,   403,     0,     0,
       0,     0,     0,     0,   374,   156,   156,   156,   156,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    -2,
       4,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,   156,   156,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,    29,     0,    30,    31,    32,    33,
      34,    35,    36,    37,   -74,     0,    38,    39,    40,     0,
      41,  -259,     0,    42,    43,    44,     0,    45,    46,    47,
     -45,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -259,   -45,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -74,    83,    84,
       0,     0,    85,     0,    86,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,    98,     0,
       0,    99,     0,   100,     0,   101,     0,   102,     0,     4,
     103,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -593,     0,    12,    13,    14,    15,    16,  -593,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,  -593,
      27,    28,  -593,    29,     0,    30,    31,    32,    33,    34,
      35,    36,    37,   -74,     0,    38,    39,    40,     0,    41,
    -259,     0,    42,    43,    44,     0,    45,    46,    47,   -45,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -259,   -45,    60,    61,
      62,  -593,    63,    64,    65,  -593,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -74,    83,    84,     0,
       0,    85,     0,    86,     0,     0,  -593,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -593,  -593,    90,  -593,  -593,  -593,  -593,  -593,
    -593,  -593,     0,  -593,  -593,  -593,  -593,  -593,     0,  -593,
    -593,  -593,  -593,  -593,  -593,  -593,  -593,    98,  -593,  -593,
    -593,     0,   100,  -593,   101,     0,   102,     0,   312,  -593,
       5,   276,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,    29,     0,    30,    31,    32,    33,    34,    35,
      36,    37,   -74,     0,    38,    39,    40,     0,    41,  -259,
       0,    42,    43,    44,     0,    45,    46,    47,   -45,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,    57,    58,    59,  -259,   -45,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -74,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,   313,   101,     0,   102,     0,     4,   103,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,    36,
      37,   -74,     0,    38,    39,    40,     0,    41,  -259,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,    57,    58,    59,  -259,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -74,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,   492,   101,     0,   102,     0,   511,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,    36,    37,
     -74,     0,    38,    39,    40,     0,    41,  -259,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
      57,    58,    59,  -259,   -45,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -74,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
     512,   101,     0,   102,     0,   312,   103,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -74,
       0,    38,    39,    40,     0,    41,  -259,     0,    42,    43,
      44,     0,    45,    46,    47,   -45,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -259,   -45,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -74,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,   313,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,    36,    37,   -74,     0,
      38,    39,    40,     0,    41,  -259,     0,    42,    43,    44,
       0,    45,    46,    47,   -45,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,  -259,   -45,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -74,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,   691,   101,
       0,   102,     0,     4,   103,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,    36,    37,   -74,     0,    38,
      39,    40,     0,    41,  -259,     0,    42,    43,    44,     0,
      45,    46,    47,   -45,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,   328,    58,    59,
    -259,   -45,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -74,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,     4,   103,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,    29,     0,    30,    31,
      32,    33,    34,    35,    36,    37,   -74,     0,    38,    39,
      40,     0,    41,  -259,     0,    42,    43,    44,     0,    45,
      46,    47,   -45,    48,    49,     0,    50,    51,    52,     0,
       0,     0,    53,    54,    55,    56,     0,    58,    59,  -259,
     -45,    60,    61,    62,     0,    63,    64,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
       0,    76,    77,    78,    79,    80,     0,    81,    82,   -74,
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
      33,    34,    35,   608,    37,   -74,     0,    38,    39,    40,
       0,    41,  -259,     0,    42,    43,    44,     0,    45,    46,
      47,   -45,    48,    49,     0,    50,    51,    52,     0,     0,
       0,    53,    54,    55,    56,     0,    58,    59,  -259,   -45,
      60,    61,    62,     0,    63,    64,    65,     0,    66,    67,
      68,    69,    70,    71,    72,    73,     0,    74,    75,     0,
      76,    77,    78,    79,    80,     0,    81,    82,   -74,    83,
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
      34,    35,   614,    37,   -74,     0,    38,    39,    40,     0,
      41,  -259,     0,    42,    43,    44,     0,    45,    46,    47,
     -45,    48,    49,     0,    50,    51,    52,     0,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -259,   -45,    60,
      61,    62,     0,    63,    64,    65,     0,    66,    67,    68,
      69,    70,    71,    72,    73,     0,    74,    75,     0,    76,
      77,    78,    79,    80,     0,    81,    82,   -74,    83,    84,
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
      35,   902,    37,   -74,     0,    38,    39,    40,     0,    41,
    -259,     0,    42,    43,    44,     0,    45,    46,    47,   -45,
      48,    49,     0,    50,    51,    52,     0,     0,     0,    53,
      54,    55,    56,     0,    58,    59,  -259,   -45,    60,    61,
      62,     0,    63,    64,    65,     0,    66,    67,    68,    69,
      70,    71,    72,    73,     0,    74,    75,     0,    76,    77,
      78,    79,    80,     0,    81,    82,   -74,    83,    84,     0,
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
     904,    37,   -74,     0,    38,    39,    40,     0,    41,  -259,
       0,    42,    43,    44,     0,    45,    46,    47,   -45,    48,
      49,     0,    50,    51,    52,     0,     0,     0,    53,    54,
      55,    56,     0,    58,    59,  -259,   -45,    60,    61,    62,
       0,    63,    64,    65,     0,    66,    67,    68,    69,    70,
      71,    72,    73,     0,    74,    75,     0,    76,    77,    78,
      79,    80,     0,    81,    82,   -74,    83,    84,     0,     0,
      85,     0,    86,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,    98,     0,     0,    99,
       0,   100,     0,   101,     0,   102,     0,     4,   103,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,    29,     0,    30,    31,    32,    33,    34,    35,   909,
      37,   -74,     0,    38,    39,    40,     0,    41,  -259,     0,
      42,    43,    44,     0,    45,    46,    47,   -45,    48,    49,
       0,    50,    51,    52,     0,     0,     0,    53,    54,    55,
      56,     0,    58,    59,  -259,   -45,    60,    61,    62,     0,
      63,    64,    65,     0,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    74,    75,     0,    76,    77,    78,    79,
      80,     0,    81,    82,   -74,    83,    84,     0,     0,    85,
       0,    86,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,    98,     0,     0,    99,     0,
     100,     0,   101,     0,   102,     0,     4,   103,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
      29,     0,    30,    31,    32,    33,    34,    35,   912,    37,
     -74,     0,    38,    39,    40,     0,    41,  -259,     0,    42,
      43,    44,     0,    45,    46,    47,   -45,    48,    49,     0,
      50,    51,    52,     0,     0,     0,    53,    54,    55,    56,
       0,    58,    59,  -259,   -45,    60,    61,    62,     0,    63,
      64,    65,     0,    66,    67,    68,    69,    70,    71,    72,
      73,     0,    74,    75,     0,    76,    77,    78,    79,    80,
       0,    81,    82,   -74,    83,    84,     0,     0,    85,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,    98,     0,     0,    99,     0,   100,
       0,   101,     0,   102,     0,     4,   103,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,   935,    28,     0,    29,
       0,    30,    31,    32,    33,    34,    35,    36,    37,   -74,
       0,    38,    39,    40,     0,    41,  -259,     0,    42,    43,
      44,     0,    45,    46,    47,   -45,    48,    49,     0,    50,
      51,    52,     0,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -259,   -45,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,    69,    70,    71,    72,    73,
       0,    74,    75,     0,    76,    77,    78,    79,    80,     0,
      81,    82,   -74,    83,    84,     0,     0,    85,     0,    86,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,    98,     0,     0,    99,     0,   100,     0,
     101,     0,   102,     0,     4,   103,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,    29,     0,
      30,    31,    32,    33,    34,    35,   941,    37,   -74,     0,
      38,    39,    40,     0,    41,  -259,     0,    42,    43,    44,
       0,    45,    46,    47,   -45,    48,    49,     0,    50,    51,
      52,     0,     0,     0,    53,    54,    55,    56,     0,    58,
      59,  -259,   -45,    60,    61,    62,     0,    63,    64,    65,
       0,    66,    67,    68,    69,    70,    71,    72,    73,     0,
      74,    75,     0,    76,    77,    78,    79,    80,     0,    81,
      82,   -74,    83,    84,     0,     0,    85,     0,    86,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,    98,     0,     0,    99,     0,   100,     0,   101,
       0,   102,     0,     4,   103,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,    29,     0,    30,
      31,    32,    33,    34,    35,   945,    37,   -74,     0,    38,
      39,    40,     0,    41,  -259,     0,    42,    43,    44,     0,
      45,    46,    47,   -45,    48,    49,     0,    50,    51,    52,
       0,     0,     0,    53,    54,    55,    56,     0,    58,    59,
    -259,   -45,    60,    61,    62,     0,    63,    64,    65,     0,
      66,    67,    68,    69,    70,    71,    72,    73,     0,    74,
      75,     0,    76,    77,    78,    79,    80,     0,    81,    82,
     -74,    83,    84,     0,     0,    85,     0,    86,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,    98,     0,     0,    99,     0,   100,     0,   101,     0,
     102,     0,  1127,   103,     5,   276,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
    1128,   791,   103,     5,   276,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,   219,    23,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,    63,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,    83,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     0,   103,     5,
     276,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,   280,   281,     0,
      80,   317,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
     318,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     0,   103,     5,   276,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   179,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,   280,   281,     0,    80,
     317,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   182,
       0,   101,   675,   183,     0,     0,   103,     5,   276,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,   280,   281,     0,    80,   317,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,   677,   183,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,   573,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,   574,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,   575,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
     576,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   179,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,   280,
     281,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,   282,     0,     0,
      99,     0,   182,     0,   101,     0,   183,     0,     0,   103,
       5,   276,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,   280,   281,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,   793,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,   280,   281,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,   183,     0,     5,   103,     6,     7,     8,   208,
      10,    11,   209,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   210,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,   211,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   179,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,   213,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,   214,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   182,     0,   101,     0,   183,     0,     0,   103,
       5,   276,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,   214,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,   280,   281,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,   183,     0,     0,   103,     5,   276,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,   673,   101,
       0,   183,     0,     0,   103,     5,   276,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   176,     0,     0,
       0,    32,   177,   178,     0,   179,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   180,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   182,     0,   101,     0,
     183,   683,     0,   103,     5,   276,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
    1059,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   179,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,   214,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   182,     0,   101,     0,   183,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,   256,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   179,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   100,
       0,   101,     0,   183,     0,     0,   103,     5,   276,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    28,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,    39,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,   181,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,   183,     0,     5,   103,     6,     7,     8,   208,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   210,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,   211,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   179,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   182,     0,   101,     0,   183,   688,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   179,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,     0,     0,     0,    80,
     819,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   182,
       0,   101,     0,   183,     0,     0,   103,     5,   276,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   647,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,   648,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,   649,    70,    71,    72,   650,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,  1000,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,  1025,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   183,
       0,     0,   103,     5,   276,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,  1000,     0,
       0,   103,     5,   276,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   982,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   182,     0,   101,     0,   183,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   179,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     5,   103,     6,     7,
       8,   204,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   205,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   183,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   179,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   100,     0,   101,     0,   183,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,   619,
       0,   182,     0,   101,     0,   183,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   647,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,   648,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,   649,    70,    71,    72,   650,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,   651,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,    89,    90,    91,    92,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,    95,    96,    97,
       0,     0,     0,    99,     0,   182,     0,   101,     0,   651,
       0,     5,   103,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   174,     0,     0,    15,    16,     0,    17,     0,
     175,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   176,     0,     0,     0,    32,   177,   178,
       0,   730,    37,     0,     0,     0,    39,     0,     0,    41,
       0,     0,     0,    43,    44,     0,     0,    46,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
      54,     0,    56,     0,    58,     0,     0,     0,     0,    61,
     180,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,    72,   181,     0,    74,     0,     0,    76,     0,
       0,     0,    80,     0,     0,    82,     0,     0,    84,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,    89,    90,    91,    92,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      99,     0,   182,     0,   101,     0,   183,     0,     5,   103,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   733,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,     0,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,     0,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,     0,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,    95,    96,    97,     0,     0,     0,    99,     0,   182,
       0,   101,     0,   183,     0,     5,   103,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   174,     0,     0,    15,
      16,     0,    17,     0,   175,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,    28,     0,   176,     0,     0,
       0,    32,   177,   178,     0,   976,    37,     0,     0,     0,
      39,     0,     0,    41,     0,     0,     0,    43,    44,     0,
       0,    46,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,    54,     0,    56,     0,    58,     0,
       0,     0,     0,    61,   180,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,   181,     0,    74,
       0,     0,    76,     0,     0,     0,    80,     0,     0,    82,
       0,     0,    84,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,    89,    90,    91,
      92,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,    95,    96,
      97,     0,     0,     0,    99,     0,   182,     0,   101,     0,
     183,     0,     5,   103,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   977,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    89,    90,    91,    92,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,    95,    96,    97,     0,     0,
       0,    99,     0,   182,     0,   101,     0,   183,     0,     5,
     103,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     174,     0,     0,    15,    16,     0,    17,     0,   175,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    28,
       0,   176,     0,     0,     0,    32,   177,   178,     0,   979,
      37,     0,     0,     0,    39,     0,     0,    41,     0,     0,
       0,    43,    44,     0,     0,    46,    47,     0,    48,    49,
       0,    50,    51,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,   180,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
      72,   181,     0,    74,     0,     0,    76,     0,     0,     0,
      80,     0,     0,    82,     0,     0,    84,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    90,    91,    92,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,    95,    96,    97,     0,     0,     0,    99,     0,
     182,     0,   101,     0,   183,     0,     5,   103,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   980,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,    95,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   183,     0,     5,   103,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   174,     0,     0,    15,    16,     0,
      17,     0,   175,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,    28,     0,   176,     0,     0,     0,    32,
     177,   178,     0,   981,    37,     0,     0,     0,    39,     0,
       0,    41,     0,     0,     0,    43,    44,     0,     0,    46,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,    54,     0,    56,     0,    58,     0,     0,     0,
       0,    61,   180,     0,     0,     0,     0,     0,     0,     0,
      68,    69,    70,    71,    72,   181,     0,    74,     0,     0,
      76,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,    95,    96,    97,     0,
       0,     0,    99,     0,   182,     0,   101,     0,   183,     0,
       5,   103,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      28,     0,   176,     0,     0,     0,    32,   177,   178,     0,
     982,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,    95,    96,    97,     0,     0,     0,    99,
       0,   182,     0,   101,     0,   183,     0,     5,   103,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   174,     0,
       0,    15,    16,     0,    17,     0,   175,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   647,     0,   176,
       0,     0,     0,    32,   177,   178,     0,   179,    37,     0,
       0,     0,   648,     0,     0,    41,     0,     0,     0,    43,
      44,     0,     0,    46,    47,     0,    48,    49,     0,    50,
      51,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,   180,     0,     0,     0,
       0,     0,     0,     0,    68,   649,    70,    71,    72,   650,
       0,    74,     0,     0,    76,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,    89,
      90,    91,    92,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
      95,    96,    97,     0,     0,     0,    99,     0,   182,     0,
     101,     0,  1083,     0,     5,   103,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,    28,     0,   176,     0,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     5,
       0,    76,     0,     0,     0,    80,     0,     0,    82,     0,
       0,    84,     0,    15,     0,     0,    17,     0,     0,    87,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   176,   831,     0,   832,    88,    89,    90,    91,    92,
      37,     0,     0,   833,    93,     0,     0,    41,   834,   227,
     835,   836,     0,     0,    94,    46,   837,    95,    96,    97,
       0,     0,    51,    99,   230,     0,   176,   101,     0,  1083,
     838,     0,   103,     0,     0,   839,     0,    61,     0,   233,
       0,     0,   840,     0,   841,     0,     0,     0,     0,    71,
     842,     0,     0,    74,     0,     0,   843,   844,     0,     0,
      80,     0,     0,   238,     0,     0,    84,     0,     0,     0,
       0,     0,   845,     0,     0,     0,     0,     0,     0,     0,
       0,   240,   241,     0,   846,     0,   243,     0,   847,     0,
       0,   848,     0,     0,     0,   849,     0,     0,   247,     0,
       0,   850,     0,     0,     0,     0,     0,     0,     0,     0,
     445,   376,   377,   378,   379,   380,     0,     0,   383,   384,
     385,   386,     0,   388,   389,   851,   852,   853,   854,   855,
       0,     0,   856,     0,     0,     0,   857,   858,   859,   860,
     861,   862,   863,   864,   865,   866,   867,     0,   868,     0,
       0,   869,   870,   871,   872,     0,     5,   873,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   174,     0,     0,
      15,    16,     0,    17,     0,   175,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,    28,     0,   176,     0,
       0,     0,    32,   177,   178,     0,   179,    37,     0,     0,
       0,    39,     0,     0,    41,     0,     0,     0,    43,    44,
       0,     0,    46,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,   180,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,   181,     0,
      74,     0,     0,    76,     0,     0,     0,    80,     0,     0,
      82,     0,     0,    84,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,     0,     0,
      96,    97,     0,     0,     0,    99,     0,   182,     0,   101,
       0,   183,     5,   276,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,   347,    17,
       0,   175,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    28,     0,   176,     0,     0,     0,    32,   177,
     178,     0,   179,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
     348,    54,     0,    56,   349,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,  -422,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,     0,   360,     0,    90,     0,     0,     0,  -422,
     366,   367,     0,  -422,   370,     0,     0,   371,   372,   373,
       0,     0,     0,     0,     0,     0,     0,    97,   374,     0,
       0,     0,     0,   182,     0,   101,  -422,  1000,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   174,
       0,     0,    15,    16,     0,    17,     0,   175,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,    28,     0,
     176,     0,     0,     0,    32,   177,   178,     0,   179,    37,
       0,     0,     0,    39,     0,     0,    41,     0,     0,     0,
      43,    44,  -404,     0,    46,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,    54,     0,    56,
       0,    58,     0,     0,     0,     0,    61,   180,     0,     0,
       0,  -404,     0,     0,     0,    68,    69,    70,    71,    72,
     181,     0,    74,     0,  -404,    76,     0,     0,     0,    80,
       0,     0,    82,     0,     0,    84,     0,  -404,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,    90,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   174,     0,     0,    15,    16,     0,    17,     0,   175,
       0,     0,    21,    97,  -404,     0,     0,     0,     0,  -404,
      28,   101,   176,  1083,     0,     0,    32,   177,   178,     0,
     179,    37,     0,     0,     0,    39,     0,     0,    41,     0,
       0,     0,    43,    44,     0,     0,    46,    47,     0,    48,
      49,     0,    50,    51,     0,     0,     0,     0,     0,    54,
       0,    56,     0,    58,     0,     0,     0,     0,    61,   180,
       0,     0,     0,     0,     0,     0,     0,    68,    69,    70,
      71,    72,   181,     0,    74,     0,     0,    76,     0,     0,
       0,    80,     0,     0,    82,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     5,    90,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   174,     0,     0,    15,    16,     0,    17,
       0,   175,     0,     0,    21,    97,     0,     0,     0,     0,
       0,   182,    28,   101,   176,   183,     0,     0,    32,   177,
     178,     0,   179,    37,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,    43,    44,     0,     0,    46,    47,
       0,    48,    49,     0,    50,    51,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,   180,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,    72,   181,     0,    74,     0,     0,    76,
       0,     0,     0,    80,     0,     0,    82,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    90,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   174,     0,     0,    15,    16,
       0,    17,     0,   175,     0,     0,    21,    97,     0,     0,
       0,     0,     0,   182,    28,   101,   176,   651,     0,     0,
      32,   177,   178,     0,   179,    37,     0,     0,     0,    39,
       0,     0,    41,     0,     0,     0,    43,    44,     0,     0,
      46,    47,     0,    48,    49,     0,    50,    51,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,   180,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,   181,     0,    74,     0,
       0,    76,     0,  1037,     0,    80,     0,     0,    82,     0,
       0,    84,   833,     0,     0,     0,     0,   226,   227,   835,
     228,     0,     0,     0,     0,   229,     0,   271,     0,     0,
       0,    22,    23,   230,     0,     0,     0,    90,     0,   838,
     272,     0,    30,   273,   232,     0,     0,    35,   233,     0,
       0,   234,     0,   841,    40,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,   843,   237,   101,     0,   183,
       0,     0,   238,     0,     0,     0,    53,     0,    55,     0,
     328,   239,   959,     0,     0,   960,     0,   274,     0,    63,
     240,   241,     0,   242,   345,   243,     0,  1038,     0,     0,
     848,   346,     0,     0,   246,     0,     0,   247,    79,     0,
     248,    81,     0,   347,    83,     0,     0,     0,     0,   445,
     376,   377,   378,   379,   380,     0,     0,   383,   384,   385,
     386,     0,   388,   389,   851,   852,   853,   854,   855,     0,
       0,   856,     0,     0,     0,   857,   858,   859,   860,   861,
     862,   863,   864,   865,   866,   867,     0,   868,     0,     0,
     869,   870,   871,   872,    98,   348,     0,     0,     0,   349,
     345,     0,     0,     0,     0,     0,     0,   346,   901,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,   445,   376,   377,   378,   379,   380,     0,     0,   383,
     384,   385,   386,     0,   388,   389,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,   403,     0,     0,     0,
     345,     0,     0,   374,   908,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,   350,   445,   376,   377,
     378,   379,   380,     0,     0,   383,   384,   385,   386,     0,
     388,   389,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,   403,     0,     0,   349,   345,     0,     0,   374,
       0,     0,     0,   346,  1051,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,   445,   376,   377,
     378,   379,   380,     0,     0,   383,   384,   385,   386,     0,
     388,   389,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,   403,     0,     0,     0,   345,     0,     0,   374,
    1052,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   350,   445,   376,   377,   378,   379,   380,     0,
       0,   383,   384,   385,   386,     0,   388,   389,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,   403,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
    1053,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   350,   445,   376,   377,   378,   379,   380,     0,
       0,   383,   384,   385,   386,     0,   388,   389,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,   348,   371,   372,   373,   349,   403,     0,
       0,     0,   345,     0,     0,   374,  1054,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,   350,   445,
     376,   377,   378,   379,   380,     0,     0,   383,   384,   385,
     386,     0,   388,   389,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,   348,   403,     0,     0,   349,   345,     0,
       0,   374,     0,     0,     0,   346,  1077,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,   445,
     376,   377,   378,   379,   380,     0,     0,   383,   384,   385,
     386,     0,   388,   389,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,   403,     0,     0,     0,   345,     0,
       0,   374,  1078,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,   437,     0,
       0,     0,     0,     0,   350,   445,   376,   377,   378,   379,
     380,     0,   438,   383,   384,   385,   386,     0,   388,   389,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,     0,   371,   372,   373,   348,
     403,     0,     0,   349,   345,     0,     0,   374,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   347,   431,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   350,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   348,   371,   372,   373,   349,
     403,     0,     0,     0,     0,     0,     0,   374,     0,     0,
       0,     0,     0,     0,   345,   699,     0,     0,     0,     0,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,   347,     0,     0,     0,     0,   700,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
     701,     0,   371,   372,   373,     0,   403,     0,     0,     0,
       0,     0,     0,   374,     0,   348,     0,     0,     0,   349,
     345,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     350,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
       0,   348,   371,   372,   373,   349,     0,     0,     0,     0,
     345,     0,     0,   374,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
     256,     0,   169,     0,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,     0,   371,   372,
     373,   348,     0,     0,     0,   349,   345,     0,     0,   374,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   582,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   350,     0,     0,     0,
     583,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   348,   371,   372,
     373,   349,   403,     0,     0,     0,   345,     0,     0,   374,
       0,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,   584,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,     0,     0,
     585,     0,     0,     0,     0,     0,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,   348,     0,     0,
       0,   349,   345,     0,     0,   374,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   755,     0,     0,     0,     0,     0,     0,     0,
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
     371,   372,   373,   348,   403,     0,     0,   349,   345,     0,
       0,   374,     0,     0,     0,   346,   773,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,   348,
     371,   372,   373,   349,   403,     0,     0,     0,     0,     0,
       0,   374,     0,     0,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
     434,     0,     0,     0,   350,     0,     0,   347,     0,     0,
       0,     0,   954,     0,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,   955,   345,   371,   372,   373,     0,
       0,     0,   346,     0,     0,   811,     0,   374,     0,   348,
       0,     0,     0,   349,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   345,     0,     0,     0,     0,
       0,     0,   346,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,   348,   356,   357,   358,
     349,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,   345,     0,   371,   372,   373,     0,
       0,   346,     0,     0,     0,     0,     0,   374,     0,     0,
       0,   350,     0,   347,     0,     0,   348,     0,     0,     0,
     349,     0,     0,     0,     0,     0,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,   350,     0,   371,   372,   373,     0,   477,     0,     0,
       0,     0,     0,     0,   374,   348,     0,   351,   352,   349,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,   345,   481,   371,   372,   373,     0,     0,   346,     0,
     350,     0,     0,     0,   374,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,   351,   352,     0,   353,
     354,   355,     0,   356,   357,   358,     0,   359,   360,   361,
     362,   363,     0,   364,   365,   366,   367,   368,   369,   370,
     345,   483,   371,   372,   373,     0,     0,   346,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,   347,
       0,     0,   348,     0,     0,     0,   349,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,   346,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,   348,     0,   351,   352,   349,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   485,   371,
     372,   373,     0,     0,     0,     0,   350,     0,     0,     0,
     374,     0,     0,     0,     0,     0,     0,   348,     0,     0,
       0,   349,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,   345,   490,   371,   372,
     373,   507,   350,   346,     0,     0,     0,     0,     0,   374,
       0,     0,     0,     0,     0,   347,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,   345,     0,   371,   372,   373,     0,     0,   346,
       0,     0,     0,     0,     0,   374,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,   348,     0,     0,
       0,   349,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,   345,   695,     0,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,   350,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,     0,     0,     0,   349,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,     0,   350,     0,
       0,     0,     0,   348,     0,   374,     0,   349,     0,     0,
       0,     0,     0,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,   350,     0,
     371,   372,   373,     0,     0,   345,     0,   674,     0,     0,
       0,   374,   346,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   696,   358,   347,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,     0,     0,     0,     0,     0,     0,     0,
     345,   374,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
     349,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   347,
       0,   350,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,     0,     0,     0,   349,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,  -526,   364,   365,   366,   367,   368,   369,
     370,     0,   169,   371,   372,   373,   350,     0,     0,     0,
       0,   348,     0,     0,   374,   349,     0,     0,     0,     0,
       0,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,     0,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,   350,     0,   371,   372,
     373,     0,     0,   345,   768,     0,     0,     0,     0,   374,
     346,     0,   351,   352,     0,   353,   354,   355,     0,   356,
     357,   358,   347,   359,   360,   361,   362,   363,     0,   364,
     365,   366,   367,   368,   369,   370,     0,   740,   371,   372,
     373,     0,     0,   345,     0,     0,     0,     0,     0,   374,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,   771,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,   349,     0,
       0,     0,     0,   345,     0,     0,     0,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,   350,
       0,     0,     0,     0,   348,     0,     0,     0,   349,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,   350,
       0,   371,   372,   373,   348,     0,     0,     0,   349,     0,
       0,     0,   374,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,   350,
       0,   371,   372,   373,     0,     0,   345,     0,     0,     0,
       0,     0,   374,   346,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   347,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
     271,   371,   372,   373,    22,    23,     0,   345,   795,     0,
       0,     0,   374,   272,   346,    30,   273,     0,     0,     0,
      35,     0,     0,   -75,     0,     0,   347,    40,     0,     0,
    -259,     0,     0,     0,     0,     0,     0,   348,     0,     0,
       0,   349,     0,     0,     0,     0,     0,   345,     0,    53,
       0,    55,     0,     0,   346,     0,  -259,     0,     0,     0,
     274,     0,    63,     0,     0,     0,   347,   888,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,   348,     0,
       0,    79,   349,     0,    81,     0,   -75,    83,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,   814,   350,   371,   372,   373,     0,   348,     0,
       0,   796,   349,     0,     0,   374,     0,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,   350,     0,   371,   372,   373,     0,     0,
       0,     0,     0,     0,     0,     0,   374,     0,     0,   351,
     352,     0,   353,   354,   355,     0,   356,   357,   358,     0,
     359,   360,   361,   362,   363,     0,   364,   365,   366,   367,
     368,   369,   370,   345,  -240,   371,   372,   373,     0,     0,
     346,     0,     0,     0,     0,     0,   374,     0,  -240,  -240,
       0,  -240,   347,   891,     0,     0,  -240,     0,     0,     0,
       0,     0,     0,     0,  -240,     0,     0,     0,     0,     0,
    -240,     0,     0,   345,     0,  -240,     0,     0,     0,  -240,
     346,     0,  -240,     0,     0,     0,     0,     0,     0,     0,
    -240,     0,   347,     0,     0,     0,  -240,  -240,     0,     0,
       0,     0,     0,  -240,   348,     0,     0,     0,   349,     0,
       0,     0,  -240,   345,   951,     0,     0,     0,     0,     0,
     346,  -240,  -240,     0,  -240,     0,  -240,     0,  -240,     0,
       0,  -240,   347,     0,     0,  -240,     0,     0,  -240,   350,
       0,  -240,     0,     0,   348,     0,     0,     0,   349,     0,
       0,     0,     0,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,   350,
       0,   371,   372,   373,   348,     0,     0,     0,   349,     0,
       0,     0,   374,     0,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,     0,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,   350,
       0,   371,   372,   373,     0,     0,   345,     0,     0,     0,
     893,     0,   374,   346,     0,   351,   352,     0,   353,   354,
     355,     0,   356,   357,   358,   347,   359,   360,   361,   362,
     363,     0,   364,   365,   366,   367,   368,   369,   370,     0,
       0,   371,   372,   373,     0,     0,   345,     0,     0,     0,
       0,     0,   374,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,     0,     0,
       0,   349,     0,     0,     0,     0,   345,   994,     0,     0,
     975,     0,     0,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,   348,     0,     0,
       0,   349,     0,     0,     0,     0,     0,     0,   351,   352,
     978,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,   350,     0,   371,   372,   373,   348,     0,     0,
       0,   349,     0,     0,     0,   374,     0,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,     0,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,   350,     0,   371,   372,   373,     0,     0,   345,
       0,     0,     0,     0,     0,   374,   346,     0,   351,   352,
       0,   353,   354,   355,     0,   356,   357,   358,   347,   359,
     360,   361,   362,   363,     0,   364,   365,   366,   367,   368,
     369,   370,     0,     0,   371,   372,   373,     0,     0,   345,
       0,     0,     0,     0,     0,   374,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,     0,     0,   349,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   350,     0,     0,     0,     0,
     348,     0,     0,     0,   349,     0,     0,     0,     0,     0,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,     0,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,   350,     0,   371,   372,   373,
       0,     0,   345,     0,  1036,     0,     0,     0,   374,   346,
       0,   351,   352,     0,   353,   354,   355,     0,   356,   357,
     358,   347,   359,   360,   361,   362,   363,     0,   364,   365,
     366,   367,   368,   369,   370,     0,     0,   371,   372,   373,
       0,     0,   345,     0,     0,     0,  1073,     0,   374,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   348,     0,     0,     0,   349,     0,     0,
       0,     0,   345,     0,     0,     0,  1092,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,     0,   348,     0,     0,     0,   349,     0,     0,
       0,     0,     0,     0,   351,   352,  1093,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,   350,     0,
     371,   372,   373,   348,     0,     0,     0,   349,     0,     0,
       0,   374,     0,     0,   351,   352,  1094,   353,   354,   355,
       0,   356,   357,   358,     0,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,   350,     0,
     371,   372,   373,     0,     0,   345,     0,     0,     0,     0,
       0,   374,   346,     0,   351,   352,     0,   353,   354,   355,
       0,   356,   357,   358,   347,   359,   360,   361,   362,   363,
       0,   364,   365,   366,   367,   368,   369,   370,     0,     0,
     371,   372,   373,     0,     0,   345,     0,     0,     0,     0,
       0,   374,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   348,     0,     0,     0,
     349,     0,     0,     0,     0,   345,     0,     0,     0,  1095,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,   350,     0,     0,     0,     0,   348,     0,     0,     0,
     349,     0,     0,     0,     0,     0,     0,   351,   352,  1096,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,   350,     0,   371,   372,   373,   348,     0,     0,     0,
     349,     0,     0,     0,   374,     0,     0,   351,   352,  1097,
     353,   354,   355,     0,   356,   357,   358,     0,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,   350,     0,   371,   372,   373,     0,     0,   345,     0,
       0,     0,     0,     0,   374,   346,     0,   351,   352,     0,
     353,   354,   355,     0,   356,   357,   358,   347,   359,   360,
     361,   362,   363,     0,   364,   365,   366,   367,   368,   369,
     370,     0,     0,   371,   372,   373,     0,     0,   345,     0,
       0,     0,     0,  1121,   374,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,   349,     0,     0,     0,     0,   345,     0,
       0,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,   348,
       0,     0,     0,   349,     0,     0,     0,     0,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,   350,     0,   371,   372,   373,   348,
       0,     0,     0,   349,     0,     0,     0,   374,     0,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
       0,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,   350,     0,   371,   372,   373,     0,
       0,   345,     0,     0,     0,  1150,     0,   374,   346,     0,
     351,   352,     0,   353,   354,   355,     0,   356,   357,   358,
     347,   359,   360,   361,   362,   363,     0,   364,   365,   366,
     367,   368,   369,   370,     0,   271,   371,   372,   373,    22,
      23,     0,     0,     0,     0,     0,     0,   374,   272,     0,
      30,   273,     0,     0,     0,    35,     0,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   728,     0,     0,     0,   349,     0,     0,     0,
       0,     0,     0,     0,    53,     0,    55,     0,    57,     0,
     959,     0,     0,   960,     0,   274,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   350,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,    81,
       0,     0,    83,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,     0,   359,   360,   361,   362,   363,     0,
     364,   365,   366,   367,   368,   369,   370,     0,   271,   371,
     372,   373,    22,    23,     0,     0,     0,     0,     0,     0,
     374,   272,     0,    30,   273,     0,   271,     0,    35,     0,
      22,    23,    98,     0,     0,    40,     0,     0,  1050,   272,
       0,    30,   273,     0,     0,     0,    35,     0,     0,     0,
     347,     0,     0,    40,     0,     0,     0,    53,     0,    55,
       0,    57,     0,   959,     0,     0,   960,     0,   274,     0,
      63,     0,     0,     0,     0,    53,     0,    55,     0,    57,
       0,   959,     0,     0,   960,   347,   274,     0,    63,    79,
       0,     0,    81,     0,     0,    83,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,   349,    79,     0,     0,
      81,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   348,     0,     0,
       0,   349,     0,     0,     0,    98,     0,     0,     0,     0,
       0,  1098,     0,   351,   352,     0,   353,   354,   355,     0,
     356,   357,   358,    98,   359,   360,   361,   362,     0,  1099,
     364,   365,   366,   367,   368,     0,   370,     0,     0,   371,
     372,   373,     0,     0,     0,     0,     0,     0,   351,   352,
     374,   353,   354,   355,     0,   356,   357,   358,     0,     0,
     360,   361,   362,     0,     0,   364,   365,   366,   367,     0,
     704,   370,   225,     0,   371,   372,   373,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   226,   227,     0,   228,
       0,     0,     0,     0,   229,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
     234,     0,  -250,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,   237,  -250,  -250,     0,  -250,
       0,   238,     0,     0,  -250,     0,     0,     0,     0,     0,
     239,     0,  -250,     0,     0,     0,     0,     0,  -250,   240,
     241,     0,   242,  -250,   243,     0,   244,  -250,     0,   245,
    -250,     0,   225,   246,     0,     0,   247,     0,  -250,   248,
       0,     0,     0,     0,  -250,  -250,   226,   227,     0,   228,
       0,  -250,     0,     0,   229,     0,     0,     0,     0,     0,
    -250,     0,   230,     0,     0,     0,     0,     0,   231,  -250,
    -250,     0,  -250,   232,  -250,     0,  -250,   233,     0,  -250,
     234,     0,  -241,  -250,     0,     0,  -250,     0,   235,  -250,
       0,     0,     0,     0,   236,   237,  -241,  -241,     0,  -241,
       0,   238,     0,     0,  -241,     0,     0,     0,     0,     0,
     239,     0,  -241,     0,     0,     0,     0,     0,  -241,   240,
     241,     0,   242,  -241,   243,     0,   244,  -241,     0,   245,
    -241,     0,  -172,   246,     0,     0,   247,     0,  -241,   248,
       0,     0,     0,     0,  -241,  -241,  -172,  -172,     0,  -172,
       0,  -241,     0,     0,  -172,     0,     0,     0,     0,     0,
    -241,     0,  -172,     0,     0,     0,     0,     0,  -172,  -241,
    -241,     0,  -241,  -172,  -241,     0,  -241,  -172,     0,  -241,
    -172,     0,  -168,  -241,     0,     0,  -241,     0,  -172,  -241,
       0,     0,     0,     0,  -172,  -172,  -168,  -168,     0,  -168,
       0,  -172,     0,     0,  -168,     0,     0,     0,     0,     0,
    -172,     0,  -168,     0,     0,     0,     0,     0,  -168,  -172,
    -172,     0,  -172,  -168,  -172,     0,  -172,  -168,     0,  -172,
    -168,     0,     0,  -172,     0,     0,  -172,     0,  -168,  -172,
       0,     0,     0,     0,  -168,  -168,     0,     0,     0,     0,
       0,  -168,     0,     0,     0,     0,     0,     0,     0,     0,
    -168,     0,     0,     0,     0,     0,     0,     0,     0,  -168,
    -168,     0,  -168,     0,  -168,     0,  -168,     0,     0,  -168,
       0,     0,     0,  -168,     0,     0,  -168,     0,     0,  -168
};

static const yytype_int16 yycheck[] =
{
      12,    64,   153,    60,    16,   224,   461,   486,    20,   203,
     396,    19,   442,     2,    26,   612,   790,   713,    30,    31,
     195,    33,    34,    35,    36,   218,   701,    46,   139,   700,
     224,   486,    42,    45,  1010,    47,   211,   646,   456,   102,
      52,     1,    54,    22,    56,    32,    46,   114,    86,   824,
     269,     1,    64,    65,    66,    67,    68,    69,   878,   879,
      32,    73,    69,    75,    32,    77,    78,   925,    32,    79,
      82,   922,   291,    85,    86,   269,    88,   114,    90,    57,
      45,    93,    94,     1,    96,   196,    32,   140,   100,   101,
     102,   103,  1059,     0,    46,     3,     1,   291,   140,     1,
     600,    36,   169,   603,   111,   159,  1111,    27,     3,   883,
       3,   140,    20,    83,   166,   453,   169,    87,   727,    69,
     183,   140,   132,   165,   134,    33,   164,   114,   725,    46,
     323,   324,   169,    68,   112,    46,    46,   149,   167,    57,
     140,    46,   114,   108,  1111,  1150,     3,   298,   999,     3,
     169,    59,   926,    73,  1130,    57,    33,    34,   165,   119,
     114,   111,   174,   357,    84,   177,   178,   179,    86,   181,
     182,   183,   140,   392,   393,  1033,   166,   164,   787,   139,
     140,    99,    46,   103,    86,   195,   194,   962,   140,    97,
    1128,   203,   164,    57,   112,   103,   164,   114,   392,   393,
     164,   211,    97,   213,    97,    77,    78,   167,  1146,   984,
     112,   986,   224,   270,   119,   165,    18,   169,   164,  1039,
    1040,   139,    86,   140,   998,   102,   612,   645,   207,   140,
     140,    65,    86,   212,   139,   140,    27,   139,   112,    46,
     219,   159,   252,    97,   744,   134,   164,   747,   112,    46,
       8,    84,   169,   142,   159,   119,   164,   269,   169,   169,
     479,   273,   164,    52,    94,    57,   140,   101,   280,   281,
      72,   701,  1047,   164,  1049,   971,   140,    84,   953,   291,
     955,   952,    73,   954,  1058,   479,   166,    84,    90,    78,
      57,    45,   140,    84,    86,   159,   140,   166,    86,   168,
     164,   140,   140,   167,   166,   107,   318,    99,   897,   906,
     899,   422,   103,    67,    79,   159,   427,   165,   109,    86,
     112,   333,   334,    77,    78,   207,   315,   159,   167,   167,
     212,   343,    99,   345,   346,   347,   348,   349,   350,   725,
     352,   353,   354,   355,   356,   112,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   781,   119,   158,   429,   159,   140,   381,
     382,   159,   164,    57,   140,   387,   164,   159,   390,   391,
     392,   393,   394,   395,   396,   140,   119,   817,   159,   144,
      27,   140,   159,   405,   983,   167,   985,   164,   410,   159,
     412,   167,    86,   150,   598,   165,    45,   140,   159,    46,
     420,   144,   167,    50,   424,    99,   119,   429,   167,   431,
     432,   159,    61,   616,   617,   437,   438,    27,   112,   441,
     442,   140,   444,    70,   167,   447,    73,    27,    75,   193,
      79,   779,   145,   159,   147,   148,   456,    84,   151,   152,
      83,   463,   140,   932,    87,   467,   468,   520,   167,    49,
     472,   215,   216,   217,   218,   159,   103,   479,   222,   108,
     925,   348,   349,    73,   119,   159,   166,   932,   168,   167,
     164,    71,   494,    73,    84,   497,   824,   499,   681,   682,
      31,   503,   504,   505,    84,   507,   508,   672,   166,   144,
     140,   695,   696,   140,   514,   259,   140,   134,   520,   109,
     140,   265,   522,    31,   134,   142,     2,   394,   395,   109,
     906,   140,   142,   953,   513,   955,    12,   167,   405,   159,
     167,   165,   164,   287,    27,   140,   497,   159,   499,    25,
     159,    27,    83,   159,   140,   140,    87,   381,   382,   140,
     166,   305,   168,   387,   159,   432,   390,   391,   140,   113,
     140,   438,   713,   159,   159,    83,   159,    82,   159,    87,
     582,   583,   584,   585,   586,   587,   588,   159,  1033,   159,
      73,   589,   590,   166,   596,   168,   598,    73,   651,   599,
      82,    84,   602,   134,   606,   166,   608,   168,   139,   166,
     612,   142,   614,   134,  1059,   146,   159,   166,   139,   168,
     103,   142,   165,   159,   100,   146,   134,    77,  1073,   105,
      80,   139,   166,   141,   142,   119,   503,   166,   146,   967,
     166,   641,   168,   166,    46,   645,   154,   649,   650,   651,
     166,     8,   159,   655,   162,    57,   984,    61,   986,   139,
     164,   142,   406,    64,   159,   119,  1111,   167,   668,   140,
     159,    46,   672,   497,    46,   499,     3,   167,   166,    34,
     140,   167,   119,  1128,    86,    57,    21,   140,   167,   167,
     164,   693,   436,    20,   170,   697,   440,   699,   700,   701,
     169,  1146,   169,    31,   166,  1150,    33,   159,   159,   167,
     112,   159,   159,   713,    86,   159,   583,   119,   585,  1047,
     159,  1049,   140,   725,   933,   934,   728,   729,   730,   169,
     159,   733,    59,   159,    31,   159,    34,    61,   140,   167,
     112,   140,   165,   119,   165,   167,   799,   119,    84,   933,
     934,   753,   167,   755,    32,    83,    32,   159,   760,    87,
     169,   763,   164,    34,   766,   167,   768,   164,   140,   771,
      97,   164,   596,   249,   139,   775,   103,    84,   778,   169,
     256,   781,   166,   166,   169,   139,    83,   159,   169,   159,
      87,   101,   164,   159,   165,   167,   142,   799,   166,   142,
     142,   134,   142,   803,   142,   142,   134,   809,   134,   811,
     142,   139,   814,   141,   142,   817,   134,   142,   146,   142,
     142,   142,   134,  1064,   826,   134,   154,   827,   142,    34,
     971,   655,   160,   161,   162,   164,   167,   134,   135,   315,
      67,   138,   139,   159,   141,   142,   159,   164,   167,   146,
      99,   159,   167,   159,   330,   159,   167,   154,   158,   134,
     134,   728,   729,   160,   161,   162,   610,   611,   158,   330,
     169,   873,   616,   617,  1115,  1116,  1117,  1118,  1119,  1120,
     140,   450,   693,    24,   628,   887,   888,   299,   890,   891,
     892,   893,   803,   203,   827,   971,   967,   779,   638,   901,
     902,   830,   904,  1130,   906,  1058,   908,   909,  1088,  1121,
     912,     2,  1114,   960,   224,    35,   464,   598,    30,    -1,
      -1,    12,    -1,    -1,   668,    -1,    -1,   403,    -1,   931,
     932,   933,   934,    -1,    25,    -1,    27,   939,    -1,   941,
      -1,    -1,    -1,   945,    -1,    -1,    -1,  1000,    -1,   951,
     952,   953,   954,   955,    -1,   431,    -1,    -1,    -1,   269,
      -1,   437,    -1,    -1,    -1,    -1,    -1,    -1,   444,    -1,
      -1,   971,    -1,   975,   976,   977,   978,   979,   980,   981,
     982,   291,    73,    -1,    -1,    -1,    77,    78,   215,   216,
     217,   218,   994,    -1,   738,   739,    -1,    27,  1000,    -1,
      -1,    -1,    -1,  1005,    -1,    -1,   750,  1009,   752,   100,
      -1,    -1,    -1,    -1,   105,   759,    46,    -1,   762,    -1,
      50,   765,    -1,    -1,    -1,   501,    -1,    -1,    -1,   505,
    1083,    -1,   259,    -1,    -1,    -1,    -1,   513,   265,    -1,
      70,    -1,    -1,    73,    -1,    75,    -1,    -1,    -1,  1051,
    1052,  1053,  1054,    -1,    84,    -1,    -1,    -1,    -1,    -1,
     287,    -1,   806,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1073,    -1,   103,    -1,  1077,  1078,    27,   305,   170,
      -1,  1083,   392,   393,    -1,    -1,    -1,    -1,    -1,    -1,
    1092,  1093,  1094,  1095,  1096,  1097,    46,    -1,    -1,    -1,
      50,    -1,    -1,   579,    -1,     2,    -1,    -1,  1110,  1111,
      -1,  1113,    31,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    73,    -1,    75,    -1,     2,    25,    -1,
      27,    -1,    -1,    -1,    84,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,  1146,    -1,    -1,    -1,  1150,    -1,
      25,   627,    27,   103,    -1,    -1,    -1,     2,   249,    -1,
      -1,    -1,   472,    -1,    83,   256,    -1,    12,    87,   479,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,   406,
      25,    -1,    27,    -1,   494,    -1,    -1,   497,    -1,   499,
      -1,    -1,    -1,   937,   504,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,   100,   680,    -1,    -1,    -1,   105,   436,
     686,    -1,    -1,   440,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   315,   100,    -1,   146,    73,    -1,
     105,    -1,    -1,    -1,   153,   154,    -1,   713,   157,   330,
      -1,   160,   161,   162,   988,   989,    -1,    -1,   992,   993,
      -1,    -1,   728,   729,    -1,   100,    -1,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,  1010,    -1,    -1,    -1,
      -1,    -1,    -1,   170,  1018,    -1,  1020,   753,  1022,   755,
      -1,    -1,    -1,    -1,   760,    -1,   596,   763,   598,    -1,
       1,    -1,    -1,    -1,   770,   170,    -1,   773,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,   403,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,   798,    -1,    35,   170,    37,    38,    -1,    -1,
      -1,    42,    -1,   809,    -1,   811,    -1,    -1,    49,    -1,
     431,    52,    -1,    -1,    -1,    -1,   437,    -1,    -1,    -1,
      83,    -1,   249,   444,    87,    -1,    -1,    -1,    -1,   256,
      71,    -1,    73,    -1,    75,    -1,    77,    78,    -1,    80,
      -1,    82,    -1,    84,   249,    -1,    -1,    -1,    -1,    -1,
      -1,   256,    -1,   610,   611,    -1,  1130,    -1,    -1,   616,
     617,    -1,   103,    -1,    -1,   106,    -1,    -1,   109,    -1,
      -1,   134,   135,    -1,   249,    -1,   139,    -1,   141,   142,
     501,   256,    -1,   146,   505,    -1,    -1,    -1,   315,    -1,
      -1,   154,   513,    -1,    -1,   901,    -1,   160,   161,   162,
      -1,    -1,   908,   330,    -1,    -1,    -1,    22,    -1,    -1,
     315,    26,    27,    -1,   920,    -1,    -1,    -1,   159,    -1,
      35,    -1,    37,    38,   165,   330,    -1,    42,    -1,   935,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,   944,    -1,
     315,    -1,   948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   330,    71,    -1,    73,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    82,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,   403,    -1,    -1,    -1,
      -1,   738,   739,    -1,    -1,    -1,    -1,    -1,   103,   995,
      -1,   106,    -1,   750,   109,    -1,    -1,    -1,   403,    -1,
      -1,    -1,    -1,    -1,   431,   762,   627,    -1,   765,    -1,
     437,     2,    -1,    -1,    -1,    -1,    -1,   444,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   431,    -1,   403,    -1,
      -1,    -1,   437,    -1,    25,    -1,    27,    -1,    -1,   444,
      -1,    -1,    -1,    -1,   159,  1051,  1052,  1053,  1054,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   431,    -1,    -1,   680,
      -1,    -1,   437,    -1,    -1,   686,    -1,    -1,    -1,   444,
      -1,  1077,  1078,    -1,   501,    -1,    -1,    -1,   505,    -1,
      -1,    -1,    73,    -1,    -1,   925,   513,    -1,    -1,    -1,
      -1,    -1,    -1,   933,   934,    -1,   501,    -1,    -1,    -1,
     505,    -1,    -1,    -1,    -1,     3,    -1,    -1,   513,   100,
      -1,    -1,    -1,    -1,   105,    -1,    -1,    31,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,   501,    25,    -1,    -1,
     505,    -1,   753,    -1,   755,    33,    -1,    -1,   513,   760,
      -1,    39,   763,    -1,    -1,    -1,    44,    -1,    -1,   770,
      48,    -1,   773,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    -1,  1005,    -1,    65,    66,    83,
      -1,    -1,    -1,    87,    72,    -1,    -1,   798,    -1,   170,
     937,    -1,    -1,    81,    -1,    -1,    -1,    -1,   809,    -1,
     811,    -1,    90,    91,    -1,    93,    -1,    95,    -1,    97,
     627,    -1,   100,    -1,   118,    -1,   104,   105,    -1,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,  1059,
     134,   135,   627,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,    -1,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    -1,
      -1,    -1,   627,   680,    -1,    -1,    -1,   171,   249,   686,
      -1,    -1,    -1,    -1,    -1,   256,    -1,    -1,   166,    -1,
      -1,  1111,    -1,    -1,    -1,   680,    -1,    -1,    -1,    -1,
     901,   686,    -1,    -1,    -1,    -1,    -1,   908,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   920,
      -1,    -1,    22,    -1,    -1,   680,    26,    27,    -1,    -1,
    1150,   686,    -1,    -1,   935,    35,    -1,    37,    38,    -1,
      -1,    -1,    42,   944,   315,    -1,   753,   948,   755,    49,
      -1,    -1,    -1,   760,    -1,    -1,   763,    -1,    -1,   330,
      -1,    -1,    -1,   770,    -1,    -1,   773,    -1,   753,    -1,
     755,    71,    -1,    73,    -1,   760,     2,    -1,   763,    -1,
      -1,    -1,    82,    -1,    84,   770,    12,    -1,   773,    -1,
      -1,   798,    -1,    -1,   995,    -1,    -1,    -1,   753,    25,
     755,    27,   809,   103,   811,   760,   106,    -1,   763,   109,
      -1,    -1,    -1,   798,    -1,   770,    -1,    -1,   773,    -1,
      -1,    -1,    -1,    -1,   809,    -1,   811,    -1,    -1,    -1,
      -1,    -1,   403,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   798,    -1,    -1,    -1,    73,    -1,    -1,
    1051,  1052,  1053,  1054,   809,    -1,   811,    -1,    -1,    -1,
     431,    -1,    -1,    -1,    -1,    -1,   437,    -1,    -1,    -1,
      -1,    -1,    -1,   444,   100,    -1,  1077,  1078,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   901,    -1,     2,     2,    -1,    -1,
      -1,   908,    -1,    -1,    -1,    -1,    12,    12,    -1,    -1,
      -1,    -1,    -1,   920,    -1,    -1,   901,    -1,    -1,    25,
      25,    27,    27,   908,    -1,    -1,    -1,    -1,   935,    -1,
     501,    -1,    -1,    -1,   505,   920,    -1,   944,    -1,    -1,
      -1,   948,   513,    -1,   170,    -1,   901,    -1,    -1,    -1,
     935,    -1,    -1,   908,    -1,    -1,    -1,    -1,    -1,   944,
      -1,    -1,    -1,   948,    -1,   920,    -1,    73,    73,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
     935,    -1,    -1,    -1,    35,    -1,    37,    38,   995,   944,
      -1,    42,    -1,   948,   100,   100,    -1,    -1,    49,   105,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     995,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    73,   249,    75,    -1,    77,    -1,    -1,    80,
     256,    82,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
     995,    -1,    -1,    -1,  1051,  1052,  1053,  1054,    -1,    -1,
      -1,    -1,   103,    -1,    -1,   106,   627,    -1,   109,    -1,
      -1,    -1,    -1,    -1,   170,   170,  1051,  1052,  1053,  1054,
    1077,  1078,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,   315,
      -1,    -1,  1077,  1078,    -1,    -1,  1051,  1052,  1053,  1054,
      -1,    -1,    -1,   150,   330,    -1,    -1,    -1,   159,   680,
      -1,    -1,    -1,    -1,   165,   686,   163,    -1,    -1,   166,
      -1,   168,  1077,  1078,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    31,    20,   249,   249,    -1,    -1,    25,    -1,    -1,
     256,   256,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    -1,    -1,   403,    -1,    -1,
      -1,    59,   753,    -1,   755,    -1,    -1,    65,    66,   760,
      -1,    -1,   763,    83,    72,    -1,    -1,    87,    31,   770,
      -1,    -1,   773,    81,    -1,   431,    -1,    -1,    -1,   315,
     315,   437,    90,    91,    -1,    93,    -1,    95,   444,    97,
      -1,    -1,   100,    -1,   330,   330,   104,   798,   118,   107,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,   809,    -1,
     811,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      83,   141,   142,   143,    87,   145,   146,   147,   148,    -1,
      -1,   151,   152,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,   501,    -1,    -1,    -1,   505,
      -1,   171,    -1,    -1,    -1,    -1,   164,   513,   166,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   403,   403,    19,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
      -1,    31,    -1,   146,    -1,    -1,    36,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,   431,   431,   160,   161,   162,
     901,   437,   437,    -1,    -1,    -1,    -1,   908,   444,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,   920,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,   935,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,   944,    -1,    -1,    -1,   948,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   501,   501,    -1,   118,   505,
     505,   627,    -1,    -1,    -1,    -1,    -1,   513,   513,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,   995,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,   159,
     160,   161,   162,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,   171,    -1,    -1,   680,    -1,    -1,    -1,    -1,    -1,
     686,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
    1051,  1052,  1053,  1054,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    -1,  1077,  1078,    -1,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,   627,   627,    -1,    -1,    -1,    81,   753,    -1,   755,
      -1,    -1,    -1,    -1,   760,    90,    91,   763,    93,    -1,
      95,    -1,    97,    -1,   770,   100,    -1,   773,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,   798,    -1,   680,   680,    -1,    -1,    -1,    -1,
     686,   686,     3,   809,    -1,   811,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
     163,    -1,    -1,   166,    25,   168,    -1,    -1,    -1,    -1,
      -1,   166,    33,    12,    -1,    -1,    -1,    -1,    39,    -1,
      19,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    31,    32,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,   753,   753,   755,
     755,    72,    -1,    -1,   760,   760,    -1,   763,   763,    -1,
      81,    -1,    -1,    -1,   770,   770,    -1,   773,   773,    90,
      91,    -1,    93,    -1,    95,   901,    97,    -1,    -1,   100,
      -1,    -1,   908,   104,    83,    -1,   107,    -1,    87,   110,
      -1,    -1,   798,   798,   920,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   809,   809,   811,   811,    -1,    -1,   935,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,   944,   118,
      -1,    -1,   948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,   166,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    -1,   164,    -1,    -1,    -1,   995,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   901,    -1,    -1,    -1,    -1,
      -1,    -1,   908,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   920,   920,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1051,  1052,  1053,  1054,   935,
     935,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   944,   944,
      -1,    -1,   948,   948,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1077,  1078,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   995,
     995,   118,   119,   120,   121,   122,   123,   124,    -1,    -1,
     127,   128,   129,   130,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,   171,  1051,  1052,  1053,  1054,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,  1077,  1078,    14,    15,    16,    17,    18,    -1,    20,
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
      -1,   162,    -1,   164,    -1,   166,    -1,   168,    -1,     1,
     171,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    -1,    51,
      52,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    67,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    97,    98,    -1,   100,   101,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,   115,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,   166,    -1,   168,    -1,     1,   171,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     156,   157,   158,   159,    -1,    -1,   162,    -1,   164,   165,
     166,    -1,   168,    -1,     1,   171,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    54,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    66,
      67,    -1,    -1,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    -1,    84,    85,    86,
      -1,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      97,    98,    -1,   100,   101,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,    -1,    -1,   162,    -1,   164,   165,   166,
      -1,   168,    -1,     1,   171,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    -1,    51,    52,    -1,    54,    55,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    67,
      -1,    -1,    -1,    71,    72,    73,    74,    75,    76,    77,
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
     168,    -1,     1,   171,     3,     4,     5,     6,     7,     8,
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
     169,     1,   171,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,
      43,    44,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    55,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    -1,    65,    66,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,    -1,
      -1,   104,    -1,    -1,   107,    -1,   109,   110,    -1,    -1,
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
      94,    95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,
     104,   105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
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
      95,    -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,
     105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,   167,   168,    -1,    -1,   171,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,   105,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,   167,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    46,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
     109,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
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
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,
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
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    -1,
      76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,     3,   171,     5,     6,     7,     8,
       9,    10,    11,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,   103,   104,    -1,    -1,   107,    -1,
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
      72,    73,    74,    -1,    76,    -1,    -1,    -1,    -1,    81,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    94,    95,    -1,    97,    -1,    -1,   100,    -1,
      -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,
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
      -1,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,    -1,
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
      -1,    97,    -1,    -1,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,
     166,    -1,   168,    -1,    -1,   171,     3,     4,     5,     6,
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
     157,   158,    -1,    -1,    -1,   162,    -1,   164,   165,   166,
      -1,   168,    -1,    -1,   171,     3,     4,     5,     6,     7,
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
     168,   169,    -1,   171,     3,     4,     5,     6,     7,     8,
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
     169,     3,   171,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,   156,   157,   158,    -1,    -1,    -1,
     162,    -1,   164,    -1,   166,    -1,   168,    -1,     3,   171,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
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
      -1,   166,    -1,   168,    -1,    -1,   171,     3,     4,     5,
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
      -1,   100,    -1,    -1,   103,   104,    -1,    -1,   107,    -1,
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
     162,    -1,   164,    -1,   166,    -1,   168,   169,     3,   171,
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
     105,    -1,   107,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,
      -1,   156,   157,   158,    -1,    -1,    -1,   162,    -1,   164,
      -1,   166,    -1,   168,    -1,    -1,   171,     3,     4,     5,
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
      -1,   100,    -1,    -1,    -1,   104,   105,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,   156,   157,   158,
      -1,    -1,    -1,   162,    -1,   164,    -1,   166,    -1,   168,
      -1,    -1,   171,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,     3,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    17,    -1,    -1,    20,    -1,    -1,   118,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,     1,    -1,     3,   134,   135,   136,   137,   138,
      44,    -1,    -1,    12,   143,    -1,    -1,    51,    17,    18,
      19,    20,    -1,    -1,   153,    59,    25,   156,   157,   158,
      -1,    -1,    66,   162,    33,    -1,    35,   166,    -1,   168,
      39,    -1,   171,    -1,    -1,    44,    -1,    81,    -1,    48,
      -1,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    93,
      59,    -1,    -1,    97,    -1,    -1,    65,    66,    -1,    -1,
     104,    -1,    -1,    72,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    93,    -1,    95,    -1,    97,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,   123,   124,    -1,    -1,   127,   128,
     129,   130,    -1,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,   141,    -1,    -1,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,   157,    -1,
      -1,   160,   161,   162,   163,    -1,     3,   166,     5,     6,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,   136,
     137,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,
     157,   158,    -1,    -1,    -1,   162,    -1,   164,    -1,   166,
      -1,   168,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    31,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      83,    72,    -1,    74,    87,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,    -1,   146,    -1,   136,    -1,    -1,    -1,   140,
     153,   154,    -1,   144,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   171,    -1,
      -1,    -1,    -1,   164,    -1,   166,   167,   168,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    -1,    43,    44,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      65,    66,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      -1,    76,    -1,    -1,    -1,    -1,    81,    82,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    90,    91,    92,    93,    94,
      95,    -1,    97,    -1,    99,   100,    -1,    -1,    -1,   104,
      -1,    -1,   107,    -1,    -1,   110,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,   136,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   158,   159,    -1,    -1,    -1,    -1,   164,
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
      -1,   134,     3,   136,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,   158,    -1,    -1,    -1,    -1,
      -1,   164,    33,   166,    35,   168,    -1,    -1,    39,    40,
      41,    -1,    43,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    76,    -1,    -1,    -1,    -1,
      81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,   136,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,   158,    -1,    -1,
      -1,    -1,    -1,   164,    33,   166,    35,   168,    -1,    -1,
      39,    40,    41,    -1,    43,    44,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    62,    63,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,    -1,
      -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    94,    95,    -1,    97,    -1,
      -1,   100,    -1,     3,    -1,   104,    -1,    -1,   107,    -1,
      -1,   110,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    22,    -1,    -1,
      -1,    26,    27,    33,    -1,    -1,    -1,   136,    -1,    39,
      35,    -1,    37,    38,    44,    -1,    -1,    42,    48,    -1,
      -1,    51,    -1,    53,    49,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    65,    66,   166,    -1,   168,
      -1,    -1,    72,    -1,    -1,    -1,    71,    -1,    73,    -1,
      75,    81,    77,    -1,    -1,    80,    -1,    82,    -1,    84,
      90,    91,    -1,    93,    12,    95,    -1,    97,    -1,    -1,
     100,    19,    -1,    -1,   104,    -1,    -1,   107,   103,    -1,
     110,   106,    -1,    31,   109,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,   136,   137,   138,    -1,
      -1,   141,    -1,    -1,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,   157,    -1,    -1,
     160,   161,   162,   163,   159,    83,    -1,    -1,    -1,    87,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    96,    -1,
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
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,    -1,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,   164,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,    -1,
      -1,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    83,   160,   161,   162,    87,   164,    -1,
      -1,    -1,    12,    -1,    -1,   171,    96,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    83,   164,    -1,    -1,    87,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,   119,
     120,   121,   122,   123,   124,    -1,    -1,   127,   128,   129,
     130,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,   164,    -1,    -1,    -1,    12,    -1,
      -1,   171,    96,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,   121,   122,   123,
     124,    -1,    46,   127,   128,   129,   130,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    -1,   160,   161,   162,    83,
     164,    -1,    -1,    87,    12,    -1,    -1,   171,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    83,   160,   161,   162,    87,
     164,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      68,    -1,   160,   161,   162,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    83,    -1,    -1,    -1,    87,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      -1,    83,   160,   161,   162,    87,    -1,    -1,    -1,    -1,
      12,    -1,    -1,   171,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    -1,   160,   161,
     162,    83,    -1,    -1,    -1,    87,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,    83,   160,   161,
     162,    87,   164,    -1,    -1,    -1,    12,    -1,    -1,   171,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    83,    -1,    -1,
      -1,    87,    12,    -1,    -1,   171,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     160,   161,   162,    83,   164,    -1,    -1,    87,    12,    -1,
      -1,   171,    -1,    -1,    -1,    19,    96,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    83,
     160,   161,   162,    87,   164,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    68,    12,   160,   161,   162,    -1,
      -1,    -1,    19,    -1,    -1,   169,    -1,   171,    -1,    83,
      -1,    -1,    -1,    87,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    83,   141,   142,   143,
      87,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    12,    -1,   160,   161,   162,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,   118,    -1,    31,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   118,    -1,   160,   161,   162,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    83,    -1,   134,   135,    87,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    12,   159,   160,   161,   162,    -1,    -1,    19,    -1,
     118,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
      12,   159,   160,   161,   162,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    83,    -1,   134,   135,    87,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,   159,   160,
     161,   162,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    12,   159,   160,   161,
     162,   117,   118,    19,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    12,    -1,   160,   161,   162,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    87,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    -1,   118,    -1,
      -1,    -1,    -1,    83,    -1,   171,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    -1,    -1,    12,    -1,   167,    -1,    -1,
      -1,   171,    19,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    31,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,   171,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    87,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,   114,   160,   161,   162,   118,    -1,    -1,    -1,
      -1,    83,    -1,    -1,   171,    87,    -1,    -1,    -1,    -1,
      -1,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,   118,    -1,   160,   161,
     162,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,   171,
      19,    -1,   134,   135,    -1,   137,   138,   139,    -1,   141,
     142,   143,    31,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,   156,   157,    -1,   159,   160,   161,
     162,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   171,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   118,
      -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,   171,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   118,
      -1,   160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   171,    19,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    31,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      22,   160,   161,   162,    26,    27,    -1,    12,   167,    -1,
      -1,    -1,   171,    35,    19,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    31,    49,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    12,    -1,    71,
      -1,    73,    -1,    -1,    19,    -1,    78,    -1,    -1,    -1,
      82,    -1,    84,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      -1,   103,    87,    -1,   106,    -1,   108,   109,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   117,   118,   160,   161,   162,    -1,    83,    -1,
      -1,   167,    87,    -1,    -1,   171,    -1,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   118,    -1,   160,   161,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,   134,
     135,    -1,   137,   138,   139,    -1,   141,   142,   143,    -1,
     145,   146,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,    12,     3,   160,   161,   162,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   171,    -1,    17,    18,
      -1,    20,    31,    32,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    12,    -1,    44,    -1,    -1,    -1,    48,
      19,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    31,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    -1,    -1,    72,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    81,    12,    13,    -1,    -1,    -1,    -1,    -1,
      19,    90,    91,    -1,    93,    -1,    95,    -1,    97,    -1,
      -1,   100,    31,    -1,    -1,   104,    -1,    -1,   107,   118,
      -1,   110,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   118,
      -1,   160,   161,   162,    83,    -1,    -1,    -1,    87,    -1,
      -1,    -1,   171,    -1,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    -1,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   118,
      -1,   160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,
     169,    -1,   171,    19,    -1,   134,   135,    -1,   137,   138,
     139,    -1,   141,   142,   143,    31,   145,   146,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,    -1,
      -1,   160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   171,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      96,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
      96,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   118,    -1,   160,   161,   162,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,   171,    -1,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,   118,    -1,   160,   161,   162,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   171,    19,    -1,   134,   135,
      -1,   137,   138,   139,    -1,   141,   142,   143,    31,   145,
     146,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
     156,   157,    -1,    -1,   160,   161,   162,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   171,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    -1,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   118,    -1,   160,   161,   162,
      -1,    -1,    12,    -1,   167,    -1,    -1,    -1,   171,    19,
      -1,   134,   135,    -1,   137,   138,   139,    -1,   141,   142,
     143,    31,   145,   146,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,    -1,    -1,   160,   161,   162,
      -1,    -1,    12,    -1,    -1,    -1,   169,    -1,   171,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    96,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,   134,   135,    96,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    83,    -1,    -1,    -1,    87,    -1,    -1,
      -1,   171,    -1,    -1,   134,   135,    96,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,   118,    -1,
     160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   171,    19,    -1,   134,   135,    -1,   137,   138,   139,
      -1,   141,   142,   143,    31,   145,   146,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,   156,   157,    -1,    -1,
     160,   161,   162,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   171,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    96,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,    96,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   118,    -1,   160,   161,   162,    83,    -1,    -1,    -1,
      87,    -1,    -1,    -1,   171,    -1,    -1,   134,   135,    96,
     137,   138,   139,    -1,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,   118,    -1,   160,   161,   162,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   171,    19,    -1,   134,   135,    -1,
     137,   138,   139,    -1,   141,   142,   143,    31,   145,   146,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,   160,   161,   162,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    57,   171,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   118,    -1,   160,   161,   162,    83,
      -1,    -1,    -1,    87,    -1,    -1,    -1,   171,    -1,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,   118,    -1,   160,   161,   162,    -1,
      -1,    12,    -1,    -1,    -1,   169,    -1,   171,    19,    -1,
     134,   135,    -1,   137,   138,   139,    -1,   141,   142,   143,
      31,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,   156,   157,    -1,    22,   160,   161,   162,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,   171,    35,    -1,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,    -1,
      77,    -1,    -1,    80,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,   106,
      -1,    -1,   109,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,    -1,   145,   146,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,    -1,    22,   160,
     161,   162,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     171,    35,    -1,    37,    38,    -1,    22,    -1,    42,    -1,
      26,    27,   159,    -1,    -1,    49,    -1,    -1,   165,    35,
      -1,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      31,    -1,    -1,    49,    -1,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    77,    -1,    -1,    80,    -1,    82,    -1,
      84,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,    75,
      -1,    77,    -1,    -1,    80,    31,    82,    -1,    84,   103,
      -1,    -1,   106,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,   103,    -1,    -1,
     106,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    87,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,   165,    -1,   134,   135,    -1,   137,   138,   139,    -1,
     141,   142,   143,   159,   145,   146,   147,   148,    -1,   165,
     151,   152,   153,   154,   155,    -1,   157,    -1,    -1,   160,
     161,   162,    -1,    -1,    -1,    -1,    -1,    -1,   134,   135,
     171,   137,   138,   139,    -1,   141,   142,   143,    -1,    -1,
     146,   147,   148,    -1,    -1,   151,   152,   153,   154,    -1,
       1,   157,     3,    -1,   160,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,     3,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,     3,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    17,    18,    -1,    20,
      -1,    72,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    90,
      91,    -1,    93,    44,    95,    -1,    97,    48,    -1,   100,
      51,    -1,    -1,   104,    -1,    -1,   107,    -1,    59,   110,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    93,    -1,    95,    -1,    97,    -1,    -1,   100,
      -1,    -1,    -1,   104,    -1,    -1,   107,    -1,    -1,   110
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
     186,   188,   194,   195,   196,   199,   200,   204,   206,   209,
     210,   212,   213,   214,   219,   220,   222,   223,   224,   228,
     229,   232,   233,   236,   237,   240,   243,   244,   269,   272,
     273,   293,   294,   295,   296,   297,   298,   299,   307,   308,
     309,   310,   311,   314,   315,   316,   317,   318,   319,   320,
     321,   323,   324,   325,   326,   327,   159,   180,   311,   114,
     300,   301,     3,   201,    14,    22,    35,    40,    41,    43,
      82,    95,   164,   168,   240,   293,   298,   309,   310,   311,
     314,   316,   317,   300,   311,   103,   272,    84,   201,   180,
     287,   311,   180,   166,     8,    82,   311,   312,     8,    11,
      82,   103,   312,    73,   116,   221,   311,   221,   311,    26,
     273,   311,   311,   187,   166,     3,    17,    18,    20,    25,
      33,    39,    44,    48,    51,    59,    65,    66,    72,    81,
      90,    91,    93,    95,    97,   100,   104,   107,   110,   203,
     205,   241,   166,   203,   274,   275,    32,   188,   208,   311,
     311,    18,    72,    90,   107,   311,   311,   311,     8,   166,
     211,    22,    35,    38,    82,   212,     4,   263,   286,   311,
     101,   102,   159,   311,   313,   311,   208,   311,   311,   311,
      94,   166,   180,   311,   311,   188,   200,   311,   314,   188,
     200,   311,   203,   270,   311,   311,   311,   311,   311,   311,
     311,   311,     1,   165,   178,   189,   286,   105,   144,   263,
     288,   289,   313,   221,   286,   311,   322,   311,    75,   180,
     164,    79,   184,    45,   108,   203,   203,    52,    78,   258,
     274,   158,   159,   150,   311,    12,    19,    31,    83,    87,
     118,   134,   135,   137,   138,   139,   141,   142,   143,   145,
     146,   147,   148,   149,   151,   152,   153,   154,   155,   156,
     157,   160,   161,   162,   171,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     163,   250,   166,   168,    83,    87,   166,   180,   159,   311,
     311,   311,   286,   164,   188,    46,   300,   270,   274,   159,
     140,   159,   113,   204,   263,   290,   291,   292,   313,    82,
     217,   244,   272,    82,   215,   270,   244,   272,   203,   166,
     208,    32,    46,   208,   114,   208,   303,    32,    46,   208,
     303,    36,    68,   159,    96,   119,   188,   250,    77,    80,
     185,   290,   180,   166,   252,   105,   166,   203,   276,   277,
       1,   139,   281,    46,   140,   180,   208,   166,   166,   208,
     290,   212,   140,   159,   311,   311,   159,   164,   208,   166,
     290,   159,   225,   159,   225,   159,   119,   271,   159,   208,
     159,   165,   165,   178,   140,   165,   311,   140,   167,   140,
     167,   169,   303,    46,   140,   169,   303,   117,   140,   169,
       8,     1,   165,   189,    61,   197,   198,   311,   191,   311,
     139,   234,   164,   245,   159,   311,   311,   311,   311,   221,
     311,   221,   311,   311,   311,   311,   311,   311,   311,    20,
      33,    59,   103,   204,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,    64,   313,   313,   313,   313,   313,   290,   290,   221,
     311,   221,   311,    27,    46,    84,   109,   302,   305,   306,
     311,   327,    32,    46,    32,    46,    96,    46,   169,   221,
     311,   208,   159,   159,   311,   311,   119,   167,   140,   218,
     203,   274,   216,   203,   159,   274,    46,   286,    43,   311,
     221,   311,   166,   208,    43,   311,   221,   311,   208,   162,
     190,   193,   311,   190,   192,   180,   311,    34,   311,   184,
     167,    27,    46,    50,    70,    73,    84,   103,   179,   253,
     254,   255,   256,   242,   277,   140,   167,    33,    48,    91,
      95,   168,   207,   282,   294,   119,   278,   311,   275,   311,
     311,   167,   263,   311,     1,   230,   290,   167,    21,   226,
     282,   294,   140,   165,   167,   167,   288,   167,   288,   180,
     169,   221,   311,   169,   180,   311,   169,   311,   169,   311,
     165,   165,   203,   140,   159,    13,   142,   140,   159,    13,
      36,    68,   286,   164,     1,   203,   238,   239,    27,    73,
      84,   103,   247,   257,   166,   159,   159,   159,   159,   159,
     159,   167,   169,    46,    84,   140,   167,   293,    83,    83,
      43,   221,   311,    43,   221,   311,   221,   311,   300,   300,
     159,   263,   313,   292,   203,   234,   159,   203,   234,   159,
     311,   167,   311,    32,   208,    32,   208,   304,   305,   311,
      32,   208,   303,    32,   208,   303,   140,   159,    13,   159,
      34,    34,   180,    96,   188,    61,    46,    84,   255,   140,
     167,   166,   203,    27,    73,    84,   103,   259,   167,   277,
     281,     1,   286,    64,   313,   167,   167,   165,    69,   111,
     165,   231,   167,   166,   188,   203,   227,   270,   180,   169,
     303,   169,   303,   180,   117,   197,   204,   164,   311,   105,
     311,   193,   192,     1,   235,   165,   119,   140,   165,    84,
     246,     1,     3,    12,    17,    19,    20,    25,    39,    44,
      51,    53,    59,    65,    66,    81,    93,    97,   100,   104,
     110,   134,   135,   136,   137,   138,   141,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   157,   160,
     161,   162,   163,   166,   202,   203,   205,   248,   249,   250,
     293,   167,   305,   281,   293,   293,   311,    32,    32,   311,
      32,    32,   169,   169,   208,   208,   234,   164,   234,   164,
     208,    96,    43,   311,    43,   311,   140,   167,    96,    43,
     311,   208,    43,   311,   208,   311,   311,   180,   311,   180,
      34,   203,   203,   254,   277,   139,   285,    84,   281,   278,
     169,    46,   169,   166,   166,    32,   180,   286,   227,   139,
     188,    43,   180,   311,   169,    43,   180,   311,   169,   311,
     190,    13,    36,    68,    36,    68,   159,   159,   165,    77,
      80,   165,   179,   210,   311,   239,   259,   166,   251,   311,
     134,   142,   251,   251,   278,    96,    43,    43,    96,    43,
      43,    43,    43,   164,   235,   164,   235,   311,   311,   311,
     305,   311,   311,   311,    13,    34,   180,   159,   285,   167,
     168,   207,   263,   284,   294,   144,   264,   278,    57,   112,
     265,   311,   282,   294,   290,   290,   180,   208,   167,   311,
     311,   180,   311,   180,   165,   105,   311,   193,   192,   193,
     192,   210,     1,   139,   280,   253,   167,     3,    97,   249,
     250,   311,   311,   311,   311,   311,   311,   235,   165,   235,
     165,    96,    96,    96,    96,   311,   180,   278,   285,   169,
     286,   263,   311,     3,    86,    97,   266,   267,   268,   311,
     188,   209,   262,   169,   167,   167,   188,    96,    96,   159,
     159,   159,   159,   168,   207,   279,   294,    99,   260,   167,
     251,   251,    96,    96,    96,    96,    96,    96,   165,   165,
     311,   311,   311,   311,   264,   278,   263,   283,   284,   294,
      46,   169,   268,   112,   140,   119,   145,   147,   148,   151,
     152,    57,   294,   158,   158,   311,   311,     1,   169,   286,
     265,   311,   283,   284,   311,   267,   268,   268,   268,   268,
     268,   268,   266,   169,   279,   294,   169,   159,   261,   262,
     169,   279,   294,   283
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
     190,   190,   190,   190,   191,   191,   191,   191,   191,   191,
     192,   192,   193,   193,   194,   194,   194,   195,   195,   195,
     195,   195,   195,   195,   196,   197,   197,   197,   198,   198,
     199,   200,   200,   200,   200,   200,   200,   201,   201,   202,
     202,   202,   202,   202,   202,   203,   203,   203,   203,   203,
     203,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   207,   207,   207,   207,   208,   208,
     209,   209,   210,   210,   210,   211,   210,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   213,   213,   215,   214,
     216,   214,   217,   214,   218,   214,   214,   214,   214,   214,
     219,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   223,   224,   224,   224,
     224,   224,   224,   225,   225,   226,   226,   226,   227,   227,
     228,   229,   229,   230,   230,   231,   231,   231,   232,   232,
     233,   233,   233,   234,   234,   235,   235,   235,   236,   236,
     237,   238,   238,   238,   239,   239,   241,   242,   240,   243,
     243,   243,   245,   246,   244,   247,   247,   247,   247,   247,
     248,   248,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   251,   251,   252,   253,   253,   253,   254,   254,   254,
     254,   254,   255,   255,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   257,   257,   257,   257,   257,   257,   258,
     258,   259,   259,   259,   259,   259,   259,   260,   260,   261,
     261,   262,   262,   263,   264,   264,   264,   265,   265,   265,
     265,   265,   266,   266,   267,   267,   267,   267,   267,   267,
     267,   268,   268,   269,   269,   269,   270,   270,   271,   271,
     271,   272,   272,   272,   272,   272,   273,   273,   274,   274,
     275,   275,   276,   276,   276,   277,   277,   277,   278,   278,
     278,   279,   279,   279,   279,   279,   279,   279,   280,   280,
     280,   280,   280,   281,   281,   281,   281,   281,   282,   282,
     282,   282,   283,   283,   283,   284,   284,   284,   284,   284,
     285,   285,   285,   285,   285,   286,   286,   286,   286,   287,
     287,   288,   288,   288,   289,   289,   290,   290,   291,   291,
     292,   292,   292,   292,   293,   293,   294,   294,   294,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   297,   298,
     299,   299,   299,   299,   299,   299,   299,   299,   300,   300,
     301,   302,   302,   303,   304,   304,   305,   305,   305,   306,
     306,   306,   306,   306,   306,   307,   307,   308,   308,   308,
     308,   308,   309,   309,   309,   309,   309,   310,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   312,   312,   313,   313,   313,   314,
     314,   314,   314,   315,   315,   315,   315,   315,   316,   316,
     316,   317,   317,   317,   317,   317,   318,   318,   318,   318,
     319,   319,   320,   320,   321,   321,   321,   321,   321,   321,
     321,   321,   321,   321,   321,   321,   321,   322,   322,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   324,   324,   324,   324,   324,   324,   324,   325,
     325,   325,   325,   326,   326,   326,   326,   327,   327,   327,
     327,   327,   327,   327
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
       1,     3,     3,     5,     1,     3,     3,     3,     5,     5,
       0,     1,     1,     1,     0,     1,     1,     4,     6,     8,
       8,     6,     8,     8,     4,     1,     3,     5,     1,     3,
       3,     4,     4,     4,     4,     4,     4,     0,     1,     1,
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
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
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
       1,     1,     1,     1,     1,     1,     4,     3,     3,     3,
       2,     2,     2,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 506 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 6187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 511 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 6193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 512 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 6199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 519 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 6205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 6211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 525 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 6217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 6229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 549 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 6241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 550 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 6247 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 551 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 6253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 552 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 6259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 6265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 555 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 6277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 556 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6283 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 557 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 558 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 6295 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 559 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 6301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 560 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 6307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 561 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6313 "bison-chapel.cpp" /* yacc.c:1663  */
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
#line 6327 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 579 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pmodsymbol)->block = new BlockStmt();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-2].pmodsymbol)));
      currentModuleName = (yylsp[-2]).prevModule;  // restore previous module name
    }
#line 6336 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 584 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = (yyvsp[-1].pblockstmt);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6345 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 589 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-3].pmodsymbol)->block = buildErrorStandin();
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr((yyvsp[-3].pmodsymbol)));
      currentModuleName = (yylsp[-3]).prevModule;  // restore previous module name
    }
#line 6354 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 597 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6366 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6372 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 602 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 603 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6384 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 607 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6390 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6396 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 609 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6402 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    {
    (yylsp[0]).comment = context->latestComment;
    context->latestComment = NULL;
  }
#line 6411 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 619 "chapel.ypp" /* yacc.c:1663  */
    {
   (yyval.pblockstmt) = buildIncludeModule((yyvsp[-1].pch), (yyvsp[-4].b), (yyvsp[-3].b), (yylsp[-6]).comment);
 }
#line 6419 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 634 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6425 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 635 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 6431 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 57:
#line 636 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6437 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6443 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 59:
#line 642 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 6449 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 647 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6459 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 652 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6469 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 657 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 661 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 670 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 6497 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 675 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6507 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 680 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyval.ponlylist)->push_back(cur); }
#line 6517 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 685 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 6526 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 689 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6535 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 693 "chapel.ypp" /* yacc.c:1663  */
    { PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), new UnresolvedSymExpr("_"));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 6544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6554 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 710 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<PotentialRename*>();
                                         PotentialRename* cur = new PotentialRename();
                                         cur->tag = PotentialRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 6564 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 719 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6570 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 720 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6576 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 721 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; }
#line 6582 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 725 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].ponlylist), (yyvsp[-3].b)); }
#line 6588 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 726 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), true, (yyvsp[-5].b)); }
#line 6594 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 728 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), true, (yyvsp[-7].b)); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 81:
#line 729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), "", (yyvsp[-1].ponlylist), false, (yyvsp[-5].b)); }
#line 6612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 730 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), (yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6618 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 731 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-5].pexpr), new UnresolvedSymExpr("_"), (yyvsp[-1].ponlylist), false, (yyvsp[-7].b)); }
#line 6624 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);
                                               setImportPrivacy((yyval.pblockstmt), (yyvsp[-3].b));}
#line 6631 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 740 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[0].pexpr)); }
#line 6637 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 741 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6643 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pimportstmt) = buildImportStmt((yyvsp[-4].pexpr), (yyvsp[-1].ponlylist)); }
#line 6649 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pimportstmt)); }
#line 6655 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-2].pblockstmt); (yyval.pblockstmt)->insertAtTail((yyvsp[0].pimportstmt)); }
#line 6661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 90:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 6667 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 6673 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 758 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 6679 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 93:
#line 760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 6685 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 94:
#line 762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 6691 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 95:
#line 764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 6697 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 96:
#line 766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), new SymExpr(gNoInit), "="); }
#line 6703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 97:
#line 772 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 6709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 98:
#line 773 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6715 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6721 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 780 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 781 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6745 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6757 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 786 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "none"; redefiningReservedWordError((yyval.pch)); }
#line 6763 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 6769 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 6775 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 6781 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 790 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 6787 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6793 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 803 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "this"; }
#line 6799 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 814 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bool"; }
#line 6805 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "int"; }
#line 6811 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "uint"; }
#line 6817 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "real"; }
#line 6823 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "imag"; }
#line 6829 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "complex"; }
#line 6835 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 820 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "bytes"; }
#line 6841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 821 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "string"; }
#line 6847 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 822 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "sync"; }
#line 6853 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 823 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "single"; }
#line 6859 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 824 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "owned"; }
#line 6865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "shared"; }
#line 6871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 826 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "borrowed"; }
#line 6877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 827 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "unmanaged"; }
#line 6883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 828 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "domain"; }
#line 6889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 829 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "index"; }
#line 6895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 830 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "locale"; }
#line 6901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "nothing"; }
#line 6907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "void"; }
#line 6913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 836 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 837 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyEnumerated->symbol); }
#line 6925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 838 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 839 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 840 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 841 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6949 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 842 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 843 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBytes->symbol); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 845 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6973 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 846 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtNothing->symbol); }
#line 6979 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 847 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtVoid->symbol); }
#line 6985 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_syncvar"; }
#line 6991 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 855 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_singlevar"; }
#line 6997 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_domain"; }
#line 7003 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 857 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "_index"; }
#line 7009 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 861 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7015 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 862 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7021 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 866 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 7027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 867 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 7033 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 871 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 7039 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 873 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7045 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 883 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = parsingPrivate; parsingPrivate=true;}
#line 7051 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 884 "chapel.ypp" /* yacc.c:1663  */
    { parsingPrivate=(yyvsp[-1].b); applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 7057 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 898 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 7063 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 899 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 7069 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 900 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 7075 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 901 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 7081 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 906 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7090 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 911 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7104 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 921 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7113 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 926 "chapel.ypp" /* yacc.c:1663  */
    {

      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-6]).comment));
    }
#line 7128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 937 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7137 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 942 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 952 "chapel.ypp" /* yacc.c:1663  */
    {
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7160 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 957 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-7].pch),
                                             AGGREGATE_RECORD,
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXPORT,
                                             (yylsp[-6]).comment));
    }
#line 7174 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 968 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXTERN, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 972 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternExportFunctionDecl(FLAG_EXPORT, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt));
    }
#line 7190 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 977 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7202 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 985 "chapel.ypp" /* yacc.c:1663  */
    {
      const char* comment = context->latestComment;
      context->latestComment = NULL;

      (yyvsp[-2].pflagset)->insert(FLAG_EXPORT);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), comment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
    }
#line 7214 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 996 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 7222 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 1002 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 7228 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 1003 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7234 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 1004 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));       }
#line 7240 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 1005 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 7246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 1006 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));     }
#line 7252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 1007 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7258 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 1008 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7264 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 1009 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 7270 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 1010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true, false); }
#line 7276 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 1011 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7282 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 1012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 192:
#line 1013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7294 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1014 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr),   (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 194:
#line 1015 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr),   (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7306 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1016 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), false, false); }
#line 7312 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1017 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), false, false); }
#line 7318 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1018 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true,  false); }
#line 7324 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1019 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true,  false); }
#line 7330 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1021 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1027 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7350 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1033 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7360 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1039 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1045 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(),                       NULL, new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7381 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1052 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(),                       (yyvsp[-2].pcallexpr), new BlockStmt((yyvsp[0].pblockstmt)), false, true);
    }
#line 7392 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1059 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1063 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true,  true);
    }
#line 7408 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1069 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 7414 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1073 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 7420 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1074 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 7426 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1075 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1076 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1077 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7445 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1079 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7452 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1081 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7459 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1083 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1088 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 7472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 7478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 7484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1093 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 7490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 7496 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7502 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1096 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 7508 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1100 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1101 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7520 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1105 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 7526 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7532 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1107 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pdefexpr), (yyvsp[0].pblockstmt)); }
#line 7538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 7544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1112 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 7550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1116 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 7556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1120 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 7562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1122 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 7568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1126 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 7574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1127 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 7580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1132 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 7586 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1134 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1136 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 7598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1143 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7612 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1153 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 7626 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1166 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7636 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1171 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7646 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1176 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 7656 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1184 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7662 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1185 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 7668 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 7674 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1192 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 7680 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1194 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 7686 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1199 "chapel.ypp" /* yacc.c:1663  */
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
#line 7706 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1215 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 7714 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1222 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 7724 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1231 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 7736 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1239 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 7744 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1243 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 7752 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1249 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 7758 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1250 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 7764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1255 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 7773 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1260 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7782 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1265 "chapel.ypp" /* yacc.c:1663  */
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
#line 7804 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1287 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                }
#line 7812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1290 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);
                }
#line 7821 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1294 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);
                }
#line 7830 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1302 "chapel.ypp" /* yacc.c:1663  */
    {
      // Capture the latest comment
      (yylsp[0]).comment = context->latestComment;
      context->latestComment = NULL;

      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 7844 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1312 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 7855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1319 "chapel.ypp" /* yacc.c:1663  */
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
#line 7882 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1345 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 7890 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1349 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7899 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1354 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 7907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1358 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 7916 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1363 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 7924 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 271:
#line 1370 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 272:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 7936 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1375 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 7942 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 7948 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1377 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 7954 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 7960 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 277:
#line 1379 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 7966 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1380 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 7972 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1381 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 7978 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 7984 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1383 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 7990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1384 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 7996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1385 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 8002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 8008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1387 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 8014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 8020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1389 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 8026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1390 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 8032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1391 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 8038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 8044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1393 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 8050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1394 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 8056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1395 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 8062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1396 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 8068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1397 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 8074 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1398 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 8080 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "init="; }
#line 8086 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 298:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr((yyvsp[-1].pch), "!"); }
#line 8092 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 8098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 8104 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 8110 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 8116 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1408 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 8122 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1409 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 8128 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1410 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 8134 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1411 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 8140 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1412 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 8146 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1413 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 8152 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 8158 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1415 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 8164 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1419 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 8170 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1420 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8176 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1424 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 8182 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1428 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 8188 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1429 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 8194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1430 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 8200 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1435 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 8206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1437 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 8212 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1439 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 8218 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1441 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8224 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1443 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 8230 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1447 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8236 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1448 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 8242 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1452 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 8248 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1453 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 8254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1454 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 8260 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1455 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 8266 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1456 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 8272 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1457 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 8278 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1458 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8284 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1459 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 8290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1460 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 8296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1464 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 8302 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1465 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 8308 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1466 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 8314 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 8320 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1468 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 8326 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1469 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 8332 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1473 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 8338 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1474 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 8344 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1478 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8350 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1479 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 8356 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1480 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 8362 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1481 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 8368 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1482 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 8374 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1483 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 8380 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1487 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 8386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1488 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 8392 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1491 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 8398 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1497 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 8404 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1501 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildQueriedExpr((yyvsp[0].pch)); }
#line 8410 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1505 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 8416 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1506 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8422 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1507 "chapel.ypp" /* yacc.c:1663  */
    { if (DefExpr* def = toDefExpr((yyvsp[0].pexpr))) {
                             def->sym->addFlag(FLAG_PARAM);
                           }
                           (yyval.pexpr) = (yyvsp[0].pexpr);
                         }
#line 8432 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1515 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, NULL); }
#line 8438 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1517 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), NULL); }
#line 8444 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1519 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(NULL, (yyvsp[0].pexpr)); }
#line 8450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1521 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8456 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1523 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8462 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1528 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8468 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1530 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(",", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1534 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8480 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1535 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8486 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1536 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8492 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1537 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8498 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1538 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8504 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1539 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr));}
#line 8510 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_RETURN, (yyvsp[0].pexpr)); }
#line 8516 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1545 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr((yyvsp[0].pch))); }
#line 8522 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1547 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_LIFETIME_OF, new UnresolvedSymExpr("this")); }
#line 8528 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1552 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 8534 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1554 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 8540 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 375:
#line 1556 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 8546 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1561 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 8563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1574 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 8581 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1590 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8593 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1594 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 8599 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 8605 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1599 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 8611 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 383:
#line 1600 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 8617 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 384:
#line 1601 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 8623 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 385:
#line 1602 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 8629 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 386:
#line 1607 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 387:
#line 1613 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 8648 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1622 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 8657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1630 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 8663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 8669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1637 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 8675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1639 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 8681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 8687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1646 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 8693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1648 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 8699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1650 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 8705 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1656 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8711 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1657 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 8717 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8723 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1664 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1666 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 8735 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1668 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8743 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 8751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1676 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8757 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1678 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8765 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1682 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8773 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1690 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8785 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1691 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8791 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1692 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8797 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8803 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8809 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8815 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 8821 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8827 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8833 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 418:
#line 1723 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 8841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 419:
#line 1727 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 8849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 420:
#line 1731 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 8861 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 421:
#line 1739 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 8869 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 422:
#line 1745 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8875 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 423:
#line 1746 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8881 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 424:
#line 1747 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8887 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8893 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8899 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 427:
#line 1760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 8905 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1762 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 8911 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 8917 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1768 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 8923 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1769 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8929 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8935 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1771 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8941 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1772 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8947 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8953 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1779 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 8959 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1780 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1781 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8971 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1785 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 8977 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1786 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8983 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1790 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 8989 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1791 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8995 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 443:
#line 1792 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9001 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1797 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1801 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 9019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 448:
#line 1806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 9025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 449:
#line 1807 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 450:
#line 1811 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 451:
#line 1812 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 9043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 452:
#line 1813 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 453:
#line 1814 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 454:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 9061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1831 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1833 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr)); }
#line 9079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1835 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gUninstantiated); }
#line 9085 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 464:
#line 1846 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 9091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 465:
#line 1848 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 9097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 466:
#line 1850 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 9103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 467:
#line 1852 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 9109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 468:
#line 1854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 9115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 469:
#line 1856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 9121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 9127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1861 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_owned"); }
#line 9133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1863 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 9139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1865 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtUnmanaged->symbol); }
#line 9145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1867 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1869 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_shared"); }
#line 9157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1871 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 9163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1873 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtBorrowed->symbol); }
#line 9169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1875 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS_CHECKED, (yyvsp[0].pexpr)); }
#line 9175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1878 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyManagementNonNilable->symbol); }
#line 9181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1880 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(dtAnyRecord->symbol); }
#line 9187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1885 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1887 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1889 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1891 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1893 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1895 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1897 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1899 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 9235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1901 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1903 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9247 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1905 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 9253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1907 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 9259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1909 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 9270 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1916 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 9280 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1922 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 9290 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1928 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9300 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1934 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 9310 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1943 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9316 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1952 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 9322 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1968 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9328 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1972 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 9334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1977 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1981 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9346 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1982 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1986 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 9358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1990 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1991 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 9370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1996 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 9378 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 2000 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9386 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 2004 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 9394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 2010 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 9400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 2011 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 9406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 2012 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 9412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 2013 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 9418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 2014 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 9424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 524:
#line 2015 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 9430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 2021 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 2026 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW); }
#line 9442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 2028 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol))); }
#line 9450 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 2032 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol))); }
#line 9458 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 2036 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtUnmanaged->symbol))); }
#line 9466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 2040 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtBorrowed->symbol))); }
#line 9474 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 2048 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pcallexpr)->insertAtTail((yyvsp[0].pexpr));
      (yyval.pexpr) = (yyvsp[-1].pcallexpr); }
#line 9481 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 2053 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9491 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 2059 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr((yyvsp[-4].pexpr), (yyvsp[-1].pcallexpr)));
    }
#line 9501 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 2065 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtOwned->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9512 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 2072 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW,
                        new NamedExpr(astr_chpl_manager,
                                      new SymExpr(dtShared->symbol)),
                        new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED,
                                     new CallExpr((yyvsp[-5].pexpr), (yyvsp[-2].pcallexpr))));
    }
#line 9523 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 537:
#line 2082 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 9529 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 546:
#line 2098 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 9535 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 547:
#line 2100 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9541 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 548:
#line 2102 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9547 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 549:
#line 2104 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBoundedRange((yyvsp[-2].pexpr), (yyvsp[0].pexpr), false, true); }
#line 9553 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 550:
#line 2119 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLowBoundedRange((yyvsp[-1].pexpr)); }
#line 9559 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 551:
#line 2121 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr)); }
#line 9565 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 552:
#line 2123 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildHighBoundedRange((yyvsp[0].pexpr), true); }
#line 9571 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 553:
#line 2125 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildUnboundedRange(); }
#line 9577 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 554:
#line 2129 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 9583 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 555:
#line 2130 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9589 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 556:
#line 2134 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 9595 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 557:
#line 2135 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 9601 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 558:
#line 2136 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9607 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 563:
#line 2152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9613 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 564:
#line 2153 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9619 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 565:
#line 2154 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pexpr) = new CallExpr(PRIM_TO_NILABLE_CLASS_CHECKED, (yyvsp[-1].pexpr));}
#line 9625 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 566:
#line 2155 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 9631 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 568:
#line 2160 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9637 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 569:
#line 2161 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 9643 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 570:
#line 2162 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 9649 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 571:
#line 2166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 9655 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 572:
#line 2167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 9661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 573:
#line 2168 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 9667 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 574:
#line 2169 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 9673 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 575:
#line 2170 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(buildDotExpr((yyvsp[-4].pexpr), "chpl_bytes")); }
#line 9679 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 576:
#line 2178 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 9685 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 577:
#line 2179 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 9691 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 578:
#line 2180 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 9697 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 579:
#line 2181 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 9703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 580:
#line 2185 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 9709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 581:
#line 2186 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 9715 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 582:
#line 2190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 9721 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 583:
#line 2191 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildBytesLiteral((yyvsp[0].pch)); }
#line 9727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 586:
#line 2197 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 9733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 587:
#line 2198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 9739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 588:
#line 2199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 9745 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 589:
#line 2200 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 9751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 590:
#line 2201 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNone); }
#line 9757 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 591:
#line 2202 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr),
                                            new SymExpr(gTrue)); }
#line 9764 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 592:
#line 2204 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr),
                                                   new SymExpr(gTrue)); }
#line 9771 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 593:
#line 2206 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 9777 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 594:
#line 2207 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 9783 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 595:
#line 2209 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 9791 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 596:
#line 2213 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 9799 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 597:
#line 2219 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9805 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 598:
#line 2220 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 9811 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 599:
#line 2224 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9817 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 600:
#line 2225 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9823 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 601:
#line 2226 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9829 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 602:
#line 2227 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9835 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 603:
#line 2228 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 604:
#line 2229 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9847 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 605:
#line 2230 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9853 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 606:
#line 2231 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9859 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 607:
#line 2232 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 608:
#line 2233 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 609:
#line 2234 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 610:
#line 2235 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 611:
#line 2236 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 612:
#line 2237 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 613:
#line 2238 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 614:
#line 2239 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 615:
#line 2240 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 616:
#line 2241 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 617:
#line 2242 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 618:
#line 2243 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 619:
#line 2244 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 620:
#line 2245 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 621:
#line 2246 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr),
                                               new SymExpr(gTrue)); }
#line 9950 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 622:
#line 2251 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 9956 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 623:
#line 2252 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 9962 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 624:
#line 2253 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 9968 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 625:
#line 2254 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 9974 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 626:
#line 2255 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 9980 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 627:
#line 2256 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("postfix!", (yyvsp[-1].pexpr)); }
#line 9986 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 628:
#line 2257 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 9992 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 629:
#line 2261 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 9998 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 630:
#line 2262 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10004 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 631:
#line 2263 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10010 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 632:
#line 2264 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10016 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 633:
#line 2268 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10022 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 634:
#line 2269 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10028 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 635:
#line 2270 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 10034 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 636:
#line 2271 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 10040 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 637:
#line 2276 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 10046 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 638:
#line 2277 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 10052 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 639:
#line 2278 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 10058 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 640:
#line 2279 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 10064 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 641:
#line 2280 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 10070 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 642:
#line 2281 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 10076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 643:
#line 2282 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 10082 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 10086 "bison-chapel.cpp" /* yacc.c:1663  */
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
