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
    TBORROWED = 270,
    TBY = 271,
    TCATCH = 272,
    TCLASS = 273,
    TCOBEGIN = 274,
    TCOFORALL = 275,
    TCONFIG = 276,
    TCONST = 277,
    TCONTINUE = 278,
    TDEFER = 279,
    TDELETE = 280,
    TDMAPPED = 281,
    TDO = 282,
    TDOMAIN = 283,
    TELSE = 284,
    TENUM = 285,
    TEXCEPT = 286,
    TEXPORT = 287,
    TEXTERN = 288,
    TFOR = 289,
    TFORALL = 290,
    TFORWARDING = 291,
    TIF = 292,
    TIN = 293,
    TINDEX = 294,
    TINLINE = 295,
    TINOUT = 296,
    TITER = 297,
    TLABEL = 298,
    TLAMBDA = 299,
    TLET = 300,
    TLOCAL = 301,
    TMINUSMINUS = 302,
    TMODULE = 303,
    TNEW = 304,
    TNIL = 305,
    TNOINIT = 306,
    TON = 307,
    TONLY = 308,
    TOTHERWISE = 309,
    TOUT = 310,
    TOVERRIDE = 311,
    TOWNED = 312,
    TPARAM = 313,
    TPLUSPLUS = 314,
    TPRAGMA = 315,
    TPRIMITIVE = 316,
    TPRIVATE = 317,
    TPROC = 318,
    TPROTOTYPE = 319,
    TPUBLIC = 320,
    TRECORD = 321,
    TREDUCE = 322,
    TREF = 323,
    TREQUIRE = 324,
    TRETURN = 325,
    TSCAN = 326,
    TSELECT = 327,
    TSERIAL = 328,
    TSHARED = 329,
    TSINGLE = 330,
    TSPARSE = 331,
    TSUBDOMAIN = 332,
    TSYNC = 333,
    TTHEN = 334,
    TTHROW = 335,
    TTHROWS = 336,
    TTRY = 337,
    TTRYBANG = 338,
    TTYPE = 339,
    TUNDERSCORE = 340,
    TUNION = 341,
    TUNMANAGED = 342,
    TUSE = 343,
    TVAR = 344,
    TWHEN = 345,
    TWHERE = 346,
    TWHILE = 347,
    TWITH = 348,
    TYIELD = 349,
    TZIP = 350,
    TALIAS = 351,
    TAND = 352,
    TASSIGN = 353,
    TASSIGNBAND = 354,
    TASSIGNBOR = 355,
    TASSIGNBXOR = 356,
    TASSIGNDIVIDE = 357,
    TASSIGNEXP = 358,
    TASSIGNLAND = 359,
    TASSIGNLOR = 360,
    TASSIGNMINUS = 361,
    TASSIGNMOD = 362,
    TASSIGNMULTIPLY = 363,
    TASSIGNPLUS = 364,
    TASSIGNSL = 365,
    TASSIGNSR = 366,
    TBAND = 367,
    TBNOT = 368,
    TBOR = 369,
    TBXOR = 370,
    TCOLON = 371,
    TCOMMA = 372,
    TDIVIDE = 373,
    TDOT = 374,
    TDOTDOT = 375,
    TDOTDOTDOT = 376,
    TEQUAL = 377,
    TEXP = 378,
    TGREATER = 379,
    TGREATEREQUAL = 380,
    THASH = 381,
    TLESS = 382,
    TLESSEQUAL = 383,
    TMINUS = 384,
    TMOD = 385,
    TNOT = 386,
    TNOTEQUAL = 387,
    TOR = 388,
    TPLUS = 389,
    TQUESTION = 390,
    TSEMI = 391,
    TSHIFTLEFT = 392,
    TSHIFTRIGHT = 393,
    TSTAR = 394,
    TSWAP = 395,
    TASSIGNREDUCE = 396,
    TIO = 397,
    TLCBR = 398,
    TRCBR = 399,
    TLP = 400,
    TRP = 401,
    TLSBR = 402,
    TRSBR = 403,
    TNOELSE = 404,
    TUPLUS = 405,
    TUMINUS = 406
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

#line 420 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 426 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 500 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   14208

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  152
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  127
/* YYNRULES -- Number of rules.  */
#define YYNRULES  536
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  992

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   406

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
     145,   146,   147,   148,   149,   150,   151
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
     673,   674,   675,   679,   694,   695,   696,   697,   698,   699,
     700,   701,   702,   703,   704,   705,   706,   707,   708,   709,
     710,   716,   722,   728,   734,   741,   751,   755,   756,   757,
     758,   759,   761,   763,   765,   770,   773,   774,   775,   776,
     777,   778,   782,   783,   787,   788,   789,   793,   794,   798,
     801,   803,   808,   809,   813,   815,   817,   824,   834,   844,
     854,   867,   872,   877,   885,   886,   891,   892,   894,   899,
     915,   922,   931,   939,   943,   950,   951,   956,   961,   955,
     986,   992,   999,  1007,  1015,  1026,  1032,  1025,  1060,  1064,
    1069,  1073,  1081,  1082,  1086,  1087,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1130,  1131,  1135,  1139,  1140,  1141,  1145,  1147,
    1149,  1151,  1153,  1158,  1159,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1171,  1175,  1176,  1177,  1178,  1179,  1180,
    1184,  1185,  1189,  1190,  1191,  1192,  1193,  1194,  1198,  1199,
    1202,  1203,  1207,  1208,  1212,  1214,  1219,  1220,  1224,  1225,
    1229,  1230,  1234,  1236,  1238,  1243,  1256,  1273,  1274,  1276,
    1281,  1289,  1297,  1305,  1314,  1324,  1325,  1326,  1330,  1331,
    1339,  1341,  1346,  1348,  1350,  1355,  1357,  1359,  1366,  1367,
    1368,  1373,  1375,  1377,  1381,  1385,  1387,  1391,  1399,  1400,
    1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1414,
    1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,
    1444,  1448,  1452,  1460,  1467,  1468,  1469,  1473,  1475,  1481,
    1483,  1485,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,
    1498,  1499,  1500,  1506,  1507,  1508,  1509,  1513,  1514,  1518,
    1519,  1520,  1524,  1525,  1529,  1530,  1534,  1535,  1539,  1540,
    1541,  1542,  1546,  1557,  1558,  1559,  1560,  1561,  1562,  1564,
    1566,  1568,  1570,  1572,  1574,  1576,  1578,  1580,  1582,  1587,
    1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,
    1609,  1611,  1618,  1624,  1630,  1636,  1645,  1655,  1663,  1664,
    1665,  1666,  1667,  1668,  1669,  1670,  1675,  1676,  1680,  1684,
    1685,  1689,  1693,  1694,  1698,  1702,  1706,  1713,  1714,  1715,
    1716,  1717,  1718,  1722,  1723,  1728,  1733,  1741,  1742,  1743,
    1744,  1745,  1746,  1747,  1748,  1749,  1751,  1753,  1755,  1757,
    1759,  1764,  1765,  1766,  1769,  1770,  1771,  1772,  1783,  1784,
    1788,  1789,  1790,  1794,  1795,  1796,  1804,  1805,  1806,  1807,
    1811,  1812,  1813,  1814,  1815,  1816,  1817,  1818,  1819,  1820,
    1824,  1832,  1833,  1837,  1838,  1839,  1840,  1841,  1842,  1843,
    1844,  1845,  1846,  1847,  1848,  1849,  1850,  1851,  1852,  1853,
    1854,  1855,  1856,  1857,  1858,  1859,  1863,  1864,  1865,  1866,
    1867,  1868,  1872,  1873,  1874,  1875,  1879,  1880,  1881,  1882,
    1887,  1888,  1889,  1890,  1891,  1892,  1893
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBORROWED", "TBY", "TCATCH",
  "TCLASS", "TCOBEGIN", "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE",
  "TDEFER", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM",
  "TEXCEPT", "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TFORWARDING", "TIF",
  "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA",
  "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT",
  "TON", "TONLY", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE",
  "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
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
  "require_stmt", "assignment_stmt", "opt_ident", "ident",
  "opt_ident_or_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "forwarding_stmt", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "defer_stmt", "try_stmt",
  "catch_stmt_ls", "catch_stmt", "catch_expr", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_header",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_query_expr", "var_arg_expr",
  "opt_where_part", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "ret_array_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "simple_expr_ls",
  "tuple_component", "tuple_expr_ls", "opt_actual_ls", "actual_ls",
  "actual_expr", "ident_expr", "type_level_expr", "for_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "shadow_var_prefix", "io_expr",
  "new_expr", "let_expr", "expr", "opt_try_expr", "lhs_expr", "fun_expr",
  "call_expr", "dot_expr", "parenthesized_expr", "literal_expr",
  "assoc_expr_ls", "binary_op_expr", "unary_op_expr", "reduce_expr",
  "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
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
     405,   406
};
# endif

#define YYPACT_NINF -840

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-840)))

#define YYTABLE_NINF -488

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -840,   126,  2997,  -840,   -36,  -840,  -840,  -840,  -840,  -840,
    -840,  4309,    17,   148,  8437,  -840,    17,  8437,    78,   148,
    4309,  8437,  4309,   161,  -840,   304,   595,  7112,  8437,  7232,
    8437,   192,  -840,   148,  -840,    57,  7617,  8437,  8437,  -840,
    8437,  -840,  8437,  8437,   208,   216,   939,  1147,  -840,  7737,
    6727,  8437,  7617,  8437,  8437,   272,   222,  4309,  8437,  8557,
    8557,   148,  -840,  8437,  7737,  8437,  8437,  -840,  -840,  8437,
    -840,  -840,  1541,  8437,  8437,  -840,  8437,  -840,  -840,  3439,
    6172,  7737,  -840,  4164,  -840,  -840,   307,  -840,  -840,  -840,
    -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,
    -840,  -840,  -840,  -840,   148,  -840,   148,   298,    52,  -840,
    -840,  -840,   296,   247,  -840,  -840,  -840,   251,   265,   276,
     292,   293, 13849,  1652,   118,   334,   335,  -840,  -840,  -840,
    -840,  -840,  -840,   368,  -840,  -840, 13849,   297,  4309,  -840,
     339,  -840,  8437,  8437,  8437,  8437,  8437,  7737,  7737,   119,
    -840,  -840,  -840,  -840,   322,   320,  -840,  -840,   336, 12169,
     148,   342,  -840,   163, 13849,   355,  6847,  -840,  -840,  -840,
    -840,   148,   167,   148,   343,    18, 11710, 11676,  -840,  -840,
    -840, 12094, 10927,  6847,  4309,   345,    65,    96,     6,  -840,
    4309,  -840,  -840, 11760,   373,   322, 11760,   322,   373,  -840,
    6847,   194,  -840,  -840,   148,  -840,   204, 13849,  8437,  8437,
    -840, 13849,   344, 11969,  -840, 11760,   322, 13849,   352,  6847,
    -840, 13849, 12378,  -840,  -840, 12424,  2003,  -840,  -840, 12503,
     389,   362,   322,   242, 12219, 11760, 12549,   458,  1787,   373,
     458,   373,   -46,  -840,  -840,  3584,   -37,  -840,  8437,  -840,
     -13,   105,  -840,    76, 12628,   100,   493,  -840,  -840,   453,
     400,   370,  -840,  -840,  -840,    50,    57,    57,    57,  -840,
    8437,  8437,  8437,  8437,  7882,  7882,  8437,  8437,  8437,  8437,
    8437,  8437,    92,  1541,  8437,  8437,  8437,  8437,  8437,  8437,
    8437,  8437,  8437,  8437,  8437,  8437,  8437,  8437,  8437,  7352,
    7352,  7352,  7352,  7352,  7352,  7352,  7352,  7352,  7352,  7352,
    7352,  7352,  7352,  7352,  7352,  6847,  6847,  7882,  7882,  6607,
    -840,  -840, 12294, 12344, 12692,    83,  3729,  -840,  7882,    18,
     381,  -840,  8437,  -840,  8437,   420,  -840,   374,   410,  -840,
    -840,   387,   148,   495,  7737,  -840,  4454,  7882,  -840,  4599,
    7882,   386,  -840,    18,  8677,  8437,  -840,  4309,  -840,  -840,
    -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,
     505,  8437,   393,  -840,    51,  -840,  -840,    65,  -840,   423,
     398,  -840,  8797,   447,  8437,    57,  -840,  -840,  -840,   401,
    -840,  -840,  7737,  -840, 13849, 13849,  -840,    42,  -840,  6847,
     402,  -840,   529,  -840,   529,  -840,  8917,   433,  -840,  -840,
    8677,  8437,  -840,  -840,  -840,  -840,  -840,  7472,  -840, 10793,
    6317,  -840,  6462,  -840,  7882,  5734,  3294,   403,  8437,  6026,
    -840,  -840,   148,  7737,   409,   176,   316,    57,   243,   245,
     260,   271, 12044, 13959, 13959,   350,  -840,   350,  -840,   350,
   14023,   997,   789,  1254,   322,   458,  -840,  -840,  -840,  1787,
   14069,   350,  1033,  1033, 13959,  1033,  1033,   629,   458, 14069,
   13988,   629,   373,   373,   458,   419,   424,   426,   434,   436,
     437,   443,   444,   446,   448,   450,   451,   452,   454,   455,
     457,   461,   408,  -840,   350,  -840,   350,    84,  -840,  -840,
    -840,   106,  -840,   148, 13913,   397,  9037,  7882,  9157,  7882,
    8437,  7882, 10549,    17, 12765,  -840,  -840, 13849, 12829,  6847,
    -840,  6847,  -840,   400,  8437,   109,  8437, 13849,    66, 11885,
    8437, 13849,    60, 11835,  6607,  -840,   465,   472,   467, 12902,
     472,   473,   581, 12966,  4309, 11919,  -840,   182,  -840,  -840,
    -840,  -840,  -840,  -840,   537,   110,  -840,    63,  -840,   348,
     468,    65,    96,  8437,   161,  8437,  8437,  8437,  8437,  8437,
    3145,  -840,   259,  6992,  -840, 13849,  -840,  -840,  -840, 13849,
     474,    15,   470,  -840,    28,  -840,  -840,   275,   148,   475,
     483,  -840,  -840,  -840,  -840,  -840,  -840,    10, 10585,  -840,
    -840, 13849,  4309, 13849,  -840, 13035,   477,   506,  1025,   478,
     526,    70,  -840,   557,  -840,  -840,  -840,  -840,  2782,   300,
    -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,
    -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,
    -840,  -840,  -840,  -840,  6607,  -840,    32,  7882,  7882,  8437,
     601, 13099,  8437,   602, 13168,   482, 10624,    18,    18,  -840,
    -840,  -840,  -840,   488, 11760,  -840, 11005,  4744,  -840,  4889,
    -840, 11083,  5034,  -840,    18,  5179,  -840,    18,   188,  -840,
    8437,  -840,  8437,  -840,  4309,  8437,  -840,  4309,   608,  -840,
    -840,   148,   952,  -840,    65,   522,   571,  -840,  -840,  -840,
      38,  -840,  -840,   447,   492,    87,  -840,  -840,  -840,  5324,
    7737,  -840,  -840,  -840,   148,  -840,   525,   336,  -840,  -840,
    -840,  5469,   496,  5614,   500,  -840,  8437,  3874,   508,  1270,
    -840,  8437,   148,  -840,  -840,   348,  -840,  -840,  -840,   148,
    -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,
    -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,  -840,
    8437,   530,   535,   511,   511,  -840,  -840,  -840,   447,   193,
     205, 13232,  9277,  9397, 13305,  9517,  9637,  9757,  9877,  -840,
    -840,  1208,  -840,  4309,  8437, 13849,  8437, 13849,  4309,  8437,
   13849,  -840,  8437, 13849,  -840,  6607,  -840, 13369, 13849,  -840,
   13849,   628,  4309,   522,  -840,   514,  8002,   141,  -840,   573,
    -840,  -840,  7882, 10472,  4309,  -840,    48,   520,  8437,  -840,
    8437,  -840, 13849,  4309,  8437,  -840, 13849,  4309, 13849,   302,
    -840,  4019,  -840,   305,  1195,  1195,  -840,  1422,  -840, 13849,
    -840,    47,  -840, 10832,  2837,    51,  -840,  -840,  -840,  8437,
    8437,  8437,  8437,  8437,  8437,  8437,  8437,   527,  2138, 12966,
   11161, 11239, 12966, 11317, 11395,  -840,  8437,  4309,  -840,   447,
     522,  8437,   161,  8437,  8437,  8437,  8437,  8437,  5880,  -840,
    -840,   113,  7737,  -840,  -840,  8437,    29, 10663,  -840,   515,
     355,  -840,   336, 13849, 11473,  -840, 11551,  -840,  -840,  -840,
    -840,  -840,  9997,   588,  -840,  -840,   511,   511,   226, 13438,
   13502, 13575, 13639, 13712, 13776,  -840,  -840,  4309,  4309,  4309,
    4309, 13849,  -840,  -840,   141,  8147,    99,  -840,  -840, 13849,
   13849,  -840,  -840,  -840,  9877,  -840,  4309,  4309,  8437,   161,
    8437,  8437,  8437,  8437,  8437,  2426,  -840,    21,  -840,   573,
    -840,  -840,  -840,  8437,  8437,  8437,  8437,  8437,  8437, 12966,
   12966, 12966, 12966,  -840,  -840,  -840,  -840,  -840,   121,  7882,
   10237,   545, 12966, 12966,   524, 10117,   101,    85, 10754,  -840,
    -840,  -840,  -840,   232, 10358,  -840,  -840,  -840,  8292,  -840,
     250,  -840
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   480,   481,   482,   483,
     484,     0,   426,    80,     0,   161,   426,     0,   296,    80,
       0,     0,     0,     0,   171,    80,    80,     0,     0,   295,
       0,     0,   181,     0,   177,     0,     0,     0,     0,   417,
       0,   184,     0,     0,     0,     0,   295,   295,   162,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   163,     0,     0,     0,     0,   532,   534,     0,
     535,   536,   460,     0,     0,   533,   530,    89,   531,     0,
       0,     0,     4,     0,     5,     9,    45,    10,    11,    12,
      14,   382,    22,    13,    90,    96,    15,    17,    16,    19,
      20,    21,    18,    95,     0,    93,     0,   452,     0,    97,
      94,    98,     0,   464,   448,   449,   385,   383,     0,     0,
     453,   454,     0,   384,     0,   465,   466,   467,   447,   387,
     386,   450,   451,     0,    38,    24,   393,     0,     0,   427,
       0,    81,     0,     0,     0,     0,     0,     0,     0,   452,
     464,   383,   453,   454,   395,   384,   465,   466,     0,   426,
       0,     0,   135,     0,   367,     0,   374,    84,    83,   182,
     103,     0,   183,     0,     0,     0,     0,     0,   296,   297,
     102,     0,     0,   374,     0,     0,     0,     0,     0,   298,
       0,    86,    32,     0,   518,   445,     0,   396,   519,     7,
     374,   297,    92,    91,   275,   364,     0,   363,     0,     0,
      87,   463,     0,     0,    35,     0,   398,   388,     0,   374,
      36,   394,     0,   142,   138,     0,   384,   142,   139,     0,
     287,     0,   397,     0,   363,     0,     0,   521,   459,   517,
     520,   516,     0,    47,    50,     0,     0,   369,     0,   371,
       0,     0,   370,     0,   363,     0,     0,     6,    46,     0,
     164,     0,   261,   260,   185,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   374,   374,     0,     0,     0,
      25,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,    28,     0,    29,     0,   382,   380,     0,   375,   376,
     381,     0,     0,     0,     0,   112,     0,     0,   111,     0,
       0,     0,   118,     0,     0,    56,    99,     0,   220,   227,
     228,   229,   224,   226,   222,   225,   223,   221,   231,   230,
     128,     0,     0,    30,   235,   178,   302,     0,   303,   305,
       0,   339,     0,   308,     0,     0,    85,    31,    33,     0,
     183,   274,     0,    63,   461,   462,    88,     0,    34,   374,
       0,   149,   140,   136,   141,   137,     0,   285,   282,    60,
       0,    56,   105,    37,    49,    48,    51,     0,   485,     0,
       0,   476,     0,   478,     0,     0,     0,     0,     0,     0,
     489,     8,     0,     0,     0,     0,   254,     0,     0,     0,
       0,     0,   425,   513,   512,   515,   523,   522,   527,   526,
     509,   506,   507,   508,   456,   496,   475,   474,   473,   457,
     500,   511,   505,   503,   514,   504,   502,   494,   499,   501,
     510,   493,   497,   498,   495,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   524,   529,   528,   437,   438,   440,
     442,     0,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   487,   426,   426,   108,   283,   368,     0,     0,
     390,     0,   284,   164,     0,     0,     0,   401,     0,     0,
       0,   407,     0,     0,     0,   119,   531,    59,     0,    52,
      57,     0,   127,     0,     0,     0,   389,   248,   245,   246,
     247,   251,   252,   253,   243,     0,   236,     0,   244,   262,
       0,   306,     0,   335,   332,   336,   338,   333,   334,   337,
       0,   331,   448,     0,   300,   446,   299,   472,   366,   365,
       0,     0,     0,   391,     0,   143,   289,   448,     0,     0,
       0,   486,   455,   477,   372,   479,   373,     0,     0,   488,
     124,   411,     0,   491,   490,     0,     0,   165,     0,     0,
     175,     0,   172,   258,   255,   256,   259,   186,     0,     0,
     291,   290,   292,   294,    64,    71,    72,    73,    68,    70,
      78,    79,    66,    69,    67,    65,    75,    74,    76,    77,
     470,   471,   439,   441,     0,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
     378,   379,   377,     0,     0,   126,     0,     0,   110,     0,
     109,     0,     0,   116,     0,     0,   114,     0,     0,   432,
       0,   100,     0,   101,     0,     0,   130,     0,   132,   249,
     250,     0,   243,   234,     0,   352,   263,   266,   265,   267,
       0,   304,   307,   308,     0,     0,   309,   310,   151,     0,
       0,   150,   153,   392,     0,   144,   147,     0,   286,    61,
      62,     0,     0,     0,     0,   125,     0,     0,     0,   295,
     170,     0,   173,   169,   257,   262,   217,   215,   196,   199,
     197,   198,   209,   200,   213,   205,   203,   216,   204,   202,
     207,   212,   214,   201,   206,   210,   211,   208,   218,   219,
       0,   194,     0,   232,   232,   192,   293,   430,   308,   464,
     464,     0,     0,     0,     0,     0,     0,     0,     0,   107,
     106,     0,   113,     0,     0,   400,     0,   399,     0,     0,
     406,   117,     0,   405,   115,     0,   431,    54,    53,   129,
     416,   131,     0,   352,   237,     0,     0,   308,   264,   280,
     301,   343,     0,   487,     0,   155,     0,     0,     0,   145,
       0,   122,   413,     0,     0,   120,   412,     0,   492,     0,
      39,     0,   159,    80,   295,   295,   157,   295,   167,   176,
     174,     0,   195,     0,     0,   235,   188,   189,   434,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   295,   404,
       0,     0,   410,     0,     0,   433,     0,     0,   134,   308,
     352,   358,   355,   359,   361,   356,   357,   360,     0,   354,
     362,   448,   276,   240,   238,     0,     0,     0,   341,   448,
     156,   154,     0,   148,     0,   123,     0,   121,    41,    40,
     168,   328,     0,   268,   193,   194,   232,   232,     0,     0,
       0,     0,     0,     0,     0,   160,   158,     0,     0,     0,
       0,    55,   133,   239,   308,   344,     0,   277,   279,   278,
     281,   272,   273,   179,     0,   146,     0,     0,   324,   321,
     325,   327,   322,   323,   326,     0,   320,   448,   269,   280,
     190,   191,   233,     0,     0,     0,     0,     0,     0,   403,
     402,   409,   408,   242,   241,   346,   347,   349,   448,     0,
     487,   448,   415,   414,     0,   312,     0,     0,     0,   348,
     350,   317,   315,   448,   487,   270,   187,   271,   344,   316,
     448,   351
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -840,  -840,  -840,     0,  -373,  1982,  -840,  -840,  -840,   361,
     -53,  -231,   266,   277,  -840,  -840,   181,   406,  2557,    -3,
     306,  -821,  -653,   -43,  -840,  -840,  -840,   -14,  -840,  -840,
    -840,   449,  -840,   -28,  -840,  -840,  -840,  -840,  -840,   517,
     169,   -88,  -840,  -840,  -840,   -38,  1022,  -840,  -840,  -840,
    -840,  -840,  -840,  -840,  -840,  -149,  -607,  -740,  -840,  -148,
       7,   144,  -840,  -840,  -840,   -34,  -840,  -840,  -275,   311,
    -840,  -220,  -244,  -840,  -155,  -840,   677,  -840,  -183,   324,
    -840,  -369,  -680,  -572,  -840,  -530,  -399,  -642,  -839,  -702,
     -35,  -840,    54,  -840,  -141,  -840,   190,   740,  -376,  -840,
    -840,  1163,  -840,     1,  -840,  -840,  -232,  -840,  -497,  -840,
    -840,  1454,  1530,    -2,   149,   994,  -840,  1691,  1950,  -840,
    -840,  -840,  -840,  -840,  -840,  -840,  -309
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   244,    83,   542,    85,    86,   259,    87,
     245,   537,   541,   538,    88,    89,    90,   168,    91,   172,
     192,    92,    93,    94,    95,    96,    97,   655,    98,    99,
     100,   402,   585,   717,   101,   102,   581,   712,   103,   104,
     434,   729,   105,   106,   611,   612,   149,   185,   559,   108,
     109,   436,   735,   617,   762,   763,   371,   846,   375,   555,
     556,   557,   558,   618,   264,   700,   949,   986,   933,   205,
     928,   883,   886,   110,   231,   407,   111,   112,   188,   189,
     379,   380,   574,   946,   903,   383,   571,   966,   880,   807,
     246,   163,   250,   251,   337,   338,   339,   150,   114,   115,
     116,   151,   118,   138,   139,   501,   353,   678,   502,   503,
     119,   152,   153,   211,   340,   155,   124,   156,   157,   127,
     128,   255,   129,   130,   131,   132,   133
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     122,   554,    82,   202,   203,   330,   572,   586,   560,   136,
     505,   764,   154,   175,   206,   159,   341,   158,   122,   164,
     122,   427,   169,   810,   847,   176,   177,   181,   182,   233,
     587,     5,   703,   381,   193,   194,   195,   679,   196,   381,
     197,   198,   372,   580,   384,   190,   253,   207,   901,   213,
     215,   216,   217,     5,  -243,   221,   222,   225,   229,   389,
       5,   232,   234,   235,   236,   932,     5,   237,     5,   709,
     238,   239,   240,   547,   241,   190,   838,   207,   400,   254,
     417,   122,   438,   439,   440,   441,   967,   672,   848,   548,
     134,  -319,   549,   667,   262,     5,  -152,   381,   414,    50,
     134,   869,  -319,   351,   420,   710,   550,   418,  -329,   551,
     137,    44,  -319,   325,   424,   263,   768,  -318,   437,   552,
     456,   511,   642,   385,   540,   812,     3,   884,  -318,  -329,
    -329,   980,  -152,   421,  -329,   553,   122,   969,  -318,   511,
     136,   322,   323,   324,   221,   207,   254,   767,   382,  -329,
     376,     5,   643,   351,   382,    50,   932,  -319,   721,   711,
     329,   326,   160,   902,  -319,   392,   950,   951,   924,   351,
     809,   326,   326,   714,   491,   492,   457,   609,  -329,     5,
     540,  -329,   122,  -318,   900,    15,  -152,   732,   122,   923,
    -318,   326,   702,   425,  -329,   186,  -243,     5,   390,   212,
     425,   167,   186,   326,   425,   838,   394,   395,   694,   326,
     377,  -353,   382,  -329,   733,   199,   425,   429,   425,  -345,
     689,   985,   422,   644,   426,   505,   392,   692,   326,   252,
    -353,   512,  -329,    48,  -353,   813,   -80,   907,  -345,   573,
     224,   228,  -345,   122,   964,   416,   419,   970,   430,   984,
     690,   423,   645,    62,   619,   665,   693,   -80,   582,  -353,
     446,   448,   882,   315,  -469,   316,  -469,  -345,   442,   443,
     444,   445,   447,   449,   450,   451,   452,   453,   454,   455,
     332,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,  -330,   865,   333,
     674,   677,  -311,   493,   495,   795,   166,     5,     5,   525,
    -436,   167,   167,  -311,   513,   494,   496,   504,   265,   554,
    -313,   392,  -435,  -311,   122,   805,   514,  -297,   979,  -330,
     517,  -313,   518,   528,   796,   505,   532,   183,   613,  -436,
     393,  -313,   207,   692,   527,   529,   991,   531,   533,   218,
    -330,  -435,   539,   539,   266,   543,   837,  -330,   214,   392,
     385,   200,   385,  -297,   267,   722,   724,   219,  -311,   545,
     696,   258,   952,  -297,   614,  -311,  -330,   385,   409,   620,
     122,   621,   575,  -419,   615,   268,  -313,  -418,   385,   171,
     579,   249,  -288,  -313,  -297,  -330,   622,   191,   607,   273,
     616,   269,  -330,   982,   122,  -330,   697,   623,   539,   539,
     597,  -288,   989,   191,   888,   579,   698,   385,   270,   140,
     223,   227,   598,   579,   601,   161,   603,   605,  -423,  -424,
     881,   207,   699,   718,  -422,   317,   766,   889,   134,   318,
     274,   282,   319,  -469,   275,  -469,   898,   334,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   648,  -468,   280,  -468,   318,   282,
    -421,  -420,   554,   285,   594,   321,   596,   336,   331,   326,
     396,   345,   348,   352,   273,   837,   505,   406,   344,   280,
     374,   281,   282,   650,   336,   653,   285,   399,   408,   387,
     431,   432,   388,   292,   527,   651,   531,   654,   543,   656,
     601,   336,   298,   435,   657,   658,   433,   516,   519,   327,
     520,   398,   664,   522,   666,   274,   947,   521,   671,   275,
     336,   534,   504,   524,   544,   705,   191,   191,   191,   546,
     561,   412,   122,   370,   562,   573,   584,   577,   583,   968,
     588,   602,   608,  -340,   191,   624,   641,   191,   971,   547,
     625,   154,   626,   197,   216,   217,   221,   232,   254,   252,
     627,   252,   628,   629,   280,   548,   191,   282,   549,   630,
     631,   285,   632,  -342,   633,  -340,   634,   635,   636,   680,
     637,   638,   550,   639,   968,   551,   191,   256,     5,   983,
     122,   -58,   167,   681,   170,   552,  -340,   640,   990,   683,
     684,   719,   968,  -340,   701,  -342,   713,  -297,   708,   720,
     727,   553,   730,   392,   731,   734,   336,   336,   772,   775,
     777,   781,  -340,   446,   493,   515,  -342,   802,   806,   808,
     811,   818,   504,  -342,   823,   447,   494,   771,   827,  -382,
     774,  -340,   832,  -297,   844,   273,   845,   867,  -340,   535,
     870,  -340,  -342,  -297,   885,   785,   892,   787,   661,   948,
     790,   915,   981,   793,   831,   816,   404,   590,   797,   171,
     798,  -342,   122,   800,  -297,   122,   817,   589,  -342,   342,
     191,  -342,   663,   858,   840,   906,   274,   908,   691,   804,
     275,   841,   987,   578,   963,   977,   180,   122,   207,   576,
     336,   662,     0,     0,   191,     0,     0,     0,     0,   822,
       0,   826,   707,     0,   828,   122,     0,     0,   578,   839,
       0,   249,     0,   249,     0,     0,   578,     0,     0,     0,
       0,   277,   113,   278,   279,   280,     0,   281,   282,     0,
       0,   113,   285,     0,     0,     0,     0,     0,   843,   292,
     113,     0,   113,     0,     0,     0,   296,   297,   298,     0,
     785,   787,     0,   790,   793,   822,   826,     0,     0,     0,
       0,   859,   860,     0,   861,     0,   862,   863,     0,     0,
     864,   202,   203,   504,     0,     0,     0,   113,     0,     0,
     122,     0,     0,     0,   122,     0,     0,     0,     0,     0,
     887,   601,   122,     0,     0,   273,   893,     0,   894,   113,
       0,   122,   896,   113,     0,   122,     0,     0,     0,   122,
     660,   416,   336,     0,   668,   670,     0,     0,   673,   676,
       0,     0,     0,   926,     0,     0,     0,   859,   909,   910,
     862,   911,   912,   913,   914,     0,   274,     0,     0,     0,
     275,     0,     0,     0,   921,   122,     0,     0,     0,   154,
       0,   197,   216,   217,   221,   232,   254,     0,   113,     0,
     929,     0,     0,   930,     0,     0,     0,     0,     0,   191,
     191,     0,     0,   191,   191,     0,     0,     0,     0,     0,
     122,   277,     0,     0,   279,   280,   688,   281,   282,     0,
     976,     0,   285,     0,     0,   959,   960,   961,   962,   292,
       0,     0,     0,   122,   113,     0,   296,   297,   298,     0,
     113,     0,   826,     0,   972,   973,   154,     0,   197,   216,
     217,   221,   232,   254,     0,   715,     0,     0,     0,     0,
       0,   959,   960,   961,   962,   972,   973,     0,     0,     0,
     178,     0,     0,   779,   780,     0,     0,   978,   601,     0,
     782,    25,   201,   122,   547,     0,     0,     0,     0,    32,
     791,  -180,   601,   794,     0,   113,   826,   -44,     0,     0,
     548,     0,     0,   549,     0,    41,   123,     0,     0,     0,
       0,     0,  -180,   -44,     0,   123,     0,   550,     0,     0,
     551,     0,    44,     0,   123,     0,   123,     0,   191,   191,
     552,     0,     0,   273,   107,   191,   728,   370,     0,     0,
       0,     0,   370,   107,     0,   191,   553,     0,   191,     0,
       0,     0,   107,  -166,   107,     0,  -166,  -166,     0,     0,
       0,   123,     0,   226,   226,  -166,     0,  -166,  -166,   273,
       0,  -166,     0,     0,   274,  -166,   113,  -166,   275,     0,
       0,     0,     0,   123,     0,     0,     0,   123,   819,   107,
       0,  -166,     0,  -166,     0,  -166,   113,  -166,  -166,   113,
    -166,  -166,     0,  -166,     0,     0,     0,   113,     0,     0,
     274,   107,     0,     0,   275,   107,     0,     0,     0,  -166,
       0,  -166,     0,   280,  -166,   281,   282,   879,     0,     0,
     285,     0,   891,     0,     0,     0,     0,   292,     0,     0,
       0,     0,   123,     0,   296,   297,   298,     0,     0,     0,
       0,     0,     0,     0,     0,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,     0,   285,     0,     0,     0,
     107,  -166,   291,   292,     0,   117,   113,   295,   178,  -166,
     296,   297,   298,     0,   117,     0,     0,   191,   123,    25,
     201,     0,     0,   117,   123,   117,     0,    32,     0,  -180,
       0,     0,     0,   927,     0,   -43,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,   107,     0,     0,   857,
    -180,   -43,   107,     0,     0,     0,   178,     0,     0,     0,
     117,   370,   370,     0,   370,   370,  -166,    25,   201,  -166,
    -166,     0,     0,     0,     0,    32,   965,  -180,  -166,   123,
    -166,  -166,   117,   646,  -166,     0,   117,   931,  -166,     0,
    -166,    41,     0,   935,     0,   370,     0,   370,  -180,     0,
       0,     0,     0,     0,  -166,     0,  -166,   107,  -166,     0,
    -166,  -166,     0,  -166,  -166,     0,  -166,     0,     0,     0,
     273,   965,     0,     0,   113,     0,     0,     0,    15,     0,
       0,    18,  -166,     0,  -166,     0,     0,  -166,     0,   965,
      24,   117,    25,   833,     0,     0,    29,     0,     0,     0,
      32,     0,  -180,     0,     0,     0,     0,     0,     0,     0,
     123,   274,     0,     0,     0,   275,    41,     0,     0,     0,
      44,     0,   834,  -180,     0,   835,    48,     0,   931,     0,
     123,     0,   113,   123,  -166,     0,     0,   117,   107,     0,
       0,   123,  -166,   117,    61,     0,    62,     0,   765,     0,
       0,     0,     0,     0,     0,     0,   277,     0,   107,     0,
     280,   107,   281,   282,     0,     0,     0,   285,     0,   107,
       0,     0,     0,     0,   292,     0,     0,   769,   770,     0,
       0,   296,   297,   298,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    77,   113,   117,   113,
       0,     0,   113,     0,   836,   113,     0,     0,     0,     0,
     123,     0,     0,     0,   113,     0,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      15,     0,     0,    18,     0,     0,     0,     0,   107,   113,
       0,     0,    24,     0,    25,   833,   120,     0,    29,     0,
       0,   113,    32,   113,  -180,   120,     0,   113,     0,     0,
       0,     0,     0,     0,   120,     0,   120,     0,    41,     0,
       0,     0,   256,     0,   834,  -180,     0,   835,    48,   117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,   117,
       0,   120,   117,     0,     0,     0,     0,     0,     0,     0,
     117,     0,     0,   113,     0,     0,     0,     0,   113,     0,
       0,     0,   121,   120,     0,     0,     0,   120,   123,     0,
       0,   121,   113,     0,     5,     6,     7,     8,     9,    10,
     121,     0,   121,   142,   113,     0,    14,     0,    77,     0,
       0,     0,     0,   113,     0,     0,   107,   113,     0,    23,
       0,   113,     0,     0,     0,   143,   144,     0,   145,     0,
      31,     0,     0,     0,     0,    34,    35,   121,    37,   117,
      38,    39,   120,     0,     0,     0,   123,     0,    42,     0,
      43,     0,    45,     0,     0,     0,     0,   113,     0,   121,
       0,     0,     0,   121,     0,    53,    54,    55,    56,   146,
       0,     0,     0,     0,   107,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   120,     0,
       0,     0,     0,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,    68,    69,    70,    71,   113,   113,   113,
     113,   123,     0,   123,     0,     0,   123,     0,   121,   123,
      73,     0,    74,     0,     0,    76,   113,   113,   123,     0,
      78,   123,     0,     0,   147,     0,    80,     0,   148,   107,
       0,   107,     0,   125,   107,     0,     0,   107,     0,   120,
       0,     0,   125,   123,     0,     0,   107,   117,     0,   107,
       0,   125,     0,   125,   121,   123,     0,   123,     0,     0,
     121,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   107,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,     0,   107,     0,     0,   125,   107,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,     0,   117,     0,     0,     0,     0,
     125,     0,     0,     0,   125,   121,     0,   123,     0,     0,
     120,     0,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,   314,  -443,     0,   123,  -468,     0,  -468,
     120,     0,     0,   120,     0,   107,     0,     0,   123,     0,
     107,   120,     0,   273,     0,     0,     0,   123,     0,     0,
       0,   123,     0,     0,   107,   123,     0,     0,     0,   125,
     117,     0,   117,     0,     0,   117,   107,     0,   117,     0,
       0,     0,     0,     0,     0,   107,     0,   117,     0,   107,
     117,     0,     0,   107,   274,     0,   121,     0,   275,     0,
       0,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   117,     0,     0,   125,   121,     0,     0,   121,
     120,   125,     0,     0,   117,     0,   117,   121,     0,   107,
     117,     0,     0,     0,     0,     0,     0,     0,     0,   277,
       0,   278,   279,   280,     0,   281,   282,     0,     0,     0,
     285,   123,   123,   123,   123,     0,   291,   292,     0,     0,
       0,   295,     0,     0,   296,   297,   298,     0,     0,     0,
     123,   123,     0,     0,     0,     0,   125,     0,     0,   107,
     107,   107,   107,     0,     0,     0,   117,     0,     0,     0,
       0,   117,   126,     0,     0,     0,   121,     0,   107,   107,
       0,   126,     0,     0,     0,   117,     0,     0,     0,     0,
     126,     0,   126,     0,     0,     0,     0,   117,     0,     0,
       0,     0,     0,     0,    84,     0,   117,     0,     0,     0,
     117,     0,     0,   135,   117,     0,     0,     0,   120,     0,
       0,     0,   162,     0,   165,     0,     0,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   126,
     117,     0,     0,   126,     0,     0,     0,   125,     0,   220,
     125,     0,     0,     0,     0,     0,     0,     0,   125,     0,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
       0,    84,     0,     0,     0,   257,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
     117,   117,   117,   117,     0,     0,     0,     0,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   117,
     117,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,     0,     0,   125,     0,     0,
     320,   120,     0,   120,     0,     0,   120,     0,     0,   120,
       0,     0,   121,     0,   126,     0,     0,     0,   120,     0,
     126,   120,     0,   313,   314,     0,     0,     0,  -468,     0,
    -468,     0,     0,     0,     0,     0,    15,     0,     0,    18,
       0,     0,     0,   120,     0,     0,   373,     0,    24,     0,
      25,   833,   386,     0,    29,   120,     0,   120,    32,     0,
    -180,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    41,   126,     0,   121,    44,   121,
     834,  -180,   121,   835,    48,   121,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,   121,     0,     0,
       0,     0,    61,     0,    62,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,   125,     0,   120,     0,   121,
       0,     0,   120,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,   121,     0,     0,   120,   121,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   120,     0,
       0,     0,     0,     0,    77,     0,   126,   120,     0,     0,
       0,   120,   916,     0,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,   126,     0,     0,   126,
       0,     0,     0,     0,     0,     0,     0,   126,    84,     0,
       0,     0,     0,   121,     0,     0,     0,     0,   121,     0,
       0,   120,     0,     0,     0,     0,     0,     0,   386,     0,
       0,   386,   121,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,     0,   121,     0,     0,     0,   121,   125,     0,
     125,   121,     0,   125,     0,     0,   125,     0,     0,     0,
       0,   120,   120,   120,   120,   125,   126,     0,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     120,   120,     0,     0,     0,     0,     0,   121,     0,     0,
     125,     0,     0,     0,     0,     0,     0,     0,   600,     0,
       0,     0,   125,     0,   125,     0,     0,     0,   125,     0,
       0,     0,     0,     0,     0,     0,     0,   974,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,   121,   121,   121,
     121,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,   121,   121,     0,     0,
      34,    35,     0,    37,   125,    38,    39,     0,     0,   125,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,   125,   126,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,   125,     0,     0,     0,     0,
       0,     0,     0,    63,   125,     0,     0,     0,   125,     0,
       0,     0,   125,    67,     0,     0,   686,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,   126,     0,     0,    73,     0,    74,   125,    75,
      76,   204,     0,     0,     0,    78,     0,     0,     0,   147,
     141,    80,     0,   148,   975,     0,   141,     0,     0,     0,
       0,     0,   141,   141,   725,     0,     0,     0,     0,     0,
     184,     0,   187,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   125,   125,
     125,   125,     0,     0,     0,     0,     0,   126,   230,   126,
       0,     0,   126,     0,     0,   126,     0,   125,   125,     0,
       0,     0,     0,     0,   126,     0,     0,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   386,
       0,   386,     0,     0,   386,     0,     0,   386,     0,   126,
       0,   260,     0,   261,     0,     0,   799,     0,     0,   801,
       0,   126,     0,   126,     0,     0,     0,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   815,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   821,     0,   825,     0,     0,     0,    84,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
       0,     0,     0,   335,     0,     0,     0,     0,   230,     0,
     343,     0,     0,   126,     0,     0,     0,     0,   126,     0,
     335,     0,     0,   378,     0,     0,     0,     0,     0,     0,
       0,     0,   126,     0,     0,     0,     0,   335,   141,     0,
       0,   391,     0,     0,   126,     0,     0,     0,     0,     0,
       0,     0,     0,   126,     0,     0,   335,   126,     0,     0,
       0,   126,     0,     0,   868,     5,     0,     0,     0,     0,
       0,     0,   736,     0,     0,     0,   890,     0,   737,     0,
       0,     0,     0,     0,     0,   895,     0,     0,     0,   897,
       0,     0,     0,    84,     0,     0,     0,   126,     0,     0,
       0,     0,   187,   187,   187,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       5,     0,     0,     0,     0,     0,     0,   736,     0,   922,
       0,     0,     0,   737,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   126,   126,   126,
     126,     0,   335,   335,     0,     0,     0,     0,     0,     0,
     358,   359,   360,   361,   362,   363,   126,   126,   364,   365,
     366,   367,   368,   369,   738,   739,   740,   741,     0,   523,
     742,     0,     0,     0,   743,   744,   745,   746,   747,   748,
     749,   750,   751,   752,   753,     0,   754,     0,     0,   755,
     756,   757,   758,     0,   759,     0,     0,   760,     0,     0,
       0,     0,     0,     0,   378,   358,   359,   360,   361,   362,
     363,     0,   187,   364,   365,   366,   367,   368,   369,   738,
     739,   740,   741,     0,     0,   742,   335,     0,     0,   743,
     744,   745,   746,   747,   748,   749,   750,   751,   752,   753,
       0,   754,     0,     0,   755,   756,   757,   758,     0,   759,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   606,
       0,     0,   610,     0,   187,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,     0,     0,    15,    16,    17,    18,  -295,
      19,    20,    21,     0,    22,    23,     0,    24,     0,    25,
      26,    27,    28,    29,    30,     0,    31,    32,     0,  -180,
      33,    34,    35,    36,    37,   -42,    38,    39,     0,    40,
       0,     0,     0,    41,    42,  -295,    43,    44,    45,    46,
    -180,   -42,    47,    48,     0,  -295,    49,    50,     0,    51,
      52,    53,    54,    55,    56,    57,     0,    58,   335,    59,
      60,    61,     0,    62,    63,    64,  -295,     0,     0,    65,
       0,    66,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,   695,     0,     0,    72,   378,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,     0,    77,     0,     0,    78,     0,     0,     0,
      79,   716,    80,     0,    81,   230,   704,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   142,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,   761,     0,     0,     0,   143,
     144,     0,   145,     0,    31,     0,     0,     0,     0,    34,
      35,     0,    37,     0,    38,    39,     0,     0,     0,     0,
       0,     0,    42,     0,    43,     0,    45,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
      54,    55,    56,   146,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,   803,     0,
       0,   378,     0,     0,     0,     0,     0,    68,    69,    70,
      71,     0,     0,     0,     0,    72,     0,     0,     0,     0,
       0,   716,     0,     0,    73,     0,    74,     0,    75,    76,
     204,     0,     0,     0,    78,     0,     0,     0,   147,   610,
      80,     0,   148,     0,     0,     4,   842,     5,     6,     7,
       8,     9,    10,     0,  -487,     0,    11,    12,    13,    14,
    -487,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
    -487,    22,    23,  -487,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,  -487,  -295,    49,    50,  -487,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
     141,  -487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   905,     0,     0,     0,     0,  -487,    69,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,     0,  -487,  -487,  -487,  -487,
    -487,  -487,  -487,  -487,  -487,    74,  -487,  -487,  -487,     0,
      77,  -487,  -487,  -487,     0,     0,     0,    79,  -487,    80,
     242,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,    44,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,   204,    77,     0,     0,    78,     0,
       0,     0,    79,   243,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,    44,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,   415,    80,
     242,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,    44,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,   243,    80,   829,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,    44,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,   830,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,    44,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,   899,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,   256,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,    30,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   526,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   530,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   784,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   786,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   789,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   792,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,   814,    23,     0,    24,     0,    25,    26,    27,    28,
      29,    30,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,     0,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       4,    81,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,     0,     0,    15,    16,    17,
      18,  -295,    19,    20,    21,     0,    22,    23,     0,    24,
       0,    25,    26,    27,    28,    29,   820,     0,    31,    32,
       0,  -180,    33,    34,    35,    36,    37,   -42,    38,    39,
       0,    40,     0,     0,     0,    41,    42,  -295,    43,     0,
      45,    46,  -180,   -42,    47,    48,     0,  -295,    49,    50,
       0,    51,    52,    53,    54,    55,    56,    57,     0,    58,
       0,    59,    60,    61,     0,    62,    63,    64,  -295,     0,
       0,    65,     0,    66,     0,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,    69,    70,    71,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,     0,     0,    73,     0,
      74,     0,    75,    76,     0,    77,     0,     0,    78,     0,
       0,     0,    79,     0,    80,     4,    81,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
       0,     0,    15,    16,    17,    18,  -295,    19,    20,    21,
       0,    22,    23,     0,    24,     0,    25,    26,    27,    28,
      29,   824,     0,    31,    32,     0,  -180,    33,    34,    35,
      36,    37,   -42,    38,    39,     0,    40,     0,     0,     0,
      41,    42,  -295,    43,     0,    45,    46,  -180,   -42,    47,
      48,     0,  -295,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,    58,     0,    59,    60,    61,     0,
      62,    63,    64,  -295,     0,     0,    65,     0,    66,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     5,     6,     7,
       8,     9,    10,    73,     0,    74,   142,    75,    76,    14,
      77,     0,     0,    78,     0,     0,     0,    79,     0,    80,
       0,    81,    23,     0,     0,     0,     0,     0,   143,   144,
       0,   145,     0,    31,     0,     0,     0,     0,    34,    35,
       0,    37,     0,    38,    39,     0,     0,     0,     0,     0,
       0,    42,     0,    43,     0,    45,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    54,
      55,    56,   146,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,    69,    70,    71,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    73,     0,    74,     0,    75,    76,   204,
       0,     0,     0,    78,     0,     0,     0,   147,     0,    80,
       0,   148,   599,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   142,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    23,     0,
       0,     0,     0,     0,   143,   144,     0,   145,     0,    31,
       0,     0,     0,     0,    34,    35,     0,    37,     0,    38,
      39,     0,     0,     0,     0,     0,     0,    42,     0,    43,
       0,    45,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,    56,   146,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,    69,    70,    71,     0,     0,     0,     0,
      72,     0,     0,     0,     0,     0,     0,     0,     0,    73,
       0,    74,     0,    75,    76,   204,     0,     0,     0,    78,
       0,     0,     0,   147,     0,    80,     0,   148,   925,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   142,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
     143,   144,     0,   145,     0,    31,     0,     0,     0,     0,
      34,    35,     0,    37,     0,    38,    39,     0,     0,     0,
       0,     0,     0,    42,     0,    43,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,   146,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,    73,     0,    74,     0,    75,
      76,     0,     0,     0,     0,    78,     0,     0,     0,   147,
       0,    80,     0,   148,   604,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,   247,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,   248,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,   208,
     209,     0,   247,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,   593,   148,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,   247,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,   595,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,   497,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,   498,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,   500,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,   208,
     209,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,   210,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,   208,
     209,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,   706,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
     173,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,   174,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,   178,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,   179,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,   208,   209,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,    14,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,   591,    80,     0,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   190,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
      79,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,   174,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     5,     6,     7,     8,     9,
      10,    73,     0,    74,   142,    75,    76,   871,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
     872,     0,     0,     0,     0,     0,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,   873,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   874,   875,    55,    56,
     876,     0,     0,     0,     0,     0,     0,     0,     0,   877,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   878,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,    74,     0,
      75,    76,   204,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   878,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,   143,   144,     0,   856,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,    72,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,    74,     0,    75,    76,   204,     0,     0,
       0,    78,     0,     0,     0,   147,     0,    80,     0,   148,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
       0,     0,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
      79,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,   563,     0,     0,     0,   536,     0,     0,     0,
     147,     0,    80,     0,   148,   564,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,   565,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   566,   567,    55,    56,   568,     0,     0,     0,     0,
       0,     0,     0,     0,   569,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   570,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   570,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   649,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   652,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   850,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   851,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   853,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   854,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   855,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   856,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,   938,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   148,   939,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,   940,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   941,   942,    55,    56,   943,     0,     0,     0,     0,
       0,     0,     0,     0,   944,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
     147,     0,    80,     0,   945,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    72,     0,     0,
       5,     6,     7,     8,     9,    10,    73,     0,    74,   142,
      75,    76,    14,     0,     0,     0,    78,     0,     0,     0,
       0,     0,    80,     0,   945,    23,     0,     0,     0,     0,
       0,   143,   144,     0,   145,     0,    31,     0,     0,     0,
       0,    34,    35,     0,    37,     0,    38,    39,     0,     0,
       0,     0,     0,     0,    42,     0,    43,     0,    45,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,    54,    55,    56,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -344,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,  -344,     0,     0,     0,  -344,     0,
       0,     5,     6,     7,     8,     9,    10,     0,    74,     0,
     142,     0,   204,    14,     0,     0,     0,     0,     0,     0,
     147,     0,    80,  -344,   878,     0,    23,     0,     0,     0,
       0,     0,   143,   144,     0,   145,     0,    31,     0,     0,
       0,     0,    34,    35,     0,    37,     0,    38,    39,     0,
       0,     0,     0,     0,     0,    42,     0,    43,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,  -314,     0,
       0,     0,    53,    54,    55,    56,   146,     0,     0,  -314,
       0,     0,     0,     0,     0,    63,     0,     0,     0,  -314,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   142,     0,     0,    14,     0,    74,
       0,     0,     0,     0,  -314,     0,     0,     0,     0,     0,
      23,  -314,     0,    80,     0,   945,   143,   144,     0,   145,
       0,    31,     0,     0,     0,     0,    34,    35,     0,    37,
       0,    38,    39,     0,     0,     0,     0,     0,     0,    42,
       0,    43,     0,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
     146,     0,     5,     6,     7,     8,     9,    10,     0,    63,
       0,   142,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,   143,   144,    69,   145,     0,    31,     0,
       0,     0,     0,    34,    35,   271,    37,     0,    38,    39,
       0,   272,     0,    74,     0,     0,    42,     0,    43,     0,
      45,   273,     0,     0,     0,   147,     0,    80,     0,   570,
       0,     0,     0,    53,    54,    55,    56,   146,     0,     0,
       0,     0,     0,     0,   271,     0,    63,     0,     0,     0,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     273,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,   351,   272,
      74,     0,   276,     0,     0,     0,     0,     0,     0,   273,
       0,   274,     0,     0,    80,   275,   148,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,     0,   293,   294,   295,
       0,   276,   296,   297,   298,     0,     0,     0,     0,     0,
     274,     0,     0,   723,   275,     0,   277,     0,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,     0,
     276,   296,   297,   298,   271,     0,     0,     0,     0,     0,
     272,     0,   778,     0,     0,   277,     0,   278,   279,   280,
     273,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,     0,
     296,   297,   298,   271,     0,     0,     0,     0,     0,   272,
       0,   934,     0,     0,     0,     0,     0,     0,     0,   273,
       0,   274,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,   276,     0,     0,     0,     0,     0,     0,   273,     0,
     274,     0,     0,     0,   275,     0,   277,     0,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,     0,
     276,   296,   297,   298,     0,     0,     0,     0,     0,   274,
       0,     0,   988,   275,     0,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,   276,
     296,   297,   298,     0,     0,     0,     0,   271,     0,   592,
       0,     0,     0,   272,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   273,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,     0,     0,     0,     0,     0,     0,   904,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,     0,   357,     0,     0,     0,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,   276,   358,   359,   360,   361,   362,
     363,   273,     0,   364,   365,   366,   367,   368,   369,   277,
       0,   278,   279,   280,     0,   281,   282,   283,     0,   284,
     285,   286,   287,   288,   289,   290,   291,   292,     0,   293,
     294,   295,     0,     0,   296,   297,   298,     0,     0,     0,
     326,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,     0,   783,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,   272,
       0,     0,   276,   358,   359,   360,   361,   362,   363,   273,
       0,   364,   365,   366,   367,   368,   369,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,     0,   293,   294,   295,
       0,     0,   296,   297,   298,     0,     0,     0,   326,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,     0,   788,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,   272,     0,     0,
     276,   358,   359,   360,   361,   362,   363,   273,     0,   364,
     365,   366,   367,   368,   369,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,     0,
     296,   297,   298,     0,     0,     0,   326,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,     0,
     917,     0,     0,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,   276,   358,
     359,   360,   361,   362,   363,   273,     0,   364,   365,   366,
     367,   368,   369,   277,     0,   278,   279,   280,     0,   281,
     282,   283,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,     0,     0,   296,   297,
     298,     0,     0,     0,   326,     0,   274,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,     0,   918,     0,
       0,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,     0,     0,   272,     0,     0,   276,   358,   359,   360,
     361,   362,   363,   273,     0,   364,   365,   366,   367,   368,
     369,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,     0,   296,   297,   298,     0,
       0,     0,   326,     0,   274,     0,     0,     0,   275,     0,
       0,     0,     0,     0,     0,     0,   919,     0,     0,     0,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,   276,   358,   359,   360,   361,   362,
     363,   273,     0,   364,   365,   366,   367,   368,   369,   277,
       0,   278,   279,   280,     0,   281,   282,   283,     0,   284,
     285,   286,   287,   288,   289,   290,   291,   292,     0,   293,
     294,   295,     0,     0,   296,   297,   298,     0,     0,     0,
     326,     0,   274,     0,     0,     0,   275,     0,     0,     0,
       0,     0,     0,     0,   920,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,   272,
       0,     0,   276,   358,   359,   360,   361,   362,   363,   273,
       0,   364,   365,   366,   367,   368,   369,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,     0,   293,   294,   295,
       0,     0,   296,   297,   298,     0,     0,     0,   326,     0,
     274,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,     0,   936,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,   272,     0,     0,
     276,   358,   359,   360,   361,   362,   363,   273,     0,   364,
     365,   366,   367,   368,   369,   277,     0,   278,   279,   280,
       0,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,     0,
     296,   297,   298,     0,     0,     0,   326,     0,   274,     0,
       0,     0,   275,     0,     0,     0,     0,     0,     0,     0,
     937,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   276,   358,
     359,   360,   361,   362,   363,     0,     0,   364,   365,   366,
     367,   368,   369,   277,     0,   278,   279,   280,     0,   281,
     282,   283,     0,   284,   285,   286,   287,   288,   289,   290,
     291,   292,     0,   293,   294,   295,   271,     0,   296,   297,
     298,     0,   272,     0,   326,     0,     0,     0,     0,     0,
       0,     0,   273,   349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,   346,     0,     0,
       0,     0,     0,   274,     0,     0,     0,   275,   347,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     271,     0,     0,   276,     0,     0,   272,   274,     0,     0,
       0,   275,     0,     0,     0,     0,   273,   190,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   276,   293,   294,
     295,     0,     0,   296,   297,   298,     0,     0,     0,   326,
       0,     0,   277,     0,   278,   279,   280,   274,   281,   282,
     283,   275,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,   271,     0,   296,   297,   298,
       0,   272,     0,   326,     0,     0,     0,   276,     0,     0,
       0,   273,   675,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,   278,   279,   280,     0,   281,   282,
     283,     0,   284,   285,   286,   287,   288,   289,   290,   291,
     292,     0,   293,   294,   295,   271,     0,   296,   297,   298,
       0,   272,   274,   326,     0,     0,   275,     0,     0,     0,
       0,   273,   669,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   271,
       0,     0,   276,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,   277,     0,   278,
     279,   280,   274,   281,   282,   283,   275,   284,   285,   286,
     287,   288,   289,   290,   291,   292,     0,   293,   294,   295,
       0,     0,   296,   297,   298,     0,     0,     0,   326,   271,
       0,     0,   276,     0,     0,   272,   274,     0,     0,     0,
     275,     0,     0,     0,     0,   273,     0,   277,   687,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   276,   293,   294,   295,
       0,     0,   296,   297,   298,     0,     0,     0,   326,     0,
       0,   277,     0,   278,   279,   280,   274,   281,   282,   283,
     275,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,   271,     0,   296,   297,   298,     0,
     272,     0,   326,     0,     0,     0,   276,     0,     0,     0,
     273,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,   271,     0,   296,   297,   298,     0,
     272,   274,   397,     0,     0,   275,     0,     0,     0,     0,
     273,     0,     0,     0,     0,   354,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,   278,   279,
     280,   274,   281,   282,   283,   275,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,   271,
       0,   296,   297,   298,     0,   272,  -444,     0,     0,     0,
       0,   276,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,   328,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,   271,
     356,   296,   297,   298,     0,   272,   274,     0,     0,     0,
     275,     0,     0,     0,     0,   273,     0,     0,     0,     0,
     410,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   137,     0,     0,     0,   276,     0,     0,     0,
       0,     0,   411,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,   274,   281,   282,   283,
     275,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,   271,     0,   296,   297,   298,     0,
     272,     0,     0,     0,     0,     0,   276,     0,     0,     0,
     273,   506,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,   507,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,   271,     0,   296,   297,   298,     0,
     272,   274,     0,     0,     0,   275,     0,     0,     0,     0,
     273,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,     0,     0,     0,     0,     0,   271,     0,
       0,   276,     0,     0,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   273,     0,   277,     0,   278,   279,
     280,   274,   281,   282,   283,   275,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,     0,
       0,   296,   297,   298,   271,     0,     0,     0,     0,     0,
     272,   276,     0,     0,     0,   274,     0,     0,     0,   275,
     273,     0,     0,     0,     0,     0,   277,     0,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   276,   293,   294,   295,     0,
       0,   296,   297,   298,     0,     0,     0,     0,     0,     0,
     277,   274,   278,   279,   280,   275,   281,   282,   283,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,     0,
     293,   294,   295,   271,   401,   296,   297,   298,     0,   272,
       0,   276,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,   277,     0,   278,   279,
     280,     0,   281,   282,   283,     0,   284,   285,   286,   287,
     288,   289,   290,   291,   292,     0,   293,   294,   295,   271,
     403,   296,   297,   298,     0,   272,     0,     0,     0,     0,
     274,     0,     0,     0,   275,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   274,   278,   279,   280,
     275,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,   271,   405,
     296,   297,   298,     0,   272,     0,   276,     0,     0,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,   413,   296,   297,   298,     0,
       0,     0,     0,     0,     0,   274,     0,     0,     0,   275,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,   428,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,   278,   279,   280,     0,   281,   282,   283,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   274,
     293,   294,   295,   275,     0,   296,   297,   298,     0,     0,
       0,   510,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,   276,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,   274,     0,     0,     0,   275,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,   137,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,     0,     0,
       0,     0,   271,   682,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,   276,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,   659,   296,   297,   298,   274,
       0,     0,     0,   275,     0,     0,   271,     0,     0,     0,
       0,     0,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,   685,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   274,   293,   294,   295,   275,     0,   296,
     297,   298,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   273,     0,   276,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,   274,   296,   297,   298,   275,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,   773,     0,     0,     0,
       0,   726,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,   271,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   273,   776,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,   274,   296,   297,   298,   275,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,   278,   279,   280,     0,   281,   282,   283,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   274,
     293,   294,   295,   275,     0,   296,   297,   298,     0,     0,
       0,   849,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,   276,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,   274,     0,     0,     0,   275,     0,     0,   271,
     866,     0,     0,     0,   852,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,   271,     0,
       0,     0,     0,     0,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   273,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,   274,   296,   297,   298,   275,
       0,     0,   271,     0,     0,     0,     0,   953,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     277,     0,   278,   279,   280,     0,   281,   282,   283,     0,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   274,
     293,   294,   295,   275,     0,   296,   297,   298,     0,     0,
       0,   954,     0,     0,     0,   271,     0,     0,     0,     0,
       0,   272,     0,     0,     0,     0,     0,     0,     0,   276,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,     0,   293,   294,   295,     0,     0,   296,
     297,   298,   274,     0,     0,     0,   275,     0,     0,   271,
       0,     0,     0,     0,   955,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   277,     0,   278,
     279,   280,     0,   281,   282,   283,     0,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   274,   293,   294,   295,
     275,     0,   296,   297,   298,     0,     0,     0,   956,     0,
       0,     0,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,   276,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
       0,   293,   294,   295,     0,     0,   296,   297,   298,   274,
       0,     0,     0,   275,     0,     0,   271,     0,     0,     0,
       0,   957,   272,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,   278,   279,   280,     0,
     281,   282,   283,     0,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   274,   293,   294,   295,   275,     0,   296,
     297,   298,     0,     0,     0,   958,     0,     0,     0,   271,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,   276,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
     278,   279,   280,     0,   281,   282,   283,     0,   284,   285,
     286,   287,   288,   289,   290,   291,   292,     0,   293,   294,
     295,     0,     0,   296,   297,   298,   274,     0,     0,     0,
     275,     0,     0,   271,     0,     0,     0,     0,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     647,   293,   294,   295,   275,   273,   296,   297,   298,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,     0,     0,     0,   273,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   277,   274,   278,   279,   280,
     275,   281,   282,   283,     0,   284,   285,   286,   287,   288,
     289,   290,   291,   292,     0,   293,   294,   295,     0,   273,
     296,   297,   298,     0,     0,   274,   276,     0,     0,   275,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,   284,   285,   286,   287,   276,   289,   290,   291,   292,
     274,   293,   294,   295,   275,   273,   296,   297,   298,     0,
     277,     0,   278,   279,   280,     0,   281,   282,   283,     0,
     284,   285,   286,   287,     0,   289,   290,   291,   292,     0,
     293,     0,   295,     0,     0,   296,   297,   298,     0,     0,
       0,     0,     0,     0,     0,   277,   274,   278,   279,   280,
     275,   281,   282,   283,     0,   284,   285,   286,   287,     0,
     289,   290,   291,   292,     0,   293,     0,   295,     0,     0,
     296,   297,   298,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   277,     0,   278,   279,   280,     0,   281,   282,   283,
       0,     0,   285,   286,   287,     0,   289,   290,   291,   292,
       0,     0,     0,   295,     0,     0,   296,   297,   298
};

static const yytype_int16 yycheck[] =
{
       2,   374,     2,    46,    47,   160,   382,   406,   377,    11,
     319,   618,    14,    27,    49,    17,   171,    16,    20,    21,
      22,   253,    25,   703,   764,    27,    28,    29,    30,    64,
     406,     3,   562,     1,    36,    37,    38,   534,    40,     1,
      42,    43,   183,     1,    38,    27,    81,    49,     1,    51,
      52,    53,    54,     3,     3,    57,    58,    59,    60,   200,
       3,    63,    64,    65,    66,   886,     3,    69,     3,    54,
      72,    73,    74,    22,    76,    27,   729,    79,   219,    81,
     117,    83,   265,   266,   267,   268,   925,    27,   768,    38,
     136,    70,    41,    27,    42,     3,    54,     1,   144,    70,
     136,   803,    81,    93,   117,    90,    55,   144,    70,    58,
      93,    60,    91,   148,    38,    63,   646,    70,    68,    68,
      28,    38,    38,   117,   355,    38,     0,   807,    81,    91,
      98,   970,    90,   146,    38,    84,   138,    38,    91,    38,
     142,   143,   144,   145,   146,   147,   148,   644,   116,   117,
      85,     3,    68,    93,   116,    70,   977,   136,   148,   144,
     159,   143,    84,   116,   143,   117,   906,   907,   870,    93,
     700,   143,   143,   145,   315,   316,    84,     1,   146,     3,
     411,   143,   184,   136,   837,    18,   144,   117,   190,   869,
     143,   143,   561,   117,    98,   145,   145,     3,   201,    50,
     117,     7,   145,   143,   117,   858,   208,   209,   145,   143,
     145,    98,   116,   117,   144,     7,   117,   117,   117,    98,
      38,   136,   117,   117,   148,   534,   117,   117,   143,    80,
     117,   148,   136,    66,   121,   148,    42,   844,   117,    98,
      59,    60,   121,   245,   924,   245,   248,   148,   148,   148,
      68,   146,   146,    86,   437,   146,   146,    63,   399,   146,
     274,   275,   121,   145,   145,   147,   147,   146,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     117,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,    38,   795,   136,
     532,   533,    70,   317,   318,   117,   145,     3,     3,   344,
     117,     7,     7,    81,   328,   317,   318,   319,    22,   692,
      70,   117,   117,    91,   326,   694,   328,    22,   970,    70,
     332,    81,   334,   347,   146,   644,   350,   145,    22,   146,
     136,    91,   344,   117,   346,   347,   988,   349,   350,    77,
      91,   146,   354,   355,    58,   357,   729,    98,    52,   117,
     117,   145,   117,    58,    68,   597,   598,   145,   136,   371,
      22,    64,   146,    68,    58,   143,   117,   117,   136,   136,
     382,   136,   384,   136,    68,    89,   136,   136,   117,    84,
     392,    80,   117,   143,    89,   136,   136,    36,   433,    26,
      84,   136,   143,   975,   406,   146,    58,   136,   410,   411,
     424,   136,   984,    52,   813,   417,    68,   117,   142,    13,
      59,    60,   424,   425,   426,    19,   428,   429,   136,   136,
     806,   433,    84,   588,   136,    67,   136,   813,   136,    71,
      67,   119,   145,   145,    71,   147,   144,    92,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,    67,   145,   116,   147,    71,   119,
     136,   136,   845,   123,   420,   136,   422,   166,   136,   143,
     136,   175,   176,   177,    26,   858,   795,    98,   145,   116,
     145,   118,   119,   507,   183,   509,   123,   145,   136,   193,
       7,    48,   196,   130,   506,   507,   508,   509,   510,   511,
     512,   200,   139,   143,   513,   514,   116,   136,    98,   158,
     146,   215,   524,   136,   526,    67,   902,   117,   530,    71,
     219,   145,   534,    38,    29,   570,   175,   176,   177,   146,
     117,   235,   544,   182,   146,    98,    17,   146,   146,   925,
     117,   148,   143,    38,   193,   136,   148,   196,   934,    22,
     136,   563,   136,   565,   566,   567,   568,   569,   570,   420,
     136,   422,   136,   136,   116,    38,   215,   119,    41,   136,
     136,   123,   136,    38,   136,    70,   136,   136,   136,   117,
     136,   136,    55,   136,   970,    58,   235,    60,     3,   975,
     602,   136,     7,   136,     9,    68,    91,   146,   984,   136,
      29,   136,   988,    98,   146,    70,   146,    22,   144,   136,
     143,    84,   144,   117,    98,    68,   315,   316,    27,    27,
     148,   143,   117,   647,   648,   329,    91,    29,   116,    68,
     148,   116,   644,    98,   148,   647,   648,   649,   148,   119,
     652,   136,   144,    58,   119,    26,   145,    29,   143,   353,
     146,   146,   117,    68,    91,   667,   146,   669,   519,    81,
     672,   144,   148,   675,   727,   710,   227,   411,   680,    84,
     682,   136,   684,   685,    89,   687,   714,   410,   143,   172,
     329,   146,   523,   781,   732,   844,    67,   845,   554,   692,
      71,   735,   977,   392,   924,   949,    29,   709,   710,   385,
     399,   521,    -1,    -1,   353,    -1,    -1,    -1,    -1,   721,
      -1,   723,   573,    -1,   726,   727,    -1,    -1,   417,   731,
      -1,   420,    -1,   422,    -1,    -1,   425,    -1,    -1,    -1,
      -1,   112,     2,   114,   115,   116,    -1,   118,   119,    -1,
      -1,    11,   123,    -1,    -1,    -1,    -1,    -1,   760,   130,
      20,    -1,    22,    -1,    -1,    -1,   137,   138,   139,    -1,
     772,   773,    -1,   775,   776,   777,   778,    -1,    -1,    -1,
      -1,   783,   784,    -1,   786,    -1,   788,   789,    -1,    -1,
     792,   834,   835,   795,    -1,    -1,    -1,    57,    -1,    -1,
     802,    -1,    -1,    -1,   806,    -1,    -1,    -1,    -1,    -1,
     812,   813,   814,    -1,    -1,    26,   818,    -1,   820,    79,
      -1,   823,   824,    83,    -1,   827,    -1,    -1,    -1,   831,
     519,   831,   521,    -1,   528,   529,    -1,    -1,   532,   533,
      -1,    -1,    -1,   878,    -1,    -1,    -1,   849,   850,   851,
     852,   853,   854,   855,   856,    -1,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,   866,   867,    -1,    -1,    -1,   871,
      -1,   873,   874,   875,   876,   877,   878,    -1,   138,    -1,
     882,    -1,    -1,   885,    -1,    -1,    -1,    -1,    -1,   528,
     529,    -1,    -1,   532,   533,    -1,    -1,    -1,    -1,    -1,
     902,   112,    -1,    -1,   115,   116,   545,   118,   119,    -1,
     945,    -1,   123,    -1,    -1,   917,   918,   919,   920,   130,
      -1,    -1,    -1,   925,   184,    -1,   137,   138,   139,    -1,
     190,    -1,   934,    -1,   936,   937,   938,    -1,   940,   941,
     942,   943,   944,   945,    -1,   584,    -1,    -1,    -1,    -1,
      -1,   953,   954,   955,   956,   957,   958,    -1,    -1,    -1,
      21,    -1,    -1,   657,   658,    -1,    -1,   969,   970,    -1,
     664,    32,    33,   975,    22,    -1,    -1,    -1,    -1,    40,
     674,    42,   984,   677,    -1,   245,   988,    48,    -1,    -1,
      38,    -1,    -1,    41,    -1,    56,     2,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    11,    -1,    55,    -1,    -1,
      58,    -1,    60,    -1,    20,    -1,    22,    -1,   657,   658,
      68,    -1,    -1,    26,     2,   664,     1,   666,    -1,    -1,
      -1,    -1,   671,    11,    -1,   674,    84,    -1,   677,    -1,
      -1,    -1,    20,    18,    22,    -1,    21,    22,    -1,    -1,
      -1,    57,    -1,    59,    60,    30,    -1,    32,    33,    26,
      -1,    36,    -1,    -1,    67,    40,   326,    42,    71,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    83,   717,    57,
      -1,    56,    -1,    58,    -1,    60,   346,    62,    63,   349,
      65,    66,    -1,    68,    -1,    -1,    -1,   357,    -1,    -1,
      67,    79,    -1,    -1,    71,    83,    -1,    -1,    -1,    84,
      -1,    86,    -1,   116,    89,   118,   119,   806,    -1,    -1,
     123,    -1,   816,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,   138,    -1,   137,   138,   139,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,    -1,   123,    -1,    -1,    -1,
     138,   136,   129,   130,    -1,     2,   426,   134,    21,   144,
     137,   138,   139,    -1,    11,    -1,    -1,   816,   184,    32,
      33,    -1,    -1,    20,   190,    22,    -1,    40,    -1,    42,
      -1,    -1,    -1,   882,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,   184,    -1,    -1,     1,
      63,    64,   190,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      57,   860,   861,    -1,   863,   864,    18,    32,    33,    21,
      22,    -1,    -1,    -1,    -1,    40,   925,    42,    30,   245,
      32,    33,    79,   503,    36,    -1,    83,   886,    40,    -1,
      42,    56,    -1,   892,    -1,   894,    -1,   896,    63,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,   245,    60,    -1,
      62,    63,    -1,    65,    66,    -1,    68,    -1,    -1,    -1,
      26,   970,    -1,    -1,   544,    -1,    -1,    -1,    18,    -1,
      -1,    21,    84,    -1,    86,    -1,    -1,    89,    -1,   988,
      30,   138,    32,    33,    -1,    -1,    36,    -1,    -1,    -1,
      40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     326,    67,    -1,    -1,    -1,    71,    56,    -1,    -1,    -1,
      60,    -1,    62,    63,    -1,    65,    66,    -1,   977,    -1,
     346,    -1,   602,   349,   136,    -1,    -1,   184,   326,    -1,
      -1,   357,   144,   190,    84,    -1,    86,    -1,   618,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   346,    -1,
     116,   349,   118,   119,    -1,    -1,    -1,   123,    -1,   357,
      -1,    -1,    -1,    -1,   130,    -1,    -1,   647,   648,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   136,   667,   245,   669,
      -1,    -1,   672,    -1,   144,   675,    -1,    -1,    -1,    -1,
     426,    -1,    -1,    -1,   684,    -1,    -1,   687,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    21,    -1,    -1,    -1,    -1,   426,   709,
      -1,    -1,    30,    -1,    32,    33,     2,    -1,    36,    -1,
      -1,   721,    40,   723,    42,    11,    -1,   727,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    22,    -1,    56,    -1,
      -1,    -1,    60,    -1,    62,    63,    -1,    65,    66,   326,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    86,   346,
      -1,    57,   349,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     357,    -1,    -1,   783,    -1,    -1,    -1,    -1,   788,    -1,
      -1,    -1,     2,    79,    -1,    -1,    -1,    83,   544,    -1,
      -1,    11,   802,    -1,     3,     4,     5,     6,     7,     8,
      20,    -1,    22,    12,   814,    -1,    15,    -1,   136,    -1,
      -1,    -1,    -1,   823,    -1,    -1,   544,   827,    -1,    28,
      -1,   831,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,
      39,    -1,    -1,    -1,    -1,    44,    45,    57,    47,   426,
      49,    50,   138,    -1,    -1,    -1,   602,    -1,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    -1,   867,    -1,    79,
      -1,    -1,    -1,    83,    -1,    74,    75,    76,    77,    78,
      -1,    -1,    -1,    -1,   602,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,
      -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,   917,   918,   919,
     920,   667,    -1,   669,    -1,    -1,   672,    -1,   138,   675,
     129,    -1,   131,    -1,    -1,   134,   936,   937,   684,    -1,
     139,   687,    -1,    -1,   143,    -1,   145,    -1,   147,   667,
      -1,   669,    -1,     2,   672,    -1,    -1,   675,    -1,   245,
      -1,    -1,    11,   709,    -1,    -1,   684,   544,    -1,   687,
      -1,    20,    -1,    22,   184,   721,    -1,   723,    -1,    -1,
     190,   727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   721,    -1,   723,    -1,    -1,    57,   727,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   602,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,   245,    -1,   783,    -1,    -1,
     326,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,    -1,   802,   145,    -1,   147,
     346,    -1,    -1,   349,    -1,   783,    -1,    -1,   814,    -1,
     788,   357,    -1,    26,    -1,    -1,    -1,   823,    -1,    -1,
      -1,   827,    -1,    -1,   802,   831,    -1,    -1,    -1,   138,
     667,    -1,   669,    -1,    -1,   672,   814,    -1,   675,    -1,
      -1,    -1,    -1,    -1,    -1,   823,    -1,   684,    -1,   827,
     687,    -1,    -1,   831,    67,    -1,   326,    -1,    71,    -1,
      -1,   867,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   709,    -1,    -1,   184,   346,    -1,    -1,   349,
     426,   190,    -1,    -1,   721,    -1,   723,   357,    -1,   867,
     727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,   115,   116,    -1,   118,   119,    -1,    -1,    -1,
     123,   917,   918,   919,   920,    -1,   129,   130,    -1,    -1,
      -1,   134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     936,   937,    -1,    -1,    -1,    -1,   245,    -1,    -1,   917,
     918,   919,   920,    -1,    -1,    -1,   783,    -1,    -1,    -1,
      -1,   788,     2,    -1,    -1,    -1,   426,    -1,   936,   937,
      -1,    11,    -1,    -1,    -1,   802,    -1,    -1,    -1,    -1,
      20,    -1,    22,    -1,    -1,    -1,    -1,   814,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    -1,   823,    -1,    -1,    -1,
     827,    -1,    -1,    11,   831,    -1,    -1,    -1,   544,    -1,
      -1,    -1,    20,    -1,    22,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   326,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
     867,    -1,    -1,    83,    -1,    -1,    -1,   346,    -1,    57,
     349,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   357,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   602,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   544,    -1,    -1,    -1,    -1,    -1,
     917,   918,   919,   920,    -1,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   936,
     937,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,    -1,   426,    -1,    -1,
     138,   667,    -1,   669,    -1,    -1,   672,    -1,    -1,   675,
      -1,    -1,   602,    -1,   184,    -1,    -1,    -1,   684,    -1,
     190,   687,    -1,   140,   141,    -1,    -1,    -1,   145,    -1,
     147,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    21,
      -1,    -1,    -1,   709,    -1,    -1,   184,    -1,    30,    -1,
      32,    33,   190,    -1,    36,   721,    -1,   723,    40,    -1,
      42,   727,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,   245,    -1,   667,    60,   669,
      62,    63,   672,    65,    66,   675,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   684,    -1,    -1,   687,    -1,    -1,
      -1,    -1,    84,    -1,    86,    -1,    -1,   245,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   544,    -1,   783,    -1,   709,
      -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   721,    -1,   723,    -1,    -1,   802,   727,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   814,    -1,
      -1,    -1,    -1,    -1,   136,    -1,   326,   823,    -1,    -1,
      -1,   827,   144,    -1,    -1,   831,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   602,    -1,    -1,   346,    -1,    -1,   349,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   357,   326,    -1,
      -1,    -1,    -1,   783,    -1,    -1,    -1,    -1,   788,    -1,
      -1,   867,    -1,    -1,    -1,    -1,    -1,    -1,   346,    -1,
      -1,   349,   802,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   814,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   823,    -1,    -1,    -1,   827,   667,    -1,
     669,   831,    -1,   672,    -1,    -1,   675,    -1,    -1,    -1,
      -1,   917,   918,   919,   920,   684,   426,    -1,   687,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     936,   937,    -1,    -1,    -1,    -1,    -1,   867,    -1,    -1,
     709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   426,    -1,
      -1,    -1,   721,    -1,   723,    -1,    -1,    -1,   727,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,   917,   918,   919,
     920,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,   936,   937,    -1,    -1,
      44,    45,    -1,    47,   783,    49,    50,    -1,    -1,   788,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,   802,   544,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,   814,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,   823,    -1,    -1,    -1,   827,    -1,
      -1,    -1,   831,    97,    -1,    -1,   544,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,   602,    -1,    -1,   129,    -1,   131,   867,   133,
     134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      13,   145,    -1,   147,   148,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,   602,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,   918,
     919,   920,    -1,    -1,    -1,    -1,    -1,   667,    61,   669,
      -1,    -1,   672,    -1,    -1,   675,    -1,   936,   937,    -1,
      -1,    -1,    -1,    -1,   684,    -1,    -1,   687,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   667,
      -1,   669,    -1,    -1,   672,    -1,    -1,   675,    -1,   709,
      -1,   104,    -1,   106,    -1,    -1,   684,    -1,    -1,   687,
      -1,   721,    -1,   723,    -1,    -1,    -1,   727,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   721,    -1,   723,    -1,    -1,    -1,   727,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,   171,    -1,
     173,    -1,    -1,   783,    -1,    -1,    -1,    -1,   788,    -1,
     183,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   802,    -1,    -1,    -1,    -1,   200,   201,    -1,
      -1,   204,    -1,    -1,   814,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   823,    -1,    -1,   219,   827,    -1,    -1,
      -1,   831,    -1,    -1,   802,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,   814,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,   823,    -1,    -1,    -1,   827,
      -1,    -1,    -1,   831,    -1,    -1,    -1,   867,    -1,    -1,
      -1,    -1,   265,   266,   267,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   282,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,   867,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   917,   918,   919,
     920,    -1,   315,   316,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,   102,   103,   936,   937,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,   342,
     118,    -1,    -1,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,   134,    -1,    -1,   137,
     138,   139,   140,    -1,   142,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,    -1,   377,    98,    99,   100,   101,   102,
     103,    -1,   385,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,   399,    -1,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
      -1,   134,    -1,    -1,   137,   138,   139,   140,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   432,
      -1,    -1,   435,    -1,   437,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    27,    28,    -1,    30,    -1,    32,
      33,    34,    35,    36,    37,    -1,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    -1,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    -1,    80,   521,    82,
      83,    84,    -1,    86,    87,    88,    89,    -1,    -1,    92,
      -1,    94,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,   557,    -1,    -1,   120,   561,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,    -1,   136,    -1,    -1,   139,    -1,    -1,    -1,
     143,   584,   145,    -1,   147,   588,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,   618,    -1,    -1,    -1,    34,
      35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,
      45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,   691,    -1,
      -1,   694,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,   714,    -1,    -1,   129,    -1,   131,    -1,   133,   134,
     135,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,   732,
     145,    -1,   147,    -1,    -1,     1,   739,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
     833,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   844,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
     136,   137,   138,   139,    -1,    -1,    -1,   143,   144,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,   135,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,   144,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,   144,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,   144,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,   144,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,    -1,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
       1,   147,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    -1,    30,
      -1,    32,    33,    34,    35,    36,    37,    -1,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    80,
      -1,    82,    83,    84,    -1,    86,    87,    88,    89,    -1,
      -1,    92,    -1,    94,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,   120,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,
     131,    -1,   133,   134,    -1,   136,    -1,    -1,   139,    -1,
      -1,    -1,   143,    -1,   145,     1,   147,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    -1,    80,    -1,    82,    83,    84,    -1,
      86,    87,    88,    89,    -1,    -1,    92,    -1,    94,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,     3,     4,     5,
       6,     7,     8,   129,    -1,   131,    12,   133,   134,    15,
     136,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
      -1,    37,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,
      76,    77,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,   131,    -1,   133,   134,   135,
      -1,    -1,    -1,   139,    -1,    -1,    -1,   143,    -1,   145,
      -1,   147,   148,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    34,    35,    -1,    37,    -1,    39,
      -1,    -1,    -1,    -1,    44,    45,    -1,    47,    -1,    49,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    59,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,   131,    -1,   133,   134,   135,    -1,    -1,    -1,   139,
      -1,    -1,    -1,   143,    -1,   145,    -1,   147,   148,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,   133,
     134,    -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,   143,
      -1,   145,    -1,   147,   148,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    82,
      83,    -1,    85,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    85,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,   146,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    38,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,   136,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    82,
      83,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    21,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    33,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,   144,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,     3,     4,     5,     6,     7,
       8,   129,    -1,   131,    12,   133,   134,    15,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,   131,    -1,
     133,   134,   135,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   129,    -1,   131,    -1,   133,   134,   135,    -1,    -1,
      -1,   139,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
     143,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,    -1,    -1,
       3,     4,     5,     6,     7,     8,   129,    -1,   131,    12,
     133,   134,    15,    -1,    -1,    -1,   139,    -1,    -1,    -1,
      -1,    -1,   145,    -1,   147,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,   121,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,   131,    -1,
      12,    -1,   135,    15,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,   145,   146,   147,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    34,    35,    -1,    37,    -1,    39,    -1,    -1,
      -1,    -1,    44,    45,    -1,    47,    -1,    49,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    59,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    -1,   131,
      -1,    -1,    -1,    -1,   136,    -1,    -1,    -1,    -1,    -1,
      28,   143,    -1,   145,    -1,   147,    34,    35,    -1,    37,
      -1,    39,    -1,    -1,    -1,    -1,    44,    45,    -1,    47,
      -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    -1,     3,     4,     5,     6,     7,     8,    -1,    87,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    34,    35,   113,    37,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    10,    47,    -1,    49,    50,
      -1,    16,    -1,   131,    -1,    -1,    57,    -1,    59,    -1,
      61,    26,    -1,    -1,    -1,   143,    -1,   145,    -1,   147,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    93,    16,
     131,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    67,    -1,    -1,   145,    71,   147,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    97,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,   148,    71,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      97,   137,   138,   139,    10,    -1,    -1,    -1,    -1,    -1,
      16,    -1,   148,    -1,    -1,   112,    -1,   114,   115,   116,
      26,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    -1,
     137,   138,   139,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      67,    -1,    -1,    -1,    71,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      97,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,   148,    71,    -1,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    97,
     137,   138,   139,    -1,    -1,    -1,    -1,    10,    -1,   146,
      -1,    -1,    -1,    16,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    26,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    -1,    -1,    -1,    -1,    -1,    -1,   146,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    26,    -1,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    26,
      -1,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    26,    -1,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    26,    -1,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    -1,    -1,   137,   138,
     139,    -1,    -1,    -1,   143,    -1,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    97,    98,    99,   100,
     101,   102,   103,    26,    -1,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    -1,   137,   138,   139,    -1,
      -1,    -1,   143,    -1,    67,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    97,    98,    99,   100,   101,   102,
     103,    26,    -1,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,
     143,    -1,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    97,    98,    99,   100,   101,   102,   103,    26,
      -1,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      97,    98,    99,   100,   101,   102,   103,    26,    -1,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,   143,    -1,    67,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,   103,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,   134,    10,    -1,   137,   138,
     139,    -1,    16,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      10,    -1,    -1,    97,    -1,    -1,    16,    67,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    26,    27,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    97,   132,   133,
     134,    -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,
      -1,    -1,   112,    -1,   114,   115,   116,    67,   118,   119,
     120,    71,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    10,    -1,   137,   138,   139,
      -1,    16,    -1,   143,    -1,    -1,    -1,    97,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,   125,   126,   127,   128,   129,
     130,    -1,   132,   133,   134,    10,    -1,   137,   138,   139,
      -1,    16,    67,   143,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    10,
      -1,    -1,    97,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,   112,    -1,   114,
     115,   116,    67,   118,   119,   120,    71,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    10,
      -1,    -1,    97,    -1,    -1,    16,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    26,    -1,   112,    79,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    97,   132,   133,   134,
      -1,    -1,   137,   138,   139,    -1,    -1,    -1,   143,    -1,
      -1,   112,    -1,   114,   115,   116,    67,   118,   119,   120,
      71,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    -1,   143,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    67,   143,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    67,   118,   119,   120,    71,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    10,
      -1,   137,   138,   139,    -1,    16,   142,    -1,    -1,    -1,
      -1,    97,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    38,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    10,
     136,   137,   138,   139,    -1,    16,    67,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    67,   118,   119,   120,
      71,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    38,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    10,    -1,   137,   138,   139,    -1,
      16,    67,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    97,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,   112,    -1,   114,   115,
     116,    67,   118,   119,   120,    71,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    -1,
      -1,   137,   138,   139,    10,    -1,    -1,    -1,    -1,    -1,
      16,    97,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      26,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    97,   132,   133,   134,    -1,
      -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    67,   114,   115,   116,    71,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,   133,   134,    10,   136,   137,   138,   139,    -1,    16,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,    10,
     136,   137,   138,   139,    -1,    16,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    71,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    67,   114,   115,   116,
      71,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    10,   136,
     137,   138,   139,    -1,    16,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    67,
     132,   133,   134,    71,    -1,   137,   138,   139,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    93,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,   136,   137,   138,   139,    67,
      -1,    -1,    -1,    71,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    29,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    67,   132,   133,   134,    71,    -1,   137,
     138,   139,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,    67,   137,   138,   139,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    67,   137,   138,   139,    71,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    67,
     132,   133,   134,    71,    -1,   137,   138,   139,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      11,    -1,    -1,    -1,    79,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    67,   137,   138,   139,    71,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    79,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    67,
     132,   133,   134,    71,    -1,   137,   138,   139,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    -1,   132,   133,   134,    -1,    -1,   137,
     138,   139,    67,    -1,    -1,    -1,    71,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    79,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    67,   132,   133,   134,
      71,    -1,   137,   138,   139,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,   133,   134,    -1,    -1,   137,   138,   139,    67,
      -1,    -1,    -1,    71,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    79,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
     128,   129,   130,    67,   132,   133,   134,    71,    -1,   137,
     138,   139,    -1,    -1,    -1,    79,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,    -1,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,    -1,    -1,   137,   138,   139,    67,    -1,    -1,    -1,
      71,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      67,   132,   133,   134,    71,    26,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    67,   114,   115,   116,
      71,   118,   119,   120,    -1,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   133,   134,    -1,    26,
     137,   138,   139,    -1,    -1,    67,    97,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,   122,   123,   124,   125,    97,   127,   128,   129,   130,
      67,   132,   133,   134,    71,    26,   137,   138,   139,    -1,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,    -1,
     132,    -1,   134,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    67,   114,   115,   116,
      71,   118,   119,   120,    -1,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,    -1,   134,    -1,    -1,
     137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,    -1,    -1,   134,    -1,    -1,   137,   138,   139
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   153,   154,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    15,    18,    19,    20,    21,    23,
      24,    25,    27,    28,    30,    32,    33,    34,    35,    36,
      37,    39,    40,    43,    44,    45,    46,    47,    49,    50,
      52,    56,    57,    59,    60,    61,    62,    65,    66,    69,
      70,    72,    73,    74,    75,    76,    77,    78,    80,    82,
      83,    84,    86,    87,    88,    92,    94,    97,   112,   113,
     114,   115,   120,   129,   131,   133,   134,   136,   139,   143,
     145,   147,   155,   156,   157,   158,   159,   161,   166,   167,
     168,   170,   173,   174,   175,   176,   177,   178,   180,   181,
     182,   186,   187,   190,   191,   194,   195,   198,   201,   202,
     225,   228,   229,   249,   250,   251,   252,   253,   254,   262,
     263,   264,   265,   267,   268,   269,   270,   271,   272,   274,
     275,   276,   277,   278,   136,   157,   265,    93,   255,   256,
     169,   170,    12,    34,    35,    37,    78,   143,   147,   198,
     249,   253,   263,   264,   265,   267,   269,   270,   255,   265,
      84,   169,   157,   243,   265,   157,   145,     7,   169,   171,
       9,    84,   171,    58,    95,   179,   265,   265,    21,    33,
     228,   265,   265,   145,   170,   199,   145,   170,   230,   231,
      27,   161,   172,   265,   265,   265,   265,   265,   265,     7,
     145,    33,   175,   175,   135,   221,   242,   265,    82,    83,
     136,   265,   266,   265,   172,   265,   265,   265,    77,   145,
     157,   265,   265,   161,   168,   265,   267,   161,   168,   265,
     170,   226,   265,   242,   265,   265,   265,   265,   265,   265,
     265,   265,     1,   144,   155,   162,   242,    85,   121,   221,
     244,   245,   266,   242,   265,   273,    60,   157,    64,   160,
     170,   170,    42,    63,   216,    22,    58,    68,    89,   136,
     142,    10,    16,    26,    67,    71,    97,   112,   114,   115,
     116,   118,   119,   120,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   132,   133,   134,   137,   138,   139,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   140,   141,   145,   147,    67,    71,   145,
     157,   136,   265,   265,   265,   242,   143,   161,    38,   255,
     226,   136,   117,   136,    92,   170,   221,   246,   247,   248,
     266,   226,   191,   170,   145,   172,    27,    38,   172,    27,
      38,    93,   172,   258,    31,    53,   136,    79,    98,    99,
     100,   101,   102,   103,   106,   107,   108,   109,   110,   111,
     161,   208,   246,   157,   145,   210,    85,   145,   170,   232,
     233,     1,   116,   237,    38,   117,   157,   172,   172,   246,
     171,   170,   117,   136,   265,   265,   136,   143,   172,   145,
     246,   136,   183,   136,   183,   136,    98,   227,   136,   136,
      31,    53,   172,   136,   144,   144,   155,   117,   144,   265,
     117,   146,   117,   146,    38,   117,   148,   258,    96,   117,
     148,     7,    48,   116,   192,   143,   203,    68,   230,   230,
     230,   230,   265,   265,   265,   265,   179,   265,   179,   265,
     265,   265,   265,   265,   265,   265,    28,    84,   170,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   246,   246,   179,   265,   179,   265,    22,    38,    68,
      89,   257,   260,   261,   265,   278,    27,    38,    27,    38,
      79,    38,   148,   179,   265,   172,   136,   265,   265,    98,
     146,   117,   136,   170,    38,   242,    37,   265,   179,   265,
      37,   265,   179,   265,   145,   172,   139,   163,   165,   265,
     163,   164,   157,   265,    29,   265,   146,    22,    38,    41,
      55,    58,    68,    84,   156,   211,   212,   213,   214,   200,
     233,   117,   146,    15,    28,    57,    74,    75,    78,    87,
     147,   238,   250,    98,   234,   265,   231,   146,   221,   265,
       1,   188,   246,   146,    17,   184,   238,   250,   117,   165,
     164,   144,   146,   146,   244,   146,   244,   179,   265,   148,
     157,   265,   148,   265,   148,   265,   170,   242,   143,     1,
     170,   196,   197,    22,    58,    68,    84,   205,   215,   230,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     146,   148,    38,    68,   117,   146,   249,    67,    67,    37,
     179,   265,    37,   179,   265,   179,   265,   255,   255,   136,
     221,   266,   248,   192,   265,   146,   265,    27,   172,    27,
     172,   265,    27,   172,   258,    27,   172,   258,   259,   260,
     117,   136,    11,   136,    29,    29,   157,    79,   161,    38,
      68,   213,   117,   146,   145,   170,    22,    58,    68,    84,
     217,   146,   233,   237,     1,   242,    51,   266,   144,    54,
      90,   144,   189,   146,   145,   161,   170,   185,   226,   136,
     136,   148,   258,   148,   258,   157,    96,   143,     1,   193,
     144,    98,   117,   144,    68,   204,    10,    16,   112,   113,
     114,   115,   118,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   134,   137,   138,   139,   140,   142,
     145,   170,   206,   207,   208,   249,   136,   260,   237,   249,
     249,   265,    27,    27,   265,    27,    27,   148,   148,   172,
     172,   143,   172,    79,    37,   265,    37,   265,    79,    37,
     265,   172,    37,   265,   172,   117,   146,   265,   265,   157,
     265,   157,    29,   170,   212,   233,   116,   241,    68,   237,
     234,   148,    38,   148,    27,   157,   242,   185,   116,   161,
      37,   157,   265,   148,    37,   157,   265,   148,   265,     1,
     144,   162,   144,    33,    62,    65,   144,   156,   174,   265,
     197,   217,   170,   265,   119,   145,   209,   209,   234,    79,
      37,    37,    79,    37,    37,    37,    37,     1,   193,   265,
     265,   265,   265,   265,   265,   260,    11,    29,   157,   241,
     146,    15,    28,    57,    74,    75,    78,    87,   147,   221,
     240,   250,   121,   223,   234,    91,   224,   265,   238,   250,
     157,   172,   146,   265,   265,   157,   265,   157,   144,   144,
     174,     1,   116,   236,   146,   170,   207,   208,   211,   265,
     265,   265,   265,   265,   265,   144,   144,    79,    79,    79,
      79,   265,   157,   234,   241,   148,   242,   221,   222,   265,
     265,   161,   173,   220,   148,   161,    79,    79,    15,    28,
      57,    74,    75,    78,    87,   147,   235,   250,    81,   218,
     209,   209,   146,    79,    79,    79,    79,    79,    79,   265,
     265,   265,   265,   223,   234,   221,   239,   240,   250,    38,
     148,   250,   265,   265,     1,   148,   242,   224,   265,   239,
     240,   148,   235,   250,   148,   136,   219,   220,   148,   235,
     250,   239
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   152,   153,   154,   154,   155,   155,   156,   156,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     158,   158,   159,   159,   159,   160,   160,   161,   161,   161,
     162,   162,   163,   163,   163,   163,   164,   164,   165,   165,
     166,   166,   166,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     169,   169,   170,   171,   171,   172,   172,   173,   173,   174,
     174,   174,   174,   174,   174,   174,   174,   175,   175,   176,
     176,   176,   176,   177,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   179,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   182,   182,
     182,   182,   183,   183,   184,   184,   184,   185,   185,   186,
     187,   187,   188,   188,   189,   189,   189,   190,   190,   190,
     190,   191,   191,   191,   192,   192,   193,   193,   193,   194,
     194,   195,   196,   196,   196,   197,   197,   199,   200,   198,
     201,   201,   201,   201,   201,   203,   204,   202,   205,   205,
     205,   205,   206,   206,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   211,   211,   211,   212,   212,
     212,   212,   212,   213,   213,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   215,   215,   215,   215,   215,   215,
     216,   216,   217,   217,   217,   217,   217,   217,   218,   218,
     219,   219,   220,   220,   221,   221,   222,   222,   223,   223,
     224,   224,   225,   225,   225,   226,   226,   227,   227,   227,
     228,   228,   228,   228,   228,   229,   229,   229,   230,   230,
     231,   231,   232,   232,   232,   233,   233,   233,   234,   234,
     234,   235,   235,   235,   235,   235,   235,   235,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     238,   238,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   242,   242,   242,   242,   243,   243,   244,
     244,   244,   245,   245,   246,   246,   247,   247,   248,   248,
     248,   248,   249,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   252,   253,   254,   254,
     254,   254,   254,   254,   254,   254,   255,   255,   256,   257,
     257,   258,   259,   259,   260,   260,   260,   261,   261,   261,
     261,   261,   261,   262,   262,   263,   264,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   266,   266,   266,   267,   267,   267,   267,   268,   268,
     269,   269,   269,   270,   270,   270,   271,   271,   271,   271,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   273,   273,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   275,   275,   275,   275,
     275,   275,   276,   276,   276,   276,   277,   277,   277,   277,
     278,   278,   278,   278,   278,   278,   278
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
       0,     1,     1,     1,     1,     2,     1,     2,     3,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     2,     2,     2,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
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
#line 447 "chapel.ypp" /* yacc.c:1663  */
    { yyblock = (yyval.pblockstmt); }
#line 5113 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 3:
#line 452 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 5119 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 4:
#line 453 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5125 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 6:
#line 460 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5131 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 7:
#line 465 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5137 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 8:
#line 466 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5143 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 23:
#line 485 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5149 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 24:
#line 486 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5155 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 25:
#line 487 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5161 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 26:
#line 488 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5167 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 27:
#line 489 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5173 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 28:
#line 490 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5179 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 29:
#line 491 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5185 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 30:
#line 492 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5191 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 31:
#line 493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5197 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 32:
#line 494 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5203 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 33:
#line 495 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5209 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 34:
#line 496 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5215 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 35:
#line 497 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5221 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 36:
#line 498 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5227 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 37:
#line 499 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5233 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 38:
#line 500 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5239 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 39:
#line 505 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 40:
#line 508 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5252 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 41:
#line 510 "chapel.ypp" /* yacc.c:1663  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 42:
#line 515 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 43:
#line 516 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 44:
#line 517 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 45:
#line 521 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 5283 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 46:
#line 522 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 5289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 47:
#line 535 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5295 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 48:
#line 536 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5301 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 49:
#line 537 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5307 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 50:
#line 542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5319 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 52:
#line 548 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5329 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 53:
#line 553 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5339 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 54:
#line 558 "chapel.ypp" /* yacc.c:1663  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5348 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 55:
#line 562 "chapel.ypp" /* yacc.c:1663  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5357 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 56:
#line 569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5367 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 58:
#line 578 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5377 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 60:
#line 587 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5383 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 61:
#line 588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5389 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 62:
#line 589 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5395 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 63:
#line 593 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5401 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 64:
#line 598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5407 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 65:
#line 600 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5413 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 66:
#line 602 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5419 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 67:
#line 604 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5425 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 68:
#line 606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5431 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 69:
#line 608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5437 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 70:
#line 610 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5443 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 71:
#line 612 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5449 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 72:
#line 614 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5455 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 73:
#line 616 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5461 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 74:
#line 618 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5467 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 75:
#line 620 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5473 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 76:
#line 622 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5479 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 77:
#line 624 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5485 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 78:
#line 626 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5491 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 79:
#line 628 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5497 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 80:
#line 632 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = NULL; }
#line 5503 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 82:
#line 637 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5509 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 83:
#line 641 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5515 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 84:
#line 642 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5521 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 85:
#line 646 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5527 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 86:
#line 647 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5533 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 87:
#line 651 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5539 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 88:
#line 652 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5545 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 89:
#line 656 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5551 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 91:
#line 658 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5557 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 92:
#line 659 "chapel.ypp" /* yacc.c:1663  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5563 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 99:
#line 672 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5569 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 100:
#line 673 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5575 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 101:
#line 674 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5581 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 102:
#line 675 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5587 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 103:
#line 680 "chapel.ypp" /* yacc.c:1663  */
    {
#ifdef HAVE_LLVM
      if (externC) {
        (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
      } else {
        USR_FATAL(new BlockStmt(), "extern block syntax is turned off. Use --extern-c flag to turn on.");
      }
#else
      USR_FATAL(new BlockStmt(), "Chapel must be built with llvm in order to use the extern block syntax");
#endif
    }
#line 5603 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 104:
#line 694 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5609 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 105:
#line 695 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5615 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 106:
#line 696 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5621 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 107:
#line 697 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5627 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 108:
#line 698 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5633 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 109:
#line 699 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5639 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 110:
#line 700 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5645 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 111:
#line 701 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 112:
#line 702 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5657 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 113:
#line 703 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5663 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 114:
#line 704 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5669 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 115:
#line 705 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5675 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 116:
#line 706 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 117:
#line 707 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5687 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 118:
#line 708 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5693 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 119:
#line 709 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5699 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 120:
#line 711 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 121:
#line 717 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5719 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 122:
#line 723 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5729 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 123:
#line 729 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 124:
#line 735 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5750 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 125:
#line 742 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5761 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 126:
#line 751 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5767 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 127:
#line 755 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5773 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 128:
#line 756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5779 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 129:
#line 757 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5785 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 130:
#line 758 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5791 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 131:
#line 759 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5798 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 132:
#line 761 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5805 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 133:
#line 763 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5812 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 134:
#line 765 "chapel.ypp" /* yacc.c:1663  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5819 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 135:
#line 770 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 5825 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 136:
#line 773 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5831 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 137:
#line 774 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5837 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 138:
#line 775 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5843 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 139:
#line 776 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5849 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 140:
#line 777 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5855 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 141:
#line 778 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5861 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 142:
#line 782 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5867 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 143:
#line 783 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5873 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 144:
#line 787 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 5879 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 145:
#line 788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5885 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 146:
#line 789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 147:
#line 793 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 5897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 148:
#line 794 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 5903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 149:
#line 798 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 5909 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 150:
#line 802 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 151:
#line 804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5921 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 152:
#line 808 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 153:
#line 809 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5933 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 154:
#line 814 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 155:
#line 816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5945 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 156:
#line 818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5951 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 157:
#line 825 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5965 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 158:
#line 835 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5979 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 159:
#line 845 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5993 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 160:
#line 855 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6007 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 161:
#line 867 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6017 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 162:
#line 872 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6027 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 163:
#line 877 "chapel.ypp" /* yacc.c:1663  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 164:
#line 885 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 6043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 165:
#line 886 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 166:
#line 891 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 167:
#line 893 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 168:
#line 895 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 169:
#line 900 "chapel.ypp" /* yacc.c:1663  */
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
#line 6087 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 170:
#line 916 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6095 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 171:
#line 923 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6105 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 172:
#line 932 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6117 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 173:
#line 940 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6125 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 174:
#line 944 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 175:
#line 950 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 176:
#line 951 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 177:
#line 956 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6154 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 178:
#line 961 "chapel.ypp" /* yacc.c:1663  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 179:
#line 966 "chapel.ypp" /* yacc.c:1663  */
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
#line 6183 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 180:
#line 986 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6194 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 181:
#line 992 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6206 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 182:
#line 999 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6219 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 183:
#line 1007 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6232 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 184:
#line 1015 "chapel.ypp" /* yacc.c:1663  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6244 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 185:
#line 1026 "chapel.ypp" /* yacc.c:1663  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 186:
#line 1032 "chapel.ypp" /* yacc.c:1663  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 187:
#line 1039 "chapel.ypp" /* yacc.c:1663  */
    {
      FnSymbol* fn = (yyvsp[-6].pfnsymbol);

      fn->copyFlags((yyvsp[-9].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[-9].pfnsymbol)->name)
        fn->cname = (yyvsp[-9].pfnsymbol)->name;

      if ((yyvsp[-8].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-6].pfnsymbol), (yyvsp[-4].retTag), (yyvsp[-3].pexpr), (yyvsp[-2].b), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), (yylsp[-9]).comment);
      context->latestComment = NULL;
    }
#line 6288 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 188:
#line 1061 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6296 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 189:
#line 1065 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6305 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 190:
#line 1070 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6313 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 191:
#line 1074 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6322 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 193:
#line 1082 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6328 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 195:
#line 1087 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 196:
#line 1088 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&"; }
#line 6340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 197:
#line 1089 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|"; }
#line 6346 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 198:
#line 1090 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^"; }
#line 6352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 199:
#line 1091 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "~"; }
#line 6358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 200:
#line 1092 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "=="; }
#line 6364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 201:
#line 1093 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!="; }
#line 6370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 202:
#line 1094 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<="; }
#line 6376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 203:
#line 1095 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">="; }
#line 6382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 204:
#line 1096 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<"; }
#line 6388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 205:
#line 1097 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">"; }
#line 6394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 206:
#line 1098 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+"; }
#line 6400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 207:
#line 1099 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-"; }
#line 6406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 208:
#line 1100 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*"; }
#line 6412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 209:
#line 1101 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/"; }
#line 6418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 210:
#line 1102 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<"; }
#line 6424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 211:
#line 1103 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>"; }
#line 6430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 212:
#line 1104 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%"; }
#line 6436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 213:
#line 1105 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**"; }
#line 6442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 214:
#line 1106 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "!"; }
#line 6448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 215:
#line 1107 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_by"; }
#line 6454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 216:
#line 1108 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "#"; }
#line 6460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 217:
#line 1109 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "chpl_align"; }
#line 6466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 218:
#line 1110 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<=>"; }
#line 6472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 219:
#line 1111 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<~>"; }
#line 6478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 220:
#line 1115 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "="; }
#line 6484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 221:
#line 1116 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "+="; }
#line 6490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 222:
#line 1117 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "-="; }
#line 6496 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 223:
#line 1118 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "*="; }
#line 6502 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 224:
#line 1119 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "/="; }
#line 6508 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 225:
#line 1120 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "%="; }
#line 6514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 226:
#line 1121 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "**="; }
#line 6520 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 227:
#line 1122 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "&="; }
#line 6526 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 228:
#line 1123 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "|="; }
#line 6532 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 229:
#line 1124 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "^="; }
#line 6538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 230:
#line 1125 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = ">>="; }
#line 6544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 231:
#line 1126 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pch) = "<<="; }
#line 6550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 232:
#line 1130 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 233:
#line 1131 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 234:
#line 1135 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 235:
#line 1139 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 236:
#line 1140 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 237:
#line 1141 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6586 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 238:
#line 1146 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 239:
#line 1148 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 6598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 240:
#line 1150 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6604 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 241:
#line 1152 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6610 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 242:
#line 1154 "chapel.ypp" /* yacc.c:1663  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 243:
#line 1158 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 244:
#line 1159 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 245:
#line 1163 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_IN; }
#line 6634 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 246:
#line 1164 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6640 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 247:
#line 1165 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_OUT; }
#line 6646 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 248:
#line 1166 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST; }
#line 6652 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 249:
#line 1167 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6658 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 250:
#line 1168 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6664 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 251:
#line 1169 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6670 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 252:
#line 1170 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF; }
#line 6676 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 253:
#line 1171 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6682 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 254:
#line 1175 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6688 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 255:
#line 1176 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6694 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 256:
#line 1177 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_REF;   }
#line 6700 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 257:
#line 1178 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6706 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 258:
#line 1179 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6712 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 259:
#line 1180 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6718 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 260:
#line 1184 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6724 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 261:
#line 1185 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6730 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 262:
#line 1189 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 6736 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 263:
#line 1190 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_VALUE; }
#line 6742 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 264:
#line 1191 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6748 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 265:
#line 1192 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_REF; }
#line 6754 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 266:
#line 1193 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_PARAM; }
#line 6760 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 267:
#line 1194 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.retTag) = RET_TYPE; }
#line 6766 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 268:
#line 1198 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = false; }
#line 6772 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 269:
#line 1199 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.b) = true;  }
#line 6778 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 270:
#line 1202 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = NULL; }
#line 6784 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 273:
#line 1208 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6790 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 274:
#line 1213 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6796 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 275:
#line 1215 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6802 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 276:
#line 1219 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6808 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 278:
#line 1224 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6814 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 279:
#line 1225 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6820 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 280:
#line 1229 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 6826 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 281:
#line 1230 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6832 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 282:
#line 1235 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6838 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 283:
#line 1237 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6844 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 284:
#line 1239 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6850 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 285:
#line 1244 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6867 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 286:
#line 1257 "chapel.ypp" /* yacc.c:1663  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6885 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 287:
#line 1273 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 6891 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 288:
#line 1275 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6897 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 289:
#line 1277 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6903 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 290:
#line 1282 "chapel.ypp" /* yacc.c:1663  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6915 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 291:
#line 1290 "chapel.ypp" /* yacc.c:1663  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6927 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 292:
#line 1298 "chapel.ypp" /* yacc.c:1663  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6939 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 293:
#line 1306 "chapel.ypp" /* yacc.c:1663  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6952 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 294:
#line 1315 "chapel.ypp" /* yacc.c:1663  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6963 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 295:
#line 1324 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6969 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 296:
#line 1325 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6975 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 297:
#line 1326 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6981 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 299:
#line 1332 "chapel.ypp" /* yacc.c:1663  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6990 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 300:
#line 1340 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6996 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 301:
#line 1342 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7002 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 302:
#line 1347 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7008 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 303:
#line 1349 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 304:
#line 1351 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7020 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 305:
#line 1356 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7026 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 306:
#line 1358 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7032 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 307:
#line 1360 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7038 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 308:
#line 1366 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7044 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 309:
#line 1367 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7050 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 310:
#line 1368 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7056 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 311:
#line 1374 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7062 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 312:
#line 1376 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7068 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 313:
#line 1378 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7076 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 314:
#line 1382 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7084 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 315:
#line 1386 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7090 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 316:
#line 1388 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7098 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 317:
#line 1392 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7106 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 318:
#line 1399 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7112 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 319:
#line 1400 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7118 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 320:
#line 1401 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7124 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 321:
#line 1402 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7130 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 322:
#line 1403 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7136 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 323:
#line 1404 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7142 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 324:
#line 1405 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7148 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 325:
#line 1406 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7154 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 326:
#line 1407 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7160 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 327:
#line 1408 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7166 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 328:
#line 1409 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7172 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 329:
#line 1414 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7178 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 330:
#line 1415 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7184 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 331:
#line 1416 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7190 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 332:
#line 1417 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7196 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 333:
#line 1418 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7202 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 334:
#line 1419 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7208 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 335:
#line 1420 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7214 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 336:
#line 1421 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7220 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 337:
#line 1422 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7226 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 338:
#line 1423 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7232 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 339:
#line 1424 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7238 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 340:
#line 1445 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7246 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 341:
#line 1449 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7254 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 342:
#line 1453 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7266 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 343:
#line 1461 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7274 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 344:
#line 1467 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7280 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 345:
#line 1468 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7286 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 346:
#line 1469 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7292 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 347:
#line 1474 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7298 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 348:
#line 1476 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7304 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 349:
#line 1482 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7310 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 350:
#line 1484 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7316 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 351:
#line 1486 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7322 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 352:
#line 1490 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = NULL; }
#line 7328 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 353:
#line 1491 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7334 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 354:
#line 1492 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7340 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 355:
#line 1493 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7346 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 356:
#line 1494 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7352 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 357:
#line 1495 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7358 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 358:
#line 1496 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_borrowed"); }
#line 7364 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 359:
#line 1497 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
#line 7370 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 360:
#line 1498 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
#line 7376 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 361:
#line 1499 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
#line 7382 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 362:
#line 1500 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7388 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 363:
#line 1506 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7394 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 364:
#line 1507 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7400 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 365:
#line 1508 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7406 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 366:
#line 1509 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7412 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 367:
#line 1513 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7418 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 368:
#line 1514 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7424 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 369:
#line 1518 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7430 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 370:
#line 1519 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7436 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 371:
#line 1520 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7442 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 372:
#line 1524 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7448 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 373:
#line 1525 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7454 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 374:
#line 1529 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7460 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 376:
#line 1534 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7466 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 377:
#line 1535 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7472 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 378:
#line 1539 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7478 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 379:
#line 1540 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7484 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 380:
#line 1541 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7490 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 381:
#line 1542 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7496 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 382:
#line 1546 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7502 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 388:
#line 1563 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7508 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 389:
#line 1565 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7514 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 390:
#line 1567 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7520 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 391:
#line 1569 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7526 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 392:
#line 1571 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7532 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 393:
#line 1573 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7538 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 394:
#line 1575 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7544 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 395:
#line 1577 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 7550 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 396:
#line 1579 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 7556 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 397:
#line 1581 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 7562 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 398:
#line 1583 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 7568 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 399:
#line 1588 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7574 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 400:
#line 1590 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7580 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 401:
#line 1592 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7586 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 402:
#line 1594 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7592 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 403:
#line 1596 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7598 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 404:
#line 1598 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7604 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 405:
#line 1600 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7610 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 406:
#line 1602 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 7616 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 407:
#line 1604 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7622 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 408:
#line 1606 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7628 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 409:
#line 1608 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7634 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 410:
#line 1610 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7640 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 411:
#line 1612 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7651 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 412:
#line 1619 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7661 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 413:
#line 1625 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7671 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 414:
#line 1631 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7681 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 415:
#line 1637 "chapel.ypp" /* yacc.c:1663  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7691 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 416:
#line 1646 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7697 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 417:
#line 1655 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7703 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 425:
#line 1671 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7709 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 426:
#line 1675 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = NULL; }
#line 7715 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 428:
#line 1680 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7721 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 429:
#line 1684 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7727 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 430:
#line 1685 "chapel.ypp" /* yacc.c:1663  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7733 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 431:
#line 1689 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7739 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 432:
#line 1693 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7745 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 433:
#line 1694 "chapel.ypp" /* yacc.c:1663  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7751 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 434:
#line 1699 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 7759 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 435:
#line 1703 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7767 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 436:
#line 1707 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7775 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 437:
#line 1713 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 7781 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 438:
#line 1714 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 7787 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 439:
#line 1715 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 7793 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 440:
#line 1716 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 7799 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 441:
#line 1717 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 7805 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 442:
#line 1718 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 7811 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 444:
#line 1724 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7817 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 445:
#line 1729 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7823 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 446:
#line 1734 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7829 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 455:
#line 1750 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7835 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 456:
#line 1752 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7841 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 457:
#line 1754 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7847 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 458:
#line 1756 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 7853 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 459:
#line 1758 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7859 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 460:
#line 1760 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7865 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 461:
#line 1764 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 7871 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 462:
#line 1765 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 7877 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 463:
#line 1766 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7883 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 470:
#line 1788 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7889 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 471:
#line 1789 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7895 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 472:
#line 1790 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7901 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 473:
#line 1794 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7907 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 474:
#line 1795 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7913 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 475:
#line 1796 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7919 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 476:
#line 1804 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7925 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 477:
#line 1805 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7931 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 478:
#line 1806 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7937 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 479:
#line 1807 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7943 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 480:
#line 1811 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7949 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 481:
#line 1812 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7955 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 482:
#line 1813 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7961 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 483:
#line 1814 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7967 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 484:
#line 1815 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7973 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 485:
#line 1816 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7979 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 486:
#line 1817 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7985 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 487:
#line 1818 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7991 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 488:
#line 1819 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7997 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 489:
#line 1821 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8005 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 490:
#line 1825 "chapel.ypp" /* yacc.c:1663  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8013 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 491:
#line 1832 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8019 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 492:
#line 1833 "chapel.ypp" /* yacc.c:1663  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8025 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 493:
#line 1837 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8031 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 494:
#line 1838 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8037 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 495:
#line 1839 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8043 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 496:
#line 1840 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8049 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 497:
#line 1841 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8055 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 498:
#line 1842 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8061 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 499:
#line 1843 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8067 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 500:
#line 1844 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8073 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 501:
#line 1845 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8079 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 502:
#line 1846 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8085 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 503:
#line 1847 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8091 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 504:
#line 1848 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8097 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 505:
#line 1849 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8103 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 506:
#line 1850 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8109 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 507:
#line 1851 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8115 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 508:
#line 1852 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8121 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 509:
#line 1853 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8127 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 510:
#line 1854 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8133 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 511:
#line 1855 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8139 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 512:
#line 1856 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8145 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 513:
#line 1857 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8151 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 514:
#line 1858 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8157 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 515:
#line 1859 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8163 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 516:
#line 1863 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8169 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 517:
#line 1864 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8175 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 518:
#line 1865 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8181 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 519:
#line 1866 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8187 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 520:
#line 1867 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8193 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 521:
#line 1868 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8199 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 522:
#line 1872 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8205 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 523:
#line 1873 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8211 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 524:
#line 1874 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8217 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 525:
#line 1875 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8223 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 526:
#line 1879 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8229 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 527:
#line 1880 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8235 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 528:
#line 1881 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8241 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 529:
#line 1882 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8247 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 530:
#line 1887 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8253 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 531:
#line 1888 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8259 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 532:
#line 1889 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8265 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 533:
#line 1890 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8271 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 534:
#line 1891 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8277 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 535:
#line 1892 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8283 "bison-chapel.cpp" /* yacc.c:1663  */
    break;

  case 536:
#line 1893 "chapel.ypp" /* yacc.c:1663  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8289 "bison-chapel.cpp" /* yacc.c:1663  */
    break;


#line 8293 "bison-chapel.cpp" /* yacc.c:1663  */
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
