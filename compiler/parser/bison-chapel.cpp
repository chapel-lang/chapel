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
#define YYLAST   15302

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  130
/* YYNRULES -- Number of rules.  */
#define YYNRULES  542
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  980

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
     615,   616,   620,   624,   625,   629,   630,   634,   635,   639,
     640,   641,   642,   643,   644,   645,   646,   650,   651,   655,
     656,   657,   658,   662,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,   691,   697,   703,   709,   716,   726,   730,   731,   732,
     733,   734,   736,   738,   740,   745,   748,   749,   750,   751,
     752,   753,   757,   758,   762,   763,   764,   768,   769,   773,
     776,   778,   783,   784,   788,   790,   792,   799,   809,   819,
     829,   842,   847,   852,   860,   861,   866,   867,   869,   874,
     890,   897,   906,   914,   918,   925,   926,   931,   936,   930,
     961,   967,   974,   980,   986,   997,  1003,   996,  1035,  1039,
    1044,  1048,  1053,  1060,  1061,  1062,  1063,  1064,  1065,  1066,
    1070,  1071,  1072,  1076,  1077,  1078,  1079,  1080,  1081,  1082,
    1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,
    1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1105,
    1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,  1115,
    1116,  1120,  1121,  1125,  1129,  1130,  1131,  1135,  1137,  1139,
    1141,  1143,  1148,  1149,  1153,  1154,  1155,  1156,  1157,  1158,
    1159,  1160,  1161,  1165,  1166,  1167,  1168,  1169,  1170,  1174,
    1175,  1179,  1180,  1181,  1182,  1183,  1184,  1188,  1189,  1192,
    1193,  1197,  1198,  1202,  1204,  1209,  1210,  1214,  1215,  1219,
    1220,  1224,  1226,  1228,  1233,  1246,  1263,  1264,  1266,  1271,
    1279,  1287,  1295,  1304,  1314,  1315,  1316,  1320,  1321,  1329,
    1331,  1336,  1338,  1340,  1345,  1347,  1349,  1356,  1357,  1358,
    1363,  1365,  1367,  1371,  1375,  1377,  1381,  1389,  1390,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1404,  1405,
    1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,  1434,
    1438,  1442,  1450,  1457,  1458,  1459,  1463,  1465,  1471,  1473,
    1475,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,
    1489,  1490,  1496,  1497,  1498,  1499,  1503,  1504,  1508,  1509,
    1510,  1514,  1515,  1519,  1520,  1524,  1525,  1529,  1530,  1531,
    1532,  1536,  1540,  1541,  1552,  1553,  1554,  1555,  1556,  1557,
    1558,  1560,  1562,  1564,  1566,  1568,  1570,  1572,  1574,  1576,
    1578,  1583,  1585,  1587,  1589,  1591,  1593,  1595,  1597,  1599,
    1601,  1603,  1605,  1607,  1614,  1620,  1626,  1632,  1641,  1651,
    1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1671,  1672,
    1676,  1680,  1681,  1685,  1689,  1690,  1694,  1698,  1702,  1709,
    1710,  1711,  1712,  1713,  1714,  1718,  1719,  1724,  1729,  1737,
    1738,  1739,  1740,  1741,  1742,  1743,  1744,  1745,  1747,  1749,
    1751,  1753,  1755,  1757,  1759,  1764,  1765,  1768,  1769,  1770,
    1773,  1774,  1775,  1776,  1787,  1788,  1792,  1793,  1794,  1798,
    1799,  1800,  1808,  1809,  1810,  1811,  1815,  1816,  1817,  1818,
    1819,  1820,  1821,  1822,  1823,  1824,  1828,  1836,  1837,  1841,
    1842,  1843,  1844,  1845,  1846,  1847,  1848,  1849,  1850,  1851,
    1852,  1853,  1854,  1855,  1856,  1857,  1858,  1859,  1860,  1861,
    1862,  1863,  1867,  1868,  1869,  1870,  1871,  1872,  1876,  1877,
    1878,  1879,  1883,  1884,  1885,  1886,  1891,  1892,  1893,  1894,
    1895,  1896,  1897
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
  "$@4", "fn_decl_stmt_inner", "scalar_type", "fn_decl_receiver_expr",
  "fn_ident", "assignop_ident", "opt_formal_ls", "req_formal_ls",
  "formal_ls", "formal", "opt_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_or_iter", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "scalar_type_expr", "type_level_expr",
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

#define YYPACT_NINF -869

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-869)))

#define YYTABLE_NINF -494

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -869,   121,  3010,  -869,   -35,  -869,  -869,  -869,  -869,  -869,
    -869,  4400,    76,   275,  -869,  9371,  -869,    76,  9371,  -869,
     189,   275,  4400,  9371,  4400,   188,  -869,  9371,  6531,  7824,
    9371,  7976,  9371,  -869,   194,  -869,  -869,   275,  -869,    35,
    8509,  9371,  9371,  -869,  9371,  -869,  9371,  9371,   348,   221,
    1107,  1181,  -869,  -869,  8636,  7393,  9371,  8509,  9371,  9371,
     273,  -869,   232,  4400,  9371,  9498,  9498,   275,  -869,  -869,
    9371,  8636,  9371,  9371,  -869,  -869,  9371,  -869,  -869, 11511,
    9371,  9371,  -869,  9371,  -869,  -869,  3488,  6810,  8636,  -869,
    4248,  -869,  -869,   326,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,   275,  -869,   275,   328,   192,  -869,   245,  -869,  -869,
      39,   259,  1301,  -869,  -869,  -869,   263,   274,   284,   298,
     305, 15079, 11732,   -24,   313,   330,  -869,  -869,  -869,  -869,
    -869,  -869,   204,  -869,  -869, 15079,   334,  4400,  -869,   336,
    -869,  9371,  9371,  9371,  9371,  9371,  8636,  8636,   187,  -869,
    -869,  -869,  -869,  -869,   361,   217,  -869,  -869,   347, 13504,
     275,   357,  -869,   134, 15079,   406,  7545, 15079,  -869,   101,
    -869,   275,   163,  -869,   275,   349,    54, 12939, 12862,  -869,
    -869,  -869, 13452, 12223,  7545,  4400,   354,    11,    90,    46,
    -869,  4400,  -869,  -869, 13087,   253,   361, 13087,   361,   253,
    -869,  7545,  8103,  -869,  -869,   275,  -869,   220, 15079,  9371,
    9371,  -869, 15079,   368, 13300,  -869, 13087,   361, 15079,   362,
    7545,  -869, 15079, 13721,  -869,  -869, 13807, 12028,  -869,  -869,
   13859,   417,   382,   361,   281, 13589, 13087, 13939,   358,  1545,
     253,   358,   253,    75,  -869,  -869,  3640,    91,  -869,  9371,
    -869,   -53,   -19,  -869,    66, 13991,   -28,   522,  -869,  -869,
     475,   408,   386,  -869,  -869,  -869,  7545,    12,    35,    35,
      35,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  8230,  -869,  9371,  9371,  9371,  9371,  8788,
    8788,  9371,  9371,  9371,  9371,  9625,  9371,    85, 11511,  9371,
    9371,  9371,  9371,  9371,  9371,  9371,  9371,  9371,  9371,  9371,
    9371,  9371,  9371,  9371,  8230,  8230,  8230,  8230,  8230,  7545,
    7545,  8788,  8788,  7266,  -869,  -869, 13641, 13773, 14071,    65,
    3792,  -869,  8788,    54,   389,  -869,  9371,  -869,  9371,   430,
    -869,   388,   418,  -869,  -869,   400,   275,   504,  8636,  -869,
    4552,  8788,  -869,  4704,  8788,   395,  -869,    54,  9752,  9371,
    -869,  4400,   523,  9371,   407,  -869,    95,  -869,  -869,    11,
    -869,   431,   410,  -869,  9879,   454,  9371,    35,  -869,  -869,
    -869,   412,  -869,  8636,  -869, 15079, 15079,  -869,    26,  -869,
    7545,   415,  -869,   551,  -869,   551,  -869, 10006,   448,  -869,
    -869,  9752,  9371,  -869,  -869,  -869,  -869,  -869,  8357,  -869,
    1988,  6962,  -869,  7114,  -869,  8788,  6072,  3336,   419,  9371,
    6378,  -869,  -869,   275,  8636,   425,   369,   318,   423,    35,
     296,   307,   311,   321,   435, 13371,  2565,  2565,   332,  -869,
     332,  -869,   332, 13129,   393,   127,   468, 11852, 11852,   361,
     358,  -869,  -869,  -869,  1545,  2757,   332,   747,   747,  2565,
     747,   747,   884,   358,  2757, 15156,   884,   253,   253,   358,
     437,   438,   439,   440,   441,   433,   445,  -869,   332,  -869,
     332,    77,  -869,  -869,  -869,    92,  -869,   275, 15118,   252,
   10133,  8788, 10260,  8788,  9371,  8788, 12036,    76, 14123,  -869,
    -869, 15079, 14157,  7545,  -869,  7545,  -869,   408,  9371,    98,
    9371, 15079,    60, 13158,  9371, 15079,    29, 13010,  7266,  -869,
     460,   480,   465, 14209,   480,   474,   596, 14289,  4400, 13229,
    -869,   157,  -869,  -869,  -869,  -869,  -869,  -869,   677,   107,
    -869,    36,  -869,   319,   476,    11,    90,  9371,   188,  9371,
    9371,  9371,  9371,  9371,  5920,  -869,   487,  7697,  -869, 15079,
    -869,  -869,  -869, 15079,   481,    55,   482,  -869,    34,  -869,
    -869,   327,   275,   490,   493,  -869,  -869,  -869,  -869,  -869,
    -869,   -58,   980,  -869,  -869, 15079,  4400, 15079,  -869, 14341,
     488,   515,  1431,   491,   546,   172,  -869,   579,  -869,  -869,
    -869,  -869, 11361,  -869,   342,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  7266,
    -869,     9,  8788,  8788,  9371,   624, 14375,  9371,   626, 14507,
     502,  2173,    54,    54,  -869,  -869,  -869,  -869,   508, 13087,
    -869, 12294,  4856,  -869,  5008,  -869, 12365,  5160,  -869,    54,
    5312,  -869,    54,   108,  -869,  9371,  -869,  9371,  -869,  4400,
    9371,  -869,  4400,   629,  -869,  -869,   275,   914,  -869,    11,
     539,   592,  -869,  -869,  -869,    73,  -869,  -869,   454,   511,
     112,  -869,  -869,  -869,  5464,  8636,  -869,  -869,  -869,   275,
    -869,   545,   347,  -869,  -869,  -869,  5616,   516,  5768,   517,
    -869,  9371,  3944,   519,  1168,  -869,  9371,   275,  -869,  -869,
     319,   521,  -869,  -869,  -869,   275,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,  -869,  -869,  9371,   549,  -869,   552,
     521,   521,  -869,  -869,  -869,   454,   130,   208, 14546, 10387,
   10514, 14585, 10641, 10768, 10895, 11022,  -869,  -869,  1719,  -869,
    4400,  9371, 15079,  9371, 15079,  4400,  9371, 15079,  -869,  9371,
   15079,  -869,  7266,  -869, 14685, 15079,  -869, 15079,   646,  4400,
     539,  -869,   527,  8915,   -12,  -869,   583,  -869,  -869,  8788,
   11944,  4400,  -869,    57,   529,  9371,  -869,  9371,  -869, 15079,
    4400,  9371,  -869, 15079,  4400, 15079,   123,  -869,  4096,  -869,
    6683,   779,   779,  -869,  1520,  -869, 15079,  -869,    24,    95,
    -869,  -869, 12141, 12090,  -869,  -869,  -869,  9371,  9371,  9371,
    9371,  9371,  9371,  9371,  9371,   532,  1330, 14289, 12436, 12507,
   14289, 12578, 12649,  -869,  9371,  4400,  -869,   454,   539,  9371,
     188,  9371,  9371,  9371,  9371,  9371,  6225,  -869,  -869,   240,
    8636,  -869,  -869,  9371,    19,  2437,  -869,   526,   406,  -869,
     347, 15079, 12720,  -869, 12791,  -869,  -869,  -869,  -869,  -869,
   11149,   598,   214,  -869,  -869,   521,   521, 14724, 14763, 14863,
   14902, 14941, 15041,  -869,  -869,  4400,  4400,  4400,  4400, 15079,
    -869,  -869,   -12,  9067,   115,  -869,  -869, 15079, 15079,  -869,
    -869,  -869, 11022,  -869,  4400,  4400,  9371,   188,  9371,  9371,
    9371,  9371,  9371,  3168,  -869,   261,  -869,   583,  -869,  -869,
    -869,  9371,  9371,  9371,  9371,  9371,  9371, 14289, 14289, 14289,
   14289,  -869,  -869,  -869,  -869,  -869,   308,  8788, 11638,   569,
   14289, 14289,   535, 11276,   120,    74,  2839,  -869,  -869,  -869,
    -869,   279, 11733,  -869,  -869,  -869,  9219,  -869,   365,  -869
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    72,   486,   487,   488,   489,
     490,     0,   428,    70,   183,     0,   151,   428,     0,   188,
     295,    70,     0,     0,     0,     0,   161,   465,   296,     0,
       0,   294,     0,   187,     0,   171,   184,     0,   167,     0,
       0,     0,     0,   419,     0,   174,     0,     0,     0,     0,
     294,   294,   186,   152,     0,     0,     0,     0,     0,     0,
       0,   189,     0,     0,     0,     0,     0,     0,   185,   153,
       0,     0,     0,     0,   538,   540,     0,   541,   542,   464,
       0,     0,   539,   536,    79,   537,     0,     0,     0,     4,
       0,     5,     9,    45,    10,    11,    12,    14,   381,    22,
      13,    80,    86,    15,    17,    16,    19,    20,    21,    18,
      85,     0,    83,     0,   454,     0,    87,   382,    84,    88,
       0,   470,   385,   450,   451,   387,   384,     0,     0,   455,
     456,     0,   386,     0,   471,   472,   473,   449,   389,   388,
     452,   453,     0,    38,    24,   395,     0,     0,   429,     0,
      71,     0,     0,     0,     0,     0,     0,     0,   454,   470,
     385,   384,   455,   456,   397,   386,   471,   472,     0,   428,
       0,     0,   125,     0,   366,     0,   373,   466,   172,   489,
      93,     0,     0,   173,     0,     0,     0,     0,     0,   295,
     296,    92,     0,     0,   373,     0,     0,     0,     0,     0,
     297,     0,    76,    32,     0,   524,   447,     0,   398,   525,
       7,   373,   296,    82,    81,   274,   363,     0,   362,     0,
       0,    77,   469,     0,     0,    35,     0,   400,   390,     0,
     373,    36,   396,     0,   132,   128,     0,   386,   132,   129,
       0,   286,     0,   399,     0,   362,     0,     0,   527,   463,
     523,   526,   522,     0,    47,    50,     0,     0,   368,     0,
     370,     0,     0,   369,     0,   362,     0,     0,     6,    46,
       0,   154,     0,   260,   259,   175,   373,     0,     0,     0,
       0,   219,   226,   227,   228,   223,   225,   221,   224,   222,
     220,   230,   229,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   373,
     373,     0,     0,     0,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,     0,    29,     0,   381,
     379,     0,   374,   375,   380,     0,     0,     0,     0,   102,
       0,     0,   101,     0,     0,     0,   108,     0,     0,    56,
      89,     0,   118,     0,     0,    30,   234,   168,   301,     0,
     302,   304,     0,   338,     0,   307,     0,     0,    75,    31,
      33,     0,   273,     0,    63,   467,   468,    78,     0,    34,
     373,     0,   139,   130,   126,   131,   127,     0,   284,   281,
      60,     0,    56,    95,    37,    49,    48,    51,     0,   491,
       0,     0,   482,     0,   484,     0,     0,     0,     0,     0,
       0,   495,     8,     0,     0,     0,     0,   253,     0,     0,
       0,     0,     0,     0,     0,   427,   519,   518,   521,   529,
     528,   533,   532,   515,   512,   513,   514,   459,   460,   458,
     502,   481,   480,   479,   461,   506,   517,   511,   509,   520,
     510,   508,   500,   505,   507,   516,   499,   503,   504,   501,
       0,     0,     0,     0,     0,     0,     0,   531,   530,   535,
     534,   439,   440,   442,   444,     0,   431,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,   428,   428,    98,
     282,   367,     0,     0,   392,     0,   283,   154,     0,     0,
       0,   403,     0,     0,     0,   409,     0,     0,     0,   109,
     537,    59,     0,    52,    57,     0,   117,     0,     0,     0,
     391,   247,   244,   245,   246,   250,   251,   252,   242,     0,
     235,     0,   243,   261,     0,   305,     0,   334,   331,   335,
     337,   332,   333,   336,     0,   330,   450,     0,   299,   448,
     298,   478,   365,   364,     0,     0,     0,   393,     0,   133,
     288,   450,     0,     0,     0,   492,   457,   483,   371,   485,
     372,     0,     0,   494,   114,   413,     0,   497,   496,     0,
       0,   155,     0,     0,   165,     0,   162,   257,   254,   255,
     258,   176,     0,   383,     0,   290,   289,   291,   293,    69,
      67,    68,    65,    66,    64,   476,   477,   441,   443,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,   377,   378,   376,     0,     0,
     116,     0,     0,   100,     0,    99,     0,     0,   106,     0,
       0,   104,     0,     0,   434,     0,    90,     0,    91,     0,
       0,   120,     0,   122,   248,   249,     0,   242,   233,     0,
     351,   262,   265,   264,   266,     0,   303,   306,   307,     0,
       0,   308,   309,   141,     0,     0,   140,   143,   394,     0,
     134,   137,     0,   285,    61,    62,     0,     0,     0,     0,
     115,     0,     0,     0,   294,   160,     0,   163,   159,   256,
     261,   231,   216,   214,   195,   198,   196,   197,   208,   199,
     212,   204,   202,   215,   203,   201,   206,   211,   213,   200,
     205,   209,   210,   207,   217,   218,     0,   193,   191,     0,
     231,   231,   190,   292,   432,   307,   470,   470,     0,     0,
       0,     0,     0,     0,     0,     0,    97,    96,     0,   103,
       0,     0,   402,     0,   401,     0,     0,   408,   107,     0,
     407,   105,     0,   433,    54,    53,   119,   418,   121,     0,
     351,   236,     0,     0,   307,   263,   279,   300,   342,     0,
     493,     0,   145,     0,     0,     0,   135,     0,   112,   415,
       0,     0,   110,   414,     0,   498,     0,    39,     0,   149,
     296,   294,   294,   147,   294,   157,   166,   164,     0,   234,
     182,   194,     0,     0,   178,   179,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   406,     0,     0,
     412,     0,     0,   435,     0,     0,   124,   307,   351,   357,
     354,   358,   360,   355,   356,   359,     0,   353,   361,   450,
     275,   239,   237,     0,     0,     0,   340,   450,   146,   144,
       0,   138,     0,   113,     0,   111,    41,    40,   158,   327,
       0,   267,     0,   192,   193,   231,   231,     0,     0,     0,
       0,     0,     0,   150,   148,     0,     0,     0,     0,    55,
     123,   238,   307,   343,     0,   276,   278,   277,   280,   271,
     272,   169,     0,   136,     0,     0,   323,   320,   324,   326,
     321,   322,   325,     0,   319,   450,   268,   279,   232,   180,
     181,     0,     0,     0,     0,     0,     0,   405,   404,   411,
     410,   241,   240,   345,   346,   348,   450,     0,   493,   450,
     417,   416,     0,   311,     0,     0,     0,   347,   349,   316,
     314,   450,   493,   269,   177,   270,   343,   315,   450,   350
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -869,  -869,  -869,    -1,  -326,  1828,  -869,  -869,  -869,   402,
     -25,  -172,   280,   283,  -869,  -869,   162,   670,  2567,  -869,
     520,  -772,  -673,   -48,  -869,  -869,  -869,    96,  -869,  -869,
    -869,   458,  -869,     0,  -869,  -869,  -869,  -869,  -869,   528,
     181,   -66,  -869,  -869,  -869,   -13,  1065,  -869,  -869,  -869,
    -869,  -869,  -869,  -869,    99,  -869,  -117,  -100,  -684,  -869,
    -109,    48,   182,  -869,  -869,  -869,    17,  -869,  -869,  -234,
     385,  -869,  -180,  -199,  -869,  -157,  -869,   709,  -869,  -198,
     355,  -869,  -353,  -677,  -868,  -869,  -507,  -402,  -733,  -849,
    -700,   -31,  -869,   103,  -869,  -138,  -869,   228,   363,   623,
    -356,  -869,  -869,  1288,  -869,    -9,  -869,  -869,  -258,  -869,
    -519,  -869,  -869,  1436,  1621,   -11,   717,   150,  1000,  -869,
    1809,  2013,  -869,  -869,  -869,  -869,  -869,  -869,  -869,  -299
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   255,    90,   536,    92,    93,   270,    94,
     256,   531,   535,   532,    95,    96,    97,   149,    98,   182,
     203,    99,   100,   101,   102,   103,   104,   640,   105,   106,
     107,   403,   579,   702,   108,   109,   575,   697,   110,   111,
     435,   714,   112,   113,   605,   606,   158,   196,   553,   115,
     116,   437,   720,   611,   117,   749,   750,   373,   830,   377,
     549,   550,   551,   552,   612,   275,   685,   937,   974,   921,
     216,   916,   871,   874,   118,   242,   408,   119,   120,   199,
     200,   381,   382,   568,   934,   891,   385,   565,   954,   868,
     794,   257,   173,   261,   262,   351,   352,   353,   159,   160,
     123,   124,   125,   161,   127,   147,   148,   495,   367,   663,
     496,   497,   128,   162,   163,   131,   183,   354,   165,   133,
     166,   167,   136,   137,   266,   138,   139,   140,   141,   142
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     145,    89,   213,   214,   164,   580,   428,   169,   168,   664,
     383,   797,   174,   344,     5,     5,   177,   177,   187,   188,
     192,   193,   293,   217,   355,   889,   554,   574,   566,   204,
     205,   206,   328,   207,   499,   208,   209,     5,     5,     5,
     244,   825,   365,   218,   222,   224,   226,   227,   228,   688,
     548,   581,   232,   233,   236,   240,   374,   264,   657,   243,
     245,   246,   247,   277,   955,   248,   834,   835,   249,   250,
     251,   421,   252,   391,   383,   218,   222,   265,   836,   440,
     441,   442,   443,   201,  -142,   439,   201,   386,     5,   652,
     857,   383,   401,   567,    55,   970,   430,   706,  -242,  -317,
     422,   278,   920,   378,   977,   423,   505,   425,   143,   968,
     754,  -317,   279,   694,  -328,   461,   870,   872,   627,   541,
     -74,     3,  -317,  -142,   755,   186,   339,   431,   329,   365,
     330,  -328,   384,  -328,   424,   280,   542,   328,   438,   543,
     145,   336,   337,   338,   232,   218,   265,   890,  -328,    55,
     628,   888,   695,   799,   544,   298,   957,   545,   912,    48,
     343,   505,  -328,   379,   197,   222,   365,  -317,   546,   340,
     387,  -328,   -74,   825,  -317,   462,   146,  -142,   796,   340,
     911,   393,    16,   222,   340,   547,   699,   197,   679,   426,
     426,   485,   486,   920,   -74,  -328,   384,   534,   674,   299,
     222,   177,   687,   300,   340,   223,   696,   340,   395,   396,
     340,   939,   940,   384,  -328,   418,   629,   973,   143,   222,
     506,   427,   393,  -328,   340,   967,   415,   235,   239,   499,
     675,   677,   782,  -328,    53,   952,   426,   263,   273,   426,
     534,   614,   419,   979,   426,   630,   302,  -242,   420,   304,
     305,   650,   306,   307,  -438,   417,    69,   310,   346,   274,
     678,   783,   576,   853,   317,   222,   143,   800,   659,   662,
     958,   321,   322,   323,   886,   972,   331,   347,     5,   170,
     332,   298,   222,  -438,   445,   446,   447,   448,   450,   452,
     453,   454,   455,   456,   459,   460,   717,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   222,   222,   222,   222,   222,   222,   222,
     488,   490,   498,   718,   633,   299,   792,   519,   332,   300,
     499,   508,  -437,   707,   709,   511,  -318,   512,   677,  -475,
     176,  -475,   607,   681,   393,  -352,   194,   218,  -318,   521,
     523,   548,   525,   527,  -310,   210,   229,   533,   533,  -318,
     537,  -437,   539,   394,  -352,   121,  -310,   938,  -352,  -474,
     603,  -474,     5,   211,   121,   569,   305,  -310,   306,   307,
     608,   682,   573,   310,   230,   121,   298,   121,   824,   222,
     317,   609,   683,  -352,   269,   449,   451,   276,   876,   323,
     533,   533,  -421,   601,  -318,   393,  -420,   573,   610,   684,
     222,  -318,   222,  -344,   592,   573,   595,   294,   597,   599,
     387,   298,  -310,   218,   410,   703,   121,   487,   489,  -310,
     299,   387,  -344,   295,   300,   387,  -344,   869,   507,   615,
    -312,  -425,   202,   444,   877,   387,   164,   243,  -426,   121,
     616,  -287,  -312,   121,   617,   305,  -423,   522,   307,   202,
     526,  -344,   310,  -312,   618,   299,   387,   234,   238,   300,
    -287,  -424,   260,  -422,   480,   481,   482,   483,   484,   335,
    -475,   305,  -475,   499,   307,   753,   333,   307,   310,   521,
     636,   525,   639,   537,   641,   595,   298,   340,   642,   643,
     345,   358,   222,   548,   222,   348,   376,   649,  -312,   651,
     121,   397,   751,   656,   400,  -312,   305,   498,   306,   307,
     824,   591,   407,   310,   588,   409,   590,   433,  -329,   432,
     317,   434,   510,   690,   935,   513,   436,   321,   322,   323,
     299,   514,   515,   516,   300,   518,   164,   528,   208,   227,
     228,   232,   243,   265,   538,   555,   222,   956,   121,   567,
     540,   350,  -329,   556,   121,   571,   959,  -339,   577,   578,
     341,   263,   582,   263,   596,   602,   613,   225,   619,   350,
     620,   621,   622,   623,   624,  -329,   625,   302,   202,   202,
     202,   305,  -329,   306,   307,   372,   350,   635,   310,   638,
     626,  -339,   956,   -58,   665,   317,   202,   971,   666,   202,
    -341,  -329,   321,   322,   323,   350,   978,   668,   498,   121,
     956,   450,   488,   758,  -339,   122,   761,   669,   202,   686,
    -329,  -339,   693,   704,   122,   698,   705,  -329,   712,   393,
    -329,   772,   715,   774,  -341,   122,   777,   122,   202,   780,
    -339,   716,   719,   759,   784,   762,   785,   764,   768,   787,
     789,   350,   793,   646,   803,   795,   798,  -341,   805,  -339,
     819,   810,   814,   829,  -341,  -381,  -339,   855,   833,  -339,
     858,   873,   880,   903,   218,   936,   122,   818,   122,   122,
     969,   171,   584,  -341,   583,   809,   405,   813,   648,   804,
     815,   541,   846,   121,   827,   826,   359,   362,   366,   122,
     356,   748,  -341,   122,   350,   350,   895,   692,   542,  -341,
     892,   543,  -341,   121,   389,   791,   121,   390,   449,   487,
     676,   975,   951,   896,   121,   832,   544,   828,   965,   545,
     191,   267,   570,   647,   178,   202,   399,     0,   772,   774,
     546,   777,   780,   809,   813,     0,     0,     0,     0,   847,
     848,     0,   849,     0,   850,   851,   413,   547,   852,   202,
     122,   498,     0,   213,   214,   298,     0,     0,   572,     0,
       0,     0,     0,     0,     0,   350,     0,     0,   875,   595,
     121,     0,     0,     0,   881,     0,   882,     0,     0,     0,
     884,     0,   189,   572,     0,     0,   260,     0,   260,   177,
       0,   572,     0,    27,   212,     0,     0,   417,   122,   299,
       0,     0,    35,   300,   122,  -170,   847,   897,   898,   850,
     899,   900,   901,   902,     0,   914,     0,     0,     0,    45,
       0,     0,     0,   909,     0,     0,  -170,     0,   164,     0,
     208,   227,   228,   232,   243,   265,     0,     0,     0,   917,
     631,     0,   918,   509,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,     0,   310,     0,   122,
       0,     0,     0,   316,   317,     0,     0,   529,   320,     0,
       0,   321,   322,   323,   947,   948,   949,   950,   645,     0,
     350,   121,   964,     0,     0,     0,     0,     0,     0,     0,
       0,   813,   298,   960,   961,   164,     0,   208,   227,   228,
     232,   243,   265,     0,   202,   202,     0,     0,   202,   202,
     947,   948,   949,   950,   960,   961,     0,     0,   541,     0,
       0,   673,     0,     0,     0,     0,   966,   595,     0,     0,
       0,     0,     0,     0,     0,   542,   299,     0,   543,   121,
     300,   595,     0,   122,     0,   813,     0,     0,     0,     0,
       0,     0,     0,   544,     0,   752,   545,     0,    48,     0,
     700,     0,     0,   122,     0,     0,   122,   546,     0,     0,
     296,     0,     0,     0,   122,   756,   757,   297,     0,     0,
       0,     0,   132,   302,   547,   303,   304,   305,   298,   306,
     307,   132,     0,     0,   310,   121,     0,   121,     0,     0,
     121,   317,   132,   121,   132,     0,     0,     0,   321,   322,
     323,     0,   121,     0,     0,   121,     0,     0,     0,     0,
       0,     0,   653,   655,   202,   202,   658,   661,     0,     0,
     122,   202,   299,   372,     0,     0,   300,   121,   372,     0,
       0,   202,     0,   132,   202,   237,   237,   114,     0,   121,
       0,   121,     0,     0,     0,   121,   114,     0,     0,     0,
     365,     0,     0,     0,   301,     0,   132,   114,     0,   114,
     132,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,   806,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,     0,   321,   322,   323,     0,   114,     0,
     189,     0,     0,   121,     0,   708,     0,     0,   121,     0,
       0,    27,   212,     0,     0,     0,     0,   132,     0,     0,
      35,   114,   121,  -170,     0,   114,     0,     0,     0,   -44,
       0,   122,   766,   767,   121,     0,     0,    45,     0,   769,
       0,     0,     0,   121,  -170,   -44,     0,   121,   867,   778,
       0,   121,   781,     0,     0,     0,     0,    16,     0,     0,
       0,    20,     0,     0,     0,   132,     0,     0,     0,     0,
      26,   132,    27,   820,   189,   202,    31,     0,     0,     0,
       0,    35,   114,     0,  -170,    27,   212,     0,   121,   122,
       0,     0,     0,     0,    35,     0,     0,  -170,    45,     0,
       0,     0,    48,   -43,   821,  -170,     0,   822,     0,    53,
       0,    45,     0,     0,     0,     0,     0,     0,  -170,   -43,
     372,   372,     0,   372,   372,   915,   132,     0,    67,     0,
     114,    69,     0,     0,     0,     0,   114,     0,   121,   121,
     121,   121,     0,     0,     0,   122,   919,   122,     0,     0,
     122,     0,   923,   122,   372,     0,   372,   121,   121,     0,
     126,     0,   122,     0,     0,   122,     0,     0,   953,   126,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     126,    84,   126,     0,     0,     0,     0,   122,     0,   823,
       0,   114,     0,   879,     0,     0,     0,     0,     0,   122,
       0,   122,     0,     0,     0,   122,     0,     0,     0,     0,
     132,     0,     0,   953,     0,     0,     0,     0,     0,    16,
       0,   126,     0,    20,     0,     0,     0,     0,     0,     0,
     132,   953,    26,   132,    27,   820,     0,   919,    31,     0,
       0,   132,     0,    35,   126,     0,  -170,     0,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      45,     0,     0,   122,    48,     0,   821,  -170,   122,   822,
       0,    53,     0,     0,     0,   114,   281,   282,   283,   284,
     285,   286,   122,     0,   287,   288,   289,   290,   291,   292,
      67,     0,     0,    69,   122,   114,     0,   132,   114,     0,
       0,     0,   713,   122,     0,   126,   114,   122,   129,     0,
       0,   122,     0,     0,     0,     0,     0,   129,     0,     0,
    -156,     0,     0,     0,  -156,  -156,     0,     0,   129,     0,
     129,     0,     0,  -156,     0,  -156,  -156,     0,     0,  -156,
       0,     0,     0,    84,  -156,     0,     0,  -156,   122,     0,
       0,   904,     0,   126,     0,     0,     0,     0,     0,   126,
       0,  -156,   114,  -156,     0,  -156,     0,  -156,  -156,   129,
    -156,     0,  -156,     0,  -156,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -156,   129,     0,  -156,     0,   129,  -156,   122,   122,
     122,   122,     0,     0,     0,     0,     0,     0,   132,    16,
       0,     0,     0,    20,   126,     0,     0,   122,   122,     0,
       0,     0,    26,     0,    27,   820,     0,     0,    31,     0,
       0,     0,     0,    35,     0,     0,  -170,     0,     0,     0,
       0,     0,     0,   298,  -156,     0,     0,     0,     0,     0,
      45,     0,  -156,   129,   267,     0,   821,  -170,     0,   822,
       0,    53,     0,     0,     0,     0,   132,     0,     0,     0,
       0,     0,     0,   114,     0,     0,     0,     0,     0,     0,
      67,     0,     0,    69,     0,     0,     0,   299,     0,     0,
       0,   300,     0,   130,     0,     0,     0,     0,   126,     0,
       0,   129,   130,     0,     0,     0,     0,   129,     0,     0,
       0,     0,     0,   130,     0,   130,     0,     0,   126,     0,
       0,   126,   132,     0,   132,     0,     0,   132,     0,   126,
     132,   114,     0,    84,   302,     0,   303,   304,   305,   132,
     306,   307,   132,     0,     0,   310,     0,     0,     0,     0,
       0,   316,   317,     0,   130,     0,   320,     0,     0,   321,
     322,   323,   129,     0,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   132,   130,   132,     0,
       0,   130,   132,     0,     0,   126,     0,   114,     0,   114,
     845,     0,   114,     0,     0,   114,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   114,  -156,     0,
       0,     0,  -156,  -156,     0,     0,     0,     0,     0,     0,
       0,  -156,     0,  -156,  -156,     0,     0,  -156,     0,   114,
       0,     0,  -156,     0,     0,  -156,     0,     0,   130,     0,
     132,   114,     0,   114,     0,   132,   129,   114,     0,  -156,
       0,  -156,     0,  -156,     0,  -156,  -156,     0,  -156,   132,
    -156,     0,  -156,     0,     0,     0,   129,     0,     0,   129,
       0,   132,     0,     0,     0,     0,     0,   129,     0,  -156,
     132,   134,  -156,     0,   132,  -156,   130,     0,   132,     0,
     134,     0,   130,     0,     0,     0,   126,     0,     0,     0,
      91,   134,     0,   134,     0,   114,     0,     0,     0,   144,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     172,     0,   175,     0,   114,   132,     0,     0,     0,     0,
       0,     0,  -156,   129,     0,     0,   114,     0,     0,     0,
    -156,     0,   134,     0,     0,   114,     0,   130,     0,   114,
       0,     0,     0,   114,   126,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,   134,     0,     0,     0,   134,
       0,     0,     0,     0,     0,   132,   132,   132,   132,     0,
       0,     0,     0,     0,    91,     0,     0,     0,   268,     0,
     114,     0,     0,     0,   132,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     126,     0,   126,     0,     0,   126,     0,     0,   126,     0,
       0,     0,     0,     0,     0,     0,   134,   126,     0,     0,
     126,   130,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   114,   114,   114,   129,   334,     0,     0,     0,     0,
       0,   130,   126,     0,   130,     0,     0,     0,     0,   114,
     114,     0,   130,     0,   126,     0,   126,     0,   296,     0,
     126,     0,     0,     0,   134,   297,     0,     0,     0,     0,
     134,     0,     0,     0,     0,   135,   298,     0,     0,     0,
       0,     0,     0,   375,   135,     0,     0,     0,     0,   388,
       0,     0,   129,     0,     0,   135,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   126,     0,
     299,     0,     0,   126,   300,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   135,   126,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,   129,   126,
     129,     0,   301,   129,     0,     0,   129,     0,   126,   135,
       0,     0,   126,   135,     0,   129,   126,   302,   129,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
     129,     0,   321,   322,   323,     0,     0,     0,     0,     0,
       0,   586,   129,   126,   129,     0,     0,     0,   129,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   130,
     135,     0,     0,     0,     0,     0,     0,     0,    91,   134,
       0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
     134,     0,     0,   296,     0,     0,     0,     0,   388,     0,
     297,   388,     0,   126,   126,   126,   126,     0,     0,     0,
       0,   298,     0,     0,     0,     0,   129,     0,   135,     0,
       0,   129,   126,   126,   135,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,   129,     0,     0,
       0,     0,     0,     0,     0,   299,   129,     0,     0,   300,
     129,     0,     0,     0,   129,   594,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
       0,     0,     0,   130,     0,   130,     0,   301,   130,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
     130,   129,   302,   130,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,   130,     0,   321,   322,   323,
       0,     0,     0,     0,     0,     0,     0,   130,   765,   130,
       0,     0,     0,   130,     0,     0,     0,     0,     0,     0,
       0,   129,   129,   129,   129,     0,     0,   134,     0,     0,
       0,     0,     0,   135,     0,     0,     0,     0,     0,     0,
     129,   129,     0,     0,     0,     0,   671,     0,     0,     0,
       0,     0,     0,   135,     0,     0,   135,     0,     0,     0,
       0,     0,     0,     0,   135,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,   134,     0,     0,     0,     0,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,   710,     0,     0,     0,     0,     0,
       0,   130,     0,     0,     0,   130,     0,     0,     0,   130,
     135,     0,     0,     0,     0,     0,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,   134,     0,   134,     0,   298,   134,     0,     0,   134,
       0,     0,     0,     0,     0,     0,   130,     0,   134,     0,
     388,   134,   388,     0,     0,   388,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,   786,     0,     0,
     788,     0,     0,   134,     0,     0,     0,     0,     0,   299,
       0,     0,     0,   300,     0,   134,     0,   134,     0,     0,
       0,   134,   802,     0,     0,     0,   130,   130,   130,   130,
       0,     0,     0,     0,   808,     0,   812,     0,     0,     0,
      91,   301,     0,     0,     0,   130,   130,     0,     0,     0,
       0,   135,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   134,
     150,   321,   322,   323,   134,     0,     0,     0,   150,     0,
       0,     0,   922,   298,     0,     0,     0,     0,   134,     0,
       0,     0,     0,     0,   195,     0,   198,     0,     0,   135,
     134,     0,     0,     0,     0,     0,     0,   856,     0,   134,
       0,     0,     0,   134,     0,     0,     0,   134,     0,   878,
       0,     0,     0,     0,   241,     0,     0,   299,   883,     0,
       0,   300,   885,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,     0,   135,     0,   301,
     135,     0,     0,   135,     0,     0,     0,     0,   271,     0,
     272,     0,   135,   910,   302,   135,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,     0,   314,
     315,   316,   317,     0,   318,   319,   320,   135,     0,   321,
     322,   323,     0,     0,   134,   134,   134,   134,     0,   135,
       0,   135,     0,     0,     0,   135,     0,     0,     0,     0,
       0,     0,     0,   134,   134,     0,     0,   241,     0,     0,
       0,     0,     0,   349,     0,     0,     0,     0,   241,     0,
       0,   357,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   349,     0,     0,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,   392,   135,     0,   298,     0,     0,   135,     0,
       0,     0,     0,     0,     0,     0,     0,   349,     0,     0,
       0,     0,   135,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,     0,     0,     0,     0,     0,
       0,     0,     0,   135,     0,     0,     0,   135,     0,   299,
       0,   135,     0,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   349,   198,   198,   198,   198,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,   135,     0,
       0,     0,     0,     0,   463,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,     0,   310,   311,   312,
       0,   314,   315,   316,   317,     0,   349,   349,   320,     0,
       0,   321,   322,   323,     0,     0,     0,     0,     0,     0,
       0,   299,     0,     0,     0,   300,     0,     0,   135,   135,
     135,   135,     0,   517,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   135,   135,     0,
       0,     0,     0,   301,     0,     0,   380,     0,     0,     0,
       0,     0,     0,     0,   198,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,   349,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   976,     0,     0,     0,     0,     0,
     600,     0,     0,   604,     0,     0,   198,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -294,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -294,    47,    48,    49,    50,  -170,   -42,    51,
      52,    53,   349,  -294,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -294,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,   680,     0,
       0,     0,   380,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,   701,    80,     0,    81,   241,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     0,    88,     0,     0,     0,     0,   962,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   747,
     151,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   154,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,   790,     0,     0,   380,    58,    59,    60,
      61,    62,   155,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,   701,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   604,     0,     0,    75,    76,    77,
      78,     0,   831,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     215,     0,     0,     0,    85,     0,     0,     0,   156,     0,
      87,     0,   157,   963,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     0,     5,
       6,     7,     8,     9,    10,     0,  -493,     0,    11,    12,
      13,    14,    15,  -493,     0,    16,    17,    18,    19,    20,
    -294,    21,    22,    23,  -493,    24,    25,  -493,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -294,    47,
     894,    49,    50,  -170,   -42,    51,    52,    53,  -493,  -294,
      54,    55,  -493,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -294,     0,     0,    72,     0,    73,     0,     0,
    -493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -493,    76,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,     0,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,    81,  -493,  -493,  -493,     0,    84,
    -493,  -493,  -493,     0,     0,     0,    86,  -493,    87,   253,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -294,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -294,    47,    48,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -294,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -294,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     215,    84,     0,     0,    85,     0,     0,     0,    86,   254,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -294,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -294,    47,    48,    49,    50,  -170,   -42,    51,
      52,    53,     0,  -294,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -294,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,   416,    87,   253,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -294,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,    33,     0,    34,    35,     0,    36,  -170,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -294,    47,    48,    49,    50,  -170,
     -42,    51,    52,    53,     0,  -294,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -294,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,   254,    87,   816,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -294,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -294,    47,    48,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -294,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -294,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,   817,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -294,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -294,    47,
      48,    49,    50,  -170,   -42,    51,    52,    53,     0,  -294,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -294,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,   887,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -294,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -294,    47,   267,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -294,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -294,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -294,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -294,    47,     0,    49,    50,  -170,   -42,    51,
      52,    53,     0,  -294,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -294,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -294,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   520,    33,     0,    34,    35,     0,    36,  -170,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -294,    47,     0,    49,    50,  -170,
     -42,    51,    52,    53,     0,  -294,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -294,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,     0,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -294,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,   524,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -294,    47,     0,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -294,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -294,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -294,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,   771,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -294,    47,
       0,    49,    50,  -170,   -42,    51,    52,    53,     0,  -294,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -294,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,     0,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -294,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   773,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -294,    47,     0,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -294,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -294,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -294,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,   776,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -294,    47,     0,    49,    50,  -170,   -42,    51,
      52,    53,     0,  -294,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -294,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -294,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   779,    33,     0,    34,    35,     0,    36,  -170,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -294,    47,     0,    49,    50,  -170,
     -42,    51,    52,    53,     0,  -294,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -294,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,     0,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -294,    21,
      22,    23,     0,   801,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -294,    47,     0,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -294,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -294,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -294,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,   807,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -294,    47,
       0,    49,    50,  -170,   -42,    51,    52,    53,     0,  -294,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -294,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,     0,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -294,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   811,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -294,    47,     0,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -294,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -294,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,   689,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   151,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,   215,     0,     0,     0,    85,     0,     0,     0,
     156,     0,    87,     0,   157,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   151,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   152,   153,
       0,   154,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   215,     0,     0,     0,    85,     0,
       0,     0,   156,     0,    87,     0,   157,   593,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   151,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   155,
       0,     0,     0,     0,     0,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,   215,     0,     0,
       0,    85,     0,     0,     0,   156,     0,    87,     0,   157,
     913,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     151,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   154,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   155,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,     0,     0,     0,    85,     0,     0,     0,   156,     0,
      87,     0,   157,   598,     5,     6,     7,     8,   179,    10,
     180,     0,     0,   151,     0,     0,    14,    15,     0,     0,
     -73,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   152,   153,     0,
     154,    33,     0,    34,     0,     0,    36,  -465,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,  -465,     0,
       0,    52,   -73,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,     0,     0,
       0,   181,    68,     0,   -73,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,     0,     0,     0,     0,    85,     0,     0,
       0,   156,     0,    87,     0,   157,     5,     6,     7,     8,
     179,    10,     0,     0,     0,   151,     0,     0,    14,    15,
       0,     0,   -73,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,  -465,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
    -465,     0,     0,    52,   -73,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,     0,     0,   181,    68,     0,   -73,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   151,    82,    83,    14,    15,     0,     0,    85,
       0,     0,    19,   156,     0,    87,     0,   157,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,   219,   220,
       0,    68,   258,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,   259,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,   215,     0,     0,     0,    85,     0,     0,     0,
     156,     0,    87,     0,   157,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   151,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   152,   153,
       0,   154,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
     219,   220,     0,    68,   258,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,   215,     0,     0,     0,    85,     0,
       0,     0,   156,     0,    87,   587,   157,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   151,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,   219,   220,     0,    68,   258,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,   215,     0,     0,     0,
      85,     0,     0,     0,   156,     0,    87,   589,   157,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   151,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
     491,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,   492,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,   493,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,   494,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   151,    82,    83,    14,    15,
       0,     0,    85,     0,     0,    19,   156,     0,    87,     0,
     157,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,   219,   220,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,     0,   221,     0,     0,    85,
       0,     0,     0,   156,     0,    87,     0,   157,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   151,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   155,
       0,     0,     0,   219,   220,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,   215,     0,     0,
       0,    85,     0,     0,     0,   156,     0,    87,     0,   157,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   151,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   152,   153,     0,   154,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,   691,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   155,     0,     0,     0,   219,   220,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     5,     6,     7,
       8,     9,    10,    80,     0,    81,   151,    82,    83,    14,
      15,     0,     0,    85,     0,     0,    19,   156,     0,    87,
       0,   157,     0,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,   184,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,   185,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,     0,     0,     0,
      85,     0,     0,     0,   156,     0,    87,     0,   157,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   151,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,   189,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,   190,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   151,    82,    83,    14,    15,
       0,     0,    85,     0,     0,    19,   156,     0,    87,     0,
     157,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,  -465,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
    -465,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   151,    82,    83,    14,    15,     0,     0,    85,
       0,     0,    19,   156,     0,    87,     0,   157,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,   219,   220,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   151,
      82,    83,    14,    15,     0,     0,    85,     0,     0,    19,
     156,     0,    87,     0,   157,     0,     0,    25,     0,     0,
       0,     0,     0,   152,   153,     0,   154,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   155,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,   215,
       0,     0,     0,    85,     0,     0,     0,   156,   585,    87,
       0,   157,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   151,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,   201,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,    86,
       0,    87,     0,   157,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,   215,     0,
       0,     0,    85,     0,     0,     0,   156,     0,    87,     0,
     157,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     151,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   154,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   155,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
     185,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   151,    82,    83,
      14,   859,     0,     0,    85,     0,     0,    19,   156,     0,
      87,     0,   157,     0,     0,   860,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,   861,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   862,   863,    60,    61,    62,   864,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,   215,     0,     0,
       0,    85,     0,     0,     0,   156,     0,    87,     0,   866,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   151,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   152,   153,     0,   154,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   155,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,   215,
       0,     0,     0,    85,     0,     0,     0,   156,     0,    87,
       0,   866,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   151,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   844,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,   215,     0,     0,     0,    85,     0,     0,     0,   156,
       0,    87,     0,   157,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   151,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   152,   153,     0,
     154,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     151,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,   156,     0,    87,     0,   157,     0,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   154,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   155,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   151,    82,    83,
      14,   457,     0,     0,    85,     0,     0,    19,    86,     0,
      87,     0,   157,     0,     0,    25,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   155,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   458,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   151,    82,    83,    14,    15,     0,
       0,    85,     0,     0,    19,   156,     0,    87,     0,   157,
       0,     0,    25,     0,     0,     0,     0,     0,   152,   153,
       0,   154,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   151,    82,    83,    14,   557,     0,     0,   530,     0,
       0,    19,   156,     0,    87,     0,   157,     0,     0,   558,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
     559,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,   560,   561,
      60,    61,    62,   562,     0,     0,     0,     0,     0,     0,
      68,     0,     0,   563,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   564,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     5,     6,     7,     8,
       9,    10,    80,     0,    81,   151,    82,    83,    14,    15,
       0,     0,    85,     0,     0,    19,   156,     0,    87,     0,
     564,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   634,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   151,    82,    83,    14,    15,     0,     0,    85,
       0,     0,    19,   156,     0,    87,     0,   157,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   637,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       5,     6,     7,     8,     9,    10,    80,     0,    81,   151,
      82,    83,    14,    15,     0,     0,    85,     0,     0,    19,
     156,     0,    87,     0,   157,     0,     0,    25,     0,     0,
       0,     0,     0,   152,   153,     0,   838,    33,     0,    34,
       0,     0,    36,     0,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   155,     0,     0,     0,     0,     0,     0,    68,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     5,     6,     7,
       8,     9,    10,    80,     0,    81,   151,    82,    83,    14,
      15,     0,     0,    85,     0,     0,    19,   156,     0,    87,
       0,   157,     0,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   839,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   151,    82,    83,    14,    15,     0,     0,
      85,     0,     0,    19,   156,     0,    87,     0,   157,     0,
       0,    25,     0,     0,     0,     0,     0,   152,   153,     0,
     841,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     151,    82,    83,    14,    15,     0,     0,    85,     0,     0,
      19,   156,     0,    87,     0,   157,     0,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   842,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   155,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     5,     6,
       7,     8,     9,    10,    80,     0,    81,   151,    82,    83,
      14,    15,     0,     0,    85,     0,     0,    19,   156,     0,
      87,     0,   157,     0,     0,    25,     0,     0,     0,     0,
       0,   152,   153,     0,   843,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,   155,
       0,     0,     0,     0,     0,     0,    68,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   151,    82,    83,    14,    15,     0,
       0,    85,     0,     0,    19,   156,     0,    87,     0,   157,
       0,     0,    25,     0,     0,     0,     0,     0,   152,   153,
       0,   844,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   151,    82,    83,    14,   926,     0,     0,    85,     0,
       0,    19,   156,     0,    87,     0,   157,     0,     0,   927,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
     928,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,   929,   930,
      60,    61,    62,   931,     0,     0,     0,     0,     0,     0,
      68,     0,     0,   932,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   933,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,   721,     0,     5,     0,     0,    68,     0,     0,
      70,   722,     0,     0,     0,     0,    14,     0,   723,     0,
      74,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,    33,     0,    79,     0,     0,    36,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,     0,
       0,     0,    85,     0,     0,     0,     0,     0,    87,     0,
     933,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     285,   286,     0,     0,   287,   288,   289,   290,   291,   292,
     724,   725,   726,   727,     0,     0,   728,     0,     0,     0,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,   741,   742,   743,   744,     0,
     745,     0,     0,   746,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   151,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   152,   153,     0,
     154,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,     0,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    80,     0,    81,
     151,     0,    83,    14,    15,     0,     0,    85,     0,     0,
      19,   156,     0,    87,     0,   157,     0,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   154,    33,     0,
      34,     0,     0,    36,     0,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,    46,
       0,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   155,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    70,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,  -343,     0,   151,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,    76,     0,
       0,     0,  -343,    25,     0,     0,  -343,     0,     0,   152,
     153,     0,   154,    33,     0,    34,    81,     0,    36,     0,
     215,    38,    39,     0,    41,     0,    42,    43,   156,     0,
      87,  -343,   866,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,  -313,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
    -313,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,  -313,     0,     0,     0,     0,     0,   281,   282,   283,
     284,   285,   286,   324,   325,   287,   288,   289,   290,   291,
     292,     0,     0,    76,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   151,     0,     0,    14,    15,     0,
       0,    81,     0,     0,    19,     0,  -313,     0,     0,   326,
     327,  -445,    25,  -313,  -474,    87,  -474,   933,   152,   153,
       0,   154,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   151,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,    76,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
      81,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,   156,     0,    87,    46,   157,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   151,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,    76,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,    81,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     5,   156,     0,    87,    46,   564,    47,
     722,    49,     0,     0,     0,     0,    52,   723,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,   281,   282,   283,   284,   285,   286,   324,
     325,   287,   288,   289,   290,   291,   292,     0,     0,     0,
       0,   296,     0,     0,     0,     0,    76,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,    81,   326,   327,     0,     0,     0,
    -474,     0,  -474,     0,     0,     0,     0,     0,    87,     0,
     157,     0,     0,     0,     0,   281,   282,   283,   284,   285,
     286,     0,     0,   287,   288,   289,   290,   291,   292,   724,
     725,   726,   727,   299,     0,   728,     0,   300,     0,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,   296,   741,   742,   743,   744,     0,   745,
     297,     0,     0,     0,     0,   301,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   893,   299,     0,     0,     0,   300,
       0,     0,     0,     0,   296,     0,     0,     0,   371,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,   301,   281,   282,
     283,   284,   285,   286,     0,     0,   287,   288,   289,   290,
     291,   292,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   299,   321,   322,   323,
     300,     0,     0,   340,     0,   296,     0,     0,     0,   770,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   301,   281,
     282,   283,   284,   285,   286,     0,     0,   287,   288,   289,
     290,   291,   292,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,     0,     0,   340,     0,   296,     0,     0,     0,
     775,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,     0,     0,   301,
     281,   282,   283,   284,   285,   286,     0,     0,   287,   288,
     289,   290,   291,   292,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,   340,     0,   296,     0,     0,
       0,   905,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     301,   281,   282,   283,   284,   285,   286,     0,     0,   287,
     288,   289,   290,   291,   292,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,     0,     0,   340,     0,   296,     0,
       0,     0,   906,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,   301,   281,   282,   283,   284,   285,   286,     0,     0,
     287,   288,   289,   290,   291,   292,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,   340,     0,   296,
       0,     0,     0,   907,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   301,   281,   282,   283,   284,   285,   286,     0,
       0,   287,   288,   289,   290,   291,   292,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,   340,     0,
     296,     0,     0,     0,   908,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   301,   281,   282,   283,   284,   285,   286,
       0,     0,   287,   288,   289,   290,   291,   292,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,   340,
       0,   296,     0,     0,     0,   924,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,   301,   281,   282,   283,   284,   285,
     286,     0,     0,   287,   288,   289,   290,   291,   292,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   299,   321,   322,   323,   300,     0,     0,
     340,     0,   296,     0,     0,     0,   925,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,   363,     0,     0,     0,   301,   281,   282,   283,   284,
     285,   286,     0,   364,   287,   288,   289,   290,   291,   292,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   299,   321,   322,   323,   300,     0,
       0,   340,     0,     0,     0,     0,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,   365,     0,     0,     0,   301,   298,   360,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,     0,   321,   322,   323,     0,
       0,   299,   340,     0,     0,   300,     0,     0,     0,     0,
     296,     0,     0,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   660,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,   340,
       0,     0,     0,     0,     0,     0,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
     365,     0,     0,     0,   301,   298,   201,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,     0,   321,   322,   323,   298,     0,   299,
     340,     0,     0,   300,     0,     0,     0,     0,   296,     0,
       0,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,   654,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   299,     0,     0,     0,   300,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,   340,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,   298,   309,   310,
     311,   312,   301,   314,   315,   316,   317,     0,   318,     0,
     320,     0,     0,   321,   322,   323,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,   340,     0,
     296,     0,     0,     0,   672,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,   340,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   299,   321,   322,   323,   300,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,     0,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,   301,     0,     0,     0,     0,
     298,     0,     0,     0,     0,   368,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   369,
     318,   319,   320,     0,   296,   321,   322,   323,     0,     0,
    -446,   297,     0,     0,   299,     0,     0,     0,   300,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   342,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,   303,   304,   305,   299,   306,   307,   308,
     300,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   370,   321,   322,   323,   296,
       0,     0,     0,     0,   146,     0,   297,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   411,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,   412,     0,   321,   322,
     323,   296,     0,     0,     0,     0,     0,     0,   297,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   298,
     500,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,     0,   321,   322,   323,     0,     0,   297,     0,
       0,     0,     0,     0,     0,   301,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,   296,     0,   321,   322,   323,     0,     0,
     297,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,   298,   502,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   503,     0,     0,   296,     0,     0,
       0,     0,     0,     0,   297,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     302,     0,   303,   304,   305,   299,   306,   307,   308,   300,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   402,   321,   322,   323,     0,   296,
       0,     0,     0,     0,     0,     0,   297,   301,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   301,   318,   319,   320,     0,     0,   321,   322,   323,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
     404,   321,   322,   323,     0,     0,   297,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,   406,   321,   322,   323,     0,     0,   297,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,   414,   321,   322,   323,     0,     0,   297,     0,
       0,     0,     0,     0,   429,   301,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,   296,     0,   321,   322,   323,     0,     0,
     297,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,   298,     0,     0,     0,     0,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   296,     0,     0,
       0,     0,     0,     0,   297,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     302,     0,   303,   304,   305,   299,   306,   307,   308,   300,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,     0,   321,   322,   323,     0,   296,
     667,     0,     0,   146,     0,     0,   297,   301,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   301,   318,   319,   320,     0,     0,   321,   322,   323,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
     644,   321,   322,   323,     0,     0,   297,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,   298,     0,     0,
     670,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,     0,   321,   322,   323,     0,     0,   297,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   760,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,     0,     0,     0,
       0,     0,     0,     0,   711,   301,     0,   299,     0,     0,
       0,   300,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   301,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,   296,     0,   321,
     322,   323,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,   763,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,   298,     0,     0,     0,     0,   299,     0,
       0,     0,   300,     0,     0,     0,   302,     0,   303,   304,
     305,   837,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     301,   321,   322,   323,     0,     0,     0,   299,     0,     0,
       0,   300,     0,     0,     0,   302,     0,   303,   304,   305,
     840,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   301,
     321,   322,   323,     0,     0,   296,   854,     0,     0,     0,
       0,     0,   297,     0,   302,     0,   303,   304,   305,     0,
     306,   307,   308,   298,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,     0,   321,
     322,   323,     0,     0,   296,     0,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,   299,     0,     0,
       0,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,   301,
       0,   298,     0,     0,     0,     0,   299,     0,     0,     0,
     300,     0,     0,     0,   302,     0,   303,   304,   305,   941,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   301,   321,
     322,   323,     0,     0,     0,   299,     0,     0,     0,   300,
       0,     0,     0,   302,     0,   303,   304,   305,   942,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   301,   321,   322,
     323,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,   298,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,     0,   321,   322,   323,
       0,     0,   296,     0,     0,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,   299,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,   943,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,   298,
       0,     0,     0,     0,   299,     0,     0,     0,   300,     0,
       0,     0,   302,     0,   303,   304,   305,   944,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   301,   321,   322,   323,
       0,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,   302,     0,   303,   304,   305,   945,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   301,   321,   322,   323,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,   298,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,     0,   321,   322,   323,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,     0,     0,     0,     0,     0,   946,     0,   296,     0,
       0,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,   298,     0,     0,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,   301,   298,   321,   322,   323,     0,     0,
     632,     0,     0,     0,   300,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   301,   321,   322,   323,     0,     0,   299,     0,
       0,     0,   300,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
     301,     0,   321,   322,   323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,     0,
     314,   315,   316,   317,     0,   318,     0,   320,     0,     0,
     321,   322,   323
};

static const yytype_int16 yycheck[] =
{
      11,     2,    50,    51,    15,   407,   264,    18,    17,   528,
       1,   688,    23,   170,     3,     3,    27,    28,    29,    30,
      31,    32,   122,    54,   181,     1,   379,     1,   384,    40,
      41,    42,   132,    44,   333,    46,    47,     3,     3,     3,
      71,   714,   100,    54,    55,    56,    57,    58,    59,   556,
     376,   407,    63,    64,    65,    66,   194,    88,    29,    70,
      71,    72,    73,    24,   913,    76,   750,   751,    79,    80,
      81,   124,    83,   211,     1,    86,    87,    88,   755,   277,
     278,   279,   280,    29,    58,    73,    29,    41,     3,    29,
     790,     1,   230,   105,    75,   963,   124,   155,     3,    75,
     153,    62,   874,    92,   972,   124,    41,    41,   143,   958,
     629,    87,    73,    58,   105,    30,   128,   794,    41,    24,
      19,     0,    98,    97,   631,    29,   157,   155,   152,   100,
     154,    41,   123,   124,   153,    96,    41,   237,   276,    44,
     151,   152,   153,   154,   155,   156,   157,   123,    75,    75,
      73,   824,    97,    41,    59,    28,    41,    62,   858,    64,
     169,    41,   153,   152,   152,   176,   100,   143,    73,   150,
     124,    98,    71,   846,   150,    90,   100,   151,   685,   150,
     857,   124,    19,   194,   150,    90,   152,   152,   152,   124,
     124,   329,   330,   965,    93,   105,   123,   369,    41,    72,
     211,   212,   555,    76,   150,    55,   151,   150,   219,   220,
     150,   895,   896,   123,   124,   124,   124,   143,   143,   230,
     155,   155,   124,   150,   150,   958,   151,    65,    66,   528,
      73,   124,   124,   143,    71,   912,   124,    87,    46,   124,
     412,   439,   151,   976,   124,   153,   119,   152,   259,   122,
     123,   153,   125,   126,   124,   256,    93,   130,   124,    67,
     153,   153,   400,   782,   137,   276,   143,   155,   526,   527,
     155,   144,   145,   146,   151,   155,    72,   143,     3,    90,
      76,    28,   293,   153,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   124,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   151,    72,    72,   679,   358,    76,    76,
     629,   342,   124,   591,   592,   346,    75,   348,   124,   152,
     152,   154,    24,    24,   124,   105,   152,   358,    87,   360,
     361,   677,   363,   364,    75,     7,    83,   368,   369,    98,
     371,   153,   373,   143,   124,     2,    87,   153,   128,   152,
       1,   154,     3,   152,    11,   386,   123,    98,   125,   126,
      62,    62,   393,   130,   152,    22,    28,    24,   714,   400,
     137,    73,    73,   153,    68,   299,   300,   152,   800,   146,
     411,   412,   143,   434,   143,   124,   143,   418,    90,    90,
     421,   150,   423,   105,   425,   426,   427,   143,   429,   430,
     124,    28,   143,   434,   143,   582,    63,   331,   332,   150,
      72,   124,   124,   149,    76,   124,   128,   793,   342,   143,
      75,   143,    40,   293,   800,   124,   457,   458,   143,    86,
     143,   124,    87,    90,   143,   123,   143,   361,   126,    57,
     364,   153,   130,    98,   143,    72,   124,    65,    66,    76,
     143,   143,    87,   143,   324,   325,   326,   327,   328,   143,
     152,   123,   154,   782,   126,   143,   152,   126,   130,   500,
     501,   502,   503,   504,   505,   506,    28,   150,   507,   508,
     143,   152,   513,   829,   515,    99,   152,   518,   143,   520,
     147,   143,   612,   524,   152,   150,   123,   528,   125,   126,
     846,   425,   105,   130,   421,   143,   423,    52,    41,     7,
     137,   123,   143,   564,   890,   105,   150,   144,   145,   146,
      72,   153,   124,   143,    76,    41,   557,   152,   559,   560,
     561,   562,   563,   564,    31,   124,   567,   913,   195,   105,
     153,   176,    75,   153,   201,   153,   922,    41,   153,    18,
     168,   421,   124,   423,   155,   150,   153,    57,   143,   194,
     143,   143,   143,   143,   143,    98,   153,   119,   186,   187,
     188,   123,   105,   125,   126,   193,   211,   501,   130,   503,
     155,    75,   958,   143,   124,   137,   204,   963,   143,   207,
      41,   124,   144,   145,   146,   230,   972,   143,   629,   256,
     976,   632,   633,   634,    98,     2,   637,    31,   226,   153,
     143,   105,   151,   143,    11,   153,   143,   150,   150,   124,
     153,   652,   151,   654,    75,    22,   657,    24,   246,   660,
     124,   105,    73,    29,   665,    29,   667,   155,   150,   670,
      31,   276,   123,   513,   695,    73,   155,    98,   123,   143,
     151,   155,   155,   152,   105,   126,   150,    31,   126,   153,
     153,    98,   153,   151,   695,    87,    63,   712,    65,    66,
     155,    21,   412,   124,   411,   706,   238,   708,   517,   699,
     711,    24,   768,   340,   717,   716,   186,   187,   188,    86,
     182,   612,   143,    90,   329,   330,   833,   567,    41,   150,
     829,    44,   153,   360,   204,   677,   363,   207,   632,   633,
     548,   965,   912,   833,   371,   746,    59,   720,   937,    62,
      31,    64,   387,   515,    27,   343,   226,    -1,   759,   760,
      73,   762,   763,   764,   765,    -1,    -1,    -1,    -1,   770,
     771,    -1,   773,    -1,   775,   776,   246,    90,   779,   367,
     147,   782,    -1,   821,   822,    28,    -1,    -1,   393,    -1,
      -1,    -1,    -1,    -1,    -1,   400,    -1,    -1,   799,   800,
     427,    -1,    -1,    -1,   805,    -1,   807,    -1,    -1,    -1,
     811,    -1,    23,   418,    -1,    -1,   421,    -1,   423,   820,
      -1,   426,    -1,    34,    35,    -1,    -1,   818,   195,    72,
      -1,    -1,    43,    76,   201,    46,   837,   838,   839,   840,
     841,   842,   843,   844,    -1,   866,    -1,    -1,    -1,    60,
      -1,    -1,    -1,   854,    -1,    -1,    67,    -1,   859,    -1,
     861,   862,   863,   864,   865,   866,    -1,    -1,    -1,   870,
     497,    -1,   873,   343,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,    -1,   130,    -1,   256,
      -1,    -1,    -1,   136,   137,    -1,    -1,   367,   141,    -1,
      -1,   144,   145,   146,   905,   906,   907,   908,   513,    -1,
     515,   538,   933,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   922,    28,   924,   925,   926,    -1,   928,   929,   930,
     931,   932,   933,    -1,   522,   523,    -1,    -1,   526,   527,
     941,   942,   943,   944,   945,   946,    -1,    -1,    24,    -1,
      -1,   539,    -1,    -1,    -1,    -1,   957,   958,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    72,    -1,    44,   596,
      76,   972,    -1,   340,    -1,   976,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    -1,   612,    62,    -1,    64,    -1,
     578,    -1,    -1,   360,    -1,    -1,   363,    73,    -1,    -1,
      10,    -1,    -1,    -1,   371,   632,   633,    17,    -1,    -1,
      -1,    -1,     2,   119,    90,   121,   122,   123,    28,   125,
     126,    11,    -1,    -1,   130,   652,    -1,   654,    -1,    -1,
     657,   137,    22,   660,    24,    -1,    -1,    -1,   144,   145,
     146,    -1,   669,    -1,    -1,   672,    -1,    -1,    -1,    -1,
      -1,    -1,   522,   523,   642,   643,   526,   527,    -1,    -1,
     427,   649,    72,   651,    -1,    -1,    76,   694,   656,    -1,
      -1,   659,    -1,    63,   662,    65,    66,     2,    -1,   706,
      -1,   708,    -1,    -1,    -1,   712,    11,    -1,    -1,    -1,
     100,    -1,    -1,    -1,   104,    -1,    86,    22,    -1,    24,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,   702,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    63,    -1,
      23,    -1,    -1,   770,    -1,   155,    -1,    -1,   775,    -1,
      -1,    34,    35,    -1,    -1,    -1,    -1,   147,    -1,    -1,
      43,    86,   789,    46,    -1,    90,    -1,    -1,    -1,    52,
      -1,   538,   642,   643,   801,    -1,    -1,    60,    -1,   649,
      -1,    -1,    -1,   810,    67,    68,    -1,   814,   793,   659,
      -1,   818,   662,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    23,    -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,
      32,   201,    34,    35,    23,   803,    38,    -1,    -1,    -1,
      -1,    43,   147,    -1,    46,    34,    35,    -1,   855,   596,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    60,    -1,
      -1,    -1,    64,    52,    66,    67,    -1,    69,    -1,    71,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
     848,   849,    -1,   851,   852,   870,   256,    -1,    90,    -1,
     195,    93,    -1,    -1,    -1,    -1,   201,    -1,   905,   906,
     907,   908,    -1,    -1,    -1,   652,   874,   654,    -1,    -1,
     657,    -1,   880,   660,   882,    -1,   884,   924,   925,    -1,
       2,    -1,   669,    -1,    -1,   672,    -1,    -1,   913,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,   143,    24,    -1,    -1,    -1,    -1,   694,    -1,   151,
      -1,   256,    -1,   803,    -1,    -1,    -1,    -1,    -1,   706,
      -1,   708,    -1,    -1,    -1,   712,    -1,    -1,    -1,    -1,
     340,    -1,    -1,   958,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    63,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
     360,   976,    32,   363,    34,    35,    -1,   965,    38,    -1,
      -1,   371,    -1,    43,    86,    -1,    46,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,   770,    64,    -1,    66,    67,   775,    69,
      -1,    71,    -1,    -1,    -1,   340,   105,   106,   107,   108,
     109,   110,   789,    -1,   113,   114,   115,   116,   117,   118,
      90,    -1,    -1,    93,   801,   360,    -1,   427,   363,    -1,
      -1,    -1,     1,   810,    -1,   147,   371,   814,     2,    -1,
      -1,   818,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      19,    -1,    -1,    -1,    23,    24,    -1,    -1,    22,    -1,
      24,    -1,    -1,    32,    -1,    34,    35,    -1,    -1,    38,
      -1,    -1,    -1,   143,    43,    -1,    -1,    46,   855,    -1,
      -1,   151,    -1,   195,    -1,    -1,    -1,    -1,    -1,   201,
      -1,    60,   427,    62,    -1,    64,    -1,    66,    67,    63,
      69,    -1,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    86,    -1,    93,    -1,    90,    96,   905,   906,
     907,   908,    -1,    -1,    -1,    -1,    -1,    -1,   538,    19,
      -1,    -1,    -1,    23,   256,    -1,    -1,   924,   925,    -1,
      -1,    -1,    32,    -1,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    28,   143,    -1,    -1,    -1,    -1,    -1,
      60,    -1,   151,   147,    64,    -1,    66,    67,    -1,    69,
      -1,    71,    -1,    -1,    -1,    -1,   596,    -1,    -1,    -1,
      -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    93,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,     2,    -1,    -1,    -1,    -1,   340,    -1,
      -1,   195,    11,    -1,    -1,    -1,    -1,   201,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    24,    -1,    -1,   360,    -1,
      -1,   363,   652,    -1,   654,    -1,    -1,   657,    -1,   371,
     660,   596,    -1,   143,   119,    -1,   121,   122,   123,   669,
     125,   126,   672,    -1,    -1,   130,    -1,    -1,    -1,    -1,
      -1,   136,   137,    -1,    63,    -1,   141,    -1,    -1,   144,
     145,   146,   256,    -1,   694,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   706,    86,   708,    -1,
      -1,    90,   712,    -1,    -1,   427,    -1,   652,    -1,   654,
       1,    -1,   657,    -1,    -1,   660,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   669,    -1,    -1,   672,    19,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    34,    35,    -1,    -1,    38,    -1,   694,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,   147,    -1,
     770,   706,    -1,   708,    -1,   775,   340,   712,    -1,    60,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,   789,
      71,    -1,    73,    -1,    -1,    -1,   360,    -1,    -1,   363,
      -1,   801,    -1,    -1,    -1,    -1,    -1,   371,    -1,    90,
     810,     2,    93,    -1,   814,    96,   195,    -1,   818,    -1,
      11,    -1,   201,    -1,    -1,    -1,   538,    -1,    -1,    -1,
       2,    22,    -1,    24,    -1,   770,    -1,    -1,    -1,    11,
     775,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    24,    -1,   789,   855,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   427,    -1,    -1,   801,    -1,    -1,    -1,
     151,    -1,    63,    -1,    -1,   810,    -1,   256,    -1,   814,
      -1,    -1,    -1,   818,   596,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,   905,   906,   907,   908,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,
     855,    -1,    -1,    -1,   924,   925,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     652,    -1,   654,    -1,    -1,   657,    -1,    -1,   660,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   147,   669,    -1,    -1,
     672,   340,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     905,   906,   907,   908,   538,   147,    -1,    -1,    -1,    -1,
      -1,   360,   694,    -1,   363,    -1,    -1,    -1,    -1,   924,
     925,    -1,   371,    -1,   706,    -1,   708,    -1,    10,    -1,
     712,    -1,    -1,    -1,   195,    17,    -1,    -1,    -1,    -1,
     201,    -1,    -1,    -1,    -1,     2,    28,    -1,    -1,    -1,
      -1,    -1,    -1,   195,    11,    -1,    -1,    -1,    -1,   201,
      -1,    -1,   596,    -1,    -1,    22,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   427,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   770,    -1,
      72,    -1,    -1,   775,    76,   256,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,   789,    -1,    -1,
      -1,    -1,    -1,    -1,   256,    -1,    -1,    -1,   652,   801,
     654,    -1,   104,   657,    -1,    -1,   660,    -1,   810,    86,
      -1,    -1,   814,    90,    -1,   669,   818,   119,   672,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
     694,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,   153,   706,   855,   708,    -1,    -1,    -1,   712,   340,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,
     147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,   360,
      -1,    -1,   363,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     371,    -1,    -1,    10,    -1,    -1,    -1,    -1,   360,    -1,
      17,   363,    -1,   905,   906,   907,   908,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   770,    -1,   195,    -1,
      -1,   775,   924,   925,   201,    -1,    -1,   596,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   789,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   427,   801,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,   810,    -1,    -1,    76,
     814,    -1,    -1,    -1,   818,   427,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,
      -1,    -1,    -1,   652,    -1,   654,    -1,   104,   657,    -1,
      -1,   660,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     669,   855,   119,   672,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,   694,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   706,   155,   708,
      -1,    -1,    -1,   712,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   905,   906,   907,   908,    -1,    -1,   538,    -1,    -1,
      -1,    -1,    -1,   340,    -1,    -1,    -1,    -1,    -1,    -1,
     924,   925,    -1,    -1,    -1,    -1,   538,    -1,    -1,    -1,
      -1,    -1,    -1,   360,    -1,    -1,   363,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   371,    -1,    -1,    -1,    -1,    -1,
      -1,   770,    -1,    -1,    -1,    -1,   775,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   596,    -1,    -1,    -1,    -1,
     789,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   801,    -1,   596,    -1,    -1,    -1,    -1,    -1,
      -1,   810,    -1,    -1,    -1,   814,    -1,    -1,    -1,   818,
     427,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,   652,    -1,   654,    -1,    28,   657,    -1,    -1,   660,
      -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,   669,    -1,
     652,   672,   654,    -1,    -1,   657,    -1,    -1,   660,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   669,    -1,    -1,
     672,    -1,    -1,   694,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,   706,    -1,   708,    -1,    -1,
      -1,   712,   694,    -1,    -1,    -1,   905,   906,   907,   908,
      -1,    -1,    -1,    -1,   706,    -1,   708,    -1,    -1,    -1,
     712,   104,    -1,    -1,    -1,   924,   925,    -1,    -1,    -1,
      -1,   538,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,   770,
      13,   144,   145,   146,   775,    -1,    -1,    -1,    21,    -1,
      -1,    -1,   155,    28,    -1,    -1,    -1,    -1,   789,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    39,    -1,    -1,   596,
     801,    -1,    -1,    -1,    -1,    -1,    -1,   789,    -1,   810,
      -1,    -1,    -1,   814,    -1,    -1,    -1,   818,    -1,   801,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    72,   810,    -1,
      -1,    76,   814,    -1,    -1,    -1,   818,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   855,   652,    -1,   654,    -1,   104,
     657,    -1,    -1,   660,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,   669,   855,   119,   672,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,   694,    -1,   144,
     145,   146,    -1,    -1,   905,   906,   907,   908,    -1,   706,
      -1,   708,    -1,    -1,    -1,   712,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   924,   925,    -1,    -1,   170,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,   181,    -1,
      -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,    -1,
      -1,    -1,   215,   770,    -1,    28,    -1,    -1,   775,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,
      -1,    -1,   789,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   801,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   810,    -1,    -1,    -1,   814,    -1,    72,
      -1,   818,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   276,   277,   278,   279,   280,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,   855,    -1,
      -1,    -1,    -1,    -1,   307,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,    -1,   329,   330,   141,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,   905,   906,
     907,   908,    -1,   356,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   924,   925,    -1,
      -1,    -1,    -1,   104,    -1,    -1,   379,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,   400,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,
     433,    -1,    -1,   436,    -1,    -1,   439,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,   515,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    -1,    88,    89,
      90,    91,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   551,    -1,
      -1,    -1,   555,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   578,   136,    -1,   138,   582,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,   612,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,   676,    -1,    -1,   679,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,   699,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   717,    -1,    -1,   119,   120,   121,
     122,    -1,   725,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
     833,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    -1,    88,    89,    90,    91,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,    -1,    -1,   150,   151,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    -1,    88,    89,    90,    91,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
     142,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    -1,    88,    89,
      90,    91,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,   151,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    -1,
      88,    89,    90,    91,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    -1,    88,    89,    90,    91,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,   151,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    -1,    88,    89,    90,    91,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,   150,   151,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    -1,    88,    89,    90,    91,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    -1,    88,    89,
      90,    91,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    -1,
      88,    89,    90,    91,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    -1,    88,    89,    90,    91,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    -1,    88,    89,    90,    91,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    -1,    88,    89,    90,    91,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    29,
      30,    -1,    32,    -1,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    43,    -1,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    -1,    56,    -1,    -1,    -1,
      60,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    71,    -1,    73,    74,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    -1,    86,    -1,    88,    89,
      90,    91,    -1,    93,    94,    95,    96,    -1,    -1,    99,
      -1,   101,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,     1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,    -1,
      88,    89,    90,    91,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    -1,    88,    89,    90,    91,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    -1,    73,
      74,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    -1,    86,    -1,    88,    89,    90,    91,    -1,    93,
      94,    95,    96,    -1,    -1,    99,    -1,   101,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,   143,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,     1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    43,    -1,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    71,
      -1,    73,    74,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    -1,    86,    -1,    88,    89,    90,    91,
      -1,    93,    94,    95,    96,    -1,    -1,    99,    -1,   101,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,     1,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,   155,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
     155,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,   155,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    46,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    46,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    88,    89,
      -1,    91,    92,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      88,    89,    -1,    91,    92,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,   153,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    88,    89,    -1,    91,    92,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,   153,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    41,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    15,    16,
      -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    88,    89,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    15,
      16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    15,    16,
      -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    46,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    88,    89,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,   142,    -1,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,   142,
      -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,    15,    16,    -1,
      -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,    15,    16,    -1,    -1,   146,    -1,
      -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    15,    16,
      -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
       3,     4,     5,     6,     7,     8,   136,    -1,   138,    12,
     140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,
       6,     7,     8,   136,    -1,   138,    12,   140,   141,    15,
      16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,   152,
      -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    15,    16,    -1,    -1,
     146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,    15,    16,    -1,
      -1,   146,    -1,    -1,    22,   150,    -1,   152,    -1,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,    15,    16,    -1,    -1,   146,    -1,
      -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,     1,    -1,     3,    -1,    -1,    91,    -1,    -1,
      94,    10,    -1,    -1,    -1,    -1,    15,    -1,    17,    -1,
     104,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    40,    -1,   127,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,    -1,
      -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,   152,    -1,
     154,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,   141,    -1,    -1,   144,   145,   146,   147,    -1,
     149,    -1,    -1,   152,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,    -1,   141,    15,    16,    -1,    -1,   146,    -1,    -1,
      22,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    94,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   105,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,   120,    -1,
      -1,    -1,   124,    30,    -1,    -1,   128,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,   138,    -1,    45,    -1,
     142,    48,    49,    -1,    51,    -1,    53,    54,   150,    -1,
     152,   153,   154,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      87,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,   120,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,   138,    -1,    -1,    22,    -1,   143,    -1,    -1,   147,
     148,   149,    30,   150,   152,   152,   154,   154,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
     138,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,   150,    -1,   152,    61,   154,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,   138,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,     3,   150,    -1,   152,    61,   154,    63,
      10,    65,    -1,    -1,    -1,    -1,    70,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,   120,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   138,   147,   148,    -1,    -1,    -1,
     152,    -1,   154,    -1,    -1,    -1,    -1,    -1,   152,    -1,
     154,    -1,    -1,    -1,    -1,   105,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    72,    -1,   125,    -1,    76,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,   141,    -1,    10,   144,   145,   146,   147,    -1,   149,
      17,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    85,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    72,   144,   145,   146,    76,    -1,    -1,   150,
      -1,    10,    -1,    -1,    -1,    85,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    72,   144,   145,   146,    76,    -1,    -1,
     150,    -1,    10,    -1,    -1,    -1,    85,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,    41,   113,   114,   115,   116,   117,   118,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    72,   144,   145,   146,    76,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,   104,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    -1,   144,   145,   146,    -1,
      -1,    72,   150,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    72,   144,   145,   146,    76,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
     100,    -1,    -1,    -1,   104,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    28,    -1,    72,
     150,    -1,    -1,    76,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,   150,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    28,   129,   130,
     131,   132,   104,   134,   135,   136,   137,    -1,   139,    -1,
     141,    -1,    -1,   144,   145,   146,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,   150,    -1,
      10,    -1,    -1,    -1,    85,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    72,   144,   145,   146,    76,    -1,    -1,   150,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    72,   144,   145,   146,    76,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    57,
     139,   140,   141,    -1,    10,   144,   145,   146,    -1,    -1,
     149,    17,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    72,   125,   126,   127,
      76,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,   143,   144,   145,   146,    10,
      -1,    -1,    -1,    -1,   100,    -1,    17,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    33,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    57,    -1,   144,   145,
     146,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,    -1,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    10,    -1,   144,   145,   146,    -1,    -1,
      17,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    72,   125,   126,   127,    76,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   143,   144,   145,   146,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,   104,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   104,   139,   140,   141,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    72,   125,   126,   127,    76,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
     143,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,   143,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,   143,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    10,    -1,   144,   145,   146,    -1,    -1,
      17,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    72,   125,   126,   127,    76,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    10,
      11,    -1,    -1,   100,    -1,    -1,    17,   104,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   104,   139,   140,   141,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    72,   125,   126,   127,    76,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
     143,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    28,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,    -1,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   104,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    10,    -1,   144,
     145,   146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    28,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    85,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     104,   144,   145,   146,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      85,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,   104,
     144,   145,   146,    -1,    -1,    10,    11,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    28,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
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
     146,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    28,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    -1,   144,   145,   146,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
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
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    28,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,   104,    28,   144,   145,   146,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   104,   144,   145,   146,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
     104,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,    -1,
     134,   135,   136,   137,    -1,   139,    -1,   141,    -1,    -1,
     144,   145,   146
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   160,   161,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    16,    19,    20,    21,    22,
      23,    25,    26,    27,    29,    30,    32,    34,    35,    36,
      37,    38,    39,    40,    42,    43,    45,    47,    48,    49,
      50,    51,    53,    54,    56,    60,    61,    63,    64,    65,
      66,    69,    70,    71,    74,    75,    77,    78,    79,    80,
      81,    82,    83,    84,    86,    88,    89,    90,    91,    93,
      94,    95,    99,   101,   104,   119,   120,   121,   122,   127,
     136,   138,   140,   141,   143,   146,   150,   152,   154,   162,
     163,   164,   165,   166,   168,   173,   174,   175,   177,   180,
     181,   182,   183,   184,   185,   187,   188,   189,   193,   194,
     197,   198,   201,   202,   205,   208,   209,   213,   233,   236,
     237,   257,   258,   259,   260,   261,   262,   263,   271,   272,
     273,   274,   277,   278,   279,   280,   281,   282,   284,   285,
     286,   287,   288,   143,   164,   274,   100,   264,   265,   176,
     177,    12,    36,    37,    39,    84,   150,   154,   205,   257,
     258,   262,   272,   273,   274,   277,   279,   280,   264,   274,
      90,   176,   164,   251,   274,   164,   152,   274,   275,     7,
       9,    90,   178,   275,    62,   102,   186,   274,   274,    23,
      35,   236,   274,   274,   152,   177,   206,   152,   177,   238,
     239,    29,   168,   179,   274,   274,   274,   274,   274,   274,
       7,   152,    35,   182,   182,   142,   229,   250,   274,    88,
      89,   143,   274,   276,   274,   179,   274,   274,   274,    83,
     152,   164,   274,   274,   168,   175,   274,   277,   168,   175,
     274,   177,   234,   274,   250,   274,   274,   274,   274,   274,
     274,   274,   274,     1,   151,   162,   169,   250,    92,   128,
     229,   252,   253,   276,   250,   274,   283,    64,   164,    68,
     167,   177,   177,    46,    67,   224,   152,    24,    62,    73,
      96,   105,   106,   107,   108,   109,   110,   113,   114,   115,
     116,   117,   118,   216,   143,   149,    10,    17,    28,    72,
      76,   104,   119,   121,   122,   123,   125,   126,   127,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   139,   140,
     141,   144,   145,   146,   111,   112,   147,   148,   216,   152,
     154,    72,    76,   152,   164,   143,   274,   274,   274,   250,
     150,   168,    41,   264,   234,   143,   124,   143,    99,   177,
     229,   254,   255,   256,   276,   234,   198,   177,   152,   179,
      29,    41,   179,    29,    41,   100,   179,   267,    33,    57,
     143,    85,   168,   216,   254,   164,   152,   218,    92,   152,
     177,   240,   241,     1,   123,   245,    41,   124,   164,   179,
     179,   254,   177,   124,   143,   274,   274,   143,   150,   179,
     152,   254,   143,   190,   143,   190,   143,   105,   235,   143,
     143,    33,    57,   179,   143,   151,   151,   162,   124,   151,
     274,   124,   153,   124,   153,    41,   124,   155,   267,   103,
     124,   155,     7,    52,   123,   199,   150,   210,   254,    73,
     238,   238,   238,   238,   276,   274,   274,   274,   274,   186,
     274,   186,   274,   274,   274,   274,   274,    16,    94,   274,
     274,    30,    90,   177,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     276,   276,   276,   276,   276,   254,   254,   186,   274,   186,
     274,    24,    41,    73,    96,   266,   269,   270,   274,   288,
      29,    41,    29,    41,    85,    41,   155,   186,   274,   179,
     143,   274,   274,   105,   153,   124,   143,   177,    41,   250,
      39,   274,   186,   274,    39,   274,   186,   274,   152,   179,
     146,   170,   172,   274,   170,   171,   164,   274,    31,   274,
     153,    24,    41,    44,    59,    62,    73,    90,   163,   219,
     220,   221,   222,   207,   241,   124,   153,    16,    30,    61,
      79,    80,    84,    94,   154,   246,   259,   105,   242,   274,
     239,   153,   229,   274,     1,   195,   254,   153,    18,   191,
     246,   259,   124,   172,   171,   151,   153,   153,   252,   153,
     252,   186,   274,   155,   164,   274,   155,   274,   155,   274,
     177,   250,   150,     1,   177,   203,   204,    24,    62,    73,
      90,   212,   223,   153,   238,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   153,   155,    41,    73,   124,
     153,   257,    72,    72,    39,   186,   274,    39,   186,   274,
     186,   274,   264,   264,   143,   229,   276,   256,   199,   274,
     153,   274,    29,   179,    29,   179,   274,    29,   179,   267,
      29,   179,   267,   268,   269,   124,   143,    11,   143,    31,
      31,   164,    85,   168,    41,    73,   221,   124,   153,   152,
     177,    24,    62,    73,    90,   225,   153,   241,   245,     1,
     250,    55,   276,   151,    58,    97,   151,   196,   153,   152,
     168,   177,   192,   234,   143,   143,   155,   267,   155,   267,
     164,   103,   150,     1,   200,   151,   105,   124,   151,    73,
     211,     1,    10,    17,   119,   120,   121,   122,   125,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     141,   144,   145,   146,   147,   149,   152,   177,   213,   214,
     215,   216,   257,   143,   269,   245,   257,   257,   274,    29,
      29,   274,    29,    29,   155,   155,   179,   179,   150,   179,
      85,    39,   274,    39,   274,    85,    39,   274,   179,    39,
     274,   179,   124,   153,   274,   274,   164,   274,   164,    31,
     177,   220,   241,   123,   249,    73,   245,   242,   155,    41,
     155,    29,   164,   250,   192,   123,   168,    39,   164,   274,
     155,    39,   164,   274,   155,   274,     1,   151,   169,   151,
      35,    66,    69,   151,   163,   181,   274,   204,   225,   152,
     217,   177,   274,   126,   217,   217,   242,    85,    39,    39,
      85,    39,    39,    39,    39,     1,   200,   274,   274,   274,
     274,   274,   274,   269,    11,    31,   164,   249,   153,    16,
      30,    61,    79,    80,    84,    94,   154,   229,   248,   259,
     128,   231,   242,    98,   232,   274,   246,   259,   164,   179,
     153,   274,   274,   164,   274,   164,   151,   151,   181,     1,
     123,   244,   219,   153,   177,   215,   216,   274,   274,   274,
     274,   274,   274,   151,   151,    85,    85,    85,    85,   274,
     164,   242,   249,   155,   250,   229,   230,   274,   274,   168,
     180,   228,   155,   168,    85,    85,    16,    30,    61,    79,
      80,    84,    94,   154,   243,   259,    87,   226,   153,   217,
     217,    85,    85,    85,    85,    85,    85,   274,   274,   274,
     274,   231,   242,   229,   247,   248,   259,    41,   155,   259,
     274,   274,     1,   155,   250,   232,   274,   247,   248,   155,
     243,   259,   155,   143,   227,   228,   155,   243,   259,   247
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
     212,   212,   212,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   214,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   217,   217,   218,   219,   219,   219,   220,   220,   220,
     220,   220,   221,   221,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   223,   223,   223,   223,   223,   223,   224,
     224,   225,   225,   225,   225,   225,   225,   226,   226,   227,
     227,   228,   228,   229,   229,   230,   230,   231,   231,   232,
     232,   233,   233,   233,   234,   234,   235,   235,   235,   236,
     236,   236,   236,   236,   237,   237,   237,   238,   238,   239,
     239,   240,   240,   240,   241,   241,   241,   242,   242,   242,
     243,   243,   243,   243,   243,   243,   243,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   246,
     246,   246,   246,   247,   247,   247,   248,   248,   248,   248,
     248,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   250,   250,   250,   250,   251,   251,   252,   252,
     252,   253,   253,   254,   254,   255,   255,   256,   256,   256,
     256,   257,   258,   258,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   261,   262,
     263,   263,   263,   263,   263,   263,   263,   263,   264,   264,
     265,   266,   266,   267,   268,   268,   269,   269,   269,   270,
     270,   270,   270,   270,   270,   271,   271,   272,   273,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   275,   275,   276,   276,   276,
     277,   277,   277,   277,   278,   278,   279,   279,   279,   280,
     280,   280,   281,   281,   281,   281,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   283,   283,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   285,   285,   285,   285,   285,   285,   286,   286,
     286,   286,   287,   287,   287,   287,   288,   288,   288,   288,
     288,   288,   288
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
       5,     5,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     5,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     2,     1,     0,     1,     2,     2,     0,
       2,     3,     4,     4,     2,     4,     0,     2,     2,     4,
       4,     4,     5,     4,     0,     1,     1,     1,     3,     3,
       5,     1,     1,     3,     1,     2,     3,     0,     2,     2,
       3,     2,     4,     3,     3,     4,     3,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     0,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     4,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     2,     2,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       3,     3,     2,     2,     1,     0,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     3,     4,     3,     4,     3,     4,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
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
#line 5338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5350 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5356 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5380 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5471 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5614 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5620 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5650 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { 
      (yyval.pblockstmt) = buildErrorStandin();
      USR_FATAL_CONT((yyval.pblockstmt), "illegal assignment to type");
    }
#line 5665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5719 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5743 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 5763 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5769 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 670 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5775 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5781 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5787 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5793 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5799 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5805 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5823 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5835 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 686 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5869 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 704 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5899 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5965 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 738 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 740 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 745 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 5985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 777 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 783 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 789 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 793 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 800 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 810 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 820 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 830 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 847 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 868 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 870 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 875 "chapel.ypp" /* yacc.c:1661  */
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
#line 6247 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 891 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 898 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 907 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 941 "chapel.ypp" /* yacc.c:1661  */
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
#line 6343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 961 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 974 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 1010 "chapel.ypp" /* yacc.c:1661  */
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
#line 6448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1036 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1040 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1049 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1054 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 6490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1060 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1063 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1064 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1065 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1072 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1083 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1085 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1086 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1088 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1092 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1099 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1101 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6694 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1106 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6700 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1115 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6754 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1120 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1121 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1129 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1130 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1131 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1136 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1138 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1140 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1142 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1144 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1148 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1149 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1153 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1154 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1155 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1158 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1159 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1209 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1219 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1220 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1234 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1247 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1263 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1272 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1280 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1288 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1314 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1315 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1316 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1322 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1330 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1337 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1346 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1348 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1350 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1372 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1382 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7322 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1390 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1392 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7340 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1393 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7346 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7352 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7358 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1396 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1397 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1398 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1399 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1405 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7406 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7412 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7424 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7430 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7436 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1413 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7442 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1443 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1458 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1515 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1525 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7694 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7700 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1541 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7754 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7887 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7907 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7913 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7919 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7925 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1681 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1690 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1695 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 7975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1699 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1703 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 7997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1713 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1714 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1720 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1746 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1754 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1758 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1760 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8129 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1793 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1794 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1798 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1799 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1800 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1809 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 485:
#line 1811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 486:
#line 1815 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 8189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 487:
#line 1816 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 488:
#line 1817 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 489:
#line 1818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1819 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1820 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1821 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1823 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1825 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1829 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1836 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8259 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1837 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1841 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1842 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1843 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1844 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8289 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1846 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8331 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1852 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8373 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8379 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8385 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8391 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8403 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8415 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8427 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8433 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8439 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1876 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1877 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8457 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1884 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1892 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8511 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8517 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8523 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8533 "bison-chapel.cpp" /* yacc.c:1661  */
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
