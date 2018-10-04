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
#define YYNRULES  541
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  978

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
    1044,  1048,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1066,
    1067,  1068,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1101,  1102,
    1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,
    1116,  1117,  1121,  1125,  1126,  1127,  1131,  1133,  1135,  1137,
    1139,  1144,  1145,  1149,  1150,  1151,  1152,  1153,  1154,  1155,
    1156,  1157,  1161,  1162,  1163,  1164,  1165,  1166,  1170,  1171,
    1175,  1176,  1177,  1178,  1179,  1180,  1184,  1185,  1188,  1189,
    1193,  1194,  1198,  1200,  1205,  1206,  1210,  1211,  1215,  1216,
    1220,  1222,  1224,  1229,  1242,  1259,  1260,  1262,  1267,  1275,
    1283,  1291,  1300,  1310,  1311,  1312,  1316,  1317,  1325,  1327,
    1332,  1334,  1336,  1341,  1343,  1345,  1352,  1353,  1354,  1359,
    1361,  1363,  1367,  1371,  1373,  1377,  1385,  1386,  1387,  1388,
    1389,  1390,  1391,  1392,  1393,  1394,  1395,  1400,  1401,  1402,
    1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1430,  1434,
    1438,  1446,  1453,  1454,  1455,  1459,  1461,  1467,  1469,  1471,
    1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1484,  1485,
    1486,  1492,  1493,  1494,  1495,  1499,  1500,  1504,  1505,  1506,
    1510,  1511,  1515,  1516,  1520,  1521,  1525,  1526,  1527,  1528,
    1532,  1536,  1537,  1548,  1549,  1550,  1551,  1552,  1553,  1554,
    1556,  1558,  1560,  1562,  1564,  1566,  1568,  1570,  1572,  1574,
    1579,  1581,  1583,  1585,  1587,  1589,  1591,  1593,  1595,  1597,
    1599,  1601,  1603,  1610,  1616,  1622,  1628,  1637,  1647,  1655,
    1656,  1657,  1658,  1659,  1660,  1661,  1662,  1667,  1668,  1672,
    1676,  1677,  1681,  1685,  1686,  1690,  1694,  1698,  1705,  1706,
    1707,  1708,  1709,  1710,  1714,  1715,  1720,  1725,  1733,  1734,
    1735,  1736,  1737,  1738,  1739,  1740,  1741,  1743,  1745,  1747,
    1749,  1751,  1753,  1755,  1760,  1761,  1764,  1765,  1766,  1769,
    1770,  1771,  1772,  1783,  1784,  1788,  1789,  1790,  1794,  1795,
    1796,  1804,  1805,  1806,  1807,  1811,  1812,  1813,  1814,  1815,
    1816,  1817,  1818,  1819,  1820,  1824,  1832,  1833,  1837,  1838,
    1839,  1840,  1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,
    1849,  1850,  1851,  1852,  1853,  1854,  1855,  1856,  1857,  1858,
    1859,  1863,  1864,  1865,  1866,  1867,  1868,  1872,  1873,  1874,
    1875,  1879,  1880,  1881,  1882,  1887,  1888,  1889,  1890,  1891,
    1892,  1893
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

#define YYPACT_NINF -856

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-856)))

#define YYTABLE_NINF -493

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -856,   102,  3174,  -856,   -64,  -856,  -856,  -856,  -856,  -856,
    -856,  4390,    56,   208,  -856,  9361,  -856,    56,  9361,  -856,
      87,   208,  4390,  9361,  4390,    53,  -856,  9361,  6521,  7814,
    9361,  7966,  9361,  -856,    75,  -856,  -856,   208,  -856,    34,
    8499,  9361,  9361,  -856,  9361,  -856,  9361,  9361,   262,   127,
     951,  1147,  -856,  -856,  8626,  7383,  9361,  8499,  9361,  9361,
     258,  -856,   229,  4390,  9361,  9488,  9488,   208,  -856,  -856,
    9361,  8626,  9361,  9361,  -856,  -856,  9361,  -856,  -856, 11393,
    9361,  9361,  -856,  9361,  -856,  -856,  3478,  6800,  8626,  -856,
    4238,  -856,  -856,   286,  -856,  -856,  -856,  -856,  -856,  -856,
    -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,
    -856,   208,  -856,   208,   327,    50,  -856,   244,  -856,  -856,
     332,   233,   705,  -856,  -856,  -856,   284,   290,   295,   302,
     306, 15197,  1048,   259,   307,   311,  -856,  -856,  -856,  -856,
    -856,  -856,   202,  -856,  -856, 15197,   303,  4390,  -856,   317,
    -856,  9361,  9361,  9361,  9361,  9361,  8626,  8626,   326,  -856,
    -856,  -856,  -856,  -856,   335,   345,  -856,  -856,   323, 13622,
     208,   331,  -856,    93, 15197,   383,  7535, 15197,  -856,   141,
    -856,   208,   180,  -856,   208,   336,     5, 13134, 13057,  -856,
    -856,  -856, 13570, 12418,  7535,  4390,   346,    21,    97,    94,
    -856,  4390,  -856,  -856, 13205,   120,   335, 13205,   335,   120,
    -856,  7535,  8093,  -856,  -856,   208,  -856,   129, 15197,  9361,
    9361,  -856, 15197,   367, 13418,  -856, 13205,   335, 15197,   359,
    7535,  -856, 15197, 13839,  -856,  -856, 13925,  2344,  -856,  -856,
   13977,   410,   373,   335,   134, 13707, 13205, 14057,   204,  2066,
     120,   204,   120,   111,  -856,  -856,  3630,   -15,  -856,  9361,
    -856,   107,   118,  -856,    71, 14109,   -44,   512,  -856,  -856,
     468,   402,   376,  -856,  -856,  -856,  7535,    35,    34,    34,
      34,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,
    -856,  -856,  -856,  8220,  -856,  9361,  9361,  9361,  9361,  8778,
    8778,  9361,  9361,  9361,  9361,  9615,  9361,   148, 11393,  9361,
    9361,  9361,  9361,  9361,  9361,  9361,  9361,  9361,  9361,  9361,
    9361,  9361,  9361,  9361,  8220,  8220,  8220,  8220,  8220,  7535,
    7535,  8778,  8778,  7256,  -856,  -856, 13759, 13891, 14189,    43,
    3782,  -856,  8778,     5,   384,  -856,  9361,  -856,  9361,   423,
    -856,   378,   405,  -856,  -856,   390,   208,   493,  8626,  -856,
    4542,  8778,  -856,  4694,  8778,   385,  -856,     5,  9742,  9361,
    -856,  4390,   507,  9361,   386,  -856,   591,  -856,  -856,    21,
    -856,   416,   389,  -856,  9869,   440,  9361,    34,  -856,  -856,
    -856,   401,  -856,  8626,  -856, 15197, 15197,  -856,    25,  -856,
    7535,   404,  -856,   547,  -856,   547,  -856,  9996,   445,  -856,
    -856,  9742,  9361,  -856,  -856,  -856,  -856,  -856,  8347,  -856,
   12276,  6952,  -856,  7104,  -856,  8778,  6062,  3326,   417,  9361,
    6368,  -856,  -856,   208,  8626,   421,   484,    99,   420,    34,
     200,   218,   220,   221,   431, 13489,  2632,  2632,   -31,  -856,
     -31,  -856,   -31, 12176,   660,   494,   937, 11734, 11734,   335,
     204,  -856,  -856,  -856,  2066,  2125,   -31,  2418,  2418,  2632,
    2418,  2418,  2446,   204,  2125,  2240,  2446,   120,   120,   204,
     432,   433,   434,   435,   436,   427,   426,  -856,   -31,  -856,
     -31,   166,  -856,  -856,  -856,   128,  -856,   208, 15236,   315,
   10123,  8778, 10250,  8778,  9361,  8778, 11918,    56, 14241,  -856,
    -856, 15197, 14275,  7535,  -856,  7535,  -856,   402,  9361,   172,
    9361, 15197,    44, 13276,  9361, 15197,   -26,  1375,  7256,  -856,
     441,   459,   446, 14327,   459,   447,   557, 14407,  4390, 13347,
    -856,   174,  -856,  -856,  -856,  -856,  -856,  -856,   603,   215,
    -856,    54,  -856,   379,   438,    21,    97,  9361,    53,  9361,
    9361,  9361,  9361,  9361,  5910,  -856,   333,  7687,  -856, 15197,
    -856,  -856,  -856, 15197,   442,     6,   443,  -856,    38,  -856,
    -856,   222,   208,   452,   454,  -856,  -856,  -856,  -856,  -856,
    -856,    -3,  2990,  -856,  -856, 15197,  4390, 15197,  -856, 14459,
     448,   476,  1218,   451,   499,    79,  -856,   534,  -856,  -856,
    -856,  -856, 12001,  -856,   242,  -856,  -856,  -856,  -856,  -856,
    -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  7256,
    -856,    10,  8778,  8778,  9361,   579, 14493,  9361,   581, 14625,
     456, 12052,     5,     5,  -856,  -856,  -856,  -856,   464, 13205,
    -856, 12489,  4846,  -856,  4998,  -856, 12560,  5150,  -856,     5,
    5302,  -856,     5,   224,  -856,  9361,  -856,  9361,  -856,  4390,
    9361,  -856,  4390,   597,  -856,  -856,   208,   628,  -856,    21,
     506,   560,  -856,  -856,  -856,    41,  -856,  -856,   440,   475,
      73,  -856,  -856,  -856,  5454,  8626,  -856,  -856,  -856,   208,
    -856,   511,   323,  -856,  -856,  -856,  5606,   481,  5758,   482,
    -856,  9361,  3934,   491,  1575,  -856,  9361,   208,  -856,  -856,
     379,  -856,  -856,  -856,   208,  -856,  -856,  -856,  -856,  -856,
    -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -856,
    -856,  -856,  -856,  -856,  -856,  9361,   522,  -856,   531,   508,
     508,  -856,  -856,  -856,   440,   300,   310, 14664, 10377, 10504,
   14703, 10631, 10758, 10885, 11012,  -856,  -856,  1322,  -856,  4390,
    9361, 15197,  9361, 15197,  4390,  9361, 15197,  -856,  9361, 15197,
    -856,  7256,  -856, 14803, 15197,  -856, 15197,   627,  4390,   506,
    -856,   515,  8905,   -23,  -856,   563,  -856,  -856,  8778, 11826,
    4390,  -856,    29,   517,  9361,  -856,  9361,  -856, 15197,  4390,
    9361,  -856, 15197,  4390, 15197,   235,  -856,  4086,  -856,  6673,
     501,   501,  -856,  1989,  -856, 15197,  -856,    31,  -856, 12347,
     989,   591,  -856,  -856,  -856,  9361,  9361,  9361,  9361,  9361,
    9361,  9361,  9361,   520,  1827, 14407, 12631, 12702, 14407, 12773,
   12844,  -856,  9361,  4390,  -856,   440,   506,  9361,    53,  9361,
    9361,  9361,  9361,  9361,  6215,  -856,  -856,   231,  8626,  -856,
    -856,  9361,    18, 12134,  -856,   829,   383,  -856,   323, 15197,
   12915,  -856, 12986,  -856,  -856,  -856,  -856,  -856, 11139,   586,
    -856,  -856,   508,   508,   319, 14842, 14881, 14981, 15020, 15059,
   15159,  -856,  -856,  4390,  4390,  4390,  4390, 15197,  -856,  -856,
     -23,  9057,    80,  -856,  -856, 15197, 15197,  -856,  -856,  -856,
   11012,  -856,  4390,  4390,  9361,    53,  9361,  9361,  9361,  9361,
    9361,  2933,  -856,   292,  -856,   563,  -856,  -856,  -856,  9361,
    9361,  9361,  9361,  9361,  9361, 14407, 14407, 14407, 14407,  -856,
    -856,  -856,  -856,  -856,   343,  8778, 11520,   883, 14407, 14407,
     523, 11266,   109,    16, 12205,  -856,  -856,  -856,  -856,   308,
   11615,  -856,  -856,  -856,  9209,  -856,   334,  -856
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    72,   485,   486,   487,   488,
     489,     0,   427,    70,   182,     0,   151,   427,     0,   187,
     294,    70,     0,     0,     0,     0,   161,   464,   295,     0,
       0,   293,     0,   186,     0,   171,   183,     0,   167,     0,
       0,     0,     0,   418,     0,   174,     0,     0,     0,     0,
     293,   293,   185,   152,     0,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,   184,   153,
       0,     0,     0,     0,   537,   539,     0,   540,   541,   463,
       0,     0,   538,   535,    79,   536,     0,     0,     0,     4,
       0,     5,     9,    45,    10,    11,    12,    14,   380,    22,
      13,    80,    86,    15,    17,    16,    19,    20,    21,    18,
      85,     0,    83,     0,   453,     0,    87,   381,    84,    88,
       0,   469,   384,   449,   450,   386,   383,     0,     0,   454,
     455,     0,   385,     0,   470,   471,   472,   448,   388,   387,
     451,   452,     0,    38,    24,   394,     0,     0,   428,     0,
      71,     0,     0,     0,     0,     0,     0,     0,   453,   469,
     384,   383,   454,   455,   396,   385,   470,   471,     0,   427,
       0,     0,   125,     0,   365,     0,   372,   465,   172,   488,
      93,     0,     0,   173,     0,     0,     0,     0,     0,   294,
     295,    92,     0,     0,   372,     0,     0,     0,     0,     0,
     296,     0,    76,    32,     0,   523,   446,     0,   397,   524,
       7,   372,   295,    82,    81,   273,   362,     0,   361,     0,
       0,    77,   468,     0,     0,    35,     0,   399,   389,     0,
     372,    36,   395,     0,   132,   128,     0,   385,   132,   129,
       0,   285,     0,   398,     0,   361,     0,     0,   526,   462,
     522,   525,   521,     0,    47,    50,     0,     0,   367,     0,
     369,     0,     0,   368,     0,   361,     0,     0,     6,    46,
       0,   154,     0,   259,   258,   175,   372,     0,     0,     0,
       0,   218,   225,   226,   227,   222,   224,   220,   223,   221,
     219,   229,   228,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   372,
     372,     0,     0,     0,    25,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,    28,     0,    29,     0,   380,
     378,     0,   373,   374,   379,     0,     0,     0,     0,   102,
       0,     0,   101,     0,     0,     0,   108,     0,     0,    56,
      89,     0,   118,     0,     0,    30,   233,   168,   300,     0,
     301,   303,     0,   337,     0,   306,     0,     0,    75,    31,
      33,     0,   272,     0,    63,   466,   467,    78,     0,    34,
     372,     0,   139,   130,   126,   131,   127,     0,   283,   280,
      60,     0,    56,    95,    37,    49,    48,    51,     0,   490,
       0,     0,   481,     0,   483,     0,     0,     0,     0,     0,
       0,   494,     8,     0,     0,     0,     0,   252,     0,     0,
       0,     0,     0,     0,     0,   426,   518,   517,   520,   528,
     527,   532,   531,   514,   511,   512,   513,   458,   459,   457,
     501,   480,   479,   478,   460,   505,   516,   510,   508,   519,
     509,   507,   499,   504,   506,   515,   498,   502,   503,   500,
       0,     0,     0,     0,     0,     0,     0,   530,   529,   534,
     533,   438,   439,   441,   443,     0,   430,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   492,   427,   427,    98,
     281,   366,     0,     0,   391,     0,   282,   154,     0,     0,
       0,   402,     0,     0,     0,   408,     0,     0,     0,   109,
     536,    59,     0,    52,    57,     0,   117,     0,     0,     0,
     390,   246,   243,   244,   245,   249,   250,   251,   241,     0,
     234,     0,   242,   260,     0,   304,     0,   333,   330,   334,
     336,   331,   332,   335,     0,   329,   449,     0,   298,   447,
     297,   477,   364,   363,     0,     0,     0,   392,     0,   133,
     287,   449,     0,     0,     0,   491,   456,   482,   370,   484,
     371,     0,     0,   493,   114,   412,     0,   496,   495,     0,
       0,   155,     0,     0,   165,     0,   162,   256,   253,   254,
     257,   176,     0,   382,     0,   289,   288,   290,   292,    69,
      67,    68,    65,    66,    64,   475,   476,   440,   442,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,   376,   377,   375,     0,     0,
     116,     0,     0,   100,     0,    99,     0,     0,   106,     0,
       0,   104,     0,     0,   433,     0,    90,     0,    91,     0,
       0,   120,     0,   122,   247,   248,     0,   241,   232,     0,
     350,   261,   264,   263,   265,     0,   302,   305,   306,     0,
       0,   307,   308,   141,     0,     0,   140,   143,   393,     0,
     134,   137,     0,   284,    61,    62,     0,     0,     0,     0,
     115,     0,     0,     0,   293,   160,     0,   163,   159,   255,
     260,   215,   213,   194,   197,   195,   196,   207,   198,   211,
     203,   201,   214,   202,   200,   205,   210,   212,   199,   204,
     208,   209,   206,   216,   217,     0,   192,   190,     0,   230,
     230,   189,   291,   431,   306,   469,   469,     0,     0,     0,
       0,     0,     0,     0,     0,    97,    96,     0,   103,     0,
       0,   401,     0,   400,     0,     0,   407,   107,     0,   406,
     105,     0,   432,    54,    53,   119,   417,   121,     0,   350,
     235,     0,     0,   306,   262,   278,   299,   341,     0,   492,
       0,   145,     0,     0,     0,   135,     0,   112,   414,     0,
       0,   110,   413,     0,   497,     0,    39,     0,   149,   295,
     293,   293,   147,   293,   157,   166,   164,     0,   193,     0,
       0,   233,   178,   179,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   293,   405,     0,     0,   411,     0,
       0,   434,     0,     0,   124,   306,   350,   356,   353,   357,
     359,   354,   355,   358,     0,   352,   360,   449,   274,   238,
     236,     0,     0,     0,   339,   449,   146,   144,     0,   138,
       0,   113,     0,   111,    41,    40,   158,   326,     0,   266,
     191,   192,   230,   230,     0,     0,     0,     0,     0,     0,
       0,   150,   148,     0,     0,     0,     0,    55,   123,   237,
     306,   342,     0,   275,   277,   276,   279,   270,   271,   169,
       0,   136,     0,     0,   322,   319,   323,   325,   320,   321,
     324,     0,   318,   449,   267,   278,   180,   181,   231,     0,
       0,     0,     0,     0,     0,   404,   403,   410,   409,   240,
     239,   344,   345,   347,   449,     0,   492,   449,   416,   415,
       0,   310,     0,     0,     0,   346,   348,   315,   313,   449,
     492,   268,   177,   269,   342,   314,   449,   349
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -856,  -856,  -856,     0,  -326,  1982,  -856,  -856,  -856,   399,
     -37,  -302,   265,   271,  -856,  -856,   371,   664,  2468,  -856,
      37,  -794,  -686,   -42,  -856,  -856,  -856,    98,  -856,  -856,
    -856,   453,  -856,   -10,  -856,  -856,  -856,  -856,  -856,   516,
     177,   -65,  -856,  -856,  -856,   -14,  1027,  -856,  -856,  -856,
    -856,  -856,  -856,  -856,    92,  -856,  -124,  -109,  -679,  -856,
    -123,    30,   161,  -856,  -856,  -856,    -9,  -856,  -856,  -253,
      -1,  -856,  -198,  -222,  -856,  -156,  -856,   684,  -856,  -190,
     329,  -856,  -339,  -673,  -700,  -856,  -505,  -406,  -855,  -807,
    -726,   -32,  -856,    85,  -856,  -145,  -856,   207,   519,   876,
    -237,  -856,  -856,  1331,  -856,    -7,  -856,  -856,  -259,  -856,
    -522,  -856,  -856,  1405,  1627,   -11,   690,    45,  1003,  -856,
    1815,  2020,  -856,  -856,  -856,  -856,  -856,  -856,  -856,  -306
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   255,    90,   536,    92,    93,   270,    94,
     256,   531,   535,   532,    95,    96,    97,   149,    98,   182,
     203,    99,   100,   101,   102,   103,   104,   640,   105,   106,
     107,   403,   579,   702,   108,   109,   575,   697,   110,   111,
     435,   714,   112,   113,   605,   606,   158,   196,   553,   115,
     116,   437,   720,   611,   117,   748,   749,   373,   832,   377,
     549,   550,   551,   552,   612,   275,   685,   935,   972,   919,
     216,   914,   869,   872,   118,   242,   408,   119,   120,   199,
     200,   381,   382,   568,   932,   889,   385,   565,   952,   866,
     793,   257,   173,   261,   262,   351,   352,   353,   159,   160,
     123,   124,   125,   161,   127,   147,   148,   495,   367,   663,
     496,   497,   128,   162,   163,   131,   183,   354,   165,   133,
     166,   167,   136,   137,   266,   138,   139,   140,   141,   142
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     145,   580,    89,   657,   164,   428,   664,   169,   213,   214,
     168,   383,   174,   293,   344,   796,   177,   177,   187,   188,
     192,   193,   217,   328,     5,   355,   574,   499,   824,   204,
     205,   206,   887,   207,   201,   208,   209,     5,     5,   244,
     554,     5,   383,   218,   222,   224,   226,   227,   228,   374,
     548,   688,   232,   233,   236,   240,   264,     5,   201,   243,
     245,   246,   247,   855,   694,   248,   391,   534,   249,   250,
     251,   833,   252,   652,   365,   218,   222,   265,   918,   143,
     430,   834,   567,  -142,   505,   401,   260,   440,   441,   442,
     443,    55,   305,    55,   225,   307,   273,   365,   383,   310,
     223,   965,     3,   695,   953,   868,  -316,   753,   439,   418,
     534,   431,   425,   378,   798,  -327,  -327,   274,  -316,   977,
     870,   955,  -142,   607,   340,   339,   754,   186,   328,  -316,
     910,   438,   263,   384,  -327,   386,   419,   886,  -327,  -327,
     145,   336,   337,   338,   232,   218,   265,   566,   298,   966,
     505,     5,   706,   393,   888,   340,   146,   696,   824,   971,
     -74,   608,   343,  -327,   384,   222,   340,   426,   340,   918,
     581,   365,   609,   379,  -316,   350,  -142,   170,   461,   340,
     795,  -316,   909,   222,   485,   486,   197,   197,   340,   610,
     699,  -327,   299,   350,   340,   426,   300,   426,   506,    16,
     222,   177,  -327,   717,   426,   176,   679,   627,   395,   396,
     350,     5,   -74,   936,   937,   674,   687,   346,   387,   222,
     384,  -327,   499,   359,   362,   366,   427,   194,   799,   350,
     718,   421,   298,   426,   -74,   956,   347,   950,   462,   628,
    -327,   389,   423,   305,   390,   306,   307,   675,   420,   614,
     310,    53,   629,   393,   143,   576,   417,   317,   393,   851,
     422,   968,   415,   399,   970,   222,   323,   659,   662,   210,
     975,   424,   394,    69,   331,   350,   299,   410,   332,   211,
     300,   630,   222,   413,   445,   446,   447,   448,   450,   452,
     453,   454,   455,   456,   459,   460,   393,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   222,   222,   222,   222,   222,   222,   222,
     488,   490,   498,   499,   387,   650,   519,   305,   350,   350,
     307,   508,   707,   709,   310,   511,  -351,   512,   444,   677,
     791,   229,   387,   615,   387,   387,  -286,   218,   781,   521,
     523,   548,   525,   527,   269,  -351,   277,   533,   533,  -351,
     537,   616,   539,   617,   618,  -286,   387,  -317,   678,   480,
     481,   482,   483,   484,  -328,   569,  -420,   782,   143,  -317,
     509,   230,   573,  -309,  -351,   752,   884,   633,   823,   222,
    -317,   332,   572,   874,   278,  -309,   276,   449,   451,   350,
     533,   533,   601,   681,   529,   279,  -309,   573,  -328,  -311,
     222,   329,   222,   330,   592,   573,   595,   572,   597,   599,
     260,  -311,   260,   218,  -437,   572,   703,  -419,   280,   487,
     489,  -328,  -311,   294,  -436,  -317,   235,   239,  -328,   202,
     507,   682,  -317,   677,   295,  -424,   164,   243,  -343,  -425,
    -422,  -309,   683,  -437,  -421,   333,   202,  -328,  -309,   522,
     335,   307,   526,  -436,   234,   238,   263,  -343,   263,   684,
    -423,  -343,   938,   340,   345,   499,  -328,  -311,  -474,  -474,
    -474,  -474,   348,  -328,  -311,   603,  -328,     5,   358,   521,
     636,   525,   639,   537,   641,   595,  -343,  -473,   376,  -473,
     642,   643,   222,   750,   222,   548,   588,   649,   590,   651,
     397,   400,   645,   656,   350,   407,   409,   498,   823,   432,
     433,   121,   298,   591,   189,   434,   436,   510,   513,   515,
     121,   514,   690,   516,   518,    27,   212,   528,   538,   540,
     555,   121,   556,   121,    35,   567,   164,  -170,   208,   227,
     228,   232,   243,   265,   571,   867,   222,   577,   646,   653,
     655,    45,   875,   658,   661,   578,   299,   341,  -170,   582,
     300,   602,   596,   613,   619,   620,   621,   622,   623,   624,
     625,   626,   121,   665,   -58,   202,   202,   202,   669,   666,
     668,   686,   372,   693,  -241,   704,   698,   705,   712,   635,
     393,   638,   715,   202,   716,   121,   202,   719,   758,   121,
     761,   763,   692,   302,   767,   541,   304,   305,   498,   306,
     307,   450,   488,   757,   310,   202,   760,   541,   788,   792,
     797,   317,   542,   794,   804,   543,   809,   813,   321,   322,
     323,   771,   818,   773,   542,   202,   776,   543,  -380,   779,
     544,   933,   541,   545,   783,    48,   784,   830,   853,   786,
     831,   871,   544,   802,   546,   545,   121,   267,   856,   542,
     878,   901,   543,   934,   954,   817,   546,   584,   967,   765,
     766,   547,   583,   957,   218,   171,   768,   544,   298,   803,
     545,   405,    48,   547,   648,   808,   777,   812,   356,   780,
     814,   546,   844,   826,   747,   825,   892,   790,   894,   676,
     973,   827,   949,   963,   121,   191,   570,   178,   547,   954,
     121,   893,   647,     0,   969,     0,     0,     0,     0,     0,
     449,   487,   299,   976,   829,     0,   300,   954,     0,     0,
       0,     0,   202,  -241,     0,     0,     0,   771,   773,     0,
     776,   779,   808,   812,     0,     0,     0,     0,   845,   846,
       0,   847,     0,   848,   849,     0,   202,   850,     0,     0,
     498,     0,     0,     0,     0,   121,     0,     0,   213,   214,
       0,     0,     0,   305,     0,   306,   307,   873,   595,     0,
     310,   865,     0,   879,     0,   880,     0,   317,     0,   882,
       0,     0,     0,     0,   321,   322,   323,     0,   177,     0,
     281,   282,   283,   284,   285,   286,     0,   417,   287,   288,
     289,   290,   291,   292,   845,   895,   896,   848,   897,   898,
     899,   900,   912,     0,     0,     0,     0,     0,     0,   877,
       0,   907,     0,     0,     0,     0,   164,     0,   208,   227,
     228,   232,   243,   265,     0,     0,     0,   915,     0,   121,
     916,     0,     0,     0,     0,     0,     0,   913,     0,     0,
    -338,     0,     0,     0,     0,     0,     0,     0,   122,   121,
       0,     0,   121,     0,     0,     0,     0,   122,     0,     0,
     121,     0,   945,   946,   947,   948,     0,     0,   122,   962,
     122,     0,     0,     0,  -338,     0,     0,     0,     0,   812,
     951,   958,   959,   164,     0,   208,   227,   228,   232,   243,
     265,   202,   202,     0,  -340,   202,   202,  -338,   945,   946,
     947,   948,   958,   959,  -338,     0,     0,     0,   673,   122,
       0,   122,   122,     0,   964,   595,   121,     0,     0,     0,
       0,     0,     0,  -338,     0,   951,     0,     0,  -340,   595,
       0,     0,   122,   812,     0,   298,   122,     0,     0,     0,
       0,     0,  -338,   951,   189,     0,     0,   700,     0,  -338,
       0,  -340,  -338,     0,     0,    27,   212,     0,  -340,     0,
       0,     0,     5,     0,    35,     0,     0,  -170,     0,   721,
       0,     0,     0,   -44,     0,   132,   722,  -340,     0,   299,
       0,    45,     0,   300,   132,     0,   631,     0,  -170,   -44,
       0,     0,     0,   122,     0,   132,  -340,   132,     0,   114,
       0,     0,     0,  -340,     0,     0,  -340,     0,   114,     0,
       0,   202,   202,     0,     0,     0,     0,     0,   202,   114,
     372,   114,     0,     0,     0,   372,   302,   121,   202,     0,
     305,   202,   306,   307,     0,     0,   132,   310,   237,   237,
       0,   122,     0,     0,   317,     0,     0,   122,     0,     0,
       0,   321,   322,   323,     0,     0,     0,     0,     0,   132,
     114,     0,     0,   132,   281,   282,   283,   284,   285,   286,
       0,   805,   287,   288,   289,   290,   291,   292,   723,   724,
     725,   726,     0,   114,   727,   121,     0,   114,   728,   729,
     730,   731,   732,   733,   734,   735,   736,   737,   738,     0,
     739,   751,   122,   740,   741,   742,   743,     0,   744,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     132,   755,   756,   281,   282,   283,   284,   285,   286,   324,
     325,   287,   288,   289,   290,   291,   292,     0,     0,     0,
     189,   121,     0,   121,   114,     0,   121,     0,     0,   121,
       0,    27,   212,     0,     0,     0,     0,     0,   121,     0,
      35,   121,     0,  -170,     0,   326,   327,  -444,   132,   -43,
    -473,   202,  -473,     0,   132,     0,     0,    45,     0,     0,
       0,     0,     0,   121,  -170,   -43,   122,     0,     0,   713,
       0,     0,   114,     0,     0,   121,     0,   121,   114,     0,
       0,   121,     0,     0,     0,     0,   122,  -156,     0,   122,
       0,  -156,  -156,     0,     0,   372,   372,   122,   372,   372,
    -156,     0,  -156,  -156,     0,     0,  -156,     0,     0,   132,
       0,  -156,     0,     0,  -156,     0,     0,     0,     0,     0,
       0,   917,     0,     0,     0,     0,     0,   921,  -156,   372,
    -156,   372,  -156,   114,  -156,  -156,     0,  -156,   121,  -156,
       0,  -156,     0,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,     0,     0,     0,   121,  -156,     0,
       0,  -156,     0,     0,  -156,     0,     0,     0,     0,   121,
       0,     0,     0,   843,     0,     0,     0,     0,   121,     0,
       0,     0,   121,   126,     0,     0,   121,     0,     0,     0,
       0,  -156,   126,   132,     0,  -156,  -156,     0,     0,     0,
       0,     0,     0,   126,  -156,   126,  -156,  -156,     0,     0,
    -156,  -156,   917,   132,     0,  -156,   132,   114,  -156,  -156,
       0,     0,   121,     0,   132,     0,     0,     0,     0,     0,
       0,     0,  -156,     0,  -156,   296,  -156,   114,  -156,  -156,
     114,  -156,   297,  -156,   126,  -156,     0,     0,   114,     0,
       0,     0,     0,   298,   660,     0,     0,   129,     0,     0,
       0,     0,  -156,     0,   122,  -156,   129,   126,  -156,     0,
       0,   126,   121,   121,   121,   121,     0,   129,     0,   129,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,   121,     0,     0,     0,     0,   299,     0,     0,
       0,   300,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -156,     0,     0,   129,     0,
       0,     0,   122,  -156,     0,   365,     0,     0,   126,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   129,     0,     0,   302,   129,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,     0,   321,
     322,   323,     0,     0,     0,   340,   126,     0,   122,     0,
     122,     0,   126,   122,     0,     0,   122,     0,     0,     0,
       0,   132,     0,     0,     0,   122,     0,     0,   122,     0,
       0,     0,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,     0,     0,     0,     0,
     122,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   122,     0,   122,     0,     0,   126,   122,     0,
       0,     0,     0,     0,    16,     0,     0,     0,    20,   132,
     129,     0,     0,     0,     0,     0,   129,    26,     0,    27,
     819,     0,     0,    31,     0,     0,     0,     0,    35,     0,
       0,  -170,     0,   114,     0,     0,     0,     0,     0,   130,
       0,     0,     0,     0,     0,    45,     0,     0,   130,    48,
       0,   820,  -170,     0,   821,   122,    53,     0,     0,   130,
     122,   130,     0,     0,     0,   132,     0,   132,     0,     0,
     132,   129,     0,   132,   122,    67,     0,     0,    69,     0,
       0,   126,   132,     0,     0,   132,   122,     0,     0,   114,
       0,   114,     0,     0,   114,   122,     0,   114,     0,   122,
     130,   126,     0,   122,   126,     0,   114,   132,     0,   114,
       0,     0,   126,     0,     0,     0,     0,     0,     0,   132,
       0,   132,     0,   130,     0,   132,     0,   130,    84,     0,
       0,   114,     0,     0,     0,     0,   822,     0,     0,   122,
       0,     0,     0,   114,     0,   114,     0,     0,     0,   114,
       0,     0,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   126,     0,
       0,     0,     0,     0,     0,   129,     0,     0,   129,     0,
       0,     0,   132,     0,   130,     0,   129,   132,     0,   122,
     122,   122,   122,     0,     0,     0,     0,     0,     0,     0,
       0,   132,     0,     0,     0,     0,   114,     0,   122,   122,
       0,   114,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,   132,     0,     0,   114,   132,   134,     0,     0,
     132,     0,   130,     0,     0,     0,   134,   114,   130,     0,
       0,     0,   129,     0,     0,     0,   114,   134,     0,   134,
     114,     0,     0,     0,   114,     0,    16,     0,     0,     0,
      20,     0,     0,     0,     0,     0,   132,     0,     0,    26,
       0,    27,   819,     0,     0,    31,     0,     0,     0,   126,
      35,     0,     0,  -170,     0,     0,     0,     0,   134,     0,
     114,     0,     0,   130,     0,     0,     0,    45,     0,     0,
       0,    48,     0,   820,  -170,     0,   821,     0,    53,     0,
       0,   134,     0,     0,     0,   134,   132,   132,   132,   132,
       0,     0,     0,     0,     0,     0,     0,    67,     0,     0,
      69,     0,     0,     0,     0,   132,   132,   126,     0,     0,
     114,   114,   114,   114,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   129,     0,     0,     0,     0,     0,   114,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,     0,     0,     0,     0,   130,     0,     0,
      84,     0,     0,     0,     0,     0,     0,     0,   902,     0,
       0,     0,     0,   126,    91,   126,     0,   130,   126,     0,
     130,   126,     0,   144,     0,     0,     0,     0,   130,     0,
     126,   129,     0,   126,   172,     0,   175,     0,    16,     0,
     134,     0,    20,     0,     0,     0,   134,     0,     0,     0,
       0,    26,   135,    27,   819,   126,     0,    31,     0,     0,
       0,   135,    35,     0,     0,  -170,     0,   126,     0,   126,
       0,     0,   135,   126,   135,   231,     0,     0,     0,    45,
       0,     0,     0,   267,   130,   820,  -170,   129,   821,   129,
      53,     0,   129,     0,     0,   129,     0,     0,    91,     0,
       0,   134,   268,     0,   129,     0,     0,   129,     0,    67,
       0,     0,    69,   135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,     0,     0,   129,
     126,     0,     0,     0,     0,   126,   135,     0,     0,     0,
     135,   129,     0,   129,     0,     0,     0,   129,     0,   126,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
       0,   126,    84,     0,     0,     0,     0,     0,   299,     0,
     126,     0,   300,     0,   126,     0,     0,     0,   126,     0,
       0,     0,     0,   298,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   130,     0,   135,     0,     0,
       0,     0,     0,     0,   129,   134,     0,   375,   134,   129,
       0,     0,     0,   388,   126,   302,   134,   303,   304,   305,
       0,   306,   307,   129,     0,     0,   310,   299,     0,     0,
       0,   300,   316,   317,     0,   129,     0,   320,     0,     0,
     321,   322,   323,     0,   129,   135,     0,     0,   129,     0,
       0,   135,   129,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   126,   126,   126,   126,    91,     0,
       0,     0,   134,     0,   302,     0,   303,   304,   305,     0,
     306,   307,   308,   126,   126,   310,   311,   312,   129,   314,
     315,   316,   317,     0,     0,     0,   320,     0,   298,   321,
     322,   323,     0,     0,     0,     0,   135,     0,     0,   130,
       0,   130,     0,     0,   130,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,   130,     0,     0,   130,
       0,     0,     0,     0,     0,     0,     0,     0,   129,   129,
     129,   129,   299,     0,     0,     0,   300,     0,     0,     0,
       0,   130,    91,     0,     0,     0,     0,   129,   129,     0,
       0,     0,     0,   130,     0,   130,     0,     0,     0,   130,
       0,     0,   388,     0,   301,   388,     0,     0,     0,     0,
       0,     0,     0,   134,     0,     0,     0,     0,     0,   302,
     135,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,     0,   314,   315,   316,   317,     0,   318,
     135,   320,     0,   135,   321,   322,   323,     0,     0,     0,
       0,   135,     0,     0,     0,     0,   130,     0,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,   594,
       0,   134,     0,     0,     0,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,   130,     0,     0,     0,
     130,     0,     0,     0,   130,     0,   298,   135,     0,   281,
     282,   283,   284,   285,   286,   324,   325,   287,   288,   289,
     290,   291,   292,     0,     0,     0,     0,   134,     0,   134,
       0,     0,   134,     0,   298,   134,     0,     0,     0,     0,
     130,   150,     0,     0,   134,     0,     0,   134,     0,   150,
     299,   326,   327,     0,   300,     0,  -473,     0,  -473,     0,
       0,     0,     0,     0,     0,   195,     0,   198,     0,   134,
       0,     0,     0,     0,     0,     0,     0,     0,   299,     0,
     671,   134,   300,   134,     0,     0,     0,   134,     0,     0,
     130,   130,   130,   130,     0,   241,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,     0,   310,   130,
     130,     0,     0,     0,   316,   317,     0,     0,   135,   320,
       0,     0,   321,   322,   323,   302,     0,   303,   304,   305,
       0,   306,   307,     0,     0,     0,   310,     0,   710,   271,
       0,   272,     0,   317,   134,     0,     0,     0,     0,   134,
     321,   322,   323,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,     0,     0,     0,
       0,     0,     0,     0,   134,     0,     0,     0,   134,     0,
       0,     0,   134,     0,   388,     0,   388,     0,   241,   388,
       0,     0,   388,     0,   349,     0,     0,     0,     0,   241,
       0,   785,   357,     0,   787,     0,     0,     0,     0,     0,
     298,     0,   349,     0,     0,   380,     0,     0,   134,     0,
       0,     0,   135,     0,   135,     0,   801,   135,     0,   349,
     135,     0,     0,   392,     0,     0,     0,     0,   807,   135,
     811,     0,   135,     0,    91,     0,     0,     0,   349,     0,
       0,     0,     0,     0,   299,     0,     0,     0,   300,     0,
       0,     0,     0,     0,   135,     0,     0,     0,   134,   134,
     134,   134,     0,     0,     0,     0,   135,     0,   135,     0,
       0,     0,   135,     0,     0,     0,   301,   134,   134,     0,
       0,     0,     0,     0,   349,   198,   198,   198,   198,     0,
       0,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,     0,   314,   315,   316,   317,
     854,   318,   319,   320,     0,   463,   321,   322,   323,     0,
       0,     0,   876,     0,     0,     0,     0,     0,     0,   135,
       0,   881,     0,     0,   135,   883,     0,   349,   349,    91,
       0,     0,     0,     0,     0,     0,     0,     0,   135,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     135,     0,     0,     0,   517,     0,     0,     0,     0,   135,
       0,     0,     0,   135,     0,   908,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,   198,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,     0,   135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   600,     0,     0,   604,     0,     0,   198,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   135,   135,   135,   135,     0,     0,     0,
       0,     0,     0,     0,   960,     0,     5,     6,     7,     8,
       9,    10,   135,   135,     0,   151,     0,     0,    14,    15,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,   349,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
     296,     0,     0,    52,     0,     0,     0,   297,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,   298,   680,
       0,     0,     0,   380,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,   701,     0,     0,     0,
     241,     0,    75,    76,    77,    78,     0,     0,     0,     0,
      79,     0,   299,     0,     0,     0,   300,     0,     0,    80,
       0,    81,     0,    82,    83,   215,     0,     0,     0,    85,
     746,     0,     0,   156,     0,    87,     0,   157,   961,     0,
     365,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,     0,   321,   322,   323,     0,     0,     0,
       0,     0,     0,     0,   789,   708,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   701,     0,     0,
       0,     0,     0,     0,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   604,    11,    12,    13,    14,
      15,     0,   828,    16,    17,    18,    19,    20,  -293,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -293,    47,    48,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -293,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -293,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,   891,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,  -492,     0,    11,    12,
      13,    14,    15,  -492,     0,    16,    17,    18,    19,    20,
    -293,    21,    22,    23,  -492,    24,    25,  -492,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -293,    47,
       0,    49,    50,  -170,   -42,    51,    52,    53,  -492,  -293,
      54,    55,  -492,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -293,     0,     0,    72,     0,    73,     0,     0,
    -492,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -492,    76,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,     0,  -492,  -492,  -492,  -492,  -492,
    -492,  -492,  -492,  -492,    81,  -492,  -492,  -492,     0,    84,
    -492,  -492,  -492,     0,     0,     0,    86,  -492,    87,   253,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -293,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -293,    47,    48,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -293,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -293,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
     215,    84,     0,     0,    85,     0,     0,     0,    86,   254,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -293,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -293,    47,    48,    49,    50,  -170,   -42,    51,
      52,    53,     0,  -293,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -293,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,   416,    87,   253,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -293,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,    32,    33,     0,    34,    35,     0,    36,  -170,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -293,    47,    48,    49,    50,  -170,
     -42,    51,    52,    53,     0,  -293,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -293,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,   254,    87,   815,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -293,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -293,    47,    48,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -293,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -293,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,   816,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -293,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -293,    47,
      48,    49,    50,  -170,   -42,    51,    52,    53,     0,  -293,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -293,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,   885,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -293,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -293,    47,   267,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -293,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -293,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -293,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -293,    47,     0,    49,    50,  -170,   -42,    51,
      52,    53,     0,  -293,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -293,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -293,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   520,    33,     0,    34,    35,     0,    36,  -170,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -293,    47,     0,    49,    50,  -170,
     -42,    51,    52,    53,     0,  -293,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -293,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,     0,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -293,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,   524,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -293,    47,     0,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -293,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -293,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -293,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,   770,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -293,    47,
       0,    49,    50,  -170,   -42,    51,    52,    53,     0,  -293,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -293,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,     0,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -293,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   772,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -293,    47,     0,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -293,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -293,     0,     0,    72,     0,    73,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,     0,     0,     0,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,    81,     0,    82,    83,
       0,    84,     0,     0,    85,     0,     0,     0,    86,     0,
      87,     4,    88,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,     0,    16,
      17,    18,    19,    20,  -293,    21,    22,    23,     0,    24,
      25,     0,    26,     0,    27,    28,    29,    30,    31,   775,
      33,     0,    34,    35,     0,    36,  -170,    37,    38,    39,
      40,    41,   -42,    42,    43,     0,    44,     0,     0,     0,
      45,    46,  -293,    47,     0,    49,    50,  -170,   -42,    51,
      52,    53,     0,  -293,    54,    55,     0,    56,    57,    58,
      59,    60,    61,    62,    63,     0,    64,     0,    65,    66,
      67,    68,     0,    69,    70,    71,  -293,     0,     0,    72,
       0,    73,     0,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,    81,     0,
      82,    83,     0,    84,     0,     0,    85,     0,     0,     0,
      86,     0,    87,     4,    88,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,    14,    15,     0,
       0,    16,    17,    18,    19,    20,  -293,    21,    22,    23,
       0,    24,    25,     0,    26,     0,    27,    28,    29,    30,
      31,   778,    33,     0,    34,    35,     0,    36,  -170,    37,
      38,    39,    40,    41,   -42,    42,    43,     0,    44,     0,
       0,     0,    45,    46,  -293,    47,     0,    49,    50,  -170,
     -42,    51,    52,    53,     0,  -293,    54,    55,     0,    56,
      57,    58,    59,    60,    61,    62,    63,     0,    64,     0,
      65,    66,    67,    68,     0,    69,    70,    71,  -293,     0,
       0,    72,     0,    73,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,     0,    80,     0,
      81,     0,    82,    83,     0,    84,     0,     0,    85,     0,
       0,     0,    86,     0,    87,     4,    88,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,     0,    16,    17,    18,    19,    20,  -293,    21,
      22,    23,     0,   800,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,     0,    36,
    -170,    37,    38,    39,    40,    41,   -42,    42,    43,     0,
      44,     0,     0,     0,    45,    46,  -293,    47,     0,    49,
      50,  -170,   -42,    51,    52,    53,     0,  -293,    54,    55,
       0,    56,    57,    58,    59,    60,    61,    62,    63,     0,
      64,     0,    65,    66,    67,    68,     0,    69,    70,    71,
    -293,     0,     0,    72,     0,    73,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,     0,     0,     0,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,    81,     0,    82,    83,     0,    84,     0,     0,
      85,     0,     0,     0,    86,     0,    87,     4,    88,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,    14,    15,     0,     0,    16,    17,    18,    19,    20,
    -293,    21,    22,    23,     0,    24,    25,     0,    26,     0,
      27,    28,    29,    30,    31,   806,    33,     0,    34,    35,
       0,    36,  -170,    37,    38,    39,    40,    41,   -42,    42,
      43,     0,    44,     0,     0,     0,    45,    46,  -293,    47,
       0,    49,    50,  -170,   -42,    51,    52,    53,     0,  -293,
      54,    55,     0,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,     0,    65,    66,    67,    68,     0,    69,
      70,    71,  -293,     0,     0,    72,     0,    73,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,     0,
       0,     0,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,    80,     0,    81,     0,    82,    83,     0,    84,
       0,     0,    85,     0,     0,     0,    86,     0,    87,     4,
      88,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,    14,    15,     0,     0,    16,    17,    18,
      19,    20,  -293,    21,    22,    23,     0,    24,    25,     0,
      26,     0,    27,    28,    29,    30,    31,   810,    33,     0,
      34,    35,     0,    36,  -170,    37,    38,    39,    40,    41,
     -42,    42,    43,     0,    44,     0,     0,     0,    45,    46,
    -293,    47,     0,    49,    50,  -170,   -42,    51,    52,    53,
       0,  -293,    54,    55,     0,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    64,     0,    65,    66,    67,    68,
       0,    69,    70,    71,  -293,     0,     0,    72,     0,    73,
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
     911,     5,     6,     7,     8,     9,    10,     0,     0,     0,
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
     154,    33,     0,    34,     0,     0,    36,  -464,     0,    38,
      39,     0,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,    46,     0,    47,     0,    49,     0,  -464,     0,
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
     153,     0,   154,    33,     0,    34,     0,     0,    36,  -464,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
    -464,     0,     0,    52,   -73,     0,     0,     0,     0,     0,
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
     153,     0,   154,    33,     0,    34,     0,     0,    36,  -464,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
    -464,     0,     0,    52,     0,     0,     0,     0,     0,     0,
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
      14,   857,     0,     0,    85,     0,     0,    19,   156,     0,
      87,     0,   157,     0,     0,   858,     0,     0,     0,     0,
       0,   152,   153,     0,   154,    33,     0,    34,     0,     0,
      36,     0,     0,    38,    39,     0,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,   859,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,   860,   861,    60,    61,    62,   862,
       0,     0,     0,     0,     0,     0,    68,     0,     0,   863,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,     0,     0,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,    81,     0,    82,    83,   215,     0,     0,
       0,    85,     0,     0,     0,   156,     0,    87,     0,   864,
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
       0,   864,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   151,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,   152,   153,     0,   842,    33,
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
       0,     0,     0,   152,   153,     0,   836,    33,     0,    34,
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
     152,   153,     0,   837,    33,     0,    34,     0,     0,    36,
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
     839,    33,     0,    34,     0,     0,    36,     0,     0,    38,
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
       0,     0,     0,     0,   152,   153,     0,   840,    33,     0,
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
       0,   152,   153,     0,   841,    33,     0,    34,     0,     0,
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
       0,   842,    33,     0,    34,     0,     0,    36,     0,     0,
      38,    39,     0,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,    46,     0,    47,     0,    49,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,    62,   155,     0,     0,     0,
       0,     0,     0,    68,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,     0,     0,     0,     0,    79,
       0,     0,     5,     6,     7,     8,     9,    10,    80,     0,
      81,   151,    82,    83,    14,   924,     0,     0,    85,     0,
       0,    19,   156,     0,    87,     0,   157,     0,     0,   925,
       0,     0,     0,     0,     0,   152,   153,     0,   154,    33,
       0,    34,     0,     0,    36,     0,     0,    38,    39,     0,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
     926,     0,    47,     0,    49,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,   927,   928,
      60,    61,    62,   929,     0,     0,     0,     0,     0,     0,
      68,     0,     0,   930,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,     0,     0,     0,     0,    79,     0,     0,     5,
       6,     7,     8,     9,    10,    80,     0,    81,   151,    82,
      83,    14,    15,     0,     0,    85,     0,     0,    19,   156,
       0,    87,     0,   931,     0,     0,    25,     0,     0,     0,
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
       0,     0,    85,     0,     0,    19,     0,     0,    87,     0,
     931,     0,     0,    25,     0,     0,     0,     0,     0,   152,
     153,     0,   154,    33,     0,    34,     0,     0,    36,     0,
       0,    38,    39,     0,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,    46,     0,    47,     0,    49,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,    62,   155,     0,     0,
       0,     0,     0,     0,    68,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,     0,     0,     0,     0,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    80,
       0,    81,   151,     0,    83,    14,    15,     0,     0,    85,
       0,     0,    19,   156,     0,    87,     0,   157,     0,     0,
      25,     0,     0,     0,     0,     0,   152,   153,     0,   154,
      33,     0,    34,     0,     0,    36,     0,     0,    38,    39,
       0,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,    46,     0,    47,     0,    49,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,    62,   155,     0,     0,     0,     0,     0,
       0,    68,     0,     0,    70,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,  -342,     0,   151,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
      76,     0,     0,     0,  -342,    25,     0,     0,  -342,     0,
       0,   152,   153,     0,   154,    33,     0,    34,    81,     0,
      36,     0,   215,    38,    39,     0,    41,     0,    42,    43,
     156,     0,    87,  -342,   864,     0,    46,     0,    47,     0,
      49,     0,     0,     0,     0,    52,     0,     0,     0,     0,
    -312,     0,     0,     0,    58,    59,    60,    61,    62,   155,
       0,     0,  -312,     0,     0,     0,    68,     0,     0,    70,
       0,     0,     0,  -312,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   151,     0,     0,    14,
      15,     0,     0,    81,     0,     0,    19,     0,  -312,     0,
       0,     0,     0,     0,    25,  -312,     0,    87,     0,   931,
     152,   153,     0,   154,    33,     0,    34,     0,     0,    36,
       0,     0,    38,    39,     0,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    47,     0,    49,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,   155,     0,
       0,     0,     0,     0,     0,    68,     0,     0,    70,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   151,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,    76,     0,    25,     0,     0,     0,
       0,     0,   152,   153,     0,   154,    33,     0,    34,     0,
       0,    36,    81,     0,    38,    39,     0,    41,     0,    42,
      43,     0,     0,     0,   156,     0,    87,    46,   157,    47,
       0,    49,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,    62,
     155,     0,     0,     0,     0,     0,     0,    68,     0,     0,
      70,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     151,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,    76,     0,    25,     0,
       0,     0,     0,     0,   152,   153,     0,   154,    33,     0,
      34,     0,     0,    36,    81,     0,    38,    39,     0,    41,
       0,    42,    43,     0,     0,     0,   156,     0,    87,    46,
     564,    47,     0,    49,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,    62,   155,     0,     5,     0,     0,     0,     0,    68,
       0,   721,    70,     0,     0,     0,    14,     0,   722,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    76,     0,
       0,    33,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    81,     0,     0,     0,
       0,     0,   296,     0,     0,     0,     0,     0,     0,   297,
      87,    52,   157,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   281,   282,   283,   284,
     285,   286,     0,     0,   287,   288,   289,   290,   291,   292,
     723,   724,   725,   726,   299,     0,   727,     0,   300,     0,
     728,   729,   730,   731,   732,   733,   734,   735,   736,   737,
     738,     0,   739,     0,   296,   740,   741,   742,   743,     0,
     744,   297,     0,   745,     0,     0,   301,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,     0,     0,     0,
       0,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,     0,   321,   322,   323,     0,
       0,     0,     0,     0,   298,     0,   299,   764,     0,     0,
     300,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   299,     0,
       0,     0,   300,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,     0,     0,     0,     0,   296,     0,     0,   920,
       0,     0,     0,   297,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,   298,   309,   310,   311,   312,   301,
     314,   315,   316,   317,     0,   318,     0,   320,     0,     0,
     321,   322,   323,     0,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,     0,     0,   296,     0,     0,
     974,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,     0,     0,     0,     0,   296,   586,
       0,     0,     0,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,     0,     0,   296,
     890,     0,     0,   371,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   301,   281,   282,   283,   284,   285,   286,     0,
       0,   287,   288,   289,   290,   291,   292,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,   340,     0,
     296,     0,     0,     0,   769,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   301,   281,   282,   283,   284,   285,   286,
       0,     0,   287,   288,   289,   290,   291,   292,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   299,   321,   322,   323,   300,     0,     0,   340,
       0,   296,     0,     0,     0,   774,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,   301,   281,   282,   283,   284,   285,
     286,     0,     0,   287,   288,   289,   290,   291,   292,   302,
       0,   303,   304,   305,     0,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   299,   321,   322,   323,   300,     0,     0,
     340,     0,   296,     0,     0,     0,   903,     0,     0,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,   301,   281,   282,   283,   284,
     285,   286,     0,     0,   287,   288,   289,   290,   291,   292,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   299,   321,   322,   323,   300,     0,
       0,   340,     0,   296,     0,     0,     0,   904,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,     0,   301,   281,   282,   283,
     284,   285,   286,     0,     0,   287,   288,   289,   290,   291,
     292,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   299,   321,   322,   323,   300,
       0,     0,   340,     0,   296,     0,     0,     0,   905,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,   301,   281,   282,
     283,   284,   285,   286,     0,     0,   287,   288,   289,   290,
     291,   292,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   299,   321,   322,   323,
     300,     0,     0,   340,     0,   296,     0,     0,     0,   906,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   301,   281,
     282,   283,   284,   285,   286,     0,     0,   287,   288,   289,
     290,   291,   292,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,     0,     0,   340,     0,   296,     0,     0,     0,
     922,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,     0,     0,     0,   301,
     281,   282,   283,   284,   285,   286,     0,     0,   287,   288,
     289,   290,   291,   292,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,   340,     0,   296,     0,     0,
       0,   923,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,   363,     0,     0,     0,
     301,   281,   282,   283,   284,   285,   286,     0,   364,   287,
     288,   289,   290,   291,   292,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,   296,     0,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,   365,     0,     0,
       0,   301,   298,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
       0,   321,   322,   323,     0,     0,   299,   340,     0,     0,
     300,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   201,     0,     0,     0,   301,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,   303,   304,   305,     0,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   299,   321,   322,
     323,   300,     0,     0,   340,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,   654,     0,     0,     0,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,     0,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   299,   321,
     322,   323,   300,     0,     0,   340,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     301,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,   299,
     321,   322,   323,   300,     0,     0,   340,     0,   296,     0,
       0,     0,   672,     0,     0,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   298,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     299,   321,   322,   323,   300,     0,     0,   340,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,     0,   303,
     304,   305,     0,   306,   307,   308,     0,   309,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   318,   319,   320,
       0,   299,   321,   322,   323,   300,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     296,     0,     0,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,   301,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   368,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   369,   318,   319,
     320,     0,   296,   321,   322,   323,     0,     0,  -445,   297,
       0,     0,   299,     0,     0,     0,   300,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   342,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
       0,   303,   304,   305,   299,   306,   307,   308,   300,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   370,   321,   322,   323,   296,     0,     0,
       0,     0,   146,     0,   297,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
     411,   302,     0,   303,   304,   305,     0,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,   412,     0,   321,   322,   323,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,   500,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   503,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,   402,   321,   322,   323,     0,   296,     0,     0,
       0,     0,     0,     0,   297,   301,     0,   299,     0,     0,
       0,   300,     0,     0,     0,   298,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   301,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   299,
     306,   307,   308,   300,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,   296,   404,   321,
     322,   323,     0,     0,   297,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,   298,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
     406,   321,   322,   323,     0,     0,   297,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
     414,   321,   322,   323,     0,     0,   297,     0,     0,     0,
       0,     0,   429,   301,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   296,     0,   321,   322,   323,     0,     0,   297,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   298,
       0,     0,     0,     0,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,     0,     0,     0,     0,
       0,     0,   297,   301,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,     0,     0,     0,     0,   302,     0,
     303,   304,   305,   299,   306,   307,   308,   300,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,   296,   667,     0,
       0,   146,     0,     0,   297,   301,     0,   299,     0,     0,
       0,   300,     0,     0,     0,   298,     0,     0,     0,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   301,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
       0,     0,     0,     0,   302,     0,   303,   304,   305,   299,
     306,   307,   308,   300,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,   296,   644,   321,
     322,   323,     0,     0,   297,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,   298,     0,     0,   670,     0,
       0,     0,     0,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,   296,
       0,   321,   322,   323,     0,     0,   297,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,   301,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   759,     0,     0,     0,   302,     0,   303,   304,
     305,   299,   306,   307,   308,   300,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
       0,   321,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,   711,   301,     0,   299,     0,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   301,   318,   319,
     320,     0,     0,   321,   322,   323,     0,     0,     0,     0,
       0,     0,   302,     0,   303,   304,   305,     0,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,   296,     0,   321,   322,   323,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   298,   762,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,     0,   299,     0,     0,
       0,   300,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
     297,     0,     0,     0,     0,     0,     0,     0,     0,   301,
       0,   298,     0,     0,     0,     0,   299,     0,     0,     0,
     300,     0,     0,     0,   302,     0,   303,   304,   305,   835,
     306,   307,   308,     0,   309,   310,   311,   312,   313,   314,
     315,   316,   317,     0,   318,   319,   320,     0,   301,   321,
     322,   323,     0,     0,     0,   299,     0,     0,     0,   300,
       0,     0,     0,   302,     0,   303,   304,   305,   838,   306,
     307,   308,     0,   309,   310,   311,   312,   313,   314,   315,
     316,   317,     0,   318,   319,   320,     0,   301,   321,   322,
     323,     0,     0,   296,   852,     0,     0,     0,     0,     0,
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
       0,     0,   302,     0,   303,   304,   305,   939,   306,   307,
     308,     0,   309,   310,   311,   312,   313,   314,   315,   316,
     317,     0,   318,   319,   320,     0,   301,   321,   322,   323,
       0,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,   302,     0,   303,   304,   305,   940,   306,   307,   308,
       0,   309,   310,   311,   312,   313,   314,   315,   316,   317,
       0,   318,   319,   320,     0,   301,   321,   322,   323,     0,
       0,   296,     0,     0,     0,     0,     0,     0,   297,     0,
     302,     0,   303,   304,   305,     0,   306,   307,   308,   298,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,     0,   321,   322,   323,     0,     0,
     296,     0,     0,     0,     0,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   298,     0,
       0,     0,     0,   299,     0,     0,     0,   300,     0,     0,
       0,     0,     0,     0,     0,     0,   941,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,     0,     0,
       0,     0,     0,     0,     0,   301,     0,   298,     0,     0,
       0,     0,   299,     0,     0,     0,   300,     0,     0,     0,
     302,     0,   303,   304,   305,   942,   306,   307,   308,     0,
     309,   310,   311,   312,   313,   314,   315,   316,   317,     0,
     318,   319,   320,     0,   301,   321,   322,   323,     0,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,   302,
       0,   303,   304,   305,   943,   306,   307,   308,     0,   309,
     310,   311,   312,   313,   314,   315,   316,   317,     0,   318,
     319,   320,     0,   301,   321,   322,   323,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   297,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,   298,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,     0,     0,   321,   322,   323,     0,   296,     0,     0,
       0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   298,     0,     0,     0,     0,
       0,   299,     0,     0,     0,   300,     0,     0,     0,     0,
       0,     0,     0,     0,   944,     0,   296,     0,     0,     0,
       0,     0,     0,   297,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,   298,     0,     0,     0,     0,   299,
       0,     0,     0,   300,     0,     0,     0,     0,   302,     0,
     303,   304,   305,     0,   306,   307,   308,     0,   309,   310,
     311,   312,   313,   314,   315,   316,   317,     0,   318,   319,
     320,   301,     0,   321,   322,   323,     0,     0,   632,     0,
       0,     0,   300,     0,     0,     0,   302,     0,   303,   304,
     305,     0,   306,   307,   308,     0,   309,   310,   311,   312,
     313,   314,   315,   316,   317,     0,   318,   319,   320,     0,
     301,   321,   322,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   302,     0,   303,   304,   305,
       0,   306,   307,   308,     0,   309,   310,   311,   312,   313,
     314,   315,   316,   317,     0,   318,   319,   320,     0,     0,
     321,   322,   323
};

static const yytype_int16 yycheck[] =
{
      11,   407,     2,    29,    15,   264,   528,    18,    50,    51,
      17,     1,    23,   122,   170,   688,    27,    28,    29,    30,
      31,    32,    54,   132,     3,   181,     1,   333,   714,    40,
      41,    42,     1,    44,    29,    46,    47,     3,     3,    71,
     379,     3,     1,    54,    55,    56,    57,    58,    59,   194,
     376,   556,    63,    64,    65,    66,    88,     3,    29,    70,
      71,    72,    73,   789,    58,    76,   211,   369,    79,    80,
      81,   750,    83,    29,   100,    86,    87,    88,   872,   143,
     124,   754,   105,    58,    41,   230,    87,   277,   278,   279,
     280,    75,   123,    75,    57,   126,    46,   100,     1,   130,
      55,   956,     0,    97,   911,   128,    75,   629,    73,   124,
     412,   155,    41,    92,    41,   105,    75,    67,    87,   974,
     793,    41,    97,    24,   150,   157,   631,    29,   237,    98,
     856,   276,    87,   123,   124,    41,   151,   823,    41,    98,
     151,   152,   153,   154,   155,   156,   157,   384,    28,   956,
      41,     3,   155,   124,   123,   150,   100,   151,   844,   143,
      19,    62,   169,   153,   123,   176,   150,   124,   150,   963,
     407,   100,    73,   152,   143,   176,   151,    90,    30,   150,
     685,   150,   855,   194,   329,   330,   152,   152,   150,    90,
     152,   150,    72,   194,   150,   124,    76,   124,   155,    19,
     211,   212,   105,   124,   124,   152,   152,    41,   219,   220,
     211,     3,    71,   892,   893,    41,   555,   124,   124,   230,
     123,   124,   528,   186,   187,   188,   155,   152,   155,   230,
     151,   124,    28,   124,    93,   155,   143,   910,    90,    73,
     143,   204,   124,   123,   207,   125,   126,    73,   259,   439,
     130,    71,   124,   124,   143,   400,   256,   137,   124,   781,
     153,   961,   151,   226,   155,   276,   146,   526,   527,     7,
     970,   153,   143,    93,    72,   276,    72,   143,    76,   152,
      76,   153,   293,   246,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   124,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   629,   124,   153,   358,   123,   329,   330,
     126,   342,   591,   592,   130,   346,   105,   348,   293,   124,
     679,    83,   124,   143,   124,   124,   124,   358,   124,   360,
     361,   677,   363,   364,    68,   124,    24,   368,   369,   128,
     371,   143,   373,   143,   143,   143,   124,    75,   153,   324,
     325,   326,   327,   328,    41,   386,   143,   153,   143,    87,
     343,   152,   393,    75,   153,   143,   151,    72,   714,   400,
      98,    76,   393,   799,    62,    87,   152,   299,   300,   400,
     411,   412,   434,    24,   367,    73,    98,   418,    75,    75,
     421,   152,   423,   154,   425,   426,   427,   418,   429,   430,
     421,    87,   423,   434,   124,   426,   582,   143,    96,   331,
     332,    98,    98,   143,   124,   143,    65,    66,   105,    40,
     342,    62,   150,   124,   149,   143,   457,   458,   105,   143,
     143,   143,    73,   153,   143,   152,    57,   124,   150,   361,
     143,   126,   364,   153,    65,    66,   421,   124,   423,    90,
     143,   128,   153,   150,   143,   781,   143,   143,   152,   152,
     154,   154,    99,   150,   150,     1,   153,     3,   152,   500,
     501,   502,   503,   504,   505,   506,   153,   152,   152,   154,
     507,   508,   513,   612,   515,   831,   421,   518,   423,   520,
     143,   152,   513,   524,   515,   105,   143,   528,   844,     7,
      52,     2,    28,   425,    23,   123,   150,   143,   105,   124,
      11,   153,   564,   143,    41,    34,    35,   152,    31,   153,
     124,    22,   153,    24,    43,   105,   557,    46,   559,   560,
     561,   562,   563,   564,   153,   792,   567,   153,   513,   522,
     523,    60,   799,   526,   527,    18,    72,   168,    67,   124,
      76,   150,   155,   153,   143,   143,   143,   143,   143,   143,
     153,   155,    63,   124,   143,   186,   187,   188,    31,   143,
     143,   153,   193,   151,     3,   143,   153,   143,   150,   501,
     124,   503,   151,   204,   105,    86,   207,    73,    29,    90,
      29,   155,   567,   119,   150,    24,   122,   123,   629,   125,
     126,   632,   633,   634,   130,   226,   637,    24,    31,   123,
     155,   137,    41,    73,   123,    44,   155,   155,   144,   145,
     146,   652,   151,   654,    41,   246,   657,    44,   126,   660,
      59,   888,    24,    62,   665,    64,   667,   126,    31,   670,
     152,    98,    59,   695,    73,    62,   147,    64,   153,    41,
     153,   151,    44,    87,   911,   712,    73,   412,   155,   642,
     643,    90,   411,   920,   695,    21,   649,    59,    28,   699,
      62,   238,    64,    90,   517,   706,   659,   708,   182,   662,
     711,    73,   767,   717,   612,   716,   830,   677,   831,   548,
     963,   720,   910,   935,   195,    31,   387,    27,    90,   956,
     201,   830,   515,    -1,   961,    -1,    -1,    -1,    -1,    -1,
     632,   633,    72,   970,   745,    -1,    76,   974,    -1,    -1,
      -1,    -1,   343,   152,    -1,    -1,    -1,   758,   759,    -1,
     761,   762,   763,   764,    -1,    -1,    -1,    -1,   769,   770,
      -1,   772,    -1,   774,   775,    -1,   367,   778,    -1,    -1,
     781,    -1,    -1,    -1,    -1,   256,    -1,    -1,   820,   821,
      -1,    -1,    -1,   123,    -1,   125,   126,   798,   799,    -1,
     130,   792,    -1,   804,    -1,   806,    -1,   137,    -1,   810,
      -1,    -1,    -1,    -1,   144,   145,   146,    -1,   819,    -1,
     105,   106,   107,   108,   109,   110,    -1,   817,   113,   114,
     115,   116,   117,   118,   835,   836,   837,   838,   839,   840,
     841,   842,   864,    -1,    -1,    -1,    -1,    -1,    -1,   802,
      -1,   852,    -1,    -1,    -1,    -1,   857,    -1,   859,   860,
     861,   862,   863,   864,    -1,    -1,    -1,   868,    -1,   340,
     871,    -1,    -1,    -1,    -1,    -1,    -1,   868,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,   360,
      -1,    -1,   363,    -1,    -1,    -1,    -1,    11,    -1,    -1,
     371,    -1,   903,   904,   905,   906,    -1,    -1,    22,   931,
      24,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,   920,
     911,   922,   923,   924,    -1,   926,   927,   928,   929,   930,
     931,   522,   523,    -1,    41,   526,   527,    98,   939,   940,
     941,   942,   943,   944,   105,    -1,    -1,    -1,   539,    63,
      -1,    65,    66,    -1,   955,   956,   427,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   956,    -1,    -1,    75,   970,
      -1,    -1,    86,   974,    -1,    28,    90,    -1,    -1,    -1,
      -1,    -1,   143,   974,    23,    -1,    -1,   578,    -1,   150,
      -1,    98,   153,    -1,    -1,    34,    35,    -1,   105,    -1,
      -1,    -1,     3,    -1,    43,    -1,    -1,    46,    -1,    10,
      -1,    -1,    -1,    52,    -1,     2,    17,   124,    -1,    72,
      -1,    60,    -1,    76,    11,    -1,   497,    -1,    67,    68,
      -1,    -1,    -1,   147,    -1,    22,   143,    24,    -1,     2,
      -1,    -1,    -1,   150,    -1,    -1,   153,    -1,    11,    -1,
      -1,   642,   643,    -1,    -1,    -1,    -1,    -1,   649,    22,
     651,    24,    -1,    -1,    -1,   656,   119,   538,   659,    -1,
     123,   662,   125,   126,    -1,    -1,    63,   130,    65,    66,
      -1,   195,    -1,    -1,   137,    -1,    -1,   201,    -1,    -1,
      -1,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    86,
      63,    -1,    -1,    90,   105,   106,   107,   108,   109,   110,
      -1,   702,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    86,   125,   596,    -1,    90,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,    -1,
     141,   612,   256,   144,   145,   146,   147,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     147,   632,   633,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      23,   652,    -1,   654,   147,    -1,   657,    -1,    -1,   660,
      -1,    34,    35,    -1,    -1,    -1,    -1,    -1,   669,    -1,
      43,   672,    -1,    46,    -1,   147,   148,   149,   195,    52,
     152,   802,   154,    -1,   201,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,   694,    67,    68,   340,    -1,    -1,     1,
      -1,    -1,   195,    -1,    -1,   706,    -1,   708,   201,    -1,
      -1,   712,    -1,    -1,    -1,    -1,   360,    19,    -1,   363,
      -1,    23,    24,    -1,    -1,   846,   847,   371,   849,   850,
      32,    -1,    34,    35,    -1,    -1,    38,    -1,    -1,   256,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,   872,    -1,    -1,    -1,    -1,    -1,   878,    60,   880,
      62,   882,    64,   256,    66,    67,    -1,    69,   769,    71,
      -1,    73,    -1,   774,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   427,    -1,    -1,    -1,   788,    90,    -1,
      -1,    93,    -1,    -1,    96,    -1,    -1,    -1,    -1,   800,
      -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,   809,    -1,
      -1,    -1,   813,     2,    -1,    -1,   817,    -1,    -1,    -1,
      -1,    19,    11,   340,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    32,    24,    34,    35,    -1,    -1,
      38,   143,   963,   360,    -1,    43,   363,   340,    46,   151,
      -1,    -1,   853,    -1,   371,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    62,    10,    64,   360,    66,    67,
     363,    69,    17,    71,    63,    73,    -1,    -1,   371,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,     2,    -1,    -1,
      -1,    -1,    90,    -1,   538,    93,    11,    86,    96,    -1,
      -1,    90,   903,   904,   905,   906,    -1,    22,    -1,    24,
     427,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   922,   923,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    76,    -1,    -1,   427,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    63,    -1,
      -1,    -1,   596,   151,    -1,   100,    -1,    -1,   147,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,   119,    90,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    -1,   144,
     145,   146,    -1,    -1,    -1,   150,   195,    -1,   652,    -1,
     654,    -1,   201,   657,    -1,    -1,   660,    -1,    -1,    -1,
      -1,   538,    -1,    -1,    -1,   669,    -1,    -1,   672,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,
     694,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   706,    -1,   708,    -1,    -1,   256,   712,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    23,   596,
     195,    -1,    -1,    -1,    -1,    -1,   201,    32,    -1,    34,
      35,    -1,    -1,    38,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    46,    -1,   596,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    11,    64,
      -1,    66,    67,    -1,    69,   769,    71,    -1,    -1,    22,
     774,    24,    -1,    -1,    -1,   652,    -1,   654,    -1,    -1,
     657,   256,    -1,   660,   788,    90,    -1,    -1,    93,    -1,
      -1,   340,   669,    -1,    -1,   672,   800,    -1,    -1,   652,
      -1,   654,    -1,    -1,   657,   809,    -1,   660,    -1,   813,
      63,   360,    -1,   817,   363,    -1,   669,   694,    -1,   672,
      -1,    -1,   371,    -1,    -1,    -1,    -1,    -1,    -1,   706,
      -1,   708,    -1,    86,    -1,   712,    -1,    90,   143,    -1,
      -1,   694,    -1,    -1,    -1,    -1,   151,    -1,    -1,   853,
      -1,    -1,    -1,   706,    -1,   708,    -1,    -1,    -1,   712,
      -1,    -1,    -1,    -1,    -1,   340,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   427,    -1,
      -1,    -1,    -1,    -1,    -1,   360,    -1,    -1,   363,    -1,
      -1,    -1,   769,    -1,   147,    -1,   371,   774,    -1,   903,
     904,   905,   906,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   788,    -1,    -1,    -1,    -1,   769,    -1,   922,   923,
      -1,   774,    -1,   800,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   809,    -1,    -1,   788,   813,     2,    -1,    -1,
     817,    -1,   195,    -1,    -1,    -1,    11,   800,   201,    -1,
      -1,    -1,   427,    -1,    -1,    -1,   809,    22,    -1,    24,
     813,    -1,    -1,    -1,   817,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,   853,    -1,    -1,    32,
      -1,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,   538,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    63,    -1,
     853,    -1,    -1,   256,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    -1,    69,    -1,    71,    -1,
      -1,    86,    -1,    -1,    -1,    90,   903,   904,   905,   906,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      93,    -1,    -1,    -1,    -1,   922,   923,   596,    -1,    -1,
     903,   904,   905,   906,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,   922,
     923,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   147,    -1,    -1,    -1,    -1,   340,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,   652,     2,   654,    -1,   360,   657,    -1,
     363,   660,    -1,    11,    -1,    -1,    -1,    -1,   371,    -1,
     669,   596,    -1,   672,    22,    -1,    24,    -1,    19,    -1,
     195,    -1,    23,    -1,    -1,    -1,   201,    -1,    -1,    -1,
      -1,    32,     2,    34,    35,   694,    -1,    38,    -1,    -1,
      -1,    11,    43,    -1,    -1,    46,    -1,   706,    -1,   708,
      -1,    -1,    22,   712,    24,    63,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,   427,    66,    67,   652,    69,   654,
      71,    -1,   657,    -1,    -1,   660,    -1,    -1,    86,    -1,
      -1,   256,    90,    -1,   669,    -1,    -1,   672,    -1,    90,
      -1,    -1,    93,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   694,
     769,    -1,    -1,    -1,    -1,   774,    86,    -1,    -1,    -1,
      90,   706,    -1,   708,    -1,    -1,    -1,   712,    -1,   788,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   147,
      -1,   800,   143,    -1,    -1,    -1,    -1,    -1,    72,    -1,
     809,    -1,    76,    -1,   813,    -1,    -1,    -1,   817,    -1,
      -1,    -1,    -1,    28,    -1,   340,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   538,    -1,   147,    -1,    -1,
      -1,    -1,    -1,    -1,   769,   360,    -1,   195,   363,   774,
      -1,    -1,    -1,   201,   853,   119,   371,   121,   122,   123,
      -1,   125,   126,   788,    -1,    -1,   130,    72,    -1,    -1,
      -1,    76,   136,   137,    -1,   800,    -1,   141,    -1,    -1,
     144,   145,   146,    -1,   809,   195,    -1,    -1,   813,    -1,
      -1,   201,   817,   596,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   903,   904,   905,   906,   256,    -1,
      -1,    -1,   427,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,   922,   923,   130,   131,   132,   853,   134,
     135,   136,   137,    -1,    -1,    -1,   141,    -1,    28,   144,
     145,   146,    -1,    -1,    -1,    -1,   256,    -1,    -1,   652,
      -1,   654,    -1,    -1,   657,    -1,    -1,   660,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   669,    -1,    -1,   672,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   903,   904,
     905,   906,    72,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,   694,   340,    -1,    -1,    -1,    -1,   922,   923,    -1,
      -1,    -1,    -1,   706,    -1,   708,    -1,    -1,    -1,   712,
      -1,    -1,   360,    -1,   104,   363,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   538,    -1,    -1,    -1,    -1,    -1,   119,
     340,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,    -1,   134,   135,   136,   137,    -1,   139,
     360,   141,    -1,   363,   144,   145,   146,    -1,    -1,    -1,
      -1,   371,    -1,    -1,    -1,    -1,   769,    -1,    -1,    -1,
      -1,   774,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   427,
      -1,   596,    -1,    -1,    -1,   788,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   800,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,    -1,    -1,    -1,
     813,    -1,    -1,    -1,   817,    -1,    28,   427,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,   652,    -1,   654,
      -1,    -1,   657,    -1,    28,   660,    -1,    -1,    -1,    -1,
     853,    13,    -1,    -1,   669,    -1,    -1,   672,    -1,    21,
      72,   147,   148,    -1,    76,    -1,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,   694,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
     538,   706,    76,   708,    -1,    -1,    -1,   712,    -1,    -1,
     903,   904,   905,   906,    -1,    67,    -1,   119,    -1,   121,
     122,   123,    -1,   125,   126,   127,    -1,    -1,   130,   922,
     923,    -1,    -1,    -1,   136,   137,    -1,    -1,   538,   141,
      -1,    -1,   144,   145,   146,   119,    -1,   121,   122,   123,
      -1,   125,   126,    -1,    -1,    -1,   130,    -1,   596,   111,
      -1,   113,    -1,   137,   769,    -1,    -1,    -1,    -1,   774,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   800,   596,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   809,    -1,    -1,    -1,   813,    -1,
      -1,    -1,   817,    -1,   652,    -1,   654,    -1,   170,   657,
      -1,    -1,   660,    -1,   176,    -1,    -1,    -1,    -1,   181,
      -1,   669,   184,    -1,   672,    -1,    -1,    -1,    -1,    -1,
      28,    -1,   194,    -1,    -1,   197,    -1,    -1,   853,    -1,
      -1,    -1,   652,    -1,   654,    -1,   694,   657,    -1,   211,
     660,    -1,    -1,   215,    -1,    -1,    -1,    -1,   706,   669,
     708,    -1,   672,    -1,   712,    -1,    -1,    -1,   230,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,   694,    -1,    -1,    -1,   903,   904,
     905,   906,    -1,    -1,    -1,    -1,   706,    -1,   708,    -1,
      -1,    -1,   712,    -1,    -1,    -1,   104,   922,   923,    -1,
      -1,    -1,    -1,    -1,   276,   277,   278,   279,   280,    -1,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,    -1,   134,   135,   136,   137,
     788,   139,   140,   141,    -1,   307,   144,   145,   146,    -1,
      -1,    -1,   800,    -1,    -1,    -1,    -1,    -1,    -1,   769,
      -1,   809,    -1,    -1,   774,   813,    -1,   329,   330,   817,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   788,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     800,    -1,    -1,    -1,   356,    -1,    -1,    -1,    -1,   809,
      -1,    -1,    -1,   813,    -1,   853,    -1,   817,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   379,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   387,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   400,    -1,
      -1,    -1,    -1,   853,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   433,    -1,    -1,   436,    -1,    -1,   439,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   903,   904,   905,   906,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,   922,   923,    -1,    12,    -1,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,   515,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      10,    -1,    -1,    70,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    28,   551,
      -1,    -1,    -1,   555,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   578,    -1,    -1,    -1,
     582,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
     127,    -1,    72,    -1,    -1,    -1,    76,    -1,    -1,   136,
      -1,   138,    -1,   140,   141,   142,    -1,    -1,    -1,   146,
     612,    -1,    -1,   150,    -1,   152,    -1,   154,   155,    -1,
     100,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,   122,   123,    -1,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    -1,   139,
     140,   141,    -1,    -1,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   676,   155,    -1,   679,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   699,    -1,    -1,
      -1,    -1,    -1,    -1,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,   717,    12,    13,    14,    15,
      16,    -1,   724,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    -1,    32,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    43,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    -1,
      56,    -1,    -1,    -1,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    73,    74,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
      86,    -1,    88,    89,    90,    91,    -1,    93,    94,    95,
      96,    -1,    -1,    99,    -1,   101,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,   120,   121,   122,    -1,   830,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,   138,    -1,   140,   141,    -1,   143,    -1,    -1,
     146,    -1,    -1,    -1,   150,    -1,   152,     1,   154,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    43,
      -1,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    71,    72,    73,
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
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   120,   121,   122,    -1,
      -1,    -1,    -1,   127,    -1,    -1,     3,     4,     5,     6,
       7,     8,   136,    -1,   138,    12,   140,   141,    15,    16,
      -1,    -1,   146,    -1,    -1,    22,    -1,    -1,   152,    -1,
     154,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    81,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,   120,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,   136,
      -1,   138,    12,    -1,   141,    15,    16,    -1,    -1,   146,
      -1,    -1,    22,   150,    -1,   152,    -1,   154,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    39,
      40,    -1,    42,    -1,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    -1,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    63,    -1,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    94,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,   105,    -1,    12,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     120,    -1,    -1,    -1,   124,    30,    -1,    -1,   128,    -1,
      -1,    36,    37,    -1,    39,    40,    -1,    42,   138,    -1,
      45,    -1,   142,    48,    49,    -1,    51,    -1,    53,    54,
     150,    -1,   152,   153,   154,    -1,    61,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    80,    81,    82,    83,    84,
      -1,    -1,    87,    -1,    -1,    -1,    91,    -1,    -1,    94,
      -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    15,
      16,    -1,    -1,   138,    -1,    -1,    22,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    30,   150,    -1,   152,    -1,   154,
      36,    37,    -1,    39,    40,    -1,    42,    -1,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    -1,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    81,    82,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    94,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    -1,    42,    -1,
      -1,    45,   138,    -1,    48,    49,    -1,    51,    -1,    53,
      54,    -1,    -1,    -1,   150,    -1,   152,    61,   154,    63,
      -1,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    81,    82,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      94,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,   120,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    39,    40,    -1,
      42,    -1,    -1,    45,   138,    -1,    48,    49,    -1,    51,
      -1,    53,    54,    -1,    -1,    -1,   150,    -1,   152,    61,
     154,    63,    -1,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    81,
      82,    83,    84,    -1,     3,    -1,    -1,    -1,    -1,    91,
      -1,    10,    94,    -1,    -1,    -1,    15,    -1,    17,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   138,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
     152,    70,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,   107,   108,
     109,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    72,    -1,   125,    -1,    76,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,   141,    -1,    10,   144,   145,   146,   147,    -1,
     149,    17,    -1,   152,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,   122,   123,    -1,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
      -1,   139,   140,   141,    -1,    -1,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    72,   155,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    76,   119,    -1,   121,   122,   123,    -1,   125,
     126,   127,    -1,   129,   130,   131,   132,   133,   134,   135,
     136,   137,    -1,   139,   140,   141,    -1,    72,   144,   145,
     146,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,   155,
      -1,    -1,    -1,    17,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    28,   129,   130,   131,   132,   104,
     134,   135,   136,   137,    -1,   139,    -1,   141,    -1,    -1,
     144,   145,   146,    -1,   119,    -1,   121,   122,   123,    -1,
     125,   126,   127,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,    -1,   139,   140,   141,    -1,    72,   144,
     145,   146,    76,    -1,    -1,    -1,    -1,    10,    -1,    -1,
     155,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    72,
     144,   145,   146,    76,    -1,    -1,    -1,    -1,    10,   153,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
      72,   144,   145,   146,    76,    -1,    -1,    -1,    -1,    10,
     153,    -1,    -1,    85,    -1,    -1,    17,    -1,    -1,    -1,
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
     146,    76,    -1,    -1,   150,    -1,    10,    -1,    -1,    -1,
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
     141,   104,    -1,   144,   145,   146,    -1,    -1,    72,    -1,
      -1,    -1,    76,    -1,    -1,    -1,   119,    -1,   121,   122,
     123,    -1,   125,   126,   127,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    -1,   139,   140,   141,    -1,
     104,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,   122,   123,
      -1,   125,   126,   127,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,    -1,   139,   140,   141,    -1,    -1,
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
     211,    10,    17,   119,   120,   121,   122,   125,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   141,
     144,   145,   146,   147,   149,   152,   177,   213,   214,   215,
     216,   257,   143,   269,   245,   257,   257,   274,    29,    29,
     274,    29,    29,   155,   155,   179,   179,   150,   179,    85,
      39,   274,    39,   274,    85,    39,   274,   179,    39,   274,
     179,   124,   153,   274,   274,   164,   274,   164,    31,   177,
     220,   241,   123,   249,    73,   245,   242,   155,    41,   155,
      29,   164,   250,   192,   123,   168,    39,   164,   274,   155,
      39,   164,   274,   155,   274,     1,   151,   169,   151,    35,
      66,    69,   151,   163,   181,   274,   204,   225,   177,   274,
     126,   152,   217,   217,   242,    85,    39,    39,    85,    39,
      39,    39,    39,     1,   200,   274,   274,   274,   274,   274,
     274,   269,    11,    31,   164,   249,   153,    16,    30,    61,
      79,    80,    84,    94,   154,   229,   248,   259,   128,   231,
     242,    98,   232,   274,   246,   259,   164,   179,   153,   274,
     274,   164,   274,   164,   151,   151,   181,     1,   123,   244,
     153,   177,   215,   216,   219,   274,   274,   274,   274,   274,
     274,   151,   151,    85,    85,    85,    85,   274,   164,   242,
     249,   155,   250,   229,   230,   274,   274,   168,   180,   228,
     155,   168,    85,    85,    16,    30,    61,    79,    80,    84,
      94,   154,   243,   259,    87,   226,   217,   217,   153,    85,
      85,    85,    85,    85,    85,   274,   274,   274,   274,   231,
     242,   229,   247,   248,   259,    41,   155,   259,   274,   274,
       1,   155,   250,   232,   274,   247,   248,   155,   243,   259,
     155,   143,   227,   228,   155,   243,   259,   247
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
     212,   212,   213,   213,   213,   213,   213,   213,   213,   214,
     214,   214,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     217,   217,   218,   219,   219,   219,   220,   220,   220,   220,
     220,   221,   221,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   223,   223,   223,   223,   223,   223,   224,   224,
     225,   225,   225,   225,   225,   225,   226,   226,   227,   227,
     228,   228,   229,   229,   230,   230,   231,   231,   232,   232,
     233,   233,   233,   234,   234,   235,   235,   235,   236,   236,
     236,   236,   236,   237,   237,   237,   238,   238,   239,   239,
     240,   240,   240,   241,   241,   241,   242,   242,   242,   243,
     243,   243,   243,   243,   243,   243,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   246,   246,
     246,   246,   247,   247,   247,   248,   248,   248,   248,   248,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   250,   250,   250,   250,   251,   251,   252,   252,   252,
     253,   253,   254,   254,   255,   255,   256,   256,   256,   256,
     257,   258,   258,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   261,   262,   263,
     263,   263,   263,   263,   263,   263,   263,   264,   264,   265,
     266,   266,   267,   268,   268,   269,   269,   269,   270,   270,
     270,   270,   270,   270,   271,   271,   272,   273,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   275,   275,   276,   276,   276,   277,
     277,   277,   277,   278,   278,   279,   279,   279,   280,   280,
     280,   281,   281,   281,   281,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   283,   283,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   285,   285,   285,   285,   285,   285,   286,   286,   286,
     286,   287,   287,   287,   287,   288,   288,   288,   288,   288,
     288,   288
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
       5,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     0,     1,     3,     4,     5,     4,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       0,     1,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     2,     1,     0,     1,     2,     2,     0,     2,
       3,     4,     4,     2,     4,     0,     2,     2,     4,     4,
       4,     5,     4,     0,     1,     1,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     3,
       2,     4,     3,     3,     4,     3,     0,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     4,     4,
       6,     3,     0,     1,     1,     3,     4,     3,     4,     6,
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     4,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     2,     2,     2,     2,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     4,
       1,     3,     4,     1,     3,     4,     3,     3,     1,     1,
       2,     1,     2,     1,     1,     3,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     3,
       3,     2,     2,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     4,     3,     4,     1,     1,     1,     1,     1,
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
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 5354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5589 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5608 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { 
      (yyval.pblockstmt) = buildErrorStandin();
      USR_FATAL_CONT((yyval.pblockstmt), "illegal assignment to type");
    }
#line 5681 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5693 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5705 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5711 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 630 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5717 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5723 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 663 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 5779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5785 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 670 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5839 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5845 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5851 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5857 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5869 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 686 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 704 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 738 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 740 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 745 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6043 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 758 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6067 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6079 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 777 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6091 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 779 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 783 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 789 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 793 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6127 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6141 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6155 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6169 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 847 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 852 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 868 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 870 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6243 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 891 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 898 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6281 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 919 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 931 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6339 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 961 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 974 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 1003 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6437 "bison-chapel.cpp" /* yacc.c:1661  */
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
#line 6464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 1036 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 1040 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 1045 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 1049 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1058 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1059 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6522 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1060 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1062 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 1068 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 1073 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1074 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1075 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1083 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1085 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1086 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1088 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1092 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1101 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1104 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1106 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1121 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1125 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1134 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1136 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1138 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1140 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1144 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1145 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1149 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1150 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1151 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1152 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1153 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1154 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1155 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6912 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6918 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6924 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6936 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6942 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6948 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6954 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6960 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6966 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6990 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1185 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6996 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 7002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7008 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1199 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1211 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1216 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1230 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1243 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1259 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1261 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1263 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1268 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1276 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1292 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1301 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1318 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1326 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7214 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1328 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7220 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7226 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1335 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7232 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1337 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7238 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1342 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7244 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1344 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7250 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1346 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7256 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7262 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7268 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1354 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7274 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1372 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1374 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7316 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1378 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1385 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1387 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1388 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1390 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1392 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1393 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1394 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1400 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1401 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1402 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1403 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1405 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1406 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1407 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1408 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1409 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1447 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7498 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1454 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7504 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7510 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7522 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7528 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7534 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7540 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1476 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7546 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1477 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7552 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7564 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7570 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1494 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1495 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1500 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1511 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1521 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1537 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1663 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1677 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1686 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1691 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 7983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1695 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1699 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8017 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8023 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8029 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8035 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1742 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1744 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1746 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1754 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1760 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1761 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8119 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1788 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1789 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1794 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1795 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1796 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1806 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1807 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 485:
#line 1811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 8197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 486:
#line 1812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 487:
#line 1813 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 488:
#line 1814 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 489:
#line 1815 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1816 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1817 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1819 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1821 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1825 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8261 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1832 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8267 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1833 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8273 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1837 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8279 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1838 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8285 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1839 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8291 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1840 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8297 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1841 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8303 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 503:
#line 1842 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8309 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1843 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8315 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1844 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8321 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8327 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1846 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8333 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1847 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8339 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1848 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8345 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8351 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8357 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8363 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1852 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8369 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1853 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8375 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8381 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8393 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8399 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8405 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8411 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1863 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8417 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1864 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8423 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8429 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1866 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8435 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8447 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1872 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1873 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8459 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8471 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1889 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8513 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8519 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1891 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1892 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8531 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1893 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8537 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8541 "bison-chapel.cpp" /* yacc.c:1661  */
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
