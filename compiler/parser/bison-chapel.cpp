/* A Bison parser, made by GNU Bison 3.1.  */

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
#define YYBISON_VERSION "3.1"

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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYLAST   14816

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  130
/* YYNRULES -- Number of rules.  */
#define YYNRULES  545
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1001

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   413

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
    1061,  1065,  1073,  1077,  1078,  1079,  1083,  1084,  1085,  1086,
    1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,
    1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,
    1107,  1108,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,
    1120,  1121,  1122,  1123,  1127,  1128,  1132,  1136,  1137,  1138,
    1142,  1144,  1146,  1148,  1150,  1155,  1156,  1160,  1161,  1162,
    1163,  1164,  1165,  1166,  1167,  1168,  1172,  1173,  1174,  1175,
    1176,  1177,  1181,  1182,  1186,  1187,  1188,  1189,  1190,  1191,
    1195,  1196,  1199,  1200,  1204,  1205,  1209,  1211,  1216,  1217,
    1221,  1222,  1226,  1227,  1231,  1233,  1235,  1240,  1253,  1270,
    1271,  1273,  1278,  1286,  1294,  1302,  1311,  1321,  1322,  1323,
    1327,  1328,  1336,  1338,  1343,  1345,  1347,  1352,  1354,  1356,
    1363,  1364,  1365,  1370,  1372,  1374,  1378,  1382,  1384,  1388,
    1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,
    1406,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,
    1420,  1421,  1441,  1445,  1449,  1457,  1464,  1465,  1466,  1470,
    1472,  1478,  1480,  1482,  1487,  1488,  1489,  1490,  1491,  1492,
    1493,  1494,  1495,  1496,  1497,  1503,  1504,  1505,  1506,  1510,
    1511,  1515,  1516,  1517,  1521,  1522,  1526,  1527,  1531,  1532,
    1536,  1537,  1538,  1539,  1543,  1547,  1548,  1559,  1560,  1561,
    1562,  1563,  1564,  1565,  1567,  1569,  1571,  1573,  1575,  1577,
    1579,  1581,  1583,  1585,  1590,  1592,  1594,  1596,  1598,  1600,
    1602,  1604,  1606,  1608,  1610,  1612,  1614,  1621,  1627,  1633,
    1639,  1648,  1658,  1666,  1667,  1668,  1669,  1670,  1671,  1672,
    1673,  1678,  1679,  1683,  1687,  1688,  1692,  1696,  1697,  1701,
    1705,  1709,  1716,  1717,  1718,  1719,  1720,  1721,  1725,  1726,
    1731,  1736,  1744,  1745,  1746,  1747,  1748,  1749,  1750,  1751,
    1752,  1754,  1756,  1758,  1760,  1762,  1764,  1766,  1771,  1772,
    1775,  1776,  1777,  1780,  1781,  1782,  1783,  1794,  1795,  1799,
    1800,  1801,  1805,  1806,  1807,  1815,  1816,  1817,  1818,  1822,
    1823,  1824,  1825,  1826,  1827,  1828,  1829,  1830,  1831,  1835,
    1843,  1844,  1848,  1849,  1850,  1851,  1852,  1853,  1854,  1855,
    1856,  1857,  1858,  1859,  1860,  1861,  1862,  1863,  1864,  1865,
    1866,  1867,  1868,  1869,  1870,  1874,  1875,  1876,  1877,  1878,
    1879,  1883,  1884,  1885,  1886,  1890,  1891,  1892,  1893,  1898,
    1899,  1900,  1901,  1902,  1903,  1904
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

#define YYPACT_NINF -892

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-892)))

#define YYTABLE_NINF -497

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -892,    97,  3102,  -892,   -43,  -892,  -892,  -892,  -892,  -892,
    -892,  4318,    81,   161,  8987,  -892,    81,  8987,   109,   161,
    4318,  8987,  4318,    19,  -892,  8987,  6372,  7565,  8987,  7717,
    8987,   150,  -892,   153,   161,  -892,    57,  8225,  8987,  8987,
    -892,  8987,  -892,  8987,  8987,   263,   165,   969,   982,  -892,
    8352,  7159,  8987,  8225,  8987,  8987,   254,   170,  4318,  8987,
    9114,  9114,   161,  -892,  8987,  8352,  8987,  8987,  -892,  -892,
    8987,  -892,  -892, 11019,  8987,  8987,  -892,  8987,  -892,  -892,
    3406,  6651,  8352,  -892,  4166,  -892,  -892,   287,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,   161,  -892,   161,   299,   198,
    -892,  -892,  -892,   292,   217,  -892,  -892,  -892,  -892,   232,
     253,   269,   286,   290, 14631,  2823,   -24,   302,   312,  -892,
    -892,  -892,  -892,  -892,  -892,   181,  -892,  -892, 14631,   310,
    4318,  -892,   320,  -892,  8987,  8987,  8987,  8987,  8987,  8352,
    8352,   106,  -892,  -892,  -892,  -892,   339,   171,  -892,  -892,
     316, 13056,   161,   327,  -892,   -31, 14631,   388,  7311, 14631,
    -892,   174,  -892,   161,   242,  -892,   161,   337,    55, 12491,
   12414,  -892,  -892,  -892, 13004, 11775,  7311,  7311,  4318,   340,
      18,   128,   -10,  -892,  4318,  -892,  -892, 12639,   372,   339,
   12639,   339,   372,  -892,  7311,  7844,  -892,  -892,   161,  -892,
      12, 14631,  8987,  8987,  -892, 14631,   347, 12852,  -892, 12639,
     339, 14631,   341,  7311,  -892, 14631, 13273,  -892,  -892, 13359,
   11014,  -892,  -892, 13411,   389,   353,   339,   119, 13141, 12639,
   13491,    33,  2570,   372,    33,   372,    15,  -892,  -892,  3558,
     108,  -892,  8987,  -892,   -28,    77,  -892,    13, 13543,   -32,
     492,  -892,  -892,   448,   380,   354,  -892,  -892,  -892,    35,
      57,    57,    57,  -892,  8987,  8987,  8987,  8987,  8479,  8479,
    8987,  8987,  8987,  8987,  9241,  8987,    71, 11019,  8987,  8987,
    8987,  8987,  8987,  8987,  8987,  8987,  8987,  8987,  8987,  8987,
    8987,  8987,  8987,  7971,  7971,  7971,  7971,  7971,  7971,  7971,
    7971,  7971,  7971,  7971,  7971,  7971,  7971,  7971,  7971,  7311,
    7311,  8479,  8479,  7032,  -892,  -892, 13193, 13325, 13623,    83,
    3710,  -892,  8479,    55,   363,  -892,  8987,  -892,  8987,   414,
    -892,   367,   397,  -892,  -892,   381,   161,   482,  8352,  -892,
    4470,  8479,  -892,  4622,  8479,   373,  -892,    55,  9368,  8987,
    -892,  4318,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,   495,  8987,   376,   379,  -892,   116,
    -892,  -892,    18,  -892,   409,   384,  -892,  9495,   434,  8987,
      57,  -892,  -892,  -892,   393,  -892,  8352,  -892, 14631, 14631,
    -892,    21,  -892,  7311,   400,  -892,   527,  -892,   527,  -892,
    9622,   426,  -892,  -892,  9368,  8987,  -892,  -892,  -892,  -892,
    -892,  8098,  -892, 11633,  6778,  -892,  6905,  -892,  8479,  5965,
    3254,   402,  8987,  6219,  -892,  -892,   161,  8352,   413,   392,
     239,    57,   215,   238,   258,   262, 12923,  1921,  1921,   338,
    -892,   338,  -892,   338, 11675,   843,   627,  2048,  1814,  1814,
     339,    33,  -892,  -892,  -892,  2570,  1170,   338,  1460,  1460,
    1921,  1460,  1460,  1027,    33,  1170,  2431,  1027,   372,   372,
      33,   422,   433,   435,   436,   439,   441,   442,   443,   446,
     450,   453,   454,   456,   462,   467,   469,   437,   459,  -892,
     338,  -892,   338,    54,  -892,  -892,  -892,    88,  -892,   161,
   14670,   348,  9749,  8479,  9876,  8479,  8987,  8479, 11446,    81,
   13675,  -892,  -892, 14631, 13709,  7311,  -892,  7311,  -892,   380,
    8987,   111,  8987, 14631,    58, 12710,  8987, 14631,     4, 12562,
    7032,  -892,   472,   496,   473, 13761,   496,   476,   560, 13841,
    4318, 12781,  -892,  -892,    62,  -892,  -892,  -892,  -892,  -892,
    -892,   536,   113,  -892,    64,  -892,   307,   471,    18,   128,
    8987,    19,  8987,  8987,  8987,  8987,  8987,  5838,  -892,   508,
    7438,  -892, 14631,  -892,  -892,  -892, 14631,   474,    23,   475,
    -892,    52,  -892,  -892,   274,   161,   479,   486,  -892,  -892,
    -892,  -892,  -892,  -892,   -23,  1341,  -892,  -892, 14631,  4318,
   14631,  -892, 13893,   480,   507,  1181,   483,   528,   118,  -892,
     567,  -892,  -892,  -892,  -892,  2709,   282,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  7032,  -892,    29,  8479,  8479,  8987,   614, 13927,  8987,
     615, 14059,   490,  2170,    55,    55,  -892,  -892,  -892,  -892,
     498, 12639,  -892, 11846,  4774,  -892,  4926,  -892, 11917,  5078,
    -892,    55,  5230,  -892,    55,   194,  -892,  8987,  -892,  8987,
    -892,  4318,  8987,  -892,  4318,   628,  -892,  -892,   161,   697,
    -892,    18,   523,   587,  -892,  -892,  -892,    90,  -892,  -892,
     434,   514,   100,  -892,  -892,  -892,  5382,  8352,  -892,  -892,
    -892,   161,  -892,   539,   316,  -892,  -892,  -892,  5534,   521,
    5686,   524,  -892,  8987,  3862,   532,   604,  -892,  8987,   161,
    -892,  -892,   307,  -892,  -892,  -892,  -892,   161,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  8987,   552,
    -892,   565,   534,   534,  -892,  -892,  -892,   434,   218,   219,
   14098, 10003, 10130, 14137, 10257, 10384, 10511, 10638,  -892,  -892,
    1362,  -892,  4318,  8987, 14631,  8987, 14631,  4318,  8987, 14631,
    -892,  8987, 14631,  -892,  7032,  -892, 14237, 14631,  -892, 14631,
     669,  4318,   523,  -892,   549,  8606,    -7,  -892,   606,  -892,
    -892,  8479, 11354,  4318,  -892,    70,   553,  8987,  -892,  8987,
    -892, 14631,  4318,  8987,  -892, 14631,  4318, 14631,   103,  -892,
    4014,  -892,  6524,  1030,  1030,  -892,  1717,  -892, 14631,  -892,
      75,  -892, 11704, 11500,   116,  -892,  -892,  -892,  8987,  8987,
    8987,  8987,  8987,  8987,  8987,  8987,   556,  1025, 13841, 11988,
   12059, 13841, 12130, 12201,  -892,  8987,  4318,  -892,   434,   523,
    8987,    19,  8987,  8987,  8987,  8987,  8987,  6092,  -892,  -892,
     179,  8352,  -892,  -892,  8987,    27,  2311,  -892,   513,   388,
    -892,   316, 14631, 12272,  -892, 12343,  -892,  -892,  -892,  -892,
    -892, 10765,   621,  -892,  -892,   534,   534,   237, 14276, 14315,
   14415, 14454, 14493, 14593,  -892,  -892,  4318,  4318,  4318,  4318,
   14631,  -892,  -892,    -7,  8733,   185,  -892,  -892, 14631, 14631,
    -892,  -892,  -892, 10638,  -892,  4318,  4318,  8987,    19,  8987,
    8987,  8987,  8987,  8987,  2944,  -892,   266,  -892,   606,  -892,
    -892,  -892,  8987,  8987,  8987,  8987,  8987,  8987, 13841, 13841,
   13841, 13841,  -892,  -892,  -892,  -892,  -892,   279,  8479, 11140,
     664, 13841, 13841,   554, 10892,   190,    40, 11551,  -892,  -892,
    -892,  -892,   291, 11235,  -892,  -892,  -892,  8860,  -892,   304,
    -892
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   489,   490,   491,   492,
     493,     0,   431,    80,     0,   161,   431,     0,   298,    80,
       0,     0,     0,     0,   171,   468,   299,     0,     0,   297,
       0,     0,   181,   385,     0,   177,     0,     0,     0,     0,
     422,     0,   184,     0,     0,     0,     0,   297,   297,   162,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,     0,     0,     0,   541,   543,
       0,   544,   545,   467,     0,     0,   542,   539,    89,   540,
       0,     0,     0,     4,     0,     5,     9,    45,    10,    11,
      12,    14,   384,    22,    13,    90,    96,    15,    17,    16,
      19,    20,    21,    18,    95,     0,    93,     0,   457,     0,
      97,    94,    98,     0,   473,   388,   453,   454,   390,   387,
       0,     0,   458,   459,     0,   389,     0,   474,   475,   476,
     452,   392,   391,   455,   456,     0,    38,    24,   398,     0,
       0,   432,     0,    81,     0,     0,     0,     0,     0,     0,
       0,   457,   473,   387,   458,   459,   400,   389,   474,   475,
       0,   431,     0,     0,   135,     0,   369,     0,   376,   469,
     182,   492,   103,     0,     0,   183,     0,     0,     0,     0,
       0,   298,   299,   102,     0,     0,   376,   376,     0,     0,
       0,     0,     0,   300,     0,    86,    32,     0,   527,   450,
       0,   401,   528,     7,   376,   299,    92,    91,   277,   366,
       0,   365,     0,     0,    87,   472,     0,     0,    35,     0,
     403,   393,     0,   376,    36,   399,     0,   142,   138,     0,
     389,   142,   139,     0,   289,     0,   402,     0,   365,     0,
       0,   530,   466,   526,   529,   525,     0,    47,    50,     0,
       0,   371,     0,   373,     0,     0,   372,     0,   365,     0,
       0,     6,    46,     0,   164,     0,   263,   262,   185,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   376,
     376,     0,     0,     0,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,     0,    29,     0,   384,
     382,     0,   377,   378,   383,     0,     0,     0,     0,   112,
       0,     0,   111,     0,     0,     0,   118,     0,     0,    56,
      99,     0,   222,   229,   230,   231,   226,   228,   224,   227,
     225,   223,   233,   232,   128,     0,     0,     0,    30,   237,
     178,   304,     0,   305,   307,     0,   341,     0,   310,     0,
       0,    85,    31,    33,     0,   276,     0,    63,   470,   471,
      88,     0,    34,   376,     0,   149,   140,   136,   141,   137,
       0,   287,   284,    60,     0,    56,   105,    37,    49,    48,
      51,     0,   494,     0,     0,   485,     0,   487,     0,     0,
       0,     0,     0,     0,   498,     8,     0,     0,     0,     0,
     256,     0,     0,     0,     0,     0,   430,   522,   521,   524,
     532,   531,   536,   535,   518,   515,   516,   517,   462,   463,
     461,   505,   484,   483,   482,   464,   509,   520,   514,   512,
     523,   513,   511,   503,   508,   510,   519,   502,   506,   507,
     504,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   534,
     533,   538,   537,   442,   443,   445,   447,     0,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   496,   431,
     431,   108,   285,   370,     0,     0,   395,     0,   286,   164,
       0,     0,     0,   406,     0,     0,     0,   412,     0,     0,
       0,   119,   540,    59,     0,    52,    57,     0,   127,     0,
       0,     0,   394,   386,   250,   247,   248,   249,   253,   254,
     255,   245,     0,   238,     0,   246,   264,     0,   308,     0,
     337,   334,   338,   340,   335,   336,   339,     0,   333,   453,
       0,   302,   451,   301,   481,   368,   367,     0,     0,     0,
     396,     0,   143,   291,   453,     0,     0,     0,   495,   460,
     486,   374,   488,   375,     0,     0,   497,   124,   416,     0,
     500,   499,     0,     0,   165,     0,     0,   175,     0,   172,
     260,   257,   258,   261,   186,     0,     0,   293,   292,   294,
     296,    64,    71,    72,    73,    68,    70,    78,    79,    66,
      69,    67,    65,    75,    74,    76,    77,   479,   480,   444,
     446,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,   380,   381,   379,
       0,     0,   126,     0,     0,   110,     0,   109,     0,     0,
     116,     0,     0,   114,     0,     0,   437,     0,   100,     0,
     101,     0,     0,   130,     0,   132,   251,   252,     0,   245,
     236,     0,   354,   265,   268,   267,   269,     0,   306,   309,
     310,     0,     0,   311,   312,   151,     0,     0,   150,   153,
     397,     0,   144,   147,     0,   288,    61,    62,     0,     0,
       0,     0,   125,     0,     0,     0,   297,   170,     0,   173,
     169,   259,   264,   219,   217,   192,   198,   201,   199,   200,
     211,   202,   215,   207,   205,   218,   206,   204,   209,   214,
     216,   203,   208,   212,   213,   210,   220,   221,     0,   196,
     194,     0,   234,   234,   193,   295,   435,   310,   473,   473,
       0,     0,     0,     0,     0,     0,     0,     0,   107,   106,
       0,   113,     0,     0,   405,     0,   404,     0,     0,   411,
     117,     0,   410,   115,     0,   436,    54,    53,   129,   421,
     131,     0,   354,   239,     0,     0,   310,   266,   282,   303,
     345,     0,   496,     0,   155,     0,     0,     0,   145,     0,
     122,   418,     0,     0,   120,   417,     0,   501,     0,    39,
       0,   159,   299,   297,   297,   157,   297,   167,   176,   174,
       0,   197,     0,     0,   237,   188,   189,   439,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   297,   409,     0,
       0,   415,     0,     0,   438,     0,     0,   134,   310,   354,
     360,   357,   361,   363,   358,   359,   362,     0,   356,   364,
     453,   278,   242,   240,     0,     0,     0,   343,   453,   156,
     154,     0,   148,     0,   123,     0,   121,    41,    40,   168,
     330,     0,   270,   195,   196,   234,   234,     0,     0,     0,
       0,     0,     0,     0,   160,   158,     0,     0,     0,     0,
      55,   133,   241,   310,   346,     0,   279,   281,   280,   283,
     274,   275,   179,     0,   146,     0,     0,   326,   323,   327,
     329,   324,   325,   328,     0,   322,   453,   271,   282,   190,
     191,   235,     0,     0,     0,     0,     0,     0,   408,   407,
     414,   413,   244,   243,   348,   349,   351,   453,     0,   496,
     453,   420,   419,     0,   314,     0,     0,     0,   350,   352,
     319,   317,   453,   496,   272,   187,   273,   346,   318,   453,
     353
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -892,  -892,  -892,    -1,  -366,  2070,  -892,  -892,  -892,   362,
     -22,  -248,   295,   297,  -892,  -892,   377,   694,  2486,  -892,
      17,  -810,  -667,   -40,  -892,  -892,  -892,    89,  -892,  -892,
    -892,   485,  -892,    -4,  -892,  -892,  -892,  -892,  -892,   544,
     191,   -68,  -892,  -892,  -892,   -16,  1000,  -892,  -892,  -892,
    -892,  -892,  -892,  -892,  -892,  -892,  -129,  -619,  -687,  -892,
    -127,    30,   164,  -892,  -892,  -892,    -9,  -892,  -892,  -256,
     140,  -892,  -199,  -223,  -892,  -159,  -892,   708,  -892,  -226,
     350,  -892,  -377,  -694,  -601,  -892,  -522,  -408,  -891,  -840,
    -746,   -33,  -892,    26,  -892,   -97,  -892,   224,   447,  -892,
    -376,  -892,  -892,  1262,  -892,   -12,  -892,  -892,  -228,  -892,
    -500,  -892,  -892,  1307,  1550,    -2,   717,   168,   830,  -892,
    1661,  1937,  -892,  -892,  -892,  -892,  -892,  -892,  -892,  -313
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   248,    84,   548,    86,    87,   263,    88,
     249,   543,   547,   544,    89,    90,    91,   142,    92,   174,
     196,    93,    94,    95,    96,    97,    98,   662,    99,   100,
     101,   406,   592,   724,   102,   103,   588,   719,   104,   105,
     438,   736,   106,   107,   618,   619,   151,   189,   566,   109,
     110,   440,   742,   624,   770,   771,   772,   375,   855,   380,
     562,   563,   564,   565,   625,   268,   707,   958,   995,   942,
     209,   937,   892,   895,   111,   235,   411,   112,   113,   192,
     193,   384,   385,   581,   955,   912,   388,   578,   975,   889,
     816,   250,   165,   254,   255,   341,   342,   343,   152,   115,
     116,   117,   118,   153,   120,   140,   141,   507,   357,   685,
     508,   509,   121,   154,   155,   215,   175,   344,   157,   126,
     158,   159,   129,   130,   259,   131,   132,   133,   134,   135
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     124,    83,   593,   334,   160,   567,   773,   206,   207,   138,
     511,   579,   156,   561,   345,   161,   819,   210,   124,   166,
     124,     5,   587,   169,   169,   179,   180,   184,   185,   431,
     386,   389,   237,   679,   594,   197,   198,   199,     5,   200,
     686,   201,   202,   442,   443,   444,   445,   710,   211,   257,
     217,   219,   220,   221,   428,     5,   225,   226,   229,   233,
       5,   277,   236,   238,   239,   240,   878,     5,   241,   847,
     218,   242,   243,   244,     5,   245,   910,   355,   211,  -152,
     258,   716,   124,   857,   194,   941,   856,   674,   988,   376,
     377,   386,   433,   336,   976,   649,   424,     3,   580,   194,
     136,   462,    51,   696,   355,   278,  1000,   394,   441,   279,
     381,   546,   337,   355,   390,    51,   178,   329,  -152,  -245,
     717,   891,   893,   434,   517,   425,   404,   650,   319,   386,
     320,   777,   728,   933,  -331,   697,   396,   429,   124,   989,
     554,   821,   138,   326,   327,   328,   225,   211,   258,   333,
    -320,   776,   387,  -331,   330,   397,   284,   555,   136,   286,
     556,   463,  -320,   289,     5,  -331,   418,   546,   430,  -331,
     382,   168,  -152,  -320,   718,   557,   941,   330,   558,   909,
      45,   139,  -331,   994,   932,   818,   124,   190,  -331,   559,
     330,   709,   124,   -84,   396,   349,   352,   356,   911,   162,
     847,   426,   330,   169,   721,   330,   560,   429,   330,   190,
     398,   399,   651,   387,   392,   626,   701,   393,  -320,   216,
     330,   253,   497,   498,   429,  -320,   978,   511,   959,   960,
     427,   517,   421,  -331,   916,   396,   402,   699,   518,   973,
    -331,   652,   739,   396,   266,   -84,   136,   124,   420,   256,
     423,   387,  -331,   321,   907,   822,   416,   322,  -478,   422,
    -478,    15,   413,   620,   672,   267,   700,   -84,  -245,   740,
     203,  -331,   446,   447,   448,   449,   451,   453,   454,   455,
     456,   457,   460,   461,  -355,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   621,   186,  -355,   874,   187,   589,  -355,   340,   429,
     681,   684,   622,    49,   429,   531,   269,   204,   804,   500,
     502,   510,   223,  -477,   814,  -477,   340,   340,   124,   623,
     520,   703,  -355,   561,   523,    63,   524,   222,   511,   390,
     979,  -321,  -441,  -440,   340,   993,   211,   805,   533,   535,
     521,   537,   539,  -321,   270,   262,   545,   545,   627,   549,
    -424,   699,   390,   340,  -321,   271,  -313,   450,   452,   704,
     846,  -441,  -440,   551,   541,  -423,   729,   731,  -313,  -315,
     705,   628,   390,   991,  -347,   124,   390,   582,   272,  -313,
     961,  -315,   998,   616,   586,     5,   273,   706,  -290,   195,
     277,   629,  -315,  -347,   614,   630,   390,  -347,   124,  -321,
     499,   501,   545,   545,   897,   195,  -321,  -290,   274,   586,
     655,   519,   227,   231,   322,   775,   605,   586,   608,  -428,
     610,   612,  -347,  -429,  -313,   211,   725,   228,   232,   890,
     534,  -313,  -427,   538,   278,  -426,   898,  -315,   279,   114,
     601,  -478,   603,  -478,  -315,  -425,   156,   236,   114,   340,
     340,   284,   323,   325,   286,   286,   330,   114,   289,   114,
     335,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   338,   561,   348,
     400,   511,   379,   403,   410,   284,   412,   285,   286,   435,
     436,   846,   289,   437,   439,   114,   522,   664,   665,   296,
     533,   658,   537,   661,   549,   663,   608,   604,   302,   525,
     526,   527,   331,   530,   528,   540,   550,   114,   671,   552,
     673,   114,   553,   568,   678,   956,   585,   569,   510,   580,
     195,   195,   195,   340,   712,   591,   584,   374,   124,  -332,
     595,   675,   677,   590,  -342,   680,   683,   609,   977,   195,
     554,   585,   195,   615,   253,   631,   253,   980,   156,   585,
     201,   220,   221,   225,   236,   258,   632,   555,   633,   634,
     556,   195,   635,  -332,   636,   637,   638,   114,  -342,   639,
     647,   691,   256,   640,   256,   557,   641,   642,   558,   643,
     260,   195,   657,   977,   660,   644,  -332,   124,   992,   559,
     645,  -342,   646,  -332,   648,   -58,   688,   999,  -342,   690,
     687,   977,   726,    15,   708,   715,   560,    18,   720,   727,
     734,   396,  -332,   738,   737,   114,    24,  -342,    25,   842,
     741,   114,    29,   781,   784,   786,   815,    32,   790,   510,
    -180,  -332,   451,   500,   780,   277,  -342,   783,  -332,   811,
     817,  -332,   827,  -342,    42,   667,  -342,   340,    45,   820,
     843,  -180,   794,   844,   796,    49,   832,   799,  -384,   836,
     802,   788,   789,   841,   825,   806,   854,   807,   791,   124,
     809,   853,   124,   668,    62,   195,   114,    63,   800,   278,
     876,   803,   879,   279,   894,  -344,   901,   924,   957,   990,
     597,   596,   840,   163,   124,   211,   408,   826,   346,   195,
     670,   554,   867,   849,   915,   698,   831,   917,   835,   813,
     996,   837,   124,   850,   972,   986,   848,   183,   555,  -344,
     583,   556,   170,   450,   499,     0,   281,    78,   714,   283,
     284,   669,   285,   286,     0,   845,   557,   289,     0,   558,
       0,    45,  -344,     0,   296,     0,   852,     0,     0,  -344,
     559,   300,   301,   302,     0,     0,     0,   114,     0,   794,
     796,     0,   799,   802,   831,   835,     0,   560,  -344,     0,
     868,   869,     0,   870,     0,   871,   872,   114,     0,   873,
     114,     0,   510,   206,   207,     0,     0,  -344,   114,   124,
       0,     0,     0,   124,  -344,     0,     0,  -344,     0,   896,
     608,   124,     0,     0,     0,   902,     0,   903,     0,     0,
     124,   905,   125,     0,   124,     0,     0,     0,   124,   420,
     169,   125,   900,     0,     0,     0,     0,     0,     0,     0,
     125,     0,   125,     0,   935,     0,   868,   918,   919,   871,
     920,   921,   922,   923,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   930,   124,     0,     0,   114,   156,     0,
     201,   220,   221,   225,   236,   258,     0,     0,   125,   938,
     230,   230,   939,     0,     0,     0,   195,   195,     0,     0,
     195,   195,     0,     0,     0,     0,     0,     0,     0,   124,
     125,     0,     0,   695,   125,   278,     0,     0,     0,   279,
       0,   985,     0,     0,   968,   969,   970,   971,     0,     0,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
       0,   835,     0,   981,   982,   156,     0,   201,   220,   221,
     225,   236,   258,   722,     0,   888,   653,     0,     0,     0,
     968,   969,   970,   971,   981,   982,   284,     0,   285,   286,
     125,     0,     0,   289,     0,     0,   987,   608,     0,     0,
     296,     0,   124,     0,     0,     0,     0,   300,   301,   302,
       0,   608,   181,     0,     0,   835,     0,   114,     0,     0,
       0,     0,   108,    25,   205,   181,     0,     0,     0,     0,
       0,   108,    32,     0,     0,  -180,    25,   205,   125,     0,
     108,   -44,   108,     0,   125,    32,   195,   195,  -180,    42,
       0,   936,     0,   195,   -43,   374,  -180,   -44,     0,     0,
     374,     0,    42,   195,    15,     0,   195,     0,    18,  -180,
     -43,     0,     0,   181,     0,   277,   114,    24,   108,    25,
     842,     0,     0,    29,    25,   205,     0,     0,    32,     0,
       0,  -180,   774,    32,   974,     0,  -180,     0,     0,   125,
     108,     0,     0,     0,   108,    42,   828,     0,     0,    45,
      42,   843,  -180,     0,   844,     0,    49,  -180,     0,   278,
       0,   778,   779,   279,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,     0,    63,   974,
       0,   114,     0,   114,     0,     0,   114,     0,     0,   114,
       0,     0,     0,     0,     0,     0,     0,   974,   114,     0,
     108,   114,     0,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,     0,     0,     0,   289,     0,     0,
     125,     0,     0,   114,   296,     0,     0,     0,    78,     0,
       0,   300,   301,   302,     0,   114,   925,   114,     0,     0,
     125,   114,   735,   125,     0,     0,     0,   195,   108,     0,
       0,   125,     0,     0,   108,     0,     0,     0,   277,     0,
    -166,     0,     0,     0,  -166,  -166,     0,     0,     0,     0,
       0,     0,     0,  -166,     0,  -166,  -166,     0,     0,  -166,
       0,     0,     0,     0,  -166,     0,     0,  -166,     0,     0,
       0,   374,   374,     0,   374,   374,     0,     0,     0,   114,
       0,  -166,   278,  -166,   114,  -166,   279,  -166,  -166,   108,
    -166,     0,  -166,     0,  -166,     0,     0,   940,   114,     0,
     125,     0,     0,   944,   119,   374,     0,   374,     0,     0,
     114,  -166,     0,   119,  -166,     0,     0,  -166,     0,   114,
       0,     0,   119,   114,   119,     0,     0,   114,     0,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,     0,
     289,   290,   291,     0,   293,   294,   295,   296,     0,   122,
       0,   299,     0,     0,   300,   301,   302,     0,   122,     0,
     119,     0,     0,   114,  -166,     0,     0,   122,     0,   122,
     108,     0,  -166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,     0,     0,   119,     0,   940,     0,
     108,   275,     0,   108,     0,     0,     0,     0,   276,     0,
       0,   108,     0,   866,     0,   122,     0,     0,     0,   277,
       0,     0,     0,   114,   114,   114,   114,     0,     0,     0,
     125,  -166,     0,     0,     0,  -166,  -166,   122,     0,     0,
       0,   122,   114,   114,  -166,     0,  -166,  -166,     0,     0,
    -166,     0,   119,     0,     0,  -166,     0,     0,  -166,     0,
       0,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,     0,  -166,     0,  -166,     0,  -166,     0,  -166,  -166,
     108,  -166,     0,  -166,     0,  -166,     0,     0,     0,   125,
       0,   355,     0,     0,     0,   280,     0,   122,     0,     0,
     119,     0,  -166,     0,     0,  -166,   119,     0,  -166,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,     0,   300,   301,   302,   277,     0,
       0,     0,     0,     0,     0,   122,   730,     0,     0,     0,
       0,   122,     0,     0,   125,  -166,   125,     0,     0,   125,
       0,   119,   125,  -166,     0,     0,     0,     0,     0,     0,
       0,   125,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     0,   278,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,     0,     0,
     108,     0,   123,     0,     0,     0,   122,     0,   125,     0,
     125,   123,     0,     0,   125,     0,     0,     0,     0,     0,
     123,     0,   123,     0,     0,     0,     0,     0,     0,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,     0,
     289,     0,   119,     0,     0,     0,   295,   296,     0,     0,
       0,   299,     0,     0,   300,   301,   302,     0,   123,   108,
       0,     0,   119,     0,     0,   119,     0,     0,     0,     0,
       0,     0,   125,   119,     0,     0,     0,   125,     0,     0,
     123,     0,     0,     0,   123,     0,     0,   122,     0,     0,
       0,   125,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,     0,   122,     0,     0,
     122,     0,   125,   127,     0,     0,   125,     0,   122,     0,
     125,     0,   127,     0,   108,     0,   108,     0,     0,   108,
       0,   127,   108,   127,     0,     0,     0,     0,     0,     0,
     123,   108,   119,     0,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,   108,     0,
     108,     0,     0,     0,   108,     0,    15,   122,   123,     0,
      18,   127,     0,     0,   123,   127,     0,     0,     0,    24,
       0,    25,   842,     0,     0,    29,   125,   125,   125,   125,
      32,     0,     0,  -180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   125,   125,    42,     0,     0,
       0,   260,     0,   843,  -180,     0,   844,     0,    49,     0,
       0,     0,   108,     0,     0,     0,     0,   108,     0,   123,
       0,   127,     0,     0,     0,     0,     0,    62,     0,     0,
      63,   108,   119,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,   108,     0,     0,   144,     0,     0,     0,
      14,     0,   108,     0,     0,     0,   108,     0,     0,     0,
     108,     0,     0,     0,    23,     0,     0,     0,     0,   127,
     145,   146,     0,   147,     0,   127,    31,   122,     0,    33,
      78,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,   119,     0,     0,     0,    43,   108,    44,     0,    46,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
     123,     0,     0,   123,     0,     0,     0,     0,    64,     0,
     127,   123,     0,     0,     0,     0,   122,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108,   108,   108,   108,
       0,     0,     0,     0,    70,     0,   119,     0,   119,   128,
       0,   119,     0,     0,   119,   108,   108,     0,   128,   277,
       0,     0,    75,   119,     0,     0,   119,   128,     0,   128,
       0,     0,     0,     0,   149,     0,    81,     0,   150,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,     0,
     123,   122,     0,   122,     0,     0,   122,     0,     0,   122,
     119,   127,   119,   278,     0,   128,   119,   279,   122,     0,
       0,   122,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   127,     0,     0,   127,     0,     0,   128,     0,     0,
       0,   128,   127,   122,     0,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,     0,   122,     0,     0,
     281,   122,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   119,   293,   294,   295,   296,   119,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,    85,   119,     0,     0,   277,   128,     0,     0,
       0,   137,     0,     0,     0,   119,     0,     0,     0,     0,
     164,   127,   167,     0,   119,     0,     0,     0,   119,   122,
     123,     0,   119,     0,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   122,     0,
     278,     0,     0,     0,   279,   128,     0,     0,   224,     0,
     122,   128,     0,     0,     0,     0,     0,     0,   119,   122,
       0,     0,     0,   122,     0,     0,     0,   122,     0,     0,
      85,     0,     0,     0,   261,     0,     0,     0,     0,   123,
       0,     0,     0,     0,     0,     0,     0,   281,     0,     0,
       0,   284,     0,   285,   286,     0,     0,     0,   289,     0,
     275,     0,     0,   122,     0,   296,   128,   276,   119,   119,
     119,   119,   300,   301,   302,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   119,     0,
     324,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   123,     0,   123,     0,     0,   123,
       0,     0,   123,   122,   122,   122,   122,     0,     0,     0,
       0,   123,   278,     0,   123,     0,   279,     0,     0,     0,
       0,     0,   122,   122,     0,     0,     0,     0,   378,     0,
       0,     0,     0,     0,   391,     0,   123,   128,     0,     0,
     127,     0,     0,     0,   280,     0,     0,     0,   123,     0,
     123,     0,     0,     0,   123,     0,     0,   128,     0,   281,
     128,   282,   283,   284,     0,   285,   286,   287,   128,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,     0,   300,   301,   302,     0,     0,    85,
       0,   275,     0,     0,     0,   787,     0,     0,   276,     0,
       0,     0,     0,     0,     0,   127,     0,   127,     0,   277,
     127,     0,   123,   127,     0,     0,     0,   123,     0,     0,
       0,     0,   127,     0,     0,   127,     0,     0,     0,     0,
       0,   123,     0,     0,     0,     0,     0,   128,     0,     0,
       0,     0,     0,   123,     0,     0,     0,   127,     0,     0,
       0,     0,   123,   278,     0,     0,   123,   279,     0,   127,
     123,   127,     0,     0,     0,   127,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,     0,
     391,     0,     0,   391,     0,     0,   123,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,   127,     0,   300,   301,   302,   127,   277,
       0,     0,     0,     0,     0,     0,   943,     0,     0,     0,
       0,     0,   127,     0,     0,     0,   123,   123,   123,   123,
       0,     0,     0,     0,   127,     0,     0,   128,     0,     0,
       0,     0,     0,   127,     0,   123,   123,   127,     0,   143,
     607,   127,     0,   278,     0,   143,     0,   279,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     188,     0,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,   128,     0,   234,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,     0,   293,   294,   295,   296,     0,
     297,     0,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   127,   127,   127,
     127,   264,     0,   265,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,     0,   127,   127,     0,     0,
       0,   128,     0,   128,     0,     0,   128,     0,     0,   128,
     693,     0,     0,     0,     0,     0,     0,     0,   128,     0,
       0,   128,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   278,     0,     0,     0,   279,     0,   234,     0,
       0,     0,     0,   128,   339,     0,     0,     0,     0,   234,
       0,     0,   347,     0,     0,   128,     0,   128,     0,     0,
       0,   128,   339,   339,     0,     0,   383,     0,     0,   732,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   281,
     339,   282,   283,   284,   395,   285,   286,     0,     0,     0,
     289,     0,     0,     0,     0,     0,   295,   296,     0,   339,
       0,   299,     5,     0,   300,   301,   302,     0,     0,   743,
       0,     0,     0,     0,     0,     0,   744,     0,     0,   128,
       0,     0,     0,     0,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   391,     0,   391,     0,   128,   391,
       0,     0,   391,     0,   745,   191,   191,   191,   191,     0,
     128,   808,     0,     0,   810,     0,     0,     0,     0,   128,
       0,     0,   464,   128,     0,     0,     0,   128,     0,     0,
       0,     0,     0,     0,     0,     0,   824,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   830,     0,
     834,     0,     0,     0,    85,   339,   339,     0,     0,     0,
       0,     0,     0,   128,   362,   363,   364,   365,   366,   367,
       0,     0,   368,   369,   370,   371,   372,   373,   746,   747,
     748,   749,   529,     0,   750,     0,     0,     0,   751,   752,
     753,   754,   755,   756,   757,   758,   759,   760,   761,     0,
     762,     0,     0,   763,   764,   765,   766,     0,   767,     0,
       0,   768,     0,   128,   128,   128,   128,     0,   383,     0,
       0,     0,     0,     0,     0,     0,   191,     0,     0,     0,
       0,   877,   128,   128,     0,     0,     0,     0,     0,   339,
       0,     0,     0,   899,     0,     0,     0,     0,     0,     0,
       0,     0,   904,     0,     0,     0,   906,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   613,     0,     0,   617,     0,   191,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,     0,     0,     0,   983,   931,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   144,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     317,   318,  -448,     0,    23,  -477,     0,  -477,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
     702,     0,     0,     0,   383,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,   723,     0,     0,
      74,   234,    75,     0,    76,    77,   208,     0,     0,     0,
      79,     0,     0,     0,   149,     0,    81,     0,   150,   984,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,   769,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -297,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -297,    44,    45,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -297,    50,    51,     0,    52,
      53,    54,    55,    56,   812,    57,    58,   383,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -297,     0,
       0,    66,     0,    67,     0,     0,    68,   723,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,   617,     0,     0,     0,    73,
       0,     0,     0,   851,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,  -496,     0,    11,    12,    13,     0,
      14,  -496,     0,    15,    16,    17,     0,    18,  -297,    19,
      20,    21,  -496,    22,    23,  -496,    24,     0,    25,    26,
      27,    28,    29,    30,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -297,    44,     0,    46,
      47,  -180,   -42,    48,     0,    49,  -496,  -297,    50,    51,
    -496,    52,    53,    54,    55,    56,     0,    57,    58,   914,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -297,     0,     0,    66,     0,    67,     0,     0,  -496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -496,    70,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,     0,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,    75,  -496,  -496,  -496,     0,    78,  -496,  -496,
    -496,     0,     0,     0,    80,  -496,    81,   246,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -297,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -297,    44,
      45,    46,    47,  -180,   -42,    48,     0,    49,     0,  -297,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -297,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,   208,    78,
       0,     0,    79,     0,     0,     0,    80,   247,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -297,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,    30,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -297,    44,    45,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -297,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -297,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,   419,
      81,   246,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -297,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,    30,
       0,     0,    31,    32,     0,    33,  -180,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -297,    44,    45,    46,    47,  -180,   -42,    48,
       0,    49,     0,  -297,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -297,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,   247,    81,   838,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -297,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -297,    44,    45,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -297,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -297,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,   839,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -297,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,    30,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -297,    44,    45,    46,
      47,  -180,   -42,    48,     0,    49,     0,  -297,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -297,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,   908,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -297,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -297,    44,
     260,    46,    47,  -180,   -42,    48,     0,    49,     0,  -297,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -297,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -297,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,    30,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -297,    44,     0,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -297,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -297,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -297,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,   532,
       0,     0,    31,    32,     0,    33,  -180,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -297,    44,     0,    46,    47,  -180,   -42,    48,
       0,    49,     0,  -297,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -297,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,     0,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -297,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   536,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -297,    44,     0,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -297,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -297,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -297,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,   793,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -297,    44,     0,    46,
      47,  -180,   -42,    48,     0,    49,     0,  -297,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -297,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -297,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   795,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -297,    44,
       0,    46,    47,  -180,   -42,    48,     0,    49,     0,  -297,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -297,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -297,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,   798,     0,     0,
      31,    32,     0,    33,  -180,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -297,    44,     0,    46,    47,  -180,   -42,    48,     0,    49,
       0,  -297,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -297,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -297,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,   801,
       0,     0,    31,    32,     0,    33,  -180,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -297,    44,     0,    46,    47,  -180,   -42,    48,
       0,    49,     0,  -297,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -297,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,     0,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -297,    19,    20,    21,
       0,   823,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,     0,    31,    32,     0,    33,  -180,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -297,    44,     0,    46,    47,  -180,
     -42,    48,     0,    49,     0,  -297,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -297,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -297,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,   829,     0,     0,    31,    32,     0,    33,
    -180,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -297,    44,     0,    46,
      47,  -180,   -42,    48,     0,    49,     0,  -297,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -297,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -297,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   833,     0,     0,    31,    32,
       0,    33,  -180,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -297,    44,
       0,    46,    47,  -180,   -42,    48,     0,    49,     0,  -297,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -297,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,   711,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     144,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
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
     208,    14,     0,     0,    79,     0,     0,     0,   149,     0,
      81,     0,   150,     0,     0,    23,     0,     0,     0,     0,
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
       0,    79,     0,     0,     0,   149,     0,    81,     0,   150,
     606,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   147,     0,     0,    31,     0,     0,    33,     0,     0,
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
       0,     0,   149,     0,    81,     0,   150,   934,     0,    23,
       0,     0,     0,     0,     0,   145,   146,     0,   147,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,    75,     0,    76,
      77,     0,     0,     0,     0,    79,     0,     0,     0,   149,
       0,    81,     0,   150,   611,     5,     6,     7,     8,   171,
      10,   172,     0,     0,   144,     0,     0,     0,    14,     0,
       0,   -83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   147,     0,     0,    31,     0,     0,    33,  -468,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,  -468,
       0,     0,     0,   -83,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   148,     0,     0,     0,
       0,     0,   173,     0,     0,   -83,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,     0,     0,     0,    79,     0,
       0,     0,   149,     0,    81,     0,   150,     5,     6,     7,
       8,   171,    10,     0,     0,     0,   144,     0,     0,     0,
      14,     0,     0,   -83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
    -468,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,  -468,     0,     0,     0,   -83,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,   173,     0,     0,   -83,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
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
     213,     0,     0,   251,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,   252,
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     144,    76,    77,   208,    14,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     0,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
      31,     0,     0,    33,     0,     0,    35,    36,     0,    38,
       0,    39,    40,     0,     0,     0,     0,     0,     0,    43,
       0,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    57,   148,     0,     0,     0,   212,   213,     0,     0,
     251,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     5,     6,
       7,     8,     9,    10,    74,     0,    75,   144,    76,    77,
     208,    14,     0,     0,    79,     0,     0,     0,   149,     0,
      81,   600,   150,     0,     0,    23,     0,     0,     0,     0,
       0,   145,   146,     0,   147,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   148,
       0,     0,     0,   212,   213,     0,     0,   251,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   144,    76,    77,   208,    14,     0,
       0,    79,     0,     0,     0,   149,   503,    81,   602,   150,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   147,     0,   504,    31,     0,     0,    33,     0,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,     0,
       0,     0,     0,     0,     0,   505,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,   506,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   144,    76,    77,     0,    14,     0,     0,    79,     0,
       0,     0,   149,     0,    81,     0,   150,     0,     0,    23,
       0,     0,     0,     0,     0,   145,   146,     0,   147,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   148,     0,     0,     0,   212,   213,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,    75,     0,    76,
      77,     0,   214,     0,     0,    79,     0,     0,     0,   149,
       0,    81,     0,   150,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   144,     0,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     144,    76,    77,   208,    14,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     0,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
      31,     0,     0,    33,     0,     0,    35,    36,     0,    38,
       0,    39,    40,   713,     0,     0,     0,     0,     0,    43,
       0,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    57,   148,     0,     0,     0,   212,   213,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     5,     6,
       7,     8,     9,    10,    74,     0,    75,   144,    76,    77,
       0,    14,     0,     0,    79,     0,     0,     0,   149,     0,
      81,     0,   150,     0,     0,    23,     0,     0,     0,     0,
       0,   145,   146,     0,   147,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,   176,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,   177,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,    75,     0,    76,    77,     0,     0,     0,
       0,    79,     0,     0,     0,   149,     0,    81,     0,   150,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   144,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,   182,   145,   146,     0,   147,     0,     0,    31,
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
      14,     0,     0,    79,     0,     0,     0,   149,     0,    81,
       0,   150,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
    -468,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,  -468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
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
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     144,    76,    77,     0,    14,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     0,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
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
     208,    14,     0,     0,    79,     0,     0,     0,   149,   598,
      81,     0,   150,     0,   194,    23,     0,     0,     0,     0,
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
      10,    74,     0,    75,   144,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,    80,     0,    81,     0,   150,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   147,     0,     0,    31,     0,     0,    33,     0,     0,
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
       0,   177,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   144,    76,
      77,     0,   880,     0,     0,    79,     0,     0,     0,   149,
       0,    81,     0,   150,     0,     0,   881,     0,     0,     0,
       0,     0,   145,   146,     0,   147,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,   882,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   883,   884,    56,     0,    57,
     885,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     886,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   144,    76,    77,   208,    14,
       0,     0,    79,     0,     0,     0,   149,     0,    81,     0,
     887,     0,     0,    23,     0,     0,     0,     0,     0,   145,
     146,     0,   147,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   148,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   144,    76,    77,   208,    14,     0,     0,    79,
       0,     0,     0,   149,     0,    81,     0,   887,     0,     0,
      23,     0,     0,     0,     0,     0,   145,   146,     0,   865,
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
       8,     9,    10,    74,     0,    75,   144,    76,    77,     0,
      14,     0,     0,    79,     0,     0,     0,   149,     0,    81,
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
      74,     0,    75,   144,    76,    77,     0,   458,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     0,   150,     0,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     147,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   148,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     144,    76,    77,     0,    14,     0,     0,    79,     0,     0,
       0,   149,     0,    81,     0,   150,     0,     0,    23,     0,
       0,     0,     0,     0,   145,   146,     0,   147,     0,     0,
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
       0,   570,     0,     0,   542,     0,     0,     0,   149,     0,
      81,     0,   150,     0,     0,   571,     0,     0,     0,     0,
       0,   145,   146,     0,   147,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,   572,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   573,   574,    56,     0,    57,   575,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   144,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,   149,     0,    81,     0,   577,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   147,     0,     0,    31,     0,     0,    33,     0,     0,
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
       0,     0,   149,     0,    81,     0,   577,     0,     0,    23,
       0,     0,     0,     0,     0,   145,   146,     0,   656,     0,
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
       0,     0,   145,   146,     0,   659,     0,     0,    31,     0,
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
     146,     0,   859,     0,     0,    31,     0,     0,    33,     0,
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
      23,     0,     0,     0,     0,     0,   145,   146,     0,   860,
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
      76,    77,     0,    14,     0,     0,    79,     0,     0,     0,
     149,     0,    81,     0,   150,     0,     0,    23,     0,     0,
       0,     0,     0,   145,   146,     0,   862,     0,     0,    31,
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
      14,     0,     0,    79,     0,     0,     0,   149,     0,    81,
       0,   150,     0,     0,    23,     0,     0,     0,     0,     0,
     145,   146,     0,   863,     0,     0,    31,     0,     0,    33,
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
      79,     0,     0,     0,   149,     0,    81,     0,   150,     0,
       0,    23,     0,     0,     0,     0,     0,   145,   146,     0,
     864,     0,     0,    31,     0,     0,    33,     0,     0,    35,
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
       0,     0,     0,     0,   145,   146,     0,   865,     0,     0,
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
       0,   947,     0,     0,    79,     0,     0,     0,   149,     0,
      81,     0,   150,     0,     0,   948,     0,     0,     0,     0,
       0,   145,   146,     0,   147,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,   949,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   950,   951,    56,     0,    57,   952,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   953,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   144,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,   149,     0,    81,     0,   954,
       0,     0,    23,     0,     0,     0,     0,     0,   145,   146,
       0,   147,     0,     0,    31,     0,     0,    33,     0,     0,
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
       0,     0,     0,     0,    81,     0,   954,     0,     0,    23,
       0,     0,     0,     0,     0,   145,   146,     0,   147,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   144,     0,     0,    74,    14,    75,     0,     0,
      77,   317,   318,     0,     0,    79,  -477,     0,  -477,   149,
      23,    81,     0,   150,     0,     0,   145,   146,     0,   147,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   148,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,  -346,     0,   144,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
      70,     0,     0,     0,  -346,    23,     0,     0,  -346,     0,
       0,   145,   146,     0,   147,     0,     0,    31,    75,     0,
      33,     0,   208,    35,    36,     0,    38,     0,    39,    40,
     149,     0,    81,  -346,   887,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -316,     0,     0,     0,    54,    55,    56,     0,    57,   148,
       0,     0,  -316,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,  -316,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    70,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   144,     0,     0,     0,
      14,     0,     0,    75,     0,     0,     0,     0,  -316,     0,
       0,     0,     0,     0,    23,  -316,     0,    81,     0,   954,
     145,   146,     0,   147,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   144,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    70,     0,    23,     0,     0,     0,
       0,     0,   145,   146,     0,   147,     0,     0,    31,     0,
       0,    33,    75,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     5,   149,     0,    81,    43,   577,    44,
     743,    46,     0,     0,     0,     0,     0,   744,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     148,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   275,     0,     0,     0,     0,    70,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,    75,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    81,     0,
     150,     0,     0,     0,     0,   362,   363,   364,   365,   366,
     367,     0,     0,   368,   369,   370,   371,   372,   373,   746,
     747,   748,   749,   278,     0,   750,     0,   279,     0,   751,
     752,   753,   754,   755,   756,   757,   758,   759,   760,   761,
       0,   762,     0,   275,   763,   764,   765,   766,     0,   767,
     276,     0,     0,     0,     0,   280,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,   277,     0,   278,   997,     0,     0,   279,
       0,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   278,     0,     0,
       0,   279,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,   278,   300,   301,   302,
     279,     0,     0,     0,     0,   275,   599,     0,     0,     0,
       0,     0,   276,     0,   281,     0,   282,   283,   284,     0,
     285,   286,   287,   277,   288,   289,   290,   291,   280,   293,
     294,   295,   296,     0,   297,     0,   299,     0,     0,   300,
     301,   302,     0,   281,     0,   282,   283,   284,     0,   285,
     286,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,     0,   297,   298,   299,     0,   278,   300,   301,
     302,   279,     0,     0,     0,     0,   275,   913,     0,     0,
     361,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,   280,
     362,   363,   364,   365,   366,   367,     0,     0,   368,   369,
     370,   371,   372,   373,   281,     0,   282,   283,   284,     0,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,   278,   300,
     301,   302,   279,     0,     0,   330,     0,   275,     0,     0,
       0,   792,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,     0,     0,     0,
     280,   362,   363,   364,   365,   366,   367,     0,     0,   368,
     369,   370,   371,   372,   373,   281,     0,   282,   283,   284,
       0,   285,   286,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,     0,   297,   298,   299,     0,   278,
     300,   301,   302,   279,     0,     0,   330,     0,   275,     0,
       0,     0,   797,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,   280,   362,   363,   364,   365,   366,   367,     0,     0,
     368,   369,   370,   371,   372,   373,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     278,   300,   301,   302,   279,     0,     0,   330,     0,   275,
       0,     0,     0,   926,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,   280,   362,   363,   364,   365,   366,   367,     0,
       0,   368,   369,   370,   371,   372,   373,   281,     0,   282,
     283,   284,     0,   285,   286,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,   297,   298,   299,
       0,   278,   300,   301,   302,   279,     0,     0,   330,     0,
     275,     0,     0,     0,   927,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,   280,   362,   363,   364,   365,   366,   367,
       0,     0,   368,   369,   370,   371,   372,   373,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,   278,   300,   301,   302,   279,     0,     0,   330,
       0,   275,     0,     0,     0,   928,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,   280,   362,   363,   364,   365,   366,
     367,     0,     0,   368,   369,   370,   371,   372,   373,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,   278,   300,   301,   302,   279,     0,     0,
     330,     0,   275,     0,     0,     0,   929,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,   280,   362,   363,   364,   365,
     366,   367,     0,     0,   368,   369,   370,   371,   372,   373,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,   278,   300,   301,   302,   279,     0,
       0,   330,     0,   275,     0,     0,     0,   945,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,   280,   362,   363,   364,
     365,   366,   367,     0,     0,   368,   369,   370,   371,   372,
     373,   281,     0,   282,   283,   284,     0,   285,   286,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,   297,   298,   299,     0,   278,   300,   301,   302,   279,
       0,     0,   330,     0,   275,     0,     0,     0,   946,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   353,     0,     0,     0,   280,   362,   363,
     364,   365,   366,   367,     0,   354,   368,   369,   370,   371,
     372,   373,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,   278,   300,   301,   302,
     279,     0,     0,   330,     0,     0,     0,     0,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,   355,     0,     0,     0,   280,   277,
     350,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   351,   281,     0,   282,   283,   284,     0,   285,
     286,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,     0,   297,   298,   299,     0,     0,   300,   301,
     302,     0,     0,   278,   330,     0,     0,   279,     0,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   682,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,   278,   300,   301,   302,   279,     0,
       0,   330,     0,     0,     0,     0,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,   355,     0,     0,     0,   280,   277,   194,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   281,     0,   282,   283,   284,     0,   285,   286,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,   297,   298,   299,     0,     0,   300,   301,   302,     0,
       0,   278,   330,     0,     0,   279,     0,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,   676,
       0,     0,     0,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,     0,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,   278,   300,   301,   302,   279,     0,     0,   330,
       0,   275,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,   280,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,   278,   300,   301,   302,   279,     0,     0,
     330,     0,   275,     0,     0,     0,   694,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,   278,   300,   301,   302,   279,     0,
       0,   330,     0,   275,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,   280,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   281,     0,   282,   283,   284,     0,   285,   286,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
       0,   297,   298,   299,     0,   278,   300,   301,   302,   279,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,   280,     0,     0,
       0,     0,   277,     0,     0,     0,     0,   358,     0,     0,
       0,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   359,   297,   298,   299,     0,   275,   300,   301,   302,
       0,     0,  -449,   276,     0,     0,   278,     0,     0,     0,
     279,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   281,     0,   282,   283,   284,   278,   285,
     286,   287,   279,   288,   289,   290,   291,   292,   293,   294,
     295,   296,     0,   297,   298,   299,     0,   360,   300,   301,
     302,   275,     0,     0,     0,     0,   139,     0,   276,     0,
     280,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,   414,   281,     0,   282,   283,   284,
       0,   285,   286,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,     0,   297,   298,   299,   415,     0,
     300,   301,   302,   275,     0,     0,     0,     0,     0,     0,
     276,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,   277,   512,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,   278,   285,   286,   287,   279,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,   275,     0,   300,   301,   302,     0,     0,
     276,     0,     0,     0,     0,     0,     0,   280,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,   275,     0,   300,   301,   302,
       0,     0,   276,     0,     0,   278,     0,     0,     0,   279,
       0,     0,     0,   277,   514,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   515,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   276,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,   281,     0,   282,   283,   284,   278,   285,   286,
     287,   279,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,   405,   300,   301,   302,
       0,   275,     0,     0,     0,     0,     0,     0,   276,   280,
       0,   278,     0,     0,     0,   279,     0,     0,     0,   277,
       0,     0,     0,     0,   281,     0,   282,   283,   284,     0,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   280,   297,   298,   299,     0,     0,   300,
     301,   302,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,   278,   285,   286,   287,   279,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,   275,   407,   300,   301,   302,     0,     0,   276,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,   275,   409,   300,   301,   302,     0,     0,
     276,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,   278,   285,   286,   287,   279,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,   275,   417,   300,   301,   302,     0,     0,
     276,     0,     0,     0,     0,     0,   432,   280,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,   275,     0,   300,   301,   302,
       0,     0,   276,     0,     0,   278,     0,     0,     0,   279,
       0,     0,     0,   277,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   275,
       0,     0,     0,     0,     0,     0,   276,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,     0,
       0,     0,   281,     0,   282,   283,   284,   278,   285,   286,
     287,   279,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,     0,   300,   301,   302,
       0,   275,   689,     0,     0,   139,     0,     0,   276,   280,
       0,   278,     0,     0,     0,   279,     0,     0,     0,   277,
       0,     0,     0,     0,   281,     0,   282,   283,   284,     0,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   280,   297,   298,   299,     0,     0,   300,
     301,   302,     0,     0,     0,     0,     0,     0,   281,     0,
     282,   283,   284,   278,   285,   286,   287,   279,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,   275,   666,   300,   301,   302,     0,     0,   276,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,   277,
       0,     0,   692,     0,     0,     0,     0,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,   275,     0,   300,   301,   302,     0,     0,
     276,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,   276,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   782,     0,     0,     0,
     281,     0,   282,   283,   284,   278,   285,   286,   287,   279,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,     0,   300,   301,   302,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   280,     0,   278,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   280,   297,   298,   299,     0,     0,   300,   301,   302,
       0,     0,     0,     0,     0,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,   275,
       0,   300,   301,   302,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,   278,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,   276,     0,     0,     0,     0,     0,
       0,     0,     0,   280,     0,   277,     0,     0,     0,     0,
     278,     0,     0,     0,   279,     0,     0,     0,   281,     0,
     282,   283,   284,   858,   285,   286,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,     0,   297,   298,
     299,     0,   280,   300,   301,   302,     0,     0,     0,   278,
       0,     0,     0,   279,     0,     0,     0,   281,     0,   282,
     283,   284,   861,   285,   286,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,     0,   297,   298,   299,
       0,   280,   300,   301,   302,     0,     0,   275,   875,     0,
       0,     0,     0,     0,   276,     0,   281,     0,   282,   283,
     284,     0,   285,   286,   287,   277,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
       0,   300,   301,   302,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,   278,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,   280,     0,   277,     0,     0,     0,     0,   278,     0,
       0,     0,   279,     0,     0,     0,   281,     0,   282,   283,
     284,   962,   285,   286,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,     0,   297,   298,   299,     0,
     280,   300,   301,   302,     0,     0,     0,   278,     0,     0,
       0,   279,     0,     0,     0,   281,     0,   282,   283,   284,
     963,   285,   286,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,     0,   297,   298,   299,     0,   280,
     300,   301,   302,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   276,     0,   281,     0,   282,   283,   284,     0,
     285,   286,   287,   277,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,     0,   300,
     301,   302,     0,     0,   275,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,   278,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,     0,
     964,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,   280,
       0,   277,     0,     0,     0,     0,   278,     0,     0,     0,
     279,     0,     0,     0,   281,     0,   282,   283,   284,   965,
     285,   286,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,     0,   297,   298,   299,     0,   280,   300,
     301,   302,     0,     0,     0,   278,     0,     0,     0,   279,
       0,     0,     0,   281,     0,   282,   283,   284,   966,   285,
     286,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,     0,   297,   298,   299,     0,   280,   300,   301,
     302,     0,     0,   275,     0,     0,     0,     0,     0,     0,
     276,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,   277,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,     0,     0,   300,   301,   302,
       0,   275,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,   278,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,     0,   967,     0,
     275,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,   277,     0,
       0,     0,     0,   278,     0,     0,     0,   279,     0,     0,
       0,     0,   281,     0,   282,   283,   284,     0,   285,   286,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,     0,   297,   298,   299,   280,     0,   300,   301,   302,
       0,     0,   654,     0,     0,     0,   279,     0,     0,     0,
     281,     0,   282,   283,   284,     0,   285,   286,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,     0,
     297,   298,   299,     0,   280,   300,   301,   302,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   282,   283,   284,     0,   285,   286,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,     0,   297,
     298,   299,     0,     0,   300,   301,   302
};

static const yytype_int16 yycheck[] =
{
       2,     2,   410,   162,    16,   382,   625,    47,    48,    11,
     323,   387,    14,   379,   173,    17,   710,    50,    20,    21,
      22,     3,     1,    25,    26,    27,    28,    29,    30,   257,
       1,    41,    65,    29,   410,    37,    38,    39,     3,    41,
     540,    43,    44,   269,   270,   271,   272,   569,    50,    82,
      52,    53,    54,    55,    41,     3,    58,    59,    60,    61,
       3,    28,    64,    65,    66,    67,   812,     3,    70,   736,
      53,    73,    74,    75,     3,    77,     1,   100,    80,    58,
      82,    58,    84,   777,    29,   895,   773,    29,   979,   186,
     187,     1,   124,   124,   934,    41,   124,     0,   105,    29,
     143,    30,    75,    41,   100,    72,   997,   204,    73,    76,
      92,   359,   143,   100,   124,    75,    27,   150,    97,     3,
      97,   128,   816,   155,    41,   153,   223,    73,   152,     1,
     154,   653,   155,   879,   105,    73,   124,   124,   140,   979,
      24,    41,   144,   145,   146,   147,   148,   149,   150,   161,
      75,   651,   123,   124,   150,   143,   123,    41,   143,   126,
      44,    90,    87,   130,     3,    75,   151,   415,   155,    41,
     152,   152,   151,    98,   151,    59,   986,   150,    62,   846,
      64,   100,   153,   143,   878,   707,   188,   152,    98,    73,
     150,   568,   194,    19,   124,   178,   179,   180,   123,    90,
     867,   124,   150,   205,   152,   150,    90,   124,   150,   152,
     212,   213,   124,   123,   197,   441,   152,   200,   143,    51,
     150,    81,   319,   320,   124,   150,    41,   540,   915,   916,
     153,    41,   124,   105,   853,   124,   219,   124,   155,   933,
     150,   153,   124,   124,    46,    71,   143,   249,   249,    81,
     252,   123,   124,    72,   151,   155,   239,    76,   152,   151,
     154,    19,   143,    24,   153,    67,   153,    93,   152,   151,
       7,   143,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   105,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,    62,   152,   124,   804,   152,   403,   128,   168,   124,
     538,   539,    73,    71,   124,   348,    24,   152,   124,   321,
     322,   323,   152,   152,   701,   154,   186,   187,   330,    90,
     332,    24,   153,   699,   336,    93,   338,    83,   651,   124,
     155,    75,   124,   124,   204,   155,   348,   153,   350,   351,
     333,   353,   354,    87,    62,    68,   358,   359,   143,   361,
     143,   124,   124,   223,    98,    73,    75,   278,   279,    62,
     736,   153,   153,   375,   357,   143,   604,   605,    87,    75,
      73,   143,   124,   984,   105,   387,   124,   389,    96,    98,
     153,    87,   993,     1,   396,     3,   143,    90,   124,    37,
      28,   143,    98,   124,   437,   143,   124,   128,   410,   143,
     321,   322,   414,   415,   822,    53,   150,   143,   149,   421,
      72,   332,    60,    61,    76,   143,   428,   429,   430,   143,
     432,   433,   153,   143,   143,   437,   595,    60,    61,   815,
     351,   150,   143,   354,    72,   143,   822,   143,    76,     2,
     424,   152,   426,   154,   150,   143,   458,   459,    11,   319,
     320,   123,   152,   143,   126,   126,   150,    20,   130,    22,
     143,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,    99,   854,   152,
     143,   804,   152,   152,   105,   123,   143,   125,   126,     7,
      52,   867,   130,   123,   150,    58,   143,   519,   520,   137,
     512,   513,   514,   515,   516,   517,   518,   428,   146,   105,
     153,   124,   160,    41,   143,   152,    31,    80,   530,   153,
     532,    84,   153,   124,   536,   911,   396,   153,   540,   105,
     178,   179,   180,   403,   577,    18,   153,   185,   550,    41,
     124,   534,   535,   153,    41,   538,   539,   155,   934,   197,
      24,   421,   200,   150,   424,   143,   426,   943,   570,   429,
     572,   573,   574,   575,   576,   577,   143,    41,   143,   143,
      44,   219,   143,    75,   143,   143,   143,   140,    75,   143,
     153,    31,   424,   143,   426,    59,   143,   143,    62,   143,
      64,   239,   513,   979,   515,   143,    98,   609,   984,    73,
     143,    98,   143,   105,   155,   143,   143,   993,   105,   143,
     124,   997,   143,    19,   153,   151,    90,    23,   153,   143,
     150,   124,   124,   105,   151,   188,    32,   124,    34,    35,
      73,   194,    38,    29,    29,   155,   123,    43,   150,   651,
      46,   143,   654,   655,   656,    28,   143,   659,   150,    31,
      73,   153,   123,   150,    60,   525,   153,   527,    64,   155,
      66,    67,   674,    69,   676,    71,   155,   679,   126,   155,
     682,   664,   665,   151,   717,   687,   152,   689,   671,   691,
     692,   126,   694,   525,    90,   333,   249,    93,   681,    72,
      31,   684,   153,    76,    98,    41,   153,   151,    87,   155,
     415,   414,   734,    19,   716,   717,   231,   721,   174,   357,
     529,    24,   790,   739,   853,   561,   728,   854,   730,   699,
     986,   733,   734,   742,   933,   958,   738,    29,    41,    75,
     390,    44,    25,   654,   655,    -1,   119,   143,   580,   122,
     123,   527,   125,   126,    -1,   151,    59,   130,    -1,    62,
      -1,    64,    98,    -1,   137,    -1,   768,    -1,    -1,   105,
      73,   144,   145,   146,    -1,    -1,    -1,   330,    -1,   781,
     782,    -1,   784,   785,   786,   787,    -1,    90,   124,    -1,
     792,   793,    -1,   795,    -1,   797,   798,   350,    -1,   801,
     353,    -1,   804,   843,   844,    -1,    -1,   143,   361,   811,
      -1,    -1,    -1,   815,   150,    -1,    -1,   153,    -1,   821,
     822,   823,    -1,    -1,    -1,   827,    -1,   829,    -1,    -1,
     832,   833,     2,    -1,   836,    -1,    -1,    -1,   840,   840,
     842,    11,   825,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    22,    -1,   887,    -1,   858,   859,   860,   861,
     862,   863,   864,   865,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,   875,   876,    -1,    -1,   430,   880,    -1,
     882,   883,   884,   885,   886,   887,    -1,    -1,    58,   891,
      60,    61,   894,    -1,    -1,    -1,   534,   535,    -1,    -1,
     538,   539,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   911,
      80,    -1,    -1,   551,    84,    72,    -1,    -1,    -1,    76,
      -1,   954,    -1,    -1,   926,   927,   928,   929,    -1,    -1,
      -1,    -1,   934,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   943,    -1,   945,   946,   947,    -1,   949,   950,   951,
     952,   953,   954,   591,    -1,   815,   509,    -1,    -1,    -1,
     962,   963,   964,   965,   966,   967,   123,    -1,   125,   126,
     140,    -1,    -1,   130,    -1,    -1,   978,   979,    -1,    -1,
     137,    -1,   984,    -1,    -1,    -1,    -1,   144,   145,   146,
      -1,   993,    23,    -1,    -1,   997,    -1,   550,    -1,    -1,
      -1,    -1,     2,    34,    35,    23,    -1,    -1,    -1,    -1,
      -1,    11,    43,    -1,    -1,    46,    34,    35,   188,    -1,
      20,    52,    22,    -1,   194,    43,   664,   665,    46,    60,
      -1,   891,    -1,   671,    52,   673,    67,    68,    -1,    -1,
     678,    -1,    60,   681,    19,    -1,   684,    -1,    23,    67,
      68,    -1,    -1,    23,    -1,    28,   609,    32,    58,    34,
      35,    -1,    -1,    38,    34,    35,    -1,    -1,    43,    -1,
      -1,    46,   625,    43,   934,    -1,    46,    -1,    -1,   249,
      80,    -1,    -1,    -1,    84,    60,   724,    -1,    -1,    64,
      60,    66,    67,    -1,    69,    -1,    71,    67,    -1,    72,
      -1,   654,   655,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    93,   979,
      -1,   674,    -1,   676,    -1,    -1,   679,    -1,    -1,   682,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   997,   691,    -1,
     140,   694,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
     330,    -1,    -1,   716,   137,    -1,    -1,    -1,   143,    -1,
      -1,   144,   145,   146,    -1,   728,   151,   730,    -1,    -1,
     350,   734,     1,   353,    -1,    -1,    -1,   825,   188,    -1,
      -1,   361,    -1,    -1,   194,    -1,    -1,    -1,    28,    -1,
      19,    -1,    -1,    -1,    23,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    34,    35,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,
      -1,   869,   870,    -1,   872,   873,    -1,    -1,    -1,   792,
      -1,    60,    72,    62,   797,    64,    76,    66,    67,   249,
      69,    -1,    71,    -1,    73,    -1,    -1,   895,   811,    -1,
     430,    -1,    -1,   901,     2,   903,    -1,   905,    -1,    -1,
     823,    90,    -1,    11,    93,    -1,    -1,    96,    -1,   832,
      -1,    -1,    20,   836,    22,    -1,    -1,   840,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,    -1,     2,
      -1,   141,    -1,    -1,   144,   145,   146,    -1,    11,    -1,
      58,    -1,    -1,   876,   143,    -1,    -1,    20,    -1,    22,
     330,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    -1,   986,    -1,
     350,    10,    -1,   353,    -1,    -1,    -1,    -1,    17,    -1,
      -1,   361,    -1,     1,    -1,    58,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   926,   927,   928,   929,    -1,    -1,    -1,
     550,    19,    -1,    -1,    -1,    23,    24,    80,    -1,    -1,
      -1,    84,   945,   946,    32,    -1,    34,    35,    -1,    -1,
      38,    -1,   140,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    60,    -1,    62,    -1,    64,    -1,    66,    67,
     430,    69,    -1,    71,    -1,    73,    -1,    -1,    -1,   609,
      -1,   100,    -1,    -1,    -1,   104,    -1,   140,    -1,    -1,
     188,    -1,    90,    -1,    -1,    93,   194,    -1,    96,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    -1,   144,   145,   146,    28,    -1,
      -1,    -1,    -1,    -1,    -1,   188,   155,    -1,    -1,    -1,
      -1,   194,    -1,    -1,   674,   143,   676,    -1,    -1,   679,
      -1,   249,   682,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   691,    -1,    -1,   694,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   716,    -1,    -1,    -1,
     550,    -1,     2,    -1,    -1,    -1,   249,    -1,   728,    -1,
     730,    11,    -1,    -1,   734,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,    -1,
     130,    -1,   330,    -1,    -1,    -1,   136,   137,    -1,    -1,
      -1,   141,    -1,    -1,   144,   145,   146,    -1,    58,   609,
      -1,    -1,   350,    -1,    -1,   353,    -1,    -1,    -1,    -1,
      -1,    -1,   792,   361,    -1,    -1,    -1,   797,    -1,    -1,
      80,    -1,    -1,    -1,    84,    -1,    -1,   330,    -1,    -1,
      -1,   811,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   823,    -1,    -1,    -1,   350,    -1,    -1,
     353,    -1,   832,     2,    -1,    -1,   836,    -1,   361,    -1,
     840,    -1,    11,    -1,   674,    -1,   676,    -1,    -1,   679,
      -1,    20,   682,    22,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   691,   430,    -1,   694,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   876,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   716,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   728,    -1,
     730,    -1,    -1,    -1,   734,    -1,    19,   430,   188,    -1,
      23,    80,    -1,    -1,   194,    84,    -1,    -1,    -1,    32,
      -1,    34,    35,    -1,    -1,    38,   926,   927,   928,   929,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   945,   946,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    -1,    69,    -1,    71,    -1,
      -1,    -1,   792,    -1,    -1,    -1,    -1,   797,    -1,   249,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      93,   811,   550,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   823,    -1,    -1,    12,    -1,    -1,    -1,
      16,    -1,   832,    -1,    -1,    -1,   836,    -1,    -1,    -1,
     840,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,   188,
      36,    37,    -1,    39,    -1,   194,    42,   550,    -1,    45,
     143,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,   609,    -1,    -1,    -1,    61,   876,    63,    -1,    65,
     330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
     350,    -1,    -1,   353,    -1,    -1,    -1,    -1,    94,    -1,
     249,   361,    -1,    -1,    -1,    -1,   609,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   926,   927,   928,   929,
      -1,    -1,    -1,    -1,   120,    -1,   674,    -1,   676,     2,
      -1,   679,    -1,    -1,   682,   945,   946,    -1,    11,    28,
      -1,    -1,   138,   691,    -1,    -1,   694,    20,    -1,    22,
      -1,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   716,    -1,
     430,   674,    -1,   676,    -1,    -1,   679,    -1,    -1,   682,
     728,   330,   730,    72,    -1,    58,   734,    76,   691,    -1,
      -1,   694,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   350,    -1,    -1,   353,    -1,    -1,    80,    -1,    -1,
      -1,    84,   361,   716,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   728,    -1,   730,    -1,    -1,
     119,   734,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   792,   134,   135,   136,   137,   797,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,     2,   811,    -1,    -1,    28,   140,    -1,    -1,
      -1,    11,    -1,    -1,    -1,   823,    -1,    -1,    -1,    -1,
      20,   430,    22,    -1,   832,    -1,    -1,    -1,   836,   792,
     550,    -1,   840,    -1,   797,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   811,    -1,
      72,    -1,    -1,    -1,    76,   188,    -1,    -1,    58,    -1,
     823,   194,    -1,    -1,    -1,    -1,    -1,    -1,   876,   832,
      -1,    -1,    -1,   836,    -1,    -1,    -1,   840,    -1,    -1,
      80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,   609,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,   130,    -1,
      10,    -1,    -1,   876,    -1,   137,   249,    17,   926,   927,
     928,   929,   144,   145,   146,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   945,   946,    -1,
     140,   550,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   674,    -1,   676,    -1,    -1,   679,
      -1,    -1,   682,   926,   927,   928,   929,    -1,    -1,    -1,
      -1,   691,    72,    -1,   694,    -1,    76,    -1,    -1,    -1,
      -1,    -1,   945,   946,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,   194,    -1,   716,   330,    -1,    -1,
     609,    -1,    -1,    -1,   104,    -1,    -1,    -1,   728,    -1,
     730,    -1,    -1,    -1,   734,    -1,    -1,   350,    -1,   119,
     353,   121,   122,   123,    -1,   125,   126,   127,   361,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,   249,
      -1,    10,    -1,    -1,    -1,   155,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,   674,    -1,   676,    -1,    28,
     679,    -1,   792,   682,    -1,    -1,    -1,   797,    -1,    -1,
      -1,    -1,   691,    -1,    -1,   694,    -1,    -1,    -1,    -1,
      -1,   811,    -1,    -1,    -1,    -1,    -1,   430,    -1,    -1,
      -1,    -1,    -1,   823,    -1,    -1,    -1,   716,    -1,    -1,
      -1,    -1,   832,    72,    -1,    -1,   836,    76,    -1,   728,
     840,   730,    -1,    -1,    -1,   734,    -1,    -1,    -1,    -1,
     330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
     350,    -1,    -1,   353,    -1,    -1,   876,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,   792,    -1,   144,   145,   146,   797,    28,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,   811,    -1,    -1,    -1,   926,   927,   928,   929,
      -1,    -1,    -1,    -1,   823,    -1,    -1,   550,    -1,    -1,
      -1,    -1,    -1,   832,    -1,   945,   946,   836,    -1,    13,
     430,   840,    -1,    72,    -1,    19,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   876,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   609,    -1,    62,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,    -1,   134,   135,   136,   137,    -1,
     139,    -1,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   926,   927,   928,
     929,   105,    -1,   107,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   945,   946,    -1,    -1,
      -1,   674,    -1,   676,    -1,    -1,   679,    -1,    -1,   682,
     550,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   691,    -1,
      -1,   694,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,   162,    -1,
      -1,    -1,    -1,   716,   168,    -1,    -1,    -1,    -1,   173,
      -1,    -1,   176,    -1,    -1,   728,    -1,   730,    -1,    -1,
      -1,   734,   186,   187,    -1,    -1,   190,    -1,    -1,   609,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     204,   121,   122,   123,   208,   125,   126,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,   136,   137,    -1,   223,
      -1,   141,     3,    -1,   144,   145,   146,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,   792,
      -1,    -1,    -1,    -1,   797,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   674,    -1,   676,    -1,   811,   679,
      -1,    -1,   682,    -1,    45,   269,   270,   271,   272,    -1,
     823,   691,    -1,    -1,   694,    -1,    -1,    -1,    -1,   832,
      -1,    -1,   286,   836,    -1,    -1,    -1,   840,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   716,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   728,    -1,
     730,    -1,    -1,    -1,   734,   319,   320,    -1,    -1,    -1,
      -1,    -1,    -1,   876,   105,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   346,    -1,   125,    -1,    -1,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
     141,    -1,    -1,   144,   145,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   926,   927,   928,   929,    -1,   382,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,
      -1,   811,   945,   946,    -1,    -1,    -1,    -1,    -1,   403,
      -1,    -1,    -1,   823,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   832,    -1,    -1,    -1,   836,    -1,    -1,    -1,
     840,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   436,    -1,    -1,   439,    -1,   441,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,     1,   876,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   148,   149,    -1,    30,   152,    -1,   154,    -1,    -1,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,   527,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     564,    -1,    -1,    -1,   568,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   591,    -1,    -1,
     136,   595,   138,    -1,   140,   141,   142,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,   155,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,   625,    -1,    -1,    12,    13,    14,    -1,    16,    -1,
      -1,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      -1,    29,    30,    -1,    32,    -1,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    -1,    71,    -1,    73,    74,    75,    -1,    77,
      78,    79,    80,    81,   698,    83,    84,   701,    86,    -1,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,   721,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,   739,    -1,    -1,    -1,   127,
      -1,    -1,    -1,   747,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,   143,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,     1,   154,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    -1,
      16,    17,    -1,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    34,    35,
      36,    37,    38,    39,    -1,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    83,    84,   853,
      86,    -1,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,   123,   124,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,    -1,   143,   144,   145,
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
      -1,    -1,   136,    -1,   138,    -1,   140,   141,   142,   143,
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
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,
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
      12,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
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
     155,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
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
      -1,    -1,   150,    -1,   152,    -1,   154,   155,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,   155,     3,     4,     5,     6,     7,
       8,     9,    -1,    -1,    12,    -1,    -1,    -1,    16,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    -1,    -1,    42,    -1,    -1,    45,    46,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
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
      -1,   127,    -1,    -1,     3,     4,     5,     6,     7,     8,
     136,    -1,   138,    12,   140,   141,    -1,    16,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    88,
      89,    -1,    -1,    92,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,   128,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    84,    -1,    -1,    -1,    88,    89,    -1,    -1,
      92,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
     142,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,   153,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,
      -1,    -1,    -1,    88,    89,    -1,    -1,    92,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,     7,
       8,   136,    -1,   138,    12,   140,   141,   142,    16,    -1,
      -1,   146,    -1,    -1,    -1,   150,    24,   152,   153,   154,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    -1,    41,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    -1,
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
      81,    -1,    83,    84,    -1,    -1,    -1,    88,    89,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,
      42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      -1,    83,    84,    -1,    -1,    -1,    88,    89,    -1,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,     4,
       5,     6,     7,     8,   136,    -1,   138,    12,   140,   141,
      -1,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,   140,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    35,    36,    37,    -1,    39,    -1,    -1,    42,
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
      46,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    88,
      89,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
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
     142,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,
     152,    -1,   154,    -1,    29,    30,    -1,    -1,    -1,    -1,
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
     138,    12,   140,   141,   142,    16,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
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
       7,     8,   136,    -1,   138,    12,   140,   141,   142,    16,
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
      -1,   138,    12,   140,   141,   142,    16,    -1,    -1,   146,
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
      -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,   136,    16,   138,    -1,    -1,
     141,   147,   148,    -1,    -1,   146,   152,    -1,   154,   150,
      30,   152,    -1,   154,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,   105,    -1,    12,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    30,    -1,    -1,   128,    -1,
      -1,    36,    37,    -1,    39,    -1,    -1,    42,   138,    -1,
      45,    -1,   142,    48,    49,    -1,    51,    -1,    53,    54,
     150,    -1,   152,   153,   154,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      16,    -1,    -1,   138,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    30,   150,    -1,   152,    -1,   154,
      36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,   138,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,     3,   150,    -1,   152,    61,   154,    63,
      10,    65,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,   120,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
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
      -1,    -1,    -1,    28,    -1,    72,   155,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    76,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    72,   144,   145,   146,
      76,    -1,    -1,    -1,    -1,    10,   153,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    28,   129,   130,   131,   132,   104,   134,
     135,   136,   137,    -1,   139,    -1,   141,    -1,    -1,   144,
     145,   146,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,    -1,    -1,    10,   153,    -1,    -1,
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
      -1,    -1,    28,    29,    -1,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,    -1,    41,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    72,   144,   145,   146,
      76,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,   104,    28,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    -1,   144,   145,
     146,    -1,    -1,    72,   150,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    72,   144,   145,   146,    76,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,   104,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
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
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    72,   144,   145,   146,    76,    -1,    -1,
     150,    -1,    10,    -1,    -1,    -1,    85,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    72,   144,   145,   146,    76,    -1,
      -1,   150,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    72,   144,   145,   146,    76,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    57,   139,   140,   141,    -1,    10,   144,   145,   146,
      -1,    -1,   149,    17,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,   143,   144,   145,
     146,    10,    -1,    -1,    -1,    -1,   100,    -1,    17,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    33,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    57,    -1,
     144,   145,   146,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    72,   125,   126,   127,    76,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    10,    -1,   144,   145,   146,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    10,    -1,   144,   145,   146,
      -1,    -1,    17,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    72,   125,   126,
     127,    76,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,   143,   144,   145,   146,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,   104,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   104,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,   143,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    10,   143,   144,   145,   146,    -1,    -1,
      17,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    72,   125,   126,   127,    76,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    10,   143,   144,   145,   146,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    10,    -1,   144,   145,   146,
      -1,    -1,    17,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    72,   125,   126,
     127,    76,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    -1,   144,   145,   146,
      -1,    10,    11,    -1,    -1,   100,    -1,    -1,    17,   104,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   104,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    72,   125,   126,   127,    76,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    10,   143,   144,   145,   146,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    28,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    10,    -1,   144,   145,   146,    -1,    -1,
      17,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    72,   125,   126,   127,    76,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   104,   139,   140,   141,    -1,    -1,   144,   145,   146,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    10,
      -1,   144,   145,   146,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    28,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    85,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   104,   144,   145,   146,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    85,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,   104,   144,   145,   146,    -1,    -1,    10,    11,    -1,
      -1,    -1,    -1,    -1,    17,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    28,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      -1,   144,   145,   146,    -1,    -1,    10,    -1,    -1,    -1,
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
     144,   145,   146,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    28,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    28,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,   104,    -1,   144,   145,   146,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   104,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146
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
     209,   233,   236,   237,   257,   258,   259,   260,   261,   262,
     263,   271,   272,   273,   274,   277,   278,   279,   280,   281,
     282,   284,   285,   286,   287,   288,   143,   164,   274,   100,
     264,   265,   176,   177,    12,    36,    37,    39,    84,   150,
     154,   205,   257,   262,   272,   273,   274,   277,   279,   280,
     264,   274,    90,   176,   164,   251,   274,   164,   152,   274,
     275,     7,     9,    90,   178,   275,    62,   102,   186,   274,
     274,    23,    35,   236,   274,   274,   152,   152,   177,   206,
     152,   177,   238,   239,    29,   168,   179,   274,   274,   274,
     274,   274,   274,     7,   152,    35,   182,   182,   142,   229,
     250,   274,    88,    89,   143,   274,   276,   274,   179,   274,
     274,   274,    83,   152,   164,   274,   274,   168,   175,   274,
     277,   168,   175,   274,   177,   234,   274,   250,   274,   274,
     274,   274,   274,   274,   274,   274,     1,   151,   162,   169,
     250,    92,   128,   229,   252,   253,   276,   250,   274,   283,
      64,   164,    68,   167,   177,   177,    46,    67,   224,    24,
      62,    73,    96,   143,   149,    10,    17,    28,    72,    76,
     104,   119,   121,   122,   123,   125,   126,   127,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   139,   140,   141,
     144,   145,   146,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   147,   148,   152,
     154,    72,    76,   152,   164,   143,   274,   274,   274,   250,
     150,   168,    41,   264,   234,   143,   124,   143,    99,   177,
     229,   254,   255,   256,   276,   234,   198,   177,   152,   179,
      29,    41,   179,    29,    41,   100,   179,   267,    33,    57,
     143,    85,   105,   106,   107,   108,   109,   110,   113,   114,
     115,   116,   117,   118,   168,   216,   254,   254,   164,   152,
     218,    92,   152,   177,   240,   241,     1,   123,   245,    41,
     124,   164,   179,   179,   254,   177,   124,   143,   274,   274,
     143,   150,   179,   152,   254,   143,   190,   143,   190,   143,
     105,   235,   143,   143,    33,    57,   179,   143,   151,   151,
     162,   124,   151,   274,   124,   153,   124,   153,    41,   124,
     155,   267,   103,   124,   155,     7,    52,   123,   199,   150,
     210,    73,   238,   238,   238,   238,   274,   274,   274,   274,
     186,   274,   186,   274,   274,   274,   274,   274,    16,    94,
     274,   274,    30,    90,   177,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   254,   254,   186,
     274,   186,   274,    24,    41,    73,    96,   266,   269,   270,
     274,   288,    29,    41,    29,    41,    85,    41,   155,   186,
     274,   179,   143,   274,   274,   105,   153,   124,   143,   177,
      41,   250,    39,   274,   186,   274,    39,   274,   186,   274,
     152,   179,   146,   170,   172,   274,   170,   171,   164,   274,
      31,   274,   153,   153,    24,    41,    44,    59,    62,    73,
      90,   163,   219,   220,   221,   222,   207,   241,   124,   153,
      16,    30,    61,    79,    80,    84,    94,   154,   246,   259,
     105,   242,   274,   239,   153,   229,   274,     1,   195,   254,
     153,    18,   191,   246,   259,   124,   172,   171,   151,   153,
     153,   252,   153,   252,   186,   274,   155,   164,   274,   155,
     274,   155,   274,   177,   250,   150,     1,   177,   203,   204,
      24,    62,    73,    90,   212,   223,   238,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   153,   155,    41,
      73,   124,   153,   257,    72,    72,    39,   186,   274,    39,
     186,   274,   186,   274,   264,   264,   143,   229,   276,   256,
     199,   274,   153,   274,    29,   179,    29,   179,   274,    29,
     179,   267,    29,   179,   267,   268,   269,   124,   143,    11,
     143,    31,    31,   164,    85,   168,    41,    73,   221,   124,
     153,   152,   177,    24,    62,    73,    90,   225,   153,   241,
     245,     1,   250,    55,   276,   151,    58,    97,   151,   196,
     153,   152,   168,   177,   192,   234,   143,   143,   155,   267,
     155,   267,   164,   103,   150,     1,   200,   151,   105,   124,
     151,    73,   211,    10,    17,    45,   119,   120,   121,   122,
     125,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   141,   144,   145,   146,   147,   149,   152,   177,
     213,   214,   215,   216,   257,   143,   269,   245,   257,   257,
     274,    29,    29,   274,    29,    29,   155,   155,   179,   179,
     150,   179,    85,    39,   274,    39,   274,    85,    39,   274,
     179,    39,   274,   179,   124,   153,   274,   274,   164,   274,
     164,    31,   177,   220,   241,   123,   249,    73,   245,   242,
     155,    41,   155,    29,   164,   250,   192,   123,   168,    39,
     164,   274,   155,    39,   164,   274,   155,   274,     1,   151,
     169,   151,    35,    66,    69,   151,   163,   181,   274,   204,
     225,   177,   274,   126,   152,   217,   217,   242,    85,    39,
      39,    85,    39,    39,    39,    39,     1,   200,   274,   274,
     274,   274,   274,   274,   269,    11,    31,   164,   249,   153,
      16,    30,    61,    79,    80,    84,    94,   154,   229,   248,
     259,   128,   231,   242,    98,   232,   274,   246,   259,   164,
     179,   153,   274,   274,   164,   274,   164,   151,   151,   181,
       1,   123,   244,   153,   177,   215,   216,   219,   274,   274,
     274,   274,   274,   274,   151,   151,    85,    85,    85,    85,
     274,   164,   242,   249,   155,   250,   229,   230,   274,   274,
     168,   180,   228,   155,   168,    85,    85,    16,    30,    61,
      79,    80,    84,    94,   154,   243,   259,    87,   226,   217,
     217,   153,    85,    85,    85,    85,    85,    85,   274,   274,
     274,   274,   231,   242,   229,   247,   248,   259,    41,   155,
     259,   274,   274,     1,   155,   250,   232,   274,   247,   248,
     155,   243,   259,   155,   143,   227,   228,   155,   243,   259,
     247
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
     212,   212,   213,   214,   214,   214,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   217,   217,   218,   219,   219,   219,
     220,   220,   220,   220,   220,   221,   221,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   223,   223,   223,   223,
     223,   223,   224,   224,   225,   225,   225,   225,   225,   225,
     226,   226,   227,   227,   228,   228,   229,   229,   230,   230,
     231,   231,   232,   232,   233,   233,   233,   234,   234,   235,
     235,   235,   236,   236,   236,   236,   236,   237,   237,   237,
     238,   238,   239,   239,   240,   240,   240,   241,   241,   241,
     242,   242,   242,   243,   243,   243,   243,   243,   243,   243,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   246,   246,   246,   246,   247,   247,   247,   248,
     248,   248,   248,   248,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   250,   250,   250,   250,   251,
     251,   252,   252,   252,   253,   253,   254,   254,   255,   255,
     256,   256,   256,   256,   257,   258,   258,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   261,   262,   263,   263,   263,   263,   263,   263,   263,
     263,   264,   264,   265,   266,   266,   267,   268,   268,   269,
     269,   269,   270,   270,   270,   270,   270,   270,   271,   271,
     272,   273,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   275,   275,
     276,   276,   276,   277,   277,   277,   277,   278,   278,   279,
     279,   279,   280,   280,   280,   281,   281,   281,   281,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     283,   283,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   285,   285,   285,   285,   285,
     285,   286,   286,   286,   286,   287,   287,   287,   287,   288,
     288,   288,   288,   288,   288,   288
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
       5,     5,     1,     1,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     5,     4,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     2,     1,     0,     1,
       2,     2,     0,     2,     3,     4,     4,     2,     4,     0,
       2,     2,     4,     4,     4,     5,     4,     0,     1,     1,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     3,     2,     4,     3,     3,     4,     3,
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     4,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       2,     2,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     3,     3,     2,     2,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
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
  unsigned long yylno = yyrline[yyrule];
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
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (!yyps->yynew && yyps->yyss != yyps->yyssa)
        YYSTACK_FREE (yyps->yyss);
#endif
      free (yyps);
    }
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
                  (unsigned long) yystacksize));

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
#line 447 "chapel.ypp" /* yacc.c:1666  */
    { yyblock = (yyval.pblockstmt); }
#line 5254 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5260 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5266 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5272 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5278 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5284 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5290 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5296 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5302 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5308 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5314 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5320 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5326 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5332 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5338 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5344 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5350 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5356 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5362 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5368 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5374 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5380 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5387 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5393 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1666  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5400 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5406 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5412 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5418 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 5424 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 5430 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5436 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5442 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5448 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5454 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5460 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5470 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5480 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5489 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1666  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5498 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5508 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5518 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5524 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5530 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5536 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5542 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5548 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5554 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5560 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5566 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5572 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5578 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 70:
#line 610 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5584 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 71:
#line 612 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5590 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 72:
#line 614 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5596 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 73:
#line 616 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5602 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 74:
#line 618 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5608 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 75:
#line 620 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5614 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 76:
#line 622 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5620 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 77:
#line 624 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5626 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 78:
#line 626 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5632 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 79:
#line 628 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5638 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 80:
#line 632 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 5644 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 82:
#line 637 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5650 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 83:
#line 641 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = NULL; }
#line 5656 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 84:
#line 642 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5662 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 85:
#line 646 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5668 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 86:
#line 647 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5674 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 87:
#line 651 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5680 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 88:
#line 652 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5686 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 89:
#line 656 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5692 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 91:
#line 658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5698 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 92:
#line 659 "chapel.ypp" /* yacc.c:1666  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5704 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 99:
#line 672 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5710 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 100:
#line 673 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5716 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 101:
#line 674 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5722 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 102:
#line 675 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5728 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 103:
#line 680 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 5736 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 104:
#line 686 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5742 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 105:
#line 687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5748 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 106:
#line 688 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5754 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 107:
#line 689 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5760 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 108:
#line 690 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5766 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 109:
#line 691 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5772 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 110:
#line 692 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5778 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 111:
#line 693 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5784 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 112:
#line 694 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5790 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 113:
#line 695 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5796 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 114:
#line 696 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5802 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 115:
#line 697 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5808 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 116:
#line 698 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5814 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 117:
#line 699 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5820 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 118:
#line 700 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5826 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 119:
#line 701 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5832 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 120:
#line 703 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5842 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 121:
#line 709 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5852 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 122:
#line 715 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5862 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 123:
#line 721 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5872 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 124:
#line 727 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5883 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 125:
#line 734 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5894 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 126:
#line 743 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5900 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 127:
#line 747 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5906 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 128:
#line 748 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5912 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 129:
#line 749 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5918 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 130:
#line 750 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5924 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 131:
#line 751 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5931 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 132:
#line 753 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5938 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 133:
#line 755 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5945 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 134:
#line 757 "chapel.ypp" /* yacc.c:1666  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5952 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 135:
#line 762 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 5958 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 136:
#line 765 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5964 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 137:
#line 766 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5970 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 138:
#line 767 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5976 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 139:
#line 768 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5982 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 140:
#line 769 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5988 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 141:
#line 770 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5994 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 142:
#line 774 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6000 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 143:
#line 775 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6006 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 144:
#line 779 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6012 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 145:
#line 780 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6018 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 146:
#line 781 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 147:
#line 785 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6030 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 148:
#line 786 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6036 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 149:
#line 790 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6042 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 150:
#line 794 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6048 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 151:
#line 796 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6054 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 152:
#line 800 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6060 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 153:
#line 801 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6066 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 154:
#line 806 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6072 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 155:
#line 808 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6078 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 156:
#line 810 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6084 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 157:
#line 817 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6098 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 158:
#line 827 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6112 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 159:
#line 837 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6126 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 160:
#line 847 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6140 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 161:
#line 859 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6150 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 162:
#line 864 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6160 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 163:
#line 869 "chapel.ypp" /* yacc.c:1666  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6170 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 164:
#line 877 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 6176 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 165:
#line 878 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6182 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 166:
#line 883 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6188 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 167:
#line 885 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6194 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 168:
#line 887 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6200 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 169:
#line 892 "chapel.ypp" /* yacc.c:1666  */
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
#line 6220 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 170:
#line 908 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6228 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 171:
#line 915 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6238 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 172:
#line 924 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6250 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 173:
#line 932 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6258 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 174:
#line 936 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6266 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 175:
#line 942 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6272 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 176:
#line 943 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6278 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 177:
#line 948 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6287 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 178:
#line 953 "chapel.ypp" /* yacc.c:1666  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6296 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 179:
#line 958 "chapel.ypp" /* yacc.c:1666  */
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
#line 6316 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 180:
#line 978 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6327 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 181:
#line 984 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6339 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 182:
#line 991 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6350 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 183:
#line 997 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6361 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 184:
#line 1003 "chapel.ypp" /* yacc.c:1666  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6373 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 185:
#line 1014 "chapel.ypp" /* yacc.c:1666  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6383 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 186:
#line 1020 "chapel.ypp" /* yacc.c:1666  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6394 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 187:
#line 1027 "chapel.ypp" /* yacc.c:1666  */
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
#line 6421 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 188:
#line 1053 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6429 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 189:
#line 1057 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6438 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 190:
#line 1062 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6446 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 191:
#line 1066 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6455 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 192:
#line 1073 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6461 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 195:
#line 1079 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6467 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 197:
#line 1084 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6473 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 198:
#line 1085 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&"; }
#line 6479 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 199:
#line 1086 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|"; }
#line 6485 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 200:
#line 1087 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^"; }
#line 6491 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 201:
#line 1088 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "~"; }
#line 6497 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 202:
#line 1089 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "=="; }
#line 6503 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 203:
#line 1090 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!="; }
#line 6509 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 204:
#line 1091 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<="; }
#line 6515 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 205:
#line 1092 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">="; }
#line 6521 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 206:
#line 1093 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<"; }
#line 6527 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 207:
#line 1094 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">"; }
#line 6533 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 208:
#line 1095 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+"; }
#line 6539 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 209:
#line 1096 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-"; }
#line 6545 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 210:
#line 1097 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*"; }
#line 6551 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 211:
#line 1098 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/"; }
#line 6557 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 212:
#line 1099 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<"; }
#line 6563 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 213:
#line 1100 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>"; }
#line 6569 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 214:
#line 1101 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%"; }
#line 6575 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 215:
#line 1102 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**"; }
#line 6581 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 216:
#line 1103 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "!"; }
#line 6587 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 217:
#line 1104 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_by"; }
#line 6593 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 218:
#line 1105 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "#"; }
#line 6599 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 219:
#line 1106 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "chpl_align"; }
#line 6605 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 220:
#line 1107 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<=>"; }
#line 6611 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 221:
#line 1108 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<~>"; }
#line 6617 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 222:
#line 1112 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "="; }
#line 6623 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 223:
#line 1113 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "+="; }
#line 6629 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 224:
#line 1114 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "-="; }
#line 6635 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 225:
#line 1115 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "*="; }
#line 6641 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 226:
#line 1116 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "/="; }
#line 6647 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 227:
#line 1117 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "%="; }
#line 6653 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 228:
#line 1118 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "**="; }
#line 6659 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 229:
#line 1119 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "&="; }
#line 6665 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 230:
#line 1120 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "|="; }
#line 6671 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 231:
#line 1121 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "^="; }
#line 6677 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 232:
#line 1122 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = ">>="; }
#line 6683 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 233:
#line 1123 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pch) = "<<="; }
#line 6689 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 234:
#line 1127 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6695 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 235:
#line 1128 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6701 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 236:
#line 1132 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6707 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 237:
#line 1136 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6713 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 238:
#line 1137 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6719 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 239:
#line 1138 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6725 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 240:
#line 1143 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6731 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 241:
#line 1145 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6737 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 242:
#line 1147 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6743 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 243:
#line 1149 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6749 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 244:
#line 1151 "chapel.ypp" /* yacc.c:1666  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6755 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 245:
#line 1155 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6761 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 246:
#line 1156 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6767 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 247:
#line 1160 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_IN; }
#line 6773 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 248:
#line 1161 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6779 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 249:
#line 1162 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_OUT; }
#line 6785 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 250:
#line 1163 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST; }
#line 6791 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 251:
#line 1164 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6797 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 252:
#line 1165 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6803 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 253:
#line 1166 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6809 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 254:
#line 1167 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF; }
#line 6815 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 255:
#line 1168 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6821 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 256:
#line 1172 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6827 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 257:
#line 1173 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6833 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 258:
#line 1174 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_REF;   }
#line 6839 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 259:
#line 1175 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6845 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 260:
#line 1176 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6851 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 261:
#line 1177 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6857 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 262:
#line 1181 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6863 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 263:
#line 1182 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6869 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 264:
#line 1186 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 6875 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 265:
#line 1187 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_VALUE; }
#line 6881 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 266:
#line 1188 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6887 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 267:
#line 1189 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_REF; }
#line 6893 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 268:
#line 1190 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_PARAM; }
#line 6899 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 269:
#line 1191 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.retTag) = RET_TYPE; }
#line 6905 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 270:
#line 1195 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = false; }
#line 6911 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 271:
#line 1196 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.b) = true;  }
#line 6917 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 272:
#line 1199 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = NULL; }
#line 6923 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 275:
#line 1205 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6929 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 276:
#line 1210 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6935 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 277:
#line 1212 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6941 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 278:
#line 1216 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6947 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 280:
#line 1221 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6953 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 281:
#line 1222 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6959 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 282:
#line 1226 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 6965 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 283:
#line 1227 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6971 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 284:
#line 1232 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6977 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 285:
#line 1234 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6983 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 286:
#line 1236 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6989 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 287:
#line 1241 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7006 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 288:
#line 1254 "chapel.ypp" /* yacc.c:1666  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7024 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 289:
#line 1270 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7030 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 290:
#line 1272 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7036 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 291:
#line 1274 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7042 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 292:
#line 1279 "chapel.ypp" /* yacc.c:1666  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7054 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 293:
#line 1287 "chapel.ypp" /* yacc.c:1666  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7066 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 294:
#line 1295 "chapel.ypp" /* yacc.c:1666  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7078 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 295:
#line 1303 "chapel.ypp" /* yacc.c:1666  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7091 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 296:
#line 1312 "chapel.ypp" /* yacc.c:1666  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7102 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 297:
#line 1321 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7108 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 298:
#line 1322 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7114 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 299:
#line 1323 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7120 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 301:
#line 1329 "chapel.ypp" /* yacc.c:1666  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7129 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 302:
#line 1337 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7135 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 303:
#line 1339 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7141 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 304:
#line 1344 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 305:
#line 1346 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 306:
#line 1348 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 307:
#line 1353 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 308:
#line 1355 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 309:
#line 1357 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 310:
#line 1363 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7183 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 311:
#line 1364 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7189 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 312:
#line 1365 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 313:
#line 1371 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7201 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 314:
#line 1373 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7207 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 315:
#line 1375 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7215 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 316:
#line 1379 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7223 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 317:
#line 1383 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7229 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 318:
#line 1385 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7237 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 319:
#line 1389 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7245 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 320:
#line 1396 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7251 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 321:
#line 1397 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7257 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 322:
#line 1398 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7263 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 323:
#line 1399 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7269 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 324:
#line 1400 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7275 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 325:
#line 1401 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7281 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 326:
#line 1402 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7287 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 327:
#line 1403 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7293 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 328:
#line 1404 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7299 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 329:
#line 1405 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7305 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 330:
#line 1406 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7311 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 331:
#line 1411 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7317 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 332:
#line 1412 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7323 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 333:
#line 1413 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7329 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 334:
#line 1414 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7335 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 335:
#line 1415 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7341 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 336:
#line 1416 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7347 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 337:
#line 1417 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7353 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 338:
#line 1418 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7359 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 339:
#line 1419 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7365 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 340:
#line 1420 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7371 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 341:
#line 1421 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7377 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 342:
#line 1442 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7385 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 343:
#line 1446 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7393 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 344:
#line 1450 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7405 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 345:
#line 1458 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7413 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 346:
#line 1464 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7419 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 347:
#line 1465 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7425 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 348:
#line 1466 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7431 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 349:
#line 1471 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7437 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 350:
#line 1473 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7443 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 351:
#line 1479 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7449 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 352:
#line 1481 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7455 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 353:
#line 1483 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7461 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 354:
#line 1487 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 7467 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 355:
#line 1488 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7473 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 356:
#line 1489 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7479 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 357:
#line 1490 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7485 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 358:
#line 1491 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7491 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 359:
#line 1492 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7497 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 360:
#line 1493 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7503 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 361:
#line 1494 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7509 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 362:
#line 1495 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7515 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 363:
#line 1496 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7521 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 364:
#line 1497 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7527 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 365:
#line 1503 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7533 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 366:
#line 1504 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7539 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 367:
#line 1505 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7545 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 368:
#line 1506 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7551 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 369:
#line 1510 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7557 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 370:
#line 1511 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7563 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 371:
#line 1515 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7569 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 372:
#line 1516 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7575 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 373:
#line 1517 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7581 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 374:
#line 1521 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7587 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 375:
#line 1522 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7593 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 376:
#line 1526 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7599 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 378:
#line 1531 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7605 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 379:
#line 1532 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7611 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 380:
#line 1536 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 381:
#line 1537 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7623 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 382:
#line 1538 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7629 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 383:
#line 1539 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7635 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 384:
#line 1543 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7641 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 385:
#line 1547 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 7647 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 386:
#line 1548 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol), (yyvsp[-1].pcallexpr)); }
#line 7653 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 393:
#line 1566 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7659 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 394:
#line 1568 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7665 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 395:
#line 1570 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7671 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 396:
#line 1572 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7677 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 397:
#line 1574 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7683 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 398:
#line 1576 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7689 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 399:
#line 1578 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7695 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 400:
#line 1580 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7701 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 401:
#line 1582 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7707 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 402:
#line 1584 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7713 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 403:
#line 1586 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7719 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 404:
#line 1591 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7725 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 405:
#line 1593 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7731 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 406:
#line 1595 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7737 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 407:
#line 1597 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7743 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 408:
#line 1599 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7749 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 409:
#line 1601 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7755 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 410:
#line 1603 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7761 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 411:
#line 1605 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7767 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 412:
#line 1607 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7773 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 413:
#line 1609 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7779 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 414:
#line 1611 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7785 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 415:
#line 1613 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7791 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 416:
#line 1615 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7802 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 417:
#line 1622 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7812 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 418:
#line 1628 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7822 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 419:
#line 1634 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7832 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 420:
#line 1640 "chapel.ypp" /* yacc.c:1666  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7842 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 421:
#line 1649 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7848 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 422:
#line 1658 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7854 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 430:
#line 1674 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7860 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 431:
#line 1678 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = NULL; }
#line 7866 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 433:
#line 1683 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7872 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 434:
#line 1687 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7878 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 435:
#line 1688 "chapel.ypp" /* yacc.c:1666  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7884 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 436:
#line 1692 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7890 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 437:
#line 1696 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7896 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 438:
#line 1697 "chapel.ypp" /* yacc.c:1666  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7902 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 439:
#line 1702 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 7910 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 440:
#line 1706 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7918 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 441:
#line 1710 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7926 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 442:
#line 1716 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 7932 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 443:
#line 1717 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 7938 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 444:
#line 1718 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 7944 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 445:
#line 1719 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 7950 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 446:
#line 1720 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 7956 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 447:
#line 1721 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 7962 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 449:
#line 1727 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7968 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 450:
#line 1732 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7974 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 451:
#line 1737 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7980 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 460:
#line 1753 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7986 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 461:
#line 1755 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7992 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 462:
#line 1757 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 7998 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 463:
#line 1759 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8004 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 464:
#line 1761 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8010 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 465:
#line 1763 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8016 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 466:
#line 1765 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8022 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 467:
#line 1767 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8028 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 468:
#line 1771 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = NULL; }
#line 8034 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 469:
#line 1772 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8040 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 470:
#line 1775 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8046 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 471:
#line 1776 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8052 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 472:
#line 1777 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8058 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 479:
#line 1799 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8064 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 480:
#line 1800 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8070 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 481:
#line 1801 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8076 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 482:
#line 1805 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8082 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 483:
#line 1806 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8088 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 484:
#line 1807 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8094 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 485:
#line 1815 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8100 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 486:
#line 1816 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8106 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 487:
#line 1817 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8112 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 488:
#line 1818 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8118 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 489:
#line 1822 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 8124 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 490:
#line 1823 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8130 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 491:
#line 1824 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8136 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 492:
#line 1825 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8142 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 493:
#line 1826 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8148 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 494:
#line 1827 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8154 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 495:
#line 1828 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8160 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 496:
#line 1829 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8166 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 497:
#line 1830 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8172 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 498:
#line 1832 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8180 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 499:
#line 1836 "chapel.ypp" /* yacc.c:1666  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8188 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 500:
#line 1843 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8194 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 501:
#line 1844 "chapel.ypp" /* yacc.c:1666  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8200 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 502:
#line 1848 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8206 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 503:
#line 1849 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8212 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 504:
#line 1850 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8218 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 505:
#line 1851 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8224 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 506:
#line 1852 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8230 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 507:
#line 1853 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8236 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 508:
#line 1854 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8242 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 509:
#line 1855 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8248 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 510:
#line 1856 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8254 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 511:
#line 1857 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8260 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 512:
#line 1858 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8266 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 513:
#line 1859 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8272 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 514:
#line 1860 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8278 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 515:
#line 1861 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8284 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 516:
#line 1862 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8290 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 517:
#line 1863 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8296 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 518:
#line 1864 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8302 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 519:
#line 1865 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8308 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 520:
#line 1866 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8314 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 521:
#line 1867 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8320 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 522:
#line 1868 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8326 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 523:
#line 1869 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8332 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 524:
#line 1870 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8338 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 525:
#line 1874 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8344 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 526:
#line 1875 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8350 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 527:
#line 1876 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8356 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 528:
#line 1877 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8362 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 529:
#line 1878 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8368 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 530:
#line 1879 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8374 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 531:
#line 1883 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8380 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 532:
#line 1884 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8386 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 533:
#line 1885 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8392 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 534:
#line 1886 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8398 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 535:
#line 1890 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8404 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 536:
#line 1891 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8410 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 537:
#line 1892 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8416 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 538:
#line 1893 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8422 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 539:
#line 1898 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8428 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 540:
#line 1899 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8434 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 541:
#line 1900 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8440 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 542:
#line 1901 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8446 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 543:
#line 1902 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8452 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 544:
#line 1903 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8458 "bison-chapel.cpp" /* yacc.c:1666  */
    break;

  case 545:
#line 1904 "chapel.ypp" /* yacc.c:1666  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8464 "bison-chapel.cpp" /* yacc.c:1666  */
    break;


#line 8468 "bison-chapel.cpp" /* yacc.c:1666  */
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
