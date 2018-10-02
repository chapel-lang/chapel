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
#define YYLAST   15039

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  130
/* YYNRULES -- Number of rules.  */
#define YYNRULES  546
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1005

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
    1061,  1066,  1070,  1078,  1082,  1083,  1084,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,
    1111,  1112,  1113,  1117,  1118,  1119,  1120,  1121,  1122,  1123,
    1124,  1125,  1126,  1127,  1128,  1132,  1133,  1137,  1141,  1142,
    1143,  1147,  1149,  1151,  1153,  1155,  1160,  1161,  1165,  1166,
    1167,  1168,  1169,  1170,  1171,  1172,  1173,  1177,  1178,  1179,
    1180,  1181,  1182,  1186,  1187,  1191,  1192,  1193,  1194,  1195,
    1196,  1200,  1201,  1204,  1205,  1209,  1210,  1214,  1216,  1221,
    1222,  1226,  1227,  1231,  1232,  1236,  1238,  1240,  1245,  1258,
    1275,  1276,  1278,  1283,  1291,  1299,  1307,  1316,  1326,  1327,
    1328,  1332,  1333,  1341,  1343,  1348,  1350,  1352,  1357,  1359,
    1361,  1368,  1369,  1370,  1375,  1377,  1379,  1383,  1387,  1389,
    1393,  1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,
    1410,  1411,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1426,  1446,  1450,  1454,  1462,  1469,  1470,  1471,
    1475,  1477,  1483,  1485,  1487,  1492,  1493,  1494,  1495,  1496,
    1497,  1498,  1499,  1500,  1501,  1502,  1508,  1509,  1510,  1511,
    1515,  1516,  1520,  1521,  1522,  1526,  1527,  1531,  1532,  1536,
    1537,  1541,  1542,  1543,  1544,  1548,  1552,  1553,  1564,  1565,
    1566,  1567,  1568,  1569,  1570,  1572,  1574,  1576,  1578,  1580,
    1582,  1584,  1586,  1588,  1590,  1595,  1597,  1599,  1601,  1603,
    1605,  1607,  1609,  1611,  1613,  1615,  1617,  1619,  1626,  1632,
    1638,  1644,  1653,  1663,  1671,  1672,  1673,  1674,  1675,  1676,
    1677,  1678,  1683,  1684,  1688,  1692,  1693,  1697,  1701,  1702,
    1706,  1710,  1714,  1721,  1722,  1723,  1724,  1725,  1726,  1730,
    1731,  1736,  1741,  1749,  1750,  1751,  1752,  1753,  1754,  1755,
    1756,  1757,  1759,  1761,  1763,  1765,  1767,  1769,  1771,  1776,
    1777,  1780,  1781,  1782,  1785,  1786,  1787,  1788,  1799,  1800,
    1804,  1805,  1806,  1810,  1811,  1812,  1820,  1821,  1822,  1823,
    1827,  1828,  1829,  1830,  1831,  1832,  1833,  1834,  1835,  1836,
    1840,  1848,  1849,  1853,  1854,  1855,  1856,  1857,  1858,  1859,
    1860,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,  1869,
    1870,  1871,  1872,  1873,  1874,  1875,  1879,  1880,  1881,  1882,
    1883,  1884,  1888,  1889,  1890,  1891,  1895,  1896,  1897,  1898,
    1903,  1904,  1905,  1906,  1907,  1908,  1909
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

#define YYPACT_NINF -851

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-851)))

#define YYTABLE_NINF -498

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -851,   104,  3071,  -851,   -23,  -851,  -851,  -851,  -851,  -851,
    -851,  4446,   120,   251,  9115,  -851,   120,  9115,   194,   251,
    4446,  9115,  4446,   170,  -851,  9115,  6500,  7693,  9115,  7845,
    9115,   196,  -851,  -851,   251,  -851,    66,  8353,  9115,  9115,
    -851,  9115,  -851,  9115,  9115,   254,   201,    56,   436,  -851,
    8480,  7287,  9115,  8353,  9115,  9115,   276,   221,  4446,  9115,
    9242,  9242,   251,  -851,  9115,  8480,  9115,  9115,  -851,  -851,
    9115,  -851,  -851, 11020,  9115,  9115,  -851,  9115,  -851,  -851,
    3534,  6779,  8480,  -851,  4294,  -851,  -851,   316,  -851,  -851,
    -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,
    -851,  -851,  -851,  -851,  -851,   251,  -851,   251,   337,   171,
    -851,   240,  -851,  -851,   292,   256,  1918,  -851,  -851,  -851,
     265,   282,   253,   291,   304, 14854, 11015,    69,   305,   308,
    -851,  -851,  -851,  -851,  -851,  -851,    42,  -851,  -851, 14854,
     300,  4446,  -851,   313,  -851,  9115,  9115,  9115,  9115,  9115,
    8480,  8480,   112,  -851,  -851,  -851,  -851,  -851,   279,   333,
    -851,  -851,   324, 13279,   251,   320,  -851,   -28, 14854,   365,
    7439, 14854,  -851,   249,  -851,   251,   262,  -851,   251,   329,
      47, 12714, 12637,  -851,  -851,  -851, 13227, 11998,  7439,  4446,
     332,    13,    91,    -7,  -851,  4446,  -851,  -851, 12862,   200,
     279, 12862,   279,   200,  -851,  7439,  7972,  -851,  -851,   251,
    -851,   139, 14854,  9115,  9115,  -851, 14854,   347, 13075,  -851,
   12862,   279, 14854,   341,  7439,  -851, 14854, 13496,  -851,  -851,
   13582, 11230,  -851,  -851, 13634,   396,   367,   279,   232, 13364,
   12862, 13714,   255,  2361,   200,   255,   200,   128,  -851,  -851,
    3686,   -30,  -851,  9115,  -851,   -13,   106,  -851,   258, 13766,
     -24,   504,  -851,  -851,   472,   418,   397,  -851,  -851,  -851,
    7439,    37,    66,    66,    66,  -851,  -851,  -851,  -851,  -851,
    -851,  -851,  -851,  -851,  -851,  -851,  -851,  8099,  -851,  9115,
    9115,  9115,  9115,  8607,  8607,  9115,  9115,  9115,  9115,  9369,
    9115,    71, 11020,  9115,  9115,  9115,  9115,  9115,  9115,  9115,
    9115,  9115,  9115,  9115,  9115,  9115,  9115,  9115,  8099,  8099,
    8099,  8099,  8099,  8099,  8099,  8099,  8099,  8099,  8099,  8099,
    8099,  8099,  8099,  8099,  7439,  7439,  8607,  8607,  7160,  -851,
    -851, 13416, 13548, 13846,    44,  3838,  -851,  8607,    47,   403,
    -851,  9115,  -851,  9115,   443,  -851,   398,   425,  -851,  -851,
     410,   251,   513,  8480,  -851,  4598,  8607,  -851,  4750,  8607,
     404,  -851,    47,  9496,  9115,  -851,  4446,   527,  9115,   407,
    -851,   110,  -851,  -851,    13,  -851,   433,   412,  -851,  9623,
     461,  9115,    66,  -851,  -851,  -851,   414,  -851,  8480,  -851,
   14854, 14854,  -851,    28,  -851,  7439,   415,  -851,   563,  -851,
     563,  -851,  9750,   458,  -851,  -851,  9496,  9115,  -851,  -851,
    -851,  -851,  -851,  8226,  -851, 11859,  6906,  -851,  7033,  -851,
    8607,  6093,  3382,   430,  9115,  6347,  -851,  -851,   251,  8480,
     440,   526,   177,   438,    66,   283,   307,   311,   314,   449,
   13146,  2733,  2733,   400,  -851,   400,  -851,   400,  2018,    99,
    1507,  1640,  2539,  2539,   279,   255,  -851,  -851,  -851,  2361,
   12904,   400,  1780,  1780,  2733,  1780,  1780,   951,   255, 12904,
    1388,   951,   200,   200,   255,   450,   451,   452,   453,   455,
     456,   463,   466,   468,   469,   470,   477,   478,   480,   483,
     484,   476,   431,  -851,   400,  -851,   400,    57,  -851,  -851,
    -851,   116,  -851,   251, 14893,   370,  9877,  8607, 10004,  8607,
    9115,  8607, 11447,   120, 13898,  -851,  -851, 14854, 13932,  7439,
    -851,  7439,  -851,   418,  9115,   117,  9115, 14854,    52, 12933,
    9115, 14854,     3, 12785,  7160,  -851,   487,   507,   489, 13984,
     507,   491,   604, 14064,  4446, 13004,  -851,   162,  -851,  -851,
    -851,  -851,  -851,  -851,   560,   133,  -851,    67,  -851,   328,
     485,    13,    91,  9115,   170,  9115,  9115,  9115,  9115,  9115,
    5966,  -851,   378,  7566,  -851, 14854,  -851,  -851,  -851, 14854,
     488,    31,   490,  -851,    58,  -851,  -851,   315,   251,   493,
     501,  -851,  -851,  -851,  -851,  -851,  -851,     9,  2607,  -851,
    -851, 14854,  4446, 14854,  -851, 14116,   497,   525,  1188,   500,
     547,   -29,  -851,   580,  -851,  -851,  -851,  -851, 11501,  -851,
     325,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,
    -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,
    -851,  -851,  -851,  -851,  -851,  -851,  7160,  -851,    32,  8607,
    8607,  9115,   627, 14150,  9115,   632, 14282,   511, 11682,    47,
      47,  -851,  -851,  -851,  -851,   505, 12862,  -851, 12069,  4902,
    -851,  5054,  -851, 12140,  5206,  -851,    47,  5358,  -851,    47,
     217,  -851,  9115,  -851,  9115,  -851,  4446,  9115,  -851,  4446,
     636,  -851,  -851,   251,   601,  -851,    13,   545,   596,  -851,
    -851,  -851,    82,  -851,  -851,   461,   516,   118,  -851,  -851,
    -851,  5510,  8480,  -851,  -851,  -851,   251,  -851,   549,   324,
    -851,  -851,  -851,  5662,   518,  5814,   520,  -851,  9115,  3990,
     529,  1411,  -851,  9115,   251,  -851,  -851,   328,  -851,  -851,
    -851,   251,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,
    -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -851,
    -851,  -851,  9115,   555,  -851,   557,   536,   536,  -851,  -851,
    -851,   461,   233,   259, 14321, 10131, 10258, 14360, 10385, 10512,
   10639, 10766,  -851,  -851,  2037,  -851,  4446,  9115, 14854,  9115,
   14854,  4446,  9115, 14854,  -851,  9115, 14854,  -851,  7160,  -851,
   14460, 14854,  -851, 14854,   658,  4446,   545,  -851,   540,  8734,
       1,  -851,   598,  -851,  -851,  8607, 11355,  4446,  -851,    38,
     548,  9115,  -851,  9115,  -851, 14854,  4446,  9115,  -851, 14854,
    4446, 14854,   181,  -851,  4142,  -851,  6652,   432,   432,  -851,
    2237,  -851, 14854,  -851,    83,  -851, 11898, 11549,   110,  -851,
    -851,  -851,  9115,  9115,  9115,  9115,  9115,  9115,  9115,  9115,
     551,  1825, 14064, 12211, 12282, 14064, 12353, 12424,  -851,  9115,
    4446,  -851,   461,   545,  9115,   170,  9115,  9115,  9115,  9115,
    9115,  6220,  -851,  -851,    14,  8480,  -851,  -851,  9115,   -15,
   11720,  -851,   464,   365,  -851,   324, 14854, 12495,  -851, 12566,
    -851,  -851,  -851,  -851,  -851, 10893,   611,  -851,  -851,   536,
     536,   269, 14499, 14538, 14638, 14677, 14716, 14816,  -851,  -851,
    4446,  4446,  4446,  4446, 14854,  -851,  -851,     1,  8861,   122,
    -851,  -851, 14854, 14854,  -851,  -851,  -851, 10766,  -851,  4446,
    4446,  9115,   170,  9115,  9115,  9115,  9115,  9115,  3226,  -851,
     293,  -851,   598,  -851,  -851,  -851,  9115,  9115,  9115,  9115,
    9115,  9115, 14064, 14064, 14064, 14064,  -851,  -851,  -851,  -851,
    -851,   132,  8607, 11141,   465, 14064, 14064,   552,  2903,   125,
     115, 11759,  -851,  -851,  -851,  -851,   319, 11236,  -851,  -851,
    -851,  8988,  -851,   322,  -851
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    83,   490,   491,   492,   493,
     494,     0,   432,    81,     0,   162,   432,     0,   299,    81,
       0,     0,     0,     0,   172,   469,   300,     0,     0,   298,
       0,     0,   182,   193,     0,   178,     0,     0,     0,     0,
     423,     0,   185,     0,     0,     0,     0,   298,   298,   163,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   164,     0,     0,     0,     0,   542,   544,
       0,   545,   546,   468,     0,     0,   543,   540,    90,   541,
       0,     0,     0,     4,     0,     5,     9,    45,    10,    11,
      12,    14,   385,    22,    13,    91,    97,    15,    17,    16,
      19,    20,    21,    18,    96,     0,    94,     0,   458,     0,
      98,   386,    95,    99,     0,   474,   389,   454,   455,   391,
     388,     0,     0,   459,   460,     0,   390,     0,   475,   476,
     477,   453,   393,   392,   456,   457,     0,    38,    24,   399,
       0,     0,   433,     0,    82,     0,     0,     0,     0,     0,
       0,     0,   458,   474,   389,   388,   459,   460,   401,   390,
     475,   476,     0,   432,     0,     0,   136,     0,   370,     0,
     377,   470,   183,   493,   104,     0,     0,   184,     0,     0,
       0,     0,     0,   299,   300,   103,     0,     0,   377,     0,
       0,     0,     0,     0,   301,     0,    87,    32,     0,   528,
     451,     0,   402,   529,     7,   377,   300,    93,    92,   278,
     367,     0,   366,     0,     0,    88,   473,     0,     0,    35,
       0,   404,   394,     0,   377,    36,   400,     0,   143,   139,
       0,   390,   143,   140,     0,   290,     0,   403,     0,   366,
       0,     0,   531,   467,   527,   530,   526,     0,    47,    50,
       0,     0,   372,     0,   374,     0,     0,   373,     0,   366,
       0,     0,     6,    46,     0,   165,     0,   264,   263,   186,
     377,     0,     0,     0,     0,   223,   230,   231,   232,   227,
     229,   225,   228,   226,   224,   234,   233,     0,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   466,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,   377,     0,     0,     0,    25,
      26,     0,     0,     0,     0,     0,    27,     0,     0,     0,
      28,     0,    29,     0,   385,   383,     0,   378,   379,   384,
       0,     0,     0,     0,   113,     0,     0,   112,     0,     0,
       0,   119,     0,     0,    56,   100,     0,   129,     0,     0,
      30,   238,   179,   305,     0,   306,   308,     0,   342,     0,
     311,     0,     0,    86,    31,    33,     0,   277,     0,    63,
     471,   472,    89,     0,    34,   377,     0,   150,   141,   137,
     142,   138,     0,   288,   285,    60,     0,    56,   106,    37,
      49,    48,    51,     0,   495,     0,     0,   486,     0,   488,
       0,     0,     0,     0,     0,     0,   499,     8,     0,     0,
       0,     0,   257,     0,     0,     0,     0,     0,     0,     0,
     431,   523,   522,   525,   533,   532,   537,   536,   519,   516,
     517,   518,   463,   464,   462,   506,   485,   484,   483,   465,
     510,   521,   515,   513,   524,   514,   512,   504,   509,   511,
     520,   503,   507,   508,   505,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,   534,   539,   538,   443,   444,   446,
     448,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   432,   432,   109,   286,   371,     0,     0,
     396,     0,   287,   165,     0,     0,     0,   407,     0,     0,
       0,   413,     0,     0,     0,   120,   541,    59,     0,    52,
      57,     0,   128,     0,     0,     0,   395,   251,   248,   249,
     250,   254,   255,   256,   246,     0,   239,     0,   247,   265,
       0,   309,     0,   338,   335,   339,   341,   336,   337,   340,
       0,   334,   454,     0,   303,   452,   302,   482,   369,   368,
       0,     0,     0,   397,     0,   144,   292,   454,     0,     0,
       0,   496,   461,   487,   375,   489,   376,     0,     0,   498,
     125,   417,     0,   501,   500,     0,     0,   166,     0,     0,
     176,     0,   173,   261,   258,   259,   262,   187,     0,   387,
       0,   294,   293,   295,   297,    80,    64,    71,    72,    73,
      68,    70,    78,    79,    66,    69,    67,    65,    75,    74,
      76,    77,   480,   481,   445,   447,     0,   434,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,   381,   382,   380,     0,     0,   127,     0,     0,
     111,     0,   110,     0,     0,   117,     0,     0,   115,     0,
       0,   438,     0,   101,     0,   102,     0,     0,   131,     0,
     133,   252,   253,     0,   246,   237,     0,   355,   266,   269,
     268,   270,     0,   307,   310,   311,     0,     0,   312,   313,
     152,     0,     0,   151,   154,   398,     0,   145,   148,     0,
     289,    61,    62,     0,     0,     0,     0,   126,     0,     0,
       0,   298,   171,     0,   174,   170,   260,   265,   220,   218,
     199,   202,   200,   201,   212,   203,   216,   208,   206,   219,
     207,   205,   210,   215,   217,   204,   209,   213,   214,   211,
     221,   222,     0,   197,   195,     0,   235,   235,   194,   296,
     436,   311,   474,   474,     0,     0,     0,     0,     0,     0,
       0,     0,   108,   107,     0,   114,     0,     0,   406,     0,
     405,     0,     0,   412,   118,     0,   411,   116,     0,   437,
      54,    53,   130,   422,   132,     0,   355,   240,     0,     0,
     311,   267,   283,   304,   346,     0,   497,     0,   156,     0,
       0,     0,   146,     0,   123,   419,     0,     0,   121,   418,
       0,   502,     0,    39,     0,   160,   300,   298,   298,   158,
     298,   168,   177,   175,     0,   198,     0,     0,   238,   189,
     190,   440,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   410,     0,     0,   416,     0,     0,   439,     0,
       0,   135,   311,   355,   361,   358,   362,   364,   359,   360,
     363,     0,   357,   365,   454,   279,   243,   241,     0,     0,
       0,   344,   454,   157,   155,     0,   149,     0,   124,     0,
     122,    41,    40,   169,   331,     0,   271,   196,   197,   235,
     235,     0,     0,     0,     0,     0,     0,     0,   161,   159,
       0,     0,     0,     0,    55,   134,   242,   311,   347,     0,
     280,   282,   281,   284,   275,   276,   180,     0,   147,     0,
       0,   327,   324,   328,   330,   325,   326,   329,     0,   323,
     454,   272,   283,   191,   192,   236,     0,     0,     0,     0,
       0,     0,   409,   408,   415,   414,   245,   244,   349,   350,
     352,   454,     0,   497,   454,   421,   420,     0,   315,     0,
       0,     0,   351,   353,   320,   318,   454,   497,   273,   188,
     274,   347,   319,   454,   354
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -851,  -851,  -851,     2,  -364,  2154,  -851,  -851,  -851,   363,
     -39,  -190,   287,   296,  -851,  -851,   380,   690,  2612,  -851,
     945,  -812,  -664,   -46,  -851,  -851,  -851,   -19,  -851,  -851,
    -851,   481,  -851,   -12,  -851,  -851,  -851,  -851,  -851,   539,
     183,   -77,  -851,  -851,  -851,   -26,  1006,  -851,  -851,  -851,
    -851,  -851,  -851,  -851,    93,  -851,  -135,  -106,  -728,  -851,
    -134,    19,   163,  -851,  -851,  -851,   -21,  -851,  -851,  -262,
     174,  -851,  -208,  -232,  -851,  -153,  -851,   703,  -851,  -228,
     342,  -851,  -377,  -684,  -628,  -851,  -517,  -399,  -770,  -850,
    -723,   -44,  -851,   109,  -851,  -158,  -851,   207,   475,   626,
    -239,  -851,  -851,  1278,  -851,   -11,  -851,  -851,  -204,  -851,
    -530,  -851,  -851,  1519,  1578,    -2,   714,  1067,   999,  -851,
    1677,  1971,  -851,  -851,  -851,  -851,  -851,  -851,  -851,  -335
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   249,    84,   552,    86,    87,   264,    88,
     250,   547,   551,   548,    89,    90,    91,   143,    92,   176,
     197,    93,    94,    95,    96,    97,    98,   667,    99,   100,
     101,   408,   595,   729,   102,   103,   591,   724,   104,   105,
     440,   741,   106,   107,   621,   622,   152,   190,   569,   109,
     110,   442,   747,   627,   111,   775,   776,   378,   859,   382,
     565,   566,   567,   568,   628,   269,   712,   962,   999,   946,
     210,   941,   896,   899,   112,   236,   413,   113,   114,   193,
     194,   386,   387,   584,   959,   916,   390,   581,   979,   893,
     820,   251,   167,   255,   256,   356,   357,   358,   153,   154,
     117,   118,   119,   155,   121,   141,   142,   511,   372,   690,
     512,   513,   122,   156,   157,   216,   177,   359,   159,   127,
     160,   161,   130,   131,   260,   132,   133,   134,   135,   136
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     125,   207,   208,   515,    83,   162,   211,   570,   180,   139,
     287,   349,   158,   596,   691,   163,     5,   564,   125,   168,
     125,   238,   360,   171,   171,   181,   182,   186,   187,   590,
     379,   823,   684,   388,   391,   198,   199,   200,   258,   201,
       5,   202,   203,   445,   446,   447,   448,   396,   212,   860,
     218,   220,   221,   222,   433,   715,   226,   227,   230,   234,
      51,     5,   237,   239,   240,   241,   406,   195,   242,     5,
       5,   243,   244,   245,     5,   246,   195,   851,   212,   183,
     259,   679,   125,   388,   914,   521,  -153,   945,   980,   721,
      25,   206,   388,   882,   423,   744,   351,   861,   654,    32,
     435,   466,  -181,   370,     3,   383,   583,   344,   -44,   370,
     444,   426,   443,  -246,   336,   352,    42,   392,   337,  -356,
     137,   424,   745,  -181,   -44,  -153,   780,   292,   722,   895,
     655,   436,  -332,   993,   557,   345,   897,  -332,  -356,   125,
     427,   781,  -356,   139,   341,   342,   343,   226,   212,   259,
     582,   558,   348,   345,   559,   389,  -332,  -332,  -321,   825,
     937,   467,   398,   982,   733,   384,   521,  -356,   431,   560,
    -321,   293,   561,   597,    45,   294,   501,   502,   945,  -153,
    -332,  -321,   723,   562,   550,  -332,   913,   125,   345,   191,
      51,   963,   964,   125,   714,   822,  -332,   345,   936,   522,
     563,   623,   345,   701,   171,   389,   915,   851,   345,   515,
     726,   400,   401,   992,   389,  -332,   630,   267,   191,   706,
     140,   334,   299,   335,   300,   301,  -321,   550,   292,   304,
     428,  1004,  -332,  -321,  -332,   702,   311,  -348,   268,   624,
     656,   398,   431,   315,   316,   317,   431,   592,   125,   431,
     625,   425,   422,   977,     5,   254,  -348,   704,   998,   429,
    -348,   204,  -246,   398,  -479,   345,  -479,   626,   -85,   657,
     677,   137,   293,   826,   454,   456,   294,   983,   878,   420,
     997,    15,   399,   292,   164,  -348,   705,   450,   451,   452,
     453,   455,   457,   458,   459,   460,   461,   464,   465,   430,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   271,   503,   505,   535,
     -85,   515,   170,   299,   137,   300,   301,   293,   523,   818,
     304,   294,   911,    49,   504,   506,   514,   311,   686,   689,
     564,   808,   -85,   125,   355,   524,   317,   538,   188,   527,
     542,   528,   708,   205,   272,    63,   398,  -442,   370,   223,
     995,   212,   355,   537,   539,   273,   541,   543,  -322,  1002,
     809,   549,   549,   224,   553,   415,   555,   850,   299,   355,
    -322,   301,   431,  -441,   263,   304,  -442,   125,   274,   585,
     709,  -322,   270,   704,  -314,   617,   589,  -316,   355,  -425,
     196,   710,   289,   734,   736,   301,  -314,   392,  -424,  -316,
     125,   607,  -441,   432,   549,   549,   196,  -314,   711,  -333,
    -316,   589,   965,   228,   232,   288,   631,   901,   608,   589,
     611,   392,   613,   615,  -429,   392,  -322,   212,   392,  -291,
     229,   233,   660,  -322,   355,   730,   337,  -430,  -427,   392,
     632,  -426,   338,  -333,   633,   183,   340,   634,  -291,   183,
     158,   237,  -314,   350,   353,  -316,    25,   206,   779,  -314,
      25,   206,  -316,   515,   345,    32,  -333,   115,  -181,    32,
    -428,   363,  -181,  -333,   381,  -478,   115,  -478,   -43,  -479,
     402,  -479,    42,   405,   564,   115,    42,   115,   662,  -181,
     665,   412,  -333,  -181,   -43,  -343,  -345,   850,   355,   355,
     414,   437,   669,   670,   537,   663,   541,   666,   553,   668,
     611,  -333,   777,   299,   438,   346,   301,   619,  -333,     5,
     304,  -333,   676,   115,   678,   604,   717,   606,   683,  -343,
    -345,   439,   514,   196,   196,   196,   526,   441,   529,   531,
     377,   530,   125,   532,   534,   115,   544,   571,   554,   115,
     556,   196,  -343,  -345,   196,   572,   583,   587,   593,  -343,
    -345,   158,   588,   202,   221,   222,   226,   237,   259,   355,
     894,   594,   598,   196,   557,   612,   653,   902,  -343,  -345,
     618,   629,   635,   636,   637,   638,   639,   588,   640,   641,
     254,   558,   254,   196,   559,   588,   642,  -343,  -345,   643,
     125,   644,   645,   646,  -343,  -345,   115,  -343,  -345,   560,
     647,   648,   561,   649,   261,   557,   650,   651,   116,   652,
     -58,   692,   693,   562,   695,   696,   731,   116,   713,   720,
     454,   503,   558,   725,   732,   559,   116,   739,   116,   398,
     563,   742,   743,   746,   514,   794,   785,   455,   504,   784,
     560,   788,   787,   561,   115,    45,   790,   815,   819,   821,
     115,   824,   831,   836,   562,   840,   960,   798,   829,   800,
     845,  -385,   803,   857,   116,   806,   116,   116,   858,   880,
     810,   563,   811,   883,   125,   813,   898,   125,   961,   981,
     844,   905,   928,   672,   600,   355,   116,   994,   984,   165,
     116,   196,   599,   410,   830,   361,   675,   871,   853,   125,
     212,   774,   919,   817,   921,   115,   854,   703,  1000,   976,
     990,   835,   185,   839,   586,   196,   841,   125,   674,   172,
       0,   852,     0,     0,   981,     0,     0,     0,     0,   996,
       0,   920,     0,     0,     0,     0,     0,     0,  1003,     0,
       0,     0,   981,     0,     0,     0,     0,   116,     0,     0,
     856,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   798,   800,     0,   803,   806,   835,   839,
       0,     0,     0,     0,   872,   873,     0,   874,     0,   875,
     876,   207,   208,   877,     0,     0,   514,     0,     0,     0,
       0,     0,     0,   125,     0,   116,     0,   125,     0,     0,
     115,   116,     0,   900,   611,   125,     0,     0,     0,   906,
       0,   907,     0,     0,   125,   909,     0,     0,   125,     0,
     115,     0,   125,   115,   171,     0,   422,   939,     0,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
     872,   922,   923,   875,   924,   925,   926,   927,     0,     0,
       0,     0,     0,     0,     0,     0,   116,   934,   125,     0,
       0,     0,   158,     0,   202,   221,   222,   226,   237,   259,
       0,     0,     0,   942,     0,     0,   943,     0,     0,     0,
       0,   196,   196,     0,     0,   196,   196,   115,     0,     0,
       0,     0,     0,   125,   989,     0,     0,     0,   700,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   972,   973,
     974,   975,     0,     0,     0,     0,   125,     0,     0,     0,
       0,     0,     0,     0,     0,   839,     0,   985,   986,   158,
       0,   202,   221,   222,   226,   237,   259,   727,     0,     0,
       0,     0,     0,     0,   972,   973,   974,   975,   985,   986,
       0,   116,     0,     0,     0,     0,     0,     0,     0,   292,
     991,   611,     0,     0,     0,     0,   125,     0,   658,     0,
       0,   116,     0,   892,   116,   611,     0,     0,   219,   839,
       0,   126,   116,     0,     0,     0,     0,     0,   108,     0,
     126,     0,     0,     0,     0,     0,     0,   108,     0,   126,
       0,   126,     0,   293,     0,     0,   108,   294,   108,   115,
       0,     0,   196,   196,     0,     0,     0,     0,     0,   196,
       0,   377,     0,     0,     0,     0,   377,     0,     0,   196,
       0,     0,   196,     0,     0,     0,     0,   126,   116,   231,
     231,     0,     0,     0,   108,     0,     0,     0,     0,   940,
     296,     0,   297,   298,   299,     0,   300,   301,     0,   126,
       0,   304,     0,   126,     0,     0,   108,   115,   311,     0,
     108,     0,   832,     0,     0,   315,   316,   317,     0,     0,
       0,     0,     0,   778,     0,     0,     0,     0,     0,     0,
       0,     0,   978,     0,     0,     0,     0,     0,   217,     0,
       0,     0,     0,     0,     0,   364,   367,   371,     0,     0,
       0,     0,     0,     0,   782,   783,     0,     0,     0,     0,
     126,     0,     0,   394,     0,     0,   395,   108,   257,     0,
       0,     0,     0,     0,   115,     0,   115,   978,     0,   115,
       0,     0,   115,     0,     0,   404,     0,     0,     0,     0,
       0,   115,     0,     0,   115,   978,     0,     0,     0,     0,
     116,     0,     0,     0,     0,   418,     0,     0,   126,   740,
       0,     0,   196,     0,   126,   108,   115,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,  -167,   115,     0,
     115,  -167,  -167,     0,   115,     0,     0,     0,     0,     0,
    -167,     0,  -167,  -167,     0,     0,  -167,     0,     0,     0,
       0,  -167,     0,     0,  -167,     0,   377,   377,   116,   377,
     377,     0,     0,     0,     0,     0,     0,     0,  -167,   126,
    -167,     0,  -167,     0,  -167,  -167,   108,  -167,     0,  -167,
       0,  -167,   944,     0,     0,     0,     0,     0,   948,     0,
     377,   115,   377,     0,     0,     0,   115,     0,  -167,     0,
     120,  -167,     0,     0,  -167,     0,     0,     0,     0,   120,
     115,     0,     0,   525,     0,     0,     0,     0,   120,     0,
     120,     0,   115,     0,     0,   116,     0,   116,     0,     0,
     116,   115,     0,   116,     0,   115,     0,   545,     0,   115,
       0,     0,   116,     0,     0,   116,     0,     0,     0,     0,
       0,  -167,     0,     0,     0,     0,   120,     0,     0,  -167,
       0,     0,     0,     0,   126,     0,     0,   116,     0,     0,
       0,   108,     0,   944,   449,   115,     0,     0,   120,   116,
       0,   116,   120,     0,   126,   116,     0,   126,     0,     0,
       0,   108,     0,     0,   108,   126,     0,     0,     0,     0,
       0,     0,   108,     0,     0,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,     0,     0,     0,     0,   115,   115,   115,   115,     0,
       0,     0,     0,     0,     0,     0,   292,     0,     0,   120,
       0,     0,   116,     0,   115,   115,     0,   116,     0,     0,
      15,   126,     0,     0,    18,     0,     0,     0,   108,     0,
       0,   116,     0,    24,     0,    25,   846,     0,     0,    29,
       0,     0,     0,   116,    32,     0,     0,  -181,     0,     0,
     293,     0,   116,     0,   294,     0,   116,   120,     0,     0,
     116,    42,     0,   120,     0,    45,     0,   847,  -181,     0,
     848,     0,    49,   680,   682,     0,     0,   685,   688,     0,
       0,     0,   295,   257,     0,   257,     0,     0,     0,     0,
       0,    62,     0,     0,    63,     0,   116,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   123,   308,   309,   310,   311,     0,   312,   120,   314,
     123,     0,   315,   316,   317,   292,     0,     0,     0,   123,
       0,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   126,    78,     0,   116,   116,   116,   116,
     108,     0,   849,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   116,   123,     0,   293,
     124,     0,     0,   294,     0,     0,     0,     0,     0,   124,
       0,     0,     0,     0,     0,     0,   673,     0,   124,   123,
     124,     0,     0,   123,     0,     0,     0,     0,     0,     0,
       0,   126,     0,     0,   792,   793,     0,     0,   108,     0,
       0,   795,     0,   120,     0,     0,   296,     0,     0,   298,
     299,   804,   300,   301,   807,     0,   124,   304,     0,     0,
       0,     0,     0,   120,   311,     0,   120,     0,     0,     0,
     719,   315,   316,   317,   120,     0,     0,     0,   124,     0,
     123,     0,   124,     0,     0,     0,     0,     0,   292,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   126,   128,
     126,     0,     0,   126,     0,   108,   126,   108,   128,     0,
     108,     0,     0,   108,     0,   126,     0,   128,   126,   128,
       0,     0,   108,     0,     0,   108,     0,     0,   123,     0,
     120,     0,   293,     0,   123,     0,   294,     0,     0,   124,
     126,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,   126,     0,   126,   128,     0,     0,   126,   108,
       0,   108,     0,     0,     0,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   128,     0,   296,
       0,   128,     0,   299,     0,   300,   301,   124,     0,   123,
     304,     0,     0,   124,   904,     0,     0,   311,     0,     0,
       0,     0,     0,     0,   315,   316,   317,     0,     0,     0,
       0,     0,     0,     0,     0,   126,     0,     0,     0,     0,
     126,     0,   108,     0,     0,     0,     0,   108,   292,     0,
       0,     0,     0,     0,   126,     0,     0,     0,   128,     0,
       0,   108,     0,     0,     0,     0,   126,     0,   124,     0,
       0,     0,   120,   108,     0,   126,     0,     0,     0,   126,
       0,     0,   108,   126,    15,     0,   108,     0,    18,     0,
     108,     0,   293,     0,     0,     0,   294,    24,     0,    25,
     846,     0,     0,    29,   123,     0,   128,     0,    32,     0,
       0,  -181,   128,     0,     0,     0,     0,     0,     0,   126,
       0,     0,     0,     0,   123,    42,   108,   123,     0,    45,
     120,   847,  -181,     0,   848,   123,    49,     0,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,     0,
     304,     0,     0,     0,     0,    62,   310,   311,    63,     0,
       0,   314,     0,   124,   315,   316,   317,   128,     0,   126,
     126,   126,   126,     0,     0,     0,   108,   108,   108,   108,
       0,     0,     0,   124,     0,     0,   124,     0,   126,   126,
       0,   123,     0,     0,   124,   108,   108,   120,     0,   120,
       0,     0,   120,     0,     0,   120,     0,     0,    78,     0,
       0,     0,     0,   129,   120,     0,   929,   120,     0,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,   129,     0,   129,     0,     0,     0,     0,     0,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     124,   120,     0,   120,     0,     0,     0,   120,     0,     0,
       0,     0,   128,   275,   276,   277,   278,   279,   280,   129,
       0,   281,   282,   283,   284,   285,   286,     0,   870,     0,
       0,     0,   128,     0,     0,   128,   292,     0,     0,     0,
       0,   129,     0,   128,     0,   129,  -167,     0,     0,     0,
    -167,  -167,     0,     0,     0,     0,     0,     0,     0,  -167,
       0,  -167,  -167,   123,   120,  -167,     0,     0,     0,   120,
    -167,     0,     0,  -167,     0,     0,     0,     0,     0,     0,
     293,     0,     0,   120,   294,     0,     0,  -167,     0,  -167,
       0,  -167,     0,  -167,  -167,   120,  -167,     0,  -167,   128,
    -167,     0,   129,     0,   120,     0,     0,     0,   120,     0,
       0,     0,   120,     0,     0,     0,     0,  -167,     0,     0,
    -167,   123,   124,  -167,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,     0,   308,   309,   310,   311,    85,   312,   120,   314,
     129,     0,   315,   316,   317,   138,   129,     0,     0,     0,
       0,     0,     0,     0,   166,     0,   169,     0,     0,     0,
    -167,     0,     0,     0,     0,     0,     0,     0,  -167,     0,
     124,     0,     0,     0,     0,     0,     0,     0,   123,     0,
     123,     0,     0,   123,     0,     0,   123,     0,   120,   120,
     120,   120,   225,     0,     0,   123,     0,     0,   123,     0,
       0,   129,     0,     0,     0,     0,     0,   120,   120,     0,
       0,   128,     0,     0,    85,     0,     0,     0,   262,     0,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   123,     0,   123,     0,    15,   124,   123,   124,
      18,     0,   124,     0,     0,   124,     0,     0,     0,    24,
       0,    25,   846,     0,   124,    29,     0,   124,     0,     0,
      32,     0,     0,  -181,     0,     0,     0,     0,     0,   128,
       0,     0,     0,     0,     0,   339,     0,    42,     0,   124,
       0,   261,     0,   847,  -181,     0,   848,     0,    49,     0,
       0,   124,     0,   124,     0,   123,   129,   124,     0,     0,
     123,     0,     0,     0,     0,     0,     0,    62,     0,     0,
      63,     0,     0,     0,   123,     0,   129,     0,     0,   129,
       0,     0,     0,   380,     0,     0,   123,   129,     0,   393,
       0,     0,     0,     0,     0,   123,   128,     0,   128,   123,
       0,   128,     0,   123,   128,     0,     0,     0,     0,     0,
       0,     0,     0,   128,   124,     0,   128,     0,     0,   124,
      78,     0,     0,     0,     0,     0,     0,     0,     0,   292,
       0,     0,     0,   124,     0,     0,     0,     0,   128,   123,
       0,     0,     0,   129,    85,   124,     0,     0,     0,     0,
     128,     0,   128,     0,   124,     0,   128,     0,   124,     0,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,     0,     0,     0,   294,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   123,
     123,   123,   123,     0,     0,     0,     0,     0,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   123,   123,
       0,     0,     0,   128,     0,     0,     0,     0,   128,     0,
     296,     0,   297,   298,   299,     0,   300,   301,     0,     0,
       0,   304,   128,     0,     0,     0,     0,   310,   311,    85,
       0,     0,   314,     0,   128,   315,   316,   317,   124,   124,
     124,   124,     0,   128,     0,     0,     0,   128,     0,   393,
       0,   128,   393,     0,     0,   129,     0,   124,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   145,     0,     0,     0,    14,     0,   128,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   146,   147,     0,   148,     0,
       0,    31,     0,   129,    33,     0,   610,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,   128,   128,   128,
     128,     0,     0,     0,     0,     0,     0,   290,    54,    55,
      56,     0,    57,   149,   291,   144,   128,   128,     0,     0,
       0,   144,     0,    64,     0,   292,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,     0,   192,     0,
     129,     0,   129,     0,     0,   129,     0,     0,   129,    70,
       0,     0,     0,     0,     0,     0,     0,   129,     0,     0,
     129,     0,     0,     0,   235,     0,     0,    75,     0,   293,
       0,     0,     0,   294,     0,     0,     0,     0,     0,   150,
       0,    81,   129,   151,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   129,     0,   129,   370,   698,     0,
     129,   295,     0,     0,     0,     0,     0,   265,     0,   266,
       0,     0,     0,     0,     0,     0,   296,     0,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
       0,   315,   316,   317,     0,     0,     0,     0,     0,     0,
       0,   292,   735,     0,     0,     0,   737,   129,     0,     0,
       0,     0,   129,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   354,     0,     0,     0,   129,   235,     0,     0,
     362,     0,     0,     0,     0,     0,     0,     0,   129,     0,
     354,     0,     0,   385,     0,   293,     0,   129,     0,   294,
       0,   129,     0,     0,     0,   129,     0,   354,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,     0,   393,   354,   295,   393,     0,
       0,   393,     0,     0,     0,     0,     0,     0,     0,     0,
     812,   129,   296,   814,   297,   298,   299,     0,   300,   301,
     302,     0,   303,   304,   305,   306,     0,   308,   309,   310,
     311,     0,   312,   313,   314,   828,     0,   315,   316,   317,
       0,     0,   354,   192,   192,   192,   192,   834,     0,   838,
       0,     0,     0,    85,     0,     0,     0,     0,     0,     0,
       0,   129,   129,   129,   129,     0,     5,     6,     7,     8,
       9,    10,     0,   468,     0,   145,     0,     0,     0,    14,
     129,   129,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,   354,   354,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,   881,
       0,     0,     0,   533,     0,     0,     0,     0,     0,     0,
       0,   903,    54,    55,    56,     0,    57,   149,     0,     0,
     908,     0,     0,     0,   910,     0,   385,    64,    85,     0,
       0,     0,     0,     0,   192,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     0,   935,     0,     0,     0,     0,    74,
       0,    75,     0,    76,    77,     0,     0,     0,     0,    79,
     616,     0,     0,   620,     0,    81,   192,   958,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,     0,     0,
      15,    16,    17,     0,    18,  -298,    19,    20,    21,     0,
      22,    23,     0,    24,     0,    25,    26,    27,    28,    29,
      30,     0,     0,    31,    32,     0,    33,  -181,    34,    35,
      36,    37,    38,   -42,    39,    40,     0,    41,     0,     0,
       0,    42,    43,  -298,    44,    45,    46,    47,  -181,   -42,
      48,     0,    49,   354,  -298,    50,    51,     0,    52,    53,
      54,    55,    56,     0,    57,    58,     0,    59,     0,    60,
      61,    62,     0,     0,    63,    64,    65,  -298,     0,     0,
      66,     0,    67,     0,     0,    68,     0,     0,     0,   707,
       0,     0,     0,   385,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     0,     0,     0,     0,     0,   728,    74,     0,    75,
     235,    76,    77,     0,    78,     0,     0,    79,     0,     0,
       0,    80,     0,    81,     0,    82,     0,   987,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   145,     0,
     773,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   148,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,     0,   816,     0,     0,   385,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,   728,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,   620,     0,     0,     0,
       0,     0,    74,   855,    75,     0,    76,    77,   209,     0,
       0,     0,    79,     0,     0,     0,   150,     0,    81,     0,
     151,   988,     0,     4,     0,     5,     6,     7,     8,     9,
      10,     0,  -497,     0,    11,    12,    13,     0,    14,  -497,
       0,    15,    16,    17,     0,    18,  -298,    19,    20,    21,
    -497,    22,    23,  -497,    24,     0,    25,    26,    27,    28,
      29,    30,     0,     0,    31,    32,     0,    33,  -181,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -298,    44,     0,    46,    47,  -181,
     -42,    48,     0,    49,  -497,  -298,    50,    51,  -497,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,   918,
      60,    61,    62,     0,     0,    63,    64,    65,  -298,     0,
       0,    66,     0,    67,     0,     0,  -497,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -497,    70,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
       0,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,  -497,
      75,  -497,  -497,  -497,     0,    78,  -497,  -497,  -497,     0,
       0,     0,    80,  -497,    81,   247,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -298,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,    30,     0,     0,    31,    32,     0,    33,
    -181,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -298,    44,    45,    46,
      47,  -181,   -42,    48,     0,    49,     0,  -298,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -298,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,   209,    78,     0,     0,
      79,     0,     0,     0,    80,   248,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -298,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,     0,    31,    32,
       0,    33,  -181,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -298,    44,
      45,    46,    47,  -181,   -42,    48,     0,    49,     0,  -298,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -298,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,   421,    81,   247,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -298,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,    30,     0,     0,
      31,    32,     0,    33,  -181,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -298,    44,    45,    46,    47,  -181,   -42,    48,     0,    49,
       0,  -298,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -298,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,   248,
      81,   842,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -298,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,    30,
       0,     0,    31,    32,     0,    33,  -181,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -298,    44,    45,    46,    47,  -181,   -42,    48,
       0,    49,     0,  -298,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -298,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,   843,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -298,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,     0,    31,    32,     0,    33,  -181,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -298,    44,    45,    46,    47,  -181,
     -42,    48,     0,    49,     0,  -298,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -298,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,   912,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -298,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,    30,     0,     0,    31,    32,     0,    33,
    -181,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -298,    44,   261,    46,
      47,  -181,   -42,    48,     0,    49,     0,  -298,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -298,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -298,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,    30,     0,     0,    31,    32,
       0,    33,  -181,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -298,    44,
       0,    46,    47,  -181,   -42,    48,     0,    49,     0,  -298,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -298,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -298,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,   536,     0,     0,
      31,    32,     0,    33,  -181,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -298,    44,     0,    46,    47,  -181,   -42,    48,     0,    49,
       0,  -298,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -298,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -298,    19,    20,    21,     0,    22,
      23,     0,    24,     0,    25,    26,    27,    28,    29,   540,
       0,     0,    31,    32,     0,    33,  -181,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -298,    44,     0,    46,    47,  -181,   -42,    48,
       0,    49,     0,  -298,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -298,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,     0,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -298,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   797,     0,     0,    31,    32,     0,    33,  -181,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -298,    44,     0,    46,    47,  -181,
     -42,    48,     0,    49,     0,  -298,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -298,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -298,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,   799,     0,     0,    31,    32,     0,    33,
    -181,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -298,    44,     0,    46,
      47,  -181,   -42,    48,     0,    49,     0,  -298,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -298,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,     4,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,     0,     0,    15,    16,    17,     0,    18,
    -298,    19,    20,    21,     0,    22,    23,     0,    24,     0,
      25,    26,    27,    28,    29,   802,     0,     0,    31,    32,
       0,    33,  -181,    34,    35,    36,    37,    38,   -42,    39,
      40,     0,    41,     0,     0,     0,    42,    43,  -298,    44,
       0,    46,    47,  -181,   -42,    48,     0,    49,     0,  -298,
      50,    51,     0,    52,    53,    54,    55,    56,     0,    57,
      58,     0,    59,     0,    60,    61,    62,     0,     0,    63,
      64,    65,  -298,     0,     0,    66,     0,    67,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    75,     0,    76,    77,     0,    78,
       0,     0,    79,     0,     0,     0,    80,     0,    81,     4,
      82,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,     0,     0,    15,    16,    17,
       0,    18,  -298,    19,    20,    21,     0,    22,    23,     0,
      24,     0,    25,    26,    27,    28,    29,   805,     0,     0,
      31,    32,     0,    33,  -181,    34,    35,    36,    37,    38,
     -42,    39,    40,     0,    41,     0,     0,     0,    42,    43,
    -298,    44,     0,    46,    47,  -181,   -42,    48,     0,    49,
       0,  -298,    50,    51,     0,    52,    53,    54,    55,    56,
       0,    57,    58,     0,    59,     0,    60,    61,    62,     0,
       0,    63,    64,    65,  -298,     0,     0,    66,     0,    67,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    75,     0,    76,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
      81,     4,    82,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,     0,     0,    15,
      16,    17,     0,    18,  -298,    19,    20,    21,     0,   827,
      23,     0,    24,     0,    25,    26,    27,    28,    29,    30,
       0,     0,    31,    32,     0,    33,  -181,    34,    35,    36,
      37,    38,   -42,    39,    40,     0,    41,     0,     0,     0,
      42,    43,  -298,    44,     0,    46,    47,  -181,   -42,    48,
       0,    49,     0,  -298,    50,    51,     0,    52,    53,    54,
      55,    56,     0,    57,    58,     0,    59,     0,    60,    61,
      62,     0,     0,    63,    64,    65,  -298,     0,     0,    66,
       0,    67,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,    78,     0,     0,    79,     0,     0,     0,
      80,     0,    81,     4,    82,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,     0,
       0,    15,    16,    17,     0,    18,  -298,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   833,     0,     0,    31,    32,     0,    33,  -181,    34,
      35,    36,    37,    38,   -42,    39,    40,     0,    41,     0,
       0,     0,    42,    43,  -298,    44,     0,    46,    47,  -181,
     -42,    48,     0,    49,     0,  -298,    50,    51,     0,    52,
      53,    54,    55,    56,     0,    57,    58,     0,    59,     0,
      60,    61,    62,     0,     0,    63,    64,    65,  -298,     0,
       0,    66,     0,    67,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
      75,     0,    76,    77,     0,    78,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     4,    82,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,     0,     0,    15,    16,    17,     0,    18,  -298,    19,
      20,    21,     0,    22,    23,     0,    24,     0,    25,    26,
      27,    28,    29,   837,     0,     0,    31,    32,     0,    33,
    -181,    34,    35,    36,    37,    38,   -42,    39,    40,     0,
      41,     0,     0,     0,    42,    43,  -298,    44,     0,    46,
      47,  -181,   -42,    48,     0,    49,     0,  -298,    50,    51,
       0,    52,    53,    54,    55,    56,     0,    57,    58,     0,
      59,     0,    60,    61,    62,     0,     0,    63,    64,    65,
    -298,     0,     0,    66,     0,    67,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,    75,     0,    76,    77,     0,    78,     0,     0,
      79,     0,     0,     0,    80,     0,    81,   716,    82,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   145,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   148,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   145,    76,    77,   209,    14,
       0,     0,    79,     0,     0,     0,   150,     0,    81,     0,
     151,     0,     0,    23,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   149,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   145,    76,    77,   209,    14,     0,     0,    79,
       0,     0,     0,   150,     0,    81,     0,   151,   609,     0,
      23,     0,     0,     0,     0,     0,   146,   147,     0,   148,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   145,
      76,    77,   209,    14,     0,     0,    79,     0,     0,     0,
     150,     0,    81,     0,   151,   938,     0,    23,     0,     0,
       0,     0,     0,   146,   147,     0,   148,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,    75,     0,    76,    77,     0,
       0,     0,     0,    79,     0,     0,     0,   150,     0,    81,
       0,   151,   614,     5,     6,     7,     8,   173,    10,   174,
       0,     0,   145,     0,     0,     0,    14,     0,     0,   -84,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   146,   147,     0,   148,
       0,     0,    31,     0,     0,    33,  -469,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,  -469,     0,     0,
       0,   -84,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   149,     0,     0,     0,     0,     0,
     175,     0,     0,   -84,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    75,     0,
      76,    77,     0,     0,     0,     0,    79,     0,     0,     0,
     150,     0,    81,     0,   151,     5,     6,     7,     8,   173,
      10,     0,     0,     0,   145,     0,     0,     0,    14,     0,
       0,   -84,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,   146,   147,
       0,   148,     0,     0,    31,     0,     0,    33,  -469,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,  -469,
       0,     0,     0,   -84,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   149,     0,     0,     0,
       0,     0,   175,     0,     0,   -84,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   145,    76,    77,     0,    14,     0,     0,    79,     0,
       0,     0,   150,     0,    81,     0,   151,     0,     0,    23,
       0,     0,     0,     0,     0,   146,   147,     0,   148,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   149,     0,     0,     0,   213,   214,     0,
       0,   252,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,   253,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   145,    76,
      77,   209,    14,     0,     0,    79,     0,     0,     0,   150,
       0,    81,     0,   151,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   148,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,   213,   214,     0,     0,   252,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   145,    76,    77,   209,    14,
       0,     0,    79,     0,     0,     0,   150,     0,    81,   603,
     151,     0,     0,    23,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   149,     0,     0,
       0,   213,   214,     0,     0,   252,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   145,    76,    77,   209,    14,     0,     0,    79,
       0,     0,     0,   150,   507,    81,   605,   151,     0,     0,
      23,     0,     0,     0,     0,     0,   146,   147,     0,   148,
       0,   508,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,   509,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,   510,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   145,
      76,    77,     0,    14,     0,     0,    79,     0,     0,     0,
     150,     0,    81,     0,   151,     0,     0,    23,     0,     0,
       0,     0,     0,   146,   147,     0,   148,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   149,     0,     0,     0,   213,   214,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,    75,     0,    76,    77,     0,
     215,     0,     0,    79,     0,     0,     0,   150,     0,    81,
       0,   151,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   145,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,   146,   147,     0,   148,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   149,     0,     0,     0,   213,   214,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   145,    76,
      77,   209,    14,     0,     0,    79,     0,     0,     0,   150,
       0,    81,     0,   151,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   148,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,   718,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,   213,   214,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   145,    76,    77,     0,    14,
       0,     0,    79,     0,     0,     0,   150,     0,    81,     0,
     151,     0,     0,    23,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,   178,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   149,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,   179,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,    75,     0,    76,    77,     0,     0,     0,     0,    79,
       0,     0,     0,   150,     0,    81,     0,   151,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   145,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,   183,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
     184,   146,   147,     0,   148,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   145,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,   150,     0,    81,     0,   151,
       0,     0,    23,     0,     0,     0,     0,     0,   146,   147,
       0,   148,     0,     0,    31,     0,     0,    33,  -469,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,  -469,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   145,    76,    77,     0,    14,     0,     0,    79,     0,
       0,     0,   150,     0,    81,     0,   151,     0,     0,    23,
       0,     0,     0,     0,     0,   146,   147,     0,   148,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   149,     0,     0,     0,   213,   214,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   145,    76,
      77,     0,    14,     0,     0,    79,     0,     0,     0,   150,
       0,    81,     0,   151,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   148,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   145,    76,    77,   209,    14,
       0,     0,    79,     0,     0,     0,   150,   601,    81,     0,
     151,     0,   195,    23,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,    43,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,    56,     0,    57,   149,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   145,    76,    77,     0,    14,     0,     0,    79,
       0,     0,     0,    80,     0,    81,     0,   151,     0,     0,
      23,     0,     0,     0,     0,     0,   146,   147,     0,   148,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   145,
      76,    77,   209,    14,     0,     0,    79,     0,     0,     0,
     150,     0,    81,     0,   151,     0,     0,    23,     0,     0,
       0,     0,     0,   146,   147,     0,   148,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,   179,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   145,    76,    77,     0,
     884,     0,     0,    79,     0,     0,     0,   150,     0,    81,
       0,   151,     0,     0,   885,     0,     0,     0,     0,     0,
     146,   147,     0,   148,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,   886,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   887,   888,    56,     0,    57,   889,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   890,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   145,    76,    77,   209,    14,     0,     0,
      79,     0,     0,     0,   150,     0,    81,     0,   891,     0,
       0,    23,     0,     0,     0,     0,     0,   146,   147,     0,
     148,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     145,    76,    77,   209,    14,     0,     0,    79,     0,     0,
       0,   150,     0,    81,     0,   891,     0,     0,    23,     0,
       0,     0,     0,     0,   146,   147,     0,   869,     0,     0,
      31,     0,     0,    33,     0,     0,    35,    36,     0,    38,
       0,    39,    40,     0,     0,     0,     0,     0,     0,    43,
       0,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    57,   149,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     5,     6,
       7,     8,     9,    10,    74,     0,    75,   145,    76,    77,
     209,    14,     0,     0,    79,     0,     0,     0,   150,     0,
      81,     0,   151,     0,     0,    23,     0,     0,     0,     0,
       0,   146,   147,     0,   148,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   145,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,   150,     0,    81,     0,   151,
       0,     0,    23,     0,     0,     0,     0,     0,   146,   147,
       0,   148,     0,     0,    31,     0,     0,    33,     0,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   145,    76,    77,     0,   462,     0,     0,    79,     0,
       0,     0,    80,     0,    81,     0,   151,     0,     0,    23,
       0,     0,     0,     0,     0,   146,   147,     0,   148,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   145,    76,
      77,     0,    14,     0,     0,    79,     0,     0,     0,   150,
       0,    81,     0,   151,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   148,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   145,    76,    77,     0,   573,
       0,     0,   546,     0,     0,     0,   150,     0,    81,     0,
     151,     0,     0,   574,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,   575,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   576,   577,    56,     0,    57,   578,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   579,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   145,    76,    77,     0,    14,     0,     0,    79,
       0,     0,     0,   150,     0,    81,     0,   580,     0,     0,
      23,     0,     0,     0,     0,     0,   146,   147,     0,   148,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     0,     0,     0,    73,     0,     0,
       5,     6,     7,     8,     9,    10,    74,     0,    75,   145,
      76,    77,     0,    14,     0,     0,    79,     0,     0,     0,
     150,     0,    81,     0,   580,     0,     0,    23,     0,     0,
       0,     0,     0,   146,   147,     0,   661,     0,     0,    31,
       0,     0,    33,     0,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     0,     0,     0,     0,    43,     0,
      44,     0,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,    73,     0,     0,     5,     6,     7,
       8,     9,    10,    74,     0,    75,   145,    76,    77,     0,
      14,     0,     0,    79,     0,     0,     0,   150,     0,    81,
       0,   151,     0,     0,    23,     0,     0,     0,     0,     0,
     146,   147,     0,   664,     0,     0,    31,     0,     0,    33,
       0,     0,    35,    36,     0,    38,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    43,     0,    44,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,    56,     0,    57,   149,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,     0,     0,     0,
       0,    73,     0,     0,     5,     6,     7,     8,     9,    10,
      74,     0,    75,   145,    76,    77,     0,    14,     0,     0,
      79,     0,     0,     0,   150,     0,    81,     0,   151,     0,
       0,    23,     0,     0,     0,     0,     0,   146,   147,     0,
     863,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,     0,     0,     0,     0,    73,     0,
       0,     5,     6,     7,     8,     9,    10,    74,     0,    75,
     145,    76,    77,     0,    14,     0,     0,    79,     0,     0,
       0,   150,     0,    81,     0,   151,     0,     0,    23,     0,
       0,     0,     0,     0,   146,   147,     0,   864,     0,     0,
      31,     0,     0,    33,     0,     0,    35,    36,     0,    38,
       0,    39,    40,     0,     0,     0,     0,     0,     0,    43,
       0,    44,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,    56,
       0,    57,   149,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,     0,     0,     0,     0,    73,     0,     0,     5,     6,
       7,     8,     9,    10,    74,     0,    75,   145,    76,    77,
       0,    14,     0,     0,    79,     0,     0,     0,   150,     0,
      81,     0,   151,     0,     0,    23,     0,     0,     0,     0,
       0,   146,   147,     0,   866,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,     0,     0,
       0,     0,    73,     0,     0,     5,     6,     7,     8,     9,
      10,    74,     0,    75,   145,    76,    77,     0,    14,     0,
       0,    79,     0,     0,     0,   150,     0,    81,     0,   151,
       0,     0,    23,     0,     0,     0,     0,     0,   146,   147,
       0,   867,     0,     0,    31,     0,     0,    33,     0,     0,
      35,    36,     0,    38,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    43,     0,    44,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    57,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,     0,     0,     0,     0,    73,
       0,     0,     5,     6,     7,     8,     9,    10,    74,     0,
      75,   145,    76,    77,     0,    14,     0,     0,    79,     0,
       0,     0,   150,     0,    81,     0,   151,     0,     0,    23,
       0,     0,     0,     0,     0,   146,   147,     0,   868,     0,
       0,    31,     0,     0,    33,     0,     0,    35,    36,     0,
      38,     0,    39,    40,     0,     0,     0,     0,     0,     0,
      43,     0,    44,     0,    46,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
      56,     0,    57,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
      71,    72,     0,     0,     0,     0,    73,     0,     0,     5,
       6,     7,     8,     9,    10,    74,     0,    75,   145,    76,
      77,     0,    14,     0,     0,    79,     0,     0,     0,   150,
       0,    81,     0,   151,     0,     0,    23,     0,     0,     0,
       0,     0,   146,   147,     0,   869,     0,     0,    31,     0,
       0,    33,     0,     0,    35,    36,     0,    38,     0,    39,
      40,     0,     0,     0,     0,     0,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,     0,
       0,     0,     0,    73,     0,     0,     5,     6,     7,     8,
       9,    10,    74,     0,    75,   145,    76,    77,     0,   951,
       0,     0,    79,     0,     0,     0,   150,     0,    81,     0,
     151,     0,     0,   952,     0,     0,     0,     0,     0,   146,
     147,     0,   148,     0,     0,    31,     0,     0,    33,     0,
       0,    35,    36,     0,    38,     0,    39,    40,     0,     0,
       0,     0,     0,     0,   953,     0,    44,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   954,   955,    56,     0,    57,   956,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   957,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,     0,     0,     0,     0,
      73,     0,     0,     5,     6,     7,     8,     9,    10,    74,
       0,    75,   145,    76,    77,     0,    14,     0,     0,    79,
       0,     0,     0,   150,     0,    81,     0,   958,     0,     0,
      23,     0,     0,     0,     0,     0,   146,   147,     0,   148,
       0,     0,    31,     0,     0,    33,     0,     0,    35,    36,
       0,    38,     0,    39,    40,     0,     0,     0,     0,     0,
       0,    43,     0,    44,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,    56,     0,    57,   149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,     0,     0,     0,     0,     0,    69,
      70,    71,    72,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   145,     0,     0,    74,    14,    75,     0,
       0,    77,   332,   333,  -449,     0,    79,  -478,     0,  -478,
     150,    23,    81,     0,   151,     0,     0,   146,   147,     0,
     148,     0,     0,    31,     0,     0,    33,     0,     0,    35,
      36,     0,    38,     0,    39,    40,     0,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,    56,     0,    57,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,  -347,     0,   145,     0,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,    70,     0,     0,     0,  -347,    23,     0,     0,  -347,
       0,     0,   146,   147,     0,   148,     0,     0,    31,    75,
       0,    33,     0,   209,    35,    36,     0,    38,     0,    39,
      40,   150,     0,    81,  -347,   891,     0,    43,     0,    44,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -317,     0,     0,     0,    54,    55,    56,     0,    57,
     149,     0,     0,  -317,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,  -317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,     0,
       0,     0,     0,     0,     0,     0,    70,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   145,     0,     0,
       0,    14,     0,     0,    75,     0,     0,   332,   333,  -317,
       0,     0,  -478,     0,  -478,    23,  -317,     0,    81,     0,
     958,   146,   147,     0,   148,     0,     0,    31,     0,     0,
      33,     0,     0,    35,    36,     0,    38,     0,    39,    40,
       0,     0,     0,     0,     0,     0,    43,     0,    44,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,    56,     0,    57,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   145,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    70,     0,    23,     0,     0,
       0,     0,     0,   146,   147,     0,   148,     0,     0,    31,
       0,     0,    33,    75,     0,    35,    36,     0,    38,     0,
      39,    40,     0,     0,     5,   150,     0,    81,    43,   580,
      44,   748,    46,     0,     0,     0,     0,     0,   749,     0,
       0,     0,     0,     0,     0,     0,    54,    55,    56,     0,
      57,   149,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     5,     0,     0,     0,     0,     0,     0,   748,
       0,     0,     0,     0,     0,     0,   749,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
       0,   151,     0,     0,     0,     0,   275,   276,   277,   278,
     279,   280,     0,     0,   281,   282,   283,   284,   285,   286,
     750,   751,   752,   753,     0,     0,   754,     0,     0,     0,
     755,   756,   757,   758,   759,   760,   761,   762,   763,   764,
     765,     0,   766,     0,     0,   767,   768,   769,   770,     0,
     771,     0,     0,   772,   275,   276,   277,   278,   279,   280,
       0,     0,   281,   282,   283,   284,   285,   286,   750,   751,
     752,   753,     0,     0,   754,     0,     0,     0,   755,   756,
     757,   758,   759,   760,   761,   762,   763,   764,   765,     0,
     766,     0,   290,   767,   768,   769,   770,     0,   771,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     290,     0,     0,     0,     0,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,     0,
       0,     0,     0,     0,   293,     0,     0,     0,   294,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   290,
       0,     0,     0,     0,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   295,   292,     0,     0,
       0,     0,   293,     0,     0,     0,   294,     0,     0,     0,
       0,   296,     0,   297,   298,   299,     0,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,   295,     0,   315,   316,   317,     0,
       0,   293,     0,     0,     0,   294,     0,   791,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,   295,   315,   316,   317,     0,     0,   290,
       0,     0,     0,     0,     0,   947,   291,     0,   296,     0,
     297,   298,   299,     0,   300,   301,   302,   292,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,     0,   315,   316,   317,     0,     0,   290,     0,
       0,     0,     0,     0,  1001,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   292,     0,     0,     0,
       0,   293,     0,     0,     0,   294,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   295,     0,     0,     0,     0,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,   296,     0,
     297,   298,   299,     0,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,   295,   315,   316,   317,     0,     0,   290,     0,
       0,     0,   602,     0,     0,   291,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,   292,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,     0,   315,   316,   317,     0,     0,     0,     0,     0,
       0,   917,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,     0,   290,
       0,     0,     0,   376,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,     0,     0,
       0,     0,   295,   275,   276,   277,   278,   279,   280,     0,
       0,   281,   282,   283,   284,   285,   286,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,   293,   315,   316,   317,   294,     0,     0,   345,     0,
     290,     0,     0,     0,   796,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,     0,
       0,     0,     0,   295,   275,   276,   277,   278,   279,   280,
       0,     0,   281,   282,   283,   284,   285,   286,   296,     0,
     297,   298,   299,     0,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,   293,   315,   316,   317,   294,     0,     0,   345,
       0,   290,     0,     0,     0,   801,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
       0,     0,     0,     0,   295,   275,   276,   277,   278,   279,
     280,     0,     0,   281,   282,   283,   284,   285,   286,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,   293,   315,   316,   317,   294,     0,     0,
     345,     0,   290,     0,     0,     0,   930,     0,     0,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,     0,     0,     0,     0,   295,   275,   276,   277,   278,
     279,   280,     0,     0,   281,   282,   283,   284,   285,   286,
     296,     0,   297,   298,   299,     0,   300,   301,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,     0,
     312,   313,   314,     0,   293,   315,   316,   317,   294,     0,
       0,   345,     0,   290,     0,     0,     0,   931,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,     0,     0,     0,     0,   295,   275,   276,   277,
     278,   279,   280,     0,     0,   281,   282,   283,   284,   285,
     286,   296,     0,   297,   298,   299,     0,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,     0,   293,   315,   316,   317,   294,
       0,     0,   345,     0,   290,     0,     0,     0,   932,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,     0,   295,   275,   276,
     277,   278,   279,   280,     0,     0,   281,   282,   283,   284,
     285,   286,   296,     0,   297,   298,   299,     0,   300,   301,
     302,     0,   303,   304,   305,   306,   307,   308,   309,   310,
     311,     0,   312,   313,   314,     0,   293,   315,   316,   317,
     294,     0,     0,   345,     0,   290,     0,     0,     0,   933,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,     0,     0,     0,     0,   295,   275,
     276,   277,   278,   279,   280,     0,     0,   281,   282,   283,
     284,   285,   286,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   293,   315,   316,
     317,   294,     0,     0,   345,     0,   290,     0,     0,     0,
     949,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,     0,     0,     0,     0,   295,
     275,   276,   277,   278,   279,   280,     0,     0,   281,   282,
     283,   284,   285,   286,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,   312,   313,   314,     0,   293,   315,
     316,   317,   294,     0,     0,   345,     0,   290,     0,     0,
       0,   950,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   368,     0,     0,     0,
     295,   275,   276,   277,   278,   279,   280,     0,   369,   281,
     282,   283,   284,   285,   286,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   293,
     315,   316,   317,   294,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,     0,     0,   370,     0,     0,
       0,   295,   292,   365,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   366,   296,     0,   297,   298,
     299,     0,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
       0,   315,   316,   317,     0,     0,   293,   345,     0,     0,
     294,     0,     0,     0,     0,   290,     0,     0,     0,     0,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,   687,     0,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   293,   315,   316,
     317,   294,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,     0,     0,     0,   291,
       0,     0,     0,     0,     0,   370,     0,     0,     0,   295,
     292,   195,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,   312,   313,   314,     0,     0,   315,
     316,   317,   292,     0,   293,   345,     0,     0,   294,     0,
       0,     0,     0,   290,     0,     0,     0,     0,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   292,   681,     0,     0,     0,   295,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,     0,     0,     0,
     294,   296,     0,   297,   298,   299,     0,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,     0,   293,   315,   316,   317,   294,
       0,     0,   345,     0,   290,     0,     0,     0,     0,     0,
       0,   291,     0,   296,     0,   297,   298,   299,     0,   300,
     301,   302,   292,     0,   304,   305,   306,   295,   308,   309,
     310,   311,     0,     0,     0,   314,     0,     0,   315,   316,
     317,     0,   296,     0,   297,   298,   299,     0,   300,   301,
     302,     0,   303,   304,   305,   306,   307,   308,   309,   310,
     311,     0,   312,   313,   314,     0,   293,   315,   316,   317,
     294,     0,     0,   345,     0,   290,     0,     0,     0,   699,
       0,     0,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   292,     0,     0,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   293,   315,   316,
     317,   294,     0,     0,   345,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   292,     0,     0,     0,     0,   295,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,   297,   298,   299,     0,
     300,   301,   302,     0,   303,   304,   305,   306,   307,   308,
     309,   310,   311,     0,   312,   313,   314,     0,   293,   315,
     316,   317,   294,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   290,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,     0,     0,
     295,     0,     0,     0,     0,   292,     0,     0,     0,     0,
     373,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   374,   312,   313,   314,     0,   290,
     315,   316,   317,     0,     0,  -450,   291,     0,     0,   293,
       0,     0,     0,   294,     0,     0,     0,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   296,     0,   297,   298,
     299,   293,   300,   301,   302,   294,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
     375,   315,   316,   317,   290,     0,     0,     0,     0,   140,
       0,   291,     0,   295,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,     0,   416,   296,     0,
     297,   298,   299,     0,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,   417,     0,   315,   316,   317,   290,     0,     0,     0,
       0,     0,     0,   291,     0,     0,   293,     0,     0,     0,
     294,     0,     0,     0,   292,   516,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   517,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,   293,   300,
     301,   302,   294,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,   290,     0,   315,   316,
     317,     0,     0,   291,     0,     0,     0,     0,     0,     0,
     295,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   290,     0,
     315,   316,   317,     0,     0,   291,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,   292,   518,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   519,
       0,     0,   290,     0,     0,     0,     0,     0,     0,   291,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,     0,     0,     0,     0,   296,     0,   297,   298,   299,
     293,   300,   301,   302,   294,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,   407,
     315,   316,   317,     0,   290,     0,     0,     0,     0,     0,
       0,   291,   295,     0,   293,     0,     0,     0,   294,     0,
       0,     0,   292,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   295,   312,   313,   314,
       0,     0,   315,   316,   317,     0,     0,     0,     0,     0,
       0,   296,     0,   297,   298,   299,   293,   300,   301,   302,
     294,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,   290,   409,   315,   316,   317,     0,
       0,   291,     0,     0,     0,     0,     0,     0,   295,     0,
       0,     0,   292,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,   290,   411,   315,   316,
     317,     0,     0,   291,     0,     0,   293,     0,     0,     0,
     294,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,   293,   300,
     301,   302,   294,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,   290,   419,   315,   316,
     317,     0,     0,   291,     0,     0,     0,     0,     0,   434,
     295,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   290,     0,
     315,   316,   317,     0,     0,   291,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,   292,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,     0,     0,     0,   291,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,     0,     0,     0,     0,   296,     0,   297,   298,   299,
     293,   300,   301,   302,   294,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,     0,
     315,   316,   317,     0,   290,   694,     0,     0,   140,     0,
       0,   291,   295,     0,   293,     0,     0,     0,   294,     0,
       0,     0,   292,     0,     0,     0,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   295,   312,   313,   314,
       0,     0,   315,   316,   317,     0,     0,     0,     0,     0,
       0,   296,     0,   297,   298,   299,   293,   300,   301,   302,
     294,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,   290,   671,   315,   316,   317,     0,
       0,   291,     0,     0,     0,     0,     0,     0,   295,     0,
       0,     0,   292,     0,     0,   697,     0,     0,     0,     0,
       0,     0,     0,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,   290,     0,   315,   316,
     317,     0,     0,   291,     0,     0,   293,     0,     0,     0,
     294,     0,     0,     0,   292,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     290,     0,     0,     0,     0,     0,     0,   291,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   292,   786,
       0,     0,     0,   296,     0,   297,   298,   299,   293,   300,
     301,   302,   294,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,     0,   315,   316,
     317,     0,     0,     0,     0,     0,     0,     0,     0,   738,
     295,     0,   293,     0,     0,     0,   294,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   295,   312,   313,   314,     0,     0,
     315,   316,   317,     0,     0,     0,     0,     0,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,   290,     0,   315,   316,   317,     0,     0,   291,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     292,   789,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,     0,     0,     0,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   292,
       0,     0,     0,     0,   293,     0,     0,     0,   294,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     290,     0,     0,     0,     0,     0,     0,   291,     0,     0,
       0,     0,     0,     0,     0,     0,   295,     0,   292,     0,
       0,     0,     0,   293,     0,     0,     0,   294,     0,     0,
       0,   296,     0,   297,   298,   299,   862,   300,   301,   302,
       0,   303,   304,   305,   306,   307,   308,   309,   310,   311,
       0,   312,   313,   314,     0,   295,   315,   316,   317,     0,
       0,     0,   293,     0,     0,     0,   294,     0,     0,     0,
     296,     0,   297,   298,   299,   865,   300,   301,   302,     0,
     303,   304,   305,   306,   307,   308,   309,   310,   311,     0,
     312,   313,   314,     0,   295,   315,   316,   317,     0,     0,
     290,   879,     0,     0,     0,     0,     0,   291,     0,   296,
       0,   297,   298,   299,     0,   300,   301,   302,   292,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,     0,   315,   316,   317,     0,     0,   290,
       0,     0,     0,     0,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   292,     0,     0,
       0,     0,   293,     0,     0,     0,   294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   290,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,   295,     0,   292,     0,     0,     0,
       0,   293,     0,     0,     0,   294,     0,     0,     0,   296,
       0,   297,   298,   299,   966,   300,   301,   302,     0,   303,
     304,   305,   306,   307,   308,   309,   310,   311,     0,   312,
     313,   314,     0,   295,   315,   316,   317,     0,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,   296,     0,
     297,   298,   299,   967,   300,   301,   302,     0,   303,   304,
     305,   306,   307,   308,   309,   310,   311,     0,   312,   313,
     314,     0,   295,   315,   316,   317,     0,     0,   290,     0,
       0,     0,     0,     0,     0,   291,     0,   296,     0,   297,
     298,   299,     0,   300,   301,   302,   292,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,     0,   315,   316,   317,     0,     0,   290,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,     0,     0,     0,     0,
     293,     0,     0,     0,   294,     0,     0,     0,     0,     0,
       0,     0,     0,   968,     0,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,   295,     0,   292,     0,     0,     0,     0,   293,
       0,     0,     0,   294,     0,     0,     0,   296,     0,   297,
     298,   299,   969,   300,   301,   302,     0,   303,   304,   305,
     306,   307,   308,   309,   310,   311,     0,   312,   313,   314,
       0,   295,   315,   316,   317,     0,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,   296,     0,   297,   298,
     299,   970,   300,   301,   302,     0,   303,   304,   305,   306,
     307,   308,   309,   310,   311,     0,   312,   313,   314,     0,
     295,   315,   316,   317,     0,     0,   290,     0,     0,     0,
       0,     0,     0,   291,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,   292,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,     0,     0,
     315,   316,   317,     0,   290,     0,     0,     0,     0,     0,
       0,   291,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,     0,     0,   293,     0,
       0,     0,   294,     0,     0,     0,     0,     0,     0,     0,
       0,   971,     0,   290,     0,     0,     0,     0,     0,     0,
     291,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     295,   292,     0,     0,     0,     0,   293,     0,     0,     0,
     294,     0,     0,     0,     0,   296,     0,   297,   298,   299,
       0,   300,   301,   302,     0,   303,   304,   305,   306,   307,
     308,   309,   310,   311,     0,   312,   313,   314,   295,     0,
     315,   316,   317,     0,     0,   659,     0,     0,     0,   294,
       0,     0,     0,   296,     0,   297,   298,   299,     0,   300,
     301,   302,     0,   303,   304,   305,   306,   307,   308,   309,
     310,   311,     0,   312,   313,   314,     0,   295,   315,   316,
     317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,     0,   297,   298,   299,     0,   300,   301,
     302,     0,   303,   304,   305,   306,   307,   308,   309,   310,
     311,     0,   312,   313,   314,     0,     0,   315,   316,   317
};

static const yytype_int16 yycheck[] =
{
       2,    47,    48,   338,     2,    16,    50,   384,    27,    11,
     116,   164,    14,   412,   544,    17,     3,   381,    20,    21,
      22,    65,   175,    25,    26,    27,    28,    29,    30,     1,
     188,   715,    29,     1,    41,    37,    38,    39,    82,    41,
       3,    43,    44,   271,   272,   273,   274,   205,    50,   777,
      52,    53,    54,    55,   258,   572,    58,    59,    60,    61,
      75,     3,    64,    65,    66,    67,   224,    29,    70,     3,
       3,    73,    74,    75,     3,    77,    29,   741,    80,    23,
      82,    29,    84,     1,     1,    41,    58,   899,   938,    58,
      34,    35,     1,   816,   124,   124,   124,   781,    41,    43,
     124,    30,    46,   100,     0,    92,   105,   151,    52,   100,
      73,   124,   270,     3,    72,   143,    60,   124,    76,   105,
     143,   151,   151,    67,    68,    97,   656,    28,    97,   128,
      73,   155,    41,   983,    24,   150,   820,   105,   124,   141,
     153,   658,   128,   145,   146,   147,   148,   149,   150,   151,
     389,    41,   163,   150,    44,   123,   124,    75,    75,    41,
     883,    90,   124,    41,   155,   152,    41,   153,   124,    59,
      87,    72,    62,   412,    64,    76,   334,   335,   990,   151,
      98,    98,   151,    73,   374,   153,   850,   189,   150,   152,
      75,   919,   920,   195,   571,   712,   105,   150,   882,   155,
      90,    24,   150,    41,   206,   123,   123,   871,   150,   544,
     152,   213,   214,   983,   123,   124,   444,    46,   152,   152,
     100,   152,   123,   154,   125,   126,   143,   417,    28,   130,
     124,  1001,   150,   150,   143,    73,   137,   105,    67,    62,
     124,   124,   124,   144,   145,   146,   124,   405,   250,   124,
      73,   253,   250,   937,     3,    81,   124,   124,   143,   153,
     128,     7,   152,   124,   152,   150,   154,    90,    19,   153,
     153,   143,    72,   155,   293,   294,    76,   155,   808,   151,
     155,    19,   143,    28,    90,   153,   153,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,    41,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,    24,   336,   337,   363,
      71,   656,   152,   123,   143,   125,   126,    72,   347,   706,
     130,    76,   151,    71,   336,   337,   338,   137,   542,   543,
     704,   124,    93,   345,   170,   347,   146,   366,   152,   351,
     369,   353,    24,   152,    62,    93,   124,   124,   100,    83,
     988,   363,   188,   365,   366,    73,   368,   369,    75,   997,
     153,   373,   374,   152,   376,   143,   378,   741,   123,   205,
      87,   126,   124,   124,    68,   130,   153,   389,    96,   391,
      62,    98,   152,   124,    75,   439,   398,    75,   224,   143,
      37,    73,   149,   607,   608,   126,    87,   124,   143,    87,
     412,   430,   153,   155,   416,   417,    53,    98,    90,    41,
      98,   423,   153,    60,    61,   143,   143,   826,   430,   431,
     432,   124,   434,   435,   143,   124,   143,   439,   124,   124,
      60,    61,    72,   150,   270,   598,    76,   143,   143,   124,
     143,   143,   152,    75,   143,    23,   143,   143,   143,    23,
     462,   463,   143,   143,    99,   143,    34,    35,   143,   150,
      34,    35,   150,   808,   150,    43,    98,     2,    46,    43,
     143,   152,    46,   105,   152,   152,    11,   154,    52,   152,
     143,   154,    60,   152,   858,    20,    60,    22,   517,    67,
     519,   105,   124,    67,    68,    41,    41,   871,   334,   335,
     143,     7,   523,   524,   516,   517,   518,   519,   520,   521,
     522,   143,   628,   123,    52,   162,   126,     1,   150,     3,
     130,   153,   534,    58,   536,   426,   580,   428,   540,    75,
      75,   123,   544,   180,   181,   182,   143,   150,   105,   124,
     187,   153,   554,   143,    41,    80,   152,   124,    31,    84,
     153,   198,    98,    98,   201,   153,   105,   153,   153,   105,
     105,   573,   398,   575,   576,   577,   578,   579,   580,   405,
     819,    18,   124,   220,    24,   155,   155,   826,   124,   124,
     150,   153,   143,   143,   143,   143,   143,   423,   143,   143,
     426,    41,   428,   240,    44,   431,   143,   143,   143,   143,
     612,   143,   143,   143,   150,   150,   141,   153,   153,    59,
     143,   143,    62,   143,    64,    24,   143,   143,     2,   153,
     143,   124,   143,    73,   143,    31,   143,    11,   153,   151,
     659,   660,    41,   153,   143,    44,    20,   150,    22,   124,
      90,   151,   105,    73,   656,   150,    29,   659,   660,   661,
      59,    29,   664,    62,   189,    64,   155,    31,   123,    73,
     195,   155,   123,   155,    73,   155,   915,   679,   722,   681,
     151,   126,   684,   126,    58,   687,    60,    61,   152,    31,
     692,    90,   694,   153,   696,   697,    98,   699,    87,   938,
     739,   153,   151,   529,   417,   531,    80,   155,   947,    19,
      84,   348,   416,   232,   726,   176,   533,   794,   744,   721,
     722,   628,   857,   704,   858,   250,   747,   564,   990,   937,
     962,   733,    29,   735,   392,   372,   738,   739,   531,    25,
      -1,   743,    -1,    -1,   983,    -1,    -1,    -1,    -1,   988,
      -1,   857,    -1,    -1,    -1,    -1,    -1,    -1,   997,    -1,
      -1,    -1,  1001,    -1,    -1,    -1,    -1,   141,    -1,    -1,
     772,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   785,   786,    -1,   788,   789,   790,   791,
      -1,    -1,    -1,    -1,   796,   797,    -1,   799,    -1,   801,
     802,   847,   848,   805,    -1,    -1,   808,    -1,    -1,    -1,
      -1,    -1,    -1,   815,    -1,   189,    -1,   819,    -1,    -1,
     345,   195,    -1,   825,   826,   827,    -1,    -1,    -1,   831,
      -1,   833,    -1,    -1,   836,   837,    -1,    -1,   840,    -1,
     365,    -1,   844,   368,   846,    -1,   844,   891,    -1,    -1,
      -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     862,   863,   864,   865,   866,   867,   868,   869,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   250,   879,   880,    -1,
      -1,    -1,   884,    -1,   886,   887,   888,   889,   890,   891,
      -1,    -1,    -1,   895,    -1,    -1,   898,    -1,    -1,    -1,
      -1,   538,   539,    -1,    -1,   542,   543,   432,    -1,    -1,
      -1,    -1,    -1,   915,   958,    -1,    -1,    -1,   555,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   930,   931,
     932,   933,    -1,    -1,    -1,    -1,   938,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   947,    -1,   949,   950,   951,
      -1,   953,   954,   955,   956,   957,   958,   594,    -1,    -1,
      -1,    -1,    -1,    -1,   966,   967,   968,   969,   970,   971,
      -1,   345,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
     982,   983,    -1,    -1,    -1,    -1,   988,    -1,   513,    -1,
      -1,   365,    -1,   819,   368,   997,    -1,    -1,    53,  1001,
      -1,     2,   376,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    20,
      -1,    22,    -1,    72,    -1,    -1,    20,    76,    22,   554,
      -1,    -1,   669,   670,    -1,    -1,    -1,    -1,    -1,   676,
      -1,   678,    -1,    -1,    -1,    -1,   683,    -1,    -1,   686,
      -1,    -1,   689,    -1,    -1,    -1,    -1,    58,   432,    60,
      61,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,   895,
     119,    -1,   121,   122,   123,    -1,   125,   126,    -1,    80,
      -1,   130,    -1,    84,    -1,    -1,    80,   612,   137,    -1,
      84,    -1,   729,    -1,    -1,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,   628,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   938,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,   180,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,   659,   660,    -1,    -1,    -1,    -1,
     141,    -1,    -1,   198,    -1,    -1,   201,   141,    81,    -1,
      -1,    -1,    -1,    -1,   679,    -1,   681,   983,    -1,   684,
      -1,    -1,   687,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,   696,    -1,    -1,   699,  1001,    -1,    -1,    -1,    -1,
     554,    -1,    -1,    -1,    -1,   240,    -1,    -1,   189,     1,
      -1,    -1,   829,    -1,   195,   189,   721,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,    -1,    19,   733,    -1,
     735,    23,    24,    -1,   739,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,   873,   874,   612,   876,
     877,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,   250,
      62,    -1,    64,    -1,    66,    67,   250,    69,    -1,    71,
      -1,    73,   899,    -1,    -1,    -1,    -1,    -1,   905,    -1,
     907,   796,   909,    -1,    -1,    -1,   801,    -1,    90,    -1,
       2,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,    11,
     815,    -1,    -1,   348,    -1,    -1,    -1,    -1,    20,    -1,
      22,    -1,   827,    -1,    -1,   679,    -1,   681,    -1,    -1,
     684,   836,    -1,   687,    -1,   840,    -1,   372,    -1,   844,
      -1,    -1,   696,    -1,    -1,   699,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    58,    -1,    -1,   151,
      -1,    -1,    -1,    -1,   345,    -1,    -1,   721,    -1,    -1,
      -1,   345,    -1,   990,   287,   880,    -1,    -1,    80,   733,
      -1,   735,    84,    -1,   365,   739,    -1,   368,    -1,    -1,
      -1,   365,    -1,    -1,   368,   376,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,    -1,    -1,    -1,    -1,   930,   931,   932,   933,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,   141,
      -1,    -1,   796,    -1,   949,   950,    -1,   801,    -1,    -1,
      19,   432,    -1,    -1,    23,    -1,    -1,    -1,   432,    -1,
      -1,   815,    -1,    32,    -1,    34,    35,    -1,    -1,    38,
      -1,    -1,    -1,   827,    43,    -1,    -1,    46,    -1,    -1,
      72,    -1,   836,    -1,    76,    -1,   840,   189,    -1,    -1,
     844,    60,    -1,   195,    -1,    64,    -1,    66,    67,    -1,
      69,    -1,    71,   538,   539,    -1,    -1,   542,   543,    -1,
      -1,    -1,   104,   426,    -1,   428,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    93,    -1,   880,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,     2,   134,   135,   136,   137,    -1,   139,   250,   141,
      11,    -1,   144,   145,   146,    28,    -1,    -1,    -1,    20,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   554,   143,    -1,   930,   931,   932,   933,
     554,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   949,   950,    58,    -1,    72,
       2,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,   529,    -1,    20,    80,
      22,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   612,    -1,    -1,   669,   670,    -1,    -1,   612,    -1,
      -1,   676,    -1,   345,    -1,    -1,   119,    -1,    -1,   122,
     123,   686,   125,   126,   689,    -1,    58,   130,    -1,    -1,
      -1,    -1,    -1,   365,   137,    -1,   368,    -1,    -1,    -1,
     583,   144,   145,   146,   376,    -1,    -1,    -1,    80,    -1,
     141,    -1,    84,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   679,     2,
     681,    -1,    -1,   684,    -1,   679,   687,   681,    11,    -1,
     684,    -1,    -1,   687,    -1,   696,    -1,    20,   699,    22,
      -1,    -1,   696,    -1,    -1,   699,    -1,    -1,   189,    -1,
     432,    -1,    72,    -1,   195,    -1,    76,    -1,    -1,   141,
     721,    -1,    -1,    -1,    -1,    -1,    -1,   721,    -1,    -1,
      -1,    -1,   733,    -1,   735,    58,    -1,    -1,   739,   733,
      -1,   735,    -1,    -1,    -1,   739,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,   119,
      -1,    84,    -1,   123,    -1,   125,   126,   189,    -1,   250,
     130,    -1,    -1,   195,   829,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   796,    -1,    -1,    -1,    -1,
     801,    -1,   796,    -1,    -1,    -1,    -1,   801,    28,    -1,
      -1,    -1,    -1,    -1,   815,    -1,    -1,    -1,   141,    -1,
      -1,   815,    -1,    -1,    -1,    -1,   827,    -1,   250,    -1,
      -1,    -1,   554,   827,    -1,   836,    -1,    -1,    -1,   840,
      -1,    -1,   836,   844,    19,    -1,   840,    -1,    23,    -1,
     844,    -1,    72,    -1,    -1,    -1,    76,    32,    -1,    34,
      35,    -1,    -1,    38,   345,    -1,   189,    -1,    43,    -1,
      -1,    46,   195,    -1,    -1,    -1,    -1,    -1,    -1,   880,
      -1,    -1,    -1,    -1,   365,    60,   880,   368,    -1,    64,
     612,    66,    67,    -1,    69,   376,    71,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    90,   136,   137,    93,    -1,
      -1,   141,    -1,   345,   144,   145,   146,   250,    -1,   930,
     931,   932,   933,    -1,    -1,    -1,   930,   931,   932,   933,
      -1,    -1,    -1,   365,    -1,    -1,   368,    -1,   949,   950,
      -1,   432,    -1,    -1,   376,   949,   950,   679,    -1,   681,
      -1,    -1,   684,    -1,    -1,   687,    -1,    -1,   143,    -1,
      -1,    -1,    -1,     2,   696,    -1,   151,   699,    -1,    -1,
      -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    22,    -1,    -1,    -1,    -1,    -1,   721,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     432,   733,    -1,   735,    -1,    -1,    -1,   739,    -1,    -1,
      -1,    -1,   345,   105,   106,   107,   108,   109,   110,    58,
      -1,   113,   114,   115,   116,   117,   118,    -1,     1,    -1,
      -1,    -1,   365,    -1,    -1,   368,    28,    -1,    -1,    -1,
      -1,    80,    -1,   376,    -1,    84,    19,    -1,    -1,    -1,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    34,    35,   554,   796,    38,    -1,    -1,    -1,   801,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,   815,    76,    -1,    -1,    60,    -1,    62,
      -1,    64,    -1,    66,    67,   827,    69,    -1,    71,   432,
      73,    -1,   141,    -1,   836,    -1,    -1,    -1,   840,    -1,
      -1,    -1,   844,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      93,   612,   554,    96,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,    -1,   134,   135,   136,   137,     2,   139,   880,   141,
     189,    -1,   144,   145,   146,    11,   195,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    22,    -1,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
     612,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   679,    -1,
     681,    -1,    -1,   684,    -1,    -1,   687,    -1,   930,   931,
     932,   933,    58,    -1,    -1,   696,    -1,    -1,   699,    -1,
      -1,   250,    -1,    -1,    -1,    -1,    -1,   949,   950,    -1,
      -1,   554,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
     721,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   733,    -1,   735,    -1,    19,   679,   739,   681,
      23,    -1,   684,    -1,    -1,   687,    -1,    -1,    -1,    32,
      -1,    34,    35,    -1,   696,    38,    -1,   699,    -1,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,   612,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    60,    -1,   721,
      -1,    64,    -1,    66,    67,    -1,    69,    -1,    71,    -1,
      -1,   733,    -1,   735,    -1,   796,   345,   739,    -1,    -1,
     801,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      93,    -1,    -1,    -1,   815,    -1,   365,    -1,    -1,   368,
      -1,    -1,    -1,   189,    -1,    -1,   827,   376,    -1,   195,
      -1,    -1,    -1,    -1,    -1,   836,   679,    -1,   681,   840,
      -1,   684,    -1,   844,   687,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   696,   796,    -1,   699,    -1,    -1,   801,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,   815,    -1,    -1,    -1,    -1,   721,   880,
      -1,    -1,    -1,   432,   250,   827,    -1,    -1,    -1,    -1,
     733,    -1,   735,    -1,   836,    -1,   739,    -1,   840,    -1,
      -1,    -1,   844,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   930,
     931,   932,   933,    -1,    -1,    -1,    -1,    -1,   880,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   949,   950,
      -1,    -1,    -1,   796,    -1,    -1,    -1,    -1,   801,    -1,
     119,    -1,   121,   122,   123,    -1,   125,   126,    -1,    -1,
      -1,   130,   815,    -1,    -1,    -1,    -1,   136,   137,   345,
      -1,    -1,   141,    -1,   827,   144,   145,   146,   930,   931,
     932,   933,    -1,   836,    -1,    -1,    -1,   840,    -1,   365,
      -1,   844,   368,    -1,    -1,   554,    -1,   949,   950,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    16,    -1,   880,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,   612,    45,    -1,   432,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,   930,   931,   932,
     933,    -1,    -1,    -1,    -1,    -1,    -1,    10,    79,    80,
      81,    -1,    83,    84,    17,    13,   949,   950,    -1,    -1,
      -1,    19,    -1,    94,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
     679,    -1,   681,    -1,    -1,   684,    -1,    -1,   687,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   696,    -1,    -1,
     699,    -1,    -1,    -1,    62,    -1,    -1,   138,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,   150,
      -1,   152,   721,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   733,    -1,   735,   100,   554,    -1,
     739,   104,    -1,    -1,    -1,    -1,    -1,   105,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,   155,    -1,    -1,    -1,   612,   796,    -1,    -1,
      -1,    -1,   801,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,   815,   175,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   827,    -1,
     188,    -1,    -1,   191,    -1,    72,    -1,   836,    -1,    76,
      -1,   840,    -1,    -1,    -1,   844,    -1,   205,    -1,    -1,
      -1,   209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   679,    -1,   681,   224,   104,   684,    -1,
      -1,   687,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     696,   880,   119,   699,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,   721,    -1,   144,   145,   146,
      -1,    -1,   270,   271,   272,   273,   274,   733,    -1,   735,
      -1,    -1,    -1,   739,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   930,   931,   932,   933,    -1,     3,     4,     5,     6,
       7,     8,    -1,   301,    -1,    12,    -1,    -1,    -1,    16,
     949,   950,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,   334,   335,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,   815,
      -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   827,    79,    80,    81,    -1,    83,    84,    -1,    -1,
     836,    -1,    -1,    -1,   840,    -1,   384,    94,   844,    -1,
      -1,    -1,    -1,    -1,   392,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   405,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,   880,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,   146,
     438,    -1,    -1,   441,    -1,   152,   444,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    -1,    -1,    42,    43,    -1,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    -1,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,   531,    73,    74,    75,    -1,    77,    78,
      79,    80,    81,    -1,    83,    84,    -1,    86,    -1,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,   567,
      -1,    -1,    -1,   571,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   594,   136,    -1,   138,
     598,   140,   141,    -1,   143,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,   152,    -1,   154,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
     628,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,   703,    -1,    -1,   706,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   726,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,   744,    -1,    -1,    -1,
      -1,    -1,   136,   751,   138,    -1,   140,   141,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,   155,    -1,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    -1,    16,    17,
      -1,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    -1,    -1,    42,    43,    -1,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,    83,    84,    -1,    86,   857,
      88,    89,    90,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    99,    -1,   101,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,   143,   144,   145,   146,    -1,
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
     136,    -1,   138,    -1,   140,   141,   142,   143,    -1,    -1,
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
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
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
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,   155,    -1,
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
     150,    -1,   152,    -1,   154,   155,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,   155,     3,     4,     5,     6,     7,     8,     9,
      -1,    -1,    12,    -1,    -1,    -1,    16,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    -1,    42,    -1,    -1,    45,    46,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,   138,    -1,
     140,   141,    -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    16,    -1,
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
      -1,    -1,     3,     4,     5,     6,     7,     8,   136,    -1,
     138,    12,   140,   141,    -1,    16,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,   152,    -1,   154,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,   128,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,   142,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    88,    89,    -1,    -1,    92,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,   142,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,   153,
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
     127,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,   140,   141,   142,    16,    -1,    -1,   146,
      -1,    -1,    -1,   150,    24,   152,   153,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      -1,    41,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    -1,    -1,    -1,
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
      83,    84,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,
      -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   138,    -1,   140,   141,    -1,
     143,    -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,
      -1,   154,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,
      81,    -1,    83,    84,    -1,    -1,    -1,    88,    89,    -1,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,     3,
       4,     5,     6,     7,     8,   136,    -1,   138,    12,   140,
     141,   142,    16,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,    -1,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    -1,    88,    89,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    -1,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    -1,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    -1,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,   150,    -1,   152,    -1,   154,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      35,    36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
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
      -1,    39,    -1,    -1,    42,    -1,    -1,    45,    46,    -1,
      48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,    67,
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
      81,    -1,    83,    84,    -1,    -1,    -1,    88,    89,    -1,
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
       7,     8,   136,    -1,   138,    12,   140,   141,   142,    16,
      -1,    -1,   146,    -1,    -1,    -1,   150,   151,   152,    -1,
     154,    -1,    29,    30,    -1,    -1,    -1,    -1,    -1,    36,
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
     140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,    -1,
     150,    -1,   152,    -1,   154,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
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
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   127,    -1,
      -1,     3,     4,     5,     6,     7,     8,   136,    -1,   138,
      12,   140,   141,   142,    16,    -1,    -1,   146,    -1,    -1,
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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,   119,
     120,   121,   122,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,   136,    16,   138,    -1,
      -1,   141,   147,   148,   149,    -1,   146,   152,    -1,   154,
     150,    30,   152,    -1,   154,    -1,    -1,    36,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    80,    81,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,   105,    -1,    12,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,    -1,    -1,   124,    30,    -1,    -1,   128,
      -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,   138,
      -1,    45,    -1,   142,    48,    49,    -1,    51,    -1,    53,
      54,   150,    -1,   152,   153,   154,    -1,    61,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    79,    80,    81,    -1,    83,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    98,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    16,    -1,    -1,   138,    -1,    -1,   147,   148,   143,
      -1,    -1,   152,    -1,   154,    30,   150,    -1,   152,    -1,
     154,    36,    37,    -1,    39,    -1,    -1,    42,    -1,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    80,    81,    -1,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    -1,    -1,    42,
      -1,    -1,    45,   138,    -1,    48,    49,    -1,    51,    -1,
      53,    54,    -1,    -1,     3,   150,    -1,   152,    61,   154,
      63,    10,    65,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,    -1,    -1,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,   141,    -1,    -1,   144,   145,   146,   147,    -1,
     149,    -1,    -1,   152,   105,   106,   107,   108,   109,   110,
      -1,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,   125,    -1,    -1,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
     141,    -1,    10,   144,   145,   146,   147,    -1,   149,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    28,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,   104,    -1,   144,   145,   146,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,   155,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,   104,   144,   145,   146,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,   155,    17,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    28,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,    -1,   144,   145,   146,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,   155,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   104,   144,   145,   146,    -1,    -1,    10,    -1,
      -1,    -1,   153,    -1,    -1,    17,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    28,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    10,
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
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,    41,   113,
     114,   115,   116,   117,   118,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,   104,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      -1,   144,   145,   146,    -1,    -1,    72,   150,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,   104,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    -1,   144,
     145,   146,    28,    -1,    72,   150,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      76,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    72,   144,   145,   146,    76,
      -1,    -1,   150,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    28,    -1,   130,   131,   132,   104,   134,   135,
     136,   137,    -1,    -1,    -1,   141,    -1,    -1,   144,   145,
     146,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    72,   144,   145,   146,
      76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,    85,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    57,   139,   140,   141,    -1,    10,
     144,   145,   146,    -1,    -1,   149,    17,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    72,   125,   126,   127,    76,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     143,   144,   145,   146,    10,    -1,    -1,    -1,    -1,   100,
      -1,    17,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    33,   119,    -1,
     121,   122,   123,    -1,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    57,    -1,   144,   145,   146,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    10,    -1,   144,   145,
     146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    10,    -1,
     144,   145,   146,    -1,    -1,    17,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      72,   125,   126,   127,    76,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,   143,
     144,   145,   146,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,   104,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   104,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    72,   125,   126,   127,
      76,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    10,   143,   144,   145,   146,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    10,   143,   144,   145,
     146,    -1,    -1,    17,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    10,   143,   144,   145,
     146,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    10,    -1,
     144,   145,   146,    -1,    -1,    17,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      72,   125,   126,   127,    76,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    10,    11,    -1,    -1,   100,    -1,
      -1,    17,   104,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   104,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    72,   125,   126,   127,
      76,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    10,   143,   144,   145,   146,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    28,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    10,    -1,   144,   145,
     146,    -1,    -1,    17,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    72,   125,
     126,   127,    76,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    -1,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   104,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    10,    -1,   144,   145,   146,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    28,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    85,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,   104,   144,   145,   146,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
     119,    -1,   121,   122,   123,    85,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    -1,
     139,   140,   141,    -1,   104,   144,   145,   146,    -1,    -1,
      10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    28,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    28,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    85,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,   104,   144,   145,   146,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,
     121,   122,   123,    85,   125,   126,   127,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,    -1,   139,   140,
     141,    -1,   104,   144,   145,   146,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    28,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,    -1,   144,   145,   146,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,    28,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    76,    -1,    -1,    -1,   119,    -1,   121,
     122,   123,    85,   125,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    -1,   139,   140,   141,
      -1,   104,   144,   145,   146,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    85,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     104,   144,   145,   146,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    28,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
     144,   145,   146,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    28,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,   104,    -1,
     144,   145,   146,    -1,    -1,    72,    -1,    -1,    -1,    76,
      -1,    -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,   104,   144,   145,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,   122,   123,    -1,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,    -1,   139,   140,   141,    -1,    -1,   144,   145,   146
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
     209,   213,   233,   236,   237,   257,   258,   259,   260,   261,
     262,   263,   271,   272,   273,   274,   277,   278,   279,   280,
     281,   282,   284,   285,   286,   287,   288,   143,   164,   274,
     100,   264,   265,   176,   177,    12,    36,    37,    39,    84,
     150,   154,   205,   257,   258,   262,   272,   273,   274,   277,
     279,   280,   264,   274,    90,   176,   164,   251,   274,   164,
     152,   274,   275,     7,     9,    90,   178,   275,    62,   102,
     186,   274,   274,    23,    35,   236,   274,   274,   152,   177,
     206,   152,   177,   238,   239,    29,   168,   179,   274,   274,
     274,   274,   274,   274,     7,   152,    35,   182,   182,   142,
     229,   250,   274,    88,    89,   143,   274,   276,   274,   179,
     274,   274,   274,    83,   152,   164,   274,   274,   168,   175,
     274,   277,   168,   175,   274,   177,   234,   274,   250,   274,
     274,   274,   274,   274,   274,   274,   274,     1,   151,   162,
     169,   250,    92,   128,   229,   252,   253,   276,   250,   274,
     283,    64,   164,    68,   167,   177,   177,    46,    67,   224,
     152,    24,    62,    73,    96,   105,   106,   107,   108,   109,
     110,   113,   114,   115,   116,   117,   118,   216,   143,   149,
      10,    17,    28,    72,    76,   104,   119,   121,   122,   123,
     125,   126,   127,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   139,   140,   141,   144,   145,   146,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   147,   148,   152,   154,    72,    76,   152,   164,
     143,   274,   274,   274,   250,   150,   168,    41,   264,   234,
     143,   124,   143,    99,   177,   229,   254,   255,   256,   276,
     234,   198,   177,   152,   179,    29,    41,   179,    29,    41,
     100,   179,   267,    33,    57,   143,    85,   168,   216,   254,
     164,   152,   218,    92,   152,   177,   240,   241,     1,   123,
     245,    41,   124,   164,   179,   179,   254,   177,   124,   143,
     274,   274,   143,   150,   179,   152,   254,   143,   190,   143,
     190,   143,   105,   235,   143,   143,    33,    57,   179,   143,
     151,   151,   162,   124,   151,   274,   124,   153,   124,   153,
      41,   124,   155,   267,   103,   124,   155,     7,    52,   123,
     199,   150,   210,   254,    73,   238,   238,   238,   238,   276,
     274,   274,   274,   274,   186,   274,   186,   274,   274,   274,
     274,   274,    16,    94,   274,   274,    30,    90,   177,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   254,   254,   186,   274,   186,   274,    24,    41,    73,
      96,   266,   269,   270,   274,   288,    29,    41,    29,    41,
      85,    41,   155,   186,   274,   179,   143,   274,   274,   105,
     153,   124,   143,   177,    41,   250,    39,   274,   186,   274,
      39,   274,   186,   274,   152,   179,   146,   170,   172,   274,
     170,   171,   164,   274,    31,   274,   153,    24,    41,    44,
      59,    62,    73,    90,   163,   219,   220,   221,   222,   207,
     241,   124,   153,    16,    30,    61,    79,    80,    84,    94,
     154,   246,   259,   105,   242,   274,   239,   153,   229,   274,
       1,   195,   254,   153,    18,   191,   246,   259,   124,   172,
     171,   151,   153,   153,   252,   153,   252,   186,   274,   155,
     164,   274,   155,   274,   155,   274,   177,   250,   150,     1,
     177,   203,   204,    24,    62,    73,    90,   212,   223,   153,
     238,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   153,   155,    41,    73,   124,   153,   257,    72,
      72,    39,   186,   274,    39,   186,   274,   186,   274,   264,
     264,   143,   229,   276,   256,   199,   274,   153,   274,    29,
     179,    29,   179,   274,    29,   179,   267,    29,   179,   267,
     268,   269,   124,   143,    11,   143,    31,    31,   164,    85,
     168,    41,    73,   221,   124,   153,   152,   177,    24,    62,
      73,    90,   225,   153,   241,   245,     1,   250,    55,   276,
     151,    58,    97,   151,   196,   153,   152,   168,   177,   192,
     234,   143,   143,   155,   267,   155,   267,   164,   103,   150,
       1,   200,   151,   105,   124,   151,    73,   211,    10,    17,
     119,   120,   121,   122,   125,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   141,   144,   145,   146,
     147,   149,   152,   177,   213,   214,   215,   216,   257,   143,
     269,   245,   257,   257,   274,    29,    29,   274,    29,    29,
     155,   155,   179,   179,   150,   179,    85,    39,   274,    39,
     274,    85,    39,   274,   179,    39,   274,   179,   124,   153,
     274,   274,   164,   274,   164,    31,   177,   220,   241,   123,
     249,    73,   245,   242,   155,    41,   155,    29,   164,   250,
     192,   123,   168,    39,   164,   274,   155,    39,   164,   274,
     155,   274,     1,   151,   169,   151,    35,    66,    69,   151,
     163,   181,   274,   204,   225,   177,   274,   126,   152,   217,
     217,   242,    85,    39,    39,    85,    39,    39,    39,    39,
       1,   200,   274,   274,   274,   274,   274,   274,   269,    11,
      31,   164,   249,   153,    16,    30,    61,    79,    80,    84,
      94,   154,   229,   248,   259,   128,   231,   242,    98,   232,
     274,   246,   259,   164,   179,   153,   274,   274,   164,   274,
     164,   151,   151,   181,     1,   123,   244,   153,   177,   215,
     216,   219,   274,   274,   274,   274,   274,   274,   151,   151,
      85,    85,    85,    85,   274,   164,   242,   249,   155,   250,
     229,   230,   274,   274,   168,   180,   228,   155,   168,    85,
      85,    16,    30,    61,    79,    80,    84,    94,   154,   243,
     259,    87,   226,   217,   217,   153,    85,    85,    85,    85,
      85,    85,   274,   274,   274,   274,   231,   242,   229,   247,
     248,   259,    41,   155,   259,   274,   274,     1,   155,   250,
     232,   274,   247,   248,   155,   243,   259,   155,   143,   227,
     228,   155,   243,   259,   247
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
     212,   212,   212,   213,   214,   214,   214,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   217,   217,   218,   219,   219,
     219,   220,   220,   220,   220,   220,   221,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   223,   223,   223,
     223,   223,   223,   224,   224,   225,   225,   225,   225,   225,
     225,   226,   226,   227,   227,   228,   228,   229,   229,   230,
     230,   231,   231,   232,   232,   233,   233,   233,   234,   234,
     235,   235,   235,   236,   236,   236,   236,   236,   237,   237,
     237,   238,   238,   239,   239,   240,   240,   240,   241,   241,
     241,   242,   242,   242,   243,   243,   243,   243,   243,   243,
     243,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   246,   246,   246,   246,   247,   247,   247,
     248,   248,   248,   248,   248,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   250,   250,   250,   250,
     251,   251,   252,   252,   252,   253,   253,   254,   254,   255,
     255,   256,   256,   256,   256,   257,   258,   258,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   262,   263,   263,   263,   263,   263,   263,
     263,   263,   264,   264,   265,   266,   266,   267,   268,   268,
     269,   269,   269,   270,   270,   270,   270,   270,   270,   271,
     271,   272,   273,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   275,
     275,   276,   276,   276,   277,   277,   277,   277,   278,   278,
     279,   279,   279,   280,   280,   280,   281,   281,   281,   281,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   283,   283,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   285,   285,   285,   285,
     285,   285,   286,   286,   286,   286,   287,   287,   287,   287,
     288,   288,   288,   288,   288,   288,   288
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
       3,     5,     5,     1,     1,     1,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     3,     0,     1,
       3,     4,     5,     4,     6,     6,     0,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     2,     1,     0,
       1,     2,     2,     0,     2,     3,     4,     4,     2,     4,
       0,     2,     2,     4,     4,     4,     5,     4,     0,     1,
       1,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     1,     1,     1,     1,     4,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     2,     2,     2,     2,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       3,     2,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     3,     3,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     4,     3,     4,
       1,     1,     1,     1,     1,     3,     4,     3,     4,     3,
       4,     3,     5,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
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
#line 5293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5439 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5445 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5451 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5457 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5547 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 628 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 630 "chapel.ypp" /* yacc.c:1661  */
    { 
      (yyval.pblockstmt) = buildErrorStandin();
      USR_FATAL_CONT((yyval.pblockstmt), "illegal assignment to type");
    }
#line 5686 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5692 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5698 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5710 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 651 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5716 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5722 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5728 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5740 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5752 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5764 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5770 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 5784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 693 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 714 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 720 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 739 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 754 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 755 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 756 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 760 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 780 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 785 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 795 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 799 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 801 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 806 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 813 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 815 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6132 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6146 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6160 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 864 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 874 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 890 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 892 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6248 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 913 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6286 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6298 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6344 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6364 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 983 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 989 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 996 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1019 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6442 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1067 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1071 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1092 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1099 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1100 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1101 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1104 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1106 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1119 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1120 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1121 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1122 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1123 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1124 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6719 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1128 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6743 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1133 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1137 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1141 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6761 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1142 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1143 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6773 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1148 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1150 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6785 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1152 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1154 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6839 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6845 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6851 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6857 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6869 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6881 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6887 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6893 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6899 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6911 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6917 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6923 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6935 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6941 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6953 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6959 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6965 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6971 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6977 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6989 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1226 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1231 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1232 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1239 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1241 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1246 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1259 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1277 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1279 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1292 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1326 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1327 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1328 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1342 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1344 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1349 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1351 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1370 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1376 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1380 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1384 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1388 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1390 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1402 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1403 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1405 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1416 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1417 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1418 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1419 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1420 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1421 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1423 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1424 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1425 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7433 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1455 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1463 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7491 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1510 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1511 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1541 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1544 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7719 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7743 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7761 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7773 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7785 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7854 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1639 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1645 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7884 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7890 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7896 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7902 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7908 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1693 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7932 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7938 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1702 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 7952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1711 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1715 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 7974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 7980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1723 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 7986 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 7992 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 7998 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8004 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8010 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1737 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8016 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1742 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1758 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8028 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1760 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8034 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8046 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8052 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8064 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8070 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8076 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 470:
#line 1777 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8082 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1780 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8088 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1781 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8094 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1782 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8100 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8106 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8112 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1806 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8118 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8124 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8130 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 485:
#line 1812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8136 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 486:
#line 1820 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8142 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 487:
#line 1821 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8148 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 488:
#line 1822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8154 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 489:
#line 1823 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8160 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1827 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 8166 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1828 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1829 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1830 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8184 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1831 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8190 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1832 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1833 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1834 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1835 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1837 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1841 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1849 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1862 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8350 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8356 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8380 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1884 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1895 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1896 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1897 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1898 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1903 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1904 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1905 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1906 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 544:
#line 1907 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 545:
#line 1908 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 546:
#line 1909 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8510 "bison-chapel.cpp" /* yacc.c:1661  */
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
