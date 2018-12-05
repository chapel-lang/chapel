/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

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
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
    std::set<Flag>*           pflagset;
  };

  #endif
#line 131 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_2_
  #define _BISON_CHAPEL_DEFINES_2_

  struct YYLTYPE {
    int         first_line;
    int         first_column;
    int         last_line;
    int         last_column;
    const char* comment;
  };

  #define YYLTYPE_IS_DECLARED 1
  #define YYLTYPE_IS_TRIVIAL  1

  #endif
#line 153 "chapel.ypp" /* yacc.c:355  */

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

#line 223 "bison-chapel.cpp" /* yacc.c:355  */

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
    TLABEL = 303,
    TLAMBDA = 304,
    TLET = 305,
    TLOCAL = 306,
    TLOCALE = 307,
    TMINUSMINUS = 308,
    TMODULE = 309,
    TNEW = 310,
    TNIL = 311,
    TNOINIT = 312,
    TON = 313,
    TONLY = 314,
    TOTHERWISE = 315,
    TOUT = 316,
    TOVERRIDE = 317,
    TOWNED = 318,
    TPARAM = 319,
    TPLUSPLUS = 320,
    TPRAGMA = 321,
    TPRIMITIVE = 322,
    TPRIVATE = 323,
    TPROC = 324,
    TPROTOTYPE = 325,
    TPUBLIC = 326,
    TREAL = 327,
    TRECORD = 328,
    TREDUCE = 329,
    TREF = 330,
    TREQUIRE = 331,
    TRETURN = 332,
    TSCAN = 333,
    TSELECT = 334,
    TSERIAL = 335,
    TSHARED = 336,
    TSINGLE = 337,
    TSPARSE = 338,
    TSTRING = 339,
    TSUBDOMAIN = 340,
    TSYNC = 341,
    TTHEN = 342,
    TTHIS = 343,
    TTHROW = 344,
    TTHROWS = 345,
    TTRUE = 346,
    TTRY = 347,
    TTRYBANG = 348,
    TTYPE = 349,
    TUINT = 350,
    TUNDERSCORE = 351,
    TUNION = 352,
    TUNMANAGED = 353,
    TUSE = 354,
    TVAR = 355,
    TWHEN = 356,
    TWHERE = 357,
    TWHILE = 358,
    TWITH = 359,
    TYIELD = 360,
    TZIP = 361,
    TALIAS = 362,
    TAND = 363,
    TASSIGN = 364,
    TASSIGNBAND = 365,
    TASSIGNBOR = 366,
    TASSIGNBXOR = 367,
    TASSIGNDIVIDE = 368,
    TASSIGNEXP = 369,
    TASSIGNLAND = 370,
    TASSIGNLOR = 371,
    TASSIGNMINUS = 372,
    TASSIGNMOD = 373,
    TASSIGNMULTIPLY = 374,
    TASSIGNPLUS = 375,
    TASSIGNREDUCE = 376,
    TASSIGNSL = 377,
    TASSIGNSR = 378,
    TBAND = 379,
    TBNOT = 380,
    TBOR = 381,
    TBXOR = 382,
    TCOLON = 383,
    TCOMMA = 384,
    TDIVIDE = 385,
    TDOT = 386,
    TDOTDOT = 387,
    TDOTDOTDOT = 388,
    TEQUAL = 389,
    TEXP = 390,
    TGREATER = 391,
    TGREATEREQUAL = 392,
    THASH = 393,
    TIO = 394,
    TLESS = 395,
    TLESSEQUAL = 396,
    TMINUS = 397,
    TMOD = 398,
    TNOT = 399,
    TNOTEQUAL = 400,
    TOR = 401,
    TPLUS = 402,
    TQUESTION = 403,
    TSEMI = 404,
    TSHIFTLEFT = 405,
    TSHIFTRIGHT = 406,
    TSTAR = 407,
    TSWAP = 408,
    TLCBR = 409,
    TRCBR = 410,
    TLP = 411,
    TRP = 412,
    TLSBR = 413,
    TRSBR = 414,
    TNOELSE = 415,
    TUPLUS = 416,
    TUMINUS = 417
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
#line 184 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 431 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 437 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
#line 192 "chapel.ypp" /* yacc.c:359  */

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

#line 511 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   15870

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  134
/* YYNRULES -- Number of rules.  */
#define YYNRULES  560
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1001

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   417

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
     155,   156,   157,   158,   159,   160,   161,   162
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   468,   468,   473,   474,   480,   481,   486,   487,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   525,
     528,   530,   536,   537,   538,   542,   543,   556,   557,   558,
     563,   564,   569,   574,   579,   583,   590,   595,   599,   604,
     608,   609,   610,   614,   618,   620,   622,   624,   626,   633,
     634,   638,   639,   640,   641,   642,   645,   646,   647,   648,
     649,   661,   662,   673,   674,   675,   676,   677,   678,   679,
     680,   681,   682,   683,   684,   685,   686,   687,   688,   692,
     693,   694,   695,   696,   697,   698,   699,   706,   707,   708,
     709,   710,   711,   712,   713,   718,   719,   723,   724,   728,
     729,   733,   734,   735,   736,   737,   738,   739,   740,   744,
     745,   749,   750,   751,   752,   756,   763,   764,   765,   766,
     767,   768,   769,   770,   771,   772,   773,   774,   775,   776,
     777,   778,   779,   785,   791,   797,   803,   810,   820,   824,
     825,   826,   827,   828,   830,   832,   834,   839,   842,   843,
     844,   845,   846,   847,   851,   852,   856,   857,   858,   862,
     863,   867,   870,   872,   877,   878,   882,   884,   886,   893,
     903,   913,   923,   936,   941,   946,   954,   955,   960,   961,
     963,   968,   984,   991,  1000,  1008,  1012,  1019,  1020,  1025,
    1030,  1024,  1055,  1061,  1068,  1074,  1080,  1091,  1097,  1090,
    1129,  1133,  1138,  1142,  1147,  1154,  1155,  1159,  1160,  1161,
    1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,
    1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  1183,  1184,  1188,  1189,  1190,  1191,  1192,  1193,  1194,
    1195,  1196,  1197,  1198,  1199,  1203,  1204,  1208,  1212,  1213,
    1214,  1218,  1220,  1222,  1224,  1226,  1231,  1232,  1236,  1237,
    1238,  1239,  1240,  1241,  1242,  1243,  1244,  1248,  1249,  1250,
    1251,  1252,  1253,  1257,  1258,  1262,  1263,  1264,  1265,  1266,
    1267,  1271,  1272,  1275,  1276,  1280,  1281,  1285,  1287,  1292,
    1293,  1297,  1298,  1302,  1303,  1307,  1309,  1311,  1316,  1329,
    1346,  1347,  1349,  1354,  1355,  1356,  1357,  1358,  1362,  1368,
    1374,  1382,  1383,  1391,  1393,  1398,  1400,  1402,  1407,  1409,
    1411,  1418,  1419,  1420,  1425,  1427,  1429,  1433,  1437,  1439,
    1443,  1451,  1452,  1453,  1454,  1455,  1460,  1461,  1462,  1463,
    1464,  1484,  1488,  1492,  1500,  1507,  1508,  1509,  1513,  1515,
    1521,  1523,  1525,  1530,  1531,  1532,  1533,  1534,  1540,  1541,
    1542,  1543,  1547,  1548,  1552,  1553,  1554,  1558,  1559,  1563,
    1564,  1568,  1569,  1573,  1574,  1575,  1576,  1580,  1581,  1592,
    1593,  1594,  1595,  1596,  1597,  1599,  1601,  1603,  1605,  1607,
    1609,  1611,  1613,  1615,  1617,  1622,  1624,  1626,  1628,  1630,
    1632,  1634,  1636,  1638,  1640,  1642,  1644,  1646,  1653,  1659,
    1665,  1671,  1680,  1690,  1698,  1699,  1700,  1701,  1702,  1703,
    1704,  1705,  1710,  1711,  1715,  1719,  1720,  1724,  1728,  1729,
    1733,  1737,  1741,  1748,  1749,  1750,  1751,  1752,  1753,  1757,
    1758,  1763,  1768,  1776,  1777,  1778,  1779,  1780,  1781,  1782,
    1783,  1784,  1786,  1788,  1790,  1792,  1794,  1796,  1798,  1803,
    1804,  1807,  1808,  1809,  1812,  1813,  1814,  1815,  1826,  1827,
    1831,  1832,  1833,  1837,  1838,  1839,  1840,  1848,  1849,  1850,
    1851,  1855,  1856,  1860,  1861,  1862,  1863,  1864,  1865,  1866,
    1867,  1868,  1869,  1870,  1874,  1882,  1883,  1887,  1888,  1889,
    1890,  1891,  1892,  1893,  1894,  1895,  1896,  1897,  1898,  1899,
    1900,  1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,
    1913,  1914,  1915,  1916,  1917,  1918,  1922,  1923,  1924,  1925,
    1929,  1930,  1931,  1932,  1937,  1938,  1939,  1940,  1941,  1942,
    1943
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
  "TINOUT", "TINT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL",
  "TLOCALE", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON",
  "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE",
  "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW", "TTHROWS",
  "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE", "TUNION",
  "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH",
  "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA",
  "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER",
  "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD",
  "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT",
  "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
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
  "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_type", "var_decl_stmt", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "type_level_expr", "for_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_expr", "let_expr", "expr", "opt_expr", "opt_try_expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "bool_literal", "literal_expr", "assoc_expr_ls", "binary_op_expr",
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
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
     415,   416,   417
};
# endif

#define YYPACT_NINF -880

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-880)))

#define YYTABLE_NINF -512

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -880,    25,  2667,  -880,     9,  -880,  -880,  -880,  -880,  -880,
    -880,  4074,     5,   158,  -880,  9181,  -880,     5,  9181,  -880,
     392,   111,   158,  4074,  9181,  4074,    48,  -880,  9181,  6534,
    -880,  7869,  9181,  6261,  9181,  -880,    69,  -880,  -880, 15772,
    -880,  1495,  8291,  -880,  9181,  9181,  -880,  9181,  -880,  9181,
    -880,  9181,   166,    78,  1052,  1052,  -880,  -880,  -880,  8424,
    7424,  9181,  8291,  9181,  9181,   144,  -880,    81,  4074,  -880,
    9181,  -880,  9314,  9314, 15772,  -880,  -880,  9181,  8424,  -880,
    9181,  9181,  -880,  -880,  9181,  -880,  -880, 11176,  9181,  9181,
    -880,  9181,  -880,  -880,  3138,  6667,  8424,  -880,  3918,  -880,
    -880,   172,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
   15772,  -880, 15772,   199,    55,  -880,  -880,  1495,  -880,    62,
    -880,  -880,  -880,   115,   128,   188,   211,   222, 15464,  2189,
     -59,   229,   246,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
     320,  -880,  -880, 15464,   183,  4074,  -880,  -880,   257,  9181,
    9181,  9181,  9181,  9181,  8424,  8424,   256,  -880,  -880,  -880,
    -880,   284,   281,  -880,  -880,   268, 13719, 15772,  1495,  -880,
     274,  -880,    42, 15464,   323,  7580, 15464,  -880,   151,  -880,
   15772,   153,   265, 15772,   277,    35, 13256, 13205,   265,  9181,
    -880, 13640, 12518,  7580,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  4074,  -880,   278,  1832,    70,     1,
    -880,  4074,  -880,  -880, 13394,   738,   284, 13394,   284,   738,
    -880,  7580,  9181,  -880,  -880, 15772,  -880,   143, 15464,  9181,
    9181,  -880, 15464,   292, 13570,  -880, 13394,   284, 15464,   293,
    7580,  -880, 15464, 13965,  -880,  -880, 14016, 11434,  -880,  -880,
   14103,   321,   301,   284,   145, 13789, 13394, 14154,   135,  1004,
     738,   135,   738,   132,  -880,  -880,  3294,   -42,  -880,  9181,
    -880,   -61,    30,  -880,    73, 14241,   -40,   446,  -880,  -880,
     400,   329,   307,  -880,  -880,  -880,   147,  -880,  9181,  9181,
    9181,  9181,  8580,  8580,  9181,  9181,  9181,  9181,  9447,  9181,
     279, 11176,  9181,  9181,  9181,  9181,  9181,  9181,  9181,  9181,
    9181,  9181,  9181,  9181,  9181,  9181,  9181,  -880,  -880,  -880,
    -880,  -880,  -880,  8002,  8002,  -880,  -880,  -880,  -880,  8002,
    -880,  -880,  8002,  8002,  7580,  7580,  8580,  8580,  7135,  -880,
    -880, 13864, 13934, 14190,    -3,  3450,  -880,  8580,    35,   317,
     201,  -880,  9181,  -880,  9181,   362,  -880,   315,   348,  -880,
    -880,   331, 15772,  1495,   440,  8424,  -880,  4230,  8580,  -880,
    4386,  8580,   333,  -880,    35,   265,  9580,  9181,  -880,  4074,
     456,  9181,   334,  -880,   649,  -880,  -880,  1832,  -880,   367,
     340,  -880,  9713,   394,  9181,  1495,  -880,  -880,  -880,   344,
    -880,  8424,  -880, 15464, 15464,  -880,    41,  -880,  7580,   349,
    -880,   490,  -880,   490,  -880,  9846,   380,  -880,  -880,  9580,
    9181,  -880,  -880,  -880,  -880,  -880,  8135,  -880, 12373,  6823,
    -880,  6979,  -880,  8580,  5790,  2982,   351,  9181,  6104,  -880,
    -880, 15772,  8424,   357, 15719,   190,  -880, 14328, 15565, 15565,
     269,  -880,   269,  -880,   269, 12123,   918,  1105,  1377, 11563,
   11563,   284,   135,  -880,  -880,  -880,  -880,  1004,   883,   269,
     765,   765, 15565,   765,   765,  1198,   135,   883, 15616,  1198,
     738,   738,   135,   371,   372,   375,   376,   378,   373,   370,
    -880,   269,  -880,   269,    53,  -880,  -880,  -880,    86,  -880,
     917, 15534,   335,  9979,  8580, 10112,  8580,  9181,  8580, 11755,
       5, 14379,  -880,  -880,  -880, 15464, 14410,  7580,  -880,  7580,
    -880,   329,   220,  9181,    89,  9181, 15464,    43, 13425,  9181,
   15464,    12, 13343,  7135,  -880,   384,   406,   387, 14480,   406,
     388,   510, 14555,  4074, 13495,  -880,    68,  -880,  -880,  -880,
    -880,  -880,  -880,   678,    99,  -880, 11940,  -880,   327,   386,
    1832,    70,  9181,    48,    69,  9181,  9181,  9181,  9181,  9181,
    5634,  -880,  -880,   316,  7736,  -880, 15464,  -880,  -880,  -880,
   15464,   389,    45,   393,  -880, 11887,  -880,  -880,   233, 15772,
     396,   397,  -880,  -880,  -880,  -880,  -880,  -880,   -25, 12083,
    -880,  -880, 15464,  4074, 15464,  -880, 14625,   399,   422,  2106,
     401,   445,   -35,  -880,   480,  -880,  -880,  -880,  -880,  6380,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  7135,
    -880,    36,  8580,  8580,  9181,   528, 14700,  9181,   529, 14770,
     403, 12153,    35,    35,  -880,  -880,  -880,  -880,   405,  -880,
   13394,  -880, 12593,  4542,  -880,  4698,  -880, 12668,  4854,  -880,
      35,  5010,  -880,    35,   104,  -880,  9181,  -880,  9181,  -880,
    4074,  9181,  -880,  4074,   533,  -880,  -880, 15772,  1084,  -880,
    1832,   437,   491,  -880,  -880,  -880,    29,  -880,  -880,   394,
     409,    93,  -880,  -880,  -880,  5166,  8424,  -880,  -880,  -880,
   15772,  -880,   441,   268,  -880,  -880,  -880,  5322,   411,  5478,
     413,  -880,  9181,  3606,   425,   585,  -880,  9181, 15772,  -880,
    -880,   327,   426,   450,  -880,   453,  -880,   454,  -880,   455,
     458,   463,   465,   467,   468,  -880,   470,  -880, 15772,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  9181,
    -880,  -880,   471,   426,   426,  -880,  -880,   394,   129,   131,
   14801, 10245, 10378, 14939, 10511, 10644, 10777, 10910,  -880,  -880,
   11825,  -880,  4074,  9181, 15464,  9181, 15464,  4074,  9181, 15464,
    -880,  9181, 15464,  -880,  7135,  -880, 14852, 15464,  -880, 15464,
     556,  4074,   437,  -880,   448,  8713,   142,  -880,   501,  -880,
    -880,  8580, 11659,  4074,  -880,    31,   457,  9181,  -880,  9181,
    -880, 15464,  4074,  9181,  -880, 15464,  4074, 15464,   215,  -880,
    3762,  -880,  7291,  1257,  1257,  -880,  2546,  -880, 15464,  -880,
      27,   649,  -880,  -880, 12443, 12003,  -880,  -880,  -880,  9181,
    9181,  9181,  9181,  9181,  9181,  9181,  9181,   452, 11364, 14555,
   12743, 12818, 14555, 12893, 12968,  -880,  9181,  4074,  -880,   394,
     437,  5947,  -880,  -880,  -880,   126,  8424,  -880,  -880,  9181,
     -33, 12228,  -880,   350,   323,  -880,   268, 15464, 13043,  -880,
   13118,  -880,  -880,  -880,  -880,  -880, 11043,   522,   197,  -880,
    -880,  -880,   426,   426, 15014, 15089, 15164, 15239, 15314, 15389,
    -880,  -880,  4074,  4074,  4074,  4074, 15464,  -880,  -880,   142,
    8869,    94,  -880,  -880, 15464, 15464,  -880,  -880,  -880, 10910,
    -880,  4074,  4074,  2823,  -880,  -880,   282,  -880,   501,  -880,
    -880,  -880,  9181,  9181,  9181,  9181,  9181,  9181, 14555, 14555,
   14555, 14555,  -880,  -880,  -880,  -880,  -880,   138,  8580, 11309,
     680, 14555, 14555,   459,  2373,    98,    13, 12298,  -880,  -880,
    -880,  -880,   306, 11440,  -880,  -880,  -880,  9025,  -880,   398,
    -880
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    81,   504,   505,   506,   507,
     508,     0,   442,    69,    99,     0,   193,   442,     0,   104,
       0,   324,    69,     0,     0,     0,     0,   203,   479,   479,
     501,     0,     0,     0,     0,   103,     0,   213,   100,     0,
     209,     0,     0,   106,     0,     0,   433,     0,   216,     0,
     323,     0,     0,     0,    44,    43,   102,   194,   325,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,    82,
       0,   502,     0,     0,     0,   101,   195,     0,     0,   327,
       0,     0,   556,   558,     0,   559,   560,   478,     0,     0,
     557,   554,   121,   555,     0,     0,     0,     4,     0,     5,
       9,    45,    10,    11,    12,    14,   397,   398,    22,    13,
     122,   128,    15,    17,    16,    19,    20,    21,    18,   127,
       0,   125,     0,   468,     0,   129,   126,     0,   130,   484,
     464,   465,   401,   399,     0,     0,   469,   470,     0,   400,
       0,   485,   486,   487,   503,   463,   403,   402,   466,   467,
       0,    38,    24,   409,     0,     0,   443,    70,     0,     0,
       0,     0,     0,     0,     0,     0,   468,   484,   399,   469,
     470,   411,   400,   485,   486,     0,   442,     0,     0,   326,
       0,   167,     0,   382,     0,   389,   480,   214,   507,   135,
       0,     0,   215,     0,     0,     0,     0,     0,     0,   479,
     134,     0,     0,   389,    76,    83,    94,    88,    96,    78,
      87,    97,    84,    98,    92,    86,    93,    91,    89,    90,
      77,    79,    85,    95,     0,    80,     0,     0,     0,     0,
     331,     0,   118,    32,     0,   542,   461,     0,   412,   543,
       7,   389,   479,   124,   123,   308,   379,     0,   378,     0,
       0,   119,   483,     0,     0,    35,     0,   414,   404,     0,
     389,    36,   410,     0,   174,   170,     0,   400,   174,   171,
       0,   320,     0,   413,     0,   378,     0,     0,   545,   477,
     541,   544,   540,     0,    47,    50,     0,     0,   384,     0,
     386,     0,     0,   385,     0,   378,     0,     0,     6,    46,
       0,   196,     0,   294,   293,   217,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   253,   260,   261,
     262,   257,   259,     0,     0,   255,   258,   256,   254,     0,
     264,   263,     0,     0,   389,   389,     0,     0,     0,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
       0,    28,     0,    29,     0,   397,   395,     0,   390,   391,
     396,     0,     0,     0,     0,     0,   144,     0,     0,   143,
       0,     0,     0,   150,     0,     0,     0,    56,   131,     0,
     160,     0,     0,    30,   276,   210,   335,     0,   336,   338,
       0,   360,     0,   341,     0,     0,   117,    31,    33,     0,
     307,     0,    63,   481,   482,   120,     0,    34,   389,     0,
     181,   172,   168,   173,   169,     0,   318,   315,    60,     0,
      56,   137,    37,    49,    48,    51,     0,   509,     0,     0,
     497,     0,   499,     0,     0,     0,     0,     0,     0,   513,
       8,     0,     0,     0,     0,   287,   330,   441,   537,   536,
     539,   547,   546,   551,   550,   533,   530,   531,   532,   473,
     474,   472,   520,   495,   496,   494,   493,   475,   524,   535,
     529,   527,   538,   528,   526,   518,   523,   525,   534,   517,
     521,   522,   519,     0,     0,     0,     0,     0,     0,     0,
     549,   548,   553,   552,   453,   454,   456,   458,     0,   445,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   511,
     442,   442,   140,   316,   328,   383,     0,     0,   406,     0,
     317,   196,     0,     0,     0,     0,   417,     0,     0,     0,
     423,     0,     0,     0,   151,   555,    59,     0,    52,    57,
       0,   159,     0,     0,     0,   405,   281,   278,   279,   280,
     284,   285,   286,   276,     0,   269,     0,   277,   295,     0,
     339,     0,   111,   113,   114,   109,   110,   108,   107,   112,
       0,   359,   358,   464,     0,   333,   462,   332,   492,   381,
     380,     0,     0,     0,   407,     0,   175,   322,   464,     0,
       0,     0,   510,   471,   498,   387,   500,   388,     0,     0,
     512,   156,   427,     0,   515,   514,     0,     0,   197,     0,
       0,   207,     0,   204,   291,   288,   289,   292,   218,     0,
      67,    68,    66,    65,    64,   490,   491,   455,   457,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   136,   393,   394,   392,     0,   329,
       0,   158,     0,     0,   142,     0,   141,     0,     0,   148,
       0,     0,   146,     0,     0,   448,     0,   132,     0,   133,
       0,     0,   162,     0,   164,   282,   283,     0,   276,   267,
       0,   373,   296,   299,   298,   300,     0,   337,   340,   341,
       0,     0,   342,   343,   183,     0,     0,   182,   185,   408,
       0,   176,   179,     0,   319,    61,    62,     0,     0,     0,
       0,   157,     0,     0,     0,   212,   202,     0,   205,   201,
     290,   295,   265,    71,   250,    83,   248,    88,    73,    87,
      84,    98,    86,    89,    72,    74,    85,   229,   232,   230,
     231,   242,   233,   246,   238,   236,   249,   252,   237,   235,
     240,   245,   247,   234,   239,   243,   244,   241,   251,     0,
     227,    75,     0,   265,   265,   225,   446,   341,   484,   484,
       0,     0,     0,     0,     0,     0,     0,     0,   139,   138,
       0,   145,     0,     0,   416,     0,   415,     0,     0,   422,
     149,     0,   421,   147,     0,   447,    54,    53,   161,   432,
     163,     0,   373,   270,     0,     0,   341,   297,   313,   334,
     364,     0,   511,     0,   187,     0,     0,     0,   177,     0,
     154,   429,     0,     0,   152,   428,     0,   516,     0,    39,
       0,   191,   479,   212,   212,   189,   212,   199,   208,   206,
       0,   276,   224,   228,     0,     0,   220,   221,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   420,
       0,     0,   426,     0,     0,   449,     0,     0,   166,   341,
     373,     0,   376,   375,   377,   464,   309,   273,   271,     0,
       0,     0,   362,   464,   188,   186,     0,   180,     0,   155,
       0,   153,    41,    40,   200,   355,     0,   301,     0,   226,
      71,    72,   265,   265,     0,     0,     0,     0,     0,     0,
     192,   190,     0,     0,     0,     0,    55,   165,   272,   341,
     365,     0,   310,   312,   311,   314,   305,   306,   211,     0,
     178,     0,     0,     0,   354,   353,   464,   302,   313,   266,
     222,   223,     0,     0,     0,     0,     0,     0,   419,   418,
     425,   424,   275,   274,   367,   368,   370,   464,     0,   511,
     464,   431,   430,     0,   345,     0,     0,     0,   369,   371,
     350,   348,   464,   511,   303,   219,   304,   365,   349,   464,
     372
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -880,  -880,  -880,    -1,  -330,  1811,  -880,  -880,  -880,  1692,
    -120,  -299,   175,   177,  -880,  -880,   280,   599,  -880,    17,
    -138,  -625,  -880,  -793,  -880,   511,  -842,  -709,   -52,  -880,
    -880,  -880,   107,  -880,  -880,  -880,   355,  -880,   -95,  -880,
    -880,  -880,  -880,  -880,   436,    87,  -166,  -880,  -880,  -880,
    -101,   754,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -226,  -134,  -702,  -880,  -221,   -53,    79,  -880,  -880,  -880,
     -87,  -880,  -880,  -331,   957,  -880,  -282,  -297,  -880,  -161,
    -880,   -14,   626,  -103,   248,  -880,  -372,  -694,  -781,  -880,
    -526,  -425,  -879,  -852,  -715,   -51,  -880,  -158,  -880,  -149,
    -880,   127,   364,  -349,  -880,  -880,   876,  -880,    -8,  -880,
    -880,  -283,  -880,  -541,  -880,  -880,  1069,  1262,   -11,    -9,
     141,   538,  -880,  1384,  1600,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -312
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   285,    98,   561,   100,   101,   300,   102,
     286,   556,   560,   557,   103,   104,   105,   158,   780,   228,
     106,   225,   107,   591,   191,   233,   108,   109,   110,   111,
     112,   113,   660,   114,   115,   116,   431,   606,   723,   117,
     118,   602,   718,   119,   120,   463,   735,   121,   122,   632,
     633,   166,   226,   578,   124,   125,   465,   741,   638,   782,
     783,   401,   862,   405,   574,   575,   576,   577,   639,   305,
     706,   958,   995,   948,   246,   943,   897,   900,   126,   272,
     436,   127,   128,   229,   230,   409,   410,   595,   955,   917,
     413,   592,   975,   894,   826,   287,   182,   291,   292,   377,
     378,   379,   167,   130,   131,   132,   168,   134,   155,   156,
     518,   394,   684,   519,   520,   135,   169,   170,   138,   192,
     380,   172,   140,   173,   174,   143,   144,   145,   296,   146,
     147,   148,   149,   150
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     153,    97,   243,   244,   171,   353,   178,   176,   247,   175,
     607,   456,   685,   183,   781,   829,   369,   186,   186,   187,
     196,   197,   201,   202,   306,     3,   857,   274,   915,   381,
     411,   234,   892,   235,   236,   579,   237,   411,   238,   528,
     239,   678,   601,   414,    60,   294,   522,   375,   248,   252,
     254,   256,   257,   258,   402,   709,   224,   262,   947,   263,
     231,   266,   270,   593,   231,   375,   273,   275,   449,   276,
     277,   411,   673,   278,   573,   370,   279,   280,   281,   392,
     282,   866,   867,   248,   252,   295,   608,   446,   976,   458,
      60,   271,   419,   868,   738,   647,   450,   354,   559,   355,
     988,  -184,   303,   375,  -351,   715,  -356,   889,   786,   154,
     695,   429,  -356,   447,   364,   453,   392,  -351,  1000,   459,
     739,   365,   375,   954,   304,   787,   454,   989,   648,  -351,
     415,  -356,   898,   353,   727,   831,   978,   301,   195,   302,
     528,   559,  -184,   696,   947,  -356,   716,   914,   153,   361,
     362,   363,   262,   248,   295,   916,   529,   412,   151,   451,
     421,   157,   994,   311,   412,  -356,   365,   365,   368,   857,
    -116,   372,    16,   240,   252,   939,  -351,   392,   383,  -356,
     828,  -351,   486,  -356,   178,   365,   179,   452,   186,   365,
     395,   373,   252,  -356,   271,   938,  -184,   365,   412,  -356,
     717,   253,   454,   991,   185,   508,   509,   271,   708,   312,
     384,  -435,   998,   313,   634,   649,   375,   375,   421,  -356,
     960,   961,   454,   454,  -116,   203,    57,   454,   698,   259,
     252,   186,   455,   814,   241,  -374,   293,   260,   423,   424,
     781,   522,   299,   650,   408,   973,   671,  -366,  -116,   252,
      76,   594,   832,   979,   635,  -374,   699,   993,  -452,  -374,
    -451,   815,   420,   318,  -434,   636,   320,  -366,   680,   683,
     323,  -366,   421,   885,   421,   896,   415,   307,   448,   603,
     542,   151,     5,  -374,   637,   445,  -452,   443,  -451,    21,
     375,   615,   422,   617,   438,  -366,   466,   467,   468,   469,
     470,   472,   474,   475,   476,   477,   478,   481,   482,   483,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   698,   308,   824,    50,
     415,   484,   252,   252,   544,   728,   730,   522,   252,   358,
      58,   252,   252,   252,   252,   511,   513,   521,  -438,   415,
     534,   702,   265,   269,   959,  -489,   531,  -489,  -357,  -352,
    -439,   535,  -321,   536,   151,    79,   129,    69,   573,   669,
     912,  -440,  -352,   485,   248,   129,   546,   548,  -437,   550,
     552,   383,  -321,  -344,  -352,   558,   558,   129,   562,   129,
     564,   703,  -361,  -357,   356,  -436,  -344,   318,   357,   541,
     320,   375,   704,   596,   323,   856,   360,   902,  -344,   653,
     600,   628,  -489,   357,  -489,   320,    21,   252,  -357,   471,
     473,   705,   365,   371,   408,  -357,   374,  -361,   558,   558,
     435,  -352,   129,   385,   404,   600,  -352,  -488,   252,  -488,
     252,   425,   619,   600,   622,  -357,   624,   626,   724,   428,
     437,   248,  -361,   460,   461,  -344,    50,   462,   129,  -361,
    -344,   464,   129,   510,   512,  -357,   533,    58,   171,   273,
    -357,   537,   538,  -357,   530,  -346,   895,   539,   627,  -361,
     540,   631,   543,   903,   503,   504,   177,   563,  -346,   553,
     505,   565,    79,   506,   507,   547,   580,   581,   551,  -361,
    -346,   598,   522,   594,  -361,   784,   604,  -361,   605,   609,
     623,   629,   546,   656,   550,   659,   562,   661,   622,   129,
     640,   641,   662,   663,   642,   643,   252,   644,   252,   646,
     645,   573,   670,   -58,   672,   686,   687,   689,   677,   711,
     139,   690,   521,   707,   714,   725,   726,  -346,   856,   139,
     719,   421,  -346,   733,   737,   740,   736,   791,   794,   800,
     618,   139,   796,   139,   821,   825,   827,   956,   830,   837,
     842,   171,   846,   255,   238,   257,   258,   262,   273,   295,
     851,   -81,   861,   252,   -99,  -104,  -103,   887,   129,  -100,
     293,   977,   293,   701,  -106,   129,  -102,   408,  -105,   -82,
     980,  -101,   865,   899,    16,   890,   139,   930,    20,    21,
     267,   267,   957,   850,   906,   611,   610,    27,   990,    28,
     852,   180,   722,   433,    33,   836,   271,   382,   668,    37,
     977,   655,   139,   658,   878,   992,   139,   859,   521,   922,
     918,   472,   511,   790,   999,   823,   793,    48,   977,    50,
     129,    52,   697,   853,   860,   996,   854,   972,    57,   200,
      58,   986,   804,   597,   806,   835,   667,   809,     0,     0,
     812,     0,     0,   566,     0,   816,     0,   817,   666,    74,
     819,     0,    76,     0,     0,    79,     0,     0,     0,     0,
       0,   567,     0,   139,   568,     0,     0,     0,     0,     0,
       0,     0,   566,     0,     0,   248,   386,   389,   393,     0,
     569,     0,     0,   570,   822,    52,   841,   408,   845,     0,
     567,   847,  -363,   568,   571,     0,   858,     0,     0,   129,
       0,   923,     0,     0,    92,   713,     0,   722,     0,   569,
     855,     0,   570,   572,   297,   417,     0,     0,   418,     0,
       0,   129,     0,   571,   129,   631,   123,  -363,     0,   471,
     510,     0,   139,   129,     0,   123,   311,   427,   864,   139,
       0,     0,   572,     0,     0,   863,     0,   123,  -268,   123,
     804,   806,  -363,   809,   812,   841,   845,   441,     0,  -363,
       0,   879,   880,   311,   881,     0,   882,   883,     0,     0,
     884,   243,   244,   521,     0,     0,  -268,     0,     0,  -363,
       0,     0,   312,     0,     0,     0,   313,     0,     0,   129,
     901,   622,   123,     0,   139,     0,   907,     0,   908,  -363,
       0,     0,   910,     0,  -363,     0,     0,  -363,     0,   312,
     941,   186,     0,   313,     0,     0,     0,     0,   123,   445,
       0,     0,   123,     0,     0,     0,     0,     0,   879,   924,
     925,   882,   926,   927,   928,   929,   318,     0,   319,   320,
       0,     0,     0,   323,     0,   936,     0,     0,   133,   532,
     295,   330,     0,     0,   651,   944,     0,   133,   945,   315,
     336,   316,   317,   318,     0,   319,   320,   321,     0,   133,
     323,   133,   985,   139,     0,   554,     0,   329,   330,   123,
       0,   311,   333,     0,     0,   334,   335,   336,     0,     0,
       5,   968,   969,   970,   971,   139,     0,   129,   139,     0,
       0,     0,    14,     0,     0,     0,     0,   139,   845,    19,
     981,   982,   295,     0,   133,     0,   311,     0,     0,     0,
       0,   968,   969,   970,   971,   981,   982,   312,    35,     0,
       0,   313,     0,    38,     0,     0,     0,   987,   622,    43,
     133,     0,     0,     0,   133,     0,     0,     0,   123,     0,
       0,     0,   622,     0,     0,   123,   845,   129,     0,    56,
       0,     0,   312,   139,     0,     0,   313,     0,     0,     0,
       0,    66,     0,   785,     0,    69,     0,   315,     0,   316,
     317,   318,    75,   319,   320,   321,   788,   789,   323,   324,
     325,     0,     0,   327,   328,   329,   330,     0,     0,     0,
     333,   133,   311,   334,   335,   336,     0,   129,     0,   129,
     123,     0,   129,     0,     0,   129,   318,     0,   319,   320,
       0,     0,   290,   323,   129,     0,     0,   129,   674,   676,
       0,   330,   679,   682,     0,     0,     0,     0,   334,   335,
     336,   136,     0,     0,     0,   198,    21,     0,   312,   129,
     136,     0,   313,     0,     0,     0,    28,   242,     0,     0,
       0,   129,   136,   129,   136,     0,    37,   129,     0,  -212,
     133,   139,     0,     0,     0,     0,     0,   133,   566,     0,
       0,     0,     0,     0,    48,     0,    50,     0,     0,   123,
       0,  -212,     0,     0,     0,     0,   567,    58,   315,   568,
     316,   317,   318,   311,   319,   320,     0,   136,     0,   323,
       0,   123,   376,     0,   123,   569,   329,   330,   570,     0,
      52,   333,    79,   123,   334,   335,   336,     0,     0,   571,
     376,   139,   133,   136,     0,     0,   129,   136,     0,     0,
       0,   129,     0,   798,   799,     0,     0,     0,   572,   312,
       0,   801,     0,   313,     0,   129,     0,     0,     0,     0,
       0,   810,     0,     0,   813,     0,     0,   129,   376,     0,
       0,     0,     0,     0,     0,     0,   129,     0,     0,   123,
     129,   139,     0,   139,   129,     0,   139,   376,     0,   139,
       0,     0,     0,     0,   136,     0,   311,     0,   139,   315,
       0,   139,   317,   318,     0,   319,   320,     0,     0,     0,
     323,   133,     0,     0,     0,     0,     0,     0,   330,     0,
       0,   129,     0,   139,     0,   334,   335,   336,     0,     0,
       0,     0,     0,   133,   137,   139,   133,   139,     0,     0,
       0,   139,   312,   137,     0,   133,   313,     0,     0,     0,
     198,    21,     0,     0,     0,   137,     0,   137,     0,     0,
       0,    28,   242,   136,     0,     0,   129,   129,   129,   129,
     136,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   376,   376,     0,     0,   129,   129,   123,     0,    48,
       0,    50,   315,     0,   316,   317,   318,     0,   319,   320,
     137,   133,    58,   323,     0,     0,     0,     0,     0,     0,
     139,   330,     0,     0,     0,   139,   905,     0,   334,   335,
     336,     0,     0,     0,     0,   136,   137,    79,     0,   139,
     137,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   139,     0,     0,     0,     0,     0,   123,   599,     0,
     139,     0,     0,     0,   139,   376,   141,     0,   139,     0,
       0,     0,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,   599,     0,   311,   290,   141,   290,   141,
       0,   599,     0,     0,     0,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,   139,     0,   123,     0,   123,
       0,     0,   123,     0,   136,   123,     0,     0,     0,   133,
       0,     0,     0,     0,   123,     0,     0,   123,     0,     0,
       0,   312,   141,     0,     0,   313,   136,     0,     0,   136,
       0,     0,     0,     0,     0,     0,     0,     0,   136,   123,
     139,   139,   139,   139,     0,     0,     0,     0,   141,     0,
       0,   123,   141,   123,     0,     0,   137,   123,     0,   139,
     139,     0,     0,   137,   665,     0,   376,     0,   204,   133,
       0,   315,     0,     0,     0,   318,     0,   319,   320,     0,
     205,   206,   323,     0,     0,     0,     0,   207,     0,     0,
     330,     0,     0,     0,   136,   208,     0,   334,   335,   336,
       0,   209,     0,     0,     0,     0,   210,     0,   211,   141,
       0,   212,     0,     0,     0,     0,     0,   213,   137,   133,
       0,   133,     0,     0,   133,     0,   123,   133,   214,     0,
       0,   123,     0,     0,     0,     0,   133,   215,     0,   133,
       0,     0,     0,     0,     0,   123,   216,   217,     0,   218,
       0,   219,     0,   220,     0,     0,   221,   123,     0,     0,
     222,   133,     0,   223,     0,     0,   123,     0,     0,     0,
     123,     0,   142,   133,   123,   133,     0,     0,   141,   133,
       0,   142,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,   142,     0,   142,     0,   137,     0,     0,
       0,     0,   136,     0,     0,     0,     0,     0,     0,     0,
       0,   123,     0,     0,     0,     0,     0,     0,     0,   137,
       0,   227,   137,     0,     0,     0,     0,     0,     0,     0,
       0,   137,     0,     0,     0,     0,     0,     0,   142,     0,
     141,     0,     0,     0,     0,     0,     0,     0,   133,     0,
       0,     0,     0,   133,     0,     0,   123,   123,   123,   123,
       0,     0,   136,     0,   142,     0,     0,   133,   142,     0,
       0,     0,     0,     0,     0,   123,   123,     0,     0,   133,
       0,     0,     0,     0,     0,     0,     0,   137,   133,     0,
       0,     0,   133,     0,     0,     0,   133,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,   136,     0,   136,     0,     0,   136,     0,   141,
     136,     0,     0,     0,   232,   142,     0,     0,     0,   136,
       0,     0,   136,   133,   264,   268,     0,     0,     0,     0,
       0,   141,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,   893,   141,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   136,     0,   136,     0,
       0,     0,   136,     0,     0,     0,     0,     0,   133,   133,
     133,   133,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,   152,     0,   142,   137,     0,   133,   133,     0,
       0,   142,     0,     0,   181,   204,   184,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,   205,   206,     0,
       0,     0,     0,   942,   207,     0,     0,     0,     0,     0,
       0,     0,   208,     0,     0,     0,     0,   366,   209,     0,
       0,   136,     0,   210,     0,   211,   136,     0,   212,   261,
       0,     0,     0,     0,   213,   137,   142,   232,   232,   232,
     136,     0,     0,     0,   400,   214,     0,   974,     0,     0,
       0,     0,   136,     0,   215,    99,     0,     0,     0,   298,
       0,   136,     0,   216,   217,   136,   218,     0,   219,   136,
     220,     0,     0,   221,     0,     0,   232,   222,   406,   232,
     223,     0,     0,     0,     0,   137,   974,   137,     0,     0,
     137,     0,     0,   137,     0,     0,     0,   141,   232,     0,
       0,     0,   137,     0,   974,   137,   136,     0,     0,     0,
       0,     0,     0,     0,     0,   142,   359,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   142,   407,   137,
     142,   137,     0,     0,     0,   137,     0,     0,     0,   142,
       0,   136,   136,   136,   136,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     136,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,   416,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   142,     0,   141,     0,   141,
     232,     0,   141,     0,   137,   141,     0,     0,     0,   137,
       0,     0,     0,     0,   141,     0,     0,   141,     0,     0,
       0,     0,     0,   137,     0,     0,   232,     0,     0,     0,
       0,     0,     0,     0,     0,   137,     0,    99,     0,   141,
       0,     0,     0,     0,   137,     0,     0,   734,   137,     0,
       0,   141,   137,   141,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,  -198,     0,     0,     0,  -198,
    -198,     0,     0,     0,     0,     0,     0,     0,  -198,     0,
    -198,  -198,     0,     0,     0,  -198,     0,     0,     0,   137,
    -198,     0,     0,  -198,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   142,     0,     0,     0,     0,  -198,     0,
    -198,     0,  -198,     0,  -198,  -198,    99,  -198,     0,  -198,
       0,  -198,     0,     0,     0,     0,   141,     0,     0,     0,
       0,   141,     0,     0,   137,   137,   137,   137,   416,     0,
    -198,   416,     0,  -198,     0,   141,  -198,     0,     0,     0,
       0,     0,     0,   137,   137,     0,     0,   141,     0,     0,
       0,     0,     0,   142,     0,     0,   141,     0,     0,     0,
     141,     0,     0,     0,   141,     0,     0,     0,     0,   232,
     232,     0,     0,   232,   232,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -198,   694,     0,     0,     0,
       0,  -198,     0,     0,     0,     0,   621,     0,     0,     0,
       0,   141,     0,   142,     0,   142,     0,     0,   142,     0,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
     142,     0,     0,   142,     0,     0,     0,   721,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,     0,     0,   142,   141,   141,   141,   141,
       0,     0,     0,     0,     0,     0,     0,   142,  -459,   142,
       0,     0,     0,   142,     0,   141,   141,     0,     0,     0,
       0,     0,   352,     0,     0,  -488,     0,  -488,     0,     0,
       0,     0,     0,     0,   232,   232,     0,     0,     0,     0,
       0,     0,   232,     0,   400,     0,     0,     0,     0,   400,
       0,     0,   232,     0,   692,   232,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   159,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,   142,    26,     0,     0,     0,   142,     0,    30,
     160,   161,     0,   162,    35,   838,    36,     0,     0,    38,
       0,   142,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,   142,   731,     0,    49,     0,    51,     0,
      53,     0,   142,     0,     0,    56,   142,     0,     0,     0,
     142,     0,     0,     0,    63,    64,    65,    66,    67,   163,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,   416,     0,   416,   142,     0,   416,
       0,     0,   416,     0,     0,     0,     0,    83,    84,    85,
      86,   818,     0,     0,   820,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,     0,     0,     0,     0,    93,   834,   232,     0,    95,
       0,   953,   142,   142,   142,   142,     0,     0,   840,     0,
     844,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,   142,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,    20,
      21,     0,   400,   400,     0,   400,   400,     0,    27,     0,
      28,   852,     0,     0,     0,    33,     0,     0,     0,     0,
      37,     0,   946,     0,     0,     0,     0,     0,   950,     0,
     400,     0,   400,     0,     0,     0,     0,     0,    48,     0,
      50,     0,   297,     0,   853,     0,     0,   854,     0,    57,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   888,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,    76,   904,     0,    79,     0,     0,     0,
       0,     0,     0,   909,     0,     0,     0,   911,     0,     0,
       0,    99,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   946,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    92,    25,    26,   937,    27,
       0,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,    37,     0,    38,  -212,    39,    40,    41,    42,    43,
      44,   -42,    45,    46,     0,    47,     0,     0,     0,    48,
      49,    50,    51,    52,    53,    54,  -212,   -42,    55,    56,
      57,     0,    58,    59,    60,     0,    61,    62,    63,    64,
      65,    66,    67,    68,     0,    69,    70,     0,    71,    72,
      73,    74,    75,     0,    76,    77,    78,    79,     0,     0,
      80,     0,    81,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,     0,    92,     0,     0,    93,
       0,    94,     0,    95,   983,    96,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   159,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     160,   161,     0,   162,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   163,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   245,     0,     0,     0,    93,     0,   164,     0,    95,
       0,   165,   984,     4,     0,     5,     6,     7,     8,     9,
      10,     0,  -511,     0,    11,    12,    13,    14,    15,  -511,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    -511,    25,    26,  -511,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -212,
      39,    40,    41,    42,    43,    44,   -42,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -212,   -42,    55,    56,    57,  -511,    58,    59,    60,
    -511,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
    -511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -511,    84,  -511,  -511,
    -511,  -511,  -511,  -511,  -511,     0,  -511,  -511,  -511,  -511,
    -511,     0,  -511,  -511,  -511,  -511,    89,  -511,  -511,  -511,
       0,    92,  -511,  -511,  -511,     0,    94,  -511,    95,   283,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -212,    39,    40,    41,    42,
      43,    44,   -42,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,    52,    53,    54,  -212,   -42,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,   245,    92,     0,     0,
      93,     0,    94,   284,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -212,    39,    40,    41,    42,    43,    44,   -42,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,  -212,   -42,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,   444,
      95,   283,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,    36,    37,     0,    38,  -212,    39,    40,
      41,    42,    43,    44,   -42,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,    52,    53,    54,  -212,
     -42,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,   284,    95,   848,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -212,    39,    40,    41,    42,    43,    44,
     -42,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,    52,    53,    54,  -212,   -42,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,   849,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,    34,    35,     0,    36,    37,     0,    38,  -212,
      39,    40,    41,    42,    43,    44,   -42,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,    52,    53,
      54,  -212,   -42,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,   913,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,    34,    35,
       0,    36,    37,     0,    38,  -212,    39,    40,    41,    42,
      43,    44,   -42,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,   297,    53,    54,  -212,   -42,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,    37,     0,
      38,  -212,    39,    40,    41,    42,    43,    44,   -42,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -212,   -42,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     545,    35,     0,    36,    37,     0,    38,  -212,    39,    40,
      41,    42,    43,    44,   -42,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -212,
     -42,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,    25,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,   549,    35,     0,    36,
      37,     0,    38,  -212,    39,    40,    41,    42,    43,    44,
     -42,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -212,   -42,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   803,    35,     0,    36,    37,     0,    38,  -212,
      39,    40,    41,    42,    43,    44,   -42,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -212,   -42,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   805,    35,
       0,    36,    37,     0,    38,  -212,    39,    40,    41,    42,
      43,    44,   -42,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -212,   -42,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,     4,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,    25,    26,     0,    27,     0,    28,    29,
      30,    31,    32,    33,   808,    35,     0,    36,    37,     0,
      38,  -212,    39,    40,    41,    42,    43,    44,   -42,    45,
      46,     0,    47,     0,     0,     0,    48,    49,    50,    51,
       0,    53,    54,  -212,   -42,    55,    56,    57,     0,    58,
      59,    60,     0,    61,    62,    63,    64,    65,    66,    67,
      68,     0,    69,    70,     0,    71,    72,    73,    74,    75,
       0,    76,    77,    78,    79,     0,     0,    80,     0,    81,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,    92,     0,     0,    93,     0,    94,     0,
      95,     4,    96,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,    25,
      26,     0,    27,     0,    28,    29,    30,    31,    32,    33,
     811,    35,     0,    36,    37,     0,    38,  -212,    39,    40,
      41,    42,    43,    44,   -42,    45,    46,     0,    47,     0,
       0,     0,    48,    49,    50,    51,     0,    53,    54,  -212,
     -42,    55,    56,    57,     0,    58,    59,    60,     0,    61,
      62,    63,    64,    65,    66,    67,    68,     0,    69,    70,
       0,    71,    72,    73,    74,    75,     0,    76,    77,    78,
      79,     0,     0,    80,     0,    81,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,     0,    92,
       0,     0,    93,     0,    94,     0,    95,     4,    96,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,   833,    26,     0,    27,     0,
      28,    29,    30,    31,    32,    33,    34,    35,     0,    36,
      37,     0,    38,  -212,    39,    40,    41,    42,    43,    44,
     -42,    45,    46,     0,    47,     0,     0,     0,    48,    49,
      50,    51,     0,    53,    54,  -212,   -42,    55,    56,    57,
       0,    58,    59,    60,     0,    61,    62,    63,    64,    65,
      66,    67,    68,     0,    69,    70,     0,    71,    72,    73,
      74,    75,     0,    76,    77,    78,    79,     0,     0,    80,
       0,    81,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    88,     0,
      89,     0,    90,    91,     0,    92,     0,     0,    93,     0,
      94,     0,    95,     4,    96,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       0,    25,    26,     0,    27,     0,    28,    29,    30,    31,
      32,    33,   839,    35,     0,    36,    37,     0,    38,  -212,
      39,    40,    41,    42,    43,    44,   -42,    45,    46,     0,
      47,     0,     0,     0,    48,    49,    50,    51,     0,    53,
      54,  -212,   -42,    55,    56,    57,     0,    58,    59,    60,
       0,    61,    62,    63,    64,    65,    66,    67,    68,     0,
      69,    70,     0,    71,    72,    73,    74,    75,     0,    76,
      77,    78,    79,     0,     0,    80,     0,    81,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,     0,    89,     0,    90,    91,
       0,    92,     0,     0,    93,     0,    94,     0,    95,     4,
      96,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,    25,    26,     0,
      27,     0,    28,    29,    30,    31,    32,    33,   843,    35,
       0,    36,    37,     0,    38,  -212,    39,    40,    41,    42,
      43,    44,   -42,    45,    46,     0,    47,     0,     0,     0,
      48,    49,    50,    51,     0,    53,    54,  -212,   -42,    55,
      56,    57,     0,    58,    59,    60,     0,    61,    62,    63,
      64,    65,    66,    67,    68,     0,    69,    70,     0,    71,
      72,    73,    74,    75,     0,    76,    77,    78,    79,     0,
       0,    80,     0,    81,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,     0,    89,     0,    90,    91,     0,    92,     0,     0,
      93,     0,    94,     0,    95,   710,    96,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   159,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   162,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   245,     0,     0,     0,    93,     0,   164,     0,
      95,     0,   165,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   159,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   160,   161,     0,
     162,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   163,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   245,     0,
       0,     0,    93,     0,   164,     0,    95,     0,   165,   620,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   159,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   160,   161,     0,   162,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   163,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   245,     0,     0,     0,    93,
       0,   164,     0,    95,     0,   165,   940,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   159,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   162,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,     0,     0,     0,    93,     0,   164,     0,
      95,     0,   165,   625,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   159,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,   198,    21,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,   199,    30,   160,   161,
       0,   162,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,    50,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,    58,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   163,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,    79,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   742,     0,   743,     0,    83,    84,    85,    86,     0,
     744,     0,     0,    87,     0,   745,   206,   746,     0,     0,
       0,     0,   747,    88,     0,    89,     0,    90,    91,     0,
     208,     0,     0,    93,     0,   164,   748,    95,     0,   165,
       0,   749,     0,   211,     0,     0,   750,     0,     0,     0,
       0,     0,   751,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,     0,     0,     0,     0,     0,     0,
       0,   216,   217,     0,   753,     0,   219,     0,   754,     0,
       0,   755,     0,     0,     0,   756,     0,     0,   223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   337,
     338,   339,   340,   341,   342,     0,     0,   345,   346,   347,
     348,     0,   350,   351,   757,   758,   759,   760,     0,     0,
     761,     0,     0,     0,   762,   763,   764,   765,   766,   767,
     768,   769,   770,   771,   772,   773,     0,   774,     0,     0,
     775,   776,   777,   778,     0,     0,   779,     5,     6,     7,
       8,   188,    10,   189,     0,     0,   159,     0,     0,    14,
      15,     0,     0,  -115,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   162,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,  -115,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,     0,     0,   190,    75,
       0,  -115,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   159,
      90,    91,    14,    15,     0,     0,    93,     0,   164,    19,
      95,     0,   165,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   160,   161,     0,   162,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   163,     0,    69,     0,     0,    71,   249,
     250,     0,    75,   288,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
     289,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,    89,     0,    90,    91,   245,     0,     0,     0,    93,
       0,   164,     0,    95,     0,   165,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   159,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    30,
     160,   161,     0,   162,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   163,
       0,    69,     0,     0,    71,   249,   250,     0,    75,   288,
       0,    77,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   245,     0,     0,     0,    93,     0,   164,     0,    95,
     614,   165,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   159,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   160,   161,     0,   162,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   163,     0,    69,     0,     0,
      71,   249,   250,     0,    75,   288,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   245,     0,     0,
       0,    93,     0,   164,     0,    95,   616,   165,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   159,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,   514,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   160,   161,     0,   162,    35,   515,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
     516,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   163,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,   517,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,     0,     0,     0,     0,    93,     0,   164,
       0,    95,     0,   165,     5,     6,     7,     8,   188,    10,
       0,     0,     0,   159,     0,     0,    14,    15,     0,     0,
    -115,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   160,   161,
       0,   162,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,  -115,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   163,     0,    69,
       0,     0,    71,     0,     0,   190,    75,     0,  -115,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    88,     0,    89,   159,    90,    91,    14,
      15,     0,     0,    93,     0,   164,    19,    95,     0,   165,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   162,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,   249,   250,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,     0,   251,     0,     0,    93,     0,   164,     0,
      95,     0,   165,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   159,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   160,   161,     0,
     162,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   163,     0,    69,     0,
       0,    71,   249,   250,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    88,     0,    89,     0,    90,    91,   245,     0,
       0,     0,    93,     0,   164,     0,    95,     0,   165,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   159,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   160,   161,     0,   162,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,   712,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   163,     0,    69,     0,     0,    71,   249,   250,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   159,    90,    91,    14,    15,     0,     0,    93,     0,
     164,    19,    95,     0,   165,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   160,   161,     0,   162,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,   193,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   163,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,   194,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   159,    90,    91,    14,    15,     0,
       0,    93,     0,   164,    19,    95,     0,   165,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   160,
     161,     0,   162,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   163,     0,
      69,     0,     0,    71,   249,   250,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   159,    90,    91,
      14,    15,     0,     0,    93,     0,   164,    19,    95,     0,
     165,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   160,   161,     0,   162,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   163,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,   245,     0,     0,     0,    93,     0,   164,
     612,    95,     0,   165,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   159,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
     231,    26,     0,     0,     0,     0,     0,    30,   160,   161,
       0,   162,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   163,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    88,     0,    89,   159,    90,    91,    14,
      15,     0,     0,    93,     0,    94,    19,    95,     0,   165,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   162,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    88,     0,    89,     0,
      90,    91,   245,     0,     0,     0,    93,     0,   164,     0,
      95,     0,   165,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   159,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   160,   161,     0,
     162,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   163,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,   194,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   159,    90,    91,    14,   582,
       0,     0,    93,     0,   164,    19,    95,     0,   165,     0,
       0,     0,     0,   583,     0,     0,     0,     0,     0,    30,
     160,   161,     0,   162,    35,     0,   584,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,   585,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,   586,   587,    65,    66,    67,   588,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,   589,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    88,     0,    89,     0,    90,
      91,   245,     0,     0,     0,    93,     0,   164,     0,    95,
       0,   891,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   159,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   160,   161,     0,   162,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   163,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    88,     0,    89,     0,    90,    91,   245,     0,     0,
       0,    93,     0,   164,     0,    95,     0,   891,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   159,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   160,   161,     0,   876,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   163,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,     0,    89,
       0,    90,    91,   245,     0,     0,     0,    93,     0,   164,
       0,    95,     0,   165,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   159,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   160,   161,
       0,   162,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   163,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    88,     0,    89,   159,    90,    91,    14,
      15,     0,     0,    93,     0,   164,    19,    95,     0,   165,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   162,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   159,
      90,    91,    14,   479,     0,     0,    93,     0,    94,    19,
      95,     0,   165,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   160,   161,     0,   162,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   163,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   159,    90,    91,    14,    15,     0,     0,    93,
       0,   164,    19,    95,     0,   165,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   160,   161,     0,
     162,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   163,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   159,    90,    91,    14,   582,
       0,     0,   555,     0,   164,    19,    95,     0,   165,     0,
       0,     0,     0,   583,     0,     0,     0,     0,     0,    30,
     160,   161,     0,   162,    35,     0,   584,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,   585,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,   586,   587,    65,    66,    67,   588,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,   589,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   159,    90,
      91,    14,    15,     0,     0,    93,     0,   164,    19,    95,
       0,   590,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   160,   161,     0,   162,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   163,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,    87,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   159,    90,    91,    14,    15,     0,     0,    93,     0,
     164,    19,    95,     0,   590,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   160,   161,     0,   654,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   163,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,    84,    85,    86,     0,     0,     0,
       0,    87,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    88,     0,    89,   159,    90,    91,    14,    15,     0,
       0,    93,     0,   164,    19,    95,     0,   165,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,    30,   160,
     161,     0,   657,    35,     0,    36,     0,     0,    38,     0,
       0,    40,    41,     0,    43,    44,     0,    45,    46,     0,
       0,     0,     0,     0,     0,    49,     0,    51,     0,    53,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,    63,    64,    65,    66,    67,   163,     0,
      69,     0,     0,    71,     0,     0,     0,    75,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,    85,    86,
       0,     0,     0,     0,    87,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    88,     0,    89,   159,    90,    91,
      14,    15,     0,     0,    93,     0,   164,    19,    95,     0,
     165,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    30,   160,   161,     0,   870,    35,     0,    36,     0,
       0,    38,     0,     0,    40,    41,     0,    43,    44,     0,
      45,    46,     0,     0,     0,     0,     0,     0,    49,     0,
      51,     0,    53,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   163,     0,    69,     0,     0,    71,     0,     0,     0,
      75,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,     0,     0,     0,     0,    87,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    88,     0,    89,
     159,    90,    91,    14,    15,     0,     0,    93,     0,   164,
      19,    95,     0,   165,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,    30,   160,   161,     0,   871,    35,
       0,    36,     0,     0,    38,     0,     0,    40,    41,     0,
      43,    44,     0,    45,    46,     0,     0,     0,     0,     0,
       0,    49,     0,    51,     0,    53,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    63,
      64,    65,    66,    67,   163,     0,    69,     0,     0,    71,
       0,     0,     0,    75,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,    84,    85,    86,     0,     0,     0,     0,
      87,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      88,     0,    89,   159,    90,    91,    14,    15,     0,     0,
      93,     0,   164,    19,    95,     0,   165,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    30,   160,   161,
       0,   873,    35,     0,    36,     0,     0,    38,     0,     0,
      40,    41,     0,    43,    44,     0,    45,    46,     0,     0,
       0,     0,     0,     0,    49,     0,    51,     0,    53,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,   163,     0,    69,
       0,     0,    71,     0,     0,     0,    75,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,     0,
       0,     0,     0,    87,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    88,     0,    89,   159,    90,    91,    14,
      15,     0,     0,    93,     0,   164,    19,    95,     0,   165,
       0,     0,     0,     0,    26,     0,     0,     0,     0,     0,
      30,   160,   161,     0,   874,    35,     0,    36,     0,     0,
      38,     0,     0,    40,    41,     0,    43,    44,     0,    45,
      46,     0,     0,     0,     0,     0,     0,    49,     0,    51,
       0,    53,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,    67,
     163,     0,    69,     0,     0,    71,     0,     0,     0,    75,
       0,     0,    77,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,    84,
      85,    86,     0,     0,     0,     0,    87,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    88,     0,    89,   159,
      90,    91,    14,    15,     0,     0,    93,     0,   164,    19,
      95,     0,   165,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    30,   160,   161,     0,   875,    35,     0,
      36,     0,     0,    38,     0,     0,    40,    41,     0,    43,
      44,     0,    45,    46,     0,     0,     0,     0,     0,     0,
      49,     0,    51,     0,    53,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,   163,     0,    69,     0,     0,    71,     0,
       0,     0,    75,     0,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,     0,     0,     0,     0,    87,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    88,
       0,    89,   159,    90,    91,    14,    15,     0,     0,    93,
       0,   164,    19,    95,     0,   165,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   160,   161,     0,
     876,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,   163,     0,    69,     0,
       0,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,     0,     0,
       0,     0,    87,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    88,     0,    89,   159,    90,    91,    14,   582,
       0,     0,    93,     0,   164,    19,    95,     0,   165,     0,
       0,     0,     0,   583,     0,     0,     0,     0,     0,    30,
     160,   161,     0,   162,    35,     0,   584,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,   585,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,   586,   587,    65,    66,    67,   588,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,   589,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,    84,    85,
      86,     0,     0,     0,     0,    87,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    88,     0,    89,   159,    90,
      91,    14,    15,     0,     0,    93,     0,   164,    19,    95,
       0,   953,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,    30,   160,   161,     0,   162,    35,     0,    36,
       0,     0,    38,     0,     0,    40,    41,     0,    43,    44,
       0,    45,    46,     0,     0,     0,     0,     0,     0,    49,
       0,    51,     0,    53,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    64,    65,
      66,    67,   163,     0,    69,     0,     0,    71,     0,     0,
       0,    75,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    88,     0,
      89,   159,     0,    91,    14,    15,     0,     0,    93,     0,
     164,    19,    95,     0,   165,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    30,   160,   161,     0,   162,
      35,     0,    36,     0,     0,    38,     0,     0,    40,    41,
       0,    43,    44,     0,    45,    46,     0,     0,     0,     0,
       0,     0,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,    16,     0,     0,     0,    20,    21,     0,
      63,    64,    65,    66,    67,   163,    27,    69,    28,   852,
      71,     0,     0,    33,    75,     0,     0,    77,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -365,     0,
       0,     0,     0,     0,     0,     0,    48,     0,    50,     0,
      52,     0,   853,     0,    84,   854,     0,    57,  -365,    58,
       0,     0,  -365,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   159,    89,     0,    14,    15,   245,    74,     0,
       0,    76,    19,   164,    79,    95,  -365,   891,     0,     0,
      26,     0,     0,     0,     0,     0,    30,   160,   161,     0,
     162,    35,     0,    36,     0,     0,    38,     0,     0,    40,
      41,     0,    43,    44,     0,    45,    46,     0,     0,     0,
       0,     0,     0,    49,     0,    51,     0,    53,     0,     0,
       0,     0,    56,    92,     0,     0,     0,  -347,     0,   931,
       0,    63,    64,    65,    66,    67,   163,     0,    69,     0,
    -347,    71,     0,     0,     0,    75,     0,     0,    77,     0,
       0,     0,  -347,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,     0,     0,
       0,     0,     0,     0,     0,    84,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   159,     0,     0,    14,    15,
       0,     0,     0,     0,    89,    19,     0,   352,     0,  -347,
    -488,     0,  -488,    26,  -347,     0,    95,     0,   953,    30,
     160,   161,     0,   162,    35,     0,    36,     0,     0,    38,
       0,     0,    40,    41,     0,    43,    44,     0,    45,    46,
       0,     0,     0,     0,     0,     0,    49,     0,    51,     0,
      53,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,    67,   163,
       0,    69,     0,     0,    71,     0,     0,     0,    75,     0,
       0,    77,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   159,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,    84,    26,
       0,     0,     0,     0,     0,    30,   160,   161,     0,   162,
      35,     0,    36,     0,     0,    38,     0,    89,    40,    41,
       0,    43,    44,     0,    45,    46,     0,   164,     0,    95,
       0,   165,    49,     0,    51,     0,    53,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,   163,     0,    69,     0,     0,
      71,     0,     0,     0,    75,     0,     0,    77,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   159,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,    84,    26,     0,     0,     0,     0,
       0,    30,   160,   161,     0,   162,    35,     0,    36,     0,
       0,    38,     0,    89,    40,    41,     0,    43,    44,     0,
      45,    46,     0,   164,     0,    95,     0,   590,    49,     0,
      51,     0,    53,     0,     0,     0,   877,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,   163,     0,    69,  -198,     0,    71,     0,  -198,  -198,
      75,     0,     0,    77,     0,     0,     0,  -198,     0,  -198,
    -198,     0,     0,     0,  -198,     0,     0,     0,     0,  -198,
       0,     0,  -198,     0,     0,     0,     0,     0,     0,     0,
      84,     0,     0,     0,     0,     0,     0,  -198,     0,  -198,
     204,  -198,     0,  -198,  -198,     0,  -198,     0,  -198,    89,
    -198,     0,   205,   206,     0,     0,     0,     0,     0,   207,
       0,    95,     0,   165,     0,     0,     0,   208,     0,  -198,
       0,     0,  -198,   209,     0,  -198,     0,     0,   210,     0,
     211,     0,     0,   212,     0,     0,     0,     0,     0,   213,
       0,     0,     0,   204,     0,     0,     0,     0,     0,     0,
     214,     0,     0,     0,     0,   205,   206,     0,     0,   215,
       0,     0,   207,     0,     0,     0,     0,     0,   216,   217,
     208,   218,     0,   219,  -198,   220,   209,     0,   221,     0,
    -198,   210,   222,   211,     0,   223,   212,     0,     0,     0,
       0,     0,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,     0,     0,   920,     0,     0,     0,
       0,     0,   215,   744,     0,     0,     0,     0,   205,   206,
     746,   216,   217,     0,   218,   207,   219,     0,   220,     0,
       0,   221,     0,   208,     0,   222,     0,     0,   223,   748,
       0,   365,     0,   720,   210,     0,   211,     0,     0,   212,
       0,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214,     0,     0,     0,
       0,     0,     0,     0,     0,   215,     0,     0,     0,     0,
       0,     0,     0,     0,   216,   217,     0,   218,     0,   219,
       0,   921,     0,   309,   755,     0,   700,     0,   222,     0,
     310,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,   337,   338,   339,   340,   341,   342,     0,     0,
     345,   346,   347,   348,     0,   350,   351,   757,   758,   759,
     760,     0,     0,   761,     0,     0,     0,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,     0,
     774,   311,     0,   775,   776,   777,   778,   312,     0,     0,
       0,   313,     0,   309,     0,     0,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,   392,     0,     0,
       0,   314,     0,     0,     0,     0,     0,   312,     0,     0,
       0,   313,     0,     0,     0,     0,     0,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,   312,   331,   332,
     333,   313,     0,   334,   335,   336,     0,     0,   309,     0,
       0,     0,   729,     0,     0,   310,     0,   315,     0,   316,
     317,   318,     0,   319,   320,   321,   311,   322,   323,   324,
     325,   314,     0,   327,   328,   329,   330,     0,   331,     0,
     333,     0,     0,   334,   335,   336,     0,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,   312,   334,   335,   336,   313,     0,   309,     0,
       0,     0,   797,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,   316,   317,   318,     0,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,   312,   331,   332,   333,   313,     0,   334,   335,
     336,     0,     0,   309,     0,     0,     0,   949,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,   314,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,   316,   317,   318,     0,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,     0,   331,   332,   333,     0,   312,   334,   335,
     336,   313,     0,   309,     0,     0,     0,   997,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,   312,   331,   332,
     333,   313,     0,   334,   335,   336,     0,     0,   309,     0,
     613,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,   312,   334,   335,   336,   313,     0,     0,     0,
     919,     0,     0,   309,     0,   399,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,   314,   337,   338,   339,
     340,   341,   342,     0,     0,   345,   346,   347,   348,     0,
     350,   351,   315,     0,   316,   317,   318,     0,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,     0,   331,   332,   333,     0,   312,   334,   335,
     336,   313,   365,     0,     0,     0,     0,     0,   309,     0,
     802,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,   314,   337,   338,   339,   340,   341,   342,     0,     0,
     345,   346,   347,   348,     0,   350,   351,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,   312,   334,   335,   336,   313,   365,     0,     0,
       0,     0,     0,   309,     0,   807,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,   314,   337,   338,   339,
     340,   341,   342,     0,     0,   345,   346,   347,   348,     0,
     350,   351,   315,     0,   316,   317,   318,     0,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,     0,   331,   332,   333,     0,   312,   334,   335,
     336,   313,   365,     0,     0,     0,     0,     0,   309,     0,
     932,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,   314,   337,   338,   339,   340,   341,   342,     0,     0,
     345,   346,   347,   348,     0,   350,   351,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,   312,   334,   335,   336,   313,   365,     0,     0,
       0,     0,     0,   309,     0,   933,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,   314,   337,   338,   339,
     340,   341,   342,     0,     0,   345,   346,   347,   348,     0,
     350,   351,   315,     0,   316,   317,   318,     0,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,     0,   331,   332,   333,     0,   312,   334,   335,
     336,   313,   365,     0,     0,     0,     0,     0,   309,     0,
     934,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,   314,   337,   338,   339,   340,   341,   342,     0,     0,
     345,   346,   347,   348,     0,   350,   351,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,   312,   334,   335,   336,   313,   365,     0,     0,
       0,     0,     0,   309,     0,   935,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,   314,   337,   338,   339,
     340,   341,   342,     0,     0,   345,   346,   347,   348,     0,
     350,   351,   315,     0,   316,   317,   318,     0,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,     0,   331,   332,   333,     0,   312,   334,   335,
     336,   313,   365,     0,     0,     0,     0,     0,   309,     0,
     951,     0,     0,     0,     0,   310,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,   314,   337,   338,   339,   340,   341,   342,     0,     0,
     345,   346,   347,   348,     0,   350,   351,   315,     0,   316,
     317,   318,     0,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,   312,   334,   335,   336,   313,   365,     0,     0,
       0,     0,     0,     0,     0,   952,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,   314,   337,   338,   339,
     340,   341,   342,   311,   390,   345,   346,   347,   348,     0,
     350,   351,   315,     0,   316,   317,   318,   391,   319,   320,
     321,     0,   322,   323,   324,   325,   326,     0,   327,   328,
     329,   330,     0,   331,   332,   333,   309,     0,   334,   335,
     336,     0,   365,   310,     0,     0,     0,     0,     0,   312,
       0,     0,     0,   313,   311,   387,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
     312,   316,   317,   318,   313,   319,   320,   321,     0,   322,
     323,   324,   325,   326,     0,   327,   328,   329,   330,     0,
     331,   332,   333,   309,     0,   334,   335,   336,     0,   365,
     310,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   311,   681,     0,     0,     0,     0,     0,     0,     0,
     315,     0,   316,   317,   318,     0,   319,   320,   321,     0,
     322,   323,   324,   325,   326,     0,   327,   328,   329,   330,
       0,   331,   332,   333,   309,     0,   334,   335,   336,     0,
     365,   310,     0,     0,     0,     0,     0,   312,     0,     0,
       0,   313,   311,   231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,   392,     0,     0,
       0,   314,     0,   311,   675,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,   312,   316,
     317,   318,   313,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,     0,     0,   334,   335,   336,     0,   365,     0,   312,
       0,     0,   314,   313,     0,   309,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,   311,   319,   320,   321,     0,   322,   323,
     324,   325,   326,   314,   327,   328,   329,   330,     0,   331,
     332,   333,     0,     0,   334,   335,   336,     0,   365,   315,
       0,   316,   317,   318,     0,   319,   320,   321,     0,   322,
     323,   324,   325,   326,     0,   327,   328,   329,   330,   312,
     331,   332,   333,   313,     0,   334,   335,   336,     0,   365,
     309,     0,   693,     0,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   316,   317,   318,     0,   319,   320,   321,     0,   322,
     323,   324,   325,   326,     0,   327,   328,   329,   330,     0,
     331,   332,   333,     0,   312,   334,   335,   336,   313,   365,
     309,     0,     0,     0,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,   396,     0,     0,     0,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,   316,   317,   318,   397,
     319,   320,   321,     0,   322,   323,   324,   325,   326,     0,
     327,   328,   329,   330,   312,   331,   332,   333,   313,     0,
     334,   335,   336,     0,   426,     0,     0,     0,     0,   309,
       0,     0,     0,     0,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,   315,     0,   316,   317,   318,     0,
     319,   320,   321,     0,   322,   323,   324,   325,   326,     0,
     327,   328,   329,   330,     0,   331,   332,   333,     0,   398,
     334,   335,   336,   312,     0,     0,     0,   313,     0,   309,
       0,     0,     0,     0,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,   439,   154,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,   440,   319,
     320,   321,     0,   322,   323,   324,   325,   326,     0,   327,
     328,   329,   330,   312,   331,   332,   333,   313,     0,   334,
     335,   336,     0,     0,   309,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   523,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,     0,   319,
     320,   321,     0,   322,   323,   324,   325,   326,     0,   327,
     328,   329,   330,     0,   331,   332,   333,     0,   312,   334,
     335,   336,   313,     0,   309,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,   525,     0,     0,     0,     0,     0,     0,
       0,     0,   314,     0,     0,   309,   526,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,   311,   319,   320,   321,     0,   322,   323,
     324,   325,   326,     0,   327,   328,   329,   330,   312,   331,
     332,   333,   313,     0,   334,   335,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,   310,     0,     0,     0,     0,     0,   312,
       0,     0,   314,   313,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,     0,   319,   320,   321,     0,   322,   323,
     324,   325,   326,   314,   327,   328,   329,   330,     0,   331,
     332,   333,     0,     0,   334,   335,   336,     0,     0,   315,
     312,   316,   317,   318,   313,   319,   320,   321,     0,   322,
     323,   324,   325,   326,     0,   327,   328,   329,   330,     0,
     331,   332,   333,   309,   430,   334,   335,   336,     0,     0,
     310,     0,     0,     0,   314,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,   316,   317,   318,     0,   319,   320,   321,     0,
     322,   323,   324,   325,   326,     0,   327,   328,   329,   330,
       0,   331,   332,   333,   309,   432,   334,   335,   336,     0,
       0,   310,     0,     0,     0,     0,     0,   312,     0,     0,
       0,   313,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,   310,     0,     0,
       0,   314,     0,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,   315,   312,   316,
     317,   318,   313,   319,   320,   321,     0,   322,   323,   324,
     325,   326,     0,   327,   328,   329,   330,     0,   331,   332,
     333,   309,   434,   334,   335,   336,     0,     0,   310,     0,
       0,     0,   314,     0,   312,     0,     0,     0,   313,   311,
       0,     0,     0,     0,     0,     0,     0,   527,   315,     0,
     316,   317,   318,     0,   319,   320,   321,     0,   322,   323,
     324,   325,   326,     0,   327,   328,   329,   330,   314,   331,
     332,   333,     0,   442,   334,   335,   336,     0,     0,     0,
       0,     0,     0,     0,   315,   312,   316,   317,   318,   313,
     319,   320,   321,     0,   322,   323,   324,   325,   326,     0,
     327,   328,   329,   330,     0,   331,   332,   333,   309,     0,
     334,   335,   336,     0,     0,   310,     0,     0,   457,   314,
       0,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,   316,   317,   318,
       0,   319,   320,   321,     0,   322,   323,   324,   325,   326,
       0,   327,   328,   329,   330,     0,   331,   332,   333,   309,
       0,   334,   335,   336,     0,     0,   310,     0,     0,     0,
       0,     0,   312,     0,     0,     0,   313,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,   312,   316,   317,   318,   313,   319,   320,
     321,     0,   322,   323,   324,   325,   326,  -460,   327,   328,
     329,   330,     0,   331,   332,   333,     0,     0,   334,   335,
     336,     0,     0,   154,   312,     0,     0,   314,   313,     0,
     309,   688,     0,     0,     0,     0,     0,   310,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,   311,   319,
     320,   321,     0,   322,   323,   324,   325,   326,   314,   327,
     328,   329,   330,     0,   331,   332,   333,     0,     0,   334,
     335,   336,     0,     0,   315,     0,   316,   317,   318,     0,
     319,   320,   321,     0,   322,   323,   324,   325,   326,     0,
     327,   328,   329,   330,   312,   331,   332,   333,   313,   664,
     334,   335,   336,     0,     0,   309,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   311,     0,     0,   691,     0,   314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,   316,   317,   318,     0,
     319,   320,   321,     0,   322,   323,   324,   325,   326,     0,
     327,   328,   329,   330,     0,   331,   332,   333,     0,   312,
     334,   335,   336,   313,     0,   309,     0,     0,     0,     0,
       0,     0,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   316,   317,   318,     0,   319,   320,   321,     0,   322,
     323,   324,   325,   326,     0,   327,   328,   329,   330,   312,
     331,   332,   333,   313,     0,   334,   335,   336,     0,     0,
     309,     0,     0,     0,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   792,
       0,     0,   732,   314,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   316,   317,   318,     0,   319,   320,   321,     0,   322,
     323,   324,   325,   326,     0,   327,   328,   329,   330,     0,
     331,   332,   333,     0,   312,   334,   335,   336,   313,     0,
     309,     0,     0,     0,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   311,   795,
       0,     0,     0,     0,     0,     0,     0,     0,   314,     0,
       0,   309,     0,     0,     0,     0,     0,     0,   310,     0,
       0,     0,     0,     0,   315,     0,   316,   317,   318,   311,
     319,   320,   321,     0,   322,   323,   324,   325,   326,     0,
     327,   328,   329,   330,   312,   331,   332,   333,   313,     0,
     334,   335,   336,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   309,   886,     0,     0,     0,     0,     0,   310,
       0,     0,     0,     0,     0,   312,     0,     0,   314,   313,
     311,     0,     0,     0,     0,     0,     0,     0,   869,     0,
       0,     0,     0,     0,   315,     0,   316,   317,   318,     0,
     319,   320,   321,     0,   322,   323,   324,   325,   326,   314,
     327,   328,   329,   330,     0,   331,   332,   333,     0,     0,
     334,   335,   336,     0,     0,   315,   312,   316,   317,   318,
     313,   319,   320,   321,     0,   322,   323,   324,   325,   326,
       0,   327,   328,   329,   330,     0,   331,   332,   333,   309,
       0,   334,   335,   336,     0,     0,   310,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,     0,     0,     0,     0,   315,     0,   316,   317,
     318,     0,   319,   320,   321,     0,   322,   323,   324,   325,
     326,     0,   327,   328,   329,   330,     0,   331,   332,   333,
       0,     0,   334,   335,   336,     0,     0,     0,     0,     0,
       0,     0,     0,   312,     0,     0,     0,   313,     0,     0,
       0,     0,     0,     0,   309,     0,   872,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,     0,   319,
     320,   321,     0,   322,   323,   324,   325,   326,     0,   327,
     328,   329,   330,     0,   331,   332,   333,     0,   312,   334,
     335,   336,   313,     0,     0,     0,     0,     0,     0,   309,
       0,   962,     0,     0,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,     0,   319,   320,   321,     0,   322,   323,
     324,   325,   326,     0,   327,   328,   329,   330,     0,   331,
     332,   333,     0,   312,   334,   335,   336,   313,     0,     0,
       0,     0,     0,     0,   309,     0,   963,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,     0,   319,
     320,   321,     0,   322,   323,   324,   325,   326,     0,   327,
     328,   329,   330,     0,   331,   332,   333,     0,   312,   334,
     335,   336,   313,     0,     0,     0,     0,     0,     0,   309,
       0,   964,     0,     0,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,     0,   319,   320,   321,     0,   322,   323,
     324,   325,   326,     0,   327,   328,   329,   330,     0,   331,
     332,   333,     0,   312,   334,   335,   336,   313,     0,     0,
       0,     0,     0,     0,   309,     0,   965,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,     0,   319,
     320,   321,     0,   322,   323,   324,   325,   326,     0,   327,
     328,   329,   330,     0,   331,   332,   333,     0,   312,   334,
     335,   336,   313,     0,     0,     0,     0,     0,     0,   309,
       0,   966,     0,     0,     0,     0,   310,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,     0,   319,   320,   321,     0,   322,   323,
     324,   325,   326,     0,   327,   328,   329,   330,     0,   331,
     332,   333,     0,   312,   334,   335,   336,   313,     0,     0,
       0,     0,     0,     0,   309,     0,   967,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,   314,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,   316,   317,   318,     0,   319,
     320,   321,     0,   322,   323,   324,   325,   326,     0,   327,
     328,   329,   330,     0,   331,   332,   333,     0,   312,   334,
     335,   336,   313,     0,   309,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,   311,   319,   320,   321,     0,   322,   323,
     324,   325,   326,     0,   327,   328,   329,   330,   652,   331,
     332,   333,   313,     0,   334,   335,   336,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
       0,     0,   314,   313,   311,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,     0,
     316,   317,   318,     0,   319,   320,   321,     0,   322,   323,
     324,   325,   326,   314,   327,   328,   329,   330,     0,   331,
     332,   333,     0,     0,   334,   335,   336,     0,     0,   315,
     312,   316,   317,   318,   313,   319,   320,   321,     0,   322,
     323,   324,   325,     0,     0,   327,   328,   329,   330,     0,
     331,   332,   333,     0,     0,   334,   335,   336,     0,     0,
     630,     0,   204,     0,   314,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   205,   206,     0,     0,     0,     0,
     315,   207,   316,   317,   318,     0,   319,   320,   321,   208,
     322,   323,   324,   325,     0,   209,   327,   328,   329,   330,
     210,   331,   211,   333,     0,   212,   334,   335,   336,     0,
       0,   213,     0,     0,     0,   204,     0,     0,     0,     0,
       0,     0,   214,     0,     0,     0,     0,   205,   206,     0,
       0,   215,     0,     0,   207,     0,     0,     0,     0,     0,
     216,   217,   208,   218,     0,   219,     0,   220,   209,     0,
     221,     0,     0,   210,   222,   211,     0,   223,   212,     0,
       0,     0,     0,     0,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,     0,     0,     0,     0,
       0,     0,     0,     0,   215,     0,     0,     0,     0,     0,
       0,     0,     0,   216,   217,     0,   218,     0,   219,     0,
     220,     0,     0,   221,     0,     0,     0,   222,     0,     0,
     223
};

static const yytype_int16 yycheck[] =
{
      11,     2,    54,    55,    15,   139,    20,    18,    59,    17,
     435,   294,   553,    24,   639,   709,   177,    28,    29,    28,
      31,    32,    33,    34,   127,     0,   735,    78,     1,   190,
       1,    42,   825,    44,    45,   407,    47,     1,    49,    42,
      51,    29,     1,    42,    77,    96,   358,   185,    59,    60,
      61,    62,    63,    64,   203,   581,    39,    68,   900,    70,
      29,    72,    73,   412,    29,   203,    77,    78,   129,    80,
      81,     1,    29,    84,   404,   178,    87,    88,    89,   104,
      91,   783,   784,    94,    95,    96,   435,   129,   940,   129,
      77,    74,   241,   787,   129,    42,   157,   156,   397,   158,
     979,    60,    47,   241,    77,    60,    77,   822,   649,   104,
      42,   260,    42,   155,   165,    42,   104,    90,   997,   159,
     155,   154,   260,   916,    69,   651,   129,   979,    75,   102,
     129,   102,   826,   267,   159,    42,    42,   120,    31,   122,
      42,   440,   101,    75,   986,   109,   101,   856,   159,   160,
     161,   162,   163,   164,   165,   128,   159,   128,   149,   129,
     129,     3,   149,    28,   128,   129,   154,   154,   176,   878,
      19,   129,    19,     7,   185,   890,   149,   104,   192,   109,
     706,   154,   320,   154,   198,   154,    75,   157,   199,   154,
     199,   149,   203,   157,   177,   889,   155,   154,   128,   129,
     155,    60,   129,   984,   156,   354,   355,   190,   580,    74,
     193,   149,   993,    78,    24,   129,   354,   355,   129,   149,
     922,   923,   129,   129,    73,   156,    73,   129,   129,    85,
     241,   242,   159,   129,   156,   109,    95,   156,   249,   250,
     865,   553,    70,   157,   227,   939,   157,   109,    97,   260,
      97,   109,   159,   159,    64,   129,   157,   159,   129,   133,
     129,   157,   245,   128,   149,    75,   131,   129,   551,   552,
     135,   133,   129,   814,   129,   133,   129,   149,   289,   428,
     383,   149,     3,   157,    94,   286,   157,   155,   157,    24,
     428,   449,   149,   451,   149,   157,   149,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,    30,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   129,   139,   700,    64,
     129,    52,   343,   344,   385,   618,   619,   649,   349,   156,
      75,   352,   353,   354,   355,   356,   357,   358,   149,   129,
     149,    24,    72,    73,   157,   156,   367,   158,    42,    77,
     149,   372,   129,   374,   149,   100,     2,    88,   698,   149,
     155,   149,    90,    94,   385,    11,   387,   388,   149,   390,
     391,   395,   149,    77,   102,   396,   397,    23,   399,    25,
     401,    64,    42,    77,    74,   149,    90,   128,    78,   382,
     131,   539,    75,   414,   135,   735,   149,   832,   102,    74,
     421,   462,   156,    78,   158,   131,    24,   428,   102,   312,
     313,    94,   154,   149,   407,   109,   103,    77,   439,   440,
     109,   149,    68,   156,   156,   446,   154,   156,   449,   158,
     451,   149,   453,   454,   455,   129,   457,   458,   609,   156,
     149,   462,   102,     7,    54,   149,    64,   128,    94,   109,
     154,   154,    98,   356,   357,   149,   149,    75,   479,   480,
     154,   109,   157,   157,   367,    77,   825,   129,   461,   129,
     149,   464,    42,   832,   343,   344,    94,    31,    90,   156,
     349,   157,   100,   352,   353,   388,   129,   157,   391,   149,
     102,   157,   814,   109,   154,   639,   157,   157,    18,   129,
     159,   154,   523,   524,   525,   526,   527,   528,   529,   155,
     149,   149,   530,   531,   149,   149,   537,   149,   539,   159,
     157,   861,   543,   149,   545,   129,   149,   149,   549,   590,
       2,    31,   553,   157,   155,   149,   149,   149,   878,    11,
     157,   129,   154,   154,   109,    75,   155,    29,    29,   154,
     453,    23,   159,    25,    31,   128,    75,   916,   159,   128,
     159,   582,   159,    62,   585,   586,   587,   588,   589,   590,
     155,   131,   156,   594,   131,   131,   131,    31,   224,   131,
     449,   940,   451,   576,   131,   231,   131,   580,   131,   131,
     949,   131,   131,   102,    19,   157,    68,   155,    23,    24,
      72,    73,    90,   733,   157,   440,   439,    32,   159,    34,
      35,    22,   605,   268,    39,   720,   609,   191,   541,    44,
     979,   524,    94,   526,   800,   984,    98,   738,   649,   865,
     861,   652,   653,   654,   993,   698,   657,    62,   997,    64,
     286,    66,   573,    68,   741,   986,    71,   939,    73,    33,
      75,   958,   673,   415,   675,   716,   539,   678,    -1,    -1,
     681,    -1,    -1,    24,    -1,   686,    -1,   688,   537,    94,
     691,    -1,    97,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    42,    -1,   155,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,   716,   195,   196,   197,    -1,
      61,    -1,    -1,    64,   697,    66,   727,   700,   729,    -1,
      42,   732,    42,    45,    75,    -1,   737,    -1,    -1,   365,
      -1,   865,    -1,    -1,   149,   594,    -1,   720,    -1,    61,
     155,    -1,    64,    94,    66,   234,    -1,    -1,   237,    -1,
      -1,   387,    -1,    75,   390,   738,     2,    77,    -1,   652,
     653,    -1,   224,   399,    -1,    11,    28,   256,   779,   231,
      -1,    -1,    94,    -1,    -1,   758,    -1,    23,   129,    25,
     791,   792,   102,   794,   795,   796,   797,   276,    -1,   109,
      -1,   802,   803,    28,   805,    -1,   807,   808,    -1,    -1,
     811,   853,   854,   814,    -1,    -1,   157,    -1,    -1,   129,
      -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    -1,   455,
     831,   832,    68,    -1,   286,    -1,   837,    -1,   839,   149,
      -1,    -1,   843,    -1,   154,    -1,    -1,   157,    -1,    74,
     891,   852,    -1,    78,    -1,    -1,    -1,    -1,    94,   850,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,   869,   870,
     871,   872,   873,   874,   875,   876,   128,    -1,   130,   131,
      -1,    -1,    -1,   135,    -1,   886,    -1,    -1,     2,   368,
     891,   143,    -1,    -1,   520,   896,    -1,    11,   899,   124,
     152,   126,   127,   128,    -1,   130,   131,   132,    -1,    23,
     135,    25,   953,   365,    -1,   394,    -1,   142,   143,   155,
      -1,    28,   147,    -1,    -1,   150,   151,   152,    -1,    -1,
       3,   932,   933,   934,   935,   387,    -1,   563,   390,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,   399,   949,    22,
     951,   952,   953,    -1,    68,    -1,    28,    -1,    -1,    -1,
      -1,   962,   963,   964,   965,   966,   967,    74,    41,    -1,
      -1,    78,    -1,    46,    -1,    -1,    -1,   978,   979,    52,
      94,    -1,    -1,    -1,    98,    -1,    -1,    -1,   224,    -1,
      -1,    -1,   993,    -1,    -1,   231,   997,   623,    -1,    72,
      -1,    -1,    74,   455,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    84,    -1,   639,    -1,    88,    -1,   124,    -1,   126,
     127,   128,    95,   130,   131,   132,   652,   653,   135,   136,
     137,    -1,    -1,   140,   141,   142,   143,    -1,    -1,    -1,
     147,   155,    28,   150,   151,   152,    -1,   673,    -1,   675,
     286,    -1,   678,    -1,    -1,   681,   128,    -1,   130,   131,
      -1,    -1,    95,   135,   690,    -1,    -1,   693,   547,   548,
      -1,   143,   551,   552,    -1,    -1,    -1,    -1,   150,   151,
     152,     2,    -1,    -1,    -1,    23,    24,    -1,    74,   715,
      11,    -1,    78,    -1,    -1,    -1,    34,    35,    -1,    -1,
      -1,   727,    23,   729,    25,    -1,    44,   733,    -1,    47,
     224,   563,    -1,    -1,    -1,    -1,    -1,   231,    24,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    64,    -1,    -1,   365,
      -1,    69,    -1,    -1,    -1,    -1,    42,    75,   124,    45,
     126,   127,   128,    28,   130,   131,    -1,    68,    -1,   135,
      -1,   387,   185,    -1,   390,    61,   142,   143,    64,    -1,
      66,   147,   100,   399,   150,   151,   152,    -1,    -1,    75,
     203,   623,   286,    94,    -1,    -1,   802,    98,    -1,    -1,
      -1,   807,    -1,   662,   663,    -1,    -1,    -1,    94,    74,
      -1,   670,    -1,    78,    -1,   821,    -1,    -1,    -1,    -1,
      -1,   680,    -1,    -1,   683,    -1,    -1,   833,   241,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   842,    -1,    -1,   455,
     846,   673,    -1,   675,   850,    -1,   678,   260,    -1,   681,
      -1,    -1,    -1,    -1,   155,    -1,    28,    -1,   690,   124,
      -1,   693,   127,   128,    -1,   130,   131,    -1,    -1,    -1,
     135,   365,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,   887,    -1,   715,    -1,   150,   151,   152,    -1,    -1,
      -1,    -1,    -1,   387,     2,   727,   390,   729,    -1,    -1,
      -1,   733,    74,    11,    -1,   399,    78,    -1,    -1,    -1,
      23,    24,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,
      -1,    34,    35,   224,    -1,    -1,   932,   933,   934,   935,
     231,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   354,   355,    -1,    -1,   951,   952,   563,    -1,    62,
      -1,    64,   124,    -1,   126,   127,   128,    -1,   130,   131,
      68,   455,    75,   135,    -1,    -1,    -1,    -1,    -1,    -1,
     802,   143,    -1,    -1,    -1,   807,   835,    -1,   150,   151,
     152,    -1,    -1,    -1,    -1,   286,    94,   100,    -1,   821,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   833,    -1,    -1,    -1,    -1,    -1,   623,   421,    -1,
     842,    -1,    -1,    -1,   846,   428,     2,    -1,   850,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   446,    -1,    28,   449,    23,   451,    25,
      -1,   454,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   887,    -1,   673,    -1,   675,
      -1,    -1,   678,    -1,   365,   681,    -1,    -1,    -1,   563,
      -1,    -1,    -1,    -1,   690,    -1,    -1,   693,    -1,    -1,
      -1,    74,    68,    -1,    -1,    78,   387,    -1,    -1,   390,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   399,   715,
     932,   933,   934,   935,    -1,    -1,    -1,    -1,    94,    -1,
      -1,   727,    98,   729,    -1,    -1,   224,   733,    -1,   951,
     952,    -1,    -1,   231,   537,    -1,   539,    -1,     3,   623,
      -1,   124,    -1,    -1,    -1,   128,    -1,   130,   131,    -1,
      15,    16,   135,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     143,    -1,    -1,    -1,   455,    30,    -1,   150,   151,   152,
      -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,   155,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    52,   286,   673,
      -1,   675,    -1,    -1,   678,    -1,   802,   681,    63,    -1,
      -1,   807,    -1,    -1,    -1,    -1,   690,    72,    -1,   693,
      -1,    -1,    -1,    -1,    -1,   821,    81,    82,    -1,    84,
      -1,    86,    -1,    88,    -1,    -1,    91,   833,    -1,    -1,
      95,   715,    -1,    98,    -1,    -1,   842,    -1,    -1,    -1,
     846,    -1,     2,   727,   850,   729,    -1,    -1,   224,   733,
      -1,    11,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    25,    -1,   365,    -1,    -1,
      -1,    -1,   563,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   887,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
      -1,   156,   390,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,
     286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   802,    -1,
      -1,    -1,    -1,   807,    -1,    -1,   932,   933,   934,   935,
      -1,    -1,   623,    -1,    94,    -1,    -1,   821,    98,    -1,
      -1,    -1,    -1,    -1,    -1,   951,   952,    -1,    -1,   833,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   455,   842,    -1,
      -1,    -1,   846,    -1,    -1,    -1,   850,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   673,    -1,   675,    -1,    -1,   678,    -1,   365,
     681,    -1,    -1,    -1,    62,   155,    -1,    -1,    -1,   690,
      -1,    -1,   693,   887,    72,    73,    -1,    -1,    -1,    -1,
      -1,   387,    -1,    -1,   390,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   825,   399,   715,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   727,    -1,   729,    -1,
      -1,    -1,   733,    -1,    -1,    -1,    -1,    -1,   932,   933,
     934,   935,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    -1,   224,   563,    -1,   951,   952,    -1,
      -1,   231,    -1,    -1,    23,     3,    25,    -1,    -1,   455,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,   896,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,   175,    36,    -1,
      -1,   802,    -1,    41,    -1,    43,   807,    -1,    46,    68,
      -1,    -1,    -1,    -1,    52,   623,   286,   195,   196,   197,
     821,    -1,    -1,    -1,   202,    63,    -1,   940,    -1,    -1,
      -1,    -1,   833,    -1,    72,    94,    -1,    -1,    -1,    98,
      -1,   842,    -1,    81,    82,   846,    84,    -1,    86,   850,
      88,    -1,    -1,    91,    -1,    -1,   234,    95,    96,   237,
      98,    -1,    -1,    -1,    -1,   673,   979,   675,    -1,    -1,
     678,    -1,    -1,   681,    -1,    -1,    -1,   563,   256,    -1,
      -1,    -1,   690,    -1,   997,   693,   887,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   365,   155,    -1,   276,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   715,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   156,   727,
     390,   729,    -1,    -1,    -1,   733,    -1,    -1,    -1,   399,
      -1,   932,   933,   934,   935,    -1,    -1,   623,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     951,   952,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,   231,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   455,    -1,   673,    -1,   675,
     368,    -1,   678,    -1,   802,   681,    -1,    -1,    -1,   807,
      -1,    -1,    -1,    -1,   690,    -1,    -1,   693,    -1,    -1,
      -1,    -1,    -1,   821,    -1,    -1,   394,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   833,    -1,   286,    -1,   715,
      -1,    -1,    -1,    -1,   842,    -1,    -1,     1,   846,    -1,
      -1,   727,   850,   729,    -1,    -1,    -1,   733,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      34,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,   887,
      44,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   563,    -1,    -1,    -1,    -1,    62,    -1,
      64,    -1,    66,    -1,    68,    69,   365,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,   802,    -1,    -1,    -1,
      -1,   807,    -1,    -1,   932,   933,   934,   935,   387,    -1,
      94,   390,    -1,    97,    -1,   821,   100,    -1,    -1,    -1,
      -1,    -1,    -1,   951,   952,    -1,    -1,   833,    -1,    -1,
      -1,    -1,    -1,   623,    -1,    -1,   842,    -1,    -1,    -1,
     846,    -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,   547,
     548,    -1,    -1,   551,   552,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   564,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,   455,    -1,    -1,    -1,
      -1,   887,    -1,   673,    -1,   675,    -1,    -1,   678,    -1,
      -1,   681,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     690,    -1,    -1,   693,    -1,    -1,    -1,   605,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,    -1,    -1,   715,   932,   933,   934,   935,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   727,   139,   729,
      -1,    -1,    -1,   733,    -1,   951,   952,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,   156,    -1,   158,    -1,    -1,
      -1,    -1,    -1,    -1,   662,   663,    -1,    -1,    -1,    -1,
      -1,    -1,   670,    -1,   672,    -1,    -1,    -1,    -1,   677,
      -1,    -1,   680,    -1,   563,   683,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   802,    30,    -1,    -1,    -1,   807,    -1,    36,
      37,    38,    -1,    40,    41,   723,    43,    -1,    -1,    46,
      -1,   821,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,   833,   623,    -1,    63,    -1,    65,    -1,
      67,    -1,   842,    -1,    -1,    72,   846,    -1,    -1,    -1,
     850,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,   673,    -1,   675,   887,    -1,   678,
      -1,    -1,   681,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   690,    -1,    -1,   693,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,    -1,    -1,    -1,   152,   715,   835,    -1,   156,
      -1,   158,   932,   933,   934,   935,    -1,    -1,   727,    -1,
     729,    -1,    -1,    -1,   733,    -1,    -1,    -1,    -1,    -1,
      -1,   951,   952,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,
      24,    -1,   880,   881,    -1,   883,   884,    -1,    32,    -1,
      34,    35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,   900,    -1,    -1,    -1,    -1,    -1,   906,    -1,
     908,    -1,   910,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      64,    -1,    66,    -1,    68,    -1,    -1,    71,    -1,    73,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   821,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    97,   833,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,   842,    -1,    -1,    -1,   846,    -1,    -1,
      -1,   850,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,   986,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,   149,    29,    30,   887,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    -1,    97,    98,    99,   100,    -1,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,    -1,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,
      -1,   158,   159,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    -1,    91,    92,    93,    94,    95,    -1,    97,
      98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,   149,   150,   151,   152,    -1,   154,   155,   156,     1,
     158,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    -1,    97,    98,    99,   100,    -1,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,   148,   149,    -1,    -1,
     152,    -1,   154,   155,   156,     1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    -1,    91,    92,    93,    94,    95,
      -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,   155,
     156,     1,   158,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    -1,    97,    98,    99,
     100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,
      -1,    -1,   152,    -1,   154,   155,   156,     1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    -1,    91,    92,    93,
      94,    95,    -1,    97,    98,    99,   100,    -1,    -1,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,
     154,   155,   156,     1,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    -1,    91,    92,    93,    94,    95,    -1,    97,
      98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
      -1,   149,    -1,    -1,   152,    -1,   154,   155,   156,     1,
     158,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    -1,    97,    98,    99,   100,    -1,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,
     152,    -1,   154,    -1,   156,     1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    -1,    91,    92,    93,    94,    95,
      -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,
     156,     1,   158,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    -1,    97,    98,    99,
     100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,
      -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    -1,    91,    92,    93,
      94,    95,    -1,    97,    98,    99,   100,    -1,    -1,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,
     154,    -1,   156,     1,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    -1,    91,    92,    93,    94,    95,    -1,    97,
      98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
      -1,   149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,
     158,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      62,    63,    64,    65,    -1,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    -1,    97,    98,    99,   100,    -1,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,
     152,    -1,   154,    -1,   156,     1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    73,    -1,    75,
      76,    77,    -1,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    -1,    91,    92,    93,    94,    95,
      -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,
     156,     1,   158,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    43,    44,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    -1,
      -1,    -1,    62,    63,    64,    65,    -1,    67,    68,    69,
      70,    71,    72,    73,    -1,    75,    76,    77,    -1,    79,
      80,    81,    82,    83,    84,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    -1,    97,    98,    99,
     100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,
      -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    43,
      44,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,
      64,    65,    -1,    67,    68,    69,    70,    71,    72,    73,
      -1,    75,    76,    77,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    -1,    88,    89,    -1,    91,    92,    93,
      94,    95,    -1,    97,    98,    99,   100,    -1,    -1,   103,
      -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,
     154,    -1,   156,     1,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    43,    44,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    76,    77,
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    -1,
      88,    89,    -1,    91,    92,    93,    94,    95,    -1,    97,
      98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
      -1,   149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,
     158,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      62,    63,    64,    65,    -1,    67,    68,    69,    70,    71,
      72,    73,    -1,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    -1,    97,    98,    99,   100,    -1,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,
     152,    -1,   154,    -1,   156,     1,   158,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,
     156,    -1,   158,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,   159,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,
      -1,   154,    -1,   156,    -1,   158,   159,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,
     156,    -1,   158,   159,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,    -1,   124,   125,   126,   127,    -1,
      10,    -1,    -1,   132,    -1,    15,    16,    17,    -1,    -1,
      -1,    -1,    22,   142,    -1,   144,    -1,   146,   147,    -1,
      30,    -1,    -1,   152,    -1,   154,    36,   156,    -1,   158,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,   117,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
     150,   151,   152,   153,    -1,    -1,   156,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    94,    95,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    92,
      93,    -1,    95,    96,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,
      -1,   154,    -1,   156,    -1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    92,    93,    -1,    95,    96,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,
     157,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    92,    93,    -1,    95,    96,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,    -1,
      -1,   152,    -1,   154,    -1,   156,   157,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    42,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,   154,
      -1,   156,    -1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    94,    95,    -1,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   142,    -1,   144,    12,   146,   147,    15,
      16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,
     156,    -1,   158,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    92,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   142,    -1,
     144,    12,   146,   147,    15,    16,    -1,    -1,   152,    -1,
     154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   142,    -1,   144,    12,   146,   147,    15,    16,    -1,
      -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    92,    93,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   142,    -1,   144,    12,   146,   147,
      15,    16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,
     158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,   154,
     155,   156,    -1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   142,    -1,   144,    12,   146,   147,    15,
      16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
     146,   147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,
     156,    -1,   158,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   142,    -1,   144,    12,   146,   147,    15,    16,
      -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,
      -1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,    -1,
      -1,   152,    -1,   154,    -1,   156,    -1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,   154,
      -1,   156,    -1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   142,    -1,   144,    12,   146,   147,    15,
      16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   142,
      -1,   144,    12,   146,   147,    15,    16,    -1,    -1,   152,
      -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   142,    -1,   144,    12,   146,   147,    15,    16,
      -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   142,    -1,   144,    12,   146,
     147,    15,    16,    -1,    -1,   152,    -1,   154,    22,   156,
      -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   142,    -1,
     144,    12,   146,   147,    15,    16,    -1,    -1,   152,    -1,
     154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   142,    -1,   144,    12,   146,   147,    15,    16,    -1,
      -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   142,    -1,   144,    12,   146,   147,
      15,    16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,
     158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   142,    -1,   144,
      12,   146,   147,    15,    16,    -1,    -1,   152,    -1,   154,
      22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
     142,    -1,   144,    12,   146,   147,    15,    16,    -1,    -1,
     152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   142,    -1,   144,    12,   146,   147,    15,
      16,    -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
     146,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   142,
      -1,   144,    12,   146,   147,    15,    16,    -1,    -1,   152,
      -1,   154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   142,    -1,   144,    12,   146,   147,    15,    16,
      -1,    -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   142,    -1,   144,    12,   146,
     147,    15,    16,    -1,    -1,   152,    -1,   154,    22,   156,
      -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   142,    -1,
     144,    12,    -1,   147,    15,    16,    -1,    -1,   152,    -1,
     154,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    19,    -1,    -1,    -1,    23,    24,    -1,
      81,    82,    83,    84,    85,    86,    32,    88,    34,    35,
      91,    -1,    -1,    39,    95,    -1,    -1,    98,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    -1,
      66,    -1,    68,    -1,   125,    71,    -1,    73,   129,    75,
      -1,    -1,   133,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,   144,    -1,    15,    16,   148,    94,    -1,
      -1,    97,    22,   154,   100,   156,   157,   158,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,
      40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,   149,    -1,    -1,    -1,    77,    -1,   155,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      90,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,   102,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,   144,    22,    -1,   153,    -1,   149,
     156,    -1,   158,    30,   154,    -1,   156,    -1,   158,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   125,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,   144,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,   154,    -1,   156,
      -1,   158,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    38,    -1,    40,    41,    -1,    43,    -1,
      -1,    46,    -1,   144,    49,    50,    -1,    52,    53,    -1,
      55,    56,    -1,   154,    -1,   156,    -1,   158,    63,    -1,
      65,    -1,    67,    -1,    -1,    -1,     1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    19,    -1,    91,    -1,    23,    24,
      95,    -1,    -1,    98,    -1,    -1,    -1,    32,    -1,    34,
      35,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    64,
       3,    66,    -1,    68,    69,    -1,    71,    -1,    73,   144,
      75,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   156,    -1,   158,    -1,    -1,    -1,    30,    -1,    94,
      -1,    -1,    97,    36,    -1,   100,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    72,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    81,    82,
      30,    84,    -1,    86,   149,    88,    36,    -1,    91,    -1,
     155,    41,    95,    43,    -1,    98,    46,    -1,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    72,    10,    -1,    -1,    -1,    -1,    15,    16,
      17,    81,    82,    -1,    84,    22,    86,    -1,    88,    -1,
      -1,    91,    -1,    30,    -1,    95,    -1,    -1,    98,    36,
      -1,   154,    -1,   156,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    -1,    84,    -1,    86,
      -1,    88,    -1,    10,    91,    -1,   156,    -1,    95,    -1,
      17,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,    -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    28,    -1,   150,   151,   152,   153,    74,    -1,    -1,
      -1,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,    -1,    10,    -1,
      -1,    -1,   159,    -1,    -1,    17,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    28,   134,   135,   136,
     137,   108,    -1,   140,   141,   142,   143,    -1,   145,    -1,
     147,    -1,    -1,   150,   151,   152,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    10,    -1,
      -1,    -1,   159,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    74,   145,   146,   147,    78,    -1,   150,   151,
     152,    -1,    -1,    10,    -1,    -1,    -1,   159,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    10,    -1,    -1,    -1,   159,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,    -1,    10,    -1,
     157,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,
     157,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,   108,   109,   110,   111,
     112,   113,   114,    28,    29,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126,   127,   128,    42,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    10,    -1,   150,   151,
     152,    -1,   154,    17,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    78,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      74,   126,   127,   128,    78,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    10,    -1,   150,   151,   152,    -1,   154,
      17,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    10,    -1,   150,   151,   152,    -1,
     154,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    74,   126,
     127,   128,    78,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,    -1,   154,    -1,    74,
      -1,    -1,   108,    78,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    28,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,   108,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,    -1,   154,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    74,
     145,   146,   147,    78,    -1,   150,   151,   152,    -1,   154,
      10,    -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,    74,   150,   151,   152,    78,   154,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    59,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    74,   145,   146,   147,    78,    -1,
     150,   151,   152,    -1,   154,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,   152,    74,    -1,    -1,    -1,    78,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    33,   104,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    59,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    74,   145,   146,   147,    78,    -1,   150,
     151,   152,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    10,    42,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    28,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    74,   145,
     146,   147,    78,    -1,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,   108,    78,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,   108,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,    -1,    -1,   124,
      74,   126,   127,   128,    78,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    10,   149,   150,   151,   152,    -1,    -1,
      17,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    10,   149,   150,   151,   152,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    74,   126,
     127,   128,    78,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    10,   149,   150,   151,   152,    -1,    -1,    17,    -1,
      -1,    -1,   108,    -1,    74,    -1,    -1,    -1,    78,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,   108,   145,
     146,   147,    -1,   149,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    74,   126,   127,   128,    78,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    10,    -1,
     150,   151,   152,    -1,    -1,    17,    -1,    -1,   107,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    10,
      -1,   150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    74,   126,   127,   128,    78,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,    -1,    -1,   104,    74,    -1,    -1,   108,    78,    -1,
      10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    28,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,   108,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    74,   145,   146,   147,    78,   149,
     150,   151,   152,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    31,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,    74,
     150,   151,   152,    78,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    74,
     145,   146,   147,    78,    -1,   150,   151,   152,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,    74,   150,   151,   152,    78,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    28,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    74,   145,   146,   147,    78,    -1,
     150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,   108,    78,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,   108,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,    -1,
     150,   151,   152,    -1,    -1,   124,    74,   126,   127,   128,
      78,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    10,
      -1,   150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    28,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    74,   145,
     146,   147,    78,    -1,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,   108,    78,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,   108,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,    -1,    -1,   124,
      74,   126,   127,   128,    78,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,    -1,    -1,
       1,    -1,     3,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
     124,    22,   126,   127,   128,    -1,   130,   131,   132,    30,
     134,   135,   136,   137,    -1,    36,   140,   141,   142,   143,
      41,   145,    43,   147,    -1,    46,   150,   151,   152,    -1,
      -1,    52,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    15,    16,    -1,
      -1,    72,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      81,    82,    30,    84,    -1,    86,    -1,    88,    36,    -1,
      91,    -1,    -1,    41,    95,    43,    -1,    98,    46,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    -1,    84,    -1,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   164,   165,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    29,    30,    32,    34,    35,
      36,    37,    38,    39,    40,    41,    43,    44,    46,    48,
      49,    50,    51,    52,    53,    55,    56,    58,    62,    63,
      64,    65,    66,    67,    68,    71,    72,    73,    75,    76,
      77,    79,    80,    81,    82,    83,    84,    85,    86,    88,
      89,    91,    92,    93,    94,    95,    97,    98,    99,   100,
     103,   105,   108,   124,   125,   126,   127,   132,   142,   144,
     146,   147,   149,   152,   154,   156,   158,   166,   167,   168,
     169,   170,   172,   177,   178,   179,   183,   185,   189,   190,
     191,   192,   193,   194,   196,   197,   198,   202,   203,   206,
     207,   210,   211,   214,   217,   218,   241,   244,   245,   265,
     266,   267,   268,   269,   270,   278,   279,   280,   281,   284,
     285,   286,   287,   288,   289,   290,   292,   293,   294,   295,
     296,   149,   168,   281,   104,   271,   272,     3,   180,    12,
      37,    38,    40,    86,   154,   158,   214,   265,   269,   279,
     280,   281,   284,   286,   287,   271,   281,    94,   244,    75,
     180,   168,   259,   281,   168,   156,   281,   282,     7,     9,
      94,   187,   282,    64,   106,   195,   281,   281,    23,    35,
     245,   281,   281,   156,     3,    15,    16,    22,    30,    36,
      41,    43,    46,    52,    63,    72,    81,    82,    84,    86,
      88,    91,    95,    98,   182,   184,   215,   156,   182,   246,
     247,    29,   172,   188,   281,   281,   281,   281,   281,   281,
       7,   156,    35,   191,   191,   148,   237,   258,   281,    92,
      93,   149,   281,   283,   281,   188,   281,   281,   281,    85,
     156,   168,   281,   281,   172,   179,   281,   284,   172,   179,
     281,   182,   242,   281,   258,   281,   281,   281,   281,   281,
     281,   281,   281,     1,   155,   166,   173,   258,    96,   133,
     237,   260,   261,   283,   258,   281,   291,    66,   168,    70,
     171,   182,   182,    47,    69,   232,   246,   149,   139,    10,
      17,    28,    74,    78,   108,   124,   126,   127,   128,   130,
     131,   132,   134,   135,   136,   137,   138,   140,   141,   142,
     143,   145,   146,   147,   150,   151,   152,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   153,   224,   156,   158,    74,    78,   156,   168,
     149,   281,   281,   281,   258,   154,   172,    42,   271,   242,
     246,   149,   129,   149,   103,   183,   237,   262,   263,   264,
     283,   242,   207,   244,   182,   156,   188,    29,    42,   188,
      29,    42,   104,   188,   274,   282,    33,    59,   149,    87,
     172,   224,   262,   168,   156,   226,    96,   156,   182,   248,
     249,     1,   128,   253,    42,   129,   168,   188,   188,   262,
     182,   129,   149,   281,   281,   149,   154,   188,   156,   262,
     149,   199,   149,   199,   149,   109,   243,   149,   149,    33,
      59,   188,   149,   155,   155,   166,   129,   155,   281,   129,
     157,   129,   157,    42,   129,   159,   274,   107,   129,   159,
       7,    54,   128,   208,   154,   219,   149,   281,   281,   281,
     281,   195,   281,   195,   281,   281,   281,   281,   281,    16,
      98,   281,   281,    30,    52,    94,   183,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   283,   283,   283,   283,   283,   262,   262,
     195,   281,   195,   281,    24,    42,    75,   100,   273,   276,
     277,   281,   296,    29,    42,    29,    42,    87,    42,   159,
     195,   281,   188,   149,   149,   281,   281,   109,   157,   129,
     149,   182,   246,    42,   258,    40,   281,   195,   281,    40,
     281,   195,   281,   156,   188,   152,   174,   176,   281,   174,
     175,   168,   281,    31,   281,   157,    24,    42,    45,    61,
      64,    75,    94,   167,   227,   228,   229,   230,   216,   249,
     129,   157,    16,    30,    43,    63,    81,    82,    86,    98,
     158,   186,   254,   266,   109,   250,   281,   247,   157,   237,
     281,     1,   204,   262,   157,    18,   200,   254,   266,   129,
     176,   175,   155,   157,   157,   260,   157,   260,   195,   281,
     159,   168,   281,   159,   281,   159,   281,   182,   258,   154,
       1,   182,   212,   213,    24,    64,    75,    94,   221,   231,
     149,   149,   149,   149,   149,   157,   159,    42,    75,   129,
     157,   265,    74,    74,    40,   195,   281,    40,   195,   281,
     195,   281,   271,   271,   149,   237,   283,   264,   208,   149,
     281,   157,   281,    29,   188,    29,   188,   281,    29,   188,
     274,    29,   188,   274,   275,   276,   129,   149,    11,   149,
      31,    31,   168,    87,   172,    42,    75,   229,   129,   157,
     156,   182,    24,    64,    75,    94,   233,   157,   249,   253,
       1,   258,    57,   283,   155,    60,   101,   155,   205,   157,
     156,   172,   182,   201,   242,   149,   149,   159,   274,   159,
     274,   168,   107,   154,     1,   209,   155,   109,   129,   155,
      75,   220,     1,     3,    10,    15,    17,    22,    36,    41,
      46,    52,    72,    84,    88,    91,    95,   124,   125,   126,
     127,   130,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   147,   150,   151,   152,   153,   156,
     181,   184,   222,   223,   224,   265,   276,   253,   265,   265,
     281,    29,    29,   281,    29,    29,   159,   159,   188,   188,
     154,   188,    87,    40,   281,    40,   281,    87,    40,   281,
     188,    40,   281,   188,   129,   157,   281,   281,   168,   281,
     168,    31,   182,   228,   249,   128,   257,    75,   253,   250,
     159,    42,   159,    29,   168,   258,   201,   128,   172,    40,
     168,   281,   159,    40,   168,   281,   159,   281,     1,   155,
     173,   155,    35,    68,    71,   155,   167,   190,   281,   213,
     233,   156,   225,   182,   281,   131,   225,   225,   250,    87,
      40,    40,    87,    40,    40,    40,    40,     1,   209,   281,
     281,   281,   281,   281,   281,   276,    11,    31,   168,   257,
     157,   158,   186,   237,   256,   266,   133,   239,   250,   102,
     240,   281,   254,   266,   168,   188,   157,   281,   281,   168,
     281,   168,   155,   155,   190,     1,   128,   252,   227,   157,
       3,    88,   223,   224,   281,   281,   281,   281,   281,   281,
     155,   155,    87,    87,    87,    87,   281,   168,   250,   257,
     159,   258,   237,   238,   281,   281,   172,   189,   236,   159,
     172,    87,    87,   158,   186,   251,   266,    90,   234,   157,
     225,   225,    87,    87,    87,    87,    87,    87,   281,   281,
     281,   281,   239,   250,   237,   255,   256,   266,    42,   159,
     266,   281,   281,     1,   159,   258,   240,   281,   255,   256,
     159,   251,   266,   159,   149,   235,   236,   159,   251,   266,
     255
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   163,   164,   165,   165,   166,   166,   167,   167,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   169,
     169,   169,   170,   170,   170,   171,   171,   172,   172,   172,
     173,   173,   174,   174,   174,   174,   175,   175,   176,   176,
     177,   177,   177,   178,   179,   179,   179,   179,   179,   180,
     180,   181,   181,   181,   181,   181,   182,   182,   182,   182,
     182,   183,   183,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   186,   186,   186,
     186,   186,   186,   186,   186,   187,   187,   188,   188,   189,
     189,   190,   190,   190,   190,   190,   190,   190,   190,   191,
     191,   192,   192,   192,   192,   193,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   195,   196,
     196,   196,   196,   196,   196,   196,   196,   197,   198,   198,
     198,   198,   198,   198,   199,   199,   200,   200,   200,   201,
     201,   202,   203,   203,   204,   204,   205,   205,   205,   206,
     206,   206,   206,   207,   207,   207,   208,   208,   209,   209,
     209,   210,   210,   211,   212,   212,   212,   213,   213,   215,
     216,   214,   217,   217,   217,   217,   217,   219,   220,   218,
     221,   221,   221,   221,   221,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   225,   225,   226,   227,   227,
     227,   228,   228,   228,   228,   228,   229,   229,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   231,   231,   231,
     231,   231,   231,   232,   232,   233,   233,   233,   233,   233,
     233,   234,   234,   235,   235,   236,   236,   237,   237,   238,
     238,   239,   239,   240,   240,   241,   241,   241,   242,   242,
     243,   243,   243,   244,   244,   244,   244,   244,   245,   245,
     245,   246,   246,   247,   247,   248,   248,   248,   249,   249,
     249,   250,   250,   250,   251,   251,   251,   251,   251,   251,
     251,   252,   252,   252,   252,   252,   253,   253,   253,   253,
     253,   254,   254,   254,   254,   255,   255,   255,   256,   256,
     256,   256,   256,   257,   257,   257,   257,   257,   258,   258,
     258,   258,   259,   259,   260,   260,   260,   261,   261,   262,
     262,   263,   263,   264,   264,   264,   264,   265,   265,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   268,   269,   270,   270,   270,   270,   270,   270,
     270,   270,   271,   271,   272,   273,   273,   274,   275,   275,
     276,   276,   276,   277,   277,   277,   277,   277,   277,   278,
     278,   279,   280,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   282,
     282,   283,   283,   283,   284,   284,   284,   284,   285,   285,
     286,   286,   286,   287,   287,   287,   287,   288,   288,   288,
     288,   289,   289,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   291,   291,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     293,   293,   293,   293,   293,   293,   294,   294,   294,   294,
     295,   295,   295,   295,   296,   296,   296,   296,   296,   296,
     296
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     2,     2,     3,     2,     6,
       7,     7,     0,     1,     1,     0,     1,     2,     3,     3,
       1,     2,     1,     3,     3,     5,     0,     1,     1,     1,
       3,     5,     5,     3,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     2,
       3,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     3,     5,     5,     2,     2,     5,     3,     6,     6,
       4,     5,     5,     3,     3,     6,     5,     6,     5,     6,
       3,     4,     6,     7,     6,     7,     4,     5,     4,     4,
       3,     6,     5,     6,     5,     8,     7,     2,     3,     3,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     6,
       8,     6,     8,     1,     1,     1,     0,     2,     0,     2,
       3,     5,     5,     1,     1,     2,     3,     1,     3,     0,
       0,     8,     0,     1,     2,     2,     1,     0,     0,    10,
       3,     3,     5,     5,     3,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     3,     0,     1,
       3,     4,     5,     4,     6,     6,     0,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     2,     1,     0,
       1,     2,     2,     0,     2,     3,     4,     4,     2,     4,
       0,     2,     2,     1,     1,     1,     2,     1,     4,     5,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     1,     0,     2,     2,     2,
       1,     4,     4,     6,     3,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     2,     2,     2,     2,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       3,     2,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     3,     3,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 468 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 5476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 474 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 531 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 579 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 583 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5782 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5788 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5794 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5800 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5806 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5812 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5824 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5830 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5836 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5842 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 5848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5860 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5878 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "bool"; }
#line 5884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "int"; }
#line 5890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "uint"; }
#line 5896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "real"; }
#line 5902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "imag"; }
#line 5908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "complex"; }
#line 5914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "string"; }
#line 5920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "sync"; }
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "single"; }
#line 5932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "owned"; }
#line 5938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "shared"; }
#line 5944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "borrowed"; }
#line 5950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "unmanaged"; }
#line 5956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 686 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "domain"; }
#line 5962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "index"; }
#line 5968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "locale"; }
#line 5974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 5980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 693 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 5986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 5992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 5998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_syncvar"; }
#line 6028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_singlevar"; }
#line 6034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_owned"; }
#line 6040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_shared"; }
#line 6046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_borrowed"; }
#line 6052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_unmanaged"; }
#line 6058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_domain"; }
#line 6064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 713 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_index"; }
#line 6070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 718 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 6076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 719 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 723 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 728 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 729 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 735 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 6234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 6240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 777 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 778 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 786 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 792 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 798 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 804 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 811 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 820 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 824 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 825 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 826 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 827 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 828 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 830 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 832 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 834 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 839 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 843 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 844 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 846 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 877 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 878 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 894 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 904 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 914 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 924 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 960 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 969 "chapel.ypp" /* yacc.c:1661  */
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
#line 6640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 992 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1019 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1020 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1035 "chapel.ypp" /* yacc.c:1661  */
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
      if ((yyvsp[-1].pexpr))
        (yyvsp[-5].pfnsymbol)->where = new BlockStmt((yyvsp[-1].pexpr));
      (yyvsp[-5].pfnsymbol)->insertAtTail((yyvsp[0].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[-5].pfnsymbol)));
    }
#line 6736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1055 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1068 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1074 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6793 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1104 "chapel.ypp" /* yacc.c:1661  */
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

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), (yylsp[-9]).comment);
      context->latestComment = NULL;
    }
#line 6841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1130 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1134 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1139 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1143 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1148 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 6883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1155 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1159 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 7003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 7009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 7015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 7039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 7045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 7057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1190 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 7069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 7075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 7081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 7087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1197 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7129 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1208 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1212 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1213 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1219 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1231 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1232 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 7201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1238 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 7213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1239 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 7219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1240 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1241 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 7243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1244 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1248 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7261 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1250 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 7267 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1251 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7273 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1252 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1253 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1257 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1258 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1263 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 7321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 7327 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 7333 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1271 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 7339 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 7345 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 7351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1281 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7357 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1286 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7363 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7369 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1292 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1297 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7381 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1298 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1302 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1303 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7399 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7411 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7417 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1330 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1346 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1348 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1350 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1354 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1355 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1363 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_CONFIG);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pflagset)->insert(FLAG_EXTERN);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset), (yyvsp[-3].pexpr));
      context->latestComment = NULL;
    }
#line 7520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), context->latestComment, (yyvsp[-2].pflagset));
      context->latestComment = NULL;
    }
#line 7529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1384 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1392 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1399 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1403 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1418 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1419 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1420 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1428 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1434 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1440 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1444 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1454 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1493 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1533 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1540 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1541 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1542 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1569 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8074 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1647 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1666 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1672 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 8161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1715 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1719 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1720 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1728 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1729 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1734 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1738 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1742 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1759 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1785 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1787 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1789 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1793 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1795 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1797 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1799 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1803 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1807 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1809 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1831 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1832 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1833 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1837 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1838 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1839 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1840 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1871 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1875 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1883 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1892 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1898 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1899 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1900 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1901 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1902 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1903 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1904 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1905 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1906 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1907 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1908 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1909 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1913 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1914 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1916 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 544:
#line 1917 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 545:
#line 1918 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 546:
#line 1922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 547:
#line 1923 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 548:
#line 1924 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 549:
#line 1925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 550:
#line 1929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 551:
#line 1930 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 552:
#line 1931 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 553:
#line 1932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 554:
#line 1937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 555:
#line 1938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 556:
#line 1939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 557:
#line 1940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 558:
#line 1941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 559:
#line 1942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 560:
#line 1943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8781 "bison-chapel.cpp" /* yacc.c:1661  */
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

  yyerror_range[1] = yylsp[1-yylen];
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
