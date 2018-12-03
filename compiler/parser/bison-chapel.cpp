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
#define YYLAST   16159

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  134
/* YYNRULES -- Number of rules.  */
#define YYNRULES  560
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1002

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
       0,   466,   466,   471,   472,   478,   479,   484,   485,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   523,
     526,   528,   534,   535,   536,   540,   541,   554,   555,   556,
     561,   562,   567,   572,   577,   581,   588,   593,   597,   602,
     606,   607,   608,   612,   616,   618,   620,   622,   624,   631,
     632,   636,   637,   638,   639,   640,   643,   644,   645,   646,
     647,   659,   660,   671,   672,   673,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   683,   684,   685,   686,   690,
     691,   692,   693,   694,   695,   696,   697,   704,   705,   706,
     707,   708,   709,   710,   711,   716,   717,   721,   722,   726,
     727,   731,   732,   733,   734,   735,   736,   737,   738,   742,
     743,   747,   748,   749,   750,   754,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   776,   777,   783,   789,   795,   801,   808,   818,   822,
     823,   824,   825,   826,   828,   830,   832,   837,   840,   841,
     842,   843,   844,   845,   849,   850,   854,   855,   856,   860,
     861,   865,   868,   870,   875,   876,   880,   882,   884,   891,
     901,   911,   921,   934,   939,   944,   952,   953,   958,   959,
     961,   966,   982,   989,   998,  1006,  1010,  1017,  1018,  1023,
    1028,  1022,  1053,  1059,  1066,  1072,  1078,  1089,  1095,  1088,
    1127,  1131,  1136,  1140,  1145,  1152,  1153,  1157,  1158,  1159,
    1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,
    1180,  1181,  1182,  1186,  1187,  1188,  1189,  1190,  1191,  1192,
    1193,  1194,  1195,  1196,  1197,  1201,  1202,  1206,  1210,  1211,
    1212,  1216,  1218,  1220,  1222,  1224,  1229,  1230,  1234,  1235,
    1236,  1237,  1238,  1239,  1240,  1241,  1242,  1246,  1247,  1248,
    1249,  1250,  1251,  1255,  1256,  1260,  1261,  1262,  1263,  1264,
    1265,  1269,  1270,  1273,  1274,  1278,  1279,  1283,  1285,  1290,
    1291,  1295,  1296,  1300,  1301,  1305,  1307,  1309,  1314,  1327,
    1344,  1345,  1347,  1352,  1360,  1368,  1376,  1385,  1395,  1396,
    1397,  1401,  1402,  1410,  1412,  1417,  1419,  1421,  1426,  1428,
    1430,  1437,  1438,  1439,  1444,  1446,  1448,  1452,  1456,  1458,
    1462,  1470,  1471,  1472,  1473,  1474,  1479,  1480,  1481,  1482,
    1483,  1503,  1507,  1511,  1519,  1526,  1527,  1528,  1532,  1534,
    1540,  1542,  1544,  1549,  1550,  1551,  1552,  1553,  1559,  1560,
    1561,  1562,  1566,  1567,  1571,  1572,  1573,  1577,  1578,  1582,
    1583,  1587,  1588,  1592,  1593,  1594,  1595,  1599,  1600,  1611,
    1612,  1613,  1614,  1615,  1616,  1618,  1620,  1622,  1624,  1626,
    1628,  1630,  1632,  1634,  1636,  1641,  1643,  1645,  1647,  1649,
    1651,  1653,  1655,  1657,  1659,  1661,  1663,  1665,  1672,  1678,
    1684,  1690,  1699,  1709,  1717,  1718,  1719,  1720,  1721,  1722,
    1723,  1724,  1729,  1730,  1734,  1738,  1739,  1743,  1747,  1748,
    1752,  1756,  1760,  1767,  1768,  1769,  1770,  1771,  1772,  1776,
    1777,  1782,  1787,  1795,  1796,  1797,  1798,  1799,  1800,  1801,
    1802,  1803,  1805,  1807,  1809,  1811,  1813,  1815,  1817,  1822,
    1823,  1826,  1827,  1828,  1831,  1832,  1833,  1834,  1845,  1846,
    1850,  1851,  1852,  1856,  1857,  1858,  1859,  1867,  1868,  1869,
    1870,  1874,  1875,  1879,  1880,  1881,  1882,  1883,  1884,  1885,
    1886,  1887,  1888,  1889,  1893,  1901,  1902,  1906,  1907,  1908,
    1909,  1910,  1911,  1912,  1913,  1914,  1915,  1916,  1917,  1918,
    1919,  1920,  1921,  1922,  1923,  1924,  1925,  1926,  1927,  1928,
    1932,  1933,  1934,  1935,  1936,  1937,  1941,  1942,  1943,  1944,
    1948,  1949,  1950,  1951,  1956,  1957,  1958,  1959,  1960,  1961,
    1962
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
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
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

#define YYPACT_NINF -857

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-857)))

#define YYTABLE_NINF -512

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -857,    60,  2764,  -857,   -31,  -857,  -857,  -857,  -857,  -857,
    -857,  4171,    17,   131,  -857,  9411,  -857,    17,  9411,  -857,
      88,   131,  4171,  9411,  4171,    32,  -857,  9411,  6358,  -857,
    7810,  9411,  7966,  9411,  -857,    74,  -857,  -857, 16061,  -857,
   12205,  8521,  -857,  9411,  9411,  -857,  9411,  -857,  9411,  9411,
     279,   133,  1084,  1246,  -857,  -857,  8654,  7365,  9411,  8521,
    9411,  9411,   240,  -857,   175,  4171,  -857,  9411,  -857,  9544,
    9544, 16061,  -857,  -857,  9411,  8654,  9411,  9411,  -857,  -857,
    9411,  -857,  -857, 11539,  9411,  9411,  -857,  9411,  -857,  -857,
    3235,  6764,  8654,  -857,  4015,  -857,  -857,   284,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857, 16061,  -857, 16061,   268,
     208,  -857,  -857,  -857,    95,   228,  -857,  -857,  -857,   247,
     264,   226,   269,   276, 15773,  2370,    57,   281,   293,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,   278,  -857,  -857, 15773,
     230,  4171,  -857,  -857,   295,  9411,  9411,  9411,  9411,  9411,
    8654,  8654,    82,  -857,  -857,  -857,  -857,   320,   275,  -857,
    -857,   301, 14028, 16061,   307,  -857,   -38, 15773,   354,  7521,
   15773,  -857,    36,  -857, 16061,   106,  -857, 16061,   306,    13,
   13565, 13514,  -857,  -857,  -857, 13949, 12827,  7521,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  4171,  -857,
     308,   643,    98,    -2,  -857,  4171,  -857,  -857, 13703,   365,
     320, 13703,   320,   365,  -857,  7521,  8099,  -857,  -857, 16061,
    -857,    85, 15773,  9411,  9411,  -857, 15773,   314, 13879,  -857,
   13703,   320, 15773,   317,  7521,  -857, 15773, 14274,  -857,  -857,
   14325, 11797,  -857,  -857, 14412,   366,   327,   320,   122, 14098,
   13703, 14463,   425,  1691,   365,   425,   365,   -29,  -857,  -857,
    3391,   -40,  -857,  9411,  -857,   -51,    62,  -857,    58, 14550,
     -52,   467,  -857,  -857,   424,   355,   328,  -857,  -857,  -857,
    2500, 12205, 12205, 12205,  -857,  9411,  9411,  9411,  9411,  8810,
    8810,  9411,  9411,  9411,  9411,  9677,  9411,   333, 11539,  9411,
    9411,  9411,  9411,  9411,  9411,  9411,  9411,  9411,  9411,  9411,
    9411,  9411,  9411,  9411,  -857,  -857,  -857,  -857,  -857,  -857,
    8232,  8232,  -857,  -857,  -857,  -857,  8232,  -857,  -857,  8232,
    8232,  7521,  7521,  8810,  8810,  7232,  -857,  -857, 14173, 14243,
   14499,    45,  3547,  -857,  8810,    13,   335,  -857,  9411,  -857,
    9411,   376,  -857,   337,   363,  -857,  -857,   348, 16061,   459,
    8654,  -857,  4327,  8810,  -857,  4483,  8810,   349,  -857,    13,
    9810,  9411,  -857,  4171,   475,  9411,   350,  -857,   404,  -857,
    -857,   643,  -857,   389,   369,  -857,  9943,   411,  9411, 12205,
    -857,  -857,  -857,   372,  -857,  8654,  -857, 15773, 15773,  -857,
      38,  -857,  7521,   374,  -857,   514,  -857,   514,  -857, 10076,
     406,  -857,  -857,  9810,  9411,  -857,  -857,  -857,  -857,  -857,
    8365,  -857, 12682,  6920,  -857,  7076,  -857,  8810,  5887,  3079,
     378,  9411,  6201,  -857,  -857, 16061,  8654,   385, 16017,   148,
   12205,   141,   218,   232,   239, 14637,  1795,  1795,   277,  -857,
     277,  -857,   277,  2609,   626,   899,   481, 11926, 11926,   320,
     425,  -857,  -857,  -857,  -857,  1691,  2177,   277,  2220,  2220,
    1795,  2220,  2220,   132,   425,  2177, 15874,   132,   365,   365,
     425,   391,   392,   393,   394,   397,   400,   399,  -857,   277,
    -857,   277,    48,  -857,  -857,  -857,    89,  -857,  1091, 15843,
     321, 10209,  8810, 10342,  8810,  9411,  8810, 12118,    17, 14688,
    -857,  -857, 15773, 14719,  7521,  -857,  7521,  -857,   355,  9411,
     107,  9411, 15773,    24, 13734,  9411, 15773,    54, 13652,  7232,
    -857,   401,   419,   413, 14789,   419,   415,   534, 14864,  4171,
   13804,  -857,    55,  -857,  -857,  -857,  -857,  -857,  -857,   764,
     123,  -857, 12249,  -857,   325,   409,   643,    98,  9411,    32,
      74,  9411,  9411,  9411,  9411,  9411,  5731,  -857,  -857,   119,
    7677,  -857, 15773,  -857,  -857,  -857, 15773,   422,   -17,   423,
    -857,  2563,  -857,  -857,   258, 16061,   432,   435,  -857,  -857,
    -857,  -857,  -857,  -857,    10, 12392,  -857,  -857, 15773,  4171,
   15773,  -857, 14934,   414,   457,  1001,   436,   478,   -19,  -857,
     517,  -857,  -857,  -857,  -857,  6477,   261,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    7232,  -857,    23,  8810,  8810,  9411,   564, 15009,  9411,   567,
   15079,   438, 12462,    13,    13,  -857,  -857,  -857,  -857,   444,
   13703,  -857, 12902,  4639,  -857,  4795,  -857, 12977,  4951,  -857,
      13,  5107,  -857,    13,   198,  -857,  9411,  -857,  9411,  -857,
    4171,  9411,  -857,  4171,   568,  -857,  -857, 16061,  1036,  -857,
     643,   472,   526,  -857,  -857,  -857,    35,  -857,  -857,   411,
     443,    61,  -857,  -857,  -857,  5263,  8654,  -857,  -857,  -857,
   16061,  -857,   476,   301,  -857,  -857,  -857,  5419,   448,  5575,
     451,  -857,  9411,  3703,   462,  1985,  -857,  9411, 16061,  -857,
    -857,   325,   447,   487,  -857,   489,  -857,   492,  -857,   494,
     497,   499,   503,   505,   506,  -857,   507,  -857, 16061,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  9411,
    -857,  -857,   518,   447,   447,  -857,  -857,  -857,   411,   205,
     216, 15110, 10475, 10608, 15248, 10741, 10874, 11007, 11140,  -857,
    -857,  2359,  -857,  4171,  9411, 15773,  9411, 15773,  4171,  9411,
   15773,  -857,  9411, 15773,  -857,  7232,  -857, 15161, 15773,  -857,
   15773,   614,  4171,   472,  -857,   491,  8943,   102,  -857,   548,
    -857,  -857,  8810, 12022,  4171,  -857,    12,   495,  9411,  -857,
    9411,  -857, 15773,  4171,  9411,  -857, 15773,  4171, 15773,    22,
    -857,  3859,  -857,  6631,  1155,  1155,  -857, 11727,  -857, 15773,
    -857,    27,   404,  -857,  -857, 12752, 12312,  -857,  -857,  -857,
    9411,  9411,  9411,  9411,  9411,  9411,  9411,  9411,   496,  2130,
   14864, 13052, 13127, 14864, 13202, 13277,  -857,  9411,  4171,  -857,
     411,   472,  6044,  -857,  -857,  -857,   128,  8654,  -857,  -857,
    9411,    31, 12537,  -857,   249,   354,  -857,   301, 15773, 13352,
    -857, 13427,  -857,  -857,  -857,  -857,  -857, 11273,   563,   235,
    -857,  -857,  -857,   447,   447, 15323, 15398, 15473, 15548, 15623,
   15698,  -857,  -857,  4171,  4171,  4171,  4171, 15773,  -857,  -857,
     102,  9099,    63,  -857,  -857, 15773, 15773,  -857,  -857,  -857,
   11140,  -857,  4171,  4171,  2920,  -857,  -857,   139,  -857,   548,
    -857,  -857,  -857,  9411,  9411,  9411,  9411,  9411,  9411, 14864,
   14864, 14864, 14864,  -857,  -857,  -857,  -857,  -857,   237,  8810,
   11672,   512, 14864, 14864,   498, 11406,    80,    96, 12607,  -857,
    -857,  -857,  -857,   370, 11803,  -857,  -857,  -857,  9255,  -857,
     486,  -857
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    81,   504,   505,   506,   507,
     508,     0,   442,    69,    99,     0,   193,   442,     0,   104,
     329,    69,     0,     0,     0,     0,   203,   479,   330,   501,
       0,     0,   328,     0,   103,     0,   213,   100,     0,   209,
       0,     0,   106,     0,     0,   433,     0,   216,     0,     0,
       0,     0,   328,   328,   102,   194,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,    82,     0,   502,     0,
       0,     0,   101,   195,     0,     0,     0,     0,   556,   558,
       0,   559,   560,   478,     0,     0,   557,   554,   121,   555,
       0,     0,     0,     4,     0,     5,     9,    45,    10,    11,
      12,    14,   397,   398,    22,    13,   122,   128,    15,    17,
      16,    19,    20,    21,    18,   127,     0,   125,     0,   468,
       0,   129,   126,   130,     0,   484,   464,   465,   401,   399,
       0,     0,   469,   470,     0,   400,     0,   485,   486,   487,
     503,   463,   403,   402,   466,   467,     0,    38,    24,   409,
       0,     0,   443,    70,     0,     0,     0,     0,     0,     0,
       0,     0,   468,   484,   399,   469,   470,   411,   400,   485,
     486,     0,   442,     0,     0,   167,     0,   382,     0,   389,
     480,   214,   507,   135,     0,     0,   215,     0,     0,     0,
       0,     0,   329,   330,   134,     0,     0,   389,    76,    83,
      94,    88,    96,    78,    87,    97,    84,    98,    92,    86,
      93,    91,    89,    90,    77,    79,    85,    95,     0,    80,
       0,     0,     0,     0,   331,     0,   118,    32,     0,   542,
     461,     0,   412,   543,     7,   389,   330,   124,   123,   308,
     379,     0,   378,     0,     0,   119,   483,     0,     0,    35,
       0,   414,   404,     0,   389,    36,   410,     0,   174,   170,
       0,   400,   174,   171,     0,   320,     0,   413,     0,   378,
       0,     0,   545,   477,   541,   544,   540,     0,    47,    50,
       0,     0,   384,     0,   386,     0,     0,   385,     0,   378,
       0,     0,     6,    46,     0,   196,     0,   294,   293,   217,
       0,     0,     0,     0,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   476,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   253,   260,   261,   262,   257,   259,
       0,     0,   255,   258,   256,   254,     0,   264,   263,     0,
       0,   389,   389,     0,     0,     0,    25,    26,     0,     0,
       0,     0,     0,    27,     0,     0,     0,    28,     0,    29,
       0,   397,   395,     0,   390,   391,   396,     0,     0,     0,
       0,   144,     0,     0,   143,     0,     0,     0,   150,     0,
       0,    56,   131,     0,   160,     0,     0,    30,   276,   210,
     335,     0,   336,   338,     0,   360,     0,   341,     0,     0,
     117,    31,    33,     0,   307,     0,    63,   481,   482,   120,
       0,    34,   389,     0,   181,   172,   168,   173,   169,     0,
     318,   315,    60,     0,    56,   137,    37,    49,    48,    51,
       0,   509,     0,     0,   497,     0,   499,     0,     0,     0,
       0,     0,     0,   513,     8,     0,     0,     0,     0,   287,
       0,     0,     0,     0,     0,   441,   537,   536,   539,   547,
     546,   551,   550,   533,   530,   531,   532,   473,   474,   472,
     520,   495,   496,   494,   493,   475,   524,   535,   529,   527,
     538,   528,   526,   518,   523,   525,   534,   517,   521,   522,
     519,     0,     0,     0,     0,     0,     0,     0,   549,   548,
     553,   552,   453,   454,   456,   458,     0,   445,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   511,   442,   442,
     140,   316,   383,     0,     0,   406,     0,   317,   196,     0,
       0,     0,   417,     0,     0,     0,   423,     0,     0,     0,
     151,   555,    59,     0,    52,    57,     0,   159,     0,     0,
       0,   405,   281,   278,   279,   280,   284,   285,   286,   276,
       0,   269,     0,   277,   295,     0,   339,     0,   111,   113,
     114,   109,   110,   108,   107,   112,     0,   359,   358,   464,
       0,   333,   462,   332,   492,   381,   380,     0,     0,     0,
     407,     0,   175,   322,   464,     0,     0,     0,   510,   471,
     498,   387,   500,   388,     0,     0,   512,   156,   427,     0,
     515,   514,     0,     0,   197,     0,     0,   207,     0,   204,
     291,   288,   289,   292,   218,     0,     0,   324,   323,   325,
     327,    67,    68,    66,    65,    64,   490,   491,   455,   457,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,   393,   394,   392,     0,
       0,   158,     0,     0,   142,     0,   141,     0,     0,   148,
       0,     0,   146,     0,     0,   448,     0,   132,     0,   133,
       0,     0,   162,     0,   164,   282,   283,     0,   276,   267,
       0,   373,   296,   299,   298,   300,     0,   337,   340,   341,
       0,     0,   342,   343,   183,     0,     0,   182,   185,   408,
       0,   176,   179,     0,   319,    61,    62,     0,     0,     0,
       0,   157,     0,     0,     0,   328,   202,     0,   205,   201,
     290,   295,   265,    71,   250,    83,   248,    88,    73,    87,
      84,    98,    86,    89,    72,    74,    85,   229,   232,   230,
     231,   242,   233,   246,   238,   236,   249,   252,   237,   235,
     240,   245,   247,   234,   239,   243,   244,   241,   251,     0,
     227,    75,     0,   265,   265,   225,   326,   446,   341,   484,
     484,     0,     0,     0,     0,     0,     0,     0,     0,   139,
     138,     0,   145,     0,     0,   416,     0,   415,     0,     0,
     422,   149,     0,   421,   147,     0,   447,    54,    53,   161,
     432,   163,     0,   373,   270,     0,     0,   341,   297,   313,
     334,   364,     0,   511,     0,   187,     0,     0,     0,   177,
       0,   154,   429,     0,     0,   152,   428,     0,   516,     0,
      39,     0,   191,   330,   328,   328,   189,   328,   199,   208,
     206,     0,   276,   224,   228,     0,     0,   220,   221,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
     420,     0,     0,   426,     0,     0,   449,     0,     0,   166,
     341,   373,     0,   376,   375,   377,   464,   309,   273,   271,
       0,     0,     0,   362,   464,   188,   186,     0,   180,     0,
     155,     0,   153,    41,    40,   200,   355,     0,   301,     0,
     226,    71,    72,   265,   265,     0,     0,     0,     0,     0,
       0,   192,   190,     0,     0,     0,     0,    55,   165,   272,
     341,   365,     0,   310,   312,   311,   314,   305,   306,   211,
       0,   178,     0,     0,     0,   354,   353,   464,   302,   313,
     266,   222,   223,     0,     0,     0,     0,     0,     0,   419,
     418,   425,   424,   275,   274,   367,   368,   370,   464,     0,
     511,   464,   431,   430,     0,   345,     0,     0,     0,   369,
     371,   350,   348,   464,   511,   303,   219,   304,   365,   349,
     464,   372
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -857,  -857,  -857,    -1,  -375,  1887,  -857,  -857,  -857,  1794,
     -78,  -321,   222,   227,  -857,  -857,   332,   642,  -857,  1154,
    -153,  -617,  -857,  -775,  -857,   701,  -833,  -704,   -44,  -857,
    -857,  -857,   105,  -857,  -857,  -857,   410,  -857,   -49,  -857,
    -857,  -857,  -857,  -857,   483,   143,  -123,  -857,  -857,  -857,
     -56,   742,  -857,  -857,  -857,  -857,  -857,  -857,  -857,  -857,
    -183,  -133,  -722,  -857,  -177,    -8,   118,  -857,  -857,  -857,
     -50,  -857,  -857,  -295,   740,  -857,  -247,  -265,  -857,  -170,
    -857,   664,  -857,  -207,   288,  -857,  -376,  -696,  -707,  -857,
    -550,  -424,  -706,  -856,  -748,    -4,  -857,     5,  -857,  -168,
    -857,   162,   326,  -372,  -857,  -857,   846,  -857,    -7,  -857,
    -857,  -282,  -857,  -534,  -857,  -857,   924,  1280,   -11,   672,
     140,   525,  -857,  1423,  1566,  -857,  -857,  -857,  -857,  -857,
    -857,  -857,  -857,  -344
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   279,    94,   557,    96,    97,   294,    98,
     280,   552,   556,   553,    99,   100,   101,   154,   780,   222,
     102,   219,   103,   587,   185,   227,   104,   105,   106,   107,
     108,   109,   661,   110,   111,   112,   425,   602,   723,   113,
     114,   598,   718,   115,   116,   457,   735,   117,   118,   628,
     629,   162,   220,   574,   120,   121,   459,   741,   634,   782,
     783,   395,   863,   399,   570,   571,   572,   573,   635,   299,
     706,   959,   996,   949,   240,   944,   898,   901,   122,   266,
     430,   123,   124,   223,   224,   403,   404,   591,   956,   918,
     407,   588,   976,   895,   827,   281,   176,   285,   286,   373,
     374,   375,   163,   126,   127,   128,   164,   130,   151,   152,
     516,   389,   684,   517,   518,   131,   165,   166,   134,   186,
     376,   168,   136,   169,   170,   139,   140,   141,   290,   142,
     143,   144,   145,   146
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     149,    93,   350,   366,   167,   603,   450,   172,   237,   238,
     171,   520,   177,   830,   377,   685,   180,   180,   781,   190,
     191,   195,   196,   569,   405,   575,   371,   709,   916,   396,
     228,   858,   229,   230,   589,   231,   405,   232,   233,   597,
     408,   225,   225,   715,   371,   242,   246,   248,   250,   251,
     252,   893,   241,   673,   256,  -116,   257,   604,   260,   264,
       3,   867,   868,   267,   269,   270,   271,   413,   948,   272,
     555,   268,   273,   274,   275,   890,   276,   452,   443,   242,
     246,   289,   371,   678,   716,   977,   423,   526,   288,   440,
     648,   368,   869,   461,   462,   463,   464,   695,  -184,   405,
     447,   371,   788,   832,  -351,   979,   444,   453,    57,  -116,
     738,   369,  -356,   555,   387,   441,   787,  -351,   147,   300,
     147,   150,   526,   649,   990,    16,   437,   409,   350,  -351,
     696,   899,  -356,  -116,   153,   189,   739,  -356,   717,  -184,
    -356,   415,   955,   940,   149,   358,   359,   360,   256,   242,
     289,   406,  -356,   915,   948,   917,   829,   361,   387,   301,
     308,  -357,   387,   406,   484,   365,   362,   362,   246,   727,
     302,   147,   630,    57,   448,   858,  -351,   913,   362,    55,
    -356,  -351,   173,   506,   507,   362,   246,   448,   179,  -356,
     448,   445,   448,  -184,   939,   303,  -357,   247,   371,   371,
     708,   961,   962,    73,   527,   520,   309,  -356,   362,   448,
     310,   590,   631,   351,   415,   352,  -352,   449,   650,   446,
     833,  -357,   980,   632,   246,   180,   406,  -356,  -357,  -352,
     197,   287,   417,   418,   416,   897,   415,  -374,  -489,   994,
    -489,  -352,   633,   246,   974,   995,   651,  -356,  -357,   781,
     362,   415,   698,   636,   599,   297,   312,  -374,   313,   314,
     315,  -374,   316,   317,   671,   680,   683,   320,  -357,   371,
     409,   432,   442,  -357,   989,   327,  -357,   298,   992,   439,
     699,   886,   331,   332,   333,  -374,   234,   999,  -352,   235,
     637,  -361,  1001,  -352,   465,   466,   467,   468,   470,   472,
     473,   474,   475,   476,   479,   480,   520,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   569,   825,   253,  -361,   815,   125,   246,
     246,   254,   728,   730,  -452,   246,     5,   125,   246,   246,
     246,   246,   509,   511,   519,  -451,  -366,   409,   125,   702,
     125,  -361,   353,   529,   293,   816,   354,   532,  -361,   533,
     857,   409,  -452,   481,   698,   305,  -366,   638,   409,   242,
    -366,   542,   544,  -451,   546,   548,   540,  -435,  -361,   554,
     554,   639,   558,   371,   560,   482,   355,  -321,   640,   703,
     409,   125,   960,   308,  -366,   654,  -434,   592,  -361,   354,
     704,   259,   263,  -361,   596,   315,  -361,  -321,   317,   903,
     786,   246,   320,   304,   469,   471,   125,  -438,  -439,   705,
     125,    66,   554,   554,  -489,  -440,  -489,   483,   562,   596,
    -437,  -488,   246,  -488,   246,   724,   615,   596,   618,   309,
     620,   622,  -436,   310,   357,   242,   563,  -344,   611,   564,
     613,   317,   624,   308,   896,   362,   367,   370,   508,   510,
    -344,   904,   380,   419,   398,   565,   167,   267,   566,   528,
      50,   520,  -344,   422,   454,   429,   431,   125,   455,   567,
     501,   502,   458,   456,   531,   534,   503,   569,   543,   504,
     505,   547,   536,   315,   535,   316,   317,   537,   568,   309,
     320,   539,   784,   310,   857,   549,   559,   561,   327,   308,
     542,   657,   546,   660,   558,   662,   618,   333,   576,  -344,
     590,   663,   664,   246,  -344,   246,   577,   135,   670,   594,
     672,   600,   601,  -268,   677,   605,   135,   619,   519,   625,
     641,   642,   643,   644,   125,   957,   645,   135,   686,   135,
     -58,   125,   614,   315,  -363,   309,   317,   646,   647,   310,
     320,  -268,   687,  -346,   689,   690,   707,   167,   733,   978,
     232,   251,   252,   256,   267,   289,  -346,   714,   981,   246,
     719,   725,   711,   287,   726,   287,   415,   737,  -346,  -363,
     135,   736,   740,   792,   261,   261,   795,   797,   801,   822,
     826,   828,   831,   862,   838,   312,   125,   843,   978,   315,
     847,   316,   317,   993,  -363,   135,   320,   852,   -81,   135,
     -99,  -363,  1000,  -104,   327,  -103,   978,   656,  -100,   659,
    -106,   331,   332,   333,  -102,  -346,  -105,   -82,  -101,   519,
    -346,  -363,   470,   509,   791,   888,   198,   794,   891,   866,
     900,   931,   907,   958,   308,   851,   607,   991,   199,   200,
     606,  -363,   805,   174,   807,   201,  -363,   810,   378,  -363,
     813,   837,   427,   202,   667,   817,   135,   818,   879,   203,
     820,   669,   860,   923,   204,   919,   205,   697,   125,   206,
     824,   861,   997,   973,   987,   207,   194,   593,   668,   181,
     309,     0,     0,     0,   310,   242,   208,     0,   125,     0,
       0,   125,   836,     0,     0,   209,   842,     0,   846,   125,
       0,   848,     0,     0,   210,   211,   859,   212,     0,   213,
     713,   214,     0,   924,   215,     0,     0,     0,   216,   400,
       0,   217,     0,   135,   119,     0,     0,     0,     0,     0,
     135,     0,     0,   119,   315,     0,   316,   317,   469,   508,
     249,   320,     0,     0,   119,     0,   119,     0,   865,   327,
       0,     0,     0,     0,     0,   125,   331,   332,   333,     0,
       0,   805,   807,     0,   810,   813,   842,   846,   562,     0,
       0,     0,   880,   881,     0,   882,     0,   883,   884,   401,
       0,   885,     0,     0,   519,   135,   563,   119,     0,   564,
     237,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   902,   618,     0,     0,   565,     0,   908,   566,   909,
     291,   284,   119,   911,     0,     0,   119,     0,     0,   567,
       0,     0,   180,     0,   652,     0,     0,     0,   129,     0,
     439,     0,     0,     0,     0,     0,     0,   129,   568,   880,
     925,   926,   883,   927,   928,   929,   930,     0,   129,     0,
     129,     0,     0,     0,     0,     0,   937,     0,     0,     0,
       0,   289,     0,     0,     0,   125,   945,   135,   942,   946,
     381,   384,   388,   119,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   135,     0,     0,
     135,   129,     0,     0,     0,     0,     0,     0,   135,   372,
       0,     0,   969,   970,   971,   972,   132,   308,     0,   411,
       0,     0,   412,     0,     0,   132,   129,   372,     0,   846,
     129,   982,   983,   289,     0,   125,   132,     0,   132,     0,
     986,   421,   969,   970,   971,   972,   982,   983,     0,     0,
     119,   785,     0,     0,     0,     0,     0,   119,   988,   618,
       0,   435,     0,   309,   135,   372,     0,   310,     0,   789,
     790,     0,     0,   618,     0,     0,     0,   846,     0,   132,
       0,     0,     0,     0,   372,     0,     0,   129,     0,   125,
       0,   125,   734,     0,   125,     0,     0,   125,     0,     0,
       0,     0,     0,     0,   132,     0,   125,     0,   132,   125,
    -198,     0,   119,   312,  -198,  -198,   314,   315,     0,   316,
     317,     0,     0,  -198,   320,  -198,  -198,     0,     0,     0,
    -198,   125,   327,     0,     0,  -198,     0,     0,  -198,   331,
     332,   333,     0,   125,     0,   125,     0,     0,     0,   125,
     562,     0,     0,  -198,   129,  -198,   530,  -198,     0,  -198,
    -198,   129,  -198,     0,  -198,   132,  -198,     0,   563,     0,
       0,   564,     0,     0,   135,     0,     0,     0,     0,     0,
     550,   372,   372,     0,     5,  -198,     0,   565,  -198,     0,
     566,  -198,    50,     0,   119,     0,    14,   192,     0,     0,
       0,   567,     0,    19,     0,     0,     0,     0,    27,   236,
       0,     0,     0,     0,   119,     0,   129,   119,    36,   125,
     568,  -212,    34,     0,   125,   119,     0,    37,   -44,     0,
       0,     0,   132,    42,   135,     0,    47,     0,   125,   132,
    -198,     0,     0,  -212,   -44,   595,  -198,     0,     0,     0,
     125,     0,   372,    54,     0,     0,     0,     0,     0,   125,
       0,     0,     0,   125,     0,    63,     0,   125,   192,    66,
     595,     0,     0,   284,     0,   284,    72,     0,   595,    27,
     236,   119,   218,     0,     0,     0,     0,     0,   135,    36,
     135,     0,  -212,   135,   132,     0,   135,     0,   129,     0,
       0,     0,     0,     0,   125,   135,     0,    47,   135,     0,
       0,     0,     0,     0,  -212,   265,     0,     0,   129,     0,
       0,   129,     0,     0,     0,     0,     0,     0,     0,   129,
     135,     0,     0,     0,   674,   676,     0,     0,   679,   682,
       0,     0,   135,     0,   135,     0,     0,     0,   135,   125,
     125,   125,   125,     0,     0,     0,     0,     0,     0,   192,
     295,     0,   296,     0,   666,     0,   372,     0,   125,   125,
      27,   236,   133,     0,     0,     0,   132,     0,     0,     0,
      36,   133,     0,  -212,     0,   129,     0,     0,     0,     0,
     -43,   119,   133,     0,   133,     0,   132,     0,    47,   132,
       0,     0,     0,     0,     0,  -212,   -43,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,   135,     0,
       0,     0,     0,   135,     0,     0,     0,     0,   265,     0,
       0,   379,     0,     0,     0,   133,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
       0,   119,     0,     0,   799,   800,     0,     0,   135,     0,
     133,   802,   135,   132,   133,   402,   135,     0,     0,     0,
       0,   811,     0,     0,   814,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   129,     0,     0,     0,     0,
       0,     0,     0,   135,     0,   119,     0,   119,     0,     0,
     119,     0,     0,   119,     0,   137,     0,     0,     0,     0,
       0,   133,   119,     0,   137,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   137,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   135,   135,
     135,   135,     0,     0,     0,   129,     0,     0,     0,   119,
       0,   119,     0,     0,     0,   119,     0,   135,   135,     0,
       0,     0,     0,   132,     0,     0,     0,     0,   137,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   133,     0,
       0,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,     0,   137,     0,     0,     0,   137,     0,   129,
       0,   129,     0,     0,   129,     0,     0,   129,     0,     0,
       0,     0,   538,     0,     0,     0,   129,   906,     0,   129,
       0,     0,     0,   132,     0,   119,     0,     0,     0,     0,
     119,     0,     0,     0,     0,   402,     0,     0,     0,     0,
     133,   129,     0,     0,   119,     0,   894,     0,   138,     0,
       0,     0,     0,   129,   137,   129,   119,   138,     0,   129,
       0,     0,     0,     0,     0,   119,     0,     0,   138,   119,
     138,     0,     0,   119,     0,     0,     0,   132,     0,   132,
       0,     0,   132,     0,     0,   132,     0,     0,     0,   623,
       0,     0,   627,     0,   132,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     119,   138,     0,     0,     0,     0,     0,   943,     0,   132,
       0,   137,   133,     0,     0,     0,     0,     0,   137,   129,
       0,   132,     0,   132,   129,     0,   138,   132,     0,     0,
     138,     0,   133,     0,     0,   133,     0,     0,   129,     0,
       0,     0,     0,   133,     0,   119,   119,   119,   119,     0,
     129,   975,     0,     0,     0,     0,     0,     0,     0,   129,
       0,     0,     0,   129,   119,   119,     0,   129,     0,     0,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,     0,   308,
     975,     0,     0,     0,     0,     0,   701,   132,     0,   133,
     402,     0,   132,     0,   129,     0,     0,     0,   975,     0,
       0,     0,     0,     0,     0,     0,   132,     0,     0,     0,
       0,     0,     0,     0,     0,   722,     0,     0,   132,   265,
       0,     0,     0,     0,     0,   309,     0,   132,     0,   310,
       0,   132,     0,     0,     0,   132,     0,     0,     0,   129,
     129,   129,   129,     0,   138,   137,     0,     0,     0,     0,
       0,   138,     0,     0,     0,     0,     0,     0,   129,   129,
       0,     0,     0,     0,     0,   137,     0,     0,   137,     0,
       0,     0,   132,     0,     0,   312,   137,   313,   314,   315,
       0,   316,   317,   308,     0,     0,   320,     0,     0,     0,
       0,     0,     0,   326,   327,   226,     0,     0,   330,   133,
       0,   331,   332,   333,     0,     0,   138,     0,     0,     0,
       0,   823,     0,   226,   402,     0,     0,   132,   132,   132,
     132,     0,     0,   258,   262,     0,     0,     0,     0,   309,
       0,     0,   137,   310,   722,     0,   132,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,   627,     0,     0,     0,     0,     0,   148,   133,
       0,     0,     0,   311,     0,     0,     0,     0,     0,   175,
       0,   178,   864,     0,     0,     0,     0,     0,     0,   312,
       0,   313,   314,   315,     0,   316,   317,   318,   138,   319,
     320,   321,   322,     0,     0,   324,   325,   326,   327,     0,
     328,   329,   330,     0,     0,   331,   332,   333,   138,     0,
       0,   138,   255,   133,     0,   133,     0,     0,   133,   138,
       0,   133,     0,     0,     0,   363,     0,     0,     0,     0,
     133,     0,     0,   133,     0,     0,     0,    95,     0,     0,
       0,   292,   137,   226,   226,   226,     0,     0,     0,     0,
     394,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,    16,     0,     0,   133,    20,   133,
       0,     0,     0,   133,     0,   138,     0,    26,     0,    27,
     853,     0,   226,     0,    32,   226,     0,     0,     0,    36,
       0,     0,  -212,     0,     0,     0,     0,     0,   356,     0,
       0,     0,   137,     0,   226,     0,     0,    47,     0,     0,
       0,    50,     0,   854,  -212,     0,   855,     0,    55,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,     0,    73,   133,     0,     0,     0,     0,   133,     0,
       0,     0,     0,     0,     0,     0,   137,     0,   137,     0,
       0,   137,   133,     0,   137,   397,     0,     0,     0,     0,
       0,     0,   410,   137,   133,     0,   137,     0,     0,     0,
       0,     0,     0,   133,     0,   138,     0,   133,     0,     0,
       0,   133,     0,     0,    88,     0,     0,     0,   137,     0,
     856,     0,     0,     0,     0,     0,     0,     0,     0,    16,
     137,     0,   137,    20,     0,     0,   137,     0,     0,   226,
       0,     0,    26,     0,    27,   853,     0,    95,   133,    32,
       0,     0,     0,     0,    36,     0,     0,  -212,     0,     0,
       0,     0,     0,   226,     0,   138,     0,     0,     0,     0,
       0,     0,    47,     0,     0,     0,    50,     0,   854,  -212,
       0,   855,     0,    55,     0,   308,     0,     0,     0,     0,
       0,     0,     0,   133,   133,   133,   133,     0,     0,     0,
       0,     0,     0,     0,    71,     0,   137,    73,     0,     0,
       0,   137,   133,   133,     0,     0,     0,     0,     0,   138,
       0,   138,     0,     0,   138,   137,     0,   138,   308,    95,
       0,   309,     0,     0,     0,   310,   138,   137,     0,   138,
       0,     0,     0,     0,     0,     0,   137,     0,     0,   410,
     137,     0,   410,     0,   137,     0,     0,     0,     0,    88,
       0,   138,     0,     0,     0,   932,     0,     0,     0,     0,
       0,     0,     0,   138,   309,   138,     0,     0,   310,   138,
       0,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   137,   320,   321,   322,     0,     0,   324,   325,   326,
     327,     0,     0,     0,   330,     0,     0,   331,   332,   333,
       0,     0,     0,     0,     0,     0,   617,   226,   226,     0,
       0,   226,   226,     0,   312,     0,   313,   314,   315,     0,
     316,   317,   318,     0,   694,   320,   137,   137,   137,   137,
     878,     0,   326,   327,     0,     0,     0,   330,     0,   138,
     331,   332,   333,     0,   138,   137,   137,     0,  -198,     0,
       0,     0,  -198,  -198,     0,     0,     0,     0,   138,     0,
       0,  -198,     0,  -198,  -198,   721,     0,     0,  -198,     0,
     138,     0,     0,  -198,     0,     0,  -198,     0,     0,   138,
       0,     0,     0,   138,     0,     0,     0,   138,     0,     0,
       0,  -198,     0,  -198,     0,  -198,     0,  -198,  -198,     0,
    -198,     0,  -198,     0,  -198,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   692,     0,     0,     0,
       0,     0,     0,  -198,   138,     0,  -198,   226,   226,  -198,
       0,     0,     0,     0,   226,     0,   394,     0,     0,     0,
       0,   394,     0,     0,   226,     0,     0,   226,     0,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,     0,     0,     0,     0,     0,   138,
     138,   138,   138,   198,     0,     0,   731,     0,  -198,  -459,
       0,     0,     0,     0,  -198,   199,   200,   839,   138,   138,
       0,     0,   201,   349,     0,     0,  -488,     0,  -488,     0,
     202,     0,     0,     0,     0,     0,   203,     0,     0,     0,
       0,   204,     0,   205,     0,     0,   206,     0,     0,     0,
       0,     0,   207,     0,     0,     0,     0,     0,     0,     0,
     410,     0,   410,   208,     0,   410,   198,     0,   410,     0,
       0,     0,   209,     0,     0,   460,     0,   819,   199,   200,
     821,   210,   211,     0,   212,   201,   213,     0,   214,     0,
       0,   215,     0,   202,     0,   216,     0,     0,   217,   203,
       0,     0,   835,     0,   204,     0,   205,     0,     0,   206,
       0,     0,     0,     0,   841,   207,   845,     0,     0,     0,
      95,     0,     0,     0,     0,     0,   208,     0,     0,     0,
     226,     0,     0,     0,     0,   209,     0,   308,     0,     0,
       0,     0,     0,     0,   210,   211,     0,   212,     0,   213,
       0,   214,     0,     0,   215,     0,   221,     0,   216,     0,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   394,   394,     0,   394,   394,
       0,     0,     0,   309,     0,     0,     0,   310,     0,     0,
       0,     0,     0,     0,     0,   947,     0,     0,     0,     0,
       0,   951,     0,   394,     0,   394,     0,     0,     0,   889,
       0,     0,     0,     0,     0,     0,     0,   362,     0,   720,
       0,   905,     0,     0,     0,     0,     0,     0,     0,     0,
     910,     0,     0,   312,   912,   313,   314,   315,    95,   316,
     317,   318,     0,   319,   320,   321,   322,     0,     0,   324,
     325,   326,   327,     0,   328,     0,   330,     0,     0,   331,
     332,   333,     0,     0,    -2,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   938,    11,    12,    13,    14,
      15,   947,     0,    16,    17,    18,    19,    20,  -328,    21,
      22,    23,     0,    24,    25,     0,    26,     0,    27,    28,
      29,    30,    31,    32,    33,    34,     0,    35,    36,     0,
      37,  -212,    38,    39,    40,    41,    42,    43,   -42,    44,
      45,     0,    46,     0,     0,     0,    47,    48,  -328,    49,
      50,    51,    52,  -212,   -42,    53,    54,    55,     0,  -328,
      56,    57,     0,    58,    59,    60,    61,    62,    63,    64,
      65,     0,    66,    67,     0,    68,    69,    70,    71,    72,
       0,    73,    74,    75,  -328,     0,     0,    76,     0,    77,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,    85,     0,
      86,    87,     0,    88,     0,     0,    89,     0,    90,     0,
      91,   984,    92,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   155,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,    89,     0,   160,     0,    91,     0,   161,   985,
       4,     0,     5,     6,     7,     8,     9,    10,     0,  -511,
       0,    11,    12,    13,    14,    15,  -511,     0,    16,    17,
      18,    19,    20,  -328,    21,    22,    23,  -511,    24,    25,
    -511,    26,     0,    27,    28,    29,    30,    31,    32,    33,
      34,     0,    35,    36,     0,    37,  -212,    38,    39,    40,
      41,    42,    43,   -42,    44,    45,     0,    46,     0,     0,
       0,    47,    48,  -328,    49,     0,    51,    52,  -212,   -42,
      53,    54,    55,  -511,  -328,    56,    57,  -511,    58,    59,
      60,    61,    62,    63,    64,    65,     0,    66,    67,     0,
      68,    69,    70,    71,    72,     0,    73,    74,    75,  -328,
       0,     0,    76,     0,    77,     0,     0,  -511,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -511,    80,  -511,  -511,  -511,  -511,  -511,
    -511,  -511,     0,  -511,  -511,  -511,  -511,  -511,     0,  -511,
    -511,  -511,  -511,    85,  -511,  -511,  -511,     0,    88,  -511,
    -511,  -511,     0,    90,  -511,    91,   277,    92,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -328,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,    33,    34,     0,    35,    36,
       0,    37,  -212,    38,    39,    40,    41,    42,    43,   -42,
      44,    45,     0,    46,     0,     0,     0,    47,    48,  -328,
      49,    50,    51,    52,  -212,   -42,    53,    54,    55,     0,
    -328,    56,    57,     0,    58,    59,    60,    61,    62,    63,
      64,    65,     0,    66,    67,     0,    68,    69,    70,    71,
      72,     0,    73,    74,    75,  -328,     0,     0,    76,     0,
      77,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,   239,    88,     0,     0,    89,     0,    90,
     278,    91,     4,    92,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -328,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,    33,    34,     0,    35,    36,     0,    37,  -212,    38,
      39,    40,    41,    42,    43,   -42,    44,    45,     0,    46,
       0,     0,     0,    47,    48,  -328,    49,    50,    51,    52,
    -212,   -42,    53,    54,    55,     0,  -328,    56,    57,     0,
      58,    59,    60,    61,    62,    63,    64,    65,     0,    66,
      67,     0,    68,    69,    70,    71,    72,     0,    73,    74,
      75,  -328,     0,     0,    76,     0,    77,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,     0,
      88,     0,     0,    89,     0,    90,   438,    91,   277,    92,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -328,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,    32,    33,    34,     0,
      35,    36,     0,    37,  -212,    38,    39,    40,    41,    42,
      43,   -42,    44,    45,     0,    46,     0,     0,     0,    47,
      48,  -328,    49,    50,    51,    52,  -212,   -42,    53,    54,
      55,     0,  -328,    56,    57,     0,    58,    59,    60,    61,
      62,    63,    64,    65,     0,    66,    67,     0,    68,    69,
      70,    71,    72,     0,    73,    74,    75,  -328,     0,     0,
      76,     0,    77,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,    85,     0,    86,    87,     0,    88,     0,     0,    89,
       0,    90,   278,    91,   849,    92,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -328,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,    32,    33,    34,     0,    35,    36,     0,    37,
    -212,    38,    39,    40,    41,    42,    43,   -42,    44,    45,
       0,    46,     0,     0,     0,    47,    48,  -328,    49,    50,
      51,    52,  -212,   -42,    53,    54,    55,     0,  -328,    56,
      57,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,    66,    67,     0,    68,    69,    70,    71,    72,     0,
      73,    74,    75,  -328,     0,     0,    76,     0,    77,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,    85,     0,    86,
      87,     0,    88,     0,     0,    89,     0,    90,   850,    91,
       4,    92,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -328,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,    33,
      34,     0,    35,    36,     0,    37,  -212,    38,    39,    40,
      41,    42,    43,   -42,    44,    45,     0,    46,     0,     0,
       0,    47,    48,  -328,    49,    50,    51,    52,  -212,   -42,
      53,    54,    55,     0,  -328,    56,    57,     0,    58,    59,
      60,    61,    62,    63,    64,    65,     0,    66,    67,     0,
      68,    69,    70,    71,    72,     0,    73,    74,    75,  -328,
       0,     0,    76,     0,    77,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    85,     0,    86,    87,     0,    88,     0,
       0,    89,     0,    90,   914,    91,     4,    92,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -328,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,    33,    34,     0,    35,    36,
       0,    37,  -212,    38,    39,    40,    41,    42,    43,   -42,
      44,    45,     0,    46,     0,     0,     0,    47,    48,  -328,
      49,   291,    51,    52,  -212,   -42,    53,    54,    55,     0,
    -328,    56,    57,     0,    58,    59,    60,    61,    62,    63,
      64,    65,     0,    66,    67,     0,    68,    69,    70,    71,
      72,     0,    73,    74,    75,  -328,     0,     0,    76,     0,
      77,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,     0,    88,     0,     0,    89,     0,    90,
       0,    91,     4,    92,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -328,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,    33,    34,     0,    35,    36,     0,    37,  -212,    38,
      39,    40,    41,    42,    43,   -42,    44,    45,     0,    46,
       0,     0,     0,    47,    48,  -328,    49,     0,    51,    52,
    -212,   -42,    53,    54,    55,     0,  -328,    56,    57,     0,
      58,    59,    60,    61,    62,    63,    64,    65,     0,    66,
      67,     0,    68,    69,    70,    71,    72,     0,    73,    74,
      75,  -328,     0,     0,    76,     0,    77,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,     0,
      88,     0,     0,    89,     0,    90,     0,    91,     4,    92,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -328,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,    32,   541,    34,     0,
      35,    36,     0,    37,  -212,    38,    39,    40,    41,    42,
      43,   -42,    44,    45,     0,    46,     0,     0,     0,    47,
      48,  -328,    49,     0,    51,    52,  -212,   -42,    53,    54,
      55,     0,  -328,    56,    57,     0,    58,    59,    60,    61,
      62,    63,    64,    65,     0,    66,    67,     0,    68,    69,
      70,    71,    72,     0,    73,    74,    75,  -328,     0,     0,
      76,     0,    77,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,    85,     0,    86,    87,     0,    88,     0,     0,    89,
       0,    90,     0,    91,     4,    92,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -328,    21,    22,
      23,     0,    24,    25,     0,    26,     0,    27,    28,    29,
      30,    31,    32,   545,    34,     0,    35,    36,     0,    37,
    -212,    38,    39,    40,    41,    42,    43,   -42,    44,    45,
       0,    46,     0,     0,     0,    47,    48,  -328,    49,     0,
      51,    52,  -212,   -42,    53,    54,    55,     0,  -328,    56,
      57,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,    66,    67,     0,    68,    69,    70,    71,    72,     0,
      73,    74,    75,  -328,     0,     0,    76,     0,    77,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,    85,     0,    86,
      87,     0,    88,     0,     0,    89,     0,    90,     0,    91,
       4,    92,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -328,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,   804,
      34,     0,    35,    36,     0,    37,  -212,    38,    39,    40,
      41,    42,    43,   -42,    44,    45,     0,    46,     0,     0,
       0,    47,    48,  -328,    49,     0,    51,    52,  -212,   -42,
      53,    54,    55,     0,  -328,    56,    57,     0,    58,    59,
      60,    61,    62,    63,    64,    65,     0,    66,    67,     0,
      68,    69,    70,    71,    72,     0,    73,    74,    75,  -328,
       0,     0,    76,     0,    77,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    85,     0,    86,    87,     0,    88,     0,
       0,    89,     0,    90,     0,    91,     4,    92,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -328,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,   806,    34,     0,    35,    36,
       0,    37,  -212,    38,    39,    40,    41,    42,    43,   -42,
      44,    45,     0,    46,     0,     0,     0,    47,    48,  -328,
      49,     0,    51,    52,  -212,   -42,    53,    54,    55,     0,
    -328,    56,    57,     0,    58,    59,    60,    61,    62,    63,
      64,    65,     0,    66,    67,     0,    68,    69,    70,    71,
      72,     0,    73,    74,    75,  -328,     0,     0,    76,     0,
      77,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,     0,    88,     0,     0,    89,     0,    90,
       0,    91,     4,    92,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,    14,    15,     0,     0,
      16,    17,    18,    19,    20,  -328,    21,    22,    23,     0,
      24,    25,     0,    26,     0,    27,    28,    29,    30,    31,
      32,   809,    34,     0,    35,    36,     0,    37,  -212,    38,
      39,    40,    41,    42,    43,   -42,    44,    45,     0,    46,
       0,     0,     0,    47,    48,  -328,    49,     0,    51,    52,
    -212,   -42,    53,    54,    55,     0,  -328,    56,    57,     0,
      58,    59,    60,    61,    62,    63,    64,    65,     0,    66,
      67,     0,    68,    69,    70,    71,    72,     0,    73,    74,
      75,  -328,     0,     0,    76,     0,    77,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,     0,
      88,     0,     0,    89,     0,    90,     0,    91,     4,    92,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,    14,    15,     0,     0,    16,    17,    18,    19,
      20,  -328,    21,    22,    23,     0,    24,    25,     0,    26,
       0,    27,    28,    29,    30,    31,    32,   812,    34,     0,
      35,    36,     0,    37,  -212,    38,    39,    40,    41,    42,
      43,   -42,    44,    45,     0,    46,     0,     0,     0,    47,
      48,  -328,    49,     0,    51,    52,  -212,   -42,    53,    54,
      55,     0,  -328,    56,    57,     0,    58,    59,    60,    61,
      62,    63,    64,    65,     0,    66,    67,     0,    68,    69,
      70,    71,    72,     0,    73,    74,    75,  -328,     0,     0,
      76,     0,    77,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,    85,     0,    86,    87,     0,    88,     0,     0,    89,
       0,    90,     0,    91,     4,    92,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,     0,    16,    17,    18,    19,    20,  -328,    21,    22,
      23,     0,   834,    25,     0,    26,     0,    27,    28,    29,
      30,    31,    32,    33,    34,     0,    35,    36,     0,    37,
    -212,    38,    39,    40,    41,    42,    43,   -42,    44,    45,
       0,    46,     0,     0,     0,    47,    48,  -328,    49,     0,
      51,    52,  -212,   -42,    53,    54,    55,     0,  -328,    56,
      57,     0,    58,    59,    60,    61,    62,    63,    64,    65,
       0,    66,    67,     0,    68,    69,    70,    71,    72,     0,
      73,    74,    75,  -328,     0,     0,    76,     0,    77,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,    85,     0,    86,
      87,     0,    88,     0,     0,    89,     0,    90,     0,    91,
       4,    92,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,     0,    16,    17,
      18,    19,    20,  -328,    21,    22,    23,     0,    24,    25,
       0,    26,     0,    27,    28,    29,    30,    31,    32,   840,
      34,     0,    35,    36,     0,    37,  -212,    38,    39,    40,
      41,    42,    43,   -42,    44,    45,     0,    46,     0,     0,
       0,    47,    48,  -328,    49,     0,    51,    52,  -212,   -42,
      53,    54,    55,     0,  -328,    56,    57,     0,    58,    59,
      60,    61,    62,    63,    64,    65,     0,    66,    67,     0,
      68,    69,    70,    71,    72,     0,    73,    74,    75,  -328,
       0,     0,    76,     0,    77,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,    85,     0,    86,    87,     0,    88,     0,
       0,    89,     0,    90,     0,    91,     4,    92,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,     0,    16,    17,    18,    19,    20,  -328,
      21,    22,    23,     0,    24,    25,     0,    26,     0,    27,
      28,    29,    30,    31,    32,   844,    34,     0,    35,    36,
       0,    37,  -212,    38,    39,    40,    41,    42,    43,   -42,
      44,    45,     0,    46,     0,     0,     0,    47,    48,  -328,
      49,     0,    51,    52,  -212,   -42,    53,    54,    55,     0,
    -328,    56,    57,     0,    58,    59,    60,    61,    62,    63,
      64,    65,     0,    66,    67,     0,    68,    69,    70,    71,
      72,     0,    73,    74,    75,  -328,     0,     0,    76,     0,
      77,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,     0,    88,     0,     0,    89,     0,    90,
       0,    91,   710,    92,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,   239,
       0,     0,     0,    89,     0,   160,     0,    91,     0,   161,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   155,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,     0,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,     0,     0,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,     0,
       0,     0,    72,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    79,    80,    81,    82,     0,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,    85,     0,    86,    87,   239,     0,     0,     0,    89,
       0,   160,     0,    91,     0,   161,   616,     5,     6,     7,
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
      91,     0,   161,   941,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,     0,
       0,     0,     0,    89,     0,   160,     0,    91,     0,   161,
     621,     5,     6,     7,     8,   182,    10,   183,     0,     0,
     155,     0,     0,    14,    15,     0,     0,  -115,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   158,    34,
       0,    35,     0,     0,    37,  -479,     0,    39,    40,     0,
      42,    43,     0,    44,    45,     0,     0,     0,     0,     0,
       0,    48,     0,    49,     0,    51,     0,  -479,     0,     0,
      54,  -115,     0,     0,     0,     0,     0,     0,     0,    60,
      61,    62,    63,    64,   159,     0,    66,     0,     0,    68,
       0,     0,   184,    72,     0,  -115,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   742,     0,
     743,     0,    79,    80,    81,    82,     0,   744,     0,     0,
      83,     0,   745,   200,   746,     0,     0,     0,     0,   747,
      84,     0,    85,     0,    86,    87,     0,   202,     0,     0,
      89,     0,   160,   748,    91,     0,   161,     0,   749,     0,
     205,     0,     0,   750,     0,     0,     0,     0,     0,   751,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,   752,
       0,     0,     0,     0,     0,     0,     0,     0,   210,   211,
       0,   753,     0,   213,     0,   754,     0,     0,   755,     0,
       0,     0,   756,     0,     0,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   334,   335,   336,   337,
     338,   339,     0,     0,   342,   343,   344,   345,     0,   347,
     348,   757,   758,   759,   760,     0,     0,   761,     0,     0,
       0,   762,   763,   764,   765,   766,   767,   768,   769,   770,
     771,   772,   773,     0,   774,     0,     0,   775,   776,   777,
     778,     0,     0,   779,     5,     6,     7,     8,   182,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
    -115,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,  -479,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
    -479,     0,     0,    54,  -115,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,     0,     0,   184,    72,     0,  -115,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    84,     0,    85,   155,    86,    87,    14,
      15,     0,     0,    89,     0,   160,    19,    91,     0,   161,
       0,     0,     0,     0,    25,     0,     0,     0,     0,     0,
      29,   156,   157,     0,   158,    34,     0,    35,     0,     0,
      37,     0,     0,    39,    40,     0,    42,    43,     0,    44,
      45,     0,     0,     0,     0,     0,     0,    48,     0,    49,
       0,    51,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    63,    64,
     159,     0,    66,     0,     0,    68,   243,   244,     0,    72,
     282,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,    80,
      81,    82,     0,     0,     0,     0,    83,   283,     0,     0,
       0,     0,     0,     0,     0,     0,    84,     0,    85,     0,
      86,    87,   239,     0,     0,     0,    89,     0,   160,     0,
      91,     0,   161,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   155,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    29,   156,   157,     0,
     158,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,   243,   244,     0,    72,   282,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,   239,     0,
       0,     0,    89,     0,   160,     0,    91,   610,   161,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,     0,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,   243,   244,
       0,    72,   282,     0,    74,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    80,    81,    82,     0,     0,     0,     0,    83,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,     0,
      85,     0,    86,    87,   239,     0,     0,     0,    89,     0,
     160,     0,    91,   612,   161,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   155,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,   512,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   158,    34,   513,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,     0,
       0,     0,     0,     0,     0,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,     0,   514,     0,     0,
       0,     0,     0,    60,    61,    62,    63,    64,   159,     0,
      66,     0,     0,    68,     0,     0,     0,    72,     0,     0,
      74,     0,   515,     0,     0,     0,     0,     0,     0,     0,
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
      64,   159,     0,    66,     0,     0,    68,   243,   244,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,     0,   245,     0,     0,    89,     0,   160,
       0,    91,     0,   161,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    29,   156,   157,
       0,   158,    34,     0,    35,     0,     0,    37,     0,     0,
      39,    40,     0,    42,    43,     0,    44,    45,     0,     0,
       0,     0,     0,     0,    48,     0,    49,     0,    51,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,   159,     0,    66,
       0,     0,    68,   243,   244,     0,    72,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    80,    81,    82,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    84,     0,    85,     0,    86,    87,   239,
       0,     0,     0,    89,     0,   160,     0,    91,     0,   161,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   155,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,     0,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,   712,     0,     0,     0,     0,     0,
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
       0,     0,     0,    48,   187,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,     0,     0,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,   188,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,    85,     0,    86,    87,     0,     0,
       0,     0,    89,     0,   160,     0,    91,     0,   161,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,     0,    19,   192,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
       0,   193,    29,   156,   157,     0,   158,    34,     0,    35,
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
      34,     0,    35,     0,     0,    37,  -479,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,  -479,     0,
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
     157,     0,   158,    34,     0,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,     0,
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
      44,    45,     0,     0,     0,     0,     0,     0,    48,     0,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,     0,     0,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,   239,     0,     0,     0,    89,     0,   160,
     608,    91,     0,   161,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
     225,    25,     0,     0,     0,     0,     0,    29,   156,   157,
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
      15,     0,     0,    89,     0,    90,    19,    91,     0,   161,
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
      91,     0,   161,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   155,     0,     0,    14,    15,     0,     0,     0,
       0,     0,    19,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,    29,   156,   157,     0,
     158,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,     0,     0,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,   188,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    84,     0,    85,   155,    86,    87,    14,   578,
       0,     0,    89,     0,   160,    19,    91,     0,   161,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   158,    34,     0,   580,     0,     0,    37,
       0,     0,    39,    40,     0,    42,    43,     0,    44,    45,
       0,     0,     0,     0,     0,     0,   581,     0,    49,     0,
      51,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,   582,   583,    62,    63,    64,   584,
       0,    66,     0,     0,    68,     0,     0,     0,    72,     0,
       0,   585,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,    85,     0,    86,
      87,   239,     0,     0,     0,    89,     0,   160,     0,    91,
       0,   892,     5,     6,     7,     8,     9,    10,     0,     0,
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
       0,    89,     0,   160,     0,    91,     0,   892,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   155,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,    29,   156,   157,     0,   877,    34,     0,    35,     0,
       0,    37,     0,     0,    39,    40,     0,    42,    43,     0,
      44,    45,     0,     0,     0,     0,     0,     0,    48,     0,
      49,     0,    51,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,   159,     0,    66,     0,     0,    68,     0,     0,     0,
      72,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      80,    81,    82,     0,     0,     0,     0,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,    85,
       0,    86,    87,   239,     0,     0,     0,    89,     0,   160,
       0,    91,     0,   161,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   155,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
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
      15,     0,     0,    89,     0,   160,    19,    91,     0,   161,
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
       5,     6,     7,     8,     9,    10,    84,     0,    85,   155,
      86,    87,    14,   477,     0,     0,    89,     0,    90,    19,
      91,     0,   161,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,    29,   156,   157,     0,   158,    34,     0,
      35,     0,     0,    37,     0,     0,    39,    40,     0,    42,
      43,     0,    44,    45,     0,     0,     0,     0,     0,     0,
      48,     0,    49,     0,    51,     0,     0,     0,     0,    54,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,   159,     0,    66,     0,     0,    68,     0,
       0,     0,    72,     0,     0,   478,     0,     0,     0,     0,
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
       0,     0,    83,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    84,     0,    85,   155,    86,    87,    14,   578,
       0,     0,   551,     0,   160,    19,    91,     0,   161,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   158,    34,     0,   580,     0,     0,    37,
       0,     0,    39,    40,     0,    42,    43,     0,    44,    45,
       0,     0,     0,     0,     0,     0,   581,     0,    49,     0,
      51,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,   582,   583,    62,    63,    64,   584,
       0,    66,     0,     0,    68,     0,     0,     0,    72,     0,
       0,   585,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    84,     0,    85,   155,    86,
      87,    14,    15,     0,     0,    89,     0,   160,    19,    91,
       0,   586,     0,     0,     0,     0,    25,     0,     0,     0,
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
     160,    19,    91,     0,   586,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   655,
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
     157,     0,   658,    34,     0,    35,     0,     0,    37,     0,
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
       0,    29,   156,   157,     0,   871,    34,     0,    35,     0,
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
     155,    86,    87,    14,    15,     0,     0,    89,     0,   160,
      19,    91,     0,   161,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   872,    34,
       0,    35,     0,     0,    37,     0,     0,    39,    40,     0,
      42,    43,     0,    44,    45,     0,     0,     0,     0,     0,
       0,    48,     0,    49,     0,    51,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,    62,    63,    64,   159,     0,    66,     0,     0,    68,
       0,     0,     0,    72,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    79,    80,    81,    82,     0,     0,     0,     0,
      83,     0,     0,     0,     5,     6,     7,     8,     9,    10,
      84,     0,    85,   155,    86,    87,    14,    15,     0,     0,
      89,     0,   160,    19,    91,     0,   161,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,    29,   156,   157,
       0,   874,    34,     0,    35,     0,     0,    37,     0,     0,
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
      29,   156,   157,     0,   875,    34,     0,    35,     0,     0,
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
       0,     0,     0,    29,   156,   157,     0,   876,    34,     0,
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
     877,    34,     0,    35,     0,     0,    37,     0,     0,    39,
      40,     0,    42,    43,     0,    44,    45,     0,     0,     0,
       0,     0,     0,    48,     0,    49,     0,    51,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,    64,   159,     0,    66,     0,
       0,    68,     0,     0,     0,    72,     0,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    80,    81,    82,     0,     0,
       0,     0,    83,     0,     0,     0,     5,     6,     7,     8,
       9,    10,    84,     0,    85,   155,    86,    87,    14,   578,
       0,     0,    89,     0,   160,    19,    91,     0,   161,     0,
       0,     0,     0,   579,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   158,    34,     0,   580,     0,     0,    37,
       0,     0,    39,    40,     0,    42,    43,     0,    44,    45,
       0,     0,     0,     0,     0,     0,   581,     0,    49,     0,
      51,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,   582,   583,    62,    63,    64,   584,
       0,    66,     0,     0,    68,     0,     0,     0,    72,     0,
       0,   585,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    78,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,    80,    81,
      82,     0,     0,     0,     0,    83,     0,     0,     0,     5,
       6,     7,     8,     9,    10,    84,     0,    85,   155,    86,
      87,    14,    15,     0,     0,    89,     0,   160,    19,    91,
       0,   954,     0,     0,     0,     0,    25,     0,     0,     0,
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
       0,    19,    91,     0,   954,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,    29,   156,   157,     0,   158,
      34,     0,    35,     0,     0,    37,     0,     0,    39,    40,
       0,    42,    43,     0,    44,    45,     0,     0,     0,     0,
       0,     0,    48,     0,    49,     0,    51,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,   159,     0,    66,     0,     0,
      68,     0,     0,     0,    72,     0,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    81,    82,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,    84,     0,    85,   155,     0,    87,    14,    15,     0,
       0,    89,     0,   160,    19,    91,     0,   161,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   158,    34,     0,    35,     0,     0,    37,     0,
       0,    39,    40,     0,    42,    43,     0,    44,    45,     0,
       0,     0,     0,     0,     0,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,    16,     0,     0,     0,
      20,     0,     0,    60,    61,    62,    63,    64,   159,    26,
      66,    27,   853,    68,     0,     0,    32,    72,     0,     0,
      74,    36,     0,     0,  -212,     0,     0,     0,     0,     0,
       0,  -365,     0,     0,     0,     0,     0,     0,     0,    47,
       0,     0,     0,   291,     0,   854,  -212,    80,   855,     0,
      55,  -365,     0,     0,     0,  -365,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   155,    85,     0,    14,    15,
     239,    71,     0,     0,    73,    19,   160,     0,    91,  -365,
     892,     0,     0,    25,     0,     0,     0,     0,     0,    29,
     156,   157,     0,   158,    34,     0,    35,     0,     0,    37,
       0,     0,    39,    40,     0,    42,    43,     0,    44,    45,
       0,     0,     0,     0,     0,     0,    48,     0,    49,     0,
      51,     0,     0,     0,     0,    54,    88,     0,     0,     0,
    -347,     0,     0,     0,    60,    61,    62,    63,    64,   159,
       0,    66,     0,  -347,    68,     0,     0,     0,    72,     0,
       0,    74,     0,     0,     0,  -347,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,     0,     0,     0,     0,     0,     0,     0,    80,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   155,     0,
       0,    14,    15,     0,     0,     0,     0,    85,    19,     0,
     349,     0,  -347,  -488,     0,  -488,    25,  -347,     0,    91,
       0,   954,    29,   156,   157,     0,   158,    34,     0,    35,
       0,     0,    37,     0,     0,    39,    40,     0,    42,    43,
       0,    44,    45,     0,     0,     0,     0,     0,     0,    48,
       0,    49,     0,    51,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,    62,
      63,    64,   159,     0,    66,     0,     0,    68,     0,     0,
       0,    72,     0,     0,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   155,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,    80,    25,     0,     0,     0,     0,     0,    29,   156,
     157,     0,   158,    34,     0,    35,     0,     0,    37,     0,
      85,    39,    40,     0,    42,    43,     0,    44,    45,     0,
     160,     0,    91,     0,   161,    48,     0,    49,     0,    51,
       0,     0,     0,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,    64,   159,     0,
      66,     0,     0,    68,     0,     0,     0,    72,     0,     0,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     155,     0,     0,    14,    15,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,    80,    25,     0,
       0,     0,     0,     0,    29,   156,   157,     0,   158,    34,
       0,    35,     0,     0,    37,     0,    85,    39,    40,     0,
      42,    43,     0,    44,    45,     0,   160,     0,    91,     0,
     586,    48,     0,    49,     0,    51,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,    62,    63,    64,   159,     0,    66,     0,   198,    68,
       0,     0,     0,    72,     0,     0,    74,     0,     0,     0,
     199,   200,     0,     0,     0,     0,     0,   201,     0,     0,
       0,     0,     0,     0,     0,   202,     0,     0,     0,     0,
       0,   203,     0,    80,     0,     0,   204,     0,   205,     0,
       0,   206,   198,     0,     0,     0,     0,   207,     0,     0,
       0,     0,    85,     0,   199,   200,     0,     0,   208,     0,
       0,   201,     0,     0,    91,     0,   161,   209,     0,   202,
       0,     0,     0,     0,     0,   203,   210,   211,     0,   212,
     204,   213,   205,   214,     0,   206,   215,     0,     0,     0,
     216,   207,     0,   217,     0,     0,     0,     0,     0,     0,
       0,     0,   208,     0,     0,   921,     0,     0,     0,     0,
       0,   209,   744,     0,     0,     0,     0,   199,   200,   746,
     210,   211,     0,   212,   201,   213,     0,   214,     0,     0,
     215,     0,   202,     0,   216,     0,     0,   217,   748,     0,
       0,     0,     0,   204,     0,   205,     0,     0,   206,     0,
       0,   221,     0,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,     0,     0,     0,     0,
       0,     0,     0,     0,   209,     0,     0,     0,     0,     0,
       0,     0,     0,   210,   211,     0,   212,     0,   213,     0,
     922,     0,   306,   755,     0,   700,     0,   216,     0,   307,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,   334,   335,   336,   337,   338,   339,     0,     0,   342,
     343,   344,   345,     0,   347,   348,   757,   758,   759,   760,
       0,     0,   761,     0,     0,     0,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,   772,   773,     0,   774,
       0,     0,   775,   776,   777,   778,   309,     0,     0,     0,
     310,     0,   306,     0,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,   387,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,   309,   328,   329,   330,
     310,     0,   331,   332,   333,     0,     0,   306,     0,     0,
       0,   729,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,   309,   331,   332,   333,   310,     0,   306,     0,     0,
       0,   798,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,   309,   328,   329,   330,   310,     0,   331,   332,   333,
       0,     0,   306,     0,     0,     0,   950,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,     0,   328,   329,   330,     0,   309,   331,   332,   333,
     310,     0,   306,     0,     0,     0,   998,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,   309,   328,   329,   330,
     310,     0,   331,   332,   333,     0,     0,   306,     0,   609,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,   309,   331,   332,   333,   310,     0,     0,     0,   920,
       0,     0,   306,     0,   393,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,   311,   334,   335,   336,   337,
     338,   339,     0,     0,   342,   343,   344,   345,     0,   347,
     348,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,     0,   328,   329,   330,     0,   309,   331,   332,   333,
     310,   362,     0,     0,     0,     0,     0,   306,     0,   803,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     311,   334,   335,   336,   337,   338,   339,     0,     0,   342,
     343,   344,   345,     0,   347,   348,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,   309,   331,   332,   333,   310,   362,     0,     0,     0,
       0,     0,   306,     0,   808,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,   311,   334,   335,   336,   337,
     338,   339,     0,     0,   342,   343,   344,   345,     0,   347,
     348,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,     0,   328,   329,   330,     0,   309,   331,   332,   333,
     310,   362,     0,     0,     0,     0,     0,   306,     0,   933,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     311,   334,   335,   336,   337,   338,   339,     0,     0,   342,
     343,   344,   345,     0,   347,   348,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,   309,   331,   332,   333,   310,   362,     0,     0,     0,
       0,     0,   306,     0,   934,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,   311,   334,   335,   336,   337,
     338,   339,     0,     0,   342,   343,   344,   345,     0,   347,
     348,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,     0,   328,   329,   330,     0,   309,   331,   332,   333,
     310,   362,     0,     0,     0,     0,     0,   306,     0,   935,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     311,   334,   335,   336,   337,   338,   339,     0,     0,   342,
     343,   344,   345,     0,   347,   348,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,   309,   331,   332,   333,   310,   362,     0,     0,     0,
       0,     0,   306,     0,   936,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,   311,   334,   335,   336,   337,
     338,   339,     0,     0,   342,   343,   344,   345,     0,   347,
     348,   312,     0,   313,   314,   315,     0,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,     0,   328,   329,   330,     0,   309,   331,   332,   333,
     310,   362,     0,     0,     0,     0,     0,   306,     0,   952,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     311,   334,   335,   336,   337,   338,   339,     0,     0,   342,
     343,   344,   345,     0,   347,   348,   312,     0,   313,   314,
     315,     0,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,   309,   331,   332,   333,   310,   362,     0,     0,     0,
       0,     0,     0,     0,   953,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,   311,   334,   335,   336,   337,
     338,   339,   308,   385,   342,   343,   344,   345,     0,   347,
     348,   312,     0,   313,   314,   315,   386,   316,   317,   318,
       0,   319,   320,   321,   322,   323,     0,   324,   325,   326,
     327,     0,   328,   329,   330,   306,     0,   331,   332,   333,
       0,   362,   307,     0,     0,     0,     0,     0,   309,     0,
       0,     0,   310,   308,   382,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   387,     0,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,   309,
     313,   314,   315,   310,   316,   317,   318,     0,   319,   320,
     321,   322,   323,     0,   324,   325,   326,   327,     0,   328,
     329,   330,   306,     0,   331,   332,   333,     0,   362,   307,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
     308,   681,     0,     0,     0,     0,     0,     0,     0,   312,
       0,   313,   314,   315,     0,   316,   317,   318,     0,   319,
     320,   321,   322,   323,     0,   324,   325,   326,   327,     0,
     328,   329,   330,   306,     0,   331,   332,   333,     0,   362,
     307,     0,     0,     0,     0,     0,   309,     0,     0,     0,
     310,   308,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,   387,     0,     0,     0,
     311,     0,   308,   675,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   309,   313,   314,
     315,   310,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
       0,     0,   331,   332,   333,     0,   362,     0,   309,     0,
       0,   311,   310,     0,   306,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,   308,   316,   317,   318,     0,   319,   320,   321,
     322,   323,   311,   324,   325,   326,   327,     0,   328,   329,
     330,     0,     0,   331,   332,   333,     0,   362,   312,     0,
     313,   314,   315,     0,   316,   317,   318,     0,   319,   320,
     321,   322,   323,     0,   324,   325,   326,   327,   309,   328,
     329,   330,   310,     0,   331,   332,   333,     0,   362,   306,
       0,   693,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,     0,     0,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,     0,
     313,   314,   315,     0,   316,   317,   318,     0,   319,   320,
     321,   322,   323,     0,   324,   325,   326,   327,     0,   328,
     329,   330,     0,   309,   331,   332,   333,   310,   362,   306,
       0,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,     0,     0,
       0,     0,   390,     0,     0,     0,     0,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,     0,   313,   314,   315,   391,   316,
     317,   318,     0,   319,   320,   321,   322,   323,     0,   324,
     325,   326,   327,   309,   328,   329,   330,   310,     0,   331,
     332,   333,     0,   420,     0,     0,     0,     0,   306,     0,
       0,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   308,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     364,     0,     0,   312,     0,   313,   314,   315,     0,   316,
     317,   318,     0,   319,   320,   321,   322,   323,     0,   324,
     325,   326,   327,     0,   328,   329,   330,     0,   392,   331,
     332,   333,   309,     0,     0,     0,   310,     0,   306,     0,
       0,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,   433,   150,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,   434,   316,   317,
     318,     0,   319,   320,   321,   322,   323,     0,   324,   325,
     326,   327,   309,   328,   329,   330,   310,     0,   331,   332,
     333,     0,     0,   306,     0,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   521,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,   522,     0,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,     0,   316,   317,
     318,     0,   319,   320,   321,   322,   323,     0,   324,   325,
     326,   327,     0,   328,   329,   330,     0,   309,   331,   332,
     333,   310,     0,   306,     0,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,   523,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,   306,   524,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,   308,   316,   317,   318,     0,   319,   320,   321,
     322,   323,     0,   324,   325,   326,   327,   309,   328,   329,
     330,   310,     0,   331,   332,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,     0,   307,     0,     0,     0,     0,     0,   309,     0,
       0,   311,   310,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,     0,   316,   317,   318,     0,   319,   320,   321,
     322,   323,   311,   324,   325,   326,   327,     0,   328,   329,
     330,     0,     0,   331,   332,   333,     0,     0,   312,   309,
     313,   314,   315,   310,   316,   317,   318,     0,   319,   320,
     321,   322,   323,     0,   324,   325,   326,   327,     0,   328,
     329,   330,   306,   424,   331,   332,   333,     0,     0,   307,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,   312,
       0,   313,   314,   315,     0,   316,   317,   318,     0,   319,
     320,   321,   322,   323,     0,   324,   325,   326,   327,     0,
     328,   329,   330,   306,   426,   331,   332,   333,     0,     0,
     307,     0,     0,     0,     0,     0,   309,     0,     0,     0,
     310,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
       0,     0,     0,     0,     0,     0,   307,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,   312,   309,   313,   314,
     315,   310,   316,   317,   318,     0,   319,   320,   321,   322,
     323,     0,   324,   325,   326,   327,     0,   328,   329,   330,
     306,   428,   331,   332,   333,     0,     0,   307,     0,     0,
       0,   311,     0,   309,     0,     0,     0,   310,   308,     0,
       0,     0,     0,     0,     0,     0,   525,   312,     0,   313,
     314,   315,     0,   316,   317,   318,     0,   319,   320,   321,
     322,   323,     0,   324,   325,   326,   327,   311,   328,   329,
     330,     0,   436,   331,   332,   333,     0,     0,     0,     0,
       0,     0,     0,   312,   309,   313,   314,   315,   310,   316,
     317,   318,     0,   319,   320,   321,   322,   323,     0,   324,
     325,   326,   327,     0,   328,   329,   330,   306,     0,   331,
     332,   333,     0,     0,   307,     0,     0,   451,   311,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,   312,     0,   313,   314,   315,     0,
     316,   317,   318,     0,   319,   320,   321,   322,   323,     0,
     324,   325,   326,   327,     0,   328,   329,   330,   306,     0,
     331,   332,   333,     0,     0,   307,     0,     0,     0,     0,
       0,   309,     0,     0,     0,   310,   308,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
       0,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,   309,   313,   314,   315,   310,   316,   317,   318,
       0,   319,   320,   321,   322,   323,  -460,   324,   325,   326,
     327,     0,   328,   329,   330,     0,     0,   331,   332,   333,
       0,     0,   150,   309,     0,     0,   311,   310,     0,   306,
     688,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,   308,   316,   317,
     318,     0,   319,   320,   321,   322,   323,   311,   324,   325,
     326,   327,     0,   328,   329,   330,     0,     0,   331,   332,
     333,     0,     0,   312,     0,   313,   314,   315,     0,   316,
     317,   318,     0,   319,   320,   321,   322,   323,     0,   324,
     325,   326,   327,   309,   328,   329,   330,   310,   665,   331,
     332,   333,     0,     0,   306,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,     0,     0,   691,     0,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   312,     0,   313,   314,   315,     0,   316,
     317,   318,     0,   319,   320,   321,   322,   323,     0,   324,
     325,   326,   327,     0,   328,   329,   330,     0,   309,   331,
     332,   333,   310,     0,   306,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,     0,
     313,   314,   315,     0,   316,   317,   318,     0,   319,   320,
     321,   322,   323,     0,   324,   325,   326,   327,   309,   328,
     329,   330,   310,     0,   331,   332,   333,     0,     0,   306,
       0,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   793,     0,
       0,   732,   311,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   312,     0,
     313,   314,   315,     0,   316,   317,   318,     0,   319,   320,
     321,   322,   323,     0,   324,   325,   326,   327,     0,   328,
     329,   330,     0,   309,   331,   332,   333,   310,     0,   306,
       0,     0,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   796,     0,
       0,     0,     0,     0,     0,     0,     0,   311,     0,     0,
     306,     0,     0,     0,     0,     0,     0,   307,     0,     0,
       0,     0,     0,   312,     0,   313,   314,   315,   308,   316,
     317,   318,     0,   319,   320,   321,   322,   323,     0,   324,
     325,   326,   327,   309,   328,   329,   330,   310,     0,   331,
     332,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   306,   887,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,   309,     0,     0,   311,   310,   308,
       0,     0,     0,     0,     0,     0,     0,   870,     0,     0,
       0,     0,     0,   312,     0,   313,   314,   315,     0,   316,
     317,   318,     0,   319,   320,   321,   322,   323,   311,   324,
     325,   326,   327,     0,   328,   329,   330,     0,     0,   331,
     332,   333,     0,     0,   312,   309,   313,   314,   315,   310,
     316,   317,   318,     0,   319,   320,   321,   322,   323,     0,
     324,   325,   326,   327,     0,   328,   329,   330,   306,     0,
     331,   332,   333,     0,     0,   307,     0,     0,     0,   311,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,     0,     0,     0,     0,   312,     0,   313,   314,   315,
       0,   316,   317,   318,     0,   319,   320,   321,   322,   323,
       0,   324,   325,   326,   327,     0,   328,   329,   330,     0,
       0,   331,   332,   333,     0,     0,     0,     0,     0,     0,
       0,     0,   309,     0,     0,     0,   310,     0,     0,     0,
       0,     0,     0,   306,     0,   873,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,     0,   316,   317,
     318,     0,   319,   320,   321,   322,   323,     0,   324,   325,
     326,   327,     0,   328,   329,   330,     0,   309,   331,   332,
     333,   310,     0,     0,     0,     0,     0,     0,   306,     0,
     963,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,     0,   316,   317,   318,     0,   319,   320,   321,
     322,   323,     0,   324,   325,   326,   327,     0,   328,   329,
     330,     0,   309,   331,   332,   333,   310,     0,     0,     0,
       0,     0,     0,   306,     0,   964,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,     0,   316,   317,
     318,     0,   319,   320,   321,   322,   323,     0,   324,   325,
     326,   327,     0,   328,   329,   330,     0,   309,   331,   332,
     333,   310,     0,     0,     0,     0,     0,     0,   306,     0,
     965,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,     0,   316,   317,   318,     0,   319,   320,   321,
     322,   323,     0,   324,   325,   326,   327,     0,   328,   329,
     330,     0,   309,   331,   332,   333,   310,     0,     0,     0,
       0,     0,     0,   306,     0,   966,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,     0,   316,   317,
     318,     0,   319,   320,   321,   322,   323,     0,   324,   325,
     326,   327,     0,   328,   329,   330,     0,   309,   331,   332,
     333,   310,     0,     0,     0,     0,     0,     0,   306,     0,
     967,     0,     0,     0,     0,   307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   308,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,     0,   316,   317,   318,     0,   319,   320,   321,
     322,   323,     0,   324,   325,   326,   327,     0,   328,   329,
     330,     0,   309,   331,   332,   333,   310,     0,     0,     0,
       0,     0,     0,   306,     0,   968,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,     0,   313,   314,   315,     0,   316,   317,
     318,     0,   319,   320,   321,   322,   323,     0,   324,   325,
     326,   327,     0,   328,   329,   330,     0,   309,   331,   332,
     333,   310,     0,   306,     0,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,   308,   316,   317,   318,     0,   319,   320,   321,
     322,   323,     0,   324,   325,   326,   327,   653,   328,   329,
     330,   310,     0,   331,   332,   333,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,     0,
       0,   311,   310,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,   313,
     314,   315,     0,   316,   317,   318,     0,   319,   320,   321,
     322,   323,   311,   324,   325,   326,   327,     0,   328,   329,
     330,     0,     0,   331,   332,   333,     0,     0,   312,     0,
     313,   314,   315,     0,   316,   317,   318,     0,   319,   320,
     321,   322,     0,     0,   324,   325,   326,   327,   626,   328,
     198,   330,     0,     0,   331,   332,   333,     0,     0,     0,
       0,     0,   199,   200,     0,     0,     0,     0,     0,   201,
       0,     0,     0,     0,     0,     0,     0,   202,     0,     0,
       0,     0,     0,   203,     0,     0,     0,     0,   204,     0,
     205,     0,     0,   206,   198,     0,     0,     0,     0,   207,
       0,     0,     0,     0,     0,     0,   199,   200,     0,     0,
     208,     0,     0,   201,     0,     0,     0,     0,     0,   209,
       0,   202,     0,     0,     0,     0,     0,   203,   210,   211,
       0,   212,   204,   213,   205,   214,     0,   206,   215,     0,
       0,     0,   216,   207,     0,   217,     0,     0,     0,     0,
       0,     0,     0,     0,   208,     0,     0,     0,     0,     0,
       0,     0,     0,   209,     0,     0,     0,     0,     0,     0,
       0,     0,   210,   211,     0,   212,     0,   213,     0,   214,
       0,     0,   215,     0,     0,     0,   216,     0,     0,   217
};

static const yytype_int16 yycheck[] =
{
      11,     2,   135,   173,    15,   429,   288,    18,    52,    53,
      17,   355,    23,   709,   184,   549,    27,    28,   635,    30,
      31,    32,    33,   398,     1,   401,   179,   577,     1,   197,
      41,   735,    43,    44,   406,    46,     1,    48,    49,     1,
      42,    29,    29,    60,   197,    56,    57,    58,    59,    60,
      61,   826,    56,    29,    65,    19,    67,   429,    69,    70,
       0,   783,   784,    74,    75,    76,    77,   235,   901,    80,
     391,    75,    83,    84,    85,   823,    87,   129,   129,    90,
      91,    92,   235,    29,   101,   941,   254,    42,    92,   129,
      42,   129,   788,   300,   301,   302,   303,    42,    60,     1,
      42,   254,   652,    42,    77,    42,   157,   159,    77,    73,
     129,   149,    77,   434,   104,   155,   650,    90,   149,    24,
     149,   104,    42,    75,   980,    19,   155,   129,   261,   102,
      75,   827,   109,    97,     3,    30,   155,   102,   155,   101,
      42,   129,   917,   891,   155,   156,   157,   158,   159,   160,
     161,   128,   129,   857,   987,   128,   706,   161,   104,    64,
      28,    42,   104,   128,   317,   172,   154,   154,   179,   159,
      75,   149,    24,    77,   129,   879,   149,   155,   154,    73,
     157,   154,    94,   351,   352,   154,   197,   129,   156,   154,
     129,   129,   129,   155,   890,   100,    77,    57,   351,   352,
     576,   923,   924,    97,   159,   549,    74,   109,   154,   129,
      78,   109,    64,   156,   129,   158,    77,   159,   129,   157,
     159,   102,   159,    75,   235,   236,   128,   129,   109,    90,
     156,    91,   243,   244,   149,   133,   129,   109,   156,   159,
     158,   102,    94,   254,   940,   149,   157,   149,   129,   866,
     154,   129,   129,   460,   422,    47,   124,   129,   126,   127,
     128,   133,   130,   131,   157,   547,   548,   135,   149,   422,
     129,   149,   283,   154,   980,   143,   157,    69,   985,   280,
     157,   815,   150,   151,   152,   157,     7,   994,   149,   156,
     149,    42,   998,   154,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   650,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   698,   700,    85,    77,   129,     2,   340,
     341,   156,   614,   615,   129,   346,     3,    11,   349,   350,
     351,   352,   353,   354,   355,   129,   109,   129,    22,    24,
      24,   102,    74,   364,    70,   157,    78,   368,   109,   370,
     735,   129,   157,    30,   129,   139,   129,   149,   129,   380,
     133,   382,   383,   157,   385,   386,   380,   149,   129,   390,
     391,   149,   393,   536,   395,    52,   156,   129,   149,    64,
     129,    65,   157,    28,   157,    74,   149,   408,   149,    78,
      75,    69,    70,   154,   415,   128,   157,   149,   131,   833,
     149,   422,   135,   149,   309,   310,    90,   149,   149,    94,
      94,    88,   433,   434,   156,   149,   158,    94,    24,   440,
     149,   156,   443,   158,   445,   605,   447,   448,   449,    74,
     451,   452,   149,    78,   149,   456,    42,    77,   443,    45,
     445,   131,   456,    28,   826,   154,   149,   103,   353,   354,
      90,   833,   156,   149,   156,    61,   477,   478,    64,   364,
      66,   815,   102,   156,     7,   109,   149,   151,    54,    75,
     340,   341,   154,   128,   149,   109,   346,   862,   383,   349,
     350,   386,   129,   128,   157,   130,   131,   149,    94,    74,
     135,    42,   635,    78,   879,   156,    31,   157,   143,    28,
     521,   522,   523,   524,   525,   526,   527,   152,   129,   149,
     109,   528,   529,   534,   154,   536,   157,     2,   539,   157,
     541,   157,    18,   129,   545,   129,    11,   159,   549,   154,
     149,   149,   149,   149,   218,   917,   149,    22,   129,    24,
     149,   225,   447,   128,    42,    74,   131,   157,   159,    78,
     135,   157,   149,    77,   149,    31,   157,   578,   154,   941,
     581,   582,   583,   584,   585,   586,    90,   155,   950,   590,
     157,   149,   586,   443,   149,   445,   129,   109,   102,    77,
      65,   155,    75,    29,    69,    70,    29,   159,   154,    31,
     128,    75,   159,   156,   128,   124,   280,   159,   980,   128,
     159,   130,   131,   985,   102,    90,   135,   155,   131,    94,
     131,   109,   994,   131,   143,   131,   998,   522,   131,   524,
     131,   150,   151,   152,   131,   149,   131,   131,   131,   650,
     154,   129,   653,   654,   655,    31,     3,   658,   157,   131,
     102,   155,   157,    90,    28,   733,   434,   159,    15,    16,
     433,   149,   673,    21,   675,    22,   154,   678,   185,   157,
     681,   720,   262,    30,   534,   686,   151,   688,   801,    36,
     691,   538,   738,   866,    41,   862,    43,   569,   362,    46,
     698,   741,   987,   940,   959,    52,    32,   409,   536,    27,
      74,    -1,    -1,    -1,    78,   716,    63,    -1,   382,    -1,
      -1,   385,   716,    -1,    -1,    72,   727,    -1,   729,   393,
      -1,   732,    -1,    -1,    81,    82,   737,    84,    -1,    86,
     590,    88,    -1,   866,    91,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   218,     2,    -1,    -1,    -1,    -1,    -1,
     225,    -1,    -1,    11,   128,    -1,   130,   131,   653,   654,
      59,   135,    -1,    -1,    22,    -1,    24,    -1,   779,   143,
      -1,    -1,    -1,    -1,    -1,   449,   150,   151,   152,    -1,
      -1,   792,   793,    -1,   795,   796,   797,   798,    24,    -1,
      -1,    -1,   803,   804,    -1,   806,    -1,   808,   809,   156,
      -1,   812,    -1,    -1,   815,   280,    42,    65,    -1,    45,
     854,   855,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   832,   833,    -1,    -1,    61,    -1,   838,    64,   840,
      66,    91,    90,   844,    -1,    -1,    94,    -1,    -1,    75,
      -1,    -1,   853,    -1,   518,    -1,    -1,    -1,     2,    -1,
     851,    -1,    -1,    -1,    -1,    -1,    -1,    11,    94,   870,
     871,   872,   873,   874,   875,   876,   877,    -1,    22,    -1,
      24,    -1,    -1,    -1,    -1,    -1,   887,    -1,    -1,    -1,
      -1,   892,    -1,    -1,    -1,   559,   897,   362,   892,   900,
     189,   190,   191,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   382,    -1,    -1,
     385,    65,    -1,    -1,    -1,    -1,    -1,    -1,   393,   179,
      -1,    -1,   933,   934,   935,   936,     2,    28,    -1,   228,
      -1,    -1,   231,    -1,    -1,    11,    90,   197,    -1,   950,
      94,   952,   953,   954,    -1,   619,    22,    -1,    24,    -1,
     954,   250,   963,   964,   965,   966,   967,   968,    -1,    -1,
     218,   635,    -1,    -1,    -1,    -1,    -1,   225,   979,   980,
      -1,   270,    -1,    74,   449,   235,    -1,    78,    -1,   653,
     654,    -1,    -1,   994,    -1,    -1,    -1,   998,    -1,    65,
      -1,    -1,    -1,    -1,   254,    -1,    -1,   151,    -1,   673,
      -1,   675,     1,    -1,   678,    -1,    -1,   681,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,   690,    -1,    94,   693,
      19,    -1,   280,   124,    23,    24,   127,   128,    -1,   130,
     131,    -1,    -1,    32,   135,    34,    35,    -1,    -1,    -1,
      39,   715,   143,    -1,    -1,    44,    -1,    -1,    47,   150,
     151,   152,    -1,   727,    -1,   729,    -1,    -1,    -1,   733,
      24,    -1,    -1,    62,   218,    64,   365,    66,    -1,    68,
      69,   225,    71,    -1,    73,   151,    75,    -1,    42,    -1,
      -1,    45,    -1,    -1,   559,    -1,    -1,    -1,    -1,    -1,
     389,   351,   352,    -1,     3,    94,    -1,    61,    97,    -1,
      64,   100,    66,    -1,   362,    -1,    15,    23,    -1,    -1,
      -1,    75,    -1,    22,    -1,    -1,    -1,    -1,    34,    35,
      -1,    -1,    -1,    -1,   382,    -1,   280,   385,    44,   803,
      94,    47,    41,    -1,   808,   393,    -1,    46,    54,    -1,
      -1,    -1,   218,    52,   619,    -1,    62,    -1,   822,   225,
     149,    -1,    -1,    69,    70,   415,   155,    -1,    -1,    -1,
     834,    -1,   422,    72,    -1,    -1,    -1,    -1,    -1,   843,
      -1,    -1,    -1,   847,    -1,    84,    -1,   851,    23,    88,
     440,    -1,    -1,   443,    -1,   445,    95,    -1,   448,    34,
      35,   449,    38,    -1,    -1,    -1,    -1,    -1,   673,    44,
     675,    -1,    47,   678,   280,    -1,   681,    -1,   362,    -1,
      -1,    -1,    -1,    -1,   888,   690,    -1,    62,   693,    -1,
      -1,    -1,    -1,    -1,    69,    71,    -1,    -1,   382,    -1,
      -1,   385,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   393,
     715,    -1,    -1,    -1,   543,   544,    -1,    -1,   547,   548,
      -1,    -1,   727,    -1,   729,    -1,    -1,    -1,   733,   933,
     934,   935,   936,    -1,    -1,    -1,    -1,    -1,    -1,    23,
     116,    -1,   118,    -1,   534,    -1,   536,    -1,   952,   953,
      34,    35,     2,    -1,    -1,    -1,   362,    -1,    -1,    -1,
      44,    11,    -1,    47,    -1,   449,    -1,    -1,    -1,    -1,
      54,   559,    22,    -1,    24,    -1,   382,    -1,    62,   385,
      -1,    -1,    -1,    -1,    -1,    69,    70,   393,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,   803,    -1,
      -1,    -1,    -1,   808,    -1,    -1,    -1,    -1,   184,    -1,
      -1,   187,    -1,    -1,    -1,    65,    -1,   822,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   834,
      -1,   619,    -1,    -1,   663,   664,    -1,    -1,   843,    -1,
      90,   670,   847,   449,    94,   221,   851,    -1,    -1,    -1,
      -1,   680,    -1,    -1,   683,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   559,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   888,    -1,   673,    -1,   675,    -1,    -1,
     678,    -1,    -1,   681,    -1,     2,    -1,    -1,    -1,    -1,
      -1,   151,   690,    -1,    11,   693,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   715,   933,   934,
     935,   936,    -1,    -1,    -1,   619,    -1,    -1,    -1,   727,
      -1,   729,    -1,    -1,    -1,   733,    -1,   952,   953,    -1,
      -1,    -1,    -1,   559,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,   673,
      -1,   675,    -1,    -1,   678,    -1,    -1,   681,    -1,    -1,
      -1,    -1,   378,    -1,    -1,    -1,   690,   836,    -1,   693,
      -1,    -1,    -1,   619,    -1,   803,    -1,    -1,    -1,    -1,
     808,    -1,    -1,    -1,    -1,   401,    -1,    -1,    -1,    -1,
     280,   715,    -1,    -1,   822,    -1,   826,    -1,     2,    -1,
      -1,    -1,    -1,   727,   151,   729,   834,    11,    -1,   733,
      -1,    -1,    -1,    -1,    -1,   843,    -1,    -1,    22,   847,
      24,    -1,    -1,   851,    -1,    -1,    -1,   673,    -1,   675,
      -1,    -1,   678,    -1,    -1,   681,    -1,    -1,    -1,   455,
      -1,    -1,   458,    -1,   690,    -1,    -1,   693,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     888,    65,    -1,    -1,    -1,    -1,    -1,   897,    -1,   715,
      -1,   218,   362,    -1,    -1,    -1,    -1,    -1,   225,   803,
      -1,   727,    -1,   729,   808,    -1,    90,   733,    -1,    -1,
      94,    -1,   382,    -1,    -1,   385,    -1,    -1,   822,    -1,
      -1,    -1,    -1,   393,    -1,   933,   934,   935,   936,    -1,
     834,   941,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   843,
      -1,    -1,    -1,   847,   952,   953,    -1,   851,    -1,    -1,
      -1,    -1,    -1,   280,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,    -1,    28,
     980,    -1,    -1,    -1,    -1,    -1,   572,   803,    -1,   449,
     576,    -1,   808,    -1,   888,    -1,    -1,    -1,   998,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   822,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   601,    -1,    -1,   834,   605,
      -1,    -1,    -1,    -1,    -1,    74,    -1,   843,    -1,    78,
      -1,   847,    -1,    -1,    -1,   851,    -1,    -1,    -1,   933,
     934,   935,   936,    -1,   218,   362,    -1,    -1,    -1,    -1,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,   952,   953,
      -1,    -1,    -1,    -1,    -1,   382,    -1,    -1,   385,    -1,
      -1,    -1,   888,    -1,    -1,   124,   393,   126,   127,   128,
      -1,   130,   131,    28,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    41,    -1,    -1,   147,   559,
      -1,   150,   151,   152,    -1,    -1,   280,    -1,    -1,    -1,
      -1,   697,    -1,    59,   700,    -1,    -1,   933,   934,   935,
     936,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    74,
      -1,    -1,   449,    78,   720,    -1,   952,   953,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,   738,    -1,    -1,    -1,    -1,    -1,    11,   619,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    24,   758,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,   362,   134,
     135,   136,   137,    -1,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,    -1,   150,   151,   152,   382,    -1,
      -1,   385,    65,   673,    -1,   675,    -1,    -1,   678,   393,
      -1,   681,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
     690,    -1,    -1,   693,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,   559,   189,   190,   191,    -1,    -1,    -1,    -1,
     196,    -1,    -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,   727,    23,   729,
      -1,    -1,    -1,   733,    -1,   449,    -1,    32,    -1,    34,
      35,    -1,   228,    -1,    39,   231,    -1,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,   619,    -1,   250,    -1,    -1,    62,    -1,    -1,
      -1,    66,    -1,    68,    69,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    -1,    97,   803,    -1,    -1,    -1,    -1,   808,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   673,    -1,   675,    -1,
      -1,   678,   822,    -1,   681,   218,    -1,    -1,    -1,    -1,
      -1,    -1,   225,   690,   834,    -1,   693,    -1,    -1,    -1,
      -1,    -1,    -1,   843,    -1,   559,    -1,   847,    -1,    -1,
      -1,   851,    -1,    -1,   149,    -1,    -1,    -1,   715,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     727,    -1,   729,    23,    -1,    -1,   733,    -1,    -1,   365,
      -1,    -1,    32,    -1,    34,    35,    -1,   280,   888,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,   389,    -1,   619,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    66,    -1,    68,    69,
      -1,    71,    -1,    73,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   933,   934,   935,   936,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,   803,    97,    -1,    -1,
      -1,   808,   952,   953,    -1,    -1,    -1,    -1,    -1,   673,
      -1,   675,    -1,    -1,   678,   822,    -1,   681,    28,   362,
      -1,    74,    -1,    -1,    -1,    78,   690,   834,    -1,   693,
      -1,    -1,    -1,    -1,    -1,    -1,   843,    -1,    -1,   382,
     847,    -1,   385,    -1,   851,    -1,    -1,    -1,    -1,   149,
      -1,   715,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   727,    74,   729,    -1,    -1,    78,   733,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   888,   135,   136,   137,    -1,    -1,   140,   141,   142,
     143,    -1,    -1,    -1,   147,    -1,    -1,   150,   151,   152,
      -1,    -1,    -1,    -1,    -1,    -1,   449,   543,   544,    -1,
      -1,   547,   548,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   560,   135,   933,   934,   935,   936,
       1,    -1,   142,   143,    -1,    -1,    -1,   147,    -1,   803,
     150,   151,   152,    -1,   808,   952,   953,    -1,    19,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,    -1,   822,    -1,
      -1,    32,    -1,    34,    35,   601,    -1,    -1,    39,    -1,
     834,    -1,    -1,    44,    -1,    -1,    47,    -1,    -1,   843,
      -1,    -1,    -1,   847,    -1,    -1,    -1,   851,    -1,    -1,
      -1,    62,    -1,    64,    -1,    66,    -1,    68,    69,    -1,
      71,    -1,    73,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   559,    -1,    -1,    -1,
      -1,    -1,    -1,    94,   888,    -1,    97,   663,   664,   100,
      -1,    -1,    -1,    -1,   670,    -1,   672,    -1,    -1,    -1,
      -1,   677,    -1,    -1,   680,    -1,    -1,   683,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,   933,
     934,   935,   936,     3,    -1,    -1,   619,    -1,   149,   139,
      -1,    -1,    -1,    -1,   155,    15,    16,   723,   952,   953,
      -1,    -1,    22,   153,    -1,    -1,   156,    -1,   158,    -1,
      30,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     673,    -1,   675,    63,    -1,   678,     3,    -1,   681,    -1,
      -1,    -1,    72,    -1,    -1,    75,    -1,   690,    15,    16,
     693,    81,    82,    -1,    84,    22,    86,    -1,    88,    -1,
      -1,    91,    -1,    30,    -1,    95,    -1,    -1,    98,    36,
      -1,    -1,   715,    -1,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,   727,    52,   729,    -1,    -1,    -1,
     733,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
     836,    -1,    -1,    -1,    -1,    72,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    -1,    84,    -1,    86,
      -1,    88,    -1,    -1,    91,    -1,   156,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   881,   882,    -1,   884,   885,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   901,    -1,    -1,    -1,    -1,
      -1,   907,    -1,   909,    -1,   911,    -1,    -1,    -1,   822,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,
      -1,   834,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     843,    -1,    -1,   124,   847,   126,   127,   128,   851,   130,
     131,   132,    -1,   134,   135,   136,   137,    -1,    -1,   140,
     141,   142,   143,    -1,   145,    -1,   147,    -1,    -1,   150,
     151,   152,    -1,    -1,     0,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,   888,    12,    13,    14,    15,
      16,   987,    -1,    19,    20,    21,    22,    23,    24,    25,
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
     146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,
     156,     1,   158,     3,     4,     5,     6,     7,     8,    -1,
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
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    17,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,   144,   145,   146,   147,    -1,   149,   150,
     151,   152,    -1,   154,   155,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,   148,   149,    -1,    -1,   152,    -1,   154,
     155,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,   154,   155,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
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
      -1,   154,   155,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,   154,   155,   156,
       1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   154,   155,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
      -1,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    -1,    97,    98,    99,   100,    -1,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,    -1,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,   156,
       1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   154,    -1,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
      -1,   156,     1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      29,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    43,    44,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    67,    68,
      69,    70,    71,    72,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    82,    83,    84,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    -1,    97,    98,
      99,   100,    -1,    -1,   103,    -1,   105,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
     149,    -1,    -1,   152,    -1,   154,    -1,   156,     1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    67,    68,    69,    70,    71,    72,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    82,
      83,    84,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    -1,    97,    98,    99,   100,    -1,    -1,
     103,    -1,   105,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
      -1,   144,    -1,   146,   147,    -1,   149,    -1,    -1,   152,
      -1,   154,    -1,   156,     1,   158,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    43,    44,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    58,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      67,    68,    69,    70,    71,    72,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    82,    83,    84,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    99,   100,    -1,    -1,   103,    -1,   105,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,   146,
     147,    -1,   149,    -1,    -1,   152,    -1,   154,    -1,   156,
       1,   158,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    43,    44,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    58,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    67,    68,    69,    70,
      71,    72,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    82,    83,    84,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,    99,   100,
      -1,    -1,   103,    -1,   105,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,    -1,   146,   147,    -1,   149,    -1,
      -1,   152,    -1,   154,    -1,   156,     1,   158,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    29,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    43,    44,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    67,    68,    69,    70,    71,    72,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    82,    83,    84,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    99,   100,    -1,    -1,   103,    -1,
     105,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
      -1,   156,     1,   158,     3,     4,     5,     6,     7,     8,
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
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,
      -1,    -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,
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
     146,   147,   148,    -1,    -1,    -1,   152,    -1,   154,    -1,
     156,    -1,   158,   159,     3,     4,     5,     6,     7,     8,
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
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,
      -1,    -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,
     159,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    19,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    47,    -1,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    65,    -1,    67,    -1,    69,    -1,    -1,
      72,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,    -1,    91,
      -1,    -1,    94,    95,    -1,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,   124,   125,   126,   127,    -1,    10,    -1,    -1,
     132,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,    22,
     142,    -1,   144,    -1,   146,   147,    -1,    30,    -1,    -1,
     152,    -1,   154,    36,   156,    -1,   158,    -1,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      -1,    84,    -1,    86,    -1,    88,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,   125,   126,   127,    -1,    -1,   130,    -1,    -1,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    -1,   147,    -1,    -1,   150,   151,   152,
     153,    -1,    -1,   156,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    47,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    72,    73,    -1,    -1,    -1,    -1,    -1,
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
      96,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,    -1,    -1,    -1,    -1,   132,   133,    -1,    -1,
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
      -1,    91,    92,    93,    -1,    95,    96,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,   152,    -1,   154,    -1,   156,   157,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    92,    93,
      -1,    95,    96,    -1,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
     144,    -1,   146,   147,   148,    -1,    -1,    -1,   152,    -1,
     154,    -1,   156,   157,   158,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    42,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      85,    86,    -1,    88,    -1,    -1,    91,    92,    93,    -1,
      95,    -1,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,
      -1,   146,   147,    -1,   149,    -1,    -1,   152,    -1,   154,
      -1,   156,    -1,   158,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,    38,
      -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    84,    85,    86,    -1,    88,
      -1,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,   144,    -1,   146,   147,   148,
      -1,    -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,    -1,
      43,    -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    63,    64,    65,    -1,    67,    -1,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    84,    85,    86,    -1,    88,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,    -1,   146,   147,    -1,    -1,
      -1,    -1,   152,    -1,   154,    -1,   156,    -1,   158,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    35,    36,    37,    38,    -1,    40,    41,    -1,    43,
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
      41,    -1,    43,    -1,    -1,    46,    47,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    69,    -1,
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
      -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,   142,    -1,
     144,    12,   146,   147,    15,    16,    -1,    -1,   152,    -1,
      -1,    22,   156,    -1,   158,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,
      41,    -1,    43,    -1,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    86,    -1,    88,    -1,    -1,
      91,    -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,   142,    -1,   144,    12,    -1,   147,    15,    16,    -1,
      -1,   152,    -1,   154,    22,   156,    -1,   158,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    81,    82,    83,    84,    85,    86,    32,
      88,    34,    35,    91,    -1,    -1,    39,    95,    -1,    -1,
      98,    44,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      -1,    -1,    -1,    66,    -1,    68,    69,   125,    71,    -1,
      73,   129,    -1,    -1,    -1,   133,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,   144,    -1,    15,    16,
     148,    94,    -1,    -1,    97,    22,   154,    -1,   156,   157,
     158,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    36,
      37,    38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    -1,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,   149,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    88,    -1,    90,    91,    -1,    -1,    -1,    95,    -1,
      -1,    98,    -1,    -1,    -1,   102,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    15,    16,    -1,    -1,    -1,    -1,   144,    22,    -1,
     153,    -1,   149,   156,    -1,   158,    30,   154,    -1,   156,
      -1,   158,    36,    37,    38,    -1,    40,    41,    -1,    43,
      -1,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      84,    85,    86,    -1,    88,    -1,    -1,    91,    -1,    -1,
      -1,    95,    -1,    -1,    98,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    -1,    40,    41,    -1,    43,    -1,    -1,    46,    -1,
     144,    49,    50,    -1,    52,    53,    -1,    55,    56,    -1,
     154,    -1,   156,    -1,   158,    63,    -1,    65,    -1,    67,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    84,    85,    86,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,
      98,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   125,    30,    -1,
      -1,    -1,    -1,    -1,    36,    37,    38,    -1,    40,    41,
      -1,    43,    -1,    -1,    46,    -1,   144,    49,    50,    -1,
      52,    53,    -1,    55,    56,    -1,   154,    -1,   156,    -1,
     158,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    86,    -1,    88,    -1,     3,    91,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36,    -1,   125,    -1,    -1,    41,    -1,    43,    -1,
      -1,    46,     3,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,   144,    -1,    15,    16,    -1,    -1,    63,    -1,
      -1,    22,    -1,    -1,   156,    -1,   158,    72,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    36,    81,    82,    -1,    84,
      41,    86,    43,    88,    -1,    46,    91,    -1,    -1,    -1,
      95,    52,    -1,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    72,    10,    -1,    -1,    -1,    -1,    15,    16,    17,
      81,    82,    -1,    84,    22,    86,    -1,    88,    -1,    -1,
      91,    -1,    30,    -1,    95,    -1,    -1,    98,    36,    -1,
      -1,    -1,    -1,    41,    -1,    43,    -1,    -1,    46,    -1,
      -1,   156,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    -1,    84,    -1,    86,    -1,
      88,    -1,    10,    91,    -1,   156,    -1,    95,    -1,    17,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,   125,   126,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,   150,   151,   152,   153,    74,    -1,    -1,    -1,
      78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    74,   145,   146,   147,
      78,    -1,   150,   151,   152,    -1,    -1,    10,    -1,    -1,
      -1,   159,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,    -1,    10,    -1,    -1,
      -1,   159,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    74,   145,   146,   147,    78,    -1,   150,   151,   152,
      -1,    -1,    10,    -1,    -1,    -1,   159,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,    -1,    10,    -1,    -1,    -1,   159,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    74,   145,   146,   147,
      78,    -1,   150,   151,   152,    -1,    -1,    10,    -1,   157,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,   157,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,   154,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,   154,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,   154,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    74,   150,   151,   152,
      78,   154,    -1,    -1,    -1,    -1,    -1,    10,    -1,    87,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,    -1,    -1,   117,
     118,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    74,   150,   151,   152,    78,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,   108,   109,   110,   111,   112,
     113,   114,    28,    29,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126,   127,   128,    42,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    10,    -1,   150,   151,   152,
      -1,   154,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    78,    28,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    74,
     126,   127,   128,    78,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    10,    -1,   150,   151,   152,    -1,   154,    17,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    10,    -1,   150,   151,   152,    -1,   154,
      17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      78,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
     108,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    74,   126,   127,
     128,    78,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,    -1,   150,   151,   152,    -1,   154,    -1,    74,    -1,
      -1,   108,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    28,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   108,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,    -1,   154,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    74,   145,
     146,   147,    78,    -1,   150,   151,   152,    -1,   154,    10,
      -1,    87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,   154,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    59,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    74,   145,   146,   147,    78,    -1,   150,
     151,   152,    -1,   154,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,   152,    74,    -1,    -1,    -1,    78,    -1,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    33,   104,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    59,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    74,   145,   146,   147,    78,    -1,   150,   151,
     152,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    10,    42,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    28,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,   108,    78,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   108,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,    -1,    -1,   124,    74,
     126,   127,   128,    78,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    10,   149,   150,   151,   152,    -1,    -1,    17,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    10,   149,   150,   151,   152,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      78,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    74,   126,   127,
     128,    78,   130,   131,   132,    -1,   134,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      10,   149,   150,   151,   152,    -1,    -1,    17,    -1,    -1,
      -1,   108,    -1,    74,    -1,    -1,    -1,    78,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,   108,   145,   146,
     147,    -1,   149,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    74,   126,   127,   128,    78,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    10,    -1,   150,
     151,   152,    -1,    -1,    17,    -1,    -1,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    10,    -1,
     150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    74,    -1,    -1,    -1,    78,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    74,   126,   127,   128,    78,   130,   131,   132,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,    -1,   150,   151,   152,
      -1,    -1,   104,    74,    -1,    -1,   108,    78,    -1,    10,
      11,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    28,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,   108,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    -1,   150,   151,
     152,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    74,   145,   146,   147,    78,   149,   150,
     151,   152,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    31,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    74,   150,
     151,   152,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    74,   145,
     146,   147,    78,    -1,   150,   151,   152,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,    74,   150,   151,   152,    78,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    28,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    74,   145,   146,   147,    78,    -1,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    74,    -1,    -1,   108,    78,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,
     131,   132,    -1,   134,   135,   136,   137,   138,   108,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,    -1,   150,
     151,   152,    -1,    -1,   124,    74,   126,   127,   128,    78,
     130,   131,   132,    -1,   134,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    10,    -1,
     150,   151,   152,    -1,    -1,    17,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
      -1,   150,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      87,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    74,   150,   151,   152,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    87,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,    74,   150,   151,
     152,    78,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    28,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    74,   145,   146,
     147,    78,    -1,   150,   151,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,   108,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
     137,   138,   108,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,    -1,   150,   151,   152,    -1,    -1,   124,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,   135,
     136,   137,    -1,    -1,   140,   141,   142,   143,     1,   145,
       3,   147,    -1,    -1,   150,   151,   152,    -1,    -1,    -1,
      -1,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      43,    -1,    -1,    46,     3,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    -1,    -1,
      63,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    36,    81,    82,
      -1,    84,    41,    86,    43,    88,    -1,    46,    91,    -1,
      -1,    -1,    95,    52,    -1,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    -1,    84,    -1,    86,    -1,    88,
      -1,    -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    98
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
     217,   218,   241,   244,   245,   265,   266,   267,   268,   269,
     270,   278,   279,   280,   281,   284,   285,   286,   287,   288,
     289,   290,   292,   293,   294,   295,   296,   149,   168,   281,
     104,   271,   272,     3,   180,    12,    37,    38,    40,    86,
     154,   158,   214,   265,   269,   279,   280,   281,   284,   286,
     287,   271,   281,    94,   180,   168,   259,   281,   168,   156,
     281,   282,     7,     9,    94,   187,   282,    64,   106,   195,
     281,   281,    23,    35,   244,   281,   281,   156,     3,    15,
      16,    22,    30,    36,    41,    43,    46,    52,    63,    72,
      81,    82,    84,    86,    88,    91,    95,    98,   182,   184,
     215,   156,   182,   246,   247,    29,   172,   188,   281,   281,
     281,   281,   281,   281,     7,   156,    35,   191,   191,   148,
     237,   258,   281,    92,    93,   149,   281,   283,   281,   188,
     281,   281,   281,    85,   156,   168,   281,   281,   172,   179,
     281,   284,   172,   179,   281,   182,   242,   281,   258,   281,
     281,   281,   281,   281,   281,   281,   281,     1,   155,   166,
     173,   258,    96,   133,   237,   260,   261,   283,   258,   281,
     291,    66,   168,    70,   171,   182,   182,    47,    69,   232,
      24,    64,    75,   100,   149,   139,    10,    17,    28,    74,
      78,   108,   124,   126,   127,   128,   130,   131,   132,   134,
     135,   136,   137,   138,   140,   141,   142,   143,   145,   146,
     147,   150,   151,   152,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   153,
     224,   156,   158,    74,    78,   156,   168,   149,   281,   281,
     281,   258,   154,   172,    42,   271,   242,   149,   129,   149,
     103,   183,   237,   262,   263,   264,   283,   242,   207,   182,
     156,   188,    29,    42,   188,    29,    42,   104,   188,   274,
      33,    59,   149,    87,   172,   224,   262,   168,   156,   226,
      96,   156,   182,   248,   249,     1,   128,   253,    42,   129,
     168,   188,   188,   262,   182,   129,   149,   281,   281,   149,
     154,   188,   156,   262,   149,   199,   149,   199,   149,   109,
     243,   149,   149,    33,    59,   188,   149,   155,   155,   166,
     129,   155,   281,   129,   157,   129,   157,    42,   129,   159,
     274,   107,   129,   159,     7,    54,   128,   208,   154,   219,
      75,   246,   246,   246,   246,   281,   281,   281,   281,   195,
     281,   195,   281,   281,   281,   281,   281,    16,    98,   281,
     281,    30,    52,    94,   183,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   283,   283,   283,   283,   283,   262,   262,   195,   281,
     195,   281,    24,    42,    75,   100,   273,   276,   277,   281,
     296,    29,    42,    29,    42,    87,    42,   159,   195,   281,
     188,   149,   281,   281,   109,   157,   129,   149,   182,    42,
     258,    40,   281,   195,   281,    40,   281,   195,   281,   156,
     188,   152,   174,   176,   281,   174,   175,   168,   281,    31,
     281,   157,    24,    42,    45,    61,    64,    75,    94,   167,
     227,   228,   229,   230,   216,   249,   129,   157,    16,    30,
      43,    63,    81,    82,    86,    98,   158,   186,   254,   266,
     109,   250,   281,   247,   157,   237,   281,     1,   204,   262,
     157,    18,   200,   254,   266,   129,   176,   175,   155,   157,
     157,   260,   157,   260,   195,   281,   159,   168,   281,   159,
     281,   159,   281,   182,   258,   154,     1,   182,   212,   213,
      24,    64,    75,    94,   221,   231,   246,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   157,   159,    42,    75,
     129,   157,   265,    74,    74,    40,   195,   281,    40,   195,
     281,   195,   281,   271,   271,   149,   237,   283,   264,   208,
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
     181,   184,   222,   223,   224,   265,   149,   276,   253,   265,
     265,   281,    29,    29,   281,    29,    29,   159,   159,   188,
     188,   154,   188,    87,    40,   281,    40,   281,    87,    40,
     281,   188,    40,   281,   188,   129,   157,   281,   281,   168,
     281,   168,    31,   182,   228,   249,   128,   257,    75,   253,
     250,   159,    42,   159,    29,   168,   258,   201,   128,   172,
      40,   168,   281,   159,    40,   168,   281,   159,   281,     1,
     155,   173,   155,    35,    68,    71,   155,   167,   190,   281,
     213,   233,   156,   225,   182,   281,   131,   225,   225,   250,
      87,    40,    40,    87,    40,    40,    40,    40,     1,   209,
     281,   281,   281,   281,   281,   281,   276,    11,    31,   168,
     257,   157,   158,   186,   237,   256,   266,   133,   239,   250,
     102,   240,   281,   254,   266,   168,   188,   157,   281,   281,
     168,   281,   168,   155,   155,   190,     1,   128,   252,   227,
     157,     3,    88,   223,   224,   281,   281,   281,   281,   281,
     281,   155,   155,    87,    87,    87,    87,   281,   168,   250,
     257,   159,   258,   237,   238,   281,   281,   172,   189,   236,
     159,   172,    87,    87,   158,   186,   251,   266,    90,   234,
     157,   225,   225,    87,    87,    87,    87,    87,    87,   281,
     281,   281,   281,   239,   250,   237,   255,   256,   266,    42,
     159,   266,   281,   281,     1,   159,   258,   240,   281,   255,
     256,   159,   251,   266,   159,   149,   235,   236,   159,   251,
     266,   255
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
       0,     2,     2,     4,     4,     4,     5,     4,     0,     1,
       1,     1,     3,     3,     5,     1,     1,     3,     1,     2,
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
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 5532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); resetTempID(); }
#line 5538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 5544 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 5550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 5556 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 5562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 5568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 5574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 506 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5580 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5610 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 513 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5622 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5628 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5634 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5640 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 517 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 529 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 540 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 541 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 555 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 561 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5758 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5776 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 588 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 597 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5808 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5814 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5820 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[-2].pch));   }
#line 5826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5838 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5844 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5850 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5862 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5868 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5874 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5880 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5886 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5892 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5898 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; redefiningReservedWordError((yyval.pch)); }
#line 5904 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "false"; redefiningReservedWordError((yyval.pch)); }
#line 5910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "true"; redefiningReservedWordError((yyval.pch)); }
#line 5916 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); redefiningReservedTypeError((yyvsp[0].pch)); }
#line 5922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5928 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "this"; }
#line 5934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 671 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "bool"; }
#line 5940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "int"; }
#line 5946 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 673 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "uint"; }
#line 5952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "real"; }
#line 5958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 675 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "imag"; }
#line 5964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "complex"; }
#line 5970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 677 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "string"; }
#line 5976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 90:
#line 678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "sync"; }
#line 5982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 679 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "single"; }
#line 5988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 680 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "owned"; }
#line 5994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 681 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "shared"; }
#line 6000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "borrowed"; }
#line 6006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "unmanaged"; }
#line 6012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "domain"; }
#line 6018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 685 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "index"; }
#line 6024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 686 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "locale"; }
#line 6030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtBools[BOOL_SIZE_DEFAULT]->symbol); }
#line 6036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtInt[INT_SIZE_DEFAULT]->symbol); }
#line 6042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtUInt[INT_SIZE_DEFAULT]->symbol); }
#line 6048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 693 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtReal[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtImag[FLOAT_SIZE_DEFAULT]->symbol); }
#line 6060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtComplex[COMPLEX_SIZE_DEFAULT]->symbol); }
#line 6066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtString->symbol); }
#line 6072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(dtLocale->symbol); }
#line 6078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 704 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_syncvar"; }
#line 6084 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_singlevar"; }
#line 6090 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_owned"; }
#line 6096 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_shared"; }
#line 6102 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_borrowed"; }
#line 6108 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_unmanaged"; }
#line 6114 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 710 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_domain"; }
#line 6120 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "_index"; }
#line 6126 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 6132 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6138 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 721 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6144 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6150 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 6156 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 727 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 6162 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 6168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6174 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 734 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 6180 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 6186 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 748 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 6192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 6198 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 6204 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 755 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[0].pch)));
    }
#line 6212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 761 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 6218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 6236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 6242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 6260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 6266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6278 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6284 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 6290 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 6296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 6302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 6308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 778 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 784 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6328 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 790 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 796 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 6348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 802 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 809 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 6370 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 6376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6382 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 823 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6388 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 824 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6394 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 825 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6400 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 826 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 828 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 830 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6421 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 832 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 837 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 6434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 840 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 6440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 841 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 6446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 842 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 6452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 843 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 6458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 844 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 845 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 6470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 849 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 850 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 854 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 6488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 855 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 6494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 6500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 860 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 6506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 6512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 6518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 6524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 871 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 6530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 6536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 876 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 6542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 6548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 6560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 892 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6574 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 902 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 912 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 6602 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 922 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 6616 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 6646 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6652 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 6658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 6664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 960 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 6670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 6676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 967 "chapel.ypp" /* yacc.c:1661  */
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
#line 6696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 983 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 6704 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 990 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 6714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 999 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 6726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 6734 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1011 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 6742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1017 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 6748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1018 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 6754 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1023 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 6763 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1028 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1033 "chapel.ypp" /* yacc.c:1661  */
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
#line 6792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1053 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1059 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXPORT, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6826 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1072 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = buildLinkageFn(FLAG_EXTERN, (yyvsp[0].pexpr));

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_OVERRIDE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
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
#line 6897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1128 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1132 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6914 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1137 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6922 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1141 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6931 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1146 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), "dummy", INTENT_BLANK, NULL);
    }
#line 6939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1153 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1157 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 6951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1158 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1159 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 7005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 7011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1168 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 7017 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 7023 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 7029 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1171 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 7035 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1172 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 7041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 7047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 7053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 7059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 7065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 7071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 7077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 7083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1180 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 7089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 7095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1182 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 7101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1186 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 7107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 7113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1188 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 7119 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 7125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1190 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 7131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1191 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 7137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1192 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 7143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 7149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 7155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1195 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 7161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 7167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1197 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 7173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1201 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 7179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1202 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1206 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 7191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 7197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1211 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 7203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1212 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 7209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1217 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 7215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1219 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildPragmaDefExpr((yyvsp[-4].vpch), buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL)); }
#line 7221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 7227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1223 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 7239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1230 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 7251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1234 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 7257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1235 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 7263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1236 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 7269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1237 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 7275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1238 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 7281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1239 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 7287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1240 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1241 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 7299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 7305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1246 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 7311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1247 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 7317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1248 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 7323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 7329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1250 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 7335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1251 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 7341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1255 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 7347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1256 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 7353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1261 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 7365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 7371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1263 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 7377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 7383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1265 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 7389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1269 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 7395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1270 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 7401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1273 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 7407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1279 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 7413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1284 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1286 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1290 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 7431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1301 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 7461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1308 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 7467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 7473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1315 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 7490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1328 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 7508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1344 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1346 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1348 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 7526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1361 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7550 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1369 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7562 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1377 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 7575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1386 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 7586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7592 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1396 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7598 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1397 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 7604 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1403 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 7613 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1411 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 7619 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1413 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 7625 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1418 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 7631 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1420 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 7637 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 7643 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 7649 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 7655 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 7661 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7667 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 7673 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7679 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1445 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7685 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1447 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, NULL); }
#line 7691 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1449 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1453 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)), NULL);
    }
#line 7707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7721 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1463 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7729 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 352:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7741 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 353:
#line 1472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7747 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7753 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7759 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 356:
#line 1479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7765 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7771 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 7777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7783 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7789 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1504 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 7797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 7805 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1512 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 7817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 7825 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7831 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1533 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1535 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1541 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 7861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 7867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1545 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 7873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 7879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1550 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1551 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1559 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1560 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1561 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1562 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1567 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 384:
#line 1571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 385:
#line 1572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 386:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 387:
#line 1577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 388:
#line 1578 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 389:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1588 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1593 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 8005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 8017 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8023 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 8029 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 8035 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 8041 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 8047 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 8053 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 8059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1629 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 8065 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_BORROWED_CLASS, (yyvsp[0].pexpr)); }
#line 8071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[0].pexpr)); }
#line 8077 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( PRIM_TO_UNMANAGED_CLASS, (yyvsp[0].pexpr)); }
#line 8083 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[0].pexpr)); }
#line 8089 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8095 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8101 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8107 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8113 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8119 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8125 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1654 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8131 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 8137 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8143 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1662 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 8155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1664 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1666 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 8172 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1673 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 8182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1679 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 8192 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1685 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8202 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1691 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pcallexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 8212 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new IfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8218 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1709 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 8224 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8230 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1729 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 8236 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1734 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8242 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1738 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8248 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1739 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8254 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1743 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 8260 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1747 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1748 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 8272 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1753 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildForPrefix((yyvsp[-3].pShadowVarPref), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pexpr));
    }
#line 8280 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1757 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1761 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 8296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST;     }
#line 8302 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_IN;        }
#line 8308 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_IN;  }
#line 8314 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_REF;       }
#line 8320 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_CONST_REF; }
#line 8326 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pShadowVarPref) = SVP_VAR;       }
#line 8332 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1778 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8338 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1783 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 8344 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1788 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 8350 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1804 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 8356 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1806 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_borrowed", (yyvsp[-2].pexpr)); }
#line 8368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_to_unmanaged", (yyvsp[-2].pexpr)); }
#line 8374 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8380 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1814 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 8386 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1816 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 8392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1818 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 8398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1822 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 8404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1823 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1826 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 8416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1827 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 8422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1828 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 8428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1850 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1851 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 8440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1852 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 8446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1856 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 8452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1857 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 8458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1858 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 8464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 496:
#line 1859 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "locale"); }
#line 8470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 497:
#line 1867 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 8476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 498:
#line 1868 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 8482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 499:
#line 1869 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 8488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 500:
#line 1870 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 8494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 501:
#line 1874 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gFalse); }
#line 8500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 502:
#line 1875 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gTrue); }
#line 8506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 504:
#line 1880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch), yyfilename, chplLineno);    }
#line 8512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 505:
#line 1881 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 8518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 506:
#line 1882 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 8524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 507:
#line 1883 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 8530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 508:
#line 1884 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 8536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 509:
#line 1885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 8542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 510:
#line 1886 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 8548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 511:
#line 1887 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 8554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 512:
#line 1888 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 8560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 513:
#line 1890 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 8568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 514:
#line 1894 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 8576 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 515:
#line 1901 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8582 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 516:
#line 1902 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 8588 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 517:
#line 1906 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 518:
#line 1907 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 519:
#line 1908 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 520:
#line 1909 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 521:
#line 1910 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 522:
#line 1911 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 523:
#line 1912 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 524:
#line 1913 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 525:
#line 1914 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 526:
#line 1915 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 527:
#line 1916 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 528:
#line 1917 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 529:
#line 1918 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 530:
#line 1919 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 531:
#line 1920 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 532:
#line 1921 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 533:
#line 1922 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 534:
#line 1923 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 535:
#line 1924 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 536:
#line 1925 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 537:
#line 1926 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 538:
#line 1927 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 539:
#line 1928 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 8726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 540:
#line 1932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 8732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 541:
#line 1933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 8738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 542:
#line 1934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 8744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 543:
#line 1935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 8750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 544:
#line 1936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 8756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 545:
#line 1937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 8762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 546:
#line 1941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 547:
#line 1942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 548:
#line 1943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 549:
#line 1944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 550:
#line 1948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 551:
#line 1949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 552:
#line 1950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 8804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 553:
#line 1951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pcallexpr), true); }
#line 8810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 554:
#line 1956 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 8816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 555:
#line 1957 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 8822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 556:
#line 1958 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 8828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 557:
#line 1959 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 8834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 558:
#line 1960 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 8840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 559:
#line 1961 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 8846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 560:
#line 1962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 8852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 8856 "bison-chapel.cpp" /* yacc.c:1661  */
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
