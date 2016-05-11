/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

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
    TROTATELEFT = 378,
    TROTATERIGHT = 379,
    TSEMI = 380,
    TSHIFTLEFT = 381,
    TSHIFTRIGHT = 382,
    TSTAR = 383,
    TSWAP = 384,
    TIO = 385,
    TLCBR = 386,
    TRCBR = 387,
    TLP = 388,
    TRP = 389,
    TLSBR = 390,
    TRSBR = 391,
    TNOELSE = 392,
    TUPLUS = 393,
    TUMINUS = 394
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

#line 409 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 415 "bison-chapel.cpp" /* yacc.c:358  */
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

#line 481 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   11780

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  140
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  108
/* YYNRULES -- Number of rules.  */
#define YYNRULES  442
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  842

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   394

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
     135,   136,   137,   138,   139
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   418,   418,   423,   424,   430,   431,   436,   437,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   471,   473,   478,   479,   480,
     495,   496,   501,   502,   507,   512,   517,   521,   527,   528,
     529,   533,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   555,   556,   560,
     564,   565,   569,   570,   574,   575,   579,   580,   581,   582,
     583,   584,   585,   589,   590,   594,   609,   610,   611,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   631,   637,   643,   649,   656,   666,   670,
     671,   672,   673,   677,   682,   683,   687,   689,   696,   701,
     709,   714,   719,   727,   728,   733,   734,   736,   741,   751,
     760,   764,   772,   773,   778,   783,   777,   808,   814,   821,
     829,   840,   846,   839,   874,   878,   883,   887,   895,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   941,   942,
     946,   950,   951,   952,   956,   958,   960,   962,   967,   968,
     972,   973,   974,   975,   976,   977,   978,   979,   980,   984,
     985,   986,   987,   991,   992,   996,   997,   998,   999,  1000,
    1001,  1005,  1006,  1010,  1011,  1015,  1017,  1022,  1023,  1027,
    1028,  1032,  1033,  1037,  1039,  1041,  1046,  1059,  1076,  1077,
    1079,  1084,  1092,  1100,  1108,  1117,  1127,  1128,  1129,  1133,
    1134,  1142,  1144,  1150,  1155,  1157,  1159,  1164,  1166,  1168,
    1175,  1176,  1177,  1181,  1182,  1187,  1188,  1189,  1190,  1210,
    1214,  1218,  1226,  1230,  1231,  1232,  1236,  1238,  1244,  1246,
    1248,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1265,  1266,
    1267,  1268,  1272,  1273,  1274,  1278,  1279,  1283,  1284,  1288,
    1289,  1293,  1294,  1295,  1296,  1297,  1301,  1312,  1313,  1314,
    1315,  1316,  1317,  1319,  1321,  1323,  1325,  1327,  1329,  1334,
    1336,  1338,  1340,  1342,  1344,  1346,  1348,  1350,  1352,  1354,
    1356,  1358,  1365,  1371,  1377,  1383,  1392,  1397,  1405,  1406,
    1407,  1408,  1409,  1410,  1411,  1412,  1417,  1418,  1422,  1426,
    1428,  1436,  1447,  1448,  1452,  1453,  1458,  1463,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1479,  1481,  1483,  1485,
    1487,  1489,  1494,  1495,  1496,  1497,  1508,  1509,  1513,  1514,
    1515,  1519,  1520,  1521,  1529,  1530,  1531,  1535,  1536,  1537,
    1538,  1539,  1540,  1541,  1542,  1549,  1550,  1554,  1555,  1556,
    1557,  1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,
    1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,
    1577,  1578,  1582,  1583,  1584,  1585,  1586,  1587,  1591,  1592,
    1593,  1594,  1598,  1599,  1600,  1601,  1606,  1607,  1608,  1609,
    1610,  1611,  1612
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
  "TOR", "TPLUS", "TQUESTION", "TROTATELEFT", "TROTATERIGHT", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR",
  "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUPLUS", "TUMINUS",
  "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls",
  "stmt", "module_decl_stmt", "access_control", "block_stmt", "stmt_ls",
  "only_ls", "use_stmt", "require_stmt", "assignment_stmt", "opt_ident",
  "ident", "opt_ident_or_string", "do_stmt", "return_stmt",
  "class_level_stmt", "private_decl", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "select_stmt", "when_stmt_ls",
  "when_stmt", "class_decl_stmt", "class_tag", "opt_inherit",
  "class_level_stmt_ls", "enum_decl_stmt", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@1", "$@2", "linkage_spec", "fn_decl_stmt", "$@3",
  "$@4", "fn_decl_stmt_inner", "fn_ident", "assignop_ident",
  "opt_formal_ls", "req_formal_ls", "formal_ls", "formal",
  "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394
};
# endif

#define YYPACT_NINF -719

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-719)))

#define YYTABLE_NINF -394

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -719,   110,  2358,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  3465,    62,   164,  -719,    62,  6481,   230,   164,  6481,
    3465,   177,   164,   211,   511,  5431,  6481,  6481,   185,  -719,
     164,  -719,    29,  3465,  6481,  6481,  -719,  6481,  6481,   314,
     246,   735,   939,  -719,  5641,  5746,  6481,  5851,  6481,   274,
     250,  3465,   164,  -719,  5641,  6481,  6481,  -719,  -719,  6481,
    -719,  -719,  7426,  6481,  6481,  -719,  6481,  -719,  -719,  2667,
    5219,  5641,  -719,  3332,  -719,  -719,   333,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
     164,  -719,   268,    41,  -719,  -719,  -719,   254,   270,  -719,
    -719,  -719,   293,   298,   276,   300,   301, 11550,  7612,   -47,
     302,   304,  -719,  -719,  -719,  -719,  -719,  -719,   227,  -719,
   11550,   299,  3465,  -719,   325,  -719,   311,  6481,  6481,  6481,
    6481,  6481,  5641,  5641,   159,  -719,  -719,  -719,  -719,  9031,
     201,  -719,  -719,   164,   334,  9295,   365,  5641,   335,  -719,
    -719,  -719,  -719,   164,   131,   164,   329,    51,  8001,  7967,
    8174,  5641,  3465,   332,    16,   327,    22,  -719,  -719,   451,
     358,  8239,   451,  -719,  5641,  -719,   317,  -719,  -719,   164,
    -719,   -17, 11550,  -719,  9329,  8854,  3465,  -719,  -719,  8239,
   11550,   336,  5641,  -719, 11550,   390,   351,    88,  9065,  8239,
    9414,   253,  1166,   451,   253,   451,  -719,  -719,  2800,    87,
    -719,  6481,  -719,   -42,   -21, 11550,     5,  9472,    -7,   470,
    -719,   164,   377,  -719,  -719,  -719,    35,    29,    29,    29,
    -719,  6481,  6481,  6481,  6481,  5956,  5956,  6481,  6481,  6481,
    6481,  6481,  6481,    68,  7426,  6481,  6481,  6481,  6481,  6481,
    6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,
    6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,
    6481,  6481,  6481,  6481,  6481,  6481,  6481,  5641,  5641,  5956,
    5956,  5114,  -719,  -719,  2933,  -719,  9180,  9214,  9537,    -4,
    5956,    51,   360,  -719,  -719,  6481,   123,  -719,   352,   384,
    -719, 11550,   164,   364,   164,   456,  5641,  -719,  3598,  5956,
    -719,  3731,  5956,  -719,    51,  3465,   465,   362,  -719,    43,
    -719,  -719,    16,  -719,   396,   367,  -719,  2128,   419,   420,
    6481,    29,  -719,   375,  -719,  -719,  5641,  -719,  -719,  -719,
    -719,  -719,  5641,   376,  4902,   408,  -719,  -719,  6481,  6481,
    -719,  -719,  -719,  -719,  -719,  7852,  5326,  -719,  5536,  -719,
    5956,  2534,   380,  6481,  6481,  -719,  -719,   388,  5641,   401,
     347,    29,   168,   171,   184,   186,  8912,  1301,  1301,   278,
    -719,   278,  -719,   278,  2406,  1252,   535,  1729,   358,   253,
    -719,  -719,  -719,  1166, 11652,   278,   766,   766,  1301,   766,
     766,  1838,   253, 11652,  2214,  1838,   451,   451,   824,   824,
     253,  9595,  9665,  9723,  9793,  9851,  9921,  9979, 10049, 10107,
   10177, 10235, 10305, 10363, 10433, 10491,   399,   398,  -719,   278,
    -719,   278,   114,  -719,  -719,  -719,  -719,  -719,  -719,   164,
      -3,  -719, 11608,   356,  6586,  5956,  6691,  5956,  6481,  5956,
    1515,    62, 10561,  -719,  -719, 10619,  6481,  5641,  -719,  5641,
     450,   167,  -719,  -719,   377,  6481,     3,  6481, 11550,    52,
    8297,  6481, 11550,    48,  8116,  -719,   510, 10689,  3465,  -719,
      70,  -719,    38,  -719,   319,   403,    16,    69,  -719,  5641,
    -719,   244,  6481,  6061,  -719, 11550,  -719,  -719,  -719, 11550,
      56,   407,  -719,  5641,  -719,   210,   164,   240, 10747,   282,
    -719,  -719,  -719,  -719,     2,  1565,  -719, 11550,  3465, 11550,
   10812,  3066,   440,  -719,  -719,  -719,  -719,  -719,  7561,   288,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  5114,  -719,  5956,  5956,  6481,   524,
   10870,  6481,   527, 10935,   416,  7671,    51,    51,  -719, 11550,
    -719, 11550,  -719,  6481,   164,  -719,   422,  8239,  -719,  8362,
    3864,  -719,  3997,  -719,  8420,  4130,  -719,    51,  4263,  -719,
      51,  3465,  6481,  -719,   689,  -719,    16,   452,   498,  -719,
    -719,  -719,    27,  -719,  -719,  4902,   420,    57, 11550,  -719,
   11550,  3465,  5641,  -719,  -719,  -719,    80,  -719,  6481,  -719,
    6481,  -719,  4396,   424,  4529,   430,  -719,  6481,  -719,  3199,
     468,   319,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,   164,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
     461,   447,   447,  -719,  -719,    73,    81, 10993,  6796,  6901,
   11058,  7006,  7111,  7216,  7321,  -719,  -719, 11550,  -719,  -719,
    -719,  3465,  6481, 11550,  6481, 11550,  3465,  6481, 11550,  -719,
    6481, 11550,  -719,  -719, 11550,  -719,   439,  6166,   200,  -719,
     500,  -719,  5956,  4636,  -719,    49,  4769, 11116, 11550,  6481,
    -719, 11550,  3465,  6481,  -719, 11550,  3465, 11550,  -719,   937,
     942,   942,  -719,  1129,  -719,    21,  -719,  7561,    43,  -719,
    -719,  6481,  6481,  6481,  6481,  6481,  6481,  6481,  6481,  1032,
   10689,  8485,  8543, 10689,  8608,  8666,   452,   177,  6481,  6481,
    5007,  -719,  -719,   281,  5641,  -719,  -719,  6481,    -6,  7729,
    -719,   324,  -719,  6481,  8731,  -719,  8789,  -719,  -719,   500,
    -719,   447,   447,    98, 11181, 11239, 11304, 11362, 11427, 11485,
    -719,  3465,  3465,  3465,  3465,   200,  6271,   169,  -719,  -719,
   11550, 11550,  -719,  -719,  -719,  7321, 11550,  3465,  3465,    64,
    -719,  -719,  -719,  6481,  6481,  6481,  6481,  6481,  6481, 10689,
   10689, 10689, 10689,  -719,  -719,  -719,  -719,  -719,   313,  5956,
    7505,   445, 10689, 10689,  -719,  -719,  -719,  7794,  -719,  -719,
    6376,  -719
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    69,   387,   388,   389,   390,
     391,     0,   346,    67,   120,   346,     0,   247,    67,     0,
       0,     0,     0,    67,    67,     0,     0,     0,     0,   138,
       0,   134,     0,     0,     0,     0,   337,     0,     0,     0,
       0,   246,   246,   121,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,     0,     0,     0,   438,   440,     0,
     441,   442,   371,     0,     0,   439,   436,    76,   437,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   306,    19,    13,    77,    15,    17,    16,    18,    82,
       0,    80,   363,     0,    83,    81,    84,     0,   372,   359,
     360,   309,   307,     0,     0,   364,   365,     0,   308,     0,
     373,   374,   375,   358,   311,   310,   361,   362,     0,    21,
     317,     0,     0,   347,     0,    68,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   372,   307,   364,   365,   346,
     308,   373,   374,     0,     0,     0,     0,   297,     0,    71,
      70,   139,    85,     0,   140,     0,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,   249,    28,   424,
     356,     0,   425,     7,   297,   247,   248,    79,    78,   226,
     289,     0,   288,    74,     0,     0,     0,    73,    31,     0,
     312,     0,   297,    32,   318,   238,     0,     0,   288,     0,
       0,   427,   370,   423,   426,   422,    40,    42,     0,     0,
     292,     0,   294,     0,     0,   293,     0,   288,     0,     0,
       6,     0,   123,   214,   213,   141,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   297,   297,     0,
       0,     0,    22,    23,     0,    24,     0,     0,     0,     0,
       0,     0,     0,    25,    26,     0,   306,   304,     0,   298,
     299,   305,     0,     0,     0,     0,     0,    94,     0,     0,
      93,     0,     0,   100,     0,     0,   110,     0,    27,   191,
     135,   254,     0,   255,   257,     0,   268,     0,     0,   260,
       0,     0,    29,     0,   140,   225,     0,    51,    75,   114,
      72,    30,   297,     0,     0,   236,   233,    48,     0,     0,
      87,    33,    41,    43,   392,     0,     0,   384,     0,   386,
       0,     0,     0,     0,     0,   394,     8,     0,     0,     0,
     209,     0,     0,     0,     0,     0,   345,   419,   418,   421,
     429,   428,   433,   432,   415,   412,   413,   414,   367,   400,
     383,   382,   381,   368,   406,   417,   411,   409,   420,   410,
     408,   398,   405,   407,   416,   397,   403,   404,   401,   402,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   431,   430,
     435,   434,   203,   200,   201,   202,   206,   207,   208,     0,
       0,   349,     0,     0,     0,     0,     0,     0,     0,     0,
     393,   346,   346,    90,   234,     0,     0,     0,   314,     0,
     132,     0,   129,   235,   123,     0,     0,     0,   321,     0,
       0,     0,   327,     0,     0,   101,   109,     0,     0,   313,
       0,   192,     0,   199,   215,     0,   258,     0,   272,     0,
     267,   359,     0,     0,   251,   357,   250,   380,   291,   290,
       0,     0,   315,     0,   240,   359,     0,     0,    44,     0,
     366,   385,   295,   296,     0,     0,   106,   331,     0,   395,
       0,     0,   124,   125,   210,   211,   212,   142,     0,     0,
     242,   241,   243,   245,    52,    59,    60,    61,    56,    58,
      65,    66,    54,    57,    55,    53,    63,    62,    64,   378,
     379,   204,   205,   351,     0,   348,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86,   303,
     301,   302,   300,     0,   130,   128,     0,     0,   108,     0,
       0,    92,     0,    91,     0,     0,    98,     0,     0,    96,
       0,     0,     0,   112,   198,   190,     0,   281,   216,   219,
     218,   220,     0,   256,   259,     0,   260,     0,   252,   261,
     262,     0,     0,   113,   115,   316,     0,   237,     0,    49,
       0,    50,     0,     0,     0,     0,   107,     0,    35,     0,
     246,   215,   173,   171,   176,   183,   184,   185,   180,   182,
     178,   181,   179,   177,   187,   186,   150,   153,   151,   152,
     163,   154,   169,   159,   157,   172,   158,   156,   161,   168,
     170,   155,   160,   166,   167,   164,   165,   162,   174,   175,
     148,   188,   188,   244,   350,   372,   372,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    88,   133,   131,   125,
      95,     0,     0,   320,     0,   319,     0,     0,   326,    99,
       0,   325,    97,   111,   336,   193,     0,     0,   260,   217,
     231,   253,     0,     0,   117,     0,     0,    46,    45,     0,
     104,   333,     0,     0,   102,   332,     0,   396,    36,    67,
     246,   246,   118,   246,   126,     0,   149,     0,   191,   144,
     145,     0,     0,     0,     0,     0,     0,     0,     0,   246,
     324,     0,     0,   330,     0,     0,   281,   284,   285,   286,
       0,   283,   287,   359,   227,   195,   194,     0,     0,     0,
     270,   359,   116,     0,     0,   105,     0,   103,   127,   231,
     148,   188,   188,     0,     0,     0,     0,     0,     0,     0,
     119,     0,     0,     0,     0,   260,   273,     0,   228,   230,
     229,   232,   223,   224,   136,     0,    47,     0,     0,     0,
     146,   147,   189,     0,     0,     0,     0,     0,     0,   323,
     322,   329,   328,   197,   196,   275,   276,   278,   359,     0,
     393,   359,   335,   334,   221,   143,   222,     0,   277,   279,
     273,   280
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -719,  -719,  -719,     1,  -583,  1721,  -719,  -719,  1307,    61,
     235,  -719,  -719,  -719,   199,  1799,   -12,    -2,  -707,  -610,
     -40,  -719,  -719,   161,  -719,  -719,  -719,  -719,  -719,   432,
     125,   -99,  -719,  -719,    17,   331,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -145,  -141,  -622,  -719,  -144,     8,  -719,
    -271,  -719,  -719,   -34,  -719,  -211,     4,  -719,  -192,  -175,
    -719,  -136,  -719,  -719,  -719,  -161,   279,  -719,  -316,  -597,
    -719,  -458,  -340,  -629,  -718,  -142,    10,    66,  -719,  -143,
    -719,   153,   321,  -302,  -719,  -719,   655,  -719,     6,  -147,
    -719,    63,  -719,   718,   913,   -11,  1012,  -719,  1142,  1297,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -268
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   476,    75,    76,    77,   208,
     507,    78,    79,    80,   150,    81,   154,   188,    82,    83,
      84,    85,    86,   564,    87,    88,   500,   614,    89,    90,
     369,   630,    91,   461,   462,   134,   163,   484,    93,    94,
     370,   631,   527,   671,   672,   739,   320,   480,   481,   482,
     483,   528,   225,   602,   835,   804,   180,   799,   765,   768,
      95,   196,   345,    96,    97,   166,   167,   324,   325,   494,
     328,   329,   490,   826,   762,   708,   209,   213,   214,   298,
     299,   300,   135,    99,   100,   101,   136,   103,   122,   123,
     440,   441,   104,   137,   138,   107,   140,   109,   141,   142,
     112,   113,   218,   114,   115,   116,   117,   118
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     120,   177,   178,    72,   504,   139,   485,   292,   145,   711,
     439,   151,   314,   443,   158,   159,   160,   303,   317,     5,
     734,   126,   326,   169,   170,   491,   171,   172,   326,   606,
     449,   333,     5,   182,   184,   185,   189,   190,     5,   360,
     194,     5,   505,   198,   199,   200,  -198,   733,   201,   343,
     740,   202,   203,   204,   181,   205,   330,    45,   182,   215,
     217,   803,   356,   432,   197,   372,   373,   374,   375,   362,
     326,     5,   585,   186,   212,   186,   580,   433,   827,   223,
     434,   216,   121,   358,  -265,   121,   277,   336,   278,   321,
    -265,   712,   357,   390,   435,   436,   371,   364,   224,  -265,
     336,   554,   803,  -265,   437,  -265,   611,   336,   337,   336,
       3,   766,   839,   359,   712,   438,   120,   286,   287,   288,
     194,   182,   217,   778,   605,   284,   331,    45,   121,   365,
     605,   555,   450,   612,   426,   427,   301,   578,   622,   734,
     391,   361,   121,   289,   710,   291,  -265,    14,   551,   322,
     301,   297,  -265,   336,  -265,   307,   310,   313,  -265,   810,
     811,   336,   164,   301,   334,   297,   733,     5,   164,   332,
     604,   596,   605,  -265,   594,   552,  -198,  -352,   297,   284,
     284,   301,   284,   284,   336,  -353,   157,   341,   613,   834,
      43,   336,   336,   713,  -265,   284,   297,   350,   824,   501,
     355,   838,   594,   829,   595,    53,   456,  -352,   457,   353,
     529,   841,   124,   347,     5,  -353,   716,   144,   149,   354,
     376,   377,   378,   379,   381,   383,   384,   385,   386,   387,
     388,   389,   812,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   301,   301,   429,   431,
     442,   574,   331,   336,   226,   331,   234,   779,  -266,   452,
     706,   297,   297,   439,   455,   493,   443,   279,   331,   453,
     331,   280,  -377,   530,  -377,   182,   531,   468,   470,   575,
     472,   474,   143,   491,   477,   830,   227,  -266,   764,   532,
     147,   533,   475,   235,  -239,   228,   466,   236,   161,   495,
       5,   173,  -266,    98,   149,   499,   587,   590,   326,  -266,
     229,   301,    98,    92,  -376,  -239,  -376,   508,   508,   598,
     498,    98,    92,   191,   618,   215,   297,   215,  -266,   515,
     517,    92,   519,   520,    98,   -67,   241,   182,  -269,   243,
     212,  -265,   212,   246,    92,   619,  -282,   623,   625,  -266,
     617,   599,    98,   770,   -67,  -266,   770,   221,   522,   174,
     600,   241,    92,   192,   243,  -282,   618,  -269,   246,  -282,
      98,   601,   331,  -342,    98,  -339,   380,   382,  -274,   524,
      92,  -377,  -269,  -377,    92,   763,   231,   621,   525,  -269,
    -263,   771,  -265,   673,   771,  -282,   557,  -274,  -338,   526,
     280,  -274,   512,   230,   513,  -343,  -344,  -341,  -269,  -340,
     327,  -265,   281,   468,   560,   472,   563,   477,   565,   517,
     428,   430,   284,    98,   295,   569,   571,  -274,   301,  -269,
     283,   451,  -265,    92,   577,  -269,   579,   566,   567,   293,
     584,   570,   306,   297,   243,   319,   302,   581,   583,   342,
     469,   586,   589,   473,   234,   344,   346,   366,   217,  -271,
     368,   608,   610,    98,    14,   454,   458,    17,   459,   463,
     465,   478,   217,    92,   828,    22,   479,    23,   729,   607,
     486,   487,   492,   831,    29,   493,  -137,    98,  -271,   497,
     502,   235,   506,   616,     5,   236,   518,    92,   149,   521,
     152,   514,    39,  -271,   730,  -137,   731,    43,   828,    98,
    -271,  -248,   523,   549,   550,   573,   591,   603,   828,    92,
      52,   615,    53,   442,   336,   381,   429,   677,   678,  -271,
     680,   681,   683,   689,   241,   707,   242,   243,   234,   709,
     722,   246,   687,  -248,   685,   686,   726,   737,   253,   693,
    -271,   695,  -248,   756,   698,   690,  -271,   701,   767,   261,
     738,   704,   629,   153,   509,   699,   304,  -248,   702,   576,
     749,   688,   781,    67,   783,   235,   782,   735,   836,   236,
     732,   182,   705,   823,   809,    98,   559,   717,   562,   718,
     496,   721,   572,   725,   795,    92,   727,   674,     0,     0,
       0,     0,   715,     0,     0,     0,     0,     0,     0,    98,
     353,     0,    98,     0,   238,     0,    98,   240,   241,    92,
     242,   243,    92,     0,     0,   246,    92,     0,     0,     0,
       0,     0,   253,     0,     0,     0,     0,   102,   257,   258,
       0,   259,   260,   261,     0,     0,   102,   693,   695,     0,
     698,   701,   721,   725,     0,   102,     0,     0,     0,     0,
     750,   751,    98,   752,     0,   753,   754,     0,   102,   755,
     177,   178,    92,     0,     0,     0,     0,     0,     0,     0,
       0,   769,   517,     0,     0,   517,   102,     0,   774,   432,
       0,   761,   776,   772,     0,     0,     0,   380,   428,     0,
     105,     0,     0,   433,   102,     0,   434,     0,   102,   105,
     750,   784,   785,   753,   786,   787,   788,   789,   105,     0,
     435,   436,     0,     0,     0,     0,     0,   190,   194,   217,
     437,   105,     0,   800,   175,     0,   801,     0,     0,     0,
     553,   438,   806,     0,    23,   176,     0,     0,   798,   105,
     797,    29,     0,  -137,     0,     0,     0,   102,     0,   -39,
     819,   820,   821,   822,     0,     0,     0,   105,     0,   234,
       0,   105,  -137,     0,   725,     0,   832,   833,     0,    98,
     825,     0,   819,   820,   821,   822,   832,   833,     0,    92,
       0,     0,     0,     0,     0,     0,     0,   102,   837,   517,
       0,     0,     0,     0,     0,     0,   235,     0,     0,   725,
     236,     0,     0,     0,   825,     0,     0,     0,     0,    98,
     105,   102,    98,     0,   825,     0,     0,   234,     0,    92,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,     0,   246,   675,   676,     0,
     105,     0,   252,   253,   235,     0,     0,   256,   236,   257,
     258,     0,   259,   260,   261,     0,     0,     0,     0,     0,
       0,    98,     0,    98,   105,     0,    98,     0,     0,    98,
       0,    92,    98,    92,     0,   106,    92,     0,     0,    92,
       0,     0,    92,     0,   106,     0,   105,   241,     0,   242,
     243,     0,    98,   106,   246,     0,     0,     0,     0,   102,
       5,   253,    92,    98,   149,    98,   106,   257,   258,     0,
      98,     0,   261,    92,     0,    92,     0,  -248,   175,     0,
      92,   175,     0,   102,   106,     0,   102,     0,    23,   176,
     102,    23,   176,     0,     0,    29,     0,  -137,    29,     0,
    -137,     0,   106,   -38,     0,     0,   106,     0,     0,  -248,
       0,     0,     0,     0,     0,     0,  -137,     0,  -248,  -137,
       0,     0,   105,     0,     0,     0,     0,     0,     0,   153,
       0,     0,    98,  -248,   108,     0,   102,    98,     0,     0,
       0,     0,    92,   108,     0,     0,   105,    92,     0,   105,
       0,     0,   108,   105,     0,   106,     0,     0,     0,     0,
       0,     0,     0,    98,     0,   108,     0,    98,    14,     0,
       0,    17,     0,    92,     0,     0,     0,    92,     0,    22,
       0,    23,   729,   108,     0,     0,     0,     0,    29,     0,
    -137,     0,     0,     0,     0,   106,     0,     0,     0,   105,
       0,   108,     0,     0,     0,   108,    39,     0,   730,  -137,
     731,    43,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,    52,     0,    53,     0,     0,     0,
       0,     0,    98,    98,    98,    98,     0,     0,     0,     0,
       0,   106,    92,    92,    92,    92,     0,     0,    98,    98,
       0,     0,     0,   102,   108,     0,     0,     0,    92,    92,
       0,     0,     0,     0,   110,    14,     0,     0,    17,     0,
       0,     0,     0,   110,     0,     0,    22,    67,    23,   729,
       0,     0,   110,     0,   790,    29,     0,  -137,     0,     0,
       0,     0,     0,   102,   108,   110,   102,     0,     0,     0,
       0,     0,     0,   219,     0,   730,  -137,   731,    43,   234,
       0,     0,     0,   110,     0,     0,   105,   106,   108,     0,
       0,    52,     0,    53,     0,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,   110,     0,     0,     0,     0,
     108,   106,     0,     0,   106,     0,   235,     0,   106,     0,
     236,     0,     0,     0,     0,   102,   105,   102,     0,   105,
     102,     0,     0,   102,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,   238,   102,   239,   240,   241,
       0,   242,   243,     0,   106,   234,   246,   102,     0,   102,
       0,     0,   252,   253,   102,     0,     0,   256,     0,   257,
     258,     0,   259,   260,   261,     0,   108,     0,   105,   111,
     105,     0,     0,   105,   110,     0,   105,     0,   111,   105,
       0,     0,   235,     0,     0,     0,   236,   111,     0,     0,
     108,     0,     0,   108,   234,     0,     0,   108,   110,   105,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,     0,   105,     0,     0,     0,   102,   105,   111,     0,
     110,   102,     0,     0,   187,   241,     0,   242,   243,     0,
       0,   235,   246,     0,     0,   236,   111,     0,     0,   253,
     111,     0,     0,   108,     0,   257,   258,   102,   259,   260,
     261,   102,     0,     0,     0,   237,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   105,
     245,   246,   247,   248,   105,   250,   251,   252,   253,   111,
     254,   255,   256,     0,   257,   258,   110,   259,   260,   261,
       0,   106,     0,   285,   106,     0,     0,     0,     0,     0,
     105,     0,     0,     0,   105,     0,   102,   102,   102,   102,
     110,     0,     0,   110,     0,     0,     0,   110,     0,   111,
       0,     0,   102,   102,   187,   187,   187,   316,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
     108,     0,     0,   106,     0,   106,   187,     0,   106,     0,
       0,   106,     0,   110,   106,   111,   187,     0,     0,   105,
     105,   105,   105,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,   106,   105,   105,   127,     0,     0,
     108,     0,     0,   108,     0,   106,     0,   106,     0,     0,
      21,     0,   106,     0,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,   111,    48,    49,    50,   131,     0,     0,   234,     0,
       0,     0,   108,     0,   108,     0,     0,   108,   187,     0,
     108,     0,     0,   108,   106,   111,     0,     0,   111,   106,
       0,     0,   111,     0,     0,    59,     0,     0,     0,     0,
     110,   187,     0,   108,     0,   235,     0,     0,     0,   236,
       0,     0,     0,    64,   108,   106,   108,     0,     0,   106,
       0,   108,     0,     0,     0,   121,     0,     0,    70,   237,
     133,     0,     0,     0,     0,     0,     0,     0,   111,     0,
     110,     0,     0,   110,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   257,   258,
       0,   259,   260,   261,     0,     0,     0,     0,     0,     0,
       0,   624,     0,   108,   106,   106,   106,   106,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,   106,   110,    74,   110,     0,     0,   110,     0,     0,
     110,     0,   119,   110,   108,     0,     0,     0,   108,     0,
       0,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,   110,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,     0,   110,     0,     0,     0,
       0,   110,   193,     0,     0,   111,   187,   187,     0,     0,
     187,   187,     0,     0,     0,     0,     0,     0,     0,   235,
      74,     0,     0,   236,   220,     0,     0,     0,     0,     0,
       0,     0,     0,   108,   108,   108,   108,     0,     0,     0,
       0,     0,   125,     0,     0,   111,     0,   125,   111,   108,
     108,   148,   125,   125,     0,     0,     0,     0,   238,   162,
       0,   165,   241,   110,   242,   243,     0,     0,   110,   246,
       0,     0,     0,   282,     0,     0,   253,     0,     0,     0,
       0,   195,   257,   258,     0,   259,   260,   261,     0,     0,
       0,   234,     0,     0,   110,     0,     0,     0,   110,     0,
       0,     0,     0,   187,   187,     0,     0,   111,     0,   111,
       0,     0,   111,   318,   187,   111,   316,     0,   111,   222,
       0,   316,     0,     0,   187,     0,     0,   187,   235,     0,
       0,     0,   236,     0,     0,     0,     0,   340,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,   111,     0,     0,     0,     0,   111,     0,     0,    74,
       0,     0,     0,   110,   110,   110,   110,   238,     0,   239,
     240,   241,   195,   242,   243,     0,   296,     0,   246,   110,
     110,     0,   195,     0,   305,   253,     0,     0,     0,     0,
     296,   257,   258,   323,   259,   260,   261,     0,     0,     0,
       0,     0,     0,   296,     0,   125,     0,     0,   335,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,     0,
       0,   296,     0,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
     367,     0,   187,   111,     0,   165,   165,   165,   165,   340,
       0,     0,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   392,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,   316,
       0,   316,   316,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   802,   296,   296,     0,     0,
       0,   316,   516,   316,     0,     0,     0,     0,   111,   111,
     111,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   460,     0,   464,   111,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   802,     0,     0,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,   488,
     165,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,   296,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
     165,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,   593,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,   234,     0,   626,
       0,     0,    74,     0,    63,     0,    64,     0,    65,    66,
       0,     0,     0,     0,     0,     0,    68,     0,   296,   132,
       0,    70,     0,   489,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,   236,     0,
       0,   597,     0,     0,     0,   323,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,   340,     0,   340,     0,   195,   340,     0,     0,   340,
       0,     0,   703,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   670,   250,   251,
     252,   253,   714,   254,     0,   256,     0,   257,   258,     0,
     259,   260,   261,   720,     0,   724,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,    -2,     4,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,   460,    14,    15,    16,    17,  -246,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,   323,  -137,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -246,    38,    39,    40,    41,  -137,    42,    43,     0,  -246,
      44,    45,     0,    46,    47,    48,    49,    50,    51,   234,
      52,     0,    53,    54,  -246,     0,     0,    55,     0,    56,
       0,     0,    57,   775,     0,     0,   736,   777,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,   235,     0,     0,     0,
     236,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,     0,     0,    67,     0,     0,    68,     0,     0,    69,
       0,    70,     0,    71,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,     0,
     250,   251,   252,   253,     0,   254,     0,   256,   125,   257,
     258,     0,   259,   260,   261,     4,   780,     5,     6,     7,
       8,     9,    10,     0,  -393,     0,    11,    12,    13,  -393,
      14,    15,    16,    17,  -246,    18,    19,  -393,    20,    21,
    -393,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -137,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -246,    38,     0,    40,
      41,  -137,    42,    43,  -393,  -246,    44,    45,  -393,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -246,     0,     0,    55,     0,    56,     0,     0,  -393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -393,    59,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,     0,  -393,  -393,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,    64,  -393,  -393,  -393,     0,  -393,  -393,    67,
    -393,  -393,  -393,     0,     0,    69,  -393,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -246,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -137,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -246,
      38,    39,    40,    41,  -137,    42,    43,     0,  -246,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -246,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,   179,
       0,     0,    67,     0,     0,    68,     0,     0,    69,   206,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -246,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -137,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -246,    38,    39,    40,    41,  -137,    42,    43,
       0,  -246,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -246,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,     0,     0,    67,     0,     0,    68,     0,
       0,    69,   352,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -246,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -137,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -246,    38,    39,    40,    41,
    -137,    42,    43,     0,  -246,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -246,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,     0,     0,    67,     0,
       0,    68,     0,     0,    69,   206,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -246,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -137,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -246,    38,
      39,    40,    41,  -137,    42,    43,     0,  -246,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -246,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,     0,
       0,    67,     0,     0,    68,     0,     0,    69,   628,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -246,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -137,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -246,    38,    39,    40,    41,  -137,    42,    43,     0,
    -246,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -246,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,     0,     0,    67,     0,     0,    68,     0,     0,
      69,   728,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -246,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -137,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -246,    38,   219,    40,    41,  -137,
      42,    43,     0,  -246,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -246,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,     0,     0,    67,     0,     0,
      68,     0,     0,    69,     0,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -246,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -137,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -246,    38,     0,
      40,    41,  -137,    42,    43,     0,  -246,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -246,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,     0,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -246,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   467,     0,    28,    29,     0,  -137,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -246,    38,     0,    40,    41,  -137,    42,    43,     0,  -246,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -246,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,     0,     0,    67,     0,     0,    68,     0,     0,    69,
       0,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -246,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   471,     0,    28,    29,     0,  -137,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -246,    38,     0,    40,    41,  -137,    42,
      43,     0,  -246,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -246,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,     0,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -246,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   692,     0,    28,
      29,     0,  -137,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -246,    38,     0,    40,
      41,  -137,    42,    43,     0,  -246,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -246,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,     0,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -246,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
     694,     0,    28,    29,     0,  -137,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -246,
      38,     0,    40,    41,  -137,    42,    43,     0,  -246,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -246,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
       0,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -246,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   697,     0,    28,    29,     0,  -137,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -246,    38,     0,    40,    41,  -137,    42,    43,
       0,  -246,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -246,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,     0,     0,    67,     0,     0,    68,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -246,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   700,     0,    28,    29,
       0,  -137,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -246,    38,     0,    40,    41,
    -137,    42,    43,     0,  -246,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -246,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,     0,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -246,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   719,
       0,    28,    29,     0,  -137,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -246,    38,
       0,    40,    41,  -137,    42,    43,     0,  -246,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -246,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,     0,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -246,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   723,     0,    28,    29,     0,  -137,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -246,    38,     0,    40,    41,  -137,    42,    43,     0,
    -246,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -246,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   488,     0,     5,
       6,     7,     8,     9,    10,    63,  -393,    64,   127,    65,
      66,  -393,     0,     0,    67,     0,     0,    68,     0,  -393,
      69,    21,    70,     0,    71,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,  -393,     0,     0,     0,
    -393,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -264,
    -393,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -393,    59,  -393,  -393,  -393,
       0,  -393,  -393,  -393,     0,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,    64,  -393,  -393,  -393,     0,  -393,
    -393,     0,  -393,  -393,  -393,     0,     0,   132,     0,    70,
     488,   503,     5,     6,     7,     8,     9,    10,     0,  -393,
       0,   127,     0,     0,  -393,     0,     0,     0,     0,     0,
       0,     0,  -393,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,  -393,
       0,     0,     0,  -393,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -393,    59,
    -393,  -393,  -393,     0,  -393,  -393,  -393,     0,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,  -393,  -393,    64,  -393,  -393,
    -393,     0,  -393,  -393,     0,  -393,  -393,  -393,     0,     0,
     132,     0,    70,   488,   503,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       5,     6,     7,     8,     9,    10,     0,     0,    63,   127,
      64,     0,    65,    66,     0,     0,     0,     0,     0,     0,
      68,     0,    21,   132,     0,    70,     0,   503,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     5,     6,     7,
       8,     9,    10,    63,     0,    64,   127,    65,    66,   179,
       0,     0,     0,     0,   432,    68,     0,     0,   132,    21,
      70,     0,   133,   796,     0,   128,   129,   130,   433,    28,
       0,   434,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,   435,   436,    38,     0,    40,
       0,     0,     0,     0,     0,   437,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,   438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     5,     6,     7,     8,     9,    10,     0,     0,
      63,   127,    64,     0,    65,    66,     0,     0,     0,     0,
       0,     0,    68,     0,    21,   132,     0,    70,     0,   133,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,   210,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   211,     0,     5,
       6,     7,     8,     9,    10,    63,     0,    64,   127,    65,
      66,   179,     0,     0,     0,     0,     0,    68,     0,     0,
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
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
     511,   133,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,   155,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   156,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,   210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,   179,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,   179,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,   183,     0,     0,    68,   186,    21,   132,     0,    70,
       0,   133,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,    69,     0,    70,     0,   133,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   156,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,   609,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,   757,   132,     0,    70,     0,   133,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   758,    49,    50,   759,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,   179,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   760,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,   179,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   760,   128,   129,   748,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,   179,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   558,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   561,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   742,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   743,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   745,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   746,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   747,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     5,     6,     7,     8,     9,    10,
       0,     0,    63,   127,    64,     0,    65,    66,     0,     0,
       0,     0,     0,     0,    68,     0,    21,   132,     0,    70,
       0,   133,   128,   129,   748,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     5,
       6,     7,     8,     9,    10,     0,     0,    63,   127,    64,
       0,    65,    66,     0,     0,     0,     0,     0,     0,    68,
       0,    21,   132,     0,    70,     0,   133,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
      21,     0,     0,     0,     0,     0,   128,   129,   130,     0,
      28,     0,    63,     0,    64,    31,    32,    66,    34,     0,
      35,    36,     0,     0,    68,     0,     0,   132,    38,    70,
      40,   133,     0,     0,     5,     0,     0,     0,     0,     0,
       0,   632,    48,    49,    50,   131,   633,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -273,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,     0,  -273,
       0,     0,     0,  -273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,     0,     0,   179,     0,     0,
       0,     0,     0,     0,     0,     0,   132,     0,    70,  -273,
     760,     0,     0,     0,     0,     0,   634,   635,   636,   637,
     638,   639,     0,     0,   640,   641,   642,   643,   644,   645,
     646,   647,   648,   649,     0,     0,   650,     0,     0,     0,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   660,
     661,   232,   662,     0,   663,   664,   233,   665,   666,   667,
     668,   669,     0,     0,   234,     0,     0,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,     0,   232,
       0,   276,  -354,     0,   233,  -376,     0,  -376,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,   257,   258,     0,   259,   260,   261,
       0,     0,     0,     0,   232,     0,     0,   684,     0,   233,
       0,     0,     0,   237,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   257,   258,   235,   259,   260,   261,   236,     0,
       0,     0,   232,     0,     0,   805,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   257,   258,     0,
     259,   260,   261,     0,     0,     0,     0,     0,     0,     0,
     840,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   257,   258,   232,   259,   260,
     261,     0,   233,     0,     0,     0,   510,     0,     0,     0,
     234,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,   308,     0,   235,     0,     0,
       0,   236,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   121,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,   236,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   237,   254,   255,   256,     0,
     257,   258,     0,   259,   260,   261,     0,     0,   284,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   257,   258,   232,   259,   260,   261,
       0,   233,   284,     0,     0,     0,     0,     0,     0,   234,
     588,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,   121,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   257,
     258,     0,   259,   260,   261,   315,     0,   284,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,   237,     0,
       0,     0,   234,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   257,   258,   235,
     259,   260,   261,   236,     0,   284,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,   582,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,   257,   258,     0,   259,   260,   261,     0,     0,
     284,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     257,   258,   235,   259,   260,   261,   236,     0,   284,     0,
     232,     0,     0,   691,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   257,   258,     0,   259,   260,
     261,   696,     0,   284,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,   237,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   257,   258,   235,   259,   260,   261,   236,
       0,   284,     0,   232,     0,     0,   791,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,   257,   258,
       0,   259,   260,   261,   792,     0,   284,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,   237,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   257,   258,   235,   259,
     260,   261,   236,     0,   284,     0,   232,     0,     0,   793,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   257,   258,     0,   259,   260,   261,   794,     0,   284,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
     237,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   257,
     258,   235,   259,   260,   261,   236,     0,   284,     0,   232,
       0,     0,   807,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,   257,   258,     0,   259,   260,   261,
     808,     0,   284,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,   237,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   257,   258,   235,   259,   260,   261,   236,     0,
     284,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   257,   258,     0,
     259,   260,   261,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   257,   258,     0,   259,   260,
     261,   232,  -355,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   290,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   235,     0,   348,     0,   236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,   349,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,   236,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   237,
     254,   255,   256,     0,   257,   258,     0,   259,   260,   261,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   257,   258,
     232,   259,   260,   261,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   446,     0,
     235,     0,     0,     0,   236,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,   236,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   237,   254,
     255,   256,     0,   257,   258,   232,   259,   260,   261,     0,
     233,     0,     0,   238,     0,   239,   240,   241,   234,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   257,   258,   232,
     259,   260,   261,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   235,     0,     0,     0,   236,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,   236,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   237,   254,   255,   256,     0,   257,   258,
     294,   259,   260,   261,   232,     0,     0,     0,   238,   233,
     239,   240,   241,     0,   242,   243,   244,   234,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   257,   258,   338,   259,   260,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,     0,   236,     0,
       0,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   257,   258,   351,
     259,   260,   261,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,   363,   237,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   257,   258,   235,   259,   260,
     261,   236,     0,     0,     0,   232,     0,     0,   448,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
     257,   258,     0,   259,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,   237,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   257,   258,
     534,   259,   260,   261,     0,   235,     0,     0,     0,   236,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,   257,   258,
     535,   259,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,   237,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   257,   258,   536,   259,
     260,   261,     0,   235,     0,     0,     0,   236,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,   257,   258,   537,   259,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,   237,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   257,   258,   538,   259,   260,   261,
       0,   235,     0,     0,     0,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,   257,   258,   539,   259,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,   237,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   257,   258,   540,   259,   260,   261,     0,   235,
       0,     0,     0,   236,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,   257,   258,   541,   259,   260,   261,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,   237,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     257,   258,   542,   259,   260,   261,     0,   235,     0,     0,
       0,   236,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
     257,   258,   543,   259,   260,   261,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,   237,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   257,   258,
     544,   259,   260,   261,     0,   235,     0,     0,     0,   236,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,   257,   258,
     545,   259,   260,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,   237,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   257,   258,   546,   259,
     260,   261,     0,   235,     0,     0,     0,   236,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,   257,   258,   547,   259,
     260,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,   237,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   257,   258,   548,   259,   260,   261,
       0,   235,     0,     0,     0,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,   121,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,   257,   258,     0,   259,   260,   261,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,   237,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   592,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   257,   258,   568,   259,   260,   261,     0,   235,
       0,     0,     0,   236,     0,     0,     0,   232,   620,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,   257,   258,     0,   259,   260,   261,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     257,   258,   235,   259,   260,   261,   236,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   679,   627,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   257,   258,     0,   259,   260,
     261,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,   237,     0,     0,     0,   234,   682,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   257,   258,   235,   259,   260,   261,   236,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,   257,   258,
       0,   259,   260,   261,   741,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,   237,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   257,   258,   235,   259,
     260,   261,   236,     0,     0,     0,   232,   773,     0,   744,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   257,   258,     0,   259,   260,   261,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
     237,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   257,
     258,   235,   259,   260,   261,   236,     0,     0,     0,   232,
       0,     0,   813,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,   257,   258,     0,   259,   260,   261,
     814,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,   237,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   257,   258,   235,   259,   260,   261,   236,     0,
       0,     0,   232,     0,     0,   815,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   257,   258,     0,
     259,   260,   261,   816,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,   237,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   257,   258,   235,   259,   260,
     261,   236,     0,     0,     0,   232,     0,     0,   817,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
     257,   258,     0,   259,   260,   261,   818,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,   237,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   257,   258,
     235,   259,   260,   261,   236,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   556,   254,
     255,   256,   236,   257,   258,   234,   259,   260,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   257,   258,     0,   259,   260,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,     0,   246,   247,   248,     0,   250,   251,   252,   253,
       0,     0,     0,   256,     0,   257,   258,     0,   259,   260,
     261
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   344,    16,   322,   143,    19,   606,
     281,    23,   159,   281,    25,    26,    27,   153,   161,     3,
     630,    15,     1,    34,    35,   327,    37,    38,     1,   487,
      34,   174,     3,    44,    45,    46,    47,    48,     3,    34,
      51,     3,   344,    54,    55,    56,     3,   630,    59,   192,
     672,    62,    63,    64,    44,    66,    34,    63,    69,    70,
      71,   768,   104,    20,    54,   226,   227,   228,   229,   216,
       1,     3,    24,    24,    70,    24,    24,    34,   796,    38,
      37,    71,    80,   104,    63,    80,   133,   104,   135,    73,
      63,    34,   134,    25,    51,    52,    61,   104,    57,    78,
     104,   104,   809,    34,    61,    78,    50,   104,   125,   104,
       0,   708,   830,   134,    34,    72,   127,   128,   129,   130,
     131,   132,   133,   733,   103,   131,   104,    63,    80,   136,
     103,   134,   136,    77,   277,   278,   147,   134,   136,   749,
      72,   136,    80,   133,   602,   139,   125,    16,    34,   133,
     161,   147,   131,   104,    85,   157,   158,   159,   131,   781,
     782,   104,   133,   174,   176,   161,   749,     3,   133,   171,
     486,   133,   103,   104,   104,    61,   133,   104,   174,   131,
     131,   192,   131,   131,   104,   104,    25,   189,   132,   125,
      59,   104,   104,   136,   125,   131,   192,   199,   795,   342,
     211,   830,   104,    34,   134,    74,    83,   134,    85,   208,
     371,   840,    13,   125,     3,   134,   136,    18,     7,   132,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   134,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   104,   104,   104,    20,   104,    23,   735,    34,   290,
     596,   277,   278,   554,   295,    85,   554,    60,   104,   291,
     104,    64,   133,   125,   135,   306,   125,   308,   309,   132,
     311,   312,    72,   605,   315,   136,    52,    63,   108,   125,
     133,   125,   314,    60,   104,    61,   306,    64,   133,   330,
       3,     7,    78,     2,     7,   336,   473,   474,     1,    85,
      76,   342,    11,     2,   133,   125,   135,   348,   349,    20,
     336,    20,    11,    69,   104,   356,   342,   358,   104,   360,
     361,    20,   363,   364,    33,    38,   103,   368,    34,   106,
     356,    34,   358,   110,    33,   125,    85,   514,   515,   125,
     506,    52,    51,   713,    57,   131,   716,    44,   368,   133,
      61,   103,    51,   133,   106,   104,   104,    63,   110,   108,
      69,    72,   104,   125,    73,   125,   235,   236,    85,    52,
      69,   133,    78,   135,    73,   707,   130,   125,    61,    85,
      83,   713,    85,   125,   716,   134,    60,   104,   125,    72,
      64,   108,   356,   125,   358,   125,   125,   125,   104,   125,
     103,   104,   133,   444,   445,   446,   447,   448,   449,   450,
     279,   280,   131,   122,    79,   456,   457,   134,   459,   125,
     125,   290,   125,   122,   465,   131,   467,   451,   452,   125,
     471,   457,   133,   459,   106,   133,   131,   469,   470,   133,
     309,   473,   474,   312,    23,    85,   125,     7,   489,    34,
     103,   492,   493,   162,    16,   125,   134,    19,   104,   125,
      34,    26,   503,   162,   796,    27,   134,    29,    30,   489,
     104,   134,    83,   805,    36,    85,    38,   186,    63,   134,
     134,    60,   104,   503,     3,    64,   136,   186,     7,   131,
       9,   360,    54,    78,    56,    57,    58,    59,   830,   208,
      85,    20,   131,   134,   136,    85,    26,   134,   840,   208,
      72,   134,    74,   554,   104,   556,   557,   558,    24,   104,
     561,    24,   136,   131,   103,   103,   105,   106,    23,    61,
     136,   110,   573,    52,   566,   567,   136,   106,   117,   580,
     125,   582,    61,   134,   585,   577,   131,   588,    78,   128,
     133,   592,   521,    72,   349,   587,   154,    76,   590,   464,
     689,   574,   737,   125,   738,    60,   737,   631,   809,    64,
     132,   612,   594,   795,   779,   284,   445,   618,   447,   620,
     331,   622,   459,   624,   756,   284,   627,   554,    -1,    -1,
      -1,    -1,   612,    -1,    -1,    -1,    -1,    -1,    -1,   308,
     629,    -1,   311,    -1,    99,    -1,   315,   102,   103,   308,
     105,   106,   311,    -1,    -1,   110,   315,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,     2,   123,   124,
      -1,   126,   127,   128,    -1,    -1,    11,   678,   679,    -1,
     681,   682,   683,   684,    -1,    20,    -1,    -1,    -1,    -1,
     691,   692,   361,   694,    -1,   696,   697,    -1,    33,   700,
     730,   731,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   712,   713,    -1,    -1,   716,    51,    -1,   719,    20,
      -1,   707,   723,   715,    -1,    -1,    -1,   556,   557,    -1,
       2,    -1,    -1,    34,    69,    -1,    37,    -1,    73,    11,
     741,   742,   743,   744,   745,   746,   747,   748,    20,    -1,
      51,    52,    -1,    -1,    -1,    -1,    -1,   758,   759,   760,
      61,    33,    -1,   764,    19,    -1,   767,    -1,    -1,    -1,
     439,    72,   773,    -1,    29,    30,    -1,    -1,   764,    51,
     760,    36,    -1,    38,    -1,    -1,    -1,   122,    -1,    44,
     791,   792,   793,   794,    -1,    -1,    -1,    69,    -1,    23,
      -1,    73,    57,    -1,   805,    -1,   807,   808,    -1,   478,
     796,    -1,   813,   814,   815,   816,   817,   818,    -1,   478,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   829,   830,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   840,
      64,    -1,    -1,    -1,   830,    -1,    -1,    -1,    -1,   518,
     122,   186,   521,    -1,   840,    -1,    -1,    23,    -1,   518,
      -1,    -1,   521,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,    -1,   110,   556,   557,    -1,
     162,    -1,   116,   117,    60,    -1,    -1,   121,    64,   123,
     124,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,
      -1,   580,    -1,   582,   186,    -1,   585,    -1,    -1,   588,
      -1,   580,   591,   582,    -1,     2,   585,    -1,    -1,   588,
      -1,    -1,   591,    -1,    11,    -1,   208,   103,    -1,   105,
     106,    -1,   611,    20,   110,    -1,    -1,    -1,    -1,   284,
       3,   117,   611,   622,     7,   624,    33,   123,   124,    -1,
     629,    -1,   128,   622,    -1,   624,    -1,    20,    19,    -1,
     629,    19,    -1,   308,    51,    -1,   311,    -1,    29,    30,
     315,    29,    30,    -1,    -1,    36,    -1,    38,    36,    -1,
      38,    -1,    69,    44,    -1,    -1,    73,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    61,    57,
      -1,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    -1,   691,    76,     2,    -1,   361,   696,    -1,    -1,
      -1,    -1,   691,    11,    -1,    -1,   308,   696,    -1,   311,
      -1,    -1,    20,   315,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   722,    -1,    33,    -1,   726,    16,    -1,
      -1,    19,    -1,   722,    -1,    -1,    -1,   726,    -1,    27,
      -1,    29,    30,    51,    -1,    -1,    -1,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,   162,    -1,    -1,    -1,   361,
      -1,    69,    -1,    -1,    -1,    73,    54,    -1,    56,    57,
      58,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    72,    -1,    74,    -1,    -1,    -1,
      -1,    -1,   791,   792,   793,   794,    -1,    -1,    -1,    -1,
      -1,   208,   791,   792,   793,   794,    -1,    -1,   807,   808,
      -1,    -1,    -1,   478,   122,    -1,    -1,    -1,   807,   808,
      -1,    -1,    -1,    -1,     2,    16,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    11,    -1,    -1,    27,   125,    29,    30,
      -1,    -1,    20,    -1,   132,    36,    -1,    38,    -1,    -1,
      -1,    -1,    -1,   518,   162,    33,   521,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    56,    57,    58,    59,    23,
      -1,    -1,    -1,    51,    -1,    -1,   478,   284,   186,    -1,
      -1,    72,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
     208,   308,    -1,    -1,   311,    -1,    60,    -1,   315,    -1,
      64,    -1,    -1,    -1,    -1,   580,   518,   582,    -1,   521,
     585,    -1,    -1,   588,    -1,    -1,   591,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    99,   611,   101,   102,   103,
      -1,   105,   106,    -1,   361,    23,   110,   622,    -1,   624,
      -1,    -1,   116,   117,   629,    -1,    -1,   121,    -1,   123,
     124,    -1,   126,   127,   128,    -1,   284,    -1,   580,     2,
     582,    -1,    -1,   585,   162,    -1,   588,    -1,    11,   591,
      -1,    -1,    60,    -1,    -1,    -1,    64,    20,    -1,    -1,
     308,    -1,    -1,   311,    23,    -1,    -1,   315,   186,   611,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     622,    -1,   624,    -1,    -1,    -1,   691,   629,    51,    -1,
     208,   696,    -1,    -1,    47,   103,    -1,   105,   106,    -1,
      -1,    60,   110,    -1,    -1,    64,    69,    -1,    -1,   117,
      73,    -1,    -1,   361,    -1,   123,   124,   722,   126,   127,
     128,   726,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,   478,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   691,
     109,   110,   111,   112,   696,   114,   115,   116,   117,   122,
     119,   120,   121,    -1,   123,   124,   284,   126,   127,   128,
      -1,   518,    -1,   126,   521,    -1,    -1,    -1,    -1,    -1,
     722,    -1,    -1,    -1,   726,    -1,   791,   792,   793,   794,
     308,    -1,    -1,   311,    -1,    -1,    -1,   315,    -1,   162,
      -1,    -1,   807,   808,   157,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
     478,    -1,    -1,   580,    -1,   582,   189,    -1,   585,    -1,
      -1,   588,    -1,   361,   591,   208,   199,    -1,    -1,   791,
     792,   793,   794,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   611,   807,   808,    12,    -1,    -1,
     518,    -1,    -1,   521,    -1,   622,    -1,   624,    -1,    -1,
      25,    -1,   629,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,   284,    67,    68,    69,    70,    -1,    -1,    23,    -1,
      -1,    -1,   580,    -1,   582,    -1,    -1,   585,   291,    -1,
     588,    -1,    -1,   591,   691,   308,    -1,    -1,   311,   696,
      -1,    -1,   315,    -1,    -1,   100,    -1,    -1,    -1,    -1,
     478,   314,    -1,   611,    -1,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,   118,   622,   722,   624,    -1,    -1,   726,
      -1,   629,    -1,    -1,    -1,    80,    -1,    -1,   133,    84,
     135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   361,    -1,
     518,    -1,    -1,   521,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   123,   124,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   691,   791,   792,   793,   794,   696,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     807,   808,   580,     2,   582,    -1,    -1,   585,    -1,    -1,
     588,    -1,    11,   591,   722,    -1,    -1,    -1,   726,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,   611,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   622,    -1,   624,    -1,    -1,    -1,
      -1,   629,    51,    -1,    -1,   478,   469,   470,    -1,    -1,
     473,   474,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      69,    -1,    -1,    64,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   791,   792,   793,   794,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    -1,   518,    -1,    18,   521,   807,
     808,    22,    23,    24,    -1,    -1,    -1,    -1,    99,    30,
      -1,    32,   103,   691,   105,   106,    -1,    -1,   696,   110,
      -1,    -1,    -1,   122,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    52,   123,   124,    -1,   126,   127,   128,    -1,    -1,
      -1,    23,    -1,    -1,   722,    -1,    -1,    -1,   726,    -1,
      -1,    -1,    -1,   566,   567,    -1,    -1,   580,    -1,   582,
      -1,    -1,   585,   162,   577,   588,   579,    -1,   591,    90,
      -1,   584,    -1,    -1,   587,    -1,    -1,   590,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,   186,   611,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   622,
      -1,   624,    -1,    -1,    -1,    -1,   629,    -1,    -1,   208,
      -1,    -1,    -1,   791,   792,   793,   794,    99,    -1,   101,
     102,   103,   143,   105,   106,    -1,   147,    -1,   110,   807,
     808,    -1,   153,    -1,   155,   117,    -1,    -1,    -1,    -1,
     161,   123,   124,   164,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,   174,    -1,   176,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   691,    -1,
      -1,   192,    -1,   696,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   722,
     221,    -1,   715,   726,    -1,   226,   227,   228,   229,   308,
      -1,    -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   751,   752,
      -1,   754,   755,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   768,   277,   278,    -1,    -1,
      -1,   774,   361,   776,    -1,    -1,    -1,    -1,   791,   792,
     793,   794,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   302,    -1,   304,   807,   808,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,    -1,    -1,    -1,
      -1,   322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
     331,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,   342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
     371,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,   478,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    23,    -1,   518,
      -1,    -1,   521,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   459,   131,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,   482,    -1,    -1,    -1,   486,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,   580,    -1,   582,    -1,   506,   585,    -1,    -1,   588,
      -1,    -1,   591,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   528,   114,   115,
     116,   117,   611,   119,    -1,   121,    -1,   123,   124,    -1,
     126,   127,   128,   622,    -1,   624,    -1,    -1,    -1,    -1,
     629,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,   574,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,   596,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    23,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,   722,    -1,    -1,   647,   726,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,   125,    -1,    -1,   128,    -1,    -1,   131,
      -1,   133,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
     114,   115,   116,   117,    -1,   119,    -1,   121,   729,   123,
     124,    -1,   126,   127,   128,     1,   737,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,    -1,   131,   132,   133,     1,   135,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,   122,
      -1,    -1,   125,    -1,    -1,   128,    -1,    -1,   131,   132,
     133,     1,   135,     3,     4,     5,     6,     7,     8,    -1,
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
     120,   121,    -1,    -1,    -1,   125,    -1,    -1,   128,    -1,
      -1,   131,   132,   133,     1,   135,     3,     4,     5,     6,
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
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,   125,    -1,
      -1,   128,    -1,    -1,   131,   132,   133,     1,   135,     3,
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
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,   125,    -1,    -1,   128,    -1,    -1,   131,   132,   133,
       1,   135,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,   125,    -1,    -1,   128,    -1,    -1,
     131,   132,   133,     1,   135,     3,     4,     5,     6,     7,
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
     118,    -1,   120,   121,    -1,    -1,    -1,   125,    -1,    -1,
     128,    -1,    -1,   131,    -1,   133,     1,   135,     3,     4,
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
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,
     125,    -1,    -1,   128,    -1,    -1,   131,    -1,   133,     1,
     135,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
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
      -1,    -1,    -1,   125,    -1,    -1,   128,    -1,    -1,   131,
      -1,   133,     1,   135,     3,     4,     5,     6,     7,     8,
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
      -1,   120,   121,    -1,    -1,    -1,   125,    -1,    -1,   128,
      -1,    -1,   131,    -1,   133,     1,   135,     3,     4,     5,
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
     116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,   125,
      -1,    -1,   128,    -1,    -1,   131,    -1,   133,     1,   135,
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
      -1,    -1,   125,    -1,    -1,   128,    -1,    -1,   131,    -1,
     133,     1,   135,     3,     4,     5,     6,     7,     8,    -1,
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
     120,   121,    -1,    -1,    -1,   125,    -1,    -1,   128,    -1,
      -1,   131,    -1,   133,     1,   135,     3,     4,     5,     6,
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
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,   125,    -1,
      -1,   128,    -1,    -1,   131,    -1,   133,     1,   135,     3,
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
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,   125,    -1,    -1,   128,    -1,    -1,   131,    -1,   133,
       1,   135,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,     1,    -1,     3,
       4,     5,     6,     7,     8,   116,    10,   118,    12,   120,
     121,    15,    -1,    -1,   125,    -1,    -1,   128,    -1,    23,
     131,    25,   133,    -1,   135,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,   123,
     124,    -1,   126,   127,   128,    -1,    -1,   131,    -1,   133,
       1,   135,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,   123,   124,    -1,   126,   127,   128,    -1,    -1,
     131,    -1,   133,     1,   135,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
       3,     4,     5,     6,     7,     8,    -1,    -1,   116,    12,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,    25,   131,    -1,   133,    -1,   135,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,     3,     4,     5,
       6,     7,     8,   116,    -1,   118,    12,   120,   121,   122,
      -1,    -1,    -1,    -1,    20,   128,    -1,    -1,   131,    25,
     133,    -1,   135,   136,    -1,    31,    32,    33,    34,    35,
      -1,    37,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    51,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,     3,     4,     5,     6,     7,     8,    -1,    -1,
     116,    12,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    25,   131,    -1,   133,    -1,   135,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,   108,    -1,     3,
       4,     5,     6,     7,     8,   116,    -1,   118,    12,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,   128,    -1,    -1,
     131,    25,   133,    -1,   135,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
     134,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,   125,    -1,    -1,   128,    24,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    47,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,     3,     4,     5,     6,     7,     8,
      -1,    -1,   116,    12,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,    25,   131,    -1,   133,
      -1,   135,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,     3,
       4,     5,     6,     7,     8,    -1,    -1,   116,    12,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    25,   131,    -1,   133,    -1,   135,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,   116,    -1,   118,    40,    41,   121,    43,    -1,
      45,    46,    -1,    -1,   128,    -1,    -1,   131,    53,   133,
      55,   135,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    67,    68,    69,    70,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,   104,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,    -1,   133,   134,
     135,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,    88,
      89,    90,    -1,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    10,   121,    -1,   123,   124,    15,   126,   127,   128,
     129,   130,    -1,    -1,    23,    -1,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    10,
      -1,   129,   130,    -1,    15,   133,    -1,   135,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,   123,   124,    -1,   126,   127,   128,
      -1,    -1,    -1,    -1,    10,    -1,    -1,   136,    -1,    15,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,    60,   126,   127,   128,    64,    -1,
      -1,    -1,    10,    -1,    -1,   136,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,   123,   124,    -1,
     126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     136,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,    10,   126,   127,
     128,    -1,    15,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    24,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    84,   119,   120,   121,    -1,
     123,   124,    -1,   126,   127,   128,    -1,    -1,   131,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,   123,   124,    10,   126,   127,   128,
      -1,    15,   131,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,   123,
     124,    -1,   126,   127,   128,    71,    -1,   131,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,   123,   124,    60,
     126,   127,   128,    64,    -1,   131,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    24,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,   123,   124,    -1,   126,   127,   128,    -1,    -1,
     131,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
     123,   124,    60,   126,   127,   128,    64,    -1,   131,    -1,
      10,    -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,   123,   124,    -1,   126,   127,
     128,    71,    -1,   131,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    84,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,   123,   124,    60,   126,   127,   128,    64,
      -1,   131,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,   123,   124,
      -1,   126,   127,   128,    71,    -1,   131,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,   123,   124,    60,   126,
     127,   128,    64,    -1,   131,    -1,    10,    -1,    -1,    71,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,   123,   124,    -1,   126,   127,   128,    71,    -1,   131,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,   123,
     124,    60,   126,   127,   128,    64,    -1,   131,    -1,    10,
      -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,   123,   124,    -1,   126,   127,   128,
      71,    -1,   131,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,    60,   126,   127,   128,    64,    -1,
     131,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,   123,   124,    -1,
     126,   127,   128,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,    -1,   126,   127,
     128,    10,   130,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    60,    -1,    28,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    49,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    84,
     119,   120,   121,    -1,   123,   124,    -1,   126,   127,   128,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   123,   124,
      10,   126,   127,   128,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    84,   119,
     120,   121,    -1,   123,   124,    10,   126,   127,   128,    -1,
      15,    -1,    -1,    99,    -1,   101,   102,   103,    23,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,   123,   124,    10,
     126,   127,   128,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    84,   119,   120,   121,    -1,   123,   124,
     125,   126,   127,   128,    10,    -1,    -1,    -1,    99,    15,
     101,   102,   103,    -1,   105,   106,   107,    23,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,   123,   124,   125,
     126,   127,   128,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    83,    84,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,    60,   126,   127,
     128,    64,    -1,    -1,    -1,    10,    -1,    -1,    71,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
     123,   124,    -1,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    84,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    84,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    60,   119,   120,   121,    64,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    84,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,   123,   124,   125,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    84,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,   123,   124,   125,   126,   127,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    84,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
     123,   124,   125,   126,   127,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    84,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,   123,   124,
     125,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    84,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    60,   119,   120,   121,    64,   123,   124,   125,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    84,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,   123,   124,    -1,   126,   127,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    84,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,   123,   124,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
     123,   124,    60,   126,   127,   128,    64,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    83,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,   123,   124,    -1,   126,   127,
     128,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    84,    -1,    -1,    -1,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,   123,   124,    60,   126,   127,   128,    64,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,   123,   124,
      -1,   126,   127,   128,    71,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,   123,   124,    60,   126,
     127,   128,    64,    -1,    -1,    -1,    10,    11,    -1,    71,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,   123,   124,    -1,   126,   127,   128,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,   123,
     124,    60,   126,   127,   128,    64,    -1,    -1,    -1,    10,
      -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,   123,   124,    -1,   126,   127,   128,
      71,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,   123,   124,    60,   126,   127,   128,    64,    -1,
      -1,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,   123,   124,    -1,
     126,   127,   128,    71,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,    60,   126,   127,
     128,    64,    -1,    -1,    -1,    10,    -1,    -1,    71,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
     123,   124,    -1,   126,   127,   128,    71,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   123,   124,
      60,   126,   127,   128,    64,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,   123,   124,    23,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,   123,   124,    -1,   126,   127,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,   127,
     128
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   141,   142,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    74,    75,    79,    81,    84,    99,   100,
     101,   102,   107,   116,   118,   120,   121,   125,   128,   131,
     133,   135,   143,   144,   145,   146,   147,   148,   151,   152,
     153,   155,   158,   159,   160,   161,   162,   164,   165,   168,
     169,   172,   175,   178,   179,   200,   203,   204,   222,   223,
     224,   225,   226,   227,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   243,   244,   245,   246,   247,   145,
     235,    80,   228,   229,   154,   155,   228,    12,    31,    32,
      33,    70,   131,   135,   175,   222,   226,   233,   234,   235,
     236,   238,   239,    72,   154,   235,   145,   133,   155,     7,
     154,   156,     9,    72,   156,    52,    82,   163,   235,   235,
     235,   133,   155,   176,   133,   155,   205,   206,   145,   235,
     235,   235,   235,     7,   133,    19,    30,   160,   160,   122,
     196,   216,   235,   125,   235,   235,    24,   148,   157,   235,
     235,    69,   133,   145,   235,   155,   201,   216,   235,   235,
     235,   235,   235,   235,   235,   235,   132,   143,   149,   216,
      73,   108,   196,   217,   218,   235,   216,   235,   242,    54,
     145,    44,   155,    38,    57,   192,    20,    52,    61,    76,
     125,   130,    10,    15,    23,    60,    64,    84,    99,   101,
     102,   103,   105,   106,   107,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   119,   120,   121,   123,   124,   126,
     127,   128,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,   129,   133,   135,    60,
      64,   133,   145,   125,   131,   148,   235,   235,   235,   216,
      34,   228,   201,   125,   125,    79,   155,   196,   219,   220,
     221,   235,   131,   201,   169,   155,   133,   157,    24,    34,
     157,    24,    34,   157,   229,    71,   148,   219,   145,   133,
     186,    73,   133,   155,   207,   208,     1,   103,   210,   211,
      34,   104,   157,   219,   156,   155,   104,   125,   125,   131,
     145,   157,   133,   219,    85,   202,   125,   125,    28,    49,
     157,   125,   132,   143,   132,   235,   104,   134,   104,   134,
      34,   136,   229,    83,   104,   136,     7,   155,   103,   170,
     180,    61,   205,   205,   205,   205,   235,   235,   235,   235,
     163,   235,   163,   235,   235,   235,   235,   235,   235,   235,
      25,    72,   155,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   219,   219,   163,   235,
     163,   235,    20,    34,    37,    51,    52,    61,    72,   190,
     230,   231,   235,   247,    24,    34,    24,    34,    71,    34,
     136,   163,   235,   157,   125,   235,    83,    85,   134,   104,
     155,   173,   174,   125,   155,    34,   216,    33,   235,   163,
     235,    33,   235,   163,   235,   157,   145,   235,    26,   134,
     187,   188,   189,   190,   177,   208,   104,   134,     1,   135,
     212,   223,    83,    85,   209,   235,   206,   134,   196,   235,
     166,   219,   134,   135,   212,   223,   104,   150,   235,   150,
     134,   134,   217,   217,   163,   235,   145,   235,   136,   235,
     235,   131,   216,   131,    52,    61,    72,   182,   191,   205,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   134,
     136,    34,    61,   222,   104,   134,    60,    60,    33,   163,
     235,    33,   163,   235,   163,   235,   228,   228,   125,   235,
     196,   235,   221,    85,   104,   132,   170,   235,   134,   235,
      24,   157,    24,   157,   235,    24,   157,   229,    24,   157,
     229,    26,    26,   145,   104,   134,   133,   155,    20,    52,
      61,    72,   193,   134,   208,   103,   211,   216,   235,    47,
     235,    50,    77,   132,   167,   134,   216,   201,   104,   125,
      11,   125,   136,   229,   136,   229,   145,    83,   132,   149,
     171,   181,    10,    15,    85,    86,    87,    88,    89,    90,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     105,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   121,   123,   124,   126,   127,   128,   129,   130,
     155,   183,   184,   125,   231,   222,   222,   235,    24,    24,
     235,    24,    24,   136,   136,   157,   157,   235,   174,   131,
     157,    71,    33,   235,    33,   235,    71,    33,   235,   157,
      33,   235,   157,   145,   235,   188,   208,   103,   215,    61,
     211,   209,    34,   136,   145,   216,   136,   235,   235,    33,
     145,   235,   136,    33,   145,   235,   136,   235,   132,    30,
      56,    58,   132,   144,   159,   193,   155,   106,   133,   185,
     185,    71,    33,    33,    71,    33,    33,    33,    33,   171,
     235,   235,   235,   235,   235,   235,   134,    25,    67,    70,
     135,   196,   214,   223,   108,   198,   209,    78,   199,   235,
     212,   223,   157,    11,   235,   145,   235,   145,   159,   211,
     155,   183,   184,   187,   235,   235,   235,   235,   235,   235,
     132,    71,    71,    71,    71,   215,   136,   216,   196,   197,
     235,   235,   148,   158,   195,   136,   235,    71,    71,   199,
     185,   185,   134,    71,    71,    71,    71,    71,    71,   235,
     235,   235,   235,   198,   209,   196,   213,   214,   223,    34,
     136,   223,   235,   235,   125,   194,   195,   235,   213,   214,
     136,   213
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   140,   141,   142,   142,   143,   143,   144,   144,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   146,   146,   147,   147,   147,
     148,   148,   149,   149,   150,   150,   150,   150,   151,   151,
     151,   152,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   154,   154,   155,
     156,   156,   157,   157,   158,   158,   159,   159,   159,   159,
     159,   159,   159,   160,   160,   161,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   163,   164,
     164,   164,   164,   165,   166,   166,   167,   167,   168,   168,
     169,   169,   169,   170,   170,   171,   171,   171,   172,   173,
     173,   173,   174,   174,   176,   177,   175,   178,   178,   178,
     178,   180,   181,   179,   182,   182,   182,   182,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   185,   185,
     186,   187,   187,   187,   188,   188,   188,   188,   189,   189,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   191,
     191,   191,   191,   192,   192,   193,   193,   193,   193,   193,
     193,   194,   194,   195,   195,   196,   196,   197,   197,   198,
     198,   199,   199,   200,   200,   200,   201,   201,   202,   202,
     202,   203,   203,   203,   203,   203,   204,   204,   204,   205,
     205,   206,   206,   206,   207,   207,   207,   208,   208,   208,
     209,   209,   209,   210,   210,   211,   211,   211,   211,   212,
     212,   212,   212,   213,   213,   213,   214,   214,   214,   214,
     214,   215,   215,   215,   215,   215,   215,   215,   216,   216,
     216,   216,   217,   217,   217,   218,   218,   219,   219,   220,
     220,   221,   221,   221,   221,   221,   222,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   225,   226,   227,   227,
     227,   227,   227,   227,   227,   227,   228,   228,   229,   230,
     230,   231,   231,   231,   232,   232,   233,   234,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   236,   236,   236,   236,   237,   237,   238,   238,
     238,   239,   239,   239,   240,   240,   240,   241,   241,   241,
     241,   241,   241,   241,   241,   242,   242,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   244,   244,   244,   244,   244,   244,   245,   245,
     245,   245,   246,   246,   246,   246,   247,   247,   247,   247,
     247,   247,   247
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
       1,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       3,     0,     1,     3,     4,     4,     6,     6,     0,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     0,     1,     2,
       2,     0,     2,     3,     4,     4,     2,     4,     0,     2,
       2,     4,     4,     4,     5,     4,     0,     1,     1,     1,
       3,     3,     4,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     0,     4,     0,     2,     2,     1,     4,
       4,     6,     1,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     1,     4,     1,
       3,     2,     3,     3,     1,     3,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     4,     3,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     5,     3,     3,     3,
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
#line 418 "chapel.ypp" /* yacc.c:1646  */
    { yyblock = (yyval.pblockstmt); }
#line 4522 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 423 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4528 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 424 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4534 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 431 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4540 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 436 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4546 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 437 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4552 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 453 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4558 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 454 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4564 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 455 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4570 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 456 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4576 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 457 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4582 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 458 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4588 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 459 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4594 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 460 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4600 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 461 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4606 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 462 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4612 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 463 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4618 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 464 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4624 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 465 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4630 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 466 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4636 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 467 "chapel.ypp" /* yacc.c:1646  */
    { printf("syntax error"); clean_exit(1); }
#line 4642 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 472 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4648 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 474 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4654 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 478 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4660 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 479 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4666 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 480 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4672 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 495 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4678 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 496 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4684 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 501 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4690 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 502 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 4696 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 507 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 4706 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 512 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 4716 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 517 "chapel.ypp" /* yacc.c:1646  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 4725 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 521 "chapel.ypp" /* yacc.c:1646  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 4734 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 527 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 4740 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 528 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 4746 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 529 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 4752 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 533 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 4758 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 537 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 4764 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 538 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 4770 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 539 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 4776 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 540 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 4782 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 541 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 4788 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 542 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 4794 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 543 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 4800 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 544 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 4806 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 545 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 4812 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 546 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 4818 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 547 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 4824 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 548 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 4830 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 549 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 4836 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 550 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 4842 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 551 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 4848 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 555 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = NULL; }
#line 4854 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 560 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4860 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 564 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4866 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 565 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 4872 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 569 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4878 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 570 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4884 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 574 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 4890 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 575 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 4896 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 579 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 4902 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 581 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4908 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 582 "chapel.ypp" /* yacc.c:1646  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 4914 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 595 "chapel.ypp" /* yacc.c:1646  */
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
#line 4930 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 609 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 4936 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 610 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4942 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 611 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4948 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 612 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 4954 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 613 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4960 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 614 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4966 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 615 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4972 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 616 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 4978 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 617 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 4984 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 618 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4990 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 619 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 4996 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 620 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5002 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 621 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt), true); }
#line 5008 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 622 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt), true); }
#line 5014 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 623 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5020 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 624 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr),   (yyvsp[0].pblockstmt)); }
#line 5026 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 626 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5036 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 632 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5046 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 638 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5056 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 644 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5066 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 650 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5077 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 657 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pcallexpr),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5088 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 666 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[-1].pcallexpr)); }
#line 5094 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 670 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5100 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 671 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5106 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 672 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5112 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 673 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5118 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 678 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5124 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 682 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5130 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 683 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5136 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 688 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5142 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 690 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5148 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 697 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5157 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 702 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5166 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 709 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5176 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 714 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5186 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 719 "chapel.ypp" /* yacc.c:1646  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5196 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 727 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = NULL; }
#line 5202 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 728 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5208 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 733 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5214 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 735 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5220 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 737 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5226 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 742 "chapel.ypp" /* yacc.c:1646  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5237 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 752 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5250 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 761 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5258 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 765 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5267 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 772 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5273 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 773 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5279 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 778 "chapel.ypp" /* yacc.c:1646  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5288 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 783 "chapel.ypp" /* yacc.c:1646  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5297 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 788 "chapel.ypp" /* yacc.c:1646  */
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
#line 5317 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 808 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5328 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 814 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5340 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 821 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5353 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 829 "chapel.ypp" /* yacc.c:1646  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5366 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 840 "chapel.ypp" /* yacc.c:1646  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5376 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 846 "chapel.ypp" /* yacc.c:1646  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5387 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 853 "chapel.ypp" /* yacc.c:1646  */
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
#line 5410 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 875 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5418 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 879 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5427 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 884 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5435 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 888 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5444 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 896 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5450 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 897 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "&"; }
#line 5456 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 898 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "|"; }
#line 5462 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 899 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "^"; }
#line 5468 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 900 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "~"; }
#line 5474 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 901 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "=="; }
#line 5480 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 902 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "!="; }
#line 5486 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 903 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<="; }
#line 5492 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 904 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">="; }
#line 5498 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 905 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<"; }
#line 5504 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 906 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">"; }
#line 5510 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 907 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "+"; }
#line 5516 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 908 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "-"; }
#line 5522 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 909 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "*"; }
#line 5528 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 910 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "/"; }
#line 5534 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 911 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<"; }
#line 5540 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 912 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>"; }
#line 5546 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 913 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<<"; }
#line 5552 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 914 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>>"; }
#line 5558 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 915 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "%"; }
#line 5564 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 916 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "**"; }
#line 5570 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 917 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "!"; }
#line 5576 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 918 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "chpl_by"; }
#line 5582 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 919 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "#"; }
#line 5588 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 920 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "chpl_align"; }
#line 5594 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 921 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<=>"; }
#line 5600 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 922 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<~>"; }
#line 5606 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 926 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "="; }
#line 5612 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 927 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "+="; }
#line 5618 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 928 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "-="; }
#line 5624 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 929 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "*="; }
#line 5630 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 930 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "/="; }
#line 5636 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 931 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "%="; }
#line 5642 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 932 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "**="; }
#line 5648 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 933 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "&="; }
#line 5654 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 934 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "|="; }
#line 5660 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 935 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "^="; }
#line 5666 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 936 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = ">>="; }
#line 5672 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 937 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pch) = "<<="; }
#line 5678 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 941 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 5684 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 942 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5690 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 946 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 5696 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 950 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 5702 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 951 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 5708 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 952 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 5714 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 957 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 5720 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 959 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 5726 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 961 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 5732 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 963 "chapel.ypp" /* yacc.c:1646  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 5738 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 967 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5744 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 968 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 5750 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 972 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_IN; }
#line 5756 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 973 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_INOUT; }
#line 5762 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 974 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_OUT; }
#line 5768 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 975 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST; }
#line 5774 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 976 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 5780 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 977 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 5786 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 978 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5792 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 979 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_REF; }
#line 5798 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 980 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_TYPE; }
#line 5804 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 984 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_BLANK; }
#line 5810 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 985 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_PARAM; }
#line 5816 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 986 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_REF;   }
#line 5822 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 987 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 5828 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 991 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 5834 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 992 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 5840 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 996 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_VALUE; }
#line 5846 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 997 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_VALUE; }
#line 5852 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 998 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 5858 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 999 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_REF; }
#line 5864 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1000 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_PARAM; }
#line 5870 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1001 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.retTag) = RET_TYPE; }
#line 5876 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1005 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = NULL; }
#line 5882 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1011 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 5888 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1016 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 5894 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1018 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5900 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1022 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 5906 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1027 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5912 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1028 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 5918 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1032 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 5924 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1033 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5930 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1038 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 5936 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1040 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 5942 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1042 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 5948 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1047 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 5965 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1060 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 5983 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1076 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 5989 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1078 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 5995 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1080 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6001 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1085 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6013 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1093 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6025 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1101 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6037 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1109 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6050 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1118 "chapel.ypp" /* yacc.c:1646  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6061 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1127 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6067 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1128 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6073 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1129 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6079 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1135 "chapel.ypp" /* yacc.c:1646  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6088 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1143 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6094 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1145 "chapel.ypp" /* yacc.c:1646  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6104 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1151 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6110 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1156 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6116 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1158 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6122 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1160 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6128 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1165 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6134 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1167 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6140 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1169 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6146 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1175 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6152 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1176 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6158 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1177 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6164 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1181 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6170 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1183 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6176 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1187 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6182 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1188 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6188 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1189 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6194 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1190 "chapel.ypp" /* yacc.c:1646  */
    {printf("bad type specification"); }
#line 6200 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1211 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6208 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1215 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6216 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1219 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6228 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1226 "chapel.ypp" /* yacc.c:1646  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6234 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1230 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6240 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1231 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6246 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1232 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6252 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1237 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6258 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1239 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6264 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1245 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6270 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1247 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6276 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1249 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6282 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1253 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = NULL; }
#line 6288 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1254 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6294 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1255 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6300 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1256 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6306 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1257 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6312 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1258 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6318 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1259 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6324 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1265 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6330 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 1266 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6336 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 1267 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6342 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 1268 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6348 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1272 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6354 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 1273 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6360 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 1274 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6366 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 1278 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6372 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 1279 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6378 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 1283 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6384 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 1288 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6390 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 1289 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6396 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 1293 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6402 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 1294 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6408 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 1295 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6414 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 1296 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6420 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 1301 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6426 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1318 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6432 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 1320 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6438 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1322 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6444 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 1324 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6450 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1326 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6456 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1328 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6462 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1330 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6468 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 1335 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6474 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 1337 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6480 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 1339 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6486 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 1341 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6492 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 1343 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6498 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 1345 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6504 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1347 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6510 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1349 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true); }
#line 6516 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 1351 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6522 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 1353 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6528 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1355 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6534 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1357 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6540 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1359 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6551 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1366 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6561 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1372 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6571 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 1378 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6581 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 1384 "chapel.ypp" /* yacc.c:1646  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6591 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 1393 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6597 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 337:
#line 1397 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6603 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 1413 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6609 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 1417 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = NULL; }
#line 6615 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 1422 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6621 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 1427 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pIntentExpr).first, (yyvsp[0].pIntentExpr).second); }
#line 6627 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 1429 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).first);
      (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pIntentExpr).second);
    }
#line 6636 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 1437 "chapel.ypp" /* yacc.c:1646  */
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
#line 6651 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 1447 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6657 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 1448 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pIntentExpr).first = (yyvsp[-2].pexpr), (yyval.pIntentExpr).second = (yyvsp[0].pexpr); }
#line 6663 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 1454 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6669 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 1459 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 6675 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 1464 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 6681 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 1480 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 6687 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 1482 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6693 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 1484 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6699 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 1486 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 6705 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 1488 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 6711 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 1490 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 6717 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 1513 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6723 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 1514 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 6729 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 1515 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 6735 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 1519 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 6741 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 1520 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 6747 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 1521 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 6753 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 384:
#line 1529 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 6759 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 1530 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 6765 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 386:
#line 1531 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 6771 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 1535 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 6777 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 1536 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 6783 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 1537 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 6789 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 1538 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 6795 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 1539 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 6801 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 1540 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6807 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 1541 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 6813 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 1543 "chapel.ypp" /* yacc.c:1646  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 6821 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 1549 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6827 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 1550 "chapel.ypp" /* yacc.c:1646  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6833 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 1554 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6839 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 1555 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6845 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 1556 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6851 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 1557 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6857 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 1558 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6863 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 1559 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6869 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 1560 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6875 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 1561 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">>>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6881 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 1562 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6887 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 1563 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6893 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 1564 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6899 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 1565 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6905 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 1566 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6911 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 1567 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6917 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 1568 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6923 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 1569 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6929 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 1570 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6935 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 1571 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6941 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 1572 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6947 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 1573 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6953 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 1574 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6959 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 1575 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6965 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 419:
#line 1576 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6971 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 1577 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6977 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 1578 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6983 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 1582 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 6989 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 1583 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 6995 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 1584 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7001 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 1585 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7007 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 1586 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7013 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 1587 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7019 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 1591 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7025 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 1592 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7031 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 1593 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7037 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 1594 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7043 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 1598 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7049 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 1599 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7055 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 1600 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7061 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 1601 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr), true); }
#line 7067 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 1606 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7073 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 1607 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7079 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 1608 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7085 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 1609 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7091 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 1610 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7097 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 1611 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7103 "bison-chapel.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 1612 "chapel.ypp" /* yacc.c:1646  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7109 "bison-chapel.cpp" /* yacc.c:1646  */
    break;


#line 7113 "bison-chapel.cpp" /* yacc.c:1646  */
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
