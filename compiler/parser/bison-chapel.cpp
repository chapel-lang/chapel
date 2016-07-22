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
#line 33 "chapel.ypp" /* yacc.c:355  */

  extern int  captureTokens;
  extern char captureString[1024];
#line 44 "chapel.ypp" /* yacc.c:355  */

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif
#line 64 "chapel.ypp" /* yacc.c:355  */

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

  struct IntentExpr {
    Expr*  first;
    Expr*  second;
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
    const char*       pch;

    Vec<const char*>* vpch;
    RetTag            retTag;
    bool              b;
    IntentTag         pt;
    Expr*             pexpr;
    DefExpr*          pdefexpr;
    CallExpr*         pcallexpr;
    BlockStmt*        pblockstmt;
    Type*             ptype;
    EnumType*         penumtype;
    FnSymbol*         pfnsymbol;
    Flag              flag;
    ProcIter          procIter;
    FlagSet*          flagSet;
    IntentExpr        pIntentExpr;
    std::vector<OnlyRename*>* ponlylist;
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
    TBY = 270,
    TCLASS = 271,
    TCOBEGIN = 272,
    TCOFORALL = 273,
    TCONFIG = 274,
    TCONST = 275,
    TCONTINUE = 276,
    TDELETE = 277,
    TDMAPPED = 278,
    TDO = 279,
    TDOMAIN = 280,
    TELSE = 281,
    TENUM = 282,
    TEXCEPT = 283,
    TEXPORT = 284,
    TEXTERN = 285,
    TFOR = 286,
    TFORALL = 287,
    TIF = 288,
    TIN = 289,
    TINDEX = 290,
    TINLINE = 291,
    TINOUT = 292,
    TITER = 293,
    TLABEL = 294,
    TLAMBDA = 295,
    TLET = 296,
    TLOCAL = 297,
    TMINUSMINUS = 298,
    TMODULE = 299,
    TNEW = 300,
    TNIL = 301,
    TNOINIT = 302,
    TON = 303,
    TONLY = 304,
    TOTHERWISE = 305,
    TOUT = 306,
    TPARAM = 307,
    TPLUSPLUS = 308,
    TPRAGMA = 309,
    TPRIMITIVE = 310,
    TPRIVATE = 311,
    TPROC = 312,
    TPUBLIC = 313,
    TRECORD = 314,
    TREDUCE = 315,
    TREF = 316,
    TREQUIRE = 317,
    TRETURN = 318,
    TSCAN = 319,
    TSELECT = 320,
    TSERIAL = 321,
    TSINGLE = 322,
    TSPARSE = 323,
    TSUBDOMAIN = 324,
    TSYNC = 325,
    TTHEN = 326,
    TTYPE = 327,
    TUNDERSCORE = 328,
    TUNION = 329,
    TUSE = 330,
    TVAR = 331,
    TWHEN = 332,
    TWHERE = 333,
    TWHILE = 334,
    TWITH = 335,
    TYIELD = 336,
    TZIP = 337,
    TALIAS = 338,
    TAND = 339,
    TASSIGN = 340,
    TASSIGNBAND = 341,
    TASSIGNBOR = 342,
    TASSIGNBXOR = 343,
    TASSIGNDIVIDE = 344,
    TASSIGNEXP = 345,
    TASSIGNLAND = 346,
    TASSIGNLOR = 347,
    TASSIGNMINUS = 348,
    TASSIGNMOD = 349,
    TASSIGNMULTIPLY = 350,
    TASSIGNPLUS = 351,
    TASSIGNSL = 352,
    TASSIGNSR = 353,
    TBAND = 354,
    TBNOT = 355,
    TBOR = 356,
    TBXOR = 357,
    TCOLON = 358,
    TCOMMA = 359,
    TDIVIDE = 360,
    TDOT = 361,
    TDOTDOT = 362,
    TDOTDOTDOT = 363,
    TEQUAL = 364,
    TEXP = 365,
    TGREATER = 366,
    TGREATEREQUAL = 367,
    THASH = 368,
    TLESS = 369,
    TLESSEQUAL = 370,
    TMINUS = 371,
    TMOD = 372,
    TNOT = 373,
    TNOTEQUAL = 374,
    TOR = 375,
    TPLUS = 376,
    TQUESTION = 377,
    TSEMI = 378,
    TSHIFTLEFT = 379,
    TSHIFTRIGHT = 380,
    TSTAR = 381,
    TSWAP = 382,
    TIO = 383,
    TLCBR = 384,
    TRCBR = 385,
    TLP = 386,
    TRP = 387,
    TLSBR = 388,
    TRSBR = 389,
    TNOELSE = 390,
    TUPLUS = 391,
    TUMINUS = 392
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

#line 407 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 413 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  int         captureTokens;
  char        captureString[1024];
#line 193 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForLoop.h"
  #include "parser.h"
  #include "stmt.h"
  #include "stringutil.h"
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

    clean_exit(1);
  }

#line 479 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   11757

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  108
/* YYNRULES -- Number of rules.  */
#define YYNRULES  439
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  838

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   392

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
     135,   136,   137
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   417,   417,   422,   423,   429,   430,   435,   436,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   470,   472,   477,   478,   479,
     494,   495,   500,   501,   506,   511,   516,   520,   526,   527,
     528,   532,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   554,   555,   559,
     563,   564,   568,   569,   573,   574,   578,   579,   580,   581,
     582,   583,   584,   588,   589,   593,   608,   609,   610,   611,
     612,   613,   614,   615,   616,   617,   618,   619,   620,   621,
     622,   623,   624,   630,   636,   642,   648,   655,   665,   669,
     670,   671,   672,   676,   681,   682,   686,   688,   690,   697,
     702,   710,   715,   720,   728,   729,   734,   735,   737,   742,
     752,   761,   765,   773,   774,   779,   784,   778,   809,   815,
     822,   830,   841,   847,   840,   875,   879,   884,   888,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   925,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   940,   941,   945,
     949,   950,   951,   955,   957,   959,   961,   966,   967,   971,
     972,   973,   974,   975,   976,   977,   978,   979,   983,   984,
     985,   986,   990,   991,   995,   996,   997,   998,   999,  1000,
    1004,  1005,  1009,  1010,  1014,  1016,  1021,  1022,  1026,  1027,
    1031,  1032,  1036,  1038,  1040,  1045,  1058,  1075,  1076,  1078,
    1083,  1091,  1099,  1107,  1116,  1126,  1127,  1128,  1132,  1133,
    1141,  1143,  1149,  1154,  1156,  1158,  1163,  1165,  1167,  1174,
    1175,  1176,  1180,  1181,  1186,  1187,  1188,  1189,  1209,  1213,
    1217,  1225,  1229,  1230,  1231,  1235,  1237,  1243,  1245,  1247,
    1252,  1253,  1254,  1255,  1256,  1257,  1258,  1264,  1265,  1266,
    1267,  1271,  1272,  1273,  1277,  1278,  1282,  1283,  1287,  1288,
    1292,  1293,  1294,  1295,  1296,  1300,  1311,  1312,  1313,  1314,
    1315,  1316,  1318,  1320,  1322,  1324,  1326,  1328,  1333,  1335,
    1337,  1339,  1341,  1343,  1345,  1347,  1349,  1351,  1353,  1355,
    1357,  1364,  1370,  1376,  1382,  1391,  1396,  1404,  1405,  1406,
    1407,  1408,  1409,  1410,  1411,  1416,  1417,  1421,  1425,  1427,
    1435,  1446,  1447,  1451,  1452,  1457,  1462,  1470,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1480,  1482,  1484,  1486,
    1488,  1493,  1494,  1495,  1496,  1507,  1508,  1512,  1513,  1514,
    1518,  1519,  1520,  1528,  1529,  1530,  1534,  1535,  1536,  1537,
    1538,  1539,  1540,  1541,  1548,  1549,  1553,  1554,  1555,  1556,
    1557,  1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1579,
    1580,  1581,  1582,  1583,  1584,  1588,  1589,  1590,  1591,  1595,
    1596,  1597,  1598,  1603,  1604,  1605,  1606,  1607,  1608,  1609
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCLASS", "TCOBEGIN",
  "TCOFORALL", "TCONFIG", "TCONST", "TCONTINUE", "TDELETE", "TDMAPPED",
  "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN",
  "TFOR", "TFORALL", "TIF", "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER",
  "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE", "TOUT", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPUBLIC",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE",
  "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "block_stmt", "stmt_ls", "only_ls",
  "use_stmt", "require_stmt", "assignment_stmt", "opt_ident", "ident",
  "opt_ident_or_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_query_expr", "var_arg_expr",
  "opt_where_part", "type_alias_decl_stmt", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_reindex_expr",
  "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_clause",
  "task_intent_ls", "intent_expr", "io_expr", "new_expr", "let_expr",
  "expr", "lhs_expr", "fun_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "literal_expr", "assoc_expr_ls", "binary_op_expr",
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
     385,   386,   387,   388,   389,   390,   391,   392
};
# endif

#define YYPACT_NINF -744

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-744)))

#define YYTABLE_NINF -393

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -744,    94,  2408,  -744,  -744,  -744,  -744,  -744,  -744,  -744,
    -744,  3456,     2,   134,  -744,     2,  6712,    57,   134,  6712,
    3456,    24,   134,   132,   579,  5636,  6712,  6712,   143,  -744,
     134,  -744,    25,  3456,  6712,  6712,  -744,  6712,  6712,   206,
     149,   751,   799,  -744,  5846,  5951,  6712,  6082,  6712,   225,
     185,  3456,   134,  -744,  5846,  6712,  6712,  -744,  -744,  6712,
    -744,  -744,  7657,  6712,  6712,  -744,  6712,  -744,  -744,  2670,
    5424,  5846,  -744,  3325,  -744,  -744,   277,  -744,  -744,  -744,
    -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,
     134,  -744,   -36,    42,  -744,  -744,  -744,   251,   210,  -744,
    -744,  -744,   253,   254,   258,   259,   268, 11457,  1870,    70,
     269,   276,  -744,  -744,  -744,  -744,  -744,  -744,   151,  -744,
   11457,   273,  3456,  -744,   289,  -744,   279,  6712,  6712,  6712,
    6712,  6712,  5846,  5846,   158,  -744,  -744,  -744,  -744,  9070,
     193,  -744,  -744,   134,   290,  9364,   330,  5846,   293,  -744,
    -744,  -744,  -744,   134,   216,   134,   312,    45,  8140,  8082,
    8266,  5846,  3456,   316,    17,    41,    47,  -744,  -744,   883,
     343,  8324,   883,  -744,  5846,  -744,    71,  -744,  -744,   134,
    -744,   -18, 11457,  -744,  9398,  8949,  3456,  -744,  -744,  8324,
   11457,   326,  5846,  -744, 11457,   373,   340,   202,  9178,  8324,
    9483,   278,   588,   883,   278,   883,  -744,  -744,  2801,    82,
    -744,  6712,  -744,    75,    84, 11457,    34,  9527,     7,   463,
    -744,   134,   368,  -744,  -744,  -744,    18,    25,    25,    25,
    -744,  6712,  6712,  6712,  6712,  6187,  6187,  6712,  6712,  6712,
    6712,  6712,  6712,   285,  7657,  6712,  6712,  6712,  6712,  6712,
    6712,  6712,  6712,  6712,  6712,  6712,  6712,  6712,  6712,  6712,
    6712,  6712,  6712,  6712,  6712,  6712,  6712,  6712,  6712,  6712,
    6712,  6712,  6712,  6712,  6712,  5846,  5846,  6187,  6187,  5319,
    -744,  -744,  2932,  -744,  9222,  9330,  9602,    50,  6187,    45,
     350,  -744,  -744,  6712,   336,  -744,   352,   382,  -744, 11457,
     134,   364,   134,   455,  5846,  -744,  3587,  6187,  -744,  3718,
    6187,  -744,    45,  3456,   464,   359,  -744,    51,  -744,  -744,
      17,  -744,   389,   363,  -744,  5004,   414,   413,  6712,    25,
    -744,   367,  -744,  -744,  5846,  -744,  -744,  -744,  -744,  -744,
    5846,   369,  5109,   410,  -744,  -744,  6712,  6712,  -744,  -744,
    -744,  -744,  -744,  8019,  5531,  -744,  5741,  -744,  6187,  2539,
     385,  6712,  6712,  -744,  -744,   392,  5846,   395,   420,    25,
     205,   218,   242,   260,  9012,  1926,  1926,    96,  -744,    96,
    -744,    96,   401,   511,  1305,  1389,   343,   278,  -744,  -744,
    -744,   588, 11631,    96,  2123,  2123,  1926,  2123,  2123,  1156,
     278, 11631, 11573,  1156,   883,   883,   278,  9646,  9714,  9758,
    9826,  9870,  9938,  9982, 10050, 10094, 10162, 10206, 10274, 10318,
   10386, 10430,   396,   397,  -744,    96,  -744,    96,   136,  -744,
    -744,  -744,  -744,  -744,  -744,   134,    87,  -744, 11515,   315,
    6817,  6187,  6922,  6187,  6712,  6187,  1008,     2, 10498,  -744,
    -744, 10542,  6712,  5846,  -744,  5846,   444,    88,  -744,  -744,
     368,  6712,   165,  6712, 11457,    48,  8387,  6712, 11457,    33,
    8203,  -744,   506, 10610,  3456,  -744,   172,  -744,    38,  -744,
     133,   404,    17,    72,  -744,  5846,  -744,   274,  6712,  6292,
    -744, 11457,  -744,  -744,  -744, 11457,   140,   405,  -744,  5846,
    -744,   264,   134,   266, 10668,   270,  -744,  -744,  -744,  -744,
      -3,  7777,  -744, 11457,  3456, 11457, 10731,  3063,   431,  -744,
    -744,  -744,  -744,  -744,  2195,   313,  -744,  -744,  -744,  -744,
    -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,
    -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,
    5319,  -744,  6187,  6187,  6712,   520, 10789,  6712,   523, 10852,
     415,  7840,    45,    45,  -744, 11457,  -744, 11457,  -744,  6712,
     134,  -744,   419,  8324,  -744,  8450,  3849,  -744,  3980,  -744,
    8513,  4111,  -744,    45,  4242,  -744,    45,  3456,  6712,  -744,
     403,  -744,    17,   447,   490,  -744,  -744,  -744,    37,  -744,
    -744,  5109,   413,    55, 11457,  -744, 11457,  4373,  5846,  -744,
    -744,  -744,    74,  -744,  6712,  -744,  6712,  -744,  4504,   418,
    4635,   422,  -744,  6712,  -744,  3194,   815,   133,  -744,  -744,
    -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,
    -744,  -744,  -744,   134,  -744,  -744,  -744,  -744,  -744,  -744,
    -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,  -744,
    -744,  -744,  -744,  -744,   448,   426,   426,  -744,  -744,   175,
     181, 10910,  7027,  7132, 10973,  7237,  7342,  7447,  7552,  -744,
    -744, 11457,  -744,  -744,  -744,  3456,  6712, 11457,  6712, 11457,
    3456,  6712, 11457,  -744,  6712, 11457,  -744,  -744, 11457,  -744,
     421,  6397,   255,  -744,   482,  -744,  6187,  4742,  3456,  -744,
      43,  4873, 11031, 11457,  6712,  -744, 11457,  3456,  6712,  -744,
   11457,  3456, 11457,  -744,   407,   566,   566,  -744,  1218,  -744,
      29,  -744,  2195,    51,  -744,  -744,  6712,  6712,  6712,  6712,
    6712,  6712,  6712,  6712,  1071, 10610,  8576,  8639, 10610,  8702,
    8765,   447,    24,  6712,  6712,  5214,  -744,  -744,   207,  5846,
    -744,  -744,  6712,    13,  7898,  -744,   322,   330,  -744,  6712,
    8828,  -744,  8891,  -744,  -744,   482,  -744,   426,   426,   182,
   11094, 11152, 11215, 11273, 11336, 11394,  -744,  3456,  3456,  3456,
    3456,   255,  6502,    76,  -744,  -744, 11457, 11457,  -744,  -744,
    -744,  7552, 11457,  3456,  3456,    64,  -744,  -744,  -744,  6712,
    6712,  6712,  6712,  6712,  6712, 10610, 10610, 10610, 10610,  -744,
    -744,  -744,  -744,  -744,   286,  6187,  7736,   460, 10610, 10610,
    -744,  -744,  -744,  7961,  -744,  -744,  6607,  -744
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    69,   386,   387,   388,   389,
     390,     0,   345,    67,   121,   345,     0,   246,    67,     0,
       0,     0,     0,    67,    67,     0,     0,     0,     0,   139,
       0,   135,     0,     0,     0,     0,   336,     0,     0,     0,
       0,   245,   245,   122,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   123,     0,     0,     0,   435,   437,     0,
     438,   439,   370,     0,     0,   436,   433,    76,   434,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   305,    19,    13,    77,    15,    17,    16,    18,    82,
       0,    80,   362,     0,    83,    81,    84,     0,   371,   358,
     359,   308,   306,     0,     0,   363,   364,     0,   307,     0,
     372,   373,   374,   357,   310,   309,   360,   361,     0,    21,
     316,     0,     0,   346,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,   362,   371,   306,   363,   364,   345,
     307,   372,   373,     0,     0,     0,     0,   296,     0,    71,
      70,   140,    85,     0,   141,     0,     0,     0,     0,     0,
       0,   296,     0,     0,     0,     0,     0,   248,    28,   421,
     355,     0,   422,     7,   296,   246,   247,    79,    78,   225,
     288,     0,   287,    74,     0,     0,     0,    73,    31,     0,
     311,     0,   296,    32,   317,   237,     0,     0,   287,     0,
       0,   424,   369,   420,   423,   419,    40,    42,     0,     0,
     291,     0,   293,     0,     0,   292,     0,   287,     0,     0,
       6,     0,   124,   213,   212,   142,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   368,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   296,   296,     0,     0,     0,
      22,    23,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,   305,   303,     0,   297,   298,   304,
       0,     0,     0,     0,     0,    94,     0,     0,    93,     0,
       0,   100,     0,     0,   110,     0,    27,   190,   136,   253,
       0,   254,   256,     0,   267,     0,     0,   259,     0,     0,
      29,     0,   141,   224,     0,    51,    75,   114,    72,    30,
     296,     0,     0,   235,   232,    48,     0,     0,    87,    33,
      41,    43,   391,     0,     0,   383,     0,   385,     0,     0,
       0,     0,     0,   393,     8,     0,     0,     0,   208,     0,
       0,     0,     0,     0,   344,   416,   415,   418,   426,   425,
     430,   429,   412,   409,   410,   411,   366,   399,   382,   381,
     380,   367,   403,   414,   408,   406,   417,   407,   405,   397,
     402,   404,   413,   396,   400,   401,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   428,   427,   432,   431,   202,   199,
     200,   201,   205,   206,   207,     0,     0,   348,     0,     0,
       0,     0,     0,     0,     0,     0,   392,   345,   345,    90,
     233,     0,     0,     0,   313,     0,   133,     0,   130,   234,
     124,     0,     0,     0,   320,     0,     0,     0,   326,     0,
       0,   101,   109,     0,     0,   312,     0,   191,     0,   198,
     214,     0,   257,     0,   271,     0,   266,   358,     0,     0,
     250,   356,   249,   379,   290,   289,     0,     0,   314,     0,
     239,   358,     0,     0,    44,     0,   365,   384,   294,   295,
       0,     0,   106,   330,     0,   394,     0,     0,   125,   126,
     209,   210,   211,   143,     0,     0,   241,   240,   242,   244,
      52,    59,    60,    61,    56,    58,    65,    66,    54,    57,
      55,    53,    63,    62,    64,   377,   378,   203,   204,   350,
       0,   347,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,   302,   300,   301,   299,     0,
     131,   129,     0,     0,   108,     0,     0,    92,     0,    91,
       0,     0,    98,     0,     0,    96,     0,     0,     0,   112,
     197,   189,     0,   280,   215,   218,   217,   219,     0,   255,
     258,     0,   259,     0,   251,   260,   261,     0,     0,   113,
     115,   315,     0,   236,     0,    49,     0,    50,     0,     0,
       0,     0,   107,     0,    35,     0,   245,   214,   172,   170,
     175,   182,   183,   184,   179,   181,   177,   180,   178,   176,
     186,   185,   151,   154,   152,   153,   164,   155,   168,   160,
     158,   171,   159,   157,   162,   167,   169,   156,   161,   165,
     166,   163,   173,   174,   149,   187,   187,   243,   349,   371,
     371,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      88,   134,   132,   126,    95,     0,     0,   319,     0,   318,
       0,     0,   325,    99,     0,   324,    97,   111,   335,   192,
       0,     0,   259,   216,   230,   252,     0,     0,     0,   117,
       0,     0,    46,    45,     0,   104,   332,     0,     0,   102,
     331,     0,   395,    36,    67,   245,   245,   119,   245,   127,
       0,   150,     0,   190,   145,   146,     0,     0,     0,     0,
       0,     0,     0,     0,   245,   323,     0,     0,   329,     0,
       0,   280,   283,   284,   285,     0,   282,   286,   358,   226,
     194,   193,     0,     0,     0,   269,   358,   118,   116,     0,
       0,   105,     0,   103,   128,   230,   149,   187,   187,     0,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
       0,   259,   272,     0,   227,   229,   228,   231,   222,   223,
     137,     0,    47,     0,     0,     0,   147,   148,   188,     0,
       0,     0,     0,     0,     0,   322,   321,   328,   327,   196,
     195,   274,   275,   277,   358,     0,   392,   358,   334,   333,
     220,   144,   221,     0,   276,   278,   272,   279
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -744,  -744,  -744,     1,  -579,  1785,  -744,  -744,  1429,    46,
     215,  -744,  -744,  -744,    78,  1614,    -5,   173,  -707,  -561,
     -40,  -744,  -744,   118,  -744,  -744,  -744,  -744,  -744,   412,
     108,  -114,  -744,  -744,     4,   599,  -744,  -744,  -744,  -744,
    -744,  -744,  -744,  -160,  -154,  -617,  -744,  -153,    -6,  -744,
    -269,  -744,  -744,   -37,  -744,  -218,   126,  -744,  -200,  -183,
    -744,  -136,  -744,  -744,  -744,  -165,   265,  -744,  -309,  -577,
    -744,  -452,  -338,  -743,  -722,  -158,   -32,    92,  -744,  -142,
    -744,   145,   347,  -296,  -744,  -744,   753,  -744,    -9,  -150,
    -744,    56,  -744,   884,   909,   -11,  1138,  -744,  1297,  1547,
    -744,  -744,  -744,  -744,  -744,  -744,  -744,  -266
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   472,    75,    76,    77,   208,
     503,    78,    79,    80,   150,    81,   154,   188,    82,    83,
      84,    85,    86,   560,    87,    88,   496,   610,    89,    90,
     367,   626,    91,   457,   458,   134,   163,   480,    93,    94,
     368,   627,   523,   665,   666,   734,   318,   476,   477,   478,
     479,   524,   225,   598,   831,   800,   180,   795,   760,   763,
      95,   196,   343,    96,    97,   166,   167,   322,   323,   490,
     326,   327,   486,   822,   757,   702,   209,   213,   214,   296,
     297,   298,   135,    99,   100,   101,   136,   103,   122,   123,
     436,   437,   104,   137,   138,   107,   140,   109,   141,   142,
     112,   113,   218,   114,   115,   116,   117,   118
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     120,   177,   178,    72,   500,   139,   126,   290,   145,   312,
     435,   481,   181,   439,   158,   159,   160,   301,   151,   315,
       5,     5,   197,   169,   170,   705,   171,   172,     5,   487,
     324,   602,   331,   182,   184,   185,   189,   190,   324,   216,
     194,     5,   324,   198,   199,   200,   501,   728,   201,   735,
     341,   202,   203,   204,  -197,   205,   799,   581,   182,   215,
     217,   370,   371,   372,   373,   729,   360,   186,   358,   186,
     823,   428,   576,   324,     5,  -264,    45,   121,   149,   369,
     223,   328,   121,   834,   445,   429,   334,  -341,   430,   706,
     319,   124,  -264,   837,     3,  -376,   144,  -376,   799,   224,
    -264,   287,   431,   432,   835,   335,  -264,  -264,   706,   -67,
     825,   362,   433,   121,   121,  -264,   120,   284,   285,   286,
     194,   182,   217,   434,  -262,   761,  -264,    45,   -67,   143,
     289,   618,   601,   422,   423,     5,   299,     5,   334,   149,
     601,   363,   282,   157,   325,  -264,   704,   334,   320,   164,
     299,   329,  -264,   594,   334,   147,   164,  -264,  -264,   334,
     806,   807,   282,   299,  -264,   728,  -264,   774,   359,   592,
     547,   332,   282,   600,   282,   601,  -264,   282,   334,   354,
     334,   299,  -197,   729,   446,   595,   334,   830,   356,   707,
     607,   550,   570,   282,   596,  -264,   212,   548,   497,   241,
     353,   275,   243,   276,   525,   597,   246,   355,   711,   351,
     826,   277,   352,   173,   820,   278,   357,   608,   571,   551,
     374,   375,   376,   377,   379,   381,   382,   383,   384,   385,
     386,   387,    14,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   299,   299,   425,   427,   438,   334,
     609,   226,   462,   295,   161,    43,   590,   448,   775,  -351,
     174,   435,   451,   700,   439,  -352,   590,   295,     5,  -376,
      53,  -376,  -281,   182,   191,   464,   466,   574,   468,   470,
     295,   234,   473,   227,   591,   487,   334,  -351,  -265,   329,
     388,  -281,   228,  -352,   808,  -281,   192,   491,   295,   583,
     586,   221,   329,   495,  -375,   345,  -375,   229,   526,   299,
     305,   308,   311,  -338,   518,   504,   504,  -265,   235,  -281,
     489,   527,   236,   215,   330,   215,   329,   511,   513,    98,
     515,   516,  -265,   378,   380,   182,  -268,   389,    98,  -265,
     619,   621,   339,   759,   329,   528,   613,    98,  -238,   765,
     614,  -273,   348,   765,   614,   553,  -337,   230,  -265,   278,
      98,   241,  -342,   529,   243,  -268,   231,  -238,   246,   615,
    -273,  -343,  -340,   617,  -273,   424,   426,  -265,    98,  -339,
    -268,   295,   295,  -265,   279,   758,   447,  -268,   282,   293,
       5,   766,   281,   291,   149,   766,    98,   329,  -273,   452,
      98,   453,   300,   428,   234,   465,  -268,  -247,   469,   464,
     556,   468,   559,   473,   561,   513,   667,   429,   562,   563,
     430,   565,   567,   304,   299,  -268,   508,   317,   509,   243,
     573,  -268,   575,   603,   431,   432,   580,   340,   342,  -247,
     494,   235,   449,   344,   433,   236,   295,   612,  -247,    98,
     364,   366,   520,   450,   217,   434,   510,   604,   606,   153,
     212,   521,   212,  -247,   454,   471,   455,   459,   217,   461,
     474,   475,   522,   482,  -270,   483,   824,   488,   489,   493,
     238,   498,   239,   240,   241,   827,   242,   243,   244,    98,
     245,   246,   247,   248,   502,   250,   251,   252,   253,   514,
     254,   517,   256,  -270,   519,   257,   258,   259,   545,   569,
     824,   546,   587,    98,   234,   334,   599,   611,  -270,   438,
     824,   379,   425,   671,   672,  -270,   674,   675,   683,   677,
     701,   703,   717,   751,   732,    98,   721,   733,   681,   555,
     762,   558,   505,   625,  -270,   687,   302,   689,   572,   744,
     692,   235,   777,   695,   682,   236,   710,   698,   778,   566,
     779,   295,     5,  -270,   699,   175,   149,   832,   152,  -270,
     730,   819,   805,   791,   492,    23,   176,   182,     0,  -247,
     568,    92,    29,   712,  -138,   713,   668,   716,     0,   720,
      92,   234,   722,     0,   241,     0,   242,   243,     0,    92,
       0,   246,     0,  -138,     0,     0,   351,     0,   253,    98,
       0,  -247,    92,     0,     0,   257,   258,   259,   577,   579,
    -247,     0,   582,   585,     0,     0,     0,     0,   235,     0,
      92,   153,   236,    98,     0,  -247,    98,     0,     0,     0,
      98,   687,   689,     0,   692,   695,   716,   720,    92,     0,
     378,   424,    92,     0,   745,   746,     0,   747,     0,   748,
     749,     0,     0,   750,     0,   177,   178,   238,     0,   239,
     240,   241,     0,   242,   243,   764,   513,     0,   246,     0,
     513,     0,     0,   770,   252,   253,    98,   772,     0,   256,
       0,     0,   257,   258,   259,     0,     0,     0,     0,     0,
       0,    92,     0,   793,     0,   745,   780,   781,   748,   782,
     783,   784,   785,     0,     0,   679,   680,     0,     0,     0,
       0,     0,   190,   194,   217,     0,   684,     0,   796,     0,
       0,   797,     0,     0,     0,   102,   693,     0,   802,   696,
       0,    92,     0,     0,   102,     0,     0,     0,     0,     0,
     175,     0,     0,   102,     0,     0,   815,   816,   817,   818,
      23,   176,   549,     0,     0,    92,   102,    29,     0,  -138,
     720,     0,   828,   829,     0,   -39,     0,     0,   815,   816,
     817,   818,   828,   829,   102,     0,     0,    92,  -138,     0,
       0,     0,     0,     0,   833,   513,     0,     0,   175,     0,
       0,    98,   102,     0,     0,   720,   102,   756,    23,   176,
       0,    14,     0,     0,    17,    29,     0,  -138,     0,     0,
       0,     0,    22,   -38,    23,   724,     0,     0,     0,     0,
       0,    29,     0,  -138,     0,     0,  -138,     0,     0,     0,
       0,    98,     0,     0,    98,     0,     0,     0,     0,    39,
       0,   725,  -138,   726,    43,   102,     0,     0,     0,     0,
       0,    92,     0,   768,     0,   794,   105,    52,     0,    53,
       0,     0,     0,     0,     0,   105,     0,     0,     0,   669,
     670,     0,     0,     0,   105,    92,   234,     0,    92,     0,
       0,   106,    92,     0,     0,   102,     0,   105,   821,     0,
     106,     0,     0,    98,     0,    98,     0,     0,    98,   106,
       0,    98,     0,     0,    98,   105,     0,     0,    67,   102,
       0,     0,   106,   235,     0,   727,     0,   236,     0,     0,
       0,     0,   821,   105,    98,     0,     0,   105,    92,     0,
     106,   102,   821,     0,     0,    98,     0,    98,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,   106,     0,
       0,     0,   106,     0,     0,     0,   241,     0,   242,   243,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,     0,     0,   105,     0,     0,   259,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,    98,    21,     0,   102,     0,    98,     0,   128,
     129,   130,     0,    28,     0,     0,   105,     0,    31,    32,
       0,    34,     0,    35,    36,    98,     0,     0,     0,   102,
       0,    38,   102,    40,    98,     0,   102,     0,    98,     0,
     105,   106,     0,    92,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,    14,     0,     0,
      17,     0,   105,     0,     0,   106,     0,     0,    22,     0,
      23,   724,     0,     0,     0,     0,     0,    29,    59,  -138,
       0,     0,   102,    92,     0,     0,    92,   106,     0,     0,
       0,     0,     0,     0,     0,    39,    64,   725,  -138,   726,
      43,     0,     0,     0,    98,    98,    98,    98,     0,    70,
     108,   133,     0,    52,     0,    53,     0,     0,     0,   108,
      98,    98,     0,     0,     0,     0,     0,     0,   108,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,     0,
       0,   108,     0,     0,     0,    92,     0,    92,     0,   234,
      92,     0,     0,    92,     0,     0,    92,     0,     0,   108,
     105,   106,     0,   105,    67,     0,     0,   105,     0,     0,
       0,   786,     0,     0,     0,     0,    92,   108,     0,     0,
       0,   108,     0,     0,     0,   106,   235,    92,   106,    92,
     236,     0,   106,     0,    92,     0,     0,   102,     0,     0,
       0,     0,     0,     0,    14,     0,     0,    17,     0,     0,
       0,     0,     0,   105,     0,    22,     0,    23,   724,     0,
       0,     0,     0,     0,    29,   238,  -138,   239,   240,   241,
     108,   242,   243,     0,     0,     0,   246,   102,   106,     0,
     102,     0,   219,   253,   725,  -138,   726,    43,     0,     0,
     257,   258,   259,     0,    92,     0,     0,     0,     0,    92,
      52,     0,    53,     0,     0,     0,     0,     0,     0,   110,
     108,     0,     0,     0,     0,     0,     0,    92,   110,     0,
       0,     0,     0,     0,     0,     0,    92,   110,     0,     0,
      92,     0,     0,     0,   108,     0,     0,     0,   234,   102,
     110,   102,     0,     0,   102,     0,     0,   102,     0,     0,
     102,    67,     0,     0,     0,     0,   108,     0,   110,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
     102,     0,     0,     0,     0,   235,   110,     0,     0,   236,
     110,   102,     0,   102,     0,     0,     0,     0,   102,     0,
       0,     0,     0,   106,     0,     0,    92,    92,    92,    92,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
       0,   105,    92,    92,   238,     0,     0,   240,   241,     0,
     242,   243,   234,     0,     0,   246,     0,     0,     0,   110,
     108,     0,   253,   106,     0,     0,   106,     0,     0,   257,
     258,   259,     0,     0,     0,     0,     0,     0,   102,     0,
       0,     0,     0,   102,   108,     0,     0,   108,     0,   235,
       0,   108,     0,   236,     0,     0,     0,     0,     0,   110,
     105,   102,   105,     0,     0,   105,     0,     0,   105,     0,
     102,   105,     0,     0,   102,     0,   187,     0,     0,     0,
       0,     0,     0,   110,     0,   106,     0,   106,   238,     0,
     106,   105,   241,   106,   242,   243,   106,   108,     0,   246,
       0,     0,   105,     0,   105,   110,   253,     0,     0,   105,
       0,     0,     0,   257,   258,   259,   106,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,   106,
       0,     0,     0,     0,   106,     0,     0,     0,     0,     0,
     102,   102,   102,   102,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,   283,   102,   102,   111,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,   105,
       0,     0,     0,     0,   105,     0,     0,     0,     0,   110,
     111,     0,     0,     0,     0,     0,   187,   187,   187,   314,
       0,     0,   105,     0,   106,     0,     0,     0,   111,   106,
     187,   105,     0,   110,     0,   105,   110,     0,     0,     0,
     110,     0,   108,     0,     0,     0,   111,   106,   187,     0,
     111,     0,     0,     0,     0,     0,   106,   125,   187,     0,
     106,     0,   125,     0,     0,     0,   148,   125,   125,     0,
       0,     0,     0,     0,   162,     0,   165,     0,     0,     0,
       0,     0,   108,     0,     0,   108,   110,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,     0,     0,   111,
       0,   105,   105,   105,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,   105,     0,
       0,     0,     0,     0,     0,     0,   106,   106,   106,   106,
       0,     0,     0,     0,   222,     0,     0,     0,     0,   111,
       0,     0,   106,   106,   108,     0,   108,     0,   187,   108,
       0,     0,   108,     0,     0,   108,     0,     0,     0,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,   187,     0,     0,     0,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   108,   195,   108,     0,
       0,   294,     0,   108,     0,     0,     0,   195,     0,   303,
       0,   110,     0,     0,     0,   294,     0,     0,   321,     0,
       0,     0,     0,     0,     0,     0,     0,    74,   294,     0,
     125,     0,     0,   333,     0,     0,   119,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   294,     0,     0,     0,
       0,   110,     0,     0,   110,     0,     0,     0,   168,     0,
       0,     0,     0,   108,     0,     0,     0,     0,   108,   111,
       0,     0,     0,     0,     0,   365,   193,     0,     0,     0,
     165,   165,   165,   165,     0,     0,   108,     0,     0,     0,
       0,     0,     0,   111,    74,   108,   111,   390,   220,   108,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,     0,   110,     0,     0,   110,     0,
       0,   110,     0,     0,   110,     0,     0,     0,     0,   294,
     294,     0,     0,     0,   187,   187,     0,     0,   187,   187,
       0,     0,     0,     0,   110,     0,   111,   280,     0,     0,
       0,     0,     0,     0,   456,   110,   460,   110,     0,     0,
       0,     0,   110,     0,     0,   108,   108,   108,   108,     0,
       0,     0,     0,     0,   321,     0,     0,     0,     0,     0,
       0,   108,   108,   165,     0,     0,     0,   316,     0,   234,
       0,     0,     0,     0,   294,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,     0,
       0,   338,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,   165,     0,     0,   235,   110,     0,     0,
     236,   187,   187,    74,     0,     0,     0,   274,  -353,     0,
       0,  -375,   187,  -375,   314,   110,     0,     0,     0,   314,
     237,     0,   187,     0,   110,   187,     0,     0,   110,     0,
       0,   111,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,     0,
     250,   251,   252,   253,     0,   254,   255,   256,     0,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,   111,     0,     0,    74,     0,   294,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,   110,   110,   110,     0,     0,
       0,   338,   593,     0,   338,     0,   321,     0,     0,     0,
     110,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,     0,     0,     0,
       0,     0,     0,   111,     0,   111,     0,     0,   111,     0,
       0,   111,     0,     0,   111,     0,     0,     0,   664,   187,
       0,     0,     0,     0,   512,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,   111,     0,     0,
       0,     0,   111,     0,     0,   314,   314,     0,   314,   314,
       0,     0,     0,   235,   456,     0,     0,   236,     0,     0,
       0,     0,   798,     0,     0,     0,     0,     0,     5,   314,
       0,   314,     0,     0,     0,   628,   321,     0,     0,     0,
     629,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   111,   246,   798,     0,     0,   111,     0,   252,
     253,     0,     0,     0,   256,     0,     0,   257,   258,   259,
       0,     0,     0,     0,     0,   111,     0,   731,     0,   589,
       0,     0,     0,     0,   111,     0,     0,     0,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     630,   631,   632,   633,   634,   635,     0,     0,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,     0,   622,
     646,     0,    74,     0,   647,   648,   649,   650,   651,   652,
     653,   654,   655,   656,   657,     0,   658,     0,     0,   659,
     660,   661,   662,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,   111,   111,   111,   125,     0,
       0,     0,     0,     0,     0,     0,   776,     0,     0,     0,
     111,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   338,     0,   338,     0,     0,   338,     0,     0,   338,
       0,     0,   697,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   709,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   715,     0,   719,     0,     0,    -2,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -245,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -138,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -245,    38,    39,    40,    41,  -138,    42,    43,     0,  -245,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -245,     0,     0,    55,     0,    56,
       0,     0,    57,   767,     0,     0,     0,     0,     0,     0,
       0,     0,   771,     0,     0,     0,   773,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,  -392,
       0,    11,    12,    13,  -392,    14,    15,    16,    17,  -245,
      18,    19,  -392,    20,    21,  -392,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -138,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -245,    38,     0,    40,    41,  -138,    42,    43,  -392,
    -245,    44,    45,  -392,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -245,     0,     0,    55,     0,
      56,     0,     0,  -392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -392,    59,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,     0,  -392,  -392,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,    64,  -392,  -392,
    -392,     0,    67,  -392,  -392,  -392,     0,     0,    69,  -392,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -245,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -138,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -245,    38,    39,    40,    41,  -138,    42,    43,
       0,  -245,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -245,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,   179,    67,     0,     0,    68,     0,     0,    69,
     206,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -245,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -138,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -245,    38,    39,    40,    41,  -138,    42,
      43,     0,  -245,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -245,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,   350,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -245,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -138,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -245,    38,    39,    40,    41,  -138,
      42,    43,     0,  -245,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -245,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,   206,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -245,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -138,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -245,    38,    39,    40,    41,
    -138,    42,    43,     0,  -245,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -245,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,   624,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -245,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -138,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -245,    38,    39,    40,
      41,  -138,    42,    43,     0,  -245,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -245,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,    69,   723,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -245,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -138,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -245,    38,   219,
      40,    41,  -138,    42,    43,     0,  -245,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -245,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -245,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -138,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -245,    38,
       0,    40,    41,  -138,    42,    43,     0,  -245,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -245,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -245,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
     463,     0,    28,    29,     0,  -138,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -245,
      38,     0,    40,    41,  -138,    42,    43,     0,  -245,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -245,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -245,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   467,     0,    28,    29,     0,  -138,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -245,    38,     0,    40,    41,  -138,    42,    43,     0,  -245,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -245,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -245,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   686,     0,    28,    29,     0,  -138,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -245,    38,     0,    40,    41,  -138,    42,    43,     0,
    -245,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -245,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -245,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   688,     0,    28,    29,     0,  -138,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -245,    38,     0,    40,    41,  -138,    42,    43,
       0,  -245,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -245,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,    69,
       0,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -245,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   691,     0,    28,    29,     0,  -138,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -245,    38,     0,    40,    41,  -138,    42,
      43,     0,  -245,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -245,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -245,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   694,     0,    28,    29,     0,
    -138,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -245,    38,     0,    40,    41,  -138,
      42,    43,     0,  -245,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -245,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -245,    18,    19,     0,   708,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -138,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -245,    38,     0,    40,    41,
    -138,    42,    43,     0,  -245,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -245,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -245,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   714,     0,    28,
      29,     0,  -138,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -245,    38,     0,    40,
      41,  -138,    42,    43,     0,  -245,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -245,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,    69,     0,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -245,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,   718,     0,
      28,    29,     0,  -138,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -245,    38,     0,
      40,    41,  -138,    42,    43,     0,  -245,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -245,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,   484,     0,     5,     6,     7,     8,     9,
      10,    63,  -392,    64,   127,    65,    66,  -392,    67,     0,
       0,    68,     0,     0,    69,  -392,    70,    21,    71,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,  -392,     0,     0,     0,  -392,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -263,  -392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -392,    59,  -392,  -392,  -392,     0,  -392,  -392,  -392,
       0,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,
      64,  -392,  -392,  -392,     0,     0,  -392,  -392,  -392,     0,
       0,   132,     0,    70,   484,   499,     5,     6,     7,     8,
       9,    10,     0,  -392,     0,   127,     0,     0,  -392,     0,
       0,     0,     0,     0,     0,     0,  -392,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,  -392,     0,     0,     0,  -392,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -392,    59,  -392,  -392,  -392,     0,  -392,  -392,
    -392,     0,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,
    -392,    64,  -392,  -392,  -392,     0,     0,  -392,  -392,  -392,
       0,     0,   132,     0,    70,   484,   499,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
     484,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,   132,    21,    70,     0,   485,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     5,     6,     7,
       8,     9,    10,     0,     0,    63,   127,    64,     0,    65,
      66,     0,     0,     0,     0,    68,     0,     0,   132,    21,
      70,     0,   499,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,   179,     0,     0,   428,
      68,     0,     0,   132,    21,    70,     0,   133,   792,     0,
     128,   129,   130,   429,    28,     0,   430,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
     431,   432,    38,     0,    40,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,   434,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     5,     6,     7,
       8,     9,    10,     0,     0,    63,   127,    64,     0,    65,
      66,     0,     0,     0,     0,    68,     0,     0,   132,    21,
      70,     0,   133,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,   210,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,   211,     0,     5,     6,     7,     8,     9,    10,
      63,     0,    64,   127,    65,    66,   179,     0,     0,     0,
      68,     0,     0,   132,     0,    70,    21,   133,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,   210,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,   179,     0,     0,     0,    68,     0,     0,
     132,    21,    70,   507,   133,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,   155,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   156,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,     0,    68,     0,     0,   132,    21,    70,     0,   133,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,   210,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,   179,     0,     0,     0,    68,     0,     0,
     132,    21,    70,     0,   133,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,   179,     0,
       0,     0,    68,     0,     0,   132,    21,    70,     0,   133,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,   183,     0,     0,    68,     0,     0,
     132,     0,    70,     0,   133,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,    21,     0,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,    69,    21,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   156,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,   605,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,   752,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   753,    49,    50,   754,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,   179,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     755,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   755,     0,     0,   128,   129,
     743,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,   179,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     554,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   557,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     737,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   738,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     740,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   741,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     742,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   743,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,    63,     0,    64,    31,    32,    66,    34,
       0,    35,    36,    68,     0,     0,   132,   232,    70,    38,
     133,    40,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -272,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,   235,     0,     0,
    -272,   236,     0,     0,  -272,     0,     0,     0,     0,     0,
     232,     0,     0,     0,    64,   233,     0,   121,   179,     0,
       0,   237,     0,   234,     0,   132,     0,    70,  -272,   755,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,   232,     0,
       0,   620,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,     0,
       0,   232,     0,     0,   678,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,   232,
       0,     0,   801,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
       0,     0,   232,     0,     0,   836,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   310,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
     232,   506,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,   121,   234,   306,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,   282,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,   584,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,   282,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,   233,     0,   121,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   282,     0,   232,     0,     0,   313,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   186,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,   282,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,   282,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,   282,     0,     0,     0,
       0,   685,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,   282,
       0,     0,     0,     0,   690,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   282,     0,     0,     0,     0,   787,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
     788,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   282,     0,
       0,     0,     0,   789,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   282,     0,     0,     0,     0,   790,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,     0,     0,     0,   803,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,   282,     0,   232,
       0,     0,   804,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
     282,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   337,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,   288,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
    -354,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   232,   254,
     255,   256,     0,   233,   257,   258,   259,     0,     0,     0,
       0,   234,     0,     0,     0,     0,   346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,   235,     0,
       0,     0,   236,     0,     0,   234,   440,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   441,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,     0,   257,   258,   259,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     232,   254,   255,   256,     0,   233,   257,   258,   259,     0,
       0,     0,     0,   234,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     235,     0,     0,     0,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   235,     0,     0,     0,   236,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   237,   254,
     255,   256,     0,     0,   257,   258,   259,     0,   235,     0,
       0,     0,   236,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   237,   254,   255,   256,     0,   292,   257,   258,
     259,     0,     0,   232,     0,     0,     0,   238,   233,   239,
     240,   241,     0,   242,   243,   244,   234,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   336,   257,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,   235,     0,     0,     0,   236,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,   235,   242,   243,
     244,   236,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   349,   257,   258,   259,
     361,   237,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
       0,   257,   258,   259,     0,     0,   232,     0,     0,     0,
       0,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,     0,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   530,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   531,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   532,   257,   258,   259,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   533,
     257,   258,   259,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   534,   257,   258,   259,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   535,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   536,   257,   258,   259,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   537,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   538,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   539,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   540,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   541,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   542,   257,   258,   259,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   543,
     257,   258,   259,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   544,   257,   258,   259,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,     0,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   588,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   564,   257,   258,   259,     0,
     235,     0,     0,     0,   236,     0,     0,     0,   232,   616,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,   673,   623,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,   676,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,   736,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,     0,
       0,   232,   769,     0,   739,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,     0,   257,   258,   259,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
       0,     0,   232,     0,     0,   809,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,   810,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,     0,     0,   232,     0,     0,   811,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,   812,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,   232,     0,     0,   813,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,   814,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   552,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,     0,   250,   251,   252,
     253,   235,   254,     0,   256,   236,     0,   257,   258,   259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
       0,   246,   247,   248,     0,   250,   251,   252,   253,     0,
       0,     0,   256,     0,     0,   257,   258,   259
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   342,    16,    15,   143,    19,   159,
     279,   320,    44,   279,    25,    26,    27,   153,    23,   161,
       3,     3,    54,    34,    35,   602,    37,    38,     3,   325,
       1,   483,   174,    44,    45,    46,    47,    48,     1,    71,
      51,     3,     1,    54,    55,    56,   342,   626,    59,   666,
     192,    62,    63,    64,     3,    66,   763,    24,    69,    70,
      71,   226,   227,   228,   229,   626,   216,    24,    34,    24,
     792,    20,    24,     1,     3,    34,    63,    80,     7,    61,
      38,    34,    80,   826,    34,    34,   104,   123,    37,    34,
      73,    13,    63,   836,     0,   131,    18,   133,   805,    57,
      63,   133,    51,    52,   826,   123,    34,    78,    34,    38,
      34,   104,    61,    80,    80,    78,   127,   128,   129,   130,
     131,   132,   133,    72,    83,   702,    85,    63,    57,    72,
     139,   134,   103,   275,   276,     3,   147,     3,   104,     7,
     103,   134,   129,    25,   103,   104,   598,   104,   131,   131,
     161,   104,   123,    20,   104,   131,   131,    85,   129,   104,
     777,   778,   129,   174,   123,   744,   129,   728,   134,   131,
      34,   176,   129,   482,   129,   103,   104,   129,   104,   104,
     104,   192,   131,   744,   134,    52,   104,   123,   104,   134,
      50,   104,   104,   129,    61,   123,    70,    61,   340,   103,
     211,   131,   106,   133,   369,    72,   110,   132,   134,   208,
     134,    60,   130,     7,   791,    64,   132,    77,   130,   132,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,    16,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   104,
     130,    20,   304,   147,   131,    59,   104,   288,   730,   104,
     131,   550,   293,   592,   550,   104,   104,   161,     3,   131,
      74,   133,    85,   304,    69,   306,   307,   132,   309,   310,
     174,    23,   313,    52,   132,   601,   104,   132,    34,   104,
      25,   104,    61,   132,   132,   108,   131,   328,   192,   469,
     470,    44,   104,   334,   131,   123,   133,    76,   123,   340,
     157,   158,   159,   123,   366,   346,   347,    63,    60,   132,
      85,   123,    64,   354,   171,   356,   104,   358,   359,     2,
     361,   362,    78,   235,   236,   366,    34,    72,    11,    85,
     510,   511,   189,   108,   104,   123,   502,    20,   104,   707,
     104,    85,   199,   711,   104,    60,   123,   123,   104,    64,
      33,   103,   123,   123,   106,    63,   128,   123,   110,   123,
     104,   123,   123,   123,   108,   277,   278,   123,    51,   123,
      78,   275,   276,   129,   131,   701,   288,    85,   129,    79,
       3,   707,   123,   123,     7,   711,    69,   104,   132,    83,
      73,    85,   129,    20,    23,   307,   104,    20,   310,   440,
     441,   442,   443,   444,   445,   446,   123,    34,   447,   448,
      37,   452,   453,   131,   455,   123,   354,   131,   356,   106,
     461,   129,   463,   485,    51,    52,   467,   131,    85,    52,
     334,    60,   289,   123,    61,    64,   340,   499,    61,   122,
       7,   103,    52,   123,   485,    72,   358,   488,   489,    72,
     354,    61,   356,    76,   132,   312,   104,   123,   499,    34,
      26,   132,    72,   104,    34,   132,   792,    83,    85,   132,
      99,   132,   101,   102,   103,   801,   105,   106,   107,   162,
     109,   110,   111,   112,   104,   114,   115,   116,   117,   134,
     119,   129,   121,    63,   129,   124,   125,   126,   132,    85,
     826,   134,    26,   186,    23,   104,   132,   132,    78,   550,
     836,   552,   553,   554,    24,    85,   557,    24,   129,   134,
     103,    61,   134,   132,   106,   208,   134,   131,   569,   441,
      78,   443,   347,   517,   104,   576,   154,   578,   460,   683,
     581,    60,   732,   584,   570,    64,   608,   588,   732,   453,
     733,   455,     3,   123,   590,    19,     7,   805,     9,   129,
     627,   791,   775,   751,   329,    29,    30,   608,    -1,    20,
     455,     2,    36,   614,    38,   616,   550,   618,    -1,   620,
      11,    23,   623,    -1,   103,    -1,   105,   106,    -1,    20,
      -1,   110,    -1,    57,    -1,    -1,   625,    -1,   117,   282,
      -1,    52,    33,    -1,    -1,   124,   125,   126,   465,   466,
      61,    -1,   469,   470,    -1,    -1,    -1,    -1,    60,    -1,
      51,    72,    64,   306,    -1,    76,   309,    -1,    -1,    -1,
     313,   672,   673,    -1,   675,   676,   677,   678,    69,    -1,
     552,   553,    73,    -1,   685,   686,    -1,   688,    -1,   690,
     691,    -1,    -1,   694,    -1,   725,   726,    99,    -1,   101,
     102,   103,    -1,   105,   106,   706,   707,    -1,   110,    -1,
     711,    -1,    -1,   714,   116,   117,   359,   718,    -1,   121,
      -1,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   755,    -1,   736,   737,   738,   739,   740,
     741,   742,   743,    -1,    -1,   562,   563,    -1,    -1,    -1,
      -1,    -1,   753,   754,   755,    -1,   573,    -1,   759,    -1,
      -1,   762,    -1,    -1,    -1,     2,   583,    -1,   769,   586,
      -1,   162,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    20,    -1,    -1,   787,   788,   789,   790,
      29,    30,   435,    -1,    -1,   186,    33,    36,    -1,    38,
     801,    -1,   803,   804,    -1,    44,    -1,    -1,   809,   810,
     811,   812,   813,   814,    51,    -1,    -1,   208,    57,    -1,
      -1,    -1,    -1,    -1,   825,   826,    -1,    -1,    19,    -1,
      -1,   474,    69,    -1,    -1,   836,    73,   701,    29,    30,
      -1,    16,    -1,    -1,    19,    36,    -1,    38,    -1,    -1,
      -1,    -1,    27,    44,    29,    30,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    -1,    -1,    57,    -1,    -1,    -1,
      -1,   514,    -1,    -1,   517,    -1,    -1,    -1,    -1,    54,
      -1,    56,    57,    58,    59,   122,    -1,    -1,    -1,    -1,
      -1,   282,    -1,   710,    -1,   759,     2,    72,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,   552,
     553,    -1,    -1,    -1,    20,   306,    23,    -1,   309,    -1,
      -1,     2,   313,    -1,    -1,   162,    -1,    33,   792,    -1,
      11,    -1,    -1,   576,    -1,   578,    -1,    -1,   581,    20,
      -1,   584,    -1,    -1,   587,    51,    -1,    -1,   123,   186,
      -1,    -1,    33,    60,    -1,   130,    -1,    64,    -1,    -1,
      -1,    -1,   826,    69,   607,    -1,    -1,    73,   359,    -1,
      51,   208,   836,    -1,    -1,   618,    -1,   620,    -1,    -1,
      -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    73,    -1,    -1,    -1,   103,    -1,   105,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,   126,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   685,    25,    -1,   282,    -1,   690,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,   162,    -1,    40,    41,
      -1,    43,    -1,    45,    46,   708,    -1,    -1,    -1,   306,
      -1,    53,   309,    55,   717,    -1,   313,    -1,   721,    -1,
     186,   162,    -1,   474,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      19,    -1,   208,    -1,    -1,   186,    -1,    -1,    27,    -1,
      29,    30,    -1,    -1,    -1,    -1,    -1,    36,   100,    38,
      -1,    -1,   359,   514,    -1,    -1,   517,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,   118,    56,    57,    58,
      59,    -1,    -1,    -1,   787,   788,   789,   790,    -1,   131,
       2,   133,    -1,    72,    -1,    74,    -1,    -1,    -1,    11,
     803,   804,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   282,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,   576,    -1,   578,    -1,    23,
     581,    -1,    -1,   584,    -1,    -1,   587,    -1,    -1,    51,
     306,   282,    -1,   309,   123,    -1,    -1,   313,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,   607,    69,    -1,    -1,
      -1,    73,    -1,    -1,    -1,   306,    60,   618,   309,   620,
      64,    -1,   313,    -1,   625,    -1,    -1,   474,    -1,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   359,    -1,    27,    -1,    29,    30,    -1,
      -1,    -1,    -1,    -1,    36,    99,    38,   101,   102,   103,
     122,   105,   106,    -1,    -1,    -1,   110,   514,   359,    -1,
     517,    -1,    54,   117,    56,    57,    58,    59,    -1,    -1,
     124,   125,   126,    -1,   685,    -1,    -1,    -1,    -1,   690,
      72,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,     2,
     162,    -1,    -1,    -1,    -1,    -1,    -1,   708,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   717,    20,    -1,    -1,
     721,    -1,    -1,    -1,   186,    -1,    -1,    -1,    23,   576,
      33,   578,    -1,    -1,   581,    -1,    -1,   584,    -1,    -1,
     587,   123,    -1,    -1,    -1,    -1,   208,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   474,    -1,
     607,    -1,    -1,    -1,    -1,    60,    69,    -1,    -1,    64,
      73,   618,    -1,   620,    -1,    -1,    -1,    -1,   625,    -1,
      -1,    -1,    -1,   474,    -1,    -1,   787,   788,   789,   790,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   514,    -1,
      -1,   517,   803,   804,    99,    -1,    -1,   102,   103,    -1,
     105,   106,    23,    -1,    -1,   110,    -1,    -1,    -1,   122,
     282,    -1,   117,   514,    -1,    -1,   517,    -1,    -1,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,   685,    -1,
      -1,    -1,    -1,   690,   306,    -1,    -1,   309,    -1,    60,
      -1,   313,    -1,    64,    -1,    -1,    -1,    -1,    -1,   162,
     576,   708,   578,    -1,    -1,   581,    -1,    -1,   584,    -1,
     717,   587,    -1,    -1,   721,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,   576,    -1,   578,    99,    -1,
     581,   607,   103,   584,   105,   106,   587,   359,    -1,   110,
      -1,    -1,   618,    -1,   620,   208,   117,    -1,    -1,   625,
      -1,    -1,    -1,   124,   125,   126,   607,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   618,    -1,   620,
      -1,    -1,    -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,
     787,   788,   789,   790,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,   126,   803,   804,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,   685,
      -1,    -1,    -1,    -1,   690,    -1,    -1,    -1,    -1,   282,
      33,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,   160,
      -1,    -1,   708,    -1,   685,    -1,    -1,    -1,    51,   690,
     171,   717,    -1,   306,    -1,   721,   309,    -1,    -1,    -1,
     313,    -1,   474,    -1,    -1,    -1,    69,   708,   189,    -1,
      73,    -1,    -1,    -1,    -1,    -1,   717,    13,   199,    -1,
     721,    -1,    18,    -1,    -1,    -1,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   514,    -1,    -1,   517,   359,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,   122,
      -1,   787,   788,   789,   790,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   803,   804,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   787,   788,   789,   790,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,   162,
      -1,    -1,   803,   804,   576,    -1,   578,    -1,   289,   581,
      -1,    -1,   584,    -1,    -1,   587,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   312,    -1,    -1,    -1,   607,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   618,   143,   620,    -1,
      -1,   147,    -1,   625,    -1,    -1,    -1,   153,    -1,   155,
      -1,   474,    -1,    -1,    -1,   161,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,   174,    -1,
     176,    -1,    -1,   179,    -1,    -1,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,   192,    -1,    -1,    -1,
      -1,   514,    -1,    -1,   517,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,   685,    -1,    -1,    -1,    -1,   690,   282,
      -1,    -1,    -1,    -1,    -1,   221,    51,    -1,    -1,    -1,
     226,   227,   228,   229,    -1,    -1,   708,    -1,    -1,    -1,
      -1,    -1,    -1,   306,    69,   717,   309,   243,    73,   721,
     313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   576,    -1,   578,    -1,    -1,   581,    -1,
      -1,   584,    -1,    -1,   587,    -1,    -1,    -1,    -1,   275,
     276,    -1,    -1,    -1,   465,   466,    -1,    -1,   469,   470,
      -1,    -1,    -1,    -1,   607,    -1,   359,   122,    -1,    -1,
      -1,    -1,    -1,    -1,   300,   618,   302,   620,    -1,    -1,
      -1,    -1,   625,    -1,    -1,   787,   788,   789,   790,    -1,
      -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,    -1,    -1,
      -1,   803,   804,   329,    -1,    -1,    -1,   162,    -1,    23,
      -1,    -1,    -1,    -1,   340,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   685,   369,    -1,    -1,    60,   690,    -1,    -1,
      64,   562,   563,   208,    -1,    -1,    -1,   127,   128,    -1,
      -1,   131,   573,   133,   575,   708,    -1,    -1,    -1,   580,
      84,    -1,   583,    -1,   717,   586,    -1,    -1,   721,    -1,
      -1,   474,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   514,    -1,    -1,   517,    -1,    -1,   282,    -1,   455,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   787,   788,   789,   790,    -1,    -1,
      -1,   306,   478,    -1,   309,    -1,   482,    -1,    -1,    -1,
     803,   804,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   502,    -1,    -1,    -1,
      -1,    -1,    -1,   576,    -1,   578,    -1,    -1,   581,    -1,
      -1,   584,    -1,    -1,   587,    -1,    -1,    -1,   524,   710,
      -1,    -1,    -1,    -1,   359,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   607,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   618,    -1,   620,    -1,    -1,
      -1,    -1,   625,    -1,    -1,   746,   747,    -1,   749,   750,
      -1,    -1,    -1,    60,   570,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   763,    -1,    -1,    -1,    -1,    -1,     3,   770,
      -1,   772,    -1,    -1,    -1,    10,   592,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   685,   110,   805,    -1,    -1,   690,    -1,   116,
     117,    -1,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,   708,    -1,   643,    -1,   474,
      -1,    -1,    -1,    -1,   717,    -1,    -1,    -1,   721,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    86,    87,    88,    89,    90,    -1,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   514,
     105,    -1,   517,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,    -1,    -1,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   787,   788,   789,   790,   724,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   732,    -1,    -1,    -1,
     803,   804,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   576,    -1,   578,    -1,    -1,   581,    -1,    -1,   584,
      -1,    -1,   587,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   607,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   618,    -1,   620,    -1,    -1,     0,     1,
     625,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,   708,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   717,    -1,    -1,    -1,   721,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,
       1,   133,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,   103,   104,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,     1,   133,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,   122,   123,    -1,    -1,   126,    -1,    -1,   129,
     130,   131,     1,   133,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
     129,   130,   131,     1,   133,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,   129,   130,   131,     1,   133,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,   129,   130,   131,     1,   133,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,   129,   130,   131,     1,   133,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,   129,    -1,   131,     1,   133,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,   129,    -1,   131,     1,   133,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,     1,
     133,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,
       1,   133,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,   129,    -1,
     131,     1,   133,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,   129,
      -1,   131,     1,   133,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
     129,    -1,   131,     1,   133,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,   129,    -1,   131,     1,   133,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,   129,    -1,   131,     1,   133,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,   129,    -1,   131,     1,   133,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,     1,    -1,     3,     4,     5,     6,     7,
       8,   116,    10,   118,    12,   120,   121,    15,   123,    -1,
      -1,   126,    -1,    -1,   129,    23,   131,    25,   133,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,   124,   125,   126,    -1,
      -1,   129,    -1,   131,     1,   133,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    12,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
      -1,    -1,   129,    -1,   131,     1,   133,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
       1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
     116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,
       6,     7,     8,    -1,    -1,   116,    12,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,
     131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
     116,    12,   118,    -1,   120,   121,   122,    -1,    -1,    20,
     126,    -1,    -1,   129,    25,   131,    -1,   133,   134,    -1,
      31,    32,    33,    34,    35,    -1,    37,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,     3,     4,     5,
       6,     7,     8,    -1,    -1,   116,    12,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,    25,
     131,    -1,   133,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,   108,    -1,     3,     4,     5,     6,     7,     8,
     116,    -1,   118,    12,   120,   121,   122,    -1,    -1,    -1,
     126,    -1,    -1,   129,    -1,   131,    25,   133,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,
     129,    25,   131,   132,   133,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,
     129,    25,   131,    -1,   133,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,   133,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
     129,    -1,   131,    -1,   133,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    47,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,     3,     4,     5,     6,     7,
       8,    -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,    25,   131,    -1,
     133,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,   116,    -1,   118,    40,    41,   121,    43,
      -1,    45,    46,   126,    -1,    -1,   129,    10,   131,    53,
     133,    55,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    60,    -1,    -1,
     104,    64,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,   118,    15,    -1,    80,   122,    -1,
      -1,    84,    -1,    23,    -1,   129,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,
      -1,   134,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    10,    -1,    -1,   134,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,    10,
      -1,    -1,   134,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    10,    -1,    -1,   134,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,
      10,   132,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    23,    24,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,   129,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    80,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,   129,    -1,    10,    -1,    -1,    71,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,   129,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,   129,    -1,    -1,    -1,
      -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,   129,
      -1,    -1,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,   129,    -1,    -1,    -1,    -1,    71,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,   129,    -1,    -1,    -1,    -1,
      71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,   129,    -1,
      -1,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,   129,    -1,    -1,    -1,    -1,    71,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,   129,    -1,    -1,    -1,    -1,    71,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,   129,    -1,    10,
      -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
     129,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,   129,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    10,   119,
     120,   121,    -1,    15,   124,   125,   126,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    60,    -1,
      -1,    -1,    64,    -1,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    -1,   124,   125,   126,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      10,   119,   120,   121,    -1,    15,   124,   125,   126,    -1,
      -1,    -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    60,    -1,    -1,    -1,    64,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    84,   119,
     120,   121,    -1,    -1,   124,   125,   126,    -1,    60,    -1,
      -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    84,   119,   120,   121,    -1,   123,   124,   125,
     126,    -1,    -1,    10,    -1,    -1,    -1,    99,    15,   101,
     102,   103,    -1,   105,   106,   107,    23,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    60,   105,   106,
     107,    64,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,   126,
      83,    84,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      -1,   124,   125,   126,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,    -1,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    60,    -1,
      -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    60,   105,   106,   107,    64,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    60,    -1,
      -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,    -1,   124,   125,   126,    15,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,    -1,
      -1,    10,    11,    -1,    71,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    60,   119,   120,   121,    64,    -1,   124,   125,   126,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    60,   124,   125,   126,    64,    -1,
      -1,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,    -1,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,    -1,    -1,    10,    -1,    -1,    71,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    71,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,    60,   119,    -1,   121,    64,    -1,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,    -1,   124,   125,   126
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   139,   140,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    74,    75,    79,    81,    84,    99,   100,
     101,   102,   107,   116,   118,   120,   121,   123,   126,   129,
     131,   133,   141,   142,   143,   144,   145,   146,   149,   150,
     151,   153,   156,   157,   158,   159,   160,   162,   163,   166,
     167,   170,   173,   176,   177,   198,   201,   202,   220,   221,
     222,   223,   224,   225,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   241,   242,   243,   244,   245,   143,
     233,    80,   226,   227,   152,   153,   226,    12,    31,    32,
      33,    70,   129,   133,   173,   220,   224,   231,   232,   233,
     234,   236,   237,    72,   152,   233,   143,   131,   153,     7,
     152,   154,     9,    72,   154,    52,    82,   161,   233,   233,
     233,   131,   153,   174,   131,   153,   203,   204,   143,   233,
     233,   233,   233,     7,   131,    19,    30,   158,   158,   122,
     194,   214,   233,   123,   233,   233,    24,   146,   155,   233,
     233,    69,   131,   143,   233,   153,   199,   214,   233,   233,
     233,   233,   233,   233,   233,   233,   130,   141,   147,   214,
      73,   108,   194,   215,   216,   233,   214,   233,   240,    54,
     143,    44,   153,    38,    57,   190,    20,    52,    61,    76,
     123,   128,    10,    15,    23,    60,    64,    84,    99,   101,
     102,   103,   105,   106,   107,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   119,   120,   121,   124,   125,   126,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   127,   131,   133,    60,    64,   131,
     143,   123,   129,   146,   233,   233,   233,   214,    34,   226,
     199,   123,   123,    79,   153,   194,   217,   218,   219,   233,
     129,   199,   167,   153,   131,   155,    24,    34,   155,    24,
      34,   155,   227,    71,   146,   217,   143,   131,   184,    73,
     131,   153,   205,   206,     1,   103,   208,   209,    34,   104,
     155,   217,   154,   153,   104,   123,   123,   129,   143,   155,
     131,   217,    85,   200,   123,   123,    28,    49,   155,   123,
     130,   141,   130,   233,   104,   132,   104,   132,    34,   134,
     227,    83,   104,   134,     7,   153,   103,   168,   178,    61,
     203,   203,   203,   203,   233,   233,   233,   233,   161,   233,
     161,   233,   233,   233,   233,   233,   233,   233,    25,    72,
     153,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   217,   217,   161,   233,   161,   233,    20,    34,
      37,    51,    52,    61,    72,   188,   228,   229,   233,   245,
      24,    34,    24,    34,    71,    34,   134,   161,   233,   155,
     123,   233,    83,    85,   132,   104,   153,   171,   172,   123,
     153,    34,   214,    33,   233,   161,   233,    33,   233,   161,
     233,   155,   143,   233,    26,   132,   185,   186,   187,   188,
     175,   206,   104,   132,     1,   133,   210,   221,    83,    85,
     207,   233,   204,   132,   194,   233,   164,   217,   132,   133,
     210,   221,   104,   148,   233,   148,   132,   132,   215,   215,
     161,   233,   143,   233,   134,   233,   233,   129,   214,   129,
      52,    61,    72,   180,   189,   203,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   132,   134,    34,    61,   220,
     104,   132,    60,    60,    33,   161,   233,    33,   161,   233,
     161,   233,   226,   226,   123,   233,   194,   233,   219,    85,
     104,   130,   168,   233,   132,   233,    24,   155,    24,   155,
     233,    24,   155,   227,    24,   155,   227,    26,    26,   143,
     104,   132,   131,   153,    20,    52,    61,    72,   191,   132,
     206,   103,   209,   214,   233,    47,   233,    50,    77,   130,
     165,   132,   214,   199,   104,   123,    11,   123,   134,   227,
     134,   227,   143,    83,   130,   147,   169,   179,    10,    15,
      85,    86,    87,    88,    89,    90,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   105,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   121,   124,
     125,   126,   127,   128,   153,   181,   182,   123,   229,   220,
     220,   233,    24,    24,   233,    24,    24,   134,   134,   155,
     155,   233,   172,   129,   155,    71,    33,   233,    33,   233,
      71,    33,   233,   155,    33,   233,   155,   143,   233,   186,
     206,   103,   213,    61,   209,   207,    34,   134,    24,   143,
     214,   134,   233,   233,    33,   143,   233,   134,    33,   143,
     233,   134,   233,   130,    30,    56,    58,   130,   142,   157,
     191,   153,   106,   131,   183,   183,    71,    33,    33,    71,
      33,    33,    33,    33,   169,   233,   233,   233,   233,   233,
     233,   132,    25,    67,    70,   133,   194,   212,   221,   108,
     196,   207,    78,   197,   233,   210,   221,   143,   155,    11,
     233,   143,   233,   143,   157,   209,   153,   181,   182,   185,
     233,   233,   233,   233,   233,   233,   130,    71,    71,    71,
      71,   213,   134,   214,   194,   195,   233,   233,   146,   156,
     193,   134,   233,    71,    71,   197,   183,   183,   132,    71,
      71,    71,    71,    71,    71,   233,   233,   233,   233,   196,
     207,   194,   211,   212,   221,    34,   134,   221,   233,   233,
     123,   192,   193,   233,   211,   212,   134,   211
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   138,   139,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   144,   144,   145,   145,   145,
     146,   146,   147,   147,   148,   148,   148,   148,   149,   149,
     149,   150,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   152,   152,   153,
     154,   154,   155,   155,   156,   156,   157,   157,   157,   157,
     157,   157,   157,   158,   158,   159,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   161,   162,
     162,   162,   162,   163,   164,   164,   165,   165,   165,   166,
     166,   167,   167,   167,   168,   168,   169,   169,   169,   170,
     171,   171,   171,   172,   172,   174,   175,   173,   176,   176,
     176,   176,   178,   179,   177,   180,   180,   180,   180,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   183,   183,   184,
     185,   185,   185,   186,   186,   186,   186,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   189,   189,
     189,   189,   190,   190,   191,   191,   191,   191,   191,   191,
     192,   192,   193,   193,   194,   194,   195,   195,   196,   196,
     197,   197,   198,   198,   198,   199,   199,   200,   200,   200,
     201,   201,   201,   201,   201,   202,   202,   202,   203,   203,
     204,   204,   204,   205,   205,   205,   206,   206,   206,   207,
     207,   207,   208,   208,   209,   209,   209,   209,   210,   210,
     210,   210,   211,   211,   211,   212,   212,   212,   212,   212,
     213,   213,   213,   213,   213,   213,   213,   214,   214,   214,
     214,   215,   215,   215,   216,   216,   217,   217,   218,   218,
     219,   219,   219,   219,   219,   220,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   223,   224,   225,   225,   225,
     225,   225,   225,   225,   225,   226,   226,   227,   228,   228,
     229,   229,   229,   230,   230,   231,   232,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   234,   234,   234,   234,   235,   235,   236,   236,   236,
     237,   237,   237,   238,   238,   238,   239,   239,   239,   239,
     239,   239,   239,   239,   240,   240,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   242,
     242,   242,   242,   242,   242,   243,   243,   243,   243,   244,
     244,   244,   244,   245,   245,   245,   245,   245,   245,   245
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     1,     3,     3,     5,     3,     5,
       5,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     0,     1,     1,
       1,     1,     2,     1,     2,     3,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     2,     5,     3,     6,     6,
       4,     5,     5,     3,     3,     6,     5,     6,     5,     6,
       3,     4,     6,     7,     6,     7,     4,     5,     4,     4,
       3,     6,     5,     5,     0,     2,     3,     2,     3,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       1,     2,     3,     1,     3,     0,     0,     8,     0,     1,
       2,     2,     0,     0,     9,     3,     3,     5,     5,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       0,     1,     3,     4,     4,     6,     6,     0,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     0,     1,     2,     2,
       0,     2,     3,     4,     4,     2,     4,     0,     2,     2,
       4,     4,     4,     5,     4,     0,     1,     1,     1,     3,
       3,     4,     5,     1,     1,     3,     1,     2,     3,     0,
       2,     2,     0,     4,     0,     2,     2,     1,     4,     4,
       6,     1,     0,     1,     1,     3,     4,     3,     4,     6,
       0,     2,     2,     2,     2,     2,     2,     1,     1,     3,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     4,     1,     3,
       2,     3,     3,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     4,     3,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 417 "chapel.ypp" /* yacc.c:1646  */
    { yyblock = (yyval.pblockstmt); }
#line 4507 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 422 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4513 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 423 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4519 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 430 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4525 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 435 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4531 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 436 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4537 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 452 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4543 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 453 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4549 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 454 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4555 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 455 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4561 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 456 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4567 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 457 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4573 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 458 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4579 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 459 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4585 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 460 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4591 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 461 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4597 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 462 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4603 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 463 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4609 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 464 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4615 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 465 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4621 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 466 "chapel.ypp" /* yacc.c:1646  */
    { printf("syntax error"); clean_exit(1); }
#line 4627 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 471 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4633 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 473 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4639 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 477 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4645 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 478 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4651 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 479 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4657 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 494 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4663 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 495 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4669 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 500 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4675 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 501 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4681 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 506 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4691 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 511 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4701 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 516 "chapel.ypp" /* yacc.c:1646  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4710 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 520 "chapel.ypp" /* yacc.c:1646  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4719 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 526 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4725 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 527 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 4731 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 528 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 4737 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 532 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 4743 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 536 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4749 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 537 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4755 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 538 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4761 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 539 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4767 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 540 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4773 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 541 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4779 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 542 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4785 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 543 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4791 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 544 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4797 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 545 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4803 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 546 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4809 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 547 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4815 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 548 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4821 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 549 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4827 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 550 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4833 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 554 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = NULL; }
#line 4839 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 559 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4845 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 563 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4851 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 564 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4857 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 568 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4863 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 569 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4869 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 573 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4875 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 574 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4881 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 578 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4887 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 580 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4893 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 581 "chapel.ypp" /* yacc.c:1646  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4899 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 594 "chapel.ypp" /* yacc.c:1646  */
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
#line 4915 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 608 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 4921 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 609 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4927 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 610 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4933 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 611 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 4939 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 612 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4945 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 613 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4951 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 614 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4957 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 615 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4963 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 616 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4969 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 617 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4975 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 618 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4981 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 619 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4987 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 620 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 4993 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 621 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 4999 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 622 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5005 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 623 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5011 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 625 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5021 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 631 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5031 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 637 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5041 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 643 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5051 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 649 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5062 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 656 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5073 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 665 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[-1].pcallexpr)); }
#line 5079 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 669 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5085 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 670 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5091 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 671 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5097 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 672 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5103 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 677 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5109 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 681 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5115 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 682 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5121 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 687 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5127 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 689 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5133 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 691 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5139 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 698 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5148 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 703 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5157 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 710 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5167 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 715 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5177 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 720 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5187 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 728 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = NULL; }
#line 5193 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 729 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5199 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 734 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5205 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 736 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5211 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 738 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5217 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 743 "chapel.ypp" /* yacc.c:1646  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5228 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 753 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5241 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 762 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5249 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 766 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5258 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 773 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5264 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 774 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5270 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 779 "chapel.ypp" /* yacc.c:1646  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5279 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 784 "chapel.ypp" /* yacc.c:1646  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5288 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 789 "chapel.ypp" /* yacc.c:1646  */
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
#line 5308 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 809 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5319 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 815 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5331 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 822 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5344 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 830 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5357 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 841 "chapel.ypp" /* yacc.c:1646  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5367 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 847 "chapel.ypp" /* yacc.c:1646  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5378 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 854 "chapel.ypp" /* yacc.c:1646  */
    {
      FnSymbol* fn = (yyvsp[-5].pfnsymbol);

      fn->copyFlags((yyvsp[-8].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[-8].pfnsymbol)->name)
        fn->cname = (yyvsp[-8].pfnsymbol)->name;

      if ((yyvsp[-7].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[-5].pfnsymbol), (yyvsp[-3].retTag), (yyvsp[-2].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), (yylsp[-8]).comment);
      context->latestComment = NULL;
    }
#line 5401 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 876 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5409 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 880 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5418 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 885 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5426 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 889 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5435 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 897 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5441 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 898 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "&"; }
#line 5447 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 899 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "|"; }
#line 5453 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 900 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "^"; }
#line 5459 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 901 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "~"; }
#line 5465 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 902 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "=="; }
#line 5471 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 903 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "!="; }
#line 5477 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 904 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<="; }
#line 5483 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 905 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">="; }
#line 5489 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 906 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<"; }
#line 5495 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 907 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">"; }
#line 5501 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 908 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "+"; }
#line 5507 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 909 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "-"; }
#line 5513 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 910 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "*"; }
#line 5519 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 911 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "/"; }
#line 5525 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 912 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<"; }
#line 5531 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 913 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>"; }
#line 5537 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 914 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "%"; }
#line 5543 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 915 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "**"; }
#line 5549 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 916 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "!"; }
#line 5555 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 917 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "chpl_by"; }
#line 5561 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 918 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "#"; }
#line 5567 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 919 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "chpl_align"; }
#line 5573 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 920 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<=>"; }
#line 5579 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 921 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<~>"; }
#line 5585 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 925 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "="; }
#line 5591 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 926 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "+="; }
#line 5597 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 927 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "-="; }
#line 5603 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 928 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "*="; }
#line 5609 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 929 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "/="; }
#line 5615 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 930 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "%="; }
#line 5621 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 931 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "**="; }
#line 5627 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 932 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "&="; }
#line 5633 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 933 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "|="; }
#line 5639 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 934 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "^="; }
#line 5645 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 935 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>="; }
#line 5651 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 936 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<="; }
#line 5657 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 940 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5663 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 941 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5669 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 945 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5675 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 949 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5681 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 950 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5687 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 951 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5693 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 956 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5699 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 958 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5705 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 960 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5711 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 962 "chapel.ypp" /* yacc.c:1646  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5717 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 966 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5723 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 967 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 5729 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 971 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_IN; }
#line 5735 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 972 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5741 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 973 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_OUT; }
#line 5747 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 974 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST; }
#line 5753 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 975 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5759 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 976 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5765 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 977 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5771 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 978 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_REF; }
#line 5777 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 979 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5783 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 983 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5789 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 984 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5795 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 985 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_REF;   }
#line 5801 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 986 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 5807 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 990 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5813 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 991 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5819 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 995 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_VALUE; }
#line 5825 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 996 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_VALUE; }
#line 5831 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 997 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 5837 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 998 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_REF; }
#line 5843 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 999 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_PARAM; }
#line 5849 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1000 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_TYPE; }
#line 5855 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1004 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = NULL; }
#line 5861 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1010 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5867 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1015 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5873 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1017 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5879 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1021 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5885 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1026 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5891 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1027 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5897 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1031 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 5903 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1032 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5909 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1037 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 5915 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1039 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 5921 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1041 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 5927 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1046 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 5944 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1059 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 5962 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1075 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 5968 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1077 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5974 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1079 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 5980 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1084 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 5992 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1092 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6004 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1100 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6016 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1108 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6029 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1117 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6040 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1126 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6046 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1127 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6052 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1128 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6058 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1134 "chapel.ypp" /* yacc.c:1646  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6067 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1142 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6073 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1144 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6083 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1150 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6089 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1155 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6095 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1157 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6101 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1159 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6107 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1164 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6113 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1166 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6119 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1168 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6125 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1174 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6131 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1175 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6137 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1176 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6143 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1180 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6149 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1182 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6155 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1186 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6161 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1187 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6167 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1188 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6173 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1189 "chapel.ypp" /* yacc.c:1646  */
    {printf("bad type specification"); }
#line 6179 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1210 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6187 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1214 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6195 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1218 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6207 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1225 "chapel.ypp" /* yacc.c:1646  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6213 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1229 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6219 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1230 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6225 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1231 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6231 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1236 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6237 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1238 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6243 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1244 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6249 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1246 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6255 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1248 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6261 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1252 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6267 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1253 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6273 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1254 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6279 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1255 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6285 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1256 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6291 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1257 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6297 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1258 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6303 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1264 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6309 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1265 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6315 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 1266 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6321 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 1267 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6327 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 1271 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6333 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1272 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6339 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 1273 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6345 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 1277 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6351 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 1278 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6357 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 1282 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6363 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 1287 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6369 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 1288 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6375 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 1292 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6381 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 1293 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6387 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 1294 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6393 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 1295 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6399 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 1300 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6405 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 1317 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6411 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1319 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6417 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 1321 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6423 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1323 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6429 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 1325 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6435 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1327 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6441 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1329 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6447 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1334 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6453 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 1336 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6459 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 1338 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6465 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 1340 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6471 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 1342 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6477 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 1344 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6483 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 1346 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6489 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1348 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6495 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1350 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6501 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 1352 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6507 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 1354 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6513 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1356 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6519 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1358 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6530 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1365 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6540 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1371 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6550 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1377 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6560 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 1383 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6570 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 1392 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6576 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 1396 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6582 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 1412 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6588 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 1416 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = NULL; }
#line 6594 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 1421 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6600 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 1426 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6606 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 1428 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6615 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 1436 "chapel.ypp" /* yacc.c:1646  */
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[-1].pt));
      if (!tiMark) {
        USR_FATAL_CONT((yyvsp[0].pexpr), "%s is not supported in a 'with' clause",
                           intentDescrString((yyvsp[-1].pt)));
        tiMark = tiMarkForIntent(INTENT_IN); //dummy, so parser can continue
      }
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[0].pexpr);
    }
#line 6630 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 1446 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6636 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 1447 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6642 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 1453 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6648 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 1458 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6654 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 1463 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6660 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 1479 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6666 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 1481 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6672 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 1483 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6678 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 1485 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6684 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 1487 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 6690 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 1489 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 6696 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 1512 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6702 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 1513 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6708 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 1514 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6714 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 1518 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6720 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 1519 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6726 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 1520 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6732 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 1528 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6738 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 384:
#line 1529 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6744 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 1530 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6750 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 386:
#line 1534 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6756 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 1535 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6762 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 1536 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6768 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 1537 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6774 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 1538 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6780 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 1539 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6786 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 1540 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6792 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 1542 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6800 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 1548 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6806 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 1549 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6812 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 1553 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6818 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 1554 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6824 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 1555 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6830 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 1556 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6836 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 1557 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6842 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 1558 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6848 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 1559 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6854 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 1560 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6860 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 1561 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6866 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 1562 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6872 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 1563 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6878 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 1564 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6884 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 1565 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6890 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 1566 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6896 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 1567 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6902 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 1568 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6908 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 1569 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6914 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 1570 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6920 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 1571 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6926 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 1572 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6932 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 1573 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6938 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 1574 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6944 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 1575 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6950 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 419:
#line 1579 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 6956 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 1580 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 6962 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 1581 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 6968 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 1582 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 6974 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 1583 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 6980 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 1584 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 6986 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 1588 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6992 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 1589 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6998 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 1590 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7004 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 1591 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7010 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 1595 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7016 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 1596 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7022 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 1597 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7028 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 1598 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7034 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 1603 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7040 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 1604 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7046 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 1605 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7052 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 1606 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7058 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 1607 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7064 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 1608 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7070 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 1609 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7076 "bison-chapel.cpp" /* yacc.c:1646  */
    break;


#line 7080 "bison-chapel.cpp" /* yacc.c:1646  */
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
