/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */


/* Line 268 of yacc.c  */
#line 71 "bison-chapel.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 288 of yacc.c  */
#line 33 "chapel.ypp"

  extern int  captureTokens;
  extern char captureString[1024];


/* Line 288 of yacc.c  */
#line 44 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_0_
  #define _BISON_CHAPEL_DEFINES_0_

  #define YYLEX_NEWLINE                  -1
  #define YYLEX_SINGLE_LINE_COMMENT      -2
  #define YYLEX_BLOCK_COMMENT            -3

  typedef void* yyscan_t;

  int processNewline(yyscan_t scanner);
  void stringBufferInit();

  #endif


/* Line 288 of yacc.c  */
#line 64 "chapel.ypp"

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


/* Line 288 of yacc.c  */
#line 134 "chapel.ypp"

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


/* Line 288 of yacc.c  */
#line 156 "chapel.ypp"

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



/* Line 288 of yacc.c  */
#line 238 "bison-chapel.cpp"

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
     TTHROW = 327,
     TTHROWS = 328,
     TTRY = 329,
     TTRYBANG = 330,
     TTYPE = 331,
     TUNDERSCORE = 332,
     TUNION = 333,
     TUSE = 334,
     TVAR = 335,
     TWHEN = 336,
     TWHERE = 337,
     TWHILE = 338,
     TWITH = 339,
     TYIELD = 340,
     TZIP = 341,
     TALIAS = 342,
     TAND = 343,
     TASSIGN = 344,
     TASSIGNBAND = 345,
     TASSIGNBOR = 346,
     TASSIGNBXOR = 347,
     TASSIGNDIVIDE = 348,
     TASSIGNEXP = 349,
     TASSIGNLAND = 350,
     TASSIGNLOR = 351,
     TASSIGNMINUS = 352,
     TASSIGNMOD = 353,
     TASSIGNMULTIPLY = 354,
     TASSIGNPLUS = 355,
     TASSIGNSL = 356,
     TASSIGNSR = 357,
     TBAND = 358,
     TBNOT = 359,
     TBOR = 360,
     TBXOR = 361,
     TCOLON = 362,
     TCOMMA = 363,
     TDIVIDE = 364,
     TDOT = 365,
     TDOTDOT = 366,
     TDOTDOTDOT = 367,
     TEQUAL = 368,
     TEXP = 369,
     TGREATER = 370,
     TGREATEREQUAL = 371,
     THASH = 372,
     TLESS = 373,
     TLESSEQUAL = 374,
     TMINUS = 375,
     TMOD = 376,
     TNOT = 377,
     TNOTEQUAL = 378,
     TOR = 379,
     TPLUS = 380,
     TQUESTION = 381,
     TSEMI = 382,
     TSHIFTLEFT = 383,
     TSHIFTRIGHT = 384,
     TSTAR = 385,
     TSWAP = 386,
     TASSIGNREDUCE = 387,
     TIO = 388,
     TLCBR = 389,
     TRCBR = 390,
     TLP = 391,
     TRP = 392,
     TLSBR = 393,
     TRSBR = 394,
     TNOELSE = 395,
     TUMINUS = 396,
     TUPLUS = 397
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

#ifndef YYPUSH_DECLS
#  define YYPUSH_DECLS
struct yypstate;
typedef struct yypstate yypstate;
enum { YYPUSH_MORE = 4 };

#if defined __STDC__ || defined __cplusplus
int yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE const *yypushed_loc, ParserContext* context);
#else
int yypush_parse ();
#endif

#if defined __STDC__ || defined __cplusplus
yypstate * yypstate_new (void);
#else
yypstate * yypstate_new ();
#endif
#if defined __STDC__ || defined __cplusplus
void yypstate_delete (yypstate *yyps);
#else
void yypstate_delete ();
#endif
#endif

/* "%code provides" blocks.  */

/* Line 340 of yacc.c  */
#line 187 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 448 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 454 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  int         captureTokens;
  char        captureString[1024];


/* Line 344 of yacc.c  */
#line 195 "chapel.ypp"

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



/* Line 344 of yacc.c  */
#line 529 "bison-chapel.cpp"

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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
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
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   13318

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  143
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  116
/* YYNRULES -- Number of rules.  */
#define YYNRULES  460
/* YYNRULES -- Number of states.  */
#define YYNSTATES  873

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   397

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
     135,   136,   137,   138,   139,   140,   141,   142
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    50,    53,    57,    61,    65,    69,    73,
      77,    80,    84,    88,    91,    94,    98,   100,   106,   113,
     114,   116,   118,   121,   125,   127,   130,   132,   136,   140,
     146,   147,   149,   151,   153,   157,   163,   169,   173,   178,
     183,   188,   193,   198,   203,   208,   213,   218,   223,   228,
     233,   238,   243,   248,   253,   254,   256,   258,   260,   262,
     265,   267,   270,   274,   276,   278,   281,   284,   286,   288,
     290,   292,   294,   297,   303,   307,   314,   321,   326,   332,
     338,   342,   346,   353,   359,   366,   372,   379,   383,   388,
     395,   403,   410,   418,   423,   429,   434,   439,   443,   450,
     456,   459,   462,   466,   472,   473,   476,   480,   483,   487,
     494,   503,   505,   507,   509,   510,   513,   514,   517,   521,
     527,   529,   532,   536,   538,   542,   543,   544,   553,   554,
     556,   559,   562,   563,   564,   575,   579,   583,   589,   595,
     597,   601,   603,   606,   608,   610,   612,   614,   616,   618,
     620,   622,   624,   626,   628,   630,   632,   634,   636,   638,
     640,   642,   644,   646,   648,   650,   652,   654,   656,   658,
     660,   662,   664,   666,   668,   670,   672,   674,   676,   678,
     679,   683,   687,   688,   690,   694,   699,   704,   711,   718,
     719,   721,   723,   725,   727,   729,   732,   735,   737,   739,
     741,   742,   744,   746,   748,   750,   752,   753,   755,   758,
     760,   762,   764,   765,   767,   769,   771,   773,   775,   778,
     780,   781,   783,   786,   789,   790,   793,   797,   802,   807,
     810,   815,   816,   819,   822,   827,   832,   837,   843,   848,
     849,   851,   853,   855,   859,   863,   868,   874,   876,   878,
     882,   884,   887,   891,   892,   895,   898,   899,   904,   905,
     908,   911,   913,   918,   923,   930,   932,   933,   935,   937,
     941,   946,   950,   955,   962,   963,   966,   969,   972,   975,
     978,   981,   983,   985,   989,   993,   995,   997,   999,  1003,
    1007,  1008,  1010,  1012,  1016,  1020,  1024,  1028,  1030,  1032,
    1034,  1036,  1038,  1040,  1042,  1044,  1047,  1052,  1057,  1062,
    1068,  1071,  1074,  1081,  1088,  1093,  1103,  1113,  1121,  1128,
    1135,  1140,  1150,  1160,  1168,  1173,  1180,  1187,  1197,  1207,
    1214,  1216,  1218,  1220,  1222,  1224,  1226,  1228,  1230,  1234,
    1235,  1237,  1242,  1244,  1248,  1253,  1255,  1259,  1262,  1266,
    1270,  1272,  1276,  1279,  1284,  1286,  1288,  1290,  1292,  1294,
    1296,  1298,  1300,  1305,  1309,  1313,  1316,  1319,  1321,  1323,
    1325,  1327,  1329,  1331,  1333,  1338,  1343,  1348,  1352,  1356,
    1360,  1364,  1369,  1373,  1378,  1380,  1382,  1384,  1386,  1388,
    1392,  1397,  1401,  1406,  1410,  1415,  1419,  1425,  1429,  1433,
    1437,  1441,  1445,  1449,  1453,  1457,  1461,  1465,  1469,  1473,
    1477,  1481,  1485,  1489,  1493,  1497,  1501,  1505,  1509,  1513,
    1517,  1520,  1523,  1526,  1529,  1532,  1535,  1539,  1543,  1547,
    1551,  1555,  1559,  1563,  1567,  1569,  1571,  1573,  1575,  1577,
    1579
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     144,     0,    -1,   145,    -1,    -1,   145,   146,    -1,   148,
      -1,   147,   148,    -1,    54,     7,    -1,   147,    54,     7,
      -1,   149,    -1,   151,    -1,   156,    -1,   157,    -1,   164,
      -1,   158,    -1,   166,    -1,   169,    -1,   167,    -1,   172,
      -1,   170,    -1,   171,    -1,   163,    -1,   236,   127,    -1,
      12,   148,    -1,    13,   237,   148,    -1,    14,   159,   127,
      -1,    17,   237,   151,    -1,    21,   159,   127,    -1,    22,
     246,   127,    -1,    39,   160,   148,    -1,    42,   148,    -1,
      48,   246,   162,    -1,    66,   246,   162,    -1,    66,   162,
      -1,    70,   148,    -1,    85,   246,   127,    -1,     1,    -1,
     150,    44,   160,   134,   135,    -1,   150,    44,   160,   134,
     152,   135,    -1,    -1,    58,    -1,    56,    -1,   134,   135,
      -1,   134,   152,   135,    -1,   146,    -1,   152,   146,    -1,
     246,    -1,   246,    11,   246,    -1,   153,   108,   246,    -1,
     153,   108,   246,    11,   246,    -1,    -1,   153,    -1,   130,
      -1,   153,    -1,    79,   225,   127,    -1,    79,   246,    28,
     155,   127,    -1,    79,   246,    49,   154,   127,    -1,    62,
     225,   127,    -1,   247,    89,   246,   127,    -1,   247,   100,
     246,   127,    -1,   247,    97,   246,   127,    -1,   247,    99,
     246,   127,    -1,   247,    93,   246,   127,    -1,   247,    98,
     246,   127,    -1,   247,    94,   246,   127,    -1,   247,    90,
     246,   127,    -1,   247,    91,   246,   127,    -1,   247,    92,
     246,   127,    -1,   247,   102,   246,   127,    -1,   247,   101,
     246,   127,    -1,   247,   131,   246,   127,    -1,   247,   132,
     246,   127,    -1,   247,    95,   246,   127,    -1,   247,    96,
     246,   127,    -1,    -1,   160,    -1,     3,    -1,   159,    -1,
       7,    -1,    24,   148,    -1,   151,    -1,    63,   127,    -1,
      63,   246,   127,    -1,   127,    -1,   165,    -1,    58,   165,
      -1,    56,   165,    -1,   179,    -1,   209,    -1,   175,    -1,
     186,    -1,   212,    -1,    30,     9,    -1,    24,   148,    83,
     246,   127,    -1,    83,   246,   162,    -1,    18,   246,    34,
     246,   237,   162,    -1,    18,   246,    34,   168,   237,   162,
      -1,    18,   246,   237,   162,    -1,    31,   246,    34,   246,
     162,    -1,    31,   246,    34,   168,   162,    -1,    31,   246,
     162,    -1,    31,   168,   162,    -1,    31,    52,   160,    34,
     246,   162,    -1,    32,   246,    34,   246,   162,    -1,    32,
     246,    34,   246,   240,   162,    -1,    32,   246,    34,   168,
     162,    -1,    32,   246,    34,   168,   240,   162,    -1,    32,
     246,   162,    -1,    32,   246,   240,   162,    -1,   138,   225,
      34,   246,   139,   148,    -1,   138,   225,    34,   246,   240,
     139,   148,    -1,   138,   225,    34,   168,   139,   148,    -1,
     138,   225,    34,   168,   240,   139,   148,    -1,   138,   225,
     139,   148,    -1,   138,   225,   240,   139,   148,    -1,    86,
     136,   225,   137,    -1,    33,   246,    71,   148,    -1,    33,
     246,   151,    -1,    33,   246,    71,   148,    26,   148,    -1,
      33,   246,   151,    26,   148,    -1,    74,   148,    -1,    75,
     148,    -1,    72,   246,   127,    -1,    65,   246,   134,   173,
     135,    -1,    -1,   173,   174,    -1,    81,   225,   162,    -1,
      50,   148,    -1,    50,    24,   148,    -1,   176,   160,   177,
     134,   178,   135,    -1,    30,   161,   176,   160,   177,   134,
     178,   135,    -1,    16,    -1,    59,    -1,    78,    -1,    -1,
     107,   225,    -1,    -1,   178,   164,    -1,   178,   147,   164,
      -1,    27,   160,   134,   180,   135,    -1,   181,    -1,   180,
     108,    -1,   180,   108,   181,    -1,   160,    -1,   160,    89,
     246,    -1,    -1,    -1,    40,   183,   194,   184,   201,   220,
     208,   204,    -1,    -1,    36,    -1,    29,   161,    -1,    30,
     161,    -1,    -1,    -1,   185,   200,   187,   189,   188,   201,
     220,   202,   208,   203,    -1,   199,   191,   193,    -1,   199,
     192,   193,    -1,   199,   190,   110,   191,   193,    -1,   199,
     190,   110,   192,   193,    -1,   231,    -1,   136,   246,   137,
      -1,   160,    -1,   104,   160,    -1,   103,    -1,   105,    -1,
     106,    -1,   104,    -1,   113,    -1,   123,    -1,   119,    -1,
     116,    -1,   118,    -1,   115,    -1,   125,    -1,   120,    -1,
     130,    -1,   109,    -1,   128,    -1,   129,    -1,   121,    -1,
     114,    -1,   122,    -1,    15,    -1,   117,    -1,    10,    -1,
     131,    -1,   133,    -1,    89,    -1,   100,    -1,    97,    -1,
      99,    -1,    93,    -1,    98,    -1,    94,    -1,    90,    -1,
      91,    -1,    92,    -1,   102,    -1,   101,    -1,    -1,   136,
     195,   137,    -1,   136,   195,   137,    -1,    -1,   196,    -1,
     195,   108,   196,    -1,   197,   160,   224,   218,    -1,   197,
     160,   224,   207,    -1,   197,   136,   217,   137,   224,   218,
      -1,   197,   136,   217,   137,   224,   207,    -1,    -1,   198,
      -1,    34,    -1,    37,    -1,    51,    -1,    20,    -1,    20,
      34,    -1,    20,    61,    -1,    52,    -1,    61,    -1,    76,
      -1,    -1,    52,    -1,    61,    -1,    76,    -1,    57,    -1,
      38,    -1,    -1,    20,    -1,    20,    61,    -1,    61,    -1,
      52,    -1,    76,    -1,    -1,    73,    -1,   127,    -1,   204,
      -1,   151,    -1,   163,    -1,   126,   160,    -1,   126,    -1,
      -1,   205,    -1,   112,   246,    -1,   112,   206,    -1,    -1,
      82,   246,    -1,    76,   210,   127,    -1,    19,    76,   210,
     127,    -1,    30,    76,   210,   127,    -1,   160,   211,    -1,
     160,   211,   108,   210,    -1,    -1,    89,   232,    -1,    89,
     221,    -1,   213,    52,   214,   127,    -1,   213,    20,   214,
     127,    -1,   213,    61,   214,   127,    -1,   213,    20,    61,
     214,   127,    -1,   213,    80,   214,   127,    -1,    -1,    19,
      -1,    30,    -1,   215,    -1,   214,   108,   215,    -1,   160,
     220,   218,    -1,   160,   219,    87,   246,    -1,   136,   217,
     137,   220,   218,    -1,    77,    -1,   160,    -1,   136,   217,
     137,    -1,   216,    -1,   216,   108,    -1,   216,   108,   217,
      -1,    -1,    89,    47,    -1,    89,   246,    -1,    -1,   107,
     138,   225,   139,    -1,    -1,   107,   232,    -1,   107,   221,
      -1,     1,    -1,   138,   225,   139,   232,    -1,   138,   225,
     139,   221,    -1,   138,   225,    34,   246,   139,   232,    -1,
       1,    -1,    -1,   232,    -1,   205,    -1,   138,   139,   222,
      -1,   138,   225,   139,   222,    -1,   138,   139,   223,    -1,
     138,   225,   139,   223,    -1,   138,   225,    34,   246,   139,
     222,    -1,    -1,   107,   232,    -1,   107,   205,    -1,   107,
      25,    -1,   107,    67,    -1,   107,    70,    -1,   107,   223,
      -1,   246,    -1,   205,    -1,   225,   108,   246,    -1,   225,
     108,   205,    -1,    77,    -1,   246,    -1,   205,    -1,   226,
     108,   226,    -1,   227,   108,   226,    -1,    -1,   229,    -1,
     230,    -1,   229,   108,   230,    -1,   160,    89,   205,    -1,
     160,    89,   246,    -1,   160,    87,   246,    -1,   205,    -1,
     246,    -1,   160,    -1,   235,    -1,   247,    -1,   234,    -1,
     255,    -1,   254,    -1,    67,   246,    -1,    35,   136,   228,
     137,    -1,    25,   136,   228,   137,    -1,    69,   136,   228,
     137,    -1,    68,    69,   136,   228,   137,    -1,    12,   246,
      -1,    70,   246,    -1,    31,   246,    34,   246,    24,   246,
      -1,    31,   246,    34,   168,    24,   246,    -1,    31,   246,
      24,   246,    -1,    31,   246,    34,   246,    24,    33,   246,
      71,   246,    -1,    31,   246,    34,   168,    24,    33,   246,
      71,   246,    -1,    31,   246,    24,    33,   246,    71,   246,
      -1,    32,   246,    34,   246,    24,   246,    -1,    32,   246,
      34,   168,    24,   246,    -1,    32,   246,    24,   246,    -1,
      32,   246,    34,   246,    24,    33,   246,    71,   246,    -1,
      32,   246,    34,   168,    24,    33,   246,    71,   246,    -1,
      32,   246,    24,    33,   246,    71,   246,    -1,   138,   225,
     139,   246,    -1,   138,   225,    34,   246,   139,   246,    -1,
     138,   225,    34,   168,   139,   246,    -1,   138,   225,    34,
     246,   139,    33,   246,    71,   246,    -1,   138,   225,    34,
     168,   139,    33,   246,    71,   246,    -1,    33,   246,    71,
     246,    26,   246,    -1,    46,    -1,   235,    -1,   231,    -1,
     250,    -1,   249,    -1,   182,    -1,   244,    -1,   245,    -1,
     243,   133,   246,    -1,    -1,   238,    -1,    84,   136,   239,
     137,    -1,   242,    -1,   239,   108,   242,    -1,    84,   136,
     241,   137,    -1,   242,    -1,   241,   108,   242,    -1,   198,
     231,    -1,   246,    60,   231,    -1,   258,    60,   231,    -1,
     247,    -1,   243,   133,   246,    -1,    45,   246,    -1,    41,
     214,    34,   246,    -1,   252,    -1,   232,    -1,   233,    -1,
     256,    -1,   257,    -1,   182,    -1,   244,    -1,   245,    -1,
     136,   112,   246,   137,    -1,   246,   107,   246,    -1,   246,
     111,   246,    -1,   246,   111,    -1,   111,   246,    -1,   111,
      -1,   231,    -1,   249,    -1,   250,    -1,   251,    -1,   247,
      -1,   182,    -1,   248,   136,   228,   137,    -1,   248,   138,
     228,   139,    -1,    55,   136,   228,   137,    -1,   246,   110,
     160,    -1,   246,   110,    76,    -1,   246,   110,    25,    -1,
     136,   226,   137,    -1,   136,   226,   108,   137,    -1,   136,
     227,   137,    -1,   136,   227,   108,   137,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,   134,   225,
     135,    -1,   134,   225,   108,   135,    -1,   138,   225,   139,
      -1,   138,   225,   108,   139,    -1,   138,   253,   139,    -1,
     138,   253,   108,   139,    -1,   246,    87,   246,    -1,   253,
     108,   246,    87,   246,    -1,   246,   125,   246,    -1,   246,
     120,   246,    -1,   246,   130,   246,    -1,   246,   109,   246,
      -1,   246,   128,   246,    -1,   246,   129,   246,    -1,   246,
     121,   246,    -1,   246,   113,   246,    -1,   246,   123,   246,
      -1,   246,   119,   246,    -1,   246,   116,   246,    -1,   246,
     118,   246,    -1,   246,   115,   246,    -1,   246,   103,   246,
      -1,   246,   105,   246,    -1,   246,   106,   246,    -1,   246,
      88,   246,    -1,   246,   124,   246,    -1,   246,   114,   246,
      -1,   246,    15,   246,    -1,   246,    10,   246,    -1,   246,
     117,   246,    -1,   246,    23,   246,    -1,   125,   246,    -1,
     120,   246,    -1,    43,   246,    -1,    53,   246,    -1,   122,
     246,    -1,   104,   246,    -1,   246,    60,   246,    -1,   246,
      60,   168,    -1,   258,    60,   246,    -1,   258,    60,   168,
      -1,   246,    64,   246,    -1,   246,    64,   168,    -1,   258,
      64,   246,    -1,   258,    64,   168,    -1,   125,    -1,   130,
      -1,    88,    -1,   124,    -1,   103,    -1,   105,    -1,   106,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   426,   426,   431,   432,   438,   439,   444,   445,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   474,   475,   476,   477,   481,   483,   488,
     489,   490,   505,   506,   511,   512,   517,   522,   527,   531,
     537,   542,   546,   551,   555,   556,   557,   561,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   584,   585,   589,   593,   594,   598,
     599,   603,   604,   608,   609,   610,   611,   612,   613,   614,
     618,   619,   623,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     660,   666,   672,   678,   685,   695,   699,   700,   701,   702,
     706,   707,   711,   714,   719,   720,   724,   726,   728,   735,
     740,   748,   753,   758,   766,   767,   772,   773,   775,   780,
     790,   799,   803,   811,   812,   817,   822,   816,   847,   853,
     860,   868,   879,   885,   878,   913,   917,   922,   926,   934,
     935,   939,   940,   941,   942,   943,   944,   945,   946,   947,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,   961,   962,   963,   964,   968,   969,   970,
     971,   972,   973,   974,   975,   976,   977,   978,   979,   983,
     984,   988,   992,   993,   994,   998,  1000,  1002,  1004,  1009,
    1010,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1026,  1027,  1028,  1029,  1033,  1034,  1038,  1039,  1040,  1041,
    1042,  1043,  1047,  1048,  1051,  1052,  1056,  1057,  1061,  1063,
    1068,  1069,  1073,  1074,  1078,  1079,  1083,  1085,  1087,  1092,
    1105,  1122,  1123,  1125,  1130,  1138,  1146,  1154,  1163,  1173,
    1174,  1175,  1179,  1180,  1188,  1190,  1196,  1201,  1203,  1205,
    1210,  1212,  1214,  1221,  1222,  1223,  1227,  1228,  1233,  1234,
    1235,  1236,  1256,  1260,  1264,  1272,  1276,  1277,  1278,  1282,
    1284,  1290,  1292,  1294,  1299,  1300,  1301,  1302,  1303,  1304,
    1305,  1311,  1312,  1313,  1314,  1318,  1319,  1320,  1324,  1325,
    1329,  1330,  1334,  1335,  1339,  1340,  1341,  1342,  1343,  1347,
    1358,  1359,  1360,  1361,  1362,  1363,  1365,  1367,  1369,  1371,
    1373,  1375,  1380,  1382,  1384,  1386,  1388,  1390,  1392,  1394,
    1396,  1398,  1400,  1402,  1404,  1411,  1417,  1423,  1429,  1438,
    1443,  1451,  1452,  1453,  1454,  1455,  1456,  1457,  1458,  1463,
    1464,  1468,  1473,  1476,  1481,  1486,  1489,  1494,  1498,  1499,
    1503,  1504,  1509,  1514,  1522,  1523,  1524,  1525,  1526,  1527,
    1528,  1529,  1530,  1532,  1534,  1536,  1538,  1540,  1545,  1546,
    1547,  1548,  1559,  1560,  1564,  1565,  1566,  1570,  1571,  1572,
    1580,  1581,  1582,  1583,  1587,  1588,  1589,  1590,  1591,  1592,
    1593,  1594,  1595,  1596,  1600,  1608,  1609,  1613,  1614,  1615,
    1616,  1617,  1618,  1619,  1620,  1621,  1622,  1623,  1624,  1625,
    1626,  1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,
    1639,  1640,  1641,  1642,  1643,  1644,  1648,  1649,  1650,  1651,
    1655,  1656,  1657,  1658,  1663,  1664,  1665,  1666,  1667,  1668,
    1669
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
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
  "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE", "TUNDERSCORE",
  "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR",
  "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT",
  "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH", "TLESS",
  "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS",
  "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP",
  "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR",
  "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_stmt",
  "access_control", "block_stmt", "stmt_ls", "only_ls", "opt_only_ls",
  "except_ls", "use_stmt", "require_stmt", "assignment_stmt", "opt_ident",
  "ident", "opt_ident_or_string", "do_stmt", "return_stmt",
  "class_level_stmt", "private_decl", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "try_stmt", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt", "enum_ls",
  "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
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
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", 0
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
     395,   396,   397
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   143,   144,   145,   145,   146,   146,   147,   147,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   149,   149,   150,
     150,   150,   151,   151,   152,   152,   153,   153,   153,   153,
     154,   154,   155,   155,   156,   156,   156,   157,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   159,   159,   160,   161,   161,   162,
     162,   163,   163,   164,   164,   164,   164,   164,   164,   164,
     165,   165,   166,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   168,   169,   169,   169,   169,
     170,   170,   171,   172,   173,   173,   174,   174,   174,   175,
     175,   176,   176,   176,   177,   177,   178,   178,   178,   179,
     180,   180,   180,   181,   181,   183,   184,   182,   185,   185,
     185,   185,   187,   188,   186,   189,   189,   189,   189,   190,
     190,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   193,
     193,   194,   195,   195,   195,   196,   196,   196,   196,   197,
     197,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     199,   199,   199,   199,   200,   200,   201,   201,   201,   201,
     201,   201,   202,   202,   203,   203,   204,   204,   205,   205,
     206,   206,   207,   207,   208,   208,   209,   209,   209,   210,
     210,   211,   211,   211,   212,   212,   212,   212,   212,   213,
     213,   213,   214,   214,   215,   215,   215,   216,   216,   216,
     217,   217,   217,   218,   218,   218,   219,   219,   220,   220,
     220,   220,   221,   221,   221,   221,   222,   222,   222,   223,
     223,   223,   223,   223,   224,   224,   224,   224,   224,   224,
     224,   225,   225,   225,   225,   226,   226,   226,   227,   227,
     228,   228,   229,   229,   230,   230,   230,   230,   230,   231,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   234,
     235,   236,   236,   236,   236,   236,   236,   236,   236,   237,
     237,   238,   239,   239,   240,   241,   241,   242,   242,   242,
     243,   243,   244,   245,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   247,   247,
     247,   247,   248,   248,   249,   249,   249,   250,   250,   250,
     251,   251,   251,   251,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   253,   253,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     255,   255,   255,   255,   255,   255,   256,   256,   256,   256,
     257,   257,   257,   257,   258,   258,   258,   258,   258,   258,
     258
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     3,     1,     5,     6,     0,
       1,     1,     2,     3,     1,     2,     1,     3,     3,     5,
       0,     1,     1,     1,     3,     5,     5,     3,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     0,     1,     1,     1,     1,     2,
       1,     2,     3,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     2,     5,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     4,     3,     6,     5,
       2,     2,     3,     5,     0,     2,     3,     2,     3,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       1,     2,     3,     1,     3,     0,     0,     8,     0,     1,
       2,     2,     0,     0,    10,     3,     3,     5,     5,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     3,     0,     1,     3,     4,     4,     6,     6,     0,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     2,     1,
       0,     1,     2,     2,     0,     2,     3,     4,     4,     2,
       4,     0,     2,     2,     4,     4,     4,     5,     4,     0,
       1,     1,     1,     3,     3,     4,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     0,     4,     0,     2,
       2,     1,     4,     4,     6,     1,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       1,     4,     1,     3,     4,     1,     3,     2,     3,     3,
       1,     3,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     4,     3,     4,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    36,    76,   404,   405,   406,   407,
     408,     0,   359,    74,   131,   359,     0,   260,    74,     0,
       0,     0,     0,    74,    74,     0,     0,     0,     0,   149,
       0,   145,     0,     0,     0,     0,   350,     0,     0,     0,
       0,   259,   259,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   133,     0,     0,     0,
     456,   458,     0,   459,   460,   387,     0,     0,   457,   454,
      83,   455,     0,     0,     0,     4,     0,     5,     9,     0,
      10,    11,    12,    14,   319,    21,    13,    84,    15,    17,
      16,    19,    20,    18,    89,     0,    87,   379,     0,    90,
      88,    91,     0,   388,   375,   376,   322,   320,     0,     0,
     380,   381,     0,   321,     0,   389,   390,   391,   374,   324,
     323,   377,   378,     0,    23,   330,     0,     0,   360,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,   379,
     388,   320,   380,   381,   359,   321,   389,   390,     0,     0,
       0,     0,   310,     0,    78,    77,   150,    92,     0,   151,
       0,     0,     0,     0,     0,     0,   310,     0,     0,     0,
       0,     0,   262,    30,   442,   372,     0,   443,     7,   310,
     260,   261,    86,    85,   239,   302,     0,   301,    81,     0,
       0,     0,    80,    33,     0,   325,     0,   310,    34,   331,
       0,   120,   121,   251,     0,     0,   301,     0,     0,   445,
     386,   441,   444,   440,    42,    44,     0,     0,   305,     0,
     307,     0,     0,   306,     0,   301,     0,     0,     6,     0,
     134,   225,   224,   152,     0,     0,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   385,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   310,   310,     0,     0,     0,    24,
      25,     0,    26,     0,     0,     0,     0,     0,     0,     0,
      27,    28,     0,   319,   317,     0,   311,   312,   318,     0,
       0,     0,     0,     0,   101,     0,     0,   100,     0,     0,
       0,   107,     0,     0,   117,     0,    29,   202,   146,   267,
       0,   268,   270,     0,   281,     0,     0,   273,     0,     0,
      31,     0,   151,   238,     0,    57,    82,   124,    79,    32,
     310,     0,   122,     0,   249,   246,    54,     0,    50,    94,
      35,    43,    45,     0,   409,     0,     0,   400,     0,   402,
       0,     0,     0,     0,     0,     0,   413,     8,     0,     0,
       0,   220,     0,     0,     0,     0,     0,   358,   437,   436,
     439,   447,   446,   451,   450,   433,   430,   431,   432,   383,
     420,   399,   398,   397,   384,   424,   435,   429,   427,   438,
     428,   426,   418,   423,   425,   434,   417,   421,   422,   419,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   449,   448,
     453,   452,   214,   211,   212,   213,   217,   218,   219,     0,
       0,   362,     0,     0,     0,     0,     0,     0,     0,     0,
     411,   359,   359,    97,   247,     0,     0,     0,   327,     0,
     143,     0,   140,   248,   134,     0,     0,     0,   334,     0,
       0,     0,   340,     0,     0,     0,   108,   116,     0,     0,
     326,     0,   203,     0,   210,   226,     0,   271,     0,   285,
       0,   280,   375,     0,     0,   264,   373,   263,   396,   304,
     303,     0,     0,   328,     0,   253,   375,     0,   455,    53,
       0,    46,    51,     0,   410,   382,   401,   308,   403,   309,
       0,     0,   412,   113,   344,     0,   415,   414,     0,     0,
     135,   136,   221,   222,   223,   153,     0,     0,   255,   254,
     256,   258,    58,    65,    66,    67,    62,    64,    72,    73,
      60,    63,    61,    59,    69,    68,    70,    71,   394,   395,
     215,   216,   367,     0,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,   316,   314,
     315,   313,     0,   141,   139,     0,     0,   115,     0,     0,
      99,     0,    98,     0,     0,   105,     0,     0,   103,     0,
       0,   365,     0,     0,   119,   209,   201,     0,   294,   227,
     230,   229,   231,     0,   269,   272,     0,   273,     0,   265,
     274,   275,     0,     0,   123,   125,   329,     0,   250,     0,
      55,     0,    56,     0,     0,     0,     0,   114,     0,    37,
       0,   259,   226,   184,   182,   187,   194,   195,   196,   191,
     193,   189,   192,   190,   188,   198,   197,   163,   166,   164,
     165,   176,   167,   180,   172,   170,   183,   171,   169,   174,
     179,   181,   168,   173,   177,   178,   175,   185,   186,     0,
     161,     0,   199,   199,   159,   257,   363,   388,   388,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    95,   144,
     142,   136,   102,     0,     0,   333,     0,   332,     0,     0,
     339,   106,     0,   338,   104,     0,   364,   118,   349,   204,
       0,     0,   273,   228,   244,   266,     0,     0,     0,   127,
       0,     0,    48,    47,     0,   111,   346,     0,     0,   109,
     345,     0,   416,    38,    74,   259,   259,   129,   259,   137,
       0,   162,     0,     0,   202,   155,   156,     0,     0,     0,
       0,     0,     0,     0,     0,   259,   337,     0,     0,   343,
       0,     0,   366,   294,   297,   298,   299,     0,   296,   300,
     375,   240,   206,   205,     0,     0,     0,   283,   375,   128,
     126,     0,     0,   112,     0,   110,   138,   232,   160,   161,
     199,   199,     0,     0,     0,     0,     0,     0,     0,   130,
       0,     0,     0,     0,   273,   286,     0,   241,   243,   242,
     245,   236,   237,   147,     0,    49,     0,     0,   233,   244,
     157,   158,   200,     0,     0,     0,     0,     0,     0,   336,
     335,   342,   341,   208,   207,   288,   289,   291,   375,     0,
     411,   375,   348,   347,     0,     0,   290,   292,   234,   154,
     235,   286,   293
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   215,    76,   487,    78,    79,    80,   216,
     519,   523,   520,    81,    82,    83,   155,    84,   159,   193,
      85,    86,    87,    88,    89,   583,    90,    91,    92,    93,
     511,   635,    94,    95,   380,   651,    96,   471,   472,   139,
     168,   495,    98,    99,   381,   652,   545,   691,   692,   693,
     765,   328,   491,   492,   493,   449,   546,   233,   623,   839,
     869,   833,   185,   828,   792,   795,   100,   204,   354,   101,
     102,   171,   172,   332,   333,   505,   336,   337,   501,   856,
     789,   732,   217,   221,   222,   305,   306,   307,   140,   104,
     105,   106,   141,   108,   127,   128,   450,   322,   610,   451,
     109,   142,   143,   112,   145,   114,   146,   147,   117,   118,
     226,   119,   120,   121,   122,   123
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -757
static const yytype_int16 yypact[] =
{
    -757,    85,  2252,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  3518,   115,   149,  -757,   115,  7465,    63,   149,  7465,
    3518,    73,   149,    90,   347,  6105,  7465,  7465,    78,  -757,
     149,  -757,    41,  3518,  7465,  7465,  -757,  7465,  7465,   213,
      88,   763,   825,  -757,  6377,  6513,  7465,  6649,  7465,   214,
     157,  3518,  7465,  3518,  3518,   149,  -757,  6377,  7465,  7465,
    -757,  -757,  7465,  -757,  -757,  8825,  7465,  7465,  -757,  7465,
    -757,  -757,  2702,  5697,  6377,  -757,  3382,  -757,  -757,   286,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,   149,  -757,   -37,   165,  -757,
    -757,  -757,   136,   191,  -757,  -757,  -757,   204,   207,   208,
     221,   238, 13049,  1803,   236,   246,   268,  -757,  -757,  -757,
    -757,  -757,  -757,   155,  -757, 13049,   267,  3518,  -757,   280,
    -757,   270,  7465,  7465,  7465,  7465,  7465,  6377,  6377,   283,
    -757,  -757,  -757,  -757, 10424,   301,  -757,  -757,   149,   293,
   10686,   341,  6377,   295,  -757,  -757,  -757,  -757,   149,    56,
     149,   297,    35,  9501,  9463,  9618,  6377,  3518,   298,    54,
      64,    34,  -757,  -757,   321,   328,  9656,   321,  -757,  6377,
    -757,   285,  -757,  -757,   149,  -757,   186, 13049,  -757, 10720,
   10295,  3518,  -757,  -757,  9656, 13049,   318,  6377,  -757, 13049,
   10809,  -757,  -757,   368,   332,   201, 10536,  9656, 10843,   437,
    1451,   321,   437,   321,  -757,  -757,  2838,   -35,  -757,  7465,
    -757,   -28,   -27, 13049,   206, 10932,   -33,   455,  -757,   149,
     361,  -757,  -757,  -757,    46,    41,    41,    41,  -757,  7465,
    7465,  7465,  7465,  6785,  6785,  7465,  7465,  7465,  7465,  7465,
    7465,    83,  8825,  7465,  7465,  7465,  7465,  7465,  7465,  7465,
    7465,  7465,  7465,  7465,  7465,  7465,  7465,  7465,  7465,  7465,
    7465,  7465,  7465,  7465,  7465,  7465,  7465,  7465,  7465,  7465,
    7465,  7465,  7465,  7465,  6377,  6377,  6785,  6785,  5561,  -757,
    -757,  2974,  -757, 10574, 10648, 10994,    40,  6785,    35,   345,
    -757,  -757,  7465,   324,  -757,   336,   367,  -757, 13049,   149,
     349,   149,   445,  6377,  -757,  3654,  6785,  -757,  3790,  6785,
     348,  -757,    35,  3518,   457,   351,  -757,    68,  -757,  -757,
      54,  -757,   377,   353,  -757,  2111,   399,   405,  7465,    41,
    -757,   358,  -757,  -757,  6377,  -757,  -757,  -757,  -757,  -757,
    6377,   359,  -757,  2397,   390,  -757,  -757,  7601,  7465,  -757,
    -757,  -757,  -757,  6241,  -757,  9351,  5833,  -757,  5969,  -757,
    6785,  5150,  2566,   360,  7465,  5424,  -757,  -757,   366,  6377,
     370,   317,    41,   235,   243,   274,   275, 10357, 13159, 13159,
     -23,  -757,   -23,  -757,   -23, 13188,  1274,  1166,   870,   328,
     437,  -757,  -757,  -757,  1451,  1014,   -23,  1179,  1179, 13159,
    1179,  1179,   819,   437,  1014,  9315,   819,   321,   321,   437,
   11056, 11118, 11180, 11242, 11304, 11366, 11428, 11490, 11552, 11614,
   11676, 11738, 11800, 11862, 11924, 11986,   365,   372,  -757,   -23,
    -757,   -23,   160,  -757,  -757,  -757,  -757,  -757,  -757,   149,
     -26,  -757, 13111,   257,  7737,  6785,  7873,  6785,  7465,  6785,
    8985,   115, 12048,  -757,  -757, 12110,  7465,  6377,  -757,  6377,
     418,    92,  -757,  -757,   361,  7465,    -5,  7465, 13049,    36,
    9690,  7465, 13049,    53,  9539,  5561,  -757,   486, 12181,  3518,
    -757,    38,  -757,    47,  -757,   277,   378,    54,   192,  -757,
    6377,  -757,   398,  7465,  6921,  -757, 13049,  -757,  -757,  -757,
   13049,    14,   379,  -757,  6377,  -757,   278,   149,   387,   412,
     394, 12243,   412,   397,  -757,  -757,  -757,  -757,  -757,  -757,
      -6,  1506,  -757,  -757, 13049,  3518, 13049,  -757, 12305,  3110,
     419,  -757,  -757,  -757,  -757,  -757,  9036,   279,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  5561,  -757,  6785,  6785,  7465,   504, 12367,
    7465,   509, 12429,   391,  9201,    35,    35,  -757, 13049,  -757,
   13049,  -757,  7465,   149,  -757,   400,  9656,  -757,  9779,  3926,
    -757,  4062,  -757,  9841,  4198,  -757,    35,  4334,  -757,    35,
      58,  -757,  3518,  7465,  -757,   615,  -757,    54,   429,   476,
    -757,  -757,  -757,    31,  -757,  -757,  2397,   405,    42, 13049,
    -757, 13049,  4470,  6377,  -757,  -757,  -757,   176,  -757,  7465,
    -757,  7465,  -757,  4606,   401,  4742,   402,  -757,  7465,  -757,
    3246,   582,   277,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,   149,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  7465,
     428,   433,   410,   410,  -757,  -757,  -757,    59,   187, 12491,
    8009,  8145, 12553,  8281,  8417,  8553,  8689,  -757,  -757, 13049,
    -757,  -757,  -757,  3518,  7465, 13049,  7465, 13049,  3518,  7465,
   13049,  -757,  7465, 13049,  -757,  5561,  -757,  -757, 13049,  -757,
     416,  7057,    28,  -757,   467,  -757,  6785,  4878,  3518,  -757,
      55,  5014, 12615, 13049,  7465,  -757, 13049,  3518,  7465,  -757,
   13049,  3518, 13049,  -757,   515,   716,   716,  -757,   713,  -757,
      91,  -757,  9389,  9084,    68,  -757,  -757,  7465,  7465,  7465,
    7465,  7465,  7465,  7465,  7465,  1407, 12181,  9908,  9970, 12181,
   10037, 10099,  -757,   429,    73,  7465,  7465,  5287,  -757,  -757,
     198,  6377,  -757,  -757,  7465,   -18,  9239,  -757,   479,   341,
    -757,  7465, 10166,  -757, 10228,  -757,  -757,   485,  -757,  -757,
     410,   410,   195, 12677, 12739, 12801, 12863, 12925, 12987,  -757,
    3518,  3518,  3518,  3518,    28,  7193,   177,  -757,  -757, 13049,
   13049,  -757,  -757,  -757,  8689, 13049,  3518,  3518,  -757,   467,
    -757,  -757,  -757,  7465,  7465,  7465,  7465,  7465,  7465, 12181,
   12181, 12181, 12181,  -757,  -757,  -757,  -757,  -757,   288,  6785,
    8911,   537, 12181, 12181,   -38,  9277,  -757,  -757,  -757,  -757,
    -757,  7329,  -757
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -757,  -757,  -757,     1,  -639,  1813,  -757,  -757,  1184,    24,
     212,  -757,  -757,  -757,  -757,  -757,   194,  1898,    -1,    -2,
    -752,  -630,   -40,  -757,  -757,    93,  -757,  -757,  -757,  -757,
    -757,  -757,  -757,   413,    99,  -137,  -757,  -757,   -16,   364,
    -757,  -757,  -757,  -757,  -757,  -757,  -757,  -757,  -188,  -184,
    -626,  -757,  -179,   -29,  -757,  -314,  -757,  -757,   -63,  -757,
    -757,  -272,   948,  -757,  -227,  -240,  -757,  -141,  -757,  -757,
    -757,  -206,   264,  -757,  -319,  -618,  -757,  -480,  -343,  -658,
    -756,  -178,     9,   101,  -757,  -127,  -757,   139,   338,  -315,
    -757,  -757,   506,  -757,    -9,  -757,  -757,  -205,  -757,  -443,
    -757,   855,   890,   -11,  1050,  -757,  1195,  1205,  -757,  -757,
    -757,  -757,  -757,  -757,  -757,  -284
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -412
static const yytype_int16 yytable[] =
{
     125,   182,   183,    75,   453,   144,   131,   299,   150,   735,
     515,   496,   758,   494,   163,   164,   165,   310,   627,   373,
     502,   759,   156,   174,   175,    45,   176,   177,   383,   384,
     385,   386,   334,   187,   189,   190,   194,   195,   516,   325,
     199,   200,   611,   832,     5,    45,   206,   207,   208,     5,
       5,   209,   341,   186,   210,   211,   212,     5,   213,   191,
     599,   187,   223,   225,   632,   334,   205,   766,   338,   857,
     351,  -209,    14,   363,   459,   375,   736,   604,   320,   191,
     366,   368,   573,   224,   249,     3,     5,   251,   442,   868,
    -355,   254,   334,     5,  -278,   633,   291,   154,  -278,  -393,
     364,  -393,   443,   344,   867,   444,   376,   382,   401,   367,
     369,   574,   832,  -278,   793,    43,   291,   504,   162,   445,
     446,   125,   293,   294,   295,   199,   187,   225,   806,   447,
     696,   329,   597,   643,    56,   298,   758,   320,   626,   148,
     791,   308,   339,   734,   448,   759,   615,   296,   371,   634,
     371,  -276,     5,  -278,  -278,   308,   234,   436,   437,   402,
     314,   317,   321,   344,  -278,  -278,   725,  -368,   308,   291,
     291,   335,  -278,  -278,   340,   616,   547,   169,   625,   460,
     342,   737,   169,   617,   840,   841,   308,   291,   235,   291,
     330,  -278,   349,   334,   570,   726,  -368,   236,   626,   126,
     593,   453,   866,   231,  -209,   359,   854,   129,   365,   152,
     736,   859,   149,   872,   166,   286,   237,   362,  -278,   287,
     178,   571,   232,   512,   179,  -278,  -278,   594,   387,   388,
     389,   390,   392,   394,   395,   396,   397,   398,   399,   400,
     370,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   308,   308,   439,   441,   452,   606,   609,
     807,  -278,   782,   196,   371,   371,   462,  -295,     5,   453,
     320,   465,   154,   197,   344,  -369,   463,   619,   730,   626,
    -278,   494,   187,   615,   478,   480,  -295,   482,   484,   344,
    -295,   502,   488,   345,   371,   741,   860,   576,  -352,  -278,
     486,   287,   476,   -74,  -369,   644,   646,   506,   356,   620,
     229,  -351,   842,   510,   238,  -295,   391,   393,   621,   308,
     103,   239,   -74,   339,   242,   372,   521,   521,  -356,   103,
       5,   339,   510,   622,   154,   223,   157,   223,   103,   531,
     510,   534,   548,   536,   538,  -357,    97,  -261,   187,   542,
     549,   103,   284,  -354,   285,    97,   638,  -287,   543,   438,
     440,   243,   339,   339,    97,   244,  -252,   339,   540,   103,
     461,   103,   103,   544,   797,  -353,  -287,    97,   797,  -261,
    -287,   550,   551,   288,   291,  -252,   695,   290,  -261,   479,
     103,   466,   483,   467,   103,    97,   790,    97,    97,  -393,
     300,  -393,   798,   158,   302,  -287,   798,  -261,   249,   309,
     250,   251,  -279,   313,   327,   254,    97,  -392,   251,  -392,
      97,   453,   261,   478,   579,   482,   582,   488,   584,   534,
     494,   267,   585,   586,   350,   588,   590,   353,   308,   355,
     242,  -279,   377,   530,   596,   103,   598,   527,   379,   529,
     603,  -279,   464,   468,   452,   469,   473,   600,   602,   475,
    -279,   605,   608,   489,   485,   497,   503,  -279,   490,   225,
     498,    97,   629,   631,   504,   508,   513,   243,   517,   535,
     539,   244,   568,   225,   541,   103,  -279,   592,   107,   628,
     858,   569,   612,  -282,   -52,   624,   636,   107,     5,   861,
     639,   640,   154,   637,   642,  -279,   107,   344,   700,   103,
     705,    97,  -279,   703,   711,  -261,   731,   733,  -319,   107,
     747,   751,  -282,   763,   249,   858,   764,   251,   578,   794,
     581,   254,  -282,   783,   103,    97,   858,   107,   838,   107,
     107,  -282,   452,   650,   392,   439,   699,  -261,  -282,   702,
     522,  -284,   311,   595,   775,   810,  -261,   710,   107,   811,
      97,   709,   107,   707,   708,   812,   729,  -282,   715,   760,
     717,   158,   870,   720,   712,  -261,   723,   853,    14,   864,
    -284,    17,   728,   507,   721,   824,  -282,   724,   591,    22,
    -284,    23,   754,  -282,     0,     0,     0,     0,    29,  -284,
    -148,     0,   187,     0,     0,     0,  -284,     0,   742,   103,
     743,     0,   746,   107,   750,   442,    39,   752,   755,  -148,
     756,    43,   740,     0,     0,  -284,     0,     0,     0,   443,
       0,   362,   444,   103,     0,    97,   103,     0,    55,     0,
      56,   103,     0,     0,  -284,     0,   445,   446,   391,   438,
       0,  -284,     0,   107,     0,     0,   447,     0,   762,    97,
       0,     0,    97,     0,     0,     0,     0,    97,     0,   715,
     717,   448,   720,   723,   746,   750,     0,   107,     0,     0,
       0,     0,   776,   777,     0,   778,     0,   779,   780,    70,
     103,   781,     0,     0,   452,   182,   183,   757,     0,     0,
       0,     0,   107,     0,     0,   796,   534,     0,     0,    14,
     534,     0,    17,   802,     0,   180,    97,   804,   800,     0,
      22,     0,    23,   754,     0,    23,   181,     0,     0,    29,
       0,  -148,    29,     0,  -148,     0,   776,   813,   814,   779,
     815,   816,   817,   818,     0,     0,     0,   227,     0,   755,
    -148,   756,    43,  -148,   195,   199,   225,     0,     0,     0,
     829,     0,   180,   830,     0,     0,     0,   572,     0,    55,
     835,    56,    23,   181,     0,     0,   826,   107,     0,    29,
       0,  -148,     0,     0,     0,     0,     0,   -41,     0,   849,
     850,   851,   852,     0,     0,     0,     0,     0,     0,     0,
    -148,   107,     0,   750,   107,   862,   863,   103,     0,   107,
       0,     0,   849,   850,   851,   852,   862,   863,     0,     0,
      70,     0,   242,     0,   180,     0,     0,     0,   865,   534,
       0,     0,     0,    97,    23,   181,     0,   110,     0,     0,
     750,    29,     0,  -148,     0,     0,   110,     0,     0,   -40,
       0,     0,     0,   103,     0,   110,     0,   103,   107,   243,
       0,     0,  -148,   244,   694,     0,     0,     0,   110,     0,
       0,     0,   111,   242,     0,     0,     0,     0,     0,    97,
       0,   111,     0,    97,     0,     0,   110,     0,   110,   110,
     111,     0,     0,   697,   698,     0,     0,     0,     0,     0,
       0,     0,   246,   111,   247,   248,   249,   110,   250,   251,
     243,   110,     0,   254,   244,     0,     0,   103,     0,   103,
     261,   111,   103,   111,   111,   103,     0,   265,   266,   267,
     103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,    97,     0,    97,   111,     0,    97,     0,
     103,    97,     0,   246,     0,     0,    97,   249,     0,   250,
     251,   103,   110,   103,   254,     0,     0,     0,   103,     0,
       0,   261,     0,     0,     0,   107,    97,     0,   265,   266,
     267,     0,     0,     0,     0,     0,     0,    97,     0,    97,
       0,     0,     0,     0,    97,     0,     0,   111,     0,     0,
       0,   220,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,   107,     0,     0,     0,   107,   110,     0,     0,     0,
       0,   103,   113,     0,     0,     0,   103,   111,     0,     0,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   110,     0,     0,   243,     0,   103,    97,   244,     0,
       0,   111,    97,   113,     0,   103,     0,     0,     0,   103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     304,   113,    97,   113,   113,   107,   111,   107,     0,     0,
     107,    97,     0,   107,   304,    97,     0,   246,   107,   247,
     248,   249,   113,   250,   251,   252,   113,   304,   254,   255,
     256,     0,   258,   259,   260,   261,     0,     0,   107,   264,
       0,     0,   265,   266,   267,   304,   110,     0,     0,   107,
       0,   107,     0,     0,     0,     0,   107,     0,   103,   103,
     103,   103,     0,     0,     0,     0,     0,     0,     0,     0,
     110,     0,     0,   110,   103,   103,     0,   113,   110,     0,
       0,   111,     0,     0,    97,    97,    97,    97,     0,   242,
       0,     0,     0,     0,     0,     0,     0,   115,     0,     0,
      97,    97,   242,     0,     0,   111,   115,   116,   111,     0,
       0,     0,     0,   111,     0,   115,   116,   113,     0,   107,
       0,     0,     0,     0,   107,   116,   243,   110,   115,     0,
     244,   192,   304,   304,     0,     0,     0,     0,   116,   243,
       0,   113,     0,   244,   107,     0,   115,     0,   115,   115,
       0,     0,     0,   107,     0,     0,   116,   107,   116,   116,
       0,     0,   111,     0,     0,     0,   113,   115,     0,   246,
       0,   115,   248,   249,     0,   250,   251,   116,     0,     0,
     254,   116,   246,     0,   247,   248,   249,   261,   250,   251,
     252,     0,   509,   254,   265,   266,   267,   242,   304,   260,
     261,     0,     0,     0,   264,     0,     0,   265,   266,   267,
       0,   509,     0,     0,   220,   292,   220,     0,     0,   509,
       0,     0,   115,     0,     0,     0,   107,   107,   107,   107,
       0,     0,   116,     0,   243,     0,     0,     0,   244,     0,
       0,   113,   107,   107,   110,     0,   192,   192,   192,   324,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,     0,   115,     0,     0,   113,     0,     0,   113,     0,
       0,     0,   116,   113,     0,     0,     0,     0,   192,   111,
       0,   249,     0,   250,   251,     0,   115,     0,   254,     0,
     110,   192,     0,     0,   110,   261,   116,     0,     0,     0,
       0,     0,   265,   266,   267,     0,     0,     0,     0,     0,
       0,   115,     0,     0,     0,   589,     0,   304,     0,     0,
       0,   116,   113,    14,     0,   111,    17,     0,     0,   111,
       0,     0,     0,     0,    22,     0,    23,   754,     0,     0,
       0,     0,     0,    29,     0,  -148,     0,     0,     0,     0,
       0,     0,     0,     0,   110,     0,   110,     0,     0,   110,
       0,    39,   110,   755,  -148,   756,    43,   110,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,   192,    55,     0,    56,   115,   110,     0,   111,
       0,   111,     0,     0,   111,     0,   116,   111,   110,     0,
     110,     0,   111,     0,     0,   110,   192,     0,     0,     0,
     115,   243,     0,   115,     0,   244,   240,     0,   115,     0,
     116,   241,   111,   116,     0,     0,     0,     0,   116,   242,
       0,     0,     0,   111,    70,   111,     0,     0,     0,   113,
     111,     0,   819,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,     0,     0,     0,   254,   243,   115,   110,     0,
     244,   260,   261,   110,     0,     0,   264,   116,     0,   265,
     266,   267,     0,     0,     0,   113,     0,     0,     0,   113,
     320,     0,     0,   110,   245,     0,     0,     0,     0,     0,
       0,     0,   110,   111,     0,     0,   110,     0,   111,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   111,   262,
     263,   264,     0,     0,   265,   266,   267,   111,     0,     0,
       0,   111,     0,     0,     0,   645,     0,     0,     0,   113,
       0,   113,     0,     0,   113,     0,     0,   113,     0,     0,
       0,     0,   113,   192,   192,     0,     0,   192,   192,     0,
       0,     0,     0,     0,     0,   110,   110,   110,   110,   788,
       0,     0,   113,     0,   115,     0,     0,     0,     0,     0,
       0,   110,   110,   113,   116,   113,     0,     0,     0,     0,
     113,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   111,   111,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,   111,     0,     0,
     115,     0,     0,     0,   115,     0,     0,     0,     0,   827,
     116,     0,     0,     0,   116,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   113,     0,     0,     0,     0,   113,   192,
     192,     0,     0,   855,     0,     0,     0,     0,     0,     0,
     192,     0,   324,     0,     0,     0,     0,   324,   113,     0,
     192,     0,     0,   192,   115,     0,   115,   113,     0,   115,
       0,   113,   115,     0,   116,     0,   116,   115,   855,   116,
       0,     0,   116,     0,     0,    77,     0,   116,     0,   855,
       0,     0,     0,     0,   124,     0,     0,   115,     0,     0,
       0,     0,     0,   151,     0,     0,     0,   116,   115,     0,
     115,     0,     0,     0,     0,   115,   173,     0,   116,     0,
     116,     0,     0,     0,     0,   116,     0,     0,     0,     0,
       0,     0,     0,     0,   198,     0,   201,   202,     0,     0,
     113,   113,   113,   113,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    77,   113,   113,     0,   228,
       0,     0,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,     0,     0,   115,     0,
       0,   130,     0,   115,     0,     0,   130,     0,   116,     0,
     153,   130,   130,   116,   192,     0,     0,     0,   167,     0,
     170,     0,     0,   115,   282,   283,  -370,     0,     0,  -392,
     289,  -392,   115,   116,     0,     0,   115,     0,     0,     0,
       0,     0,   116,   203,     0,     0,   116,     0,     0,     0,
       0,   324,   324,     0,   324,   324,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   831,
     326,     0,     0,     0,     0,     0,   324,     0,   324,     0,
       0,     0,     0,   230,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   115,   115,   115,   115,     0,
       0,     0,     0,     0,     0,   116,   116,   116,   116,    77,
       0,   115,   115,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   116,     0,     0,     0,   203,     0,   831,     0,
     303,     0,     0,     0,     0,     0,   203,     0,   312,     0,
       0,     0,     0,     0,   303,     0,     0,   331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,   130,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,     0,   499,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,   378,   348,     0,
       0,   348,   170,   170,   170,   170,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,   403,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,   303,   303,     0,   533,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,   470,     0,   474,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,   331,     0,
       0,    66,     0,    67,     0,    68,    69,   170,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,   303,   500,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -259,    18,    19,     0,    20,    21,     0,    22,
     170,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,   614,     0,  -259,    38,    39,    40,    41,  -148,
      42,    43,     0,  -259,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -259,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,   647,     0,
       0,     0,    77,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,   303,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,    73,     0,
      74,   618,     0,     0,     0,   331,     0,     0,   499,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,   348,     0,   348,   203,     0,   348,     0,     0,
     348,     0,    21,     0,     0,   727,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,   690,   739,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,   745,     0,   749,     0,
       0,     0,     0,    77,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,   331,     0,    66,     0,    67,
       0,    68,    69,     0,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   514,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   799,     0,     0,     0,     0,     0,     0,     0,     0,
     803,     0,     0,     0,   805,     0,   761,     4,     0,     5,
       6,     7,     8,     9,    10,     0,  -411,     0,    11,    12,
      13,  -411,    14,    15,    16,    17,  -259,    18,    19,  -411,
      20,    21,  -411,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -259,    38,
       0,    40,    41,  -148,    42,    43,  -411,  -259,    44,    45,
    -411,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -259,     0,     0,    58,
       0,    59,   130,     0,  -411,     0,     0,     0,     0,     0,
       0,   809,     0,     0,     0,     0,     0,     0,     0,  -411,
      62,  -411,  -411,  -411,  -411,  -411,  -411,  -411,     0,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,    67,  -411,
    -411,  -411,     0,    70,  -411,  -411,  -411,     0,     0,     0,
      72,  -411,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -259,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -259,    38,    39,    40,    41,  -148,
      42,    43,     0,  -259,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -259,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,   184,    70,
       0,     0,    71,     0,     0,     0,    72,   214,    73,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -259,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -259,    38,    39,    40,    41,  -148,    42,    43,     0,  -259,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -259,     0,
       0,    58,     0,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,   361,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -259,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -259,    38,    39,    40,
      41,  -148,    42,    43,     0,  -259,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -259,     0,     0,    58,     0,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,    70,     0,     0,    71,     0,     0,     0,    72,   214,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -259,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -259,    38,    39,    40,    41,  -148,    42,    43,
       0,  -259,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -259,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,   649,    73,     4,    74,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -259,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -259,    38,
      39,    40,    41,  -148,    42,    43,     0,  -259,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -259,     0,     0,    58,
       0,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,    64,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,   753,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -259,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -259,    38,   227,    40,    41,  -148,
      42,    43,     0,  -259,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -259,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,    73,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -259,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -259,    38,     0,    40,    41,  -148,    42,    43,     0,  -259,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -259,     0,
       0,    58,     0,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,     0,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -259,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   477,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -259,    38,     0,    40,
      41,  -148,    42,    43,     0,  -259,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -259,     0,     0,    58,     0,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,    70,     0,     0,    71,     0,     0,     0,    72,     0,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -259,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   481,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -259,    38,     0,    40,    41,  -148,    42,    43,
       0,  -259,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -259,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,    73,     4,    74,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -259,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   714,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -259,    38,
       0,    40,    41,  -148,    42,    43,     0,  -259,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -259,     0,     0,    58,
       0,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,    64,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,     0,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -259,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   716,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -259,    38,     0,    40,    41,  -148,
      42,    43,     0,  -259,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -259,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,    73,     4,
      74,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -259,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   719,     0,    28,    29,     0,  -148,    30,    31,    32,
      33,    34,   -39,    35,    36,     0,    37,     0,     0,     0,
    -259,    38,     0,    40,    41,  -148,    42,    43,     0,  -259,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,    55,     0,    56,    57,  -259,     0,
       0,    58,     0,    59,     0,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,    62,    63,    64,     0,     0,     0,     0,    65,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      67,     0,    68,    69,     0,    70,     0,     0,    71,     0,
       0,     0,    72,     0,    73,     4,    74,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -259,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   722,     0,    28,
      29,     0,  -148,    30,    31,    32,    33,    34,   -39,    35,
      36,     0,    37,     0,     0,     0,  -259,    38,     0,    40,
      41,  -148,    42,    43,     0,  -259,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
      55,     0,    56,    57,  -259,     0,     0,    58,     0,    59,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,    70,     0,     0,    71,     0,     0,     0,    72,     0,
      73,     4,    74,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -259,    18,    19,     0,   738,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -148,    30,
      31,    32,    33,    34,   -39,    35,    36,     0,    37,     0,
       0,     0,  -259,    38,     0,    40,    41,  -148,    42,    43,
       0,  -259,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,    55,     0,    56,    57,
    -259,     0,     0,    58,     0,    59,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,    73,     4,    74,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -259,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   744,
       0,    28,    29,     0,  -148,    30,    31,    32,    33,    34,
     -39,    35,    36,     0,    37,     0,     0,     0,  -259,    38,
       0,    40,    41,  -148,    42,    43,     0,  -259,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,     0,    56,    57,  -259,     0,     0,    58,
       0,    59,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      62,    63,    64,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    67,     0,
      68,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,     0,    73,     4,    74,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -259,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   748,     0,    28,    29,     0,
    -148,    30,    31,    32,    33,    34,   -39,    35,    36,     0,
      37,     0,     0,     0,  -259,    38,     0,    40,    41,  -148,
      42,    43,     0,  -259,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,     0,
      56,    57,  -259,     0,     0,    58,     0,    59,     0,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    62,    63,    64,     0,
       0,     0,     0,    65,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    67,     0,    68,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,    73,   499,
      74,     5,     6,     7,     8,     9,    10,     0,  -411,     0,
     132,     0,     0,  -411,     0,     0,     0,     0,     0,     0,
       0,  -411,     0,    21,     0,     0,     0,     0,     0,   133,
     134,   135,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,  -411,     0,
       0,     0,  -411,     0,     0,    48,    49,    50,   136,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -277,  -411,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -411,    62,  -411,  -411,  -411,     0,  -411,  -411,  -411,
       0,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
      67,  -411,  -411,  -411,     0,     0,  -411,  -411,  -411,     0,
       0,     0,   137,     0,    73,   499,   514,     5,     6,     7,
       8,     9,    10,     0,  -411,     0,   132,     0,     0,  -411,
       0,     0,     0,     0,     0,     0,     0,  -411,     0,    21,
       0,     0,     0,     0,     0,   133,   134,   135,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,  -411,     0,     0,     0,  -411,     0,
       0,    48,    49,    50,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -411,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -411,    62,  -411,
    -411,  -411,     0,  -411,  -411,  -411,     0,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,    67,  -411,  -411,  -411,
       0,     0,  -411,  -411,  -411,     0,     0,     0,   137,     0,
      73,     0,   514,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   133,   134,   135,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     136,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    62,    63,    64,     0,     0,     0,
       0,    65,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    67,     0,    68,    69,   184,     0,     0,     0,
      71,     0,     0,     0,   137,     0,    73,     0,   138,   532,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,   184,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,   825,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   133,   134,   135,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   136,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    62,    63,
      64,     0,     0,     0,     0,    65,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    67,     0,    68,    69,
       0,     0,     0,     0,    71,     0,     0,     0,   137,     0,
      73,     0,   138,   537,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,   442,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,   443,    28,     0,   444,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,   445,   446,    38,     0,    40,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,     0,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,   218,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,   219,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,   184,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   135,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
     218,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,   184,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
     526,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   135,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,   218,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,   184,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,   528,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,   160,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   161,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,   184,     0,     0,
       0,    71,     0,     0,     0,   137,   524,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,   184,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   135,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,     0,
     188,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,    21,     0,     0,     0,     0,     0,
     133,   134,   135,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,    72,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   161,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,   630,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,     0,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   784,     0,     0,     0,     0,     0,   133,   134,
     135,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   785,    49,    50,   786,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,   184,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   787,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   135,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,   184,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   787,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   774,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,   184,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,     0,     0,     0,
       0,   518,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     577,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,     0,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   580,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,     0,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   768,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   769,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    67,     0,    68,
      69,     0,     0,     0,     0,    71,     0,     0,     0,   137,
       0,    73,     0,   138,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   771,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    67,     0,    68,    69,     0,     0,     0,
       0,    71,     0,     0,     0,   137,     0,    73,     0,   138,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   133,   134,
     772,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   136,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,    63,    64,     0,     0,     0,     0,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    67,
       0,    68,    69,     0,     0,     0,     0,    71,     0,     0,
       0,   137,     0,    73,     0,   138,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   133,   134,   773,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   136,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,    62,    63,    64,
       0,     0,     0,     0,    65,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    67,     0,    68,    69,     0,
       0,     0,     0,    71,     0,     0,     0,   137,     0,    73,
       0,   138,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     133,   134,   774,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   136,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    62,    63,    64,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    67,     0,    68,    69,     0,     0,     0,     0,    71,
       0,     0,     0,   137,     0,    73,     0,   138,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   133,   134,   135,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   132,     0,     0,     0,     0,    61,    62,
      63,    64,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   133,   134,   135,    66,    28,    67,     0,     0,
      69,    31,    32,     0,    34,    71,    35,    36,     0,   137,
       0,    73,     0,   138,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   136,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   132,     0,     0,
    -286,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,    62,   133,   134,   135,  -286,
      28,     0,     0,  -286,     0,    31,    32,     0,    34,     0,
      35,    36,     0,    67,     0,     0,     0,   184,    38,     5,
      40,     0,     0,     0,     0,   137,   653,    73,  -286,   787,
       0,   654,    48,    49,    50,   136,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,    62,
       0,     0,     0,     0,   653,     0,     0,     0,     0,   654,
       0,     0,     0,     0,     0,     0,     0,    67,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,   138,     0,   655,   656,   657,   658,   659,
     660,     0,     0,   661,   662,   663,   664,   665,   666,   667,
     668,   669,   670,     0,     0,   671,     0,     0,     0,   672,
     673,   674,   675,   676,   677,   678,   679,   680,   681,   682,
       0,   683,     0,     0,   684,   685,   686,   687,     0,   688,
       0,     0,   689,   655,   656,   657,   658,   659,   660,     0,
       0,   661,   662,   663,   664,   665,   666,   667,   668,   669,
     670,     0,     0,   671,     0,     0,     0,   672,   673,   674,
     675,   676,   677,   678,   679,   680,   681,   682,     0,   683,
       0,   240,   684,   685,   686,   687,   241,   688,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   243,   242,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,     0,   245,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   243,
     242,     0,     0,   244,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,     0,   262,   263,   264,   245,     0,   265,
     266,   267,     0,     0,     0,     0,     0,   243,   242,     0,
     706,   244,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   240,   262,   263,   264,   245,   241,   265,   266,   267,
       0,     0,     0,     0,   242,   243,     0,     0,   834,   244,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   240,
     262,   263,   264,   245,   241,   265,   266,   267,     0,     0,
       0,   243,   242,     0,     0,   244,   871,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,     0,   258,   259,   260,   261,     0,   262,   245,
     264,     0,     0,   265,   266,   267,     0,     0,     0,   243,
       0,     0,     0,   244,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   240,   262,   263,   264,   245,   241,   265,
     266,   267,     0,     0,     0,     0,   242,   318,   525,     0,
       0,     0,   246,     0,   247,   248,   249,   319,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   240,   262,   263,   264,     0,   241,   265,   266,   267,
       0,     0,     0,   243,   242,   315,   808,   244,     0,     0,
       0,     0,     0,     0,     0,   316,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   320,     0,   240,
       0,   245,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   243,   242,   607,     0,   244,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,     0,   262,   263,   264,   245,
       0,   265,   266,   267,     0,     0,     0,   291,     0,   243,
       0,     0,     0,   244,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   320,   262,   263,   264,   245,   240,   265,
     266,   267,     0,   241,     0,   291,     0,     0,     0,     0,
       0,   242,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,     0,   262,   263,   264,     0,   240,   265,   266,   267,
       0,   241,     0,   291,     0,     0,     0,     0,   243,   242,
     191,     0,   244,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,   241,   245,     0,     0,     0,
       0,     0,     0,   242,   601,     0,   243,     0,     0,     0,
     244,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
       0,   262,   263,   264,   245,     0,   265,   266,   267,     0,
     243,     0,   291,     0,   244,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   245,   262,
     263,   264,     0,     0,   265,   266,   267,     0,     0,   240,
     291,     0,     0,   246,   241,   247,   248,   249,     0,   250,
     251,   252,   242,   253,   254,   255,   256,   257,   258,   259,
     260,   261,     0,   262,   263,   264,     0,     0,   265,   266,
     267,     0,     0,     0,   291,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
     713,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,   718,   291,     0,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,   245,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,     0,   262,   263,   264,     0,   243,   265,
     266,   267,   244,     0,     0,   291,     0,     0,     0,   820,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,     0,   265,   266,   267,     0,
       0,   821,   291,     0,     0,     0,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,   245,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,     0,   262,   263,   264,     0,   243,   265,   266,
     267,   244,     0,     0,   291,     0,     0,     0,   822,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
     823,   291,     0,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,   245,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,     0,   262,   263,   264,     0,   243,   265,   266,   267,
     244,     0,     0,   291,     0,     0,     0,   836,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,     0,   265,   266,   267,     0,     0,   837,
     291,     0,     0,     0,     0,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,   245,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
       0,   262,   263,   264,     0,   243,   265,   266,   267,   244,
       0,     0,   291,     0,     0,     0,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,     0,   347,
       0,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,   245,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,     0,
     262,   263,   264,     0,   243,   265,   266,   267,   244,     0,
    -371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   126,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   240,   262,   263,   264,
       0,   241,   265,   266,   267,     0,     0,     0,     0,   242,
       0,     0,     0,     0,   357,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   240,   358,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,   243,   242,   454,     0,
     244,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,     0,     0,   244,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   240,   262,
     263,   264,   245,   241,   265,   266,   267,     0,     0,     0,
       0,   242,   456,     0,     0,     0,     0,   246,     0,   247,
     248,   249,   457,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   240,   262,   263,   264,
       0,   241,   265,   266,   267,     0,     0,     0,   243,   242,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,   241,   245,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   243,     0,     0,     0,
     244,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
       0,   262,   263,   264,   245,     0,   265,   266,   267,     0,
     243,     0,     0,     0,   244,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   245,   262,
     263,   264,     0,   301,   265,   266,   267,     0,     0,   240,
       0,     0,     0,   246,   241,   247,   248,   249,     0,   250,
     251,   252,   242,   253,   254,   255,   256,   257,   258,   259,
     260,   261,     0,   262,   263,   264,     0,   346,   265,   266,
     267,     0,     0,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   243,
       0,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,   243,     0,     0,     0,   244,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   245,   262,   263,   264,     0,   352,   265,   266,   267,
       0,     0,   240,     0,     0,     0,   246,   241,   247,   248,
     249,     0,   250,   251,   252,   242,   253,   254,   255,   256,
     257,   258,   259,   260,   261,     0,   262,   263,   264,     0,
     360,   265,   266,   267,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,     0,     0,   244,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,   374,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,     0,
     265,   266,   267,     0,     0,   458,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,     0,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   552,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,   553,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   554,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,   555,
     265,   266,   267,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,   556,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   557,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,   558,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   559,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,   560,
     265,   266,   267,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,   561,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   562,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,   563,   265,   266,   267,     0,
       0,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   245,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   243,   262,   263,   264,   244,   564,   265,   266,
     267,     0,     0,     0,   240,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,     0,   247,   248,   249,
       0,   250,   251,   252,     0,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   243,   262,   263,   264,   244,   565,
     265,   266,   267,     0,     0,     0,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   246,     0,   247,
     248,   249,     0,   250,   251,   252,     0,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   243,   262,   263,   264,
     244,   566,   265,   266,   267,     0,     0,     0,   240,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   245,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,   247,   248,   249,     0,   250,   251,   252,     0,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   243,   262,
     263,   264,   244,   567,   265,   266,   267,     0,     0,     0,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   126,   242,     0,     0,   245,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     243,   262,   263,   264,   244,     0,   265,   266,   267,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,   245,     0,
       0,     0,     0,     0,   242,     0,     0,   613,     0,     0,
       0,     0,     0,   246,     0,   247,   248,   249,     0,   250,
     251,   252,     0,   253,   254,   255,   256,   257,   258,   259,
     260,   261,     0,   262,   263,   264,     0,   587,   265,   266,
     267,   243,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,   240,   641,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,     0,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   243,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,   701,   648,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,     0,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,   704,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,   767,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,   770,   240,   801,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   243,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,   843,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
     844,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   243,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,   845,   240,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,   247,   248,   249,     0,
     250,   251,   252,     0,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   243,   262,   263,   264,   244,     0,   265,
     266,   267,     0,     0,   846,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,   242,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   246,     0,   247,   248,
     249,     0,   250,   251,   252,     0,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   243,   262,   263,   264,   244,
       0,   265,   266,   267,     0,     0,   847,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
     247,   248,   249,     0,   250,   251,   252,     0,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   243,   262,   263,
     264,   244,     0,   265,   266,   267,     0,     0,   848,   240,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   242,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     246,     0,   247,   248,   249,     0,   250,   251,   252,     0,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   243,
     262,   263,   264,   244,     0,   265,   266,   267,     0,     0,
       0,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   575,   262,   263,   264,   244,     0,   265,   266,   267,
       0,     0,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,   246,     0,   247,   248,   249,   243,
     250,   251,   252,   244,   253,   254,   255,   256,   257,   258,
     259,   260,   261,     0,   262,   263,   264,     0,     0,   265,
     266,   267,     0,     0,     0,     0,     0,   245,   243,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   246,     0,   247,   248,   249,     0,   250,   251,
     252,     0,   253,   254,   255,   256,     0,   258,   259,   260,
     261,     0,   262,   263,   264,     0,     0,   265,   266,   267,
       0,   246,     0,   247,   248,   249,     0,   250,   251,   252,
       0,   253,   254,   255,   256,     0,   258,   259,   260,   261,
       0,   262,     0,   264,     0,     0,   265,   266,   267
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-757))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,    41,    42,     2,   288,    16,    15,   148,    19,   627,
     353,   330,   651,   327,    25,    26,    27,   158,   498,   224,
     335,   651,    23,    34,    35,    63,    37,    38,   234,   235,
     236,   237,     1,    44,    45,    46,    47,    48,   353,   166,
      51,    52,   485,   795,     3,    63,    57,    58,    59,     3,
       3,    62,   179,    44,    65,    66,    67,     3,    69,    24,
      24,    72,    73,    74,    50,     1,    57,   693,    34,   825,
     197,     3,    16,   108,    34,   108,    34,    24,    84,    24,
     108,   108,   108,    74,   107,     0,     3,   110,    20,   127,
     127,   114,     1,     3,    63,    81,   134,     7,    34,   136,
     135,   138,    34,   108,   860,    37,   139,    61,    25,   137,
     137,   137,   864,    82,   732,    59,   134,    89,    25,    51,
      52,   132,   133,   134,   135,   136,   137,   138,   758,    61,
     573,    77,   137,   139,    78,   144,   775,    84,   107,    76,
     112,   152,   108,   623,    76,   775,   108,   138,   108,   135,
     108,    87,     3,    89,    63,   166,    20,   284,   285,    76,
     162,   163,   164,   108,    73,   134,   108,   108,   179,   134,
     134,   107,   108,    82,   176,   137,   382,   136,   497,   139,
     181,   139,   136,   136,   810,   811,   197,   134,    52,   134,
     136,   127,   194,     1,    34,   137,   137,    61,   107,    84,
     108,   485,   860,    38,   136,   207,   824,    13,   219,   136,
      34,    34,    18,   871,   136,    60,    80,   216,   127,    64,
       7,    61,    57,   350,   136,   134,    34,   135,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      34,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   483,   484,
     760,    89,   725,    69,   108,   108,   297,    89,     3,   573,
      84,   302,     7,   136,   108,   108,   298,    20,   617,   107,
     108,   615,   313,   108,   315,   316,   108,   318,   319,   108,
     112,   626,   323,   127,   108,   139,   139,    60,   127,   127,
     322,    64,   313,    38,   137,   530,   531,   338,   127,    52,
      44,   127,   137,   344,   127,   137,   243,   244,    61,   350,
       2,   133,    57,   108,    23,   139,   357,   358,   127,    11,
       3,   108,   363,    76,     7,   366,     9,   368,    20,   370,
     371,   372,   127,   374,   375,   127,     2,    20,   379,    52,
     127,    33,   136,   127,   138,    11,   517,    89,    61,   286,
     287,    60,   108,   108,    20,    64,   108,   108,   379,    51,
     297,    53,    54,    76,   737,   127,   108,    33,   741,    52,
     112,   127,   127,   136,   134,   127,   127,   127,    61,   316,
      72,    87,   319,    89,    76,    51,   731,    53,    54,   136,
     127,   138,   737,    76,    83,   137,   741,    80,   107,   134,
     109,   110,    34,   136,   136,   114,    72,   136,   110,   138,
      76,   725,   121,   454,   455,   456,   457,   458,   459,   460,
     764,   130,   461,   462,   136,   466,   467,    89,   469,   127,
      23,    63,     7,   370,   475,   127,   477,   366,   107,   368,
     481,    73,   127,   137,   485,   108,   127,   479,   480,    34,
      82,   483,   484,    26,   136,   108,    87,    89,   137,   500,
     137,   127,   503,   504,    89,   137,   137,    60,   108,   139,
     134,    64,   137,   514,   134,   167,   108,    89,     2,   500,
     825,   139,    26,    34,   127,   137,   137,    11,     3,   834,
     108,   127,     7,   514,   127,   127,    20,   108,    24,   191,
     139,   167,   134,    24,   134,    20,   107,    61,   110,    33,
     139,   139,    63,   110,   107,   860,   136,   110,   455,    82,
     457,   114,    73,   137,   216,   191,   871,    51,    73,    53,
      54,    82,   573,   539,   575,   576,   577,    52,    89,   580,
     358,    34,   159,   474,   711,   763,    61,   593,    72,   763,
     216,   592,    76,   585,   586,   764,   615,   108,   599,   652,
     601,    76,   864,   604,   596,    80,   607,   824,    16,   839,
      63,    19,   613,   339,   606,   783,   127,   609,   469,    27,
      73,    29,    30,   134,    -1,    -1,    -1,    -1,    36,    82,
      38,    -1,   633,    -1,    -1,    -1,    89,    -1,   639,   291,
     641,    -1,   643,   127,   645,    20,    54,   648,    56,    57,
      58,    59,   633,    -1,    -1,   108,    -1,    -1,    -1,    34,
      -1,   650,    37,   315,    -1,   291,   318,    -1,    76,    -1,
      78,   323,    -1,    -1,   127,    -1,    51,    52,   575,   576,
      -1,   134,    -1,   167,    -1,    -1,    61,    -1,   689,   315,
      -1,    -1,   318,    -1,    -1,    -1,    -1,   323,    -1,   700,
     701,    76,   703,   704,   705,   706,    -1,   191,    -1,    -1,
      -1,    -1,   713,   714,    -1,   716,    -1,   718,   719,   127,
     372,   722,    -1,    -1,   725,   755,   756,   135,    -1,    -1,
      -1,    -1,   216,    -1,    -1,   736,   737,    -1,    -1,    16,
     741,    -1,    19,   744,    -1,    19,   372,   748,   740,    -1,
      27,    -1,    29,    30,    -1,    29,    30,    -1,    -1,    36,
      -1,    38,    36,    -1,    38,    -1,   767,   768,   769,   770,
     771,   772,   773,   774,    -1,    -1,    -1,    54,    -1,    56,
      57,    58,    59,    57,   785,   786,   787,    -1,    -1,    -1,
     791,    -1,    19,   794,    -1,    -1,    -1,   449,    -1,    76,
     801,    78,    29,    30,    -1,    -1,   787,   291,    -1,    36,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,   820,
     821,   822,   823,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,   315,    -1,   834,   318,   836,   837,   489,    -1,   323,
      -1,    -1,   843,   844,   845,   846,   847,   848,    -1,    -1,
     127,    -1,    23,    -1,    19,    -1,    -1,    -1,   859,   860,
      -1,    -1,    -1,   489,    29,    30,    -1,     2,    -1,    -1,
     871,    36,    -1,    38,    -1,    -1,    11,    -1,    -1,    44,
      -1,    -1,    -1,   535,    -1,    20,    -1,   539,   372,    60,
      -1,    -1,    57,    64,   546,    -1,    -1,    -1,    33,    -1,
      -1,    -1,     2,    23,    -1,    -1,    -1,    -1,    -1,   535,
      -1,    11,    -1,   539,    -1,    -1,    51,    -1,    53,    54,
      20,    -1,    -1,   575,   576,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    33,   105,   106,   107,    72,   109,   110,
      60,    76,    -1,   114,    64,    -1,    -1,   599,    -1,   601,
     121,    51,   604,    53,    54,   607,    -1,   128,   129,   130,
     612,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,   599,    -1,   601,    76,    -1,   604,    -1,
     632,   607,    -1,   103,    -1,    -1,   612,   107,    -1,   109,
     110,   643,   127,   645,   114,    -1,    -1,    -1,   650,    -1,
      -1,   121,    -1,    -1,    -1,   489,   632,    -1,   128,   129,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   643,    -1,   645,
      -1,    -1,    -1,    -1,   650,    -1,    -1,   127,    -1,    -1,
      -1,    73,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,   535,    -1,    -1,    -1,   539,   191,    -1,    -1,    -1,
      -1,   713,     2,    -1,    -1,    -1,   718,   167,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,   216,    -1,    -1,    60,    -1,   738,   713,    64,    -1,
      -1,   191,   718,    33,    -1,   747,    -1,    -1,    -1,   751,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    51,   738,    53,    54,   599,   216,   601,    -1,    -1,
     604,   747,    -1,   607,   166,   751,    -1,   103,   612,   105,
     106,   107,    72,   109,   110,   111,    76,   179,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    -1,   632,   125,
      -1,    -1,   128,   129,   130,   197,   291,    -1,    -1,   643,
      -1,   645,    -1,    -1,    -1,    -1,   650,    -1,   820,   821,
     822,   823,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,    -1,    -1,   318,   836,   837,    -1,   127,   323,    -1,
      -1,   291,    -1,    -1,   820,   821,   822,   823,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
     836,   837,    23,    -1,    -1,   315,    11,     2,   318,    -1,
      -1,    -1,    -1,   323,    -1,    20,    11,   167,    -1,   713,
      -1,    -1,    -1,    -1,   718,    20,    60,   372,    33,    -1,
      64,    47,   284,   285,    -1,    -1,    -1,    -1,    33,    60,
      -1,   191,    -1,    64,   738,    -1,    51,    -1,    53,    54,
      -1,    -1,    -1,   747,    -1,    -1,    51,   751,    53,    54,
      -1,    -1,   372,    -1,    -1,    -1,   216,    72,    -1,   103,
      -1,    76,   106,   107,    -1,   109,   110,    72,    -1,    -1,
     114,    76,   103,    -1,   105,   106,   107,   121,   109,   110,
     111,    -1,   344,   114,   128,   129,   130,    23,   350,   120,
     121,    -1,    -1,    -1,   125,    -1,    -1,   128,   129,   130,
      -1,   363,    -1,    -1,   366,   131,   368,    -1,    -1,   371,
      -1,    -1,   127,    -1,    -1,    -1,   820,   821,   822,   823,
      -1,    -1,   127,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,   291,   836,   837,   489,    -1,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,   167,    -1,    -1,   315,    -1,    -1,   318,    -1,
      -1,    -1,   167,   323,    -1,    -1,    -1,    -1,   194,   489,
      -1,   107,    -1,   109,   110,    -1,   191,    -1,   114,    -1,
     535,   207,    -1,    -1,   539,   121,   191,    -1,    -1,    -1,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,
      -1,   216,    -1,    -1,    -1,   467,    -1,   469,    -1,    -1,
      -1,   216,   372,    16,    -1,   535,    19,    -1,    -1,   539,
      -1,    -1,    -1,    -1,    27,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   599,    -1,   601,    -1,    -1,   604,
      -1,    54,   607,    56,    57,    58,    59,   612,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   298,    76,    -1,    78,   291,   632,    -1,   599,
      -1,   601,    -1,    -1,   604,    -1,   291,   607,   643,    -1,
     645,    -1,   612,    -1,    -1,   650,   322,    -1,    -1,    -1,
     315,    60,    -1,   318,    -1,    64,    10,    -1,   323,    -1,
     315,    15,   632,   318,    -1,    -1,    -1,    -1,   323,    23,
      -1,    -1,    -1,   643,   127,   645,    -1,    -1,    -1,   489,
     650,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,    -1,    -1,    -1,   114,    60,   372,   713,    -1,
      64,   120,   121,   718,    -1,    -1,   125,   372,    -1,   128,
     129,   130,    -1,    -1,    -1,   535,    -1,    -1,    -1,   539,
      84,    -1,    -1,   738,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   747,   713,    -1,    -1,   751,    -1,   718,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   738,   123,
     124,   125,    -1,    -1,   128,   129,   130,   747,    -1,    -1,
      -1,   751,    -1,    -1,    -1,   139,    -1,    -1,    -1,   599,
      -1,   601,    -1,    -1,   604,    -1,    -1,   607,    -1,    -1,
      -1,    -1,   612,   479,   480,    -1,    -1,   483,   484,    -1,
      -1,    -1,    -1,    -1,    -1,   820,   821,   822,   823,   731,
      -1,    -1,   632,    -1,   489,    -1,    -1,    -1,    -1,    -1,
      -1,   836,   837,   643,   489,   645,    -1,    -1,    -1,    -1,
     650,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     820,   821,   822,   823,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   836,   837,    -1,    -1,
     535,    -1,    -1,    -1,   539,    -1,    -1,    -1,    -1,   791,
     535,    -1,    -1,    -1,   539,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   713,    -1,    -1,    -1,    -1,   718,   585,
     586,    -1,    -1,   825,    -1,    -1,    -1,    -1,    -1,    -1,
     596,    -1,   598,    -1,    -1,    -1,    -1,   603,   738,    -1,
     606,    -1,    -1,   609,   599,    -1,   601,   747,    -1,   604,
      -1,   751,   607,    -1,   599,    -1,   601,   612,   860,   604,
      -1,    -1,   607,    -1,    -1,     2,    -1,   612,    -1,   871,
      -1,    -1,    -1,    -1,    11,    -1,    -1,   632,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,    -1,   632,   643,    -1,
     645,    -1,    -1,    -1,    -1,   650,    33,    -1,   643,    -1,
     645,    -1,    -1,    -1,    -1,   650,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    53,    54,    -1,    -1,
     820,   821,   822,   823,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,   836,   837,    -1,    76,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,   713,    -1,
      -1,    13,    -1,   718,    -1,    -1,    18,    -1,   713,    -1,
      22,    23,    24,   718,   740,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,   738,   131,   132,   133,    -1,    -1,   136,
     127,   138,   747,   738,    -1,    -1,   751,    -1,    -1,    -1,
      -1,    -1,   747,    55,    -1,    -1,   751,    -1,    -1,    -1,
      -1,   777,   778,    -1,   780,   781,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   795,
     167,    -1,    -1,    -1,    -1,    -1,   802,    -1,   804,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   820,   821,   822,   823,    -1,
      -1,    -1,    -1,    -1,    -1,   820,   821,   822,   823,   216,
      -1,   836,   837,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   836,   837,    -1,    -1,    -1,   148,    -1,   864,    -1,
     152,    -1,    -1,    -1,    -1,    -1,   158,    -1,   160,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   179,    -1,   181,
      -1,    -1,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   229,   315,    -1,
      -1,   318,   234,   235,   236,   237,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,   251,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,   284,   285,    -1,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,   311,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,   330,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,   339,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,   350,   138,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
     382,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,   489,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   535,    -1,
      -1,    -1,   539,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,   469,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,
     138,   493,    -1,    -1,    -1,   497,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,   599,    -1,   601,   517,    -1,   604,    -1,    -1,
     607,    -1,    25,    -1,    -1,   612,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,   546,   632,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,   643,    -1,   645,    -1,
      -1,    -1,    -1,   650,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,   593,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,   617,    -1,   120,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   738,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     747,    -1,    -1,    -1,   751,    -1,   668,     1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,   754,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,   763,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,    -1,    -1,
     134,   135,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,   126,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,   135,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,   127,    -1,    -1,   130,    -1,    -1,    -1,   134,   135,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,   135,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
     124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
     134,   135,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,   127,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
     124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
     134,    -1,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    78,    79,    80,    -1,
      -1,    83,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,    -1,   124,   125,    -1,   127,    -1,    -1,   130,    -1,
      -1,    -1,   134,    -1,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    78,    79,    80,    -1,    -1,    83,    -1,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,   127,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,     1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    78,    79,
      80,    -1,    -1,    83,    -1,    85,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,    -1,   127,    -1,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,     1,   138,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    78,    79,    80,    -1,    -1,    83,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
     104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,
     124,   125,    -1,   127,    -1,    -1,   130,    -1,    -1,    -1,
     134,    -1,   136,     1,   138,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      78,    79,    80,    -1,    -1,    83,    -1,    85,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,   127,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,     1,
     138,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,   129,   130,    -1,
      -1,    -1,   134,    -1,   136,     1,   138,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    12,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,
     136,    -1,   138,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,    -1,   124,   125,   126,    -1,    -1,    -1,
     130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,   139,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,   139,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,
      -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,
     136,    -1,   138,   139,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    37,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    51,    52,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,   126,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
     137,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,   126,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,   137,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,   135,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,
     127,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    47,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,   126,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,   126,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,    -1,   124,
     125,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,   138,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,   104,   105,   106,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,    -1,   124,   125,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,    -1,   138,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,   105,   106,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
      -1,   124,   125,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,   134,    -1,   136,    -1,   138,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,   104,   105,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
      -1,   138,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,   104,   105,   106,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,    -1,   124,   125,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,   138,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,   103,   104,
     105,   106,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,   120,    35,   122,    -1,    -1,
     125,    40,    41,    -1,    43,   130,    45,    46,    -1,   134,
      -1,   136,    -1,   138,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,   104,    31,    32,    33,   108,
      35,    -1,    -1,   112,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,   122,    -1,    -1,    -1,   126,    53,     3,
      55,    -1,    -1,    -1,    -1,   134,    10,   136,   137,   138,
      -1,    15,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,   104,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   136,    -1,   138,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,    -1,    -1,   109,    -1,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,   125,    -1,    -1,   128,   129,   130,   131,    -1,   133,
      -1,    -1,   136,    89,    90,    91,    92,    93,    94,    -1,
      -1,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,   109,    -1,    -1,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,   125,
      -1,    10,   128,   129,   130,   131,    15,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    23,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    88,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      23,    -1,    -1,    64,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,   123,   124,   125,    88,    -1,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    60,    23,    -1,
     139,    64,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    10,   123,   124,   125,    88,    15,   128,   129,   130,
      -1,    -1,    -1,    -1,    23,    60,    -1,    -1,   139,    64,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    10,
     123,   124,   125,    88,    15,   128,   129,   130,    -1,    -1,
      -1,    60,    23,    -1,    -1,    64,   139,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,   123,    88,
     125,    -1,    -1,   128,   129,   130,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    10,   123,   124,   125,    88,    15,   128,
     129,   130,    -1,    -1,    -1,    -1,    23,    24,   137,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    34,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    10,   123,   124,   125,    -1,    15,   128,   129,   130,
      -1,    -1,    -1,    60,    23,    24,   137,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    10,
      -1,    88,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    23,    24,    -1,    64,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,   123,   124,   125,    88,
      -1,   128,   129,   130,    -1,    -1,    -1,   134,    -1,    60,
      -1,    -1,    -1,    64,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    84,   123,   124,   125,    88,    10,   128,
     129,   130,    -1,    15,    -1,   134,    -1,    -1,    -1,    -1,
      -1,    23,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,   123,   124,   125,    -1,    10,   128,   129,   130,
      -1,    15,    -1,   134,    -1,    -1,    -1,    -1,    60,    23,
      24,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    -1,    60,    -1,    -1,    -1,
      64,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,   125,    88,    -1,   128,   129,   130,    -1,
      60,    -1,   134,    -1,    64,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    88,   123,
     124,   125,    -1,    -1,   128,   129,   130,    -1,    -1,    10,
     134,    -1,    -1,   103,    15,   105,   106,   107,    -1,   109,
     110,   111,    23,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,    -1,   128,   129,
     130,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    71,   134,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,   123,   124,   125,    -1,    60,   128,
     129,   130,    64,    -1,    -1,   134,    -1,    -1,    -1,    71,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,    -1,   128,   129,   130,    -1,
      -1,    71,   134,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,    60,   128,   129,
     130,    64,    -1,    -1,   134,    -1,    -1,    -1,    71,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      71,   134,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    88,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    -1,   123,   124,   125,    -1,    60,   128,   129,   130,
      64,    -1,    -1,   134,    -1,    -1,    -1,    71,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,    -1,   128,   129,   130,    -1,    -1,    71,
     134,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    88,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,   125,    -1,    60,   128,   129,   130,    64,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
     123,   124,   125,    -1,    60,   128,   129,   130,    64,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    10,   123,   124,   125,
      -1,    15,   128,   129,   130,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    49,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    23,    24,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    10,   123,
     124,   125,    88,    15,   128,   129,   130,    -1,    -1,    -1,
      -1,    23,    24,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    34,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    10,   123,   124,   125,
      -1,    15,   128,   129,   130,    -1,    -1,    -1,    60,    23,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    -1,    -1,
      64,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,   123,   124,   125,    88,    -1,   128,   129,   130,    -1,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    88,   123,
     124,   125,    -1,   127,   128,   129,   130,    -1,    -1,    10,
      -1,    -1,    -1,   103,    15,   105,   106,   107,    -1,   109,
     110,   111,    23,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
     130,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    88,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,    -1,    10,    -1,    -1,    -1,   103,    15,   105,   106,
     107,    -1,   109,   110,   111,    23,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,    -1,
     128,   129,   130,    -1,    -1,    71,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,    -1,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,   127,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,   127,
     128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,   127,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,   127,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,   127,
     128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,   127,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,   127,   128,   129,   130,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    60,   123,   124,   125,    64,   127,   128,   129,
     130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    60,   123,   124,   125,    64,   127,
     128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    60,   123,   124,   125,
      64,   127,   128,   129,   130,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    60,   123,
     124,   125,    64,   127,   128,   129,   130,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    23,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      60,   123,   124,   125,    64,    -1,   128,   129,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    -1,
      -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    -1,   123,   124,   125,    -1,   127,   128,   129,
     130,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    24,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    71,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    71,    10,    11,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    71,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      71,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    71,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    60,   123,   124,   125,    64,    -1,   128,
     129,   130,    -1,    -1,    71,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    60,   123,   124,   125,    64,
      -1,   128,   129,   130,    -1,    -1,    71,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    60,   123,   124,
     125,    64,    -1,   128,   129,   130,    -1,    -1,    71,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    60,
     123,   124,   125,    64,    -1,   128,   129,   130,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,    60,   123,   124,   125,    64,    -1,   128,   129,   130,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,   103,    -1,   105,   106,   107,    60,
     109,   110,   111,    64,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,   123,   124,   125,    -1,    -1,   128,
     129,   130,    -1,    -1,    -1,    -1,    -1,    88,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,    -1,   113,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,   128,   129,   130,
      -1,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
      -1,   113,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,   123,    -1,   125,    -1,    -1,   128,   129,   130
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   144,   145,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    74,    75,    76,    78,    79,    83,    85,
      88,   103,   104,   105,   106,   111,   120,   122,   124,   125,
     127,   130,   134,   136,   138,   146,   147,   148,   149,   150,
     151,   156,   157,   158,   160,   163,   164,   165,   166,   167,
     169,   170,   171,   172,   175,   176,   179,   182,   185,   186,
     209,   212,   213,   231,   232,   233,   234,   235,   236,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   254,
     255,   256,   257,   258,   148,   246,    84,   237,   238,   159,
     160,   237,    12,    31,    32,    33,    70,   134,   138,   182,
     231,   235,   244,   245,   246,   247,   249,   250,    76,   159,
     246,   148,   136,   160,     7,   159,   161,     9,    76,   161,
      52,    86,   168,   246,   246,   246,   136,   160,   183,   136,
     160,   214,   215,   148,   246,   246,   246,   246,     7,   136,
      19,    30,   165,   165,   126,   205,   225,   246,   127,   246,
     246,    24,   151,   162,   246,   246,    69,   136,   148,   246,
     246,   148,   148,   160,   210,   225,   246,   246,   246,   246,
     246,   246,   246,   246,   135,   146,   152,   225,    77,   112,
     205,   226,   227,   246,   225,   246,   253,    54,   148,    44,
     160,    38,    57,   200,    20,    52,    61,    80,   127,   133,
      10,    15,    23,    60,    64,    88,   103,   105,   106,   107,
     109,   110,   111,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   123,   124,   125,   128,   129,   130,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   131,   132,   136,   138,    60,    64,   136,   148,
     127,   134,   151,   246,   246,   246,   225,    34,   237,   210,
     127,   127,    83,   160,   205,   228,   229,   230,   246,   134,
     210,   176,   160,   136,   162,    24,    34,   162,    24,    34,
      84,   162,   240,    71,   151,   228,   148,   136,   194,    77,
     136,   160,   216,   217,     1,   107,   219,   220,    34,   108,
     162,   228,   161,   160,   108,   127,   127,   134,   148,   162,
     136,   228,   127,    89,   211,   127,   127,    28,    49,   162,
     127,   135,   146,   108,   135,   246,   108,   137,   108,   137,
      34,   108,   139,   240,    87,   108,   139,     7,   160,   107,
     177,   187,    61,   214,   214,   214,   214,   246,   246,   246,
     246,   168,   246,   168,   246,   246,   246,   246,   246,   246,
     246,    25,    76,   160,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   228,   228,   168,   246,
     168,   246,    20,    34,    37,    51,    52,    61,    76,   198,
     239,   242,   246,   258,    24,    34,    24,    34,    71,    34,
     139,   168,   246,   162,   127,   246,    87,    89,   137,   108,
     160,   180,   181,   127,   160,    34,   225,    33,   246,   168,
     246,    33,   246,   168,   246,   136,   162,   148,   246,    26,
     137,   195,   196,   197,   198,   184,   217,   108,   137,     1,
     138,   221,   232,    87,    89,   218,   246,   215,   137,   205,
     246,   173,   228,   137,   138,   221,   232,   108,   130,   153,
     155,   246,   153,   154,   135,   137,   137,   226,   137,   226,
     168,   246,   139,   148,   246,   139,   246,   139,   246,   134,
     225,   134,    52,    61,    76,   189,   199,   214,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   137,   139,
      34,    61,   231,   108,   137,    60,    60,    33,   168,   246,
      33,   168,   246,   168,   246,   237,   237,   127,   246,   205,
     246,   230,    89,   108,   135,   177,   246,   137,   246,    24,
     162,    24,   162,   246,    24,   162,   240,    24,   162,   240,
     241,   242,    26,    26,   148,   108,   137,   136,   160,    20,
      52,    61,    76,   201,   137,   217,   107,   220,   225,   246,
      47,   246,    50,    81,   135,   174,   137,   225,   210,   108,
     127,    11,   127,   139,   240,   139,   240,   148,    87,   135,
     152,   178,   188,    10,    15,    89,    90,    91,    92,    93,
      94,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   109,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   125,   128,   129,   130,   131,   133,   136,
     160,   190,   191,   192,   231,   127,   242,   231,   231,   246,
      24,    24,   246,    24,    24,   139,   139,   162,   162,   246,
     181,   134,   162,    71,    33,   246,    33,   246,    71,    33,
     246,   162,    33,   246,   162,   108,   137,   148,   246,   196,
     217,   107,   224,    61,   220,   218,    34,   139,    24,   148,
     225,   139,   246,   246,    33,   148,   246,   139,    33,   148,
     246,   139,   246,   135,    30,    56,    58,   135,   147,   164,
     201,   160,   246,   110,   136,   193,   193,    71,    33,    33,
      71,    33,    33,    33,    33,   178,   246,   246,   246,   246,
     246,   246,   242,   137,    25,    67,    70,   138,   205,   223,
     232,   112,   207,   218,    82,   208,   246,   221,   232,   148,
     162,    11,   246,   148,   246,   148,   164,   220,   137,   160,
     191,   192,   195,   246,   246,   246,   246,   246,   246,   135,
      71,    71,    71,    71,   224,   139,   225,   205,   206,   246,
     246,   151,   163,   204,   139,   246,    71,    71,    73,   202,
     193,   193,   137,    71,    71,    71,    71,    71,    71,   246,
     246,   246,   246,   207,   218,   205,   222,   223,   232,    34,
     139,   232,   246,   246,   208,   246,   222,   223,   127,   203,
     204,   139,   222
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

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
    return 0;
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
yypush_parse (yypstate *yyps, int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE const *yypushed_loc, ParserContext* context)
#else
int
yypush_parse (yyps, yypushed_char, yypushed_val, yypushed_loc, context)
    yypstate *yyps;
    int yypushed_char;
    YYSTYPE const *yypushed_val;
    YYLTYPE const *yypushed_loc;
    ParserContext* context;
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
  *++yyvsp = yylval;
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

/* Line 1806 of yacc.c  */
#line 426 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 431 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 432 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 466 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 467 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 468 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 469 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 470 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 471 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 472 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 473 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 474 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 475 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 476 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 477 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 489 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 594 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 599 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 611 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 624 "chapel.ypp"
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

  case 93:

/* Line 1806 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), zipToTuple((yyvsp[(4) - (5)].pcallexpr)), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pForallIntents),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 655 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 673 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (7)].pcallexpr)), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 679 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 686 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 711 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 725 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 748 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 758 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 766 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 767 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 772 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 774 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 781 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 791 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 800 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 804 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 811 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 812 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 817 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 822 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 827 "chapel.ypp"
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

  case 148:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 853 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 892 "chapel.ypp"
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

  case 155:

/* Line 1806 of yacc.c  */
#line 914 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 918 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 962 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 963 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 964 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 969 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 970 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 984 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1005 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1010 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1016 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1051 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1064 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1068 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
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

  case 251:

/* Line 1806 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1147 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1155 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1164 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1191 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1236 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1265 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1320 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1324 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1335 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1376 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1385 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1387 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1389 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1391 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1393 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1405 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1412 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1418 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1424 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1430 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pForallIntents) = (yyvsp[(3) - (4)].pForallIntents); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pForallIntents), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1495 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1587 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1588 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1597 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1601 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1613 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1614 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1621 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1650 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1651 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1666 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1667 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1668 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8257 "bison-chapel.cpp"
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

  *++yyvsp = yylval;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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



