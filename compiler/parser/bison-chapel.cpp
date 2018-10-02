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
#define YYLAST   15382

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  130
/* YYNRULES -- Number of rules.  */
#define YYNRULES  552
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1011

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
     629,   637,   638,   642,   646,   647,   651,   652,   656,   657,
     661,   662,   663,   664,   665,   666,   667,   668,   672,   673,
     677,   678,   679,   680,   684,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   707,   713,   719,   725,   731,   738,   748,   752,   753,
     754,   755,   756,   758,   760,   762,   767,   770,   771,   772,
     773,   774,   775,   779,   780,   784,   785,   786,   790,   791,
     795,   798,   800,   805,   806,   810,   812,   814,   821,   831,
     841,   851,   864,   869,   874,   882,   883,   888,   889,   891,
     896,   912,   919,   928,   936,   940,   947,   948,   953,   958,
     952,   983,   989,   996,  1002,  1008,  1019,  1025,  1018,  1057,
    1061,  1066,  1070,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1088,  1089,  1090,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
    1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,  1123,
    1124,  1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,
    1134,  1138,  1139,  1143,  1147,  1148,  1149,  1153,  1155,  1157,
    1159,  1161,  1166,  1167,  1171,  1172,  1173,  1174,  1175,  1176,
    1177,  1178,  1179,  1183,  1184,  1185,  1186,  1187,  1188,  1192,
    1193,  1197,  1198,  1199,  1200,  1201,  1202,  1206,  1207,  1210,
    1211,  1215,  1216,  1220,  1222,  1227,  1228,  1232,  1233,  1237,
    1238,  1242,  1244,  1246,  1251,  1264,  1281,  1282,  1284,  1289,
    1297,  1305,  1313,  1322,  1332,  1333,  1334,  1338,  1339,  1347,
    1349,  1354,  1356,  1358,  1363,  1365,  1367,  1374,  1375,  1376,
    1381,  1383,  1385,  1389,  1393,  1395,  1399,  1407,  1408,  1409,
    1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1452,
    1456,  1460,  1468,  1475,  1476,  1477,  1481,  1483,  1489,  1491,
    1493,  1498,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,
    1507,  1508,  1514,  1515,  1516,  1517,  1521,  1522,  1526,  1527,
    1528,  1532,  1533,  1537,  1538,  1542,  1543,  1547,  1548,  1549,
    1550,  1554,  1558,  1559,  1570,  1571,  1572,  1573,  1574,  1575,
    1576,  1578,  1580,  1582,  1584,  1586,  1588,  1590,  1592,  1594,
    1596,  1601,  1603,  1605,  1607,  1609,  1611,  1613,  1615,  1617,
    1619,  1621,  1623,  1625,  1632,  1638,  1644,  1650,  1659,  1669,
    1677,  1678,  1679,  1680,  1681,  1682,  1683,  1684,  1689,  1690,
    1694,  1698,  1699,  1703,  1707,  1708,  1712,  1716,  1720,  1727,
    1728,  1729,  1730,  1731,  1732,  1736,  1737,  1742,  1747,  1755,
    1756,  1757,  1758,  1759,  1760,  1761,  1762,  1763,  1765,  1767,
    1769,  1771,  1773,  1775,  1777,  1782,  1783,  1786,  1787,  1788,
    1791,  1792,  1793,  1794,  1805,  1806,  1810,  1811,  1812,  1816,
    1817,  1818,  1826,  1827,  1828,  1829,  1833,  1834,  1835,  1836,
    1837,  1838,  1839,  1840,  1841,  1842,  1846,  1854,  1855,  1859,
    1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,  1869,
    1870,  1871,  1872,  1873,  1874,  1875,  1876,  1877,  1878,  1879,
    1880,  1881,  1885,  1886,  1887,  1888,  1889,  1890,  1894,  1895,
    1896,  1897,  1901,  1902,  1903,  1904,  1909,  1910,  1911,  1912,
    1913,  1914,  1915
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

#define YYPACT_NINF -854

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-854)))

#define YYTABLE_NINF -504

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -854,    58,  3011,  -854,   -49,  -854,  -854,  -854,  -854,  -854,
    -854,  4386,     6,    96,  -854,  9357,  -854,     6,  9357,  -854,
      19,    96,  4386,  9357,  4386,   -33,  -854,  9357,  6517,  7810,
    9357,  7962,  9357,  -854,   -18,  -854,  -854,    96,  -854,    40,
    8495,  9357,  9357,  -854,  9357,  -854,  9357,  9357,   187,   130,
     961,   992,  -854,  -854,  8622,  7379,  9357,  8495,  9357,  9357,
     100,  -854,   134,  4386,  9357,  9484,  9484,    96,  -854,  -854,
    9357,  8622,  9357,  9357,  -854,  -854,  9357,  -854,  -854, 11389,
    9357,  9357,  -854,  9357,  -854,  -854,  3474,  6796,  8622,  -854,
    4234,  -854,  -854,   198,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,    96,  -854,    96,   126,   217,  -854,   185,  -854,  -854,
     298,   132,   525,  -854,  -854,  -854,   236,   265,   277,   285,
     290, 15107,  1927,   -59,   297,   304,  -854,  -854,  -854,  -854,
    -854,  -854,   215,  -854,  -854, 15107,   293,  4386,  -854,   305,
    -854,  9357,  9357,  9357,  9357,  9357,  8622,  8622,   176,  -854,
    -854,  -854,  -854,  -854,   324,   237,  -854,  -854,   301, 13532,
      96,   311,  -854,   136, 15107,   356,  7531, 15107,  -854,   212,
    -854,    96,   252,  -854,    96,   308,    53, 12967, 12890,  -854,
    -854,  -854, 13480, 12251,  7531,  4386,   309,    20,    84,    24,
    -854,  4386,  -854,  -854, 13115,   213,   324, 13115,   324,   213,
    -854,  7531,  8089,  -854,  -854,    96,  -854,   205, 15107,  9357,
    9357,  -854, 15107,   314, 13328,  -854, 13115,   324, 15107,   313,
    7531,  -854, 15107, 13749,  -854,  -854, 13835,  2463,  -854,  -854,
   13887,   358,   315,   324,   220, 13617, 13115, 13967,   147,  2107,
     213,   147,   213,   210,  -854,  -854,  3626,    -7,  -854,  9357,
    -854,   -14,    -6,  -854,    82, 14019,   -28,   461,  -854,  -854,
     417,   347,   322,  -854,  -854,  -854,  7531,    34,    40,    40,
      40,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  8216,  -854,  9357,  9357,  9357,  9357,  8774,
    8774,  9357,  9357,  9357,  9357,  9611,  9357,    86, 11389,  9357,
    9357,  9357,  9357,  9357,  9357,  9357,  9357,  9357,  9357,  9357,
    9357,  9357,  9357,  9357,  8216,  8216,  8216,  8216,  8216,  8216,
    8216,  8216,  8216,  8216,  8216,  8216,  8216,  8216,  8216,  8216,
    7531,  7531,  8774,  8774,  7252,  -854,  -854, 13669, 13801, 14099,
      56,  3778,  -854,  8774,    53,   334,  -854,  9357,  -854,  9357,
     376,  -854,   325,   359,  -854,  -854,   339,    96,   443,  8622,
    -854,  4538,  8774,  -854,  4690,  8774,   333,  -854,    53,  9738,
    9357,  -854,  4386,   455,  9357,   335,  -854,    97,  -854,  -854,
      20,  -854,   365,   337,  -854,  9865,   386,  9357,    40,  -854,
    -854,  -854,   340,  -854,  8622,  -854, 15107, 15107,  -854,    14,
    -854,  7531,   341,  -854,   493,  -854,   493,  -854,  9992,   388,
    -854,  -854,  9738,  9357,  -854,  -854,  -854,  -854,  -854,  8343,
    -854, 12109,  6948,  -854,  7100,  -854,  8774,  6058,  3322,   360,
    9357,  6364,  -854,  -854,    96,  8622,   363,   373,   302,   374,
      40,   222,   242,   244,   260,   375, 13399, 15184, 15184,   289,
    -854,   289,  -854,   289,  2170,  1483,  1661,  1254,  2359,  2359,
     324,   147,  -854,  -854,  -854,  2107, 12080,   289,  1751,  1751,
   15184,  1751,  1751,  1976,   147, 12080, 15236,  1976,   213,   213,
     147,   385,   389,   390,   393,   396,   399,   403,   404,   406,
     407,   413,   418,   420,   421,   422,   424,   378,   405,  -854,
     289,  -854,   289,    64,  -854,  -854,  -854,    98,  -854,    96,
   15146,   353, 10119,  8774, 10246,  8774,  9357,  8774, 11822,     6,
   14151,  -854,  -854, 15107, 14185,  7531,  -854,  7531,  -854,   347,
    9357,   106,  9357, 15107,    54, 13186,  9357, 15107,    31, 13038,
    7252,  -854,   426,   447,   431, 14237,   447,   432,   547, 14317,
    4386, 13257,  -854,   123,  -854,  -854,  -854,  -854,  -854,  -854,
    1030,   111,  -854,    48,  -854,   328,   433,    20,    84,  9357,
     -33,  9357,  9357,  9357,  9357,  9357,  5906,  -854,   306,  7683,
    -854, 15107,  -854,  -854,  -854, 15107,   434,    77,   435,  -854,
      38,  -854,  -854,   262,    96,   444,   446,  -854,  -854,  -854,
    -854,  -854,  -854,     8, 11956,  -854,  -854, 15107,  4386, 15107,
    -854, 14369,   450,   469,  1044,   445,   492,   105,  -854,   528,
    -854,  -854,  -854,  -854, 11905,  -854,   264,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  7252,  -854,     9,  8774,  8774,  9357,   573, 14403,
    9357,   575, 14535,   452,  1095,    53,    53,  -854,  -854,  -854,
    -854,   459, 13115,  -854, 12322,  4842,  -854,  4994,  -854, 12393,
    5146,  -854,    53,  5298,  -854,    53,   114,  -854,  9357,  -854,
    9357,  -854,  4386,  9357,  -854,  4386,   579,  -854,  -854,    96,
    1261,  -854,    20,   491,   542,  -854,  -854,  -854,    45,  -854,
    -854,   386,   462,    60,  -854,  -854,  -854,  5450,  8622,  -854,
    -854,  -854,    96,  -854,   496,   301,  -854,  -854,  -854,  5602,
     466,  5754,   471,  -854,  9357,  3930,   472,  1391,  -854,  9357,
      96,  -854,  -854,   328,  -854,  -854,  -854,    96,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  9357,   499,
    -854,   501,   495,   495,  -854,  -854,  -854,   386,   115,   121,
   14574, 10373, 10500, 14613, 10627, 10754, 10881, 11008,  -854,  -854,
    1464,  -854,  4386,  9357, 15107,  9357, 15107,  4386,  9357, 15107,
    -854,  9357, 15107,  -854,  7252,  -854, 14713, 15107,  -854, 15107,
     614,  4386,   491,  -854,   500,  8901,   119,  -854,   556,  -854,
    -854,  8774, 11730,  4386,  -854,    47,   503,  9357,  -854,  9357,
    -854, 15107,  4386,  9357,  -854, 15107,  4386, 15107,   266,  -854,
    4082,  -854,  6669,  1112,  1112,  -854,  1706,  -854, 15107,  -854,
      17,  -854, 12180,  2833,    97,  -854,  -854,  -854,  9357,  9357,
    9357,  9357,  9357,  9357,  9357,  9357,   506,  1552, 14317, 12464,
   12535, 14317, 12606, 12677,  -854,  9357,  4386,  -854,   386,   491,
    9357,   -33,  9357,  9357,  9357,  9357,  9357,  6211,  -854,  -854,
     137,  8622,  -854,  -854,  9357,    23,  2736,  -854,   531,   356,
    -854,   301, 15107, 12748,  -854, 12819,  -854,  -854,  -854,  -854,
    -854, 11135,   572,  -854,  -854,   495,   495,   203, 14752, 14791,
   14891, 14930, 14969, 15069,  -854,  -854,  4386,  4386,  4386,  4386,
   15107,  -854,  -854,   119,  9053,    61,  -854,  -854, 15107, 15107,
    -854,  -854,  -854, 11008,  -854,  4386,  4386,  9357,   -33,  9357,
    9357,  9357,  9357,  9357,  3166,  -854,   138,  -854,   556,  -854,
    -854,  -854,  9357,  9357,  9357,  9357,  9357,  9357, 14317, 14317,
   14317, 14317,  -854,  -854,  -854,  -854,  -854,   230,  8774, 11516,
     629, 14317, 14317,   511, 11262,    85,    71, 12038,  -854,  -854,
    -854,  -854,   468, 11611,  -854,  -854,  -854,  9205,  -854,   470,
    -854
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    83,   496,   497,   498,   499,
     500,     0,   438,    81,   193,     0,   162,   438,     0,   198,
     305,    81,     0,     0,     0,     0,   172,   475,   306,     0,
       0,   304,     0,   197,     0,   182,   194,     0,   178,     0,
       0,     0,     0,   429,     0,   185,     0,     0,     0,     0,
     304,   304,   196,   163,     0,     0,     0,     0,     0,     0,
       0,   199,     0,     0,     0,     0,     0,     0,   195,   164,
       0,     0,     0,     0,   548,   550,     0,   551,   552,   474,
       0,     0,   549,   546,    90,   547,     0,     0,     0,     4,
       0,     5,     9,    45,    10,    11,    12,    14,   391,    22,
      13,    91,    97,    15,    17,    16,    19,    20,    21,    18,
      96,     0,    94,     0,   464,     0,    98,   392,    95,    99,
       0,   480,   395,   460,   461,   397,   394,     0,     0,   465,
     466,     0,   396,     0,   481,   482,   483,   459,   399,   398,
     462,   463,     0,    38,    24,   405,     0,     0,   439,     0,
      82,     0,     0,     0,     0,     0,     0,     0,   464,   480,
     395,   394,   465,   466,   407,   396,   481,   482,     0,   438,
       0,     0,   136,     0,   376,     0,   383,   476,   183,   499,
     104,     0,     0,   184,     0,     0,     0,     0,     0,   305,
     306,   103,     0,     0,   383,     0,     0,     0,     0,     0,
     307,     0,    87,    32,     0,   534,   457,     0,   408,   535,
       7,   383,   306,    93,    92,   284,   373,     0,   372,     0,
       0,    88,   479,     0,     0,    35,     0,   410,   400,     0,
     383,    36,   406,     0,   143,   139,     0,   396,   143,   140,
       0,   296,     0,   409,     0,   372,     0,     0,   537,   473,
     533,   536,   532,     0,    47,    50,     0,     0,   378,     0,
     380,     0,     0,   379,     0,   372,     0,     0,     6,    46,
       0,   165,     0,   270,   269,   186,   383,     0,     0,     0,
       0,   229,   236,   237,   238,   233,   235,   231,   234,   232,
     230,   240,   239,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   472,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     383,   383,     0,     0,     0,    25,    26,     0,     0,     0,
       0,     0,    27,     0,     0,     0,    28,     0,    29,     0,
     391,   389,     0,   384,   385,   390,     0,     0,     0,     0,
     113,     0,     0,   112,     0,     0,     0,   119,     0,     0,
      56,   100,     0,   129,     0,     0,    30,   244,   179,   311,
       0,   312,   314,     0,   348,     0,   317,     0,     0,    86,
      31,    33,     0,   283,     0,    63,   477,   478,    89,     0,
      34,   383,     0,   150,   141,   137,   142,   138,     0,   294,
     291,    60,     0,    56,   106,    37,    49,    48,    51,     0,
     501,     0,     0,   492,     0,   494,     0,     0,     0,     0,
       0,     0,   505,     8,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,   437,   529,   528,   531,
     539,   538,   543,   542,   525,   522,   523,   524,   469,   470,
     468,   512,   491,   490,   489,   471,   516,   527,   521,   519,
     530,   520,   518,   510,   515,   517,   526,   509,   513,   514,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   541,
     540,   545,   544,   449,   450,   452,   454,     0,   441,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   503,   438,
     438,   109,   292,   377,     0,     0,   402,     0,   293,   165,
       0,     0,     0,   413,     0,     0,     0,   419,     0,     0,
       0,   120,   547,    59,     0,    52,    57,     0,   128,     0,
       0,     0,   401,   257,   254,   255,   256,   260,   261,   262,
     252,     0,   245,     0,   253,   271,     0,   315,     0,   344,
     341,   345,   347,   342,   343,   346,     0,   340,   460,     0,
     309,   458,   308,   488,   375,   374,     0,     0,     0,   403,
       0,   144,   298,   460,     0,     0,     0,   502,   467,   493,
     381,   495,   382,     0,     0,   504,   125,   423,     0,   507,
     506,     0,     0,   166,     0,     0,   176,     0,   173,   267,
     264,   265,   268,   187,     0,   393,     0,   300,   299,   301,
     303,    80,    64,    71,    72,    73,    68,    70,    78,    79,
      66,    69,    67,    65,    75,    74,    76,    77,   486,   487,
     451,   453,     0,   440,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,   387,   388,
     386,     0,     0,   127,     0,     0,   111,     0,   110,     0,
       0,   117,     0,     0,   115,     0,     0,   444,     0,   101,
       0,   102,     0,     0,   131,     0,   133,   258,   259,     0,
     252,   243,     0,   361,   272,   275,   274,   276,     0,   313,
     316,   317,     0,     0,   318,   319,   152,     0,     0,   151,
     154,   404,     0,   145,   148,     0,   295,    61,    62,     0,
       0,     0,     0,   126,     0,     0,     0,   304,   171,     0,
     174,   170,   266,   271,   226,   224,   205,   208,   206,   207,
     218,   209,   222,   214,   212,   225,   213,   211,   216,   221,
     223,   210,   215,   219,   220,   217,   227,   228,     0,   203,
     201,     0,   241,   241,   200,   302,   442,   317,   480,   480,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   107,
       0,   114,     0,     0,   412,     0,   411,     0,     0,   418,
     118,     0,   417,   116,     0,   443,    54,    53,   130,   428,
     132,     0,   361,   246,     0,     0,   317,   273,   289,   310,
     352,     0,   503,     0,   156,     0,     0,     0,   146,     0,
     123,   425,     0,     0,   121,   424,     0,   508,     0,    39,
       0,   160,   306,   304,   304,   158,   304,   168,   177,   175,
       0,   204,     0,     0,   244,   189,   190,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   304,   416,     0,
       0,   422,     0,     0,   445,     0,     0,   135,   317,   361,
     367,   364,   368,   370,   365,   366,   369,     0,   363,   371,
     460,   285,   249,   247,     0,     0,     0,   350,   460,   157,
     155,     0,   149,     0,   124,     0,   122,    41,    40,   169,
     337,     0,   277,   202,   203,   241,   241,     0,     0,     0,
       0,     0,     0,     0,   161,   159,     0,     0,     0,     0,
      55,   134,   248,   317,   353,     0,   286,   288,   287,   290,
     281,   282,   180,     0,   147,     0,     0,   333,   330,   334,
     336,   331,   332,   335,     0,   329,   460,   278,   289,   191,
     192,   242,     0,     0,     0,     0,     0,     0,   415,   414,
     421,   420,   251,   250,   355,   356,   358,   460,     0,   503,
     460,   427,   426,     0,   321,     0,     0,     0,   357,   359,
     326,   324,   460,   503,   279,   188,   280,   353,   325,   460,
     360
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -854,  -854,  -854,    -1,  -385,  2006,  -854,  -854,  -854,   366,
     -84,  -293,   248,   245,  -854,  -854,   348,   651,  2546,  -854,
      46,  -839,  -698,   -46,  -854,  -854,  -854,    99,  -854,  -854,
    -854,   437,  -854,   -56,  -854,  -854,  -854,  -854,  -854,   497,
     139,  -127,  -854,  -854,  -854,   -73,  1012,  -854,  -854,  -854,
    -854,  -854,  -854,  -854,    63,  -854,  -183,  -115,  -724,  -854,
    -182,   -23,   120,  -854,  -854,  -854,   -61,  -854,  -854,  -302,
     475,  -854,  -248,  -269,  -854,  -167,  -854,   671,  -854,  -245,
     310,  -854,  -379,  -697,  -751,  -854,  -498,  -410,  -745,  -853,
    -747,   -35,  -854,    12,  -854,  -163,  -854,   172,   626,   451,
    -345,  -854,  -854,  1284,  -854,     0,  -854,  -854,  -217,  -854,
    -538,  -854,  -854,  1322,  1581,    -2,   683,   171,   967,  -854,
    1800,  1968,  -854,  -854,  -854,  -854,  -854,  -854,  -854,  -338
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   255,    90,   558,    92,    93,   270,    94,
     256,   553,   557,   554,    95,    96,    97,   149,    98,   182,
     203,    99,   100,   101,   102,   103,   104,   673,   105,   106,
     107,   414,   601,   735,   108,   109,   597,   730,   110,   111,
     446,   747,   112,   113,   627,   628,   158,   196,   575,   115,
     116,   448,   753,   633,   117,   781,   782,   384,   865,   388,
     571,   572,   573,   574,   634,   275,   718,   968,  1005,   952,
     216,   947,   902,   905,   118,   242,   419,   119,   120,   199,
     200,   392,   393,   590,   965,   922,   396,   587,   985,   899,
     826,   257,   173,   261,   262,   362,   363,   364,   159,   160,
     123,   124,   125,   161,   127,   147,   148,   517,   378,   696,
     518,   519,   128,   162,   163,   222,   183,   365,   165,   133,
     166,   167,   136,   137,   266,   138,   139,   140,   141,   142
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     131,    89,   570,   355,   213,   214,   521,   293,   602,   145,
     394,   576,   697,   164,   366,   596,   169,   168,   920,   217,
     131,   174,   131,     5,   829,   177,   177,   187,   188,   192,
     193,   385,   451,   452,   453,   454,   244,     5,   204,   205,
     206,     5,   207,     5,   208,   209,   394,   439,   402,   857,
     588,     5,   218,   264,   224,   226,   227,   228,     3,   866,
     690,   232,   233,   236,   240,   397,   951,   412,   243,   245,
     246,   247,  -153,   603,   248,   888,   201,   249,   250,   251,
     721,   252,   201,   685,   218,   394,   265,   556,   131,     5,
     867,   986,  -327,   340,   143,   341,   441,   527,    55,     5,
    -252,   831,   988,   225,  -327,   660,   146,   450,   376,   170,
     432,  -153,   389,   449,  -338,  -327,   472,   429,   434,   176,
    -338,   563,   350,   436,   786,  -338,   527,   442,   186,   903,
     556,   376,   395,  -338,   194,   727,   999,   661,   564,   433,
     921,   565,   943,  -338,   430,   131,    55,   435,   398,   145,
     347,   348,   349,   232,   218,   265,   566,   951,   919,   567,
    -327,    48,  -338,   739,   707,  -153,   787,  -327,   395,   354,
     568,   404,   390,   351,   728,   298,   473,   507,   508,   857,
     437,   351,   376,   229,   437,   437,   197,   569,   351,  -338,
     732,   942,   197,   131,   210,  -338,   708,   351,   720,   131,
     712,   969,   970,   351,   351,   636,   437,   395,  -338,   437,
     177,   528,   521,  -328,  1004,   832,   989,   406,   407,   299,
     828,   351,   662,   300,   589,  -328,   223,  -338,   729,   750,
     404,   -85,   370,   373,   377,   710,  -328,   438,   814,  -448,
    1003,   298,  -362,  1001,   998,  -447,   983,   901,   598,  -252,
     400,   663,  1008,   401,   131,   428,   751,   431,   263,   683,
     357,  -362,  1010,   273,   711,  -362,   269,   815,  -448,  -434,
     305,    16,   410,   307,  -447,  -431,   884,   310,  -485,   358,
    -485,  -328,   211,   -85,   274,   299,   230,   342,  -328,   300,
    -362,   343,   424,   456,   457,   458,   459,   461,   463,   464,
     465,   466,   467,   470,   471,   -85,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   277,    53,   521,   570,   629,   710,  -485,   404,
    -485,   692,   695,   824,   541,  -354,   305,   276,   306,   307,
     510,   512,   520,   310,   404,    69,   398,  -339,   405,   131,
     317,   530,   714,   143,  -354,   533,   971,   534,  -354,   323,
     278,   426,   856,   421,   630,   637,   398,   218,   398,   543,
     545,   279,   547,   549,   625,   631,     5,   555,   555,  -430,
     559,  -339,   561,  -354,   398,   638,  -297,   639,   398,  -484,
     715,  -484,   632,   131,   280,   591,   740,   742,   460,   462,
     531,   716,   595,   640,  -339,  -297,   202,   785,   294,   143,
     623,  -339,   305,   235,   239,   307,   131,   917,   717,   310,
     555,   555,   907,   202,   551,   666,   295,   595,  -435,   343,
    -339,   234,   238,  -436,   614,   595,   617,   736,   619,   621,
    -433,   509,   511,   218,   610,   344,   612,  -432,   346,  -339,
     307,   351,   529,   122,   356,   359,  -339,   408,   420,  -339,
     369,   387,   122,   418,   455,   411,   164,   243,   443,   444,
     445,   544,   447,   122,   548,   122,   521,   532,   536,   570,
     900,   535,   538,   537,   540,   550,   560,   908,   562,   577,
     578,   589,   856,   593,   599,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   503,   504,   505,
     506,   600,   604,   624,   122,   618,   122,   122,   641,   783,
     543,   669,   547,   672,   559,   674,   617,   635,   642,   675,
     676,   658,   643,   644,   352,   613,   645,   122,   682,   646,
     684,   122,   647,  -320,   689,  -322,   648,   649,   520,   650,
     651,   723,   202,   202,   202,  -320,   652,  -322,   131,   383,
     659,   653,   260,   654,   655,   656,  -320,   657,  -322,   -58,
     202,   698,  -349,   202,   699,   701,   966,   164,   702,   208,
     227,   228,   232,   243,   265,   726,   719,   737,   731,   738,
     686,   688,   202,   404,   691,   694,   748,   749,   122,   987,
     745,   752,   791,   263,   794,   263,  -349,   796,   990,   800,
     821,  -320,   202,  -322,   825,   827,   131,   830,  -320,   837,
    -322,   842,   668,   851,   671,  -391,   846,   863,   121,  -349,
     281,   282,   283,   284,   285,   286,  -349,   121,   287,   288,
     289,   290,   291,   292,   987,   886,   122,   864,   121,  1002,
     121,   361,   122,   889,   904,  -349,   911,   934,  1009,   967,
     520,   850,   987,   461,   510,   790,  1000,   605,   793,   361,
    -351,   606,   171,   877,  -349,   416,   836,   859,   681,   367,
     925,  -349,   927,   804,  -349,   806,   361,   823,   809,   121,
     709,   812,   860,   835,  1006,   982,   816,   780,   817,   996,
     131,   819,   191,   131,  -351,   361,   679,   122,   592,   680,
     178,     0,   121,     0,     0,     0,   121,     0,     0,     0,
     202,   798,   799,     0,     0,   131,   218,  -351,   801,     0,
       0,     0,     0,     0,  -351,     0,     0,   841,   810,   845,
       0,   813,   847,   131,   202,     0,     0,   858,   926,     0,
       0,   361,     0,  -351,     0,     0,     0,     0,     0,     0,
     725,     0,     0,     0,   460,   509,     0,     0,     0,     0,
       0,     0,  -351,   121,     0,     0,   862,     0,     0,  -351,
       0,     0,  -351,     0,     0,     0,     0,     0,     0,   804,
     806,     0,   809,   812,   841,   845,     0,     0,     0,     0,
     878,   879,   122,   880,     0,   881,   882,   213,   214,   883,
       0,     0,   520,     0,     0,   361,   361,     0,     0,   131,
       0,   121,   122,   131,     0,   122,     0,   121,     0,   906,
     617,   131,     0,   122,     0,   912,     0,   913,     0,     0,
     131,   915,     0,     0,   131,     0,     0,     0,   131,   428,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   945,     0,     0,     0,   878,   928,   929,   881,
     930,   931,   932,   933,     0,     0,     0,     0,     0,   594,
       0,   910,   121,   940,   131,     0,   361,     0,   164,   122,
     208,   227,   228,   232,   243,   265,     0,     0,     0,   948,
       0,     0,   949,     0,   594,     0,     0,   260,     0,   260,
     202,   202,   594,     0,   202,   202,     0,     0,     0,   131,
       0,     0,     0,     0,     0,     0,     0,   706,     0,   995,
       0,     0,     0,     0,   978,   979,   980,   981,     0,     0,
       0,     0,   131,     0,     0,     0,     0,     0,     0,     0,
       0,   845,     0,   991,   992,   164,     0,   208,   227,   228,
     232,   243,   265,     0,     0,     0,   733,     0,     0,   132,
     978,   979,   980,   981,   991,   992,     0,   121,   132,     0,
       0,     0,     0,     0,   189,     0,   997,   617,     0,   132,
       0,   132,   131,     0,     0,    27,   212,   121,     0,     0,
     121,   617,     0,     0,    35,   845,     0,  -181,   121,     0,
     678,   122,   361,   -44,   114,   189,     0,     0,     0,     0,
       0,    45,     0,   114,     0,     0,    27,   212,  -181,   -44,
     132,     0,   237,   237,   114,    35,   114,     0,  -181,     0,
       0,   202,   202,     0,   -43,   746,     0,     0,   202,     0,
     383,     0,    45,   132,   563,   383,     0,   132,   202,  -181,
     -43,   202,     0,  -167,   121,     0,     0,  -167,  -167,   122,
       0,   564,     0,     0,   565,   114,  -167,     0,  -167,  -167,
       0,     0,  -167,     0,     0,     0,     0,  -167,     0,   566,
    -167,     0,   567,     0,   267,     0,     0,     0,   114,     0,
       0,   838,   114,   568,  -167,   296,  -167,     0,  -167,     0,
    -167,  -167,   297,  -167,   132,  -167,     0,  -167,     0,     0,
     569,     0,     0,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -167,   189,   122,  -167,   122,     0,
    -167,   122,     0,     0,   122,   664,    27,   212,     0,     0,
       0,     0,     0,   122,     0,    35,   122,     0,  -181,   114,
       0,     0,   132,     0,     0,     0,     0,   299,   132,     0,
       0,   300,    45,     0,     0,     0,     0,     0,   122,  -181,
       0,     0,     0,     0,     0,     0,   121,  -167,     0,     0,
     122,     0,   122,     0,     0,  -167,   122,     0,     0,   301,
       0,   202,     0,     0,     0,     0,     0,   114,     0,     0,
       0,     0,     0,   114,   302,     0,   303,   304,   305,     0,
     306,   307,   308,   132,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,     0,   321,
     322,   323,     0,     0,   121,   383,   383,     0,   383,   383,
     797,     0,     0,   122,     0,     0,     0,     0,   122,     0,
     784,     0,     0,     0,     0,     0,     0,     0,   114,     0,
       0,   950,   122,     0,     0,     0,     0,   954,     0,   383,
       0,   383,   298,     0,   122,   563,   126,     0,     0,     0,
       0,   788,   789,   122,     0,   126,     0,   122,     0,     0,
     898,   122,   564,     0,     0,   565,   126,     0,   126,     0,
       0,   121,     0,   121,     0,     0,   121,     0,   132,   121,
     566,     0,     0,   567,   129,    48,   299,     0,   121,     0,
     300,   121,     0,   129,   568,     0,     0,   122,   132,     0,
       0,   132,     0,     0,   129,     0,   129,   126,     0,   132,
       0,   569,     0,   121,     0,     0,     0,     0,     0,     0,
       0,     0,   950,   114,     0,   121,     0,   121,     0,     0,
     126,   121,     0,   302,   126,     0,   946,   305,     0,   306,
     307,     0,     0,   114,   310,   129,   114,   122,   122,   122,
     122,   317,     0,     0,   114,     0,     0,     0,   321,   322,
     323,     0,     0,     0,     0,   132,   122,   122,   129,     0,
      16,     0,   129,     0,    20,     0,     0,     0,     0,   984,
       0,     0,     0,    26,     0,    27,   852,     0,   121,    31,
       0,   126,     0,   121,    35,     0,     0,  -181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,     0,     0,
     114,    45,     0,     0,     0,    48,     0,   853,  -181,   121,
     854,     0,    53,     0,   984,   876,     0,     0,   121,   129,
       0,     0,   121,     0,     0,     0,   121,     0,     0,   126,
       0,    67,   984,  -167,    69,   126,     0,  -167,  -167,     0,
       0,     0,     0,     0,     0,     0,  -167,     0,  -167,  -167,
       0,     0,  -167,     0,     0,     0,     0,  -167,     0,     0,
    -167,   298,   121,     0,     0,     0,     0,   129,     0,     0,
       0,     0,     0,   129,  -167,     0,  -167,   132,  -167,     0,
    -167,  -167,     0,  -167,    84,  -167,     0,  -167,     0,     0,
     126,     0,   855,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -167,   299,     0,  -167,     0,   300,
    -167,     0,   121,   121,   121,   121,     0,     0,     0,     0,
       0,    16,   114,     0,     0,    20,     0,     0,   129,     0,
       0,   121,   121,   130,    26,   132,    27,   852,     0,     0,
      31,     0,   130,     0,     0,    35,     0,     0,  -181,     0,
       0,     0,     0,   130,     0,   130,   305,  -167,   306,   307,
       0,     0,    45,   310,     0,  -167,    48,     0,   853,  -181,
     317,   854,     0,    53,     0,     0,     0,   321,   322,   323,
     114,     0,     0,     0,     0,   126,     0,     0,     0,     0,
       0,     0,    67,     0,   130,    69,     0,     0,     0,     0,
       0,     0,   132,     0,   132,   126,     0,   132,   126,     0,
     132,     0,     0,     0,     0,     0,   126,   130,     0,   132,
       0,   130,   132,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,   129,   132,    84,   129,   114,     0,   114,
       0,     0,   114,   935,   129,   114,   132,     0,   132,     0,
       0,     0,   132,     0,   114,     0,     0,   114,     0,     0,
       0,     0,   126,     0,     0,    16,     0,     0,   130,    20,
       0,     0,     0,   299,     0,     0,     0,   300,    26,   114,
      27,   852,     0,     0,    31,     0,     0,     0,     0,    35,
       0,   114,  -181,   114,     0,     0,     0,   114,     0,     0,
     129,     0,     0,     0,     0,     0,    45,     0,     0,   132,
     267,     0,   853,  -181,   132,   854,   130,    53,     0,   298,
     302,     0,   130,   304,   305,     0,   306,   307,   132,     0,
       0,   310,     0,     0,     0,     0,    67,     0,   317,    69,
     132,     0,   134,     0,     0,   321,   322,   323,     0,   132,
       0,   134,     0,   132,   114,     0,     0,   132,     0,   114,
       0,     0,   134,   299,   134,     0,     0,   300,     0,     0,
       0,     0,     0,   114,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,   126,   114,     0,     0,     0,    84,
       0,     0,     0,   132,   114,     0,     0,     0,   114,     0,
       0,     0,   114,   134,     0,     0,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
       0,   310,   129,     0,     0,     0,   134,   316,   317,     0,
     134,     0,   320,     0,     0,   321,   322,   323,   114,     0,
       0,     0,   126,   132,   132,   132,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,   132,     0,     0,     0,     0,     0,     0,
       0,     0,   130,     0,     0,     0,     0,     0,     0,     0,
     129,     0,     0,     0,     0,     0,     0,   134,   114,   114,
     114,   114,   130,     0,     0,   130,     0,     0,     0,     0,
       0,     0,     0,   130,     0,     0,     0,   114,   114,   126,
     135,   126,     0,     0,   126,     0,     0,   126,     0,   135,
       0,     0,     0,     0,     0,     0,   126,     0,     0,   126,
     135,     0,   135,     0,     0,   134,     0,     0,     0,     0,
       0,   134,     0,     0,   298,     0,     0,   129,    91,   129,
       0,   126,   129,     0,     0,   129,     0,   144,     0,   130,
       0,     0,     0,   126,   129,   126,     0,   129,   172,   126,
     175,   135,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,     0,     0,   299,   129,
       0,     0,   300,     0,   135,     0,   134,     0,   135,     0,
       0,   129,     0,   129,     0,     0,     0,   129,     0,   231,
       0,     0,     0,     0,   338,   339,  -455,     0,     0,  -484,
       0,  -484,     0,     0,     0,     0,   126,     0,     0,     0,
       0,   126,    91,     0,     0,   302,   268,   303,   304,   305,
       0,   306,   307,     0,     0,   126,   310,     0,     0,     0,
       0,     0,     0,   317,     0,   135,     0,   126,     0,     0,
     321,   322,   323,     0,   129,     0,   126,     0,     0,   129,
     126,     0,     0,     0,   126,   298,     0,     0,     0,     0,
       0,   130,     0,   129,     0,     0,     0,     0,     0,     0,
       0,   134,     0,   345,     0,   129,     0,     0,     0,     0,
       0,     0,     0,   135,   129,     0,     0,     0,   129,   135,
     126,   134,   129,     0,   134,     0,     0,     0,     0,   299,
       0,     0,   134,   300,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   130,
       0,   386,     0,     0,     0,     0,     0,   399,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     126,   126,   126,   126,   135,     0,   302,     0,   303,   304,
     305,     0,   306,   307,     0,     0,     0,   310,   134,   126,
     126,     0,   299,   316,   317,     0,   300,     0,   320,     0,
       0,   321,   322,   323,     0,     0,     0,     0,   129,   129,
     129,   129,    91,     0,     0,     0,   130,     0,   130,     0,
       0,   130,     0,     0,   130,     0,     0,   129,   129,     0,
       0,     0,     0,   130,     0,     0,   130,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,     0,   314,   315,   316,   317,   130,   318,
       0,   320,     0,     0,   321,   322,   323,     0,     0,   135,
     130,     0,   130,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
       0,     0,   135,     0,     0,     0,     0,     0,     0,     0,
     135,     0,     0,     0,     0,     0,     0,    91,     0,     0,
     134,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   151,     0,     0,    14,    15,     0,   399,     0,     0,
     399,    19,     0,   130,     0,     0,     0,     0,   130,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,   130,     0,    36,     0,   135,    38,    39,     0,
      41,     0,    42,    43,   130,     0,     0,     0,   134,     0,
      46,     0,    47,   130,    49,     0,     0,   130,     0,    52,
       0,   130,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,   616,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,     0,     0,   134,     0,   134,     0,     0,
     134,     0,     0,   134,     0,     0,     0,    81,     0,     0,
       0,     0,   134,     0,     0,   134,     0,     0,     0,   156,
       0,    87,     0,   157,     0,     0,     0,   130,   130,   130,
     130,     0,     0,     0,     0,     0,     0,   134,   135,     0,
       0,     0,     0,     0,     0,     0,   130,   130,     0,   134,
       0,   134,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   150,
       0,     0,     0,     0,     0,     0,   704,   150,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,     0,   195,     0,   198,   135,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,     0,     0,     0,     0,   134,     0,     0,
     338,   339,     0,   241,     0,  -484,     0,  -484,     0,     0,
       0,   134,     0,     0,   743,     0,     0,     0,     0,     0,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
       0,     0,   134,     0,     0,     0,   134,     0,     0,     0,
     134,     0,     0,   135,     0,   135,     0,   271,   135,   272,
       0,   135,     0,     0,     0,     0,     0,     0,     0,     0,
     135,     0,     0,   135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,     0,     0,     0,
       0,   399,     0,   399,     0,   135,   399,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,   135,   818,   135,
       0,   820,     0,   135,     0,     0,   241,     0,     0,     0,
       0,     0,   360,     0,     0,     0,     0,   241,     0,     0,
     368,     0,     0,   834,     0,     0,   134,   134,   134,   134,
     360,     0,     0,   391,     0,   840,   296,   844,     0,     0,
       0,    91,     0,   297,     0,   134,   134,   360,     0,     0,
       0,   403,     0,     0,   298,     0,     0,     0,     0,     0,
     135,     0,     0,     0,     0,   135,   360,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   135,     0,     0,     0,     0,     0,     0,   299,     0,
     135,     0,   300,     0,   135,     0,     0,     0,   135,     0,
       0,     0,   360,   198,   198,   198,   198,   887,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     0,     0,   909,
     301,     0,     0,   754,     0,     0,     0,     0,   914,     0,
     755,     0,   916,   474,   135,   302,    91,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,     0,
     321,   322,   323,     0,     0,     0,   360,   360,     0,     0,
       0,   953,   941,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   135,   135,   135,     0,     0,
       0,     0,     0,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   135,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,     0,   281,   282,
     283,   284,   285,   286,   198,     0,   287,   288,   289,   290,
     291,   292,   756,   757,   758,   759,     0,   360,   760,     0,
       0,     0,   761,   762,   763,   764,   765,   766,   767,   768,
     769,   770,   771,     0,   772,     0,     0,   773,   774,   775,
     776,     0,   777,     0,     0,     0,     0,     0,     0,     0,
     622,     0,     0,   626,     0,     0,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -304,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,     0,    36,  -181,    37,    38,
      39,    40,    41,   -42,    42,    43,     0,    44,     0,     0,
       0,    45,    46,  -304,    47,    48,    49,    50,  -181,   -42,
      51,    52,    53,   360,  -304,    54,    55,     0,    56,    57,
      58,    59,    60,    61,    62,    63,     0,    64,     0,    65,
      66,    67,    68,     0,    69,    70,    71,  -304,     0,     0,
      72,     0,    73,     0,     0,    74,     0,     0,     0,   713,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,   734,    80,     0,    81,
     241,    82,    83,     0,    84,     0,     0,    85,     0,     0,
       0,    86,     0,    87,     0,    88,     0,   993,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   151,     0,
     779,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,   822,     0,    68,   391,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,   734,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,   626,     0,     0,     0,
       0,     0,    80,   861,    81,     0,    82,    83,   215,     0,
       0,     0,    85,     0,     0,     0,   156,     0,    87,     0,
     157,   994,     0,     4,     0,     5,     6,     7,     8,     9,
      10,     0,  -503,     0,    11,    12,    13,    14,    15,  -503,
       0,    16,    17,    18,    19,    20,  -304,    21,    22,    23,
    -503,    24,    25,  -503,    26,     0,    27,    28,    29,    30,
      31,    32,    33,     0,    34,    35,     0,    36,  -181,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -304,    47,     0,    49,    50,  -181,
     -42,    51,    52,    53,  -503,  -304,    54,    55,  -503,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,   924,
      65,    66,    67,    68,     0,    69,    70,    71,  -304,     0,
       0,    72,     0,    73,     0,     0,  -503,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -503,    76,  -503,  -503,  -503,  -503,  -503,  -503,  -503,
       0,  -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,  -503,
      81,  -503,  -503,  -503,     0,    84,  -503,  -503,  -503,     0,
       0,     0,    86,  -503,    87,   253,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -304,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -181,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -304,    47,    48,    49,
      50,  -181,   -42,    51,    52,    53,     0,  -304,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -304,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,   215,    84,     0,     0,
      85,     0,     0,     0,    86,   254,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -304,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -181,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -304,    47,
      48,    49,    50,  -181,   -42,    51,    52,    53,     0,  -304,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -304,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,   427,    87,   253,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -304,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -181,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -304,    47,    48,    49,    50,  -181,   -42,    51,    52,    53,
       0,  -304,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -304,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,   254,
      87,   848,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -304,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -181,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -304,    47,    48,    49,    50,  -181,   -42,    51,
      52,    53,     0,  -304,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -304,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,   849,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -304,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,    33,     0,    34,    35,     0,    36,  -181,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -304,    47,    48,    49,    50,  -181,
     -42,    51,    52,    53,     0,  -304,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -304,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,   918,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -304,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -181,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -304,    47,   267,    49,
      50,  -181,   -42,    51,    52,    53,     0,  -304,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -304,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -304,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -181,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -304,    47,
       0,    49,    50,  -181,   -42,    51,    52,    53,     0,  -304,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -304,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,     0,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -304,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   542,    33,     0,
      34,    35,     0,    36,  -181,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -304,    47,     0,    49,    50,  -181,   -42,    51,    52,    53,
       0,  -304,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -304,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -304,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,   546,
      33,     0,    34,    35,     0,    36,  -181,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -304,    47,     0,    49,    50,  -181,   -42,    51,
      52,    53,     0,  -304,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -304,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -304,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   803,    33,     0,    34,    35,     0,    36,  -181,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -304,    47,     0,    49,    50,  -181,
     -42,    51,    52,    53,     0,  -304,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -304,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,     0,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -304,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,   805,    33,     0,    34,    35,     0,    36,
    -181,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -304,    47,     0,    49,
      50,  -181,   -42,    51,    52,    53,     0,  -304,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -304,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -304,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,   808,    33,     0,    34,    35,
       0,    36,  -181,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -304,    47,
       0,    49,    50,  -181,   -42,    51,    52,    53,     0,  -304,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -304,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,     0,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -304,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   811,    33,     0,
      34,    35,     0,    36,  -181,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -304,    47,     0,    49,    50,  -181,   -42,    51,    52,    53,
       0,  -304,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -304,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -304,    21,    22,    23,     0,   833,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -181,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -304,    47,     0,    49,    50,  -181,   -42,    51,
      52,    53,     0,  -304,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -304,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -304,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   839,    33,     0,    34,    35,     0,    36,  -181,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -304,    47,     0,    49,    50,  -181,
     -42,    51,    52,    53,     0,  -304,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -304,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,     0,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -304,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,   843,    33,     0,    34,    35,     0,    36,
    -181,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -304,    47,     0,    49,
      50,  -181,   -42,    51,    52,    53,     0,  -304,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -304,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,   722,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   151,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
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
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     215,     0,     0,     0,    85,     0,     0,     0,   156,     0,
      87,     0,   157,   615,     5,     6,     7,     8,     9,    10,
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
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,   215,     0,     0,     0,    85,     0,     0,
       0,   156,     0,    87,     0,   157,   944,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   151,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,     0,     0,     0,
      85,     0,     0,     0,   156,     0,    87,     0,   157,   620,
       5,     6,     7,     8,   179,    10,   180,     0,     0,   151,
       0,     0,    14,    15,     0,     0,   -84,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,   152,   153,     0,   154,    33,     0,    34,
       0,     0,    36,  -475,     0,    38,    39,     0,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,    46,     0,
      47,     0,    49,     0,  -475,     0,     0,    52,   -84,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,   155,     0,     0,     0,     0,     0,   181,    68,     0,
     -84,    70,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
       0,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,    80,     0,    81,     0,    82,    83,     0,
       0,     0,     0,    85,     0,     0,     0,   156,     0,    87,
       0,   157,     5,     6,     7,     8,   179,    10,     0,     0,
       0,   151,     0,     0,    14,    15,     0,     0,   -84,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,  -475,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,  -475,     0,     0,    52,
     -84,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,     0,     0,     0,     0,     0,   181,
      68,     0,   -84,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   157,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,   219,   220,     0,    68,   258,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,   259,     0,     0,     0,     0,     0,
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
      61,    62,   155,     0,     0,     0,   219,   220,     0,    68,
     258,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     215,     0,     0,     0,    85,     0,     0,     0,   156,     0,
      87,   609,   157,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   151,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,   219,   220,
       0,    68,   258,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,   215,     0,     0,     0,    85,     0,     0,     0,
     156,     0,    87,   611,   157,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   151,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,   513,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,   152,   153,
       0,   154,    33,   514,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,   515,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,   516,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   151,    82,    83,    14,    15,     0,     0,    85,     0,
       0,    19,   156,     0,    87,     0,   157,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,     0,     0,     0,   219,   220,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     0,
       0,     0,     0,     0,     0,    80,     0,    81,     0,    82,
      83,     0,   221,     0,     0,    85,     0,     0,     0,   156,
       0,    87,     0,   157,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   151,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,   152,   153,     0,
     154,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,     0,   219,
     220,     0,    68,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,   215,     0,     0,     0,    85,     0,     0,
       0,   156,     0,    87,     0,   157,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   151,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,   724,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,   219,   220,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   151,    82,    83,    14,    15,     0,     0,    85,
       0,     0,    19,   156,     0,    87,     0,   157,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,   184,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,   185,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,     0,     0,     0,    85,     0,     0,     0,
     156,     0,    87,     0,   157,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   151,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,   189,     0,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,   190,   152,   153,
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
      81,   151,    82,    83,    14,    15,     0,     0,    85,     0,
       0,    19,   156,     0,    87,     0,   157,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,  -475,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,  -475,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   157,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,   219,   220,     0,    68,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,   215,     0,     0,     0,    85,
       0,     0,     0,   156,   607,    87,     0,   157,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   151,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,   201,    25,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
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
       0,    85,     0,     0,    19,    86,     0,    87,     0,   157,
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
       0,     0,   156,     0,    87,     0,   157,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   151,     0,     0,    14,
      15,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,   185,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   151,    82,    83,    14,   890,     0,     0,
      85,     0,     0,    19,   156,     0,    87,     0,   157,     0,
       0,   891,     0,     0,     0,     0,     0,   152,   153,     0,
     154,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,   892,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
     893,   894,    60,    61,    62,   895,     0,     0,     0,     0,
       0,     0,    68,     0,     0,   896,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,     0,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,    80,     0,    81,
       0,    82,    83,   215,     0,     0,     0,    85,     0,     0,
       0,   156,     0,    87,     0,   897,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   151,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,     0,     0,     0,     0,     0,     0,     0,    80,
       0,    81,     0,    82,    83,   215,     0,     0,     0,    85,
       0,     0,     0,   156,     0,    87,     0,   897,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   151,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,   152,   153,     0,   875,    33,     0,    34,     0,     0,
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
       0,     0,     0,     0,    79,     0,     0,     5,     6,     7,
       8,     9,    10,    80,     0,    81,   151,    82,    83,    14,
      15,     0,     0,    85,     0,     0,    19,   156,     0,    87,
       0,   157,     0,     0,    25,     0,     0,     0,     0,     0,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     5,     6,     7,     8,     9,    10,
      80,     0,    81,   151,    82,    83,    14,   468,     0,     0,
      85,     0,     0,    19,    86,     0,    87,     0,   157,     0,
       0,    25,     0,     0,     0,     0,     0,   152,   153,     0,
     154,    33,     0,    34,     0,     0,    36,     0,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,     0,     0,
       0,     0,    68,     0,     0,   469,     0,     0,     0,     0,
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
      14,   579,     0,     0,   552,     0,     0,    19,   156,     0,
      87,     0,   157,     0,     0,   580,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,   581,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   582,   583,    60,    61,    62,   584,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   585,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   151,    82,    83,    14,    15,     0,
       0,    85,     0,     0,    19,   156,     0,    87,     0,   586,
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
      81,   151,    82,    83,    14,    15,     0,     0,    85,     0,
       0,    19,   156,     0,    87,     0,   586,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   667,    33,
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
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   157,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   670,    33,     0,    34,     0,
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
     153,     0,   869,    33,     0,    34,     0,     0,    36,     0,
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
      25,     0,     0,     0,     0,     0,   152,   153,     0,   870,
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
       0,     0,     0,   152,   153,     0,   872,    33,     0,    34,
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
     152,   153,     0,   873,    33,     0,    34,     0,     0,    36,
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
     874,    33,     0,    34,     0,     0,    36,     0,     0,    38,
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
       0,     0,     0,     0,   152,   153,     0,   875,    33,     0,
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
      14,   957,     0,     0,    85,     0,     0,    19,   156,     0,
      87,     0,   157,     0,     0,   958,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,   959,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   960,   961,    60,    61,    62,   962,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   963,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     5,     6,     7,     8,     9,
      10,    80,     0,    81,   151,    82,    83,    14,    15,     0,
       0,    85,     0,     0,    19,   156,     0,    87,     0,   964,
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
      81,   151,    82,    83,    14,    15,     0,     0,    85,     0,
       0,    19,     0,     0,    87,     0,   964,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
      46,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,   155,     0,     0,     0,     0,     0,     0,
      68,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,     0,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   157,     0,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,     0,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,    46,     0,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     0,     0,     0,     5,     6,     7,     8,     9,    10,
       0,  -353,     0,   151,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,    76,     0,     0,     0,
    -353,    25,     0,     0,  -353,     0,     0,   152,   153,     0,
     154,    33,     0,    34,    81,     0,    36,     0,   215,    38,
      39,     0,    41,     0,    42,    43,   156,     0,    87,  -353,
     897,     0,    46,     0,    47,     0,    49,     0,     0,     0,
       0,    52,     0,     0,     0,     0,  -323,     0,     0,     0,
      58,    59,    60,    61,    62,   155,     0,     0,  -323,     0,
       0,     0,    68,     0,     0,    70,     0,     0,     0,  -323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   151,     0,     0,    14,    15,     0,     0,    81,
       0,     0,    19,     0,  -323,     0,     0,     0,     0,     0,
      25,  -323,     0,    87,     0,   964,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   151,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
      76,     0,    25,     0,     0,     0,     0,     0,   152,   153,
       0,   154,    33,     0,    34,     0,     0,    36,    81,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
     156,     0,    87,    46,   586,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     5,     0,
       0,     0,     0,    68,     0,   754,    70,     0,     0,     0,
      14,     0,   755,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,     0,     0,    33,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,     0,     0,     0,     0,     0,   296,     0,     0,     0,
       0,     0,     0,   297,    87,    52,   157,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,   282,   283,   284,   285,   286,     0,     0,   287,   288,
     289,   290,   291,   292,   756,   757,   758,   759,   299,     0,
     760,     0,   300,     0,   761,   762,   763,   764,   765,   766,
     767,   768,   769,   770,   771,     0,   772,     0,   296,   773,
     774,   775,   776,     0,   777,   297,   376,   778,     0,     0,
     301,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,     0,
     321,   322,   323,     0,     0,     0,     0,     0,   298,     0,
     299,   741,     0,     0,   300,     0,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   299,     0,     0,     0,   300,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,     0,     0,
     296,     0,     0,  1007,     0,     0,     0,   297,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,   298,     0,
     310,   311,   312,   301,   314,   315,   316,   317,     0,     0,
       0,   320,     0,     0,   321,   322,   323,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,     0,
       0,   296,   608,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   299,   321,   322,   323,   300,     0,     0,
       0,     0,   296,   923,     0,     0,   382,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,   301,   281,   282,   283,   284,
     285,   286,     0,     0,   287,   288,   289,   290,   291,   292,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   299,   321,   322,   323,   300,     0,
       0,   351,     0,   296,     0,     0,     0,   802,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,   301,   281,   282,   283,
     284,   285,   286,     0,     0,   287,   288,   289,   290,   291,
     292,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   299,   321,   322,   323,   300,
       0,     0,   351,     0,   296,     0,     0,     0,   807,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,   301,   281,   282,
     283,   284,   285,   286,     0,     0,   287,   288,   289,   290,
     291,   292,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   299,   321,   322,   323,
     300,     0,     0,   351,     0,   296,     0,     0,     0,   936,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   301,   281,
     282,   283,   284,   285,   286,     0,     0,   287,   288,   289,
     290,   291,   292,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,     0,     0,   351,     0,   296,     0,     0,     0,
     937,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,     0,     0,   301,
     281,   282,   283,   284,   285,   286,     0,     0,   287,   288,
     289,   290,   291,   292,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,   351,     0,   296,     0,     0,
       0,   938,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     301,   281,   282,   283,   284,   285,   286,     0,     0,   287,
     288,   289,   290,   291,   292,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,     0,     0,   351,     0,   296,     0,
       0,     0,   939,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,   301,   281,   282,   283,   284,   285,   286,     0,     0,
     287,   288,   289,   290,   291,   292,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,   351,     0,   296,
       0,     0,     0,   955,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   301,   281,   282,   283,   284,   285,   286,     0,
       0,   287,   288,   289,   290,   291,   292,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,   351,     0,
     296,     0,     0,     0,   956,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,   374,
       0,     0,     0,   301,   281,   282,   283,   284,   285,   286,
       0,   375,   287,   288,   289,   290,   291,   292,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,   351,
       0,     0,     0,     0,     0,     0,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
     376,     0,     0,     0,   301,   298,   371,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   372,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,     0,   321,   322,   323,     0,     0,   299,
     351,     0,     0,   300,     0,     0,     0,     0,   296,     0,
       0,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,   693,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,   351,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,   376,     0,
       0,     0,   301,   298,   201,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,     0,   321,   322,   323,     0,     0,   299,   351,     0,
       0,   300,     0,     0,     0,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   687,     0,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,   351,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,     0,     0,   351,     0,   296,     0,
       0,     0,   705,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,   351,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     296,     0,     0,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   379,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   380,   318,   319,
     320,     0,   296,   321,   322,   323,     0,     0,  -456,   297,
       0,     0,   299,     0,     0,     0,   300,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   353,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,   299,   306,   307,   308,   300,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   381,   321,   322,   323,   296,     0,     0,
       0,     0,   146,     0,   297,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     422,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,   423,     0,   321,   322,   323,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
       0,   321,   322,   323,     0,     0,   297,     0,     0,     0,
       0,     0,     0,   301,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,     0,   321,   322,   323,     0,     0,   297,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   298,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   413,   321,   322,   323,     0,   296,     0,     0,
       0,     0,     0,     0,   297,   301,     0,   299,     0,     0,
       0,   300,     0,     0,     0,   298,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   301,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   299,
     306,   307,   308,   300,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,   296,   415,   321,
     322,   323,     0,     0,   297,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
     417,   321,   322,   323,     0,     0,   297,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
     425,   321,   322,   323,     0,     0,   297,     0,     0,     0,
       0,     0,   440,   301,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,     0,   321,   322,   323,     0,     0,   297,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   298,
       0,     0,     0,     0,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,   296,   700,     0,
       0,   146,     0,     0,   297,   301,     0,   299,     0,     0,
       0,   300,     0,     0,     0,   298,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   301,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   299,
     306,   307,   308,   300,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,   296,   677,   321,
     322,   323,     0,     0,   297,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,   298,     0,     0,   703,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
       0,   321,   322,   323,     0,     0,   297,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   792,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
       0,   321,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   744,   301,     0,   299,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   301,   318,   319,
     320,     0,     0,   321,   322,   323,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,   296,     0,   321,   322,   323,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   795,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,   299,     0,     0,
       0,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,   301,
       0,   298,     0,     0,     0,     0,   299,     0,     0,     0,
     300,     0,     0,     0,   302,     0,   303,   304,   305,   868,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   301,   321,
     322,   323,     0,     0,     0,   299,     0,     0,     0,   300,
       0,     0,     0,   302,     0,   303,   304,   305,   871,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   301,   321,   322,
     323,     0,     0,   296,   885,     0,     0,     0,     0,     0,
     297,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,   298,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,     0,   321,   322,   323,
       0,     0,   296,     0,     0,     0,     0,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,   299,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,   298,
       0,     0,     0,     0,   299,     0,     0,     0,   300,     0,
       0,     0,   302,     0,   303,   304,   305,   972,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   301,   321,   322,   323,
       0,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,   302,     0,   303,   304,   305,   973,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   301,   321,   322,   323,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,   298,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
     296,     0,     0,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,     0,     0,     0,     0,     0,   974,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,   301,     0,   298,     0,     0,
       0,     0,   299,     0,     0,     0,   300,     0,     0,     0,
     302,     0,   303,   304,   305,   975,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   301,   321,   322,   323,     0,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   302,
       0,   303,   304,   305,   976,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   301,   321,   322,   323,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,   298,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,     0,
       0,     0,     0,     0,   977,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,   298,     0,     0,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   301,   298,   321,   322,   323,     0,     0,   665,     0,
       0,     0,   300,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     301,   321,   322,   323,     0,     0,   299,     0,     0,     0,
     300,     0,     0,     0,   298,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,   301,     0,
     321,   322,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,   303,   304,   305,   299,   306,
     307,   308,   300,   309,   310,   311,   312,     0,   314,   315,
     316,   317,     0,   318,   319,   320,     0,     0,   321,   322,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,     0,
     314,   315,   316,   317,     0,   318,     0,   320,     0,     0,
     321,   322,   323
};

static const yytype_int16 yycheck[] =
{
       2,     2,   387,   170,    50,    51,   344,   122,   418,    11,
       1,   390,   550,    15,   181,     1,    18,    17,     1,    54,
      22,    23,    24,     3,   721,    27,    28,    29,    30,    31,
      32,   194,   277,   278,   279,   280,    71,     3,    40,    41,
      42,     3,    44,     3,    46,    47,     1,   264,   211,   747,
     395,     3,    54,    88,    56,    57,    58,    59,     0,   783,
      29,    63,    64,    65,    66,    41,   905,   230,    70,    71,
      72,    73,    58,   418,    76,   822,    29,    79,    80,    81,
     578,    83,    29,    29,    86,     1,    88,   380,    90,     3,
     787,   944,    75,   152,   143,   154,   124,    41,    75,     3,
       3,    41,    41,    57,    87,    41,   100,    73,   100,    90,
     124,    97,    92,   276,   105,    98,    30,   124,   124,   152,
      75,    24,   157,    41,   662,    41,    41,   155,    29,   826,
     423,   100,   123,   124,   152,    58,   989,    73,    41,   153,
     123,    44,   889,    98,   151,   147,    75,   153,   124,   151,
     152,   153,   154,   155,   156,   157,    59,   996,   856,    62,
     143,    64,   153,   155,    41,   151,   664,   150,   123,   169,
      73,   124,   152,   150,    97,    28,    90,   340,   341,   877,
     124,   150,   100,    83,   124,   124,   152,    90,   150,   105,
     152,   888,   152,   195,     7,   150,    73,   150,   577,   201,
     152,   925,   926,   150,   150,   450,   124,   123,   124,   124,
     212,   155,   550,    75,   143,   155,   155,   219,   220,    72,
     718,   150,   124,    76,   105,    87,    55,   143,   151,   124,
     124,    19,   186,   187,   188,   124,    98,   155,   124,   124,
     155,    28,   105,   994,   989,   124,   943,   128,   411,   152,
     204,   153,  1003,   207,   256,   256,   151,   259,    87,   153,
     124,   124,  1007,    46,   153,   128,    68,   153,   153,   143,
     123,    19,   226,   126,   153,   143,   814,   130,   152,   143,
     154,   143,   152,    71,    67,    72,   152,    72,   150,    76,
     153,    76,   246,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    93,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    24,    71,   662,   710,    24,   124,   152,   124,
     154,   548,   549,   712,   369,   105,   123,   152,   125,   126,
     342,   343,   344,   130,   124,    93,   124,    41,   143,   351,
     137,   353,    24,   143,   124,   357,   153,   359,   128,   146,
      62,   151,   747,   143,    62,   143,   124,   369,   124,   371,
     372,    73,   374,   375,     1,    73,     3,   379,   380,   143,
     382,    75,   384,   153,   124,   143,   124,   143,   124,   152,
      62,   154,    90,   395,    96,   397,   613,   614,   299,   300,
     354,    73,   404,   143,    98,   143,    40,   143,   143,   143,
     445,   105,   123,    65,    66,   126,   418,   151,    90,   130,
     422,   423,   832,    57,   378,    72,   149,   429,   143,    76,
     124,    65,    66,   143,   436,   437,   438,   604,   440,   441,
     143,   342,   343,   445,   432,   152,   434,   143,   143,   143,
     126,   150,   353,     2,   143,    99,   150,   143,   143,   153,
     152,   152,    11,   105,   293,   152,   468,   469,     7,    52,
     123,   372,   150,    22,   375,    24,   814,   143,   153,   864,
     825,   105,   143,   124,    41,   152,    31,   832,   153,   124,
     153,   105,   877,   153,   153,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,    18,   124,   150,    63,   155,    65,    66,   143,   634,
     522,   523,   524,   525,   526,   527,   528,   153,   143,   529,
     530,   153,   143,   143,   168,   436,   143,    86,   540,   143,
     542,    90,   143,    75,   546,    75,   143,   143,   550,   143,
     143,   586,   186,   187,   188,    87,   143,    87,   560,   193,
     155,   143,    87,   143,   143,   143,    98,   143,    98,   143,
     204,   124,    41,   207,   143,   143,   921,   579,    31,   581,
     582,   583,   584,   585,   586,   151,   153,   143,   153,   143,
     544,   545,   226,   124,   548,   549,   151,   105,   147,   944,
     150,    73,    29,   432,    29,   434,    75,   155,   953,   150,
      31,   143,   246,   143,   123,    73,   618,   155,   150,   123,
     150,   155,   523,   151,   525,   126,   155,   126,     2,    98,
     105,   106,   107,   108,   109,   110,   105,    11,   113,   114,
     115,   116,   117,   118,   989,    31,   195,   152,    22,   994,
      24,   176,   201,   153,    98,   124,   153,   151,  1003,    87,
     662,   745,  1007,   665,   666,   667,   155,   422,   670,   194,
      41,   423,    21,   800,   143,   238,   732,   750,   539,   182,
     863,   150,   864,   685,   153,   687,   211,   710,   690,    63,
     570,   693,   753,   728,   996,   943,   698,   634,   700,   968,
     702,   703,    31,   705,    75,   230,   535,   256,   398,   537,
      27,    -1,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,
     354,   675,   676,    -1,    -1,   727,   728,    98,   682,    -1,
      -1,    -1,    -1,    -1,   105,    -1,    -1,   739,   692,   741,
      -1,   695,   744,   745,   378,    -1,    -1,   749,   863,    -1,
      -1,   276,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
     589,    -1,    -1,    -1,   665,   666,    -1,    -1,    -1,    -1,
      -1,    -1,   143,   147,    -1,    -1,   778,    -1,    -1,   150,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,   791,
     792,    -1,   794,   795,   796,   797,    -1,    -1,    -1,    -1,
     802,   803,   351,   805,    -1,   807,   808,   853,   854,   811,
      -1,    -1,   814,    -1,    -1,   340,   341,    -1,    -1,   821,
      -1,   195,   371,   825,    -1,   374,    -1,   201,    -1,   831,
     832,   833,    -1,   382,    -1,   837,    -1,   839,    -1,    -1,
     842,   843,    -1,    -1,   846,    -1,    -1,    -1,   850,   850,
     852,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   897,    -1,    -1,    -1,   868,   869,   870,   871,
     872,   873,   874,   875,    -1,    -1,    -1,    -1,    -1,   404,
      -1,   835,   256,   885,   886,    -1,   411,    -1,   890,   438,
     892,   893,   894,   895,   896,   897,    -1,    -1,    -1,   901,
      -1,    -1,   904,    -1,   429,    -1,    -1,   432,    -1,   434,
     544,   545,   437,    -1,   548,   549,    -1,    -1,    -1,   921,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   561,    -1,   964,
      -1,    -1,    -1,    -1,   936,   937,   938,   939,    -1,    -1,
      -1,    -1,   944,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   953,    -1,   955,   956,   957,    -1,   959,   960,   961,
     962,   963,   964,    -1,    -1,    -1,   600,    -1,    -1,     2,
     972,   973,   974,   975,   976,   977,    -1,   351,    11,    -1,
      -1,    -1,    -1,    -1,    23,    -1,   988,   989,    -1,    22,
      -1,    24,   994,    -1,    -1,    34,    35,   371,    -1,    -1,
     374,  1003,    -1,    -1,    43,  1007,    -1,    46,   382,    -1,
     535,   560,   537,    52,     2,    23,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    11,    -1,    -1,    34,    35,    67,    68,
      63,    -1,    65,    66,    22,    43,    24,    -1,    46,    -1,
      -1,   675,   676,    -1,    52,     1,    -1,    -1,   682,    -1,
     684,    -1,    60,    86,    24,   689,    -1,    90,   692,    67,
      68,   695,    -1,    19,   438,    -1,    -1,    23,    24,   618,
      -1,    41,    -1,    -1,    44,    63,    32,    -1,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    59,
      46,    -1,    62,    -1,    64,    -1,    -1,    -1,    86,    -1,
      -1,   735,    90,    73,    60,    10,    62,    -1,    64,    -1,
      66,    67,    17,    69,   147,    71,    -1,    73,    -1,    -1,
      90,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    23,   685,    93,   687,    -1,
      96,   690,    -1,    -1,   693,   519,    34,    35,    -1,    -1,
      -1,    -1,    -1,   702,    -1,    43,   705,    -1,    46,   147,
      -1,    -1,   195,    -1,    -1,    -1,    -1,    72,   201,    -1,
      -1,    76,    60,    -1,    -1,    -1,    -1,    -1,   727,    67,
      -1,    -1,    -1,    -1,    -1,    -1,   560,   143,    -1,    -1,
     739,    -1,   741,    -1,    -1,   151,   745,    -1,    -1,   104,
      -1,   835,    -1,    -1,    -1,    -1,    -1,   195,    -1,    -1,
      -1,    -1,    -1,   201,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,   256,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,   618,   879,   880,    -1,   882,   883,
     155,    -1,    -1,   802,    -1,    -1,    -1,    -1,   807,    -1,
     634,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,    -1,
      -1,   905,   821,    -1,    -1,    -1,    -1,   911,    -1,   913,
      -1,   915,    28,    -1,   833,    24,     2,    -1,    -1,    -1,
      -1,   665,   666,   842,    -1,    11,    -1,   846,    -1,    -1,
     825,   850,    41,    -1,    -1,    44,    22,    -1,    24,    -1,
      -1,   685,    -1,   687,    -1,    -1,   690,    -1,   351,   693,
      59,    -1,    -1,    62,     2,    64,    72,    -1,   702,    -1,
      76,   705,    -1,    11,    73,    -1,    -1,   886,   371,    -1,
      -1,   374,    -1,    -1,    22,    -1,    24,    63,    -1,   382,
      -1,    90,    -1,   727,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   996,   351,    -1,   739,    -1,   741,    -1,    -1,
      86,   745,    -1,   119,    90,    -1,   901,   123,    -1,   125,
     126,    -1,    -1,   371,   130,    63,   374,   936,   937,   938,
     939,   137,    -1,    -1,   382,    -1,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,    -1,   438,   955,   956,    86,    -1,
      19,    -1,    90,    -1,    23,    -1,    -1,    -1,    -1,   944,
      -1,    -1,    -1,    32,    -1,    34,    35,    -1,   802,    38,
      -1,   147,    -1,   807,    43,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   821,    -1,    -1,
     438,    60,    -1,    -1,    -1,    64,    -1,    66,    67,   833,
      69,    -1,    71,    -1,   989,     1,    -1,    -1,   842,   147,
      -1,    -1,   846,    -1,    -1,    -1,   850,    -1,    -1,   195,
      -1,    90,  1007,    19,    93,   201,    -1,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      46,    28,   886,    -1,    -1,    -1,    -1,   195,    -1,    -1,
      -1,    -1,    -1,   201,    60,    -1,    62,   560,    64,    -1,
      66,    67,    -1,    69,   143,    71,    -1,    73,    -1,    -1,
     256,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    72,    -1,    93,    -1,    76,
      96,    -1,   936,   937,   938,   939,    -1,    -1,    -1,    -1,
      -1,    19,   560,    -1,    -1,    23,    -1,    -1,   256,    -1,
      -1,   955,   956,     2,    32,   618,    34,    35,    -1,    -1,
      38,    -1,    11,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    22,    -1,    24,   123,   143,   125,   126,
      -1,    -1,    60,   130,    -1,   151,    64,    -1,    66,    67,
     137,    69,    -1,    71,    -1,    -1,    -1,   144,   145,   146,
     618,    -1,    -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    63,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   685,    -1,   687,   371,    -1,   690,   374,    -1,
     693,    -1,    -1,    -1,    -1,    -1,   382,    86,    -1,   702,
      -1,    90,   705,   351,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   371,   727,   143,   374,   685,    -1,   687,
      -1,    -1,   690,   151,   382,   693,   739,    -1,   741,    -1,
      -1,    -1,   745,    -1,   702,    -1,    -1,   705,    -1,    -1,
      -1,    -1,   438,    -1,    -1,    19,    -1,    -1,   147,    23,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    32,   727,
      34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,
      -1,   739,    46,   741,    -1,    -1,    -1,   745,    -1,    -1,
     438,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   802,
      64,    -1,    66,    67,   807,    69,   195,    71,    -1,    28,
     119,    -1,   201,   122,   123,    -1,   125,   126,   821,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    90,    -1,   137,    93,
     833,    -1,     2,    -1,    -1,   144,   145,   146,    -1,   842,
      -1,    11,    -1,   846,   802,    -1,    -1,   850,    -1,   807,
      -1,    -1,    22,    72,    24,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,   821,    -1,    -1,    -1,   256,    -1,    -1,
      -1,    -1,    -1,    -1,   560,   833,    -1,    -1,    -1,   143,
      -1,    -1,    -1,   886,   842,    -1,    -1,    -1,   846,    -1,
      -1,    -1,   850,    63,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,   127,    -1,
      -1,   130,   560,    -1,    -1,    -1,    86,   136,   137,    -1,
      90,    -1,   141,    -1,    -1,   144,   145,   146,   886,    -1,
      -1,    -1,   618,   936,   937,   938,   939,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   955,   956,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     618,    -1,    -1,    -1,    -1,    -1,    -1,   147,   936,   937,
     938,   939,   371,    -1,    -1,   374,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   382,    -1,    -1,    -1,   955,   956,   685,
       2,   687,    -1,    -1,   690,    -1,    -1,   693,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,   702,    -1,    -1,   705,
      22,    -1,    24,    -1,    -1,   195,    -1,    -1,    -1,    -1,
      -1,   201,    -1,    -1,    28,    -1,    -1,   685,     2,   687,
      -1,   727,   690,    -1,    -1,   693,    -1,    11,    -1,   438,
      -1,    -1,    -1,   739,   702,   741,    -1,   705,    22,   745,
      24,    63,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,    -1,    72,   727,
      -1,    -1,    76,    -1,    86,    -1,   256,    -1,    90,    -1,
      -1,   739,    -1,   741,    -1,    -1,    -1,   745,    -1,    63,
      -1,    -1,    -1,    -1,   147,   148,   149,    -1,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,   802,    -1,    -1,    -1,
      -1,   807,    86,    -1,    -1,   119,    90,   121,   122,   123,
      -1,   125,   126,    -1,    -1,   821,   130,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,   147,    -1,   833,    -1,    -1,
     144,   145,   146,    -1,   802,    -1,   842,    -1,    -1,   807,
     846,    -1,    -1,    -1,   850,    28,    -1,    -1,    -1,    -1,
      -1,   560,    -1,   821,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,   147,    -1,   833,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   842,    -1,    -1,    -1,   846,   201,
     886,   371,   850,    -1,   374,    -1,    -1,    -1,    -1,    72,
      -1,    -1,   382,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,   618,
      -1,   195,    -1,    -1,    -1,    -1,    -1,   201,   886,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     936,   937,   938,   939,   256,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,    -1,    -1,    -1,   130,   438,   955,
     956,    -1,    72,   136,   137,    -1,    76,    -1,   141,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,   936,   937,
     938,   939,   256,    -1,    -1,    -1,   685,    -1,   687,    -1,
      -1,   690,    -1,    -1,   693,    -1,    -1,   955,   956,    -1,
      -1,    -1,    -1,   702,    -1,    -1,   705,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,    -1,   134,   135,   136,   137,   727,   139,
      -1,   141,    -1,    -1,   144,   145,   146,    -1,    -1,   351,
     739,    -1,   741,    -1,    -1,    -1,   745,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,
      -1,    -1,   374,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     382,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,
     560,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,   371,    -1,    -1,
     374,    22,    -1,   802,    -1,    -1,    -1,    -1,   807,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,   821,    -1,    45,    -1,   438,    48,    49,    -1,
      51,    -1,    53,    54,   833,    -1,    -1,    -1,   618,    -1,
      61,    -1,    63,   842,    65,    -1,    -1,   846,    -1,    70,
      -1,   850,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,   438,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   886,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,   685,    -1,   687,    -1,    -1,
     690,    -1,    -1,   693,    -1,    -1,    -1,   138,    -1,    -1,
      -1,    -1,   702,    -1,    -1,   705,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,    -1,    -1,   936,   937,   938,
     939,    -1,    -1,    -1,    -1,    -1,    -1,   727,   560,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   955,   956,    -1,   739,
      -1,   741,    -1,    -1,    -1,   745,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,   560,    21,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,    37,    -1,    39,   618,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   802,    -1,    -1,    -1,    -1,   807,    -1,    -1,
     147,   148,    -1,    67,    -1,   152,    -1,   154,    -1,    -1,
      -1,   821,    -1,    -1,   618,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   833,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   842,    -1,    -1,    -1,   846,    -1,    -1,    -1,
     850,    -1,    -1,   685,    -1,   687,    -1,   111,   690,   113,
      -1,   693,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     702,    -1,    -1,   705,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   886,    -1,    -1,    -1,
      -1,   685,    -1,   687,    -1,   727,   690,    -1,    -1,   693,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   739,   702,   741,
      -1,   705,    -1,   745,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    -1,   181,    -1,    -1,
     184,    -1,    -1,   727,    -1,    -1,   936,   937,   938,   939,
     194,    -1,    -1,   197,    -1,   739,    10,   741,    -1,    -1,
      -1,   745,    -1,    17,    -1,   955,   956,   211,    -1,    -1,
      -1,   215,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
     802,    -1,    -1,    -1,    -1,   807,   230,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   821,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   833,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
     842,    -1,    76,    -1,   846,    -1,    -1,    -1,   850,    -1,
      -1,    -1,   276,   277,   278,   279,   280,   821,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,   833,
     104,    -1,    -1,    10,    -1,    -1,    -1,    -1,   842,    -1,
      17,    -1,   846,   307,   886,   119,   850,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,   340,   341,    -1,    -1,
      -1,   155,   886,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   936,   937,   938,   939,    -1,    -1,
      -1,    -1,    -1,   367,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   955,   956,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   390,    -1,   105,   106,
     107,   108,   109,   110,   398,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,   411,   125,    -1,
      -1,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,    -1,   141,    -1,    -1,   144,   145,   146,
     147,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     444,    -1,    -1,   447,    -1,    -1,   450,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,   537,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    86,    -1,    88,
      89,    90,    91,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   573,
      -1,    -1,    -1,   577,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   600,   136,    -1,   138,
     604,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
     634,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,   709,    -1,    91,   712,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   732,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,   750,    -1,    -1,    -1,
      -1,    -1,   136,   757,   138,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,   155,    -1,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    16,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    -1,    86,   863,
      88,    89,    90,    91,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    -1,
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
     136,    -1,   138,    -1,   140,   141,   142,   143,    -1,    -1,
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
      -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,   151,
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
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
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
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,    -1,   154,   155,     3,     4,     5,     6,     7,     8,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,   155,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,    -1,   154,   155,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,
      -1,    -1,    45,    46,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    67,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,
      83,    84,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    46,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
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
      84,    -1,    -1,    -1,    88,    89,    -1,    91,    92,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,   128,    -1,    -1,    -1,    -1,    -1,
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
      82,    83,    84,    -1,    -1,    -1,    88,    89,    -1,    91,
      92,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,
     142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,
     152,   153,   154,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    88,    89,
      -1,    91,    92,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,   153,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    41,    42,    -1,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    96,    -1,
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
      81,    82,    83,    84,    -1,    -1,    -1,    88,    89,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,   140,
     141,    -1,   143,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    -1,    88,
      89,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    55,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    88,    89,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    62,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    35,    36,    37,
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
      -1,    42,    -1,    -1,    45,    46,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    70,
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
      84,    -1,    -1,    -1,    88,    89,    -1,    91,    -1,    -1,
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
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,   150,   151,   152,    -1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    30,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
     138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,     3,     4,
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
      -1,    22,    -1,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,    -1,
     141,    15,    16,    -1,    -1,   146,    -1,    -1,    22,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,   105,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,   120,    -1,    -1,    -1,
     124,    30,    -1,    -1,   128,    -1,    -1,    36,    37,    -1,
      39,    40,    -1,    42,   138,    -1,    45,    -1,   142,    48,
      49,    -1,    51,    -1,    53,    54,   150,    -1,   152,   153,
     154,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      79,    80,    81,    82,    83,    84,    -1,    -1,    87,    -1,
      -1,    -1,    91,    -1,    -1,    94,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,   138,
      -1,    -1,    22,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      30,   150,    -1,   152,    -1,   154,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    39,    40,    -1,    42,    -1,    -1,    45,   138,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
     150,    -1,   152,    61,   154,    63,    -1,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,    80,    81,    82,    83,    84,    -1,     3,    -1,
      -1,    -1,    -1,    91,    -1,    10,    94,    -1,    -1,    -1,
      15,    -1,    17,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     138,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,   152,    70,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    82,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     105,   106,   107,   108,   109,   110,    -1,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    72,    -1,
     125,    -1,    76,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,    -1,   141,    -1,    10,   144,
     145,   146,   147,    -1,   149,    17,   100,   152,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      72,   155,    -1,    -1,    76,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    72,   144,   145,   146,    76,    -1,    -1,    -1,    -1,
      10,    -1,    -1,   155,    -1,    -1,    -1,    17,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    28,    -1,
     130,   131,   132,   104,   134,   135,   136,   137,    -1,    -1,
      -1,   141,    -1,    -1,   144,   145,   146,    -1,   119,    -1,
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
      -1,    -1,   144,   145,   146,    -1,    -1,    72,   150,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
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
      76,    -1,    -1,    -1,    28,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,   104,    -1,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,    -1,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     141,   144,   145,   146,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   147,   148,
     152,   154,    72,    76,   152,   164,   143,   274,   274,   274,
     250,   150,   168,    41,   264,   234,   143,   124,   143,    99,
     177,   229,   254,   255,   256,   276,   234,   198,   177,   152,
     179,    29,    41,   179,    29,    41,   100,   179,   267,    33,
      57,   143,    85,   168,   216,   254,   164,   152,   218,    92,
     152,   177,   240,   241,     1,   123,   245,    41,   124,   164,
     179,   179,   254,   177,   124,   143,   274,   274,   143,   150,
     179,   152,   254,   143,   190,   143,   190,   143,   105,   235,
     143,   143,    33,    57,   179,   143,   151,   151,   162,   124,
     151,   274,   124,   153,   124,   153,    41,   124,   155,   267,
     103,   124,   155,     7,    52,   123,   199,   150,   210,   254,
      73,   238,   238,   238,   238,   276,   274,   274,   274,   274,
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
      31,   274,   153,    24,    41,    44,    59,    62,    73,    90,
     163,   219,   220,   221,   222,   207,   241,   124,   153,    16,
      30,    61,    79,    80,    84,    94,   154,   246,   259,   105,
     242,   274,   239,   153,   229,   274,     1,   195,   254,   153,
      18,   191,   246,   259,   124,   172,   171,   151,   153,   153,
     252,   153,   252,   186,   274,   155,   164,   274,   155,   274,
     155,   274,   177,   250,   150,     1,   177,   203,   204,    24,
      62,    73,    90,   212,   223,   153,   238,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   153,   155,
      41,    73,   124,   153,   257,    72,    72,    39,   186,   274,
      39,   186,   274,   186,   274,   264,   264,   143,   229,   276,
     256,   199,   274,   153,   274,    29,   179,    29,   179,   274,
      29,   179,   267,    29,   179,   267,   268,   269,   124,   143,
      11,   143,    31,    31,   164,    85,   168,    41,    73,   221,
     124,   153,   152,   177,    24,    62,    73,    90,   225,   153,
     241,   245,     1,   250,    55,   276,   151,    58,    97,   151,
     196,   153,   152,   168,   177,   192,   234,   143,   143,   155,
     267,   155,   267,   164,   103,   150,     1,   200,   151,   105,
     124,   151,    73,   211,    10,    17,   119,   120,   121,   122,
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
     175,   176,   176,   177,   178,   178,   179,   179,   180,   180,
     181,   181,   181,   181,   181,   181,   181,   181,   182,   182,
     183,   183,   183,   183,   184,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   188,   189,   189,   189,
     189,   189,   189,   190,   190,   191,   191,   191,   192,   192,
     193,   194,   194,   195,   195,   196,   196,   196,   197,   197,
     197,   197,   198,   198,   198,   199,   199,   200,   200,   200,
     201,   201,   202,   203,   203,   203,   204,   204,   206,   207,
     205,   208,   208,   208,   208,   208,   210,   211,   209,   212,
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
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     0,     1,     1,     0,     1,     2,     1,     2,     3,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       3,     5,     5,     2,     2,     5,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     4,     3,
       6,     5,     6,     5,     8,     7,     2,     3,     3,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     6,     8,
       6,     8,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     5,     1,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     2,     2,     1,     0,     0,    10,     3,
       3,     5,     5,     1,     1,     1,     1,     1,     1,     1,
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
#line 5369 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5381 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5399 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5411 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5417 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5423 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5429 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5435 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5447 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5459 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5471 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5595 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5613 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 630 "chapel.ypp" /* yacc.c:1661  */
    { 
      (yyval.pblockstmt) = buildErrorStandin();
      USR_FATAL_CONT((yyval.pblockstmt), "illegal assignment to type");
    }
#line 5762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 5860 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5872 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 693 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5878 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 714 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 720 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 739 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 754 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 755 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 756 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 760 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 785 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 795 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 799 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 801 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 806 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6190 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 813 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 815 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 822 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 832 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 864 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 874 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 890 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 892 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 897 "chapel.ypp" /* yacc.c:1661  */
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
#line 6344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 913 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6352 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 929 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6411 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 963 "chapel.ypp" /* yacc.c:1661  */
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
#line 6440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 983 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1019 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
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
#line 6545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6553 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1071 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1083 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1099 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1101 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1104 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1106 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1115 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1119 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1123 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1124 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1128 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6813 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1129 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6819 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1130 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6825 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1131 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6831 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1133 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1134 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1138 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1139 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1143 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1147 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1148 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1149 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1154 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1158 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7017 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7023 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7029 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1197 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7035 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 7053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 7059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1202 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 7065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 7071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 7077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 7083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1232 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1233 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7119 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1238 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1245 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1247 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1252 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1281 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7190 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1298 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7226 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7238 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1314 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1323 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1340 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7289 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1348 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1350 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1355 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7331 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1382 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1384 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1390 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1396 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1400 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7411 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7417 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7423 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7429 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7435 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1413 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7447 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1414 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1415 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7459 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1416 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1417 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7471 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1423 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1424 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1425 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1428 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1432 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7553 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7565 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1469 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1475 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7591 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7597 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7603 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7615 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7621 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7627 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7633 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7639 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7645 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7657 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7663 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7669 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7675 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1517 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1533 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7795 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7801 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7807 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7813 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7819 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7825 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7831 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1639 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1645 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1651 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 8020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1699 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1708 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1713 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1717 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1721 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1727 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1728 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1729 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8098 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1738 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1743 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 470:
#line 1770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1778 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1782 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1783 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8194 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1787 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1788 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 486:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 487:
#line 1811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 488:
#line 1812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 489:
#line 1816 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1817 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1826 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1827 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1828 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1829 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1833 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 8278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1834 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1835 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1836 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1837 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1838 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1839 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1840 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1841 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1843 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8334 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1847 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1876 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1877 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1878 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8522 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1901 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1902 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 544:
#line 1903 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 545:
#line 1904 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 546:
#line 1909 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 547:
#line 1910 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 548:
#line 1911 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 549:
#line 1912 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 550:
#line 1913 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 551:
#line 1914 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 552:
#line 1915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8622 "bison-chapel.cpp" /* yacc.c:1661  */
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
