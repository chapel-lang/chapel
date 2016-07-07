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
#define YYLAST   11590

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  108
/* YYNRULES -- Number of rules.  */
#define YYNRULES  438
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  836

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
     670,   671,   672,   676,   681,   682,   686,   688,   695,   700,
     708,   713,   718,   726,   727,   732,   733,   735,   740,   750,
     759,   763,   771,   772,   777,   782,   776,   807,   813,   820,
     828,   839,   845,   838,   873,   877,   882,   886,   894,   895,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,   908,   909,   910,   911,   912,   913,   914,   915,
     916,   917,   918,   919,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   932,   933,   934,   938,   939,   943,   947,
     948,   949,   953,   955,   957,   959,   964,   965,   969,   970,
     971,   972,   973,   974,   975,   976,   977,   981,   982,   983,
     984,   988,   989,   993,   994,   995,   996,   997,   998,  1002,
    1003,  1007,  1008,  1012,  1014,  1019,  1020,  1024,  1025,  1029,
    1030,  1034,  1036,  1038,  1043,  1056,  1073,  1074,  1076,  1081,
    1089,  1097,  1105,  1114,  1124,  1125,  1126,  1130,  1131,  1139,
    1141,  1147,  1152,  1154,  1156,  1161,  1163,  1165,  1172,  1173,
    1174,  1178,  1179,  1184,  1185,  1186,  1187,  1207,  1211,  1215,
    1223,  1227,  1228,  1229,  1233,  1235,  1241,  1243,  1245,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,  1262,  1263,  1264,  1265,
    1269,  1270,  1271,  1275,  1276,  1280,  1281,  1285,  1286,  1290,
    1291,  1292,  1293,  1294,  1298,  1309,  1310,  1311,  1312,  1313,
    1314,  1316,  1318,  1320,  1322,  1324,  1326,  1331,  1333,  1335,
    1337,  1339,  1341,  1343,  1345,  1347,  1349,  1351,  1353,  1355,
    1362,  1368,  1374,  1380,  1389,  1394,  1402,  1403,  1404,  1405,
    1406,  1407,  1408,  1409,  1414,  1415,  1419,  1423,  1425,  1433,
    1444,  1445,  1449,  1450,  1455,  1460,  1468,  1469,  1470,  1471,
    1472,  1473,  1474,  1475,  1476,  1478,  1480,  1482,  1484,  1486,
    1491,  1492,  1493,  1494,  1505,  1506,  1510,  1511,  1512,  1516,
    1517,  1518,  1526,  1527,  1528,  1532,  1533,  1534,  1535,  1536,
    1537,  1538,  1539,  1546,  1547,  1551,  1552,  1553,  1554,  1555,
    1556,  1557,  1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,
    1566,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1577,  1578,
    1579,  1580,  1581,  1582,  1586,  1587,  1588,  1589,  1593,  1594,
    1595,  1596,  1601,  1602,  1603,  1604,  1605,  1606,  1607
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

#define YYPACT_NINF -745

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-745)))

#define YYTABLE_NINF -392

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -745,    81,  2457,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  3567,     9,   103,  -745,     9,  6587,   123,   103,  6587,
    3567,    71,   103,   356,   851,  5511,  6587,  6587,    88,  -745,
     103,  -745,    17,  3567,  6587,  6587,  -745,  6587,  6587,   279,
     178,   795,   832,  -745,  5721,  5826,  6587,  5957,  6587,   243,
     183,  3567,   103,  -745,  5721,  6587,  6587,  -745,  -745,  6587,
    -745,  -745,  7532,  6587,  6587,  -745,  6587,  -745,  -745,  2781,
    5299,  5721,  -745,  3436,  -745,  -745,   281,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
     103,  -745,   244,   154,  -745,  -745,  -745,    77,   229,  -745,
    -745,  -745,   256,   265,   275,   270,   271, 11290,  1629,   -47,
     290,   291,  -745,  -745,  -745,  -745,  -745,  -745,   321,  -745,
   11290,   284,  3567,  -745,   294,  -745,   289,  6587,  6587,  6587,
    6587,  6587,  5721,  5721,    70,  -745,  -745,  -745,  -745,  8903,
     278,  -745,  -745,   103,   296,  9197,   341,  5721,   292,  -745,
    -745,  -745,  -745,   103,   131,   103,   295,    30,  7978,  7915,
    8099,  5721,  3567,   306,    35,    90,     5,  -745,  -745,   363,
     339,  8157,   363,  -745,  5721,  -745,   267,  -745,  -745,   103,
    -745,   169, 11290,  -745,  9231,  8782,  3567,  -745,  -745,  8157,
   11290,   315,  5721,  -745, 11290,   362,   325,   185,  9011,  8157,
    9316,   376,  1079,   363,   376,   363,  -745,  -745,  2912,   -34,
    -745,  6587,  -745,   -29,    75, 11290,   257,  9360,    27,   442,
    -745,   103,   352,  -745,  -745,  -745,    15,    17,    17,    17,
    -745,  6587,  6587,  6587,  6587,  6062,  6062,  6587,  6587,  6587,
    6587,  6587,  6587,    68,  7532,  6587,  6587,  6587,  6587,  6587,
    6587,  6587,  6587,  6587,  6587,  6587,  6587,  6587,  6587,  6587,
    6587,  6587,  6587,  6587,  6587,  6587,  6587,  6587,  6587,  6587,
    6587,  6587,  6587,  6587,  6587,  5721,  5721,  6062,  6062,  5194,
    -745,  -745,  3043,  -745,  9055,  9163,  9435,     7,  6062,    30,
     335,  -745,  -745,  6587,    52,  -745,   331,   366,  -745, 11290,
     103,   344,   103,   437,  5721,  -745,  3698,  6062,  -745,  3829,
    6062,  -745,    30,  3567,   446,   343,  -745,    53,  -745,  -745,
      35,  -745,   379,   360,  -745,  2310,   410,   399,  6587,    17,
    -745,   364,  -745,  -745,  5721,  -745,  -745,  -745,  -745,  -745,
    5721,   365,  4984,   390,  -745,  -745,  6587,  6587,  -745,  -745,
    -745,  -745,  -745,  7857,  5406,  -745,  5616,  -745,  6062,  2648,
     361,  6587,  6587,  -745,  -745,   369,  5721,   372,   254,    17,
     193,   199,   216,   253,  8845, 11406, 11406,   302,  -745,   302,
    -745,   302,  2144,  1321,   970,  1138,   339,   376,  -745,  -745,
    -745,  1079,  1365,   302,   401,   401, 11406,   401,   401,   489,
     376,  1365, 11464,   489,   363,   363,   376,  9479,  9547,  9591,
    9659,  9703,  9771,  9815,  9883,  9927,  9995, 10039, 10107, 10151,
   10219, 10263,   373,   382,  -745,   302,  -745,   302,    94,  -745,
    -745,  -745,  -745,  -745,  -745,   103,    82,  -745, 11348,   340,
    6692,  6062,  6797,  6062,  6587,  6062,  1101,     9, 10331,  -745,
    -745, 10375,  6587,  5721,  -745,  5721,   425,   102,  -745,  -745,
     352,  6587,    84,  6587, 11290,    45,  8220,  6587, 11290,    33,
    8036,  -745,   487, 10443,  3567,  -745,    85,  -745,    25,  -745,
     258,   387,    35,   184,  -745,  5721,  -745,   293,  6587,  6167,
    -745, 11290,  -745,  -745,  -745, 11290,    50,   388,  -745,  5721,
    -745,   261,   103,   264, 10501,   266,  -745,  -745,  -745,  -745,
     -33,   862,  -745, 11290,  3567, 11290, 10564,  3174,   417,  -745,
    -745,  -745,  -745,  -745,  7667,   269,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    5194,  -745,  6062,  6062,  6587,   506, 10622,  6587,   507, 10685,
     398,  1874,    30,    30,  -745, 11290,  -745, 11290,  -745,  6587,
     103,  -745,   404,  8157,  -745,  8283,  3960,  -745,  4091,  -745,
    8346,  4222,  -745,    30,  4353,  -745,    30,  3567,  6587,  -745,
     582,  -745,    35,   432,   477,  -745,  -745,  -745,    20,  -745,
    -745,  4984,   399,    38, 11290,  -745, 11290,  3567,  5721,  -745,
    -745,  -745,    65,  -745,  6587,  -745,  6587,  -745,  4484,   406,
    4615,   411,  -745,  6587,  -745,  3305,   791,   258,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,   103,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,   438,   416,   416,  -745,  -745,   168,
     172, 10743,  6902,  7007, 10806,  7112,  7217,  7322,  7427,  -745,
    -745, 11290,  -745,  -745,  -745,  3567,  6587, 11290,  6587, 11290,
    3567,  6587, 11290,  -745,  6587, 11290,  -745,  -745, 11290,  -745,
     418,  6272,    -6,  -745,   470,  -745,  6062,  4722,  -745,    41,
    4853, 10864, 11290,  6587,  -745, 11290,  3567,  6587,  -745, 11290,
    3567, 11290,  -745,   549,   424,   424,  -745,  1249,  -745,    29,
    -745,  7667,    53,  -745,  -745,  6587,  6587,  6587,  6587,  6587,
    6587,  6587,  6587,  1025, 10443,  8409,  8472, 10443,  8535,  8598,
     432,    71,  6587,  6587,  5089,  -745,  -745,    83,  5721,  -745,
    -745,  6587,   -14,  7736,  -745,   451,  -745,  6587,  8661,  -745,
    8724,  -745,  -745,   470,  -745,   416,   416,   181, 10927, 10985,
   11048, 11106, 11169, 11227,  -745,  3567,  3567,  3567,  3567,    -6,
    6377,    78,  -745,  -745, 11290, 11290,  -745,  -745,  -745,  7427,
   11290,  3567,  3567,    31,  -745,  -745,  -745,  6587,  6587,  6587,
    6587,  6587,  6587, 10443, 10443, 10443, 10443,  -745,  -745,  -745,
    -745,  -745,   186,  6062,  7611,   494, 10443, 10443,  -745,  -745,
    -745,  7794,  -745,  -745,  6482,  -745
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    69,   385,   386,   387,   388,
     389,     0,   344,    67,   120,   344,     0,   245,    67,     0,
       0,     0,     0,    67,    67,     0,     0,     0,     0,   138,
       0,   134,     0,     0,     0,     0,   335,     0,     0,     0,
       0,   244,   244,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,     0,     0,     0,   434,   436,     0,
     437,   438,   369,     0,     0,   435,   432,    76,   433,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   304,    19,    13,    77,    15,    17,    16,    18,    82,
       0,    80,   361,     0,    83,    81,    84,     0,   370,   357,
     358,   307,   305,     0,     0,   362,   363,     0,   306,     0,
     371,   372,   373,   356,   309,   308,   359,   360,     0,    21,
     315,     0,     0,   345,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,   361,   370,   305,   362,   363,   344,
     306,   371,   372,     0,     0,     0,     0,   295,     0,    71,
      70,   139,    85,     0,   140,     0,     0,     0,     0,     0,
       0,   295,     0,     0,     0,     0,     0,   247,    28,   420,
     354,     0,   421,     7,   295,   245,   246,    79,    78,   224,
     287,     0,   286,    74,     0,     0,     0,    73,    31,     0,
     310,     0,   295,    32,   316,   236,     0,     0,   286,     0,
       0,   423,   368,   419,   422,   418,    40,    42,     0,     0,
     290,     0,   292,     0,     0,   291,     0,   286,     0,     0,
       6,     0,   123,   212,   211,   141,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   295,   295,     0,     0,     0,
      22,    23,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,   304,   302,     0,   296,   297,   303,
       0,     0,     0,     0,     0,    94,     0,     0,    93,     0,
       0,   100,     0,     0,   110,     0,    27,   189,   135,   252,
       0,   253,   255,     0,   266,     0,     0,   258,     0,     0,
      29,     0,   140,   223,     0,    51,    75,   114,    72,    30,
     295,     0,     0,   234,   231,    48,     0,     0,    87,    33,
      41,    43,   390,     0,     0,   382,     0,   384,     0,     0,
       0,     0,     0,   392,     8,     0,     0,     0,   207,     0,
       0,     0,     0,     0,   343,   415,   414,   417,   425,   424,
     429,   428,   411,   408,   409,   410,   365,   398,   381,   380,
     379,   366,   402,   413,   407,   405,   416,   406,   404,   396,
     401,   403,   412,   395,   399,   400,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   427,   426,   431,   430,   201,   198,
     199,   200,   204,   205,   206,     0,     0,   347,     0,     0,
       0,     0,     0,     0,     0,     0,   391,   344,   344,    90,
     232,     0,     0,     0,   312,     0,   132,     0,   129,   233,
     123,     0,     0,     0,   319,     0,     0,     0,   325,     0,
       0,   101,   109,     0,     0,   311,     0,   190,     0,   197,
     213,     0,   256,     0,   270,     0,   265,   357,     0,     0,
     249,   355,   248,   378,   289,   288,     0,     0,   313,     0,
     238,   357,     0,     0,    44,     0,   364,   383,   293,   294,
       0,     0,   106,   329,     0,   393,     0,     0,   124,   125,
     208,   209,   210,   142,     0,     0,   240,   239,   241,   243,
      52,    59,    60,    61,    56,    58,    65,    66,    54,    57,
      55,    53,    63,    62,    64,   376,   377,   202,   203,   349,
       0,   346,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,   301,   299,   300,   298,     0,
     130,   128,     0,     0,   108,     0,     0,    92,     0,    91,
       0,     0,    98,     0,     0,    96,     0,     0,     0,   112,
     196,   188,     0,   279,   214,   217,   216,   218,     0,   254,
     257,     0,   258,     0,   250,   259,   260,     0,     0,   113,
     115,   314,     0,   235,     0,    49,     0,    50,     0,     0,
       0,     0,   107,     0,    35,     0,   244,   213,   171,   169,
     174,   181,   182,   183,   178,   180,   176,   179,   177,   175,
     185,   184,   150,   153,   151,   152,   163,   154,   167,   159,
     157,   170,   158,   156,   161,   166,   168,   155,   160,   164,
     165,   162,   172,   173,   148,   186,   186,   242,   348,   370,
     370,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      88,   133,   131,   125,    95,     0,     0,   318,     0,   317,
       0,     0,   324,    99,     0,   323,    97,   111,   334,   191,
       0,     0,   258,   215,   229,   251,     0,     0,   117,     0,
       0,    46,    45,     0,   104,   331,     0,     0,   102,   330,
       0,   394,    36,    67,   244,   244,   118,   244,   126,     0,
     149,     0,   189,   144,   145,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   322,     0,     0,   328,     0,     0,
     279,   282,   283,   284,     0,   281,   285,   357,   225,   193,
     192,     0,     0,     0,   268,   357,   116,     0,     0,   105,
       0,   103,   127,   229,   148,   186,   186,     0,     0,     0,
       0,     0,     0,     0,   119,     0,     0,     0,     0,   258,
     271,     0,   226,   228,   227,   230,   221,   222,   136,     0,
      47,     0,     0,     0,   146,   147,   187,     0,     0,     0,
       0,     0,     0,   321,   320,   327,   326,   195,   194,   273,
     274,   276,   357,     0,   391,   357,   333,   332,   219,   143,
     220,     0,   275,   277,   271,   278
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -745,  -745,  -745,     1,  -604,  1573,  -745,  -745,  1652,    34,
     207,  -745,  -745,  -745,    64,  2052,   -12,   175,  -677,  -560,
     -40,  -745,  -745,   118,  -745,  -745,  -745,  -745,  -745,   408,
     100,  -120,  -745,  -745,    -4,   649,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -167,  -163,  -599,  -745,  -161,   -15,  -745,
    -267,  -745,  -745,   -51,  -745,  -225,     4,  -745,  -208,  -191,
    -745,  -136,  -745,  -745,  -745,  -165,   255,  -745,  -311,  -592,
    -745,  -454,  -338,  -513,  -744,  -164,    24,  -146,  -745,  -142,
    -745,   128,   329,  -300,  -745,  -745,   712,  -745,    -9,  -128,
    -745,    37,  -745,   733,   989,   -11,  1322,  -745,  1343,  1572,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -266
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   472,    75,    76,    77,   208,
     503,    78,    79,    80,   150,    81,   154,   188,    82,    83,
      84,    85,    86,   560,    87,    88,   496,   610,    89,    90,
     367,   626,    91,   457,   458,   134,   163,   480,    93,    94,
     368,   627,   523,   665,   666,   733,   318,   476,   477,   478,
     479,   524,   225,   598,   829,   798,   180,   793,   759,   762,
      95,   196,   343,    96,    97,   166,   167,   322,   323,   490,
     326,   327,   486,   820,   756,   702,   209,   213,   214,   296,
     297,   298,   135,    99,   100,   101,   136,   103,   122,   123,
     436,   437,   104,   137,   138,   107,   140,   109,   141,   142,
     112,   113,   218,   114,   115,   116,   117,   118
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     120,   177,   178,    72,   500,   139,   126,   290,   145,   481,
     705,   151,   435,   439,   158,   159,   160,   301,     5,   315,
       5,   324,   727,   169,   170,   487,   171,   172,     5,   602,
     324,   312,   331,   182,   184,   185,   189,   190,     5,   328,
     194,   445,   501,   198,   199,   200,   821,   121,   201,    45,
     341,   202,   203,   204,   186,   205,  -196,   581,   182,   215,
     217,   370,   371,   372,   373,   186,   728,   734,   181,   576,
     334,     5,   706,   428,   212,   354,   369,   124,   197,   489,
     833,     3,   144,  -263,   275,   797,   276,   429,   360,   121,
     430,   324,  -263,   388,    45,   216,   352,   226,  -263,   706,
     607,   618,   758,   355,   431,   432,     5,  -263,   319,   329,
     760,   334,   823,   121,   433,   282,   120,   284,   285,   286,
     194,   182,   217,   601,  -263,   434,   797,   608,   547,   227,
     289,   362,   601,   422,   423,   452,   299,   453,   228,   727,
     389,   446,   334,   157,   704,   334,   164,    14,   164,  -263,
     299,   295,  -263,   229,   828,   548,   592,   287,  -263,   282,
     282,   363,   282,   299,   332,   295,   320,   772,  -280,   334,
     282,   600,   707,  -261,   282,  -263,   804,   805,   295,   356,
     609,   299,   334,   728,  -196,   324,   550,  -280,   334,   590,
      43,  -280,   223,   325,  -263,   143,   295,   818,   497,   710,
     353,  -375,   147,  -375,   525,    53,   570,   357,   508,   351,
     509,   224,   824,  -263,   551,  -280,   574,   591,  -263,   161,
     374,   375,   376,   377,   379,   381,   382,   383,   384,   385,
     386,   387,   571,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   299,   299,   425,   427,   438,  -263,
       5,  -272,  -350,   334,   149,   773,  -351,   448,   594,   295,
     295,   700,   451,   435,   439,   590,   173,   601,  -263,   334,
    -272,   358,   335,   182,  -272,   464,   466,   329,   468,   470,
    -350,   487,   473,   329,  -351,   -67,   520,  -263,   345,   174,
     595,   832,   191,   806,   192,   521,   526,   491,  -272,   596,
     329,   835,   527,   495,   -67,   221,   522,  -264,   462,   299,
     597,    98,   305,   308,   311,   504,   504,   121,   494,   528,
      98,   583,   586,   215,   295,   215,   330,   511,   513,    98,
     515,   516,  -337,   378,   380,   182,  -264,   329,   212,     5,
     212,   334,    98,   149,   339,  -237,   613,  -340,   614,   764,
     614,  -264,   764,   329,   348,  -375,   529,  -375,  -264,  -336,
      98,   277,   619,   621,  -237,   278,   234,   615,   230,   617,
     518,   359,   667,  -341,  -342,   424,   426,  -264,    98,   234,
     553,   757,    98,   231,   278,   241,   447,   765,   243,  -374,
     765,  -374,   246,  -339,  -338,   279,  -264,   281,   282,   291,
     293,   300,  -264,   235,   234,   465,   304,   236,   469,   464,
     556,   468,   559,   473,   561,   513,   235,   317,   562,   563,
     236,   565,   567,   175,   299,   243,   340,   342,   344,   364,
     573,    98,   575,    23,   176,   366,   580,   566,   450,   295,
      29,   235,  -137,   454,   449,   236,   241,   459,   242,   243,
     455,   461,   474,   246,   217,   475,   510,   604,   606,   241,
     253,  -137,   243,   482,   489,  -267,   246,   471,   217,   259,
     822,    98,   483,   488,   502,   514,   493,   498,   517,   825,
     238,   519,   239,   240,   241,   545,   242,   243,   244,   603,
     569,   246,   234,   587,  -267,    98,   546,   252,   253,   599,
     611,   334,   256,   612,   822,   257,   258,   259,  -269,  -267,
     672,   675,   677,   683,   822,   701,  -267,    98,   703,   438,
     716,   379,   425,   671,   731,   720,   674,   732,   761,   235,
     750,   625,     5,   236,   505,  -267,   149,  -269,   681,   555,
     572,   558,   302,   743,   775,   687,   682,   689,   776,  -246,
     692,   777,  -269,   695,  -267,   699,   729,   698,   830,  -269,
    -267,   817,   803,   568,   492,     0,   789,   668,   238,     0,
     239,   240,   241,     0,   242,   243,     0,   182,  -269,   246,
       0,  -246,   428,   711,     0,   712,   253,   715,     0,   719,
    -246,    98,   721,   257,   258,   259,   429,  -269,     0,   430,
       0,   153,     0,  -269,     0,  -246,   351,     0,     0,     0,
       0,     0,   709,   431,   432,    98,     0,     0,    98,     0,
     577,   579,    98,   433,   582,   585,     0,     0,     0,     0,
       0,    92,     0,     0,   434,     0,     0,     0,     0,     0,
      92,   687,   689,     0,   692,   695,   715,   719,     0,    92,
     378,   424,     0,     0,   744,   745,     0,   746,     0,   747,
     748,     0,    92,   749,   177,   178,     0,     0,    98,     0,
       0,     0,     0,     0,     0,   763,   513,     0,     0,   513,
      92,     0,   768,     0,     0,   755,   770,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     0,    92,     0,
       0,     0,    92,   102,   744,   778,   779,   747,   780,   781,
     782,   783,   102,     0,     0,   105,     0,   679,   680,     0,
       0,   190,   194,   217,   105,   102,     0,   794,   684,     0,
     795,     0,     0,   105,     0,     0,   800,     0,   693,     0,
       0,   696,   792,   102,   549,     0,   105,     0,     0,     0,
       0,    92,     0,     0,   813,   814,   815,   816,   791,     0,
       0,   102,     0,     0,   105,   102,     0,     0,   719,     0,
     826,   827,     0,     0,   819,     0,   813,   814,   815,   816,
     826,   827,   105,    98,     0,     0,   105,    14,     0,     0,
      17,    92,   831,   513,   175,     0,     0,     0,    22,     0,
      23,   723,     0,   719,    23,   176,     0,    29,   819,  -137,
       0,    29,     0,  -137,   102,    92,     0,     0,   819,   -39,
       0,     0,     0,    98,     0,    39,    98,   724,  -137,   725,
      43,   175,  -137,     0,     5,   105,     0,    92,   149,     0,
     152,    23,   176,    52,     0,    53,     0,     0,    29,     0,
    -137,  -246,   232,     0,   102,     0,   -38,   233,     0,     0,
       0,   669,   670,     0,   766,   234,     0,     0,     0,  -137,
       0,     0,     0,     0,     0,   105,     0,     0,   102,     0,
       0,     0,     0,  -246,     0,    98,     0,    98,     0,     0,
      98,     0,  -246,    98,    67,     0,    98,     0,     0,   105,
     102,   726,   235,   153,     0,     0,   236,  -246,     0,     0,
       0,    92,     0,     0,     0,     0,    98,     0,     0,     0,
       0,   105,   121,     0,     0,     0,   237,    98,     0,    98,
       0,     0,     0,     0,    98,    92,     0,     0,    92,     0,
       0,   238,    92,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,     0,   257,   258,   259,     0,
       0,   106,     0,   234,   102,     0,   620,     0,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,    92,   106,
       0,     0,     0,     0,    98,   105,     0,     0,   102,    98,
       0,   102,   106,     0,     0,   102,     0,     0,     0,     0,
     235,     0,     0,     0,   236,     0,     0,     0,     0,   105,
     106,    14,   105,     0,    17,    98,   105,     0,     0,    98,
       0,     0,    22,     0,    23,   723,     0,     0,   106,     0,
       0,    29,   106,  -137,     0,     0,     0,     0,     0,   238,
       0,   102,   240,   241,     0,   242,   243,     0,     0,    39,
     246,   724,  -137,   725,    43,     0,     0,   253,     0,     0,
       0,     0,   105,     0,   257,   258,   259,    52,     0,    53,
       0,     0,   234,     0,     5,     6,     7,     8,     9,    10,
       0,   106,     0,   127,    98,    98,    98,    98,     0,     0,
       0,     0,     0,    92,     0,     0,    21,     0,     0,     0,
      98,    98,   128,   129,   130,     0,    28,     0,     0,   235,
       0,    31,    32,   236,    34,     0,    35,    36,    67,     0,
       0,   106,     0,     0,    38,   784,    40,     0,     0,     0,
       0,   234,     0,    92,     0,     0,    92,     0,    48,    49,
      50,   131,     0,     0,     0,   106,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   102,     0,     0,   246,
       0,     0,     0,     0,     0,   252,   253,   106,   235,     0,
     256,    59,   236,   257,   258,   259,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,    92,   102,    92,     0,   102,
      92,     0,    70,    92,   133,     0,    92,   238,     0,     0,
       0,   241,     0,   242,   243,     0,     0,   105,   246,     0,
     105,     0,     0,     0,     0,   253,    92,     0,     0,     0,
       0,     0,   257,   258,   259,    14,     0,    92,    17,    92,
       0,   106,     0,     0,    92,     0,    22,     0,    23,   723,
       0,     0,     0,     0,     0,    29,     0,  -137,   102,     0,
     102,     0,     0,   102,     0,   106,   102,     0,   106,   102,
       0,     0,   106,   219,     0,   724,  -137,   725,    43,   105,
       0,   105,     0,     0,   105,     0,     0,   105,     0,   102,
     105,    52,     0,    53,   108,     0,     0,     0,     0,     0,
     102,     0,   102,   108,    92,     0,     0,   102,     0,    92,
     105,     0,   108,     0,   234,   110,     0,     0,   106,     0,
       0,   105,     0,   105,   110,   108,     0,     0,   105,     0,
       0,     0,     0,   110,     0,    92,     0,     0,     0,    92,
       0,     0,    67,   108,     0,     0,   110,     0,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,   234,     0,
       0,   108,     0,     0,   110,   108,     0,   102,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,     0,     0,     0,   110,     0,   105,     0,
       0,     0,     0,   105,   241,   235,   242,   243,   102,   236,
       0,   246,   102,     0,    92,    92,    92,    92,   253,     0,
       0,     0,     0,     0,   108,   257,   258,   259,     0,   105,
      92,    92,     0,   105,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,   238,   110,   239,   240,   241,     0,
     242,   243,   244,     0,     0,   246,   247,   248,     0,   250,
     251,   252,   253,     0,   108,     0,   256,     0,     0,   257,
     258,   259,     0,     0,     0,     0,     0,   102,   102,   102,
     102,     0,     0,   106,     0,   110,   106,     0,   108,     0,
       0,     0,     0,   102,   102,     0,     0,     0,   105,   105,
     105,   105,     0,     0,     0,     0,     0,     0,     0,   110,
     108,     0,     0,     0,   105,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,     0,   106,     0,     0,
     106,     0,     0,   106,   111,    74,   106,     0,     0,     0,
       0,     0,     0,   111,   119,     0,     0,     0,     0,     0,
       0,     0,   111,   146,     0,     0,   106,     0,     0,     0,
       0,     0,     0,     0,   108,   111,   168,   106,     0,   106,
       0,     0,     0,     0,   106,     0,     0,     0,     0,     0,
       0,     0,     0,   111,   193,   110,     0,     0,   108,     0,
       0,   108,     0,     0,     0,   108,     0,     0,     0,     0,
       0,   111,    74,     0,     0,   111,   220,     0,     0,   110,
       0,     0,   110,     0,     0,     0,   110,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,     0,     0,     0,     0,   106,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,   280,     0,     0,     0,   187,
       0,     0,   110,     0,     0,   106,     0,     0,     0,   106,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,     0,     0,
       0,     0,     0,     0,   111,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,  -352,   111,   338,
    -374,     0,  -374,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,   106,   106,   106,   283,     0,
     111,    74,     0,     0,     0,     0,     0,     0,     0,     0,
     106,   106,     0,     0,     0,     0,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     187,   187,   314,     0,     0,     0,     0,   110,     0,     0,
       0,     0,     0,   187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   108,     0,     0,   108,
       0,   187,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,     0,     0,   111,    74,     0,   110,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,   338,
       0,   111,   338,     0,   232,   111,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   108,     0,
     108,     0,     0,   108,     0,     0,   108,     0,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
       0,   110,     0,     0,   110,     0,     0,   110,     0,   108,
     110,   111,   512,     0,   235,     0,     0,     0,   236,     0,
     108,   187,   108,     0,     0,     0,     0,   108,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,   110,     0,   110,   187,     0,     0,     0,   110,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,     0,   257,   258,
     259,     0,     0,     0,     0,     0,     0,   108,   678,     0,
       0,     0,   108,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,     0,
       0,     0,     0,   110,     0,     0,     0,     0,   108,     0,
       0,     0,   108,     0,     0,     0,   111,   589,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
       0,     0,     0,   110,     0,   125,     0,     0,     0,     0,
     125,     0,     0,     0,   148,   125,   125,     0,     0,     0,
       0,     0,   162,     0,   165,     0,   111,   622,     0,   111,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   195,     0,     0,   108,   108,   108,
     108,     0,     0,     0,     0,     0,     0,   187,   187,     0,
       0,   187,   187,   108,   108,     0,     0,     0,   110,   110,
     110,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   222,     0,   110,   110,     0,     0,   111,   338,
     111,   338,     0,   111,   338,     0,   111,   338,     0,   111,
     697,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
     708,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   714,   111,   718,     0,   195,     0,   111,    74,   294,
       0,     0,     0,     0,   235,   195,     0,   303,   236,     0,
       0,     0,     0,   294,   187,   187,   321,     0,     0,     0,
       0,     0,     0,     0,     0,   187,   294,   314,   125,     0,
       0,   333,   314,     0,     0,   187,     0,     0,   187,     0,
       0,     0,     0,   238,   294,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   111,   250,   251,
     252,   253,   111,   254,     0,   256,     0,     0,   257,   258,
     259,     0,     0,   365,     0,     0,     0,     0,   165,   165,
     165,   165,     0,     0,     0,     0,     0,     0,   111,   769,
       0,     0,   111,   771,     0,   390,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   484,     0,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,   294,   294,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,   456,    34,   460,    35,    36,   111,   111,   111,
     111,   187,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,   321,   111,   111,     0,     0,    48,    49,    50,
     131,   165,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,     0,    57,     0,     0,   314,   314,     0,
     314,   314,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,   796,     0,     0,    62,     0,     0,
     314,   165,   314,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,   132,
       0,    70,     0,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   796,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -244,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -137,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,   294,     0,  -244,
      38,    39,    40,    41,  -137,    42,    43,     0,  -244,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
     593,    53,    54,  -244,   321,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   195,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,   664,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   456,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   321,     0,     0,     0,     0,     4,
       0,     5,     6,     7,     8,     9,    10,     0,  -391,     0,
      11,    12,    13,  -391,    14,    15,    16,    17,  -244,    18,
      19,  -391,    20,    21,  -391,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -137,    30,    31,    32,
      33,    34,   -37,    35,    36,   730,    37,     0,     0,     0,
    -244,    38,     0,    40,    41,  -137,    42,    43,  -391,  -244,
      44,    45,  -391,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -244,     0,     0,    55,     0,    56,
       0,     0,  -391,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -391,    59,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,     0,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,    64,  -391,  -391,  -391,
       0,    67,  -391,  -391,  -391,   125,     0,    69,  -391,    70,
       0,    71,     4,   774,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -244,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -137,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -244,    38,    39,    40,    41,  -137,    42,
      43,     0,  -244,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -244,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,   179,    67,     0,     0,    68,     0,     0,
      69,   206,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -244,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -137,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -244,    38,    39,    40,    41,  -137,
      42,    43,     0,  -244,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -244,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,   350,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -244,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -137,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -244,    38,    39,    40,    41,
    -137,    42,    43,     0,  -244,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -244,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,   206,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -244,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -137,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -244,    38,    39,    40,
      41,  -137,    42,    43,     0,  -244,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -244,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,    69,   624,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -244,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -137,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -244,    38,    39,
      40,    41,  -137,    42,    43,     0,  -244,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -244,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,   722,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -244,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -137,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -244,    38,
     219,    40,    41,  -137,    42,    43,     0,  -244,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -244,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -244,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -137,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -244,
      38,     0,    40,    41,  -137,    42,    43,     0,  -244,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -244,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -244,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   463,     0,    28,    29,     0,  -137,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -244,    38,     0,    40,    41,  -137,    42,    43,     0,  -244,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -244,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -244,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   467,     0,    28,    29,     0,  -137,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -244,    38,     0,    40,    41,  -137,    42,    43,     0,
    -244,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -244,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -244,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   686,     0,    28,    29,     0,  -137,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -244,    38,     0,    40,    41,  -137,    42,    43,
       0,  -244,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -244,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,    69,
       0,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -244,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   688,     0,    28,    29,     0,  -137,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -244,    38,     0,    40,    41,  -137,    42,
      43,     0,  -244,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -244,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -244,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   691,     0,    28,    29,     0,
    -137,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -244,    38,     0,    40,    41,  -137,
      42,    43,     0,  -244,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -244,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -244,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   694,     0,    28,    29,
       0,  -137,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -244,    38,     0,    40,    41,
    -137,    42,    43,     0,  -244,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -244,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -244,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   713,     0,    28,
      29,     0,  -137,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -244,    38,     0,    40,
      41,  -137,    42,    43,     0,  -244,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -244,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,    69,     0,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -244,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,   717,     0,
      28,    29,     0,  -137,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -244,    38,     0,
      40,    41,  -137,    42,    43,     0,  -244,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -244,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,   484,     0,     5,     6,     7,     8,     9,
      10,    63,  -391,    64,   127,    65,    66,  -391,    67,     0,
       0,    68,     0,     0,    69,  -391,    70,    21,    71,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,  -391,     0,     0,     0,  -391,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -262,  -391,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -391,    59,  -391,  -391,  -391,     0,  -391,  -391,  -391,
       0,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
      64,  -391,  -391,  -391,     0,     0,  -391,  -391,  -391,     0,
       0,   132,     0,    70,   484,   499,     5,     6,     7,     8,
       9,    10,     0,  -391,     0,   127,     0,     0,  -391,     0,
       0,     0,     0,     0,     0,     0,  -391,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,  -391,     0,     0,     0,  -391,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -391,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -391,    59,  -391,  -391,  -391,     0,  -391,  -391,
    -391,     0,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,    64,  -391,  -391,  -391,     0,     0,  -391,  -391,  -391,
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
       0,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,   132,    21,    70,     0,   499,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     5,     6,     7,
       8,     9,    10,     0,     0,    63,   127,    64,     0,    65,
      66,   179,     0,     0,   428,    68,     0,     0,   132,    21,
      70,     0,   133,   790,     0,   128,   129,   130,   429,    28,
       0,   430,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,   431,   432,    38,     0,    40,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,   434,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,   132,    21,    70,     0,   133,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,   210,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   211,     0,     5,
       6,     7,     8,     9,    10,    63,     0,    64,   127,    65,
      66,   179,     0,     0,     0,    68,     0,     0,   132,     0,
      70,    21,   133,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,   210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,   179,     0,
       0,     0,    68,     0,     0,   132,    21,    70,   507,   133,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,   155,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   156,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,    68,     0,     0,
     132,    21,    70,     0,   133,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,   210,
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
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,   183,
       0,     0,    68,     0,     0,   132,     0,    70,     0,   133,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,    21,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,    69,    21,    70,     0,
     133,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   156,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,   605,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,   751,    70,     0,
     133,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   752,
      49,    50,   753,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   754,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,   179,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     754,     0,     0,   128,   129,   742,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   554,     0,    28,     0,     0,
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
     557,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     5,     6,     7,     8,     9,
      10,     0,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,     0,   128,   129,   736,     0,    28,     0,     0,
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
     133,     0,     0,   128,   129,   739,     0,    28,     0,     0,
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
     133,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,    63,     0,
      64,    31,    32,    66,    34,     0,    35,    36,    68,     0,
       0,   132,     0,    70,    38,   133,    40,     0,     0,     0,
       5,     0,     0,     0,     0,     0,     0,   628,    48,    49,
      50,   131,   629,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -271,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,  -271,     0,     0,     0,  -271,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,   179,     0,     0,     0,     0,     0,     0,
     132,     0,    70,  -271,   754,     0,   232,     0,     0,     0,
       0,   233,   630,   631,   632,   633,   634,   635,     0,   234,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
       0,     0,   646,     0,     0,     0,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,     0,   658,     0,
       0,   659,   660,   661,   662,   663,   235,     0,     0,     0,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,   232,     0,     0,
     799,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,   232,     0,     0,   834,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,   309,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,   232,   506,
       0,     0,     0,   233,     0,   121,     0,     0,     0,   237,
       0,   234,   306,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   307,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     584,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,   282,     0,   232,
       0,     0,     0,     0,   233,     0,   121,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,   232,     0,     0,
     313,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,   186,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,   282,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,   578,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,   282,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,   282,
       0,     0,     0,     0,   685,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   282,     0,     0,     0,     0,   690,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
     785,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   282,     0,
       0,     0,     0,   786,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   282,     0,     0,     0,     0,   787,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,     0,     0,     0,   788,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,   282,     0,     0,
       0,     0,   801,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
     282,     0,   232,     0,     0,   802,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,   282,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   337,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,   288,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,  -353,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   121,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   232,   254,   255,   256,     0,   233,   257,   258,   259,
       0,     0,     0,     0,   234,     0,     0,     0,     0,   346,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,   235,     0,     0,     0,   236,     0,     0,   234,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,   235,   242,   243,   244,   236,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,     0,   257,   258,   259,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   232,   254,   255,   256,     0,   233,   257,
     258,   259,     0,     0,     0,     0,   234,   442,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   235,     0,     0,     0,   236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,   237,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   235,     0,     0,
       0,   236,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   237,   254,   255,   256,     0,     0,   257,   258,   259,
       0,   235,     0,     0,     0,   236,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   237,   254,   255,   256,     0,
     292,   257,   258,   259,     0,     0,   232,     0,     0,     0,
     238,   233,   239,   240,   241,     0,   242,   243,   244,   234,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   336,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   349,
     257,   258,   259,   361,   237,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,     0,   257,   258,   259,     0,     0,   232,
       0,     0,     0,     0,   233,   235,     0,     0,     0,   236,
       0,     0,   234,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,   232,     0,   257,
     258,   259,   233,   237,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   530,   257,   258,   259,   233,   235,     0,     0,
       0,   236,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,   232,
     531,   257,   258,   259,   233,   237,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,   532,   257,   258,   259,   233,   235,
       0,     0,     0,   236,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,   235,   242,   243,   244,   236,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   533,   257,   258,   259,   233,   237,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,   232,   534,   257,   258,   259,
     233,   235,     0,     0,     0,   236,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,   235,   242,   243,   244,   236,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,   535,   257,   258,   259,   233,   237,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,   232,   536,   257,
     258,   259,   233,   235,     0,     0,     0,   236,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,   235,   242,   243,
     244,   236,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,   232,   537,   257,   258,   259,
     233,   237,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,   232,
     538,   257,   258,   259,   233,   235,     0,     0,     0,   236,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,   232,   539,   257,
     258,   259,   233,   237,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   540,   257,   258,   259,   233,   235,     0,     0,
       0,   236,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,   232,
     541,   257,   258,   259,   233,   237,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,   542,   257,   258,   259,   233,   235,
       0,     0,     0,   236,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,   235,   242,   243,   244,   236,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   543,   257,   258,   259,   233,   237,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,   232,   544,   257,   258,   259,
     233,   235,     0,     0,     0,   236,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,   235,   242,   243,   244,   236,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,     0,   257,   258,   259,   233,   237,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   588,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   564,   257,
     258,   259,     0,   235,     0,     0,     0,   236,     0,     0,
       0,   232,   616,     0,     0,     0,   233,     0,     0,     0,
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
       0,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   673,   623,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,   676,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,   735,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,   232,   767,     0,   738,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,   232,     0,     0,   807,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,   808,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,     0,     0,   232,     0,     0,   809,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,   810,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,     0,     0,   232,     0,     0,
     811,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,   812,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   552,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,     0,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,     0,   250,   251,
     252,   253,     0,   254,     0,   256,     0,     0,   257,   258,
     259
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   342,    16,    15,   143,    19,   320,
     602,    23,   279,   279,    25,    26,    27,   153,     3,   161,
       3,     1,   626,    34,    35,   325,    37,    38,     3,   483,
       1,   159,   174,    44,    45,    46,    47,    48,     3,    34,
      51,    34,   342,    54,    55,    56,   790,    80,    59,    63,
     192,    62,    63,    64,    24,    66,     3,    24,    69,    70,
      71,   226,   227,   228,   229,    24,   626,   666,    44,    24,
     104,     3,    34,    20,    70,   104,    61,    13,    54,    85,
     824,     0,    18,    63,   131,   762,   133,    34,   216,    80,
      37,     1,    63,    25,    63,    71,   130,    20,    78,    34,
      50,   134,   108,   132,    51,    52,     3,    78,    73,   104,
     702,   104,    34,    80,    61,   129,   127,   128,   129,   130,
     131,   132,   133,   103,    34,    72,   803,    77,    34,    52,
     139,   104,   103,   275,   276,    83,   147,    85,    61,   743,
      72,   134,   104,    25,   598,   104,   131,    16,   131,   129,
     161,   147,   123,    76,   123,    61,   131,   133,   129,   129,
     129,   134,   129,   174,   176,   161,   131,   727,    85,   104,
     129,   482,   134,    83,   129,    85,   775,   776,   174,   104,
     130,   192,   104,   743,   131,     1,   104,   104,   104,   104,
      59,   108,    38,   103,   104,    72,   192,   789,   340,   134,
     211,   131,   131,   133,   369,    74,   104,   132,   354,   208,
     356,    57,   134,   123,   132,   132,   132,   132,    34,   131,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   130,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,    85,
       3,    85,   104,   104,     7,   729,   104,   288,    20,   275,
     276,   592,   293,   550,   550,   104,     7,   103,   104,   104,
     104,    34,   123,   304,   108,   306,   307,   104,   309,   310,
     132,   601,   313,   104,   132,    38,    52,   123,   123,   131,
      52,   824,    69,   132,   131,    61,   123,   328,   132,    61,
     104,   834,   123,   334,    57,    44,    72,    34,   304,   340,
      72,     2,   157,   158,   159,   346,   347,    80,   334,   123,
      11,   469,   470,   354,   340,   356,   171,   358,   359,    20,
     361,   362,   123,   235,   236,   366,    63,   104,   354,     3,
     356,   104,    33,     7,   189,   104,   502,   123,   104,   707,
     104,    78,   710,   104,   199,   131,   123,   133,    85,   123,
      51,    60,   510,   511,   123,    64,    23,   123,   123,   123,
     366,   134,   123,   123,   123,   277,   278,   104,    69,    23,
      60,   701,    73,   128,    64,   103,   288,   707,   106,   131,
     710,   133,   110,   123,   123,   131,   123,   123,   129,   123,
      79,   129,   129,    60,    23,   307,   131,    64,   310,   440,
     441,   442,   443,   444,   445,   446,    60,   131,   447,   448,
      64,   452,   453,    19,   455,   106,   131,    85,   123,     7,
     461,   122,   463,    29,    30,   103,   467,   453,   123,   455,
      36,    60,    38,   132,   289,    64,   103,   123,   105,   106,
     104,    34,    26,   110,   485,   132,   358,   488,   489,   103,
     117,    57,   106,   104,    85,    34,   110,   312,   499,   126,
     790,   162,   132,    83,   104,   134,   132,   132,   129,   799,
      99,   129,   101,   102,   103,   132,   105,   106,   107,   485,
      85,   110,    23,    26,    63,   186,   134,   116,   117,   132,
     132,   104,   121,   499,   824,   124,   125,   126,    34,    78,
      24,    24,   134,   129,   834,   103,    85,   208,    61,   550,
     134,   552,   553,   554,   106,   134,   557,   131,    78,    60,
     132,   517,     3,    64,   347,   104,     7,    63,   569,   441,
     460,   443,   154,   683,   731,   576,   570,   578,   731,    20,
     581,   732,    78,   584,   123,   590,   627,   588,   803,    85,
     129,   789,   773,   455,   329,    -1,   750,   550,    99,    -1,
     101,   102,   103,    -1,   105,   106,    -1,   608,   104,   110,
      -1,    52,    20,   614,    -1,   616,   117,   618,    -1,   620,
      61,   282,   623,   124,   125,   126,    34,   123,    -1,    37,
      -1,    72,    -1,   129,    -1,    76,   625,    -1,    -1,    -1,
      -1,    -1,   608,    51,    52,   306,    -1,    -1,   309,    -1,
     465,   466,   313,    61,   469,   470,    -1,    -1,    -1,    -1,
      -1,     2,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      11,   672,   673,    -1,   675,   676,   677,   678,    -1,    20,
     552,   553,    -1,    -1,   685,   686,    -1,   688,    -1,   690,
     691,    -1,    33,   694,   724,   725,    -1,    -1,   359,    -1,
      -1,    -1,    -1,    -1,    -1,   706,   707,    -1,    -1,   710,
      51,    -1,   713,    -1,    -1,   701,   717,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    73,    11,   735,   736,   737,   738,   739,   740,
     741,   742,    20,    -1,    -1,     2,    -1,   562,   563,    -1,
      -1,   752,   753,   754,    11,    33,    -1,   758,   573,    -1,
     761,    -1,    -1,    20,    -1,    -1,   767,    -1,   583,    -1,
      -1,   586,   758,    51,   435,    -1,    33,    -1,    -1,    -1,
      -1,   122,    -1,    -1,   785,   786,   787,   788,   754,    -1,
      -1,    69,    -1,    -1,    51,    73,    -1,    -1,   799,    -1,
     801,   802,    -1,    -1,   790,    -1,   807,   808,   809,   810,
     811,   812,    69,   474,    -1,    -1,    73,    16,    -1,    -1,
      19,   162,   823,   824,    19,    -1,    -1,    -1,    27,    -1,
      29,    30,    -1,   834,    29,    30,    -1,    36,   824,    38,
      -1,    36,    -1,    38,   122,   186,    -1,    -1,   834,    44,
      -1,    -1,    -1,   514,    -1,    54,   517,    56,    57,    58,
      59,    19,    57,    -1,     3,   122,    -1,   208,     7,    -1,
       9,    29,    30,    72,    -1,    74,    -1,    -1,    36,    -1,
      38,    20,    10,    -1,   162,    -1,    44,    15,    -1,    -1,
      -1,   552,   553,    -1,   709,    23,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,   186,    -1,
      -1,    -1,    -1,    52,    -1,   576,    -1,   578,    -1,    -1,
     581,    -1,    61,   584,   123,    -1,   587,    -1,    -1,   186,
     208,   130,    60,    72,    -1,    -1,    64,    76,    -1,    -1,
      -1,   282,    -1,    -1,    -1,    -1,   607,    -1,    -1,    -1,
      -1,   208,    80,    -1,    -1,    -1,    84,   618,    -1,   620,
      -1,    -1,    -1,    -1,   625,   306,    -1,    -1,   309,    -1,
      -1,    99,   313,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    -1,   124,   125,   126,    -1,
      -1,     2,    -1,    23,   282,    -1,   134,    -1,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   359,    20,
      -1,    -1,    -1,    -1,   685,   282,    -1,    -1,   306,   690,
      -1,   309,    33,    -1,    -1,   313,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,   306,
      51,    16,   309,    -1,    19,   716,   313,    -1,    -1,   720,
      -1,    -1,    27,    -1,    29,    30,    -1,    -1,    69,    -1,
      -1,    36,    73,    38,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   359,   102,   103,    -1,   105,   106,    -1,    -1,    54,
     110,    56,    57,    58,    59,    -1,    -1,   117,    -1,    -1,
      -1,    -1,   359,    -1,   124,   125,   126,    72,    -1,    74,
      -1,    -1,    23,    -1,     3,     4,     5,     6,     7,     8,
      -1,   122,    -1,    12,   785,   786,   787,   788,    -1,    -1,
      -1,    -1,    -1,   474,    -1,    -1,    25,    -1,    -1,    -1,
     801,   802,    31,    32,    33,    -1,    35,    -1,    -1,    60,
      -1,    40,    41,    64,    43,    -1,    45,    46,   123,    -1,
      -1,   162,    -1,    -1,    53,   130,    55,    -1,    -1,    -1,
      -1,    23,    -1,   514,    -1,    -1,   517,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,   186,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   474,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,   116,   117,   208,    60,    -1,
     121,   100,    64,   124,   125,   126,    -1,   474,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,   576,   514,   578,    -1,   517,
     581,    -1,   131,   584,   133,    -1,   587,    99,    -1,    -1,
      -1,   103,    -1,   105,   106,    -1,    -1,   514,   110,    -1,
     517,    -1,    -1,    -1,    -1,   117,   607,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,    16,    -1,   618,    19,   620,
      -1,   282,    -1,    -1,   625,    -1,    27,    -1,    29,    30,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    38,   576,    -1,
     578,    -1,    -1,   581,    -1,   306,   584,    -1,   309,   587,
      -1,    -1,   313,    54,    -1,    56,    57,    58,    59,   576,
      -1,   578,    -1,    -1,   581,    -1,    -1,   584,    -1,   607,
     587,    72,    -1,    74,     2,    -1,    -1,    -1,    -1,    -1,
     618,    -1,   620,    11,   685,    -1,    -1,   625,    -1,   690,
     607,    -1,    20,    -1,    23,     2,    -1,    -1,   359,    -1,
      -1,   618,    -1,   620,    11,    33,    -1,    -1,   625,    -1,
      -1,    -1,    -1,    20,    -1,   716,    -1,    -1,    -1,   720,
      -1,    -1,   123,    51,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,
      -1,    69,    -1,    -1,    51,    73,    -1,   685,    -1,    -1,
      -1,    -1,   690,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    73,    -1,   685,    -1,
      -1,    -1,    -1,   690,   103,    60,   105,   106,   716,    64,
      -1,   110,   720,    -1,   785,   786,   787,   788,   117,    -1,
      -1,    -1,    -1,    -1,   122,   124,   125,   126,    -1,   716,
     801,   802,    -1,   720,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   474,    99,   122,   101,   102,   103,    -1,
     105,   106,   107,    -1,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,   162,    -1,   121,    -1,    -1,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,   785,   786,   787,
     788,    -1,    -1,   514,    -1,   162,   517,    -1,   186,    -1,
      -1,    -1,    -1,   801,   802,    -1,    -1,    -1,   785,   786,
     787,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     208,    -1,    -1,    -1,   801,   802,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   576,    -1,   578,    -1,    -1,
     581,    -1,    -1,   584,     2,     2,   587,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    20,    -1,    -1,   607,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   282,    33,    33,   618,    -1,   620,
      -1,    -1,    -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    51,   282,    -1,    -1,   306,    -1,
      -1,   309,    -1,    -1,    -1,   313,    -1,    -1,    -1,    -1,
      -1,    69,    69,    -1,    -1,    73,    73,    -1,    -1,   306,
      -1,    -1,   309,    -1,    -1,    -1,   313,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   685,    -1,    -1,    -1,    -1,   690,
      -1,   359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,   122,    -1,    -1,    -1,    47,
      -1,    -1,   359,    -1,    -1,   716,    -1,    -1,    -1,   720,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   186,   186,
     131,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   785,   786,   787,   788,   126,    -1,
     208,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     801,   802,    -1,    -1,    -1,    -1,   474,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   159,   160,    -1,    -1,    -1,    -1,   474,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   514,    -1,    -1,   517,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   199,    -1,    -1,   282,   282,    -1,   514,    -1,    -1,
     517,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,   306,
      -1,   309,   309,    -1,    10,   313,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,   576,    -1,
     578,    -1,    -1,   581,    -1,    -1,   584,    -1,    -1,   587,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   576,
      -1,   578,    -1,    -1,   581,    -1,    -1,   584,    -1,   607,
     587,   359,   359,    -1,    60,    -1,    -1,    -1,    64,    -1,
     618,   289,   620,    -1,    -1,    -1,    -1,   625,    -1,    -1,
     607,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,   618,    -1,   620,   312,    -1,    -1,    -1,   625,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    -1,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,   685,   134,    -1,
      -1,    -1,   690,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   685,    -1,
      -1,    -1,    -1,   690,    -1,    -1,    -1,    -1,   716,    -1,
      -1,    -1,   720,    -1,    -1,    -1,   474,   474,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   716,
      -1,    -1,    -1,   720,    -1,    13,    -1,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    22,    23,    24,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    32,    -1,   514,   514,    -1,   517,
     517,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    -1,   785,   786,   787,
     788,    -1,    -1,    -1,    -1,    -1,    -1,   465,   466,    -1,
      -1,   469,   470,   801,   802,    -1,    -1,    -1,   785,   786,
     787,   788,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,   801,   802,    -1,    -1,   576,   576,
     578,   578,    -1,   581,   581,    -1,   584,   584,    -1,   587,
     587,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   607,
     607,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     618,   618,   620,   620,    -1,   143,    -1,   625,   625,   147,
      -1,    -1,    -1,    -1,    60,   153,    -1,   155,    64,    -1,
      -1,    -1,    -1,   161,   562,   563,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   573,   174,   575,   176,    -1,
      -1,   179,   580,    -1,    -1,   583,    -1,    -1,   586,    -1,
      -1,    -1,    -1,    99,   192,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   685,   114,   115,
     116,   117,   690,   119,    -1,   121,    -1,    -1,   124,   125,
     126,    -1,    -1,   221,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,    -1,    -1,    -1,    -1,    -1,    -1,   716,   716,
      -1,    -1,   720,   720,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   275,   276,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,   300,    43,   302,    45,    46,   785,   786,   787,
     788,   709,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,   320,   801,   802,    -1,    -1,    67,    68,    69,
      70,   329,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   340,    -1,    84,    -1,    -1,   745,   746,    -1,
     748,   749,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,   762,    -1,    -1,   107,    -1,    -1,
     768,   369,   770,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,
      -1,   131,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   803,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,   455,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
     478,    74,    75,    76,   482,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   502,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,   524,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,   129,    -1,   131,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   570,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   592,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,   643,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,   125,   126,   723,    -1,   129,   130,   131,
      -1,   133,     1,   731,     3,     4,     5,     6,     7,     8,
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
      -1,   120,   121,   122,   123,    -1,    -1,   126,    -1,    -1,
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
      -1,   126,    -1,    -1,   129,   130,   131,     1,   133,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
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
      -1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
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
     121,   122,    -1,    -1,    20,   126,    -1,    -1,   129,    25,
     131,    -1,   133,   134,    -1,    31,    32,    33,    34,    35,
      -1,    37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
     116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,   129,    25,   131,    -1,   133,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,   108,    -1,     3,
       4,     5,     6,     7,     8,   116,    -1,   118,    12,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,    -1,   129,    -1,
     131,    25,   133,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,   126,    -1,    -1,   129,    25,   131,   132,   133,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,
     129,    25,   131,    -1,   133,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,
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
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,   129,    -1,   131,    -1,   133,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
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
      -1,    -1,    -1,    -1,    82,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,    25,   131,    -1,   133,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,   116,    -1,
     118,    40,    41,   121,    43,    -1,    45,    46,   126,    -1,
      -1,   129,    -1,   131,    53,   133,    55,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    10,    67,    68,
      69,    70,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,   104,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
     129,    -1,   131,   132,   133,    -1,    10,    -1,    -1,    -1,
      -1,    15,    85,    86,    87,    88,    89,    90,    -1,    23,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,    -1,
      -1,   124,   125,   126,   127,   128,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,
     134,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    10,    -1,    -1,   134,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    10,   132,
      -1,    -1,    -1,    15,    -1,    80,    -1,    -1,    -1,    84,
      -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,   129,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,    -1,   129,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    80,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,   129,    -1,    10,    -1,    -1,
      71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    24,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,   129,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
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
      -1,    60,   124,   125,   126,    64,    -1,   129,    -1,    -1,
      -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    60,   124,   125,   126,    64,    -1,
     129,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,   129,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,   129,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    10,   119,   120,   121,    -1,    15,   124,   125,   126,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    60,   105,   106,   107,    64,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    -1,   124,   125,   126,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    10,   119,   120,   121,    -1,    15,   124,
     125,   126,    -1,    -1,    -1,    -1,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    -1,
      -1,    64,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    84,   119,   120,   121,    -1,    -1,   124,   125,   126,
      -1,    60,    -1,    -1,    -1,    64,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    84,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    10,    -1,    -1,    -1,
      99,    15,   101,   102,   103,    -1,   105,   106,   107,    23,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,   126,    83,    84,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    -1,   124,   125,   126,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    60,    -1,    -1,    -1,    64,
      -1,    -1,    23,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,
     105,   106,   107,    64,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    10,    -1,   124,
     125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    60,    -1,    -1,
      -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    60,   105,   106,   107,    64,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    10,
     123,   124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,   123,   124,   125,   126,    15,    60,
      -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    60,   105,   106,   107,    64,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    10,   123,   124,   125,   126,
      15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    60,   105,   106,   107,    64,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,   123,   124,   125,   126,    15,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    10,   123,   124,
     125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    60,   105,   106,
     107,    64,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    10,   123,   124,   125,   126,
      15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    10,
     123,   124,   125,   126,    15,    60,    -1,    -1,    -1,    64,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,
     105,   106,   107,    64,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    10,   123,   124,
     125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    60,    -1,    -1,
      -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    60,   105,   106,   107,    64,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    10,
     123,   124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,   123,   124,   125,   126,    15,    60,
      -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    60,   105,   106,   107,    64,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    10,   123,   124,   125,   126,
      15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    60,   105,   106,   107,    64,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,    -1,   124,   125,   126,    15,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,   126,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,    -1,
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
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    24,    83,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
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
      64,    -1,    -1,    -1,    10,    11,    -1,    71,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    10,    -1,    -1,    71,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    71,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,    71,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,    -1,    71,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,
      71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    71,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,    -1,   119,    -1,   121,    -1,    -1,   124,   125,
     126
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
     206,   103,   213,    61,   209,   207,    34,   134,   143,   214,
     134,   233,   233,    33,   143,   233,   134,    33,   143,   233,
     134,   233,   130,    30,    56,    58,   130,   142,   157,   191,
     153,   106,   131,   183,   183,    71,    33,    33,    71,    33,
      33,    33,    33,   169,   233,   233,   233,   233,   233,   233,
     132,    25,    67,    70,   133,   194,   212,   221,   108,   196,
     207,    78,   197,   233,   210,   221,   155,    11,   233,   143,
     233,   143,   157,   209,   153,   181,   182,   185,   233,   233,
     233,   233,   233,   233,   130,    71,    71,    71,    71,   213,
     134,   214,   194,   195,   233,   233,   146,   156,   193,   134,
     233,    71,    71,   197,   183,   183,   132,    71,    71,    71,
      71,    71,    71,   233,   233,   233,   233,   196,   207,   194,
     211,   212,   221,    34,   134,   221,   233,   233,   123,   192,
     193,   233,   211,   212,   134,   211
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
     162,   162,   162,   163,   164,   164,   165,   165,   166,   166,
     167,   167,   167,   168,   168,   169,   169,   169,   170,   171,
     171,   171,   172,   172,   174,   175,   173,   176,   176,   176,
     176,   178,   179,   177,   180,   180,   180,   180,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   183,   183,   184,   185,
     185,   185,   186,   186,   186,   186,   187,   187,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   189,   189,   189,
     189,   190,   190,   191,   191,   191,   191,   191,   191,   192,
     192,   193,   193,   194,   194,   195,   195,   196,   196,   197,
     197,   198,   198,   198,   199,   199,   200,   200,   200,   201,
     201,   201,   201,   201,   202,   202,   202,   203,   203,   204,
     204,   204,   205,   205,   205,   206,   206,   206,   207,   207,
     207,   208,   208,   209,   209,   209,   209,   210,   210,   210,
     210,   211,   211,   211,   212,   212,   212,   212,   212,   213,
     213,   213,   213,   213,   213,   213,   214,   214,   214,   214,
     215,   215,   215,   216,   216,   217,   217,   218,   218,   219,
     219,   219,   219,   219,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   223,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   227,   228,   228,   229,
     229,   229,   230,   230,   231,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     234,   234,   234,   234,   235,   235,   236,   236,   236,   237,
     237,   237,   238,   238,   238,   239,   239,   239,   239,   239,
     239,   239,   239,   240,   240,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   242,   242,
     242,   242,   242,   242,   243,   243,   243,   243,   244,   244,
     244,   244,   245,   245,   245,   245,   245,   245,   245
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
       3,     6,     5,     5,     0,     2,     3,     2,     6,     8,
       1,     1,     1,     0,     2,     0,     2,     3,     5,     1,
       2,     3,     1,     3,     0,     0,     8,     0,     1,     2,
       2,     0,     0,     9,     3,     3,     5,     5,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     4,     6,     6,     0,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     0,     1,     2,     2,     0,
       2,     3,     4,     4,     2,     4,     0,     2,     2,     4,
       4,     4,     5,     4,     0,     1,     1,     1,     3,     3,
       4,     5,     1,     1,     3,     1,     2,     3,     0,     2,
       2,     0,     4,     0,     2,     2,     1,     4,     4,     6,
       1,     0,     1,     1,     3,     4,     3,     4,     6,     0,
       2,     2,     2,     2,     2,     2,     1,     1,     3,     3,
       1,     1,     1,     3,     3,     0,     1,     1,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     2,
       3,     3,     1,     3,     2,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     4,     3,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 4475 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 422 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4481 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 423 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4487 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 430 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4493 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 435 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4499 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 436 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4505 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 452 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4511 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 453 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4517 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 454 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4523 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 455 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4529 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 456 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4535 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 457 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4541 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 458 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4547 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 459 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4553 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 460 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4559 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 461 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4565 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 462 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4571 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 463 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4577 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 464 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4583 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 465 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4589 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 466 "chapel.ypp" /* yacc.c:1646  */
    { printf("syntax error"); clean_exit(1); }
#line 4595 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 471 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4601 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 473 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4607 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 477 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4613 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 478 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4619 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 479 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4625 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 494 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4631 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 495 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4637 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 500 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4643 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 501 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4649 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 506 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4659 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 511 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4669 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 516 "chapel.ypp" /* yacc.c:1646  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4678 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 520 "chapel.ypp" /* yacc.c:1646  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4687 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 526 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4693 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 527 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 4699 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 528 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 4705 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 532 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 4711 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 536 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4717 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 537 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4723 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 538 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4729 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 539 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4735 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 540 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4741 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 541 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4747 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 542 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4753 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 543 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4759 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 544 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4765 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 545 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4771 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 546 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4777 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 547 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4783 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 548 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4789 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 549 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4795 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 550 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4801 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 554 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = NULL; }
#line 4807 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 559 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4813 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 563 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4819 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 564 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4825 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 568 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4831 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 569 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4837 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 573 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4843 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 574 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4849 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 578 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4855 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 580 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4861 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 581 "chapel.ypp" /* yacc.c:1646  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4867 "bison-chapel.cpp" /* yacc.c:1646  */
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
#line 4883 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 608 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 4889 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 609 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4895 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 610 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4901 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 611 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 4907 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 612 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4913 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 613 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4919 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 614 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4925 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 615 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4931 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 616 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4937 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 617 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4943 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 618 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4949 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 619 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4955 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 620 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 4961 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 621 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 4967 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 622 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4973 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 623 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 4979 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 625 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4989 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 631 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 4999 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 637 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5009 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 643 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5019 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 649 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5030 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 656 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5041 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 665 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("_build_tuple_always_allow_ref", (yyvsp[-1].pcallexpr)); }
#line 5047 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 669 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5053 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 670 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5059 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 671 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5065 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 672 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5071 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 677 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5077 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 681 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5083 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 682 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5089 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 687 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5095 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 689 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5101 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 696 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5110 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 701 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5119 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 708 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5129 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 713 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5139 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 718 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5149 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 726 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = NULL; }
#line 5155 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 727 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5161 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 732 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5167 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 734 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5173 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 736 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5179 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 741 "chapel.ypp" /* yacc.c:1646  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5190 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 751 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5203 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 760 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5211 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 764 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5220 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 771 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5226 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 772 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5232 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 777 "chapel.ypp" /* yacc.c:1646  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5241 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 782 "chapel.ypp" /* yacc.c:1646  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5250 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 787 "chapel.ypp" /* yacc.c:1646  */
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
#line 5270 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 807 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5281 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 813 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5293 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 820 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5306 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 828 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5319 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 839 "chapel.ypp" /* yacc.c:1646  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5329 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 845 "chapel.ypp" /* yacc.c:1646  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5340 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 852 "chapel.ypp" /* yacc.c:1646  */
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
#line 5363 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 874 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5371 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 878 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5380 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 883 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5388 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 887 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5397 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 895 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5403 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 896 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "&"; }
#line 5409 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 897 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "|"; }
#line 5415 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 898 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "^"; }
#line 5421 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 899 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "~"; }
#line 5427 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 900 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "=="; }
#line 5433 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 901 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "!="; }
#line 5439 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 902 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<="; }
#line 5445 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 903 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">="; }
#line 5451 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 904 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<"; }
#line 5457 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 905 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">"; }
#line 5463 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 906 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "+"; }
#line 5469 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 907 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "-"; }
#line 5475 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 908 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "*"; }
#line 5481 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 909 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "/"; }
#line 5487 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 910 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<"; }
#line 5493 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 911 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>"; }
#line 5499 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 912 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "%"; }
#line 5505 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 913 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "**"; }
#line 5511 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 914 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "!"; }
#line 5517 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 915 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "chpl_by"; }
#line 5523 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 916 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "#"; }
#line 5529 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 917 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "chpl_align"; }
#line 5535 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 918 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<=>"; }
#line 5541 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 919 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<~>"; }
#line 5547 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 923 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "="; }
#line 5553 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 924 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "+="; }
#line 5559 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 925 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "-="; }
#line 5565 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 926 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "*="; }
#line 5571 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 927 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "/="; }
#line 5577 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 928 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "%="; }
#line 5583 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 929 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "**="; }
#line 5589 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 930 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "&="; }
#line 5595 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 931 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "|="; }
#line 5601 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 932 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "^="; }
#line 5607 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 933 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>="; }
#line 5613 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 934 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<="; }
#line 5619 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 938 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5625 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 939 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5631 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 943 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5637 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 947 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5643 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 948 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5649 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 949 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5655 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 954 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5661 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 956 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5667 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 958 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5673 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 960 "chapel.ypp" /* yacc.c:1646  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5679 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 964 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5685 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 965 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 5691 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 969 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_IN; }
#line 5697 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 970 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5703 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 971 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_OUT; }
#line 5709 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 972 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST; }
#line 5715 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 973 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5721 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 974 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5727 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 975 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5733 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 976 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_REF; }
#line 5739 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 977 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5745 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 981 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5751 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 982 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5757 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 983 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_REF;   }
#line 5763 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 984 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 5769 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 988 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5775 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 989 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5781 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 993 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_VALUE; }
#line 5787 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 994 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_VALUE; }
#line 5793 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 995 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 5799 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 996 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_REF; }
#line 5805 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 997 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_PARAM; }
#line 5811 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 998 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_TYPE; }
#line 5817 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1002 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = NULL; }
#line 5823 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1008 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5829 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1013 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5835 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1015 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5841 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1019 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5847 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1024 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5853 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1025 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5859 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1029 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 5865 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1030 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5871 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1035 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 5877 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1037 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 5883 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1039 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 5889 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1044 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 5906 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1057 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 5924 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1073 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 5930 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1075 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5936 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1077 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 5942 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1082 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 5954 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1090 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 5966 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1098 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 5978 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1106 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 5991 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1115 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6002 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1124 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6008 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1125 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6014 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1126 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6020 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1132 "chapel.ypp" /* yacc.c:1646  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6029 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1140 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6035 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1142 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6045 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1148 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6051 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1153 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6057 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1155 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6063 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1157 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6069 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1162 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6075 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1164 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6081 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1166 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6087 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1172 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6093 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1173 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6099 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1174 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6105 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1178 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6111 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1180 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6117 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1184 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6123 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1185 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6129 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1186 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6135 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1187 "chapel.ypp" /* yacc.c:1646  */
    {printf("bad type specification"); }
#line 6141 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1208 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6149 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1212 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6157 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1216 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6169 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1223 "chapel.ypp" /* yacc.c:1646  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6175 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1227 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6181 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1228 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6187 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1229 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6193 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1234 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6199 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1236 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6205 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1242 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6211 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1244 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6217 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1246 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6223 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1250 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6229 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1251 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6235 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1252 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6241 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1253 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6247 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1254 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6253 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1255 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6259 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1256 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6265 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1262 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6271 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1263 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6277 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1264 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6283 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 1265 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6289 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 1269 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6295 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 1270 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6301 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1271 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6307 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 1275 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6313 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 1276 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6319 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 1280 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6325 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 1285 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6331 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 1286 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6337 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 1290 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6343 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 1291 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6349 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 1292 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6355 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 1293 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6361 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 1298 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6367 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 1315 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6373 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 1317 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6379 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1319 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6385 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 1321 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6391 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1323 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6397 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 1325 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6403 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1327 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6409 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1332 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6415 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1334 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6421 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 1336 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6427 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 1338 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6433 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 1340 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6439 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 1342 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6445 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 1344 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6451 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 1346 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6457 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1348 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6463 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1350 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6469 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 1352 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 1354 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6481 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1356 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6492 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1363 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6502 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1369 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6512 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1375 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6522 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1381 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6532 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 1390 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6538 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 1394 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6544 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 1410 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6550 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 1414 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = NULL; }
#line 6556 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 1419 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6562 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 1424 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6568 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 1426 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6577 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 1434 "chapel.ypp" /* yacc.c:1646  */
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
#line 6592 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 1444 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6598 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 1445 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6604 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 1451 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6610 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 1456 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6616 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 1461 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6622 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 1477 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6628 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 1479 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6634 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 1481 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6640 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 1483 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6646 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 1485 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 6652 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 1487 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 6658 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 376:
#line 1510 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6664 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 1511 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6670 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 1512 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6676 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 1516 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6682 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 1517 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6688 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 1518 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6694 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 1526 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6700 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 1527 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6706 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 384:
#line 1528 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6712 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 1532 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6718 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 386:
#line 1533 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6724 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 1534 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6730 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 1535 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6736 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 1536 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6742 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 1537 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6748 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 1538 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6754 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 1540 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6762 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 1546 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6768 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 1547 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6774 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 1551 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6780 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 1552 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6786 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 1553 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6792 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 1554 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6798 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 1555 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6804 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 1556 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6810 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 1557 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6816 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 1558 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6822 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 1559 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6828 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 1560 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6834 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 1561 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6840 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 1562 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6846 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 1563 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6852 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 1564 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6858 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 1565 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6864 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 1566 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6870 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 1567 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6876 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 1568 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6882 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 1569 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6888 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 1570 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6894 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 1571 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6900 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 1572 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6906 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 1573 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6912 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 1577 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 6918 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 419:
#line 1578 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 6924 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 1579 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 6930 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 1580 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 6936 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 1581 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 6942 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 1582 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 6948 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 1586 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6954 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 1587 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6960 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 1588 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6966 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 1589 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6972 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 1593 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6978 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 1594 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6984 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 1595 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6990 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 1596 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 6996 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 1601 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7002 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 1602 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7008 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 1603 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7014 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 1604 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7020 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 1605 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7026 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 1606 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7032 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 1607 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7038 "bison-chapel.cpp" /* yacc.c:1646  */
    break;


#line 7042 "bison-chapel.cpp" /* yacc.c:1646  */
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
