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
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif
#line 130 "chapel.ypp" /* yacc.c:355  */

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
#line 152 "chapel.ypp" /* yacc.c:355  */

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

#line 222 "bison-chapel.cpp" /* yacc.c:355  */

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
    TBY = 270,
    TCATCH = 271,
    TCLASS = 272,
    TCOBEGIN = 273,
    TCOFORALL = 274,
    TCONFIG = 275,
    TCONST = 276,
    TCONTINUE = 277,
    TDEFER = 278,
    TDELETE = 279,
    TDMAPPED = 280,
    TDO = 281,
    TDOMAIN = 282,
    TELSE = 283,
    TENUM = 284,
    TEXCEPT = 285,
    TEXPORT = 286,
    TEXTERN = 287,
    TFOR = 288,
    TFORALL = 289,
    TFORWARDING = 290,
    TIF = 291,
    TIN = 292,
    TINDEX = 293,
    TINLINE = 294,
    TINOUT = 295,
    TITER = 296,
    TLABEL = 297,
    TLAMBDA = 298,
    TLET = 299,
    TLOCAL = 300,
    TMINUSMINUS = 301,
    TMODULE = 302,
    TNEW = 303,
    TNIL = 304,
    TNOINIT = 305,
    TON = 306,
    TONLY = 307,
    TOTHERWISE = 308,
    TOUT = 309,
    TPARAM = 310,
    TPLUSPLUS = 311,
    TPRAGMA = 312,
    TPRIMITIVE = 313,
    TPRIVATE = 314,
    TPROC = 315,
    TPROTOTYPE = 316,
    TPUBLIC = 317,
    TRECORD = 318,
    TREDUCE = 319,
    TREF = 320,
    TREQUIRE = 321,
    TRETURN = 322,
    TSCAN = 323,
    TSELECT = 324,
    TSERIAL = 325,
    TSINGLE = 326,
    TSPARSE = 327,
    TSUBDOMAIN = 328,
    TSYNC = 329,
    TTHEN = 330,
    TTHROW = 331,
    TTHROWS = 332,
    TTRY = 333,
    TTRYBANG = 334,
    TTYPE = 335,
    TUNDERSCORE = 336,
    TUNION = 337,
    TUSE = 338,
    TVAR = 339,
    TWHEN = 340,
    TWHERE = 341,
    TWHILE = 342,
    TWITH = 343,
    TYIELD = 344,
    TZIP = 345,
    TALIAS = 346,
    TAND = 347,
    TASSIGN = 348,
    TASSIGNBAND = 349,
    TASSIGNBOR = 350,
    TASSIGNBXOR = 351,
    TASSIGNDIVIDE = 352,
    TASSIGNEXP = 353,
    TASSIGNLAND = 354,
    TASSIGNLOR = 355,
    TASSIGNMINUS = 356,
    TASSIGNMOD = 357,
    TASSIGNMULTIPLY = 358,
    TASSIGNPLUS = 359,
    TASSIGNSL = 360,
    TASSIGNSR = 361,
    TBAND = 362,
    TBNOT = 363,
    TBOR = 364,
    TBXOR = 365,
    TCOLON = 366,
    TCOMMA = 367,
    TDIVIDE = 368,
    TDOT = 369,
    TDOTDOT = 370,
    TDOTDOTDOT = 371,
    TEQUAL = 372,
    TEXP = 373,
    TGREATER = 374,
    TGREATEREQUAL = 375,
    THASH = 376,
    TLESS = 377,
    TLESSEQUAL = 378,
    TMINUS = 379,
    TMOD = 380,
    TNOT = 381,
    TNOTEQUAL = 382,
    TOR = 383,
    TPLUS = 384,
    TQUESTION = 385,
    TSEMI = 386,
    TSHIFTLEFT = 387,
    TSHIFTRIGHT = 388,
    TSTAR = 389,
    TSWAP = 390,
    TASSIGNREDUCE = 391,
    TIO = 392,
    TLCBR = 393,
    TRCBR = 394,
    TLP = 395,
    TRP = 396,
    TLSBR = 397,
    TRSBR = 398,
    TNOELSE = 399,
    TUPLUS = 400,
    TUMINUS = 401
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
#line 183 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 414 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 420 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  #include <string>
  int         captureTokens;
  std::string captureString;
#line 191 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
  #include "CatchStmt.h"
  #include "DeferStmt.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForallStmt.h"
  #include "ForLoop.h"
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

#line 493 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   13609

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  147
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  124
/* YYNRULES -- Number of rules.  */
#define YYNRULES  495
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  938

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   401

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
     145,   146
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   441,   441,   446,   447,   453,   454,   459,   460,   465,
     466,   467,   468,   469,   470,   471,   472,   473,   474,   475,
     476,   477,   478,   479,   480,   481,   482,   483,   484,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   498,
     501,   503,   509,   510,   511,   515,   516,   529,   530,   531,
     536,   537,   542,   547,   552,   556,   563,   568,   572,   577,
     581,   582,   583,   587,   591,   593,   595,   597,   599,   601,
     603,   605,   607,   609,   611,   613,   615,   617,   619,   621,
     626,   627,   631,   635,   636,   640,   641,   645,   646,   650,
     651,   652,   653,   654,   655,   656,   657,   661,   662,   666,
     667,   668,   669,   673,   688,   689,   690,   691,   692,   693,
     694,   695,   696,   697,   698,   699,   700,   701,   702,   703,
     704,   710,   716,   722,   728,   735,   745,   749,   750,   751,
     752,   753,   755,   757,   759,   764,   767,   768,   769,   770,
     771,   772,   776,   777,   781,   782,   783,   787,   788,   792,
     795,   797,   802,   803,   807,   809,   811,   818,   828,   838,
     848,   861,   866,   871,   879,   880,   885,   886,   888,   893,
     909,   916,   925,   933,   937,   944,   945,   950,   955,   949,
     980,   986,   993,  1001,  1013,  1019,  1012,  1047,  1051,  1056,
    1060,  1068,  1069,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,
    1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1102,
    1103,  1104,  1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,
    1113,  1117,  1118,  1122,  1126,  1127,  1128,  1132,  1134,  1136,
    1138,  1143,  1144,  1148,  1149,  1150,  1151,  1152,  1153,  1154,
    1155,  1156,  1160,  1161,  1162,  1163,  1164,  1165,  1169,  1170,
    1174,  1175,  1176,  1177,  1178,  1179,  1183,  1184,  1187,  1188,
    1192,  1193,  1197,  1199,  1204,  1205,  1209,  1210,  1214,  1215,
    1219,  1221,  1223,  1228,  1241,  1258,  1259,  1261,  1266,  1274,
    1282,  1290,  1299,  1309,  1310,  1311,  1315,  1316,  1324,  1326,
    1331,  1333,  1335,  1340,  1342,  1344,  1351,  1352,  1353,  1357,
    1358,  1359,  1360,  1380,  1384,  1388,  1396,  1403,  1404,  1405,
    1409,  1411,  1417,  1419,  1421,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1438,  1439,  1440,  1441,  1445,  1446,  1450,  1451,
    1452,  1456,  1457,  1461,  1462,  1466,  1467,  1471,  1472,  1473,
    1474,  1478,  1489,  1490,  1491,  1492,  1493,  1494,  1496,  1498,
    1500,  1502,  1504,  1506,  1511,  1513,  1515,  1517,  1519,  1521,
    1523,  1525,  1527,  1529,  1531,  1533,  1535,  1542,  1548,  1554,
    1560,  1569,  1579,  1587,  1588,  1589,  1590,  1591,  1592,  1593,
    1594,  1599,  1600,  1604,  1608,  1611,  1616,  1620,  1623,  1628,
    1631,  1634,  1640,  1641,  1646,  1651,  1659,  1660,  1661,  1662,
    1663,  1664,  1665,  1666,  1667,  1669,  1671,  1673,  1675,  1677,
    1682,  1683,  1684,  1687,  1688,  1689,  1690,  1701,  1702,  1706,
    1707,  1708,  1712,  1713,  1714,  1722,  1723,  1724,  1725,  1729,
    1730,  1731,  1732,  1733,  1734,  1735,  1736,  1737,  1738,  1742,
    1750,  1751,  1755,  1756,  1757,  1758,  1759,  1760,  1761,  1762,
    1763,  1764,  1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,
    1773,  1774,  1775,  1776,  1777,  1781,  1782,  1783,  1784,  1785,
    1786,  1790,  1791,  1792,  1793,  1797,  1798,  1799,  1800,  1805,
    1806,  1807,  1808,  1809,  1810,  1811
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCATCH", "TCLASS",
  "TCOBEGIN", "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE", "TDEFER",
  "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT",
  "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIN",
  "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET",
  "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON",
  "TONLY", "TOTHERWISE", "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC", "TRECORD",
  "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHROW",
  "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE",
  "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS",
  "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL", "TMINUS",
  "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO",
  "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUPLUS",
  "TUMINUS", "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt",
  "pragma_ls", "stmt", "module_decl_stmt", "access_control",
  "opt_prototype", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt", "opt_ident",
  "ident", "opt_ident_or_string", "do_stmt", "return_stmt",
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
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "forall_intent_clause", "forall_intent_ls",
  "intent_expr", "io_expr", "new_expr", "let_expr", "expr", "opt_try_expr",
  "lhs_expr", "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
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
     395,   396,   397,   398,   399,   400,   401
};
# endif

#define YYPACT_NINF -775

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-775)))

#define YYTABLE_NINF -447

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -775,   100,  2520,  -775,   -37,  -775,  -775,  -775,  -775,  -775,
    -775,  3705,    75,   167,  -775,    75,  8048,   108,   167,  3705,
    8048,  3705,    37,  -775,   297,   923,  6648,  8048,  6788,  8048,
      78,  -775,   167,  -775,    61,  7208,  8048,  8048,  -775,  8048,
    8048,   236,   148,  1191,  1234,  -775,  7348,  6228,  8048,  7208,
    8048,   224,   163,  3705,  8048,  8188,  8188,   167,  -775,  7348,
    8048,  8048,  -775,  -775,  8048,  -775,  -775,  9728,  8048,  8048,
    -775,  8048,  -775,  -775,  2865,  5808,  7348,  -775,  3565,  -775,
    -775,   301,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,   167,
    -775,   167,   256,    45,  -775,  -775,  -775,    49,   239,  -775,
    -775,  -775,   244,   248,   175,   249,   261, 13404,  2079,   290,
     276,   279,  -775,  -775,  -775,  -775,  -775,  -775,    48,  -775,
    -775, 13404,   227,  3705,  -775,   326,  -775,   314,  8048,  8048,
    8048,  8048,  8048,  7348,  7348,   306,  -775,  -775,  -775,  -775,
   11672,   313,  -775,  -775,   167,   328,  -775,   124, 13404,   357,
    6368,  -775,  -775,  -775,  -775,   167,   149,   167,   323,    47,
   11137, 11066,  -775,  -775,  -775, 11610, 10456,  6368,  3705,   334,
      29,   293,    13,  -775,  3705,  -775,  -775, 11270,   613,   355,
   11270,   613,  -775,  6368,   172,  -775,  -775,   167,  -775,   126,
   13404,  8048,  8048,  -775, 13404,   346, 11465,  -775, 11270, 13404,
     340,  6368,  -775, 13404, 11941,  -775,  -775, 12012,  2372,  -775,
    -775, 12074,   389,   353,   214, 11746, 11270, 12145,   365,  1337,
     613,   365,   613,   -36,  -775,  -775,  3005,   107,  -775,  8048,
    -775,   -21,   110,  -775,    98, 12207,    97,   478,  -775,  -775,
     440,   375,   351,  -775,  -775,  -775,    52,    61,    61,    61,
    -775,  8048,  8048,  8048,  8048,  7488,  7488,  8048,  8048,  8048,
    8048,  8048,  8048,    94,  9728,  8048,  8048,  8048,  8048,  8048,
    8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,
    6928,  6928,  6928,  6928,  6928,  6928,  6928,  6928,  6928,  6928,
    6928,  6928,  6928,  6928,  6928,  6928,  6368,  6368,  7488,  7488,
    5668,  -775,  -775,  3145,  -775, 11808, 11879, 12278,    38,  7488,
      47,   360,  -775,  8048,  -775,  8048,   400,  -775,   364,   384,
    -775,  -775,   366,   167,   469,  7348,  -775,  3845,  7488,  -775,
    3985,  7488,   367,  -775,    47,  8328,  8048,  -775,  3705,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,   481,  8048,   369,  -775,   311,  -775,  -775,    29,  -775,
     407,   380,  -775,  8468,   430,  8048,    61,  -775,  -775,  -775,
     386,  -775,  -775,  7348,  -775, 13404, 13404,  -775,    28,  -775,
    6368,   387,  -775,   515,  -775,   515,  -775,  8468,   413,  -775,
    -775,  8328,  8048,  -775,  -775,  -775,  -775,  -775,  7068,  -775,
   10335,  5948,  -775,  6088,  -775,  7488,  5245,  2671,   391,  8048,
    5527,  -775,  -775,   167,  7348,   394,   463,   329,    61,   222,
     225,   230,   251, 11536,  2215,  2215,   324,  -775,   324,  -775,
     324,  5216,  1515,   997,   486,   355,   365,  -775,  -775,  -775,
    1337,  5357,   324,   456,   456,  2215,   456,   456,  1484,   365,
    5357,  5076,  1484,   613,   613,   365,   404,   406,   409,   410,
     412,   414,   417,   418,   420,   422,   424,   425,   426,   427,
     428,   429,   403,   419,  -775,   324,  -775,   324,   255,  -775,
    -775,  -775,  -775,  -775,  -775,   167,   113,  -775, 13475,   321,
    8608,  7488,  8748,  7488,  8048,  7488,  2389,    75, 12340,  -775,
    -775, 13404, 12411,  6368,  -775,  6368,  -775,   375,  8048,   117,
    8048, 13404,    59, 11332,  8048, 13404,    35, 11199,  5668,  -775,
     442,   465,   447, 12473,   465,   451,   555, 12544,  3705, 11403,
    -775,   130,  -775,    68,  -775,   356,   443,    29,   293,  5105,
    -775,   475,  6508,  -775, 13404,  -775,  -775,  -775, 13404,   452,
      26,   457,  -775,    53,  -775,  -775,   252,   167,   461,   470,
    -775,  -775,  -775,  -775,  -775,  -775,    21, 10143,  -775,  -775,
   13404,  3705, 13404,  -775, 12606,   467,   491,    67,   471,   516,
     190,  -775,   543,  -775,  -775,  -775,  -775,  9973,   257,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  5668,  -775,  7488,  7488,  8048,   586,
   12677,  8048,   588, 12739,   472, 10181,    47,    47,  -775,  -775,
    -775,  -775,   489, 11270,  -775, 10518,  4125,  -775,  4265,  -775,
   10593,  4405,  -775,    47,  4545,  -775,    47,   177,  -775,  8048,
    -775,  8048,  -775,  3705,  8048,  -775,  3705,   600,   611,  -775,
      29,   505,   565,  -775,  -775,  -775,    92,  -775,  -775,   430,
     490,    46,  -775,  -775,  -775,  4685,  7348,  -775,  -775,  -775,
     167,  -775,   524,   314,  -775,  -775,  -775,  4825,   494,  4965,
     496,  -775,  8048,  3285,   502,   833,  -775,  8048,   167,  -775,
    -775,   356,  -775,  -775,  -775,   167,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -775,  8048,   529,   536,   514,
     514,  -775,  -775,  -775,   181,   186, 12810,  8888,  9028, 12872,
    9168,  9308,  9448,  9588,  -775,  -775,  1328,  -775,  3705,  8048,
   13404,  8048, 13404,  3705,  8048, 13404,  -775,  8048, 13404,  -775,
    5668,  -775, 12943, 13404,  -775, 13404,   628,  3705,  -775,   517,
    7628,   140,  -775,   574,  -775,  -775,  7488,  9923,  3705,  -775,
      56,   522,  8048,  -775,  8048,  -775, 13404,  3705,  8048,  -775,
   13404,  3705, 13404,   170,  -775,  3425,  -775,   995,   185,   185,
    -775,   408,  -775, 13404,  -775,    76,  -775, 10373, 10022,   311,
    -775,  -775,  8048,  8048,  8048,  8048,  8048,  8048,  8048,  8048,
     528,  1124, 12544, 10655, 10730, 12544, 10792, 10867,  -775,  8048,
    3705,  -775,   505,    37,  8048,  8048,  5386,  -775,  -775,   262,
    7348,  -775,  -775,  8048,    22, 10219,  -775,   509,   357,  -775,
     314, 13404, 10929,  -775, 11004,  -775,  -775,  -775,  -775,   591,
    -775,  -775,   514,   514,   187, 13005, 13076, 13138, 13209, 13271,
   13342,  -775,  -775,  3705,  3705,  3705,  3705, 13404,  -775,   140,
    7768,   109,  -775,  -775, 13404, 13404,  -775,  -775,  -775,  9588,
    -775,  3705,  3705,  -775,   574,  -775,  -775,  -775,  8048,  8048,
    8048,  8048,  8048,  8048, 12544, 12544, 12544, 12544,  -775,  -775,
    -775,  -775,  -775,   304,  7488,  9807,   721, 12544, 12544,    23,
   10297,  -775,  -775,  -775,  -775,  -775,  7908,  -775
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   439,   440,   441,   442,
     443,     0,   391,    80,   161,   391,     0,   294,    80,     0,
       0,     0,     0,   171,    80,    80,     0,     0,   293,     0,
       0,   181,     0,   177,     0,     0,     0,     0,   382,     0,
       0,     0,     0,   293,   293,   162,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   163,     0,
       0,     0,   491,   493,     0,   494,   495,   419,     0,     0,
     492,   489,    89,   490,     0,     0,     0,     4,     0,     5,
       9,    45,    10,    11,    12,    14,   351,    22,    13,    90,
      96,    15,    17,    16,    19,    20,    21,    18,    95,     0,
      93,     0,   411,     0,    97,    94,    98,     0,   423,   407,
     408,   354,   352,     0,     0,   412,   413,     0,   353,     0,
     424,   425,   426,   406,   356,   355,   409,   410,     0,    38,
      24,   362,     0,     0,   392,     0,    81,     0,     0,     0,
       0,     0,     0,     0,     0,   411,   423,   352,   412,   413,
     391,   353,   424,   425,     0,     0,   135,     0,   336,     0,
     343,    84,    83,   182,   103,     0,   183,     0,     0,     0,
       0,     0,   294,   295,   102,     0,     0,   343,     0,     0,
       0,     0,     0,   296,     0,    86,    32,     0,   477,   404,
       0,   478,     7,   343,   295,    92,    91,   273,   333,     0,
     332,     0,     0,    87,   422,     0,     0,    35,     0,   357,
       0,   343,    36,   363,     0,   142,   138,     0,   353,   142,
     139,     0,   285,     0,     0,   332,     0,     0,   480,   418,
     476,   479,   475,     0,    47,    50,     0,     0,   338,     0,
     340,     0,     0,   339,     0,   332,     0,     0,     6,    46,
       0,   164,     0,   259,   258,   184,     0,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   417,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   343,     0,     0,
       0,    25,    26,     0,    27,     0,     0,     0,     0,     0,
       0,     0,    28,     0,    29,     0,   351,   349,     0,   344,
     345,   350,     0,     0,     0,     0,   112,     0,     0,   111,
       0,     0,     0,   118,     0,     0,    56,    99,     0,   219,
     226,   227,   228,   223,   225,   221,   224,   222,   220,   230,
     229,   128,     0,     0,    30,   234,   178,   300,     0,   301,
     303,     0,   312,     0,   306,     0,     0,    85,    31,    33,
       0,   183,   272,     0,    63,   420,   421,    88,     0,    34,
     343,     0,   149,   140,   136,   141,   137,     0,   283,   280,
      60,     0,    56,   105,    37,    49,    48,    51,     0,   444,
       0,     0,   435,     0,   437,     0,     0,     0,     0,     0,
       0,   448,     8,     0,     0,     0,     0,   252,     0,     0,
       0,     0,     0,   390,   472,   471,   474,   482,   481,   486,
     485,   468,   465,   466,   467,   415,   455,   434,   433,   432,
     416,   459,   470,   464,   462,   473,   463,   461,   453,   458,
     460,   469,   452,   456,   457,   454,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   484,   483,   488,   487,   246,   243,
     244,   245,   249,   250,   251,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,   446,   391,   391,   108,
     281,   337,     0,     0,   359,     0,   282,   164,     0,     0,
       0,   366,     0,     0,     0,   372,     0,     0,     0,   119,
     490,    59,     0,    52,    57,     0,   127,     0,     0,     0,
     358,     0,   235,     0,   242,   260,     0,   304,     0,     0,
     311,   407,     0,   298,   405,   297,   431,   335,   334,     0,
       0,     0,   360,     0,   143,   287,   407,     0,     0,     0,
     445,   414,   436,   341,   438,   342,     0,     0,   447,   124,
     376,     0,   450,   449,     0,     0,   165,     0,     0,   175,
       0,   172,   256,   253,   254,   257,   185,     0,     0,   289,
     288,   290,   292,    64,    71,    72,    73,    68,    70,    78,
      79,    66,    69,    67,    65,    75,    74,    76,    77,   429,
     430,   247,   248,   399,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,   347,
     348,   346,     0,     0,   126,     0,     0,   110,     0,   109,
       0,     0,   116,     0,     0,   114,     0,     0,   397,     0,
     100,     0,   101,     0,     0,   130,     0,   132,   241,   233,
       0,   325,   261,   264,   263,   265,     0,   302,   305,   306,
       0,     0,   307,   308,   151,     0,     0,   150,   153,   361,
       0,   144,   147,     0,   284,    61,    62,     0,     0,     0,
       0,   125,     0,     0,     0,   293,   170,     0,   173,   169,
     255,   260,   216,   214,   195,   198,   196,   197,   208,   199,
     212,   204,   202,   215,   203,   201,   206,   211,   213,   200,
     205,   209,   210,   207,   217,   218,     0,   193,     0,   231,
     231,   191,   291,   395,   423,   423,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   106,     0,   113,     0,     0,
     365,     0,   364,     0,     0,   371,   117,     0,   370,   115,
       0,   396,    54,    53,   129,   381,   131,     0,   236,     0,
       0,   306,   262,   278,   299,   316,     0,   446,     0,   155,
       0,     0,     0,   145,     0,   122,   378,     0,     0,   120,
     377,     0,   451,     0,    39,     0,   159,    80,   293,   293,
     157,   293,   167,   176,   174,     0,   194,     0,     0,   234,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   369,     0,     0,   375,     0,     0,   398,     0,
       0,   134,   325,   328,   329,   330,     0,   327,   331,   407,
     274,   238,   237,     0,     0,     0,   314,   407,   156,   154,
       0,   148,     0,   123,     0,   121,    41,    40,   168,   266,
     192,   193,   231,   231,     0,     0,     0,     0,     0,     0,
       0,   160,   158,     0,     0,     0,     0,    55,   133,   306,
     317,     0,   275,   277,   276,   279,   270,   271,   179,     0,
     146,     0,     0,   267,   278,   189,   190,   232,     0,     0,
       0,     0,     0,     0,   368,   367,   374,   373,   240,   239,
     319,   320,   322,   407,     0,   446,   407,   380,   379,     0,
       0,   321,   323,   268,   186,   269,   317,   324
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -775,  -775,  -775,    -1,  -660,  1705,  -775,  -775,  -775,  1444,
     -34,  -254,   269,   271,  -775,  -775,   194,   226,  2058,    -4,
    1551,  -774,  -627,   -39,  -775,  -775,  -775,    93,  -775,  -775,
    -775,   454,  -775,   -12,  -775,  -775,  -775,  -775,  -775,   508,
     162,   -76,  -775,  -775,  -775,   -26,   634,  -775,  -775,  -775,
    -775,  -775,  -775,  -775,  -775,  -133,  -591,  -677,  -775,  -131,
      18,  -775,  -358,  -775,  -775,   -22,  -775,  -775,  -239,   -59,
    -775,  -197,  -211,  -775,  -144,  -775,   666,  -775,  -217,   320,
    -775,  -345,  -666,  -520,  -394,  -753,  -710,  -143,   -16,  -775,
      62,  -775,  -162,  -775,   183,   294,  -361,  -775,  -775,   922,
    -775,     7,  -775,  -775,  -236,  -775,  -517,  -775,   990,  1150,
      -2,   734,   439,  -775,  1218,  1363,  -775,  -775,  -775,  -775,
    -775,  -775,  -775,  -308
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   235,    78,   536,    80,    81,   250,    82,
     236,   531,   535,   532,    83,    84,    85,   162,    86,   166,
     186,    87,    88,    89,    90,    91,    92,   634,    93,    94,
      95,   393,   564,   693,    96,    97,   560,   688,    98,    99,
     425,   705,   100,   101,   590,   591,   145,   179,   545,   103,
     104,   427,   711,   596,   738,   739,   362,   820,   366,   541,
     542,   543,   495,   597,   255,   676,   904,   934,   898,   198,
     893,   851,   854,   105,   223,   398,   106,   107,   182,   183,
     370,   371,   553,   374,   550,   921,   848,   781,   237,   157,
     241,   242,   328,   329,   330,   146,   109,   110,   111,   147,
     113,   133,   134,   496,   344,   657,   497,   114,   148,   149,
     204,   331,   151,   119,   152,   153,   122,   123,   246,   124,
     125,   126,   127,   128
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     117,    77,   499,   565,   195,   196,   740,   544,   418,   131,
     321,   658,   551,   784,   150,   363,   240,   117,   158,   117,
     163,   332,   137,   546,   170,   171,   175,   176,   679,   559,
     199,   380,     5,   187,   188,   189,   566,   190,   191,   429,
     430,   431,   432,   224,   200,   811,   206,   208,   209,   391,
     375,   213,   214,   217,   221,     5,     5,   225,   226,   227,
     244,   651,   228,   821,     5,   229,   230,   231,   704,   232,
     256,     5,   200,   184,   245,   505,   117,   372,   812,   685,
     897,  -152,   184,   786,  -166,   646,   253,  -166,  -166,    47,
      47,   411,   534,   372,   129,   129,  -166,     5,  -166,  -166,
       3,   327,  -166,   405,   257,   254,  -166,   743,  -166,   342,
     367,   686,   308,  -152,   258,   852,   309,   428,   327,   169,
     412,   447,  -166,   342,  -166,   376,  -166,  -166,   318,  -166,
    -166,   117,  -166,   259,   327,   415,   131,   315,   316,   317,
     213,   200,   245,  -309,   482,   483,   924,  -166,   534,  -166,
     416,  -166,   327,  -309,   933,   897,   783,   320,   416,  -309,
     313,   313,  -309,   132,   697,   687,    14,  -152,   383,   368,
       5,   811,   931,   313,   448,     5,   117,   160,  -309,   161,
     922,   506,   117,   937,   868,   313,   342,   373,   154,   787,
     381,   313,   180,   690,   313,   905,   906,   313,  -166,   385,
     386,   180,   678,   373,   812,   172,  -166,  -309,   670,   420,
     416,   598,    45,   -80,  -309,   932,    24,   194,   177,   408,
     499,   416,   413,   919,    31,   624,  -180,   873,   561,   383,
    -309,    58,   -80,   552,   117,   407,   323,   410,   383,   135,
     421,   417,   668,   192,   155,  -180,   409,   327,   327,   216,
     220,   414,   925,   838,   625,   324,   850,   384,   644,   433,
     434,   435,   436,   438,   440,   441,   442,   443,   444,   445,
     446,   669,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   193,   770,
     653,   656,   621,  -400,   372,   869,   108,   210,  -401,   668,
       5,   129,   708,   211,   161,   108,   485,   487,   498,   866,
     544,   117,   261,   108,  -241,   108,   499,   508,   771,   519,
     622,   511,  -400,   512,   557,   779,   383,  -401,   907,   709,
    -309,   327,   488,   200,   376,   521,   523,   376,   525,   527,
     698,   700,   376,   533,   533,   400,   537,   108,   489,   557,
     592,   490,   240,   599,   240,  -326,   600,   557,   437,   439,
     539,   601,   249,   376,  -286,   491,   492,   310,   108,   376,
    -384,   117,   108,   554,  -326,  -383,   493,   672,  -326,   260,
    -388,   558,   602,  -286,   593,   627,  -309,  -387,   742,   309,
     264,   494,  -389,   856,   594,   117,  -428,  -318,  -428,   533,
     533,   484,   486,  -326,   373,  -309,   558,  -386,   586,   595,
    -385,   673,   507,   577,   558,   580,  -318,   582,   584,   849,
    -318,   674,   200,   694,  -309,    14,   857,   108,    17,   265,
     306,   522,   307,   266,   526,   271,   675,    23,   273,    24,
     807,   118,   276,    28,   325,  -318,  -428,    31,  -428,  -180,
     118,  -241,   313,  -427,   639,  -427,   327,   312,   118,   322,
     118,   544,   499,   335,   588,   247,     5,   808,  -180,   273,
     809,    45,   108,   573,   365,   575,   271,   387,   108,   273,
     390,   264,   397,   276,   399,   422,   424,   423,    57,   426,
      58,   510,   118,   513,   218,   218,   515,   516,   521,   630,
     525,   633,   537,   635,   580,   514,   518,   528,   576,   538,
     540,   264,  -310,   118,   636,   637,   643,   118,   645,   547,
     265,   548,   650,   552,   266,   567,   498,   556,   562,   923,
     108,   563,   587,   681,   581,   603,   117,   604,   926,    72,
     605,   606,  -310,   607,   619,   608,  -313,   245,   609,   610,
     265,   611,  -310,   612,   266,   613,   614,   615,   616,   617,
     618,  -310,   620,   268,   923,   269,   270,   271,  -310,   272,
     273,   274,   118,   -58,   276,   923,  -313,   659,   660,   117,
     282,   283,   662,   663,   677,   286,  -313,  -310,   287,   288,
     289,   684,   695,   268,   629,  -313,   632,   271,   689,   272,
     273,   696,  -313,   383,   276,   703,  -310,   108,   710,   707,
     706,   283,   747,  -310,   750,   752,   780,   118,   287,   288,
     289,  -313,   498,   118,   438,   485,   746,   756,   777,   749,
     782,   108,   488,   785,   108,   792,   102,   797,   264,   801,
    -313,   806,   108,  -351,   760,   102,   762,  -313,   489,   765,
     818,   490,   768,   102,   819,   102,   840,   772,   842,   773,
     853,   117,   775,   860,   117,   491,   492,   881,   903,   805,
     790,   569,   568,   395,   333,   118,   493,   265,   791,   642,
     831,   266,   814,   117,   200,   872,   778,   102,   874,   815,
     935,   494,   918,   929,   174,   796,   555,   800,   641,   889,
     802,   117,     0,     0,     0,   813,     0,     0,   102,     0,
       0,   108,   102,     0,     0,     0,     0,     0,     0,   437,
     484,   847,     0,     0,   271,     0,   272,   273,     0,     0,
       0,   276,     0,     0,   817,     0,     0,     0,   283,     0,
       0,     0,     0,     0,     0,   760,   762,   289,   765,   768,
     796,   800,   118,     0,     0,     0,   832,   833,  -315,   834,
       0,   835,   836,     0,     0,   837,     0,   102,   498,   195,
     196,     0,     0,     0,     0,   117,   118,     0,   117,   118,
       0,   205,     0,     0,   855,   580,   117,   118,  -315,   623,
     861,   892,   862,     0,     0,   117,   864,     0,  -315,   117,
       0,     0,     0,   117,   407,     0,     0,  -315,     0,   243,
       0,     0,   102,     0,  -315,     0,     0,     0,   102,     0,
     832,   875,   876,   835,   877,   878,   879,   880,     0,     0,
     891,   920,   108,  -315,     0,     0,     0,   887,   117,     0,
       0,     0,   209,   213,   245,     0,     0,     0,   894,     0,
      14,   895,  -315,    17,     0,     0,   118,     0,     0,  -315,
       0,     0,    23,     0,    24,   807,   920,     0,    28,     0,
     102,     0,    31,     0,  -180,   108,     0,   920,     0,     0,
       0,   914,   915,   916,   917,     0,     0,     0,   117,     0,
      41,   741,   808,  -180,     0,   809,    45,   800,     0,   927,
     928,     0,     0,     0,     0,     0,   914,   915,   916,   917,
     927,   928,     0,    57,     0,    58,     0,     0,     0,     0,
     744,   745,   930,   580,   112,     0,     5,     0,     0,     0,
     161,     0,   164,   112,   800,     0,     0,     0,     0,     0,
     108,   112,   108,   112,  -295,   108,     0,   102,   108,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
     108,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,   102,   810,     0,   102,   112,     0,   118,  -295,   108,
       0,     0,   102,     0,     0,     0,     0,     0,  -295,     0,
       0,   108,   115,   108,     0,     0,   112,   108,     5,     0,
     112,   115,   161,   165,     0,     0,     0,  -295,     0,   115,
       0,   115,     0,     0,     0,     0,  -295,     0,     0,     0,
     118,     0,   264,     0,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
       0,     0,     0,   115,     0,     0,     0,     0,     0,     0,
    -295,   102,   108,     0,     0,   112,     0,   108,     0,     0,
    -295,   265,     0,     0,   115,   266,     0,     0,   115,     0,
       0,   108,     0,     0,     0,   165,     0,     0,     0,  -295,
       0,     0,   108,     0,     0,   118,     0,   118,     0,     0,
     118,   108,     0,   118,     0,   108,     0,     0,     0,   108,
     112,     0,   118,     0,   268,   118,   112,   270,   271,     0,
     272,   273,     0,     0,     0,   276,     0,     0,     0,     0,
       0,     0,   283,   115,   118,     0,     0,     0,     0,   287,
     288,   289,     0,     0,   108,     0,   118,     0,   118,     0,
       0,    14,   118,     0,    17,   243,     0,   243,     0,     0,
       0,     0,   116,    23,     0,    24,   807,     0,   112,    28,
       0,   116,     0,    31,     0,  -180,     0,     0,   115,   116,
       0,   116,   102,     0,   115,     0,     0,   108,   108,   108,
     108,    41,     0,   808,  -180,     0,   809,    45,     0,     0,
       0,     0,     0,     0,     0,   108,   108,   118,     0,     0,
       0,     0,   118,   116,    57,     0,    58,     0,     0,     0,
       0,   172,     0,     0,     0,   102,   118,     0,     0,     0,
     120,     0,    24,   194,   116,     0,   115,   118,   116,   120,
      31,     0,  -180,     0,     0,   112,   118,   120,   -44,   120,
     118,     0,     0,     0,   118,     0,     0,   640,     0,     0,
       0,  -180,   -44,     0,   172,    72,     0,     0,     0,   112,
       0,     0,   112,   882,     0,    24,   194,     0,     0,     0,
     112,   120,     0,    31,     0,  -180,     0,     0,     0,   118,
     102,   -43,   102,   116,     0,   102,   683,     0,   102,     0,
       0,     0,   120,     0,  -180,   -43,   120,   102,     0,     0,
     102,     0,     0,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   118,   118,   118,   118,     0,   115,   116,   830,
     115,   102,     0,   102,   116,     0,     0,   102,   115,   112,
     118,   118,     0,     0,     0,  -166,     0,     0,  -166,  -166,
       0,   120,     0,     0,     0,     0,     0,  -166,     0,  -166,
    -166,     0,   264,  -166,     0,   121,     0,  -166,     0,  -166,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,   121,  -166,   121,  -166,   116,  -166,  -166,     0,
    -166,  -166,   102,  -166,     0,     0,   120,   102,     0,     0,
       0,   265,   120,     0,     0,   266,     0,   115,  -166,     0,
    -166,   102,  -166,     0,     0,     0,   121,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,     0,   102,     0,   121,     0,   102,
       0,   121,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,     0,     0,   120,   276,     0,     0,     0,  -166,
     112,   282,   283,   116,     0,     0,   286,  -166,     0,   287,
     288,   289,     0,     0,   102,     0,     0,     0,     0,   185,
       0,     0,     0,     0,     0,     0,     0,   116,     0,     0,
     116,     0,     0,   185,     0,     0,   121,     0,   116,   215,
     219,     0,     0,   112,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,   102,   102,   102,
     102,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,   120,     0,     0,     0,   102,   102,     0,     0,     0,
     264,   121,     0,     0,     0,     0,     0,   121,   265,     0,
       0,     0,   266,     0,     0,   120,     0,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   120,   116,   112,     0,
     112,   115,     0,   112,     0,     0,   112,     0,     0,   265,
       0,   314,     0,   266,     0,   112,     0,     0,   112,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   121,
     207,     0,   276,     0,     0,     0,     0,   112,     0,   283,
       0,     0,     0,   185,   185,   185,   287,   288,   289,   112,
     361,   112,     0,     0,     0,   112,   271,     0,   272,   273,
       0,   185,     0,   276,   185,   120,   115,     0,   115,     0,
     283,   115,     0,     0,   115,     0,     0,   287,   288,   289,
       0,     0,   185,   115,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,     0,     0,     0,     0,   115,   121,     0,     0,     0,
     112,     0,     0,     0,     0,   112,     0,   115,   116,   115,
       0,     0,     0,   115,     0,     0,     0,     0,     0,   112,
     121,     0,     0,   121,     0,     0,     0,    79,     0,     0,
     112,   121,     0,     0,     0,     0,   130,     0,     0,   112,
     336,   339,   343,   112,   156,     0,   159,   112,     0,     0,
       0,   116,     0,     0,     0,     0,     0,     0,   378,     0,
       0,   379,     0,     0,     0,     0,     0,     0,   115,     0,
       0,     0,     0,   115,     0,     0,   120,     0,   212,   389,
       0,     0,   112,     0,   185,     0,     0,   115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   115,    79,
     121,     0,     0,   248,     0,     0,     0,   115,   185,     0,
       0,   115,     0,     0,     0,   115,   116,     0,   116,   120,
       0,   116,     0,     0,   116,   112,   112,   112,   112,     0,
       0,     0,     0,   116,     0,     0,   116,     0,     0,     0,
       0,     0,     0,   112,   112,     0,     0,     0,     0,     0,
     115,     0,     0,     0,     0,   116,     0,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,   116,     0,   116,
       0,     0,     0,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,     0,   120,     0,     0,   120,
       0,   509,   120,   115,   115,   115,   115,     0,     0,     0,
       0,   120,     0,   364,   120,     0,     0,     0,     0,   377,
       0,   115,   115,     0,     0,   529,     0,     0,     0,     0,
       0,   121,     0,   120,     0,     0,     0,     0,   116,     0,
       0,     0,     0,   116,     0,   120,     0,   120,     0,     0,
       0,   120,     0,     0,     0,     0,     0,   116,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,     0,
       0,    79,     0,     0,   121,     0,     0,   116,     0,     0,
       0,   116,     0,     0,     0,   116,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   185,   185,     0,     0,
     185,   185,     0,     0,     0,     0,   120,     0,     0,     0,
       0,   120,     0,   667,     0,     0,     0,     0,     0,     0,
     116,     0,     0,     0,     0,   120,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,   691,     0,   121,
       0,   121,     0,     0,   121,   120,     0,   121,    79,   120,
       0,     0,     0,   120,     0,     0,   121,     0,     0,   121,
       0,     0,     0,   116,   116,   116,   116,     0,     0,     0,
       0,     0,   377,     0,     0,   377,     0,     0,   121,     0,
       0,   116,   116,     0,     0,     0,     0,     0,   120,     0,
     121,     0,   121,     0,     0,     0,   121,     0,     0,     0,
       0,   136,     0,   647,   649,     0,   136,   652,   655,     0,
     185,   185,   136,   136,     0,     0,     0,   185,     0,   361,
     178,     0,   181,     0,   361,     0,     0,   185,     0,     0,
     185,   120,   120,   120,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   222,     0,     0,     0,   120,
     120,   121,   579,     0,     0,     0,   121,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   793,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,     0,     0,     0,   251,     0,   252,
     121,     0,     0,     0,   121,     0,     0,     0,   121,     0,
       0,     0,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,     0,   754,   755,     0,
       0,     0,     0,     0,   757,     0,     0,     0,     0,     0,
       0,     0,     0,   121,   766,     0,     0,   769,     0,     0,
       0,     0,   222,     0,   304,   305,  -402,     0,   326,  -427,
       0,  -427,     0,   222,     0,   334,     0,     0,     0,     0,
       0,     0,     0,     0,   185,   326,     0,     0,   369,     0,
     264,     0,     0,   665,     0,     0,   121,   121,   121,   121,
       0,   326,   136,     0,     0,   382,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   121,     0,     0,     0,   326,
       0,     0,     0,     0,     0,     0,     0,   361,   361,   265,
     361,   361,     0,   266,     0,     0,   701,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   896,     0,
       0,     0,     0,     0,   900,     0,   361,   267,   361,     0,
       0,     0,     0,     0,   181,   181,   181,   181,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,   449,   275,   276,   277,   278,     0,   280,   281,   282,
     283,   859,   284,   285,   286,     0,     0,   287,   288,   289,
       0,   377,     0,   377,     0,     0,   377,     0,     0,   377,
       0,     0,     0,     0,   326,   326,     0,     0,   774,     0,
       0,   776,     0,   896,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     789,   517,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   138,   795,     0,   799,     0,     0,     0,    79,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   139,   140,     0,   141,   369,    30,     0,     0,
       0,     0,    33,    34,   181,    36,     0,    37,    38,     0,
       0,     0,     0,     0,     0,    40,     0,    42,   326,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    52,   142,     0,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,     0,
       0,   585,   841,     0,   589,     0,   181,     0,     0,     0,
       0,     0,     0,   858,     0,     0,     0,    64,     0,     0,
       0,     0,   863,     0,     0,     0,   865,   304,   305,     0,
      79,     0,  -427,     0,  -427,    69,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,    75,
       0,   144,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,   888,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,   326,     0,  -293,    40,    41,    42,    43,
    -180,   -42,    44,    45,     0,  -293,    46,    47,     0,    48,
      49,    50,    51,    52,    53,     0,    54,     0,    55,    56,
      57,   671,    58,    59,  -293,   369,     0,    60,     0,    61,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,   692,     0,     0,     0,   222,     0,    63,    64,    65,
      66,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    69,     0,    70,    71,
       0,    72,     0,     0,    73,   737,     0,     0,    74,     0,
      75,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     0,     5,     6,     7,     8,     9,    10,
       0,  -446,     0,    11,    12,    13,  -446,     0,    14,    15,
      16,    17,  -293,    18,    19,    20,  -446,    21,    22,  -446,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -180,    32,    33,    34,    35,    36,   -42,    37,
      38,     0,    39,     0,     0,     0,  -293,    40,   369,    42,
      43,  -180,   -42,    44,    45,  -446,  -293,    46,    47,  -446,
      48,    49,    50,    51,    52,    53,     0,    54,   692,    55,
      56,    57,     0,    58,    59,  -293,     0,     0,    60,     0,
      61,     0,     0,  -446,     0,     0,   589,     0,     0,     0,
       0,     0,     0,   816,     0,     0,     0,     0,  -446,    64,
    -446,  -446,  -446,  -446,  -446,  -446,  -446,     0,  -446,  -446,
    -446,  -446,  -446,  -446,  -446,  -446,  -446,    69,  -446,  -446,
    -446,     0,    72,  -446,  -446,  -446,     0,     0,     0,    74,
    -446,    75,     0,    76,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,   233,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   871,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   197,    72,     0,     0,    73,
       0,     0,     0,    74,   234,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   406,    75,   233,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   234,    75,   803,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   804,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,    41,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,   867,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,   247,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   520,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   524,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   759,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   761,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   764,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   767,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,   788,    22,     0,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   794,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,     0,     0,    74,     0,    75,     4,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   798,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
    -293,    40,     0,    42,    43,  -180,   -42,    44,    45,     0,
    -293,    46,    47,     0,    48,    49,    50,    51,    52,    53,
       0,    54,     0,    55,    56,    57,     0,    58,    59,  -293,
       0,     0,    60,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,     0,    72,     0,     0,    73,
       0,   264,     0,    74,     0,    75,   680,    76,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   139,   140,
     265,   141,     0,    30,   266,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   142,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,    62,   280,   281,
     282,   283,     0,   284,     0,   286,     0,     0,   287,   288,
     289,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   197,     0,     0,     0,    73,
       0,   264,     0,   143,     0,    75,     0,   144,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   139,   140,
     265,   141,     0,    30,   266,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
       0,    40,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    52,   142,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,    62,   280,   281,
     282,   283,     0,   284,     0,   286,     0,     0,   287,   288,
     289,     0,    63,    64,    65,    66,     0,     0,     0,     0,
      67,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    69,     0,    70,    71,   197,     0,     0,     0,    73,
       0,     0,   264,   143,     0,    75,     0,   144,   578,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   139,
     140,   265,   141,     0,    30,   266,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,     0,    40,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    52,
     142,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,     0,   276,   277,   278,    62,   280,
     281,   282,   283,     0,     0,     0,   286,     0,     0,   287,
     288,   289,     0,    63,    64,    65,    66,     0,     0,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    69,     0,    70,    71,   197,     0,     0,     0,
      73,     0,     0,     0,   143,     0,    75,     0,   144,   890,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     139,   140,     0,   141,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,     0,    40,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    63,    64,    65,    66,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    69,     0,    70,    71,     0,     0,     0,
       0,    73,     0,     0,     0,   143,     0,    75,     0,   144,
     583,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,   488,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,   489,    30,     0,   490,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,   491,   492,    40,     0,    42,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,   239,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,   572,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,   574,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,   203,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,   682,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,   167,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,   172,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
     173,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,   201,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,   570,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   843,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   844,
      51,    52,   845,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     846,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     846,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   829,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,   197,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,    74,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,   530,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     549,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   628,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   631,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   823,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   824,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   826,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   827,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   828,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   829,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    69,     0,    70,    71,     0,     0,
       0,     0,    73,     0,     0,     0,   143,     0,    75,     0,
     144,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   139,   140,     0,   141,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,     0,    40,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    52,   142,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,    63,    64,    65,    66,     0,
     139,   140,     0,   141,     0,    30,     0,     0,     0,     0,
      33,    34,    68,    36,    69,    37,    38,    71,     0,     0,
       0,     0,    73,    40,     0,    42,   143,     0,    75,     0,
     144,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      52,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -317,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,     0,     0,     0,  -317,
       0,     0,     0,  -317,     0,     0,     5,     6,     7,     8,
       9,    10,     0,    69,     0,   138,     0,   197,     0,     0,
       0,     0,     0,     0,     0,   143,     0,    75,  -317,   846,
      22,     0,     0,     0,     0,     0,   139,   140,     0,   141,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     5,     0,     0,    40,
       0,    42,     0,   712,     0,     0,     0,     0,   713,     0,
       0,     0,     0,     0,    50,    51,    52,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,     0,     0,     0,
       0,    64,   712,     0,     0,     0,     0,   713,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,    75,     0,   549,   349,   350,   351,   352,
     353,   354,     0,     0,   355,   356,   357,   358,   359,   360,
     714,   715,   716,   717,     0,     0,   718,     0,     0,     0,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,     0,   730,     0,     0,   731,   732,   733,   734,     0,
     735,     0,     0,   736,     0,   349,   350,   351,   352,   353,
     354,     0,     0,   355,   356,   357,   358,   359,   360,   714,
     715,   716,   717,     0,     0,   718,     0,     0,     0,   719,
     720,   721,   722,   723,   724,   725,   726,   727,   728,   729,
       0,   730,     0,   262,   731,   732,   733,   734,   263,   735,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,   265,     0,     0,
       0,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,   342,     0,     0,   263,   267,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   265,     0,     0,     0,   266,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,     0,
     284,   285,   286,   267,     0,   287,   288,   289,     0,     0,
       0,     0,     0,   265,     0,     0,   699,   266,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   262,   284,   285,
     286,   267,   263,   287,   288,   289,     0,     0,     0,     0,
       0,     0,   264,     0,   753,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   262,   284,   285,   286,     0,
     263,   287,   288,   289,     0,     0,     0,     0,     0,     0,
     264,   265,   899,     0,     0,   266,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   263,   267,
       0,     0,     0,     0,     0,     0,     0,     0,   264,   265,
       0,     0,     0,   266,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,     0,   284,   285,   286,   267,     0,   287,
     288,   289,     0,     0,     0,     0,     0,   265,     0,     0,
     936,   266,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,   267,   262,   287,   288,   289,
       0,   263,     0,     0,     0,     0,   571,     0,     0,     0,
     268,   264,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,     0,
     284,   285,   286,     0,     0,   287,   288,   289,     0,     0,
       0,     0,     0,     0,   870,     0,     0,     0,     0,     0,
     265,     0,     0,     0,   266,     0,     0,     0,   262,     0,
       0,   348,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,   267,   349,
     350,   351,   352,   353,   354,     0,     0,   355,   356,   357,
     358,   359,   360,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   265,   284,   285,   286,   266,     0,   287,   288,
     289,     0,     0,   758,   313,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
     267,   349,   350,   351,   352,   353,   354,     0,   264,   355,
     356,   357,   358,   359,   360,   268,     0,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,     0,   284,   285,   286,     0,     0,
     287,   288,   289,     0,     0,     0,   313,   265,     0,     0,
       0,   266,     0,     0,     0,   262,     0,     0,   763,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   267,   349,   350,   351,   352,
     353,   354,     0,     0,   355,   356,   357,   358,   359,   360,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   265,
     284,   285,   286,   266,     0,   287,   288,   289,     0,     0,
     883,   313,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,   267,   349,   350,
     351,   352,   353,   354,     0,   264,   355,   356,   357,   358,
     359,   360,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289,
       0,     0,     0,   313,   265,     0,     0,     0,   266,     0,
       0,     0,   262,     0,     0,   884,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,   267,   349,   350,   351,   352,   353,   354,     0,
       0,   355,   356,   357,   358,   359,   360,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,     0,   287,   288,   289,     0,     0,   885,   313,     0,
       0,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,   267,   349,   350,   351,   352,   353,
     354,     0,   264,   355,   356,   357,   358,   359,   360,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,   284,
     285,   286,     0,     0,   287,   288,   289,     0,     0,     0,
     313,   265,     0,     0,     0,   266,     0,     0,     0,   262,
       0,     0,   886,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,     0,     0,     0,     0,   267,
     349,   350,   351,   352,   353,   354,     0,     0,   355,   356,
     357,   358,   359,   360,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   265,   284,   285,   286,   266,     0,   287,
     288,   289,     0,     0,   901,   313,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,   267,   349,   350,   351,   352,   353,   354,     0,   264,
     355,   356,   357,   358,   359,   360,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,   284,   285,   286,     0,
       0,   287,   288,   289,     0,     0,     0,   313,   265,     0,
       0,     0,   266,     0,     0,     0,   262,     0,     0,   902,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,   340,     0,     0,     0,   267,   349,   350,   351,
     352,   353,   354,   341,     0,   355,   356,   357,   358,   359,
     360,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     265,   284,   285,   286,   266,     0,   287,   288,   289,     0,
       0,     0,   313,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,   342,     0,     0,     0,   267,     0,
       0,     0,   264,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,   338,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,     0,   284,   285,   286,     0,     0,   287,   288,
     289,   265,     0,     0,   313,   266,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   654,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   265,   284,   285,   286,   266,     0,   287,
     288,   289,     0,     0,     0,   313,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,   342,     0,     0,
       0,   267,     0,     0,     0,   264,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,   284,   285,   286,     0,
       0,   287,   288,   289,   265,     0,     0,   313,   266,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,   648,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,     0,   287,   288,   289,     0,     0,     0,   313,     0,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   267,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,   284,
     285,   286,     0,     0,   287,   288,   289,   265,     0,     0,
     313,   266,     0,     0,     0,   262,     0,     0,   666,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   265,
     284,   285,   286,   266,     0,   287,   288,   289,     0,     0,
       0,   313,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   267,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289,
     265,     0,     0,   388,   266,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,     0,   267,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
     345,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   346,   284,   285,   286,     0,     0,   287,   288,
     289,     0,     0,  -403,   265,     0,     0,     0,   266,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,   347,   287,   288,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
     132,   263,     0,     0,   267,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,   401,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   402,   284,
     285,   286,     0,     0,   287,   288,   289,     0,     0,     0,
     265,     0,     0,     0,   266,     0,     0,     0,   262,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,   500,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,   501,     0,     0,     0,     0,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   265,   284,   285,   286,   266,     0,   287,   288,
     289,     0,     0,     0,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
     267,     0,     0,     0,   264,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   503,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,     0,   284,   285,   286,     0,     0,
     287,   288,   289,   265,     0,     0,     0,   266,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   265,   284,   285,   286,   266,
       0,   287,   288,   289,     0,     0,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,   267,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,     0,   284,   285,
     286,     0,   392,   287,   288,   289,   265,     0,     0,     0,
     266,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   265,   284,
     285,   286,   266,   394,   287,   288,   289,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   267,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
       0,   284,   285,   286,     0,   396,   287,   288,   289,   265,
       0,     0,     0,   266,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   264,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   265,   284,   285,   286,   266,   404,   287,   288,   289,
       0,     0,     0,     0,     0,     0,     0,     0,   262,     0,
       0,     0,     0,   263,     0,     0,     0,     0,   419,   267,
       0,     0,     0,   264,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,     0,   284,   285,   286,     0,     0,   287,
     288,   289,   265,     0,     0,     0,   266,     0,     0,     0,
     262,     0,     0,   504,     0,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   264,     0,     0,     0,     0,
     267,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   265,   284,   285,   286,   266,     0,
     287,   288,   289,     0,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,   132,     0,
       0,     0,   267,     0,     0,     0,   264,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,     0,   284,   285,   286,
       0,     0,   287,   288,   289,   265,     0,     0,     0,   266,
       0,     0,     0,   262,   661,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   264,     0,
       0,     0,     0,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   265,   284,   285,
     286,   266,   638,   287,   288,   289,     0,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,   267,     0,     0,     0,   264,
       0,     0,   664,     0,     0,     0,     0,     0,     0,     0,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,     0,
     284,   285,   286,     0,     0,   287,   288,   289,   265,     0,
       0,     0,   266,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,     0,     0,   267,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     265,   284,   285,   286,   266,     0,   287,   288,   289,     0,
       0,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,   702,   267,     0,
       0,     0,   264,   748,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,     0,   284,   285,   286,     0,     0,   287,   288,
     289,   265,     0,     0,     0,   266,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   264,   751,     0,     0,     0,   267,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,     0,   269,   270,   271,     0,
     272,   273,   274,     0,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   265,   284,   285,   286,   266,     0,   287,
     288,   289,     0,     0,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,     0,     0,     0,
       0,   267,     0,     0,     0,   264,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,     0,   284,   285,   286,     0,
       0,   287,   288,   289,   265,     0,     0,     0,   266,     0,
       0,     0,   262,     0,     0,   822,     0,   263,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   264,     0,     0,
       0,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   268,     0,   269,
     270,   271,     0,   272,   273,   274,     0,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   265,   284,   285,   286,
     266,     0,   287,   288,   289,     0,     0,   825,     0,     0,
       0,     0,     0,   262,   839,     0,     0,     0,   263,     0,
       0,     0,     0,     0,   267,     0,     0,     0,   264,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,     0,   284,
     285,   286,     0,     0,   287,   288,   289,   265,     0,     0,
       0,   266,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     264,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     268,     0,   269,   270,   271,     0,   272,   273,   274,     0,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   265,
     284,   285,   286,   266,     0,   287,   288,   289,     0,     0,
     908,     0,     0,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,   267,     0,     0,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289,
     265,     0,     0,     0,   266,     0,     0,     0,   262,     0,
       0,   909,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   264,     0,     0,     0,     0,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   268,     0,   269,   270,   271,     0,   272,
     273,   274,     0,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   265,   284,   285,   286,   266,     0,   287,   288,
     289,     0,     0,   910,     0,     0,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,     0,
     267,     0,     0,     0,   264,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,   269,   270,   271,
       0,   272,   273,   274,     0,   275,   276,   277,   278,   279,
     280,   281,   282,   283,     0,   284,   285,   286,     0,     0,
     287,   288,   289,   265,     0,     0,     0,   266,     0,     0,
       0,   262,     0,     0,   911,     0,   263,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   264,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,     0,   269,   270,
     271,     0,   272,   273,   274,     0,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   265,   284,   285,   286,   266,
       0,   287,   288,   289,     0,     0,   912,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,     0,   267,     0,     0,     0,   264,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   268,     0,
     269,   270,   271,     0,   272,   273,   274,     0,   275,   276,
     277,   278,   279,   280,   281,   282,   283,     0,   284,   285,
     286,     0,     0,   287,   288,   289,   265,     0,     0,     0,
     266,     0,     0,     0,   262,     0,     0,   913,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,   267,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   269,   270,   271,     0,   272,   273,   274,     0,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   265,   284,
     285,   286,   266,     0,   287,   288,   289,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,   267,     0,     0,     0,
     264,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   269,   270,   271,     0,   272,   273,   274,
       0,   275,   276,   277,   278,   279,   280,   281,   282,   283,
       0,   284,   285,   286,     0,     0,   287,   288,   289,   626,
       0,     0,     0,   266,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,   269,   270,   271,     0,   272,   273,
     274,     0,   275,   276,   277,   278,   279,   280,   281,   282,
     283,     0,   284,   285,   286,     0,     0,   287,   288,   289
};

static const yytype_int16 yycheck[] =
{
       2,     2,   310,   397,    43,    44,   597,   365,   244,    11,
     154,   528,   373,   679,    16,   177,    75,    19,    20,    21,
      24,   165,    15,   368,    26,    27,    28,    29,   548,     1,
      46,   193,     3,    35,    36,    37,   397,    39,    40,   256,
     257,   258,   259,    59,    46,   705,    48,    49,    50,   211,
      37,    53,    54,    55,    56,     3,     3,    59,    60,    61,
      76,    26,    64,   740,     3,    67,    68,    69,     1,    71,
      21,     3,    74,    26,    76,    37,    78,     1,   705,    53,
     854,    53,    26,    37,    17,    26,    41,    20,    21,    67,
      67,   112,   346,     1,   131,   131,    29,     3,    31,    32,
       0,   160,    35,   139,    55,    60,    39,   624,    41,    88,
      81,    85,    64,    85,    65,   781,    68,    65,   177,    26,
     141,    27,    55,    88,    57,   112,    59,    60,   144,    62,
      63,   133,    65,    84,   193,    37,   138,   139,   140,   141,
     142,   143,   144,    67,   306,   307,    37,    80,   402,    82,
     112,    84,   211,    77,   131,   929,   676,   150,   112,    67,
     138,   138,    86,    88,   143,   139,    17,   139,   112,   140,
       3,   831,   925,   138,    80,     3,   178,   140,    86,     7,
     890,   143,   184,   936,   811,   138,    88,   111,    80,   143,
     194,   138,   140,   140,   138,   872,   873,   138,   131,   201,
     202,   140,   547,   111,   831,    20,   139,   131,   140,   112,
     112,   428,    63,    41,   138,   925,    31,    32,   140,   112,
     528,   112,   112,   889,    39,   112,    41,   818,   390,   112,
     138,    82,    60,    93,   236,   236,   112,   239,   112,    13,
     143,   143,   112,     7,    18,    60,   139,   306,   307,    55,
      56,   141,   143,   770,   141,   131,   116,   131,   141,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   141,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   140,   112,
     526,   527,    37,   112,     1,   815,     2,    73,   112,   112,
       3,   131,   112,   140,     7,    11,   308,   309,   310,   139,
     668,   313,   137,    19,     3,    21,   624,   319,   141,   335,
      65,   323,   141,   325,   383,   670,   112,   141,   141,   139,
      37,   390,    21,   335,   112,   337,   338,   112,   340,   341,
     576,   577,   112,   345,   346,   131,   348,    53,    37,   408,
      21,    40,   411,   131,   413,    93,   131,   416,   265,   266,
     362,   131,    61,   112,   112,    54,    55,   140,    74,   112,
     131,   373,    78,   375,   112,   131,    65,    21,   116,   131,
     131,   383,   131,   131,    55,    64,    93,   131,   131,    68,
      25,    80,   131,   787,    65,   397,   140,    93,   142,   401,
     402,   308,   309,   141,   111,   112,   408,   131,   424,    80,
     131,    55,   319,   415,   416,   417,   112,   419,   420,   780,
     116,    65,   424,   567,   131,    17,   787,   133,    20,    64,
     140,   338,   142,    68,   341,   111,    80,    29,   114,    31,
      32,     2,   118,    35,    87,   141,   140,    39,   142,    41,
      11,   140,   138,   140,   513,   142,   515,   131,    19,   131,
      21,   819,   770,   140,     1,    57,     3,    59,    60,   114,
      62,    63,   178,   411,   140,   413,   111,   131,   184,   114,
     140,    25,    93,   118,   131,     7,   111,    47,    80,   138,
      82,   131,    53,    93,    55,    56,   112,   131,   500,   501,
     502,   503,   504,   505,   506,   141,    37,   140,   415,    28,
     141,    25,    37,    74,   507,   508,   518,    78,   520,   112,
      64,   141,   524,    93,    68,   112,   528,   141,   141,   890,
     236,    16,   138,   549,   143,   131,   538,   131,   899,   131,
     131,   131,    67,   131,   141,   131,    37,   549,   131,   131,
      64,   131,    77,   131,    68,   131,   131,   131,   131,   131,
     131,    86,   143,   107,   925,   109,   110,   111,    93,   113,
     114,   115,   133,   131,   118,   936,    67,   112,   131,   581,
     124,   125,   131,    28,   141,   129,    77,   112,   132,   133,
     134,   139,   131,   107,   501,    86,   503,   111,   141,   113,
     114,   131,    93,   112,   118,   138,   131,   313,    65,    93,
     139,   125,    26,   138,    26,   143,   111,   178,   132,   133,
     134,   112,   624,   184,   626,   627,   628,   138,    28,   631,
      65,   337,    21,   143,   340,   111,     2,   143,    25,   143,
     131,   139,   348,   114,   646,    11,   648,   138,    37,   651,
     114,    40,   654,    19,   140,    21,    28,   659,   141,   661,
      86,   663,   664,   141,   666,    54,    55,   139,    77,   703,
     686,   402,   401,   219,   166,   236,    65,    64,   690,   517,
     756,    68,   708,   685,   686,   818,   668,    53,   819,   711,
     929,    80,   889,   904,    28,   697,   376,   699,   515,   842,
     702,   703,    -1,    -1,    -1,   707,    -1,    -1,    74,    -1,
      -1,   417,    78,    -1,    -1,    -1,    -1,    -1,    -1,   626,
     627,   780,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,   118,    -1,    -1,   736,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,   747,   748,   134,   750,   751,
     752,   753,   313,    -1,    -1,    -1,   758,   759,    37,   761,
      -1,   763,   764,    -1,    -1,   767,    -1,   133,   770,   808,
     809,    -1,    -1,    -1,    -1,   777,   337,    -1,   780,   340,
      -1,    47,    -1,    -1,   786,   787,   788,   348,    67,   495,
     792,   850,   794,    -1,    -1,   797,   798,    -1,    77,   801,
      -1,    -1,    -1,   805,   805,    -1,    -1,    86,    -1,    75,
      -1,    -1,   178,    -1,    93,    -1,    -1,    -1,   184,    -1,
     822,   823,   824,   825,   826,   827,   828,   829,    -1,    -1,
     846,   890,   538,   112,    -1,    -1,    -1,   839,   840,    -1,
      -1,    -1,   844,   845,   846,    -1,    -1,    -1,   850,    -1,
      17,   853,   131,    20,    -1,    -1,   417,    -1,    -1,   138,
      -1,    -1,    29,    -1,    31,    32,   925,    -1,    35,    -1,
     236,    -1,    39,    -1,    41,   581,    -1,   936,    -1,    -1,
      -1,   883,   884,   885,   886,    -1,    -1,    -1,   890,    -1,
      57,   597,    59,    60,    -1,    62,    63,   899,    -1,   901,
     902,    -1,    -1,    -1,    -1,    -1,   908,   909,   910,   911,
     912,   913,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
     626,   627,   924,   925,     2,    -1,     3,    -1,    -1,    -1,
       7,    -1,     9,    11,   936,    -1,    -1,    -1,    -1,    -1,
     646,    19,   648,    21,    21,   651,    -1,   313,   654,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   663,    -1,    -1,
     666,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      -1,   337,   139,    -1,   340,    53,    -1,   538,    55,   685,
      -1,    -1,   348,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,   697,     2,   699,    -1,    -1,    74,   703,     3,    -1,
      78,    11,     7,    80,    -1,    -1,    -1,    84,    -1,    19,
      -1,    21,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
     581,    -1,    25,    -1,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      55,   417,   758,    -1,    -1,   133,    -1,   763,    -1,    -1,
      65,    64,    -1,    -1,    74,    68,    -1,    -1,    78,    -1,
      -1,   777,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      -1,    -1,   788,    -1,    -1,   646,    -1,   648,    -1,    -1,
     651,   797,    -1,   654,    -1,   801,    -1,    -1,    -1,   805,
     178,    -1,   663,    -1,   107,   666,   184,   110,   111,    -1,
     113,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   133,   685,    -1,    -1,    -1,    -1,   132,
     133,   134,    -1,    -1,   840,    -1,   697,    -1,   699,    -1,
      -1,    17,   703,    -1,    20,   411,    -1,   413,    -1,    -1,
      -1,    -1,     2,    29,    -1,    31,    32,    -1,   236,    35,
      -1,    11,    -1,    39,    -1,    41,    -1,    -1,   178,    19,
      -1,    21,   538,    -1,   184,    -1,    -1,   883,   884,   885,
     886,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   901,   902,   758,    -1,    -1,
      -1,    -1,   763,    53,    80,    -1,    82,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,   581,   777,    -1,    -1,    -1,
       2,    -1,    31,    32,    74,    -1,   236,   788,    78,    11,
      39,    -1,    41,    -1,    -1,   313,   797,    19,    47,    21,
     801,    -1,    -1,    -1,   805,    -1,    -1,   513,    -1,    -1,
      -1,    60,    61,    -1,    20,   131,    -1,    -1,    -1,   337,
      -1,    -1,   340,   139,    -1,    31,    32,    -1,    -1,    -1,
     348,    53,    -1,    39,    -1,    41,    -1,    -1,    -1,   840,
     646,    47,   648,   133,    -1,   651,   552,    -1,   654,    -1,
      -1,    -1,    74,    -1,    60,    61,    78,   663,    -1,    -1,
     666,    -1,    -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   685,
      -1,    -1,   883,   884,   885,   886,    -1,   337,   178,     1,
     340,   697,    -1,   699,   184,    -1,    -1,   703,   348,   417,
     901,   902,    -1,    -1,    -1,    17,    -1,    -1,    20,    21,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,
      32,    -1,    25,    35,    -1,     2,    -1,    39,    -1,    41,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    55,    21,    57,   236,    59,    60,    -1,
      62,    63,   758,    65,    -1,    -1,   178,   763,    -1,    -1,
      -1,    64,   184,    -1,    -1,    68,    -1,   417,    80,    -1,
      82,   777,    84,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   797,    -1,    -1,    -1,   801,    -1,    74,    -1,   805,
      -1,    78,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,    -1,    -1,   236,   118,    -1,    -1,    -1,   131,
     538,   124,   125,   313,    -1,    -1,   129,   139,    -1,   132,
     133,   134,    -1,    -1,   840,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,    -1,    -1,
     340,    -1,    -1,    49,    -1,    -1,   133,    -1,   348,    55,
      56,    -1,    -1,   581,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   883,   884,   885,
     886,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   538,    -1,
      -1,   313,    -1,    -1,    -1,   901,   902,    -1,    -1,    -1,
      25,   178,    -1,    -1,    -1,    -1,    -1,   184,    64,    -1,
      -1,    -1,    68,    -1,    -1,   337,    -1,    -1,   340,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   348,   417,   646,    -1,
     648,   581,    -1,   651,    -1,    -1,   654,    -1,    -1,    64,
      -1,   137,    -1,    68,    -1,   663,    -1,    -1,   666,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   236,
      49,    -1,   118,    -1,    -1,    -1,    -1,   685,    -1,   125,
      -1,    -1,    -1,   169,   170,   171,   132,   133,   134,   697,
     176,   699,    -1,    -1,    -1,   703,   111,    -1,   113,   114,
      -1,   187,    -1,   118,   190,   417,   646,    -1,   648,    -1,
     125,   651,    -1,    -1,   654,    -1,    -1,   132,   133,   134,
      -1,    -1,   208,   663,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,    -1,    -1,    -1,    -1,   685,   313,    -1,    -1,    -1,
     758,    -1,    -1,    -1,    -1,   763,    -1,   697,   538,   699,
      -1,    -1,    -1,   703,    -1,    -1,    -1,    -1,    -1,   777,
     337,    -1,    -1,   340,    -1,    -1,    -1,     2,    -1,    -1,
     788,   348,    -1,    -1,    -1,    -1,    11,    -1,    -1,   797,
     169,   170,   171,   801,    19,    -1,    21,   805,    -1,    -1,
      -1,   581,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,   758,    -1,
      -1,    -1,    -1,   763,    -1,    -1,   538,    -1,    53,   208,
      -1,    -1,   840,    -1,   320,    -1,    -1,   777,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   788,    74,
     417,    -1,    -1,    78,    -1,    -1,    -1,   797,   344,    -1,
      -1,   801,    -1,    -1,    -1,   805,   646,    -1,   648,   581,
      -1,   651,    -1,    -1,   654,   883,   884,   885,   886,    -1,
      -1,    -1,    -1,   663,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    -1,    -1,   901,   902,    -1,    -1,    -1,    -1,    -1,
     840,    -1,    -1,    -1,    -1,   685,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   697,    -1,   699,
      -1,    -1,    -1,   703,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   646,    -1,   648,    -1,    -1,   651,
      -1,   320,   654,   883,   884,   885,   886,    -1,    -1,    -1,
      -1,   663,    -1,   178,   666,    -1,    -1,    -1,    -1,   184,
      -1,   901,   902,    -1,    -1,   344,    -1,    -1,    -1,    -1,
      -1,   538,    -1,   685,    -1,    -1,    -1,    -1,   758,    -1,
      -1,    -1,    -1,   763,    -1,   697,    -1,   699,    -1,    -1,
      -1,   703,    -1,    -1,    -1,    -1,    -1,   777,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   788,    -1,
      -1,   236,    -1,    -1,   581,    -1,    -1,   797,    -1,    -1,
      -1,   801,    -1,    -1,    -1,   805,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   522,   523,    -1,    -1,
     526,   527,    -1,    -1,    -1,    -1,   758,    -1,    -1,    -1,
      -1,   763,    -1,   539,    -1,    -1,    -1,    -1,    -1,    -1,
     840,    -1,    -1,    -1,    -1,   777,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   788,   563,    -1,   646,
      -1,   648,    -1,    -1,   651,   797,    -1,   654,   313,   801,
      -1,    -1,    -1,   805,    -1,    -1,   663,    -1,    -1,   666,
      -1,    -1,    -1,   883,   884,   885,   886,    -1,    -1,    -1,
      -1,    -1,   337,    -1,    -1,   340,    -1,    -1,   685,    -1,
      -1,   901,   902,    -1,    -1,    -1,    -1,    -1,   840,    -1,
     697,    -1,   699,    -1,    -1,    -1,   703,    -1,    -1,    -1,
      -1,    13,    -1,   522,   523,    -1,    18,   526,   527,    -1,
     636,   637,    24,    25,    -1,    -1,    -1,   643,    -1,   645,
      32,    -1,    34,    -1,   650,    -1,    -1,   653,    -1,    -1,
     656,   883,   884,   885,   886,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,   901,
     902,   758,   417,    -1,    -1,    -1,   763,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   693,    -1,    -1,
     777,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   788,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     797,    -1,    -1,    -1,   801,    -1,    -1,    -1,   805,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,   636,   637,    -1,
      -1,    -1,    -1,    -1,   643,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   840,   653,    -1,    -1,   656,    -1,    -1,
      -1,    -1,   154,    -1,   135,   136,   137,    -1,   160,   140,
      -1,   142,    -1,   165,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   790,   177,    -1,    -1,   180,    -1,
      25,    -1,    -1,   538,    -1,    -1,   883,   884,   885,   886,
      -1,   193,   194,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   901,   902,    -1,    -1,    -1,   211,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   833,   834,    64,
     836,   837,    -1,    68,    -1,    -1,   581,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   854,    -1,
      -1,    -1,    -1,    -1,   860,    -1,   862,    92,   864,    -1,
      -1,    -1,    -1,    -1,   256,   257,   258,   259,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   273,   117,   118,   119,   120,    -1,   122,   123,   124,
     125,   790,   127,   128,   129,    -1,    -1,   132,   133,   134,
      -1,   646,    -1,   648,    -1,    -1,   651,    -1,    -1,   654,
      -1,    -1,    -1,    -1,   306,   307,    -1,    -1,   663,    -1,
      -1,   666,    -1,   929,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     685,   333,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,   697,    -1,   699,    -1,    -1,    -1,   703,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,   368,    38,    -1,    -1,
      -1,    -1,    43,    44,   376,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    58,   390,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    72,    73,    74,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,    -1,
      -1,   423,   777,    -1,   426,    -1,   428,    -1,    -1,    -1,
      -1,    -1,    -1,   788,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,   797,    -1,    -1,    -1,   801,   135,   136,    -1,
     805,    -1,   140,    -1,   142,   126,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,   140,
      -1,   142,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,   840,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,   515,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    -1,    76,    -1,    78,    79,
      80,   543,    82,    83,    84,   547,    -1,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   563,    -1,    -1,    -1,   567,    -1,   107,   108,   109,
     110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,   129,
      -1,   131,    -1,    -1,   134,   597,    -1,    -1,   138,    -1,
     140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    13,    14,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,   670,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,   690,    78,
      79,    80,    -1,    82,    83,    84,    -1,    -1,    87,    -1,
      89,    -1,    -1,    92,    -1,    -1,   708,    -1,    -1,    -1,
      -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,    -1,    -1,    -1,   138,
     139,   140,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   807,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,   818,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,   130,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,   139,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    -1,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    66,    67,    -1,    69,    70,    71,    72,    73,    74,
      -1,    76,    -1,    78,    79,    80,    -1,    82,    83,    84,
      -1,    -1,    87,    -1,    89,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,    -1,   131,    -1,    -1,   134,
      -1,    25,    -1,   138,    -1,   140,     1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      64,    36,    -1,    38,    68,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    92,   122,   123,
     124,   125,    -1,   127,    -1,   129,    -1,    -1,   132,   133,
     134,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,
      -1,    25,    -1,   138,    -1,   140,    -1,   142,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      64,    36,    -1,    38,    68,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    92,   122,   123,
     124,   125,    -1,   127,    -1,   129,    -1,    -1,   132,   133,
     134,    -1,   107,   108,   109,   110,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,   129,   130,    -1,    -1,    -1,   134,
      -1,    -1,    25,   138,    -1,   140,    -1,   142,   143,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    64,    36,    -1,    38,    68,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,    -1,   118,   119,   120,    92,   122,
     123,   124,   125,    -1,    -1,    -1,   129,    -1,    -1,   132,
     133,   134,    -1,   107,   108,   109,   110,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,   128,   129,   130,    -1,    -1,    -1,
     134,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,    -1,
      -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,
     143,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    37,    38,    -1,    40,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    54,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,   141,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,   131,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    50,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      32,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,   139,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,   108,   109,   110,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,   138,    -1,   140,    -1,
     142,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,   107,   108,   109,   110,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,   124,    46,   126,    48,    49,   129,    -1,    -1,
      -1,    -1,   134,    56,    -1,    58,   138,    -1,   140,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,   126,    -1,    12,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    -1,   140,   141,   142,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,     3,    -1,    -1,    56,
      -1,    58,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,   108,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,    -1,    -1,   140,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,    -1,    10,   132,   133,   134,   135,    15,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    88,    -1,    -1,    15,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    64,    -1,    -1,    -1,    68,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    92,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,   143,    68,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    10,   127,   128,
     129,    92,    15,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,   143,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    10,   127,   128,   129,    -1,
      15,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    64,   143,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    64,
      -1,    -1,    -1,    68,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    92,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
     143,    68,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    92,    10,   132,   133,   134,
      -1,    15,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,
     107,    25,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    75,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      92,    93,    94,    95,    96,    97,    98,    -1,    25,   101,
     102,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    -1,    -1,    -1,   138,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      75,   138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    92,    93,    94,
      95,    96,    97,    98,    -1,    25,   101,   102,   103,   104,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      -1,    -1,    -1,   138,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    75,   138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    92,    93,    94,    95,    96,    97,
      98,    -1,    25,   101,   102,   103,   104,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
     138,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,   101,   102,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    75,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    92,    93,    94,    95,    96,    97,    98,    -1,    25,
     101,   102,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    -1,    -1,    -1,   138,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    98,    37,    -1,   101,   102,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    -1,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    37,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,   138,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,   138,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    64,    -1,    -1,   138,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    -1,   138,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
     138,    68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,   138,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      30,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    52,   127,   128,   129,    -1,    -1,   132,   133,
     134,    -1,    -1,   137,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,   131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      88,    15,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    52,   127,
     128,   129,    -1,    -1,   132,   133,   134,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    37,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,   131,   132,   133,   134,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,   131,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,   131,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    64,   127,   128,   129,    68,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    91,    92,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,    -1,    -1,   132,
     133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    64,   127,   128,   129,    68,    -1,
     132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
      -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    64,   127,   128,
     129,    68,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,
      -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,    -1,   132,   133,   134,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      64,   127,   128,   129,    68,    -1,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    91,    92,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,    -1,    -1,   132,   133,
     134,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    64,   127,   128,   129,    68,    -1,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,    -1,
      -1,   132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    64,   127,   128,   129,
      68,    -1,   132,   133,   134,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    64,
     127,   128,   129,    68,    -1,   132,   133,   134,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    10,    -1,
      -1,    75,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    64,   127,   128,   129,    68,    -1,   132,   133,
     134,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,    -1,    -1,
     132,   133,   134,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    10,    -1,    -1,    75,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    64,   127,   128,   129,    68,
      -1,   132,   133,   134,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,    -1,    -1,   132,   133,   134,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    10,    -1,    -1,    75,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    64,   127,
     128,   129,    68,    -1,   132,   133,   134,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,    -1,    -1,   132,   133,   134,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,    -1,    -1,   132,   133,   134
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   148,   149,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      24,    26,    27,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    42,    43,    44,    45,    46,    48,    49,    51,
      56,    57,    58,    59,    62,    63,    66,    67,    69,    70,
      71,    72,    73,    74,    76,    78,    79,    80,    82,    83,
      87,    89,    92,   107,   108,   109,   110,   115,   124,   126,
     128,   129,   131,   134,   138,   140,   142,   150,   151,   152,
     153,   154,   156,   161,   162,   163,   165,   168,   169,   170,
     171,   172,   173,   175,   176,   177,   181,   182,   185,   186,
     189,   190,   193,   196,   197,   220,   223,   224,   242,   243,
     244,   245,   246,   247,   254,   255,   256,   257,   259,   260,
     261,   262,   263,   264,   266,   267,   268,   269,   270,   131,
     152,   257,    88,   248,   249,   164,   165,   248,    12,    33,
      34,    36,    74,   138,   142,   193,   242,   246,   255,   256,
     257,   259,   261,   262,    80,   164,   152,   236,   257,   152,
     140,     7,   164,   166,     9,    80,   166,    55,    90,   174,
     257,   257,    20,    32,   223,   257,   257,   140,   165,   194,
     140,   165,   225,   226,    26,   156,   167,   257,   257,   257,
     257,   257,     7,   140,    32,   170,   170,   130,   216,   235,
     257,    78,    79,   131,   257,   258,   257,   167,   257,   257,
      73,   140,   152,   257,   257,   156,   163,   257,   259,   156,
     163,   257,   165,   221,   235,   257,   257,   257,   257,   257,
     257,   257,   257,     1,   139,   150,   157,   235,    81,   116,
     216,   237,   238,   258,   235,   257,   265,    57,   152,    61,
     155,   165,   165,    41,    60,   211,    21,    55,    65,    84,
     131,   137,    10,    15,    25,    64,    68,    92,   107,   109,
     110,   111,   113,   114,   115,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   127,   128,   129,   132,   133,   134,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   135,   136,   140,   142,    64,    68,
     140,   152,   131,   138,   156,   257,   257,   257,   235,    37,
     248,   221,   131,   112,   131,    87,   165,   216,   239,   240,
     241,   258,   221,   186,   165,   140,   167,    26,    37,   167,
      26,    37,    88,   167,   251,    30,    52,   131,    75,    93,
      94,    95,    96,    97,    98,   101,   102,   103,   104,   105,
     106,   156,   203,   239,   152,   140,   205,    81,   140,   165,
     227,   228,     1,   111,   230,    37,   112,   152,   167,   167,
     239,   166,   165,   112,   131,   257,   257,   131,   138,   167,
     140,   239,   131,   178,   131,   178,   131,    93,   222,   131,
     131,    30,    52,   167,   131,   139,   139,   150,   112,   139,
     257,   112,   141,   112,   141,    37,   112,   143,   251,    91,
     112,   143,     7,    47,   111,   187,   138,   198,    65,   225,
     225,   225,   225,   257,   257,   257,   257,   174,   257,   174,
     257,   257,   257,   257,   257,   257,   257,    27,    80,   165,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   258,   239,   239,   174,   257,   174,   257,    21,    37,
      40,    54,    55,    65,    80,   209,   250,   253,   257,   270,
      26,    37,    26,    37,    75,    37,   143,   174,   257,   167,
     131,   257,   257,    93,   141,   112,   131,   165,    37,   235,
      36,   257,   174,   257,    36,   257,   174,   257,   140,   167,
     134,   158,   160,   257,   158,   159,   152,   257,    28,   257,
     141,   206,   207,   208,   209,   195,   228,   112,   141,   142,
     231,   243,    93,   229,   257,   226,   141,   216,   257,     1,
     183,   239,   141,    16,   179,   231,   243,   112,   160,   159,
     139,   141,   141,   237,   141,   237,   174,   257,   143,   152,
     257,   143,   257,   143,   257,   165,   235,   138,     1,   165,
     191,   192,    21,    55,    65,    80,   200,   210,   225,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   141,
     143,    37,    65,   242,   112,   141,    64,    64,    36,   174,
     257,    36,   174,   257,   174,   257,   248,   248,   131,   216,
     258,   241,   187,   257,   141,   257,    26,   167,    26,   167,
     257,    26,   167,   251,    26,   167,   251,   252,   253,   112,
     131,    11,   131,    28,    28,   152,    75,   156,   112,   141,
     140,   165,    21,    55,    65,    80,   212,   141,   228,   230,
       1,   235,    50,   258,   139,    53,    85,   139,   184,   141,
     140,   156,   165,   180,   221,   131,   131,   143,   251,   143,
     251,   152,    91,   138,     1,   188,   139,    93,   112,   139,
      65,   199,    10,    15,   107,   108,   109,   110,   113,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     129,   132,   133,   134,   135,   137,   140,   165,   201,   202,
     203,   242,   131,   253,   242,   242,   257,    26,    26,   257,
      26,    26,   143,   143,   167,   167,   138,   167,    75,    36,
     257,    36,   257,    75,    36,   257,   167,    36,   257,   167,
     112,   141,   257,   257,   152,   257,   152,    28,   207,   228,
     111,   234,    65,   230,   229,   143,    37,   143,    26,   152,
     235,   180,   111,   156,    36,   152,   257,   143,    36,   152,
     257,   143,   257,     1,   139,   157,   139,    32,    59,    62,
     139,   151,   169,   257,   192,   212,   165,   257,   114,   140,
     204,   204,    75,    36,    36,    75,    36,    36,    36,    36,
       1,   188,   257,   257,   257,   257,   257,   257,   253,    11,
      28,   152,   141,    27,    71,    74,   142,   216,   233,   243,
     116,   218,   229,    86,   219,   257,   231,   243,   152,   167,
     141,   257,   257,   152,   257,   152,   139,   139,   169,   230,
     141,   165,   202,   203,   206,   257,   257,   257,   257,   257,
     257,   139,   139,    75,    75,    75,    75,   257,   152,   234,
     143,   235,   216,   217,   257,   257,   156,   168,   215,   143,
     156,    75,    75,    77,   213,   204,   204,   141,    75,    75,
      75,    75,    75,    75,   257,   257,   257,   257,   218,   229,
     216,   232,   233,   243,    37,   143,   243,   257,   257,   219,
     257,   232,   233,   131,   214,   215,   143,   232
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   147,   148,   149,   149,   150,   150,   151,   151,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   153,
     153,   153,   154,   154,   154,   155,   155,   156,   156,   156,
     157,   157,   158,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   161,   162,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     164,   164,   165,   166,   166,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   169,   169,   170,   170,   171,
     171,   171,   171,   172,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   177,   177,   177,   177,
     177,   177,   178,   178,   179,   179,   179,   180,   180,   181,
     182,   182,   183,   183,   184,   184,   184,   185,   185,   185,
     185,   186,   186,   186,   187,   187,   188,   188,   188,   189,
     189,   190,   191,   191,   191,   192,   192,   194,   195,   193,
     196,   196,   196,   196,   198,   199,   197,   200,   200,   200,
     200,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     203,   204,   204,   205,   206,   206,   206,   207,   207,   207,
     207,   208,   208,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   210,   210,   210,   210,   211,   211,
     212,   212,   212,   212,   212,   212,   213,   213,   214,   214,
     215,   215,   216,   216,   217,   217,   218,   218,   219,   219,
     220,   220,   220,   221,   221,   222,   222,   222,   223,   223,
     223,   223,   223,   224,   224,   224,   225,   225,   226,   226,
     227,   227,   227,   228,   228,   228,   229,   229,   229,   230,
     230,   230,   230,   231,   231,   231,   231,   232,   232,   232,
     233,   233,   233,   233,   233,   234,   234,   234,   234,   234,
     234,   234,   235,   235,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     241,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   245,   246,   247,   247,   247,   247,   247,   247,   247,
     247,   248,   248,   249,   250,   250,   251,   252,   252,   253,
     253,   253,   254,   254,   255,   256,   257,   257,   257,   257,
     257,   257,   257,   257,   257,   257,   257,   257,   257,   257,
     258,   258,   258,   259,   259,   259,   259,   260,   260,   261,
     261,   261,   262,   262,   262,   263,   263,   263,   263,   264,
     264,   264,   264,   264,   264,   264,   264,   264,   264,   264,
     265,   265,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   267,   267,   267,   267,   267,
     267,   268,   268,   268,   268,   269,   269,   269,   269,   270,
     270,   270,   270,   270,   270,   270
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
       0,     1,     2,     2,     0,     0,    10,     3,     3,     5,
       5,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     0,     1,     3,     4,     4,     6,
       6,     0,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       0,     1,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     2,     1,     0,     1,     2,     2,     0,     2,
       3,     4,     4,     2,     4,     0,     2,     2,     4,     4,
       4,     5,     4,     0,     1,     1,     1,     3,     3,     5,
       1,     1,     3,     1,     2,     3,     0,     2,     2,     0,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     2,
       3,     3,     1,     3,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     2,     2,     1,
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
#line 441 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4952 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 446 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4958 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 447 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 454 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4970 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 459 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4976 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4982 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4988 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 480 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4994 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5000 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 482 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 5006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 483 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 5012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 484 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 5018 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[-1].pcallexpr)); }
#line 5024 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 5030 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5036 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 488 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 5042 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 489 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5048 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5054 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 5060 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 492 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 5066 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 493 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 5072 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 494 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5078 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[-5].b), (yyvsp[-4].b), (yylsp[-5]).comment))); }
#line 5085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType, (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5091 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 504 "chapel.ypp" /* yacc.c:1661  */
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[-6].b), (yyvsp[-5].b), (yylsp[-6]).comment))); }
#line 5098 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 510 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 511 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 5116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 515 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 5122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 5128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 5140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 50:
#line 536 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 537 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 52:
#line 542 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5168 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 547 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5178 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 552 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 556 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5196 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5206 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 592 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 594 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 598 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 602 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 74:
#line 612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 83:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 86:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 87:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
#line 5378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 88:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 89:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 653 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 666 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-1].pexpr)); }
#line 5408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 667 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 669 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[0].pblockstmt)); }
#line 5426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 674 "chapel.ypp" /* yacc.c:1661  */
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
#line 5442 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 688 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5448 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 689 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5454 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 690 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5460 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 691 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5466 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5472 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 693 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5478 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 694 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5484 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 695 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5490 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5496 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5502 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5508 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5514 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 700 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5520 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 701 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pexpr), (yyvsp[-2].pcallexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5526 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 702 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5532 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 703 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5538 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 705 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 711 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5558 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5568 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 723 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pcallexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 729 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5589 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 736 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 745 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 749 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 751 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 752 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 753 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-4].pexpr),(yyvsp[-3].pch),(yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5637 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 755 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-3].pexpr),(yyvsp[-2].pch),(yyvsp[-1].pexpr)), (yyvsp[0].pblockstmt)); }
#line 5644 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 757 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-6].pexpr),(yyvsp[-5].pch),(yyvsp[-4].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5651 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 759 "chapel.ypp" /* yacc.c:1661  */
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[-5].pexpr),(yyvsp[-4].pch),(yyvsp[-3].pexpr)), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5658 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[0].pblockstmt)); }
#line 5664 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pexpr)); }
#line 5670 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pexpr)); }
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5682 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5694 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[-1].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5700 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 777 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 781 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[0].pblockstmt)); }
#line 5718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 782 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 783 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = CatchStmt::build((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5730 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 787 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch)), NULL, new UnresolvedSymExpr("Error")); }
#line 5736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 788 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[-2].pch)), NULL, (yyvsp[0].pexpr));   }
#line 5742 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[-1].pexpr)); }
#line 5748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 796 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5754 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 798 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildErrorStandin(); }
#line 5760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 802 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5766 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 803 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5772 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5778 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 812 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 157:
#line 819 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 829 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             (yyvsp[-1].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5818 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 839 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             NULL,
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5832 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 849 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch),
                                             (yyvsp[-6].pch),
                                             (yyvsp[-5].aggrTag),
                                             (yyvsp[-3].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 861 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5856 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 866 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5866 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 871 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5876 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 879 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5882 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 880 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5888 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 885 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5894 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 887 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5900 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 889 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5906 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 894 "chapel.ypp" /* yacc.c:1661  */
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
#line 5926 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 910 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
#line 5934 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 917 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
#line 5944 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 926 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
#line 5956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pvecOfDefs) = (yyvsp[-1].pvecOfDefs);
    }
#line 5964 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].pvecOfDefs)->push_back((yyvsp[0].pdefexpr));
    }
#line 5972 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5978 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5984 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString.clear();
    }
#line 5993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6002 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 960 "chapel.ypp" /* yacc.c:1661  */
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
#line 6022 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6058 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 6071 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
#line 6081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 1019 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 6092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 1026 "chapel.ypp" /* yacc.c:1661  */
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
#line 6115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 1048 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 6123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 1052 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6132 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 1057 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
    }
#line 6140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 1061 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 6149 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 1069 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6155 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 1074 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 6161 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 1075 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 6167 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 6173 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 1077 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 6179 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 6185 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 1079 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 6191 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 6197 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 1081 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 6203 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 1082 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 6209 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 1083 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 6215 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 1084 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 6221 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1085 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 6227 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1086 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 6233 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1087 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 6239 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1088 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 6245 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1089 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 6251 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1090 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 6257 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1091 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 6263 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1092 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 6269 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1093 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 6275 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1094 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 6281 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1095 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 6287 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1096 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 6293 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1097 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 6299 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 6305 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1102 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 6311 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1103 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 6317 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1104 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 6323 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1105 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 6329 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1106 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 6335 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1107 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 6341 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1108 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 6347 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1109 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6353 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1110 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6359 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1111 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6365 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1112 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6371 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1113 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 231:
#line 1117 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6383 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 232:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6389 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1122 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6395 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1126 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6401 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1127 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1128 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6413 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 237:
#line 1133 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6419 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1135 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6425 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1137 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6431 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1139 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6437 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1143 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6443 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1144 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6449 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1148 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6455 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1149 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6461 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1150 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6467 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1151 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6473 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1152 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6479 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1153 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6485 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1154 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6491 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1155 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6497 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6503 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1160 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6509 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1161 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6515 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1162 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6521 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1163 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF;   }
#line 6527 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1164 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST;   }
#line 6533 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6539 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 258:
#line 1169 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1170 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6551 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1174 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6557 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1175 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6563 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1176 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6569 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1177 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1178 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6581 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1179 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1184 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1187 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1193 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1200 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1204 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1209 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1210 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1220 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1222 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1224 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1229 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6688 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1242 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6706 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1258 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1260 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6718 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1262 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6724 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1267 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6736 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6748 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6760 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1291 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6773 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1300 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6784 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1309 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6790 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6796 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6802 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6811 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1325 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6817 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1327 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6823 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6829 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6835 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1336 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6841 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1341 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6847 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1343 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6853 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1345 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6859 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1351 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6865 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 307:
#line 1352 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6871 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1353 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6877 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1357 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6883 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6889 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1359 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6895 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6901 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 314:
#line 1385 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6917 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 316:
#line 1397 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
#line 6937 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 317:
#line 1403 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6943 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 318:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6949 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 319:
#line 1405 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6955 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 320:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6961 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1412 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6967 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1418 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6973 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1420 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6979 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6985 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1426 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6991 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1427 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6997 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1428 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7003 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 7009 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1430 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 7015 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 7021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1432 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7027 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1438 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7033 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1439 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 7039 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1440 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7045 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1441 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 7051 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1445 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr));}
#line 7057 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1446 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7063 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1450 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 7069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1452 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1456 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1457 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 7099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 7105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1467 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 347:
#line 1471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 7117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 348:
#line 1472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 7123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 349:
#line 1473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 7129 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 350:
#line 1474 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 351:
#line 1478 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 7141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1495 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 7147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 7153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1499 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 7159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1501 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 7165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 7171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1505 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 7177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 7183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1512 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 7195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 366:
#line 1516 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1518 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1520 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1522 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 370:
#line 1524 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 371:
#line 1526 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 7231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 372:
#line 1528 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 373:
#line 1530 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 374:
#line 1532 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 7249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 375:
#line 1534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 376:
#line 1536 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 7266 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 377:
#line 1543 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 7276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1549 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 7286 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1555 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7296 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1561 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 7306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 7312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 7318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1599 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 7330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1604 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 7355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7362 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pShadowVar)); }
#line 7368 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1628 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromArgIntent((yyvsp[-1].pt), (yyvsp[0].pexpr));
    }
#line 7376 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 7392 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7398 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7404 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1652 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7410 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1668 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7416 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1670 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = createCast((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7422 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1672 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7428 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1674 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 7434 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1676 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7440 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1678 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7446 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1682 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryExpr((yyvsp[0].pexpr)); }
#line 7452 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1683 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = tryBangExpr((yyvsp[0].pexpr)); }
#line 7458 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1684 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 7464 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7470 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7476 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1708 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7482 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7488 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1713 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7494 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1714 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7500 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7506 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1723 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7512 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7518 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1725 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7524 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1729 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7530 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1730 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7536 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1731 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7542 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1732 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7548 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1733 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7554 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1734 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7560 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1735 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7566 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1736 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7572 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1737 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7578 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1739 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7586 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1743 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7594 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1750 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7600 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1751 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7606 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1755 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7612 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1756 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7618 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1757 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7624 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1758 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7630 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1759 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7636 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 457:
#line 1760 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7642 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 458:
#line 1761 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7648 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 459:
#line 1762 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7654 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 460:
#line 1763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7660 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 461:
#line 1764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7666 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 462:
#line 1765 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7672 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 463:
#line 1766 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7678 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 464:
#line 1767 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7684 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 465:
#line 1768 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7690 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 466:
#line 1769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7696 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 467:
#line 1770 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7702 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 468:
#line 1771 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7708 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 469:
#line 1772 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7714 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 470:
#line 1773 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7720 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 471:
#line 1774 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7726 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 472:
#line 1775 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7732 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 473:
#line 1776 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7738 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 474:
#line 1777 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7744 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 475:
#line 1781 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7750 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 476:
#line 1782 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7756 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 477:
#line 1783 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7762 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 478:
#line 1784 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7768 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 479:
#line 1785 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7774 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 480:
#line 1786 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7780 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 481:
#line 1790 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 482:
#line 1791 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7792 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 483:
#line 1792 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7798 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 484:
#line 1793 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7804 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 485:
#line 1797 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7810 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 486:
#line 1798 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7816 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 487:
#line 1799 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7822 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 488:
#line 1800 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7828 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 489:
#line 1805 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7834 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 490:
#line 1806 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7840 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 491:
#line 1807 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7846 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 492:
#line 1808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7852 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 493:
#line 1809 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7858 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 494:
#line 1810 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7864 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 495:
#line 1811 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7870 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7874 "bison-chapel.cpp" /* yacc.c:1661  */
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
