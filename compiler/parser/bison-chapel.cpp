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
    Flag                      flag;
    ProcIter                  procIter;
    FlagSet*                  flagSet;
    ShadowVarSymbol*          pShadowVar;
    ShadowVarPrefix           pShadowVarPref;
    std::vector<OnlyRename*>* ponlylist;
    std::set<Flag>*           pflagset;
  };

  #endif
#line 132 "chapel.ypp" /* yacc.c:355  */

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
#line 154 "chapel.ypp" /* yacc.c:355  */

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

#line 224 "bison-chapel.cpp" /* yacc.c:355  */

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
#line 185 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 432 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 438 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
#line 193 "chapel.ypp" /* yacc.c:359  */

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

#line 512 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   15828

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  135
/* YYNRULES -- Number of rules.  */
#define YYNRULES  561
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  995

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
       0,   470,   470,   475,   476,   482,   483,   488,   489,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   527,
     530,   532,   538,   539,   540,   544,   545,   558,   559,   560,
     565,   566,   571,   576,   581,   585,   592,   597,   601,   606,
     610,   611,   612,   616,   620,   622,   624,   626,   628,   635,
     636,   640,   641,   642,   643,   644,   647,   648,   649,   650,
     651,   663,   664,   675,   676,   677,   678,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   688,   689,   690,   694,
     695,   696,   697,   698,   699,   700,   701,   708,   709,   710,
     711,   712,   713,   714,   715,   720,   721,   725,   726,   730,
     731,   735,   736,   737,   738,   739,   740,   741,   742,   746,
     747,   751,   752,   753,   754,   758,   765,   766,   767,   768,
     769,   770,   771,   772,   773,   774,   775,   776,   777,   778,
     779,   780,   781,   787,   793,   799,   805,   812,   822,   826,
     827,   828,   829,   830,   832,   834,   836,   841,   844,   845,
     846,   847,   848,   849,   853,   854,   858,   859,   860,   864,
     865,   869,   872,   874,   879,   880,   884,   886,   888,   895,
     905,   915,   925,   938,   943,   948,   956,   957,   962,   963,
     965,   970,   986,   993,  1002,  1010,  1014,  1021,  1022,  1027,
    1032,  1026,  1057,  1063,  1070,  1076,  1082,  1093,  1099,  1092,
    1131,  1135,  1140,  1144,  1149,  1156,  1157,  1161,  1162,  1163,
    1164,  1165,  1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,
    1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,  1183,
    1184,  1185,  1186,  1190,  1191,  1192,  1193,  1194,  1195,  1196,
    1197,  1198,  1199,  1200,  1201,  1205,  1206,  1210,  1214,  1215,
    1216,  1220,  1222,  1224,  1226,  1228,  1233,  1234,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1250,  1251,  1252,
    1253,  1254,  1255,  1259,  1260,  1264,  1265,  1266,  1267,  1268,
    1269,  1273,  1274,  1277,  1278,  1282,  1283,  1287,  1289,  1294,
    1295,  1299,  1300,  1304,  1305,  1309,  1311,  1313,  1318,  1331,
    1348,  1349,  1351,  1356,  1357,  1358,  1359,  1360,  1364,  1373,
    1374,  1375,  1379,  1380,  1388,  1390,  1395,  1397,  1399,  1404,
    1406,  1408,  1415,  1416,  1417,  1422,  1424,  1426,  1430,  1434,
    1436,  1440,  1448,  1449,  1450,  1451,  1452,  1457,  1458,  1459,
    1460,  1461,  1481,  1485,  1489,  1497,  1504,  1505,  1506,  1510,
    1512,  1518,  1520,  1522,  1527,  1528,  1529,  1530,  1531,  1537,
    1538,  1539,  1540,  1544,  1545,  1549,  1550,  1551,  1555,  1556,
    1560,  1561,  1565,  1566,  1570,  1571,  1572,  1573,  1577,  1578,
    1589,  1590,  1591,  1592,  1593,  1594,  1596,  1598,  1600,  1602,
    1604,  1606,  1608,  1610,  1612,  1614,  1619,  1621,  1623,  1625,
    1627,  1629,  1631,  1633,  1635,  1637,  1639,  1641,  1643,  1650,
    1656,  1662,  1668,  1677,  1687,  1695,  1696,  1697,  1698,  1699,
    1700,  1701,  1702,  1707,  1708,  1712,  1716,  1717,  1721,  1725,
    1726,  1730,  1734,  1738,  1745,  1746,  1747,  1748,  1749,  1750,
    1754,  1755,  1760,  1765,  1773,  1774,  1775,  1776,  1777,  1778,
    1779,  1780,  1781,  1783,  1785,  1787,  1789,  1791,  1793,  1795,
    1800,  1801,  1804,  1805,  1806,  1809,  1810,  1811,  1812,  1823,
    1824,  1828,  1829,  1830,  1834,  1835,  1836,  1837,  1845,  1846,
    1847,  1848,  1852,  1853,  1857,  1858,  1859,  1860,  1861,  1862,
    1863,  1864,  1865,  1866,  1867,  1871,  1879,  1880,  1884,  1885,
    1886,  1887,  1888,  1889,  1890,  1891,  1892,  1893,  1894,  1895,
    1896,  1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,  1905,
    1906,  1910,  1911,  1912,  1913,  1914,  1915,  1919,  1920,  1921,
    1922,  1926,  1927,  1928,  1929,  1934,  1935,  1936,  1937,  1938,
    1939,  1940
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
  "var_decl_type", "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
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

#define YYTABLE_NINF -513

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -880,   179,  2488,  -880,   -87,  -880,  -880,  -880,  -880,  -880,
    -880,  3736,    14,   202,  -880,  8976,  -880,    14,  8976,  -880,
     158,   202,  3736,  8976,  3736,   103,  -880,  8976,  5923,  -880,
    7375,  8976,  7531,  8976,  -880,   113,  -880,  -880, 15680,  -880,
    2214,  8086,  -880,  8976,  8976,  -880,  8976,  -880,  8976,  8976,
     321,   182,   826,  1162,  -880,  -880,  8219,  6930,  8976,  8086,
    8976,  8976,   277,  -880,   212,  3736,  -880,  8976,  -880,  9109,
    9109, 15680,  -880,  -880,  8976,  8219,  8976,  8976,  -880,  -880,
    8976,  -880,  -880, 11104,  8976,  8976,  -880,  8976,  -880,  -880,
    2800,  6329,  8219,  -880,  3580,  -880,  -880,   285,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880, 15680,  -880, 15680,   261,
     171,  -880,  -880,  -880,   350,   243,  -880,  -880,  -880,   244,
     254,   268,   264,   267, 15392, 11362,   253,   271,   300,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,   279,  -880,  -880, 15392,
     278,  3736,  -880,  -880,   305,  8976,  8976,  8976,  8976,  8976,
    8219,  8219,   270,  -880,  -880,  -880,  -880,   269,   273,  -880,
    -880,   282, 13647, 15680,   311,  -880,   109, 15392,   359,  7086,
   15392,  -880,   100,  -880, 15680,   139,  -880, 15680,   310,    12,
   13184, 13133,  -880,  -880,  -880, 13568, 12446,  7086,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  3736,  -880,
     312,  1549,   128,    33,  -880,  3736,  -880,  -880, 13322,   475,
     269, 13322,   269,   475,  -880,  7086,  7664,  -880,  -880, 15680,
    -880,   115, 15392,  8976,  8976,  -880, 15392,   320, 13498,  -880,
   13322,   269, 15392,   314,  7086,  -880, 15392, 13893,  -880,  -880,
   13944, 15670,  -880,  -880, 14031,   358,   323,   269,   132, 13717,
   13322, 14082,   251,  5282,   475,   251,   475,   -23,  -880,  -880,
    2956,   -35,  -880,  8976,  -880,    46,    91,  -880,     1, 14169,
     -17,   466,  -880,  -880,   420,   353,   329,  -880,  -880,  -880,
     409,  -880,  -880,  -880,  2214,  -880,  8976,  8976,  8976,  8976,
    8375,  8375,  8976,  8976,  8976,  8976,  9242,  8976,   199, 11104,
    8976,  8976,  8976,  8976,  8976,  8976,  8976,  8976,  8976,  8976,
    8976,  8976,  8976,  8976,  8976,  -880,  -880,  -880,  -880,  -880,
    -880,  7797,  7797,  -880,  -880,  -880,  -880,  7797,  -880,  -880,
    7797,  7797,  7086,  7086,  8375,  8375,  6797,  -880,  -880, 13792,
   13862, 14118,    45,  3112,  -880,  8375,    12,   339,  -880,  8976,
    -880,  8976,   380,  -880,   334,   363,  -880,  -880,   345, 15680,
     453,  8219,  -880,  3892,  8375,  -880,  4048,  8375,   340,  -880,
      12,  9375,  8976,  -880,  3736,   469,  8976,   349,  -880,   559,
    -880,  -880,  1549,  -880,   368,   360,  -880,  9508,   406,  8976,
    2214,  -880,  -880,  -880,   362,  -880,  8219,  -880, 15392, 15392,
    -880,    35,  -880,  7086,   372,  -880,   498,  -880,   498,  -880,
    9641,   391,  -880,  -880,  9375,  8976,  -880,  -880,  -880,  -880,
    -880,  7930,  -880, 12301,  6485,  -880,  6641,  -880,  8375,  5452,
    2644,   366,  8976,  5766,  -880,  -880, 15680,  8219,   376, 15636,
      29,  -880,   134, 14256,  1764,  1764,   259,  -880,   259,  -880,
     259, 12051,   465,  1599,   328, 11491, 11491,   269,   251,  -880,
    -880,  -880,  -880,  5282, 13392,   259,   978,   978,  1764,   978,
     978,   897,   251, 13392, 15493,   897,   475,   475,   251,   385,
     392,   393,   395,   399,   383,   396,  -880,   259,  -880,   259,
      28,  -880,  -880,  -880,    98,  -880,  1138, 15462,   287,  9774,
    8375,  9907,  8375,  8976,  8375, 11683,    14, 14307,  -880,  -880,
   15392, 14338,  7086,  -880,  7086,  -880,   353,  8976,   116,  8976,
   15392,    22, 13353,  8976, 15392,     2, 13271,  6797,  -880,   401,
     422,   403, 14408,   422,   408,   527, 14483,  3736, 13423,  -880,
      49,  -880,  -880,  -880,  -880,  -880,  -880,   676,   118,  -880,
   11868,  -880,   324,   404,  1549,   128,  8976,   103,   113,  8976,
    8976,  8976,  8976,  8976,  5296,  -880,  -880,   445,  7242,  -880,
   15392,  -880,  -880,  -880, 15392,   407,   -33,   410,  -880, 11815,
    -880,  -880,   197, 15680,   417,   427,  -880,  -880,  -880,  -880,
    -880,  -880,   -19, 12011,  -880,  -880, 15392,  3736, 15392,  -880,
   14553,   428,   455,  2010,   431,   478,    30,  -880,   513,  -880,
    -880,  -880,  -880,  6042,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  6797,  -880,    25,  8375,  8375,  8976,
     561, 14628,  8976,   568, 14698,   439, 12081,    12,    12,  -880,
    -880,  -880,  -880,   457, 13322,  -880, 12521,  4204,  -880,  4360,
    -880, 12596,  4516,  -880,    12,  4672,  -880,    12,   121,  -880,
    8976,  -880,  8976,  -880,  3736,  8976,  -880,  3736,   576,  -880,
    -880, 15680,  1017,  -880,  1549,   481,   537,  -880,  -880,  -880,
      39,  -880,  -880,   406,   460,    55,  -880,  -880,  -880,  4828,
    8219,  -880,  -880,  -880, 15680,  -880,   486,   282,  -880,  -880,
    -880,  4984,   462,  5140,   467,  -880,  8976,  3268,   473,  1325,
    -880,  8976, 15680,  -880,  -880,   324,   468,   499,  -880,   500,
    -880,   501,  -880,   504,   509,   511,   512,   514,   515,  -880,
     517,  -880, 15680,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  8976,  -880,  -880,   518,   468,   468,  -880,
    -880,   406,   125,   133, 14729, 10040, 10173, 14867, 10306, 10439,
   10572, 10705,  -880,  -880, 11753,  -880,  3736,  8976, 15392,  8976,
   15392,  3736,  8976, 15392,  -880,  8976, 15392,  -880,  6797,  -880,
   14780, 15392,  -880, 15392,   598,  3736,   481,  -880,   487,  8508,
      62,  -880,   548,  -880,  -880,  8375, 11587,  3736,  -880,    26,
     494,  8976,  -880,  8976,  -880, 15392,  3736,  8976,  -880, 15392,
    3736, 15392,   -18,  -880,  3424,  -880,  6196,   737,   737,  -880,
     670,  -880, 15392,  -880,    23,   559,  -880,  -880, 12371, 11931,
    -880,  -880,  -880,  8976,  8976,  8976,  8976,  8976,  8976,  8976,
    8976,   505,  2024, 14483, 12671, 12746, 14483, 12821, 12896,  -880,
    8976,  3736,  -880,   406,   481,  5609,  -880,  -880,  -880,   238,
    8219,  -880,  -880,  8976,    15, 12156,  -880,   850,   359,  -880,
     282, 15392, 12971,  -880, 13046,  -880,  -880,  -880,  -880,  -880,
   10838,   564,   206,  -880,  -880,  -880,   468,   468, 14942, 15017,
   15092, 15167, 15242, 15317,  -880,  -880,  3736,  3736,  3736,  3736,
   15392,  -880,  -880,    62,  8664,    60,  -880,  -880, 15392, 15392,
    -880,  -880,  -880, 10705,  -880,  3736,  3736,  2319,  -880,  -880,
     111,  -880,   548,  -880,  -880,  -880,  8976,  8976,  8976,  8976,
    8976,  8976, 14483, 14483, 14483, 14483,  -880,  -880,  -880,  -880,
    -880,   318,  8375, 11237,   889, 14483, 14483,   496, 10971,    69,
     -16, 12226,  -880,  -880,  -880,  -880,   140, 11368,  -880,  -880,
    -880,  8820,  -880,   247,  -880
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    81,   505,   506,   507,   508,
     509,     0,   443,    69,    99,     0,   193,   443,     0,   104,
     330,    69,     0,     0,     0,     0,   203,   480,   331,   502,
       0,     0,   329,     0,   103,     0,   213,   100,     0,   209,
       0,     0,   106,     0,     0,   434,     0,   216,     0,     0,
       0,     0,   329,   329,   102,   194,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,    82,     0,   503,     0,
       0,     0,   101,   195,     0,     0,     0,     0,   557,   559,
       0,   560,   561,   479,     0,     0,   558,   555,   121,   556,
       0,     0,     0,     4,     0,     5,     9,    45,    10,    11,
      12,    14,   398,   399,    22,    13,   122,   128,    15,    17,
      16,    19,    20,    21,    18,   127,     0,   125,     0,   469,
       0,   129,   126,   130,     0,   485,   465,   466,   402,   400,
       0,     0,   470,   471,     0,   401,     0,   486,   487,   488,
     504,   464,   404,   403,   467,   468,     0,    38,    24,   410,
       0,     0,   444,    70,     0,     0,     0,     0,     0,     0,
       0,     0,   469,   485,   400,   470,   471,   412,   401,   486,
     487,     0,   443,     0,     0,   167,     0,   383,     0,   390,
     481,   214,   508,   135,     0,     0,   215,     0,     0,     0,
       0,     0,   330,   331,   134,     0,     0,   390,    76,    83,
      94,    88,    96,    78,    87,    97,    84,    98,    92,    86,
      93,    91,    89,    90,    77,    79,    85,    95,     0,    80,
       0,     0,     0,     0,   332,     0,   118,    32,     0,   543,
     462,     0,   413,   544,     7,   390,   331,   124,   123,   308,
     380,     0,   379,     0,     0,   119,   484,     0,     0,    35,
       0,   415,   405,     0,   390,    36,   411,     0,   174,   170,
       0,   401,   174,   171,     0,   320,     0,   414,     0,   379,
       0,     0,   546,   478,   542,   545,   541,     0,    47,    50,
       0,     0,   385,     0,   387,     0,     0,   386,     0,   379,
       0,     0,     6,    46,     0,   196,     0,   294,   293,   217,
     324,   323,   325,   327,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   477,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   253,   260,   261,   262,   257,
     259,     0,     0,   255,   258,   256,   254,     0,   264,   263,
       0,     0,   390,   390,     0,     0,     0,    25,    26,     0,
       0,     0,     0,     0,    27,     0,     0,     0,    28,     0,
      29,     0,   398,   396,     0,   391,   392,   397,     0,     0,
       0,     0,   144,     0,     0,   143,     0,     0,     0,   150,
       0,     0,    56,   131,     0,   160,     0,     0,    30,   276,
     210,   336,     0,   337,   339,     0,   361,     0,   342,     0,
       0,   117,    31,    33,     0,   307,     0,    63,   482,   483,
     120,     0,    34,   390,     0,   181,   172,   168,   173,   169,
       0,   318,   315,    60,     0,    56,   137,    37,    49,    48,
      51,     0,   510,     0,     0,   498,     0,   500,     0,     0,
       0,     0,     0,     0,   514,     8,     0,     0,     0,     0,
     287,   326,     0,   442,   538,   537,   540,   548,   547,   552,
     551,   534,   531,   532,   533,   474,   475,   473,   521,   496,
     497,   495,   494,   476,   525,   536,   530,   528,   539,   529,
     527,   519,   524,   526,   535,   518,   522,   523,   520,     0,
       0,     0,     0,     0,     0,     0,   550,   549,   554,   553,
     454,   455,   457,   459,     0,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   512,   443,   443,   140,   316,
     384,     0,     0,   407,     0,   317,   196,     0,     0,     0,
     418,     0,     0,     0,   424,     0,     0,     0,   151,   556,
      59,     0,    52,    57,     0,   159,     0,     0,     0,   406,
     281,   278,   279,   280,   284,   285,   286,   276,     0,   269,
       0,   277,   295,     0,   340,     0,   111,   113,   114,   109,
     110,   108,   107,   112,     0,   360,   359,   465,     0,   334,
     463,   333,   493,   382,   381,     0,     0,     0,   408,     0,
     175,   322,   465,     0,     0,     0,   511,   472,   499,   388,
     501,   389,     0,     0,   513,   156,   428,     0,   516,   515,
       0,     0,   197,     0,     0,   207,     0,   204,   291,   288,
     289,   292,   218,     0,   328,    67,    68,    66,    65,    64,
     491,   492,   456,   458,     0,   445,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
     394,   395,   393,     0,     0,   158,     0,     0,   142,     0,
     141,     0,     0,   148,     0,     0,   146,     0,     0,   449,
       0,   132,     0,   133,     0,     0,   162,     0,   164,   282,
     283,     0,   276,   267,     0,   374,   296,   299,   298,   300,
       0,   338,   341,   342,     0,     0,   343,   344,   183,     0,
       0,   182,   185,   409,     0,   176,   179,     0,   319,    61,
      62,     0,     0,     0,     0,   157,     0,     0,     0,   329,
     202,     0,   205,   201,   290,   295,   265,    71,   250,    83,
     248,    88,    73,    87,    84,    98,    86,    89,    72,    74,
      85,   229,   232,   230,   231,   242,   233,   246,   238,   236,
     249,   252,   237,   235,   240,   245,   247,   234,   239,   243,
     244,   241,   251,     0,   227,    75,     0,   265,   265,   225,
     447,   342,   485,   485,     0,     0,     0,     0,     0,     0,
       0,     0,   139,   138,     0,   145,     0,     0,   417,     0,
     416,     0,     0,   423,   149,     0,   422,   147,     0,   448,
      54,    53,   161,   433,   163,     0,   374,   270,     0,     0,
     342,   297,   313,   335,   365,     0,   512,     0,   187,     0,
       0,     0,   177,     0,   154,   430,     0,     0,   152,   429,
       0,   517,     0,    39,     0,   191,   331,   329,   329,   189,
     329,   199,   208,   206,     0,   276,   224,   228,     0,     0,
     220,   221,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   329,   421,     0,     0,   427,     0,     0,   450,
       0,     0,   166,   342,   374,     0,   377,   376,   378,   465,
     309,   273,   271,     0,     0,     0,   363,   465,   188,   186,
       0,   180,     0,   155,     0,   153,    41,    40,   200,   356,
       0,   301,     0,   226,    71,    72,   265,   265,     0,     0,
       0,     0,     0,     0,   192,   190,     0,     0,     0,     0,
      55,   165,   272,   342,   366,     0,   310,   312,   311,   314,
     305,   306,   211,     0,   178,     0,     0,     0,   355,   354,
     465,   302,   313,   266,   222,   223,     0,     0,     0,     0,
       0,     0,   420,   419,   426,   425,   275,   274,   368,   369,
     371,   465,     0,   512,   465,   432,   431,     0,   346,     0,
       0,     0,   370,   372,   351,   349,   465,   512,   303,   219,
     304,   366,   350,   465,   373
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -880,  -880,  -880,     0,  -365,  1150,  -880,  -880,  -880,  1582,
     -68,  -314,   227,   229,  -880,  -880,   215,   644,  -880,   791,
    -137,  -620,  -880,  -796,  -880,    18,  -865,  -711,   -47,  -880,
    -880,  -880,   191,  -880,  -880,  -880,   411,  -880,   -46,  -880,
    -880,  -880,  -880,  -880,   492,   136,  -116,  -880,  -880,  -880,
     -53,   733,  -880,  -880,  -880,  -880,  -880,  -880,  -880,  -880,
    -179,  -134,  -694,  -880,  -174,    -9,   117,  -880,  -880,  -880,
     -50,  -880,  -880,  -294,    -1,  -880,  -246,  -262,  -880,  -159,
    -880,  -880,   659,  -880,   390,   286,  -880,  -387,  -692,  -879,
    -880,  -536,  -422,  -641,  -877,  -749,    -4,  -880,  -170,  -880,
    -153,  -880,   163,   367,  -321,  -880,  -880,   875,  -880,    11,
    -880,  -880,  -279,  -880,  -537,  -880,  -880,  1053,  1124,   -11,
     674,   135,   141,  -880,  1286,  1467,  -880,  -880,  -880,  -880,
    -880,  -880,  -880,  -880,  -353
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   279,    94,   555,    96,    97,   294,    98,
     280,   550,   554,   551,    99,   100,   101,   154,   774,   265,
     102,   219,   103,   585,   185,   227,   104,   105,   106,   107,
     108,   109,   655,   110,   111,   112,   426,   600,   717,   113,
     114,   596,   712,   115,   116,   458,   729,   117,   118,   626,
     627,   162,   220,   572,   120,   121,   460,   735,   632,   776,
     777,   396,   856,   400,   568,   569,   570,   571,   633,   299,
     700,   952,   989,   942,   240,   937,   891,   894,   122,   266,
     431,   304,   123,   124,   223,   224,   404,   405,   589,   949,
     911,   408,   586,   969,   888,   820,   281,   176,   285,   286,
     374,   375,   376,   163,   126,   127,   128,   164,   130,   151,
     152,   514,   390,   678,   515,   516,   131,   165,   166,   134,
     186,   377,   168,   136,   169,   170,   139,   140,   141,   290,
     142,   143,   144,   145,   146
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     149,   351,    93,   518,   167,   237,   238,   172,   601,   451,
     679,   823,   177,   775,   367,   573,   180,   180,   851,   190,
     191,   195,   196,   886,   909,   378,   406,   709,   171,   941,
     228,   672,   229,   230,   567,   231,   595,   232,   233,   703,
     406,   225,   372,   448,   397,   242,   246,   248,   250,   251,
     252,   667,   241,   628,   256,   225,   257,   970,   260,   264,
     372,    57,   147,   267,   269,   270,   271,   883,   710,   272,
     642,   268,   273,   274,   275,   409,   276,   249,   553,   242,
     246,   289,   414,   860,   861,   388,   587,   524,   288,   862,
     284,   689,    57,   629,   441,  -184,   983,   825,   372,   985,
    -352,   424,   972,   643,   630,   388,   388,   780,   992,   602,
     781,   524,   453,  -352,   948,   941,  -357,   372,   150,  -116,
     442,   553,   711,   631,   690,  -352,   147,   351,   892,   406,
     449,   147,   438,   988,  -357,   933,  -184,   906,   363,   908,
     721,  -357,   454,   135,   149,   359,   360,   361,   256,   242,
     289,   910,   135,   407,  -357,   416,   363,   362,    16,   732,
     450,   851,   410,   135,   822,   135,   363,   407,   246,   363,
    -357,   588,  -352,  -116,   449,   444,   363,  -352,   373,     3,
     363,   482,  -357,   366,   449,   733,   246,   702,  -353,   449,
    -184,   932,   247,  -357,   518,   890,   373,  -116,   449,   504,
     505,  -353,     5,   445,   525,   153,   135,   382,   385,   389,
     261,   261,    55,  -353,   826,   372,   372,  -345,   297,   973,
     446,   189,   954,   955,   246,   180,   287,   644,   987,   479,
    -345,   135,   418,   419,   373,   135,    73,  -357,   369,   775,
     298,   967,  -345,   246,   416,   416,   412,   692,   447,   413,
     808,   480,   173,   373,  -453,   645,   407,  -357,   370,   179,
    -353,   416,  -452,   410,   417,  -353,   674,   677,   422,   197,
     597,   879,   443,   665,   609,   693,   611,  -357,   809,   309,
     440,   433,  -453,   634,   259,   263,   372,    66,   436,  -345,
    -452,   518,   135,   481,  -345,   463,   464,   465,   466,   468,
     470,   471,   472,   473,   474,   477,   478,   818,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,  -347,   310,  -321,   567,   234,   311,
     246,   246,   982,   722,   724,   692,   246,  -347,   235,   246,
     246,   246,   246,   507,   509,   517,  -321,  -375,   696,  -347,
     994,   373,   373,   354,   527,   293,   309,   355,   530,   135,
     531,   648,   253,   953,   850,   355,   135,  -375,   254,   125,
     242,  -375,   540,   542,   300,   544,   546,   538,   125,   316,
     552,   552,   318,   556,   528,   558,   321,   316,   697,   125,
     318,   125,  -436,  -435,   321,  -375,  -347,   372,   590,   698,
     318,  -347,   310,   305,   896,   594,   311,   306,   548,   352,
    -439,   353,   246,  -440,   301,   593,  -441,  -490,   699,  -490,
    -438,   135,   373,   552,   552,   302,  -490,  -367,  -490,  -489,
     594,  -489,   125,   246,   356,   246,   363,   613,   594,   616,
     593,   618,   620,   284,   718,   284,   242,  -367,   593,  -437,
     303,  -367,   313,   622,   358,   518,   316,   125,   317,   318,
     368,   125,   371,   321,   167,   267,   381,   430,   399,   420,
     423,   328,   432,   455,   456,  -367,   499,   500,   332,   333,
     334,   457,   501,   459,   461,   502,   503,  -358,   529,   532,
     567,   533,   534,   309,   535,   537,   547,   574,   889,   778,
     557,   467,   469,   309,   135,   897,   559,   850,   540,   651,
     544,   654,   556,   656,   616,   588,   599,   575,   125,   592,
     603,   246,  -358,   246,   135,   617,   664,   135,   666,   598,
     623,   660,   671,   373,   635,   135,   517,   657,   658,   310,
     640,   636,   637,   311,   638,   506,   508,  -358,   639,   310,
     -58,   680,   681,   311,  -358,   641,   526,   683,   684,   668,
     670,   701,   708,   673,   676,   167,   719,   713,   232,   251,
     252,   256,   267,   289,  -358,   541,   720,   246,   545,   287,
     705,   287,   727,   560,   416,   125,   730,   731,   734,   950,
     785,   135,   125,   316,  -358,   317,   318,   788,   790,  -358,
     321,   561,  -358,   316,   562,   317,   318,   815,   328,   819,
     321,   794,   821,   971,   831,   332,   333,   334,   328,   824,
     563,   836,   974,   564,   855,    50,   840,   334,   845,   881,
     -81,   -99,  -104,   517,   565,  -103,   468,   507,   784,   612,
    -100,   787,  -106,  -102,   884,  -105,   -82,   125,  -101,   859,
     893,   900,   971,   566,   951,   984,   798,   986,   800,   844,
     924,   803,   605,   604,   806,   174,   993,   661,   830,   810,
     971,   811,   663,   428,   813,   792,   793,   379,   872,   853,
     916,   912,   795,   817,   691,   854,   990,   966,  -268,    16,
     980,   194,   804,    20,   462,   807,   591,   662,   135,   242,
     560,   181,    26,     0,    27,   846,   829,     0,     0,    32,
     835,   650,   839,   653,    36,   841,  -268,  -212,   561,     0,
     852,   562,     0,   707,     0,   917,     0,     0,     0,     0,
     125,     0,    47,     0,     0,   119,   291,   563,   847,  -212,
     564,   848,   291,    55,   119,     0,     0,     0,     0,     0,
     125,   565,     0,   125,     0,   119,     0,   119,   135,     0,
     192,   125,   858,     0,    71,     0,     0,    73,     0,     0,
     566,    27,   236,     0,   798,   800,     0,   803,   806,   835,
     839,    36,     0,     0,  -212,   873,   874,     0,   875,     0,
     876,   877,     0,     0,   878,     0,     0,   517,   119,    47,
     237,   238,     0,     0,     0,     0,  -212,     0,   135,     0,
     135,     0,     0,   135,   895,   616,   135,   125,   887,    88,
     901,     0,   902,   119,     0,   135,   904,   119,   135,   218,
       0,   222,     0,     0,     0,   180,     0,     0,   467,   506,
       0,     0,     0,     0,   440,     0,     0,   899,     0,   192,
     135,     0,   873,   918,   919,   876,   920,   921,   922,   923,
      27,   236,   135,     0,   135,     0,     0,     0,   135,   930,
      36,     0,     0,  -212,   289,     0,     0,   129,     0,   938,
     -44,   935,   939,   646,   119,     0,   129,     0,    47,   936,
       0,     0,  -362,     0,     0,  -212,   -44,   129,     0,   129,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   296,
       0,     0,     0,     0,     0,   962,   963,   964,   965,     0,
       0,     0,     0,     0,   125,   309,     0,  -362,     0,     0,
       0,  -364,   839,   968,   975,   976,   289,   135,     0,     0,
     129,     0,   135,   979,     0,   962,   963,   964,   965,   975,
     976,   119,  -362,     0,     0,     0,   135,     0,   119,  -362,
       0,   981,   616,     0,     0,   129,  -364,     0,   135,   129,
       0,   310,   968,     0,     0,   311,   616,   135,   380,  -362,
     839,   135,     0,     0,   125,   135,     0,     0,     0,     0,
     968,  -364,     0,     0,     0,     0,     0,     0,  -364,  -362,
     779,     0,     0,     0,  -362,     0,   309,  -362,     0,     0,
       0,     0,   403,   119,   782,   783,     0,     0,  -364,     0,
       0,   313,   135,   314,   315,   316,   129,   317,   318,     0,
     415,     0,   321,     0,   125,     0,   125,     0,  -364,   125,
     328,   560,   125,  -364,     0,     0,  -364,   332,   333,   334,
       0,   125,   310,     0,   125,   132,   311,     0,     0,   561,
       0,     0,   562,     0,   132,     0,     0,   135,   135,   135,
     135,     0,     0,     0,     0,   132,   125,   132,   563,     0,
       0,   564,     0,    50,     0,     0,   135,   135,   125,     0,
     125,     0,   565,   129,   125,   222,   119,     0,     0,     0,
     129,     0,   313,     0,   314,   315,   316,     0,   317,   318,
     319,   566,     0,   321,     0,     0,   119,     0,   132,   119,
     327,   328,     0,     0,     0,   331,   133,   119,   332,   333,
     334,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     5,     0,   132,     0,     0,   133,   132,   133,     0,
       0,     0,    95,    14,     0,   129,     0,     0,     0,     0,
      19,   148,     0,   125,     0,     0,     0,     0,   125,     0,
     536,     0,   175,     0,   178,     0,     0,     0,     0,    34,
       0,     0,   125,   119,    37,   192,     0,     0,     0,   133,
      42,     0,     0,   403,   125,     0,    27,   236,     0,     0,
       0,   222,     0,   125,   132,     0,    36,   125,     0,  -212,
      54,   125,     0,     0,   133,   255,   -43,     0,   133,     0,
       0,     0,    63,     0,    47,     0,    66,     0,     0,     0,
       0,  -212,   -43,    72,     0,     0,     0,     0,   129,     0,
      95,     0,     0,     0,   292,     0,     0,   621,   125,     0,
     625,     0,     0,     0,     0,     0,     0,     0,   129,     0,
       0,   129,     0,     0,     0,     0,     0,     0,     0,   129,
       0,   132,     0,     0,     0,   133,     0,     0,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   137,     0,
     119,     0,     0,   125,   125,   125,   125,   137,     0,     0,
       0,   357,     0,     0,     0,     0,     0,     0,   137,     0,
     137,     0,   125,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,   133,     0,    16,     0,     0,     0,    20,   133,
     119,   137,     0,     0,     0,     0,     0,    26,     0,    27,
     846,   695,     0,     0,    32,   403,     0,     0,   398,    36,
       0,     0,  -212,     0,     0,   411,   137,     0,     0,     0,
     137,     0,     0,     0,     0,     0,     0,    47,     0,     0,
     716,    50,     0,   847,  -212,     0,   848,     0,    55,     0,
     119,     0,   119,     0,   133,   119,     0,     0,   119,     0,
       0,     0,     0,     0,     0,     0,   132,   119,     0,    71,
     119,     0,    73,     0,     0,     0,     0,     0,     0,     0,
      95,     0,   129,     0,     0,     0,   132,   137,     0,   132,
       0,     0,   119,     0,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,   119,     0,   119,     0,     0,     0,
     119,     0,     0,     0,     0,     0,     0,     0,     0,   138,
       0,     0,     0,     0,    88,     0,     0,     0,   138,     0,
     849,     0,   816,     0,     0,   403,     0,   133,     0,   138,
       0,   138,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,   137,   716,     0,   133,     0,     0,
     133,   137,     0,    95,     0,     0,     0,     0,   133,     0,
       0,     0,     0,   625,     0,     0,     0,     0,     0,   119,
       0,     0,   138,   411,   119,     0,   411,     0,     0,     0,
       0,     0,   129,   857,   129,     0,     0,   129,   119,     0,
     129,     0,   198,     0,     0,     0,     0,   138,     0,   129,
     119,   138,   129,     0,   199,   200,   137,     0,     0,   119,
       0,   201,     0,   119,   133,     0,     0,   119,     0,   202,
       0,     0,     0,     0,   129,   203,     0,     0,     0,     0,
     204,     0,   205,     0,     0,   206,   129,     0,   129,     0,
     615,   207,   129,     0,     0,     0,     0,     0,     0,     0,
     132,     0,   208,     0,   119,     0,     0,     0,   138,     0,
       0,   209,     0,   226,     0,     0,     0,   309,     0,     0,
     210,   211,     0,   212,     0,   213,     0,   214,     0,     0,
     215,   226,     0,     0,   216,   401,     0,   217,     0,   137,
       0,   258,   262,     0,     0,     0,     0,     0,     0,   119,
     119,   119,   119,     0,     0,     0,     0,     0,     0,   137,
     132,   129,   137,   310,     0,     0,   129,   311,   119,   119,
     137,   133,     0,     0,     0,   138,     0,     0,     0,     0,
     129,     0,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   129,     0,     0,   402,     0,   686,     0,     0,
       0,   129,     0,     0,     0,   129,     0,     0,     0,   129,
     132,     0,   132,   313,     0,   132,   315,   316,   132,   317,
     318,     0,     0,     0,   321,     0,   137,   132,     0,     0,
     132,   133,   328,     0,     0,     0,     0,   138,     0,   332,
     333,   334,     0,   364,     0,     0,   129,     0,     0,     0,
       0,     0,   132,     0,     0,     0,     0,   725,     0,     0,
       0,   226,   226,   226,   132,     0,   132,     0,   395,     0,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   133,   309,   133,     0,     0,   133,     0,     0,   133,
       0,   129,   129,   129,   129,     0,     0,     0,   133,     0,
     226,   133,     0,   226,     0,     0,     0,   411,     0,   411,
     129,   129,   411,     0,     0,   411,     0,     0,     0,     0,
     138,     0,   226,   133,   812,     0,     0,   814,   310,     0,
       0,     0,   311,   137,     0,   133,     0,   133,     0,   132,
     138,   133,   226,   138,   132,     0,     0,     0,     0,   828,
       0,   138,     0,     0,     0,     0,     0,     0,   132,     0,
       0,   834,   312,   838,     0,     0,     0,    95,     0,     0,
     132,     0,     0,     0,     0,     0,     0,     0,   313,   132,
     314,   315,   316,   132,   317,   318,   319,   132,   320,   321,
     322,   323,     0,   137,   325,   326,   327,   328,     0,   329,
     330,   331,     0,     0,   332,   333,   334,   138,     0,     0,
     133,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,   132,     0,     0,     0,     0,   133,
       0,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,   133,     0,   137,     0,   137,     0,     0,   137,     0,
     133,   137,     0,     0,   133,   882,     0,     0,   133,     0,
     137,     0,   226,   137,     0,     0,     0,   898,     0,   132,
     132,   132,   132,     0,     0,     0,   903,     0,     0,     0,
     905,     0,     0,     0,    95,   137,     0,     0,   132,   132,
       0,     0,     0,     0,     0,   133,     0,   137,     0,   137,
       0,   728,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   138,     0,     0,     0,     0,  -198,
       0,   931,     0,  -198,  -198,     0,     0,     0,     0,     0,
       0,     0,  -198,    16,  -198,  -198,     0,    20,     0,  -198,
     133,   133,   133,   133,  -198,     0,    26,  -198,    27,   846,
       0,     0,     0,    32,     0,     0,     0,     0,    36,   133,
     133,  -212,  -198,     0,  -198,     0,  -198,     0,  -198,  -198,
       0,  -198,   137,  -198,   138,  -198,    47,   137,     0,     0,
      50,     0,   847,  -212,     0,   848,     0,    55,     0,     0,
       0,   137,     0,     0,  -198,     0,     0,  -198,     0,     0,
    -198,     0,     0,   137,     0,     0,     0,     0,    71,     0,
       0,    73,   137,   226,   226,     0,   137,   226,   226,     0,
     137,     0,     0,     0,   138,     0,   138,     0,     0,   138,
     688,     0,   138,     0,     0,     0,     0,     0,     0,     0,
       0,   138,     0,     0,   138,     0,     0,     0,     0,  -198,
       0,     0,     0,     0,     0,  -198,     0,   137,     0,     0,
       0,     0,     0,    88,     0,     0,   138,     0,     0,   925,
       0,   715,     0,     0,     0,     0,     0,     0,   138,     0,
     138,     0,     0,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,   137,   137,   137,     0,   198,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   199,
     200,   137,   137,     0,     0,     0,   201,     0,     0,   226,
     226,     0,     0,     0,   202,     0,   226,     0,   395,     0,
     203,     0,     0,   395,     0,   204,   226,   205,     0,   226,
     206,     0,     0,   138,     0,     0,   207,     0,   138,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,     0,
       0,     0,   138,     0,     0,     0,   209,     0,     0,     0,
       0,     0,     0,     0,   138,   210,   211,     0,   212,   832,
     213,     0,   214,   138,     0,   215,     0,   138,     0,   216,
       0,   138,   217,     0,     0,     0,     0,     0,     0,     0,
     977,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   155,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,   138,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   158,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
     221,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,   138,   138,   138,   138,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,   226,   138,   138,    72,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,   395,   395,     0,   395,
     395,    84,     0,    85,     0,    86,    87,   239,     0,     0,
       0,    89,     0,   160,     0,    91,   940,   161,   978,     0,
       0,     0,   944,     0,   395,     0,   395,     0,    -2,     4,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -329,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,    34,
       0,    35,    36,     0,    37,  -212,    38,    39,    40,    41,
      42,    43,   -42,    44,    45,     0,    46,     0,     0,     0,
      47,    48,  -329,    49,    50,    51,    52,  -212,   -42,    53,
      54,    55,   940,  -329,    56,    57,     0,    58,    59,    60,
      61,    62,    63,    64,    65,     0,    66,    67,     0,    68,
      69,    70,    71,    72,     0,    73,    74,    75,  -329,     0,
       0,    76,     0,    77,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    85,     0,    86,    87,     0,    88,     0,     0,
      89,     0,    90,     0,    91,     4,    92,     5,     6,     7,
       8,     9,    10,     0,  -512,     0,    11,    12,    13,    14,
      15,  -512,     0,    16,    17,    18,    19,    20,  -329,    21,
      22,    23,  -512,    24,    25,  -512,    26,     0,    27,    28,
      29,    30,    31,    32,    33,    34,     0,    35,    36,     0,
      37,  -212,    38,    39,    40,    41,    42,    43,   -42,    44,
      45,     0,    46,     0,     0,     0,    47,    48,  -329,    49,
       0,    51,    52,  -212,   -42,    53,    54,    55,  -512,  -329,
      56,    57,  -512,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,    68,    69,    70,    71,    72,
       0,    73,    74,    75,  -329,     0,     0,    76,     0,    77,
       0,     0,  -512,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -512,    80,
    -512,  -512,  -512,  -512,  -512,  -512,  -512,     0,  -512,  -512,
    -512,  -512,  -512,     0,  -512,  -512,  -512,  -512,    85,  -512,
    -512,  -512,     0,    88,  -512,  -512,  -512,     0,    90,  -512,
      91,   277,    92,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -329,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,    34,     0,    35,    36,     0,    37,  -212,    38,    39,
      40,    41,    42,    43,   -42,    44,    45,     0,    46,     0,
       0,     0,    47,    48,  -329,    49,    50,    51,    52,  -212,
     -42,    53,    54,    55,     0,  -329,    56,    57,     0,    58,
      59,    60,    61,    62,    63,    64,    65,     0,    66,    67,
       0,    68,    69,    70,    71,    72,     0,    73,    74,    75,
    -329,     0,     0,    76,     0,    77,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,   239,    88,
       0,     0,    89,     0,    90,   278,    91,     4,    92,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -329,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,    34,     0,    35,
      36,     0,    37,  -212,    38,    39,    40,    41,    42,    43,
     -42,    44,    45,     0,    46,     0,     0,     0,    47,    48,
    -329,    49,    50,    51,    52,  -212,   -42,    53,    54,    55,
       0,  -329,    56,    57,     0,    58,    59,    60,    61,    62,
      63,    64,    65,     0,    66,    67,     0,    68,    69,    70,
      71,    72,     0,    73,    74,    75,  -329,     0,     0,    76,
       0,    77,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,    86,    87,     0,    88,     0,     0,    89,     0,
      90,   439,    91,   277,    92,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -329,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,    33,    34,     0,    35,    36,     0,    37,  -212,
      38,    39,    40,    41,    42,    43,   -42,    44,    45,     0,
      46,     0,     0,     0,    47,    48,  -329,    49,    50,    51,
      52,  -212,   -42,    53,    54,    55,     0,  -329,    56,    57,
       0,    58,    59,    60,    61,    62,    63,    64,    65,     0,
      66,    67,     0,    68,    69,    70,    71,    72,     0,    73,
      74,    75,  -329,     0,     0,    76,     0,    77,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,    85,     0,    86,    87,
       0,    88,     0,     0,    89,     0,    90,   278,    91,   842,
      92,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -329,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,    34,
       0,    35,    36,     0,    37,  -212,    38,    39,    40,    41,
      42,    43,   -42,    44,    45,     0,    46,     0,     0,     0,
      47,    48,  -329,    49,    50,    51,    52,  -212,   -42,    53,
      54,    55,     0,  -329,    56,    57,     0,    58,    59,    60,
      61,    62,    63,    64,    65,     0,    66,    67,     0,    68,
      69,    70,    71,    72,     0,    73,    74,    75,  -329,     0,
       0,    76,     0,    77,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    85,     0,    86,    87,     0,    88,     0,     0,
      89,     0,    90,   843,    91,     4,    92,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -329,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,    34,     0,    35,    36,     0,
      37,  -212,    38,    39,    40,    41,    42,    43,   -42,    44,
      45,     0,    46,     0,     0,     0,    47,    48,  -329,    49,
      50,    51,    52,  -212,   -42,    53,    54,    55,     0,  -329,
      56,    57,     0,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,    68,    69,    70,    71,    72,
       0,    73,    74,    75,  -329,     0,     0,    76,     0,    77,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,    85,     0,
      86,    87,     0,    88,     0,     0,    89,     0,    90,   907,
      91,     4,    92,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -329,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,    34,     0,    35,    36,     0,    37,  -212,    38,    39,
      40,    41,    42,    43,   -42,    44,    45,     0,    46,     0,
       0,     0,    47,    48,  -329,    49,   291,    51,    52,  -212,
     -42,    53,    54,    55,     0,  -329,    56,    57,     0,    58,
      59,    60,    61,    62,    63,    64,    65,     0,    66,    67,
       0,    68,    69,    70,    71,    72,     0,    73,    74,    75,
    -329,     0,     0,    76,     0,    77,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,     0,    88,
       0,     0,    89,     0,    90,     0,    91,     4,    92,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -329,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,    34,     0,    35,
      36,     0,    37,  -212,    38,    39,    40,    41,    42,    43,
     -42,    44,    45,     0,    46,     0,     0,     0,    47,    48,
    -329,    49,     0,    51,    52,  -212,   -42,    53,    54,    55,
       0,  -329,    56,    57,     0,    58,    59,    60,    61,    62,
      63,    64,    65,     0,    66,    67,     0,    68,    69,    70,
      71,    72,     0,    73,    74,    75,  -329,     0,     0,    76,
       0,    77,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,    86,    87,     0,    88,     0,     0,    89,     0,
      90,     0,    91,     4,    92,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -329,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,   539,    34,     0,    35,    36,     0,    37,  -212,
      38,    39,    40,    41,    42,    43,   -42,    44,    45,     0,
      46,     0,     0,     0,    47,    48,  -329,    49,     0,    51,
      52,  -212,   -42,    53,    54,    55,     0,  -329,    56,    57,
       0,    58,    59,    60,    61,    62,    63,    64,    65,     0,
      66,    67,     0,    68,    69,    70,    71,    72,     0,    73,
      74,    75,  -329,     0,     0,    76,     0,    77,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,    85,     0,    86,    87,
       0,    88,     0,     0,    89,     0,    90,     0,    91,     4,
      92,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -329,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,   543,    34,
       0,    35,    36,     0,    37,  -212,    38,    39,    40,    41,
      42,    43,   -42,    44,    45,     0,    46,     0,     0,     0,
      47,    48,  -329,    49,     0,    51,    52,  -212,   -42,    53,
      54,    55,     0,  -329,    56,    57,     0,    58,    59,    60,
      61,    62,    63,    64,    65,     0,    66,    67,     0,    68,
      69,    70,    71,    72,     0,    73,    74,    75,  -329,     0,
       0,    76,     0,    77,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    85,     0,    86,    87,     0,    88,     0,     0,
      89,     0,    90,     0,    91,     4,    92,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -329,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,   797,    34,     0,    35,    36,     0,
      37,  -212,    38,    39,    40,    41,    42,    43,   -42,    44,
      45,     0,    46,     0,     0,     0,    47,    48,  -329,    49,
       0,    51,    52,  -212,   -42,    53,    54,    55,     0,  -329,
      56,    57,     0,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,    68,    69,    70,    71,    72,
       0,    73,    74,    75,  -329,     0,     0,    76,     0,    77,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,    85,     0,
      86,    87,     0,    88,     0,     0,    89,     0,    90,     0,
      91,     4,    92,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -329,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
     799,    34,     0,    35,    36,     0,    37,  -212,    38,    39,
      40,    41,    42,    43,   -42,    44,    45,     0,    46,     0,
       0,     0,    47,    48,  -329,    49,     0,    51,    52,  -212,
     -42,    53,    54,    55,     0,  -329,    56,    57,     0,    58,
      59,    60,    61,    62,    63,    64,    65,     0,    66,    67,
       0,    68,    69,    70,    71,    72,     0,    73,    74,    75,
    -329,     0,     0,    76,     0,    77,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,     0,    88,
       0,     0,    89,     0,    90,     0,    91,     4,    92,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -329,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,   802,    34,     0,    35,
      36,     0,    37,  -212,    38,    39,    40,    41,    42,    43,
     -42,    44,    45,     0,    46,     0,     0,     0,    47,    48,
    -329,    49,     0,    51,    52,  -212,   -42,    53,    54,    55,
       0,  -329,    56,    57,     0,    58,    59,    60,    61,    62,
      63,    64,    65,     0,    66,    67,     0,    68,    69,    70,
      71,    72,     0,    73,    74,    75,  -329,     0,     0,    76,
       0,    77,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,    86,    87,     0,    88,     0,     0,    89,     0,
      90,     0,    91,     4,    92,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -329,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,   805,    34,     0,    35,    36,     0,    37,  -212,
      38,    39,    40,    41,    42,    43,   -42,    44,    45,     0,
      46,     0,     0,     0,    47,    48,  -329,    49,     0,    51,
      52,  -212,   -42,    53,    54,    55,     0,  -329,    56,    57,
       0,    58,    59,    60,    61,    62,    63,    64,    65,     0,
      66,    67,     0,    68,    69,    70,    71,    72,     0,    73,
      74,    75,  -329,     0,     0,    76,     0,    77,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,    85,     0,    86,    87,
       0,    88,     0,     0,    89,     0,    90,     0,    91,     4,
      92,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -329,    21,    22,    23,     0,   827,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,    34,
       0,    35,    36,     0,    37,  -212,    38,    39,    40,    41,
      42,    43,   -42,    44,    45,     0,    46,     0,     0,     0,
      47,    48,  -329,    49,     0,    51,    52,  -212,   -42,    53,
      54,    55,     0,  -329,    56,    57,     0,    58,    59,    60,
      61,    62,    63,    64,    65,     0,    66,    67,     0,    68,
      69,    70,    71,    72,     0,    73,    74,    75,  -329,     0,
       0,    76,     0,    77,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    85,     0,    86,    87,     0,    88,     0,     0,
      89,     0,    90,     0,    91,     4,    92,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -329,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,   833,    34,     0,    35,    36,     0,
      37,  -212,    38,    39,    40,    41,    42,    43,   -42,    44,
      45,     0,    46,     0,     0,     0,    47,    48,  -329,    49,
       0,    51,    52,  -212,   -42,    53,    54,    55,     0,  -329,
      56,    57,     0,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,    68,    69,    70,    71,    72,
       0,    73,    74,    75,  -329,     0,     0,    76,     0,    77,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,    85,     0,
      86,    87,     0,    88,     0,     0,    89,     0,    90,     0,
      91,     4,    92,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -329,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
     837,    34,     0,    35,    36,     0,    37,  -212,    38,    39,
      40,    41,    42,    43,   -42,    44,    45,     0,    46,     0,
       0,     0,    47,    48,  -329,    49,     0,    51,    52,  -212,
     -42,    53,    54,    55,     0,  -329,    56,    57,     0,    58,
      59,    60,    61,    62,    63,    64,    65,     0,    66,    67,
       0,    68,    69,    70,    71,    72,     0,    73,    74,    75,
    -329,     0,     0,    76,     0,    77,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,     0,    88,
       0,     0,    89,     0,    90,     0,    91,   704,    92,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
     309,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,   310,     0,     0,    48,
     311,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
       0,    72,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,   313,     0,   314,   315,
     316,     0,   317,   318,     0,     0,     0,   321,     0,     0,
      79,    80,    81,    82,   327,   328,     0,     0,    83,   331,
       0,     0,   332,   333,   334,     0,     0,     0,    84,     0,
      85,     0,    86,    87,   239,     0,     0,     0,    89,     0,
     160,     0,    91,     0,   161,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   155,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   158,    34,     0,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,     0,
       0,     0,     0,     0,     0,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,    64,   159,     0,
      66,     0,     0,    68,     0,     0,     0,    72,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,    85,     0,    86,    87,
     239,     0,     0,     0,    89,     0,   160,     0,    91,     0,
     161,   614,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   155,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   158,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,     0,     0,     0,    72,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    85,     0,    86,    87,   239,     0,     0,
       0,    89,     0,   160,     0,    91,     0,   161,   934,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
       0,    72,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,    86,    87,     0,     0,     0,     0,    89,     0,
     160,     0,    91,     0,   161,   619,     5,     6,     7,     8,
     182,    10,   183,     0,     0,   155,     0,     0,    14,    15,
       0,     0,  -115,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   158,    34,     0,    35,     0,     0,    37,
    -480,     0,    39,    40,     0,    42,    43,     0,    44,    45,
       0,     0,     0,     0,     0,     0,    48,     0,    49,     0,
      51,     0,  -480,     0,     0,    54,  -115,     0,     0,     0,
       0,     0,     0,     0,    60,    61,    62,    63,    64,   159,
       0,    66,     0,     0,    68,     0,     0,   184,    72,     0,
    -115,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   736,     0,   737,     0,    79,    80,    81,
      82,     0,   738,     0,     0,    83,     0,   739,   200,   740,
       0,     0,     0,     0,   741,    84,     0,    85,     0,    86,
      87,     0,   202,     0,     0,    89,     0,   160,   742,    91,
       0,   161,     0,   743,     0,   205,     0,     0,   744,     0,
       0,     0,     0,     0,   745,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,     0,     0,     0,     0,
       0,     0,     0,     0,   746,     0,     0,     0,     0,     0,
       0,     0,     0,   210,   211,     0,   747,     0,   213,     0,
     748,     0,     0,   749,     0,     0,     0,   750,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,   336,   337,   338,   339,   340,     0,     0,   343,
     344,   345,   346,     0,   348,   349,   751,   752,   753,   754,
       0,     0,   755,     0,     0,     0,   756,   757,   758,   759,
     760,   761,   762,   763,   764,   765,   766,   767,     0,   768,
       0,     0,   769,   770,   771,   772,     0,     0,   773,     5,
       6,     7,     8,   182,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,  -115,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,  -480,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,  -480,     0,     0,    54,  -115,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
     184,    72,     0,  -115,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    84,     0,
      85,   155,    86,    87,    14,    15,     0,     0,    89,     0,
     160,    19,    91,     0,   161,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   158,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,   243,   244,     0,    72,   282,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,   283,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    85,     0,    86,    87,   239,     0,     0,
       0,    89,     0,   160,     0,    91,     0,   161,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   155,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,    29,   156,   157,     0,   158,    34,     0,    35,     0,
       0,    37,     0,     0,    39,    40,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    48,     0,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,   243,   244,     0,
      72,   282,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,   239,     0,     0,     0,    89,     0,   160,
       0,    91,   608,   161,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,     0,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,   243,   244,     0,    72,   282,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,   239,
       0,     0,     0,    89,     0,   160,     0,    91,   610,   161,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   155,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,   510,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,   511,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,     0,     0,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,   512,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,     0,
       0,     0,    72,     0,     0,    74,     0,   513,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    84,
       0,    85,   155,    86,    87,    14,    15,     0,     0,    89,
       0,   160,    19,    91,     0,   161,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    29,   156,   157,     0,
     158,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,   243,   244,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,     0,   245,
       0,     0,    89,     0,   160,     0,    91,     0,   161,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,   243,   244,
       0,    72,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,    86,    87,   239,     0,     0,     0,    89,     0,
     160,     0,    91,     0,   161,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   155,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   158,    34,     0,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,   706,
       0,     0,     0,     0,     0,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,    64,   159,     0,
      66,     0,     0,    68,   243,   244,     0,    72,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    84,     0,    85,   155,    86,    87,
      14,    15,     0,     0,    89,     0,   160,    19,    91,     0,
     161,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,    29,   156,   157,     0,   158,    34,     0,    35,     0,
       0,    37,     0,     0,    39,    40,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    48,   187,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,     0,     0,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,   188,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,     0,     0,     0,     0,    89,     0,   160,
       0,    91,     0,   161,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,   192,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,   193,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,     0,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,     0,     0,     0,    72,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    84,     0,    85,   155,    86,    87,    14,
      15,     0,     0,    89,     0,   160,    19,    91,     0,   161,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
      29,   156,   157,     0,   158,    34,     0,    35,     0,     0,
      37,  -480,     0,    39,    40,     0,    42,    43,     0,    44,
      45,     0,     0,     0,     0,     0,     0,    48,     0,    49,
       0,    51,     0,  -480,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    63,    64,
     159,     0,    66,     0,     0,    68,     0,     0,     0,    72,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    84,     0,    85,   155,
      86,    87,    14,    15,     0,     0,    89,     0,   160,    19,
      91,     0,   161,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,     0,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,     0,     0,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,   243,
     244,     0,    72,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    84,
       0,    85,   155,    86,    87,    14,    15,     0,     0,    89,
       0,   160,    19,    91,     0,   161,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    29,   156,   157,     0,
     158,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,     0,     0,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,   239,     0,
       0,     0,    89,     0,   160,   606,    91,     0,   161,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,   225,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
       0,    72,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    84,     0,
      85,   155,    86,    87,    14,    15,     0,     0,    89,     0,
      90,    19,    91,     0,   161,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   158,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,     0,     0,     0,    72,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    85,     0,    86,    87,   239,     0,     0,
       0,    89,     0,   160,     0,    91,     0,   161,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   155,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,    29,   156,   157,     0,   158,    34,     0,    35,     0,
       0,    37,     0,     0,    39,    40,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    48,     0,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,     0,     0,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,   188,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    84,     0,    85,
     155,    86,    87,    14,   576,     0,     0,    89,     0,   160,
      19,    91,     0,   161,     0,     0,     0,     0,   577,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   158,    34,
       0,   578,     0,     0,    37,     0,     0,    39,    40,     0,
      42,    43,     0,    44,    45,     0,     0,     0,     0,     0,
       0,   579,     0,    49,     0,    51,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,   580,
     581,    62,    63,    64,   582,     0,    66,     0,     0,    68,
       0,     0,     0,    72,     0,     0,   583,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    85,     0,    86,    87,   239,     0,     0,     0,
      89,     0,   160,     0,    91,     0,   885,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   155,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
      29,   156,   157,     0,   158,    34,     0,    35,     0,     0,
      37,     0,     0,    39,    40,     0,    42,    43,     0,    44,
      45,     0,     0,     0,     0,     0,     0,    48,     0,    49,
       0,    51,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    63,    64,
     159,     0,    66,     0,     0,    68,     0,     0,     0,    72,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,    85,     0,
      86,    87,   239,     0,     0,     0,    89,     0,   160,     0,
      91,     0,   885,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   155,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    29,   156,   157,     0,
     870,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,     0,     0,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,   239,     0,
       0,     0,    89,     0,   160,     0,    91,     0,   161,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
       0,    72,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    84,     0,
      85,   155,    86,    87,    14,    15,     0,     0,    89,     0,
     160,    19,    91,     0,   161,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   158,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,     0,     0,     0,    72,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    84,     0,    85,   155,    86,    87,    14,   475,     0,
       0,    89,     0,    90,    19,    91,     0,   161,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   158,    34,     0,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,     0,
       0,     0,     0,     0,     0,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,    64,   159,     0,
      66,     0,     0,    68,     0,     0,     0,    72,     0,     0,
     476,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    84,     0,    85,   155,    86,    87,
      14,    15,     0,     0,    89,     0,   160,    19,    91,     0,
     161,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,    29,   156,   157,     0,   158,    34,     0,    35,     0,
       0,    37,     0,     0,    39,    40,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    48,     0,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,     0,     0,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    84,     0,    85,
     155,    86,    87,    14,   576,     0,     0,   549,     0,   160,
      19,    91,     0,   161,     0,     0,     0,     0,   577,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   158,    34,
       0,   578,     0,     0,    37,     0,     0,    39,    40,     0,
      42,    43,     0,    44,    45,     0,     0,     0,     0,     0,
       0,   579,     0,    49,     0,    51,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,   580,
     581,    62,    63,    64,   582,     0,    66,     0,     0,    68,
       0,     0,     0,    72,     0,     0,   583,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      84,     0,    85,   155,    86,    87,    14,    15,     0,     0,
      89,     0,   160,    19,    91,     0,   584,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,     0,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,     0,     0,     0,    72,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    84,     0,    85,   155,    86,    87,    14,
      15,     0,     0,    89,     0,   160,    19,    91,     0,   584,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
      29,   156,   157,     0,   649,    34,     0,    35,     0,     0,
      37,     0,     0,    39,    40,     0,    42,    43,     0,    44,
      45,     0,     0,     0,     0,     0,     0,    48,     0,    49,
       0,    51,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    63,    64,
     159,     0,    66,     0,     0,    68,     0,     0,     0,    72,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    84,     0,    85,   155,
      86,    87,    14,    15,     0,     0,    89,     0,   160,    19,
      91,     0,   161,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   652,    34,     0,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,     0,     0,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,     0,
       0,     0,    72,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    84,
       0,    85,   155,    86,    87,    14,    15,     0,     0,    89,
       0,   160,    19,    91,     0,   161,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    29,   156,   157,     0,
     864,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,     0,     0,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    84,     0,    85,   155,    86,    87,    14,    15,
       0,     0,    89,     0,   160,    19,    91,     0,   161,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   865,    34,     0,    35,     0,     0,    37,
       0,     0,    39,    40,     0,    42,    43,     0,    44,    45,
       0,     0,     0,     0,     0,     0,    48,     0,    49,     0,
      51,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,    62,    63,    64,   159,
       0,    66,     0,     0,    68,     0,     0,     0,    72,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    84,     0,    85,   155,    86,
      87,    14,    15,     0,     0,    89,     0,   160,    19,    91,
       0,   161,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   867,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
       0,    72,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     5,     6,     7,     8,     9,    10,    84,     0,
      85,   155,    86,    87,    14,    15,     0,     0,    89,     0,
     160,    19,    91,     0,   161,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   868,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,     0,     0,     0,    72,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    84,     0,    85,   155,    86,    87,    14,    15,     0,
       0,    89,     0,   160,    19,    91,     0,   161,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   869,    34,     0,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,     0,
       0,     0,     0,     0,     0,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,    64,   159,     0,
      66,     0,     0,    68,     0,     0,     0,    72,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,    80,    81,    82,
       0,     0,     0,     0,    83,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    84,     0,    85,   155,    86,    87,
      14,    15,     0,     0,    89,     0,   160,    19,    91,     0,
     161,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,    29,   156,   157,     0,   870,    34,     0,    35,     0,
       0,    37,     0,     0,    39,    40,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    48,     0,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,     0,     0,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    84,     0,    85,
     155,    86,    87,    14,   576,     0,     0,    89,     0,   160,
      19,    91,     0,   161,     0,     0,     0,     0,   577,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   158,    34,
       0,   578,     0,     0,    37,     0,     0,    39,    40,     0,
      42,    43,     0,    44,    45,     0,     0,     0,     0,     0,
       0,   579,     0,    49,     0,    51,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,   580,
     581,    62,    63,    64,   582,     0,    66,     0,     0,    68,
       0,     0,     0,    72,     0,     0,   583,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      84,     0,    85,   155,    86,    87,    14,    15,     0,     0,
      89,     0,   160,    19,    91,     0,   947,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,     0,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,     0,     0,     0,    72,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    84,     0,    85,   155,    86,    87,    14,
      15,     0,     0,    89,     0,     0,    19,    91,     0,   947,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
      29,   156,   157,     0,   158,    34,     0,    35,     0,     0,
      37,     0,     0,    39,    40,     0,    42,    43,     0,    44,
      45,     0,     0,     0,     0,     0,     0,    48,     0,    49,
       0,    51,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    63,    64,
     159,     0,    66,     0,     0,    68,     0,     0,     0,    72,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,    84,     0,    85,   155,
       0,    87,    14,    15,     0,     0,    89,     0,   160,    19,
      91,     0,   161,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,     0,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,     0,     0,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,     0,
       0,     0,    72,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -366,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,     0,     0,  -366,     0,     0,     0,
    -366,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     155,    85,     0,    14,    15,   239,     0,     0,     0,     0,
      19,   160,     0,    91,  -366,   885,     0,     0,    25,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   158,    34,
       0,    35,     0,     0,    37,     0,     0,    39,    40,     0,
      42,    43,     0,    44,    45,     0,     0,     0,     0,     0,
       0,    48,     0,    49,     0,    51,     0,     0,     0,     0,
      54,     0,     0,     0,     0,  -348,     0,     0,     0,    60,
      61,    62,    63,    64,   159,     0,    66,     0,  -348,    68,
       0,     0,     0,    72,     0,     0,    74,     0,     0,     0,
    -348,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,     0,     0,     0,     0,
       0,     0,     0,    80,     5,     6,     7,     8,     9,    10,
       0,  -460,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,    85,    19,     0,   350,     0,  -348,  -489,     0,
    -489,    25,  -348,     0,    91,     0,   947,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,     0,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,     0,     0,     0,    72,     0,     0,    74,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   155,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,    80,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,     0,
      35,     0,     0,    37,     0,    85,    39,    40,     0,    42,
      43,     0,    44,    45,     0,   160,     0,    91,     0,   161,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,     0,
       0,     0,    72,     0,     0,    74,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   155,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,    80,    25,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   158,    34,     0,    35,     0,     0,    37,
       0,    85,    39,    40,     0,    42,    43,     0,    44,    45,
       0,   160,     0,    91,     0,   584,    48,     0,    49,     0,
      51,     0,     0,     0,   871,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,    62,    63,    64,   159,
       0,    66,  -198,     0,    68,     0,  -198,  -198,    72,     0,
       0,    74,     0,     0,     0,  -198,     0,  -198,  -198,     0,
       0,     0,  -198,     0,     0,     0,     0,  -198,     0,     0,
    -198,     0,     0,     0,     0,     0,     0,     0,    80,     0,
       0,     0,     0,     0,     0,  -198,     0,  -198,   198,  -198,
       0,  -198,  -198,     0,  -198,     0,  -198,    85,  -198,     0,
     199,   200,     0,     0,     0,     0,     0,   201,     0,    91,
       0,   161,     0,     0,     0,   202,     0,  -198,     0,     0,
    -198,   203,     0,  -198,     0,     0,   204,     0,   205,     0,
       0,   206,     0,     0,     0,     0,     0,   207,     0,     0,
       0,   198,     0,     0,     0,     0,     0,     0,   208,     0,
       0,     0,     0,   199,   200,     0,     0,   209,     0,     0,
     201,     0,     0,     0,     0,     0,   210,   211,   202,   212,
       0,   213,  -198,   214,   203,     0,   215,     0,  -198,   204,
     216,   205,     0,   217,   206,     0,     0,     0,     0,     0,
     207,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   208,     0,     0,   914,     0,     0,     0,     0,     0,
     209,   738,     0,     0,     0,     0,   199,   200,   740,   210,
     211,     0,   212,   201,   213,     0,   214,     0,     0,   215,
       0,   202,     0,   216,     0,     0,   217,   742,     0,   363,
       0,   714,   204,     0,   205,     0,     0,   206,     0,     0,
       0,     0,     0,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,     0,     0,     0,     0,     0,
       0,     0,     0,   209,     0,     0,     0,     0,     0,     0,
       0,     0,   210,   211,     0,   212,     0,   213,     0,   915,
       0,   307,   749,     0,   694,     0,   216,     0,   308,   217,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
     335,   336,   337,   338,   339,   340,     0,     0,   343,   344,
     345,   346,     0,   348,   349,   751,   752,   753,   754,     0,
       0,   755,     0,     0,     0,   756,   757,   758,   759,   760,
     761,   762,   763,   764,   765,   766,   767,     0,   768,   309,
       0,   769,   770,   771,   772,   310,     0,     0,     0,   311,
       0,   307,     0,     0,     0,     0,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,     0,   388,     0,     0,     0,   312,
       0,     0,     0,     0,     0,   310,     0,     0,     0,   311,
       0,     0,     0,     0,     0,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,   310,   329,   330,   331,   311,
       0,   332,   333,   334,     0,     0,   307,     0,     0,     0,
     723,     0,     0,   308,     0,   313,     0,   314,   315,   316,
       0,   317,   318,   319,   309,   320,   321,   322,   323,   312,
       0,   325,   326,   327,   328,     0,   329,     0,   331,     0,
       0,   332,   333,   334,     0,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     310,   332,   333,   334,   311,     0,   307,     0,     0,     0,
     791,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,   314,   315,   316,     0,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
     310,   329,   330,   331,   311,     0,   332,   333,   334,     0,
       0,   307,     0,     0,     0,   943,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,   312,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,   314,   315,   316,     0,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   310,   332,   333,   334,   311,
       0,   307,     0,     0,     0,   991,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,   310,   329,   330,   331,   311,
       0,   332,   333,   334,     0,     0,   307,     0,   607,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,   312,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     310,   332,   333,   334,   311,     0,     0,     0,   913,     0,
       0,   307,     0,   394,     0,     0,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,   312,   335,   336,   337,   338,   339,
     340,     0,     0,   343,   344,   345,   346,     0,   348,   349,
     313,     0,   314,   315,   316,     0,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   310,   332,   333,   334,   311,
     363,     0,     0,     0,     0,     0,   307,     0,   796,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,   312,
     335,   336,   337,   338,   339,   340,     0,     0,   343,   344,
     345,   346,     0,   348,   349,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     310,   332,   333,   334,   311,   363,     0,     0,     0,     0,
       0,   307,     0,   801,     0,     0,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,   312,   335,   336,   337,   338,   339,
     340,     0,     0,   343,   344,   345,   346,     0,   348,   349,
     313,     0,   314,   315,   316,     0,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   310,   332,   333,   334,   311,
     363,     0,     0,     0,     0,     0,   307,     0,   926,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,   312,
     335,   336,   337,   338,   339,   340,     0,     0,   343,   344,
     345,   346,     0,   348,   349,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     310,   332,   333,   334,   311,   363,     0,     0,     0,     0,
       0,   307,     0,   927,     0,     0,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,   312,   335,   336,   337,   338,   339,
     340,     0,     0,   343,   344,   345,   346,     0,   348,   349,
     313,     0,   314,   315,   316,     0,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   310,   332,   333,   334,   311,
     363,     0,     0,     0,     0,     0,   307,     0,   928,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,   312,
     335,   336,   337,   338,   339,   340,     0,     0,   343,   344,
     345,   346,     0,   348,   349,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     310,   332,   333,   334,   311,   363,     0,     0,     0,     0,
       0,   307,     0,   929,     0,     0,     0,     0,   308,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,   312,   335,   336,   337,   338,   339,
     340,     0,     0,   343,   344,   345,   346,     0,   348,   349,
     313,     0,   314,   315,   316,     0,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
       0,   329,   330,   331,     0,   310,   332,   333,   334,   311,
     363,     0,     0,     0,     0,     0,   307,     0,   945,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,   312,
     335,   336,   337,   338,   339,   340,     0,     0,   343,   344,
     345,   346,     0,   348,   349,   313,     0,   314,   315,   316,
       0,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     310,   332,   333,   334,   311,   363,     0,     0,     0,     0,
       0,     0,     0,   946,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,   312,   335,   336,   337,   338,   339,
     340,   309,   386,   343,   344,   345,   346,     0,   348,   349,
     313,     0,   314,   315,   316,   387,   317,   318,   319,     0,
     320,   321,   322,   323,   324,     0,   325,   326,   327,   328,
       0,   329,   330,   331,   307,     0,   332,   333,   334,     0,
     363,   308,     0,     0,     0,     0,     0,   310,     0,     0,
       0,   311,   309,   383,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   384,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   388,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   310,   314,
     315,   316,   311,   317,   318,   319,     0,   320,   321,   322,
     323,   324,     0,   325,   326,   327,   328,     0,   329,   330,
     331,   307,     0,   332,   333,   334,     0,   363,   308,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,   309,
     675,     0,     0,     0,     0,     0,     0,     0,   313,     0,
     314,   315,   316,     0,   317,   318,   319,     0,   320,   321,
     322,   323,   324,     0,   325,   326,   327,   328,     0,   329,
     330,   331,   307,     0,   332,   333,   334,     0,   363,   308,
       0,     0,     0,     0,     0,   310,     0,     0,     0,   311,
     309,   225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,   388,     0,     0,     0,   312,
       0,   309,   669,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,   310,   314,   315,   316,
     311,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,     0,
     309,   332,   333,   334,     0,   363,     0,   310,     0,     0,
     312,   311,     0,   307,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,   309,   317,   318,   319,     0,   320,   321,   322,   323,
     324,   312,   325,   326,   327,   328,   310,   329,   330,   331,
     311,     0,   332,   333,   334,     0,   363,   313,     0,   314,
     315,   316,     0,   317,   318,   319,     0,   320,   321,   322,
     323,   324,     0,   325,   326,   327,   328,   310,   329,   330,
     331,   311,     0,   332,   333,   334,     0,   363,   307,     0,
     687,     0,     0,     0,     0,   308,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   309,   321,   322,   323,
       0,   312,   325,   326,   327,   328,     0,     0,     0,   331,
       0,     0,   332,   333,   334,     0,     0,   313,     0,   314,
     315,   316,     0,   317,   318,   319,     0,   320,   321,   322,
     323,   324,     0,   325,   326,   327,   328,     0,   329,   330,
     331,     0,   310,   332,   333,   334,   311,   363,   307,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,   391,     0,     0,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,   314,   315,   316,   392,   317,   318,
     319,     0,   320,   321,   322,   323,   324,     0,   325,   326,
     327,   328,   310,   329,   330,   331,   311,     0,   332,   333,
     334,     0,   421,     0,     0,     0,     0,   307,     0,     0,
       0,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
       0,     0,   313,     0,   314,   315,   316,     0,   317,   318,
     319,     0,   320,   321,   322,   323,   324,     0,   325,   326,
     327,   328,     0,   329,   330,   331,     0,   393,   332,   333,
     334,   310,     0,     0,     0,   311,     0,   307,     0,     0,
       0,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
     434,   150,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,   435,   317,   318,   319,
       0,   320,   321,   322,   323,   324,     0,   325,   326,   327,
     328,   310,   329,   330,   331,   311,     0,   332,   333,   334,
       0,     0,   307,     0,     0,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   519,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,     0,   317,   318,   319,
       0,   320,   321,   322,   323,   324,     0,   325,   326,   327,
     328,     0,   329,   330,   331,     0,   310,   332,   333,   334,
     311,     0,   307,     0,     0,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   521,     0,     0,     0,     0,     0,     0,     0,     0,
     312,     0,     0,   307,   522,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,   309,   317,   318,   319,     0,   320,   321,   322,   323,
     324,     0,   325,   326,   327,   328,   310,   329,   330,   331,
     311,     0,   332,   333,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,     0,   310,     0,     0,
     312,   311,   309,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   320,   321,   322,   323,
     324,   312,   325,   326,   327,   328,     0,   329,   330,   331,
       0,     0,   332,   333,   334,     0,     0,   313,   310,   314,
     315,   316,   311,   317,   318,   319,     0,   320,   321,   322,
     323,   324,     0,   325,   326,   327,   328,     0,   329,   330,
     331,   307,   425,   332,   333,   334,     0,     0,   308,     0,
       0,     0,   312,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,   313,     0,
     314,   315,   316,     0,   317,   318,   319,     0,   320,   321,
     322,   323,   324,     0,   325,   326,   327,   328,     0,   329,
     330,   331,   307,   427,   332,   333,   334,     0,     0,   308,
       0,     0,     0,     0,     0,   310,     0,     0,     0,   311,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,   312,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,   313,   310,   314,   315,   316,
     311,   317,   318,   319,     0,   320,   321,   322,   323,   324,
       0,   325,   326,   327,   328,     0,   329,   330,   331,   307,
     429,   332,   333,   334,     0,     0,   308,     0,     0,     0,
     312,     0,   310,     0,     0,     0,   311,   309,     0,     0,
       0,     0,     0,     0,     0,   523,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   320,   321,   322,   323,
     324,     0,   325,   326,   327,   328,   312,   329,   330,   331,
       0,   437,   332,   333,   334,     0,     0,     0,     0,     0,
       0,     0,   313,   310,   314,   315,   316,   311,   317,   318,
     319,     0,   320,   321,   322,   323,   324,     0,   325,   326,
     327,   328,     0,   329,   330,   331,   307,     0,   332,   333,
     334,     0,     0,   308,     0,     0,   452,   312,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,   314,   315,   316,     0,   317,
     318,   319,     0,   320,   321,   322,   323,   324,     0,   325,
     326,   327,   328,     0,   329,   330,   331,   307,     0,   332,
     333,   334,     0,     0,   308,     0,     0,     0,     0,     0,
     310,     0,     0,     0,   311,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,   312,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,   310,   314,   315,   316,   311,   317,   318,   319,     0,
     320,   321,   322,   323,   324,  -461,   325,   326,   327,   328,
       0,   329,   330,   331,     0,     0,   332,   333,   334,     0,
       0,   150,   310,     0,     0,   312,   311,     0,   307,   682,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,   309,   317,   318,   319,
       0,   320,   321,   322,   323,   324,   312,   325,   326,   327,
     328,     0,   329,   330,   331,     0,     0,   332,   333,   334,
       0,     0,   313,     0,   314,   315,   316,     0,   317,   318,
     319,     0,   320,   321,   322,   323,   324,     0,   325,   326,
     327,   328,   310,   329,   330,   331,   311,   659,   332,   333,
     334,     0,     0,   307,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,     0,   685,     0,   312,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,   314,   315,   316,     0,   317,   318,
     319,     0,   320,   321,   322,   323,   324,     0,   325,   326,
     327,   328,     0,   329,   330,   331,     0,   310,   332,   333,
     334,   311,     0,   307,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,   314,
     315,   316,     0,   317,   318,   319,     0,   320,   321,   322,
     323,   324,     0,   325,   326,   327,   328,   310,   329,   330,
     331,   311,     0,   332,   333,   334,     0,     0,   307,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   786,     0,     0,
     726,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,     0,   314,
     315,   316,     0,   317,   318,   319,     0,   320,   321,   322,
     323,   324,     0,   325,   326,   327,   328,     0,   329,   330,
     331,     0,   310,   332,   333,   334,   311,     0,   307,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   309,   789,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,     0,   307,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,     0,   313,     0,   314,   315,   316,   309,   317,   318,
     319,     0,   320,   321,   322,   323,   324,     0,   325,   326,
     327,   328,   310,   329,   330,   331,   311,     0,   332,   333,
     334,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,   880,     0,     0,     0,     0,     0,   308,     0,     0,
       0,     0,     0,   310,     0,     0,   312,   311,   309,     0,
       0,     0,     0,     0,     0,     0,   863,     0,     0,     0,
       0,     0,   313,     0,   314,   315,   316,     0,   317,   318,
     319,     0,   320,   321,   322,   323,   324,   312,   325,   326,
     327,   328,     0,   329,   330,   331,     0,     0,   332,   333,
     334,     0,     0,   313,   310,   314,   315,   316,   311,   317,
     318,   319,     0,   320,   321,   322,   323,   324,     0,   325,
     326,   327,   328,     0,   329,   330,   331,   307,     0,   332,
     333,   334,     0,     0,   308,     0,     0,     0,   312,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,   314,   315,   316,     0,
     317,   318,   319,     0,   320,   321,   322,   323,   324,     0,
     325,   326,   327,   328,     0,   329,   330,   331,     0,     0,
     332,   333,   334,     0,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,   307,     0,   866,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,     0,   317,   318,   319,
       0,   320,   321,   322,   323,   324,     0,   325,   326,   327,
     328,     0,   329,   330,   331,     0,   310,   332,   333,   334,
     311,     0,     0,     0,     0,     0,     0,   307,     0,   956,
       0,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   320,   321,   322,   323,
     324,     0,   325,   326,   327,   328,     0,   329,   330,   331,
       0,   310,   332,   333,   334,   311,     0,     0,     0,     0,
       0,     0,   307,     0,   957,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,     0,   317,   318,   319,
       0,   320,   321,   322,   323,   324,     0,   325,   326,   327,
     328,     0,   329,   330,   331,     0,   310,   332,   333,   334,
     311,     0,     0,     0,     0,     0,     0,   307,     0,   958,
       0,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   320,   321,   322,   323,
     324,     0,   325,   326,   327,   328,     0,   329,   330,   331,
       0,   310,   332,   333,   334,   311,     0,     0,     0,     0,
       0,     0,   307,     0,   959,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,     0,   317,   318,   319,
       0,   320,   321,   322,   323,   324,     0,   325,   326,   327,
     328,     0,   329,   330,   331,     0,   310,   332,   333,   334,
     311,     0,     0,     0,     0,     0,     0,   307,     0,   960,
       0,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   320,   321,   322,   323,
     324,     0,   325,   326,   327,   328,     0,   329,   330,   331,
       0,   310,   332,   333,   334,   311,     0,     0,     0,     0,
       0,     0,   307,     0,   961,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,   312,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   313,     0,   314,   315,   316,     0,   317,   318,   319,
       0,   320,   321,   322,   323,   324,     0,   325,   326,   327,
     328,     0,   329,   330,   331,     0,   310,   332,   333,   334,
     311,     0,   307,     0,     0,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     312,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,   309,   317,   318,   319,     0,   320,   321,   322,   323,
     324,     0,   325,   326,   327,   328,   647,   329,   330,   331,
     311,     0,   332,   333,   334,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   310,     0,     0,
     312,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   314,   315,
     316,     0,   317,   318,   319,     0,   320,   321,   322,   323,
     324,   312,   325,   326,   327,   328,     0,   329,   330,   331,
       0,     0,   332,   333,   334,     0,     0,   313,     0,   314,
     315,   316,     0,   317,   318,   319,     0,   320,   321,   322,
     323,     0,     0,   325,   326,   327,   328,   624,   329,   198,
     331,     0,     0,   332,   333,   334,     0,     0,     0,     0,
       0,   199,   200,     0,     0,     0,     0,     0,   201,     0,
       0,     0,     0,     0,     0,     0,   202,     0,     0,     0,
       0,     0,   203,     0,     0,     0,     0,   204,     0,   205,
       0,     0,   206,   198,     0,     0,     0,     0,   207,     0,
       0,     0,     0,     0,     0,   199,   200,     0,     0,   208,
       0,     0,   201,     0,     0,     0,     0,     0,   209,     0,
     202,     0,     0,     0,     0,     0,   203,   210,   211,     0,
     212,   204,   213,   205,   214,     0,   206,   215,     0,     0,
       0,   216,   207,     0,   217,     0,     0,     0,     0,     0,
       0,     0,     0,   208,     0,     0,     0,     0,     0,     0,
       0,     0,   209,     0,     0,     0,     0,     0,     0,     0,
       0,   210,   211,     0,   212,     0,   213,     0,   214,     0,
       0,   215,     0,     0,     0,   216,     0,     0,   217,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,     0,  -489,     0,  -489
};

static const yytype_int16 yycheck[] =
{
      11,   135,     2,   356,    15,    52,    53,    18,   430,   288,
     547,   703,    23,   633,   173,   402,    27,    28,   729,    30,
      31,    32,    33,   819,     1,   184,     1,    60,    17,   894,
      41,    29,    43,    44,   399,    46,     1,    48,    49,   575,
       1,    29,   179,    42,   197,    56,    57,    58,    59,    60,
      61,    29,    56,    24,    65,    29,    67,   934,    69,    70,
     197,    77,   149,    74,    75,    76,    77,   816,   101,    80,
      42,    75,    83,    84,    85,    42,    87,    59,   392,    90,
      91,    92,   235,   777,   778,   104,   407,    42,    92,   781,
      91,    42,    77,    64,   129,    60,   973,    42,   235,   978,
      77,   254,    42,    75,    75,   104,   104,   644,   987,   430,
     646,    42,   129,    90,   910,   980,    77,   254,   104,    19,
     155,   435,   155,    94,    75,   102,   149,   261,   820,     1,
     129,   149,   155,   149,   109,   884,   101,   155,   154,   850,
     159,   102,   159,     2,   155,   156,   157,   158,   159,   160,
     161,   128,    11,   128,   129,   129,   154,   161,    19,   129,
     159,   872,   129,    22,   700,    24,   154,   128,   179,   154,
      42,   109,   149,    73,   129,   129,   154,   154,   179,     0,
     154,   318,   157,   172,   129,   155,   197,   574,    77,   129,
     155,   883,    57,   154,   547,   133,   197,    97,   129,   352,
     353,    90,     3,   157,   159,     3,    65,   189,   190,   191,
      69,    70,    73,   102,   159,   352,   353,    77,    47,   159,
     129,    30,   916,   917,   235,   236,    91,   129,   159,    30,
      90,    90,   243,   244,   235,    94,    97,   109,   129,   859,
      69,   933,   102,   254,   129,   129,   228,   129,   157,   231,
     129,    52,    94,   254,   129,   157,   128,   129,   149,   156,
     149,   129,   129,   129,   149,   154,   545,   546,   250,   156,
     423,   808,   283,   157,   444,   157,   446,   149,   157,    28,
     280,   149,   157,   149,    69,    70,   423,    88,   270,   149,
     157,   644,   151,    94,   154,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   694,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,    77,    74,   129,   692,     7,    78,
     341,   342,   973,   612,   613,   129,   347,    90,   156,   350,
     351,   352,   353,   354,   355,   356,   149,   109,    24,   102,
     991,   352,   353,    74,   365,    70,    28,    78,   369,   218,
     371,    74,    85,   157,   729,    78,   225,   129,   156,     2,
     381,   133,   383,   384,    24,   386,   387,   381,    11,   128,
     391,   392,   131,   394,   366,   396,   135,   128,    64,    22,
     131,    24,   149,   149,   135,   157,   149,   534,   409,    75,
     131,   154,    74,   149,   826,   416,    78,   139,   390,   156,
     149,   158,   423,   149,    64,   416,   149,   156,    94,   158,
     149,   280,   423,   434,   435,    75,   156,   109,   158,   156,
     441,   158,    65,   444,   156,   446,   154,   448,   449,   450,
     441,   452,   453,   444,   603,   446,   457,   129,   449,   149,
     100,   133,   124,   457,   149,   808,   128,    90,   130,   131,
     149,    94,   103,   135,   475,   476,   156,   109,   156,   149,
     156,   143,   149,     7,    54,   157,   341,   342,   150,   151,
     152,   128,   347,   154,    75,   350,   351,    42,   149,   109,
     855,   157,   129,    28,   149,    42,   156,   129,   819,   633,
      31,   310,   311,    28,   363,   826,   157,   872,   519,   520,
     521,   522,   523,   524,   525,   109,    18,   157,   151,   157,
     129,   532,    77,   534,   383,   159,   537,   386,   539,   157,
     154,   532,   543,   534,   149,   394,   547,   526,   527,    74,
     157,   149,   149,    78,   149,   354,   355,   102,   149,    74,
     149,   129,   149,    78,   109,   159,   365,   149,    31,   541,
     542,   157,   155,   545,   546,   576,   149,   157,   579,   580,
     581,   582,   583,   584,   129,   384,   149,   588,   387,   444,
     584,   446,   154,    24,   129,   218,   155,   109,    75,   910,
      29,   450,   225,   128,   149,   130,   131,    29,   159,   154,
     135,    42,   157,   128,    45,   130,   131,    31,   143,   128,
     135,   154,    75,   934,   128,   150,   151,   152,   143,   159,
      61,   159,   943,    64,   156,    66,   159,   152,   155,    31,
     131,   131,   131,   644,    75,   131,   647,   648,   649,   448,
     131,   652,   131,   131,   157,   131,   131,   280,   131,   131,
     102,   157,   973,    94,    90,   159,   667,   978,   669,   727,
     155,   672,   435,   434,   675,    21,   987,   532,   714,   680,
     991,   682,   536,   262,   685,   657,   658,   185,   794,   732,
     859,   855,   664,   692,   567,   735,   980,   933,   129,    19,
     952,    32,   674,    23,   304,   677,   410,   534,   557,   710,
      24,    27,    32,    -1,    34,    35,   710,    -1,    -1,    39,
     721,   520,   723,   522,    44,   726,   157,    47,    42,    -1,
     731,    45,    -1,   588,    -1,   859,    -1,    -1,    -1,    -1,
     363,    -1,    62,    -1,    -1,     2,    66,    61,    68,    69,
      64,    71,    66,    73,    11,    -1,    -1,    -1,    -1,    -1,
     383,    75,    -1,   386,    -1,    22,    -1,    24,   617,    -1,
      23,   394,   773,    -1,    94,    -1,    -1,    97,    -1,    -1,
      94,    34,    35,    -1,   785,   786,    -1,   788,   789,   790,
     791,    44,    -1,    -1,    47,   796,   797,    -1,   799,    -1,
     801,   802,    -1,    -1,   805,    -1,    -1,   808,    65,    62,
     847,   848,    -1,    -1,    -1,    -1,    69,    -1,   667,    -1,
     669,    -1,    -1,   672,   825,   826,   675,   450,   819,   149,
     831,    -1,   833,    90,    -1,   684,   837,    94,   687,    38,
      -1,    40,    -1,    -1,    -1,   846,    -1,    -1,   647,   648,
      -1,    -1,    -1,    -1,   844,    -1,    -1,   829,    -1,    23,
     709,    -1,   863,   864,   865,   866,   867,   868,   869,   870,
      34,    35,   721,    -1,   723,    -1,    -1,    -1,   727,   880,
      44,    -1,    -1,    47,   885,    -1,    -1,     2,    -1,   890,
      54,   885,   893,   516,   151,    -1,    11,    -1,    62,   890,
      -1,    -1,    42,    -1,    -1,    69,    70,    22,    -1,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,    -1,    -1,    -1,    -1,   926,   927,   928,   929,    -1,
      -1,    -1,    -1,    -1,   557,    28,    -1,    77,    -1,    -1,
      -1,    42,   943,   934,   945,   946,   947,   796,    -1,    -1,
      65,    -1,   801,   947,    -1,   956,   957,   958,   959,   960,
     961,   218,   102,    -1,    -1,    -1,   815,    -1,   225,   109,
      -1,   972,   973,    -1,    -1,    90,    77,    -1,   827,    94,
      -1,    74,   973,    -1,    -1,    78,   987,   836,   187,   129,
     991,   840,    -1,    -1,   617,   844,    -1,    -1,    -1,    -1,
     991,   102,    -1,    -1,    -1,    -1,    -1,    -1,   109,   149,
     633,    -1,    -1,    -1,   154,    -1,    28,   157,    -1,    -1,
      -1,    -1,   221,   280,   647,   648,    -1,    -1,   129,    -1,
      -1,   124,   881,   126,   127,   128,   151,   130,   131,    -1,
     239,    -1,   135,    -1,   667,    -1,   669,    -1,   149,   672,
     143,    24,   675,   154,    -1,    -1,   157,   150,   151,   152,
      -1,   684,    74,    -1,   687,     2,    78,    -1,    -1,    42,
      -1,    -1,    45,    -1,    11,    -1,    -1,   926,   927,   928,
     929,    -1,    -1,    -1,    -1,    22,   709,    24,    61,    -1,
      -1,    64,    -1,    66,    -1,    -1,   945,   946,   721,    -1,
     723,    -1,    75,   218,   727,   304,   363,    -1,    -1,    -1,
     225,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    94,    -1,   135,    -1,    -1,   383,    -1,    65,   386,
     142,   143,    -1,    -1,    -1,   147,     2,   394,   150,   151,
     152,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,     3,    -1,    90,    -1,    -1,    22,    94,    24,    -1,
      -1,    -1,     2,    15,    -1,   280,    -1,    -1,    -1,    -1,
      22,    11,    -1,   796,    -1,    -1,    -1,    -1,   801,    -1,
     379,    -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    41,
      -1,    -1,   815,   450,    46,    23,    -1,    -1,    -1,    65,
      52,    -1,    -1,   402,   827,    -1,    34,    35,    -1,    -1,
      -1,   410,    -1,   836,   151,    -1,    44,   840,    -1,    47,
      72,   844,    -1,    -1,    90,    65,    54,    -1,    94,    -1,
      -1,    -1,    84,    -1,    62,    -1,    88,    -1,    -1,    -1,
      -1,    69,    70,    95,    -1,    -1,    -1,    -1,   363,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,   456,   881,    -1,
     459,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   383,    -1,
      -1,   386,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   394,
      -1,   218,    -1,    -1,    -1,   151,    -1,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
     557,    -1,    -1,   926,   927,   928,   929,    11,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,   945,   946,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   450,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   280,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    19,    -1,    -1,    -1,    23,   225,
     617,    65,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,
      35,   570,    -1,    -1,    39,   574,    -1,    -1,   218,    44,
      -1,    -1,    47,    -1,    -1,   225,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
     599,    66,    -1,    68,    69,    -1,    71,    -1,    73,    -1,
     667,    -1,   669,    -1,   280,   672,    -1,    -1,   675,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   363,   684,    -1,    94,
     687,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     280,    -1,   557,    -1,    -1,    -1,   383,   151,    -1,   386,
      -1,    -1,   709,    -1,    -1,    -1,    -1,   394,    -1,    -1,
      -1,    -1,    -1,    -1,   721,    -1,   723,    -1,    -1,    -1,
     727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    11,    -1,
     155,    -1,   691,    -1,    -1,   694,    -1,   363,    -1,    22,
      -1,    24,   617,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   450,   218,   714,    -1,   383,    -1,    -1,
     386,   225,    -1,   363,    -1,    -1,    -1,    -1,   394,    -1,
      -1,    -1,    -1,   732,    -1,    -1,    -1,    -1,    -1,   796,
      -1,    -1,    65,   383,   801,    -1,   386,    -1,    -1,    -1,
      -1,    -1,   667,   752,   669,    -1,    -1,   672,   815,    -1,
     675,    -1,     3,    -1,    -1,    -1,    -1,    90,    -1,   684,
     827,    94,   687,    -1,    15,    16,   280,    -1,    -1,   836,
      -1,    22,    -1,   840,   450,    -1,    -1,   844,    -1,    30,
      -1,    -1,    -1,    -1,   709,    36,    -1,    -1,    -1,    -1,
      41,    -1,    43,    -1,    -1,    46,   721,    -1,   723,    -1,
     450,    52,   727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     557,    -1,    63,    -1,   881,    -1,    -1,    -1,   151,    -1,
      -1,    72,    -1,    41,    -1,    -1,    -1,    28,    -1,    -1,
      81,    82,    -1,    84,    -1,    86,    -1,    88,    -1,    -1,
      91,    59,    -1,    -1,    95,    96,    -1,    98,    -1,   363,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,   926,
     927,   928,   929,    -1,    -1,    -1,    -1,    -1,    -1,   383,
     617,   796,   386,    74,    -1,    -1,   801,    78,   945,   946,
     394,   557,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
     815,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   827,    -1,    -1,   156,    -1,   557,    -1,    -1,
      -1,   836,    -1,    -1,    -1,   840,    -1,    -1,    -1,   844,
     667,    -1,   669,   124,    -1,   672,   127,   128,   675,   130,
     131,    -1,    -1,    -1,   135,    -1,   450,   684,    -1,    -1,
     687,   617,   143,    -1,    -1,    -1,    -1,   280,    -1,   150,
     151,   152,    -1,   171,    -1,    -1,   881,    -1,    -1,    -1,
      -1,    -1,   709,    -1,    -1,    -1,    -1,   617,    -1,    -1,
      -1,   189,   190,   191,   721,    -1,   723,    -1,   196,    -1,
     727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   667,    28,   669,    -1,    -1,   672,    -1,    -1,   675,
      -1,   926,   927,   928,   929,    -1,    -1,    -1,   684,    -1,
     228,   687,    -1,   231,    -1,    -1,    -1,   667,    -1,   669,
     945,   946,   672,    -1,    -1,   675,    -1,    -1,    -1,    -1,
     363,    -1,   250,   709,   684,    -1,    -1,   687,    74,    -1,
      -1,    -1,    78,   557,    -1,   721,    -1,   723,    -1,   796,
     383,   727,   270,   386,   801,    -1,    -1,    -1,    -1,   709,
      -1,   394,    -1,    -1,    -1,    -1,    -1,    -1,   815,    -1,
      -1,   721,   108,   723,    -1,    -1,    -1,   727,    -1,    -1,
     827,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   836,
     126,   127,   128,   840,   130,   131,   132,   844,   134,   135,
     136,   137,    -1,   617,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    -1,   150,   151,   152,   450,    -1,    -1,
     796,    -1,    -1,    -1,    -1,   801,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   881,    -1,    -1,    -1,    -1,   815,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,
      -1,   827,    -1,   667,    -1,   669,    -1,    -1,   672,    -1,
     836,   675,    -1,    -1,   840,   815,    -1,    -1,   844,    -1,
     684,    -1,   390,   687,    -1,    -1,    -1,   827,    -1,   926,
     927,   928,   929,    -1,    -1,    -1,   836,    -1,    -1,    -1,
     840,    -1,    -1,    -1,   844,   709,    -1,    -1,   945,   946,
      -1,    -1,    -1,    -1,    -1,   881,    -1,   721,    -1,   723,
      -1,     1,    -1,   727,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   557,    -1,    -1,    -1,    -1,    19,
      -1,   881,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    19,    34,    35,    -1,    23,    -1,    39,
     926,   927,   928,   929,    44,    -1,    32,    47,    34,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,   945,
     946,    47,    62,    -1,    64,    -1,    66,    -1,    68,    69,
      -1,    71,   796,    73,   617,    75,    62,   801,    -1,    -1,
      66,    -1,    68,    69,    -1,    71,    -1,    73,    -1,    -1,
      -1,   815,    -1,    -1,    94,    -1,    -1,    97,    -1,    -1,
     100,    -1,    -1,   827,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    97,   836,   541,   542,    -1,   840,   545,   546,    -1,
     844,    -1,    -1,    -1,   667,    -1,   669,    -1,    -1,   672,
     558,    -1,   675,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   684,    -1,    -1,   687,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,   155,    -1,   881,    -1,    -1,
      -1,    -1,    -1,   149,    -1,    -1,   709,    -1,    -1,   155,
      -1,   599,    -1,    -1,    -1,    -1,    -1,    -1,   721,    -1,
     723,    -1,    -1,    -1,   727,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   926,   927,   928,   929,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      16,   945,   946,    -1,    -1,    -1,    22,    -1,    -1,   657,
     658,    -1,    -1,    -1,    30,    -1,   664,    -1,   666,    -1,
      36,    -1,    -1,   671,    -1,    41,   674,    43,    -1,   677,
      46,    -1,    -1,   796,    -1,    -1,    52,    -1,   801,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,   815,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   827,    81,    82,    -1,    84,   717,
      86,    -1,    88,   836,    -1,    91,    -1,   840,    -1,    95,
      -1,   844,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   881,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
     156,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,   926,   927,   928,   929,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,   829,   945,   946,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,   874,   875,    -1,   877,
     878,   142,    -1,   144,    -1,   146,   147,   148,    -1,    -1,
      -1,   152,    -1,   154,    -1,   156,   894,   158,   159,    -1,
      -1,    -1,   900,    -1,   902,    -1,   904,    -1,     0,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    43,    44,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,   980,    75,    76,    77,    -1,    79,    80,    81,
      82,    83,    84,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    -1,    97,    98,    99,   100,    -1,
      -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,
     152,    -1,   154,    -1,   156,     1,   158,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    17,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    43,    44,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,
      -1,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    -1,    88,    89,    -1,    91,    92,    93,    94,    95,
      -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,   105,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,   149,   150,   151,   152,    -1,   154,   155,
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
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,   149,
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
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      28,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    74,    -1,    -1,    63,
      78,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,    -1,    -1,    -1,   135,    -1,    -1,
     124,   125,   126,   127,   142,   143,    -1,    -1,   132,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,    -1,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,
     148,    -1,    -1,    -1,   152,    -1,   154,    -1,   156,    -1,
     158,   159,     3,     4,     5,     6,     7,     8,    -1,    -1,
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
      -1,   152,    -1,   154,    -1,   156,    -1,   158,   159,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,    -1,   158,   159,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      47,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    69,    -1,    -1,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    -1,    91,    -1,    -1,    94,    95,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,   124,   125,   126,
     127,    -1,    10,    -1,    -1,   132,    -1,    15,    16,    17,
      -1,    -1,    -1,    -1,    22,   142,    -1,   144,    -1,   146,
     147,    -1,    30,    -1,    -1,   152,    -1,   154,    36,   156,
      -1,   158,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    -1,    84,    -1,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,   150,   151,   152,   153,    -1,    -1,   156,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    47,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      94,    95,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,
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
      91,    92,    93,    -1,    95,    96,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      85,    86,    -1,    88,    -1,    -1,    91,    92,    93,    -1,
      95,    96,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,   154,
      -1,   156,   157,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    92,    93,    -1,    95,    96,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,
      -1,    -1,    -1,   152,    -1,   154,    -1,   156,   157,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    42,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,   100,    -1,    -1,
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
      -1,    91,    92,    93,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    92,    93,
      -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,    -1,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    57,
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
      55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,
      85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,    -1,    -1,    -1,   152,    -1,   154,
      -1,   156,    -1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    35,    36,    37,    38,
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
      46,    47,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
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
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    92,
      93,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
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
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,   152,    -1,   154,   155,   156,    -1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,
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
      -1,   106,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
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
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,   144,    -1,   146,   147,   148,    -1,    -1,    -1,
     152,    -1,   154,    -1,   156,    -1,   158,     3,     4,     5,
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
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
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
      16,    -1,    -1,   152,    -1,    -1,    22,   156,    -1,   158,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      86,    -1,    88,    -1,    -1,    91,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,   142,    -1,   144,    12,
      -1,   147,    15,    16,    -1,    -1,   152,    -1,   154,    22,
     156,    -1,   158,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,   129,    -1,    -1,    -1,
     133,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,   144,    -1,    15,    16,   148,    -1,    -1,    -1,    -1,
      22,   154,    -1,   156,   157,   158,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    90,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
     102,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,     3,     4,     5,     6,     7,     8,
      -1,   139,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,   144,    22,    -1,   153,    -1,   149,   156,    -1,
     158,    30,   154,    -1,   156,    -1,   158,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,   144,    49,    50,    -1,    52,
      53,    -1,    55,    56,    -1,   154,    -1,   156,    -1,   158,
      63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,   144,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,   154,    -1,   156,    -1,   158,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,     1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    19,    -1,    91,    -1,    23,    24,    95,    -1,
      -1,    98,    -1,    -1,    -1,    32,    -1,    34,    35,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,     3,    66,
      -1,    68,    69,    -1,    71,    -1,    73,   144,    75,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,   156,
      -1,   158,    -1,    -1,    -1,    30,    -1,    94,    -1,    -1,
      97,    36,    -1,   100,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    15,    16,    -1,    -1,    72,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    81,    82,    30,    84,
      -1,    86,   149,    88,    36,    -1,    91,    -1,   155,    41,
      95,    43,    -1,    98,    46,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      72,    10,    -1,    -1,    -1,    -1,    15,    16,    17,    81,
      82,    -1,    84,    22,    86,    -1,    88,    -1,    -1,    91,
      -1,    30,    -1,    95,    -1,    -1,    98,    36,    -1,   154,
      -1,   156,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,
      -1,    10,    91,    -1,   156,    -1,    95,    -1,    17,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,    -1,    -1,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,    28,
      -1,   150,   151,   152,   153,    74,    -1,    -1,    -1,    78,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    74,   145,   146,   147,    78,
      -1,   150,   151,   152,    -1,    -1,    10,    -1,    -1,    -1,
     159,    -1,    -1,    17,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    28,   134,   135,   136,   137,   108,
      -1,   140,   141,   142,   143,    -1,   145,    -1,   147,    -1,
      -1,   150,   151,   152,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      74,   150,   151,   152,    78,    -1,    10,    -1,    -1,    -1,
     159,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      74,   145,   146,   147,    78,    -1,   150,   151,   152,    -1,
      -1,    10,    -1,    -1,    -1,   159,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,    74,   150,   151,   152,    78,
      -1,    10,    -1,    -1,    -1,   159,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    74,   145,   146,   147,    78,
      -1,   150,   151,   152,    -1,    -1,    10,    -1,   157,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      74,   150,   151,   152,    78,    -1,    -1,    -1,   157,    -1,
      -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,    74,   150,   151,   152,    78,
     154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      74,   150,   151,   152,    78,   154,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,    74,   150,   151,   152,    78,
     154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      74,   150,   151,   152,    78,   154,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,    74,   150,   151,   152,    78,
     154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      74,   150,   151,   152,    78,   154,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,    74,   150,   151,   152,    78,
     154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      74,   150,   151,   152,    78,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,    28,    29,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,    42,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    10,    -1,   150,   151,   152,    -1,
     154,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
      -1,    78,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    74,   126,
     127,   128,    78,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    10,    -1,   150,   151,   152,    -1,   154,    17,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    10,    -1,   150,   151,   152,    -1,   154,    17,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    74,   126,   127,   128,
      78,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      28,   150,   151,   152,    -1,   154,    -1,    74,    -1,    -1,
     108,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    28,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,   108,   140,   141,   142,   143,    74,   145,   146,   147,
      78,    -1,   150,   151,   152,    -1,   154,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,   154,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,    28,   135,   136,   137,
      -1,   108,   140,   141,   142,   143,    -1,    -1,    -1,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,   154,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    59,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    74,   145,   146,   147,    78,    -1,   150,   151,
     152,    -1,   154,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
     152,    74,    -1,    -1,    -1,    78,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,   104,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    59,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    74,   145,   146,   147,    78,    -1,   150,   151,   152,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    10,    42,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    28,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    74,   145,   146,   147,
      78,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
     108,    78,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,   108,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,   124,    74,   126,
     127,   128,    78,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    10,   149,   150,   151,   152,    -1,    -1,    17,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    10,   149,   150,   151,   152,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,    78,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    74,   126,   127,   128,
      78,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    10,
     149,   150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,
     108,    -1,    74,    -1,    -1,    -1,    78,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,   108,   145,   146,   147,
      -1,   149,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    74,   126,   127,   128,    78,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    10,    -1,   150,   151,
     152,    -1,    -1,    17,    -1,    -1,   107,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    10,    -1,   150,
     151,   152,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    78,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    74,   126,   127,   128,    78,   130,   131,   132,    -1,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,    -1,   150,   151,   152,    -1,
      -1,   104,    74,    -1,    -1,   108,    78,    -1,    10,    11,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    28,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,   108,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    74,   145,   146,   147,    78,   149,   150,   151,
     152,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    31,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    28,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    74,   145,   146,   147,    78,    -1,   150,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,   108,    78,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,   108,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,    -1,    -1,   124,    74,   126,   127,   128,    78,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    10,    -1,   150,
     151,   152,    -1,    -1,    17,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,    -1,
     150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    28,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    74,   145,   146,   147,
      78,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,
     108,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,   108,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    -1,   150,   151,   152,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,    -1,   140,   141,   142,   143,     1,   145,     3,
     147,    -1,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,     3,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    63,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    81,    82,    -1,
      84,    41,    86,    43,    88,    -1,    46,    91,    -1,    -1,
      -1,    95,    52,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,   156,    -1,   158
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   164,   165,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    25,    26,    27,    29,    30,    32,    34,    35,    36,
      37,    38,    39,    40,    41,    43,    44,    46,    48,    49,
      50,    51,    52,    53,    55,    56,    58,    62,    63,    65,
      66,    67,    68,    71,    72,    73,    76,    77,    79,    80,
      81,    82,    83,    84,    85,    86,    88,    89,    91,    92,
      93,    94,    95,    97,    98,    99,   103,   105,   108,   124,
     125,   126,   127,   132,   142,   144,   146,   147,   149,   152,
     154,   156,   158,   166,   167,   168,   169,   170,   172,   177,
     178,   179,   183,   185,   189,   190,   191,   192,   193,   194,
     196,   197,   198,   202,   203,   206,   207,   210,   211,   214,
     217,   218,   241,   245,   246,   266,   267,   268,   269,   270,
     271,   279,   280,   281,   282,   285,   286,   287,   288,   289,
     290,   291,   293,   294,   295,   296,   297,   149,   168,   282,
     104,   272,   273,     3,   180,    12,    37,    38,    40,    86,
     154,   158,   214,   266,   270,   280,   281,   282,   285,   287,
     288,   272,   282,    94,   180,   168,   260,   282,   168,   156,
     282,   283,     7,     9,    94,   187,   283,    64,   106,   195,
     282,   282,    23,    35,   245,   282,   282,   156,     3,    15,
      16,    22,    30,    36,    41,    43,    46,    52,    63,    72,
      81,    82,    84,    86,    88,    91,    95,    98,   182,   184,
     215,   156,   182,   247,   248,    29,   172,   188,   282,   282,
     282,   282,   282,   282,     7,   156,    35,   191,   191,   148,
     237,   259,   282,    92,    93,   149,   282,   284,   282,   188,
     282,   282,   282,    85,   156,   168,   282,   282,   172,   179,
     282,   285,   172,   179,   282,   182,   242,   282,   259,   282,
     282,   282,   282,   282,   282,   282,   282,     1,   155,   166,
     173,   259,    96,   133,   237,   261,   262,   284,   259,   282,
     292,    66,   168,    70,   171,   182,   182,    47,    69,   232,
      24,    64,    75,   100,   244,   149,   139,    10,    17,    28,
      74,    78,   108,   124,   126,   127,   128,   130,   131,   132,
     134,   135,   136,   137,   138,   140,   141,   142,   143,   145,
     146,   147,   150,   151,   152,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     153,   224,   156,   158,    74,    78,   156,   168,   149,   282,
     282,   282,   259,   154,   172,    42,   272,   242,   149,   129,
     149,   103,   183,   237,   263,   264,   265,   284,   242,   207,
     182,   156,   188,    29,    42,   188,    29,    42,   104,   188,
     275,    33,    59,   149,    87,   172,   224,   263,   168,   156,
     226,    96,   156,   182,   249,   250,     1,   128,   254,    42,
     129,   168,   188,   188,   263,   182,   129,   149,   282,   282,
     149,   154,   188,   156,   263,   149,   199,   149,   199,   149,
     109,   243,   149,   149,    33,    59,   188,   149,   155,   155,
     166,   129,   155,   282,   129,   157,   129,   157,    42,   129,
     159,   275,   107,   129,   159,     7,    54,   128,   208,   154,
     219,    75,   247,   282,   282,   282,   282,   195,   282,   195,
     282,   282,   282,   282,   282,    16,    98,   282,   282,    30,
      52,    94,   183,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   284,
     284,   284,   284,   284,   263,   263,   195,   282,   195,   282,
      24,    42,    75,   100,   274,   277,   278,   282,   297,    29,
      42,    29,    42,    87,    42,   159,   195,   282,   188,   149,
     282,   282,   109,   157,   129,   149,   182,    42,   259,    40,
     282,   195,   282,    40,   282,   195,   282,   156,   188,   152,
     174,   176,   282,   174,   175,   168,   282,    31,   282,   157,
      24,    42,    45,    61,    64,    75,    94,   167,   227,   228,
     229,   230,   216,   250,   129,   157,    16,    30,    43,    63,
      81,    82,    86,    98,   158,   186,   255,   267,   109,   251,
     282,   248,   157,   237,   282,     1,   204,   263,   157,    18,
     200,   255,   267,   129,   176,   175,   155,   157,   157,   261,
     157,   261,   195,   282,   159,   168,   282,   159,   282,   159,
     282,   182,   259,   154,     1,   182,   212,   213,    24,    64,
      75,    94,   221,   231,   149,   149,   149,   149,   149,   149,
     157,   159,    42,    75,   129,   157,   266,    74,    74,    40,
     195,   282,    40,   195,   282,   195,   282,   272,   272,   149,
     237,   284,   265,   208,   282,   157,   282,    29,   188,    29,
     188,   282,    29,   188,   275,    29,   188,   275,   276,   277,
     129,   149,    11,   149,    31,    31,   168,    87,   172,    42,
      75,   229,   129,   157,   156,   182,    24,    64,    75,    94,
     233,   157,   250,   254,     1,   259,    57,   284,   155,    60,
     101,   155,   205,   157,   156,   172,   182,   201,   242,   149,
     149,   159,   275,   159,   275,   168,   107,   154,     1,   209,
     155,   109,   129,   155,    75,   220,     1,     3,    10,    15,
      17,    22,    36,    41,    46,    52,    72,    84,    88,    91,
      95,   124,   125,   126,   127,   130,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   147,   150,
     151,   152,   153,   156,   181,   184,   222,   223,   224,   266,
     277,   254,   266,   266,   282,    29,    29,   282,    29,    29,
     159,   159,   188,   188,   154,   188,    87,    40,   282,    40,
     282,    87,    40,   282,   188,    40,   282,   188,   129,   157,
     282,   282,   168,   282,   168,    31,   182,   228,   250,   128,
     258,    75,   254,   251,   159,    42,   159,    29,   168,   259,
     201,   128,   172,    40,   168,   282,   159,    40,   168,   282,
     159,   282,     1,   155,   173,   155,    35,    68,    71,   155,
     167,   190,   282,   213,   233,   156,   225,   182,   282,   131,
     225,   225,   251,    87,    40,    40,    87,    40,    40,    40,
      40,     1,   209,   282,   282,   282,   282,   282,   282,   277,
      11,    31,   168,   258,   157,   158,   186,   237,   257,   267,
     133,   239,   251,   102,   240,   282,   255,   267,   168,   188,
     157,   282,   282,   168,   282,   168,   155,   155,   190,     1,
     128,   253,   227,   157,     3,    88,   223,   224,   282,   282,
     282,   282,   282,   282,   155,   155,    87,    87,    87,    87,
     282,   168,   251,   258,   159,   259,   237,   238,   282,   282,
     172,   189,   236,   159,   172,    87,    87,   158,   186,   252,
     267,    90,   234,   157,   225,   225,    87,    87,    87,    87,
      87,    87,   282,   282,   282,   282,   239,   251,   237,   256,
     257,   267,    42,   159,   267,   282,   282,     1,   159,   259,
     240,   282,   256,   257,   159,   252,   267,   159,   149,   235,
     236,   159,   252,   267,   256
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
     243,   243,   243,   244,   244,   244,   244,   244,   245,   246,
     246,   246,   247,   247,   248,   248,   249,   249,   249,   250,
     250,   250,   251,   251,   251,   252,   252,   252,   252,   252,
     252,   252,   253,   253,   253,   253,   253,   254,   254,   254,
     254,   254,   255,   255,   255,   255,   256,   256,   256,   257,
     257,   257,   257,   257,   258,   258,   258,   258,   258,   259,
     259,   259,   259,   260,   260,   261,   261,   261,   262,   262,
     263,   263,   264,   264,   265,   265,   265,   265,   266,   266,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   267,   267,   267,   267,   268,   268,   268,   268,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   269,   270,   271,   271,   271,   271,   271,
     271,   271,   271,   272,   272,   273,   274,   274,   275,   276,
     276,   277,   277,   277,   278,   278,   278,   278,   278,   278,
     279,   279,   280,   281,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     283,   283,   284,   284,   284,   285,   285,   285,   285,   286,
     286,   287,   287,   287,   288,   288,   288,   288,   289,   289,
     289,   289,   290,   290,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   293,   293,   293,   293,   293,   293,   293,   293,   293,
     293,   294,   294,   294,   294,   294,   294,   295,   295,   295,
     295,   296,   296,   296,   296,   297,   297,   297,   297,   297,
     297,   297
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
       0,     2,     2,     1,     1,     1,     2,     1,     4,     0,
       1,     1,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     3,     2,     4,     3,     3,
       4,     3,     0,     2,     2,     2,     1,     0,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     2,     2,     2,     2,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     4,     1,     3,     4,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     3,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     3,     3,     2,     2,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     3,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
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
#line 470 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 5464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 476 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 533 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 539 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 540 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 585 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5728 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5782 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5788 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5794 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5800 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5806 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5812 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5824 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5830 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 5836 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5842 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5860 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "bool"; }
#line 5872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "int"; }
#line 5878 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "uint"; }
#line 5884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "real"; }
#line 5890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "imag"; }
#line 5896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "complex"; }
#line 5902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "string"; }
#line 5908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "sync"; }
#line 5914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "single"; }
#line 5920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "owned"; }
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "shared"; }
#line 5932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 686 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "borrowed"; }
#line 5938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "unmanaged"; }
#line 5944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "domain"; }
#line 5950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "index"; }
#line 5956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "locale"; }
#line 5962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 5968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 5974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 5980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 5986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 5992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 5998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_syncvar"; }
#line 6016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_singlevar"; }
#line 6022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_owned"; }
#line 6028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_shared"; }
#line 6034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_borrowed"; }
#line 6040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 713 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_unmanaged"; }
#line 6046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 714 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_domain"; }
#line 6052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 715 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_index"; }
#line 6058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 720 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 6064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 735 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 737 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 738 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 754 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 759 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 777 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 778 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 6228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 782 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 788 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 794 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 800 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 806 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 813 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 826 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 827 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 828 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 829 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 830 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6339 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 832 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 834 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 836 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 841 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 844 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 846 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 854 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 880 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 896 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 906 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 916 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 956 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 957 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 966 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 971 "chapel.ypp" /* yacc.c:1661  */
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
#line 6628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 987 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 994 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1011 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1015 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6674 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1022 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1027 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1037 "chapel.ypp" /* yacc.c:1661  */
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
#line 6724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1063 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1070 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6769 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1099 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1106 "chapel.ypp" /* yacc.c:1661  */
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
#line 6829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1136 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1141 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1145 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1150 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 6871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 7003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 7009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 7015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1190 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 7039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 7045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 7057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 7069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 7075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1197 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 7081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 7087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7129 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1233 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1234 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1238 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 7189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1239 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1240 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 7201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1241 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 7207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1244 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1245 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1246 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1250 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1251 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1252 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 7255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1253 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7261 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1254 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7267 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1255 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7273 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1259 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1266 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 7309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1268 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 7315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1269 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 7321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 7327 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 7333 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1277 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 7339 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7345 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7357 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1294 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7363 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1299 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7369 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1304 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7381 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7399 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1314 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1319 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1348 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1350 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_PARAM); }
#line 7464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST); }
#line 7470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_REF_VAR); }
#line 7476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(FLAG_CONST, FLAG_REF_VAR); }
#line 7482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pflagset) = buildVarDeclFlags(); }
#line 7488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1365 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pflagset)->insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), (yyvsp[-2].pflagset), (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1373 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1396 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1398 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1400 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1405 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1415 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1416 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1417 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1423 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1425 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7619 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7625 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1441 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1448 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1449 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1450 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1458 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7709 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7743 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7761 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7773 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7785 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1539 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1540 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7839 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7845 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1545 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7851 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7857 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1551 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7869 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1556 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7881 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7887 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7893 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1566 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7899 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7911 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7917 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7923 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7935 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7941 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7953 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7959 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1603 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7965 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7971 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7977 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7989 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8043 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8067 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1644 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1651 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1657 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1663 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1669 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 8148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1717 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1726 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1731 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1735 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1739 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1745 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1746 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8220 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8226 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8232 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8238 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8244 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1761 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8256 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1782 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1784 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1788 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8292 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8298 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1794 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8304 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1796 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8310 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1800 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1801 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8322 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1806 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8340 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1828 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1829 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8352 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1830 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1834 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1835 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1836 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1837 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1846 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8406 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1852 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8412 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8424 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8430 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8436 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8442 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8454 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1880 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1884 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1892 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8584 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1898 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8590 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1899 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1900 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1901 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1902 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1903 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1904 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1905 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1906 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1910 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1911 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1912 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 544:
#line 1913 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8662 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 545:
#line 1914 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8668 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 546:
#line 1915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8674 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 547:
#line 1919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8680 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 548:
#line 1920 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 549:
#line 1921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 550:
#line 1922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 551:
#line 1926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 552:
#line 1927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 553:
#line 1928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 554:
#line 1929 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 555:
#line 1934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8728 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 556:
#line 1935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 557:
#line 1936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 558:
#line 1937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 559:
#line 1938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 560:
#line 1939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 561:
#line 1940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8768 "bison-chapel.cpp" /* yacc.c:1661  */
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
