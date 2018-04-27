/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* Copy the first part of user declarations.  */

/* Line 371 of yacc.c  */
#line 68 "bison-chapel.cpp"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
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
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 387 of yacc.c  */
#line 32 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;

/* Line 387 of yacc.c  */
#line 45 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif

/* Line 387 of yacc.c  */
#line 65 "chapel.ypp"

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

/* Line 387 of yacc.c  */
#line 130 "chapel.ypp"

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

/* Line 387 of yacc.c  */
#line 152 "chapel.ypp"

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


/* Line 387 of yacc.c  */
#line 234 "bison-chapel.cpp"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
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
     TOWNED = 310,
     TPARAM = 311,
     TPLUSPLUS = 312,
     TPRAGMA = 313,
     TPRIMITIVE = 314,
     TPRIVATE = 315,
     TPROC = 316,
     TPROTOTYPE = 317,
     TPUBLIC = 318,
     TRECORD = 319,
     TREDUCE = 320,
     TREF = 321,
     TREQUIRE = 322,
     TRETURN = 323,
     TSCAN = 324,
     TSELECT = 325,
     TSERIAL = 326,
     TSHARED = 327,
     TSINGLE = 328,
     TSPARSE = 329,
     TSUBDOMAIN = 330,
     TSYNC = 331,
     TTHEN = 332,
     TTHROW = 333,
     TTHROWS = 334,
     TTRY = 335,
     TTRYBANG = 336,
     TTYPE = 337,
     TUNDERSCORE = 338,
     TUNION = 339,
     TUNMANAGED = 340,
     TUSE = 341,
     TVAR = 342,
     TWHEN = 343,
     TWHERE = 344,
     TWHILE = 345,
     TWITH = 346,
     TYIELD = 347,
     TZIP = 348,
     TALIAS = 349,
     TAND = 350,
     TASSIGN = 351,
     TASSIGNBAND = 352,
     TASSIGNBOR = 353,
     TASSIGNBXOR = 354,
     TASSIGNDIVIDE = 355,
     TASSIGNEXP = 356,
     TASSIGNLAND = 357,
     TASSIGNLOR = 358,
     TASSIGNMINUS = 359,
     TASSIGNMOD = 360,
     TASSIGNMULTIPLY = 361,
     TASSIGNPLUS = 362,
     TASSIGNSL = 363,
     TASSIGNSR = 364,
     TBAND = 365,
     TBNOT = 366,
     TBOR = 367,
     TBXOR = 368,
     TCOLON = 369,
     TCOMMA = 370,
     TDIVIDE = 371,
     TDOT = 372,
     TDOTDOT = 373,
     TDOTDOTDOT = 374,
     TEQUAL = 375,
     TEXP = 376,
     TGREATER = 377,
     TGREATEREQUAL = 378,
     THASH = 379,
     TLESS = 380,
     TLESSEQUAL = 381,
     TMINUS = 382,
     TMOD = 383,
     TNOT = 384,
     TNOTEQUAL = 385,
     TOR = 386,
     TPLUS = 387,
     TQUESTION = 388,
     TSEMI = 389,
     TSHIFTLEFT = 390,
     TSHIFTRIGHT = 391,
     TSTAR = 392,
     TSWAP = 393,
     TASSIGNREDUCE = 394,
     TIO = 395,
     TLCBR = 396,
     TRCBR = 397,
     TLP = 398,
     TRP = 399,
     TLSBR = 400,
     TRSBR = 401,
     TNOELSE = 402,
     TUMINUS = 403,
     TUPLUS = 404
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;

#if defined __STDC__ || defined __cplusplus
int yypush_parse (yypstate *ps, int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc, ParserContext* context);
#else
int yypush_parse ();
#endif

#if defined __STDC__ || defined __cplusplus
yypstate * yypstate_new (void);
#else
yypstate * yypstate_new ();
#endif
#if defined __STDC__ || defined __cplusplus
void yypstate_delete (yypstate *ps);
#else
void yypstate_delete ();
#endif
/* "%code provides" blocks.  */
/* Line 387 of yacc.c  */
#line 183 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);


/* Line 387 of yacc.c  */
#line 448 "bison-chapel.cpp"

#endif /* !YY_YY_INCLUDE_BISON_CHAPEL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 455 "bison-chapel.cpp"
/* Unqualified %code blocks.  */
/* Line 391 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;

/* Line 391 of yacc.c  */
#line 191 "chapel.ypp"

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


/* Line 391 of yacc.c  */
#line 533 "bison-chapel.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

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
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   13877

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  150
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  124
/* YYNRULES -- Number of rules.  */
#define YYNRULES  507
/* YYNRULES -- Number of states.  */
#define YYNSTATES  953

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   404

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
     145,   146,   147,   148,   149
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    49,    52,    55,    59,    63,    67,    71,
      75,    79,    83,    86,    90,    94,    97,   100,   104,   107,
     114,   122,   130,   131,   133,   135,   136,   138,   141,   145,
     149,   151,   154,   156,   160,   164,   170,   171,   173,   175,
     177,   181,   187,   193,   197,   202,   207,   212,   217,   222,
     227,   232,   237,   242,   247,   252,   257,   262,   267,   272,
     277,   278,   280,   282,   284,   286,   289,   291,   294,   298,
     300,   302,   305,   308,   310,   312,   314,   316,   318,   320,
     324,   330,   336,   339,   342,   348,   352,   359,   366,   371,
     377,   383,   387,   391,   398,   404,   411,   417,   424,   428,
     433,   440,   448,   455,   463,   468,   474,   479,   484,   488,
     495,   501,   508,   514,   523,   531,   534,   538,   542,   545,
     548,   552,   556,   557,   560,   563,   567,   573,   575,   579,
     583,   589,   595,   596,   599,   603,   606,   610,   617,   626,
     633,   642,   644,   646,   648,   649,   652,   653,   656,   660,
     666,   672,   674,   676,   679,   683,   685,   689,   690,   691,
     700,   701,   703,   706,   709,   710,   711,   722,   726,   730,
     736,   742,   744,   748,   750,   753,   755,   757,   759,   761,
     763,   765,   767,   769,   771,   773,   775,   777,   779,   781,
     783,   785,   787,   789,   791,   793,   795,   797,   799,   801,
     803,   805,   807,   809,   811,   813,   815,   817,   819,   821,
     823,   825,   826,   830,   834,   835,   837,   841,   846,   851,
     858,   865,   866,   868,   870,   872,   874,   876,   879,   882,
     884,   886,   888,   889,   891,   893,   896,   898,   900,   902,
     904,   905,   907,   910,   912,   914,   916,   917,   919,   921,
     923,   925,   927,   930,   932,   933,   935,   938,   941,   942,
     945,   949,   954,   959,   962,   967,   968,   971,   974,   979,
     984,   989,   995,  1000,  1001,  1003,  1005,  1007,  1011,  1015,
    1021,  1023,  1025,  1029,  1031,  1034,  1038,  1039,  1042,  1045,
    1046,  1049,  1052,  1055,  1058,  1061,  1064,  1067,  1070,  1072,
    1077,  1082,  1089,  1093,  1094,  1096,  1098,  1102,  1107,  1111,
    1116,  1123,  1124,  1127,  1130,  1133,  1136,  1139,  1142,  1145,
    1148,  1151,  1153,  1155,  1159,  1163,  1165,  1169,  1171,  1173,
    1175,  1179,  1183,  1184,  1186,  1188,  1192,  1196,  1200,  1202,
    1204,  1206,  1208,  1210,  1212,  1214,  1216,  1219,  1224,  1229,
    1234,  1240,  1243,  1246,  1249,  1252,  1255,  1262,  1269,  1274,
    1284,  1294,  1302,  1309,  1316,  1321,  1331,  1341,  1349,  1354,
    1361,  1368,  1378,  1388,  1395,  1397,  1399,  1401,  1403,  1405,
    1407,  1409,  1411,  1415,  1416,  1418,  1423,  1425,  1429,  1434,
    1436,  1440,  1443,  1447,  1451,  1453,  1457,  1460,  1465,  1467,
    1469,  1471,  1473,  1475,  1477,  1479,  1481,  1486,  1490,  1494,
    1497,  1500,  1502,  1505,  1508,  1510,  1512,  1514,  1516,  1518,
    1520,  1522,  1527,  1532,  1537,  1541,  1545,  1549,  1553,  1558,
    1562,  1567,  1569,  1571,  1573,  1575,  1577,  1581,  1586,  1590,
    1595,  1599,  1604,  1608,  1614,  1618,  1622,  1626,  1630,  1634,
    1638,  1642,  1646,  1650,  1654,  1658,  1662,  1666,  1670,  1674,
    1678,  1682,  1686,  1690,  1694,  1698,  1702,  1706,  1709,  1712,
    1715,  1718,  1721,  1724,  1728,  1732,  1736,  1740,  1744,  1748,
    1752,  1756,  1758,  1760,  1762,  1764,  1766,  1768
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     151,     0,    -1,   152,    -1,    -1,   152,   153,    -1,   155,
      -1,   154,   155,    -1,    58,     7,    -1,   154,    58,     7,
      -1,   156,    -1,   159,    -1,   164,    -1,   165,    -1,   172,
      -1,   166,    -1,   175,    -1,   178,    -1,   176,    -1,   185,
      -1,   179,    -1,   180,    -1,   184,    -1,   171,    -1,   250,
     134,    -1,    12,   155,    -1,    13,   251,   155,    -1,    14,
     167,   134,    -1,    18,   251,   159,    -1,    22,   167,   134,
      -1,    24,   239,   134,    -1,    42,   168,   155,    -1,    45,
     260,   170,    -1,    45,   170,    -1,    51,   260,   170,    -1,
      71,   260,   170,    -1,    71,   170,    -1,    76,   155,    -1,
      92,   260,   134,    -1,     1,   134,    -1,   157,   158,    47,
     168,   141,   142,    -1,   157,   158,    47,   168,   141,   160,
     142,    -1,   157,   158,    47,   168,   141,     1,   142,    -1,
      -1,    63,    -1,    60,    -1,    -1,    62,    -1,   141,   142,
      -1,   141,   160,   142,    -1,   141,     1,   142,    -1,   153,
      -1,   160,   153,    -1,   260,    -1,   260,    11,   260,    -1,
     161,   115,   260,    -1,   161,   115,   260,    11,   260,    -1,
      -1,   161,    -1,   137,    -1,   161,    -1,    86,   238,   134,
      -1,    86,   260,    30,   163,   134,    -1,    86,   260,    52,
     162,   134,    -1,    67,   238,   134,    -1,   262,    96,   261,
     134,    -1,   262,   107,   261,   134,    -1,   262,   104,   261,
     134,    -1,   262,   106,   261,   134,    -1,   262,   100,   261,
     134,    -1,   262,   105,   261,   134,    -1,   262,   101,   261,
     134,    -1,   262,    97,   261,   134,    -1,   262,    98,   261,
     134,    -1,   262,    99,   261,   134,    -1,   262,   109,   261,
     134,    -1,   262,   108,   261,   134,    -1,   262,   138,   261,
     134,    -1,   262,   139,   261,   134,    -1,   262,   102,   261,
     134,    -1,   262,   103,   261,   134,    -1,    -1,   168,    -1,
       3,    -1,   167,    -1,     7,    -1,    26,   155,    -1,   159,
      -1,    68,   134,    -1,    68,   261,   134,    -1,   134,    -1,
     173,    -1,    63,   173,    -1,    60,   173,    -1,   192,    -1,
     223,    -1,   188,    -1,   174,    -1,   200,    -1,   226,    -1,
      35,   260,   134,    -1,    35,   260,    30,   163,   134,    -1,
      35,   260,    52,   162,   134,    -1,    35,   226,    -1,    32,
       9,    -1,    26,   155,    90,   260,   134,    -1,    90,   260,
     170,    -1,    19,   260,    37,   260,   251,   170,    -1,    19,
     260,    37,   177,   251,   170,    -1,    19,   260,   251,   170,
      -1,    33,   260,    37,   260,   170,    -1,    33,   260,    37,
     177,   170,    -1,    33,   260,   170,    -1,    33,   177,   170,
      -1,    33,    56,   168,    37,   260,   170,    -1,    34,   260,
      37,   260,   170,    -1,    34,   260,    37,   260,   254,   170,
      -1,    34,   260,    37,   177,   170,    -1,    34,   260,    37,
     177,   254,   170,    -1,    34,   260,   170,    -1,    34,   260,
     254,   170,    -1,   145,   238,    37,   260,   146,   155,    -1,
     145,   238,    37,   260,   254,   146,   155,    -1,   145,   238,
      37,   177,   146,   155,    -1,   145,   238,    37,   177,   254,
     146,   155,    -1,   145,   238,   146,   155,    -1,   145,   238,
     254,   146,   155,    -1,    93,   143,   238,   144,    -1,    36,
     260,    77,   155,    -1,    36,   260,   159,    -1,    36,   260,
      77,   155,    28,   155,    -1,    36,   260,   159,    28,   155,
      -1,    36,   260,   206,   260,    77,   155,    -1,    36,   260,
     206,   260,   159,    -1,    36,   260,   206,   260,    77,   155,
      28,   155,    -1,    36,   260,   206,   260,   159,    28,   155,
      -1,    23,   155,    -1,    80,   260,   134,    -1,    81,   260,
     134,    -1,    80,   166,    -1,    81,   166,    -1,    80,   159,
     181,    -1,    81,   159,   181,    -1,    -1,   181,   182,    -1,
      16,   159,    -1,    16,   183,   159,    -1,    16,   143,   183,
     144,   159,    -1,   168,    -1,   168,   114,   260,    -1,    78,
     260,   134,    -1,    70,   260,   141,   186,   142,    -1,    70,
     260,   141,     1,   142,    -1,    -1,   186,   187,    -1,    88,
     238,   170,    -1,    53,   155,    -1,    53,    26,   155,    -1,
     189,   168,   190,   141,   191,   142,    -1,    32,   169,   189,
     168,   190,   141,   191,   142,    -1,   189,   168,   190,   141,
       1,   142,    -1,    32,   169,   189,   168,   190,   141,     1,
     142,    -1,    17,    -1,    64,    -1,    84,    -1,    -1,   114,
     238,    -1,    -1,   191,   172,    -1,   191,   154,   172,    -1,
     193,   168,   141,   194,   142,    -1,   193,   168,   141,     1,
     142,    -1,    29,    -1,   195,    -1,   194,   115,    -1,   194,
     115,   195,    -1,   168,    -1,   168,    96,   260,    -1,    -1,
      -1,    43,   197,   208,   198,   215,   233,   222,   218,    -1,
      -1,    39,    -1,    31,   169,    -1,    32,   169,    -1,    -1,
      -1,   199,   214,   201,   203,   202,   215,   233,   216,   222,
     217,    -1,   213,   205,   207,    -1,   213,   206,   207,    -1,
     213,   204,   117,   205,   207,    -1,   213,   204,   117,   206,
     207,    -1,   245,    -1,   143,   260,   144,    -1,   168,    -1,
     111,   168,    -1,   110,    -1,   112,    -1,   113,    -1,   111,
      -1,   120,    -1,   130,    -1,   126,    -1,   123,    -1,   125,
      -1,   122,    -1,   132,    -1,   127,    -1,   137,    -1,   116,
      -1,   135,    -1,   136,    -1,   128,    -1,   121,    -1,   129,
      -1,    15,    -1,   124,    -1,    10,    -1,   138,    -1,   140,
      -1,    96,    -1,   107,    -1,   104,    -1,   106,    -1,   100,
      -1,   105,    -1,   101,    -1,    97,    -1,    98,    -1,    99,
      -1,   109,    -1,   108,    -1,    -1,   143,   209,   144,    -1,
     143,   209,   144,    -1,    -1,   210,    -1,   209,   115,   210,
      -1,   211,   168,   237,   232,    -1,   211,   168,   237,   221,
      -1,   211,   143,   231,   144,   237,   232,    -1,   211,   143,
     231,   144,   237,   221,    -1,    -1,   212,    -1,    37,    -1,
      40,    -1,    54,    -1,    21,    -1,    21,    37,    -1,    21,
      66,    -1,    56,    -1,    66,    -1,    82,    -1,    -1,    56,
      -1,    66,    -1,    21,    66,    -1,    21,    -1,    82,    -1,
      61,    -1,    41,    -1,    -1,    21,    -1,    21,    66,    -1,
      66,    -1,    56,    -1,    82,    -1,    -1,    79,    -1,   134,
      -1,   218,    -1,   159,    -1,   171,    -1,   133,   168,    -1,
     133,    -1,    -1,   219,    -1,   119,   260,    -1,   119,   220,
      -1,    -1,    89,   260,    -1,    82,   224,   134,    -1,    20,
      82,   224,   134,    -1,    32,    82,   224,   134,    -1,   168,
     225,    -1,   168,   225,   115,   224,    -1,    -1,    96,   246,
      -1,    96,   234,    -1,   227,    56,   228,   134,    -1,   227,
      21,   228,   134,    -1,   227,    66,   228,   134,    -1,   227,
      21,    66,   228,   134,    -1,   227,    87,   228,   134,    -1,
      -1,    20,    -1,    32,    -1,   229,    -1,   228,   115,   229,
      -1,   168,   233,   232,    -1,   143,   231,   144,   233,   232,
      -1,    83,    -1,   168,    -1,   143,   231,   144,    -1,   230,
      -1,   230,   115,    -1,   230,   115,   231,    -1,    -1,    96,
      50,    -1,    96,   261,    -1,    -1,   114,   246,    -1,   114,
     234,    -1,   114,    27,    -1,   114,    73,    -1,   114,    76,
      -1,   114,    55,    -1,   114,    85,    -1,   114,    72,    -1,
       1,    -1,   145,   238,   146,   246,    -1,   145,   238,   146,
     234,    -1,   145,   238,    37,   260,   146,   246,    -1,   145,
       1,   146,    -1,    -1,   246,    -1,   219,    -1,   145,   146,
     235,    -1,   145,   238,   146,   235,    -1,   145,   146,   236,
      -1,   145,   238,   146,   236,    -1,   145,   238,    37,   260,
     146,   235,    -1,    -1,   114,   246,    -1,   114,   219,    -1,
     114,    27,    -1,   114,    73,    -1,   114,    76,    -1,   114,
      55,    -1,   114,    85,    -1,   114,    72,    -1,   114,   236,
      -1,   260,    -1,   219,    -1,   238,   115,   260,    -1,   238,
     115,   219,    -1,   260,    -1,   239,   115,   260,    -1,    83,
      -1,   261,    -1,   219,    -1,   240,   115,   240,    -1,   241,
     115,   240,    -1,    -1,   243,    -1,   244,    -1,   243,   115,
     244,    -1,   168,    96,   219,    -1,   168,    96,   261,    -1,
     219,    -1,   261,    -1,   168,    -1,   249,    -1,   262,    -1,
     248,    -1,   270,    -1,   269,    -1,    73,   260,    -1,    38,
     143,   242,   144,    -1,    27,   143,   242,   144,    -1,    75,
     143,   242,   144,    -1,    74,    75,   143,   242,   144,    -1,
      12,   260,    -1,    76,   260,    -1,    55,   260,    -1,    85,
     260,    -1,    72,   260,    -1,    33,   260,    37,   260,    26,
     260,    -1,    33,   260,    37,   177,    26,   260,    -1,    33,
     260,    26,   260,    -1,    33,   260,    37,   260,    26,    36,
     260,    77,   260,    -1,    33,   260,    37,   177,    26,    36,
     260,    77,   260,    -1,    33,   260,    26,    36,   260,    77,
     260,    -1,    34,   260,    37,   260,    26,   260,    -1,    34,
     260,    37,   177,    26,   260,    -1,    34,   260,    26,   260,
      -1,    34,   260,    37,   260,    26,    36,   260,    77,   260,
      -1,    34,   260,    37,   177,    26,    36,   260,    77,   260,
      -1,    34,   260,    26,    36,   260,    77,   260,    -1,   145,
     238,   146,   260,    -1,   145,   238,    37,   260,   146,   260,
      -1,   145,   238,    37,   177,   146,   260,    -1,   145,   238,
      37,   260,   146,    36,   260,    77,   260,    -1,   145,   238,
      37,   177,   146,    36,   260,    77,   260,    -1,    36,   260,
      77,   260,    28,   260,    -1,    49,    -1,   249,    -1,   245,
      -1,   265,    -1,   264,    -1,   196,    -1,   258,    -1,   259,
      -1,   257,   140,   260,    -1,    -1,   252,    -1,    91,   143,
     253,   144,    -1,   256,    -1,   253,   115,   256,    -1,    91,
     143,   255,   144,    -1,   256,    -1,   255,   115,   256,    -1,
     212,   245,    -1,   260,    65,   245,    -1,   273,    65,   245,
      -1,   262,    -1,   257,   140,   260,    -1,    48,   260,    -1,
      44,   228,    37,   260,    -1,   267,    -1,   246,    -1,   247,
      -1,   271,    -1,   272,    -1,   196,    -1,   258,    -1,   259,
      -1,   143,   119,   260,   144,    -1,   260,   114,   260,    -1,
     260,   118,   260,    -1,   260,   118,    -1,   118,   260,    -1,
     118,    -1,    80,   260,    -1,    81,   260,    -1,   260,    -1,
     245,    -1,   264,    -1,   265,    -1,   266,    -1,   262,    -1,
     196,    -1,   263,   143,   242,   144,    -1,   263,   145,   242,
     146,    -1,    59,   143,   242,   144,    -1,   260,   117,   168,
      -1,   260,   117,    82,    -1,   260,   117,    27,    -1,   143,
     240,   144,    -1,   143,   240,   115,   144,    -1,   143,   241,
     144,    -1,   143,   241,   115,   144,    -1,     4,    -1,     5,
      -1,     6,    -1,     7,    -1,     8,    -1,   141,   238,   142,
      -1,   141,   238,   115,   142,    -1,   145,   238,   146,    -1,
     145,   238,   115,   146,    -1,   145,   268,   146,    -1,   145,
     268,   115,   146,    -1,   260,    94,   260,    -1,   268,   115,
     260,    94,   260,    -1,   260,   132,   260,    -1,   260,   127,
     260,    -1,   260,   137,   260,    -1,   260,   116,   260,    -1,
     260,   135,   260,    -1,   260,   136,   260,    -1,   260,   128,
     260,    -1,   260,   120,   260,    -1,   260,   130,   260,    -1,
     260,   126,   260,    -1,   260,   123,   260,    -1,   260,   125,
     260,    -1,   260,   122,   260,    -1,   260,   110,   260,    -1,
     260,   112,   260,    -1,   260,   113,   260,    -1,   260,    95,
     260,    -1,   260,   131,   260,    -1,   260,   121,   260,    -1,
     260,    15,   260,    -1,   260,    10,   260,    -1,   260,   124,
     260,    -1,   260,    25,   260,    -1,   132,   260,    -1,   127,
     260,    -1,    46,   260,    -1,    57,   260,    -1,   129,   260,
      -1,   111,   260,    -1,   260,    65,   260,    -1,   260,    65,
     177,    -1,   273,    65,   260,    -1,   273,    65,   177,    -1,
     260,    69,   260,    -1,   260,    69,   177,    -1,   273,    69,
     260,    -1,   273,    69,   177,    -1,   132,    -1,   137,    -1,
      95,    -1,   131,    -1,   110,    -1,   112,    -1,   113,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   443,   443,   448,   449,   455,   456,   461,   462,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   500,
     503,   505,   511,   512,   513,   517,   518,   531,   532,   533,
     538,   539,   544,   549,   554,   558,   565,   570,   574,   579,
     583,   584,   585,   589,   593,   595,   597,   599,   601,   603,
     605,   607,   609,   611,   613,   615,   617,   619,   621,   623,
     628,   629,   633,   637,   638,   642,   643,   647,   648,   652,
     653,   654,   655,   656,   657,   658,   659,   663,   664,   668,
     669,   670,   671,   675,   690,   691,   692,   693,   694,   695,
     696,   697,   698,   699,   700,   701,   702,   703,   704,   705,
     706,   712,   718,   724,   730,   737,   747,   751,   752,   753,
     754,   755,   757,   759,   761,   766,   769,   770,   771,   772,
     773,   774,   778,   779,   783,   784,   785,   789,   790,   794,
     797,   799,   804,   805,   809,   811,   813,   820,   830,   840,
     850,   863,   868,   873,   881,   882,   887,   888,   890,   895,
     911,   918,   927,   935,   939,   946,   947,   952,   957,   951,
     982,   988,   995,  1003,  1015,  1021,  1014,  1049,  1053,  1058,
    1062,  1070,  1071,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1104,
    1105,  1106,  1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,
    1115,  1119,  1120,  1124,  1128,  1129,  1130,  1134,  1136,  1138,
    1140,  1145,  1146,  1150,  1151,  1152,  1153,  1154,  1155,  1156,
    1157,  1158,  1162,  1163,  1164,  1165,  1166,  1167,  1171,  1172,
    1176,  1177,  1178,  1179,  1180,  1181,  1185,  1186,  1189,  1190,
    1194,  1195,  1199,  1201,  1206,  1207,  1211,  1212,  1216,  1217,
    1221,  1223,  1225,  1230,  1243,  1260,  1261,  1263,  1268,  1276,
    1284,  1292,  1301,  1311,  1312,  1313,  1317,  1318,  1326,  1328,
    1333,  1335,  1337,  1342,  1344,  1346,  1353,  1354,  1355,  1359,
    1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1388,
    1392,  1396,  1404,  1411,  1412,  1413,  1417,  1419,  1425,  1427,
    1429,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,
    1443,  1449,  1450,  1451,  1452,  1456,  1457,  1461,  1462,  1463,
    1467,  1468,  1472,  1473,  1477,  1478,  1482,  1483,  1484,  1485,
    1489,  1500,  1501,  1502,  1503,  1504,  1505,  1507,  1509,  1511,
    1513,  1515,  1517,  1519,  1521,  1523,  1528,  1530,  1532,  1534,
    1536,  1538,  1540,  1542,  1544,  1546,  1548,  1550,  1552,  1559,
    1565,  1571,  1577,  1586,  1596,  1604,  1605,  1606,  1607,  1608,
    1609,  1610,  1611,  1616,  1617,  1621,  1625,  1628,  1633,  1637,
    1640,  1645,  1648,  1651,  1657,  1658,  1663,  1668,  1676,  1677,
    1678,  1679,  1680,  1681,  1682,  1683,  1684,  1686,  1688,  1690,
    1692,  1694,  1699,  1700,  1701,  1704,  1705,  1706,  1707,  1718,
    1719,  1723,  1724,  1725,  1729,  1730,  1731,  1739,  1740,  1741,
    1742,  1746,  1747,  1748,  1749,  1750,  1751,  1752,  1753,  1754,
    1755,  1759,  1767,  1768,  1772,  1773,  1774,  1775,  1776,  1777,
    1778,  1779,  1780,  1781,  1782,  1783,  1784,  1785,  1786,  1787,
    1788,  1789,  1790,  1791,  1792,  1793,  1794,  1798,  1799,  1800,
    1801,  1802,  1803,  1807,  1808,  1809,  1810,  1814,  1815,  1816,
    1817,  1822,  1823,  1824,  1825,  1826,  1827,  1828
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
  "TONLY", "TOTHERWISE", "TOUT", "TOWNED", "TPARAM", "TPLUSPLUS",
  "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE", "TPUBLIC",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSHARED", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE",
  "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND",
  "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP",
  "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP",
  "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
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
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
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
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   150,   151,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     156,   156,   157,   157,   157,   158,   158,   159,   159,   159,
     160,   160,   161,   161,   161,   161,   162,   162,   163,   163,
     164,   164,   164,   165,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     167,   167,   168,   169,   169,   170,   170,   171,   171,   172,
     172,   172,   172,   172,   172,   172,   172,   173,   173,   174,
     174,   174,   174,   175,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   177,   178,   178,   178,
     178,   178,   178,   178,   178,   179,   180,   180,   180,   180,
     180,   180,   181,   181,   182,   182,   182,   183,   183,   184,
     185,   185,   186,   186,   187,   187,   187,   188,   188,   188,
     188,   189,   189,   189,   190,   190,   191,   191,   191,   192,
     192,   193,   194,   194,   194,   195,   195,   197,   198,   196,
     199,   199,   199,   199,   201,   202,   200,   203,   203,   203,
     203,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   207,   207,   208,   209,   209,   209,   210,   210,   210,
     210,   211,   211,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   213,   213,   213,   213,   213,   213,   214,   214,
     215,   215,   215,   215,   215,   215,   216,   216,   217,   217,
     218,   218,   219,   219,   220,   220,   221,   221,   222,   222,
     223,   223,   223,   224,   224,   225,   225,   225,   226,   226,
     226,   226,   226,   227,   227,   227,   228,   228,   229,   229,
     230,   230,   230,   231,   231,   231,   232,   232,   232,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   234,
     234,   234,   234,   235,   235,   235,   236,   236,   236,   236,
     236,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   238,   238,   238,   238,   239,   239,   240,   240,   240,
     241,   241,   242,   242,   243,   243,   244,   244,   244,   244,
     245,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   248,   249,   250,   250,   250,   250,   250,
     250,   250,   250,   251,   251,   252,   253,   253,   254,   255,
     255,   256,   256,   256,   257,   257,   258,   259,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   261,   261,   262,   262,   262,   262,   263,
     263,   264,   264,   264,   265,   265,   265,   266,   266,   266,
     266,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   268,   268,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   269,   269,   269,
     269,   269,   269,   269,   269,   269,   269,   270,   270,   270,
     270,   270,   270,   271,   271,   271,   271,   272,   272,   272,
     272,   273,   273,   273,   273,   273,   273,   273
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
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
       2,     2,     2,     2,     2,     2,     2,     2,     1,     4,
       4,     6,     3,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     2,     2,     2,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     4,     1,     3,     4,     1,
       3,     2,     3,     3,     1,     3,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     2,
       2,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,     0,    82,   451,   452,   453,   454,
     455,     0,   403,    80,   161,   403,     0,   294,    80,     0,
       0,     0,     0,   171,    80,    80,     0,     0,   293,     0,
       0,   181,     0,   177,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,   293,   293,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     163,     0,     0,     0,     0,   503,   505,     0,   506,   507,
     431,     0,     0,   504,   501,    89,   502,     0,     0,     0,
       4,     0,     5,     9,    45,    10,    11,    12,    14,   360,
      22,    13,    90,    96,    15,    17,    16,    19,    20,    21,
      18,    95,     0,    93,     0,   423,     0,    97,    94,    98,
       0,   435,   419,   420,   363,   361,     0,     0,   424,   425,
       0,   362,     0,   436,   437,   438,   418,   365,   364,   421,
     422,     0,    38,    24,   371,     0,     0,   404,     0,    81,
       0,     0,     0,     0,     0,     0,     0,     0,   423,   435,
     361,   424,   425,   403,   362,   436,   437,     0,     0,   135,
       0,   345,     0,   352,    84,    83,   182,   103,     0,   183,
       0,     0,     0,     0,     0,   294,   295,   102,     0,     0,
     352,     0,     0,     0,     0,     0,   296,     0,    86,    32,
       0,   489,   416,     0,   373,   490,     7,   352,   295,    92,
      91,   273,   342,     0,   341,     0,     0,    87,   434,     0,
       0,    35,     0,   375,   366,     0,   352,    36,   372,     0,
     142,   138,     0,   362,   142,   139,     0,   285,     0,   374,
       0,   341,     0,     0,   492,   430,   488,   491,   487,     0,
      47,    50,     0,     0,   347,     0,   349,     0,     0,   348,
       0,   341,     0,     0,     6,    46,     0,   164,     0,   259,
     258,   184,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   352,     0,     0,     0,    25,    26,     0,
      27,     0,     0,     0,     0,     0,     0,     0,    28,     0,
      29,     0,   360,   358,     0,   353,   354,   359,     0,     0,
       0,     0,   112,     0,     0,   111,     0,     0,     0,   118,
       0,     0,    56,    99,     0,   219,   226,   227,   228,   223,
     225,   221,   224,   222,   220,   230,   229,   128,     0,     0,
      30,   234,   178,   300,     0,   301,   303,     0,   318,     0,
     306,     0,     0,    85,    31,    33,     0,   183,   272,     0,
      63,   432,   433,    88,     0,    34,   352,     0,   149,   140,
     136,   141,   137,     0,   283,   280,    60,     0,    56,   105,
      37,    49,    48,    51,     0,   456,     0,     0,   447,     0,
     449,     0,     0,     0,     0,     0,     0,   460,     8,     0,
       0,     0,     0,   252,     0,     0,     0,     0,     0,   402,
     484,   483,   486,   494,   493,   498,   497,   480,   477,   478,
     479,   427,   467,   446,   445,   444,   428,   471,   482,   476,
     474,   485,   475,   473,   465,   470,   472,   481,   464,   468,
     469,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   495,   500,   499,   246,   243,   244,   245,   249,   250,
     251,     0,     0,   406,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   403,   403,   108,   281,   346,     0,     0,
     368,     0,   282,   164,     0,     0,     0,   378,     0,     0,
       0,   384,     0,     0,     0,   119,   502,    59,     0,    52,
      57,     0,   127,     0,     0,     0,   367,     0,   235,     0,
     242,   260,     0,   304,     0,   312,   315,   317,   313,   314,
     316,     0,   311,   419,     0,   298,   417,   297,   443,   344,
     343,     0,     0,     0,   369,     0,   143,   287,   419,     0,
       0,     0,   457,   426,   448,   350,   450,   351,     0,     0,
     459,   124,   388,     0,   462,   461,     0,     0,   165,     0,
       0,   175,     0,   172,   256,   253,   254,   257,   185,     0,
       0,   289,   288,   290,   292,    64,    71,    72,    73,    68,
      70,    78,    79,    66,    69,    67,    65,    75,    74,    76,
      77,   441,   442,   247,   248,   411,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,   356,   357,   355,     0,     0,   126,     0,     0,   110,
       0,   109,     0,     0,   116,     0,     0,   114,     0,     0,
     409,     0,   100,     0,   101,     0,     0,   130,     0,   132,
     241,   233,     0,   331,   261,   264,   263,   265,     0,   302,
     305,   306,     0,     0,   307,   308,   151,     0,     0,   150,
     153,   370,     0,   144,   147,     0,   284,    61,    62,     0,
       0,     0,     0,   125,     0,     0,     0,   293,   170,     0,
     173,   169,   255,   260,   216,   214,   195,   198,   196,   197,
     208,   199,   212,   204,   202,   215,   203,   201,   206,   211,
     213,   200,   205,   209,   210,   207,   217,   218,     0,   193,
       0,   231,   231,   191,   291,   407,   435,   435,     0,     0,
       0,     0,     0,     0,     0,     0,   107,   106,     0,   113,
       0,     0,   377,     0,   376,     0,     0,   383,   117,     0,
     382,   115,     0,   408,    54,    53,   129,   393,   131,     0,
     236,     0,     0,   306,   262,   278,   299,   322,     0,   458,
       0,   155,     0,     0,     0,   145,     0,   122,   390,     0,
       0,   120,   389,     0,   463,     0,    39,     0,   159,    80,
     293,   293,   157,   293,   167,   176,   174,     0,   194,     0,
       0,   234,   187,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   293,   381,     0,     0,   387,     0,     0,
     410,     0,     0,   134,   331,   334,   337,   339,   335,   336,
     338,     0,   333,   340,   419,   274,   238,   237,     0,     0,
       0,   320,   419,   156,   154,     0,   148,     0,   123,     0,
     121,    41,    40,   168,   266,   192,   193,   231,   231,     0,
       0,     0,     0,     0,     0,     0,   160,   158,     0,     0,
       0,     0,    55,   133,   306,   323,     0,   275,   277,   276,
     279,   270,   271,   179,     0,   146,     0,     0,   267,   278,
     189,   190,   232,     0,     0,     0,     0,     0,     0,   380,
     379,   386,   385,   240,   239,   325,   326,   328,   419,     0,
     458,   419,   392,   391,     0,     0,   327,   329,   268,   186,
     269,   323,   330
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   241,    81,   542,    83,    84,   256,    85,
     242,   537,   541,   538,    86,    87,    88,   165,    89,   169,
     189,    90,    91,    92,    93,    94,    95,   646,    96,    97,
      98,   399,   576,   705,    99,   100,   572,   700,   101,   102,
     431,   717,   103,   104,   602,   603,   148,   182,   551,   106,
     107,   433,   723,   608,   750,   751,   368,   832,   372,   547,
     548,   549,   501,   609,   261,   688,   919,   949,   913,   202,
     908,   866,   869,   108,   228,   404,   109,   110,   185,   186,
     376,   377,   565,   380,   562,   936,   863,   793,   243,   160,
     247,   248,   334,   335,   336,   149,   112,   113,   114,   150,
     116,   136,   137,   502,   350,   669,   503,   117,   151,   152,
     208,   337,   154,   122,   155,   156,   125,   126,   252,   127,
     128,   129,   130,   131
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -809
static const yytype_int16 yypact[] =
{
    -809,   103,  2640,  -809,   -32,  -809,  -809,  -809,  -809,  -809,
    -809,  3896,    89,   124,  -809,    89,  8332,   115,   124,  3896,
    8332,  3896,    96,  -809,   355,   677,  6902,  8332,  7045,  8332,
     106,  -809,   124,  -809,    63,  7474,  8332,  8332,  -809,  8332,
    8332,  8332,   247,   134,   395,  1019,  -809,  7617,  6473,  8332,
    7474,  8332,  8332,   219,   158,  3896,  8332,  8475,  8475,   124,
    -809,  8332,  7617,  8332,  8332,  -809,  -809,  8332,  -809,  -809,
   10191,  8332,  8332,  -809,  8332,  -809,  -809,  3038,  6044,  7617,
    -809,  3753,  -809,  -809,   257,  -809,  -809,  -809,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,
    -809,  -809,   124,  -809,   124,   -35,   177,  -809,  -809,  -809,
     160,   200,  -809,  -809,  -809,   233,   250,   186,   262,   263,
   13626, 11715,   165,   268,   274,  -809,  -809,  -809,  -809,  -809,
    -809,   303,  -809,  -809, 13626,   279,  3896,  -809,   283,  -809,
     289,  8332,  8332,  8332,  8332,  8332,  7617,  7617,   190,  -809,
    -809,  -809,  -809, 12064,   226,  -809,  -809,   124,   299,  -809,
     -29, 13626,   349,  6616,  -809,  -809,  -809,  -809,   124,    83,
     124,   297,    47, 11511, 11477,  -809,  -809,  -809, 11986, 10806,
    6616,  3896,   298,    33,    87,    54,  -809,  3896,  -809,  -809,
   11664,   495,   326, 11664,   326,   495,  -809,  6616,   302,  -809,
    -809,   124,  -809,   206, 13626,  8332,  8332,  -809, 13626,   312,
   11856,  -809, 11664,   326, 13626,   304,  6616,  -809, 13626, 12311,
    -809,  -809, 12349,  2076,  -809,  -809, 12395,   353,   316,   326,
     231, 12141, 11664, 12472,   183,  2262,   495,   183,   495,    -9,
    -809,  -809,  3181,   102,  -809,  8332,  -809,     7,     8,  -809,
      59, 12518,   -17,   444,  -809,  -809,   405,   339,   314,  -809,
    -809,  -809,    49,    63,    63,    63,  -809,  8332,  8332,  8332,
    8332,  7760,  7760,  8332,  8332,  8332,  8332,  8332,  8332,    80,
   10191,  8332,  8332,  8332,  8332,  8332,  8332,  8332,  8332,  8332,
    8332,  8332,  8332,  8332,  8332,  8332,  7188,  7188,  7188,  7188,
    7188,  7188,  7188,  7188,  7188,  7188,  7188,  7188,  7188,  7188,
    7188,  7188,  6616,  6616,  7760,  7760,  5901,  -809,  -809,  3324,
    -809, 12187, 12265, 12556,    69,  7760,    47,   324,  -809,  8332,
    -809,  8332,   363,  -809,   319,   345,  -809,  -809,   334,   124,
     451,  7617,  -809,  4039,  7760,  -809,  4182,  7760,   346,  -809,
      47,  8618,  8332,  -809,  3896,  -809,  -809,  -809,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -809,  -809,   463,  8332,   348,
    -809,    64,  -809,  -809,    33,  -809,   378,   351,  -809,  8761,
     401,  8332,    63,  -809,  -809,  -809,   354,  -809,  -809,  7617,
    -809, 13626, 13626,  -809,    29,  -809,  6616,   356,  -809,   483,
    -809,   483,  -809,  8904,   386,  -809,  -809,  8618,  8332,  -809,
    -809,  -809,  -809,  -809,  7331,  -809, 10683,  6187,  -809,  6330,
    -809,  7760,  5469,  2895,   357,  8332,  5757,  -809,  -809,   124,
    7617,   370,   256,   168,    63,   236,   241,   244,   258, 11940,
    2425,  2425,   290,  -809,   290,  -809,   290, 11550,  1119,   957,
     842,   326,   183,  -809,  -809,  -809,  2262,  1756,   290,  1334,
    1334,  2425,  1334,  1334,   614,   183,  1756, 13740,   614,   495,
     495,   183,   381,   383,   385,   389,   391,   392,   396,   399,
     400,   402,   407,   409,   410,   415,   416,   418,   368,   419,
    -809,   290,  -809,   290,    98,  -809,  -809,  -809,  -809,  -809,
    -809,   124,    51,  -809, 13690,   329,  9047,  7760,  9190,  7760,
    8332,  7760,  1993,    89, 12637,  -809,  -809, 13626, 12675,  6616,
    -809,  6616,  -809,   339,  8332,    75,  8332, 13626,    52, 11783,
    8332, 13626,    68, 11630,  5901,  -809,   427,   414,   428, 12765,
     414,   436,   488, 12803,  3896, 11817,  -809,   187,  -809,    71,
    -809,   240,   430,    33,    87,    96,  8332,  8332,  8332,  8332,
    8332,  5326,  -809,   320,  6759,  -809, 13626,  -809,  -809,  -809,
   13626,   411,    40,   432,  -809,    55,  -809,  -809,   259,   124,
     437,   438,  -809,  -809,  -809,  -809,  -809,  -809,   -27, 10480,
    -809,  -809, 13626,  3896, 13626,  -809, 12842,   439,   464,   972,
     440,   482,   121,  -809,   515,  -809,  -809,  -809,  -809,  2499,
     261,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -809,  5901,  -809,  7760,  7760,
    8332,   558, 12961,  8332,   559, 12995,   442, 10526,    47,    47,
    -809,  -809,  -809,  -809,   448, 11664,  -809, 10875,  4325,  -809,
    4468,  -809, 10944,  4611,  -809,    47,  4754,  -809,    47,   188,
    -809,  8332,  -809,  8332,  -809,  3896,  8332,  -809,  3896,   562,
     481,  -809,    33,   479,   533,  -809,  -809,  -809,    22,  -809,
    -809,   401,   454,    76,  -809,  -809,  -809,  4897,  7617,  -809,
    -809,  -809,   124,  -809,   489,   289,  -809,  -809,  -809,  5040,
     456,  5183,   462,  -809,  8332,  3467,   468,   566,  -809,  8332,
     124,  -809,  -809,   240,  -809,  -809,  -809,   124,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -809,  -809,  -809,  8332,   496,
     497,   472,   472,  -809,  -809,  -809,   192,   203, 13085,  9333,
    9476, 13119,  9619,  9762,  9905, 10048,  -809,  -809, 10446,  -809,
    3896,  8332, 13626,  8332, 13626,  3896,  8332, 13626,  -809,  8332,
   13626,  -809,  5901,  -809, 13158, 13626,  -809, 13626,   589,  3896,
    -809,   474,  7903,    -7,  -809,   530,  -809,  -809,  7760, 10397,
    3896,  -809,    45,   476,  8332,  -809,  8332,  -809, 13626,  3896,
    8332,  -809, 13626,  3896, 13626,    36,  -809,  3610,  -809,   382,
     507,   507,  -809,  1116,  -809, 13626,  -809,    86,  -809, 10729,
    2733,    64,  -809,  -809,  8332,  8332,  8332,  8332,  8332,  8332,
    8332,  8332,   480,  1254, 12803, 11013, 11082, 12803, 11151, 11220,
    -809,  8332,  3896,  -809,   479,    96,  8332,  8332,  8332,  8332,
    8332,  5613,  -809,  -809,   114,  7617,  -809,  -809,  8332,    16,
   10564,  -809,   793,   349,  -809,   289, 13626, 11289,  -809, 11358,
    -809,  -809,  -809,  -809,   542,  -809,  -809,   472,   472,   209,
   13242, 13306, 13370, 13434, 13498, 13562,  -809,  -809,  3896,  3896,
    3896,  3896, 13626,  -809,    -7,  8046,    84,  -809,  -809, 13626,
   13626,  -809,  -809,  -809, 10048,  -809,  3896,  3896,  -809,   530,
    -809,  -809,  -809,  8332,  8332,  8332,  8332,  8332,  8332, 12803,
   12803, 12803, 12803,  -809,  -809,  -809,  -809,  -809,   116,  7760,
   10278,   948, 12803, 12803,    27, 10610,  -809,  -809,  -809,  -809,
    -809,  8189,  -809
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -809,  -809,  -809,    -1,  -666,  1765,  -809,  -809,  -809,  1285,
     -90,  -271,   223,   228,  -809,  -809,   325,   224,  2184,    -4,
     610,  -789,  -685,   -42,  -809,  -809,  -809,    66,  -809,  -809,
    -809,   420,  -809,   -69,  -809,  -809,  -809,  -809,  -809,   477,
     122,  -121,  -809,  -809,  -809,   -71,   874,  -809,  -809,  -809,
    -809,  -809,  -809,  -809,  -809,  -179,  -598,  -739,  -809,  -178,
     -26,  -809,  -365,  -809,  -809,   -66,  -809,  -809,  -285,    48,
    -809,  -239,  -252,  -809,  -147,  -809,   640,  -809,  -221,   288,
    -809,  -366,  -679,  -532,  -399,  -689,  -808,  -182,   -16,  -809,
      12,  -809,  -140,  -809,   154,   309,  -374,  -809,  -809,  1010,
    -809,     0,  -809,  -809,  -234,  -809,  -527,  -809,  1086,  1346,
      -2,   175,   585,  -809,  1417,  1617,  -809,  -809,  -809,  -809,
    -809,  -809,  -809,  -288
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -459
static const yytype_int16 yytable[] =
{
     120,    80,   199,   200,   577,   563,   550,   670,   552,   134,
     327,   752,   796,   833,   153,   140,   424,   120,   161,   120,
     166,   338,   691,   378,   173,   174,   178,   179,   505,   578,
     571,   203,   824,   190,   191,   192,     5,   193,   194,   195,
     369,   435,   436,   437,   438,   204,   230,   210,   212,   213,
     214,   823,     5,   218,   219,   222,   226,   386,     5,   229,
     231,   232,   233,   250,   348,   234,     5,  -241,   235,   236,
     237,   187,   238,   187,     5,   204,   397,   251,   658,   120,
     912,   540,  -152,     5,    48,   494,   329,   378,   378,   564,
    -309,   381,   172,   697,   663,    48,   421,   937,   426,  -399,
      14,   495,   132,     3,   496,   330,   511,   453,  -440,   755,
    -440,  -309,   865,   798,   867,   434,   373,  -152,   497,   709,
     498,   939,   417,   419,  -309,   132,   246,     5,   698,   427,
     499,   324,   947,   411,   120,   633,   379,   540,   883,   134,
     321,   322,   323,   218,   204,   251,   500,    46,   920,   921,
     348,   418,   420,   326,  -309,   912,   795,   319,   824,   348,
     389,   948,   454,  -309,   634,  -309,   636,    60,   319,   382,
     132,  -152,   488,   489,   422,  -309,   374,   823,   881,   120,
     135,   262,   699,  -309,   422,   120,   319,   690,   319,   604,
     389,   422,   183,   319,   387,   637,   319,   157,   702,   422,
     379,   379,  -309,   391,   392,   423,   183,  -241,   270,   319,
    -332,   333,  -324,   610,   682,   512,   263,   414,   259,   656,
    -309,  -309,   799,   209,   605,   934,   264,  -309,   333,  -332,
     940,  -324,   888,  -332,   606,  -324,   720,   138,   260,   163,
     120,   413,   158,   416,   415,   333,   505,   265,   271,   180,
     607,   946,   272,   249,   196,   850,   573,   600,  -332,     5,
    -324,   684,   952,   721,   333,   439,   440,   441,   442,   444,
     446,   447,   448,   449,   450,   451,   452,   197,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   215,   884,   685,   277,   665,   668,
     279,   216,   680,   782,   282,     5,   686,  -412,   312,   164,
     313,   111,   491,   493,   504,   550,   791,   120,  -413,   255,
     111,   389,   687,   514,   680,   525,   267,   517,   111,   518,
     111,   681,   783,  -440,  -396,  -440,  -412,   443,   445,   204,
     390,   527,   529,   -80,   531,   533,   389,  -413,   505,   539,
     539,   382,   543,   922,   710,   712,   382,  -310,     5,   382,
     333,   333,   164,   -80,   111,   406,   545,  -395,   314,  -439,
     611,  -439,   315,   382,  -286,   612,   382,   120,   613,   566,
     490,   492,   221,   225,   266,     5,   111,   570,  -310,   164,
     111,   513,   614,  -286,   639,   754,  -400,  -401,   315,  -310,
     871,   120,  -398,  -295,   277,   539,   539,   279,  -397,  -310,
     528,   282,   570,   532,   598,   175,  -310,   318,   864,   589,
     570,   592,   316,   594,   596,   872,    24,   198,   204,   585,
     319,   587,   706,   328,    31,  -310,  -180,   569,  -295,   331,
     341,   371,   -44,   279,   333,   111,   393,   396,  -295,   403,
     405,   428,   429,   430,  -310,   432,  -180,   -44,   516,   519,
     521,  -310,   569,   520,   168,   246,   550,   246,   522,  -295,
     569,   472,   473,   474,   475,   476,   477,   478,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   588,   524,   534,
     111,   544,   546,   553,   505,   554,   111,   564,   568,   575,
     574,   579,   494,   593,   527,   642,   531,   645,   543,   647,
     592,   599,   631,   648,   649,   615,   675,   616,   495,   617,
     270,   496,   655,   618,   657,   619,   620,   175,   662,   671,
     621,   938,   504,   622,   623,   497,   624,   498,    24,   198,
     941,   625,   120,   626,   627,   693,    31,   499,  -180,   628,
     629,   111,   630,   696,   194,   213,   214,   218,   229,   251,
     271,   -58,   672,   500,   272,   632,   938,   651,  -180,   333,
     674,   707,   708,   641,   689,   644,   701,   938,   719,   389,
     715,   722,   718,    14,   759,   762,    17,   121,   764,   768,
     789,   120,   249,   792,   249,    23,   121,    24,   819,   794,
     797,    28,   809,   804,   121,    31,   121,  -180,   813,   277,
     818,   278,   279,  -360,   830,   831,   282,   852,   854,   868,
     875,   918,   896,   289,    42,   817,   820,  -180,   111,   821,
      46,   581,   295,   803,   504,   580,   444,   491,   758,   270,
     121,   761,   223,   223,   401,   654,   339,   843,    59,   826,
      60,   887,   111,   889,   790,   111,   772,   827,   774,   950,
     211,   777,   121,   111,   780,   933,   121,   944,   177,   784,
     567,   785,   904,   120,   787,   653,   120,     0,     0,   271,
       5,     0,   802,   272,   164,     0,   167,     0,     0,     0,
       0,     0,     0,     0,   652,   120,   204,     0,  -295,     0,
      75,     0,     0,     0,   443,   490,     0,   808,   822,   812,
       0,     0,   814,   120,     0,     0,     0,   825,     0,     0,
       0,   121,     0,     0,   274,     0,   275,   276,   277,     0,
     278,   279,   111,  -295,     0,   282,     0,     0,     0,   695,
       0,     0,   289,  -295,     0,     0,   829,     0,     0,   293,
     294,   295,     0,     0,     0,     0,     0,   772,   774,   168,
     777,   780,   808,   812,  -295,     0,   121,     0,   844,   845,
       0,   846,   121,   847,   848,     0,     0,   849,   199,   200,
     504,     0,   342,   345,   349,     0,     0,   120,     0,     0,
     120,     0,     0,     0,     0,     0,   870,   592,   120,     0,
     384,     0,   876,   385,   877,     0,     0,   120,   879,     0,
     635,   120,     0,     0,     0,   120,   413,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,   121,     0,     0,
    -319,     0,   844,   890,   891,   847,   892,   893,   894,   895,
     862,     0,   409,     0,     0,   906,     0,     0,     0,   902,
     120,     0,     0,   111,   194,   213,   214,   218,   229,   251,
       0,  -319,     0,   909,     0,     0,   910,   270,     0,     0,
       0,     0,  -319,     0,     0,     0,   105,     0,     0,     0,
       0,     0,  -319,     0,     0,   105,     0,     0,     0,  -319,
       0,     0,     0,   105,     0,   105,   929,   930,   931,   932,
       0,     0,   111,   120,   121,     0,     0,   271,  -319,     0,
       0,   272,   812,   907,   942,   943,     0,     0,   753,     0,
       0,   929,   930,   931,   932,   942,   943,  -319,   121,   105,
       0,   121,     0,     0,  -319,     0,   515,   945,   592,   121,
       0,     0,     0,     0,     0,     0,     0,   756,   757,   812,
       0,   105,   274,   935,     0,   105,   277,     0,   278,   279,
     535,     0,     0,   282,     0,     0,     0,   111,     0,   111,
     289,     0,   111,   716,     0,   111,     0,   293,   294,   295,
       0,     0,   270,     0,   111,  -321,     0,   111,   935,  -166,
       0,     0,  -166,  -166,     0,     0,     0,     0,     0,   935,
       0,  -166,     0,  -166,  -166,     0,   111,  -166,   121,     0,
     105,  -166,   115,  -166,     0,     0,  -321,     0,   111,     0,
     111,   115,   271,     0,   111,     0,   272,  -321,  -166,   115,
    -166,   115,  -166,  -166,     0,  -166,  -166,  -321,  -166,   175,
       0,     0,     0,     0,  -321,     0,     0,     0,     0,     0,
      24,   198,     0,     0,  -166,   105,  -166,     0,    31,  -166,
    -180,   105,     0,  -321,     0,   115,   -43,   274,     0,     0,
     276,   277,     0,   278,   279,     0,     0,     0,   282,   111,
    -180,   -43,  -321,     0,   111,   289,     0,   115,   118,  -321,
       0,   115,   293,   294,   295,     0,     0,   118,   111,     0,
       0,     0,     0,     0,     0,   118,  -166,   118,     0,   111,
       0,     0,     0,     0,  -166,     0,   105,     0,   111,     0,
       0,     0,   111,     0,     0,     0,   111,     0,     0,   121,
       0,     0,     0,    14,     0,     0,    17,     0,   659,   661,
       0,   118,   664,   667,   270,    23,   115,    24,   819,     0,
       0,    28,     0,     0,     0,    31,     0,  -180,     0,     0,
       0,   111,     0,   118,     0,     0,     0,   118,     0,     0,
       0,     0,     0,     0,   253,     0,   820,  -180,   121,   821,
      46,     0,     0,     0,   271,     0,     0,     0,   272,     0,
       0,   115,     0,   105,     0,     0,     0,   115,    59,     0,
      60,     0,     0,     0,     0,     0,     0,   111,   111,   111,
     111,     0,     0,     0,     0,     0,     0,   105,     0,     0,
     105,     0,   118,     0,     0,   111,   111,     0,   105,     0,
       0,     0,     0,   277,     0,   278,   279,     0,     0,     0,
     282,     0,     0,   121,     0,   121,     0,   289,   121,     0,
      75,   121,   115,     0,   293,   294,   295,     0,   766,   767,
     121,     0,     0,   121,     0,   769,     0,   118,     0,     0,
       0,    14,     0,   118,    17,   778,     0,     0,   781,     0,
       0,     0,   121,    23,     0,    24,   819,     0,     0,    28,
       0,     0,     0,    31,   121,  -180,   121,   105,     0,     0,
     121,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,     0,   820,  -180,     0,   821,    46,     0,
     188,     0,     0,     0,     0,     0,     0,     0,   118,   115,
       0,     0,     0,     0,     0,   188,    59,     0,    60,     0,
       0,     0,   220,   224,     0,     0,     0,     0,   119,     0,
       0,     0,     0,   115,     0,   121,   115,   119,     0,   270,
     121,     0,     0,     0,   115,   119,     0,   119,     0,     0,
       0,     0,     0,     0,   121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   121,     0,     0,    75,     0,
       0,     0,     0,     0,   121,     0,   897,     0,   121,   271,
       0,   119,   121,   272,     0,   118,     0,     0,     0,     0,
       0,     0,   874,     0,     0,     0,     0,     0,   105,   123,
       0,     0,     0,   119,     0,   320,     0,   119,   123,   118,
       0,     0,   118,   115,     0,     0,   123,   121,   123,     0,
     118,     0,     0,     0,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,     0,   282,     0,   188,   188,   188,
       0,   288,   289,     0,   367,     0,   292,   105,     0,   293,
     294,   295,   123,     0,     0,   188,     0,     0,   188,     0,
       0,     0,   119,   121,   121,   121,   121,     0,     0,     0,
       0,     0,     0,     0,   123,     0,     0,   188,   123,     0,
       0,   121,   121,     0,     0,     0,     0,     0,     0,   118,
       0,     0,     0,     0,     0,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,     0,     0,
       0,     0,   105,   119,   105,     0,     0,   105,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,   105,
       0,     0,   105,   123,   115,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,   105,     0,     0,   119,   105,
       0,     0,     0,     0,     0,     0,     0,     0,   123,     0,
       0,     0,     0,   115,   123,     0,     0,     0,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,     0,   124,
       0,     0,     0,     0,     0,     0,     0,     0,   124,     0,
     118,     0,     0,     0,     0,   188,   124,     0,   124,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   123,
       0,     0,     0,   105,     0,   119,     0,     0,   115,     0,
     115,     0,   124,   115,   105,     0,   115,     0,     0,   118,
       0,     0,     0,   105,     0,   115,     0,   105,   115,   119,
       0,   105,   119,     0,   124,     0,     0,     0,   124,     0,
     119,     0,     0,     0,     0,     0,     0,   115,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
       0,   115,     0,     0,     0,   115,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   123,     0,     0,     0,
       0,     0,     0,     0,   118,     0,   118,     0,     0,   118,
       0,     0,   118,   124,     0,     0,     0,     0,     0,     0,
     123,   118,     0,   123,   118,     0,     0,    82,     0,   119,
       0,   123,   105,   105,   105,   105,   133,     0,     0,     0,
     115,   270,     0,   118,   159,   115,   162,     0,     0,     0,
     105,   105,     0,     0,     0,   118,     0,   118,   124,   115,
       0,   118,     0,     0,   124,     0,     0,     0,     0,     0,
     115,     0,     0,   188,   188,     0,     0,   188,   188,   115,
     217,   271,     0,   115,     0,   272,     0,   115,     0,     0,
     679,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,     0,    82,     0,     0,     0,   254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   118,     0,     0,   124,
     703,   118,   115,     0,     0,     0,   274,     0,   275,   276,
     277,     0,   278,   279,   280,   118,     0,   282,   283,   284,
       0,   286,   287,   288,   289,     0,   118,     0,   292,     0,
     119,   293,   294,   295,     0,   118,     0,     0,     0,   118,
       0,   317,     0,   118,     0,     0,     0,     0,   115,   115,
     115,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   115,     0,     0,
       0,     0,     0,   188,   188,     0,   124,     0,   118,   119,
     188,     0,   367,     0,     0,     0,   370,   367,     0,     0,
     188,     0,   383,   188,     0,     0,     0,     0,     0,     0,
     124,   123,     0,   124,     0,     0,     0,     0,     0,     0,
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   118,   118,   118,   118,     0,     0,
     805,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,   118,   118,   119,   141,   119,    82,     0,   119,
     123,     0,   119,     0,     0,     0,     0,     0,     0,     0,
      22,   119,     0,     0,   119,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
     124,    37,    38,   119,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,   119,     0,   119,     0,     0,
       0,   119,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,     0,     0,   123,     0,   123,    61,     0,
     123,     0,     0,   123,    82,     0,     0,   188,     0,     0,
       0,     0,   123,     0,     0,   123,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,     0,     0,   383,     0,
       0,   383,     0,     0,   123,     0,   119,     0,     0,     0,
       0,   119,    72,     0,     0,     0,   123,     0,   123,     0,
     367,   367,   123,   367,   367,   119,    78,     0,   147,     0,
       0,     0,     0,     0,     0,     0,   119,     0,     0,     0,
       0,     0,     0,     0,   911,   119,     0,     0,     0,   119,
     915,   124,   367,   119,   367,     0,     0,     0,     0,     0,
       0,     0,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,     0,   123,   591,     0,
       0,     0,   123,     0,     0,     0,     0,   139,   119,     0,
       0,     0,   139,     0,     0,     0,   123,     0,   139,   139,
     124,     0,     0,     0,   310,   311,   181,   123,   184,  -439,
       0,  -439,     0,     0,     0,     0,   123,     0,     0,   911,
     123,     0,     0,     0,   123,     0,     0,     0,     0,     0,
       0,     0,     0,   227,   119,   119,   119,   119,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,   119,     0,     0,     0,     0,     0,   123,
       0,     0,     0,     0,     0,   124,     0,   124,     0,     0,
     124,     0,     0,   124,     0,     0,   257,   270,   258,     0,
       0,     0,   124,     0,     0,   124,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   677,
       0,     0,     0,     0,   124,   123,   123,   123,   123,     0,
       0,     0,     0,     0,     0,     0,   124,   271,   124,     0,
       0,   272,   124,   123,   123,     0,     0,     0,     0,     0,
       0,   227,     0,     0,     0,     0,     0,   332,     0,     0,
       0,     0,   227,     0,   340,     0,     0,     0,   713,     0,
       0,     0,     0,     0,   332,     0,     0,   375,     0,     0,
       0,     0,   274,     0,   275,   276,   277,     0,   278,   279,
       0,   332,   139,   282,     0,   388,     0,   124,     0,   288,
     289,     0,   124,     0,   292,     0,     0,   293,   294,   295,
     332,     0,     0,     0,     0,     0,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   124,     0,     0,
       0,     0,     0,   383,     0,   383,   124,     0,   383,     0,
     124,   383,     0,     0,   124,     0,     0,     0,     0,     0,
     786,     0,     0,   788,     0,     0,   184,   184,   184,   184,
     270,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   801,   455,     0,     0,     0,     0,     0,   124,
       0,     0,     0,     0,   807,     0,   811,     0,     0,     0,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,   272,     0,   332,   332,     0,     0,
       0,     0,     5,     0,     0,     0,     0,     0,     0,   724,
       0,     0,     0,     0,   725,   124,   124,   124,   124,     0,
     273,     0,     0,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   124,   124,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,     0,
     286,   287,   288,   289,   853,   290,   291,   292,   375,     0,
     293,   294,   295,     0,     0,   873,   184,     0,     0,     0,
       0,     0,     0,     0,   878,     0,     0,     0,   880,     0,
     332,     0,    82,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   355,   356,   357,   358,   359,
     360,     0,     0,   361,   362,   363,   364,   365,   366,   726,
     727,   728,   729,   597,     0,   730,   601,   903,   184,   731,
     732,   733,   734,   735,   736,   737,   738,   739,   740,   741,
       0,   742,     0,     0,   743,   744,   745,   746,     0,   747,
      -2,     4,   748,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,     0,     0,    40,  -293,    41,    42,    43,
      44,  -180,   -42,    45,    46,   332,  -293,    47,    48,     0,
      49,    50,    51,    52,    53,    54,    55,     0,    56,     0,
      57,    58,    59,     0,    60,    61,    62,  -293,     0,     0,
      63,     0,    64,   683,     0,    65,     5,   375,     0,     0,
       0,     0,     0,   724,     0,     0,     0,     0,   725,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,   704,
       0,     0,     0,   227,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,    75,     0,     0,    76,     0,     0,
       0,    77,     0,    78,     0,    79,     0,     0,     0,     0,
       0,     0,     0,   749,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   355,
     356,   357,   358,   359,   360,     0,     0,   361,   362,   363,
     364,   365,   366,   726,   727,   728,   729,     0,     0,   730,
       0,     0,     0,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,     0,   742,   375,     0,   743,   744,
     745,   746,     0,   747,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   704,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     0,     5,     6,
       7,     8,     9,    10,   601,  -458,     0,    11,    12,    13,
    -458,   828,    14,    15,    16,    17,  -293,    18,    19,    20,
    -458,    21,    22,  -458,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
      40,  -293,    41,     0,    43,    44,  -180,   -42,    45,    46,
    -458,  -293,    47,    48,  -458,    49,    50,    51,    52,    53,
      54,    55,     0,    56,     0,    57,    58,    59,     0,    60,
      61,    62,  -293,     0,     0,    63,     0,    64,     0,     0,
    -458,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,  -458,    67,  -458,  -458,  -458,
    -458,  -458,  -458,  -458,   886,  -458,  -458,  -458,  -458,  -458,
    -458,  -458,  -458,  -458,    72,  -458,  -458,  -458,     0,    75,
    -458,  -458,  -458,     0,     0,     0,    77,  -458,    78,   239,
      79,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -293,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,  -180,
      32,    33,    34,    35,    36,   -42,    37,    38,     0,    39,
       0,     0,     0,    40,  -293,    41,    42,    43,    44,  -180,
     -42,    45,    46,     0,  -293,    47,    48,     0,    49,    50,
      51,    52,    53,    54,    55,     0,    56,     0,    57,    58,
      59,     0,    60,    61,    62,  -293,     0,     0,    63,     0,
      64,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,   201,    75,     0,     0,    76,     0,     0,     0,    77,
     240,    78,     4,    79,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -293,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,    29,     0,    30,
      31,     0,  -180,    32,    33,    34,    35,    36,   -42,    37,
      38,     0,    39,     0,     0,     0,    40,  -293,    41,    42,
      43,    44,  -180,   -42,    45,    46,     0,  -293,    47,    48,
       0,    49,    50,    51,    52,    53,    54,    55,     0,    56,
       0,    57,    58,    59,     0,    60,    61,    62,  -293,     0,
       0,    63,     0,    64,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,    75,     0,     0,    76,     0,
       0,     0,    77,   412,    78,   239,    79,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -293,    18,    19,    20,     0,
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,  -180,    32,    33,    34,    35,
      36,   -42,    37,    38,     0,    39,     0,     0,     0,    40,
    -293,    41,    42,    43,    44,  -180,   -42,    45,    46,     0,
    -293,    47,    48,     0,    49,    50,    51,    52,    53,    54,
      55,     0,    56,     0,    57,    58,    59,     0,    60,    61,
      62,  -293,     0,     0,    63,     0,    64,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,    75,     0,
       0,    76,     0,     0,     0,    77,   240,    78,   815,    79,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -293,    18,
      19,    20,     0,    21,    22,     0,    23,     0,    24,    25,
      26,    27,    28,    29,     0,    30,    31,     0,  -180,    32,
      33,    34,    35,    36,   -42,    37,    38,     0,    39,     0,
       0,     0,    40,  -293,    41,    42,    43,    44,  -180,   -42,
      45,    46,     0,  -293,    47,    48,     0,    49,    50,    51,
      52,    53,    54,    55,     0,    56,     0,    57,    58,    59,
       0,    60,    61,    62,  -293,     0,     0,    63,     0,    64,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,    75,     0,     0,    76,     0,     0,     0,    77,   816,
      78,     4,    79,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,     0,     0,    40,  -293,    41,    42,    43,
      44,  -180,   -42,    45,    46,     0,  -293,    47,    48,     0,
      49,    50,    51,    52,    53,    54,    55,     0,    56,     0,
      57,    58,    59,     0,    60,    61,    62,  -293,     0,     0,
      63,     0,    64,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,    75,     0,     0,    76,     0,     0,
       0,    77,   882,    78,     4,    79,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -293,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,    29,
       0,    30,    31,     0,  -180,    32,    33,    34,    35,    36,
     -42,    37,    38,     0,    39,     0,     0,     0,    40,  -293,
      41,   253,    43,    44,  -180,   -42,    45,    46,     0,  -293,
      47,    48,     0,    49,    50,    51,    52,    53,    54,    55,
       0,    56,     0,    57,    58,    59,     0,    60,    61,    62,
    -293,     0,     0,    63,     0,    64,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,    75,     0,     0,
      76,     0,     0,     0,    77,     0,    78,     4,    79,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,  -293,    18,    19,
      20,     0,    21,    22,     0,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,  -180,    32,    33,
      34,    35,    36,   -42,    37,    38,     0,    39,     0,     0,
       0,    40,  -293,    41,     0,    43,    44,  -180,   -42,    45,
      46,     0,  -293,    47,    48,     0,    49,    50,    51,    52,
      53,    54,    55,     0,    56,     0,    57,    58,    59,     0,
      60,    61,    62,  -293,     0,     0,    63,     0,    64,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,     0,
      75,     0,     0,    76,     0,     0,     0,    77,     0,    78,
       4,    79,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,     0,    14,    15,    16,    17,
    -293,    18,    19,    20,     0,    21,    22,     0,    23,     0,
      24,    25,    26,    27,    28,   526,     0,    30,    31,     0,
    -180,    32,    33,    34,    35,    36,   -42,    37,    38,     0,
      39,     0,     0,     0,    40,  -293,    41,     0,    43,    44,
    -180,   -42,    45,    46,     0,  -293,    47,    48,     0,    49,
      50,    51,    52,    53,    54,    55,     0,    56,     0,    57,
      58,    59,     0,    60,    61,    62,  -293,     0,     0,    63,
       0,    64,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,    75,     0,     0,    76,     0,     0,     0,
      77,     0,    78,     4,    79,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,     0,    14,
      15,    16,    17,  -293,    18,    19,    20,     0,    21,    22,
       0,    23,     0,    24,    25,    26,    27,    28,   530,     0,
      30,    31,     0,  -180,    32,    33,    34,    35,    36,   -42,
      37,    38,     0,    39,     0,     0,     0,    40,  -293,    41,
       0,    43,    44,  -180,   -42,    45,    46,     0,  -293,    47,
      48,     0,    49,    50,    51,    52,    53,    54,    55,     0,
      56,     0,    57,    58,    59,     0,    60,    61,    62,  -293,
       0,     0,    63,     0,    64,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,     0,    75,     0,     0,    76,
       0,     0,     0,    77,     0,    78,     4,    79,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,     0,    14,    15,    16,    17,  -293,    18,    19,    20,
       0,    21,    22,     0,    23,     0,    24,    25,    26,    27,
      28,   771,     0,    30,    31,     0,  -180,    32,    33,    34,
      35,    36,   -42,    37,    38,     0,    39,     0,     0,     0,
      40,  -293,    41,     0,    43,    44,  -180,   -42,    45,    46,
       0,  -293,    47,    48,     0,    49,    50,    51,    52,    53,
      54,    55,     0,    56,     0,    57,    58,    59,     0,    60,
      61,    62,  -293,     0,     0,    63,     0,    64,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,    75,
       0,     0,    76,     0,     0,     0,    77,     0,    78,     4,
      79,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,     0,    14,    15,    16,    17,  -293,
      18,    19,    20,     0,    21,    22,     0,    23,     0,    24,
      25,    26,    27,    28,   773,     0,    30,    31,     0,  -180,
      32,    33,    34,    35,    36,   -42,    37,    38,     0,    39,
       0,     0,     0,    40,  -293,    41,     0,    43,    44,  -180,
     -42,    45,    46,     0,  -293,    47,    48,     0,    49,    50,
      51,    52,    53,    54,    55,     0,    56,     0,    57,    58,
      59,     0,    60,    61,    62,  -293,     0,     0,    63,     0,
      64,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,    75,     0,     0,    76,     0,     0,     0,    77,
       0,    78,     4,    79,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,     0,    14,    15,
      16,    17,  -293,    18,    19,    20,     0,    21,    22,     0,
      23,     0,    24,    25,    26,    27,    28,   776,     0,    30,
      31,     0,  -180,    32,    33,    34,    35,    36,   -42,    37,
      38,     0,    39,     0,     0,     0,    40,  -293,    41,     0,
      43,    44,  -180,   -42,    45,    46,     0,  -293,    47,    48,
       0,    49,    50,    51,    52,    53,    54,    55,     0,    56,
       0,    57,    58,    59,     0,    60,    61,    62,  -293,     0,
       0,    63,     0,    64,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,    75,     0,     0,    76,     0,
       0,     0,    77,     0,    78,     4,    79,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
       0,    14,    15,    16,    17,  -293,    18,    19,    20,     0,
      21,    22,     0,    23,     0,    24,    25,    26,    27,    28,
     779,     0,    30,    31,     0,  -180,    32,    33,    34,    35,
      36,   -42,    37,    38,     0,    39,     0,     0,     0,    40,
    -293,    41,     0,    43,    44,  -180,   -42,    45,    46,     0,
    -293,    47,    48,     0,    49,    50,    51,    52,    53,    54,
      55,     0,    56,     0,    57,    58,    59,     0,    60,    61,
      62,  -293,     0,     0,    63,     0,    64,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,    75,     0,
       0,    76,     0,     0,     0,    77,     0,    78,     4,    79,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,     0,    14,    15,    16,    17,  -293,    18,
      19,    20,     0,   800,    22,     0,    23,     0,    24,    25,
      26,    27,    28,    29,     0,    30,    31,     0,  -180,    32,
      33,    34,    35,    36,   -42,    37,    38,     0,    39,     0,
       0,     0,    40,  -293,    41,     0,    43,    44,  -180,   -42,
      45,    46,     0,  -293,    47,    48,     0,    49,    50,    51,
      52,    53,    54,    55,     0,    56,     0,    57,    58,    59,
       0,    60,    61,    62,  -293,     0,     0,    63,     0,    64,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,    75,     0,     0,    76,     0,     0,     0,    77,     0,
      78,     4,    79,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,     0,    14,    15,    16,
      17,  -293,    18,    19,    20,     0,    21,    22,     0,    23,
       0,    24,    25,    26,    27,    28,   806,     0,    30,    31,
       0,  -180,    32,    33,    34,    35,    36,   -42,    37,    38,
       0,    39,     0,     0,     0,    40,  -293,    41,     0,    43,
      44,  -180,   -42,    45,    46,     0,  -293,    47,    48,     0,
      49,    50,    51,    52,    53,    54,    55,     0,    56,     0,
      57,    58,    59,     0,    60,    61,    62,  -293,     0,     0,
      63,     0,    64,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,    75,     0,     0,    76,     0,     0,
       0,    77,     0,    78,     4,    79,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,     0,
      14,    15,    16,    17,  -293,    18,    19,    20,     0,    21,
      22,     0,    23,     0,    24,    25,    26,    27,    28,   810,
       0,    30,    31,     0,  -180,    32,    33,    34,    35,    36,
     -42,    37,    38,     0,    39,     0,     0,     0,    40,  -293,
      41,     0,    43,    44,  -180,   -42,    45,    46,     0,  -293,
      47,    48,     0,    49,    50,    51,    52,    53,    54,    55,
       0,    56,     0,    57,    58,    59,     0,    60,    61,    62,
    -293,     0,     0,    63,     0,    64,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,    75,     0,     0,
      76,     0,     0,     0,    77,     0,    78,   692,    79,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,   201,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
       0,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,   201,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,   590,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,   201,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   147,   905,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,     0,     0,     0,    76,     0,     0,     0,   146,     0,
      78,     0,   147,   595,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,   494,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   142,   143,     0,   144,   495,    30,
       0,   496,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,   497,    40,   498,    41,     0,
      43,     0,     0,     0,     0,     0,     0,   499,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,     0,     0,   500,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,     0,     0,     0,    76,     0,
       0,     0,   146,     0,    78,     0,   147,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,    40,
       0,    41,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
     145,     0,     0,     0,   205,   206,     0,   244,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,   245,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,   201,     0,     0,
       0,    76,     0,     0,     0,   146,     0,    78,     0,   147,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,   205,   206,     0,
     244,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
     201,     0,     0,     0,    76,     0,     0,     0,   146,     0,
      78,   584,   147,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   142,   143,     0,   144,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,   145,     0,     0,     0,
     205,   206,     0,   244,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,   201,     0,     0,     0,    76,     0,     0,
       0,   146,     0,    78,   586,   147,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,   205,   206,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,   207,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   147,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,   205,   206,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,   201,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
       0,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   144,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,   694,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,   205,
     206,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,   170,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,   171,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,     0,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,   176,   142,   143,
       0,   144,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,     0,
       0,     0,    76,     0,     0,     0,   146,     0,    78,     0,
     147,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,    40,     0,    41,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,   205,   206,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,     0,     0,     0,    76,     0,     0,     0,   146,
       0,    78,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   142,   143,     0,   144,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,    40,     0,    41,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,   201,     0,     0,     0,    76,     0,
       0,     0,   146,   582,    78,     0,   147,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,    22,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,    40,
       0,    41,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
     145,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,     0,     0,
       0,    76,     0,     0,     0,    77,     0,    78,     0,   147,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
     201,     0,     0,     0,    76,     0,     0,     0,   146,     0,
      78,     0,   147,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   142,   143,     0,   144,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,   171,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,     0,     0,     0,    76,     0,     0,
       0,   146,     0,    78,     0,   147,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     855,     0,     0,     0,     0,     0,   142,   143,     0,   144,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,   856,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   857,   858,    53,    54,   859,
       0,     0,     0,     0,     0,     0,     0,     0,   860,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,   201,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   861,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   144,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,   201,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
       0,   861,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   841,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,   201,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,     0,   144,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,     0,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,     0,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   142,   143,
       0,   144,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,     0,
       0,     0,    76,     0,     0,     0,    77,     0,    78,     0,
     147,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,    40,     0,    41,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,     0,     0,     0,   536,     0,     0,     0,   146,
       0,    78,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   555,     0,
       0,     0,     0,     0,   142,   143,     0,   144,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,   556,     0,    41,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   557,   558,    53,    54,   559,     0,     0,
       0,     0,     0,     0,     0,     0,   560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,    74,     0,     0,     0,     0,    76,     0,
       0,     0,   146,     0,    78,     0,   561,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     0,     0,     0,   142,   143,     0,
     144,     0,    30,     0,     0,     0,     0,    33,    34,     0,
      36,     0,    37,    38,     0,     0,     0,     0,     0,    40,
       0,    41,     0,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    52,    53,    54,
     145,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,    69,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,    74,     0,     0,     0,
       0,    76,     0,     0,     0,   146,     0,    78,     0,   561,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   640,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,     0,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      52,    53,    54,   145,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
      69,     0,     0,     0,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,    71,     0,    72,     0,    73,    74,
       0,     0,     0,     0,    76,     0,     0,     0,   146,     0,
      78,     0,   147,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,   142,   143,     0,   643,     0,    30,     0,
       0,     0,     0,    33,    34,     0,    36,     0,    37,    38,
       0,     0,     0,     0,     0,    40,     0,    41,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    52,    53,    54,   145,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,    69,     0,     0,     0,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,    74,     0,     0,     0,     0,    76,     0,     0,
       0,   146,     0,    78,     0,   147,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     0,     0,   142,   143,     0,   835,
       0,    30,     0,     0,     0,     0,    33,    34,     0,    36,
       0,    37,    38,     0,     0,     0,     0,     0,    40,     0,
      41,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,    52,    53,    54,   145,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,    69,     0,     0,     0,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,    72,     0,    73,    74,     0,     0,     0,     0,
      76,     0,     0,     0,   146,     0,    78,     0,   147,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,   142,
     143,     0,   836,     0,    30,     0,     0,     0,     0,    33,
      34,     0,    36,     0,    37,    38,     0,     0,     0,     0,
       0,    40,     0,    41,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
      53,    54,   145,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,    69,
       0,     0,     0,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,    71,     0,    72,     0,    73,    74,     0,
       0,     0,     0,    76,     0,     0,     0,   146,     0,    78,
       0,   147,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,   142,   143,     0,   838,     0,    30,     0,     0,
       0,     0,    33,    34,     0,    36,     0,    37,    38,     0,
       0,     0,     0,     0,    40,     0,    41,     0,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    52,    53,    54,   145,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,    69,     0,     0,     0,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,    72,     0,
      73,    74,     0,     0,     0,     0,    76,     0,     0,     0,
     146,     0,    78,     0,   147,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    22,
       0,     0,     0,     0,     0,   142,   143,     0,   839,     0,
      30,     0,     0,     0,     0,    33,    34,     0,    36,     0,
      37,    38,     0,     0,     0,     0,     0,    40,     0,    41,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    52,    53,    54,   145,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,    69,     0,     0,     0,     0,
      70,     0,     0,     0,     0,     0,     0,     0,     0,    71,
       0,    72,     0,    73,    74,     0,     0,     0,     0,    76,
       0,     0,     0,   146,     0,    78,     0,   147,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,     0,     0,   142,   143,
       0,   840,     0,    30,     0,     0,     0,     0,    33,    34,
       0,    36,     0,    37,    38,     0,     0,     0,     0,     0,
      40,     0,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,    53,
      54,   145,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,    69,     0,
       0,     0,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,    71,     0,    72,     0,    73,    74,     0,     0,
       0,     0,    76,     0,     0,     0,   146,     0,    78,     0,
     147,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   841,     0,    30,     0,     0,     0,
       0,    33,    34,     0,    36,     0,    37,    38,     0,     0,
       0,     0,     0,    40,     0,    41,     0,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    69,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,    71,     0,    72,     0,    73,
      74,     0,     0,     0,     0,    76,     0,     0,     0,   146,
       0,    78,     0,   147,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,   142,   143,     0,   144,     0,    30,
       0,     0,     0,     0,    33,    34,     0,    36,     0,    37,
      38,     0,     0,     0,     0,     0,    40,     0,    41,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    52,    53,    54,   145,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,    69,    22,     0,     0,     0,     0,
       0,   142,   143,     0,   144,     0,    30,     0,    71,     0,
      72,    33,    34,    74,    36,     0,    37,    38,    76,     0,
       0,     0,   146,    40,    78,    41,   147,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    53,    54,   145,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -323,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,     0,     0,  -323,     0,     0,     0,  -323,     0,     0,
       5,     6,     7,     8,     9,    10,     0,    72,     0,   141,
       0,   201,     0,     0,     0,     0,     0,     0,     0,   146,
       0,    78,  -323,   861,    22,     0,     0,     0,     0,     0,
     142,   143,     0,   144,     0,    30,     0,     0,     0,     0,
      33,    34,     0,    36,     0,    37,    38,   842,     0,     0,
       0,     0,    40,     0,    41,     0,    43,     0,     0,     0,
       0,     0,     0,  -166,     0,     0,  -166,  -166,     0,    51,
      52,    53,    54,   145,     0,  -166,     0,  -166,  -166,     0,
       0,  -166,    61,     0,     0,  -166,     0,  -166,     0,     0,
     268,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,  -166,     0,  -166,   270,  -166,  -166,    67,  -166,
    -166,     0,  -166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    72,     0,  -166,     0,
    -166,     0,     0,  -166,     0,     0,   268,     0,   146,     0,
      78,   269,   561,     0,     0,   271,     0,     0,     0,   272,
       0,   270,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,     0,     0,   268,   273,     0,     0,     0,   269,
    -166,     0,     0,     0,     0,     0,     0,     0,  -166,   270,
     274,   271,   275,   276,   277,   272,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,     0,     0,   293,   294,   295,     0,     0,
     268,   273,     0,     0,     0,   269,   711,     0,     0,   271,
       0,     0,     0,   272,     0,   270,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,   273,
       0,   293,   294,   295,     0,     0,     0,     0,     0,     0,
       0,     0,   765,     0,   274,   271,   275,   276,   277,   272,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   268,   290,   291,   292,     0,   269,   293,
     294,   295,     0,     0,     0,   273,     0,     0,   270,     0,
     914,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   268,
     290,   291,   292,     0,   269,   293,   294,   295,   271,     0,
       0,     0,   272,     0,   270,     0,   951,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,   271,   275,   276,   277,   272,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,     0,   290,   291,   292,   268,     0,   293,   294,
     295,   269,     0,     0,   273,     0,     0,   583,     0,     0,
       0,   270,     0,     0,     0,     0,     0,     0,     0,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,     0,   290,
     291,   292,     0,     0,   293,   294,   295,     0,     0,     0,
       0,   271,     0,   885,     0,   272,     0,     0,     0,     0,
       0,     0,     0,   354,     0,   268,     0,     0,     0,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   273,   355,   356,   357,   358,   359,   360,     0,     0,
     361,   362,   363,   364,   365,   366,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
     271,   293,   294,   295,   272,     0,     0,   319,     0,     0,
       0,     0,   770,     0,   268,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     273,   355,   356,   357,   358,   359,   360,     0,     0,   361,
     362,   363,   364,   365,   366,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,     0,   271,
     293,   294,   295,   272,     0,     0,   319,     0,     0,     0,
       0,   775,     0,   268,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,   273,
     355,   356,   357,   358,   359,   360,     0,     0,   361,   362,
     363,   364,   365,   366,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,     0,   290,   291,   292,     0,   271,   293,
     294,   295,   272,     0,     0,   319,     0,     0,     0,     0,
     898,     0,   268,     0,     0,     0,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   273,   355,
     356,   357,   358,   359,   360,     0,     0,   361,   362,   363,
     364,   365,   366,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,     0,   290,   291,   292,     0,   271,   293,   294,
     295,   272,     0,     0,   319,     0,     0,     0,     0,   899,
       0,   268,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,   273,   355,   356,
     357,   358,   359,   360,     0,     0,   361,   362,   363,   364,
     365,   366,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,     0,   290,   291,   292,     0,   271,   293,   294,   295,
     272,     0,     0,   319,     0,     0,     0,     0,   900,     0,
     268,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,   273,   355,   356,   357,
     358,   359,   360,     0,     0,   361,   362,   363,   364,   365,
     366,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
       0,   290,   291,   292,     0,   271,   293,   294,   295,   272,
       0,     0,   319,     0,     0,     0,     0,   901,     0,   268,
       0,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,   273,   355,   356,   357,   358,
     359,   360,     0,     0,   361,   362,   363,   364,   365,   366,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,     0,   271,   293,   294,   295,   272,     0,
       0,   319,     0,     0,     0,     0,   916,     0,   268,     0,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,   273,   355,   356,   357,   358,   359,
     360,     0,     0,   361,   362,   363,   364,   365,   366,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,     0,   290,
     291,   292,     0,   271,   293,   294,   295,   272,     0,     0,
     319,     0,     0,     0,     0,   917,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   273,   355,   356,   357,   358,   359,   360,
       0,     0,   361,   362,   363,   364,   365,   366,   274,     0,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   268,   290,   291,
     292,     0,   269,   293,   294,   295,     0,     0,     0,   319,
       0,     0,   270,   346,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,   268,     0,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   270,   343,     0,     0,
       0,     0,   271,     0,     0,     0,   272,     0,   344,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   348,     0,
       0,     0,   273,     0,     0,   270,   271,     0,     0,     0,
     272,     0,     0,     0,     0,     0,     0,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   273,   290,   291,   292,
       0,     0,   293,   294,   295,   271,     0,     0,   319,   272,
       0,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     268,   290,   291,   292,     0,   269,   293,   294,   295,     0,
       0,     0,   319,     0,     0,   270,   666,     0,     0,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   268,   286,   287,   288,   289,   269,
     290,     0,   292,     0,     0,   293,   294,   295,     0,   270,
     187,     0,     0,     0,     0,   271,     0,     0,     0,   272,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,     0,     0,     0,   273,     0,     0,     0,   271,
       0,     0,     0,   272,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   273,
     290,   291,   292,     0,     0,   293,   294,   295,     0,     0,
       0,   319,     0,     0,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   268,   290,   291,   292,     0,   269,   293,
     294,   295,     0,     0,     0,   319,     0,     0,   270,   660,
       0,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,     0,     0,   268,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,     0,     0,   271,     0,
       0,     0,   272,   310,   311,  -414,     0,     0,  -439,     0,
    -439,     0,     0,     0,     0,     0,   268,     0,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,   273,     0,
       0,   270,   271,     0,     0,     0,   272,     0,     0,     0,
       0,     0,     0,   274,   678,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   273,   290,   291,   292,     0,     0,   293,   294,
     295,   271,     0,     0,   319,   272,     0,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,     0,   290,   291,   292,
     268,   273,   293,   294,   295,   269,     0,     0,   319,     0,
       0,     0,     0,     0,     0,   270,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
       0,   293,   294,   295,     0,     0,   268,   394,     0,     0,
       0,   269,     0,     0,     0,   271,     0,     0,     0,   272,
       0,   270,     0,     0,     0,     0,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,   352,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   271,   275,   276,   277,   272,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,     0,   268,   293,   294,   295,     0,   269,
    -415,   273,     0,     0,     0,     0,     0,     0,     0,   270,
       0,     0,     0,     0,     0,     0,   274,     0,   275,   276,
     277,   325,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,     0,   290,   291,   292,     0,
     353,   293,   294,   295,     0,     0,     0,     0,     0,   271,
       0,     0,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   268,     0,     0,     0,   135,   269,     0,     0,   273,
       0,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,   407,     0,     0,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   408,   290,   291,   292,   268,     0,   293,
     294,   295,   269,     0,     0,     0,   271,     0,     0,     0,
     272,     0,   270,   506,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   271,   275,   276,   277,   272,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
       0,   290,   291,   292,     0,   268,   293,   294,   295,     0,
     269,     0,   273,     0,     0,     0,     0,     0,     0,     0,
     270,   508,     0,     0,     0,     0,     0,   274,     0,   275,
     276,   277,   509,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,     0,   290,   291,   292,
       0,   268,   293,   294,   295,     0,   269,     0,     0,     0,
     271,     0,     0,     0,   272,     0,   270,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   268,
     273,     0,     0,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,   274,   271,   275,   276,   277,
     272,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,     0,     0,
     293,   294,   295,     0,     0,   268,   273,     0,     0,     0,
     269,     0,     0,     0,   271,     0,     0,     0,   272,     0,
     270,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
       0,   290,   291,   292,   273,   398,   293,   294,   295,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
     271,   275,   276,   277,   272,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,     0,   290,
     291,   292,   268,   400,   293,   294,   295,   269,     0,     0,
     273,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,     0,     0,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,   268,   402,
     293,   294,   295,   269,     0,     0,     0,   271,     0,     0,
       0,   272,     0,   270,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   268,   273,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,   274,   271,   275,   276,   277,   272,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,     0,   290,   291,   292,     0,   410,   293,   294,   295,
       0,     0,   425,   273,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   272,     0,     0,   274,     0,
     275,   276,   277,   510,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   268,   290,   291,
     292,   273,   269,   293,   294,   295,     0,     0,     0,     0,
       0,     0,   270,     0,     0,     0,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   268,   290,   291,   292,     0,
     269,   293,   294,   295,     0,     0,     0,     0,     0,     0,
     270,     0,   271,     0,     0,     0,   272,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   135,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,   272,     0,     0,   274,     0,   275,
     276,   277,     0,   278,   279,   280,     0,   281,   282,   283,
     284,   285,   286,   287,   288,   289,     0,   290,   291,   292,
     273,     0,   293,   294,   295,   268,   673,     0,     0,     0,
     269,     0,     0,     0,     0,   274,     0,   275,   276,   277,
     270,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,     0,   650,
     293,   294,   295,   268,     0,     0,     0,     0,   269,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270,     0,
     271,   676,     0,     0,   272,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   268,     0,     0,     0,     0,   269,     0,     0,
     273,     0,     0,     0,     0,     0,     0,   270,   271,     0,
       0,     0,   272,     0,     0,   274,     0,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,   273,     0,
     293,   294,   295,     0,     0,     0,     0,   271,     0,     0,
       0,   272,     0,   274,     0,   275,   276,   277,     0,   278,
     279,   280,     0,   281,   282,   283,   284,   285,   286,   287,
     288,   289,     0,   290,   291,   292,   714,   273,   293,   294,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   268,   290,   291,   292,     0,   269,   293,   294,   295,
       0,     0,     0,     0,     0,     0,   270,   760,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   268,     0,     0,     0,     0,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     270,   763,     0,     0,     0,     0,   271,     0,     0,     0,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   273,     0,     0,     0,
     271,     0,     0,     0,   272,     0,     0,     0,     0,     0,
       0,   274,     0,   275,   276,   277,     0,   278,   279,   280,
       0,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     273,   290,   291,   292,     0,   268,   293,   294,   295,     0,
     269,     0,     0,     0,     0,   274,     0,   275,   276,   277,
     270,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   290,   291,   292,     0,   268,
     293,   294,   295,     0,   269,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   270,     0,     0,     0,     0,     0,
     271,     0,     0,     0,   272,     0,     0,     0,     0,     0,
       0,     0,   834,     0,     0,     0,     0,     0,   268,   851,
       0,     0,     0,   269,     0,     0,     0,     0,     0,     0,
     273,     0,     0,   270,   271,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,   274,   837,   275,   276,   277,
       0,   278,   279,   280,     0,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   273,   290,   291,   292,     0,     0,
     293,   294,   295,   271,     0,     0,     0,   272,     0,   274,
       0,   275,   276,   277,     0,   278,   279,   280,     0,   281,
     282,   283,   284,   285,   286,   287,   288,   289,     0,   290,
     291,   292,   268,   273,   293,   294,   295,   269,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,   274,     0,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,     0,   290,   291,
     292,     0,     0,   293,   294,   295,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,   272,     0,     0,     0,     0,   268,     0,     0,   923,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   271,   290,   291,   292,   272,     0,   293,   294,   295,
     268,     0,     0,   924,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   271,   290,   291,   292,   272,
       0,   293,   294,   295,   268,     0,     0,   925,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   271,
     290,   291,   292,   272,     0,   293,   294,   295,   268,     0,
       0,   926,     0,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,   273,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,     0,   275,   276,   277,     0,
     278,   279,   280,     0,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   271,   290,   291,   292,   272,     0,   293,
     294,   295,   268,     0,     0,   927,     0,   269,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270,     0,     0,
       0,     0,     0,   273,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,     0,
     275,   276,   277,     0,   278,   279,   280,     0,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   271,   290,   291,
     292,   272,     0,   293,   294,   295,   268,     0,     0,   928,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   270,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,     0,   275,   276,   277,     0,   278,   279,
     280,     0,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   271,   290,   291,   292,   272,     0,   293,   294,   295,
     268,     0,     0,     0,     0,   269,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,     0,   275,   276,
     277,     0,   278,   279,   280,     0,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   638,   290,   291,   292,   272,
       0,   293,   294,   295,     0,   270,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,   271,   278,   279,   280,   272,
     281,   282,   283,   284,   285,   286,   287,   288,   289,     0,
     290,   291,   292,     0,     0,   293,   294,   295,     0,     0,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,     0,   275,   276,   277,     0,   278,   279,   280,     0,
     281,   282,   283,   284,     0,   286,   287,   288,   289,     0,
     290,     0,   292,     0,     0,   293,   294,   295
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-809)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,     2,    44,    45,   403,   379,   371,   534,   374,    11,
     157,   609,   691,   752,    16,    15,   250,    19,    20,    21,
      24,   168,   554,     1,    26,    27,    28,    29,   316,   403,
       1,    47,   717,    35,    36,    37,     3,    39,    40,    41,
     180,   262,   263,   264,   265,    47,    62,    49,    50,    51,
      52,   717,     3,    55,    56,    57,    58,   197,     3,    61,
      62,    63,    64,    79,    91,    67,     3,     3,    70,    71,
      72,    26,    74,    26,     3,    77,   216,    79,    26,    81,
     869,   352,    53,     3,    68,    21,   115,     1,     1,    96,
      68,    37,    26,    53,    26,    68,    37,   905,   115,   134,
      17,    37,   134,     0,    40,   134,    37,    27,   143,   636,
     145,    89,   119,    37,   793,    66,    83,    88,    54,   146,
      56,    37,   115,   115,    37,   134,    78,     3,    88,   146,
      66,   147,   940,   142,   136,    37,   114,   408,   823,   141,
     142,   143,   144,   145,   146,   147,    82,    64,   887,   888,
      91,   144,   144,   153,    68,   944,   688,   141,   843,    91,
     115,   134,    82,   141,    66,    79,   115,    84,   141,   115,
     134,   142,   312,   313,   115,    89,   143,   843,   142,   181,
      91,    21,   142,    96,   115,   187,   141,   553,   141,    21,
     115,   115,   143,   141,   198,   144,   141,    82,   143,   115,
     114,   114,   115,   205,   206,   146,   143,   143,    25,   141,
      96,   163,    96,   434,   143,   146,    56,   115,    41,   144,
     134,   134,   146,    48,    56,   904,    66,   141,   180,   115,
     146,   115,   830,   119,    66,   119,   115,    13,    61,   143,
     242,   242,    18,   245,   142,   197,   534,    87,    65,   143,
      82,   940,    69,    78,     7,   782,   396,     1,   144,     3,
     144,    21,   951,   142,   216,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   143,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,    75,   827,    56,   114,   532,   533,
     117,   143,   115,   115,   121,     3,    66,   115,   143,     7,
     145,     2,   314,   315,   316,   680,   682,   319,   115,    62,
      11,   115,    82,   325,   115,   341,   140,   329,    19,   331,
      21,   144,   144,   143,   134,   145,   144,   271,   272,   341,
     134,   343,   344,    41,   346,   347,   115,   144,   636,   351,
     352,   115,   354,   144,   588,   589,   115,    37,     3,   115,
     312,   313,     7,    61,    55,   134,   368,   134,    65,   143,
     134,   145,    69,   115,   115,   134,   115,   379,   134,   381,
     314,   315,    57,    58,   134,     3,    77,   389,    68,     7,
      81,   325,   134,   134,    65,   134,   134,   134,    69,    79,
     799,   403,   134,    21,   114,   407,   408,   117,   134,    89,
     344,   121,   414,   347,   430,    20,    96,   134,   792,   421,
     422,   423,   143,   425,   426,   799,    31,    32,   430,   417,
     141,   419,   579,   134,    39,   115,    41,   389,    56,    90,
     143,   143,    47,   117,   396,   136,   134,   143,    66,    96,
     134,     7,    47,   114,   134,   141,    61,    62,   134,    96,
     115,   141,   414,   144,    82,   417,   831,   419,   134,    87,
     422,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   421,    37,   143,
     181,    28,   144,   115,   782,   144,   187,    96,   144,    16,
     144,   115,    21,   146,   506,   507,   508,   509,   510,   511,
     512,   141,   144,   513,   514,   134,    28,   134,    37,   134,
      25,    40,   524,   134,   526,   134,   134,    20,   530,   115,
     134,   905,   534,   134,   134,    54,   134,    56,    31,    32,
     914,   134,   544,   134,   134,   561,    39,    66,    41,   134,
     134,   242,   134,   142,   556,   557,   558,   559,   560,   561,
      65,   134,   134,    82,    69,   146,   940,   519,    61,   521,
     134,   134,   134,   507,   144,   509,   144,   951,    96,   115,
     141,    66,   142,    17,    26,    26,    20,     2,   146,   141,
      28,   593,   417,   114,   419,    29,    11,    31,    32,    66,
     146,    35,   146,   114,    19,    39,    21,    41,   146,   114,
     142,   116,   117,   117,   117,   143,   121,    28,   144,    89,
     144,    79,   142,   128,    58,   715,    60,    61,   319,    63,
      64,   408,   137,   702,   636,   407,   638,   639,   640,    25,
      55,   643,    57,    58,   224,   523,   169,   768,    82,   720,
      84,   830,   343,   831,   680,   346,   658,   723,   660,   944,
      50,   663,    77,   354,   666,   904,    81,   919,    28,   671,
     382,   673,   854,   675,   676,   521,   678,    -1,    -1,    65,
       3,    -1,   698,    69,     7,    -1,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   519,   697,   698,    -1,    21,    -1,
     134,    -1,    -1,    -1,   638,   639,    -1,   709,   142,   711,
      -1,    -1,   714,   715,    -1,    -1,    -1,   719,    -1,    -1,
      -1,   136,    -1,    -1,   110,    -1,   112,   113,   114,    -1,
     116,   117,   423,    56,    -1,   121,    -1,    -1,    -1,   564,
      -1,    -1,   128,    66,    -1,    -1,   748,    -1,    -1,   135,
     136,   137,    -1,    -1,    -1,    -1,    -1,   759,   760,    82,
     762,   763,   764,   765,    87,    -1,   181,    -1,   770,   771,
      -1,   773,   187,   775,   776,    -1,    -1,   779,   820,   821,
     782,    -1,   172,   173,   174,    -1,    -1,   789,    -1,    -1,
     792,    -1,    -1,    -1,    -1,    -1,   798,   799,   800,    -1,
     190,    -1,   804,   193,   806,    -1,    -1,   809,   810,    -1,
     501,   813,    -1,    -1,    -1,   817,   817,    -1,    -1,    -1,
      -1,    -1,   212,    -1,    -1,    -1,    -1,   242,    -1,    -1,
      37,    -1,   834,   835,   836,   837,   838,   839,   840,   841,
     792,    -1,   232,    -1,    -1,   861,    -1,    -1,    -1,   851,
     852,    -1,    -1,   544,   856,   857,   858,   859,   860,   861,
      -1,    68,    -1,   865,    -1,    -1,   868,    25,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    11,    -1,    -1,    -1,    96,
      -1,    -1,    -1,    19,    -1,    21,   898,   899,   900,   901,
      -1,    -1,   593,   905,   319,    -1,    -1,    65,   115,    -1,
      -1,    69,   914,   865,   916,   917,    -1,    -1,   609,    -1,
      -1,   923,   924,   925,   926,   927,   928,   134,   343,    55,
      -1,   346,    -1,    -1,   141,    -1,   326,   939,   940,   354,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   638,   639,   951,
      -1,    77,   110,   905,    -1,    81,   114,    -1,   116,   117,
     350,    -1,    -1,   121,    -1,    -1,    -1,   658,    -1,   660,
     128,    -1,   663,     1,    -1,   666,    -1,   135,   136,   137,
      -1,    -1,    25,    -1,   675,    37,    -1,   678,   940,    17,
      -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    -1,   951,
      -1,    29,    -1,    31,    32,    -1,   697,    35,   423,    -1,
     136,    39,     2,    41,    -1,    -1,    68,    -1,   709,    -1,
     711,    11,    65,    -1,   715,    -1,    69,    79,    56,    19,
      58,    21,    60,    61,    -1,    63,    64,    89,    66,    20,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    82,   181,    84,    -1,    39,    87,
      41,   187,    -1,   115,    -1,    55,    47,   110,    -1,    -1,
     113,   114,    -1,   116,   117,    -1,    -1,    -1,   121,   770,
      61,    62,   134,    -1,   775,   128,    -1,    77,     2,   141,
      -1,    81,   135,   136,   137,    -1,    -1,    11,   789,    -1,
      -1,    -1,    -1,    -1,    -1,    19,   134,    21,    -1,   800,
      -1,    -1,    -1,    -1,   142,    -1,   242,    -1,   809,    -1,
      -1,    -1,   813,    -1,    -1,    -1,   817,    -1,    -1,   544,
      -1,    -1,    -1,    17,    -1,    -1,    20,    -1,   528,   529,
      -1,    55,   532,   533,    25,    29,   136,    31,    32,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    41,    -1,    -1,
      -1,   852,    -1,    77,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    60,    61,   593,    63,
      64,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    -1,
      -1,   181,    -1,   319,    -1,    -1,    -1,   187,    82,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,   898,   899,   900,
     901,    -1,    -1,    -1,    -1,    -1,    -1,   343,    -1,    -1,
     346,    -1,   136,    -1,    -1,   916,   917,    -1,   354,    -1,
      -1,    -1,    -1,   114,    -1,   116,   117,    -1,    -1,    -1,
     121,    -1,    -1,   658,    -1,   660,    -1,   128,   663,    -1,
     134,   666,   242,    -1,   135,   136,   137,    -1,   648,   649,
     675,    -1,    -1,   678,    -1,   655,    -1,   181,    -1,    -1,
      -1,    17,    -1,   187,    20,   665,    -1,    -1,   668,    -1,
      -1,    -1,   697,    29,    -1,    31,    32,    -1,    -1,    35,
      -1,    -1,    -1,    39,   709,    41,   711,   423,    -1,    -1,
     715,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    60,    61,    -1,    63,    64,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   242,   319,
      -1,    -1,    -1,    -1,    -1,    50,    82,    -1,    84,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,   343,    -1,   770,   346,    11,    -1,    25,
     775,    -1,    -1,    -1,   354,    19,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,   789,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   800,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,   809,    -1,   142,    -1,   813,    65,
      -1,    55,   817,    69,    -1,   319,    -1,    -1,    -1,    -1,
      -1,    -1,   802,    -1,    -1,    -1,    -1,    -1,   544,     2,
      -1,    -1,    -1,    77,    -1,   140,    -1,    81,    11,   343,
      -1,    -1,   346,   423,    -1,    -1,    19,   852,    21,    -1,
     354,    -1,    -1,    -1,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,    -1,   121,    -1,   172,   173,   174,
      -1,   127,   128,    -1,   179,    -1,   132,   593,    -1,   135,
     136,   137,    55,    -1,    -1,   190,    -1,    -1,   193,    -1,
      -1,    -1,   136,   898,   899,   900,   901,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,   212,    81,    -1,
      -1,   916,   917,    -1,    -1,    -1,    -1,    -1,    -1,   423,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   232,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,   658,   187,   660,    -1,    -1,   663,    -1,    -1,
     666,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   675,
      -1,    -1,   678,   136,   544,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   697,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   709,    -1,   711,    -1,    -1,   242,   715,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,   593,   187,    -1,    -1,    -1,    -1,    -1,
      -1,   326,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
     544,    -1,    -1,    -1,    -1,   350,    19,    -1,    21,    -1,
      -1,    -1,    -1,    -1,   770,    -1,    -1,    -1,    -1,   775,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   242,
      -1,    -1,    -1,   789,    -1,   319,    -1,    -1,   658,    -1,
     660,    -1,    55,   663,   800,    -1,   666,    -1,    -1,   593,
      -1,    -1,    -1,   809,    -1,   675,    -1,   813,   678,   343,
      -1,   817,   346,    -1,    77,    -1,    -1,    -1,    81,    -1,
     354,    -1,    -1,    -1,    -1,    -1,    -1,   697,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   709,
      -1,   711,    -1,    -1,    -1,   715,   852,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   319,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   658,    -1,   660,    -1,    -1,   663,
      -1,    -1,   666,   136,    -1,    -1,    -1,    -1,    -1,    -1,
     343,   675,    -1,   346,   678,    -1,    -1,     2,    -1,   423,
      -1,   354,   898,   899,   900,   901,    11,    -1,    -1,    -1,
     770,    25,    -1,   697,    19,   775,    21,    -1,    -1,    -1,
     916,   917,    -1,    -1,    -1,   709,    -1,   711,   181,   789,
      -1,   715,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
     800,    -1,    -1,   528,   529,    -1,    -1,   532,   533,   809,
      55,    65,    -1,   813,    -1,    69,    -1,   817,    -1,    -1,
     545,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     423,    -1,    77,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   770,    -1,    -1,   242,
     575,   775,   852,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,   789,    -1,   121,   122,   123,
      -1,   125,   126,   127,   128,    -1,   800,    -1,   132,    -1,
     544,   135,   136,   137,    -1,   809,    -1,    -1,    -1,   813,
      -1,   136,    -1,   817,    -1,    -1,    -1,    -1,   898,   899,
     900,   901,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   916,   917,    -1,    -1,
      -1,    -1,    -1,   648,   649,    -1,   319,    -1,   852,   593,
     655,    -1,   657,    -1,    -1,    -1,   181,   662,    -1,    -1,
     665,    -1,   187,   668,    -1,    -1,    -1,    -1,    -1,    -1,
     343,   544,    -1,   346,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   898,   899,   900,   901,    -1,    -1,
     705,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,   916,   917,   658,    12,   660,   242,    -1,   663,
     593,    -1,   666,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,   675,    -1,    -1,   678,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
     423,    48,    49,   697,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,   709,    -1,   711,    -1,    -1,
      -1,   715,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,   658,    -1,   660,    85,    -1,
     663,    -1,    -1,   666,   319,    -1,    -1,   802,    -1,    -1,
      -1,    -1,   675,    -1,    -1,   678,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,   343,    -1,
      -1,   346,    -1,    -1,   697,    -1,   770,    -1,    -1,    -1,
      -1,   775,   129,    -1,    -1,    -1,   709,    -1,   711,    -1,
     845,   846,   715,   848,   849,   789,   143,    -1,   145,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   800,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   869,   809,    -1,    -1,    -1,   813,
     875,   544,   877,   817,   879,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,    -1,   770,   423,    -1,
      -1,    -1,   775,    -1,    -1,    -1,    -1,    13,   852,    -1,
      -1,    -1,    18,    -1,    -1,    -1,   789,    -1,    24,    25,
     593,    -1,    -1,    -1,   138,   139,    32,   800,    34,   143,
      -1,   145,    -1,    -1,    -1,    -1,   809,    -1,    -1,   944,
     813,    -1,    -1,    -1,   817,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,   898,   899,   900,   901,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   916,   917,    -1,    -1,    -1,    -1,    -1,   852,
      -1,    -1,    -1,    -1,    -1,   658,    -1,   660,    -1,    -1,
     663,    -1,    -1,   666,    -1,    -1,   102,    25,   104,    -1,
      -1,    -1,   675,    -1,    -1,   678,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   544,
      -1,    -1,    -1,    -1,   697,   898,   899,   900,   901,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   709,    65,   711,    -1,
      -1,    69,   715,   916,   917,    -1,    -1,    -1,    -1,    -1,
      -1,   157,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    -1,   168,    -1,   170,    -1,    -1,    -1,   593,    -1,
      -1,    -1,    -1,    -1,   180,    -1,    -1,   183,    -1,    -1,
      -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,   117,
      -1,   197,   198,   121,    -1,   201,    -1,   770,    -1,   127,
     128,    -1,   775,    -1,   132,    -1,    -1,   135,   136,   137,
     216,    -1,    -1,    -1,    -1,    -1,   789,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   800,    -1,    -1,
      -1,    -1,    -1,   658,    -1,   660,   809,    -1,   663,    -1,
     813,   666,    -1,    -1,   817,    -1,    -1,    -1,    -1,    -1,
     675,    -1,    -1,   678,    -1,    -1,   262,   263,   264,   265,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   697,   279,    -1,    -1,    -1,    -1,    -1,   852,
      -1,    -1,    -1,    -1,   709,    -1,   711,    -1,    -1,    -1,
     715,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,   312,   313,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,   898,   899,   900,   901,    -1,
      95,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   916,   917,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,   789,   130,   131,   132,   374,    -1,
     135,   136,   137,    -1,    -1,   800,   382,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   809,    -1,    -1,    -1,   813,    -1,
     396,    -1,   817,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   429,    -1,   116,   432,   852,   434,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
      -1,   132,    -1,    -1,   135,   136,   137,   138,    -1,   140,
       0,     1,   143,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,   521,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    92,   549,    -1,    95,     3,   553,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,   575,
      -1,    -1,    -1,   579,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,    -1,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   609,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,    -1,   116,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,    -1,   132,   682,    -1,   135,   136,
     137,   138,    -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   702,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,   720,    10,    -1,    12,    13,    14,
      15,   727,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    -1,    80,    81,    82,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   819,    -1,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   830,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,   134,
     135,   136,   137,    -1,    -1,    -1,   141,   142,   143,     1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,   133,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,
     142,   143,     1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      -1,    80,    81,    82,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,
      -1,    -1,   141,   142,   143,     1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,
      -1,   137,    -1,    -1,    -1,   141,   142,   143,     1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    -1,    80,    81,    82,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,   142,
     143,     1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,
      -1,   141,   142,   143,     1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    -1,    80,    81,    82,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,     1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    26,    27,    -1,    29,    -1,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    51,    -1,    -1,
      -1,    55,    56,    57,    -1,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    -1,    70,    71,    72,    73,
      74,    75,    76,    -1,    78,    -1,    80,    81,    82,    -1,
      84,    85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,
     134,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
       1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    27,    -1,    29,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      51,    -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,
      61,    62,    63,    64,    -1,    66,    67,    68,    -1,    70,
      71,    72,    73,    74,    75,    76,    -1,    78,    -1,    80,
      81,    82,    -1,    84,    85,    86,    87,    -1,    -1,    90,
      -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,     1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    26,    27,
      -1,    29,    -1,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    60,    61,    62,    63,    64,    -1,    66,    67,
      68,    -1,    70,    71,    72,    73,    74,    75,    76,    -1,
      78,    -1,    80,    81,    82,    -1,    84,    85,    86,    87,
      -1,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,     1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    26,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,
      55,    56,    57,    -1,    59,    60,    61,    62,    63,    64,
      -1,    66,    67,    68,    -1,    70,    71,    72,    73,    74,
      75,    76,    -1,    78,    -1,    80,    81,    82,    -1,    84,
      85,    86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,   134,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,     1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    26,    27,    -1,    29,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    -1,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    68,    -1,    70,    71,
      72,    73,    74,    75,    76,    -1,    78,    -1,    80,    81,
      82,    -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,
      92,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,     1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    26,    27,    -1,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    51,    -1,    -1,    -1,    55,    56,    57,    -1,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      -1,    70,    71,    72,    73,    74,    75,    76,    -1,    78,
      -1,    80,    81,    82,    -1,    84,    85,    86,    87,    -1,
      -1,    90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,     1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    27,    -1,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,
      56,    57,    -1,    59,    60,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    71,    72,    73,    74,    75,
      76,    -1,    78,    -1,    80,    81,    82,    -1,    84,    85,
      86,    87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,     1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    27,    -1,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    51,    -1,
      -1,    -1,    55,    56,    57,    -1,    59,    60,    61,    62,
      63,    64,    -1,    66,    67,    68,    -1,    70,    71,    72,
      73,    74,    75,    76,    -1,    78,    -1,    80,    81,    82,
      -1,    84,    85,    86,    87,    -1,    -1,    90,    -1,    92,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,   134,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,     1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    26,    27,    -1,    29,
      -1,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    51,    -1,    -1,    -1,    55,    56,    57,    -1,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    -1,
      70,    71,    72,    73,    74,    75,    76,    -1,    78,    -1,
      80,    81,    82,    -1,    84,    85,    86,    87,    -1,    -1,
      90,    -1,    92,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,   134,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,     1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    26,
      27,    -1,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    51,    -1,    -1,    -1,    55,    56,
      57,    -1,    59,    60,    61,    62,    63,    64,    -1,    66,
      67,    68,    -1,    70,    71,    72,    73,    74,    75,    76,
      -1,    78,    -1,    80,    81,    82,    -1,    84,    85,    86,
      87,    -1,    -1,    90,    -1,    92,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,     1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
      -1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,   146,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,   146,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,    -1,   145,   146,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    37,    38,
      -1,    40,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    80,    81,    -1,    83,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,   133,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    80,    81,    -1,
      83,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
     133,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,   144,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      80,    81,    -1,    83,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,   144,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,   134,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
      -1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    50,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    32,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,    -1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,   133,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   141,   142,   143,    -1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
     133,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,    -1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,    -1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,   133,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
      -1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,   133,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,    -1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,
     129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,
      46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,
     113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,
      -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,
     143,    -1,   145,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,
      -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,
      -1,   131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,
      -1,   141,    -1,   143,    -1,   145,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      57,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,
      44,    -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,
      -1,   145,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,   112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,
     131,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,
     141,    -1,   143,    -1,   145,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   129,    -1,   131,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,    -1,   141,    -1,   143,    -1,   145,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    -1,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    48,    49,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    57,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,    -1,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    -1,   129,    -1,   131,   132,    -1,    -1,
      -1,    -1,   137,    -1,    -1,    -1,   141,    -1,   143,    -1,
     145,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    48,    49,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,   113,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,   131,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,   141,
      -1,   143,    -1,   145,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,    48,
      49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,   112,   113,    27,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    38,    -1,   127,    -1,
     129,    43,    44,   132,    46,    -1,    48,    49,   137,    -1,
      -1,    -1,   141,    55,   143,    57,   145,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,   115,    -1,    -1,    -1,   119,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,   129,    -1,    12,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,   145,    27,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    -1,    48,    49,     1,    -1,    -1,
      -1,    -1,    55,    -1,    57,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    -1,    -1,    20,    21,    -1,    72,
      73,    74,    75,    76,    -1,    29,    -1,    31,    32,    -1,
      -1,    35,    85,    -1,    -1,    39,    -1,    41,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    25,    60,    61,   111,    63,
      64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    82,    -1,
      84,    -1,    -1,    87,    -1,    -1,    10,    -1,   141,    -1,
     143,    15,   145,    -1,    -1,    65,    -1,    -1,    -1,    69,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    10,    95,    -1,    -1,    -1,    15,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    25,
     110,    65,   112,   113,   114,    69,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,
      10,    95,    -1,    -1,    -1,    15,   146,    -1,    -1,    65,
      -1,    -1,    -1,    69,    -1,    25,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    95,
      -1,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   146,    -1,   110,    65,   112,   113,   114,    69,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    10,   130,   131,   132,    -1,    15,   135,
     136,   137,    -1,    -1,    -1,    95,    -1,    -1,    25,    -1,
     146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    10,
     130,   131,   132,    -1,    15,   135,   136,   137,    65,    -1,
      -1,    -1,    69,    -1,    25,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    65,   112,   113,   114,    69,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,    10,    -1,   135,   136,
     137,    15,    -1,    -1,    95,    -1,    -1,   144,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,    -1,
      -1,    65,    -1,   144,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    95,    96,    97,    98,    99,   100,   101,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
      65,   135,   136,   137,    69,    -1,    -1,   141,    -1,    -1,
      -1,    -1,    77,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      95,    96,    97,    98,    99,   100,   101,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    -1,    65,
     135,   136,   137,    69,    -1,    -1,   141,    -1,    -1,    -1,
      -1,    77,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    95,
      96,    97,    98,    99,   100,   101,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,    -1,    65,   135,
     136,   137,    69,    -1,    -1,   141,    -1,    -1,    -1,    -1,
      77,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    95,    96,
      97,    98,    99,   100,   101,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,    -1,    65,   135,   136,
     137,    69,    -1,    -1,   141,    -1,    -1,    -1,    -1,    77,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    95,    96,    97,
      98,    99,   100,   101,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,    -1,    65,   135,   136,   137,
      69,    -1,    -1,   141,    -1,    -1,    -1,    -1,    77,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,    -1,    65,   135,   136,   137,    69,
      -1,    -1,   141,    -1,    -1,    -1,    -1,    77,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    95,    96,    97,    98,    99,
     100,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    -1,    65,   135,   136,   137,    69,    -1,
      -1,   141,    -1,    -1,    -1,    -1,    77,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    95,    96,    97,    98,    99,   100,
     101,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,    -1,    65,   135,   136,   137,    69,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    10,   130,   131,
     132,    -1,    15,   135,   136,   137,    -1,    -1,    -1,   141,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    69,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    25,    65,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    95,   130,   131,   132,
      -1,    -1,   135,   136,   137,    65,    -1,    -1,   141,    69,
      -1,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      10,   130,   131,   132,    -1,    15,   135,   136,   137,    -1,
      -1,    -1,   141,    -1,    -1,    25,    26,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,    10,   125,   126,   127,   128,    15,
     130,    -1,   132,    -1,    -1,   135,   136,   137,    -1,    25,
      26,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    -1,    -1,    -1,    95,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    95,
     130,   131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,
      -1,   141,    -1,    -1,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    10,   130,   131,   132,    -1,    15,   135,
     136,   137,    -1,    -1,    -1,   141,    -1,    -1,    25,    26,
      -1,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    69,   138,   139,   140,    -1,    -1,   143,    -1,
     145,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      -1,    25,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    77,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    95,   130,   131,   132,    -1,    -1,   135,   136,
     137,    65,    -1,    -1,   141,    69,    -1,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
      10,    95,   135,   136,   137,    15,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
      -1,   135,   136,   137,    -1,    -1,    10,   141,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,
      -1,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    65,   112,   113,   114,    69,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    -1,    10,   135,   136,   137,    -1,    15,
     140,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    37,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    91,    15,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    30,    -1,    -1,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    52,   130,   131,   132,    10,    -1,   135,
     136,   137,    15,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    65,   112,   113,   114,    69,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,    -1,    10,   135,   136,   137,    -1,
      15,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
     113,   114,    37,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
      -1,    10,   135,   136,   137,    -1,    15,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      95,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   110,    65,   112,   113,   114,
      69,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    -1,    -1,
     135,   136,   137,    -1,    -1,    10,    95,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    65,    -1,    -1,    -1,    69,    -1,
      25,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,    95,   134,   135,   136,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      65,   112,   113,   114,    69,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,    10,   134,   135,   136,   137,    15,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    10,   134,
     135,   136,   137,    15,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    95,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,   110,    65,   112,   113,   114,    69,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,    -1,   134,   135,   136,   137,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    69,    -1,    -1,   110,    -1,
     112,   113,   114,    77,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    10,   130,   131,
     132,    95,    15,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    10,   130,   131,   132,    -1,
      15,   135,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
      95,    -1,   135,   136,   137,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,
      25,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,   137,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      65,    28,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    25,    65,    -1,
      -1,    -1,    69,    -1,    -1,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    95,    -1,
     135,   136,   137,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,    94,    95,   135,   136,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    10,   130,   131,   132,    -1,    15,   135,   136,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      95,   130,   131,   132,    -1,    10,   135,   136,   137,    -1,
      15,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,
      25,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,    -1,    10,
     135,   136,   137,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    25,    65,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    77,   112,   113,   114,
      -1,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,    95,   130,   131,   132,    -1,    -1,
     135,   136,   137,    65,    -1,    -1,    -1,    69,    -1,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,    10,    95,   135,   136,   137,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,    -1,    -1,   135,   136,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    10,    -1,    -1,    77,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    65,   130,   131,   132,    69,    -1,   135,   136,   137,
      10,    -1,    -1,    77,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    65,   130,   131,   132,    69,
      -1,   135,   136,   137,    10,    -1,    -1,    77,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    65,
     130,   131,   132,    69,    -1,   135,   136,   137,    10,    -1,
      -1,    77,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,   112,   113,   114,    -1,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,    65,   130,   131,   132,    69,    -1,   135,
     136,   137,    10,    -1,    -1,    77,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
     112,   113,   114,    -1,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    65,   130,   131,
     132,    69,    -1,   135,   136,   137,    10,    -1,    -1,    77,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,   113,   114,    -1,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    65,   130,   131,   132,    69,    -1,   135,   136,   137,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,   113,
     114,    -1,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,    65,   130,   131,   132,    69,
      -1,   135,   136,   137,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    65,   116,   117,   118,    69,
     120,   121,   122,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,    -1,    -1,   135,   136,   137,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,   112,   113,   114,    -1,   116,   117,   118,    -1,
     120,   121,   122,   123,    -1,   125,   126,   127,   128,    -1,
     130,    -1,   132,    -1,    -1,   135,   136,   137
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   151,   152,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    17,    18,    19,    20,    22,    23,
      24,    26,    27,    29,    31,    32,    33,    34,    35,    36,
      38,    39,    42,    43,    44,    45,    46,    48,    49,    51,
      55,    57,    58,    59,    60,    63,    64,    67,    68,    70,
      71,    72,    73,    74,    75,    76,    78,    80,    81,    82,
      84,    85,    86,    90,    92,    95,   110,   111,   112,   113,
     118,   127,   129,   131,   132,   134,   137,   141,   143,   145,
     153,   154,   155,   156,   157,   159,   164,   165,   166,   168,
     171,   172,   173,   174,   175,   176,   178,   179,   180,   184,
     185,   188,   189,   192,   193,   196,   199,   200,   223,   226,
     227,   245,   246,   247,   248,   249,   250,   257,   258,   259,
     260,   262,   263,   264,   265,   266,   267,   269,   270,   271,
     272,   273,   134,   155,   260,    91,   251,   252,   167,   168,
     251,    12,    33,    34,    36,    76,   141,   145,   196,   245,
     249,   258,   259,   260,   262,   264,   265,    82,   167,   155,
     239,   260,   155,   143,     7,   167,   169,     9,    82,   169,
      56,    93,   177,   260,   260,    20,    32,   226,   260,   260,
     143,   168,   197,   143,   168,   228,   229,    26,   159,   170,
     260,   260,   260,   260,   260,   260,     7,   143,    32,   173,
     173,   133,   219,   238,   260,    80,    81,   134,   260,   261,
     260,   170,   260,   260,   260,    75,   143,   155,   260,   260,
     159,   166,   260,   262,   159,   166,   260,   168,   224,   260,
     238,   260,   260,   260,   260,   260,   260,   260,   260,     1,
     142,   153,   160,   238,    83,   119,   219,   240,   241,   261,
     238,   260,   268,    58,   155,    62,   158,   168,   168,    41,
      61,   214,    21,    56,    66,    87,   134,   140,    10,    15,
      25,    65,    69,    95,   110,   112,   113,   114,   116,   117,
     118,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     130,   131,   132,   135,   136,   137,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     138,   139,   143,   145,    65,    69,   143,   155,   134,   141,
     159,   260,   260,   260,   238,    37,   251,   224,   134,   115,
     134,    90,   168,   219,   242,   243,   244,   261,   224,   189,
     168,   143,   170,    26,    37,   170,    26,    37,    91,   170,
     254,    30,    52,   134,    77,    96,    97,    98,    99,   100,
     101,   104,   105,   106,   107,   108,   109,   159,   206,   242,
     155,   143,   208,    83,   143,   168,   230,   231,     1,   114,
     233,    37,   115,   155,   170,   170,   242,   169,   168,   115,
     134,   260,   260,   134,   141,   170,   143,   242,   134,   181,
     134,   181,   134,    96,   225,   134,   134,    30,    52,   170,
     134,   142,   142,   153,   115,   142,   260,   115,   144,   115,
     144,    37,   115,   146,   254,    94,   115,   146,     7,    47,
     114,   190,   141,   201,    66,   228,   228,   228,   228,   260,
     260,   260,   260,   177,   260,   177,   260,   260,   260,   260,
     260,   260,   260,    27,    82,   168,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   242,   242,
     177,   260,   177,   260,    21,    37,    40,    54,    56,    66,
      82,   212,   253,   256,   260,   273,    26,    37,    26,    37,
      77,    37,   146,   177,   260,   170,   134,   260,   260,    96,
     144,   115,   134,   168,    37,   238,    36,   260,   177,   260,
      36,   260,   177,   260,   143,   170,   137,   161,   163,   260,
     161,   162,   155,   260,    28,   260,   144,   209,   210,   211,
     212,   198,   231,   115,   144,    27,    55,    72,    73,    76,
      85,   145,   234,   246,    96,   232,   260,   229,   144,   219,
     260,     1,   186,   242,   144,    16,   182,   234,   246,   115,
     163,   162,   142,   144,   144,   240,   144,   240,   177,   260,
     146,   155,   260,   146,   260,   146,   260,   168,   238,   141,
       1,   168,   194,   195,    21,    56,    66,    82,   203,   213,
     228,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   144,   146,    37,    66,   245,   115,   144,    65,    65,
      36,   177,   260,    36,   177,   260,   177,   260,   251,   251,
     134,   219,   261,   244,   190,   260,   144,   260,    26,   170,
      26,   170,   260,    26,   170,   254,    26,   170,   254,   255,
     256,   115,   134,    11,   134,    28,    28,   155,    77,   159,
     115,   144,   143,   168,    21,    56,    66,    82,   215,   144,
     231,   233,     1,   238,    50,   261,   142,    53,    88,   142,
     187,   144,   143,   159,   168,   183,   224,   134,   134,   146,
     254,   146,   254,   155,    94,   141,     1,   191,   142,    96,
     115,   142,    66,   202,    10,    15,   110,   111,   112,   113,
     116,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   132,   135,   136,   137,   138,   140,   143,   168,
     204,   205,   206,   245,   134,   256,   245,   245,   260,    26,
      26,   260,    26,    26,   146,   146,   170,   170,   141,   170,
      77,    36,   260,    36,   260,    77,    36,   260,   170,    36,
     260,   170,   115,   144,   260,   260,   155,   260,   155,    28,
     210,   231,   114,   237,    66,   233,   232,   146,    37,   146,
      26,   155,   238,   183,   114,   159,    36,   155,   260,   146,
      36,   155,   260,   146,   260,     1,   142,   160,   142,    32,
      60,    63,   142,   154,   172,   260,   195,   215,   168,   260,
     117,   143,   207,   207,    77,    36,    36,    77,    36,    36,
      36,    36,     1,   191,   260,   260,   260,   260,   260,   260,
     256,    11,    28,   155,   144,    27,    55,    72,    73,    76,
      85,   145,   219,   236,   246,   119,   221,   232,    89,   222,
     260,   234,   246,   155,   170,   144,   260,   260,   155,   260,
     155,   142,   142,   172,   233,   144,   168,   205,   206,   209,
     260,   260,   260,   260,   260,   260,   142,   142,    77,    77,
      77,    77,   260,   155,   237,   146,   238,   219,   220,   260,
     260,   159,   171,   218,   146,   159,    77,    77,    79,   216,
     207,   207,   144,    77,    77,    77,    77,    77,    77,   260,
     260,   260,   260,   221,   232,   219,   235,   236,   246,    37,
     146,   246,   260,   260,   222,   260,   235,   236,   134,   217,
     218,   146,   235
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
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
    while (YYID (0))
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, context); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, context)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    ParserContext* context;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (context);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, context)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    ParserContext* context;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, context);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ParserContext* context)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, context)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    ParserContext* context;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , context);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, context); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParserContext* context)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, context)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    ParserContext* context;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}



struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
yypstate *
yypstate_new (void)
#else
yypstate *
yypstate_new ()

#endif
{
  yypstate *yyps;
  yyps = (yypstate *) malloc (sizeof *yyps);
  if (!yyps)
    return YY_NULL;
  yyps->yynew = 1;
  return yyps;
}

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void
yypstate_delete (yypstate *yyps)
#else
void
yypstate_delete (yyps)
    yypstate *yyps;
#endif
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

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, ParserContext* context)
#else
int
yypush_parse (yyps, yypushed_char, yypushed_val, yypushed_loc, context)
    yypstate *yyps;
    int yypushed_char;
    YYSTYPE const *yypushed_val;
    YYLTYPE *yypushed_loc;
    ParserContext* context;
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
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
     `$$ = $1'.

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
/* Line 1792 of yacc.c  */
#line 443 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 461 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = buildDeleteStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (6)].pch), currentModuleType,
    new BlockStmt(), yyfilename, (yyvsp[(1) - (6)].b), (yyvsp[(2) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType, (yyvsp[(6) - (7)].pblockstmt), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 506 "chapel.ypp"
    {(yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(4) - (7)].pch), currentModuleType,
        buildErrorStandin(), yyfilename, (yyvsp[(1) - (7)].b), (yyvsp[(2) - (7)].b), (yylsp[(1) - (7)]).comment))); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 554 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 558 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 600 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 606 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 655 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 102:
/* Line 1792 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 103:
/* Line 1792 of yacc.c  */
#line 676 "chapel.ypp"
    {
#ifdef HAVE_LLVM
      if (externC) {
        (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
      } else {
        USR_FATAL(new BlockStmt(), "extern block syntax is turned off. Use --extern-c flag to turn on.");
      }
#else
      USR_FATAL(new BlockStmt(), "Chapel must be built with llvm in order to use the extern block syntax");
#endif
    }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 105:
/* Line 1792 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:
/* Line 1792 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:
/* Line 1792 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 108:
/* Line 1792 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 109:
/* Line 1792 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 110:
/* Line 1792 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 111:
/* Line 1792 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 112:
/* Line 1792 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 113:
/* Line 1792 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 114:
/* Line 1792 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 115:
/* Line 1792 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 116:
/* Line 1792 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 117:
/* Line 1792 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pcallexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 118:
/* Line 1792 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 119:
/* Line 1792 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 120:
/* Line 1792 of yacc.c  */
#line 707 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 121:
/* Line 1792 of yacc.c  */
#line 713 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 122:
/* Line 1792 of yacc.c  */
#line 719 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pcallexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 123:
/* Line 1792 of yacc.c  */
#line 725 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = ForallStmt::build((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 124:
/* Line 1792 of yacc.c  */
#line 731 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 125:
/* Line 1792 of yacc.c  */
#line 738 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = ForallStmt::build(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = ForallStmt::build(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 126:
/* Line 1792 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 127:
/* Line 1792 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 128:
/* Line 1792 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 129:
/* Line 1792 of yacc.c  */
#line 753 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 130:
/* Line 1792 of yacc.c  */
#line 754 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 131:
/* Line 1792 of yacc.c  */
#line 755 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (6)].pexpr),(yyvsp[(3) - (6)].pch),(yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 132:
/* Line 1792 of yacc.c  */
#line 757 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (5)].pexpr),(yyvsp[(3) - (5)].pch),(yyvsp[(4) - (5)].pexpr)), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 133:
/* Line 1792 of yacc.c  */
#line 759 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (8)].pexpr),(yyvsp[(3) - (8)].pch),(yyvsp[(4) - (8)].pexpr)), (yyvsp[(6) - (8)].pblockstmt), (yyvsp[(8) - (8)].pblockstmt)); }
    break;

  case 134:
/* Line 1792 of yacc.c  */
#line 761 "chapel.ypp"
    {
(yyval.pblockstmt) = buildIfStmt(convertAssignmentAndWarn((yyvsp[(2) - (7)].pexpr),(yyvsp[(3) - (7)].pch),(yyvsp[(4) - (7)].pexpr)), (yyvsp[(5) - (7)].pblockstmt), (yyvsp[(7) - (7)].pblockstmt)); }
    break;

  case 135:
/* Line 1792 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pblockstmt) = DeferStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 136:
/* Line 1792 of yacc.c  */
#line 769 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 137:
/* Line 1792 of yacc.c  */
#line 770 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 138:
/* Line 1792 of yacc.c  */
#line 771 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 139:
/* Line 1792 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 140:
/* Line 1792 of yacc.c  */
#line 773 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 141:
/* Line 1792 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (3)].pblockstmt), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 142:
/* Line 1792 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 143:
/* Line 1792 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 144:
/* Line 1792 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 145:
/* Line 1792 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 146:
/* Line 1792 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pexpr) = CatchStmt::build((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 147:
/* Line 1792 of yacc.c  */
#line 789 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch)), NULL, new UnresolvedSymExpr("Error")); }
    break;

  case 148:
/* Line 1792 of yacc.c  */
#line 790 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), NULL, (yyvsp[(3) - (3)].pexpr));   }
    break;

  case 149:
/* Line 1792 of yacc.c  */
#line 794 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 150:
/* Line 1792 of yacc.c  */
#line 798 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 151:
/* Line 1792 of yacc.c  */
#line 800 "chapel.ypp"
    { (yyval.pblockstmt) = buildErrorStandin(); }
    break;

  case 152:
/* Line 1792 of yacc.c  */
#line 804 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 153:
/* Line 1792 of yacc.c  */
#line 805 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 154:
/* Line 1792 of yacc.c  */
#line 810 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 155:
/* Line 1792 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 156:
/* Line 1792 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 157:
/* Line 1792 of yacc.c  */
#line 821 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch),
                                             NULL,
                                             (yyvsp[(1) - (6)].aggrTag),
                                             (yyvsp[(3) - (6)].pcallexpr),
                                             (yyvsp[(5) - (6)].pblockstmt),
                                             FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 158:
/* Line 1792 of yacc.c  */
#line 831 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             (yyvsp[(2) - (8)].pch),
                                             (yyvsp[(3) - (8)].aggrTag),
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             (yyvsp[(7) - (8)].pblockstmt),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 159:
/* Line 1792 of yacc.c  */
#line 841 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch),
                                             NULL,
                                             (yyvsp[(1) - (6)].aggrTag),
                                             (yyvsp[(3) - (6)].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 160:
/* Line 1792 of yacc.c  */
#line 851 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch),
                                             (yyvsp[(2) - (8)].pch),
                                             (yyvsp[(3) - (8)].aggrTag),
                                             (yyvsp[(5) - (8)].pcallexpr),
                                             new BlockStmt(),
                                             FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 161:
/* Line 1792 of yacc.c  */
#line 863 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_CLASS;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 162:
/* Line 1792 of yacc.c  */
#line 868 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_RECORD;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 163:
/* Line 1792 of yacc.c  */
#line 873 "chapel.ypp"
    {
             (yyval.aggrTag)                     = AGGREGATE_UNION;
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 164:
/* Line 1792 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 165:
/* Line 1792 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 166:
/* Line 1792 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 167:
/* Line 1792 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 168:
/* Line 1792 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 169:
/* Line 1792 of yacc.c  */
#line 896 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(1) - (5)].penumtype);
      for_vector(DefExpr, ec, *(yyvsp[(4) - (5)].pvecOfDefs)) {
        ec->sym->type = pdt;
        pdt->constants.insertAtTail(ec);
        if (pdt->defaultValue == NULL) {
          pdt->defaultValue = ec->sym;
        }
      }
      delete (yyvsp[(4) - (5)].pvecOfDefs);
      pdt->doc = (yylsp[(1) - (5)]).comment;
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(1) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 170:
/* Line 1792 of yacc.c  */
#line 912 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildErrorStandin();
    }
    break;

  case 171:
/* Line 1792 of yacc.c  */
#line 919 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyloc).comment = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 172:
/* Line 1792 of yacc.c  */
#line 928 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = new std::vector<DefExpr*>();
      (yyval.pvecOfDefs)->push_back((yyvsp[(1) - (1)].pdefexpr));
      //$$->doc = context->latestComment;
      // start here for enabling documentation of enum constants
      //context->latestComment = NULL;
    }
    break;

  case 173:
/* Line 1792 of yacc.c  */
#line 936 "chapel.ypp"
    {
      (yyval.pvecOfDefs) = (yyvsp[(1) - (2)].pvecOfDefs);
    }
    break;

  case 174:
/* Line 1792 of yacc.c  */
#line 940 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pvecOfDefs)->push_back((yyvsp[(3) - (3)].pdefexpr));
    }
    break;

  case 175:
/* Line 1792 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 176:
/* Line 1792 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 177:
/* Line 1792 of yacc.c  */
#line 952 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 178:
/* Line 1792 of yacc.c  */
#line 957 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 179:
/* Line 1792 of yacc.c  */
#line 962 "chapel.ypp"
    {
      (yyvsp[(3) - (8)].pfnsymbol)->retTag = (yyvsp[(5) - (8)].retTag);
      if ((yyvsp[(5) - (8)].retTag) == RET_REF || (yyvsp[(5) - (8)].retTag) == RET_CONST_REF)
        USR_FATAL("'ref' return types are not allowed in lambdas");
      if ((yyvsp[(5) - (8)].retTag) == RET_PARAM)
        USR_FATAL("'param' return types are not allowed in lambdas");
      if ((yyvsp[(5) - (8)].retTag) == RET_TYPE)
        USR_FATAL("'type' return types are not allowed in lambdas");
      if ((yyvsp[(6) - (8)].pexpr))
        (yyvsp[(3) - (8)].pfnsymbol)->retExprType = new BlockStmt((yyvsp[(6) - (8)].pexpr), BLOCK_SCOPELESS);
      if ((yyvsp[(7) - (8)].pexpr))
        (yyvsp[(3) - (8)].pfnsymbol)->where = new BlockStmt((yyvsp[(7) - (8)].pexpr));
      (yyvsp[(3) - (8)].pfnsymbol)->insertAtTail((yyvsp[(8) - (8)].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[(3) - (8)].pfnsymbol)));
    }
    break;

  case 180:
/* Line 1792 of yacc.c  */
#line 982 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 181:
/* Line 1792 of yacc.c  */
#line 988 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 182:
/* Line 1792 of yacc.c  */
#line 995 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 183:
/* Line 1792 of yacc.c  */
#line 1003 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 184:
/* Line 1792 of yacc.c  */
#line 1015 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 185:
/* Line 1792 of yacc.c  */
#line 1021 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 186:
/* Line 1792 of yacc.c  */
#line 1028 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[(4) - (10)].pfnsymbol);

      fn->copyFlags((yyvsp[(1) - (10)].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[(1) - (10)].pfnsymbol)->name)
        fn->cname = (yyvsp[(1) - (10)].pfnsymbol)->name;

      if ((yyvsp[(2) - (10)].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (10)].pfnsymbol), (yyvsp[(6) - (10)].retTag), (yyvsp[(7) - (10)].pexpr), (yyvsp[(8) - (10)].b), (yyvsp[(9) - (10)].pexpr), (yyvsp[(10) - (10)].pblockstmt), (yylsp[(1) - (10)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 187:
/* Line 1792 of yacc.c  */
#line 1050 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 188:
/* Line 1792 of yacc.c  */
#line 1054 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 189:
/* Line 1792 of yacc.c  */
#line 1059 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 190:
/* Line 1792 of yacc.c  */
#line 1063 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 192:
/* Line 1792 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 194:
/* Line 1792 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 195:
/* Line 1792 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 196:
/* Line 1792 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 197:
/* Line 1792 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 198:
/* Line 1792 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 199:
/* Line 1792 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 200:
/* Line 1792 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 201:
/* Line 1792 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 202:
/* Line 1792 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 203:
/* Line 1792 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 204:
/* Line 1792 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 205:
/* Line 1792 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 206:
/* Line 1792 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 207:
/* Line 1792 of yacc.c  */
#line 1089 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 208:
/* Line 1792 of yacc.c  */
#line 1090 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 209:
/* Line 1792 of yacc.c  */
#line 1091 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 210:
/* Line 1792 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 211:
/* Line 1792 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 212:
/* Line 1792 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 213:
/* Line 1792 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 214:
/* Line 1792 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 215:
/* Line 1792 of yacc.c  */
#line 1097 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 216:
/* Line 1792 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 217:
/* Line 1792 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 218:
/* Line 1792 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 219:
/* Line 1792 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 220:
/* Line 1792 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 221:
/* Line 1792 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 222:
/* Line 1792 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 223:
/* Line 1792 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 224:
/* Line 1792 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 225:
/* Line 1792 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 226:
/* Line 1792 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 227:
/* Line 1792 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 228:
/* Line 1792 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 229:
/* Line 1792 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 230:
/* Line 1792 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 231:
/* Line 1792 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 232:
/* Line 1792 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 233:
/* Line 1792 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 234:
/* Line 1792 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 235:
/* Line 1792 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 236:
/* Line 1792 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 237:
/* Line 1792 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 238:
/* Line 1792 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 239:
/* Line 1792 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 240:
/* Line 1792 of yacc.c  */
#line 1141 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 241:
/* Line 1792 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 242:
/* Line 1792 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 243:
/* Line 1792 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 244:
/* Line 1792 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 245:
/* Line 1792 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 246:
/* Line 1792 of yacc.c  */
#line 1153 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 247:
/* Line 1792 of yacc.c  */
#line 1154 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 248:
/* Line 1792 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 249:
/* Line 1792 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 250:
/* Line 1792 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 251:
/* Line 1792 of yacc.c  */
#line 1158 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 252:
/* Line 1792 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 253:
/* Line 1792 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 254:
/* Line 1792 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 255:
/* Line 1792 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 256:
/* Line 1792 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 257:
/* Line 1792 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 258:
/* Line 1792 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 259:
/* Line 1792 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 260:
/* Line 1792 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 261:
/* Line 1792 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 262:
/* Line 1792 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 263:
/* Line 1792 of yacc.c  */
#line 1179 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 264:
/* Line 1792 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 265:
/* Line 1792 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 266:
/* Line 1792 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 267:
/* Line 1792 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 268:
/* Line 1792 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 271:
/* Line 1792 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 272:
/* Line 1792 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 273:
/* Line 1792 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 274:
/* Line 1792 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 276:
/* Line 1792 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 277:
/* Line 1792 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 278:
/* Line 1792 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 279:
/* Line 1792 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:
/* Line 1792 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 281:
/* Line 1792 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 282:
/* Line 1792 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 283:
/* Line 1792 of yacc.c  */
#line 1231 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 284:
/* Line 1792 of yacc.c  */
#line 1244 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (4)].pexpr));

      (yyvsp[(4) - (4)].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[(4) - (4)].pblockstmt));
    }
    break;

  case 285:
/* Line 1792 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 286:
/* Line 1792 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 287:
/* Line 1792 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 288:
/* Line 1792 of yacc.c  */
#line 1269 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 289:
/* Line 1792 of yacc.c  */
#line 1277 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 290:
/* Line 1792 of yacc.c  */
#line 1285 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 291:
/* Line 1792 of yacc.c  */
#line 1293 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 292:
/* Line 1792 of yacc.c  */
#line 1302 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 293:
/* Line 1792 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 294:
/* Line 1792 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 295:
/* Line 1792 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 297:
/* Line 1792 of yacc.c  */
#line 1319 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 298:
/* Line 1792 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 299:
/* Line 1792 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 300:
/* Line 1792 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 301:
/* Line 1792 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 302:
/* Line 1792 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 303:
/* Line 1792 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 304:
/* Line 1792 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 305:
/* Line 1792 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 306:
/* Line 1792 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 307:
/* Line 1792 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 308:
/* Line 1792 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 309:
/* Line 1792 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 310:
/* Line 1792 of yacc.c  */
#line 1360 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 311:
/* Line 1792 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 312:
/* Line 1792 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 313:
/* Line 1792 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 314:
/* Line 1792 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 315:
/* Line 1792 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 316:
/* Line 1792 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 317:
/* Line 1792 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 318:
/* Line 1792 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 319:
/* Line 1792 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 320:
/* Line 1792 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 321:
/* Line 1792 of yacc.c  */
#line 1397 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 322:
/* Line 1792 of yacc.c  */
#line 1405 "chapel.ypp"
    {
      (yyval.pcallexpr) = new CallExpr(PRIM_ERROR);
    }
    break;

  case 323:
/* Line 1792 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 324:
/* Line 1792 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 325:
/* Line 1792 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 326:
/* Line 1792 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 327:
/* Line 1792 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 328:
/* Line 1792 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 329:
/* Line 1792 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 330:
/* Line 1792 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 331:
/* Line 1792 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 332:
/* Line 1792 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 333:
/* Line 1792 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 334:
/* Line 1792 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 335:
/* Line 1792 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 336:
/* Line 1792 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 337:
/* Line 1792 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_owned"); }
    break;

  case 338:
/* Line 1792 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_unmanaged"); }
    break;

  case 339:
/* Line 1792 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_shared"); }
    break;

  case 340:
/* Line 1792 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 341:
/* Line 1792 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 342:
/* Line 1792 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 343:
/* Line 1792 of yacc.c  */
#line 1451 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 344:
/* Line 1792 of yacc.c  */
#line 1452 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 345:
/* Line 1792 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr));}
    break;

  case 346:
/* Line 1792 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 347:
/* Line 1792 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 348:
/* Line 1792 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 349:
/* Line 1792 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 350:
/* Line 1792 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 351:
/* Line 1792 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 352:
/* Line 1792 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 354:
/* Line 1792 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 355:
/* Line 1792 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 356:
/* Line 1792 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 357:
/* Line 1792 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 358:
/* Line 1792 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 359:
/* Line 1792 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 360:
/* Line 1792 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 366:
/* Line 1792 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 367:
/* Line 1792 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 368:
/* Line 1792 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 369:
/* Line 1792 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:
/* Line 1792 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 371:
/* Line 1792 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 372:
/* Line 1792 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 373:
/* Line 1792 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_owned", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 374:
/* Line 1792 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_to_unmanaged", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 375:
/* Line 1792 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_shared", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 376:
/* Line 1792 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 377:
/* Line 1792 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 378:
/* Line 1792 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 379:
/* Line 1792 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 380:
/* Line 1792 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 381:
/* Line 1792 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 382:
/* Line 1792 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 383:
/* Line 1792 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 384:
/* Line 1792 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 385:
/* Line 1792 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 386:
/* Line 1792 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 387:
/* Line 1792 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 388:
/* Line 1792 of yacc.c  */
#line 1553 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 389:
/* Line 1792 of yacc.c  */
#line 1560 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 390:
/* Line 1792 of yacc.c  */
#line 1566 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 391:
/* Line 1792 of yacc.c  */
#line 1572 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 392:
/* Line 1792 of yacc.c  */
#line 1578 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 393:
/* Line 1792 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 394:
/* Line 1792 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 402:
/* Line 1792 of yacc.c  */
#line 1612 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:
/* Line 1792 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 405:
/* Line 1792 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 406:
/* Line 1792 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 407:
/* Line 1792 of yacc.c  */
#line 1629 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 408:
/* Line 1792 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 409:
/* Line 1792 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addForallIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pShadowVar)); }
    break;

  case 410:
/* Line 1792 of yacc.c  */
#line 1641 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pShadowVar)); }
    break;

  case 411:
/* Line 1792 of yacc.c  */
#line 1645 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromArgIntent((yyvsp[(1) - (2)].pt), (yyvsp[(2) - (2)].pexpr));
    }
    break;

  case 412:
/* Line 1792 of yacc.c  */
#line 1648 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 413:
/* Line 1792 of yacc.c  */
#line 1651 "chapel.ypp"
    {
      (yyval.pShadowVar) = ShadowVarSymbol::buildFromReduceIntent((yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr));
    }
    break;

  case 415:
/* Line 1792 of yacc.c  */
#line 1659 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:
/* Line 1792 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 417:
/* Line 1792 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 426:
/* Line 1792 of yacc.c  */
#line 1685 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 427:
/* Line 1792 of yacc.c  */
#line 1687 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:
/* Line 1792 of yacc.c  */
#line 1689 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:
/* Line 1792 of yacc.c  */
#line 1691 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 430:
/* Line 1792 of yacc.c  */
#line 1693 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 431:
/* Line 1792 of yacc.c  */
#line 1695 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 432:
/* Line 1792 of yacc.c  */
#line 1699 "chapel.ypp"
    { (yyval.pexpr) = tryExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 433:
/* Line 1792 of yacc.c  */
#line 1700 "chapel.ypp"
    { (yyval.pexpr) = tryBangExpr((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 434:
/* Line 1792 of yacc.c  */
#line 1701 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 441:
/* Line 1792 of yacc.c  */
#line 1723 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 442:
/* Line 1792 of yacc.c  */
#line 1724 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 443:
/* Line 1792 of yacc.c  */
#line 1725 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 444:
/* Line 1792 of yacc.c  */
#line 1729 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 445:
/* Line 1792 of yacc.c  */
#line 1730 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 446:
/* Line 1792 of yacc.c  */
#line 1731 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 447:
/* Line 1792 of yacc.c  */
#line 1739 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 448:
/* Line 1792 of yacc.c  */
#line 1740 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 449:
/* Line 1792 of yacc.c  */
#line 1741 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 450:
/* Line 1792 of yacc.c  */
#line 1742 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 451:
/* Line 1792 of yacc.c  */
#line 1746 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 452:
/* Line 1792 of yacc.c  */
#line 1747 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 453:
/* Line 1792 of yacc.c  */
#line 1748 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 454:
/* Line 1792 of yacc.c  */
#line 1749 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 455:
/* Line 1792 of yacc.c  */
#line 1750 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 456:
/* Line 1792 of yacc.c  */
#line 1751 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 457:
/* Line 1792 of yacc.c  */
#line 1752 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 458:
/* Line 1792 of yacc.c  */
#line 1753 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 459:
/* Line 1792 of yacc.c  */
#line 1754 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 460:
/* Line 1792 of yacc.c  */
#line 1756 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 461:
/* Line 1792 of yacc.c  */
#line 1760 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 462:
/* Line 1792 of yacc.c  */
#line 1767 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 463:
/* Line 1792 of yacc.c  */
#line 1768 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 464:
/* Line 1792 of yacc.c  */
#line 1772 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 465:
/* Line 1792 of yacc.c  */
#line 1773 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 466:
/* Line 1792 of yacc.c  */
#line 1774 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 467:
/* Line 1792 of yacc.c  */
#line 1775 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 468:
/* Line 1792 of yacc.c  */
#line 1776 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 469:
/* Line 1792 of yacc.c  */
#line 1777 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 470:
/* Line 1792 of yacc.c  */
#line 1778 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 471:
/* Line 1792 of yacc.c  */
#line 1779 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 472:
/* Line 1792 of yacc.c  */
#line 1780 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 473:
/* Line 1792 of yacc.c  */
#line 1781 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 474:
/* Line 1792 of yacc.c  */
#line 1782 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 475:
/* Line 1792 of yacc.c  */
#line 1783 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 476:
/* Line 1792 of yacc.c  */
#line 1784 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 477:
/* Line 1792 of yacc.c  */
#line 1785 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 478:
/* Line 1792 of yacc.c  */
#line 1786 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 479:
/* Line 1792 of yacc.c  */
#line 1787 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 480:
/* Line 1792 of yacc.c  */
#line 1788 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 481:
/* Line 1792 of yacc.c  */
#line 1789 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 482:
/* Line 1792 of yacc.c  */
#line 1790 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 483:
/* Line 1792 of yacc.c  */
#line 1791 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 484:
/* Line 1792 of yacc.c  */
#line 1792 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 485:
/* Line 1792 of yacc.c  */
#line 1793 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 486:
/* Line 1792 of yacc.c  */
#line 1794 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 487:
/* Line 1792 of yacc.c  */
#line 1798 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 488:
/* Line 1792 of yacc.c  */
#line 1799 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 489:
/* Line 1792 of yacc.c  */
#line 1800 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 490:
/* Line 1792 of yacc.c  */
#line 1801 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 491:
/* Line 1792 of yacc.c  */
#line 1802 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 492:
/* Line 1792 of yacc.c  */
#line 1803 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 493:
/* Line 1792 of yacc.c  */
#line 1807 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 494:
/* Line 1792 of yacc.c  */
#line 1808 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 495:
/* Line 1792 of yacc.c  */
#line 1809 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 496:
/* Line 1792 of yacc.c  */
#line 1810 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 497:
/* Line 1792 of yacc.c  */
#line 1814 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 498:
/* Line 1792 of yacc.c  */
#line 1815 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 499:
/* Line 1792 of yacc.c  */
#line 1816 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 500:
/* Line 1792 of yacc.c  */
#line 1817 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 501:
/* Line 1792 of yacc.c  */
#line 1822 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 502:
/* Line 1792 of yacc.c  */
#line 1823 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 503:
/* Line 1792 of yacc.c  */
#line 1824 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 504:
/* Line 1792 of yacc.c  */
#line 1825 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 505:
/* Line 1792 of yacc.c  */
#line 1826 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 506:
/* Line 1792 of yacc.c  */
#line 1827 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 507:
/* Line 1792 of yacc.c  */
#line 1828 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;


/* Line 1792 of yacc.c  */
#line 8418 "bison-chapel.cpp"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


