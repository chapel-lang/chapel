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
    Expr*     iVar;
    IntentTag tfIntent; // undefined for a reduce intent
    Expr*     riExp;    // non-NULL for a reduce intent
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
    ForallIntents*    pForallIntents;
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif
#line 134 "chapel.ypp" /* yacc.c:355  */

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
#line 156 "chapel.ypp" /* yacc.c:355  */

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

#line 226 "bison-chapel.cpp" /* yacc.c:355  */

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
    TTHROWS = 327,
    TTRY = 328,
    TTRYBANG = 329,
    TTYPE = 330,
    TUNDERSCORE = 331,
    TUNION = 332,
    TUSE = 333,
    TVAR = 334,
    TWHEN = 335,
    TWHERE = 336,
    TWHILE = 337,
    TWITH = 338,
    TYIELD = 339,
    TZIP = 340,
    TALIAS = 341,
    TAND = 342,
    TASSIGN = 343,
    TASSIGNBAND = 344,
    TASSIGNBOR = 345,
    TASSIGNBXOR = 346,
    TASSIGNDIVIDE = 347,
    TASSIGNEXP = 348,
    TASSIGNLAND = 349,
    TASSIGNLOR = 350,
    TASSIGNMINUS = 351,
    TASSIGNMOD = 352,
    TASSIGNMULTIPLY = 353,
    TASSIGNPLUS = 354,
    TASSIGNSL = 355,
    TASSIGNSR = 356,
    TBAND = 357,
    TBNOT = 358,
    TBOR = 359,
    TBXOR = 360,
    TCOLON = 361,
    TCOMMA = 362,
    TDIVIDE = 363,
    TDOT = 364,
    TDOTDOT = 365,
    TDOTDOTDOT = 366,
    TEQUAL = 367,
    TEXP = 368,
    TGREATER = 369,
    TGREATEREQUAL = 370,
    THASH = 371,
    TLESS = 372,
    TLESSEQUAL = 373,
    TMINUS = 374,
    TMOD = 375,
    TNOT = 376,
    TNOTEQUAL = 377,
    TOR = 378,
    TPLUS = 379,
    TQUESTION = 380,
    TSEMI = 381,
    TSHIFTLEFT = 382,
    TSHIFTRIGHT = 383,
    TSTAR = 384,
    TSWAP = 385,
    TASSIGNREDUCE = 386,
    TIO = 387,
    TLCBR = 388,
    TRCBR = 389,
    TLP = 390,
    TRP = 391,
    TLSBR = 392,
    TRSBR = 393,
    TNOELSE = 394,
    TUPLUS = 395,
    TUMINUS = 396
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
#line 187 "chapel.ypp" /* yacc.c:355  */

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);

#line 413 "bison-chapel.cpp" /* yacc.c:355  */

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 419 "bison-chapel.cpp" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 38 "chapel.ypp" /* yacc.c:359  */

  int         captureTokens;
  char        captureString[1024];
#line 195 "chapel.ypp" /* yacc.c:359  */

  #include "build.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForLoop.h"
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

#line 486 "bison-chapel.cpp" /* yacc.c:359  */

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
#define YYLAST   13267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  142
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  114
/* YYNRULES -- Number of rules.  */
#define YYNRULES  456
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  864

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   396

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
     135,   136,   137,   138,   139,   140,   141
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   424,   424,   429,   430,   436,   437,   442,   443,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   478,   480,   485,   486,
     487,   502,   503,   508,   509,   514,   519,   524,   528,   534,
     539,   543,   548,   552,   553,   554,   558,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   581,   582,   586,   590,   591,   595,   596,
     600,   601,   605,   606,   607,   608,   609,   610,   611,   615,
     616,   620,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   657,
     663,   669,   675,   682,   692,   696,   697,   698,   699,   706,
     707,   711,   716,   717,   721,   723,   725,   732,   737,   745,
     750,   755,   763,   764,   769,   770,   772,   777,   787,   796,
     800,   808,   809,   814,   819,   813,   844,   850,   857,   865,
     876,   882,   875,   910,   914,   919,   923,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   955,   956,   960,   961,   962,   963,   964,   965,   966,
     967,   968,   969,   970,   971,   975,   976,   980,   984,   985,
     986,   990,   992,   994,   996,  1001,  1002,  1006,  1007,  1008,
    1009,  1010,  1011,  1012,  1013,  1014,  1018,  1019,  1020,  1021,
    1025,  1026,  1030,  1031,  1032,  1033,  1034,  1035,  1039,  1040,
    1043,  1044,  1048,  1049,  1053,  1055,  1060,  1061,  1065,  1066,
    1070,  1071,  1075,  1077,  1079,  1084,  1097,  1114,  1115,  1117,
    1122,  1130,  1138,  1146,  1155,  1165,  1166,  1167,  1171,  1172,
    1180,  1182,  1188,  1193,  1195,  1197,  1202,  1204,  1206,  1213,
    1214,  1215,  1219,  1220,  1225,  1226,  1227,  1228,  1248,  1252,
    1256,  1264,  1268,  1269,  1270,  1274,  1276,  1282,  1284,  1286,
    1291,  1292,  1293,  1294,  1295,  1296,  1297,  1303,  1304,  1305,
    1306,  1310,  1311,  1312,  1316,  1317,  1321,  1322,  1326,  1327,
    1331,  1332,  1333,  1334,  1335,  1339,  1350,  1351,  1352,  1353,
    1354,  1355,  1357,  1359,  1361,  1363,  1365,  1367,  1372,  1374,
    1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1392,  1394,
    1396,  1403,  1409,  1415,  1421,  1430,  1435,  1443,  1444,  1445,
    1446,  1447,  1448,  1449,  1450,  1455,  1456,  1460,  1465,  1468,
    1473,  1478,  1481,  1486,  1490,  1491,  1495,  1496,  1501,  1506,
    1514,  1515,  1516,  1517,  1518,  1519,  1520,  1521,  1522,  1524,
    1526,  1528,  1530,  1532,  1537,  1538,  1539,  1540,  1551,  1552,
    1556,  1557,  1558,  1562,  1563,  1564,  1572,  1573,  1574,  1575,
    1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1592,  1600,  1601,  1605,  1606,  1607,  1608,  1609,  1610,  1611,
    1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1625,  1626,  1627,  1631,  1632,  1633,  1634,
    1635,  1636,  1640,  1641,  1642,  1643,  1647,  1648,  1649,  1650,
    1655,  1656,  1657,  1658,  1659,  1660,  1661
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
  "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN",
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
  "pragma_ls", "stmt", "module_decl_stmt", "access_control", "block_stmt",
  "stmt_ls", "only_ls", "opt_only_ls", "except_ls", "use_stmt",
  "require_stmt", "assignment_stmt", "opt_ident", "ident",
  "opt_ident_or_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "try_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_ls", "enum_item", "lambda_decl_expr", "$@1",
  "$@2", "linkage_spec", "fn_decl_stmt", "$@3", "$@4",
  "fn_decl_stmt_inner", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
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
  "task_intent_ls", "forall_intent_clause", "forall_intent_ls",
  "intent_expr", "io_expr", "new_expr", "let_expr", "expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
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
     395,   396
};
# endif

#define YYPACT_NINF -763

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-763)))

#define YYTABLE_NINF -408

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -763,    95,  2501,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  3581,    13,   101,  -763,    13,  7499,   111,   101,  7499,
    3581,    11,   101,   215,   529,  6149,  7499,  7499,    41,  -763,
     101,  -763,    45,  3581,  7499,  7499,  -763,  7499,  7499,   204,
     168,   637,   761,  -763,  6419,  6554,  7499,  6689,  7499,   249,
     196,  3581,  3581,  3581,   101,  -763,  6419,  7499,  7499,  -763,
    -763,  7499,  -763,  -763,  8849,  7499,  7499,  -763,  7499,  -763,
    -763,  2771,  5744,  6419,  -763,  3446,  -763,  -763,   291,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,   101,  -763,    72,   262,  -763,  -763,  -763,
      32,   212,  -763,  -763,  -763,   219,   220,   246,   232,   237,
   13030,  1191,   197,   275,   278,  -763,  -763,  -763,  -763,  -763,
    -763,   159,  -763, 13030,   270,  3581,  -763,   283,  -763,   280,
    7499,  7499,  7499,  7499,  7499,  6419,  6419,   238,  -763,  -763,
    -763,  -763, 10439,   254,  -763,  -763,   101,   292, 10709,   339,
    6419,   289,  -763,  -763,  -763,  -763,   101,   136,   101,   294,
      46,  9479,  9418,  9601,  6419,  3581,   297,    16,   281,    42,
    -763,  -763,   414,   337,  9667,   414,  -763,  6419,  -763,   287,
    -763,  -763,   101,  -763,   213, 13030,  -763, 10782, 10317,  3581,
    -763,  -763,  9667, 13030,   312,  6419,  -763, 13030,  -763,  -763,
     365,   329,   214, 10550,  9667, 10829,   290,  1904,   414,   290,
     414,  -763,  -763,  2906,   -19,  -763,  7499,  -763,   -30,   -26,
   13030,   203, 10902,   -21,   449,  -763,   101,   351,  -763,  -763,
    -763,    26,    45,    45,    45,  -763,  7499,  7499,  7499,  7499,
    6824,  6824,  7499,  7499,  7499,  7499,  7499,  7499,    66,  8849,
    7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,
    7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,
    7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,  7499,
    7499,  6419,  6419,  6824,  6824,  5609,  -763,  -763,  3041,  -763,
   10593, 10666, 10963,    37,  6824,    46,   332,  -763,  -763,  7499,
     340,  -763,   323,   354,  -763, 13030,   101,   338,   101,   439,
    6419,  -763,  3716,  6824,  -763,  3851,  6824,   344,  -763,    46,
    3581,   456,   364,  -763,    79,  -763,  -763,    16,  -763,   398,
     372,  -763,  4931,   423,   426,  7499,    45,  -763,   375,  -763,
    -763,  6419,  -763,  -763,  -763,  -763,  -763,  6419,   380,  5066,
     418,  -763,  -763,  7634,  7499,  -763,  -763,  -763,  -763,  6284,
    -763,  9357,  5879,  -763,  6014,  -763,  6824,  5201,  2636,   388,
    7499,  5473,  -763,  -763,   395,  6419,   396,   356,    45,   221,
     245,   267,   276, 10378,  1975,  1975,   183,  -763,   183,  -763,
     183, 10516,  1002,   771,  1019,   337,   290,  -763,  -763,  -763,
    1904, 13062,   183,  1861,  1861,  1975,  1861,  1861,   363,   290,
   13062, 13138,   363,   414,   414,   290, 11029, 11090, 11151, 11212,
   11273, 11334, 11395, 11456, 11517, 11578, 11639, 11700, 11761, 11822,
   11883, 11944,   394,   393,  -763,   183,  -763,   183,   149,  -763,
    -763,  -763,  -763,  -763,  -763,   101,    -9,  -763, 13091,   231,
    7769,  6824,  7904,  6824,  7499,  6824,  9007,    13, 12005,  -763,
    -763, 12066,  7499,  6419,  -763,  6419,   451,   174,  -763,  -763,
     351,  7499,    60,  7499, 13030,    48,  9728,  7499, 13030,    35,
    9540,  5609,  -763,   511, 12136,  3581,  -763,    80,  -763,    55,
    -763,   142,   406,    16,    67,  -763,  6419,  -763,   544,  7499,
    6959,  -763, 13030,  -763,  -763,  -763, 13030,    25,   408,  -763,
    6419,  -763,   293,   101,   419,   440,   420, 12197,   440,   424,
    -763,  -763,  -763,  -763,  -763,  -763,     0,  9174,  -763,  -763,
   13030,  3581, 13030,  -763, 12258,  3176,   447,  -763,  -763,  -763,
    -763,  -763,  9058,   299,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  5609,
    -763,  6824,  6824,  7499,   533, 12319,  7499,   535, 12380,   425,
    9104,    46,    46,  -763, 13030,  -763, 13030,  -763,  7499,   101,
    -763,   436,  9667,  -763,  9789,  3986,  -763,  4121,  -763,  9855,
    4256,  -763,    46,  4391,  -763,    46,    84,  -763,  3581,  7499,
    -763,   443,  -763,    16,   464,   510,  -763,  -763,  -763,    27,
    -763,  -763,  5066,   426,    40, 13030,  -763, 13030,  4526,  6419,
    -763,  -763,  -763,    51,  -763,  7499,  -763,  7499,  -763,  4661,
     438,  4796,   441,  -763,  7499,  -763,  3311,   686,   142,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,   101,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -763,  -763,   465,   445,   445,  -763,  -763,
      85,   170, 12441,  8039,  8174, 12507,  8309,  8444,  8579,  8714,
    -763,  -763, 13030,  -763,  -763,  -763,  3581,  7499, 13030,  7499,
   13030,  3581,  7499, 13030,  -763,  7499, 13030,  -763,  5609,  -763,
    -763, 13030,  -763,   446,  7094,    97,  -763,   502,  -763,  6824,
    2144,  3581,  -763,    49,  2288, 12573, 13030,  7499,  -763, 13030,
    3581,  7499,  -763, 13030,  3581, 13030,  -763,   548,   483,   483,
    -763,   537,  -763,    31,  -763,  9058,    79,  -763,  -763,  7499,
    7499,  7499,  7499,  7499,  7499,  7499,  7499,  1107, 12136,  9921,
    9987, 12136, 10053, 10119,  -763,   464,    11,  7499,  7499,  5337,
    -763,  -763,   226,  6419,  -763,  -763,  7499,    15,  9235,  -763,
     894,   339,  -763,  7499, 10185,  -763, 10251,  -763,  -763,   513,
    -763,   445,   445,   171, 12634, 12700, 12766, 12832, 12898, 12964,
    -763,  3581,  3581,  3581,  3581,    97,  7229,   178,  -763,  -763,
   13030, 13030,  -763,  -763,  -763,  8714, 13030,  3581,  3581,  -763,
     502,  -763,  -763,  -763,  7499,  7499,  7499,  7499,  7499,  7499,
   12136, 12136, 12136, 12136,  -763,  -763,  -763,  -763,  -763,   261,
    6824,  8934,   931, 12136, 12136,    39,  9296,  -763,  -763,  -763,
    -763,  -763,  7364,  -763
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    35,    75,   400,   401,   402,   403,
     404,     0,   355,    73,   129,   355,     0,   256,    73,     0,
       0,     0,     0,    73,    73,     0,     0,     0,     0,   147,
       0,   143,     0,     0,     0,     0,   346,     0,     0,     0,
       0,   255,   255,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   131,     0,     0,     0,   452,
     454,     0,   455,   456,   383,     0,     0,   453,   450,    82,
     451,     0,     0,     0,     4,     0,     5,     9,     0,    10,
      11,    12,    14,   315,    20,    13,    83,    15,    17,    16,
      19,    18,    88,     0,    86,   375,     0,    89,    87,    90,
       0,   384,   371,   372,   318,   316,     0,     0,   376,   377,
       0,   317,     0,   385,   386,   387,   370,   320,   319,   373,
     374,     0,    22,   326,     0,     0,   356,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,   375,   384,   316,
     376,   377,   355,   317,   385,   386,     0,     0,     0,     0,
     306,     0,    77,    76,   148,    91,     0,   149,     0,     0,
       0,     0,     0,     0,   306,     0,     0,     0,     0,     0,
     258,    29,   438,   368,     0,   439,     7,   306,   256,   257,
      85,    84,   235,   298,     0,   297,    80,     0,     0,     0,
      79,    32,     0,   321,     0,   306,    33,   327,   119,   120,
     247,     0,     0,   297,     0,     0,   441,   382,   437,   440,
     436,    41,    43,     0,     0,   301,     0,   303,     0,     0,
     302,     0,   297,     0,     0,     6,     0,   132,   221,   220,
     150,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   381,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   306,   306,     0,     0,     0,    23,    24,     0,    25,
       0,     0,     0,     0,     0,     0,     0,    26,    27,     0,
     315,   313,     0,   307,   308,   314,     0,     0,     0,     0,
       0,   100,     0,     0,    99,     0,     0,     0,   106,     0,
       0,   116,     0,    28,   198,   144,   263,     0,   264,   266,
       0,   277,     0,     0,   269,     0,     0,    30,     0,   149,
     234,     0,    56,    81,   122,    78,    31,   306,     0,     0,
     245,   242,    53,     0,    49,    93,    34,    42,    44,     0,
     405,     0,     0,   396,     0,   398,     0,     0,     0,     0,
       0,     0,   409,     8,     0,     0,     0,   216,     0,     0,
       0,     0,     0,   354,   433,   432,   435,   443,   442,   447,
     446,   429,   426,   427,   428,   379,   416,   395,   394,   393,
     380,   420,   431,   425,   423,   434,   424,   422,   414,   419,
     421,   430,   413,   417,   418,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,   444,   449,   448,   210,   207,
     208,   209,   213,   214,   215,     0,     0,   358,     0,     0,
       0,     0,     0,     0,     0,     0,   407,   355,   355,    96,
     243,     0,     0,     0,   323,     0,   141,     0,   138,   244,
     132,     0,     0,     0,   330,     0,     0,     0,   336,     0,
       0,     0,   107,   115,     0,     0,   322,     0,   199,     0,
     206,   222,     0,   267,     0,   281,     0,   276,   371,     0,
       0,   260,   369,   259,   392,   300,   299,     0,     0,   324,
       0,   249,   371,     0,   451,    52,     0,    45,    50,     0,
     406,   378,   397,   304,   399,   305,     0,     0,   408,   112,
     340,     0,   411,   410,     0,     0,   133,   134,   217,   218,
     219,   151,     0,     0,   251,   250,   252,   254,    57,    64,
      65,    66,    61,    63,    71,    72,    59,    62,    60,    58,
      68,    67,    69,    70,   390,   391,   211,   212,   363,     0,
     357,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,   312,   310,   311,   309,     0,   139,
     137,     0,     0,   114,     0,     0,    98,     0,    97,     0,
       0,   104,     0,     0,   102,     0,     0,   361,     0,     0,
     118,   205,   197,     0,   290,   223,   226,   225,   227,     0,
     265,   268,     0,   269,     0,   261,   270,   271,     0,     0,
     121,   123,   325,     0,   246,     0,    54,     0,    55,     0,
       0,     0,     0,   113,     0,    36,     0,   255,   222,   180,
     178,   183,   190,   191,   192,   187,   189,   185,   188,   186,
     184,   194,   193,   159,   162,   160,   161,   172,   163,   176,
     168,   166,   179,   167,   165,   170,   175,   177,   164,   169,
     173,   174,   171,   181,   182,   157,   195,   195,   253,   359,
     384,   384,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    94,   142,   140,   134,   101,     0,     0,   329,     0,
     328,     0,     0,   335,   105,     0,   334,   103,     0,   360,
     117,   345,   200,     0,     0,   269,   224,   240,   262,     0,
       0,     0,   125,     0,     0,    47,    46,     0,   110,   342,
       0,     0,   108,   341,     0,   412,    37,    73,   255,   255,
     127,   255,   135,     0,   158,     0,   198,   153,   154,     0,
       0,     0,     0,     0,     0,     0,     0,   255,   333,     0,
       0,   339,     0,     0,   362,   290,   293,   294,   295,     0,
     292,   296,   371,   236,   202,   201,     0,     0,     0,   279,
     371,   126,   124,     0,     0,   111,     0,   109,   136,   228,
     157,   195,   195,     0,     0,     0,     0,     0,     0,     0,
     128,     0,     0,     0,     0,   269,   282,     0,   237,   239,
     238,   241,   232,   233,   145,     0,    48,     0,     0,   229,
     240,   155,   156,   196,     0,     0,     0,     0,     0,     0,
     332,   331,   338,   337,   204,   203,   284,   285,   287,   371,
       0,   407,   371,   344,   343,     0,     0,   286,   288,   230,
     152,   231,   282,   289
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -763,  -763,  -763,     2,  -617,  1853,  -763,  -763,  1062,    52,
     234,  -763,  -763,  -763,  -763,  -763,   184,  1713,   -10,   -95,
    -723,  -622,   -40,  -763,  -763,    82,  -763,  -763,  -763,  -763,
    -763,  -763,   444,   132,  -101,  -763,  -763,    17,   609,  -763,
    -763,  -763,  -763,  -763,  -763,  -763,  -150,  -145,  -631,  -763,
    -143,     4,  -763,  -318,  -763,  -763,   -31,  -763,  -763,  -236,
     134,  -763,  -194,  -208,  -763,  -136,  -763,  -763,  -763,  -190,
     295,  -763,  -316,  -611,  -763,  -477,  -340,  -762,  -688,  -141,
       7,    71,  -763,  -146,  -763,   172,   359,  -310,  -763,  -763,
     736,  -763,    -8,  -763,  -763,  -200,  -763,  -443,  -763,   863,
     893,   -11,   988,  -763,  1242,  1294,  -763,  -763,  -763,  -763,
    -763,  -763,  -763,  -282
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   212,    75,   483,    77,    78,    79,   213,
     515,   519,   516,    80,    81,    82,   153,    83,   157,   191,
      84,    85,    86,    87,    88,   579,    89,    90,    91,   507,
     631,    92,    93,   376,   647,    94,   467,   468,   137,   166,
     491,    96,    97,   377,   648,   541,   686,   687,   757,   325,
     487,   488,   489,   445,   542,   230,   619,   830,   860,   824,
     183,   819,   784,   787,    98,   201,   350,    99,   100,   169,
     170,   329,   330,   501,   333,   334,   497,   847,   781,   725,
     214,   218,   219,   302,   303,   304,   138,   102,   103,   104,
     139,   106,   125,   126,   446,   319,   606,   447,   107,   140,
     141,   110,   143,   112,   144,   145,   115,   116,   223,   117,
     118,   119,   120,   121
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     123,   180,   181,   449,    74,   142,   490,   129,   148,   511,
     296,   492,   728,   154,   161,   162,   163,   623,   322,     5,
     307,   369,   498,   172,   173,   752,   174,   175,   331,     5,
     751,   338,   331,   185,   187,   188,   192,   193,   607,   512,
     197,   379,   380,   381,   382,   203,   204,   205,     5,   348,
     206,   184,   231,   207,   208,   209,   758,   210,     5,   600,
     185,   220,   222,   202,   823,   311,   314,   318,   331,     5,
     189,   455,   595,   189,   729,   628,   335,   362,    45,   337,
     221,   364,  -205,   317,   232,   729,   371,   378,   359,   857,
    -274,   397,   326,   233,  -274,     3,   124,   346,   569,   438,
     863,  -274,    45,  -274,     5,   629,   363,   160,  -274,   355,
     365,   234,  -274,   439,   785,   360,   440,   372,   317,   123,
     290,   291,   292,   197,   185,   222,   689,   570,   848,   798,
     441,   442,   823,   622,   295,   432,   433,   622,   639,   305,
     443,   398,   727,   293,   367,   752,   150,   367,   288,   336,
     751,   327,    14,   305,   444,  -274,   341,  -274,   367,   630,
    -274,   167,   615,   858,  -274,   859,   305,   341,   288,   339,
     831,   832,   288,   622,  -274,   456,   164,   621,   730,   288,
     167,   288,   288,   566,   305,   500,   146,   611,   543,   734,
     613,   718,  -364,  -274,   616,    43,   593,   127,  -351,   449,
     459,   508,   147,   617,   845,   361,   217,  -389,   783,  -389,
     567,   176,   850,    55,  -205,   358,   612,   618,     5,   283,
     719,  -364,   152,   284,   482,   383,   384,   385,   386,   388,
     390,   391,   392,   393,   394,   395,   396,   366,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     305,   305,   435,   437,   448,   774,   799,  -365,   611,   602,
     605,   589,   331,   458,   301,   367,   317,   449,   461,   246,
       5,   572,   248,   490,   152,   284,   251,   723,   301,   185,
     228,   474,   476,   177,   478,   480,  -365,   833,   590,   484,
     367,   301,   498,   239,  -291,  -274,   851,   472,   194,   229,
     341,   341,   387,   389,   502,   -73,   640,   642,   336,   301,
     506,   195,   281,  -291,   282,   226,   305,  -291,  -348,   342,
     352,   368,   517,   517,   -73,  -347,   235,   544,   506,  -283,
     240,   220,   336,   220,   241,   527,   506,   530,  -352,   532,
     534,   101,  -291,  -353,   185,   434,   436,  -272,  -283,  -274,
     101,   545,  -283,  -389,   336,  -389,   457,   634,   236,   101,
     596,   598,   536,   336,   601,   604,   239,   332,  -274,  -388,
     789,  -388,   101,   546,   789,   475,   246,  -283,   479,   248,
    -248,  -350,   547,   251,  -349,   285,   336,  -274,   538,   287,
     101,   101,   101,   288,   782,   301,   301,   539,   297,  -248,
     790,   299,   306,   240,   790,   688,   462,   241,   463,   310,
     101,   540,   324,   523,   101,   525,   449,   239,   490,   474,
     575,   478,   578,   484,   580,   530,   248,   347,   526,   581,
     582,   584,   586,   349,   305,   351,   373,   375,   460,   464,
     592,   465,   594,   438,   469,   243,   599,   244,   245,   246,
     448,   247,   248,   471,   240,   505,   251,   439,   241,   481,
     440,   301,   485,   258,   101,   222,   700,   701,   625,   627,
     262,   263,   264,   505,   441,   442,   217,   705,   217,   222,
     486,   505,   178,   624,   443,   493,   849,   714,   494,   499,
     717,   504,    23,   179,   500,   852,   509,   633,   444,    29,
     246,  -146,   247,   248,   101,   513,   531,   251,   535,   537,
     564,   565,     5,   574,   258,   577,   152,   608,   155,   588,
    -146,   849,   620,   264,   632,   -51,   636,   635,   101,  -257,
     638,     5,   849,    14,   341,   152,    17,   693,   448,   696,
     388,   435,   692,   698,    22,   695,    23,   747,  -257,   704,
     724,   726,   101,    29,   755,  -146,   740,   702,  -275,   744,
     756,  -257,   775,   786,   708,   829,   710,   646,   518,   713,
    -257,   224,   716,   748,  -146,   749,    43,   585,   721,   301,
    -257,   308,   591,   767,   156,   801,   703,  -275,  -257,  -257,
     802,    95,    54,   803,    55,   722,  -275,   753,   185,   861,
      95,   844,   855,   156,   735,  -275,   736,  -257,   739,    95,
     743,   503,  -275,   745,   815,     0,   733,   587,   792,     0,
       0,     0,    95,     0,     0,     0,     0,   101,   358,     0,
       0,  -275,     0,   387,   434,     0,   178,     0,     0,     0,
      95,    95,    95,    69,     0,     0,    23,   179,     0,     0,
    -275,   101,     0,    29,   101,  -146,     0,  -275,     0,   101,
      95,   -40,   708,   710,    95,   713,   716,   739,   743,     0,
       0,     0,     0,     0,  -146,   768,   769,     0,   770,     0,
     771,   772,    14,     0,   773,    17,     0,   448,   180,   181,
       0,     0,     0,    22,     0,    23,   747,     0,   788,   530,
       0,     0,    29,   530,  -146,     0,   794,   101,     0,     0,
     796,     0,     0,     0,    95,     0,     0,     0,   105,     0,
      39,     0,   748,  -146,   749,    43,     0,   105,   768,   804,
     805,   771,   806,   807,   808,   809,   105,     0,     0,     0,
       0,    54,     0,    55,     0,     0,   193,   197,   222,   105,
       0,     0,   820,     0,    95,   821,     0,     0,     0,     0,
     178,     0,   826,     0,     0,     0,   817,   105,   105,   105,
      23,   179,     0,     0,   239,     0,     0,    29,    95,  -146,
     840,   841,   842,   843,   568,   -39,     0,   105,     0,     0,
       0,   105,    69,     0,   743,     0,   853,   854,  -146,     0,
     750,     0,    95,   840,   841,   842,   843,   853,   854,     0,
       0,   240,     0,     0,     0,   241,     0,     0,     0,   856,
     530,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,   743,     0,     0,     0,     0,     0,     0,   780,     0,
       0,   105,     0,     0,     0,   108,     0,     0,     0,     0,
       0,     0,     0,   243,   108,     0,   245,   246,     0,   247,
     248,     0,     0,   108,   251,     0,     0,     0,     0,     0,
     101,   258,     0,     0,   101,   109,   108,    95,   262,   263,
     264,   105,     0,     0,   109,     0,     0,     0,     0,     0,
       0,     0,     0,   109,   108,   108,   108,   818,     0,     0,
       0,    95,     0,     0,    95,   105,   109,     0,  -278,    95,
     690,   691,     0,     0,   108,     0,     0,     0,   108,     0,
       0,     0,     0,     0,   109,   109,   109,     0,     0,   105,
     846,     0,     0,     0,   101,     0,   101,  -278,     0,   101,
       0,     0,   101,     0,   109,  -280,  -278,   101,   109,     0,
       0,     0,     0,     0,     0,  -278,     0,    95,     0,     0,
       0,     0,  -278,     0,     0,   846,     0,   101,   108,     0,
     111,     0,     0,     0,  -280,     0,   846,     0,   101,   111,
     101,  -278,     0,  -280,     0,   101,     0,     0,   111,     0,
       0,     0,  -280,     0,     0,     0,     0,     0,   109,  -280,
    -278,   111,     0,     0,   105,   239,     0,  -278,   108,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -280,   111,
     111,   111,   239,     0,     0,     0,     0,     0,   105,     0,
       0,   105,   108,     0,     0,     0,   105,  -280,   109,   111,
       0,     0,   240,   111,  -280,   101,   241,     0,     0,     0,
     101,     0,     0,     0,     0,     0,   108,     0,     0,   240,
       0,     0,   109,   241,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,    95,     0,     0,     0,     0,   101,
       0,     0,     0,   101,   105,     0,   109,     0,   246,   190,
     247,   248,     0,   111,     0,   251,     0,     0,     0,     0,
       0,   243,   258,    14,     0,   246,    17,   247,   248,   262,
     263,   264,   251,     0,    22,     0,    23,   747,     0,   258,
      95,     0,     0,    29,    95,  -146,   262,   263,   264,     0,
       0,   108,     0,   111,     0,     0,     0,     0,     0,     0,
       0,    39,     0,   748,  -146,   749,    43,     0,     0,     0,
     101,   101,   101,   101,     0,   108,     0,   111,   108,     0,
       0,   109,    54,   108,    55,     0,   101,   101,     0,     0,
       0,   289,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,     0,    95,   109,    95,     0,   109,    95,
       0,     0,    95,   109,     0,     0,     0,    95,     0,     0,
       0,   105,   190,   190,   190,   321,     0,     0,     0,     0,
       0,   108,     0,    69,     0,     0,   190,    95,     0,     0,
       0,   810,     0,     0,   113,     0,     0,     0,    95,     0,
      95,     0,     0,   113,   190,    95,     0,     0,     0,     0,
       0,   109,   113,     0,     0,     0,   190,   105,     0,     0,
       0,   105,     0,     0,     0,   113,   111,     0,     0,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   113,   113,   113,   114,     0,     0,     0,
     111,     0,     0,   111,     0,   114,     0,     0,   111,     0,
       0,     0,     0,   113,   114,    95,     0,   113,     0,     0,
      95,   279,   280,  -366,     0,     0,  -388,   114,  -388,     0,
       0,   105,     0,   105,     0,     0,   105,     0,     0,   105,
      95,     0,     0,     0,   105,   114,   114,   114,   108,    95,
       0,     0,     0,    95,     0,     0,   111,   190,     0,     0,
       0,     0,     0,     0,   105,   114,     0,   113,     0,   114,
       0,     0,     0,     0,     0,   105,     0,   105,   109,     0,
       0,   190,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   108,     0,     0,     0,   108,     0,
       0,     0,     0,     0,     0,     0,     0,   113,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
      95,    95,    95,    95,   109,     0,     0,     0,   109,     0,
       0,   113,     0,     0,     0,     0,    95,    95,     0,     0,
       0,     0,   105,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,   113,     0,     0,   108,   114,
     108,     0,     0,   108,     0,     0,   108,   105,     0,     0,
       0,   108,     0,   111,     0,     0,   105,     0,     0,     0,
     105,     0,     0,   114,     0,     0,     0,     0,   109,     0,
     109,   108,     0,   109,     0,     0,   109,     0,     0,     0,
       0,   109,   108,     0,   108,     0,     0,   114,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,   109,     0,   111,     0,     0,     0,     0,     0,     0,
     113,     0,   109,     0,   109,     0,     0,   190,   190,   109,
       0,   190,   190,     0,     0,     0,     0,   105,   105,   105,
     105,     0,     0,     0,   113,     0,     0,   113,     0,     0,
       0,     0,   113,   105,   105,     0,     0,     0,     0,   108,
       0,     0,     0,     0,   108,     0,     0,     0,     0,     0,
       0,     0,   114,   111,     0,   111,     0,     0,   111,     0,
       0,   111,     0,     0,   108,     0,   111,     0,     0,   109,
       0,     0,     0,   108,   109,     0,   114,   108,     0,   114,
     113,     0,     0,     0,   114,     0,   111,     0,     0,     0,
       0,     0,     0,     0,   109,     0,     0,   111,     0,   111,
       0,     0,     0,   109,   111,     0,     0,   109,     0,     0,
       0,     0,     0,   190,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   190,     0,   321,     0,     0,     0,
       0,   321,   114,     0,   190,     0,     0,   190,     0,     0,
       0,     0,     0,     0,   108,   108,   108,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     108,   108,     0,     0,   111,     0,     0,     0,     0,   111,
       0,     0,     0,     0,   109,   109,   109,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
     109,   109,     0,     0,     0,     0,   128,   113,   111,     0,
       0,   128,   111,     0,     0,   151,   128,   128,     0,     0,
       0,     0,     0,   165,     0,   168,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   200,     0,     0,
       0,     0,     0,   113,     0,     0,     0,   113,     0,   114,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   190,     0,     0,     0,   111,
     111,   111,   111,     0,     0,     0,   227,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   111,     0,     0,     0,
       0,     0,     0,     0,     0,   114,     0,     0,     0,   114,
       0,   321,   321,     0,   321,   321,     0,   113,     0,   113,
       0,     0,   113,     0,     0,   113,     0,     0,     0,   822,
     113,     0,     0,     0,     0,    76,   321,     0,   321,   200,
       0,     0,     0,   300,   122,     0,     0,     0,     0,   200,
     113,   309,     0,   149,     0,     0,     0,   300,     0,     0,
     328,   113,     0,   113,   239,     0,   171,     0,   113,   114,
     300,   114,   128,     0,   114,   340,     0,   114,     0,     0,
       0,     0,   114,     0,   196,   198,   199,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,   822,     0,     0,
       0,   240,   114,     0,    76,   241,     0,   239,   225,     0,
       0,     0,     0,   114,     0,   114,     0,     0,     0,   374,
     114,     0,     0,     0,   168,   168,   168,   168,   113,     0,
       0,     0,     0,   113,     0,     0,     0,     0,     0,     0,
       0,   399,     0,   243,   240,   244,   245,   246,   241,   247,
     248,   249,     0,   113,   251,     0,     0,     0,   286,     0,
     257,   258,   113,     0,     0,   261,   113,     0,   262,   263,
     264,     0,     0,     0,   300,   300,     0,     0,   239,     0,
     114,     0,     0,     0,     0,   114,   243,     0,   244,   245,
     246,     0,   247,   248,     0,     0,     0,   251,   323,   466,
       0,   470,     0,   257,   258,   114,     0,     0,   261,     0,
       0,   262,   263,   264,   114,   240,     0,     0,   114,   241,
     328,     0,   345,     0,     0,     0,     0,     0,     0,   168,
       0,     0,     0,   113,   113,   113,   113,     0,     0,     0,
     300,     0,   242,     0,     0,     0,    76,     0,     0,   113,
     113,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,     0,   247,   248,   249,     0,   250,   251,   252,
     253,   168,   255,   256,   257,   258,     0,   259,   260,   261,
       0,     0,   262,   263,   264,   114,   114,   114,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   114,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,     0,     0,     0,   495,     0,     5,     6,     7,
       8,     9,    10,     0,  -407,     0,   130,     0,     0,  -407,
       0,     0,     0,     0,     0,   345,     0,  -407,   345,    21,
       0,     0,     0,     0,     0,   131,   132,   133,   300,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,   614,     0,  -407,     0,   328,     0,  -407,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,   200,     0,     0,     0,
    -273,  -407,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -407,    61,  -407,  -407,
    -407,     0,  -407,  -407,  -407,   685,  -407,  -407,  -407,  -407,
    -407,  -407,  -407,  -407,  -407,    66,  -407,  -407,  -407,     0,
       0,  -407,  -407,  -407,     0,     0,     0,   135,     0,    72,
       0,   510,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     5,     6,     7,     8,     9,    10,     0,  -407,     0,
     130,     0,   466,  -407,     0,     0,     0,     0,     0,     0,
       0,  -407,     0,    21,     0,     0,     0,     0,     0,   131,
     132,   133,     0,    28,     0,     0,   328,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,   610,     0,
       0,    38,     0,    40,     0,     0,     0,     0,  -407,     0,
       0,     0,  -407,     0,     0,    48,    49,    50,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -407,     0,   754,     0,     0,
       0,     0,     0,     0,   643,     0,     0,     0,    76,     0,
    -407,    61,  -407,  -407,  -407,     0,  -407,  -407,  -407,     0,
    -407,  -407,  -407,  -407,  -407,  -407,  -407,  -407,  -407,    66,
    -407,  -407,  -407,     0,     0,  -407,  -407,  -407,     0,     0,
       0,   135,     0,    72,     0,   510,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   345,     0,
     345,     0,     0,   345,     0,     0,   345,     0,     0,     0,
     128,   720,     0,     0,     0,     0,     0,     0,   800,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   732,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   738,     0,   742,     0,     0,     0,     0,    76,
       0,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,    39,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,   791,    58,     0,     0,    59,     0,
       0,     0,     0,   795,     0,     0,     0,   797,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,     0,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,  -407,     0,    11,    12,
      13,  -407,    14,    15,    16,    17,  -255,    18,    19,  -407,
      20,    21,  -407,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
       0,    40,    41,  -146,    42,    43,  -407,  -255,    44,    45,
    -407,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,  -407,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -407,    61,
    -407,  -407,  -407,  -407,  -407,  -407,  -407,     0,  -407,  -407,
    -407,  -407,  -407,  -407,  -407,  -407,  -407,    66,  -407,  -407,
    -407,     0,    69,  -407,  -407,  -407,     0,     0,     0,    71,
    -407,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,    39,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,   182,    69,     0,     0,
      70,     0,     0,     0,    71,   211,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
      39,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
     357,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,    39,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,   211,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
      39,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
     645,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,    39,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,   746,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
     224,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
       0,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,     0,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,     0,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   473,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
       0,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
       0,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   477,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,     0,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,     0,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   707,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
       0,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
       0,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   709,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,     0,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,     0,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   712,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
       0,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
       0,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   715,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,     0,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,     0,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
     731,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
       0,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
       0,    72,     4,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -255,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   737,     0,    28,    29,     0,  -146,
      30,    31,    32,    33,    34,   -38,    35,    36,     0,    37,
       0,     0,     0,  -255,    38,     0,    40,    41,  -146,    42,
      43,     0,  -255,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,     0,    52,    53,    54,     0,    55,    56,
    -255,     0,     0,    57,     0,    58,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,    69,     0,     0,
      70,     0,     0,     0,    71,     0,    72,     4,    73,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -255,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   741,
       0,    28,    29,     0,  -146,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,     0,  -255,    38,
       0,    40,    41,  -146,    42,    43,     0,  -255,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,     0,    52,
      53,    54,     0,    55,    56,  -255,     0,     0,    57,     0,
      58,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,    69,     0,     0,    70,     0,     0,     0,    71,
       0,    72,   495,    73,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   131,   132,   133,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,     0,     0,     0,     0,
      70,     0,     0,     0,   135,     0,    72,   495,   496,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   131,   132,   133,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,     0,     0,     0,     0,    64,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,    66,     0,    67,
      68,     0,     0,     0,     0,    70,     0,     0,     0,   135,
       0,    72,     0,   510,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   131,   132,   133,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,    62,    63,     0,     0,     0,
       0,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    66,     0,    67,    68,   182,     0,     0,     0,
      70,     0,     0,     0,   135,     0,    72,     0,   136,   528,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   131,   132,
     133,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,    62,    63,     0,     0,     0,     0,    64,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,    66,     0,
      67,    68,   182,     0,     0,     0,    70,     0,     0,     0,
     135,     0,    72,     0,   136,   816,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   131,   132,   133,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    63,     0,
       0,     0,     0,    64,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,    66,     0,    67,    68,     0,     0,
       0,     0,    70,     0,     0,     0,   135,     0,    72,     0,
     136,   533,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,   438,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,   439,    28,     0,   440,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
     441,   442,    38,     0,    40,     0,     0,     0,     0,     0,
     443,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,   216,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,   182,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,   215,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,   182,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,   522,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,   182,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
     524,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,   158,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,   182,
       0,     0,     0,    70,     0,     0,     0,   135,   520,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,   182,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
     186,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,    71,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   159,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,   626,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   776,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   777,    49,    50,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,   182,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   779,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,   182,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   779,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   766,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,   182,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
       0,     0,     0,   514,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   573,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   576,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   760,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   761,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   763,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   764,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   765,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      66,     0,    67,    68,     0,     0,     0,     0,    70,     0,
       0,     0,   135,     0,    72,     0,   136,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   766,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
       0,     0,     0,     0,    64,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,    66,     0,    67,    68,     0,
       0,     0,     0,    70,     0,     0,     0,   135,     0,    72,
       0,   136,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     131,   132,   133,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   130,     0,     0,     0,
       0,    60,    61,    62,    63,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   131,   132,   133,    65,    28,
      66,     0,     0,    68,    31,    32,     0,    34,    70,    35,
      36,     0,   135,     0,    72,     0,   136,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   134,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   130,
       0,     0,  -282,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,    61,   131,   132,
     133,  -282,    28,     0,     0,  -282,     0,    31,    32,     0,
      34,     0,    35,    36,     0,    66,     0,     0,     0,   182,
      38,     5,    40,     0,     0,     0,     0,   135,   649,    72,
    -282,   779,     0,   650,    48,    49,    50,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,   237,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,   239,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,     0,   136,     0,   651,   652,   653,   654,
     655,   656,     0,     0,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   666,   240,     0,   667,     0,   241,     0,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,   679,     0,   237,   680,   681,   682,   683,   238,
     684,   242,     0,     0,     0,     0,     0,   239,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,     0,   247,   248,   249,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,   259,   260,   261,     0,
       0,   262,   263,   264,   240,     0,     0,     0,   241,     0,
       0,     0,   699,     0,     0,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,   317,   239,     0,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,     0,   247,   248,   249,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   240,   259,   260,   261,   241,
       0,   262,   263,   264,     0,     0,   237,     0,     0,     0,
       0,   238,   641,     0,     0,     0,     0,     0,     0,   239,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,     0,   247,   248,   249,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   240,   259,   260,   261,
     241,     0,   262,   263,   264,     0,     0,   237,     0,     0,
       0,     0,   238,   825,     0,     0,     0,     0,     0,     0,
     239,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
     244,   245,   246,     0,   247,   248,   249,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   240,   259,   260,
     261,   241,     0,   262,   263,   264,     0,     0,   237,     0,
       0,     0,     0,   238,   862,     0,     0,     0,     0,     0,
       0,   239,   315,     0,   242,     0,     0,     0,     0,     0,
       0,     0,   316,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   240,   259,
     260,   261,   241,     0,   262,   263,   264,     0,     0,   237,
       0,     0,     0,   521,   238,     0,     0,     0,     0,     0,
       0,   317,   239,   312,     0,   242,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   240,
     259,   260,   261,   241,     0,   262,   263,   264,     0,     0,
     237,   288,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   239,   603,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     240,   259,   260,   261,   241,     0,   262,   263,   264,     0,
       0,   237,   288,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,   317,   239,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,     0,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   240,   259,   260,   261,   241,     0,   262,   263,   264,
       0,     0,   320,   288,     0,     0,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,   242,     0,
     239,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,   244,   245,   246,     0,   247,
     248,   249,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,     0,   259,   260,   261,     0,   240,   262,   263,
     264,   241,     0,     0,   288,     0,     0,     0,   237,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,   239,   597,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   240,   259,
     260,   261,   241,     0,   262,   263,   264,     0,     0,   237,
     288,     0,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   240,
     259,   260,   261,   241,     0,   262,   263,   264,     0,     0,
     706,   288,     0,     0,     0,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,   242,     0,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
       0,   259,   260,   261,     0,   240,   262,   263,   264,   241,
       0,     0,   288,     0,     0,     0,   711,     0,     0,     0,
       0,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,   242,     0,   239,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,     0,   247,   248,   249,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,     0,   259,   260,   261,
       0,   240,   262,   263,   264,   241,     0,     0,   288,     0,
       0,     0,   811,     0,     0,     0,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,   242,     0,
     239,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,   244,   245,   246,     0,   247,
     248,   249,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,     0,   259,   260,   261,     0,   240,   262,   263,
     264,   241,     0,     0,   288,     0,     0,     0,   812,     0,
       0,     0,     0,   237,     0,     0,     0,     0,   238,     0,
       0,     0,     0,     0,   242,     0,   239,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,     0,   259,
     260,   261,     0,   240,   262,   263,   264,   241,     0,     0,
     288,     0,     0,     0,   813,     0,     0,     0,     0,   237,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
     242,     0,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,   244,   245,   246,
       0,   247,   248,   249,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,     0,   259,   260,   261,     0,   240,
     262,   263,   264,   241,     0,     0,   288,     0,     0,     0,
     814,     0,     0,     0,     0,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,   242,     0,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
       0,   259,   260,   261,     0,   240,   262,   263,   264,   241,
       0,     0,   288,     0,     0,     0,   827,     0,     0,     0,
       0,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,   242,     0,   239,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,     0,   247,   248,   249,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,     0,   259,   260,   261,
       0,   240,   262,   263,   264,   241,     0,     0,   288,     0,
       0,     0,   828,     0,     0,     0,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,   242,     0,
     239,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,   244,   245,   246,     0,   247,
     248,   249,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,     0,   259,   260,   261,     0,   240,   262,   263,
     264,   241,     0,     0,   288,     0,     0,     0,   237,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   240,   259,
     260,   261,   241,     0,   262,   263,   264,     0,     0,   237,
     344,     0,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,   294,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   240,
     259,   260,   261,   241,     0,   262,   263,   264,     0,     0,
    -367,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     237,   259,   260,   261,     0,   238,   262,   263,   264,     0,
       0,     0,     0,   239,     0,     0,   240,     0,   353,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   354,
       0,     0,     0,   237,     0,     0,     0,     0,   238,     0,
     240,     0,     0,     0,   241,     0,   239,   450,   243,     0,
     244,   245,   246,     0,   247,   248,   249,   451,   250,   251,
     252,   253,     0,   255,   256,   257,   258,   242,   259,     0,
     261,     0,     0,   262,   263,   264,     0,     0,     0,     0,
       0,     0,   243,   240,   244,   245,   246,   241,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,     0,   259,   260,   261,     0,   237,   262,   263,   264,
     242,   238,     0,     0,     0,     0,     0,     0,     0,   239,
     452,     0,     0,     0,     0,   243,     0,   244,   245,   246,
     453,   247,   248,   249,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,     0,   259,   260,   261,     0,   237,
     262,   263,   264,     0,   238,     0,   240,     0,     0,     0,
     241,     0,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   240,
     244,   245,   246,   241,   247,   248,   249,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,     0,   259,   260,
     261,     0,   237,   262,   263,   264,   242,   238,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
       0,   259,   260,   261,     0,   298,   262,   263,   264,   237,
       0,     0,   240,     0,   238,     0,   241,     0,     0,     0,
       0,     0,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,   244,   245,   246,   240,
     247,   248,   249,   241,   250,   251,   252,   253,   254,   255,
     256,   257,   258,     0,   259,   260,   261,     0,   343,   262,
     263,   264,   237,     0,     0,     0,   242,   238,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
       0,   259,   260,   261,     0,   356,   262,   263,   264,     0,
       0,     0,   240,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   370,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,   244,   245,   246,     0,
     247,   248,   249,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   240,   259,   260,   261,   241,     0,   262,
     263,   264,     0,     0,   454,     0,     0,     0,     0,   237,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
     242,     0,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,   244,   245,   246,
       0,   247,   248,   249,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,     0,   259,   260,   261,     0,   240,
     262,   263,   264,   241,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   239,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     240,   259,   260,   261,   241,   548,   262,   263,   264,     0,
       0,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,     0,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   240,   259,   260,   261,   241,   549,   262,   263,   264,
       0,     0,   237,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,   244,   245,   246,     0,   247,
     248,   249,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   240,   259,   260,   261,   241,   550,   262,   263,
     264,     0,     0,   237,     0,     0,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,   239,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,   244,   245,   246,     0,
     247,   248,   249,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   240,   259,   260,   261,   241,   551,   262,
     263,   264,     0,     0,   237,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,   239,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,   244,   245,   246,
       0,   247,   248,   249,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   240,   259,   260,   261,   241,   552,
     262,   263,   264,     0,     0,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,     0,   247,   248,   249,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   240,   259,   260,   261,   241,
     553,   262,   263,   264,     0,     0,   237,     0,     0,     0,
       0,   238,     0,     0,     0,     0,     0,     0,     0,   239,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,     0,   247,   248,   249,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   240,   259,   260,   261,
     241,   554,   262,   263,   264,     0,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
     244,   245,   246,     0,   247,   248,   249,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   240,   259,   260,
     261,   241,   555,   262,   263,   264,     0,     0,   237,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   240,   259,
     260,   261,   241,   556,   262,   263,   264,     0,     0,   237,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   240,
     259,   260,   261,   241,   557,   262,   263,   264,     0,     0,
     237,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   239,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     240,   259,   260,   261,   241,   558,   262,   263,   264,     0,
       0,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,     0,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   240,   259,   260,   261,   241,   559,   262,   263,   264,
       0,     0,   237,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,   244,   245,   246,     0,   247,
     248,   249,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   240,   259,   260,   261,   241,   560,   262,   263,
     264,     0,     0,   237,     0,     0,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,   239,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,   244,   245,   246,     0,
     247,   248,   249,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   240,   259,   260,   261,   241,   561,   262,
     263,   264,     0,     0,   237,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,   239,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,   244,   245,   246,
       0,   247,   248,   249,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   240,   259,   260,   261,   241,   562,
     262,   263,   264,     0,     0,   237,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,     0,   247,   248,   249,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   240,   259,   260,   261,   241,
     563,   262,   263,   264,     0,     0,   237,     0,     0,     0,
       0,   238,     0,     0,     0,     0,     0,     0,   124,   239,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,   244,
     245,   246,     0,   247,   248,   249,     0,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   240,   259,   260,   261,
     241,     0,   262,   263,   264,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,   238,     0,   242,     0,     0,     0,     0,     0,   239,
       0,     0,   609,     0,     0,     0,     0,     0,   243,     0,
     244,   245,   246,     0,   247,   248,   249,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,     0,   259,   260,
     261,     0,   583,   262,   263,   264,   240,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   237,   637,     0,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
     244,   245,   246,     0,   247,   248,   249,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   240,   259,   260,
     261,   241,     0,   262,   263,   264,     0,     0,   237,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   240,   259,
     260,   261,   241,     0,   262,   263,   264,     0,     0,   237,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,   239,   694,   644,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   240,
     259,   260,   261,   241,     0,   262,   263,   264,     0,     0,
     237,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,   239,   697,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,   244,   245,   246,     0,   247,   248,   249,
       0,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     240,   259,   260,   261,   241,     0,   262,   263,   264,     0,
       0,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,     0,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   240,   259,   260,   261,   241,     0,   262,   263,   264,
       0,     0,   759,     0,     0,     0,     0,   237,     0,     0,
       0,     0,   238,     0,     0,     0,     0,     0,   242,     0,
     239,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,   244,   245,   246,     0,   247,
     248,   249,     0,   250,   251,   252,   253,   254,   255,   256,
     257,   258,     0,   259,   260,   261,     0,   240,   262,   263,
     264,   241,     0,     0,     0,     0,     0,     0,   762,     0,
       0,     0,     0,   237,   793,     0,     0,     0,   238,     0,
       0,     0,     0,     0,   242,     0,   239,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,   244,   245,   246,     0,   247,   248,   249,     0,   250,
     251,   252,   253,   254,   255,   256,   257,   258,     0,   259,
     260,   261,     0,   240,   262,   263,   264,   241,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,   239,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,   244,   245,   246,
       0,   247,   248,   249,     0,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   240,   259,   260,   261,   241,     0,
     262,   263,   264,     0,     0,   834,     0,     0,     0,     0,
     237,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,   242,     0,   239,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,     0,   247,   248,   249,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,     0,   259,   260,   261,     0,
     240,   262,   263,   264,   241,     0,     0,     0,     0,     0,
       0,   835,     0,     0,     0,     0,   237,     0,     0,     0,
       0,   238,     0,     0,     0,     0,     0,   242,     0,   239,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,     0,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,     0,   259,   260,   261,     0,   240,   262,   263,   264,
     241,     0,     0,     0,     0,     0,     0,   836,     0,     0,
       0,     0,   237,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,   242,     0,   239,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
     244,   245,   246,     0,   247,   248,   249,     0,   250,   251,
     252,   253,   254,   255,   256,   257,   258,     0,   259,   260,
     261,     0,   240,   262,   263,   264,   241,     0,     0,     0,
       0,     0,     0,   837,     0,     0,     0,     0,   237,     0,
       0,     0,     0,   238,     0,     0,     0,     0,     0,   242,
       0,   239,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,   244,   245,   246,     0,
     247,   248,   249,     0,   250,   251,   252,   253,   254,   255,
     256,   257,   258,     0,   259,   260,   261,     0,   240,   262,
     263,   264,   241,     0,     0,     0,     0,     0,     0,   838,
       0,     0,     0,     0,   237,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,   242,     0,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,   254,   255,   256,   257,   258,     0,
     259,   260,   261,     0,   240,   262,   263,   264,   241,     0,
       0,     0,     0,     0,     0,   839,     0,     0,     0,     0,
     237,     0,     0,     0,     0,   238,     0,     0,     0,     0,
       0,   242,     0,   239,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,   244,   245,
     246,     0,   247,   248,   249,     0,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   239,   259,   260,   261,     0,
     240,   262,   263,   264,   241,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,   242,     0,     0,
       0,     0,   240,     0,     0,     0,   241,     0,     0,     0,
       0,     0,   243,     0,   244,   245,   246,     0,   247,   248,
     249,     0,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   571,   259,   260,   261,   241,     0,   262,   263,   264,
       0,   239,     0,     0,   243,     0,   244,   245,   246,     0,
     247,   248,   249,     0,     0,   251,   252,   253,   242,   255,
     256,   257,   258,     0,     0,     0,   261,     0,     0,   262,
     263,   264,     0,   243,     0,   244,   245,   246,   240,   247,
     248,   249,   241,   250,   251,   252,   253,   254,   255,   256,
     257,   258,     0,   259,   260,   261,     0,     0,   262,   263,
     264,     0,     0,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,   244,   245,   246,     0,   247,   248,   249,     0,
     250,   251,   252,   253,     0,   255,   256,   257,   258,     0,
     259,     0,   261,     0,     0,   262,   263,   264
};

static const yytype_int16 yycheck[] =
{
      11,    41,    42,   285,     2,    16,   324,    15,    19,   349,
     146,   327,   623,    23,    25,    26,    27,   494,   164,     3,
     156,   221,   332,    34,    35,   647,    37,    38,     1,     3,
     647,   177,     1,    44,    45,    46,    47,    48,   481,   349,
      51,   231,   232,   233,   234,    56,    57,    58,     3,   195,
      61,    44,    20,    64,    65,    66,   687,    68,     3,    24,
      71,    72,    73,    56,   787,   160,   161,   162,     1,     3,
      24,    34,    24,    24,    34,    50,    34,   107,    63,   174,
      73,   107,     3,    83,    52,    34,   107,    61,   107,   851,
      63,    25,    76,    61,    63,     0,    83,   192,   107,    20,
     862,    34,    63,    72,     3,    80,   136,    25,    81,   204,
     136,    79,    81,    34,   725,   134,    37,   138,    83,   130,
     131,   132,   133,   134,   135,   136,   569,   136,   816,   751,
      51,    52,   855,   106,   142,   281,   282,   106,   138,   150,
      61,    75,   619,   136,   107,   767,   135,   107,   133,   107,
     767,   135,    16,   164,    75,    88,   107,   126,   107,   134,
     133,   135,    20,   851,   133,   126,   177,   107,   133,   179,
     801,   802,   133,   106,   107,   138,   135,   493,   138,   133,
     135,   133,   133,    34,   195,    88,    75,   107,   378,   138,
     135,   107,   107,   126,    52,    59,   136,    13,   126,   481,
     295,   347,    18,    61,   815,   216,    72,   135,   111,   137,
      61,     7,    34,    77,   135,   213,   136,    75,     3,    60,
     136,   136,     7,    64,   319,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,    34,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   718,   753,   107,   107,   479,
     480,   107,     1,   294,   150,   107,    83,   569,   299,   106,
       3,    60,   109,   611,     7,    64,   113,   613,   164,   310,
      38,   312,   313,   135,   315,   316,   136,   136,   134,   320,
     107,   177,   622,    23,    88,    34,   138,   310,    69,    57,
     107,   107,   240,   241,   335,    38,   526,   527,   107,   195,
     341,   135,   135,   107,   137,    44,   347,   111,   126,   126,
     126,   138,   353,   354,    57,   126,   126,   126,   359,    88,
      60,   362,   107,   364,    64,   366,   367,   368,   126,   370,
     371,     2,   136,   126,   375,   283,   284,    86,   107,    88,
      11,   126,   111,   135,   107,   137,   294,   513,   132,    20,
     475,   476,   375,   107,   479,   480,    23,   106,   107,   135,
     730,   137,    33,   126,   734,   313,   106,   136,   316,   109,
     107,   126,   126,   113,   126,   135,   107,   126,    52,   126,
      51,    52,    53,   133,   724,   281,   282,    61,   126,   126,
     730,    82,   133,    60,   734,   126,    86,    64,    88,   135,
      71,    75,   135,   362,    75,   364,   718,    23,   756,   450,
     451,   452,   453,   454,   455,   456,   109,   135,   366,   457,
     458,   462,   463,    88,   465,   126,     7,   106,   126,   136,
     471,   107,   473,    20,   126,   102,   477,   104,   105,   106,
     481,   108,   109,    34,    60,   341,   113,    34,    64,   135,
      37,   347,    26,   120,   125,   496,   581,   582,   499,   500,
     127,   128,   129,   359,    51,    52,   362,   592,   364,   510,
     136,   367,    19,   496,    61,   107,   816,   602,   136,    86,
     605,   136,    29,    30,    88,   825,   136,   510,    75,    36,
     106,    38,   108,   109,   165,   107,   138,   113,   133,   133,
     136,   138,     3,   451,   120,   453,     7,    26,     9,    88,
      57,   851,   136,   129,   136,   126,   126,   107,   189,    20,
     126,     3,   862,    16,   107,     7,    19,    24,   569,    24,
     571,   572,   573,   138,    27,   576,    29,    30,    20,   133,
     106,    61,   213,    36,   109,    38,   138,   588,    34,   138,
     135,    52,   136,    81,   595,    72,   597,   535,   354,   600,
      61,    54,   603,    56,    57,    58,    59,   463,   609,   465,
      52,   157,   470,   704,    75,   755,   589,    63,    79,    61,
     755,     2,    75,   756,    77,   611,    72,   648,   629,   855,
      11,   815,   830,    75,   635,    81,   637,    79,   639,    20,
     641,   336,    88,   644,   775,    -1,   629,   465,   733,    -1,
      -1,    -1,    33,    -1,    -1,    -1,    -1,   288,   646,    -1,
      -1,   107,    -1,   571,   572,    -1,    19,    -1,    -1,    -1,
      51,    52,    53,   126,    -1,    -1,    29,    30,    -1,    -1,
     126,   312,    -1,    36,   315,    38,    -1,   133,    -1,   320,
      71,    44,   693,   694,    75,   696,   697,   698,   699,    -1,
      -1,    -1,    -1,    -1,    57,   706,   707,    -1,   709,    -1,
     711,   712,    16,    -1,   715,    19,    -1,   718,   748,   749,
      -1,    -1,    -1,    27,    -1,    29,    30,    -1,   729,   730,
      -1,    -1,    36,   734,    38,    -1,   737,   368,    -1,    -1,
     741,    -1,    -1,    -1,   125,    -1,    -1,    -1,     2,    -1,
      54,    -1,    56,    57,    58,    59,    -1,    11,   759,   760,
     761,   762,   763,   764,   765,   766,    20,    -1,    -1,    -1,
      -1,    75,    -1,    77,    -1,    -1,   777,   778,   779,    33,
      -1,    -1,   783,    -1,   165,   786,    -1,    -1,    -1,    -1,
      19,    -1,   793,    -1,    -1,    -1,   779,    51,    52,    53,
      29,    30,    -1,    -1,    23,    -1,    -1,    36,   189,    38,
     811,   812,   813,   814,   445,    44,    -1,    71,    -1,    -1,
      -1,    75,   126,    -1,   825,    -1,   827,   828,    57,    -1,
     134,    -1,   213,   834,   835,   836,   837,   838,   839,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,   850,
     851,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,    -1,
      -1,   862,    -1,    -1,    -1,    -1,    -1,    -1,   724,    -1,
      -1,   125,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    11,    -1,   105,   106,    -1,   108,
     109,    -1,    -1,    20,   113,    -1,    -1,    -1,    -1,    -1,
     531,   120,    -1,    -1,   535,     2,    33,   288,   127,   128,
     129,   165,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    51,    52,    53,   783,    -1,    -1,
      -1,   312,    -1,    -1,   315,   189,    33,    -1,    34,   320,
     571,   572,    -1,    -1,    71,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    -1,    51,    52,    53,    -1,    -1,   213,
     816,    -1,    -1,    -1,   595,    -1,   597,    63,    -1,   600,
      -1,    -1,   603,    -1,    71,    34,    72,   608,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,   368,    -1,    -1,
      -1,    -1,    88,    -1,    -1,   851,    -1,   628,   125,    -1,
       2,    -1,    -1,    -1,    63,    -1,   862,    -1,   639,    11,
     641,   107,    -1,    72,    -1,   646,    -1,    -1,    20,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,   125,    88,
     126,    33,    -1,    -1,   288,    23,    -1,   133,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    51,
      52,    53,    23,    -1,    -1,    -1,    -1,    -1,   312,    -1,
      -1,   315,   189,    -1,    -1,    -1,   320,   126,   165,    71,
      -1,    -1,    60,    75,   133,   706,    64,    -1,    -1,    -1,
     711,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    60,
      -1,    -1,   189,    64,    -1,    -1,    -1,    -1,    -1,    -1,
     731,    -1,    -1,    -1,   485,    -1,    -1,    -1,    -1,   740,
      -1,    -1,    -1,   744,   368,    -1,   213,    -1,   106,    47,
     108,   109,    -1,   125,    -1,   113,    -1,    -1,    -1,    -1,
      -1,   102,   120,    16,    -1,   106,    19,   108,   109,   127,
     128,   129,   113,    -1,    27,    -1,    29,    30,    -1,   120,
     531,    -1,    -1,    36,   535,    38,   127,   128,   129,    -1,
      -1,   288,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,
     811,   812,   813,   814,    -1,   312,    -1,   189,   315,    -1,
      -1,   288,    75,   320,    77,    -1,   827,   828,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   213,    -1,    -1,   595,   312,   597,    -1,   315,   600,
      -1,    -1,   603,   320,    -1,    -1,    -1,   608,    -1,    -1,
      -1,   485,   160,   161,   162,   163,    -1,    -1,    -1,    -1,
      -1,   368,    -1,   126,    -1,    -1,   174,   628,    -1,    -1,
      -1,   134,    -1,    -1,     2,    -1,    -1,    -1,   639,    -1,
     641,    -1,    -1,    11,   192,   646,    -1,    -1,    -1,    -1,
      -1,   368,    20,    -1,    -1,    -1,   204,   531,    -1,    -1,
      -1,   535,    -1,    -1,    -1,    33,   288,    -1,    -1,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    51,    52,    53,     2,    -1,    -1,    -1,
     312,    -1,    -1,   315,    -1,    11,    -1,    -1,   320,    -1,
      -1,    -1,    -1,    71,    20,   706,    -1,    75,    -1,    -1,
     711,   130,   131,   132,    -1,    -1,   135,    33,   137,    -1,
      -1,   595,    -1,   597,    -1,    -1,   600,    -1,    -1,   603,
     731,    -1,    -1,    -1,   608,    51,    52,    53,   485,   740,
      -1,    -1,    -1,   744,    -1,    -1,   368,   295,    -1,    -1,
      -1,    -1,    -1,    -1,   628,    71,    -1,   125,    -1,    75,
      -1,    -1,    -1,    -1,    -1,   639,    -1,   641,   485,    -1,
      -1,   319,   646,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   531,    -1,    -1,    -1,   535,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     811,   812,   813,   814,   531,    -1,    -1,    -1,   535,    -1,
      -1,   189,    -1,    -1,    -1,    -1,   827,   828,    -1,    -1,
      -1,    -1,   706,    -1,    -1,    -1,    -1,   711,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   595,   165,
     597,    -1,    -1,   600,    -1,    -1,   603,   731,    -1,    -1,
      -1,   608,    -1,   485,    -1,    -1,   740,    -1,    -1,    -1,
     744,    -1,    -1,   189,    -1,    -1,    -1,    -1,   595,    -1,
     597,   628,    -1,   600,    -1,    -1,   603,    -1,    -1,    -1,
      -1,   608,   639,    -1,   641,    -1,    -1,   213,    -1,   646,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   531,
      -1,   628,    -1,   535,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,   639,    -1,   641,    -1,    -1,   475,   476,   646,
      -1,   479,   480,    -1,    -1,    -1,    -1,   811,   812,   813,
     814,    -1,    -1,    -1,   312,    -1,    -1,   315,    -1,    -1,
      -1,    -1,   320,   827,   828,    -1,    -1,    -1,    -1,   706,
      -1,    -1,    -1,    -1,   711,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,   595,    -1,   597,    -1,    -1,   600,    -1,
      -1,   603,    -1,    -1,   731,    -1,   608,    -1,    -1,   706,
      -1,    -1,    -1,   740,   711,    -1,   312,   744,    -1,   315,
     368,    -1,    -1,    -1,   320,    -1,   628,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   731,    -1,    -1,   639,    -1,   641,
      -1,    -1,    -1,   740,   646,    -1,    -1,   744,    -1,    -1,
      -1,    -1,    -1,   581,   582,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   592,    -1,   594,    -1,    -1,    -1,
      -1,   599,   368,    -1,   602,    -1,    -1,   605,    -1,    -1,
      -1,    -1,    -1,    -1,   811,   812,   813,   814,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     827,   828,    -1,    -1,   706,    -1,    -1,    -1,    -1,   711,
      -1,    -1,    -1,    -1,   811,   812,   813,   814,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   731,
     827,   828,    -1,    -1,    -1,    -1,    13,   485,   740,    -1,
      -1,    18,   744,    -1,    -1,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,   531,    -1,    -1,    -1,   535,    -1,   485,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   733,    -1,    -1,    -1,   811,
     812,   813,   814,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   827,   828,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   531,    -1,    -1,    -1,   535,
      -1,   769,   770,    -1,   772,   773,    -1,   595,    -1,   597,
      -1,    -1,   600,    -1,    -1,   603,    -1,    -1,    -1,   787,
     608,    -1,    -1,    -1,    -1,     2,   794,    -1,   796,   146,
      -1,    -1,    -1,   150,    11,    -1,    -1,    -1,    -1,   156,
     628,   158,    -1,    20,    -1,    -1,    -1,   164,    -1,    -1,
     167,   639,    -1,   641,    23,    -1,    33,    -1,   646,   595,
     177,   597,   179,    -1,   600,   182,    -1,   603,    -1,    -1,
      -1,    -1,   608,    -1,    51,    52,    53,    -1,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   855,    -1,    -1,
      -1,    60,   628,    -1,    71,    64,    -1,    23,    75,    -1,
      -1,    -1,    -1,   639,    -1,   641,    -1,    -1,    -1,   226,
     646,    -1,    -1,    -1,   231,   232,   233,   234,   706,    -1,
      -1,    -1,    -1,   711,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   248,    -1,   102,    60,   104,   105,   106,    64,   108,
     109,   110,    -1,   731,   113,    -1,    -1,    -1,   125,    -1,
     119,   120,   740,    -1,    -1,   124,   744,    -1,   127,   128,
     129,    -1,    -1,    -1,   281,   282,    -1,    -1,    23,    -1,
     706,    -1,    -1,    -1,    -1,   711,   102,    -1,   104,   105,
     106,    -1,   108,   109,    -1,    -1,    -1,   113,   165,   306,
      -1,   308,    -1,   119,   120,   731,    -1,    -1,   124,    -1,
      -1,   127,   128,   129,   740,    60,    -1,    -1,   744,    64,
     327,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,   336,
      -1,    -1,    -1,   811,   812,   813,   814,    -1,    -1,    -1,
     347,    -1,    87,    -1,    -1,    -1,   213,    -1,    -1,   827,
     828,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   378,   117,   118,   119,   120,    -1,   122,   123,   124,
      -1,    -1,   127,   128,   129,   811,   812,   813,   814,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   827,   828,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,   312,    -1,    23,   315,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,   465,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,   489,    -1,    60,    -1,   493,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,   513,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
     106,    -1,   108,   109,   110,   542,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    -1,
      -1,   127,   128,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,   589,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,   613,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,   485,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,   664,    -1,    -1,
      -1,    -1,    -1,    -1,   531,    -1,    -1,    -1,   535,    -1,
     102,   103,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,    -1,   127,   128,   129,    -1,    -1,
      -1,   133,    -1,   135,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   595,    -1,
     597,    -1,    -1,   600,    -1,    -1,   603,    -1,    -1,    -1,
     747,   608,    -1,    -1,    -1,    -1,    -1,    -1,   755,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   628,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   639,    -1,   641,    -1,    -1,    -1,    -1,   646,
      -1,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,   731,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,   740,    -1,    -1,    -1,   744,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,   126,   127,   128,   129,    -1,    -1,    -1,   133,
     134,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,   125,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,   134,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
     134,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,   134,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
     134,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,   134,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    -1,    73,    74,    75,    -1,    77,    78,
      79,    -1,    -1,    82,    -1,    84,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    -1,    73,
      74,    75,    -1,    77,    78,    79,    -1,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,     1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,    -1,    -1,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,     1,   137,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,   123,
     124,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,   133,
      -1,   135,    -1,   137,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    -1,   123,   124,   125,    -1,    -1,    -1,
     129,    -1,    -1,    -1,   133,    -1,   135,    -1,   137,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
     103,   104,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,    -1,
     123,   124,   125,    -1,    -1,    -1,   129,    -1,    -1,    -1,
     133,    -1,   135,    -1,   137,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,    -1,
     137,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    37,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      51,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,   136,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
     136,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    52,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,   134,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
     126,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,   123,   124,    -1,    -1,    -1,    -1,   129,    -1,
      -1,    -1,   133,    -1,   135,    -1,   137,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,   123,   124,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,    -1,   135,
      -1,   137,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,   119,    35,
     121,    -1,    -1,   124,    40,    41,    -1,    43,   129,    45,
      46,    -1,   133,    -1,   135,    -1,   137,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,   103,    31,    32,
      33,   107,    35,    -1,    -1,   111,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,   121,    -1,    -1,    -1,   125,
      53,     3,    55,    -1,    -1,    -1,    -1,   133,    10,   135,
     136,   137,    -1,    15,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,   137,    -1,    88,    89,    90,    91,
      92,    93,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,    60,    -1,   108,    -1,    64,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,    -1,    10,   127,   128,   129,   130,    15,
     132,    87,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,   123,   124,    -1,
      -1,   127,   128,   129,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,   138,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    83,    23,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    60,   122,   123,   124,    64,
      -1,   127,   128,   129,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,   138,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    60,   122,   123,   124,
      64,    -1,   127,   128,   129,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,   138,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    60,   122,   123,
     124,    64,    -1,   127,   128,   129,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,   138,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    60,   122,
     123,   124,    64,    -1,   127,   128,   129,    -1,    -1,    10,
      -1,    -1,    -1,   136,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    23,    24,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    60,
     122,   123,   124,    64,    -1,   127,   128,   129,    -1,    -1,
      10,   133,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      60,   122,   123,   124,    64,    -1,   127,   128,   129,    -1,
      -1,    10,   133,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    23,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    60,   122,   123,   124,    64,    -1,   127,   128,   129,
      -1,    -1,    71,   133,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,    60,   127,   128,
     129,    64,    -1,    -1,   133,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    60,   122,
     123,   124,    64,    -1,   127,   128,   129,    -1,    -1,    10,
     133,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    60,
     122,   123,   124,    64,    -1,   127,   128,   129,    -1,    -1,
      71,   133,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    87,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,   123,   124,    -1,    60,   127,   128,   129,    64,
      -1,    -1,   133,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    -1,   122,   123,   124,
      -1,    60,   127,   128,   129,    64,    -1,    -1,   133,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,    60,   127,   128,
     129,    64,    -1,    -1,   133,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,    60,   127,   128,   129,    64,    -1,    -1,
     133,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,    60,
     127,   128,   129,    64,    -1,    -1,   133,    -1,    -1,    -1,
      71,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    87,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,   123,   124,    -1,    60,   127,   128,   129,    64,
      -1,    -1,   133,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    -1,   122,   123,   124,
      -1,    60,   127,   128,   129,    64,    -1,    -1,   133,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,    60,   127,   128,
     129,    64,    -1,    -1,   133,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    60,   122,
     123,   124,    64,    -1,   127,   128,   129,    -1,    -1,    10,
     133,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    60,
     122,   123,   124,    64,    -1,   127,   128,   129,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      10,   122,   123,   124,    -1,    15,   127,   128,   129,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    28,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      60,    -1,    -1,    -1,    64,    -1,    23,    24,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,    34,   112,   113,
     114,   115,    -1,   117,   118,   119,   120,    87,   122,    -1,
     124,    -1,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    60,   104,   105,   106,    64,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    -1,   122,   123,   124,    -1,    10,   127,   128,   129,
      87,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
      34,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,    10,
     127,   128,   129,    -1,    15,    -1,    60,    -1,    -1,    -1,
      64,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    60,
     104,   105,   106,    64,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,    10,   127,   128,   129,    87,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,   123,   124,    -1,   126,   127,   128,   129,    10,
      -1,    -1,    60,    -1,    15,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,    60,
     108,   109,   110,    64,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    -1,   122,   123,   124,    -1,   126,   127,
     128,   129,    10,    -1,    -1,    -1,    87,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,   123,   124,    -1,   126,   127,   128,   129,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    60,   122,   123,   124,    64,    -1,   127,
     128,   129,    -1,    -1,    71,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    -1,   122,   123,   124,    -1,    60,
     127,   128,   129,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      60,   122,   123,   124,    64,   126,   127,   128,   129,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    60,   122,   123,   124,    64,   126,   127,   128,   129,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    60,   122,   123,   124,    64,   126,   127,   128,
     129,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    60,   122,   123,   124,    64,   126,   127,
     128,   129,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    60,   122,   123,   124,    64,   126,
     127,   128,   129,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    60,   122,   123,   124,    64,
     126,   127,   128,   129,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    60,   122,   123,   124,
      64,   126,   127,   128,   129,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    60,   122,   123,
     124,    64,   126,   127,   128,   129,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    60,   122,
     123,   124,    64,   126,   127,   128,   129,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    60,
     122,   123,   124,    64,   126,   127,   128,   129,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      60,   122,   123,   124,    64,   126,   127,   128,   129,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    60,   122,   123,   124,    64,   126,   127,   128,   129,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    60,   122,   123,   124,    64,   126,   127,   128,
     129,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    60,   122,   123,   124,    64,   126,   127,
     128,   129,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    60,   122,   123,   124,    64,   126,
     127,   128,   129,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    60,   122,   123,   124,    64,
     126,   127,   128,   129,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    83,    23,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,   118,   119,   120,    60,   122,   123,   124,
      64,    -1,   127,   128,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    87,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,   126,   127,   128,   129,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    60,   122,   123,
     124,    64,    -1,   127,   128,   129,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    60,   122,
     123,   124,    64,    -1,   127,   128,   129,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    60,
     122,   123,   124,    64,    -1,   127,   128,   129,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      60,   122,   123,   124,    64,    -1,   127,   128,   129,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    60,   122,   123,   124,    64,    -1,   127,   128,   129,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,    60,   127,   128,
     129,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,    60,   127,   128,   129,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,   118,   119,   120,    60,   122,   123,   124,    64,    -1,
     127,   128,   129,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,   123,   124,    -1,
      60,   127,   128,   129,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    87,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    -1,   122,   123,   124,    -1,    60,   127,   128,   129,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,    -1,   122,   123,
     124,    -1,    60,   127,   128,   129,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    -1,   122,   123,   124,    -1,    60,   127,
     128,   129,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,    60,   127,   128,   129,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    23,   122,   123,   124,    -1,
      60,   127,   128,   129,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,    60,   122,   123,   124,    64,    -1,   127,   128,   129,
      -1,    23,    -1,    -1,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,    -1,    -1,   113,   114,   115,    87,   117,
     118,   119,   120,    -1,    -1,    -1,   124,    -1,    -1,   127,
     128,   129,    -1,   102,    -1,   104,   105,   106,    60,   108,
     109,   110,    64,   112,   113,   114,   115,   116,   117,   118,
     119,   120,    -1,   122,   123,   124,    -1,    -1,   127,   128,
     129,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,    -1,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,    -1,
     122,    -1,   124,    -1,    -1,   127,   128,   129
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   143,   144,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    73,    74,    75,    77,    78,    82,    84,    87,
     102,   103,   104,   105,   110,   119,   121,   123,   124,   126,
     129,   133,   135,   137,   145,   146,   147,   148,   149,   150,
     155,   156,   157,   159,   162,   163,   164,   165,   166,   168,
     169,   170,   173,   174,   177,   180,   183,   184,   206,   209,
     210,   228,   229,   230,   231,   232,   233,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   251,   252,   253,
     254,   255,   147,   243,    83,   234,   235,   158,   159,   234,
      12,    31,    32,    33,    70,   133,   137,   180,   228,   232,
     241,   242,   243,   244,   246,   247,    75,   158,   243,   147,
     135,   159,     7,   158,   160,     9,    75,   160,    52,    85,
     167,   243,   243,   243,   135,   159,   181,   135,   159,   211,
     212,   147,   243,   243,   243,   243,     7,   135,    19,    30,
     164,   164,   125,   202,   222,   243,   126,   243,   243,    24,
     150,   161,   243,   243,    69,   135,   147,   243,   147,   147,
     159,   207,   222,   243,   243,   243,   243,   243,   243,   243,
     243,   134,   145,   151,   222,    76,   111,   202,   223,   224,
     243,   222,   243,   250,    54,   147,    44,   159,    38,    57,
     197,    20,    52,    61,    79,   126,   132,    10,    15,    23,
      60,    64,    87,   102,   104,   105,   106,   108,   109,   110,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   122,
     123,   124,   127,   128,   129,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   130,
     131,   135,   137,    60,    64,   135,   147,   126,   133,   150,
     243,   243,   243,   222,    34,   234,   207,   126,   126,    82,
     159,   202,   225,   226,   227,   243,   133,   207,   174,   159,
     135,   161,    24,    34,   161,    24,    34,    83,   161,   237,
      71,   150,   225,   147,   135,   191,    76,   135,   159,   213,
     214,     1,   106,   216,   217,    34,   107,   161,   225,   160,
     159,   107,   126,   126,   133,   147,   161,   135,   225,    88,
     208,   126,   126,    28,    49,   161,   126,   134,   145,   107,
     134,   243,   107,   136,   107,   136,    34,   107,   138,   237,
      86,   107,   138,     7,   159,   106,   175,   185,    61,   211,
     211,   211,   211,   243,   243,   243,   243,   167,   243,   167,
     243,   243,   243,   243,   243,   243,   243,    25,    75,   159,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   225,   225,   167,   243,   167,   243,    20,    34,
      37,    51,    52,    61,    75,   195,   236,   239,   243,   255,
      24,    34,    24,    34,    71,    34,   138,   167,   243,   161,
     126,   243,    86,    88,   136,   107,   159,   178,   179,   126,
     159,    34,   222,    33,   243,   167,   243,    33,   243,   167,
     243,   135,   161,   147,   243,    26,   136,   192,   193,   194,
     195,   182,   214,   107,   136,     1,   137,   218,   229,    86,
      88,   215,   243,   212,   136,   202,   243,   171,   225,   136,
     137,   218,   229,   107,   129,   152,   154,   243,   152,   153,
     134,   136,   136,   223,   136,   223,   167,   243,   138,   147,
     243,   138,   243,   138,   243,   133,   222,   133,    52,    61,
      75,   187,   196,   211,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   136,   138,    34,    61,   228,   107,
     136,    60,    60,    33,   167,   243,    33,   167,   243,   167,
     243,   234,   234,   126,   243,   202,   243,   227,    88,   107,
     134,   175,   243,   136,   243,    24,   161,    24,   161,   243,
      24,   161,   237,    24,   161,   237,   238,   239,    26,    26,
     147,   107,   136,   135,   159,    20,    52,    61,    75,   198,
     136,   214,   106,   217,   222,   243,    47,   243,    50,    80,
     134,   172,   136,   222,   207,   107,   126,    11,   126,   138,
     237,   138,   237,   147,    86,   134,   151,   176,   186,    10,
      15,    88,    89,    90,    91,    92,    93,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   108,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   124,
     127,   128,   129,   130,   132,   159,   188,   189,   126,   239,
     228,   228,   243,    24,    24,   243,    24,    24,   138,   138,
     161,   161,   243,   179,   133,   161,    71,    33,   243,    33,
     243,    71,    33,   243,   161,    33,   243,   161,   107,   136,
     147,   243,   193,   214,   106,   221,    61,   217,   215,    34,
     138,    24,   147,   222,   138,   243,   243,    33,   147,   243,
     138,    33,   147,   243,   138,   243,   134,    30,    56,    58,
     134,   146,   163,   198,   159,   109,   135,   190,   190,    71,
      33,    33,    71,    33,    33,    33,    33,   176,   243,   243,
     243,   243,   243,   243,   239,   136,    25,    67,    70,   137,
     202,   220,   229,   111,   204,   215,    81,   205,   243,   218,
     229,   147,   161,    11,   243,   147,   243,   147,   163,   217,
     159,   188,   189,   192,   243,   243,   243,   243,   243,   243,
     134,    71,    71,    71,    71,   221,   138,   222,   202,   203,
     243,   243,   150,   162,   201,   138,   243,    71,    71,    72,
     199,   190,   190,   136,    71,    71,    71,    71,    71,    71,
     243,   243,   243,   243,   204,   215,   202,   219,   220,   229,
      34,   138,   229,   243,   243,   205,   243,   219,   220,   126,
     200,   201,   138,   219
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   142,   143,   144,   144,   145,   145,   146,   146,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   148,   148,   149,   149,
     149,   150,   150,   151,   151,   152,   152,   152,   152,   153,
     153,   154,   154,   155,   155,   155,   156,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   158,   158,   159,   160,   160,   161,   161,
     162,   162,   163,   163,   163,   163,   163,   163,   163,   164,
     164,   165,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   167,   168,   168,   168,   168,   169,
     169,   170,   171,   171,   172,   172,   172,   173,   173,   174,
     174,   174,   175,   175,   176,   176,   176,   177,   178,   178,
     178,   179,   179,   181,   182,   180,   183,   183,   183,   183,
     185,   186,   184,   187,   187,   187,   187,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   190,   190,   191,   192,   192,
     192,   193,   193,   193,   193,   194,   194,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   196,   196,   196,   196,
     197,   197,   198,   198,   198,   198,   198,   198,   199,   199,
     200,   200,   201,   201,   202,   202,   203,   203,   204,   204,
     205,   205,   206,   206,   206,   207,   207,   208,   208,   208,
     209,   209,   209,   209,   209,   210,   210,   210,   211,   211,
     212,   212,   212,   213,   213,   213,   214,   214,   214,   215,
     215,   215,   216,   216,   217,   217,   217,   217,   218,   218,
     218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
     221,   221,   221,   221,   221,   221,   221,   222,   222,   222,
     222,   223,   223,   223,   224,   224,   225,   225,   226,   226,
     227,   227,   227,   227,   227,   228,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   231,   232,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   235,   236,   236,
     237,   238,   238,   239,   239,   239,   240,   240,   241,   242,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   244,   244,   244,   244,   245,   245,
     246,   246,   246,   247,   247,   247,   248,   248,   248,   248,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   250,   250,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   252,   252,   252,   252,
     252,   252,   253,   253,   253,   253,   254,   254,   254,   254,
     255,   255,   255,   255,   255,   255,   255
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     3,     1,     5,     6,     0,     1,
       1,     2,     3,     1,     2,     1,     3,     3,     5,     0,
       1,     1,     1,     3,     5,     5,     3,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     2,
       2,     5,     0,     2,     3,     2,     3,     6,     8,     1,
       1,     1,     0,     2,     0,     2,     3,     5,     1,     2,
       3,     1,     3,     0,     0,     8,     0,     1,     2,     2,
       0,     0,    10,     3,     3,     5,     5,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     3,     3,     0,     1,
       3,     4,     4,     6,     6,     0,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     0,     1,     2,     1,     1,     1,     0,     1,
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
       4,     1,     3,     2,     3,     3,     1,     3,     2,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     2,     2,     1,     1,     1,     1,     1,     1,     1,
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
#line 424 "chapel.ypp" /* yacc.c:1661  */
    { yyblock = (yyval.pblockstmt); }
#line 4837 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 3:
#line 429 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
#line 4843 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 4:
#line 430 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); context->generatedStmt = (yyvsp[-1].pblockstmt); resetTempID(); }
#line 4849 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 6:
#line 437 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[-1].vpch), (yyvsp[0].pblockstmt) ); }
#line 4855 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 7:
#line 442 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[0].pch))); }
#line 4861 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 8:
#line 443 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].vpch)->add(astr((yyvsp[0].pch))); }
#line 4867 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 21:
#line 460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 4873 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 22:
#line 461 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[0].pblockstmt)); }
#line 4879 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 23:
#line 462 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 4885 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 24:
#line 463 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[-1].pch)); }
#line 4891 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 25:
#line 464 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt));  }
#line 4897 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 26:
#line 465 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[-1].pch)); }
#line 4903 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 27:
#line 466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[-1].pexpr)); }
#line 4909 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 28:
#line 467 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[-1].pch), (yyvsp[0].pblockstmt)); }
#line 4915 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 29:
#line 468 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[0].pblockstmt)); }
#line 4921 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 30:
#line 469 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4927 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 31:
#line 470 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 4933 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 32:
#line 471 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[0].pblockstmt)); }
#line 4939 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 33:
#line 472 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[0].pblockstmt)); }
#line 4945 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 34:
#line 473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[-1].pexpr)); }
#line 4951 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 35:
#line 474 "chapel.ypp" /* yacc.c:1661  */
    { printf("syntax error"); clean_exit(1); }
#line 4957 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 36:
#line 479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-2].pch), new BlockStmt(), yyfilename, (yyvsp[-4].b), (yylsp[-4]).comment))); }
#line 4963 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 37:
#line 481 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[-3].pch), (yyvsp[-1].pblockstmt), yyfilename, (yyvsp[-5].b), (yylsp[-5]).comment))); }
#line 4969 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 38:
#line 485 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4975 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 39:
#line 486 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4981 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 40:
#line 487 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 4987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 41:
#line 502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 4993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 42:
#line 503 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt);              }
#line 4999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 43:
#line 508 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5005 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 44:
#line 509 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->appendChapelStmt((yyvsp[0].pblockstmt)); }
#line 5011 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 45:
#line 514 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
#line 5021 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 46:
#line 519 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
#line 5031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 47:
#line 524 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[0].pexpr);
                                         (yyvsp[-2].ponlylist)->push_back(cur); }
#line 5040 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 48:
#line 528 "chapel.ypp" /* yacc.c:1661  */
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[-2].pexpr), (yyvsp[0].pexpr));
                                         (yyvsp[-4].ponlylist)->push_back(cur); }
#line 5049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 49:
#line 534 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5059 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 51:
#line 543 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
#line 5069 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 53:
#line 552 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-1].pcallexpr)); }
#line 5075 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 54:
#line 553 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), true); }
#line 5081 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 55:
#line 554 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[-3].pexpr), (yyvsp[-1].ponlylist), false); }
#line 5087 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 56:
#line 558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[-1].pcallexpr)); }
#line 5093 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 57:
#line 562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "=");   }
#line 5099 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 58:
#line 563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "+=");  }
#line 5105 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 59:
#line 564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "-=");  }
#line 5111 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 60:
#line 565 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "*=");  }
#line 5117 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 61:
#line 566 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "/=");  }
#line 5123 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 62:
#line 567 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "%=");  }
#line 5129 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 63:
#line 568 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "**="); }
#line 5135 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 64:
#line 569 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "&=");  }
#line 5141 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 65:
#line 570 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "|=");  }
#line 5147 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 66:
#line 571 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "^=");  }
#line 5153 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 67:
#line 572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), ">>="); }
#line 5159 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 68:
#line 573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<<="); }
#line 5165 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 69:
#line 574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), "<=>"); }
#line 5171 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 70:
#line 575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), PRIM_REDUCE_ASSIGN); }
#line 5177 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 71:
#line 576 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));    }
#line 5183 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 72:
#line 577 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[-3].pexpr), (yyvsp[-1].pexpr));     }
#line 5189 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 73:
#line 581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = NULL; }
#line 5195 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 75:
#line 586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5201 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 76:
#line 590 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5207 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 77:
#line 591 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = (yyvsp[0].pch); }
#line 5213 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 78:
#line 595 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5219 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 79:
#line 596 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5225 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 80:
#line 600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
#line 5231 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 81:
#line 601 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[-1].pexpr)); }
#line 5237 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 82:
#line 605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
#line 5243 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 84:
#line 607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5249 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 85:
#line 608 "chapel.ypp" /* yacc.c:1661  */
    { applyPrivateToBlock((yyvsp[0].pblockstmt)); (yyval.pblockstmt) = (yyvsp[0].pblockstmt); }
#line 5255 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 91:
#line 621 "chapel.ypp" /* yacc.c:1661  */
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
#line 5271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 92:
#line 635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[-1].pexpr), (yyvsp[-3].pblockstmt)); }
#line 5277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 93:
#line 636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 94:
#line 637 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5289 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 95:
#line 638 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), true); }
#line 5295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 96:
#line 639 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt)); }
#line 5301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 97:
#line 640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 98:
#line 641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5313 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 99:
#line 642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt), false, false); }
#line 5319 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 100:
#line 643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[-1].pcallexpr), (yyvsp[0].pblockstmt), false,  true); }
#line 5325 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 101:
#line 644 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[-3].pch), (yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5331 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 102:
#line 645 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5337 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 103:
#line 646 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5343 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 104:
#line 647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-3].pexpr), zipToTuple((yyvsp[-1].pcallexpr)), NULL, (yyvsp[0].pblockstmt), true); }
#line 5349 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 105:
#line 648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt), true); }
#line 5355 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 106:
#line 649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-1].pexpr), NULL, (yyvsp[0].pblockstmt)); }
#line 5361 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 107:
#line 650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pexpr), (yyvsp[-1].pForallIntents),   (yyvsp[0].pblockstmt)); }
#line 5367 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 108:
#line 652 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5377 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 109:
#line 658 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), (yyvsp[-3].pexpr), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5387 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 110:
#line 664 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5397 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 111:
#line 670 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-5].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-3].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[-5].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)), true);
    }
#line 5407 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 112:
#line 676 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5418 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 113:
#line 683 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-3].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-3].pcallexpr)), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[-3].pcallexpr)->get(1)->remove(), (yyvsp[-2].pForallIntents),   new BlockStmt((yyvsp[0].pblockstmt)));
    }
#line 5429 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 114:
#line 692 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[-1].pcallexpr)); }
#line 5435 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 115:
#line 696 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-2].pexpr), (yyvsp[0].pblockstmt)); }
#line 5441 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 116:
#line 697 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-1].pexpr), (yyvsp[0].pblockstmt)); }
#line 5447 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 117:
#line 698 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-4].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 118:
#line 699 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[-3].pexpr), (yyvsp[-2].pblockstmt), (yyvsp[0].pblockstmt)); }
#line 5459 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 119:
#line 706 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[0].pblockstmt)); }
#line 5465 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 120:
#line 707 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[0].pblockstmt)); }
#line 5471 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 121:
#line 712 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[-3].pexpr), (yyvsp[-1].pblockstmt))); }
#line 5477 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 122:
#line 716 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(); }
#line 5483 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 123:
#line 717 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pexpr)); }
#line 5489 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 124:
#line 722 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[-1].pcallexpr)), (yyvsp[0].pblockstmt)); }
#line 5495 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 125:
#line 724 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5501 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 126:
#line 726 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[0].pblockstmt)); }
#line 5507 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 127:
#line 733 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), NULL, (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[-5]).comment));
    }
#line 5516 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 128:
#line 738 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[-4].pch), (yyvsp[-6].pch), (yyvsp[-5].ptype), (yyvsp[-3].pcallexpr), (yyvsp[-1].pblockstmt), FLAG_EXTERN,
                                             (yylsp[-5]).comment));
    }
#line 5525 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 129:
#line 745 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5535 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 130:
#line 750 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5545 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 131:
#line 755 "chapel.ypp" /* yacc.c:1661  */
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
#line 5555 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 132:
#line 763 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 5561 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 133:
#line 764 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[0].pcallexpr); }
#line 5567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 134:
#line 769 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt(); }
#line 5573 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 135:
#line 771 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-1].pblockstmt)->insertAtTail((yyvsp[0].pblockstmt)); }
#line 5579 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 136:
#line 773 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[-1].vpch), (yyvsp[0].pblockstmt))); }
#line 5585 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 137:
#line 778 "chapel.ypp" /* yacc.c:1661  */
    {
      EnumType* pdt = (yyvsp[-1].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[-3].pch), pdt);
      (yyvsp[-1].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
#line 5596 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 138:
#line 788 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[0].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[0].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
#line 5609 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 139:
#line 797 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.penumtype) = (yyvsp[-1].penumtype);
    }
#line 5617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 140:
#line 801 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyvsp[-2].penumtype)->constants.insertAtTail((yyvsp[0].pdefexpr));
      (yyvsp[0].pdefexpr)->sym->type = (yyvsp[-2].penumtype);
    }
#line 5626 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 141:
#line 808 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[0].pch))); }
#line 5632 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 142:
#line 809 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr)); }
#line 5638 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 143:
#line 814 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 144:
#line 819 "chapel.ypp" /* yacc.c:1661  */
    {
      captureTokens = 0;
      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5656 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 145:
#line 824 "chapel.ypp" /* yacc.c:1661  */
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
#line 5676 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 146:
#line 844 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5687 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 147:
#line 850 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5699 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 148:
#line 857 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5712 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 149:
#line 865 "chapel.ypp" /* yacc.c:1661  */
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[0].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
#line 5725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 150:
#line 876 "chapel.ypp" /* yacc.c:1661  */
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
#line 5735 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 151:
#line 882 "chapel.ypp" /* yacc.c:1661  */
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[0].pfnsymbol)->userString = astr(captureString);
    }
#line 5746 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 152:
#line 889 "chapel.ypp" /* yacc.c:1661  */
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
#line 5769 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 153:
#line 911 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
    }
#line 5777 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 154:
#line 915 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-2].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5786 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 155:
#line 920 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
    }
#line 5794 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 156:
#line 924 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[0].pfnsymbol), (yyvsp[-1].pch), (yyvsp[-4].pt), (yyvsp[-3].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
#line 5803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 158:
#line 932 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = astr("~", (yyvsp[0].pch)); }
#line 5809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 159:
#line 933 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&"; }
#line 5815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 160:
#line 934 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|"; }
#line 5821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 161:
#line 935 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^"; }
#line 5827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 162:
#line 936 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "~"; }
#line 5833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 163:
#line 937 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "=="; }
#line 5839 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 164:
#line 938 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!="; }
#line 5845 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 165:
#line 939 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<="; }
#line 5851 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 166:
#line 940 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">="; }
#line 5857 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 167:
#line 941 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<"; }
#line 5863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 168:
#line 942 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">"; }
#line 5869 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 169:
#line 943 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+"; }
#line 5875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 170:
#line 944 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-"; }
#line 5881 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 171:
#line 945 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*"; }
#line 5887 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 172:
#line 946 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/"; }
#line 5893 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 173:
#line 947 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<"; }
#line 5899 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 174:
#line 948 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>"; }
#line 5905 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 175:
#line 949 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%"; }
#line 5911 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 176:
#line 950 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**"; }
#line 5917 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 177:
#line 951 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "!"; }
#line 5923 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 178:
#line 952 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_by"; }
#line 5929 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 179:
#line 953 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "#"; }
#line 5935 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 180:
#line 954 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "chpl_align"; }
#line 5941 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 181:
#line 955 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<=>"; }
#line 5947 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 182:
#line 956 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<~>"; }
#line 5953 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 183:
#line 960 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "="; }
#line 5959 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 184:
#line 961 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "+="; }
#line 5965 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 185:
#line 962 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "-="; }
#line 5971 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 186:
#line 963 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "*="; }
#line 5977 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 187:
#line 964 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "/="; }
#line 5983 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 188:
#line 965 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "%="; }
#line 5989 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 189:
#line 966 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "**="; }
#line 5995 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 190:
#line 967 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "&="; }
#line 6001 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 191:
#line 968 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "|="; }
#line 6007 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 192:
#line 969 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "^="; }
#line 6013 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 193:
#line 970 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = ">>="; }
#line 6019 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 194:
#line 971 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pch) = "<<="; }
#line 6025 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 195:
#line 975 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
#line 6031 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 196:
#line 976 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6037 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 197:
#line 980 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = (yyvsp[-1].pfnsymbol); }
#line 6043 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 198:
#line 984 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
#line 6049 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 199:
#line 985 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[0].pdefexpr)); }
#line 6055 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 200:
#line 986 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[-2].pfnsymbol), (yyvsp[0].pdefexpr)); }
#line 6061 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 201:
#line 991 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), (yyvsp[0].pexpr), NULL); }
#line 6067 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 202:
#line 993 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[-3].pt), (yyvsp[-2].pch), (yyvsp[-1].pexpr), NULL, (yyvsp[0].pexpr)); }
#line 6073 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 203:
#line 995 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[-5].pt), (yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6079 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 204:
#line 997 "chapel.ypp" /* yacc.c:1661  */
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
#line 6085 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 205:
#line 1001 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6091 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 206:
#line 1002 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = (yyvsp[0].pt); }
#line 6097 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 207:
#line 1006 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_IN; }
#line 6103 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 208:
#line 1007 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_INOUT; }
#line 6109 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 209:
#line 1008 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_OUT; }
#line 6115 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 210:
#line 1009 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST; }
#line 6121 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 211:
#line 1010 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_IN; }
#line 6127 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 212:
#line 1011 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_CONST_REF; }
#line 6133 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 213:
#line 1012 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6139 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 214:
#line 1013 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF; }
#line 6145 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 215:
#line 1014 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE; }
#line 6151 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 216:
#line 1018 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_BLANK; }
#line 6157 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 217:
#line 1019 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_PARAM; }
#line 6163 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 218:
#line 1020 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_REF;   }
#line 6169 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 219:
#line 1021 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pt) = INTENT_TYPE;  }
#line 6175 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 220:
#line 1025 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_PROC; }
#line 6181 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 221:
#line 1026 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.procIter) = ProcIter_ITER; }
#line 6187 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 222:
#line 1030 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6193 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 223:
#line 1031 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_VALUE; }
#line 6199 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 224:
#line 1032 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_CONST_REF; }
#line 6205 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 225:
#line 1033 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_REF; }
#line 6211 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 226:
#line 1034 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_PARAM; }
#line 6217 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 227:
#line 1035 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.retTag) = RET_TYPE; }
#line 6223 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 228:
#line 1039 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = false; }
#line 6229 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 229:
#line 1040 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.b) = true;  }
#line 6235 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 230:
#line 1043 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = NULL; }
#line 6241 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 233:
#line 1049 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[0].pblockstmt)); }
#line 6247 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 234:
#line 1054 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6253 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 235:
#line 1056 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6259 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 236:
#line 1060 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
#line 6265 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 238:
#line 1065 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6271 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 239:
#line 1066 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[0].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6277 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 240:
#line 1070 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6283 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 241:
#line 1071 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6289 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 242:
#line 1076 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = (yyvsp[-1].pblockstmt); }
#line 6295 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 243:
#line 1078 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[-1].pblockstmt)); }
#line 6301 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 244:
#line 1080 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[-1].pblockstmt)); }
#line 6307 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 245:
#line 1085 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-1].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[0].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
#line 6324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 246:
#line 1098 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[-2].pexpr));

      (yyvsp[0].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pblockstmt));
    }
#line 6342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 247:
#line 1114 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 248:
#line 1116 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 249:
#line 1118 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[0].pcallexpr)); }
#line 6360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 250:
#line 1123 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 251:
#line 1131 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 252:
#line 1139 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 253:
#line 1147 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-4].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-4]).comment);
      context->latestComment = NULL;
    }
#line 6409 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 254:
#line 1156 "chapel.ypp" /* yacc.c:1661  */
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[-3].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[-1].pblockstmt), flags, (yylsp[-3]).comment);
      context->latestComment = NULL;
    }
#line 6420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 255:
#line 1165 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 256:
#line 1166 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 257:
#line 1167 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
#line 6438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 259:
#line 1173 "chapel.ypp" /* yacc.c:1661  */
    {
      for_alist(expr, (yyvsp[0].pblockstmt)->body)
        (yyvsp[-2].pblockstmt)->insertAtTail(expr->remove());
    }
#line 6447 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 260:
#line 1181 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[-2].pch)), (yyvsp[0].pexpr), (yyvsp[-1].pexpr))); }
#line 6453 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 261:
#line 1183 "chapel.ypp" /* yacc.c:1661  */
    {
      VarSymbol* var = new VarSymbol((yyvsp[-3].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[0].pexpr), (yyvsp[-2].pexpr)));
    }
#line 6463 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 262:
#line 1189 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[-3].pblockstmt), (yyvsp[-1].pexpr), (yyvsp[0].pexpr)); }
#line 6469 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 263:
#line 1194 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
#line 6475 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 264:
#line 1196 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[0].pch))); }
#line 6481 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 265:
#line 1198 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pblockstmt); }
#line 6487 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 266:
#line 1203 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[0].pexpr)); }
#line 6493 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 267:
#line 1205 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[-1].pexpr)); }
#line 6499 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 268:
#line 1207 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pblockstmt) = ((yyvsp[0].pblockstmt)->insertAtHead((yyvsp[-2].pexpr)), (yyvsp[0].pblockstmt)); }
#line 6505 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 269:
#line 1213 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6511 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 270:
#line 1214 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNoInit); }
#line 6517 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 271:
#line 1215 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6523 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 272:
#line 1219 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6529 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 273:
#line 1221 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 6535 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 274:
#line 1225 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6541 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 275:
#line 1226 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6547 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 276:
#line 1227 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pcallexpr); }
#line 6553 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 277:
#line 1228 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad type specification"); }
#line 6559 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 278:
#line 1249 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr));
    }
#line 6567 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 279:
#line 1253 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pcallexpr));
    }
#line 6575 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 280:
#line 1257 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pexpr)->copy()));
    }
#line 6587 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 281:
#line 1264 "chapel.ypp" /* yacc.c:1661  */
    {printf("bad array type specification"); clean_exit(1); }
#line 6593 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 282:
#line 1268 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6599 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 283:
#line 1269 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6605 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 284:
#line 1270 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6611 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 285:
#line 1275 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6617 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 286:
#line 1277 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6623 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 287:
#line 1283 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[0].pexpr)); }
#line 6629 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 288:
#line 1285 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pcallexpr), (yyvsp[0].pexpr)); }
#line 6635 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 289:
#line 1287 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[-2].pexpr), (yyvsp[0].pexpr), (yyvsp[-4].pcallexpr)); }
#line 6641 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 290:
#line 1291 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = NULL; }
#line 6647 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 291:
#line 1292 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6653 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 292:
#line 1293 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6659 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 293:
#line 1294 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
#line 6665 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 294:
#line 1295 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
#line 6671 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 295:
#line 1296 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
#line 6677 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 296:
#line 1297 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6683 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 297:
#line 1303 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6689 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 298:
#line 1304 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pdefexpr)); }
#line 6695 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 299:
#line 1305 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6701 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 300:
#line 1306 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pdefexpr)); }
#line 6707 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 301:
#line 1310 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
#line 6713 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 302:
#line 1311 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pexpr); }
#line 6719 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 303:
#line 1312 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6725 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 304:
#line 1316 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6731 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 305:
#line 1317 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6737 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 306:
#line 1321 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
#line 6743 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 308:
#line 1326 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[0].pexpr)); }
#line 6749 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 309:
#line 1327 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-2].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 6755 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 310:
#line 1331 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pdefexpr)); }
#line 6761 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 311:
#line 1332 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6767 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 312:
#line 1333 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[-2].pch), (yyvsp[0].pexpr)); }
#line 6773 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 313:
#line 1334 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[0].pdefexpr); }
#line 6779 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 315:
#line 1339 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[0].pch)); }
#line 6785 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 321:
#line 1356 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[0].pexpr)); }
#line 6791 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 322:
#line 1358 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[-1].pcallexpr)); }
#line 6797 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 323:
#line 1360 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[-1].pcallexpr)); }
#line 6803 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 324:
#line 1362 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[-1].pcallexpr)); }
#line 6809 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 325:
#line 1364 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[-1].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[-1].pcallexpr)); }
#line 6815 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 326:
#line 1366 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[0].pexpr)); }
#line 6821 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 327:
#line 1368 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[0].pexpr)); }
#line 6827 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 328:
#line 1373 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6833 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 329:
#line 1375 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6839 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 330:
#line 1377 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6845 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 331:
#line 1379 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6851 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 332:
#line 1381 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6857 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 333:
#line 1383 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6863 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 334:
#line 1385 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6869 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 335:
#line 1387 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pexpr), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true); }
#line 6875 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 336:
#line 1389 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6881 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 337:
#line 1391 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6887 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 338:
#line 1393 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pexpr), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true); }
#line 6893 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 339:
#line 1395 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 6899 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 340:
#line 1397 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-2].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[-2].pcallexpr)->get(1)->remove(), (yyvsp[0].pexpr), NULL, true);
    }
#line 6910 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 341:
#line 1404 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), (yyvsp[-2].pexpr), (yyvsp[0].pexpr), NULL, true);
    }
#line 6920 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 342:
#line 1410 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-4].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-2].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-4].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-2].pcallexpr)), (yyvsp[0].pexpr), NULL, false, true);
    }
#line 6930 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 343:
#line 1416 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), (yyvsp[-5].pexpr), (yyvsp[0].pexpr), (yyvsp[-2].pexpr));
    }
#line 6940 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 344:
#line 1422 "chapel.ypp" /* yacc.c:1661  */
    {
      if ((yyvsp[-7].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[-5].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[-7].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[-5].pcallexpr)), (yyvsp[0].pexpr), (yyvsp[-2].pexpr), false, true);
    }
#line 6950 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 345:
#line 1431 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[-4].pexpr), (yyvsp[-2].pexpr), (yyvsp[0].pexpr)))); }
#line 6956 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 346:
#line 1435 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new SymExpr(gNil); }
#line 6962 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 354:
#line 1451 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 6968 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 355:
#line 1455 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = NULL; }
#line 6974 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 357:
#line 1460 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = (yyvsp[-1].pcallexpr); }
#line 6980 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 358:
#line 1466 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6987 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 359:
#line 1469 "chapel.ypp" /* yacc.c:1661  */
    { addTaskIntent((yyvsp[-2].pcallexpr), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 6993 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 360:
#line 1473 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = (yyvsp[-1].pForallIntents); }
#line 6999 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 361:
#line 1479 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7006 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 362:
#line 1482 "chapel.ypp" /* yacc.c:1661  */
    { addForallIntent((yyvsp[-2].pForallIntents), (yyvsp[0].pIntentExpr).iVar, (yyvsp[0].pIntentExpr).tfIntent, (yyvsp[0].pIntentExpr).riExp); }
#line 7012 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 363:
#line 1487 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[-1].pt); (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
#line 7020 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 364:
#line 1490 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7026 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 365:
#line 1491 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pIntentExpr).riExp = (yyvsp[-2].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[0].pexpr); }
#line 7032 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 367:
#line 1497 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7038 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 368:
#line 1502 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[0].pexpr)); }
#line 7044 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 369:
#line 1507 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildLetExpr((yyvsp[-2].pblockstmt), (yyvsp[0].pexpr)); }
#line 7050 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 378:
#line 1523 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[-1].pexpr)); }
#line 7056 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 379:
#line 1525 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7062 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 380:
#line 1527 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7068 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 381:
#line 1529 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[-1].pexpr)); }
#line 7074 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 382:
#line 1531 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[0].pexpr)); }
#line 7080 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 383:
#line 1533 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
#line 7086 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 390:
#line 1556 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7092 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 391:
#line 1557 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[-3].pexpr), (yyvsp[-1].pcallexpr)); }
#line 7098 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 392:
#line 1558 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[-1].pcallexpr)); }
#line 7104 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 393:
#line 1562 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), (yyvsp[0].pch)); }
#line 7110 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 394:
#line 1563 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[-2].pexpr)); }
#line 7116 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 395:
#line 1564 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildDotExpr((yyvsp[-2].pexpr), "_dom"); }
#line 7122 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 396:
#line 1572 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = (yyvsp[-1].pexpr); }
#line 7128 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 397:
#line 1573 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildOneTuple((yyvsp[-2].pexpr)); }
#line 7134 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 398:
#line 1574 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-1].pcallexpr)); }
#line 7140 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 399:
#line 1575 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildTuple((yyvsp[-2].pcallexpr)); }
#line 7146 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 400:
#line 1579 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildIntLiteral((yyvsp[0].pch));    }
#line 7152 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 401:
#line 1580 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildRealLiteral((yyvsp[0].pch));   }
#line 7158 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 402:
#line 1581 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildImagLiteral((yyvsp[0].pch));   }
#line 7164 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 403:
#line 1582 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildStringLiteral((yyvsp[0].pch)); }
#line 7170 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 404:
#line 1583 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[0].pch)); }
#line 7176 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 405:
#line 1584 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-1].pcallexpr)); }
#line 7182 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 406:
#line 1585 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[-2].pcallexpr)); }
#line 7188 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 407:
#line 1586 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-1].pcallexpr)); }
#line 7194 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 408:
#line 1587 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[-2].pcallexpr)); }
#line 7200 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 409:
#line 1589 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-1].pcallexpr));
    }
#line 7208 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 410:
#line 1593 "chapel.ypp" /* yacc.c:1661  */
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[-2].pcallexpr));
    }
#line 7216 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 411:
#line 1600 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7222 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 412:
#line 1601 "chapel.ypp" /* yacc.c:1661  */
    { (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[-2].pexpr)); (yyvsp[-4].pcallexpr)->insertAtTail((yyvsp[0].pexpr)); }
#line 7228 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 413:
#line 1605 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7234 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 414:
#line 1606 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7240 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 415:
#line 1607 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7246 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 416:
#line 1608 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7252 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 417:
#line 1609 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7258 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 418:
#line 1610 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7264 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 419:
#line 1611 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7270 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 420:
#line 1612 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7276 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 421:
#line 1613 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7282 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 422:
#line 1614 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7288 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 423:
#line 1615 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7294 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 424:
#line 1616 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7300 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 425:
#line 1617 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7306 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 426:
#line 1618 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7312 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 427:
#line 1619 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7318 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 428:
#line 1620 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7324 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 429:
#line 1621 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7330 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 430:
#line 1622 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7336 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 431:
#line 1623 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7342 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 432:
#line 1624 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7348 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 433:
#line 1625 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7354 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 434:
#line 1626 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7360 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 435:
#line 1627 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[0].pexpr), (yyvsp[-2].pexpr)); }
#line 7366 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 436:
#line 1631 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[0].pexpr)); }
#line 7372 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 437:
#line 1632 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[0].pexpr)); }
#line 7378 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 438:
#line 1633 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '-'); }
#line 7384 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 439:
#line 1634 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[0].pexpr), '+'); }
#line 7390 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 440:
#line 1635 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[0].pexpr)); }
#line 7396 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 441:
#line 1636 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[0].pexpr)); }
#line 7402 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 442:
#line 1640 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7408 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 443:
#line 1641 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7414 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 444:
#line 1642 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7420 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 445:
#line 1643 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildReduceExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7426 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 446:
#line 1647 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7432 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 447:
#line 1648 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7438 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 448:
#line 1649 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), (yyvsp[0].pexpr)); }
#line 7444 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 449:
#line 1650 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = buildScanExpr((yyvsp[-2].pexpr), zipToTuple((yyvsp[0].pcallexpr)), true); }
#line 7450 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 450:
#line 1655 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
#line 7456 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 451:
#line 1656 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
#line 7462 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 452:
#line 1657 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
#line 7468 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 453:
#line 1658 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
#line 7474 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 454:
#line 1659 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
#line 7480 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 455:
#line 1660 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
#line 7486 "bison-chapel.cpp" /* yacc.c:1661  */
    break;

  case 456:
#line 1661 "chapel.ypp" /* yacc.c:1661  */
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
#line 7492 "bison-chapel.cpp" /* yacc.c:1661  */
    break;


#line 7496 "bison-chapel.cpp" /* yacc.c:1661  */
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
