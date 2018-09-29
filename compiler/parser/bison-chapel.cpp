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
    TFOR = 291,
    TFORALL = 292,
    TFORWARDING = 293,
    TIF = 294,
    TIMAG = 295,
    TIN = 296,
    TINDEX = 297,
    TINLINE = 298,
    TINOUT = 299,
    TINT = 300,
    TITER = 301,
    TLABEL = 302,
    TLAMBDA = 303,
    TLET = 304,
    TLOCAL = 305,
    TMINUSMINUS = 306,
    TMODULE = 307,
    TNEW = 308,
    TNIL = 309,
    TNOINIT = 310,
    TON = 311,
    TONLY = 312,
    TOTHERWISE = 313,
    TOUT = 314,
    TOVERRIDE = 315,
    TOWNED = 316,
    TPARAM = 317,
    TPLUSPLUS = 318,
    TPRAGMA = 319,
    TPRIMITIVE = 320,
    TPRIVATE = 321,
    TPROC = 322,
    TPROTOTYPE = 323,
    TPUBLIC = 324,
    TREAL = 325,
    TRECORD = 326,
    TREDUCE = 327,
    TREF = 328,
    TREQUIRE = 329,
    TRETURN = 330,
    TSCAN = 331,
    TSELECT = 332,
    TSERIAL = 333,
    TSHARED = 334,
    TSINGLE = 335,
    TSPARSE = 336,
    TSTRING = 337,
    TSUBDOMAIN = 338,
    TSYNC = 339,
    TTHEN = 340,
    TTHROW = 341,
    TTHROWS = 342,
    TTRY = 343,
    TTRYBANG = 344,
    TTYPE = 345,
    TUINT = 346,
    TUNDERSCORE = 347,
    TUNION = 348,
    TUNMANAGED = 349,
    TUSE = 350,
    TVAR = 351,
    TWHEN = 352,
    TWHERE = 353,
    TWHILE = 354,
    TWITH = 355,
    TYIELD = 356,
    TZIP = 357,
    TALIAS = 358,
    TAND = 359,
    TASSIGN = 360,
    TASSIGNBAND = 361,
    TASSIGNBOR = 362,
    TASSIGNBXOR = 363,
    TASSIGNDIVIDE = 364,
    TASSIGNEXP = 365,
    TASSIGNLAND = 366,
    TASSIGNLOR = 367,
    TASSIGNMINUS = 368,
    TASSIGNMOD = 369,
    TASSIGNMULTIPLY = 370,
    TASSIGNPLUS = 371,
    TASSIGNSL = 372,
    TASSIGNSR = 373,
    TBAND = 374,
    TBNOT = 375,
    TBOR = 376,
    TBXOR = 377,
    TCOLON = 378,
    TCOMMA = 379,
    TDIVIDE = 380,
    TDOT = 381,
    TDOTDOT = 382,
    TDOTDOTDOT = 383,
    TEQUAL = 384,
    TEXP = 385,
    TGREATER = 386,
    TGREATEREQUAL = 387,
    THASH = 388,
    TLESS = 389,
    TLESSEQUAL = 390,
    TMINUS = 391,
    TMOD = 392,
    TNOT = 393,
    TNOTEQUAL = 394,
    TOR = 395,
    TPLUS = 396,
    TQUESTION = 397,
    TSEMI = 398,
    TSHIFTLEFT = 399,
    TSHIFTRIGHT = 400,
    TSTAR = 401,
    TSWAP = 402,
    TASSIGNREDUCE = 403,
    TIO = 404,
    TLCBR = 405,
    TRCBR = 406,
    TLP = 407,
    TRP = 408,
    TLSBR = 409,
    TRSBR = 410,
    TNOELSE = 411,
    TUPLUS = 412,
    TUMINUS = 413
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

#line 427 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 433 "bison-chapel.cpp" /* yacc.c:358  */
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
    setupError(__FILE__, __LINE__, 3);

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

#line 507 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   14960

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  129
/* YYNRULES -- Number of rules.  */
#define YYNRULES  543
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  999

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   413

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
     155,   156,   157,   158
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   447,   447,   452,   453,   459,   460,   465,   466,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   504,
     507,   509,   515,   516,   517,   521,   522,   535,   536,   537,
     542,   543,   548,   553,   558,   562,   569,   574,   578,   583,
     587,   588,   589,   593,   597,   599,   601,   603,   605,   607,
     609,   611,   613,   615,   617,   619,   621,   623,   625,   627,
     632,   633,   637,   641,   642,   646,   647,   651,   652,   656,
     657,   658,   659,   660,   661,   662,   663,   667,   668,   672,
     673,   674,   675,   679,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   708,   714,   720,   726,   733,   743,   747,   748,   749,
     750,   751,   753,   755,   757,   762,   765,   766,   767,   768,
     769,   770,   774,   775,   779,   780,   781,   785,   786,   790,
     793,   795,   800,   801,   805,   807,   809,   816,   826,   836,
     846,   859,   864,   869,   877,   878,   883,   884,   886,   891,
     907,   914,   923,   931,   935,   942,   943,   948,   953,   947,
     978,   984,   991,   997,  1003,  1014,  1020,  1013,  1052,  1056,
    1061,  1065,  1073,  1074,  1078,  1079,  1080,  1081,  1082,  1083,
    1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,
    1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,  1116,
    1117,  1118,  1122,  1123,  1127,  1131,  1132,  1133,  1137,  1139,
    1141,  1143,  1145,  1150,  1151,  1155,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1163,  1167,  1168,  1169,  1170,  1171,  1172,
    1176,  1177,  1181,  1182,  1183,  1184,  1185,  1186,  1190,  1191,
    1194,  1195,  1199,  1200,  1204,  1206,  1211,  1212,  1216,  1217,
    1221,  1222,  1226,  1228,  1230,  1235,  1248,  1265,  1266,  1268,
    1273,  1281,  1289,  1297,  1306,  1316,  1317,  1318,  1322,  1323,
    1331,  1333,  1338,  1340,  1342,  1347,  1349,  1351,  1358,  1359,
    1360,  1365,  1367,  1369,  1373,  1377,  1379,  1383,  1391,  1392,
    1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1401,  1406,
    1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,
    1436,  1440,  1444,  1452,  1459,  1460,  1461,  1465,  1467,  1473,
    1475,  1477,  1482,  1483,  1484,  1485,  1486,  1487,  1488,  1489,
    1490,  1491,  1492,  1498,  1499,  1500,  1501,  1505,  1506,  1510,
    1511,  1512,  1516,  1517,  1521,  1522,  1526,  1527,  1531,  1532,
    1533,  1534,  1538,  1542,  1543,  1554,  1555,  1556,  1557,  1558,
    1559,  1560,  1562,  1564,  1566,  1568,  1570,  1572,  1574,  1576,
    1578,  1580,  1585,  1587,  1589,  1591,  1593,  1595,  1597,  1599,
    1601,  1603,  1605,  1607,  1609,  1616,  1622,  1628,  1634,  1643,
    1653,  1661,  1662,  1663,  1664,  1665,  1666,  1667,  1668,  1673,
    1674,  1678,  1682,  1683,  1687,  1691,  1692,  1696,  1700,  1704,
    1711,  1712,  1713,  1714,  1715,  1716,  1720,  1721,  1726,  1731,
    1739,  1740,  1741,  1742,  1743,  1744,  1745,  1746,  1747,  1749,
    1751,  1753,  1755,  1757,  1759,  1761,  1766,  1767,  1770,  1771,
    1772,  1775,  1776,  1777,  1778,  1789,  1790,  1794,  1795,  1796,
    1800,  1801,  1802,  1810,  1811,  1812,  1813,  1817,  1818,  1819,
    1820,  1821,  1822,  1823,  1824,  1825,  1826,  1830,  1838,  1839,
    1843,  1844,  1845,  1846,  1847,  1848,  1849,  1850,  1851,  1852,
    1853,  1854,  1855,  1856,  1857,  1858,  1859,  1860,  1861,  1862,
    1863,  1864,  1865,  1869,  1870,  1871,  1872,  1873,  1874,  1878,
    1879,  1880,  1881,  1885,  1886,  1887,  1888,  1893,  1894,  1895,
    1896,  1897,  1898,  1899
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
  "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFOR", "TFORALL",
  "TFORWARDING", "TIF", "TIMAG", "TIN", "TINDEX", "TINLINE", "TINOUT",
  "TINT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS",
  "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE",
  "TOUT", "TOVERRIDE", "TOWNED", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TREAL",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE",
  "TUINT", "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP",
  "TLSBR", "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "opt_prototype", "block_stmt",
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_stmt",
  "require_stmt", "assignment_stmt", "opt_ident", "ident", "opt_string",
  "do_stmt", "return_stmt", "class_level_stmt", "private_decl",
  "forwarding_stmt", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "defer_stmt", "try_stmt", "catch_stmt_ls", "catch_stmt",
  "catch_expr", "throw_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
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
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "simple_type", "type_level_expr",
  "for_expr", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "forall_intent_clause", "forall_intent_ls", "intent_expr",
  "shadow_var_prefix", "io_expr", "new_expr", "let_expr", "expr",
  "opt_expr", "opt_try_expr", "lhs_expr", "fun_expr", "call_expr",
  "dot_expr", "parenthesized_expr", "literal_expr", "assoc_expr_ls",
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
     405,   406,   407,   408,   409,   410,   411,   412,   413
};
# endif

#define YYPACT_NINF -859

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-859)))

#define YYTABLE_NINF -495

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -859,   112,  2961,  -859,    15,  -859,  -859,  -859,  -859,  -859,
    -859,  4190,   107,   114,  8859,  -859,   107,  8859,   147,   114,
    4190,  8859,  4190,   102,  -859,  8859,  6244,  7437,  8859,  7589,
    8859,   111,  -859,   132,   114,  -859,    73,  8097,  8859,  8859,
    -859,  8859,  -859,  8859,  8859,   301,   165,   961,  1014,  -859,
    8224,  7031,  8859,  8097,  8859,  8859,   242,   170,  4190,  8859,
    8986,  8986,   114,  -859,  8859,  8224,  8859,  8859,  -859,  -859,
    8859,  -859,  -859, 10891,  8859,  8859,  -859,  8859,  -859,  -859,
    3278,  6523,  8224,  -859,  4038,  -859,  -859,   270,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,   114,  -859,   114,   310,    49,
    -859,  -859,  -859,   379,   212,  -859,  -859,  -859,  -859,   222,
     252,   230,   257,   268, 14737,  2349,   -52,   280,   281,  -859,
    -859,  -859,  -859,  -859,  -859,    29,  -859,  -859, 14737,   293,
    4190,  -859,   289,  -859,  8859,  8859,  8859,  8859,  8859,  8224,
    8224,    47,  -859,  -859,  -859,  -859,   324,   226,  -859,  -859,
     308, 13162,   114,   316,  -859,    -9, 14737,   366,  7183, 14737,
    -859,   236,  -859,   114,   247,  -859,   114,   322,    25, 12597,
   12520,  -859,  -859,  -859, 13110, 11881,  7183,  7183,  4190,   326,
      31,   108,    28,  -859,  4190,  -859,  -859, 12745,   587,   324,
   12745,   324,   587,  -859,  7183,  7716,  -859,  -859,   114,  -859,
      95, 14737,  8859,  8859,  -859, 14737,   325, 12958,  -859, 12745,
     324, 14737,   327,  7183,  -859, 14737, 13379,  -859,  -859, 13465,
    2489,  -859,  -859, 13517,   375,   339,   324,   103, 13247, 12745,
   13597,   240,   362,   587,   240,   587,   106,  -859,  -859,  3430,
     -26,  -859,  8859,  -859,   -32,   -31,  -859,    63, 13649,    13,
     484,  -859,  -859,   445,   377,   351,  -859,  -859,  -859,    14,
      73,    73,    73,  -859,  8859,  8859,  8859,  8859,  8351,  8351,
    8859,  8859,  8859,  8859,  9113,  8859,    81, 10891,  8859,  8859,
    8859,  8859,  8859,  8859,  8859,  8859,  8859,  8859,  8859,  8859,
    8859,  8859,  8859,  7843,  7843,  7843,  7843,  7843,  7843,  7843,
    7843,  7843,  7843,  7843,  7843,  7843,  7843,  7843,  7843,  7183,
    7183,  8351,  8351,  6904,  -859,  -859, 13299, 13431, 13729,    36,
    3582,  -859,  8351,    25,   380,  -859,  8859,  -859,  8859,   399,
    -859,   352,   385,  -859,  -859,   382,   114,   477,  8224,  -859,
    4342,  8351,  -859,  4494,  8351,   370,  -859,    25,  9240,  8859,
    -859,  4190,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,   495,  8859,   378,   383,  -859,   115,
    -859,  -859,    31,  -859,   403,   384,  -859,  9367,   427,  8859,
      73,  -859,  -859,  -859,   389,  -859,  8224,  -859, 14737, 14737,
    -859,    30,  -859,  7183,   390,  -859,   517,  -859,   517,  -859,
    9494,   420,  -859,  -859,  9240,  8859,  -859,  -859,  -859,  -859,
    -859,  7970,  -859, 11739,  6650,  -859,  6777,  -859,  8351,  5837,
    3126,   395,  8859,  6091,  -859,  -859,   114,  8224,   404,   439,
     285,    73,   128,   221,   259,   264, 13029,  2066,  2066,   337,
    -859,   337,  -859,   337, 11639,   842,  1036,  1161, 11226, 11226,
     324,   240,  -859,  -859,  -859,   362, 12787,   337,  1328,  1328,
    2066,  1328,  1328,  2191,   240, 12787, 14814,  2191,   587,   587,
     240,   402,   410,   414,   416,   417,   419,   422,   423,   424,
     433,   434,   436,   442,   447,   448,   449,   435,   441,  -859,
     337,  -859,   337,    55,  -859,  -859,  -859,    16,  -859,   114,
   14776,   229,  9621,  8351,  9748,  8351,  8859,  8351, 11410,   107,
   13781,  -859,  -859, 14737, 13815,  7183,  -859,  7183,  -859,   377,
    8859,   105,  8859, 14737,    32, 12816,  8859, 14737,    20, 12668,
    6904,  -859,   454,   474,   458, 13867,   474,   465,   578, 13947,
    4190, 12887,  -859,  -859,    58,  -859,  -859,  -859,  -859,  -859,
    -859,   540,   109,  -859,    78,  -859,   343,   457,    31,   108,
    8859,   102,  8859,  8859,  8859,  8859,  8859,  5710,  -859,   286,
    7310,  -859, 14737,  -859,  -859,  -859, 14737,   460,    33,   463,
    -859,    52,  -859,  -859,   272,   114,   475,   476,  -859,  -859,
    -859,  -859,  -859,  -859,    65,  2151,  -859,  -859, 14737,  4190,
   14737,  -859, 13999,   467,   497,  1518,   471,   518,   -16,  -859,
     551,  -859,  -859,  -859,  -859,  2786,   277,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  6904,  -859,     9,  8351,  8351,  8859,   597, 14033,  8859,
     599, 14165,   478, 11515,    25,    25,  -859,  -859,  -859,  -859,
     479, 12745,  -859, 11952,  4646,  -859,  4798,  -859, 12023,  4950,
    -859,    25,  5102,  -859,    25,   117,  -859,  8859,  -859,  8859,
    -859,  4190,  8859,  -859,  4190,   603,  -859,  -859,   114,   679,
    -859,    31,   509,   564,  -859,  -859,  -859,    66,  -859,  -859,
     427,   485,    69,  -859,  -859,  -859,  5254,  8224,  -859,  -859,
    -859,   114,  -859,   516,   308,  -859,  -859,  -859,  5406,   489,
    5558,   490,  -859,  8859,  3734,   492,  1812,  -859,  8859,   114,
    -859,  -859,   343,  -859,  -859,  -859,   114,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -859,  -859,  8859,   520,   522,
     504,   504,  -859,  -859,  -859,   427,   191,   200, 14204,  9875,
   10002, 14243, 10129, 10256, 10383, 10510,  -859,  -859,  2009,  -859,
    4190,  8859, 14737,  8859, 14737,  4190,  8859, 14737,  -859,  8859,
   14737,  -859,  6904,  -859, 14343, 14737,  -859, 14737,   627,  4190,
     509,  -859,   507,  8478,   116,  -859,   563,  -859,  -859,  8351,
   11318,  4190,  -859,    56,   512,  8859,  -859,  8859,  -859, 14737,
    4190,  8859,  -859, 14737,  4190, 14737,   118,  -859,  3886,  -859,
    6396,   560,   560,  -859,  1056,  -859, 14737,  -859,   239,  -859,
   11810, 11464,   115,  -859,  -859,  -859,  8859,  8859,  8859,  8859,
    8859,  8859,  8859,  8859,   511,  2382, 13947, 12094, 12165, 13947,
   12236, 12307,  -859,  8859,  4190,  -859,   427,   509,  8859,   102,
    8859,  8859,  8859,  8859,  8859,  5964,  -859,  -859,   342,  8224,
    -859,  -859,  8859,     4, 11597,  -859,   488,   366,  -859,   308,
   14737, 12378,  -859, 12449,  -859,  -859,  -859,  -859,  -859, 10637,
     579,  -859,  -859,   504,   504,   244, 14382, 14421, 14521, 14560,
   14599, 14699,  -859,  -859,  4190,  4190,  4190,  4190, 14737,  -859,
    -859,   116,  8605,    88,  -859,  -859, 14737, 14737,  -859,  -859,
    -859, 10510,  -859,  4190,  4190,  8859,   102,  8859,  8859,  8859,
    8859,  8859,  2714,  -859,   256,  -859,   563,  -859,  -859,  -859,
    8859,  8859,  8859,  8859,  8859,  8859, 13947, 13947, 13947, 13947,
    -859,  -859,  -859,  -859,  -859,   349,  8351, 11012,   601, 13947,
   13947,   514, 10764,   110,    92, 11668,  -859,  -859,  -859,  -859,
     294, 11107,  -859,  -859,  -859,  8732,  -859,   374,  -859
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   487,   488,   489,   490,
     491,     0,   429,    80,     0,   161,   429,     0,   296,    80,
       0,     0,     0,     0,   171,   466,   297,     0,     0,   295,
       0,     0,   181,   383,     0,   177,     0,     0,     0,     0,
     420,     0,   184,     0,     0,     0,     0,   295,   295,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,     0,     0,     0,   539,   541,
       0,   542,   543,   465,     0,     0,   540,   537,    89,   538,
       0,     0,     0,     4,     0,     5,     9,    45,    10,    11,
      12,    14,   382,    22,    13,    90,    96,    15,    17,    16,
      19,    20,    21,    18,    95,     0,    93,     0,   455,     0,
      97,    94,    98,     0,   471,   386,   451,   452,   388,   385,
       0,     0,   456,   457,     0,   387,     0,   472,   473,   474,
     450,   390,   389,   453,   454,     0,    38,    24,   396,     0,
       0,   430,     0,    81,     0,     0,     0,     0,     0,     0,
       0,   455,   471,   385,   456,   457,   398,   387,   472,   473,
       0,   429,     0,     0,   135,     0,   367,     0,   374,   467,
     182,   490,   103,     0,     0,   183,     0,     0,     0,     0,
       0,   296,   297,   102,     0,     0,   374,   374,     0,     0,
       0,     0,     0,   298,     0,    86,    32,     0,   525,   448,
       0,   399,   526,     7,   374,   297,    92,    91,   275,   364,
       0,   363,     0,     0,    87,   470,     0,     0,    35,     0,
     401,   391,     0,   374,    36,   397,     0,   142,   138,     0,
     387,   142,   139,     0,   287,     0,   400,     0,   363,     0,
       0,   528,   464,   524,   527,   523,     0,    47,    50,     0,
       0,   369,     0,   371,     0,     0,   370,     0,   363,     0,
       0,     6,    46,     0,   164,     0,   261,   260,   185,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   374,
     374,     0,     0,     0,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,     0,    29,     0,   382,
     380,     0,   375,   376,   381,     0,     0,     0,     0,   112,
       0,     0,   111,     0,     0,     0,   118,     0,     0,    56,
      99,     0,   220,   227,   228,   229,   224,   226,   222,   225,
     223,   221,   231,   230,   128,     0,     0,     0,    30,   235,
     178,   302,     0,   303,   305,     0,   339,     0,   308,     0,
       0,    85,    31,    33,     0,   274,     0,    63,   468,   469,
      88,     0,    34,   374,     0,   149,   140,   136,   141,   137,
       0,   285,   282,    60,     0,    56,   105,    37,    49,    48,
      51,     0,   492,     0,     0,   483,     0,   485,     0,     0,
       0,     0,     0,     0,   496,     8,     0,     0,     0,     0,
     254,     0,     0,     0,     0,     0,   428,   520,   519,   522,
     530,   529,   534,   533,   516,   513,   514,   515,   460,   461,
     459,   503,   482,   481,   480,   462,   507,   518,   512,   510,
     521,   511,   509,   501,   506,   508,   517,   500,   504,   505,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   532,
     531,   536,   535,   440,   441,   443,   445,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   429,
     429,   108,   283,   368,     0,     0,   393,     0,   284,   164,
       0,     0,     0,   404,     0,     0,     0,   410,     0,     0,
       0,   119,   538,    59,     0,    52,    57,     0,   127,     0,
       0,     0,   392,   384,   248,   245,   246,   247,   251,   252,
     253,   243,     0,   236,     0,   244,   262,     0,   306,     0,
     335,   332,   336,   338,   333,   334,   337,     0,   331,   451,
       0,   300,   449,   299,   479,   366,   365,     0,     0,     0,
     394,     0,   143,   289,   451,     0,     0,     0,   493,   458,
     484,   372,   486,   373,     0,     0,   495,   124,   414,     0,
     498,   497,     0,     0,   165,     0,     0,   175,     0,   172,
     258,   255,   256,   259,   186,     0,     0,   291,   290,   292,
     294,    64,    71,    72,    73,    68,    70,    78,    79,    66,
      69,    67,    65,    75,    74,    76,    77,   477,   478,   442,
     444,     0,   431,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,   378,   379,   377,
       0,     0,   126,     0,     0,   110,     0,   109,     0,     0,
     116,     0,     0,   114,     0,     0,   435,     0,   100,     0,
     101,     0,     0,   130,     0,   132,   249,   250,     0,   243,
     234,     0,   352,   263,   266,   265,   267,     0,   304,   307,
     308,     0,     0,   309,   310,   151,     0,     0,   150,   153,
     395,     0,   144,   147,     0,   286,    61,    62,     0,     0,
       0,     0,   125,     0,     0,     0,   295,   170,     0,   173,
     169,   257,   262,   217,   215,   196,   199,   197,   198,   209,
     200,   213,   205,   203,   216,   204,   202,   207,   212,   214,
     201,   206,   210,   211,   208,   218,   219,     0,   194,     0,
     232,   232,   192,   293,   433,   308,   471,   471,     0,     0,
       0,     0,     0,     0,     0,     0,   107,   106,     0,   113,
       0,     0,   403,     0,   402,     0,     0,   409,   117,     0,
     408,   115,     0,   434,    54,    53,   129,   419,   131,     0,
     352,   237,     0,     0,   308,   264,   280,   301,   343,     0,
     494,     0,   155,     0,     0,     0,   145,     0,   122,   416,
       0,     0,   120,   415,     0,   499,     0,    39,     0,   159,
     297,   295,   295,   157,   295,   167,   176,   174,     0,   195,
       0,     0,   235,   188,   189,   437,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   295,   407,     0,     0,   413,
       0,     0,   436,     0,     0,   134,   308,   352,   358,   355,
     359,   361,   356,   357,   360,     0,   354,   362,   451,   276,
     240,   238,     0,     0,     0,   341,   451,   156,   154,     0,
     148,     0,   123,     0,   121,    41,    40,   168,   328,     0,
     268,   193,   194,   232,   232,     0,     0,     0,     0,     0,
       0,     0,   160,   158,     0,     0,     0,     0,    55,   133,
     239,   308,   344,     0,   277,   279,   278,   281,   272,   273,
     179,     0,   146,     0,     0,   324,   321,   325,   327,   322,
     323,   326,     0,   320,   451,   269,   280,   190,   191,   233,
       0,     0,     0,     0,     0,     0,   406,   405,   412,   411,
     242,   241,   346,   347,   349,   451,     0,   494,   451,   418,
     417,     0,   312,     0,     0,     0,   348,   350,   317,   315,
     451,   494,   270,   187,   271,   344,   316,   451,   351
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -859,  -859,  -859,    -1,  -376,  1979,  -859,  -859,  -859,   361,
     -67,  -265,   255,   261,  -859,  -859,   311,   652,  2345,  -859,
      17,  -827,  -689,   -43,  -859,  -859,  -859,   -19,  -859,  -859,
    -859,   452,  -859,   -42,  -859,  -859,  -859,  -859,  -859,   510,
     149,  -102,  -859,  -859,  -859,   -48,   989,  -859,  -859,  -859,
    -859,  -859,  -859,  -859,  -859,  -158,  -612,  -741,  -859,  -157,
      -3,   136,  -859,  -859,  -859,   -40,  -859,  -859,  -284,    22,
    -859,  -227,  -251,  -859,  -140,  -859,   680,  -859,  -226,   318,
    -859,  -368,  -678,  -797,  -859,  -529,  -403,  -591,  -858,  -724,
     -44,  -859,    45,  -859,   -97,  -859,   184,   567,  -859,  -327,
    -859,  -859,  1087,  -859,     0,  -859,  -859,  -228,  -859,  -538,
    -859,  -859,  1291,  1441,    -2,   691,   902,   804,  -859,  1696,
    1753,  -859,  -859,  -859,  -859,  -859,  -859,  -859,  -312
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   248,    84,   548,    86,    87,   263,    88,
     249,   543,   547,   544,    89,    90,    91,   142,    92,   174,
     196,    93,    94,    95,    96,    97,    98,   662,    99,   100,
     101,   406,   592,   724,   102,   103,   588,   719,   104,   105,
     438,   736,   106,   107,   618,   619,   151,   189,   566,   109,
     110,   440,   742,   624,   769,   770,   375,   853,   380,   562,
     563,   564,   565,   625,   268,   707,   956,   993,   940,   209,
     935,   890,   893,   111,   235,   411,   112,   113,   192,   193,
     384,   385,   581,   953,   910,   388,   578,   973,   887,   814,
     250,   165,   254,   255,   341,   342,   343,   152,   115,   116,
     117,   118,   153,   120,   140,   141,   507,   357,   685,   508,
     509,   121,   154,   155,   215,   175,   344,   157,   126,   158,
     159,   129,   130,   259,   131,   132,   133,   134,   135
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     124,    83,   686,   561,   206,   207,   210,   593,   178,   138,
     386,   511,   156,   771,   567,   161,   160,     5,   124,   166,
     124,   237,   334,   169,   169,   179,   180,   184,   185,   431,
     854,   587,   817,   345,     5,   197,   198,   199,   257,   200,
     710,   201,   202,   442,   443,   444,   445,   845,   211,   679,
     217,   219,   220,   221,   194,     5,   225,   226,   229,   233,
     579,   674,   236,   238,   239,   240,   939,   386,   241,   389,
     218,   242,   243,   244,   974,   245,     5,   517,   211,    51,
     258,     5,   124,   594,     5,   194,   876,   441,  -152,   376,
     377,   716,   424,   426,   546,   266,   649,   855,   421,   696,
     319,   321,   320,   253,   428,   322,   329,   394,   739,   386,
     819,   462,     3,   774,  -329,   336,   267,     5,  -243,   987,
     355,   425,   427,   381,   775,   422,   404,  -152,   650,   976,
     717,   697,   387,  -329,   337,   740,   891,   433,   124,   554,
     651,  -329,   138,   326,   327,   328,   225,   211,   258,  -329,
     546,   517,   390,   931,   330,   907,   555,   939,   136,   556,
     429,   333,  -329,   355,  -329,   355,   190,    51,   434,   652,
     330,   463,   957,   958,   557,   330,   845,   558,   816,    45,
     396,  -152,   330,   382,   718,   989,   124,   429,   559,   387,
     340,   518,   124,   429,   996,   349,   352,   356,   930,  -476,
     709,  -476,   330,   169,   721,   560,   330,   139,   340,   340,
     398,   399,   429,  -329,   392,   626,  -329,   393,   430,   396,
     728,   580,   497,   498,   820,   190,   340,   396,   511,   396,
     701,   387,  -329,   699,   429,   992,   402,   162,   397,   914,
     908,   802,   330,   977,   889,   340,   413,   124,   420,   136,
     423,  -329,   390,   971,   168,   -84,   416,   418,   672,   450,
     452,   136,   700,   186,   872,   991,    15,  -243,   277,   905,
     803,   627,   446,   447,   448,   449,   451,   453,   454,   455,
     456,   457,   460,   461,   187,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   655,   499,   501,   531,   322,   589,   -84,   203,   620,
     681,   684,   278,   519,  -318,  -439,   279,   204,    49,   500,
     502,   510,   223,   561,  -438,   222,  -318,  -330,   124,   -84,
     520,  -319,   534,   812,   523,   538,   524,  -318,   262,   511,
      63,   340,   340,  -319,  -439,   390,   211,   621,   533,   535,
     521,   537,   539,  -438,  -319,  -422,   545,   545,   622,   549,
     844,  -330,   909,   284,   628,  -421,   286,   703,   699,  -311,
     289,   228,   232,   551,   541,   623,   729,   731,  -475,   274,
    -475,  -311,  -318,   390,  -330,   124,   986,   582,   390,  -318,
     277,  -330,  -311,   614,   586,   273,  -288,   959,   195,  -319,
    -426,   390,   629,   269,   998,   704,  -319,   630,   124,   604,
    -330,  -427,   545,   545,   195,  -288,   705,   895,   585,   586,
     773,   227,   231,  -424,  -423,   340,   605,   586,   608,  -330,
     610,   612,   325,   706,   278,   211,  -330,  -311,   279,  -330,
     616,   270,     5,   585,  -311,   323,   253,  -353,   253,  -313,
     286,   585,   271,  -425,  -345,   725,   156,   236,   330,   335,
     284,  -313,  -476,   286,  -476,   338,  -353,   289,   400,   601,
    -353,   603,  -313,  -345,   348,   272,   561,  -345,   379,   403,
     410,   281,   412,   282,   283,   284,   888,   285,   286,   844,
     511,   435,   289,   896,   657,  -353,   660,   436,   295,   296,
     437,   439,  -345,   299,   525,   526,   300,   301,   302,   527,
     533,   658,   537,   661,   549,   663,   608,  -313,   530,   664,
     665,   331,   540,   522,  -313,   528,   550,   568,   671,  -340,
     673,   552,   580,   712,   678,   591,   553,   569,   510,   195,
     195,   195,   584,   590,   595,   631,   374,   667,   124,   340,
     609,   675,   677,   632,   615,   680,   683,   633,   195,   634,
     635,   195,   636,  -340,   554,   637,   638,   639,   156,   114,
     201,   220,   221,   225,   236,   258,   640,   641,   114,   642,
     195,   555,   954,   181,   556,   643,  -340,   114,   647,   114,
     644,   645,   646,  -340,    25,   205,   648,   -58,   687,   557,
     195,   688,   558,    32,   260,   975,  -180,   124,   690,   691,
     708,   715,  -340,   559,   978,   277,   720,   734,   726,   727,
      42,   396,   737,   738,   741,   114,   779,  -180,   782,   788,
     560,  -340,   813,   784,   809,   450,   499,   815,  -340,   825,
     818,  -340,  -342,   839,   830,   834,  -382,   114,   851,   510,
     975,   114,   451,   500,   778,   990,   852,   781,   874,   278,
     877,   892,   922,   279,   997,   899,   955,   838,   975,   988,
     597,   163,   792,   823,   794,   596,  -342,   797,   670,   824,
     800,   786,   787,   408,   346,   804,   865,   805,   789,   124,
     807,   847,   124,   913,   195,   915,   811,   698,   798,  -342,
     994,   801,   848,   554,   970,   984,  -342,   114,   583,   183,
     284,   669,   285,   286,   124,   211,   170,   289,   195,     0,
     555,     0,     0,   556,   296,  -342,   829,     0,   833,     0,
       0,   835,   124,   302,     0,     0,   846,     0,   557,     0,
       0,   558,     0,    45,  -342,     0,     0,     0,     0,     0,
       0,  -342,   559,     0,  -342,   114,     0,     0,     0,     0,
       0,   114,     0,     0,     0,   850,     0,     0,     0,   560,
       0,     0,     0,     0,     0,     0,     0,   792,   794,     0,
     797,   800,   829,   833,     0,     0,     0,     0,   866,   867,
       0,   868,     0,   869,   870,     0,     0,   871,   206,   207,
     510,     0,     0,     0,     0,     0,   125,   124,     0,     0,
       0,   124,     0,     0,     0,   125,   114,   894,   608,   124,
       0,     0,     0,   900,   125,   901,   125,     0,   124,   903,
       0,     0,   124,     0,     0,   886,   124,   420,   169,     0,
     898,   933,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   866,   916,   917,   869,   918,   919,
     920,   921,   125,     0,   230,   230,     0,     0,     0,     0,
     277,   928,   124,     0,     0,     0,   156,     0,   201,   220,
     221,   225,   236,   258,   125,     0,     0,   936,   125,     0,
     937,     0,     0,     0,     0,   195,   195,   114,     0,   195,
     195,     0,     0,     0,     0,     0,     0,   124,   983,     0,
       0,   934,   695,     0,   278,     0,     0,   114,   279,     0,
     114,     0,   966,   967,   968,   969,     0,     0,   114,     0,
     124,     0,     0,     0,     0,     0,     0,     0,     0,   833,
       0,   979,   980,   156,   125,   201,   220,   221,   225,   236,
     258,     0,   722,   216,   972,     0,     0,     0,   966,   967,
     968,   969,   979,   980,     0,   284,     0,   285,   286,     0,
       0,     0,   289,     0,   985,   608,     0,     0,     0,   296,
     124,     0,     0,   256,   181,     0,   300,   301,   302,   608,
       0,   108,   125,   833,     0,    25,   205,   114,   125,   972,
     108,     0,     0,     0,    32,     0,     0,  -180,     0,   108,
       0,   108,     0,   -44,     0,     0,     0,   972,     0,     0,
       0,    42,     0,     0,     0,   195,   195,     0,  -180,   -44,
       0,     0,   195,     0,   374,     0,     0,   181,     0,   374,
       0,     0,   195,     0,     0,   195,     0,   108,    25,   205,
       0,     0,     0,   125,     0,     0,     0,    32,     0,     0,
    -180,     0,     0,     0,   277,     0,   -43,     0,     0,   108,
       0,     0,     0,   108,    42,    15,   653,     0,     0,    18,
       0,  -180,   -43,     0,     0,   826,     0,     0,    24,   119,
      25,   840,     0,     0,    29,     0,     0,     0,   119,    32,
       0,     0,  -180,     0,     0,     0,     0,   119,   278,   119,
       0,     0,   279,     0,     0,     0,    42,   114,     0,     0,
     260,     0,   841,  -180,     0,   842,     0,    49,     0,   108,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,    62,     0,     0,    63,
       0,     0,     0,     0,   125,   281,     0,   125,   283,   284,
       0,   285,   286,     0,     0,   125,   289,   119,     0,     0,
       0,   119,     0,   296,     0,     0,   114,   108,     0,     0,
     300,   301,   302,   108,   195,     0,     0,     0,     0,   277,
       0,     0,   772,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   776,   777,     0,     0,     0,     0,   119,   374,   374,
       0,   374,   374,   278,   125,     0,     0,   279,   108,     0,
       0,   114,     0,   114,     0,     0,   114,     0,     0,   114,
       0,     0,     0,     0,   938,     0,     0,     0,   114,     0,
     942,   114,   374,     0,   374,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,     0,     0,     0,     0,
     281,   119,     0,   114,   284,     0,   285,   286,     0,     0,
       0,   289,     0,   122,     0,   114,     0,   114,   296,     0,
       0,   114,   122,     0,     0,   300,   301,   302,     0,     0,
       0,   122,     0,   122,     0,     0,     0,     0,     0,   108,
       0,     0,     0,     0,     0,     0,   256,     0,   256,     0,
       0,     0,     0,     0,     0,     0,   119,     0,     0,   108,
       0,     0,   108,     0,     0,   938,     0,     0,     0,   122,
     108,     0,     0,     0,   125,     0,   277,   114,     0,     0,
       0,     0,   114,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,     0,     0,   122,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,     0,     0,     0,     0,     0,     0,   114,     0,     0,
     278,   114,     0,     0,   279,   114,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,     0,   119,     0,   108,
       0,     0,     0,     0,     0,     0,     0,   668,     0,     0,
       0,   122,     0,     0,     0,     0,     0,   119,     0,     0,
     119,   114,     0,   123,     0,     0,     0,   281,   119,   282,
     283,   284,   123,   285,   286,   287,     0,     0,   289,     0,
       0,   123,     0,   123,   295,   296,     0,     0,     0,   299,
       0,     0,   300,   301,   302,     0,     0,     0,   125,   122,
     125,     0,   714,   125,     0,   122,   125,     0,     0,     0,
       0,   114,   114,   114,   114,   125,     0,     0,   125,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   114,     0,     0,     0,     0,     0,   119,     0,   735,
     125,   123,     0,     0,     0,   123,     0,     0,     0,     0,
       0,     0,   125,     0,   125,     0,     0,  -166,   125,   108,
     122,  -166,  -166,     0,     0,     0,     0,     0,     0,     0,
    -166,     0,  -166,  -166,     0,     0,  -166,     0,     0,     0,
       0,  -166,     0,     0,  -166,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -166,     0,
    -166,   123,  -166,     0,  -166,  -166,     0,  -166,     0,  -166,
       0,  -166,     0,     0,   125,     0,     0,     0,   108,   125,
       0,     0,     0,     0,     0,     0,     0,     0,  -166,     0,
       0,  -166,     0,   125,  -166,     0,     0,     0,     0,     0,
       0,   122,     0,     0,     0,   125,     0,     0,     0,   123,
       0,     0,     0,     0,   125,   123,     0,   119,   125,     0,
       0,   122,   125,     0,   122,     0,     0,     0,     0,     0,
       0,     0,   122,     0,     0,     0,     0,     0,     0,     0,
       0,  -166,     0,   108,     0,   108,     0,     0,   108,  -166,
       0,   108,     0,     0,     0,     0,     0,     0,   125,     0,
     108,     0,     0,   108,     0,     0,     0,     0,     0,     0,
     123,     0,     0,     0,     0,     0,   119,     0,   127,     0,
       0,     0,     0,     0,     0,   108,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,   127,   108,   127,   108,
       0,   122,     0,   108,     0,     0,     0,     0,   125,   125,
     125,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,   125,     0,
       0,     0,     0,     0,   127,   128,     0,     0,     0,     0,
       0,   119,     0,   119,   128,     0,   119,     0,     0,   119,
       0,   123,     0,   128,     0,   128,   127,     0,   119,   108,
     127,   119,     0,     0,   108,     0,     0,     0,     0,     0,
       0,   123,     0,     0,   123,     0,     0,     0,   108,     0,
       0,     0,   123,   119,     0,     0,     0,     0,     0,     0,
     108,   128,     0,     0,     0,   119,     0,   119,     0,   108,
       0,   119,     0,   108,     0,     0,     0,   108,     0,     0,
       0,    15,     0,   128,     0,    18,   127,   128,     0,     0,
       0,   122,     0,     0,    24,     0,    25,   840,     0,     0,
      29,     0,     0,     0,     0,    32,     0,     0,  -180,     0,
       0,     0,     0,   108,     0,     0,     0,     0,     0,     0,
       0,   123,    42,     0,     0,     0,    45,   119,   841,  -180,
       0,   842,   119,    49,   127,     0,     0,     0,     0,     0,
     127,     0,     0,   128,     0,     0,   119,     0,     0,     0,
     122,     0,    62,     0,     0,    63,     0,     0,   119,     0,
       0,     0,     0,   108,   108,   108,   108,   119,     0,     0,
       0,   119,     0,     0,     0,   119,     0,     0,     0,     0,
       0,     0,   108,   108,     0,     0,     0,     0,     0,     0,
       0,   128,     0,     0,     0,   127,     0,   128,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,     0,
       0,   119,     0,   843,     0,   122,     0,   122,     0,     0,
     122,     0,     0,   122,     0,     0,     0,     0,     0,     0,
       0,    85,   122,     0,     0,   122,     0,     0,     0,     0,
     137,   123,     0,     0,     0,     0,     0,     0,     0,   164,
       0,   167,   128,     0,     0,     0,     0,   122,     0,     0,
     864,   119,   119,   119,   119,     0,     0,     0,     0,   122,
       0,   122,     0,     0,     0,   122,   127,     0,  -166,     0,
     119,   119,  -166,  -166,     0,     0,     0,   224,     0,     0,
       0,  -166,     0,  -166,  -166,     0,   127,  -166,     0,   127,
     123,     0,  -166,     0,     0,  -166,     0,   127,     0,    85,
       0,     0,     0,   261,     0,     0,     0,     0,     0,  -166,
       0,  -166,     0,  -166,     0,  -166,  -166,     0,  -166,     0,
    -166,   122,  -166,   128,     0,     0,   122,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,  -166,
     122,     0,  -166,   128,     0,  -166,   128,     0,     0,     0,
       0,     0,   122,     0,   128,   123,     0,   123,     0,   324,
     123,   122,     0,   123,     0,   122,   127,     0,     0,   122,
       0,     0,   123,     0,     0,   123,     0,     0,   278,     0,
       0,     0,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -166,     0,     0,     0,     0,   123,     0,     0,
    -166,   275,     0,     0,     0,   122,     0,   378,   276,   123,
     280,   123,     0,   391,     0,   123,     0,     0,     0,   277,
       0,     0,     0,   128,     0,   281,     0,   282,   283,   284,
       0,   285,   286,   287,     0,   288,   289,   290,   291,     0,
     293,   294,   295,   296,     0,   297,   298,   299,     0,     0,
     300,   301,   302,     0,     0,   122,   122,   122,   122,   277,
       0,     0,     0,   278,     0,     0,     0,   279,    85,     0,
       0,   123,     0,     0,   122,   122,   123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   127,     0,     0,     0,
     123,   355,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,   123,   278,     0,     0,     0,   279,     0,     0,
     281,   123,   282,   283,   284,   123,   285,   286,   287,   123,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,   128,     0,   127,   730,     0,     0,    85,
     281,     0,   282,   283,   284,   123,   285,   286,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,   296,   391,
       0,     0,   391,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,   128,     0,   143,   123,   123,   123,   123,     0,
     127,     0,   127,     0,     0,   127,     0,     0,   127,   188,
       0,   191,     0,     0,   123,   123,     0,   127,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,     0,    18,     0,   234,     0,   607,
       0,     0,   127,     0,    24,     0,    25,   840,     0,     0,
      29,     0,     0,     0,   127,    32,   127,   128,  -180,   128,
     127,     0,   128,     0,     0,   128,     0,     0,     0,     0,
       0,     0,    42,     0,   128,     0,    45,   128,   841,  -180,
     264,   842,   265,    49,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,     0,   128,
       0,     0,    62,     0,     0,    63,     0,     0,     0,     0,
       0,   128,     0,   128,     0,     0,   127,   128,     0,     0,
       0,   127,     0,     0,     0,     0,   317,   318,  -446,     0,
       0,  -475,     0,  -475,     0,   127,     0,   234,     0,     0,
       0,     0,     0,   339,     0,     0,     0,   127,   234,     0,
       0,   347,     0,     0,     0,    78,   127,     0,     0,   693,
     127,   339,   339,   923,   127,   383,     0,     0,     0,     0,
       0,     0,     0,   128,     0,     0,     0,     0,   128,   339,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,   128,     0,     0,     0,     0,     0,   339,     0,
     127,     0,     0,     0,   128,     0,     0,     0,     0,     0,
       0,     0,     0,   128,     0,     0,     0,   128,   732,     0,
       0,   128,     0,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,     0,     0,
       0,     0,     0,     0,   191,   191,   191,   191,     0,     0,
     127,   127,   127,   127,     0,     0,     0,   128,     0,     0,
       0,   464,     0,     0,     0,     0,   317,   318,     0,   127,
     127,  -475,     0,  -475,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   391,     0,   391,     0,     0,   391,     0,
       0,   391,     0,     0,   339,   339,     0,     0,     0,     0,
     806,     0,     0,   808,     0,     0,     0,   128,   128,   128,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,   822,   128,   128,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   828,     0,   832,
       0,     0,     0,    85,     0,   981,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   144,   383,     0,     0,
      14,     0,     0,     0,     0,   191,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,   339,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,   613,     0,     0,   617,     0,   191,     0,   875,     5,
       0,     0,     0,    54,    55,    56,   743,    57,   148,     0,
     897,     0,     0,   744,     0,     0,     0,     0,    64,   902,
       0,     0,     0,   904,     0,     0,     0,    85,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,   929,    76,    77,   208,     0,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   150,   982,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   362,   363,   364,   365,   366,   367,     0,     0,   368,
     369,   370,   371,   372,   373,   745,   746,   747,   748,   702,
       0,   749,     0,   383,     0,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,   760,     0,   761,     0,     0,
     762,   763,   764,   765,     0,   766,   723,     0,   767,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
     768,     0,     0,    11,    12,    13,     0,    14,     0,     0,
      15,    16,    17,     0,    18,  -295,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,     0,    31,    32,     0,    33,  -180,    34,    35,
      36,    37,    38,   -42,    39,    40,     0,    41,     0,     0,
       0,    42,    43,  -295,    44,    45,    46,    47,  -180,   -42,
      48,     0,    49,     0,  -295,    50,    51,     0,    52,    53,
      54,    55,    56,   810,    57,    58,   383,    59,     0,    60,
      61,    62,     0,     0,    63,    64,    65,  -295,     0,     0,
      66,     0,    67,     0,     0,    68,   723,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,   617,     0,     0,     0,    73,     0,
       0,   849,     0,     0,     0,     0,     0,    74,     0,    75,
       0,    76,    77,     0,    78,     0,     0,    79,     0,     0,
       0,    80,     0,    81,     0,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     0,     5,
       6,     7,     8,     9,    10,     0,  -494,     0,    11,    12,
      13,     0,    14,  -494,     0,    15,    16,    17,     0,    18,
    -295,    19,    20,    21,  -494,    22,    23,  -494,    24,     0,
      25,    26,    27,    28,    29,    30,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -295,    44,
       0,    46,    47,  -180,   -42,    48,   912,    49,  -494,  -295,
      50,    51,  -494,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -295,     0,     0,    66,     0,    67,     0,     0,
    -494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -494,    70,  -494,  -494,  -494,
    -494,  -494,  -494,  -494,     0,  -494,  -494,  -494,  -494,  -494,
    -494,  -494,  -494,  -494,    75,  -494,  -494,  -494,     0,    78,
    -494,  -494,  -494,     0,     0,     0,    80,  -494,    81,   246,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -295,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,    30,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -295,    44,    45,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -295,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -295,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
     208,    78,     0,     0,    79,     0,     0,     0,    80,   247,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -295,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,    30,
       0,     0,    31,    32,     0,    33,  -180,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -295,    44,    45,    46,    47,  -180,   -42,    48,
       0,    49,     0,  -295,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -295,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,   419,    81,   246,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -295,    44,    45,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -295,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -295,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,   247,    81,   836,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -295,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,    30,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -295,    44,    45,    46,
      47,  -180,   -42,    48,     0,    49,     0,  -295,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -295,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,   837,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -295,    44,
      45,    46,    47,  -180,   -42,    48,     0,    49,     0,  -295,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -295,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,   906,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -295,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,    30,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -295,    44,   260,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -295,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -295,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -295,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,    30,
       0,     0,    31,    32,     0,    33,  -180,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -295,    44,     0,    46,    47,  -180,   -42,    48,
       0,    49,     0,  -295,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -295,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,     0,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   532,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -295,    44,     0,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -295,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -295,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -295,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,   536,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -295,    44,     0,    46,
      47,  -180,   -42,    48,     0,    49,     0,  -295,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -295,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   791,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -295,    44,
       0,    46,    47,  -180,   -42,    48,     0,    49,     0,  -295,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -295,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -295,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,   793,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -295,    44,     0,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -295,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -295,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -295,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,   796,
       0,     0,    31,    32,     0,    33,  -180,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -295,    44,     0,    46,    47,  -180,   -42,    48,
       0,    49,     0,  -295,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -295,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,     0,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   799,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -295,    44,     0,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -295,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -295,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -295,    19,
      20,    21,     0,   821,    23,     0,    24,     0,    25,    26,
      27,    28,    29,    30,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -295,    44,     0,    46,
      47,  -180,   -42,    48,     0,    49,     0,  -295,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -295,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -295,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   827,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -295,    44,
       0,    46,    47,  -180,   -42,    48,     0,    49,     0,  -295,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -295,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -295,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,   831,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -295,    44,     0,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -295,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -295,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,   711,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   144,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   144,
      76,    77,   208,    14,     0,     0,    79,     0,     0,     0,
     149,     0,    81,     0,   150,     0,     0,    23,     0,     0,
       0,     0,     0,   145,   146,     0,   147,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   144,    76,    77,   208,
      14,     0,     0,    79,     0,     0,     0,   149,     0,    81,
       0,   150,   606,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   144,    76,    77,   208,    14,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   150,   932,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     147,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,    75,
       0,    76,    77,     0,     0,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,   611,     5,     6,     7,
       8,   171,    10,   172,     0,     0,   144,     0,     0,     0,
      14,     0,     0,   -83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
    -466,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,  -466,     0,     0,     0,   -83,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,   173,     0,     0,   -83,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,     0,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   150,     5,
       6,     7,     8,   171,    10,     0,     0,     0,   144,     0,
       0,     0,    14,     0,     0,   -83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,   145,   146,     0,   147,     0,     0,    31,     0,
       0,    33,  -466,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,  -466,     0,     0,     0,   -83,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     148,     0,     0,     0,     0,     0,   173,     0,     0,   -83,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   144,    76,    77,     0,    14,
       0,     0,    79,     0,     0,     0,   149,     0,    81,     0,
     150,     0,     0,    23,     0,     0,     0,     0,     0,   145,
     146,     0,   147,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   148,     0,     0,
       0,   212,   213,     0,     0,   251,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,   252,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   144,    76,    77,   208,    14,     0,     0,    79,
       0,     0,     0,   149,     0,    81,     0,   150,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,   212,   213,
       0,     0,   251,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   144,
      76,    77,   208,    14,     0,     0,    79,     0,     0,     0,
     149,     0,    81,   600,   150,     0,     0,    23,     0,     0,
       0,     0,     0,   145,   146,     0,   147,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   148,     0,     0,     0,   212,   213,     0,     0,   251,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   144,    76,    77,   208,
      14,     0,     0,    79,     0,     0,     0,   149,   503,    81,
     602,   150,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,   504,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,   505,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
     506,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   144,    76,    77,     0,    14,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   150,     0,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     147,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   148,     0,     0,     0,   212,
     213,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,    75,
       0,    76,    77,     0,   214,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   144,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,   145,
     146,     0,   147,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   148,     0,     0,
       0,   212,   213,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   144,    76,    77,   208,    14,     0,     0,    79,
       0,     0,     0,   149,     0,    81,     0,   150,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,   713,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,   212,   213,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   144,
      76,    77,     0,    14,     0,     0,    79,     0,     0,     0,
     149,     0,    81,     0,   150,     0,     0,    23,     0,     0,
       0,     0,     0,   145,   146,     0,   147,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,   176,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   177,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,    75,     0,    76,    77,     0,
       0,     0,     0,    79,     0,     0,     0,   149,     0,    81,
       0,   150,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   144,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,   182,   145,   146,     0,   147,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   144,    76,
      77,     0,    14,     0,     0,    79,     0,     0,     0,   149,
       0,    81,     0,   150,     0,     0,    23,     0,     0,     0,
       0,     0,   145,   146,     0,   147,     0,     0,    31,     0,
       0,    33,  -466,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,  -466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   144,    76,    77,     0,    14,
       0,     0,    79,     0,     0,     0,   149,     0,    81,     0,
     150,     0,     0,    23,     0,     0,     0,     0,     0,   145,
     146,     0,   147,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   148,     0,     0,
       0,   212,   213,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   144,    76,    77,     0,    14,     0,     0,    79,
       0,     0,     0,   149,     0,    81,     0,   150,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   144,
      76,    77,   208,    14,     0,     0,    79,     0,     0,     0,
     149,   598,    81,     0,   150,     0,   194,    23,     0,     0,
       0,     0,     0,   145,   146,     0,   147,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   144,    76,    77,     0,
      14,     0,     0,    79,     0,     0,     0,    80,     0,    81,
       0,   150,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   144,    76,    77,   208,    14,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   150,     0,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     147,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,   177,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     144,    76,    77,     0,   878,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     0,     0,   879,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
      31,     0,     0,    33,     0,     0,    35,    36,     0,    38,
       0,    39,    40,     0,     0,     0,     0,     0,     0,   880,
       0,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   881,   882,    56,
       0,    57,   883,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   884,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     5,     6,
       7,     8,     9,    10,    74,     0,    75,   144,    76,    77,
     208,    14,     0,     0,    79,     0,     0,     0,   149,     0,
      81,     0,   885,     0,     0,    23,     0,     0,     0,     0,
       0,   145,   146,     0,   147,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   144,    76,    77,   208,    14,     0,
       0,    79,     0,     0,     0,   149,     0,    81,     0,   885,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   863,     0,     0,    31,     0,     0,    33,     0,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   144,    76,    77,   208,    14,     0,     0,    79,     0,
       0,     0,   149,     0,    81,     0,   150,     0,     0,    23,
       0,     0,     0,     0,     0,   145,   146,     0,   147,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   144,    76,
      77,     0,    14,     0,     0,    79,     0,     0,     0,   149,
       0,    81,     0,   150,     0,     0,    23,     0,     0,     0,
       0,     0,   145,   146,     0,   147,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   144,    76,    77,     0,   458,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     0,
     150,     0,     0,    23,     0,     0,     0,     0,     0,   145,
     146,     0,   147,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   148,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   459,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   144,    76,    77,     0,    14,     0,     0,    79,
       0,     0,     0,   149,     0,    81,     0,   150,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   144,
      76,    77,     0,   570,     0,     0,   542,     0,     0,     0,
     149,     0,    81,     0,   150,     0,     0,   571,     0,     0,
       0,     0,     0,   145,   146,     0,   147,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,   572,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,    56,     0,
      57,   575,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   144,    76,    77,     0,
      14,     0,     0,    79,     0,     0,     0,   149,     0,    81,
       0,   577,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   144,    76,    77,     0,    14,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   577,     0,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     656,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     144,    76,    77,     0,    14,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     0,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   659,     0,     0,
      31,     0,     0,    33,     0,     0,    35,    36,     0,    38,
       0,    39,    40,     0,     0,     0,     0,     0,     0,    43,
       0,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    57,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     5,     6,
       7,     8,     9,    10,    74,     0,    75,   144,    76,    77,
       0,    14,     0,     0,    79,     0,     0,     0,   149,     0,
      81,     0,   150,     0,     0,    23,     0,     0,     0,     0,
       0,   145,   146,     0,   857,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   144,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,   149,     0,    81,     0,   150,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   858,     0,     0,    31,     0,     0,    33,     0,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   144,    76,    77,     0,    14,     0,     0,    79,     0,
       0,     0,   149,     0,    81,     0,   150,     0,     0,    23,
       0,     0,     0,     0,     0,   145,   146,     0,   860,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   144,    76,
      77,     0,    14,     0,     0,    79,     0,     0,     0,   149,
       0,    81,     0,   150,     0,     0,    23,     0,     0,     0,
       0,     0,   145,   146,     0,   861,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   144,    76,    77,     0,    14,
       0,     0,    79,     0,     0,     0,   149,     0,    81,     0,
     150,     0,     0,    23,     0,     0,     0,     0,     0,   145,
     146,     0,   862,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   148,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   144,    76,    77,     0,    14,     0,     0,    79,
       0,     0,     0,   149,     0,    81,     0,   150,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   863,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   144,
      76,    77,     0,   945,     0,     0,    79,     0,     0,     0,
     149,     0,    81,     0,   150,     0,     0,   946,     0,     0,
       0,     0,     0,   145,   146,     0,   147,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,   947,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   948,   949,    56,     0,
      57,   950,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   951,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   144,    76,    77,     0,
      14,     0,     0,    79,     0,     0,     0,   149,     0,    81,
       0,   952,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   144,    76,    77,     0,    14,     0,     0,
      79,     0,     0,     0,     0,     0,    81,     0,   952,     0,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     147,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   144,     0,     0,    74,    14,    75,
       0,     0,    77,     0,     0,     0,     0,    79,     0,     0,
       0,   149,    23,    81,     0,   150,     0,     0,   145,   146,
       0,   147,     0,     0,    31,     0,     0,    33,     0,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,  -344,     0,   144,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,    70,     0,     0,     0,  -344,    23,     0,     0,
    -344,     0,     0,   145,   146,     0,   147,     0,     0,    31,
      75,     0,    33,     0,   208,    35,    36,     0,    38,     0,
      39,    40,   149,     0,    81,  -344,   885,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -314,     0,     0,     0,    54,    55,    56,     0,
      57,   148,     0,     0,  -314,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,  -314,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    70,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   144,     0,
       0,     0,    14,     0,     0,    75,     0,     0,     0,     0,
    -314,     0,     0,     0,     0,     0,    23,  -314,     0,    81,
       0,   952,   145,   146,     0,   147,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     144,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
      31,     0,     0,    33,    75,     0,    35,    36,     0,    38,
       0,    39,    40,     0,     0,     0,   149,     0,    81,    43,
     150,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    57,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   144,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    70,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,    75,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     5,   149,     0,
      81,    43,   577,    44,   743,    46,     0,     0,     0,     0,
       0,   744,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,     0,
      70,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,    75,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    81,     0,   150,     0,     0,     0,     0,   362,
     363,   364,   365,   366,   367,     0,     0,   368,   369,   370,
     371,   372,   373,   745,   746,   747,   748,   278,     0,   749,
       0,   279,     0,   750,   751,   752,   753,   754,   755,   756,
     757,   758,   759,   760,     0,   761,     0,   275,   762,   763,
     764,   765,     0,   766,   276,     0,     0,     0,     0,   280,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,     0,     0,   281,     0,   282,   283,   284,     0,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,     0,   300,
     301,   302,     0,     0,     0,     0,     0,   277,     0,   278,
     785,     0,     0,   279,     0,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   278,     0,     0,     0,   279,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     278,   300,   301,   302,   279,     0,     0,     0,     0,   275,
       0,     0,   941,     0,     0,     0,   276,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   277,   288,   289,
     290,   291,   280,   293,   294,   295,   296,     0,   297,     0,
     299,     0,     0,   300,   301,   302,     0,   281,     0,   282,
     283,   284,     0,   285,   286,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,   297,   298,   299,
       0,   278,   300,   301,   302,   279,     0,     0,     0,     0,
     275,     0,     0,   995,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,   278,   300,   301,   302,   279,     0,     0,     0,
       0,   275,   599,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,   280,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,   278,   300,   301,   302,   279,     0,     0,
       0,     0,   275,   911,     0,     0,   361,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,   280,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   371,   372,   373,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,   278,   300,   301,   302,   279,     0,
       0,   330,     0,   275,     0,     0,     0,   790,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,   280,   362,   363,   364,
     365,   366,   367,     0,     0,   368,   369,   370,   371,   372,
     373,   281,     0,   282,   283,   284,     0,   285,   286,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,   297,   298,   299,     0,   278,   300,   301,   302,   279,
       0,     0,   330,     0,   275,     0,     0,     0,   795,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,   280,   362,   363,
     364,   365,   366,   367,     0,     0,   368,   369,   370,   371,
     372,   373,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,   278,   300,   301,   302,
     279,     0,     0,   330,     0,   275,     0,     0,     0,   924,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,   280,   362,
     363,   364,   365,   366,   367,     0,     0,   368,   369,   370,
     371,   372,   373,   281,     0,   282,   283,   284,     0,   285,
     286,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,     0,   297,   298,   299,     0,   278,   300,   301,
     302,   279,     0,     0,   330,     0,   275,     0,     0,     0,
     925,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,   280,
     362,   363,   364,   365,   366,   367,     0,     0,   368,   369,
     370,   371,   372,   373,   281,     0,   282,   283,   284,     0,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,   278,   300,
     301,   302,   279,     0,     0,   330,     0,   275,     0,     0,
       0,   926,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
     280,   362,   363,   364,   365,   366,   367,     0,     0,   368,
     369,   370,   371,   372,   373,   281,     0,   282,   283,   284,
       0,   285,   286,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,     0,   297,   298,   299,     0,   278,
     300,   301,   302,   279,     0,     0,   330,     0,   275,     0,
       0,     0,   927,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,   280,   362,   363,   364,   365,   366,   367,     0,     0,
     368,   369,   370,   371,   372,   373,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     278,   300,   301,   302,   279,     0,     0,   330,     0,   275,
       0,     0,     0,   943,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,   280,   362,   363,   364,   365,   366,   367,     0,
       0,   368,   369,   370,   371,   372,   373,   281,     0,   282,
     283,   284,     0,   285,   286,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,   297,   298,   299,
       0,   278,   300,   301,   302,   279,     0,     0,   330,     0,
     275,     0,     0,     0,   944,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   353,
       0,     0,     0,   280,   362,   363,   364,   365,   366,   367,
       0,   354,   368,   369,   370,   371,   372,   373,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,   278,   300,   301,   302,   279,     0,     0,   330,
       0,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
     355,     0,     0,     0,   280,   277,   350,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,     0,   300,   301,   302,     0,     0,   278,
     330,     0,     0,   279,     0,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,   682,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     278,   300,   301,   302,   279,     0,     0,   330,     0,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,   355,     0,
       0,     0,   280,   277,   194,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   281,     0,   282,
     283,   284,     0,   285,   286,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,   297,   298,   299,
       0,     0,   300,   301,   302,   277,     0,   278,   330,     0,
       0,   279,     0,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   676,     0,     0,     0,   280,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   278,
       0,     0,     0,   279,   281,     0,   282,   283,   284,     0,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,   278,   300,
     301,   302,   279,     0,     0,   330,     0,   275,     0,     0,
       0,     0,     0,     0,   276,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,   277,     0,   289,   290,   291,
     280,   293,   294,   295,   296,     0,     0,     0,   299,     0,
       0,   300,   301,   302,     0,   281,     0,   282,   283,   284,
       0,   285,   286,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,     0,   297,   298,   299,     0,   278,
     300,   301,   302,   279,     0,     0,   330,     0,   275,     0,
       0,     0,   694,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     278,   300,   301,   302,   279,     0,     0,   330,     0,   275,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,   280,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   281,     0,   282,
     283,   284,     0,   285,   286,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,   297,   298,   299,
       0,   278,   300,   301,   302,   279,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,   280,     0,     0,     0,     0,   277,     0,
       0,     0,     0,   358,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   359,   297,   298,
     299,     0,   275,   300,   301,   302,     0,     0,  -447,   276,
       0,     0,   278,     0,     0,     0,   279,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   280,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   282,   283,   284,   278,   285,   286,   287,   279,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,   360,   300,   301,   302,   275,     0,     0,
       0,     0,   139,     0,   276,     0,   280,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
     414,   281,     0,   282,   283,   284,     0,   285,   286,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,   297,   298,   299,   415,     0,   300,   301,   302,   275,
       0,     0,     0,     0,     0,     0,   276,     0,     0,   278,
       0,     0,     0,   279,     0,     0,     0,   277,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,   278,   285,   286,   287,   279,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,   275,
       0,   300,   301,   302,     0,     0,   276,     0,     0,     0,
       0,     0,     0,   280,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,   275,     0,   300,   301,   302,     0,     0,   276,     0,
       0,   278,     0,     0,     0,   279,     0,     0,     0,   277,
     514,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   515,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   276,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,   281,     0,
     282,   283,   284,   278,   285,   286,   287,   279,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,   405,   300,   301,   302,     0,   275,     0,     0,
       0,     0,     0,     0,   276,   280,     0,   278,     0,     0,
       0,   279,     0,     0,     0,   277,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   280,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,     0,   281,     0,   282,   283,   284,   278,
     285,   286,   287,   279,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,   275,   407,   300,
     301,   302,     0,     0,   276,     0,     0,     0,     0,     0,
       0,   280,     0,     0,     0,   277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,   275,
     409,   300,   301,   302,     0,     0,   276,     0,     0,   278,
       0,     0,     0,   279,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,   278,   285,   286,   287,   279,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,   275,
     417,   300,   301,   302,     0,     0,   276,     0,     0,     0,
       0,     0,   432,   280,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,   275,     0,   300,   301,   302,     0,     0,   276,     0,
       0,   278,     0,     0,     0,   279,     0,     0,     0,   277,
       0,     0,     0,     0,   516,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   276,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,     0,     0,     0,     0,   281,     0,
     282,   283,   284,   278,   285,   286,   287,   279,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,     0,   300,   301,   302,     0,   275,   689,     0,
       0,   139,     0,     0,   276,   280,     0,   278,     0,     0,
       0,   279,     0,     0,     0,   277,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   280,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,     0,   281,     0,   282,   283,   284,   278,
     285,   286,   287,   279,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,   275,   666,   300,
     301,   302,     0,     0,   276,     0,     0,     0,     0,     0,
       0,   280,     0,     0,     0,   277,     0,     0,   692,     0,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,   275,
       0,   300,   301,   302,     0,     0,   276,     0,     0,   278,
       0,     0,     0,   279,     0,     0,     0,   277,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     276,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   780,     0,     0,     0,   281,     0,   282,   283,
     284,   278,   285,   286,   287,   279,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
       0,   300,   301,   302,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   280,     0,   278,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   280,   297,   298,
     299,     0,     0,   300,   301,   302,     0,     0,     0,     0,
       0,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,   275,     0,   300,   301,   302,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   277,   783,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,   278,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,   280,
       0,   277,     0,     0,     0,     0,   278,     0,     0,     0,
     279,     0,     0,     0,   281,     0,   282,   283,   284,   856,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,   280,   300,
     301,   302,     0,     0,     0,   278,     0,     0,     0,   279,
       0,     0,     0,   281,     0,   282,   283,   284,   859,   285,
     286,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,     0,   297,   298,   299,     0,   280,   300,   301,
     302,     0,     0,   275,   873,     0,     0,     0,     0,     0,
     276,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,   277,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,     0,   300,   301,   302,
       0,     0,   275,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,   278,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,   280,     0,   277,
       0,     0,     0,     0,   278,     0,     0,     0,   279,     0,
       0,     0,   281,     0,   282,   283,   284,   960,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,   280,   300,   301,   302,
       0,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,   281,     0,   282,   283,   284,   961,   285,   286,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,   297,   298,   299,     0,   280,   300,   301,   302,     0,
       0,   275,     0,     0,     0,     0,     0,     0,   276,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,   277,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
     275,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,     0,   962,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,   277,     0,     0,
       0,     0,   278,     0,     0,     0,   279,     0,     0,     0,
     281,     0,   282,   283,   284,   963,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,   280,   300,   301,   302,     0,     0,
       0,   278,     0,     0,     0,   279,     0,     0,     0,   281,
       0,   282,   283,   284,   964,   285,   286,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,   280,   300,   301,   302,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   276,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,   277,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,     0,   300,   301,   302,     0,   275,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
       0,   278,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,     0,   965,     0,   275,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,   277,     0,     0,     0,     0,   278,
       0,     0,     0,   279,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,   280,   277,   300,   301,   302,     0,     0,   654,     0,
       0,     0,   279,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     280,   300,   301,   302,     0,     0,   278,     0,     0,     0,
     279,     0,     0,     0,     0,   281,     0,   282,   283,   284,
       0,   285,   286,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,     0,   297,   298,   299,   280,     0,
     300,   301,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   281,     0,   282,   283,   284,     0,   285,
     286,   287,     0,   288,   289,   290,   291,     0,   293,   294,
     295,   296,     0,   297,     0,   299,     0,     0,   300,   301,
     302
};

static const yytype_int16 yycheck[] =
{
       2,     2,   540,   379,    47,    48,    50,   410,    27,    11,
       1,   323,    14,   625,   382,    17,    16,     3,    20,    21,
      22,    65,   162,    25,    26,    27,    28,    29,    30,   257,
     771,     1,   710,   173,     3,    37,    38,    39,    82,    41,
     569,    43,    44,   269,   270,   271,   272,   736,    50,    29,
      52,    53,    54,    55,    29,     3,    58,    59,    60,    61,
     387,    29,    64,    65,    66,    67,   893,     1,    70,    41,
      53,    73,    74,    75,   932,    77,     3,    41,    80,    75,
      82,     3,    84,   410,     3,    29,   810,    73,    58,   186,
     187,    58,   124,   124,   359,    46,    41,   775,   124,    41,
     152,    72,   154,    81,    41,    76,   150,   204,   124,     1,
      41,    30,     0,   651,   105,   124,    67,     3,     3,   977,
     100,   153,   153,    92,   653,   151,   223,    97,    73,    41,
      97,    73,   123,   124,   143,   151,   814,   124,   140,    24,
     124,    75,   144,   145,   146,   147,   148,   149,   150,    41,
     415,    41,   124,   877,   150,   844,    41,   984,   143,    44,
     124,   161,   153,   100,    98,   100,   152,    75,   155,   153,
     150,    90,   913,   914,    59,   150,   865,    62,   707,    64,
     124,   151,   150,   152,   151,   982,   188,   124,    73,   123,
     168,   155,   194,   124,   991,   178,   179,   180,   876,   152,
     568,   154,   150,   205,   152,    90,   150,   100,   186,   187,
     212,   213,   124,   105,   197,   441,   150,   200,   155,   124,
     155,   105,   319,   320,   155,   152,   204,   124,   540,   124,
     152,   123,   124,   124,   124,   143,   219,    90,   143,   851,
       1,   124,   150,   155,   128,   223,   143,   249,   249,   143,
     252,   143,   124,   931,   152,    19,   239,   151,   153,   278,
     279,   143,   153,   152,   802,   155,    19,   152,    28,   151,
     153,   143,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   152,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,    72,   321,   322,   348,    76,   403,    71,     7,    24,
     538,   539,    72,   332,    75,   124,    76,   152,    71,   321,
     322,   323,   152,   699,   124,    83,    87,    41,   330,    93,
     332,    75,   351,   701,   336,   354,   338,    98,    68,   651,
      93,   319,   320,    87,   153,   124,   348,    62,   350,   351,
     333,   353,   354,   153,    98,   143,   358,   359,    73,   361,
     736,    75,   123,   123,   143,   143,   126,    24,   124,    75,
     130,    60,    61,   375,   357,    90,   604,   605,   152,   149,
     154,    87,   143,   124,    98,   387,   977,   389,   124,   150,
      28,   105,    98,   437,   396,   143,   124,   153,    37,   143,
     143,   124,   143,    24,   995,    62,   150,   143,   410,   428,
     124,   143,   414,   415,    53,   143,    73,   820,   396,   421,
     143,    60,    61,   143,   143,   403,   428,   429,   430,   143,
     432,   433,   143,    90,    72,   437,   150,   143,    76,   153,
       1,    62,     3,   421,   150,   152,   424,   105,   426,    75,
     126,   429,    73,   143,   105,   595,   458,   459,   150,   143,
     123,    87,   152,   126,   154,    99,   124,   130,   143,   424,
     128,   426,    98,   124,   152,    96,   852,   128,   152,   152,
     105,   119,   143,   121,   122,   123,   813,   125,   126,   865,
     802,     7,   130,   820,   513,   153,   515,    52,   136,   137,
     123,   150,   153,   141,   105,   153,   144,   145,   146,   124,
     512,   513,   514,   515,   516,   517,   518,   143,    41,   519,
     520,   160,   152,   143,   150,   143,    31,   124,   530,    41,
     532,   153,   105,   577,   536,    18,   153,   153,   540,   178,
     179,   180,   153,   153,   124,   143,   185,   525,   550,   527,
     155,   534,   535,   143,   150,   538,   539,   143,   197,   143,
     143,   200,   143,    75,    24,   143,   143,   143,   570,     2,
     572,   573,   574,   575,   576,   577,   143,   143,    11,   143,
     219,    41,   909,    23,    44,   143,    98,    20,   153,    22,
     143,   143,   143,   105,    34,    35,   155,   143,   124,    59,
     239,   143,    62,    43,    64,   932,    46,   609,   143,    31,
     153,   151,   124,    73,   941,    28,   153,   150,   143,   143,
      60,   124,   151,   105,    73,    58,    29,    67,    29,   150,
      90,   143,   123,   155,    31,   654,   655,    73,   150,   123,
     155,   153,    41,   151,   155,   155,   126,    80,   126,   651,
     977,    84,   654,   655,   656,   982,   152,   659,    31,    72,
     153,    98,   151,    76,   991,   153,    87,   734,   995,   155,
     415,    19,   674,   717,   676,   414,    75,   679,   529,   721,
     682,   664,   665,   231,   174,   687,   788,   689,   671,   691,
     692,   739,   694,   851,   333,   852,   699,   561,   681,    98,
     984,   684,   742,    24,   931,   956,   105,   140,   390,    29,
     123,   527,   125,   126,   716,   717,    25,   130,   357,    -1,
      41,    -1,    -1,    44,   137,   124,   728,    -1,   730,    -1,
      -1,   733,   734,   146,    -1,    -1,   738,    -1,    59,    -1,
      -1,    62,    -1,    64,   143,    -1,    -1,    -1,    -1,    -1,
      -1,   150,    73,    -1,   153,   188,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    -1,    -1,   767,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   779,   780,    -1,
     782,   783,   784,   785,    -1,    -1,    -1,    -1,   790,   791,
      -1,   793,    -1,   795,   796,    -1,    -1,   799,   841,   842,
     802,    -1,    -1,    -1,    -1,    -1,     2,   809,    -1,    -1,
      -1,   813,    -1,    -1,    -1,    11,   249,   819,   820,   821,
      -1,    -1,    -1,   825,    20,   827,    22,    -1,   830,   831,
      -1,    -1,   834,    -1,    -1,   813,   838,   838,   840,    -1,
     823,   885,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   856,   857,   858,   859,   860,   861,
     862,   863,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      28,   873,   874,    -1,    -1,    -1,   878,    -1,   880,   881,
     882,   883,   884,   885,    80,    -1,    -1,   889,    84,    -1,
     892,    -1,    -1,    -1,    -1,   534,   535,   330,    -1,   538,
     539,    -1,    -1,    -1,    -1,    -1,    -1,   909,   952,    -1,
      -1,   889,   551,    -1,    72,    -1,    -1,   350,    76,    -1,
     353,    -1,   924,   925,   926,   927,    -1,    -1,   361,    -1,
     932,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   941,
      -1,   943,   944,   945,   140,   947,   948,   949,   950,   951,
     952,    -1,   591,    51,   932,    -1,    -1,    -1,   960,   961,
     962,   963,   964,   965,    -1,   123,    -1,   125,   126,    -1,
      -1,    -1,   130,    -1,   976,   977,    -1,    -1,    -1,   137,
     982,    -1,    -1,    81,    23,    -1,   144,   145,   146,   991,
      -1,     2,   188,   995,    -1,    34,    35,   430,   194,   977,
      11,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    20,
      -1,    22,    -1,    52,    -1,    -1,    -1,   995,    -1,    -1,
      -1,    60,    -1,    -1,    -1,   664,   665,    -1,    67,    68,
      -1,    -1,   671,    -1,   673,    -1,    -1,    23,    -1,   678,
      -1,    -1,   681,    -1,    -1,   684,    -1,    58,    34,    35,
      -1,    -1,    -1,   249,    -1,    -1,    -1,    43,    -1,    -1,
      46,    -1,    -1,    -1,    28,    -1,    52,    -1,    -1,    80,
      -1,    -1,    -1,    84,    60,    19,   509,    -1,    -1,    23,
      -1,    67,    68,    -1,    -1,   724,    -1,    -1,    32,     2,
      34,    35,    -1,    -1,    38,    -1,    -1,    -1,    11,    43,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    20,    72,    22,
      -1,    -1,    76,    -1,    -1,    -1,    60,   550,    -1,    -1,
      64,    -1,    66,    67,    -1,    69,    -1,    71,    -1,   140,
      -1,    -1,    -1,    -1,   330,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    90,    -1,    -1,    93,
      -1,    -1,    -1,    -1,   350,   119,    -1,   353,   122,   123,
      -1,   125,   126,    -1,    -1,   361,   130,    80,    -1,    -1,
      -1,    84,    -1,   137,    -1,    -1,   609,   188,    -1,    -1,
     144,   145,   146,   194,   823,    -1,    -1,    -1,    -1,    28,
      -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   654,   655,    -1,    -1,    -1,    -1,   140,   867,   868,
      -1,   870,   871,    72,   430,    -1,    -1,    76,   249,    -1,
      -1,   674,    -1,   676,    -1,    -1,   679,    -1,    -1,   682,
      -1,    -1,    -1,    -1,   893,    -1,    -1,    -1,   691,    -1,
     899,   694,   901,    -1,   903,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
     119,   194,    -1,   716,   123,    -1,   125,   126,    -1,    -1,
      -1,   130,    -1,     2,    -1,   728,    -1,   730,   137,    -1,
      -1,   734,    11,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    20,    -1,    22,    -1,    -1,    -1,    -1,    -1,   330,
      -1,    -1,    -1,    -1,    -1,    -1,   424,    -1,   426,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   249,    -1,    -1,   350,
      -1,    -1,   353,    -1,    -1,   984,    -1,    -1,    -1,    58,
     361,    -1,    -1,    -1,   550,    -1,    28,   790,    -1,    -1,
      -1,    -1,   795,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,   809,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   821,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   830,    -1,    -1,
      72,   834,    -1,    -1,    76,   838,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   609,    -1,    -1,    -1,   330,    -1,   430,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   525,    -1,    -1,
      -1,   140,    -1,    -1,    -1,    -1,    -1,   350,    -1,    -1,
     353,   874,    -1,     2,    -1,    -1,    -1,   119,   361,   121,
     122,   123,    11,   125,   126,   127,    -1,    -1,   130,    -1,
      -1,    20,    -1,    22,   136,   137,    -1,    -1,    -1,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,   674,   188,
     676,    -1,   580,   679,    -1,   194,   682,    -1,    -1,    -1,
      -1,   924,   925,   926,   927,   691,    -1,    -1,   694,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     943,   944,    -1,    -1,    -1,    -1,    -1,   430,    -1,     1,
     716,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,   728,    -1,   730,    -1,    -1,    19,   734,   550,
     249,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      62,   140,    64,    -1,    66,    67,    -1,    69,    -1,    71,
      -1,    73,    -1,    -1,   790,    -1,    -1,    -1,   609,   795,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    93,    -1,   809,    96,    -1,    -1,    -1,    -1,    -1,
      -1,   330,    -1,    -1,    -1,   821,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,   830,   194,    -1,   550,   834,    -1,
      -1,   350,   838,    -1,   353,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,   674,    -1,   676,    -1,    -1,   679,   151,
      -1,   682,    -1,    -1,    -1,    -1,    -1,    -1,   874,    -1,
     691,    -1,    -1,   694,    -1,    -1,    -1,    -1,    -1,    -1,
     249,    -1,    -1,    -1,    -1,    -1,   609,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,   716,    -1,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,   728,    22,   730,
      -1,   430,    -1,   734,    -1,    -1,    -1,    -1,   924,   925,
     926,   927,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   943,   944,    -1,
      -1,    -1,    -1,    -1,    58,     2,    -1,    -1,    -1,    -1,
      -1,   674,    -1,   676,    11,    -1,   679,    -1,    -1,   682,
      -1,   330,    -1,    20,    -1,    22,    80,    -1,   691,   790,
      84,   694,    -1,    -1,   795,    -1,    -1,    -1,    -1,    -1,
      -1,   350,    -1,    -1,   353,    -1,    -1,    -1,   809,    -1,
      -1,    -1,   361,   716,    -1,    -1,    -1,    -1,    -1,    -1,
     821,    58,    -1,    -1,    -1,   728,    -1,   730,    -1,   830,
      -1,   734,    -1,   834,    -1,    -1,    -1,   838,    -1,    -1,
      -1,    19,    -1,    80,    -1,    23,   140,    84,    -1,    -1,
      -1,   550,    -1,    -1,    32,    -1,    34,    35,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,   874,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   430,    60,    -1,    -1,    -1,    64,   790,    66,    67,
      -1,    69,   795,    71,   188,    -1,    -1,    -1,    -1,    -1,
     194,    -1,    -1,   140,    -1,    -1,   809,    -1,    -1,    -1,
     609,    -1,    90,    -1,    -1,    93,    -1,    -1,   821,    -1,
      -1,    -1,    -1,   924,   925,   926,   927,   830,    -1,    -1,
      -1,   834,    -1,    -1,    -1,   838,    -1,    -1,    -1,    -1,
      -1,    -1,   943,   944,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,   249,    -1,   194,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,
      -1,   874,    -1,   151,    -1,   674,    -1,   676,    -1,    -1,
     679,    -1,    -1,   682,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     2,   691,    -1,    -1,   694,    -1,    -1,    -1,    -1,
      11,   550,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    22,   249,    -1,    -1,    -1,    -1,   716,    -1,    -1,
       1,   924,   925,   926,   927,    -1,    -1,    -1,    -1,   728,
      -1,   730,    -1,    -1,    -1,   734,   330,    -1,    19,    -1,
     943,   944,    23,    24,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    32,    -1,    34,    35,    -1,   350,    38,    -1,   353,
     609,    -1,    43,    -1,    -1,    46,    -1,   361,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    -1,
      71,   790,    73,   330,    -1,    -1,   795,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    90,
     809,    -1,    93,   350,    -1,    96,   353,    -1,    -1,    -1,
      -1,    -1,   821,    -1,   361,   674,    -1,   676,    -1,   140,
     679,   830,    -1,   682,    -1,   834,   430,    -1,    -1,   838,
      -1,    -1,   691,    -1,    -1,   694,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,   716,    -1,    -1,
     151,    10,    -1,    -1,    -1,   874,    -1,   188,    17,   728,
     104,   730,    -1,   194,    -1,   734,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   430,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,    -1,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    -1,   924,   925,   926,   927,    28,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,   249,    -1,
      -1,   790,    -1,    -1,   943,   944,   795,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   550,    -1,    -1,    -1,
     809,   100,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,   821,    72,    -1,    -1,    -1,    76,    -1,    -1,
     119,   830,   121,   122,   123,   834,   125,   126,   127,   838,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,   550,    -1,   609,   155,    -1,    -1,   330,
     119,    -1,   121,   122,   123,   874,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,   137,   350,
      -1,    -1,   353,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,   609,    -1,    19,   924,   925,   926,   927,    -1,
     674,    -1,   676,    -1,    -1,   679,    -1,    -1,   682,    34,
      -1,    36,    -1,    -1,   943,   944,    -1,   691,    -1,    -1,
     694,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    23,    -1,    62,    -1,   430,
      -1,    -1,   716,    -1,    32,    -1,    34,    35,    -1,    -1,
      38,    -1,    -1,    -1,   728,    43,   730,   674,    46,   676,
     734,    -1,   679,    -1,    -1,   682,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,   691,    -1,    64,   694,    66,    67,
     105,    69,   107,    71,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,   716,
      -1,    -1,    90,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   728,    -1,   730,    -1,    -1,   790,   734,    -1,    -1,
      -1,   795,    -1,    -1,    -1,    -1,   147,   148,   149,    -1,
      -1,   152,    -1,   154,    -1,   809,    -1,   162,    -1,    -1,
      -1,    -1,    -1,   168,    -1,    -1,    -1,   821,   173,    -1,
      -1,   176,    -1,    -1,    -1,   143,   830,    -1,    -1,   550,
     834,   186,   187,   151,   838,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   790,    -1,    -1,    -1,    -1,   795,   204,
      -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   809,    -1,    -1,    -1,    -1,    -1,   223,    -1,
     874,    -1,    -1,    -1,   821,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   830,    -1,    -1,    -1,   834,   609,    -1,
      -1,   838,    -1,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,
     924,   925,   926,   927,    -1,    -1,    -1,   874,    -1,    -1,
      -1,   286,    -1,    -1,    -1,    -1,   147,   148,    -1,   943,
     944,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   674,    -1,   676,    -1,    -1,   679,    -1,
      -1,   682,    -1,    -1,   319,   320,    -1,    -1,    -1,    -1,
     691,    -1,    -1,   694,    -1,    -1,    -1,   924,   925,   926,
     927,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   346,    -1,    -1,    -1,   716,   943,   944,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   728,    -1,   730,
      -1,    -1,    -1,   734,    -1,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,   382,    -1,    -1,
      16,    -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,   403,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,   436,    -1,    -1,   439,    -1,   441,    -1,   809,     3,
      -1,    -1,    -1,    79,    80,    81,    10,    83,    84,    -1,
     821,    -1,    -1,    17,    -1,    -1,    -1,    -1,    94,   830,
      -1,    -1,    -1,   834,    -1,    -1,    -1,   838,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,   874,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,   155,
      -1,    -1,   527,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   105,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   564,
      -1,   125,    -1,   568,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,   141,    -1,    -1,
     144,   145,   146,   147,    -1,   149,   591,    -1,   152,    -1,
     595,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
     625,    -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    -1,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,   698,    83,    84,   701,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,   721,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,   739,    -1,    -1,    -1,   127,    -1,
      -1,   746,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    -1,    16,    17,    -1,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,   851,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    83,
      84,    -1,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,    -1,    -1,   150,   151,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    -1,    -1,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      -1,    83,    84,    -1,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
     142,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      -1,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    -1,    83,    84,    -1,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,   151,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    -1,
      -1,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,    -1,    83,    84,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    -1,    -1,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    -1,    83,    84,    -1,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,   151,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    -1,    -1,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    -1,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    -1,    83,
      84,    -1,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,   150,   151,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    -1,    -1,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    -1,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      -1,    83,    84,    -1,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      -1,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    -1,    83,    84,    -1,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    -1,
      -1,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    -1,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,    -1,    83,    84,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    -1,    -1,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    -1,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    -1,    83,    84,    -1,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    -1,    -1,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    -1,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    -1,    83,
      84,    -1,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    -1,    -1,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    -1,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      -1,    83,    84,    -1,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      -1,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      -1,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    -1,    83,    84,    -1,    86,    -1,    88,    89,
      90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    -1,
      -1,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    -1,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,    -1,    83,    84,    -1,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    -1,    -1,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    -1,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    -1,    83,    84,    -1,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    -1,    -1,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    -1,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    -1,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    -1,    83,
      84,    -1,    86,    -1,    88,    89,    90,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    -1,    -1,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    -1,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    -1,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      -1,    83,    84,    -1,    86,    -1,    88,    89,    90,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,   142,
      16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,   155,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,   142,    16,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,   155,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,   155,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,
      16,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      46,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    16,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    46,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    -1,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,    88,    89,    -1,    -1,    92,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,   128,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,   142,    16,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    88,    89,
      -1,    -1,    92,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,   153,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    88,    89,    -1,    -1,    92,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,   142,
      16,    -1,    -1,   146,    -1,    -1,    -1,   150,    24,   152,
     153,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    -1,    41,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    88,
      89,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,    88,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,   142,    16,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    88,    89,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    -1,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    35,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    -1,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    46,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    -1,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,    88,    89,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,   151,   152,    -1,   154,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    -1,
      16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,   142,    16,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    -1,    16,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
     142,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,   142,    16,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,   142,    16,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    -1,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    -1,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    -1,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    -1,
      16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    -1,    16,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      -1,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,    -1,    16,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,    -1,    16,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    -1,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    -1,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    -1,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    -1,
      16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,
     146,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,   136,    16,   138,
      -1,    -1,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    30,   152,    -1,   154,    -1,    -1,    36,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   105,    -1,    12,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,   124,    30,    -1,    -1,
     128,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
     138,    -1,    45,    -1,   142,    48,    49,    -1,    51,    -1,
      53,    54,   150,    -1,   152,   153,   154,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    16,    -1,    -1,   138,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    30,   150,    -1,   152,
      -1,   154,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    45,   138,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,   150,    -1,   152,    61,
     154,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,   138,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,     3,   150,    -1,
     152,    61,   154,    63,    10,    65,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
     120,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    72,    -1,   125,
      -1,    76,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,   141,    -1,    10,   144,   145,
     146,   147,    -1,   149,    17,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    28,    -1,    72,
     155,    -1,    -1,    76,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,    -1,    -1,    10,
      -1,    -1,   155,    -1,    -1,    -1,    17,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    28,   129,   130,
     131,   132,   104,   134,   135,   136,   137,    -1,   139,    -1,
     141,    -1,    -1,   144,   145,   146,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,    -1,    -1,
      10,    -1,    -1,   155,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    72,   144,   145,   146,    76,    -1,    -1,    -1,
      -1,    10,   153,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    72,   144,   145,   146,    76,    -1,    -1,
      -1,    -1,    10,   153,    -1,    -1,    85,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    72,   144,   145,   146,    76,    -1,
      -1,   150,    -1,    10,    -1,    -1,    -1,    85,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    72,   144,   145,   146,    76,
      -1,    -1,   150,    -1,    10,    -1,    -1,    -1,    85,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    72,   144,   145,   146,
      76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,    85,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,
      85,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,
      -1,    85,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,
      -1,    -1,    85,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,   150,    -1,    10,
      -1,    -1,    -1,    85,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,    -1,
      -1,   113,   114,   115,   116,   117,   118,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,   150,    -1,
      10,    -1,    -1,    -1,    85,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
      -1,    41,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    72,   144,   145,   146,    76,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,   104,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,    72,
     150,    -1,    -1,    76,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,   104,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    -1,   144,   145,   146,    28,    -1,    72,   150,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    28,    -1,   130,   131,   132,
     104,   134,   135,   136,   137,    -1,    -1,    -1,   141,    -1,
      -1,   144,   145,   146,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,
      -1,    -1,    85,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,   150,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    57,   139,   140,
     141,    -1,    10,   144,   145,   146,    -1,    -1,   149,    17,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    72,   125,   126,   127,    76,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,   143,   144,   145,   146,    10,    -1,    -1,
      -1,    -1,   100,    -1,    17,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    57,    -1,   144,   145,   146,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    72,   125,   126,   127,    76,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
      -1,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,    -1,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   143,   144,   145,   146,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,   104,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   104,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,
     125,   126,   127,    76,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    10,   143,   144,
     145,   146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
     143,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    72,   125,   126,   127,    76,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
     143,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,    -1,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    10,    11,    -1,
      -1,   100,    -1,    -1,    17,   104,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   104,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,
     125,   126,   127,    76,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    10,   143,   144,
     145,   146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
      -1,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    72,   125,   126,   127,    76,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   104,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    10,    -1,   144,   145,   146,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    28,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    85,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,   104,   144,
     145,   146,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    85,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,   104,   144,   145,
     146,    -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,
      17,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    28,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    -1,   144,   145,   146,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    28,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    85,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   104,   144,   145,   146,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    85,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,   104,   144,   145,   146,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    28,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    28,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    85,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   104,   144,   145,   146,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    85,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,   104,   144,   145,   146,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    28,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    28,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,   104,    28,   144,   145,   146,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     104,   144,   145,   146,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,   104,    -1,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,    -1,   134,   135,
     136,   137,    -1,   139,    -1,   141,    -1,    -1,   144,   145,
     146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   160,   161,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    19,    20,    21,    23,    25,
      26,    27,    29,    30,    32,    34,    35,    36,    37,    38,
      39,    42,    43,    45,    47,    48,    49,    50,    51,    53,
      54,    56,    60,    61,    63,    64,    65,    66,    69,    71,
      74,    75,    77,    78,    79,    80,    81,    83,    84,    86,
      88,    89,    90,    93,    94,    95,    99,   101,   104,   119,
     120,   121,   122,   127,   136,   138,   140,   141,   143,   146,
     150,   152,   154,   162,   163,   164,   165,   166,   168,   173,
     174,   175,   177,   180,   181,   182,   183,   184,   185,   187,
     188,   189,   193,   194,   197,   198,   201,   202,   205,   208,
     209,   232,   235,   236,   256,   257,   258,   259,   260,   261,
     262,   270,   271,   272,   273,   276,   277,   278,   279,   280,
     281,   283,   284,   285,   286,   287,   143,   164,   273,   100,
     263,   264,   176,   177,    12,    36,    37,    39,    84,   150,
     154,   205,   256,   261,   271,   272,   273,   276,   278,   279,
     263,   273,    90,   176,   164,   250,   273,   164,   152,   273,
     274,     7,     9,    90,   178,   274,    62,   102,   186,   273,
     273,    23,    35,   235,   273,   273,   152,   152,   177,   206,
     152,   177,   237,   238,    29,   168,   179,   273,   273,   273,
     273,   273,   273,     7,   152,    35,   182,   182,   142,   228,
     249,   273,    88,    89,   143,   273,   275,   273,   179,   273,
     273,   273,    83,   152,   164,   273,   273,   168,   175,   273,
     276,   168,   175,   273,   177,   233,   273,   249,   273,   273,
     273,   273,   273,   273,   273,   273,     1,   151,   162,   169,
     249,    92,   128,   228,   251,   252,   275,   249,   273,   282,
      64,   164,    68,   167,   177,   177,    46,    67,   223,    24,
      62,    73,    96,   143,   149,    10,    17,    28,    72,    76,
     104,   119,   121,   122,   123,   125,   126,   127,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   139,   140,   141,
     144,   145,   146,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   147,   148,   152,
     154,    72,    76,   152,   164,   143,   273,   273,   273,   249,
     150,   168,    41,   263,   233,   143,   124,   143,    99,   177,
     228,   253,   254,   255,   275,   233,   198,   177,   152,   179,
      29,    41,   179,    29,    41,   100,   179,   266,    33,    57,
     143,    85,   105,   106,   107,   108,   109,   110,   113,   114,
     115,   116,   117,   118,   168,   215,   253,   253,   164,   152,
     217,    92,   152,   177,   239,   240,     1,   123,   244,    41,
     124,   164,   179,   179,   253,   177,   124,   143,   273,   273,
     143,   150,   179,   152,   253,   143,   190,   143,   190,   143,
     105,   234,   143,   143,    33,    57,   179,   143,   151,   151,
     162,   124,   151,   273,   124,   153,   124,   153,    41,   124,
     155,   266,   103,   124,   155,     7,    52,   123,   199,   150,
     210,    73,   237,   237,   237,   237,   273,   273,   273,   273,
     186,   273,   186,   273,   273,   273,   273,   273,    16,    94,
     273,   273,    30,    90,   177,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   275,   275,   275,   275,   275,   275,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   275,   253,   253,   186,
     273,   186,   273,    24,    41,    73,    96,   265,   268,   269,
     273,   287,    29,    41,    29,    41,    85,    41,   155,   186,
     273,   179,   143,   273,   273,   105,   153,   124,   143,   177,
      41,   249,    39,   273,   186,   273,    39,   273,   186,   273,
     152,   179,   146,   170,   172,   273,   170,   171,   164,   273,
      31,   273,   153,   153,    24,    41,    44,    59,    62,    73,
      90,   163,   218,   219,   220,   221,   207,   240,   124,   153,
      16,    30,    61,    79,    80,    84,    94,   154,   245,   258,
     105,   241,   273,   238,   153,   228,   273,     1,   195,   253,
     153,    18,   191,   245,   258,   124,   172,   171,   151,   153,
     153,   251,   153,   251,   186,   273,   155,   164,   273,   155,
     273,   155,   273,   177,   249,   150,     1,   177,   203,   204,
      24,    62,    73,    90,   212,   222,   237,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   153,   155,    41,
      73,   124,   153,   256,    72,    72,    39,   186,   273,    39,
     186,   273,   186,   273,   263,   263,   143,   228,   275,   255,
     199,   273,   153,   273,    29,   179,    29,   179,   273,    29,
     179,   266,    29,   179,   266,   267,   268,   124,   143,    11,
     143,    31,    31,   164,    85,   168,    41,    73,   220,   124,
     153,   152,   177,    24,    62,    73,    90,   224,   153,   240,
     244,     1,   249,    55,   275,   151,    58,    97,   151,   196,
     153,   152,   168,   177,   192,   233,   143,   143,   155,   266,
     155,   266,   164,   103,   150,     1,   200,   151,   105,   124,
     151,    73,   211,    10,    17,   119,   120,   121,   122,   125,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   141,   144,   145,   146,   147,   149,   152,   177,   213,
     214,   215,   256,   143,   268,   244,   256,   256,   273,    29,
      29,   273,    29,    29,   155,   155,   179,   179,   150,   179,
      85,    39,   273,    39,   273,    85,    39,   273,   179,    39,
     273,   179,   124,   153,   273,   273,   164,   273,   164,    31,
     177,   219,   240,   123,   248,    73,   244,   241,   155,    41,
     155,    29,   164,   249,   192,   123,   168,    39,   164,   273,
     155,    39,   164,   273,   155,   273,     1,   151,   169,   151,
      35,    66,    69,   151,   163,   181,   273,   204,   224,   177,
     273,   126,   152,   216,   216,   241,    85,    39,    39,    85,
      39,    39,    39,    39,     1,   200,   273,   273,   273,   273,
     273,   273,   268,    11,    31,   164,   248,   153,    16,    30,
      61,    79,    80,    84,    94,   154,   228,   247,   258,   128,
     230,   241,    98,   231,   273,   245,   258,   164,   179,   153,
     273,   273,   164,   273,   164,   151,   151,   181,     1,   123,
     243,   153,   177,   214,   215,   218,   273,   273,   273,   273,
     273,   273,   151,   151,    85,    85,    85,    85,   273,   164,
     241,   248,   155,   249,   228,   229,   273,   273,   168,   180,
     227,   155,   168,    85,    85,    16,    30,    61,    79,    80,
      84,    94,   154,   242,   258,    87,   225,   216,   216,   153,
      85,    85,    85,    85,    85,    85,   273,   273,   273,   273,
     230,   241,   228,   246,   247,   258,    41,   155,   258,   273,
     273,     1,   155,   249,   231,   273,   246,   247,   155,   242,
     258,   155,   143,   226,   227,   155,   242,   258,   246
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   159,   160,   161,   161,   162,   162,   163,   163,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   165,
     165,   165,   166,   166,   166,   167,   167,   168,   168,   168,
     169,   169,   170,   170,   170,   170,   171,   171,   172,   172,
     173,   173,   173,   174,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     176,   176,   177,   178,   178,   179,   179,   180,   180,   181,
     181,   181,   181,   181,   181,   181,   181,   182,   182,   183,
     183,   183,   183,   184,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   186,   187,   187,   187,
     187,   187,   187,   187,   187,   188,   189,   189,   189,   189,
     189,   189,   190,   190,   191,   191,   191,   192,   192,   193,
     194,   194,   195,   195,   196,   196,   196,   197,   197,   197,
     197,   198,   198,   198,   199,   199,   200,   200,   200,   201,
     201,   202,   203,   203,   203,   204,   204,   206,   207,   205,
     208,   208,   208,   208,   208,   210,   211,   209,   212,   212,
     212,   212,   213,   213,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   216,   217,   218,   218,   218,   219,   219,
     219,   219,   219,   220,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   222,   222,   222,   222,   222,   222,
     223,   223,   224,   224,   224,   224,   224,   224,   225,   225,
     226,   226,   227,   227,   228,   228,   229,   229,   230,   230,
     231,   231,   232,   232,   232,   233,   233,   234,   234,   234,
     235,   235,   235,   235,   235,   236,   236,   236,   237,   237,
     238,   238,   239,   239,   239,   240,   240,   240,   241,   241,
     241,   242,   242,   242,   242,   242,   242,   242,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     245,   245,   245,   245,   246,   246,   246,   247,   247,   247,
     247,   247,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   249,   249,   249,   249,   250,   250,   251,
     251,   251,   252,   252,   253,   253,   254,   254,   255,   255,
     255,   255,   256,   257,   257,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   260,
     261,   262,   262,   262,   262,   262,   262,   262,   262,   263,
     263,   264,   265,   265,   266,   267,   267,   268,   268,   268,
     269,   269,   269,   269,   269,   269,   270,   270,   271,   272,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   274,   274,   275,   275,
     275,   276,   276,   276,   276,   277,   277,   278,   278,   278,
     279,   279,   279,   280,   280,   280,   280,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   282,   282,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   284,   284,   284,   284,   284,   284,   285,
     285,   285,   285,   286,   286,   286,   286,   287,   287,   287,
     287,   287,   287,   287
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
       3,     5,     5,     3,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       0,     1,     1,     0,     1,     2,     1,     2,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       5,     5,     2,     2,     5,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     4,     3,     6,
       5,     6,     5,     8,     7,     2,     3,     3,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     3,     6,     8,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       5,     1,     1,     2,     3,     1,     3,     0,     0,     8,
       0,     1,     2,     2,     1,     0,     0,    10,     3,     3,
       5,     5,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     5,
       4,     6,     6,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     2,     1,     0,     1,     2,     2,
       0,     2,     3,     4,     4,     2,     4,     0,     2,     2,
       4,     4,     4,     5,     4,     0,     1,     1,     1,     3,
       3,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     3,     2,     4,     3,     3,     4,     3,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     0,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       1,     1,     1,     1,     4,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     2,     2,
       2,     2,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     3,     2,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     3,     3,     2,     2,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     3,
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
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 5276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5511 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 659 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 686 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5782 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5788 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5794 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5800 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 693 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5806 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5812 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5824 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5830 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5836 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5842 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 703 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 715 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 727 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 743 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5953 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 755 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 5980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 781 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 785 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 794 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 796 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 800 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 801 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 806 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 817 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 827 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 837 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 847 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 859 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 864 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 877 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 887 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 892 "chapel.ypp" /* yacc.c:1661  */
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
#line 6242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 908 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 924 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 958 "chapel.ypp" /* yacc.c:1661  */
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
#line 6338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 978 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 984 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1020 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1027 "chapel.ypp" /* yacc.c:1661  */
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
#line 6443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1074 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1083 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1085 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1086 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1088 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1092 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1099 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1101 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1115 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1122 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1123 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1131 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1133 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1138 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1140 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1142 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1144 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1146 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1150 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1151 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1155 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1158 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1159 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6813 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6819 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6825 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6831 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1190 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1211 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1222 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1231 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1269 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1274 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1282 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1298 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1307 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1316 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1324 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1343 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1348 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1350 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7211 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7217 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7223 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1370 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1380 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1384 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7261 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7267 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1392 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7273 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1393 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1396 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1397 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1398 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1399 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1400 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7327 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7333 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7339 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7345 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7357 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7363 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7369 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1413 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7381 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1415 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1416 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1441 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1445 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7429 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7435 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7447 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7459 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7471 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7543 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7549 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1500 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1506 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1517 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1533 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1538 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol), (yyvsp[-1].pcallexpr)); }
#line 7669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7848 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1653 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1683 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1687 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1692 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1697 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 7926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1701 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1705 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 7948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 7954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1713 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 7960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1714 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 7966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1715 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 7972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 7978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1727 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1754 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1758 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1760 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 470:
#line 1772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8074 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1794 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1795 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1796 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1800 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8098 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1801 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1802 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 485:
#line 1812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 486:
#line 1813 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 487:
#line 1817 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 8140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 488:
#line 1818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 489:
#line 1819 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1820 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1821 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1823 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1824 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1825 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1827 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1831 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1838 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8210 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1839 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1843 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1844 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1846 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1852 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1898 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1899 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8484 "bison-chapel.cpp" /* yacc.c:1661  */
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
