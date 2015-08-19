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

  #endif


/* Line 288 of yacc.c  */
#line 63 "chapel.ypp"

  #ifndef _BISON_CHAPEL_DEFINES_1_
  #define _BISON_CHAPEL_DEFINES_1_

  #include "symbol.h"

  #include <cstdio>

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
  };

  #endif


/* Line 288 of yacc.c  */
#line 119 "chapel.ypp"

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
#line 141 "chapel.ypp"

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
#line 223 "bison-chapel.cpp"

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
     EXTERNCODE = 263,
     TALIGN = 264,
     TATOMIC = 265,
     TBEGIN = 266,
     TBREAK = 267,
     TBY = 268,
     TCLASS = 269,
     TCOBEGIN = 270,
     TCOFORALL = 271,
     TCONFIG = 272,
     TCONST = 273,
     TCONTINUE = 274,
     TDELETE = 275,
     TDMAPPED = 276,
     TDO = 277,
     TDOMAIN = 278,
     TELSE = 279,
     TENUM = 280,
     TEXPORT = 281,
     TEXTERN = 282,
     TFOR = 283,
     TFORALL = 284,
     TIF = 285,
     TIN = 286,
     TINDEX = 287,
     TINLINE = 288,
     TINOUT = 289,
     TITER = 290,
     TLABEL = 291,
     TLAMBDA = 292,
     TLET = 293,
     TLOCAL = 294,
     TMINUSMINUS = 295,
     TMODULE = 296,
     TNEW = 297,
     TNIL = 298,
     TNOINIT = 299,
     TON = 300,
     TOTHERWISE = 301,
     TOUT = 302,
     TPARAM = 303,
     TPLUSPLUS = 304,
     TPRAGMA = 305,
     TPRIMITIVE = 306,
     TPRIVATE = 307,
     TPROC = 308,
     TPUBLIC = 309,
     TRECORD = 310,
     TREDUCE = 311,
     TREF = 312,
     TRETURN = 313,
     TSCAN = 314,
     TSELECT = 315,
     TSERIAL = 316,
     TSINGLE = 317,
     TSPARSE = 318,
     TSUBDOMAIN = 319,
     TSYNC = 320,
     TTHEN = 321,
     TTYPE = 322,
     TUNDERSCORE = 323,
     TUNION = 324,
     TUSE = 325,
     TVAR = 326,
     TWHEN = 327,
     TWHERE = 328,
     TWHILE = 329,
     TWITH = 330,
     TYIELD = 331,
     TZIP = 332,
     TALIAS = 333,
     TAND = 334,
     TASSIGN = 335,
     TASSIGNBAND = 336,
     TASSIGNBOR = 337,
     TASSIGNBXOR = 338,
     TASSIGNDIVIDE = 339,
     TASSIGNEXP = 340,
     TASSIGNLAND = 341,
     TASSIGNLOR = 342,
     TASSIGNMINUS = 343,
     TASSIGNMOD = 344,
     TASSIGNMULTIPLY = 345,
     TASSIGNPLUS = 346,
     TASSIGNSL = 347,
     TASSIGNSR = 348,
     TBAND = 349,
     TBNOT = 350,
     TBOR = 351,
     TBXOR = 352,
     TCOLON = 353,
     TCOMMA = 354,
     TDIVIDE = 355,
     TDOT = 356,
     TDOTDOT = 357,
     TDOTDOTDOT = 358,
     TEQUAL = 359,
     TEXP = 360,
     TGREATER = 361,
     TGREATEREQUAL = 362,
     THASH = 363,
     TLESS = 364,
     TLESSEQUAL = 365,
     TMINUS = 366,
     TMOD = 367,
     TNOT = 368,
     TNOTEQUAL = 369,
     TOR = 370,
     TPLUS = 371,
     TQUESTION = 372,
     TSEMI = 373,
     TSHIFTLEFT = 374,
     TSHIFTRIGHT = 375,
     TSTAR = 376,
     TSWAP = 377,
     TIO = 378,
     TLCBR = 379,
     TRCBR = 380,
     TLP = 381,
     TRP = 382,
     TLSBR = 383,
     TRSBR = 384,
     TNOELSE = 385,
     TUMINUS = 386,
     TUPLUS = 387
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
#line 172 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 423 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 429 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  int         captureTokens;
  char        captureString[1024];


/* Line 344 of yacc.c  */
#line 180 "chapel.ypp"

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



/* Line 344 of yacc.c  */
#line 503 "bison-chapel.cpp"

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
#define YYLAST   10983

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  428
/* YYNRULES -- Number of states.  */
#define YYNSTATES  820

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   387

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
     125,   126,   127,   128,   129,   130,   131,   132
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    46,    49,    53,    57,    61,    65,    69,    73,    76,
      80,    84,    87,    90,    94,    96,   102,   109,   110,   112,
     114,   117,   121,   123,   126,   130,   135,   140,   145,   150,
     155,   160,   165,   170,   175,   180,   185,   190,   195,   200,
     205,   206,   208,   210,   212,   214,   217,   219,   222,   226,
     228,   230,   233,   236,   238,   240,   242,   244,   246,   249,
     255,   259,   266,   273,   278,   284,   290,   294,   298,   305,
     311,   318,   324,   331,   335,   340,   347,   355,   362,   370,
     375,   381,   386,   391,   395,   402,   408,   414,   415,   418,
     422,   425,   432,   439,   448,   450,   452,   454,   455,   458,
     459,   462,   466,   472,   474,   477,   481,   483,   487,   488,
     489,   498,   499,   501,   504,   507,   508,   509,   519,   523,
     527,   533,   539,   541,   544,   546,   548,   550,   552,   554,
     556,   558,   560,   562,   564,   566,   568,   570,   572,   574,
     576,   578,   580,   582,   584,   586,   588,   590,   592,   594,
     596,   598,   600,   602,   604,   606,   608,   610,   612,   614,
     616,   617,   621,   625,   626,   628,   632,   637,   642,   649,
     656,   657,   659,   661,   663,   665,   668,   671,   673,   675,
     677,   678,   680,   682,   684,   686,   688,   689,   691,   693,
     695,   697,   699,   701,   703,   705,   708,   710,   711,   713,
     716,   719,   720,   723,   727,   732,   737,   740,   745,   746,
     749,   752,   757,   762,   767,   773,   778,   779,   781,   783,
     785,   789,   793,   798,   804,   806,   808,   812,   814,   817,
     821,   822,   825,   828,   829,   834,   835,   838,   841,   843,
     848,   853,   860,   862,   863,   865,   867,   871,   876,   880,
     885,   892,   893,   896,   899,   902,   905,   908,   911,   913,
     915,   919,   923,   925,   927,   929,   933,   937,   938,   940,
     942,   946,   950,   954,   958,   960,   962,   964,   966,   968,
     970,   972,   974,   977,   982,   987,   992,   998,  1001,  1004,
    1011,  1018,  1023,  1033,  1043,  1051,  1058,  1065,  1070,  1080,
    1090,  1098,  1103,  1110,  1117,  1127,  1137,  1144,  1146,  1148,
    1150,  1152,  1154,  1156,  1158,  1160,  1164,  1165,  1167,  1172,
    1174,  1178,  1181,  1185,  1187,  1191,  1194,  1199,  1201,  1203,
    1205,  1207,  1209,  1211,  1213,  1215,  1220,  1224,  1228,  1231,
    1234,  1236,  1238,  1240,  1242,  1244,  1246,  1248,  1253,  1258,
    1263,  1267,  1271,  1275,  1279,  1284,  1288,  1290,  1292,  1294,
    1296,  1300,  1304,  1308,  1312,  1318,  1322,  1326,  1330,  1334,
    1338,  1342,  1346,  1350,  1354,  1358,  1362,  1366,  1370,  1374,
    1378,  1382,  1386,  1390,  1394,  1398,  1402,  1406,  1410,  1413,
    1416,  1419,  1422,  1425,  1428,  1432,  1436,  1440,  1444,  1448,
    1452,  1456,  1460,  1462,  1464,  1466,  1468,  1470,  1472
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     134,     0,    -1,   135,    -1,    -1,   135,   136,    -1,   138,
      -1,   137,   138,    -1,    50,     7,    -1,   137,    50,     7,
      -1,   139,    -1,   141,    -1,   143,    -1,   150,    -1,   144,
      -1,   152,    -1,   155,    -1,   153,    -1,   156,    -1,   159,
      -1,   149,    -1,   218,   118,    -1,    10,   138,    -1,    11,
     219,   138,    -1,    12,   145,   118,    -1,    15,   219,   141,
      -1,    19,   145,   118,    -1,    20,   226,   118,    -1,    36,
     146,   138,    -1,    39,   138,    -1,    45,   226,   148,    -1,
      61,   226,   148,    -1,    61,   148,    -1,    65,   138,    -1,
      76,   226,   118,    -1,     1,    -1,   140,    41,   146,   124,
     125,    -1,   140,    41,   146,   124,   142,   125,    -1,    -1,
      54,    -1,    52,    -1,   124,   125,    -1,   124,   142,   125,
      -1,   136,    -1,   142,   136,    -1,    70,   207,   118,    -1,
     227,    80,   226,   118,    -1,   227,    91,   226,   118,    -1,
     227,    88,   226,   118,    -1,   227,    90,   226,   118,    -1,
     227,    84,   226,   118,    -1,   227,    89,   226,   118,    -1,
     227,    85,   226,   118,    -1,   227,    81,   226,   118,    -1,
     227,    82,   226,   118,    -1,   227,    83,   226,   118,    -1,
     227,    93,   226,   118,    -1,   227,    92,   226,   118,    -1,
     227,   122,   226,   118,    -1,   227,    86,   226,   118,    -1,
     227,    87,   226,   118,    -1,    -1,   146,    -1,     3,    -1,
     145,    -1,     7,    -1,    22,   138,    -1,   141,    -1,    58,
     118,    -1,    58,   226,   118,    -1,   118,    -1,   151,    -1,
      54,   151,    -1,    52,   151,    -1,   164,    -1,   191,    -1,
     160,    -1,   171,    -1,   194,    -1,    27,     8,    -1,    22,
     138,    74,   226,   118,    -1,    74,   226,   148,    -1,    16,
     226,    31,   226,   219,   148,    -1,    16,   226,    31,   154,
     219,   148,    -1,    16,   226,   219,   148,    -1,    28,   226,
      31,   226,   148,    -1,    28,   226,    31,   154,   148,    -1,
      28,   226,   148,    -1,    28,   154,   148,    -1,    28,    48,
     146,    31,   226,   148,    -1,    29,   226,    31,   226,   148,
      -1,    29,   226,    31,   226,   220,   148,    -1,    29,   226,
      31,   154,   148,    -1,    29,   226,    31,   154,   220,   148,
      -1,    29,   226,   148,    -1,    29,   226,   220,   148,    -1,
     128,   207,    31,   226,   129,   138,    -1,   128,   207,    31,
     226,   220,   129,   138,    -1,   128,   207,    31,   154,   129,
     138,    -1,   128,   207,    31,   154,   220,   129,   138,    -1,
     128,   207,   129,   138,    -1,   128,   207,   220,   129,   138,
      -1,    77,   126,   207,   127,    -1,    30,   226,    66,   138,
      -1,    30,   226,   141,    -1,    30,   226,    66,   138,    24,
     138,    -1,    30,   226,   141,    24,   138,    -1,    60,   226,
     124,   157,   125,    -1,    -1,   157,   158,    -1,    72,   207,
     148,    -1,    46,   138,    -1,    67,    60,   207,   124,   157,
     125,    -1,   161,   146,   162,   124,   163,   125,    -1,    27,
     147,   161,   146,   162,   124,   163,   125,    -1,    14,    -1,
      55,    -1,    69,    -1,    -1,    98,   207,    -1,    -1,   163,
     150,    -1,   163,   137,   150,    -1,    25,   146,   124,   165,
     125,    -1,   166,    -1,   165,    99,    -1,   165,    99,   166,
      -1,   146,    -1,   146,    80,   226,    -1,    -1,    -1,    37,
     168,   178,   169,   184,   202,   190,   186,    -1,    -1,    33,
      -1,    26,   147,    -1,    27,   147,    -1,    -1,    -1,   170,
     183,   172,   174,   173,   184,   202,   190,   185,    -1,   182,
     175,   177,    -1,   182,   176,   177,    -1,   182,   146,   101,
     175,   177,    -1,   182,   146,   101,   176,   177,    -1,   146,
      -1,    95,   146,    -1,    94,    -1,    96,    -1,    97,    -1,
      95,    -1,   104,    -1,   114,    -1,   110,    -1,   107,    -1,
     109,    -1,   106,    -1,   116,    -1,   111,    -1,   121,    -1,
     100,    -1,   119,    -1,   120,    -1,   112,    -1,   105,    -1,
     113,    -1,    13,    -1,   108,    -1,     9,    -1,   122,    -1,
     123,    -1,    80,    -1,    91,    -1,    88,    -1,    90,    -1,
      84,    -1,    89,    -1,    85,    -1,    81,    -1,    82,    -1,
      83,    -1,    93,    -1,    92,    -1,    -1,   126,   179,   127,
      -1,   126,   179,   127,    -1,    -1,   180,    -1,   179,    99,
     180,    -1,   181,   146,   206,   200,    -1,   181,   146,   206,
     189,    -1,   181,   126,   199,   127,   206,   200,    -1,   181,
     126,   199,   127,   206,   189,    -1,    -1,    31,    -1,    34,
      -1,    47,    -1,    18,    -1,    18,    31,    -1,    18,    57,
      -1,    48,    -1,    57,    -1,    67,    -1,    -1,    48,    -1,
      57,    -1,    67,    -1,    53,    -1,    35,    -1,    -1,    18,
      -1,    57,    -1,    48,    -1,    67,    -1,   118,    -1,   186,
      -1,   141,    -1,   149,    -1,   117,   146,    -1,   117,    -1,
      -1,   187,    -1,   103,   226,    -1,   103,   188,    -1,    -1,
      73,   226,    -1,    67,   192,   118,    -1,    17,    67,   192,
     118,    -1,    27,    67,   192,   118,    -1,   146,   193,    -1,
     146,   193,    99,   192,    -1,    -1,    80,   214,    -1,    80,
     203,    -1,   195,    48,   196,   118,    -1,   195,    18,   196,
     118,    -1,   195,    57,   196,   118,    -1,   195,    18,    57,
     196,   118,    -1,   195,    71,   196,   118,    -1,    -1,    17,
      -1,    27,    -1,   197,    -1,   196,    99,   197,    -1,   146,
     202,   200,    -1,   146,   201,    78,   226,    -1,   126,   199,
     127,   202,   200,    -1,    68,    -1,   146,    -1,   126,   199,
     127,    -1,   198,    -1,   198,    99,    -1,   198,    99,   199,
      -1,    -1,    80,    44,    -1,    80,   226,    -1,    -1,    98,
     128,   207,   129,    -1,    -1,    98,   214,    -1,    98,   203,
      -1,     1,    -1,   128,   207,   129,   214,    -1,   128,   207,
     129,   203,    -1,   128,   207,    31,   226,   129,   214,    -1,
       1,    -1,    -1,   214,    -1,   187,    -1,   128,   129,   204,
      -1,   128,   207,   129,   204,    -1,   128,   129,   205,    -1,
     128,   207,   129,   205,    -1,   128,   207,    31,   226,   129,
     204,    -1,    -1,    98,   214,    -1,    98,   187,    -1,    98,
      23,    -1,    98,    62,    -1,    98,    65,    -1,    98,   205,
      -1,   226,    -1,   187,    -1,   207,    99,   226,    -1,   207,
      99,   187,    -1,    68,    -1,   226,    -1,   187,    -1,   208,
      99,   208,    -1,   209,    99,   208,    -1,    -1,   211,    -1,
     212,    -1,   211,    99,   212,    -1,   146,    80,   187,    -1,
     146,    80,   226,    -1,   146,    78,   226,    -1,   187,    -1,
     226,    -1,   146,    -1,   217,    -1,   227,    -1,   216,    -1,
     235,    -1,   234,    -1,    62,   226,    -1,    32,   126,   210,
     127,    -1,    23,   126,   210,   127,    -1,    64,   126,   210,
     127,    -1,    63,    64,   126,   210,   127,    -1,    10,   226,
      -1,    65,   226,    -1,    28,   226,    31,   226,    22,   226,
      -1,    28,   226,    31,   154,    22,   226,    -1,    28,   226,
      22,   226,    -1,    28,   226,    31,   226,    22,    30,   226,
      66,   226,    -1,    28,   226,    31,   154,    22,    30,   226,
      66,   226,    -1,    28,   226,    22,    30,   226,    66,   226,
      -1,    29,   226,    31,   226,    22,   226,    -1,    29,   226,
      31,   154,    22,   226,    -1,    29,   226,    22,   226,    -1,
      29,   226,    31,   226,    22,    30,   226,    66,   226,    -1,
      29,   226,    31,   154,    22,    30,   226,    66,   226,    -1,
      29,   226,    22,    30,   226,    66,   226,    -1,   128,   207,
     129,   226,    -1,   128,   207,    31,   226,   129,   226,    -1,
     128,   207,    31,   154,   129,   226,    -1,   128,   207,    31,
     226,   129,    30,   226,    66,   226,    -1,   128,   207,    31,
     154,   129,    30,   226,    66,   226,    -1,    30,   226,    66,
     226,    24,   226,    -1,    43,    -1,   217,    -1,   213,    -1,
     230,    -1,   229,    -1,   167,    -1,   224,    -1,   225,    -1,
     223,   123,   226,    -1,    -1,   220,    -1,    75,   126,   221,
     127,    -1,   222,    -1,   221,    99,   222,    -1,   181,   213,
      -1,   238,    56,   213,    -1,   227,    -1,   223,   123,   226,
      -1,    42,   226,    -1,    38,   196,    31,   226,    -1,   232,
      -1,   214,    -1,   215,    -1,   236,    -1,   237,    -1,   167,
      -1,   224,    -1,   225,    -1,   126,   103,   226,   127,    -1,
     226,    98,   226,    -1,   226,   102,   226,    -1,   226,   102,
      -1,   102,   226,    -1,   102,    -1,   213,    -1,   229,    -1,
     230,    -1,   231,    -1,   227,    -1,   167,    -1,   228,   126,
     210,   127,    -1,   228,   128,   210,   129,    -1,    51,   126,
     210,   127,    -1,   226,   101,   146,    -1,   226,   101,    67,
      -1,   226,   101,    23,    -1,   126,   208,   127,    -1,   126,
     208,    99,   127,    -1,   126,   209,   127,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,   124,   207,   125,    -1,
     128,   207,   129,    -1,   128,   233,   129,    -1,   226,    78,
     226,    -1,   233,    99,   226,    78,   226,    -1,   226,   116,
     226,    -1,   226,   111,   226,    -1,   226,   121,   226,    -1,
     226,   100,   226,    -1,   226,   119,   226,    -1,   226,   120,
     226,    -1,   226,   112,   226,    -1,   226,   104,   226,    -1,
     226,   114,   226,    -1,   226,   110,   226,    -1,   226,   107,
     226,    -1,   226,   109,   226,    -1,   226,   106,   226,    -1,
     226,    94,   226,    -1,   226,    96,   226,    -1,   226,    97,
     226,    -1,   226,    79,   226,    -1,   226,   115,   226,    -1,
     226,   105,   226,    -1,   226,    13,   226,    -1,   226,     9,
     226,    -1,   226,   108,   226,    -1,   226,    21,   226,    -1,
     116,   226,    -1,   111,   226,    -1,    40,   226,    -1,    49,
     226,    -1,   113,   226,    -1,    95,   226,    -1,   226,    56,
     226,    -1,   226,    56,   154,    -1,   238,    56,   226,    -1,
     238,    56,   154,    -1,   226,    59,   226,    -1,   226,    59,
     154,    -1,   238,    59,   226,    -1,   238,    59,   154,    -1,
     116,    -1,   121,    -1,    79,    -1,   115,    -1,    94,    -1,
      96,    -1,    97,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   402,   402,   407,   408,   414,   415,   420,   421,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   455,   457,   462,   463,   464,
     479,   480,   485,   486,   491,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     513,   514,   518,   522,   523,   527,   528,   532,   533,   537,
     538,   539,   540,   541,   542,   543,   547,   548,   552,   567,
     568,   569,   570,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   589,   595,   601,   607,
     614,   624,   628,   629,   630,   631,   635,   640,   641,   645,
     647,   652,   659,   664,   672,   677,   682,   690,   691,   696,
     697,   699,   704,   714,   723,   727,   735,   736,   741,   746,
     740,   771,   777,   784,   792,   803,   809,   802,   837,   841,
     846,   850,   858,   859,   860,   861,   862,   863,   864,   865,
     866,   867,   868,   869,   870,   871,   872,   873,   874,   875,
     876,   877,   878,   879,   880,   881,   882,   883,   887,   888,
     889,   890,   891,   892,   893,   894,   895,   896,   897,   898,
     902,   903,   907,   911,   912,   913,   917,   919,   921,   923,
     928,   929,   930,   931,   932,   933,   934,   935,   936,   937,
     941,   942,   943,   944,   948,   949,   953,   954,   955,   956,
     957,   961,   962,   966,   967,   971,   973,   978,   979,   983,
     984,   988,   989,   993,   995,   997,  1002,  1015,  1032,  1033,
    1035,  1040,  1047,  1054,  1061,  1069,  1078,  1079,  1080,  1084,
    1085,  1093,  1095,  1101,  1106,  1108,  1110,  1115,  1117,  1119,
    1126,  1127,  1128,  1132,  1133,  1138,  1139,  1140,  1141,  1161,
    1165,  1169,  1177,  1181,  1182,  1183,  1187,  1189,  1195,  1197,
    1199,  1204,  1205,  1206,  1207,  1208,  1209,  1210,  1216,  1217,
    1218,  1219,  1223,  1224,  1225,  1229,  1230,  1234,  1235,  1239,
    1240,  1244,  1245,  1246,  1247,  1248,  1252,  1263,  1264,  1265,
    1266,  1267,  1268,  1270,  1272,  1274,  1276,  1278,  1280,  1285,
    1287,  1289,  1291,  1293,  1295,  1297,  1299,  1301,  1303,  1305,
    1307,  1309,  1316,  1322,  1328,  1334,  1343,  1348,  1356,  1357,
    1358,  1359,  1360,  1361,  1362,  1363,  1368,  1369,  1373,  1377,
    1379,  1387,  1397,  1401,  1402,  1407,  1412,  1420,  1421,  1422,
    1423,  1424,  1425,  1426,  1427,  1428,  1430,  1432,  1434,  1436,
    1438,  1443,  1444,  1445,  1446,  1457,  1458,  1462,  1463,  1464,
    1468,  1469,  1470,  1478,  1479,  1480,  1484,  1485,  1486,  1487,
    1488,  1489,  1490,  1497,  1498,  1502,  1503,  1504,  1505,  1506,
    1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,
    1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1528,  1529,
    1530,  1531,  1532,  1533,  1537,  1538,  1539,  1540,  1544,  1545,
    1546,  1547,  1552,  1553,  1554,  1555,  1556,  1557,  1558
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "EXTERNCODE", "TALIGN", "TATOMIC",
  "TBEGIN", "TBREAK", "TBY", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCONFIG",
  "TCONST", "TCONTINUE", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE",
  "TENUM", "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TIF", "TIN", "TINDEX",
  "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL",
  "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TOTHERWISE",
  "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE",
  "TPROC", "TPUBLIC", "TRECORD", "TREDUCE", "TREF", "TRETURN", "TSCAN",
  "TSELECT", "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC",
  "TTHEN", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "block_stmt", "stmt_ls",
  "use_stmt", "assignment_stmt", "opt_ident", "ident",
  "opt_ident_or_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "select_stmt", "when_stmt_ls", "when_stmt",
  "type_select_stmt", "class_decl_stmt", "class_tag", "opt_inherit",
  "class_level_stmt_ls", "enum_decl_stmt", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@1", "$@2", "linkage_spec", "fn_decl_stmt", "$@3",
  "$@4", "fn_decl_stmt_inner", "fn_ident", "assignop_ident",
  "opt_formal_ls", "req_formal_ls", "formal_ls", "formal",
  "opt_intent_tag", "opt_this_intent_tag", "proc_or_iter", "opt_ret_tag",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
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
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", 0
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
     385,   386,   387
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   133,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   139,   139,   140,   140,   140,
     141,   141,   142,   142,   143,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   145,   146,   147,   147,   148,   148,   149,   149,   150,
     150,   150,   150,   150,   150,   150,   151,   151,   152,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   154,   155,   155,   155,   155,   156,   157,   157,   158,
     158,   159,   160,   160,   161,   161,   161,   162,   162,   163,
     163,   163,   164,   165,   165,   165,   166,   166,   168,   169,
     167,   170,   170,   170,   170,   172,   173,   171,   174,   174,
     174,   174,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     177,   177,   178,   179,   179,   179,   180,   180,   180,   180,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     182,   182,   182,   182,   183,   183,   184,   184,   184,   184,
     184,   185,   185,   186,   186,   187,   187,   188,   188,   189,
     189,   190,   190,   191,   191,   191,   192,   192,   193,   193,
     193,   194,   194,   194,   194,   194,   195,   195,   195,   196,
     196,   197,   197,   197,   198,   198,   198,   199,   199,   199,
     200,   200,   200,   201,   201,   202,   202,   202,   202,   203,
     203,   203,   203,   204,   204,   204,   205,   205,   205,   205,
     205,   206,   206,   206,   206,   206,   206,   206,   207,   207,
     207,   207,   208,   208,   208,   209,   209,   210,   210,   211,
     211,   212,   212,   212,   212,   212,   213,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   216,   217,   218,   218,
     218,   218,   218,   218,   218,   218,   219,   219,   220,   221,
     221,   222,   222,   223,   223,   224,   225,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   227,   227,   227,   227,   228,   228,   229,   229,   229,
     230,   230,   230,   231,   231,   231,   232,   232,   232,   232,
     232,   232,   232,   233,   233,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   235,   235,
     235,   235,   235,   235,   236,   236,   236,   236,   237,   237,
     237,   237,   238,   238,   238,   238,   238,   238,   238
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     3,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       0,     1,     1,     1,     1,     2,     1,     2,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     2,     5,
       3,     6,     6,     4,     5,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     6,     7,     6,     7,     4,
       5,     4,     4,     3,     6,     5,     5,     0,     2,     3,
       2,     6,     6,     8,     1,     1,     1,     0,     2,     0,
       2,     3,     5,     1,     2,     3,     1,     3,     0,     0,
       8,     0,     1,     2,     2,     0,     0,     9,     3,     3,
       5,     5,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     0,     1,     3,     4,     4,     6,     6,
       0,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     0,     1,     1,     1,
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
       3,     2,     3,     1,     3,     2,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     3,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     3,     4,     3,     1,     1,     1,     1,
       3,     3,     3,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    62,   376,   377,   378,   379,
       0,   336,    60,   114,   336,     0,   237,    60,     0,     0,
       0,     0,    60,    60,     0,     0,     0,     0,   132,     0,
     128,     0,     0,     0,     0,   327,     0,     0,     0,     0,
     236,   236,   115,     0,     0,     0,     0,     0,     0,     0,
       0,   116,     0,     0,     0,   424,   426,     0,   427,   428,
     360,     0,     0,   425,   422,    69,   423,     0,     0,     0,
       4,     0,     5,     9,     0,    10,    11,    13,   296,    19,
      12,    70,    14,    16,    15,    17,    18,    75,     0,    73,
     352,     0,    76,    74,    77,     0,   361,   348,   349,   299,
     297,     0,     0,   353,   354,     0,   298,     0,   362,   363,
     364,   347,   301,   300,   350,   351,     0,    21,   307,     0,
       0,   337,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,   352,   361,   297,   353,   354,   336,   298,   362,
     363,     0,     0,     0,     0,   287,     0,    64,    63,   133,
      78,     0,   134,     0,     0,     0,     0,     0,     0,   287,
       0,     0,     0,     0,     0,   239,    28,   410,   345,     0,
     411,     7,   287,   237,   238,    72,    71,    67,     0,     0,
       0,    66,    31,     0,   302,     0,   287,    32,   308,     0,
     228,     0,   216,   279,     0,   278,     0,     0,   413,   359,
     409,   412,   408,    40,    42,     0,     0,   282,     0,   284,
       0,     0,   283,     0,   278,     0,     0,     6,     0,   117,
     205,   204,   135,     0,     0,     0,     0,    20,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   358,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   287,   287,     0,     0,   190,    22,    23,     0,
      24,     0,     0,     0,     0,     0,     0,     0,    25,    26,
       0,   296,   294,     0,   288,   289,   295,     0,     0,     0,
       0,     0,    87,     0,     0,    86,     0,     0,    93,     0,
       0,   103,     0,    27,   183,   129,   244,     0,   245,   247,
       0,   258,     0,     0,   250,     0,     0,    29,     0,   134,
      68,   107,    65,    30,   287,     0,     0,     0,   226,   223,
     215,     0,    44,    80,    33,    41,    43,   380,     0,     0,
     373,     0,   375,     0,     0,     0,     0,     0,   382,     8,
       0,     0,     0,   200,     0,     0,     0,     0,     0,   335,
     405,   404,   407,   415,   414,   419,   418,   401,   398,   399,
     400,   356,   388,   372,   371,   370,   357,   392,   403,   397,
     395,   406,   396,   394,   386,   391,   393,   402,   385,   389,
     390,   387,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
     416,   421,   420,   194,   191,   192,   193,   197,   198,   199,
     422,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,   381,   336,   336,    83,   224,     0,     0,     0,   304,
       0,   126,     0,   123,   225,   117,     0,     0,     0,   311,
       0,     0,     0,   317,     0,     0,    94,   102,     0,     0,
     303,     0,   184,     0,   206,     0,   248,     0,   262,     0,
     257,   348,     0,     0,   241,   346,   240,   369,     0,     0,
     305,   107,     0,   230,   348,     0,   281,   280,   355,   374,
     285,   286,     0,     0,    99,   321,     0,   383,     0,     0,
     118,   119,   201,   202,   203,   136,     0,     0,   232,   231,
     233,   235,    45,    52,    53,    54,    49,    51,    58,    59,
      47,    50,    48,    46,    56,    55,    57,   367,   368,   195,
     196,   341,   190,   338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,   293,   291,   292,   290,
       0,   124,   122,     0,     0,   101,     0,     0,    85,     0,
      84,     0,     0,    91,     0,     0,    89,     0,     0,     0,
     105,   190,   182,     0,   271,   207,   209,   208,   210,     0,
     246,   249,     0,   250,     0,   242,   251,   252,     0,     0,
     106,   108,   306,     0,     0,   227,     0,     0,     0,     0,
     100,     0,    35,     0,   236,   206,   165,   163,   168,   175,
     176,   177,   172,   174,   170,   173,   171,   169,   179,   178,
     144,   147,   145,   146,   157,   148,   161,   153,   151,   164,
     152,   150,   155,   160,   162,   149,   154,   158,   159,   156,
     166,   167,   142,   180,   180,   234,   340,   342,     0,     0,
       0,     0,     0,     0,     0,     0,    82,    81,   127,   125,
     119,    88,     0,     0,   310,     0,   309,     0,     0,   316,
      92,     0,   315,    90,   104,   326,   185,     0,     0,   250,
     221,   243,     0,     0,   110,     0,   111,     0,     0,    97,
     323,     0,     0,    95,   322,     0,   384,    36,    60,   236,
     236,     0,   112,   236,   120,     0,   143,     0,   183,   138,
     139,     0,     0,     0,     0,     0,     0,     0,     0,   236,
     314,     0,     0,   320,     0,     0,   271,   274,   275,   276,
       0,   273,   277,   348,   217,   187,   186,     0,     0,     0,
     260,   348,   109,     0,    98,     0,    96,   121,   221,   142,
     180,   180,     0,     0,     0,     0,     0,     0,     0,   113,
       0,     0,     0,     0,   250,   263,     0,   218,   220,   219,
     222,   213,   214,   130,     0,     0,     0,     0,   140,   141,
     181,     0,     0,     0,     0,     0,     0,   313,   312,   319,
     318,   189,   188,   265,   266,   268,   348,     0,   381,   348,
     325,   324,   211,   137,   212,     0,   267,   269,   263,   270
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   204,    71,   467,    73,    74,    75,   205,
      76,    77,   148,    78,   152,   182,    79,    80,    81,    82,
      83,   551,    84,    85,   488,   601,    86,    87,    88,   362,
     614,    89,   452,   453,   132,   161,   474,    91,    92,   363,
     615,   515,   653,   654,   719,   315,   471,   472,   473,   516,
     222,   589,   813,   783,   193,   778,   745,   748,    93,   191,
     338,    94,    95,   164,   165,   319,   320,   484,   323,   324,
     480,   804,   742,   689,   206,   210,   211,   293,   294,   295,
     133,    97,    98,    99,   134,   101,   120,   121,   432,   433,
     102,   135,   136,   105,   138,   107,   139,   140,   110,   111,
     215,   112,   113,   114,   115,   116
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -710
static const yytype_int16 yypact[] =
{
    -710,   112,  2153,  -710,  -710,  -710,  -710,  -710,  -710,  -710,
    3227,    -6,   170,  -710,    -6,  5955,   118,   170,  5955,  3227,
      81,   170,   202,   399,  4977,  5955,  5955,   152,  -710,   170,
    -710,    22,  3227,  5955,  5955,  -710,  5955,  5955,   209,   173,
     808,   919,  -710,  5159,  5955,  5285,  5955,   274,   179,  3227,
     105,  -710,  5365,  5955,  5955,  -710,  -710,  5955,  -710,  -710,
    6675,  5955,  5955,  -710,  5955,  -710,  -710,  2471,  4773,  5365,
    -710,  3101,  -710,  -710,   319,  -710,  -710,  -710,  -710,  -710,
    -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,   170,  -710,
     315,    21,  -710,  -710,  -710,   332,   227,  -710,  -710,  -710,
     250,   260,   261,   265,   272, 10828,  6869,   -50,   276,   290,
    -710,  -710,  -710,  -710,  -710,  -710,   359,  -710, 10828,   288,
    3227,  -710,   295,  -710,   310,  5955,  5955,  5955,  5955,  5955,
    5365,  5365,   146,  -710,  -710,  -710,  -710,  8347,   164,  -710,
    -710,   170,   321,  8550,   370,  5365,   325,  -710,  -710,  -710,
    -710,   170,   144,   170,   324,    26,  7341,  7237,  7475,  5365,
    3227,   327,    16,    94,   -14,  -710,  -710,   311,   350,  7542,
     311,  -710,  5365,  -710,   266,  -710,  -710,  -710,  8617,  8212,
    3227,  -710,  -710,  7542, 10828,   328,  5365,  -710, 10828,  5365,
     375,   339,   170,  -710,   171, 10828,  7542,  8684,   299,  1174,
     311,   299,   311,  -710,  -710,  2597,   -32,  -710,  5955,  -710,
     -24,   -18, 10828,    15,  8751,    55,   451,  -710,   170,   363,
    -710,  -710,  -710,    34,    22,    22,    22,  -710,  5955,  5955,
    5955,  5955,  5467,  5467,  5955,  5955,  5955,  5955,  5955,  5955,
      69,  6675,  5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,
    5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,
    5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,
    5955,  5955,  5365,  5365,  5467,  5467,  1006,  -710,  -710,  2723,
    -710,  8415,  8483,  8818,    46,  5467,    26,   344,  -710,  -710,
    5955,   341,  -710,   336,   372,  -710, 10828,   170,   356,   170,
     447,  5365,  -710,  3353,  5467,  -710,  3479,  5467,  -710,    26,
    3227,   457,   358,  -710,    76,  -710,  -710,    16,  -710,   384,
     364,  -710,  4613,   408,   414,  5955,    22,  -710,   369,  -710,
    -710,  -710,  -710,  -710,  5365,   371,   -28,  4693,   401,  -710,
    -710,  5365,  -710,  -710,  -710,  -710,  -710,  -710,  7133,  4875,
    -710,  5057,  -710,  5467,  2343,   374,  5955,  5955,  -710,  -710,
     377,  5365,   380,   259,    22,   185,   241,   243,   277,  8279,
   10862, 10862,   287,  -710,   287,  -710,   287,   522,  1825,   645,
    1759,   350,   299,  -710,  -710,  -710,  1174,  2200,   287,  1431,
    1431, 10862,  1431,  1431,   929,   299,  2200,  1630,   929,   311,
     311,   299,  8885,  8952,  9019,  9086,  9153,  9220,  9287,  9354,
    9421,  9488,  9555,  9622,  9689,  9756,  9823,   378,   381,  -710,
     287,  -710,   287,   140,  -710,  -710,  -710,  -710,  -710,  -710,
    -710,   170,    84,  -710,   455,  6035,  5467,  6115,  5467,  5955,
    5467,  1335,    -6,  9890,  -710,  -710,  9957,  5955,  5365,  -710,
    5365,   433,   183,  -710,  -710,   363,  5955,    90,  5955, 10828,
      43,  7609,  5955, 10828,    -2,  7408,  -710,   494, 10024,  3227,
    -710,   182,  -710,    37,   270,   392,    16,    97,  -710,  5365,
    -710,   255,  5955,  5569,  -710, 10828,  -710,  -710,    83,   394,
    -710,  -710,  5365,  -710,   283,   170,  -710, 10828,  -710,  -710,
    -710,  -710,     5,  6927,  -710, 10828,  3227, 10828, 10091,  2849,
     423,  -710,  -710,  -710,  -710,  -710,  6812,   292,  -710,  -710,
    -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,
    -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,
    -710,  -710,  1006,  -710,   170,  5955,   501, 10158,  5955,   502,
   10225,   403,  6961,    26,    26,  -710, 10828,  -710, 10828,  -710,
    5955,   170,  -710,   406,  7542,  -710,  7676,  3605,  -710,  3731,
    -710,  7743,  3857,  -710,    26,  3983,  -710,    26,  3227,  5955,
    -710,   722,  -710,    16,   435,  -710,  -710,  -710,  -710,    29,
    -710,  -710,  4693,   414,    52, 10828,  -710, 10828,  3227,  5365,
    -710,  -710,  -710,   142,    57,  -710,  4109,   410,  4235,   413,
    -710,  5955,  -710,  2975,   462,   270,  -710,  -710,  -710,  -710,
    -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,
    -710,   170,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,
    -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,  -710,
    -710,  -710,   436,   420,   420,  -710,  -710,  -710, 10292,  6195,
    6275, 10359,  6355,  6435,  6515,  6595,  -710,  -710, 10828,  -710,
    -710,  -710,  3227,  5955, 10828,  5955, 10828,  3227,  5955, 10828,
    -710,  5955, 10828,  -710,  -710, 10828,  -710,   422,  5649,   100,
     478,  -710,  5467,  4361,  -710,    42,  -710,  4487,  5955,  -710,
   10828,  3227,  5955,  -710, 10828,  3227, 10828,  -710,   537,   672,
     672,   170,  -710,  1569,  -710,    28,  -710,  6812,    76,  -710,
    -710,  5955,  5955,  5955,  5955,  5955,  5955,  5955,  5955,   944,
   10024,  7810,  7877, 10024,  7944,  8011,   435,    81,  5955,  5955,
    1961,  -710,  -710,    79,  5365,  -710,  -710,  5955,   -20,  6995,
    -710,   468,  -710,  8078,  -710,  8145,  -710,  -710,   478,  -710,
     420,   420,   184, 10426, 10493, 10560, 10627, 10694, 10761,  -710,
    3227,  3227,  3227,  3227,   100,  5751,    58,  -710,  -710, 10828,
   10828,  -710,  -710,  -710,  6595,  3227,  3227,   -13,  -710,  -710,
    -710,  5955,  5955,  5955,  5955,  5955,  5955, 10024, 10024, 10024,
   10024,  -710,  -710,  -710,  -710,  -710,   230,  5467,  6755,   620,
   10024, 10024,  -710,  -710,  -710,  7099,  -710,  -710,  5853,  -710
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -710,  -710,  -710,    -1,  -565,  1538,  -710,  -710,   782,    44,
    -710,  -710,   313,  1762,    -4,   351,  -648,  -582,   -37,  -710,
    -710,    89,  -710,  -710,    61,  -710,  -710,  -710,   402,   101,
    -110,  -710,  -710,     0,   285,  -710,  -710,  -710,  -710,  -710,
    -710,  -710,  -154,  -151,  -623,  -710,  -150,   -11,  -265,  -710,
    -710,   -44,  -710,  -215,   126,  -710,  -201,  -181,  -710,  -129,
    -710,  -710,  -710,  -163,   253,  -710,  -315,  -583,  -710,  -449,
    -328,  -608,  -709,  -152,     1,    71,  -710,  -104,  -710,   132,
     304,  -316,  -710,  -710,   526,  -710,    -7,  -116,  -710,    48,
    -710,   807,   862,   -10,  1104,  -710,  1147,  1190,  -710,  -710,
    -710,  -710,  -710,  -710,  -710,  -263
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -382
static const yytype_int16 yytable[] =
{
     118,    70,   475,   175,   176,   137,   481,   124,   143,   493,
     691,   431,   287,   434,   156,   157,   158,   325,   149,     5,
     572,   494,   298,   167,   168,     5,   169,   170,   593,   321,
     321,   720,   714,   178,   179,   183,   184,     5,    43,   188,
       5,   309,   195,   196,   197,    43,   353,   198,   180,   713,
     199,   200,   201,   194,   202,   312,   220,   195,   212,   214,
     365,   366,   367,   368,   180,   567,   805,   341,   328,   119,
     213,   341,     5,   119,   221,   349,   272,   440,   273,  -190,
     119,   351,   335,   692,   316,   326,  -255,  -255,   692,   807,
     119,   364,   383,   347,   423,   321,   491,   355,   321,   817,
     782,  -255,  -255,   350,   279,   812,   746,   424,     5,   352,
     425,   279,     3,   155,   341,   118,   281,   282,   283,   188,
     195,   214,   279,   426,   427,  -255,   592,   592,  -255,   598,
     286,   757,   284,   428,   606,   296,   384,   788,   789,   782,
     690,   341,   317,   429,   354,   341,  -255,   714,   162,   296,
     279,   341,  -255,  -255,   357,   599,   341,   341,    13,  -272,
     162,   591,   296,   583,   713,   189,   279,   279,   417,   418,
     329,   539,  -253,     5,  -255,   441,   296,  -255,  -272,   195,
     483,   693,  -272,   542,   358,   141,   697,   808,   598,   341,
     336,   802,   322,  -255,   209,   592,  -255,   540,   348,    42,
     816,   517,  -190,   744,   346,     5,  -272,   145,   600,   147,
     819,   543,  -255,    51,   599,  -255,   171,   565,   369,   370,
     371,   372,   374,   376,   377,   378,   379,   380,   381,   382,
     489,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   296,   296,   420,   422,   758,   696,   687,     5,
     341,   292,  -366,   147,  -366,   443,   481,   431,   159,   434,
     446,   581,   561,   581,   326,   292,  -256,    90,   585,   342,
    -365,   195,  -365,   459,   461,    90,   463,   465,   292,   172,
     468,   -60,   457,   518,    90,   186,    96,   512,   562,   582,
    -264,   790,   292,  -256,    96,   485,   513,    90,   586,   -60,
     231,   373,   375,    96,   296,   122,   514,   587,  -256,  -264,
     142,   497,   231,  -264,    90,  -256,    96,   588,   185,   212,
     326,   212,   326,   503,   505,  -329,   507,   508,   574,   577,
     223,   195,    90,    96,  -256,   232,    90,  -264,   233,   519,
     218,   520,   510,   419,   421,   750,   605,   232,  -328,   750,
     233,    96,   743,  -256,   442,    96,   326,   751,   227,  -256,
     224,   751,  -229,  -333,   228,   238,   607,   609,   240,   225,
    -334,   326,   243,   460,  -331,   521,   464,   238,   292,   292,
     240,  -229,     5,   226,   243,    90,   147,   150,  -330,   238,
     655,   239,   240,   278,   276,   274,   243,  -238,   275,   447,
     500,   448,   501,   250,    96,   459,   547,   463,   550,   468,
     552,   505,   256,  -332,   279,   553,   554,   556,   558,   288,
     296,  -366,   502,  -366,   290,    90,   564,  -238,   566,   297,
     301,   240,   571,   314,   334,   337,  -238,   339,   359,   806,
     292,   361,   445,   449,    96,    90,   151,   496,   809,   214,
    -238,   450,   595,   597,   454,   209,    13,   209,   456,    16,
     594,   469,   214,   476,    96,   470,   482,    21,    22,   708,
      90,   477,   806,   604,   483,    28,   487,  -131,   490,  -259,
     495,   509,   806,   506,   511,   537,   302,   305,   308,    96,
     538,   544,    38,   560,   709,  -131,   710,    42,   578,   590,
     327,   602,   341,   659,   662,   546,  -259,   549,   100,   711,
     670,    51,   664,   688,   333,   658,   100,   717,   661,   701,
       5,  -259,   705,   231,   147,   100,   718,   343,  -259,   736,
     668,   747,   603,   613,   299,  -238,   563,   674,   100,   676,
     729,   669,   679,   760,    90,   682,   761,  -259,   762,   685,
     686,   715,   814,   801,   557,   100,   292,   787,   232,   486,
      65,   233,   559,    96,   774,  -238,  -259,   712,    90,   195,
     656,    90,  -259,   100,  -238,    90,   700,   100,   704,     0,
     695,   706,     0,     0,   151,     0,     0,    96,  -238,     0,
      96,     0,   346,     0,    96,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
       0,   247,   248,   249,   250,     0,   251,   444,   253,    90,
       0,   254,   255,   256,     0,     0,   100,     0,     0,   674,
     676,  -261,   679,   682,   700,   704,     0,     0,    96,     0,
     466,     0,   730,   731,     0,   732,   231,   733,   734,     0,
       0,   735,   175,   176,     0,     0,     0,     0,  -261,     0,
       0,     0,   749,   505,     0,     0,   100,   505,   753,   173,
       0,     0,   755,  -261,     0,     0,     0,     0,    22,   174,
    -261,   232,     0,     0,   233,    28,   100,  -131,     0,     0,
       0,   730,   763,   764,   733,   765,   766,   767,   768,  -261,
       0,     0,     0,     0,     0,  -131,     0,     0,   184,   188,
     214,   100,     0,     0,   779,   541,     0,   780,  -261,   235,
     423,   776,   237,   238,  -261,   239,   240,     0,     0,     0,
     243,     0,     0,   424,    90,     0,   425,   250,     0,     0,
     797,   798,   799,   800,   254,   255,   256,     0,     0,   426,
     427,     0,     0,    96,   704,   810,   811,     0,     0,   428,
       0,   797,   798,   799,   800,   810,   811,     0,     0,   429,
       0,    90,     0,     0,    90,     0,     0,   815,   505,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   704,   103,
      96,   568,   570,    96,   741,   573,   576,   103,     0,     0,
       0,     0,     0,     0,     0,   173,   103,   181,     0,   100,
       0,     0,   100,     0,    22,   174,   100,     0,     0,   103,
       0,    28,     0,  -131,     0,     0,     0,     0,   657,   -39,
       0,     0,    90,     0,    90,     0,   103,    90,     0,     0,
      90,  -131,     0,    90,   104,     0,     0,     0,     0,     0,
     777,    96,   104,    96,   103,     0,    96,     0,   103,    96,
     100,   104,    96,    90,     0,     0,     0,     0,     0,     0,
       0,    90,     0,    90,   104,     0,     0,     0,    90,     0,
       0,   803,    96,     0,   666,   667,   280,     0,     0,     0,
      96,   104,    96,     0,     0,   671,     0,    96,     0,     0,
       0,     0,     0,     0,     0,   680,     0,   103,   683,   104,
       0,     0,     0,   104,   803,     0,   173,   181,   181,   181,
     311,     0,     0,     0,   803,    22,   174,     0,     0,     0,
     231,   181,    28,     0,  -131,     0,     0,    90,    13,     0,
     -38,    16,    90,     0,     0,   181,     0,   103,     0,    21,
      22,   708,  -131,     0,     0,     0,    96,    28,   181,  -131,
       0,    96,   104,     0,     0,   232,    90,   103,   233,     0,
      90,     0,     0,     0,    38,   100,   709,  -131,   710,    42,
       0,     0,     0,     0,     0,    96,     0,     0,     0,    96,
       0,   711,   103,    51,     0,     0,     0,     0,     0,     0,
       0,     0,   104,   235,   423,   236,   237,   238,     0,   239,
     240,     0,   100,     0,   243,   100,     0,   424,     0,     0,
     425,   250,   104,     0,     0,     0,   752,     0,   254,   255,
     256,     0,     0,   426,   427,    90,    90,    90,    90,     0,
       0,     0,    65,   428,     0,     0,     0,   104,   181,   769,
      90,    90,     0,   429,    96,    96,    96,    96,     0,     0,
       0,     0,     0,     0,     0,    55,   103,     0,     0,    96,
      96,   181,     0,   100,     0,   100,     0,     0,   100,     0,
      56,   100,    58,    59,   100,     0,   106,     0,     0,     0,
     103,     0,     0,   103,   106,     0,     0,   103,     0,     0,
       0,    63,   430,   106,   100,     0,     0,    66,     0,     0,
       0,     0,   100,     0,   100,     0,   106,     0,     0,   100,
       0,   104,     0,     0,     0,     0,     0,     0,     0,   108,
       0,     0,     0,   106,     0,     0,     0,   108,     0,     0,
       0,   103,     0,     0,     0,   104,   108,     0,   104,     0,
       0,   106,   104,     0,     0,   106,     0,     0,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   109,     0,     0,   231,   108,     0,   100,     0,
     109,     0,     0,   100,     0,     0,     0,     0,     0,   109,
       0,     0,     0,     0,   108,     0,   104,     0,   108,     0,
       0,     0,   109,     0,   106,     0,     0,   100,     0,     0,
     232,   100,     0,   233,     0,     0,     0,     0,     0,   109,
       0,     0,   181,   181,     0,     0,   181,   181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,     0,
       0,   109,     0,     0,   106,     0,     0,   108,   235,     0,
     236,   237,   238,     0,   239,   240,   103,     0,     0,   243,
       0,     0,     0,     0,   106,   249,   250,     0,     0,     0,
     253,     0,     0,   254,   255,   256,   100,   100,   100,   100,
       0,     0,     0,     0,     0,     0,     0,   108,     0,   106,
     109,   100,   100,   103,     0,     0,   103,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,   104,     0,     0,     0,   181,   181,     0,     5,     6,
       7,     8,     9,     0,     0,   125,   181,     0,   311,     0,
     109,     0,   108,   311,     0,     0,   181,     0,    20,   181,
       0,     0,     0,   126,   127,   128,     0,    27,   104,     0,
     109,   104,    30,    31,   103,    33,   103,    34,    35,   103,
       0,     0,   103,   106,    37,   103,    39,     0,     0,     0,
       0,     0,     0,     0,     0,   109,     0,    46,    47,    48,
     129,     0,     0,     0,     0,   103,     0,   106,     0,     0,
     106,     0,     0,   103,   106,   103,     0,     0,     0,     0,
     103,     0,     0,     0,     0,     0,   108,     0,     0,   104,
      57,   104,     0,     0,   104,     0,     0,   104,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,    62,     0,
     108,     0,   231,   108,     0,     0,     0,   108,   106,     0,
     104,    68,     0,   131,     0,     0,     0,     0,   104,   109,
     104,     0,     0,     0,     0,   104,     0,   181,     0,   103,
       0,     0,     0,     0,   103,     0,     0,   232,     0,     0,
     233,     0,     0,   109,     0,     0,   109,     0,     0,     0,
     109,   108,     0,     0,     0,     0,     0,     0,   103,     0,
       0,     0,   103,   311,   311,     0,   311,   311,     0,     0,
       0,     0,     0,     0,     0,   235,     0,   236,   237,   238,
     781,   239,   240,   241,   104,   311,   243,   311,     0,   104,
      72,     0,   249,   250,   109,     0,     0,   253,   117,     0,
     254,   255,   256,     0,     0,     0,     0,   144,     0,     0,
       0,     0,     0,   104,     0,     0,     0,   104,     0,   781,
     166,     0,     0,   106,     0,     0,     0,   103,   103,   103,
     103,     0,     0,    13,     0,     0,    16,   187,     0,     0,
       0,     0,   103,   103,    21,    22,   708,     0,     0,     0,
       0,     0,    28,     0,  -131,    72,     0,     0,     0,   217,
     106,     0,     0,   106,     0,     0,   108,     0,     0,   216,
       0,   709,  -131,   710,    42,     0,     0,     0,     0,     0,
       0,     0,   104,   104,   104,   104,   711,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,   104,   104,     0,
       0,   231,     0,   108,     0,     0,   108,     0,   277,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,     0,   106,     0,     0,   106,     0,     0,   106,
       0,     0,   106,     0,     0,     0,   232,    65,     0,   233,
       0,     0,     0,     0,     0,     0,   109,     0,   313,   109,
       0,     0,   106,     0,     0,     0,     0,     0,     0,   234,
     106,     0,   106,     0,   108,     0,   108,   106,   332,   108,
       0,     0,   108,     0,   235,   108,   236,   237,   238,     0,
     239,   240,   241,     0,   242,   243,   244,   245,     0,   247,
     248,   249,   250,    72,   251,   108,   253,     0,     0,   254,
     255,   256,     0,   108,     0,   108,     0,   109,     0,   109,
     108,     0,   109,     0,     0,   109,     0,     0,   109,     0,
       0,     0,     0,     0,   123,     0,   106,     0,     0,   123,
     231,   106,     0,   146,   123,   123,     0,     0,   109,     0,
       0,   160,     0,   163,     0,     0,   109,     0,   109,     0,
       0,     0,     0,   109,     0,   106,     0,     0,     0,   106,
       0,     0,   190,     0,     0,   232,     0,    72,   233,   108,
       0,     0,     0,     0,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   332,     0,     0,   332,     0,   231,     0,   108,     0,
     219,     0,   108,   235,     0,     0,     0,   238,     0,   239,
     240,     0,   109,     0,   243,     0,     0,   109,     0,     0,
       0,   250,     0,     0,   106,   106,   106,   106,   254,   255,
     256,   232,     0,     0,   233,     0,     0,     0,     0,   106,
     106,   109,   504,     0,     0,   109,     0,     0,     0,     0,
       0,     0,     0,   190,     0,     0,     0,   291,     0,     0,
       0,     0,     0,   190,     0,   300,     0,   108,   108,   108,
     108,   291,     0,   238,   318,   239,   240,     0,     0,     0,
     243,     0,   108,   108,   291,     0,   123,   250,     0,     0,
       0,     0,     0,     0,   254,   255,   256,     0,   291,     0,
       0,     0,     0,     0,   340,     0,     0,     0,     0,     0,
     109,   109,   109,   109,     5,     6,     7,     8,     9,     0,
       0,   125,     0,     0,     0,   109,   109,     0,     0,     0,
     360,     0,     0,     0,    20,   163,   163,   163,   163,   126,
     127,   128,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,   385,    34,    35,     0,     0,   580,     0,     0,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   129,     0,     0,     0,
       0,     0,     0,     0,   291,   291,     0,     0,     0,     0,
      55,     0,     0,     0,   610,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,   451,
       0,   455,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,   192,   318,
       0,     0,    66,     0,     0,   130,     0,    68,   163,   131,
     775,     0,     0,     0,     0,     0,   291,     0,     0,     0,
       0,     0,     0,     0,     0,   332,     0,   332,     0,     0,
     332,     0,     0,   332,     0,     0,   684,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   694,     0,     0,     0,
       0,     0,     0,     0,   699,     0,   703,     0,     0,     0,
       0,    72,     0,    -2,     4,     0,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -236,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,  -131,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -236,    37,    38,    39,    40,  -131,    41,    42,     0,
    -236,    43,   291,    44,    45,    46,    47,    48,    49,     0,
      50,   231,    51,    52,  -236,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,   584,     0,     0,   318,   754,
       0,     0,     0,   756,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,   232,   190,     0,   233,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,   652,    68,
       0,    69,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,   236,   237,   238,     0,
     239,   240,   241,     0,     0,   243,   244,   245,     0,   247,
     248,   249,   250,     0,     0,     0,   253,     0,     0,   254,
     255,   256,     0,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,   318,     5,     6,     7,     8,
       9,     0,  -381,    10,    11,    12,  -381,    13,    14,    15,
      16,  -236,    17,    18,  -381,    19,    20,  -381,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,  -131,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -236,    37,   716,    39,    40,  -131,    41,    42,  -381,
    -236,    43,  -381,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -236,     0,     0,    53,     0,    54,
       0,     0,  -381,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -381,    57,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,     0,  -381,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,    62,  -381,  -381,  -381,
       0,    65,  -381,  -381,  -381,     0,     0,    67,  -381,    68,
     123,    69,     4,   190,     5,     6,     7,     8,     9,   759,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -236,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,  -131,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -236,
      37,    38,    39,    40,  -131,    41,    42,     0,  -236,    43,
       0,    44,    45,    46,    47,    48,    49,     0,    50,     0,
      51,    52,  -236,     0,     0,    53,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,   192,    65,
       0,     0,    66,     0,     0,    67,   203,    68,     4,    69,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -236,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,  -131,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -236,    37,    38,    39,    40,
    -131,    41,    42,     0,  -236,    43,     0,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -236,     0,
       0,    53,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
      62,     0,    63,    64,     0,    65,     0,     0,    66,     0,
       0,    67,   345,    68,     4,    69,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -236,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,  -131,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -236,    37,    38,    39,    40,  -131,    41,    42,     0,
    -236,    43,     0,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -236,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,   203,    68,
       4,    69,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -236,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,  -131,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -236,    37,    38,
      39,    40,  -131,    41,    42,     0,  -236,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -236,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,   612,    68,     4,    69,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -236,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -131,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -236,    37,    38,    39,    40,  -131,    41,
      42,     0,  -236,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -236,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
     707,    68,     4,    69,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -236,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,  -131,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -236,
      37,   216,    39,    40,  -131,    41,    42,     0,  -236,    43,
       0,    44,    45,    46,    47,    48,    49,     0,    50,     0,
      51,    52,  -236,     0,     0,    53,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,     0,    65,
       0,     0,    66,     0,     0,    67,     0,    68,     4,    69,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -236,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,  -131,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -236,    37,     0,    39,    40,
    -131,    41,    42,     0,  -236,    43,     0,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -236,     0,
       0,    53,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
      62,     0,    63,    64,     0,    65,     0,     0,    66,     0,
       0,    67,     0,    68,     4,    69,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -236,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,   458,     0,    27,    28,     0,  -131,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -236,    37,     0,    39,    40,  -131,    41,    42,     0,
    -236,    43,     0,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -236,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,     0,    68,
       4,    69,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -236,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,   462,
       0,    27,    28,     0,  -131,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -236,    37,     0,
      39,    40,  -131,    41,    42,     0,  -236,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -236,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,     0,    68,     4,    69,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -236,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   673,     0,    27,    28,     0,
    -131,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -236,    37,     0,    39,    40,  -131,    41,
      42,     0,  -236,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -236,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,     4,    69,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -236,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,   675,     0,    27,    28,     0,  -131,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -236,
      37,     0,    39,    40,  -131,    41,    42,     0,  -236,    43,
       0,    44,    45,    46,    47,    48,    49,     0,    50,     0,
      51,    52,  -236,     0,     0,    53,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,     0,    65,
       0,     0,    66,     0,     0,    67,     0,    68,     4,    69,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -236,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,   678,     0,    27,
      28,     0,  -131,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -236,    37,     0,    39,    40,
    -131,    41,    42,     0,  -236,    43,     0,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -236,     0,
       0,    53,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
      62,     0,    63,    64,     0,    65,     0,     0,    66,     0,
       0,    67,     0,    68,     4,    69,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -236,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,   681,     0,    27,    28,     0,  -131,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -236,    37,     0,    39,    40,  -131,    41,    42,     0,
    -236,    43,     0,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -236,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,     0,    68,
       4,    69,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -236,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,   698,
       0,    27,    28,     0,  -131,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -236,    37,     0,
      39,    40,  -131,    41,    42,     0,  -236,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -236,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,     0,    68,     4,    69,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -236,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   702,     0,    27,    28,     0,
    -131,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -236,    37,     0,    39,    40,  -131,    41,
      42,     0,  -236,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -236,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,   478,    69,     5,     6,     7,     8,     9,     0,
    -381,   125,     0,     0,  -381,     0,     0,     0,     0,     0,
       0,     0,  -381,     0,    20,     0,     0,     0,     0,   126,
     127,   128,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    34,    35,     0,     0,     0,     0,     0,
      37,     0,    39,     0,     0,     0,     0,  -381,     0,     0,
    -381,     0,     0,    46,    47,    48,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -254,
    -381,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -381,    57,  -381,  -381,  -381,
       0,  -381,  -381,  -381,     0,  -381,  -381,  -381,  -381,  -381,
    -381,  -381,  -381,  -381,    62,  -381,  -381,  -381,     0,     0,
    -381,  -381,  -381,     0,     0,   130,     0,    68,   478,   492,
       5,     6,     7,     8,     9,     0,  -381,   125,     0,     0,
    -381,     0,     0,     0,     0,     0,     0,     0,  -381,     0,
      20,     0,     0,     0,     0,   126,   127,   128,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,  -381,     0,     0,  -381,     0,     0,    46,
      47,    48,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -381,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -381,    57,  -381,  -381,  -381,     0,  -381,  -381,  -381,
       0,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
      62,  -381,  -381,  -381,     0,     0,  -381,  -381,  -381,     0,
       0,   130,     0,    68,   478,   492,     5,     6,     7,     8,
       9,     0,     0,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,     0,     0,     0,
       0,   126,   127,   128,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,   478,     0,     5,     6,     7,     8,
       9,     0,     0,   125,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,    20,     0,     0,     0,
       0,   126,   127,   128,    61,    27,    62,     0,    63,    64,
      30,    31,     0,    33,    66,    34,    35,   130,     0,    68,
       0,   479,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   125,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,    20,     0,     0,     0,
       0,   126,   127,   128,    61,    27,    62,     0,    63,    64,
      30,    31,     0,    33,    66,    34,    35,   130,     0,    68,
       0,   492,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   129,     0,
       0,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,   208,     0,     5,     6,
       7,     8,     9,     0,    61,   125,    62,     0,    63,    64,
     192,     0,     0,     0,    66,     0,     0,   130,    20,    68,
       0,   131,     0,   126,   127,   128,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       5,     6,     7,     8,     9,     0,    61,   125,    62,     0,
      63,    64,   192,     0,     0,     0,    66,     0,     0,   130,
      20,    68,   499,   131,     0,   126,   127,   128,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,   153,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   125,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   126,   127,   128,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   130,     0,    68,     0,   131,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   129,     0,     0,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     5,     6,     7,     8,     9,     0,    61,   125,
      62,     0,    63,    64,   192,     0,     0,     0,    66,     0,
       0,   130,    20,    68,     0,   131,     0,   126,   127,   128,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,   177,     0,     0,
      66,     0,     0,   130,     0,    68,     0,   131,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,    20,     0,
       0,     0,     0,   126,   127,   128,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   128,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,    67,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       5,     6,     7,     8,     9,     0,    61,   125,    62,     0,
      63,    64,   192,     0,     0,     0,    66,     0,     0,   130,
      20,    68,     0,   131,     0,   126,   127,   128,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     5,     6,     7,     8,     9,     0,    61,   125,
      62,     0,    63,    64,     0,     0,     0,     0,    66,     0,
       0,   130,    20,    68,     0,   131,     0,   126,   127,   128,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,   596,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   125,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,   737,     0,     0,     0,     0,   126,   127,   128,
      61,    27,    62,     0,    63,    64,    30,    31,     0,    33,
      66,    34,    35,   130,     0,    68,     0,   131,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   738,    47,    48,   739,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     5,     6,     7,     8,     9,     0,
      61,   125,    62,     0,    63,    64,   192,     0,     0,     0,
      66,     0,     0,   130,    20,    68,     0,   740,     0,   126,
     127,   128,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    34,    35,     0,     0,     0,     0,     0,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   129,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     5,     6,     7,     8,
       9,     0,    61,   125,    62,     0,    63,    64,   192,     0,
       0,     0,    66,     0,     0,   130,    20,    68,     0,   740,
       0,   126,   127,   728,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   129,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     5,     6,
       7,     8,     9,     0,    61,   125,    62,     0,    63,    64,
     192,     0,     0,     0,    66,     0,     0,   130,    20,    68,
       0,   131,     0,   126,   127,   128,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   545,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   548,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   722,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   723,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   725,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   726,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   727,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   728,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,    20,     0,
       0,     0,     0,   126,   127,   128,    61,    27,    62,     0,
      63,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   125,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,     0,    20,     0,
       0,     0,     0,   126,   127,   128,    61,    27,    62,     0,
       0,    64,    30,    31,     0,    33,    66,    34,    35,   130,
       0,    68,     0,   131,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     0,    46,    47,    48,
     129,   616,     0,     0,     0,   617,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,  -263,     0,     0,     0,  -263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
       0,     0,   192,     0,     0,     0,     0,     0,     0,   130,
       0,    68,  -263,   740,     0,     0,     0,     0,     0,     0,
       0,     0,   618,   619,   620,   621,   622,   623,     0,     0,
     624,   625,   626,   627,   628,   629,   630,   631,   632,   633,
       0,     0,   634,     0,     0,     0,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,   645,     0,   646,     0,
       0,   647,   648,   649,   650,   651,   229,     0,     0,     0,
     230,     0,     0,     0,     0,     0,     0,     0,   231,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,     0,   230,     0,     0,     0,     0,     0,
       0,     0,   231,   232,     0,     0,   233,     0,     0,     0,
       0,   271,  -343,     0,     0,  -365,     0,  -365,     0,     0,
       0,     0,   119,     0,   229,     0,   234,     0,   230,     0,
       0,     0,     0,     0,     0,     0,   231,   232,     0,     0,
     233,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     234,   251,   252,   253,     0,     0,   254,   255,   256,     0,
       0,   232,     0,     0,   233,   235,   608,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   234,   251,   252,   253,     0,     0,
     254,   255,   256,     0,     0,     0,     0,     0,     0,   235,
     665,   236,   237,   238,     0,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   229,   251,
     252,   253,   230,     0,   254,   255,   256,     0,     0,     0,
     231,     0,     0,     0,   784,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,     0,   230,     0,     0,     0,
       0,     0,     0,     0,   231,   232,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,   233,   235,     0,   236,   237,   238,     0,   239,
     240,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   234,   251,   252,   253,     0,     0,   254,   255,
     256,     0,     0,     0,     0,     0,     0,   235,   818,   236,
     237,   238,     0,   239,   240,   241,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   229,   251,   252,   253,
     230,     0,   254,   255,   256,     0,     0,     0,   231,   306,
     498,     0,     0,     0,     0,     0,     0,     0,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     229,   251,   252,   253,   230,     0,   254,   255,   256,     0,
       0,   279,   231,   303,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,     0,
     234,   230,     0,     0,     0,     0,     0,     0,     0,   231,
     575,     0,     0,     0,     0,   235,     0,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,     0,
     254,   255,   256,     0,   232,   279,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   119,   229,     0,     0,   234,   230,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,   235,     0,   236,   237,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   253,     0,     0,   254,   255,   256,
       0,   232,   279,     0,   233,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,   234,   230,     0,     0,     0,     0,
       0,     0,     0,   231,   180,     0,     0,     0,     0,   235,
       0,   236,   237,   238,     0,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,     0,   254,   255,   256,     0,   232,   279,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,     0,
       0,   234,   230,     0,     0,     0,     0,     0,     0,     0,
     231,   569,     0,     0,     0,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,   253,     0,
       0,   254,   255,   256,     0,   232,   279,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,     0,     0,   234,   230,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,   235,     0,   236,   237,   238,     0,   239,
     240,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     0,   251,   252,   253,     0,     0,   254,   255,
     256,     0,   232,   279,     0,   233,     0,     0,     0,     0,
       0,     0,   672,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,   234,   230,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     235,     0,   236,   237,   238,     0,   239,   240,   241,     0,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,   253,     0,     0,   254,   255,   256,     0,   232,
     279,     0,   233,     0,     0,     0,     0,     0,     0,   677,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,   234,   230,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   235,     0,   236,
     237,   238,     0,   239,   240,   241,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     0,   251,   252,   253,
       0,     0,   254,   255,   256,     0,   232,   279,     0,   233,
       0,     0,     0,     0,     0,     0,   770,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,   234,
     230,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,   235,     0,   236,   237,   238,     0,
     239,   240,   241,     0,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   251,   252,   253,     0,     0,   254,
     255,   256,     0,   232,   279,     0,   233,     0,     0,     0,
       0,     0,     0,   771,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,     0,     0,   234,   230,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
       0,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       0,   251,   252,   253,     0,     0,   254,   255,   256,     0,
     232,   279,     0,   233,     0,     0,     0,     0,     0,     0,
     772,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,   234,   230,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   235,     0,
     236,   237,   238,     0,   239,   240,   241,     0,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
     253,     0,     0,   254,   255,   256,     0,   232,   279,     0,
     233,     0,     0,     0,     0,     0,     0,   773,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,     0,
     234,   230,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,     0,     0,   235,     0,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,     0,
     254,   255,   256,     0,   232,   279,     0,   233,     0,     0,
       0,     0,     0,     0,   785,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,   234,   230,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,   235,     0,   236,   237,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   253,     0,     0,   254,   255,   256,
       0,   232,   279,     0,   233,     0,     0,     0,     0,     0,
       0,   786,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,   234,   230,     0,     0,     0,     0,
       0,     0,     0,   231,     0,     0,     0,     0,     0,   235,
       0,   236,   237,   238,     0,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,     0,   254,   255,   256,     0,   232,   279,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,     0,
       0,   234,   230,     0,     0,     0,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,   253,     0,
       0,   254,   255,   256,     0,   232,   331,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,   234,     0,
     230,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,   235,     0,   236,   237,   238,   285,   239,
     240,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     0,   251,   252,   253,     0,     0,   254,   255,
     256,     0,  -344,   232,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,     0,   229,     0,   234,     0,   230,     0,
       0,     0,     0,     0,     0,     0,   231,   435,     0,     0,
       0,   235,     0,   236,   237,   238,   436,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       0,   251,   252,   253,     0,     0,   254,   255,   256,     0,
       0,   232,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,   234,     0,   230,     0,     0,     0,
       0,     0,     0,     0,   231,   437,     0,     0,     0,   235,
       0,   236,   237,   238,   438,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,     0,   254,   255,   256,     0,     0,   232,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,   234,   230,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   235,     0,   236,
     237,   238,     0,   239,   240,   241,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     0,   251,   252,   253,
       0,     0,   254,   255,   256,     0,   232,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,   234,
     230,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,   235,     0,   236,   237,   238,     0,
     239,   240,   241,     0,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   251,   252,   253,     0,   289,   254,
     255,   256,     0,   232,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,     0,     0,   234,   230,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
       0,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       0,   251,   252,   253,     0,   330,   254,   255,   256,     0,
     232,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,   234,   230,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   235,     0,
     236,   237,   238,     0,   239,   240,   241,     0,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
     253,     0,   344,   254,   255,   256,     0,   232,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,   356,
     234,   230,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,     0,     0,   235,     0,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,     0,
     254,   255,   256,     0,   232,     0,     0,   233,     0,     0,
       0,     0,     0,     0,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,   234,   230,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,   235,     0,   236,   237,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   253,     0,     0,   254,   255,   256,
       0,   232,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,   234,   230,     0,     0,     0,     0,
       0,     0,     0,   231,     0,     0,     0,     0,     0,   235,
       0,   236,   237,   238,     0,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,   522,   254,   255,   256,     0,   232,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,     0,
       0,   234,   230,     0,     0,     0,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,   253,     0,
     523,   254,   255,   256,     0,   232,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,     0,     0,   234,   230,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,   235,     0,   236,   237,   238,     0,   239,
     240,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     0,   251,   252,   253,     0,   524,   254,   255,
     256,     0,   232,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,   234,   230,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     235,     0,   236,   237,   238,     0,   239,   240,   241,     0,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,   253,     0,   525,   254,   255,   256,     0,   232,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,   234,   230,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   235,     0,   236,
     237,   238,     0,   239,   240,   241,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     0,   251,   252,   253,
       0,   526,   254,   255,   256,     0,   232,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,   234,
     230,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,   235,     0,   236,   237,   238,     0,
     239,   240,   241,     0,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   251,   252,   253,     0,   527,   254,
     255,   256,     0,   232,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,     0,     0,   234,   230,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
       0,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       0,   251,   252,   253,     0,   528,   254,   255,   256,     0,
     232,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,   234,   230,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   235,     0,
     236,   237,   238,     0,   239,   240,   241,     0,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
     253,     0,   529,   254,   255,   256,     0,   232,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,     0,
     234,   230,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,     0,     0,   235,     0,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,   530,
     254,   255,   256,     0,   232,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,   234,   230,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
       0,     0,   235,     0,   236,   237,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   253,     0,   531,   254,   255,   256,
       0,   232,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,   234,   230,     0,     0,     0,     0,
       0,     0,     0,   231,     0,     0,     0,     0,     0,   235,
       0,   236,   237,   238,     0,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,   532,   254,   255,   256,     0,   232,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,     0,
       0,   234,   230,     0,     0,     0,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,   253,     0,
     533,   254,   255,   256,     0,   232,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,     0,     0,   234,   230,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,   235,     0,   236,   237,   238,     0,   239,
     240,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     0,   251,   252,   253,     0,   534,   254,   255,
     256,     0,   232,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,   234,   230,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     235,     0,   236,   237,   238,     0,   239,   240,   241,     0,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,   253,     0,   535,   254,   255,   256,     0,   232,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,   234,   230,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   235,     0,   236,
     237,   238,     0,   239,   240,   241,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     0,   251,   252,   253,
       0,   536,   254,   255,   256,     0,   232,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   229,     0,     0,   234,
     230,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,   235,     0,   236,   237,   238,     0,
     239,   240,   241,     0,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   251,   252,   253,     0,     0,   254,
     255,   256,     0,   232,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,     0,     0,   234,   230,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,   579,     0,
       0,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       0,   251,   252,   253,     0,   555,   254,   255,   256,     0,
     232,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,   234,   230,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   235,     0,
     236,   237,   238,     0,   239,   240,   241,     0,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
     253,     0,     0,   254,   255,   256,     0,   232,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,   611,
     234,   230,     0,     0,     0,     0,     0,     0,     0,   231,
     660,     0,     0,     0,     0,   235,     0,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,     0,
     254,   255,   256,     0,   232,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,   234,   230,     0,
       0,     0,     0,     0,     0,     0,   231,   663,     0,     0,
       0,     0,   235,     0,   236,   237,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,   253,     0,     0,   254,   255,   256,
       0,   232,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,   234,   230,     0,     0,     0,     0,
       0,     0,     0,   231,     0,     0,     0,     0,     0,   235,
       0,   236,   237,   238,     0,   239,   240,   241,     0,   242,
     243,   244,   245,   246,   247,   248,   249,   250,     0,   251,
     252,   253,     0,     0,   254,   255,   256,     0,   232,     0,
       0,   233,     0,     0,     0,     0,     0,     0,   721,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   229,     0,
       0,   234,   230,     0,     0,     0,     0,     0,     0,     0,
     231,     0,     0,     0,     0,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,   253,     0,
       0,   254,   255,   256,     0,   232,     0,     0,   233,     0,
       0,     0,     0,     0,     0,   724,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   229,     0,     0,   234,   230,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,   235,     0,   236,   237,   238,     0,   239,
     240,   241,     0,   242,   243,   244,   245,   246,   247,   248,
     249,   250,     0,   251,   252,   253,     0,     0,   254,   255,
     256,     0,   232,     0,     0,   233,     0,     0,     0,     0,
       0,     0,   791,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,   234,   230,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,     0,     0,
     235,     0,   236,   237,   238,     0,   239,   240,   241,     0,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,   253,     0,     0,   254,   255,   256,     0,   232,
       0,     0,   233,     0,     0,     0,     0,     0,     0,   792,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,   234,   230,     0,     0,     0,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,   235,     0,   236,
     237,   238,     0,   239,   240,   241,     0,   242,   243,   244,
     245,   246,   247,   248,   249,   250,     0,   251,   252,   253,
       0,     0,   254,   255,   256,     0,   232,     0,     0,   233,
       0,     0,     0,     0,     0,     0,   793,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,   234,
     230,     0,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,   235,     0,   236,   237,   238,     0,
     239,   240,   241,     0,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   251,   252,   253,     0,     0,   254,
     255,   256,     0,   232,     0,     0,   233,     0,     0,     0,
       0,     0,     0,   794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   229,     0,     0,   234,   230,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,     0,
       0,   235,     0,   236,   237,   238,     0,   239,   240,   241,
       0,   242,   243,   244,   245,   246,   247,   248,   249,   250,
       0,   251,   252,   253,     0,     0,   254,   255,   256,     0,
     232,     0,     0,   233,     0,     0,     0,     0,     0,     0,
     795,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,   234,   230,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,     0,     0,   235,     0,
     236,   237,   238,     0,   239,   240,   241,     0,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
     253,     0,     0,   254,   255,   256,     0,   232,     0,     0,
     233,     0,     0,     0,     0,     0,     0,   796,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,     0,
     234,   230,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,     0,     0,   235,     0,   236,   237,   238,
       0,   239,   240,   241,     0,   242,   243,   244,   245,   246,
     247,   248,   249,   250,     0,   251,   252,   253,     0,     0,
     254,   255,   256,   231,   232,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,   233,   235,     0,   236,   237,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   234,   251,   252,   253,     0,     0,   254,   255,   256,
       0,     0,     0,     0,     0,     0,   235,     0,   236,   237,
     238,     0,   239,   240,   241,     0,   242,   243,   244,   245,
       0,   247,   248,   249,   250,     0,   251,   252,   253,     0,
       0,   254,   255,   256
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-710))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,     2,   317,    40,    41,    15,   322,    14,    18,   337,
     593,   276,   141,   276,    24,    25,    26,    31,    22,     3,
      22,   337,   151,    33,    34,     3,    36,    37,   477,     1,
       1,   654,   614,    43,    44,    45,    46,     3,    58,    49,
       3,   157,    52,    53,    54,    58,    31,    57,    22,   614,
      60,    61,    62,    52,    64,   159,    35,    67,    68,    69,
     223,   224,   225,   226,    22,    22,   775,    99,   172,    75,
      69,    99,     3,    75,    53,    99,   126,    31,   128,     3,
      75,    99,   186,    31,    68,    99,    58,    58,    31,    31,
      75,    57,    23,   125,    18,     1,   124,   213,     1,   808,
     748,    73,    73,   127,   124,   118,   689,    31,     3,   127,
      34,   124,     0,    24,    99,   125,   126,   127,   128,   129,
     130,   131,   124,    47,    48,    31,    98,    98,    31,    46,
     137,   713,   131,    57,   129,   145,    67,   760,   761,   787,
     589,    99,   126,    67,   129,    99,   118,   729,   126,   159,
     124,    99,   124,   124,    99,    72,    99,    99,    14,    80,
     126,   476,   172,   126,   729,    60,   124,   124,   272,   273,
     174,    31,    78,     3,    80,   129,   186,    80,    99,   189,
      80,   129,   103,    99,   129,    67,   129,   129,    46,    99,
     189,   774,    98,    99,    68,    98,    99,    57,   208,    55,
     808,   364,   126,   103,   205,     3,   127,   126,   125,     7,
     818,   127,   118,    69,    72,   118,     7,   127,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     334,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   715,   125,   583,     3,
      99,   145,   126,     7,   128,   285,   592,   542,   126,   542,
     290,    99,    99,    99,    99,   159,    31,     2,    18,   118,
     126,   301,   128,   303,   304,    10,   306,   307,   172,   126,
     310,    35,   301,   118,    19,   126,     2,    48,   125,   127,
      80,   127,   186,    58,    10,   325,    57,    32,    48,    53,
      21,   232,   233,    19,   334,    12,    67,    57,    73,    99,
      17,   341,    21,   103,    49,    80,    32,    67,    64,   349,
      99,   351,    99,   353,   354,   118,   356,   357,   464,   465,
      18,   361,    67,    49,    99,    56,    71,   127,    59,   118,
      41,   118,   361,   274,   275,   693,   495,    56,   118,   697,
      59,    67,   688,   118,   285,    71,    99,   693,   118,   124,
      48,   697,    99,   118,   123,    98,   502,   503,   101,    57,
     118,    99,   105,   304,   118,   118,   307,    98,   272,   273,
     101,   118,     3,    71,   105,   120,     7,     8,   118,    98,
     118,   100,   101,   118,   126,    56,   105,    18,    59,    78,
     349,    80,   351,   112,   120,   435,   436,   437,   438,   439,
     440,   441,   121,   118,   124,   442,   443,   447,   448,   118,
     450,   126,   353,   128,    74,   160,   456,    48,   458,   124,
     126,   101,   462,   126,   126,    80,    57,   118,     7,   775,
     334,    98,   118,   127,   160,   180,    67,   341,   784,   479,
      71,    99,   482,   483,   118,   349,    14,   351,    31,    17,
     479,    24,   492,    99,   180,   127,    78,    25,    26,    27,
     205,   127,   808,   492,    80,    33,   127,    35,   127,    31,
      99,   124,   818,   129,   124,   127,   155,   156,   157,   205,
     129,    56,    50,    80,    52,    53,    54,    55,    24,   127,
     169,   127,    99,    22,    22,   436,    58,   438,     2,    67,
     124,    69,   129,    98,   183,   545,    10,   101,   548,   129,
       3,    73,   129,    21,     7,    19,   126,   196,    80,   127,
     560,    73,   491,   509,   152,    18,   455,   567,    32,   569,
     670,   561,   572,   717,   279,   575,   717,    99,   718,   579,
     581,   615,   787,   774,   448,    49,   450,   758,    56,   326,
     118,    59,   450,   279,   736,    48,   118,   125,   303,   599,
     542,   306,   124,    67,    57,   310,   606,    71,   608,    -1,
     599,   611,    -1,    -1,    67,    -1,    -1,   303,    71,    -1,
     306,    -1,   613,    -1,   310,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,   114,   286,   116,   354,
      -1,   119,   120,   121,    -1,    -1,   120,    -1,    -1,   659,
     660,    31,   662,   663,   664,   665,    -1,    -1,   354,    -1,
     309,    -1,   672,   673,    -1,   675,    21,   677,   678,    -1,
      -1,   681,   709,   710,    -1,    -1,    -1,    -1,    58,    -1,
      -1,    -1,   692,   693,    -1,    -1,   160,   697,   698,    17,
      -1,    -1,   702,    73,    -1,    -1,    -1,    -1,    26,    27,
      80,    56,    -1,    -1,    59,    33,   180,    35,    -1,    -1,
      -1,   721,   722,   723,   724,   725,   726,   727,   728,    99,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,   738,   739,
     740,   205,    -1,    -1,   744,   431,    -1,   747,   118,    94,
      18,   740,    97,    98,   124,   100,   101,    -1,    -1,    -1,
     105,    -1,    -1,    31,   469,    -1,    34,   112,    -1,    -1,
     770,   771,   772,   773,   119,   120,   121,    -1,    -1,    47,
      48,    -1,    -1,   469,   784,   785,   786,    -1,    -1,    57,
      -1,   791,   792,   793,   794,   795,   796,    -1,    -1,    67,
      -1,   506,    -1,    -1,   509,    -1,    -1,   807,   808,    -1,
      -1,    -1,    -1,    -1,    -1,   279,    -1,    -1,   818,     2,
     506,   460,   461,   509,   688,   464,   465,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    19,    45,    -1,   303,
      -1,    -1,   306,    -1,    26,    27,   310,    -1,    -1,    32,
      -1,    33,    -1,    35,    -1,    -1,    -1,    -1,   544,    41,
      -1,    -1,   567,    -1,   569,    -1,    49,   572,    -1,    -1,
     575,    53,    -1,   578,     2,    -1,    -1,    -1,    -1,    -1,
     744,   567,    10,   569,    67,    -1,   572,    -1,    71,   575,
     354,    19,   578,   598,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   606,    -1,   608,    32,    -1,    -1,    -1,   613,    -1,
      -1,   775,   598,    -1,   553,   554,   124,    -1,    -1,    -1,
     606,    49,   608,    -1,    -1,   564,    -1,   613,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   574,    -1,   120,   577,    67,
      -1,    -1,    -1,    71,   808,    -1,    17,   155,   156,   157,
     158,    -1,    -1,    -1,   818,    26,    27,    -1,    -1,    -1,
      21,   169,    33,    -1,    35,    -1,    -1,   672,    14,    -1,
      41,    17,   677,    -1,    -1,   183,    -1,   160,    -1,    25,
      26,    27,    53,    -1,    -1,    -1,   672,    33,   196,    35,
      -1,   677,   120,    -1,    -1,    56,   701,   180,    59,    -1,
     705,    -1,    -1,    -1,    50,   469,    52,    53,    54,    55,
      -1,    -1,    -1,    -1,    -1,   701,    -1,    -1,    -1,   705,
      -1,    67,   205,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    94,    18,    96,    97,    98,    -1,   100,
     101,    -1,   506,    -1,   105,   509,    -1,    31,    -1,    -1,
      34,   112,   180,    -1,    -1,    -1,   695,    -1,   119,   120,
     121,    -1,    -1,    47,    48,   770,   771,   772,   773,    -1,
      -1,    -1,   118,    57,    -1,    -1,    -1,   205,   286,   125,
     785,   786,    -1,    67,   770,   771,   772,   773,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,   279,    -1,    -1,   785,
     786,   309,    -1,   567,    -1,   569,    -1,    -1,   572,    -1,
      94,   575,    96,    97,   578,    -1,     2,    -1,    -1,    -1,
     303,    -1,    -1,   306,    10,    -1,    -1,   310,    -1,    -1,
      -1,   115,   116,    19,   598,    -1,    -1,   121,    -1,    -1,
      -1,    -1,   606,    -1,   608,    -1,    32,    -1,    -1,   613,
      -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    10,    -1,    -1,
      -1,   354,    -1,    -1,    -1,   303,    19,    -1,   306,    -1,
      -1,    67,   310,    -1,    -1,    71,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,    -1,    21,    49,    -1,   672,    -1,
      10,    -1,    -1,   677,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    67,    -1,   354,    -1,    71,    -1,
      -1,    -1,    32,    -1,   120,    -1,    -1,   701,    -1,    -1,
      56,   705,    -1,    59,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,   460,   461,    -1,    -1,   464,   465,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    71,    -1,    -1,   160,    -1,    -1,   120,    94,    -1,
      96,    97,    98,    -1,   100,   101,   469,    -1,    -1,   105,
      -1,    -1,    -1,    -1,   180,   111,   112,    -1,    -1,    -1,
     116,    -1,    -1,   119,   120,   121,   770,   771,   772,   773,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,   205,
     120,   785,   786,   506,    -1,    -1,   509,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,
      -1,   469,    -1,    -1,    -1,   553,   554,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,   564,    -1,   566,    -1,
     160,    -1,   205,   571,    -1,    -1,   574,    -1,    23,   577,
      -1,    -1,    -1,    28,    29,    30,    -1,    32,   506,    -1,
     180,   509,    37,    38,   567,    40,   569,    42,    43,   572,
      -1,    -1,   575,   279,    49,   578,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,   598,    -1,   303,    -1,    -1,
     306,    -1,    -1,   606,   310,   608,    -1,    -1,    -1,    -1,
     613,    -1,    -1,    -1,    -1,    -1,   279,    -1,    -1,   567,
      95,   569,    -1,    -1,   572,    -1,    -1,   575,    -1,    -1,
     578,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
     303,    -1,    21,   306,    -1,    -1,    -1,   310,   354,    -1,
     598,   126,    -1,   128,    -1,    -1,    -1,    -1,   606,   279,
     608,    -1,    -1,    -1,    -1,   613,    -1,   695,    -1,   672,
      -1,    -1,    -1,    -1,   677,    -1,    -1,    56,    -1,    -1,
      59,    -1,    -1,   303,    -1,    -1,   306,    -1,    -1,    -1,
     310,   354,    -1,    -1,    -1,    -1,    -1,    -1,   701,    -1,
      -1,    -1,   705,   731,   732,    -1,   734,   735,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
     748,   100,   101,   102,   672,   753,   105,   755,    -1,   677,
       2,    -1,   111,   112,   354,    -1,    -1,   116,    10,    -1,
     119,   120,   121,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   701,    -1,    -1,    -1,   705,    -1,   787,
      32,    -1,    -1,   469,    -1,    -1,    -1,   770,   771,   772,
     773,    -1,    -1,    14,    -1,    -1,    17,    49,    -1,    -1,
      -1,    -1,   785,   786,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    67,    -1,    -1,    -1,    71,
     506,    -1,    -1,   509,    -1,    -1,   469,    -1,    -1,    50,
      -1,    52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   770,   771,   772,   773,    67,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   785,   786,    -1,
      -1,    21,    -1,   506,    -1,    -1,   509,    -1,   120,   469,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   567,    -1,   569,    -1,    -1,   572,    -1,    -1,   575,
      -1,    -1,   578,    -1,    -1,    -1,    56,   118,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,   506,    -1,   160,   509,
      -1,    -1,   598,    -1,    -1,    -1,    -1,    -1,    -1,    79,
     606,    -1,   608,    -1,   567,    -1,   569,   613,   180,   572,
      -1,    -1,   575,    -1,    94,   578,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,    -1,   109,
     110,   111,   112,   205,   114,   598,   116,    -1,    -1,   119,
     120,   121,    -1,   606,    -1,   608,    -1,   567,    -1,   569,
     613,    -1,   572,    -1,    -1,   575,    -1,    -1,   578,    -1,
      -1,    -1,    -1,    -1,    12,    -1,   672,    -1,    -1,    17,
      21,   677,    -1,    21,    22,    23,    -1,    -1,   598,    -1,
      -1,    29,    -1,    31,    -1,    -1,   606,    -1,   608,    -1,
      -1,    -1,    -1,   613,    -1,   701,    -1,    -1,    -1,   705,
      -1,    -1,    50,    -1,    -1,    56,    -1,   279,    59,   672,
      -1,    -1,    -1,    -1,   677,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   303,    -1,    -1,   306,    -1,    21,    -1,   701,    -1,
      88,    -1,   705,    94,    -1,    -1,    -1,    98,    -1,   100,
     101,    -1,   672,    -1,   105,    -1,    -1,   677,    -1,    -1,
      -1,   112,    -1,    -1,   770,   771,   772,   773,   119,   120,
     121,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,   785,
     786,   701,   354,    -1,    -1,   705,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,    -1,    -1,   151,    -1,   153,    -1,   770,   771,   772,
     773,   159,    -1,    98,   162,   100,   101,    -1,    -1,    -1,
     105,    -1,   785,   786,   172,    -1,   174,   112,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,    -1,   186,    -1,
      -1,    -1,    -1,    -1,   192,    -1,    -1,    -1,    -1,    -1,
     770,   771,   772,   773,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,   785,   786,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    23,   223,   224,   225,   226,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,   240,    42,    43,    -1,    -1,   469,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,   506,    -1,    -1,   509,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,   297,
      -1,   299,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,   117,   317,
      -1,    -1,   121,    -1,    -1,   124,    -1,   126,   326,   128,
     129,    -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   567,    -1,   569,    -1,    -1,
     572,    -1,    -1,   575,    -1,    -1,   578,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   364,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   598,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   606,    -1,   608,    -1,    -1,    -1,
      -1,   613,    -1,     0,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,   450,    60,    61,    62,    63,    64,    65,    -1,
      67,    21,    69,    70,    71,    -1,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    -1,   473,    -1,    -1,   476,   701,
      -1,    -1,    -1,   705,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    56,   495,    -1,    59,
      -1,    -1,    -1,    -1,   111,    -1,   113,    -1,   115,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,   516,   126,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,    -1,    -1,    -1,   116,    -1,    -1,   119,
     120,   121,    -1,   561,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,   583,     3,     4,     5,     6,
       7,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,   631,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    -1,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
     708,   128,     1,   711,     3,     4,     5,     6,     7,   717,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    -1,    -1,    74,    -1,    76,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,   117,   118,
      -1,    -1,   121,    -1,    -1,   124,   125,   126,     1,   128,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    -1,
      -1,    74,    -1,    76,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,   115,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,   125,   126,     1,   128,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    -1,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,   113,    -1,   115,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,   125,   126,
       1,   128,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    -1,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,   125,   126,     1,   128,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,    74,
      -1,    76,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,
     115,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,
     125,   126,     1,   128,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    -1,    -1,    74,    -1,    76,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,   124,    -1,   126,     1,   128,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    -1,
      -1,    74,    -1,    76,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,   115,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,    -1,   126,     1,   128,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    -1,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,   113,    -1,   115,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,    -1,   126,
       1,   128,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    -1,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,    -1,   126,     1,   128,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,    74,
      -1,    76,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,
     115,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,
      -1,   126,     1,   128,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    -1,    -1,    74,    -1,    76,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,   124,    -1,   126,     1,   128,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      63,    64,    65,    -1,    67,    -1,    69,    70,    71,    -1,
      -1,    74,    -1,    76,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     113,    -1,   115,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,   124,    -1,   126,     1,   128,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    63,    64,    65,    -1,
      67,    -1,    69,    70,    71,    -1,    -1,    74,    -1,    76,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,   113,    -1,   115,   116,
      -1,   118,    -1,    -1,   121,    -1,    -1,   124,    -1,   126,
       1,   128,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    -1,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,    -1,   126,     1,   128,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,    74,
      -1,    76,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,   113,    -1,
     115,   116,    -1,   118,    -1,    -1,   121,    -1,    -1,   124,
      -1,   126,     1,   128,     3,     4,     5,     6,     7,    -1,
       9,    10,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      59,    -1,    -1,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    -1,   124,    -1,   126,     1,   128,
       3,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    59,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,   124,    -1,   126,     1,   128,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   111,    32,   113,    -1,   115,   116,
      37,    38,    -1,    40,   121,    42,    43,   124,    -1,   126,
      -1,   128,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   111,    32,   113,    -1,   115,   116,
      37,    38,    -1,    40,   121,    42,    43,   124,    -1,   126,
      -1,   128,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,   103,    -1,     3,     4,
       5,     6,     7,    -1,   111,    10,   113,    -1,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,    -1,   124,    23,   126,
      -1,   128,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   111,    10,   113,    -1,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,    -1,   124,
      23,   126,   127,   128,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   111,    10,
     113,    -1,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
      -1,   124,    23,   126,    -1,   128,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,    -1,   126,    -1,   128,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   111,    10,   113,    -1,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,    -1,   124,
      23,   126,    -1,   128,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   111,    10,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,   121,    -1,
      -1,   124,    23,   126,    -1,   128,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     111,    32,   113,    -1,   115,   116,    37,    38,    -1,    40,
     121,    42,    43,   124,    -1,   126,    -1,   128,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,     3,     4,     5,     6,     7,    -1,
     111,    10,   113,    -1,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,    -1,   124,    23,   126,    -1,   128,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,   102,    -1,    -1,     3,     4,     5,     6,
       7,    -1,   111,    10,   113,    -1,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,    -1,   124,    23,   126,    -1,   128,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    -1,    -1,     3,     4,
       5,     6,     7,    -1,   111,    10,   113,    -1,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,    -1,   124,    23,   126,
      -1,   128,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
     115,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   111,    32,   113,    -1,
      -1,   116,    37,    38,    -1,    40,   121,    42,    43,   124,
      -1,   126,    -1,   128,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    62,    63,    64,
      65,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    -1,    -1,    99,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    81,    82,    83,    84,    85,    -1,    -1,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      -1,    -1,   100,    -1,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,    -1,
      -1,   119,   120,   121,   122,   123,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,   122,   123,    -1,    -1,   126,    -1,   128,    -1,    -1,
      -1,    -1,    75,    -1,     9,    -1,    79,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    56,    -1,    -1,
      59,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      79,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,    56,    -1,    -1,    59,    94,   129,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    79,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    94,
     129,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     9,   114,
     115,   116,    13,    -1,   119,   120,   121,    -1,    -1,    -1,
      21,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    59,    94,    -1,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    79,   114,   115,   116,    -1,    -1,   119,   120,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    94,   129,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     9,   114,   115,   116,
      13,    -1,   119,   120,   121,    -1,    -1,    -1,    21,    22,
     127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
       9,   114,   115,   116,    13,    -1,   119,   120,   121,    -1,
      -1,   124,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    56,   124,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,     9,    -1,    -1,    79,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,    -1,   119,   120,   121,
      -1,    56,   124,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,    -1,   119,   120,   121,    -1,    56,   124,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
      -1,   119,   120,   121,    -1,    56,   124,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,    -1,   119,   120,
     121,    -1,    56,   124,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,    -1,   119,   120,   121,    -1,    56,
     124,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,    -1,   119,   120,   121,    -1,    56,   124,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,    -1,   119,
     120,   121,    -1,    56,   124,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      56,   124,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,    -1,   119,   120,   121,    -1,    56,   124,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    56,   124,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,    -1,   119,   120,   121,
      -1,    56,   124,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,    -1,   119,   120,   121,    -1,    56,   124,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
      -1,   119,   120,   121,    -1,    56,   124,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    79,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    98,    31,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,    -1,   119,   120,
     121,    -1,   123,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,     9,    -1,    79,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    31,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    79,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    31,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,    -1,   119,   120,   121,    -1,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,    -1,   119,   120,   121,    -1,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,    -1,   119,   120,   121,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    56,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,   118,   119,
     120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,   118,   119,   120,   121,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,   118,
     119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,   118,   119,   120,   121,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    56,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
     118,   119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,   118,   119,   120,   121,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,   118,   119,   120,   121,    -1,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,    -1,   119,
     120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    24,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,   118,   119,   120,   121,    -1,
      56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,    -1,   119,   120,   121,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,    -1,   119,   120,   121,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,    -1,   119,   120,   121,    -1,    56,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
      -1,   119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,    -1,   119,   120,
     121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,    -1,   119,   120,   121,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,    -1,   119,   120,   121,    -1,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,    -1,   119,
     120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,    -1,   119,   120,   121,    -1,    56,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,    -1,
     119,   120,   121,    21,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,
      -1,    59,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    79,   114,   115,   116,    -1,    -1,   119,   120,   121,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
      -1,   119,   120,   121
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   134,   135,     0,     1,     3,     4,     5,     6,     7,
      10,    11,    12,    14,    15,    16,    17,    19,    20,    22,
      23,    25,    26,    27,    28,    29,    30,    32,    33,    36,
      37,    38,    39,    40,    42,    43,    45,    49,    50,    51,
      52,    54,    55,    58,    60,    61,    62,    63,    64,    65,
      67,    69,    70,    74,    76,    79,    94,    95,    96,    97,
     102,   111,   113,   115,   116,   118,   121,   124,   126,   128,
     136,   137,   138,   139,   140,   141,   143,   144,   146,   149,
     150,   151,   152,   153,   155,   156,   159,   160,   161,   164,
     167,   170,   171,   191,   194,   195,   213,   214,   215,   216,
     217,   218,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   234,   235,   236,   237,   238,   138,   226,    75,
     219,   220,   145,   146,   219,    10,    28,    29,    30,    65,
     124,   128,   167,   213,   217,   224,   225,   226,   227,   229,
     230,    67,   145,   226,   138,   126,   146,     7,   145,   147,
       8,    67,   147,    48,    77,   154,   226,   226,   226,   126,
     146,   168,   126,   146,   196,   197,   138,   226,   226,   226,
     226,     7,   126,    17,    27,   151,   151,   118,   226,   226,
      22,   141,   148,   226,   226,    64,   126,   138,   226,    60,
     146,   192,   117,   187,   207,   226,   226,   226,   226,   226,
     226,   226,   226,   125,   136,   142,   207,    68,   103,   187,
     208,   209,   226,   207,   226,   233,    50,   138,    41,   146,
      35,    53,   183,    18,    48,    57,    71,   118,   123,     9,
      13,    21,    56,    59,    79,    94,    96,    97,    98,   100,
     101,   102,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   114,   115,   116,   119,   120,   121,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,   122,   126,   128,    56,    59,   126,   138,   118,   124,
     141,   226,   226,   226,   207,    31,   219,   192,   118,   118,
      74,   146,   187,   210,   211,   212,   226,   124,   192,   161,
     146,   126,   148,    22,    31,   148,    22,    31,   148,   220,
      66,   141,   210,   138,   126,   178,    68,   126,   146,   198,
     199,     1,    98,   201,   202,    31,    99,   148,   210,   147,
     118,   124,   138,   148,   126,   210,   207,    80,   193,   118,
     146,    99,   118,   148,   118,   125,   136,   125,   226,    99,
     127,    99,   127,    31,   129,   220,    78,    99,   129,     7,
     146,    98,   162,   172,    57,   196,   196,   196,   196,   226,
     226,   226,   226,   154,   226,   154,   226,   226,   226,   226,
     226,   226,   226,    23,    67,   146,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   210,   210,   154,
     226,   154,   226,    18,    31,    34,    47,    48,    57,    67,
     116,   181,   221,   222,   238,    22,    31,    22,    31,    66,
      31,   129,   154,   226,   148,   118,   226,    78,    80,   127,
      99,   146,   165,   166,   118,   146,    31,   207,    30,   226,
     154,   226,    30,   226,   154,   226,   148,   138,   226,    24,
     127,   179,   180,   181,   169,   199,    99,   127,     1,   128,
     203,   214,    78,    80,   200,   226,   197,   127,   157,   210,
     127,   124,   128,   203,   214,    99,   187,   226,   127,   127,
     208,   208,   154,   226,   138,   226,   129,   226,   226,   124,
     207,   124,    48,    57,    67,   174,   182,   196,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   127,   129,    31,
      57,   213,    99,   127,    56,    30,   154,   226,    30,   154,
     226,   154,   226,   219,   219,   118,   226,   187,   226,   212,
      80,    99,   125,   162,   226,   127,   226,    22,   148,    22,
     148,   226,    22,   148,   220,    22,   148,   220,    24,    24,
     138,    99,   127,   126,   146,    18,    48,    57,    67,   184,
     127,   199,    98,   202,   207,   226,    44,   226,    46,    72,
     125,   158,   127,   157,   207,   192,   129,   220,   129,   220,
     138,    78,   125,   142,   163,   173,     9,    13,    80,    81,
      82,    83,    84,    85,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   100,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   116,   119,   120,   121,
     122,   123,   146,   175,   176,   118,   222,   213,   226,    22,
      22,   226,    22,    22,   129,   129,   148,   148,   226,   166,
     124,   148,    66,    30,   226,    30,   226,    66,    30,   226,
     148,    30,   226,   148,   138,   226,   180,   199,    98,   206,
     202,   200,    31,   129,   138,   207,   125,   129,    30,   138,
     226,   129,    30,   138,   226,   129,   226,   125,    27,    52,
      54,    67,   125,   137,   150,   184,   146,   101,   126,   177,
     177,    66,    30,    30,    66,    30,    30,    30,    30,   163,
     226,   226,   226,   226,   226,   226,   127,    23,    62,    65,
     128,   187,   205,   214,   103,   189,   200,    73,   190,   226,
     203,   214,   148,   226,   138,   226,   138,   150,   202,   146,
     175,   176,   179,   226,   226,   226,   226,   226,   226,   125,
      66,    66,    66,    66,   206,   129,   207,   187,   188,   226,
     226,   141,   149,   186,   129,    66,    66,   190,   177,   177,
     127,    66,    66,    66,    66,    66,    66,   226,   226,   226,
     226,   189,   200,   187,   204,   205,   214,    31,   129,   214,
     226,   226,   118,   185,   186,   226,   204,   205,   129,   204
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
#line 402 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 407 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 408 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 415 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 420 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 421 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 437 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 438 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 451 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 540 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 553 "chapel.ypp"
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

  case 79:

/* Line 1806 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 615 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { USR_FATAL((yyvsp[(3) - (6)].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 660 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 665 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 672 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 677 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 682 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 715 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 724 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 728 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    {
      (yyvsp[(3) - (8)].pfnsymbol)->retTag = (yyvsp[(5) - (8)].retTag);
      if ((yyvsp[(5) - (8)].retTag) == RET_REF)
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

  case 131:

/* Line 1806 of yacc.c  */
#line 771 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 777 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 784 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 803 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 809 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 816 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[(4) - (9)].pfnsymbol);

      fn->copyFlags((yyvsp[(1) - (9)].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[(1) - (9)].pfnsymbol)->name)
        fn->cname = (yyvsp[(1) - (9)].pfnsymbol)->name;

      if ((yyvsp[(2) - (9)].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (9)].pfnsymbol), (yyvsp[(6) - (9)].retTag), (yyvsp[(7) - (9)].pexpr), (yyvsp[(8) - (9)].pexpr), (yyvsp[(9) - (9)].pblockstmt), (yylsp[(1) - (9)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 838 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 842 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 851 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 967 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 984 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1003 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1016 "chapel.ypp"
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

  case 228:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1055 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
    }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1070 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1094 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1096 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1118 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1141 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1162 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1170 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1177 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1196 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1317 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1323 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1329 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1335 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1364 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1380 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1388 "chapel.ypp"
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[(1) - (2)].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[(2) - (2)].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[(2) - (2)].pexpr);
    }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1429 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1431 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7499 "bison-chapel.cpp"
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



