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
#define YYLAST   11027

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  425
/* YYNRULES -- Number of states.  */
#define YYNSTATES  812

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
     228,   230,   232,   234,   236,   238,   241,   247,   251,   258,
     265,   270,   276,   282,   286,   290,   297,   303,   310,   316,
     323,   327,   332,   339,   347,   354,   362,   367,   373,   378,
     383,   387,   394,   400,   406,   407,   410,   414,   417,   424,
     431,   440,   442,   444,   446,   447,   450,   451,   454,   458,
     464,   466,   469,   473,   475,   479,   480,   481,   490,   491,
     493,   496,   499,   500,   501,   511,   515,   519,   525,   531,
     533,   536,   538,   540,   542,   544,   546,   548,   550,   552,
     554,   556,   558,   560,   562,   564,   566,   568,   570,   572,
     574,   576,   578,   580,   582,   584,   586,   588,   590,   592,
     594,   596,   598,   600,   602,   604,   606,   608,   609,   613,
     617,   618,   620,   624,   629,   634,   641,   648,   649,   651,
     653,   655,   657,   660,   663,   665,   667,   669,   670,   672,
     674,   676,   678,   680,   681,   683,   685,   687,   689,   691,
     693,   695,   697,   700,   702,   703,   705,   708,   711,   712,
     715,   719,   724,   729,   732,   737,   738,   741,   744,   749,
     754,   759,   765,   770,   771,   773,   775,   777,   781,   785,
     790,   796,   798,   800,   804,   806,   809,   813,   814,   817,
     820,   821,   826,   827,   830,   833,   835,   840,   845,   852,
     854,   855,   857,   859,   863,   868,   872,   877,   884,   885,
     888,   891,   894,   897,   900,   903,   905,   907,   911,   915,
     917,   919,   921,   925,   929,   930,   932,   934,   938,   942,
     946,   950,   952,   954,   956,   958,   960,   962,   964,   966,
     969,   974,   979,   984,   990,   993,   996,  1003,  1010,  1015,
    1025,  1035,  1043,  1050,  1057,  1062,  1072,  1082,  1090,  1095,
    1102,  1109,  1119,  1129,  1136,  1138,  1140,  1142,  1144,  1146,
    1148,  1150,  1152,  1156,  1157,  1159,  1164,  1166,  1170,  1173,
    1177,  1179,  1183,  1186,  1191,  1193,  1195,  1197,  1199,  1201,
    1203,  1205,  1207,  1212,  1216,  1220,  1223,  1226,  1228,  1230,
    1232,  1234,  1236,  1238,  1240,  1245,  1250,  1255,  1259,  1263,
    1267,  1271,  1276,  1280,  1282,  1284,  1286,  1288,  1292,  1296,
    1300,  1304,  1310,  1314,  1318,  1322,  1326,  1330,  1334,  1338,
    1342,  1346,  1350,  1354,  1358,  1362,  1366,  1370,  1374,  1378,
    1382,  1386,  1390,  1394,  1398,  1402,  1405,  1408,  1411,  1414,
    1417,  1420,  1424,  1428,  1432,  1436,  1440,  1444,  1448,  1452,
    1454,  1456,  1458,  1460,  1462,  1464
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     134,     0,    -1,   135,    -1,    -1,   135,   136,    -1,   138,
      -1,   137,   138,    -1,    50,     7,    -1,   137,    50,     7,
      -1,   139,    -1,   141,    -1,   143,    -1,   150,    -1,   144,
      -1,   151,    -1,   154,    -1,   152,    -1,   155,    -1,   158,
      -1,   149,    -1,   217,   118,    -1,    10,   138,    -1,    11,
     218,   138,    -1,    12,   145,   118,    -1,    15,   218,   141,
      -1,    19,   145,   118,    -1,    20,   225,   118,    -1,    36,
     146,   138,    -1,    39,   138,    -1,    45,   225,   148,    -1,
      61,   225,   148,    -1,    61,   148,    -1,    65,   138,    -1,
      76,   225,   118,    -1,     1,    -1,   140,    41,   146,   124,
     125,    -1,   140,    41,   146,   124,   142,   125,    -1,    -1,
      54,    -1,    52,    -1,   124,   125,    -1,   124,   142,   125,
      -1,   136,    -1,   142,   136,    -1,    70,   206,   118,    -1,
     226,    80,   225,   118,    -1,   226,    91,   225,   118,    -1,
     226,    88,   225,   118,    -1,   226,    90,   225,   118,    -1,
     226,    84,   225,   118,    -1,   226,    89,   225,   118,    -1,
     226,    85,   225,   118,    -1,   226,    81,   225,   118,    -1,
     226,    82,   225,   118,    -1,   226,    83,   225,   118,    -1,
     226,    93,   225,   118,    -1,   226,    92,   225,   118,    -1,
     226,   122,   225,   118,    -1,   226,    86,   225,   118,    -1,
     226,    87,   225,   118,    -1,    -1,   146,    -1,     3,    -1,
     145,    -1,     7,    -1,    22,   138,    -1,   141,    -1,    58,
     118,    -1,    58,   225,   118,    -1,   118,    -1,   170,    -1,
     159,    -1,   163,    -1,   190,    -1,   193,    -1,    27,     8,
      -1,    22,   138,    74,   225,   118,    -1,    74,   225,   148,
      -1,    16,   225,    31,   225,   218,   148,    -1,    16,   225,
      31,   153,   218,   148,    -1,    16,   225,   218,   148,    -1,
      28,   225,    31,   225,   148,    -1,    28,   225,    31,   153,
     148,    -1,    28,   225,   148,    -1,    28,   153,   148,    -1,
      28,    48,   146,    31,   225,   148,    -1,    29,   225,    31,
     225,   148,    -1,    29,   225,    31,   225,   219,   148,    -1,
      29,   225,    31,   153,   148,    -1,    29,   225,    31,   153,
     219,   148,    -1,    29,   225,   148,    -1,    29,   225,   219,
     148,    -1,   128,   206,    31,   225,   129,   138,    -1,   128,
     206,    31,   225,   219,   129,   138,    -1,   128,   206,    31,
     153,   129,   138,    -1,   128,   206,    31,   153,   219,   129,
     138,    -1,   128,   206,   129,   138,    -1,   128,   206,   219,
     129,   138,    -1,    77,   126,   206,   127,    -1,    30,   225,
      66,   138,    -1,    30,   225,   141,    -1,    30,   225,    66,
     138,    24,   138,    -1,    30,   225,   141,    24,   138,    -1,
      60,   225,   124,   156,   125,    -1,    -1,   156,   157,    -1,
      72,   206,   148,    -1,    46,   138,    -1,    67,    60,   206,
     124,   156,   125,    -1,   160,   146,   161,   124,   162,   125,
      -1,    27,   147,   160,   146,   161,   124,   162,   125,    -1,
      14,    -1,    55,    -1,    69,    -1,    -1,    98,   206,    -1,
      -1,   162,   150,    -1,   162,   137,   150,    -1,    25,   146,
     124,   164,   125,    -1,   165,    -1,   164,    99,    -1,   164,
      99,   165,    -1,   146,    -1,   146,    80,   225,    -1,    -1,
      -1,    37,   167,   177,   168,   183,   201,   189,   185,    -1,
      -1,    33,    -1,    26,   147,    -1,    27,   147,    -1,    -1,
      -1,   169,   182,   171,   173,   172,   183,   201,   189,   184,
      -1,   181,   174,   176,    -1,   181,   175,   176,    -1,   181,
     146,   101,   174,   176,    -1,   181,   146,   101,   175,   176,
      -1,   146,    -1,    95,   146,    -1,    94,    -1,    96,    -1,
      97,    -1,    95,    -1,   104,    -1,   114,    -1,   110,    -1,
     107,    -1,   109,    -1,   106,    -1,   116,    -1,   111,    -1,
     121,    -1,   100,    -1,   119,    -1,   120,    -1,   112,    -1,
     105,    -1,   113,    -1,    13,    -1,   108,    -1,     9,    -1,
     122,    -1,   123,    -1,    80,    -1,    91,    -1,    88,    -1,
      90,    -1,    84,    -1,    89,    -1,    85,    -1,    81,    -1,
      82,    -1,    83,    -1,    93,    -1,    92,    -1,    -1,   126,
     178,   127,    -1,   126,   178,   127,    -1,    -1,   179,    -1,
     178,    99,   179,    -1,   180,   146,   205,   199,    -1,   180,
     146,   205,   188,    -1,   180,   126,   198,   127,   205,   199,
      -1,   180,   126,   198,   127,   205,   188,    -1,    -1,    31,
      -1,    34,    -1,    47,    -1,    18,    -1,    18,    31,    -1,
      18,    57,    -1,    48,    -1,    57,    -1,    67,    -1,    -1,
      48,    -1,    57,    -1,    67,    -1,    53,    -1,    35,    -1,
      -1,    18,    -1,    57,    -1,    48,    -1,    67,    -1,   118,
      -1,   185,    -1,   141,    -1,   149,    -1,   117,   146,    -1,
     117,    -1,    -1,   186,    -1,   103,   225,    -1,   103,   187,
      -1,    -1,    73,   225,    -1,    67,   191,   118,    -1,    17,
      67,   191,   118,    -1,    27,    67,   191,   118,    -1,   146,
     192,    -1,   146,   192,    99,   191,    -1,    -1,    80,   213,
      -1,    80,   202,    -1,   194,    48,   195,   118,    -1,   194,
      18,   195,   118,    -1,   194,    57,   195,   118,    -1,   194,
      18,    57,   195,   118,    -1,   194,    71,   195,   118,    -1,
      -1,    17,    -1,    27,    -1,   196,    -1,   195,    99,   196,
      -1,   146,   201,   199,    -1,   146,   200,    78,   225,    -1,
     126,   198,   127,   201,   199,    -1,    68,    -1,   146,    -1,
     126,   198,   127,    -1,   197,    -1,   197,    99,    -1,   197,
      99,   198,    -1,    -1,    80,    44,    -1,    80,   225,    -1,
      -1,    98,   128,   206,   129,    -1,    -1,    98,   213,    -1,
      98,   202,    -1,     1,    -1,   128,   206,   129,   213,    -1,
     128,   206,   129,   202,    -1,   128,   206,    31,   225,   129,
     213,    -1,     1,    -1,    -1,   213,    -1,   186,    -1,   128,
     129,   203,    -1,   128,   206,   129,   203,    -1,   128,   129,
     204,    -1,   128,   206,   129,   204,    -1,   128,   206,    31,
     225,   129,   203,    -1,    -1,    98,   213,    -1,    98,   186,
      -1,    98,    23,    -1,    98,    62,    -1,    98,    65,    -1,
      98,   204,    -1,   225,    -1,   186,    -1,   206,    99,   225,
      -1,   206,    99,   186,    -1,    68,    -1,   225,    -1,   186,
      -1,   207,    99,   207,    -1,   208,    99,   207,    -1,    -1,
     210,    -1,   211,    -1,   210,    99,   211,    -1,   146,    80,
     186,    -1,   146,    80,   225,    -1,   146,    78,   225,    -1,
     186,    -1,   225,    -1,   146,    -1,   216,    -1,   226,    -1,
     215,    -1,   234,    -1,   233,    -1,    62,   225,    -1,    32,
     126,   209,   127,    -1,    23,   126,   209,   127,    -1,    64,
     126,   209,   127,    -1,    63,    64,   126,   209,   127,    -1,
      10,   225,    -1,    65,   225,    -1,    28,   225,    31,   225,
      22,   225,    -1,    28,   225,    31,   153,    22,   225,    -1,
      28,   225,    22,   225,    -1,    28,   225,    31,   225,    22,
      30,   225,    66,   225,    -1,    28,   225,    31,   153,    22,
      30,   225,    66,   225,    -1,    28,   225,    22,    30,   225,
      66,   225,    -1,    29,   225,    31,   225,    22,   225,    -1,
      29,   225,    31,   153,    22,   225,    -1,    29,   225,    22,
     225,    -1,    29,   225,    31,   225,    22,    30,   225,    66,
     225,    -1,    29,   225,    31,   153,    22,    30,   225,    66,
     225,    -1,    29,   225,    22,    30,   225,    66,   225,    -1,
     128,   206,   129,   225,    -1,   128,   206,    31,   225,   129,
     225,    -1,   128,   206,    31,   153,   129,   225,    -1,   128,
     206,    31,   225,   129,    30,   225,    66,   225,    -1,   128,
     206,    31,   153,   129,    30,   225,    66,   225,    -1,    30,
     225,    66,   225,    24,   225,    -1,    43,    -1,   216,    -1,
     212,    -1,   229,    -1,   228,    -1,   166,    -1,   223,    -1,
     224,    -1,   222,   123,   225,    -1,    -1,   219,    -1,    75,
     126,   220,   127,    -1,   221,    -1,   220,    99,   221,    -1,
     180,   212,    -1,   237,    56,   212,    -1,   226,    -1,   222,
     123,   225,    -1,    42,   225,    -1,    38,   195,    31,   225,
      -1,   231,    -1,   213,    -1,   214,    -1,   235,    -1,   236,
      -1,   166,    -1,   223,    -1,   224,    -1,   126,   103,   225,
     127,    -1,   225,    98,   225,    -1,   225,   102,   225,    -1,
     225,   102,    -1,   102,   225,    -1,   102,    -1,   212,    -1,
     228,    -1,   229,    -1,   230,    -1,   226,    -1,   166,    -1,
     227,   126,   209,   127,    -1,   227,   128,   209,   129,    -1,
      51,   126,   209,   127,    -1,   225,   101,   146,    -1,   225,
     101,    67,    -1,   225,   101,    23,    -1,   126,   207,   127,
      -1,   126,   207,    99,   127,    -1,   126,   208,   127,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,   124,   206,
     125,    -1,   128,   206,   129,    -1,   128,   232,   129,    -1,
     225,    78,   225,    -1,   232,    99,   225,    78,   225,    -1,
     225,   116,   225,    -1,   225,   111,   225,    -1,   225,   121,
     225,    -1,   225,   100,   225,    -1,   225,   119,   225,    -1,
     225,   120,   225,    -1,   225,   112,   225,    -1,   225,   104,
     225,    -1,   225,   114,   225,    -1,   225,   110,   225,    -1,
     225,   107,   225,    -1,   225,   109,   225,    -1,   225,   106,
     225,    -1,   225,    94,   225,    -1,   225,    96,   225,    -1,
     225,    97,   225,    -1,   225,    79,   225,    -1,   225,   115,
     225,    -1,   225,   105,   225,    -1,   225,    13,   225,    -1,
     225,     9,   225,    -1,   225,   108,   225,    -1,   225,    21,
     225,    -1,   116,   225,    -1,   111,   225,    -1,    40,   225,
      -1,    49,   225,    -1,   113,   225,    -1,    95,   225,    -1,
     225,    56,   225,    -1,   225,    56,   153,    -1,   237,    56,
     225,    -1,   237,    56,   153,    -1,   225,    59,   225,    -1,
     225,    59,   153,    -1,   237,    59,   225,    -1,   237,    59,
     153,    -1,   116,    -1,   121,    -1,    79,    -1,   115,    -1,
      94,    -1,    96,    -1,    97,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   401,   401,   406,   407,   413,   414,   419,   420,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   454,   456,   461,   462,   463,
     478,   479,   484,   485,   490,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     512,   513,   517,   521,   522,   526,   527,   531,   532,   536,
     537,   538,   539,   540,   541,   545,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   582,   588,   594,   600,   607,   617,   621,
     622,   623,   624,   628,   633,   634,   638,   640,   645,   652,
     657,   665,   670,   675,   683,   684,   689,   690,   692,   697,
     707,   716,   720,   728,   729,   734,   739,   733,   764,   770,
     777,   785,   796,   802,   795,   830,   834,   839,   843,   851,
     852,   853,   854,   855,   856,   857,   858,   859,   860,   861,
     862,   863,   864,   865,   866,   867,   868,   869,   870,   871,
     872,   873,   874,   875,   876,   880,   881,   882,   883,   884,
     885,   886,   887,   888,   889,   890,   891,   895,   896,   900,
     904,   905,   906,   910,   912,   914,   916,   921,   922,   923,
     924,   925,   926,   927,   928,   929,   930,   934,   935,   936,
     937,   941,   942,   946,   947,   948,   949,   950,   954,   955,
     959,   960,   964,   966,   971,   972,   976,   977,   981,   982,
     986,   988,   990,   995,  1008,  1025,  1026,  1028,  1033,  1040,
    1047,  1054,  1062,  1071,  1072,  1073,  1077,  1078,  1086,  1088,
    1094,  1099,  1101,  1103,  1108,  1110,  1112,  1119,  1120,  1121,
    1125,  1126,  1131,  1132,  1133,  1134,  1154,  1158,  1162,  1170,
    1174,  1175,  1176,  1180,  1182,  1188,  1190,  1192,  1197,  1198,
    1199,  1200,  1201,  1202,  1203,  1209,  1210,  1211,  1212,  1216,
    1217,  1218,  1222,  1223,  1227,  1228,  1232,  1233,  1237,  1238,
    1239,  1240,  1241,  1245,  1256,  1257,  1258,  1259,  1260,  1261,
    1263,  1265,  1267,  1269,  1271,  1273,  1278,  1280,  1282,  1284,
    1286,  1288,  1290,  1292,  1294,  1296,  1298,  1300,  1302,  1309,
    1315,  1321,  1327,  1336,  1341,  1349,  1350,  1351,  1352,  1353,
    1354,  1355,  1356,  1361,  1362,  1366,  1370,  1372,  1380,  1390,
    1394,  1395,  1400,  1405,  1413,  1414,  1415,  1416,  1417,  1418,
    1419,  1420,  1421,  1423,  1425,  1427,  1429,  1431,  1436,  1437,
    1438,  1439,  1450,  1451,  1455,  1456,  1457,  1461,  1462,  1463,
    1471,  1472,  1473,  1477,  1478,  1479,  1480,  1481,  1482,  1483,
    1490,  1491,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,
    1513,  1514,  1515,  1516,  1517,  1521,  1522,  1523,  1524,  1525,
    1526,  1530,  1531,  1532,  1533,  1537,  1538,  1539,  1540,  1545,
    1546,  1547,  1548,  1549,  1550,  1551
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
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "select_stmt", "when_stmt_ls", "when_stmt", "type_select_stmt",
  "class_decl_stmt", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_ls", "enum_item", "lambda_decl_expr", "$@1",
  "$@2", "linkage_spec", "fn_decl_stmt", "$@3", "$@4",
  "fn_decl_stmt_inner", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls", "formal", "opt_intent_tag",
  "opt_this_intent_tag", "proc_or_iter", "opt_ret_tag",
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
     150,   150,   150,   150,   150,   151,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   153,   154,
     154,   154,   154,   155,   156,   156,   157,   157,   158,   159,
     159,   160,   160,   160,   161,   161,   162,   162,   162,   163,
     164,   164,   164,   165,   165,   167,   168,   166,   169,   169,
     169,   169,   171,   172,   170,   173,   173,   173,   173,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   176,   176,   177,
     178,   178,   178,   179,   179,   179,   179,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   181,   181,   181,
     181,   182,   182,   183,   183,   183,   183,   183,   184,   184,
     185,   185,   186,   186,   187,   187,   188,   188,   189,   189,
     190,   190,   190,   191,   191,   192,   192,   192,   193,   193,
     193,   193,   193,   194,   194,   194,   195,   195,   196,   196,
     196,   197,   197,   197,   198,   198,   198,   199,   199,   199,
     200,   200,   201,   201,   201,   201,   202,   202,   202,   202,
     203,   203,   203,   204,   204,   204,   204,   204,   205,   205,
     205,   205,   205,   205,   205,   206,   206,   206,   206,   207,
     207,   207,   208,   208,   209,   209,   210,   210,   211,   211,
     211,   211,   211,   212,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   216,   217,   217,   217,   217,   217,
     217,   217,   217,   218,   218,   219,   220,   220,   221,   221,
     222,   222,   223,   224,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   226,   226,
     226,   226,   227,   227,   228,   228,   228,   229,   229,   229,
     230,   230,   230,   231,   231,   231,   231,   231,   231,   231,
     232,   232,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   234,   234,   234,
     234,   235,   235,   235,   235,   236,   236,   236,   236,   237,
     237,   237,   237,   237,   237,   237
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
       1,     1,     1,     1,     1,     2,     5,     3,     6,     6,
       4,     5,     5,     3,     3,     6,     5,     6,     5,     6,
       3,     4,     6,     7,     6,     7,     4,     5,     4,     4,
       3,     6,     5,     5,     0,     2,     3,     2,     6,     6,
       8,     1,     1,     1,     0,     2,     0,     2,     3,     5,
       1,     2,     3,     1,     3,     0,     0,     8,     0,     1,
       2,     2,     0,     0,     9,     3,     3,     5,     5,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     3,
       0,     1,     3,     4,     4,     6,     6,     0,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     0,     1,     2,     2,     0,     2,
       3,     4,     4,     2,     4,     0,     2,     2,     4,     4,
       4,     5,     4,     0,     1,     1,     1,     3,     3,     4,
       5,     1,     1,     3,     1,     2,     3,     0,     2,     2,
       0,     4,     0,     2,     2,     1,     4,     4,     6,     1,
       0,     1,     1,     3,     4,     3,     4,     6,     0,     2,
       2,     2,     2,     2,     2,     1,     1,     3,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     4,     1,     3,     2,     3,
       1,     3,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     4,     3,     1,     1,     1,     1,     3,     3,     3,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    62,   373,   374,   375,   376,
       0,   333,    60,   111,   333,     0,   234,    60,     0,     0,
       0,     0,    60,    60,     0,     0,     0,     0,   129,     0,
     125,     0,     0,     0,     0,   324,     0,     0,     0,     0,
      39,    38,   112,     0,     0,     0,     0,     0,     0,     0,
       0,   113,     0,     0,     0,   421,   423,     0,   424,   425,
     357,     0,     0,   422,   419,    69,   420,     0,     0,     0,
       4,     0,     5,     9,     0,    10,    11,    13,   293,    19,
      12,    14,    16,    15,    17,    18,    71,     0,    72,   349,
       0,    70,    73,    74,     0,   358,   345,   346,   296,   294,
       0,     0,   350,   351,     0,   295,     0,   359,   360,   361,
     344,   298,   297,   347,   348,     0,    21,   304,     0,     0,
     334,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,   349,   358,   294,   350,   351,   333,   295,   359,   360,
       0,     0,     0,     0,   284,     0,    64,    63,   130,    75,
       0,   131,     0,     0,     0,     0,     0,     0,   284,     0,
       0,     0,     0,     0,   236,    28,   407,   342,     0,   408,
       7,   284,    67,     0,     0,     0,    66,    31,     0,   299,
       0,   284,    32,   305,     0,   225,     0,   213,   276,     0,
     275,     0,     0,   410,   356,   406,   409,   405,    40,    42,
       0,     0,   279,     0,   281,     0,     0,   280,     0,   275,
       0,     0,     6,     0,   114,   202,   201,   132,     0,     0,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   284,   284,     0,
       0,   187,    22,    23,     0,    24,     0,     0,     0,     0,
       0,     0,     0,    25,    26,     0,   293,   291,     0,   285,
     286,   292,     0,     0,     0,     0,     0,    84,     0,     0,
      83,     0,     0,    90,     0,     0,   100,     0,    27,   180,
     126,   241,     0,   242,   244,     0,   255,     0,     0,   247,
       0,     0,    29,     0,    68,   104,    65,    30,   284,     0,
       0,     0,   223,   220,   212,     0,    44,    77,    33,    41,
      43,   377,     0,     0,   370,     0,   372,     0,     0,     0,
       0,     0,   379,     8,     0,     0,     0,   197,     0,     0,
       0,     0,     0,   332,   402,   401,   404,   412,   411,   416,
     415,   398,   395,   396,   397,   353,   385,   369,   368,   367,
     354,   389,   400,   394,   392,   403,   393,   391,   383,   388,
     390,   399,   382,   386,   387,   384,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   413,   418,   417,   191,   188,   189,
     190,   194,   195,   196,   419,     0,     0,   336,     0,     0,
       0,     0,     0,     0,     0,   378,   333,   333,    80,   221,
       0,     0,     0,   301,     0,   123,     0,   120,   222,   114,
       0,     0,     0,   308,     0,     0,     0,   314,     0,     0,
      91,    99,     0,     0,   300,     0,   181,     0,   203,     0,
     245,     0,   259,     0,   254,   345,     0,     0,   238,   343,
     237,   366,     0,     0,   302,   104,     0,   227,   345,     0,
     278,   277,   352,   371,   282,   283,     0,     0,    96,   318,
       0,   380,     0,     0,   115,   116,   198,   199,   200,   133,
       0,     0,   229,   228,   230,   232,    45,    52,    53,    54,
      49,    51,    58,    59,    47,    50,    48,    46,    56,    55,
      57,   364,   365,   192,   193,   338,   187,   335,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
     290,   288,   289,   287,     0,   121,   119,     0,     0,    98,
       0,     0,    82,     0,    81,     0,     0,    88,     0,     0,
      86,     0,     0,     0,   102,   187,   179,     0,   268,   204,
     206,   205,   207,     0,   243,   246,     0,   247,     0,   239,
     248,   249,     0,     0,   103,   105,   303,     0,     0,   224,
       0,     0,     0,     0,    97,     0,    35,     0,   233,   203,
     162,   160,   165,   172,   173,   174,   169,   171,   167,   170,
     168,   166,   176,   175,   141,   144,   142,   143,   154,   145,
     158,   150,   148,   161,   149,   147,   152,   157,   159,   146,
     151,   155,   156,   153,   163,   164,   139,   177,   177,   231,
     337,   339,     0,     0,     0,     0,     0,     0,     0,     0,
      79,    78,   124,   122,   116,    85,     0,     0,   307,     0,
     306,     0,     0,   313,    89,     0,   312,    87,   101,   323,
     182,     0,     0,   247,   218,   240,     0,     0,   107,     0,
     108,     0,     0,    94,   320,     0,     0,    92,   319,     0,
     381,    36,    60,     0,   109,   233,   117,     0,   140,     0,
     180,   135,   136,     0,     0,     0,     0,     0,     0,     0,
       0,   233,   311,     0,     0,   317,     0,     0,   268,   271,
     272,   273,     0,   270,   274,   345,   214,   184,   183,     0,
       0,     0,   257,   345,   106,     0,    95,     0,    93,   118,
     218,   139,   177,   177,     0,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,     0,   247,   260,     0,   215,
     217,   216,   219,   210,   211,   127,     0,     0,     0,     0,
     137,   138,   178,     0,     0,     0,     0,     0,     0,   310,
     309,   316,   315,   186,   185,   262,   263,   265,   345,     0,
     378,   345,   322,   321,   208,   134,   209,     0,   264,   266,
     260,   267
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   199,    71,   461,    73,    74,    75,   200,
      76,    77,   147,    78,   151,   177,    79,    80,    81,    82,
     545,    83,    84,   482,   595,    85,    86,    87,   356,   608,
      88,   446,   447,   131,   160,   468,    90,    91,   357,   609,
     509,   647,   648,   711,   310,   465,   466,   467,   510,   217,
     583,   805,   775,   188,   770,   737,   740,    92,   186,   332,
      93,    94,   163,   164,   314,   315,   478,   318,   319,   474,
     796,   734,   683,   201,   205,   206,   288,   289,   290,   132,
      96,    97,    98,   133,   100,   119,   120,   426,   427,   101,
     134,   135,   104,   137,   106,   138,   139,   109,   110,   210,
     111,   112,   113,   114,   115
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -694
static const yytype_int16 yypact[] =
{
    -694,    70,  2279,  -694,  -694,  -694,  -694,  -694,  -694,  -694,
    3287,    16,    89,  -694,    16,  6037,    65,    89,  6037,  3287,
       7,    89,   359,   693,  5059,  6037,  6037,    13,  -694,    89,
    -694,    28,  3287,  6037,  6037,  -694,  6037,  6037,   194,    78,
    -694,  -694,  -694,  5241,  6037,  5367,  6037,   161,   141,  3287,
      43,  -694,  5447,  6037,  6037,  -694,  -694,  6037,  -694,  -694,
    6757,  6037,  6037,  -694,  6037,  -694,  -694,  2531,  4855,  5447,
    -694,  3161,  -694,  -694,   232,  -694,  -694,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,  -694,  -694,    89,  -694,   266,
     252,  -694,  -694,  -694,   246,   193,  -694,  -694,  -694,   196,
     208,   211,   213,   226, 10840,  1619,   -50,   249,   253,  -694,
    -694,  -694,  -694,  -694,  -694,    97,  -694, 10840,   264,  3287,
    -694,   260,  -694,   259,  6037,  6037,  6037,  6037,  6037,  5447,
    5447,   251,  -694,  -694,  -694,  -694,  8359,   270,  -694,  -694,
      89,   281,  8562,   321,  5447,   282,  -694,  -694,  -694,  -694,
      89,   123,    89,   284,    45,  7353,  7249,  7487,  5447,  3287,
     286,    19,    92,    32,  -694,  -694,   988,   310,  7554,   988,
    -694,  5447,  -694,  8629,  8224,  3287,  -694,  -694,  7554, 10840,
     287,  5447,  -694, 10840,  5447,   335,   312,    89,  -694,    85,
   10840,  7554,  8696,   349,  1052,   988,   349,   988,  -694,  -694,
    2657,   -28,  -694,  6037,  -694,   -15,    -5, 10840,    82,  8763,
     -33,   426,  -694,    89,   338,  -694,  -694,  -694,    25,    28,
      28,    28,  -694,  6037,  6037,  6037,  6037,  5549,  5549,  6037,
    6037,  6037,  6037,  6037,  6037,    57,  6757,  6037,  6037,  6037,
    6037,  6037,  6037,  6037,  6037,  6037,  6037,  6037,  6037,  6037,
    6037,  6037,  6037,  6037,  6037,  6037,  6037,  6037,  6037,  6037,
    6037,  6037,  6037,  6037,  6037,  6037,  6037,  5447,  5447,  5549,
    5549,   820,  -694,  -694,  2783,  -694,  8427,  8495,  8830,    68,
    5549,    45,   325,  -694,  -694,  6037,   277,  -694,   311,   345,
    -694, 10840,    89,   327,    89,   417,  5447,  -694,  3413,  5549,
    -694,  3539,  5549,  -694,    45,  3287,   427,   326,  -694,    38,
    -694,  -694,    19,  -694,   353,   328,  -694,  2099,   378,   380,
    6037,    28,  -694,   337,  -694,  -694,  -694,  -694,  5447,   341,
      81,  4673,   374,  -694,  -694,  5447,  -694,  -694,  -694,  -694,
    -694,  -694,  2147,  4957,  -694,  5139,  -694,  5549,  2405,   340,
    6037,  6037,  -694,  -694,   350,  5447,   351,    33,    28,   163,
     203,   224,   229,  8291,  7077,  7077,   288,  -694,   288,  -694,
     288,  7285,  1326,  1165,  1197,   310,   349,  -694,  -694,  -694,
    1052, 10906,   288,   852,   852,  7077,   852,   852,   515,   349,
   10906, 10874,   515,   988,   988,   349,  8897,  8964,  9031,  9098,
    9165,  9232,  9299,  9366,  9433,  9500,  9567,  9634,  9701,  9768,
    9835,   355,   356,  -694,   288,  -694,   288,    44,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,    89,     9,  -694,   424,  6117,
    5549,  6197,  5549,  6037,  5549,  6939,    16,  9902,  -694,  -694,
    9969,  6037,  5447,  -694,  5447,   406,    84,  -694,  -694,   338,
    6037,   172,  6037, 10840,    51,  7621,  6037, 10840,    55,  7420,
    -694,   463, 10036,  3287,  -694,   180,  -694,    34,   279,   365,
      19,    67,  -694,  5447,  -694,   404,  6037,  5651,  -694, 10840,
    -694,  -694,    63,   366,  -694,  -694,  5447,  -694,   250,    89,
    -694, 10840,  -694,  -694,  -694,  -694,   -27,  1937,  -694, 10840,
    3287, 10840, 10103,  2909,   399,  -694,  -694,  -694,  -694,  -694,
    6996,   257,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,  -694,   820,  -694,    89,  6037,
     478, 10170,  6037,   479, 10237,   373,  6916,    45,    45,  -694,
   10840,  -694, 10840,  -694,  6037,    89,  -694,   381,  7554,  -694,
    7688,  3665,  -694,  3791,  -694,  7755,  3917,  -694,    45,  4043,
    -694,    45,  3287,  6037,  -694,   370,  -694,    19,   409,  -694,
    -694,  -694,  -694,    52,  -694,  -694,  4673,   380,   164, 10840,
    -694, 10840,  3287,  5447,  -694,  -694,  -694,   136,   167,  -694,
    4169,   379,  4295,   382,  -694,  6037,  -694,  3035,   347,   279,
    -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,    89,  -694,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -694,  -694,   408,   389,   389,  -694,
    -694,  -694, 10304,  6277,  6357, 10371,  6437,  6517,  6597,  6677,
    -694,  -694, 10840,  -694,  -694,  -694,  3287,  6037, 10840,  6037,
   10840,  3287,  6037, 10840,  -694,  6037, 10840,  -694,  -694, 10840,
    -694,   391,  5731,    41,   443,  -694,  5549,  4421,  -694,    39,
    -694,  4547,  6037,  -694, 10840,  3287,  6037,  -694, 10840,  3287,
   10840,  -694,   823,    89,  -694,   779,  -694,    31,  -694,  6996,
      38,  -694,  -694,  6037,  6037,  6037,  6037,  6037,  6037,  6037,
    6037,   464, 10036,  7822,  7889, 10036,  7956,  8023,   409,     7,
    6037,  6037,  4753,  -694,  -694,   205,  5447,  -694,  -694,  6037,
     -13,  7111,  -694,   565,  -694,  8090,  -694,  8157,  -694,  -694,
     443,  -694,   389,   389,   185, 10438, 10505, 10572, 10639, 10706,
   10773,  -694,  3287,  3287,  3287,  3287,    41,  5833,   169,  -694,
    -694, 10840, 10840,  -694,  -694,  -694,  6677,  3287,  3287,    88,
    -694,  -694,  -694,  6037,  6037,  6037,  6037,  6037,  6037, 10036,
   10036, 10036, 10036,  -694,  -694,  -694,  -694,  -694,   221,  5549,
    6837,   685, 10036, 10036,  -694,  -694,  -694,  7145,  -694,  -694,
    5935,  -694
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -694,  -694,  -694,    -1,  -544,  1577,  -694,  -694,  1347,    18,
    -694,  -694,   190,  1663,   501,   122,  -691,  -553,  -694,  -694,
     285,  -694,  -694,    40,  -694,  -694,  -694,   375,    75,  -137,
    -694,  -694,   -25,   439,  -694,  -694,  -694,  -694,  -694,  -694,
    -694,  -175,  -174,  -610,  -694,  -173,   -37,  -264,  -694,  -694,
     -70,  -694,  -239,    15,  -694,  -225,  -208,  -694,  -138,  -694,
    -694,  -694,  -200,   222,  -694,  -308,  -577,  -694,  -442,  -328,
    -481,  -693,  -183,    10,    64,  -694,  -141,  -694,   102,   320,
    -306,  -694,  -694,   559,  -694,    -8,  -143,  -694,    11,  -694,
     769,   842,   -10,   891,  -694,  1174,  1225,  -694,  -694,  -694,
    -694,  -694,  -694,  -694,  -262
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -379
static const yytype_int16 yytable[] =
{
     117,    70,   282,   487,   469,   136,   123,   425,   142,   428,
     685,   475,   293,   304,   155,   156,   157,   307,   359,   360,
     361,   362,     5,   166,   167,   488,   168,   169,     5,   587,
     323,     5,   316,   173,   174,   178,   179,     5,   712,   183,
     329,  -187,   190,   191,   192,    43,     5,   193,   118,   774,
     194,   195,   196,   316,   197,   706,   417,   190,   207,   209,
       5,   175,   189,   320,   705,   349,   351,   175,   316,   418,
       3,   335,   419,   561,   797,   533,   267,   566,   268,   208,
     377,   506,   358,   204,   343,   420,   421,   311,   774,  -252,
     507,   118,     5,   316,   345,   422,   352,   341,  -252,   434,
     508,   534,   600,   184,  -252,   423,   738,   809,   536,   592,
    -252,   274,   344,   347,   117,   276,   277,   278,   183,   190,
     209,   477,   346,  -252,   378,  -252,   411,   412,   281,   586,
     118,   321,   140,   144,   291,   593,   537,    13,   335,   158,
     279,   684,   780,   781,   736,   312,    43,  -252,   291,  -252,
     586,   161,   749,   269,   161,  -252,   270,   118,   511,   287,
     577,   291,   585,   274,  -187,   586,  -252,   335,   706,   274,
    -250,   291,  -252,   287,   190,   274,  -252,   705,    42,   274,
     335,   335,   592,   555,   335,  -252,   287,   483,   594,   794,
     317,  -252,    51,   342,   330,   686,   287,   435,   686,   340,
     799,   170,   121,   336,   171,   485,   804,   141,   593,   556,
    -252,   348,   274,   363,   364,   365,   366,   368,   370,   371,
     372,   373,   374,   375,   376,   180,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   291,   291,   414,
     416,   690,   321,   335,   218,   750,   335,   181,   335,   681,
     437,   335,   425,   213,   428,   440,   297,   300,   303,   575,
     475,   512,   287,   287,   575,  -269,   190,   215,   453,   455,
     322,   457,   459,   687,   219,   462,   691,   579,   800,   559,
     327,  -261,   321,   220,  -269,   216,   451,   576,  -269,   154,
     479,  -326,   782,   337,  -325,   568,   571,   221,   291,   808,
    -261,   513,    95,   321,  -261,   491,   222,   580,   321,   811,
      95,  -330,  -269,   207,   223,   207,   581,   497,   499,    95,
     501,   502,   514,   287,  -331,   190,   582,   515,  -261,  -226,
     490,   599,    95,   601,   603,   441,   321,   442,   204,   742,
     204,    13,     5,   742,    16,   504,   146,  -328,  -226,    95,
     226,  -327,    21,    22,   702,   649,   735,  -363,   273,  -363,
      28,   743,  -128,   274,  -329,   743,   233,    95,   417,   235,
     271,    95,  -363,   238,  -363,   285,  -362,    38,  -362,   283,
    -128,   418,    42,   438,   419,   227,   292,   494,   228,   495,
     296,   235,   309,   328,   703,   331,    51,   420,   421,   453,
     541,   457,   544,   462,   546,   499,   460,   422,   547,   548,
     333,   550,   552,   353,   291,  -253,   355,   423,   443,    95,
     558,    89,   560,   439,   444,   448,   565,   233,   450,    89,
     235,   463,   470,   464,   238,   471,   476,   551,    89,   287,
     477,   798,  -253,   209,   481,    65,   589,   591,   484,   500,
     801,    89,   704,   489,   503,   505,   209,  -253,    13,    95,
     538,    16,   531,   588,  -253,   532,   554,   572,    89,    21,
      22,   702,   584,   596,   798,    95,   598,    28,   335,  -128,
     653,   656,   658,  -253,   798,   664,    89,   682,   695,   709,
      89,   699,   367,   369,    38,   710,   739,  -128,   728,    42,
      95,   607,  -253,   148,   557,   597,   294,   721,  -253,   652,
     663,   703,   655,    51,   752,   753,   226,   754,   680,   707,
     806,   793,   779,   480,   662,   766,   553,   650,     0,     0,
       0,   668,     0,   670,   413,   415,   673,     0,    89,   676,
       0,    99,     0,   679,     0,   436,     0,     0,     0,    99,
       0,   227,     0,     0,   228,     0,   562,   564,    99,     0,
     567,   570,    65,   190,   454,     0,     0,   458,     0,   761,
     694,    99,   698,     0,    95,   700,  -256,     0,    89,     0,
       0,     0,     0,   689,     0,     0,   340,     0,    99,   230,
       0,   231,   232,   233,    89,   234,   235,     0,    95,     0,
     238,    95,     0,  -256,     0,    95,    99,   245,     0,     0,
      99,     0,   496,     0,   249,   250,   251,     0,  -256,    89,
       0,     0,     0,   668,   670,  -256,   673,   676,   694,   698,
       0,     0,     0,     0,     0,     0,   722,   723,     0,   724,
       0,   725,   726,     0,  -256,   727,     0,     0,    95,   660,
     661,     0,     0,     0,     0,     0,   741,   499,    99,     0,
     665,   499,   745,  -256,     0,     0,   747,     0,     0,  -256,
     674,     0,     0,   677,     0,     0,     5,   733,     0,     0,
     146,   149,     0,   722,   755,   756,   725,   757,   758,   759,
     760,  -235,     0,    89,     0,   540,  -258,   543,    99,     0,
     179,   183,   209,     0,     0,     0,   771,     0,     0,   772,
       0,     0,     0,     0,    99,     0,     0,    89,     0,     0,
      89,  -235,   768,  -258,    89,   535,     0,     0,     0,     0,
    -235,   769,   789,   790,   791,   792,     0,     0,  -258,    99,
     150,     0,     0,     0,  -235,  -258,   698,   802,   803,     0,
       0,   102,     0,   789,   790,   791,   792,   802,   803,   102,
       0,     0,   795,    95,  -258,     0,     0,    89,   102,   807,
     499,     0,     0,    13,     0,     0,    16,     0,     0,     0,
     698,   102,     0,  -258,    21,    22,   702,     0,     0,  -258,
       0,   744,    28,     0,  -128,   795,     0,     0,   102,     0,
      95,     0,     0,    95,     0,   795,     5,     0,     0,   211,
     146,     0,  -128,    99,    42,     0,   102,     0,   417,     0,
     102,  -235,     0,     0,   103,     0,   703,     0,    51,     0,
       0,   418,   103,     0,   419,     0,     0,    99,   651,     0,
      99,   103,     0,     0,    99,     0,     0,   420,   421,     0,
       0,  -235,     0,   226,   103,     0,     0,   422,     0,     0,
    -235,    95,     0,    95,     0,     0,    95,   423,   102,    95,
     150,   103,    95,   105,  -235,     0,     0,    65,     0,    55,
       0,   105,    89,     0,     0,     0,     0,    99,   227,   103,
     105,   228,    95,   103,    56,     0,    58,    59,     0,     0,
      95,     0,    95,   105,     0,     0,     0,    95,   102,     0,
       0,     0,     0,     0,     0,    63,   424,     0,     0,    89,
     105,    66,    89,     0,   102,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,     0,     0,   238,   105,     0,
       0,   103,   105,   244,   245,     0,     0,     0,   248,   102,
       0,   249,   250,   251,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
      89,   103,    89,     0,     0,    89,     0,     0,    89,   226,
     105,    89,     0,     0,     0,    95,     0,   103,     0,    95,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    89,
       0,    89,   103,   102,   227,     0,    89,   228,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,    99,     0,     0,     0,   105,   102,     0,     0,
     102,     0,     0,   226,   102,     0,     0,     0,     0,     0,
       0,     0,    95,    95,    95,    95,   233,     0,   234,   235,
       0,   105,     0,   238,     0,     0,     0,    95,    95,     0,
     245,     0,     0,     0,     0,    89,     0,     0,   227,   251,
      89,   228,     0,     0,     0,     0,   103,   102,     0,     0,
      99,     0,    99,     0,     0,    99,     0,     0,    99,     0,
       0,    99,     0,     0,    89,     0,     0,     0,    89,     0,
     103,     0,     0,   103,     0,     0,   230,   103,   231,   232,
     233,    99,   234,   235,     0,     0,     0,   238,     0,    99,
       0,    99,     0,   244,   245,   105,    99,     0,   248,     0,
       0,   249,   250,   251,     0,     0,   107,     0,     0,     0,
       0,     0,     0,     0,   107,     0,   226,     0,     0,   105,
     103,     0,   105,   107,     0,     0,   105,     0,     0,     0,
       0,    89,    89,    89,    89,     0,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    89,    89,   226,     0,
       0,   227,     0,   107,   228,    99,     0,   108,     0,     0,
      99,     0,   102,     0,     0,   108,     0,     0,     0,   105,
       0,   107,     0,     0,   108,   107,     0,     0,     0,     0,
       0,     0,     0,   227,    99,     0,   228,   108,    99,   230,
       0,     0,   232,   233,     0,   234,   235,     0,     0,   102,
     238,     0,   102,     0,   108,     0,     0,   245,     0,     0,
       0,     0,     0,     0,   249,   250,   251,     0,     0,     0,
       0,   230,   108,   107,     0,   233,   108,   234,   235,     0,
       0,     0,   238,     0,     0,   103,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,   249,   250,   251,     0,
       0,    99,    99,    99,    99,     0,     0,     0,     0,     0,
     102,     0,   102,   107,     0,   102,    99,    99,   102,     0,
       0,   102,   103,     0,   108,   103,     0,   226,     0,   107,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,   102,     0,     0,     0,     0,     0,     0,     0,   102,
       0,   102,     0,     0,   107,     0,   102,     0,     0,     0,
       0,     0,   227,     0,   108,   228,     0,     0,     0,     0,
       0,   105,   176,     0,   105,     0,     0,     0,     0,     0,
     108,     0,     0,   103,     0,   103,     0,     0,   103,     0,
       0,   103,     0,     0,   103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   108,   234,   235,     0,     0,
       0,   238,     0,     0,   103,   102,     0,     0,   245,     0,
     102,     0,   103,     0,   103,   249,   250,   251,   107,   103,
       0,     0,   105,     0,   105,     0,     0,   105,     0,     0,
     105,     0,     0,   105,   102,     0,     0,     0,   102,     0,
     275,     0,   107,     0,     0,   107,     0,     0,     0,   107,
       0,     0,     0,   105,     0,     0,     0,     0,     0,     0,
       0,   105,     0,   105,     0,     0,     0,     0,   105,   108,
       0,   176,   176,   176,   306,     0,     0,     0,   103,     0,
       0,     0,     0,   103,     0,   176,     0,     0,     0,     0,
       0,     0,   107,   108,     0,   176,   108,     0,     0,     0,
     108,   102,   102,   102,   102,     0,     0,   103,   176,     0,
       0,   103,     0,     0,     0,     0,   102,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,   105,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,     0,     0,     0,     0,     0,    72,
       0,     0,     0,     0,     0,     0,   105,   116,     0,     0,
     105,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,   103,   103,   103,   103,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
     103,     0,     0,     0,     0,     0,   182,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,    72,     0,     0,     0,   212,     0,
       0,   176,     0,   105,   105,   105,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,   105,
       0,     0,     0,     0,   107,   122,     0,   107,     0,     0,
     122,     0,     0,     0,   145,   122,   122,     0,   108,     0,
       0,     0,   159,     0,   162,     0,   272,     0,     0,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   185,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   108,     0,     0,   108,     0,
       0,     0,     0,     0,     0,   107,   308,   107,     0,     0,
     107,   266,  -340,   107,     0,  -362,   107,  -362,     0,     0,
     214,     0,   326,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
       0,     0,     0,     0,   107,     0,   107,    72,     0,     0,
       0,   107,     0,     0,     0,     0,   108,     0,   108,     0,
       0,   108,     0,     0,   108,     0,     0,   108,     0,     0,
       0,   176,   176,   185,     0,   176,   176,   286,     0,     0,
       0,     0,     0,   185,     0,   295,     0,   108,     0,     0,
       0,   286,     0,     0,   313,   108,     0,   108,     0,     0,
       0,     0,   108,     0,   286,     0,     0,     0,     0,     0,
     107,     0,     0,     0,   286,   107,     0,     0,     0,     0,
     334,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,     0,   107,     0,   326,   354,     0,   326,     0,
       0,   162,   162,   162,   162,     0,     0,     0,     0,     0,
       0,   108,     0,     0,   176,   176,   108,     0,   379,     0,
       0,     0,     0,     0,     0,   176,     0,   306,     0,     0,
       0,     0,   306,     0,     0,   176,     0,     0,   176,     0,
     108,     0,     0,     0,   108,   498,     0,     0,     0,     0,
     286,   286,     0,     0,     0,     0,   107,   107,   107,   107,
       0,     0,     0,     0,     0,     0,   224,     0,     0,     0,
     225,   107,   107,     0,     0,   445,     0,   449,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   313,     0,     0,     0,     0,
       0,     0,     0,     0,   162,     0,     0,   108,   108,   108,
     108,   286,     0,   227,     0,     0,   228,     0,     0,     0,
       0,     0,   108,   108,     0,     0,     0,     0,     0,     0,
       0,     0,   118,     0,     0,     0,   229,     0,     0,     0,
       0,   162,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,   231,   232,   233,   176,   234,   235,   236,
     574,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,     0,
       0,     0,     0,     0,     0,     0,   602,     0,     0,     0,
     306,   306,     0,   306,   306,     0,     0,   604,     0,     0,
      72,     0,     0,     0,     0,     0,     0,   773,     0,     0,
       0,     0,   306,     0,   306,     0,     0,     0,     0,     0,
     472,     0,     5,     6,     7,     8,     9,   286,     0,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,     0,     0,     0,   773,   125,   126,   127,
     578,    27,     0,   313,     0,     0,    30,    31,   326,    33,
     326,    34,    35,   326,     0,     0,   326,     0,    37,   678,
      39,     0,   185,     0,     0,     0,   224,     0,     0,     0,
     225,    46,    47,    48,   128,     0,     0,     0,   226,   688,
       0,     0,     0,   646,     0,     0,     0,   693,    55,   697,
       0,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,   227,     0,     0,   228,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,     0,   445,     0,
      66,     0,     0,   129,     0,    68,   229,   473,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,     0,
       0,     0,   746,     0,   492,     0,   748,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,     0,   708,    10,
      11,    12,     0,    13,    14,    15,    16,  -233,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,  -128,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -233,    37,    38,
      39,    40,  -128,    41,    42,     0,  -233,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -233,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,   122,   185,     0,     0,     0,
       0,     0,   751,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,     0,    68,     4,    69,     5,     6,
       7,     8,     9,     0,  -378,    10,    11,    12,  -378,    13,
      14,    15,    16,  -233,    17,    18,  -378,    19,    20,  -378,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -128,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -233,    37,     0,    39,    40,  -128,    41,
      42,  -378,  -233,    43,  -378,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -233,     0,     0,    53,
       0,    54,     0,     0,  -378,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -378,
      57,  -378,  -378,  -378,  -378,  -378,  -378,  -378,     0,  -378,
    -378,  -378,  -378,  -378,  -378,  -378,  -378,  -378,    62,  -378,
    -378,  -378,     0,    65,  -378,  -378,  -378,     0,     0,    67,
    -378,    68,     4,    69,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -233,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,  -128,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -233,
      37,    38,    39,    40,  -128,    41,    42,     0,  -233,    43,
       0,    44,    45,    46,    47,    48,    49,     0,    50,     0,
      51,    52,  -233,     0,     0,    53,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,   187,    65,
       0,     0,    66,     0,     0,    67,   198,    68,     4,    69,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -233,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,  -128,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -233,    37,    38,    39,    40,
    -128,    41,    42,     0,  -233,    43,     0,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -233,     0,
       0,    53,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
      62,     0,    63,    64,     0,    65,     0,     0,    66,     0,
       0,    67,   339,    68,     4,    69,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -233,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,  -128,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -233,    37,    38,    39,    40,  -128,    41,    42,     0,
    -233,    43,     0,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -233,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,   198,    68,
       4,    69,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -233,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,  -128,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -233,    37,    38,
      39,    40,  -128,    41,    42,     0,  -233,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -233,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,   606,    68,     4,    69,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -233,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -128,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -233,    37,    38,    39,    40,  -128,    41,
      42,     0,  -233,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -233,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
     701,    68,     4,    69,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -233,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,  -128,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -233,
      37,   211,    39,    40,  -128,    41,    42,     0,  -233,    43,
       0,    44,    45,    46,    47,    48,    49,     0,    50,     0,
      51,    52,  -233,     0,     0,    53,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,     0,    65,
       0,     0,    66,     0,     0,    67,     0,    68,     4,    69,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -233,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,  -128,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -233,    37,     0,    39,    40,
    -128,    41,    42,     0,  -233,    43,     0,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -233,     0,
       0,    53,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
      62,     0,    63,    64,     0,    65,     0,     0,    66,     0,
       0,    67,     0,    68,     4,    69,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -233,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,   452,     0,    27,    28,     0,  -128,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -233,    37,     0,    39,    40,  -128,    41,    42,     0,
    -233,    43,     0,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -233,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,     0,    68,
       4,    69,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -233,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,   456,
       0,    27,    28,     0,  -128,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -233,    37,     0,
      39,    40,  -128,    41,    42,     0,  -233,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -233,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,     0,    68,     4,    69,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -233,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   667,     0,    27,    28,     0,
    -128,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -233,    37,     0,    39,    40,  -128,    41,
      42,     0,  -233,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -233,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,     4,    69,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -233,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,   669,     0,    27,    28,     0,  -128,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -233,
      37,     0,    39,    40,  -128,    41,    42,     0,  -233,    43,
       0,    44,    45,    46,    47,    48,    49,     0,    50,     0,
      51,    52,  -233,     0,     0,    53,     0,    54,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,     0,    65,
       0,     0,    66,     0,     0,    67,     0,    68,     4,    69,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -233,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,   672,     0,    27,
      28,     0,  -128,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -233,    37,     0,    39,    40,
    -128,    41,    42,     0,  -233,    43,     0,    44,    45,    46,
      47,    48,    49,     0,    50,     0,    51,    52,  -233,     0,
       0,    53,     0,    54,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
      62,     0,    63,    64,     0,    65,     0,     0,    66,     0,
       0,    67,     0,    68,     4,    69,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -233,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,   675,     0,    27,    28,     0,  -128,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -233,    37,     0,    39,    40,  -128,    41,    42,     0,
    -233,    43,     0,    44,    45,    46,    47,    48,    49,     0,
      50,     0,    51,    52,  -233,     0,     0,    53,     0,    54,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,    62,     0,    63,    64,
       0,    65,     0,     0,    66,     0,     0,    67,     0,    68,
       4,    69,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -233,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,   692,
       0,    27,    28,     0,  -128,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -233,    37,     0,
      39,    40,  -128,    41,    42,     0,  -233,    43,     0,    44,
      45,    46,    47,    48,    49,     0,    50,     0,    51,    52,
    -233,     0,     0,    53,     0,    54,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,    62,     0,    63,    64,     0,    65,     0,     0,
      66,     0,     0,    67,     0,    68,     4,    69,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -233,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   696,     0,    27,    28,     0,
    -128,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -233,    37,     0,    39,    40,  -128,    41,
      42,     0,  -233,    43,     0,    44,    45,    46,    47,    48,
      49,     0,    50,     0,    51,    52,  -233,     0,     0,    53,
       0,    54,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,    62,     0,
      63,    64,     0,    65,     0,     0,    66,     0,     0,    67,
       0,    68,   472,    69,     5,     6,     7,     8,     9,     0,
    -378,   124,     0,     0,  -378,     0,     0,     0,     0,     0,
       0,     0,  -378,     0,    20,     0,     0,     0,     0,   125,
     126,   127,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    34,    35,     0,     0,     0,     0,     0,
      37,     0,    39,     0,     0,     0,     0,  -378,     0,     0,
    -378,     0,     0,    46,    47,    48,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -251,
    -378,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -378,    57,  -378,  -378,  -378,
       0,  -378,  -378,  -378,     0,  -378,  -378,  -378,  -378,  -378,
    -378,  -378,  -378,  -378,    62,  -378,  -378,  -378,     0,     0,
    -378,  -378,  -378,     0,     0,   129,     0,    68,   472,   486,
       5,     6,     7,     8,     9,     0,  -378,   124,     0,     0,
    -378,     0,     0,     0,     0,     0,     0,     0,  -378,     0,
      20,     0,     0,     0,     0,   125,   126,   127,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,  -378,     0,     0,  -378,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -378,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -378,    57,  -378,  -378,  -378,     0,  -378,  -378,  -378,
       0,  -378,  -378,  -378,  -378,  -378,  -378,  -378,  -378,  -378,
      62,  -378,  -378,  -378,     0,     0,  -378,  -378,  -378,     0,
       0,   129,     0,    68,   472,   486,     5,     6,     7,     8,
       9,     0,     0,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    20,     0,     0,     0,
       0,   125,   126,   127,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   124,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,    20,     0,     0,     0,
       0,   125,   126,   127,    61,    27,    62,     0,    63,    64,
      30,    31,     0,    33,    66,    34,    35,   129,     0,    68,
       0,   486,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     5,     6,
       7,     8,     9,     0,    61,   124,    62,     0,    63,    64,
     187,     0,     0,     0,    66,     0,     0,   129,    20,    68,
       0,   130,   767,   125,   126,   127,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     128,     0,     0,   202,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,   203,     0,
       5,     6,     7,     8,     9,     0,    61,   124,    62,     0,
      63,    64,   187,     0,     0,     0,    66,     0,     0,   129,
      20,    68,     0,   130,     0,   125,   126,   127,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,   202,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     5,     6,     7,     8,     9,     0,    61,   124,
      62,     0,    63,    64,   187,     0,     0,     0,    66,     0,
       0,   129,    20,    68,   493,   130,     0,   125,   126,   127,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,   152,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,     0,    55,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   124,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,    20,     0,     0,     0,     0,   125,   126,   127,
      61,    27,    62,     0,    63,    64,    30,    31,     0,    33,
      66,    34,    35,   129,     0,    68,     0,   130,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,   202,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     5,     6,     7,     8,     9,     0,
      61,   124,    62,     0,    63,    64,   187,     0,     0,     0,
      66,     0,     0,   129,    20,    68,     0,   130,     0,   125,
     126,   127,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    34,    35,     0,     0,     0,     0,     0,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,    62,     0,    63,    64,     0,   172,
       0,     0,    66,     0,     0,   129,     0,    68,     0,   130,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   175,
      20,     0,     0,     0,     0,   125,   126,   127,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   127,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,    67,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
       0,     0,     5,     6,     7,     8,     9,     0,    61,   124,
      62,     0,    63,    64,   187,     0,     0,     0,    66,     0,
       0,   129,    20,    68,     0,   130,     0,   125,   126,   127,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,   128,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,    57,    58,    59,     0,     0,     0,
       0,    60,     0,     0,     5,     6,     7,     8,     9,     0,
      61,   124,    62,     0,    63,    64,     0,     0,     0,     0,
      66,     0,     0,   129,    20,    68,     0,   130,     0,   125,
     126,   127,     0,    27,     0,     0,     0,     0,    30,    31,
       0,    33,     0,    34,    35,   590,     0,     0,     0,     0,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   124,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,   729,     0,     0,     0,     0,   125,
     126,   127,    61,    27,    62,     0,    63,    64,    30,    31,
       0,    33,    66,    34,    35,   129,     0,    68,     0,   130,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   730,    47,    48,   731,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    58,    59,     0,
       0,     0,     0,    60,     0,     0,     5,     6,     7,     8,
       9,     0,    61,   124,    62,     0,    63,    64,   187,     0,
       0,     0,    66,     0,     0,   129,    20,    68,     0,   732,
       0,   125,   126,   127,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,   128,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
      59,     0,     0,     0,     0,    60,     0,     0,     5,     6,
       7,     8,     9,     0,    61,   124,    62,     0,    63,    64,
     187,     0,     0,     0,    66,     0,     0,   129,    20,    68,
       0,   732,     0,   125,   126,   720,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
     128,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      57,    58,    59,     0,     0,     0,     0,    60,     0,     0,
       5,     6,     7,     8,     9,     0,    61,   124,    62,     0,
      63,    64,   187,     0,     0,     0,    66,     0,     0,   129,
      20,    68,     0,   130,     0,   125,   126,   127,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   539,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   542,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   714,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   715,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   717,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   718,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   719,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   720,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,    60,
      20,     0,     0,     0,     0,   125,   126,   127,    61,    27,
      62,     0,    63,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   124,     0,     0,
       0,    56,    57,    58,    59,     0,     0,     0,     0,     0,
      20,     0,     0,     0,     0,   125,   126,   127,    61,    27,
      62,     0,     0,    64,    30,    31,     0,    33,    66,    34,
      35,   129,     0,    68,     0,   130,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,   128,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -260,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,     0,   225,
       0,     0,    57,     0,     0,     0,  -260,   226,     0,     0,
    -260,     0,     5,     6,     7,     8,     9,     0,     0,   124,
      62,     0,     0,     0,   187,     0,     0,     0,     0,     0,
       0,   129,    20,    68,  -260,   732,     0,   125,   126,   127,
       0,    27,   227,     0,     0,   228,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,   229,     0,     0,     0,     5,
       0,    46,    47,    48,   128,   610,     0,     0,     0,   611,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
     237,   238,   239,   240,   241,   242,   243,   244,   245,     0,
     246,   247,   248,     0,    57,   249,   250,   251,     0,     0,
       0,     0,     0,     0,     0,   659,     0,     0,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,   130,     0,     0,
       0,     0,     0,     0,     0,     0,   612,   613,   614,   615,
     616,   617,     0,     0,   618,   619,   620,   621,   622,   623,
     624,   625,   626,   627,     0,     0,   628,     0,   226,     0,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,     0,   640,     0,     0,   641,   642,   643,   644,   645,
     224,     0,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,   226,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,   227,     0,     0,
     228,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,     0,   242,   243,   244,   245,
     229,   246,   247,   248,     0,     0,   249,   250,   251,     0,
       0,   227,     0,     0,   228,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   229,   246,   247,   248,     0,     0,
     249,   250,   251,     0,     0,     0,     0,     0,     0,   230,
     776,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   224,   246,
     247,   248,   225,     0,   249,   250,   251,     0,     0,     0,
     226,   301,     0,     0,   810,     0,     0,     0,     0,     0,
     302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   227,   226,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   118,     0,     0,     0,   229,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   227,     0,   230,   228,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   224,   246,   247,   248,   225,     0,   249,   250,
     251,     0,     0,   274,   226,   298,     0,     0,     0,   230,
       0,   231,   232,   233,   299,   234,   235,   236,     0,   237,
     238,   239,   240,     0,   242,   243,   244,   245,     0,   246,
       0,   248,     0,     0,   249,   250,   251,     0,     0,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,     0,   229,   225,     0,     0,     0,     0,     0,     0,
       0,   226,   569,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,     0,   227,   274,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   118,   224,     0,     0,   229,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,   230,     0,   231,   232,   233,     0,
     234,   235,   236,     0,   237,   238,   239,   240,   241,   242,
     243,   244,   245,     0,   246,   247,   248,     0,     0,   249,
     250,   251,     0,   227,   274,     0,   228,     0,     0,     0,
       0,     0,     0,   305,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,   229,   225,     0,     0,
       0,     0,     0,     0,     0,   226,   175,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,     0,
     227,   274,     0,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,   229,   225,     0,     0,     0,     0,     0,
       0,     0,   226,   563,     0,     0,     0,     0,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,   241,   242,   243,   244,   245,     0,   246,   247,
     248,     0,     0,   249,   250,   251,     0,   227,   274,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
     229,   225,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,     0,   227,   274,     0,   228,     0,     0,
       0,     0,     0,     0,   666,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,   229,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   230,     0,   231,   232,   233,     0,   234,   235,
     236,     0,   237,   238,   239,   240,   241,   242,   243,   244,
     245,     0,   246,   247,   248,     0,     0,   249,   250,   251,
       0,   227,   274,     0,   228,     0,     0,     0,     0,     0,
       0,   671,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   229,   225,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,     0,   227,   274,
       0,   228,     0,     0,     0,     0,     0,     0,   762,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,   229,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,     0,   237,   238,   239,   240,
     241,   242,   243,   244,   245,     0,   246,   247,   248,     0,
       0,   249,   250,   251,     0,   227,   274,     0,   228,     0,
       0,     0,     0,     0,     0,   763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,   229,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,     0,   227,   274,     0,   228,     0,     0,     0,     0,
       0,     0,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,   229,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
     237,   238,   239,   240,   241,   242,   243,   244,   245,     0,
     246,   247,   248,     0,     0,   249,   250,   251,     0,   227,
     274,     0,   228,     0,     0,     0,     0,     0,     0,   765,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,     0,   229,   225,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,     0,   227,   274,     0,   228,
       0,     0,     0,     0,     0,     0,   777,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,   229,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,   230,     0,   231,   232,   233,     0,
     234,   235,   236,     0,   237,   238,   239,   240,   241,   242,
     243,   244,   245,     0,   246,   247,   248,     0,     0,   249,
     250,   251,     0,   227,   274,     0,   228,     0,     0,     0,
       0,     0,     0,   778,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,   229,   225,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,     0,
     227,   274,     0,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,   229,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,   241,   242,   243,   244,   245,     0,   246,   247,
     248,     0,     0,   249,   250,   251,     0,   227,   325,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
     229,     0,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,   230,     0,   231,   232,   233,
     280,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,     0,  -341,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   118,     0,   224,     0,   229,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   226,   429,
       0,     0,     0,   230,     0,   231,   232,   233,   430,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,     0,     0,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,   229,     0,   225,     0,
       0,     0,     0,     0,     0,     0,   226,   431,     0,     0,
       0,   230,     0,   231,   232,   233,   432,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,     0,
       0,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   229,   225,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,     0,   227,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,   229,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,     0,   237,   238,   239,   240,
     241,   242,   243,   244,   245,     0,   246,   247,   248,     0,
     284,   249,   250,   251,     0,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,   229,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,   324,   249,   250,
     251,     0,   227,     0,     0,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,   229,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
     237,   238,   239,   240,   241,   242,   243,   244,   245,     0,
     246,   247,   248,     0,   338,   249,   250,   251,     0,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,   350,   229,   225,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,     0,   227,     0,     0,   228,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,   229,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,   230,     0,   231,   232,   233,     0,
     234,   235,   236,     0,   237,   238,   239,   240,   241,   242,
     243,   244,   245,     0,   246,   247,   248,     0,     0,   249,
     250,   251,     0,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,   229,   225,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   516,   249,   250,   251,     0,
     227,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,   229,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,   241,   242,   243,   244,   245,     0,   246,   247,
     248,     0,   517,   249,   250,   251,     0,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
     229,   225,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,   518,
     249,   250,   251,     0,   227,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,   229,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   230,     0,   231,   232,   233,     0,   234,   235,
     236,     0,   237,   238,   239,   240,   241,   242,   243,   244,
     245,     0,   246,   247,   248,     0,   519,   249,   250,   251,
       0,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   229,   225,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,   520,   249,   250,   251,     0,   227,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,   229,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,     0,   237,   238,   239,   240,
     241,   242,   243,   244,   245,     0,   246,   247,   248,     0,
     521,   249,   250,   251,     0,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,   229,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,   522,   249,   250,
     251,     0,   227,     0,     0,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,   229,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
     237,   238,   239,   240,   241,   242,   243,   244,   245,     0,
     246,   247,   248,     0,   523,   249,   250,   251,     0,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,     0,   229,   225,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,   524,   249,   250,   251,     0,   227,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,   229,
     225,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,   230,     0,   231,   232,   233,     0,
     234,   235,   236,     0,   237,   238,   239,   240,   241,   242,
     243,   244,   245,     0,   246,   247,   248,     0,   525,   249,
     250,   251,     0,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,   229,   225,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,   526,   249,   250,   251,     0,
     227,     0,     0,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,   229,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,   241,   242,   243,   244,   245,     0,   246,   247,
     248,     0,   527,   249,   250,   251,     0,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
     229,   225,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,   528,
     249,   250,   251,     0,   227,     0,     0,   228,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,   229,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   230,     0,   231,   232,   233,     0,   234,   235,
     236,     0,   237,   238,   239,   240,   241,   242,   243,   244,
     245,     0,   246,   247,   248,     0,   529,   249,   250,   251,
       0,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   229,   225,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,   530,   249,   250,   251,     0,   227,     0,
       0,   228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   118,   224,     0,
       0,   229,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,     0,   237,   238,   239,   240,
     241,   242,   243,   244,   245,     0,   246,   247,   248,     0,
       0,   249,   250,   251,     0,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,   229,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
     573,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,   549,   249,   250,
     251,     0,   227,     0,     0,   228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,   229,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
     237,   238,   239,   240,   241,   242,   243,   244,   245,     0,
     246,   247,   248,     0,     0,   249,   250,   251,     0,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,   605,   229,   225,     0,     0,     0,     0,     0,     0,
       0,   226,   654,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,     0,   227,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,   229,
     225,     0,     0,     0,     0,     0,     0,     0,   226,   657,
       0,     0,     0,     0,   230,     0,   231,   232,   233,     0,
     234,   235,   236,     0,   237,   238,   239,   240,   241,   242,
     243,   244,   245,     0,   246,   247,   248,     0,     0,   249,
     250,   251,     0,   227,     0,     0,   228,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,   229,   225,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
       0,   230,     0,   231,   232,   233,     0,   234,   235,   236,
       0,   237,   238,   239,   240,   241,   242,   243,   244,   245,
       0,   246,   247,   248,     0,     0,   249,   250,   251,     0,
     227,     0,     0,   228,     0,     0,     0,     0,     0,     0,
     713,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,   229,   225,     0,     0,     0,     0,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,   241,   242,   243,   244,   245,     0,   246,   247,
     248,     0,     0,   249,   250,   251,     0,   227,     0,     0,
     228,     0,     0,     0,     0,     0,     0,   716,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
     229,   225,     0,     0,     0,     0,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   230,     0,   231,   232,   233,
       0,   234,   235,   236,     0,   237,   238,   239,   240,   241,
     242,   243,   244,   245,     0,   246,   247,   248,     0,     0,
     249,   250,   251,     0,   227,     0,     0,   228,     0,     0,
       0,     0,     0,     0,   783,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   224,     0,     0,   229,   225,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,     0,   230,     0,   231,   232,   233,     0,   234,   235,
     236,     0,   237,   238,   239,   240,   241,   242,   243,   244,
     245,     0,   246,   247,   248,     0,     0,   249,   250,   251,
       0,   227,     0,     0,   228,     0,     0,     0,     0,     0,
       0,   784,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   229,   225,     0,     0,     0,     0,
       0,     0,     0,   226,     0,     0,     0,     0,     0,   230,
       0,   231,   232,   233,     0,   234,   235,   236,     0,   237,
     238,   239,   240,   241,   242,   243,   244,   245,     0,   246,
     247,   248,     0,     0,   249,   250,   251,     0,   227,     0,
       0,   228,     0,     0,     0,     0,     0,     0,   785,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,   229,   225,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,   230,     0,   231,   232,
     233,     0,   234,   235,   236,     0,   237,   238,   239,   240,
     241,   242,   243,   244,   245,     0,   246,   247,   248,     0,
       0,   249,   250,   251,     0,   227,     0,     0,   228,     0,
       0,     0,     0,     0,     0,   786,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,   229,   225,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,   230,     0,   231,   232,   233,     0,   234,
     235,   236,     0,   237,   238,   239,   240,   241,   242,   243,
     244,   245,     0,   246,   247,   248,     0,     0,   249,   250,
     251,     0,   227,     0,     0,   228,     0,     0,     0,     0,
       0,     0,   787,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,     0,     0,   229,   225,     0,     0,     0,
       0,     0,     0,     0,   226,     0,     0,     0,     0,     0,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
     237,   238,   239,   240,   241,   242,   243,   244,   245,     0,
     246,   247,   248,     0,     0,   249,   250,   251,     0,   227,
       0,     0,   228,     0,     0,     0,     0,     0,     0,   788,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,     0,   229,   225,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,   230,     0,   231,
     232,   233,     0,   234,   235,   236,     0,   237,   238,   239,
     240,   241,   242,   243,   244,   245,     0,   246,   247,   248,
       0,     0,   249,   250,   251,   226,   227,     0,     0,   228,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,     0,     0,   226,     0,     0,
     227,     0,     0,   228,   230,     0,   231,   232,   233,     0,
     234,   235,   236,     0,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   229,   246,   247,   248,     0,     0,   249,
     250,   251,   227,     0,     0,   228,     0,     0,   230,     0,
     231,   232,   233,     0,   234,   235,   236,     0,   237,   238,
     239,   240,     0,   242,   243,   244,   245,     0,   246,     0,
     248,     0,     0,   249,   250,   251,     0,     0,     0,     0,
     230,     0,   231,   232,   233,     0,   234,   235,   236,     0,
       0,   238,   239,   240,     0,   242,   243,   244,   245,     0,
       0,     0,   248,     0,     0,   249,   250,   251
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-694))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,     2,   140,   331,   312,    15,    14,   271,    18,   271,
     587,   317,   150,   156,    24,    25,    26,   158,   218,   219,
     220,   221,     3,    33,    34,   331,    36,    37,     3,   471,
     171,     3,     1,    43,    44,    45,    46,     3,   648,    49,
     181,     3,    52,    53,    54,    58,     3,    57,    75,   740,
      60,    61,    62,     1,    64,   608,    18,    67,    68,    69,
       3,    22,    52,    31,   608,   208,    99,    22,     1,    31,
       0,    99,    34,    22,   767,    31,   126,    22,   128,    69,
      23,    48,    57,    68,    99,    47,    48,    68,   779,    58,
      57,    75,     3,     1,    99,    57,   129,   125,    31,    31,
      67,    57,   129,    60,    73,    67,   683,   800,    99,    46,
      58,   124,   127,    31,   124,   125,   126,   127,   128,   129,
     130,    80,   127,    31,    67,    73,   267,   268,   136,    98,
      75,    99,    67,   126,   144,    72,   127,    14,    99,   126,
     130,   583,   752,   753,   103,   126,    58,    80,   158,   118,
      98,   126,   705,    56,   126,   124,    59,    75,   358,   144,
     126,   171,   470,   124,   126,    98,    99,    99,   721,   124,
      78,   181,    80,   158,   184,   124,   124,   721,    55,   124,
      99,    99,    46,    99,    99,   118,   171,   328,   125,   766,
      98,    99,    69,   203,   184,    31,   181,   129,    31,   200,
      31,     7,    12,   118,   126,   124,   118,    17,    72,   125,
     118,   129,   124,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,    64,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   125,    99,    99,    18,   707,    99,   126,    99,   577,
     280,    99,   536,    41,   536,   285,   154,   155,   156,    99,
     586,   118,   267,   268,    99,    80,   296,    35,   298,   299,
     168,   301,   302,   129,    48,   305,   129,    18,   129,   127,
     178,    80,    99,    57,    99,    53,   296,   127,   103,    24,
     320,   118,   127,   191,   118,   458,   459,    71,   328,   800,
      99,   118,     2,    99,   103,   335,   118,    48,    99,   810,
      10,   118,   127,   343,   123,   345,    57,   347,   348,    19,
     350,   351,   118,   328,   118,   355,    67,   118,   127,    99,
     335,   489,    32,   496,   497,    78,    99,    80,   343,   687,
     345,    14,     3,   691,    17,   355,     7,   118,   118,    49,
      21,   118,    25,    26,    27,   118,   682,   126,   118,   128,
      33,   687,    35,   124,   118,   691,    98,    67,    18,   101,
     126,    71,   126,   105,   128,    74,   126,    50,   128,   118,
      53,    31,    55,   281,    34,    56,   124,   343,    59,   345,
     126,   101,   126,   126,    67,    80,    69,    47,    48,   429,
     430,   431,   432,   433,   434,   435,   304,    57,   436,   437,
     118,   441,   442,     7,   444,    31,    98,    67,   127,   119,
     450,     2,   452,   118,    99,   118,   456,    98,    31,    10,
     101,    24,    99,   127,   105,   127,    78,   442,    19,   444,
      80,   767,    58,   473,   127,   118,   476,   477,   127,   129,
     776,    32,   125,    99,   124,   124,   486,    73,    14,   159,
      56,    17,   127,   473,    80,   129,    80,    24,    49,    25,
      26,    27,   127,   127,   800,   175,   486,    33,    99,    35,
      22,    22,   129,    99,   810,   124,    67,    98,   129,   101,
      71,   129,   227,   228,    50,   126,    73,    53,   127,    55,
     200,   503,   118,    22,   449,   485,   151,   664,   124,   539,
     555,    67,   542,    69,   709,   709,    21,   710,   575,   609,
     779,   766,   750,   321,   554,   728,   444,   536,    -1,    -1,
      -1,   561,    -1,   563,   269,   270,   566,    -1,   119,   569,
      -1,     2,    -1,   573,    -1,   280,    -1,    -1,    -1,    10,
      -1,    56,    -1,    -1,    59,    -1,   454,   455,    19,    -1,
     458,   459,   118,   593,   299,    -1,    -1,   302,    -1,   125,
     600,    32,   602,    -1,   274,   605,    31,    -1,   159,    -1,
      -1,    -1,    -1,   593,    -1,    -1,   607,    -1,    49,    94,
      -1,    96,    97,    98,   175,   100,   101,    -1,   298,    -1,
     105,   301,    -1,    58,    -1,   305,    67,   112,    -1,    -1,
      71,    -1,   347,    -1,   119,   120,   121,    -1,    73,   200,
      -1,    -1,    -1,   653,   654,    80,   656,   657,   658,   659,
      -1,    -1,    -1,    -1,    -1,    -1,   666,   667,    -1,   669,
      -1,   671,   672,    -1,    99,   675,    -1,    -1,   348,   547,
     548,    -1,    -1,    -1,    -1,    -1,   686,   687,   119,    -1,
     558,   691,   692,   118,    -1,    -1,   696,    -1,    -1,   124,
     568,    -1,    -1,   571,    -1,    -1,     3,   682,    -1,    -1,
       7,     8,    -1,   713,   714,   715,   716,   717,   718,   719,
     720,    18,    -1,   274,    -1,   430,    31,   432,   159,    -1,
     730,   731,   732,    -1,    -1,    -1,   736,    -1,    -1,   739,
      -1,    -1,    -1,    -1,   175,    -1,    -1,   298,    -1,    -1,
     301,    48,   732,    58,   305,   425,    -1,    -1,    -1,    -1,
      57,   736,   762,   763,   764,   765,    -1,    -1,    73,   200,
      67,    -1,    -1,    -1,    71,    80,   776,   777,   778,    -1,
      -1,     2,    -1,   783,   784,   785,   786,   787,   788,    10,
      -1,    -1,   767,   463,    99,    -1,    -1,   348,    19,   799,
     800,    -1,    -1,    14,    -1,    -1,    17,    -1,    -1,    -1,
     810,    32,    -1,   118,    25,    26,    27,    -1,    -1,   124,
      -1,   689,    33,    -1,    35,   800,    -1,    -1,    49,    -1,
     500,    -1,    -1,   503,    -1,   810,     3,    -1,    -1,    50,
       7,    -1,    53,   274,    55,    -1,    67,    -1,    18,    -1,
      71,    18,    -1,    -1,     2,    -1,    67,    -1,    69,    -1,
      -1,    31,    10,    -1,    34,    -1,    -1,   298,   538,    -1,
     301,    19,    -1,    -1,   305,    -1,    -1,    47,    48,    -1,
      -1,    48,    -1,    21,    32,    -1,    -1,    57,    -1,    -1,
      57,   561,    -1,   563,    -1,    -1,   566,    67,   119,   569,
      67,    49,   572,     2,    71,    -1,    -1,   118,    -1,    79,
      -1,    10,   463,    -1,    -1,    -1,    -1,   348,    56,    67,
      19,    59,   592,    71,    94,    -1,    96,    97,    -1,    -1,
     600,    -1,   602,    32,    -1,    -1,    -1,   607,   159,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,    -1,    -1,   500,
      49,   121,   503,    -1,   175,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,    -1,   105,    67,    -1,
      -1,   119,    71,   111,   112,    -1,    -1,    -1,   116,   200,
      -1,   119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,    -1,
      -1,   671,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     561,   159,   563,    -1,    -1,   566,    -1,    -1,   569,    21,
     119,   572,    -1,    -1,    -1,   695,    -1,   175,    -1,   699,
      -1,    -1,   463,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   592,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   600,
      -1,   602,   200,   274,    56,    -1,   607,    59,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   500,
      -1,    -1,   503,    -1,    -1,    -1,   175,   298,    -1,    -1,
     301,    -1,    -1,    21,   305,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   762,   763,   764,   765,    98,    -1,   100,   101,
      -1,   200,    -1,   105,    -1,    -1,    -1,   777,   778,    -1,
     112,    -1,    -1,    -1,    -1,   666,    -1,    -1,    56,   121,
     671,    59,    -1,    -1,    -1,    -1,   274,   348,    -1,    -1,
     561,    -1,   563,    -1,    -1,   566,    -1,    -1,   569,    -1,
      -1,   572,    -1,    -1,   695,    -1,    -1,    -1,   699,    -1,
     298,    -1,    -1,   301,    -1,    -1,    94,   305,    96,    97,
      98,   592,   100,   101,    -1,    -1,    -1,   105,    -1,   600,
      -1,   602,    -1,   111,   112,   274,   607,    -1,   116,    -1,
      -1,   119,   120,   121,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    21,    -1,    -1,   298,
     348,    -1,   301,    19,    -1,    -1,   305,    -1,    -1,    -1,
      -1,   762,   763,   764,   765,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   777,   778,    21,    -1,
      -1,    56,    -1,    49,    59,   666,    -1,     2,    -1,    -1,
     671,    -1,   463,    -1,    -1,    10,    -1,    -1,    -1,   348,
      -1,    67,    -1,    -1,    19,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,   695,    -1,    59,    32,   699,    94,
      -1,    -1,    97,    98,    -1,   100,   101,    -1,    -1,   500,
     105,    -1,   503,    -1,    49,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,   119,   120,   121,    -1,    -1,    -1,
      -1,    94,    67,   119,    -1,    98,    71,   100,   101,    -1,
      -1,    -1,   105,    -1,    -1,   463,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,    -1,
      -1,   762,   763,   764,   765,    -1,    -1,    -1,    -1,    -1,
     561,    -1,   563,   159,    -1,   566,   777,   778,   569,    -1,
      -1,   572,   500,    -1,   119,   503,    -1,    21,    -1,   175,
      -1,    -1,    -1,    -1,   463,    -1,    -1,    -1,    -1,    -1,
      -1,   592,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   600,
      -1,   602,    -1,    -1,   200,    -1,   607,    -1,    -1,    -1,
      -1,    -1,    56,    -1,   159,    59,    -1,    -1,    -1,    -1,
      -1,   500,    45,    -1,   503,    -1,    -1,    -1,    -1,    -1,
     175,    -1,    -1,   561,    -1,   563,    -1,    -1,   566,    -1,
      -1,   569,    -1,    -1,   572,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,   200,   100,   101,    -1,    -1,
      -1,   105,    -1,    -1,   592,   666,    -1,    -1,   112,    -1,
     671,    -1,   600,    -1,   602,   119,   120,   121,   274,   607,
      -1,    -1,   561,    -1,   563,    -1,    -1,   566,    -1,    -1,
     569,    -1,    -1,   572,   695,    -1,    -1,    -1,   699,    -1,
     123,    -1,   298,    -1,    -1,   301,    -1,    -1,    -1,   305,
      -1,    -1,    -1,   592,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   600,    -1,   602,    -1,    -1,    -1,    -1,   607,   274,
      -1,   154,   155,   156,   157,    -1,    -1,    -1,   666,    -1,
      -1,    -1,    -1,   671,    -1,   168,    -1,    -1,    -1,    -1,
      -1,    -1,   348,   298,    -1,   178,   301,    -1,    -1,    -1,
     305,   762,   763,   764,   765,    -1,    -1,   695,   191,    -1,
      -1,   699,    -1,    -1,    -1,    -1,   777,   778,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,
      -1,    -1,   671,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   348,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,   695,    10,    -1,    -1,
     699,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   762,   763,   764,   765,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   777,
     778,    -1,    -1,    -1,    -1,    -1,    49,    -1,   281,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   463,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    71,    -1,
      -1,   304,    -1,   762,   763,   764,   765,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   777,   778,
      -1,    -1,    -1,    -1,   500,    12,    -1,   503,    -1,    -1,
      17,    -1,    -1,    -1,    21,    22,    23,    -1,   463,    -1,
      -1,    -1,    29,    -1,    31,    -1,   119,    -1,    -1,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   500,    -1,    -1,   503,    -1,
      -1,    -1,    -1,    -1,    -1,   561,   159,   563,    -1,    -1,
     566,   122,   123,   569,    -1,   126,   572,   128,    -1,    -1,
      87,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   592,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   600,    -1,   602,   200,    -1,    -1,
      -1,   607,    -1,    -1,    -1,    -1,   561,    -1,   563,    -1,
      -1,   566,    -1,    -1,   569,    -1,    -1,   572,    -1,    -1,
      -1,   454,   455,   140,    -1,   458,   459,   144,    -1,    -1,
      -1,    -1,    -1,   150,    -1,   152,    -1,   592,    -1,    -1,
      -1,   158,    -1,    -1,   161,   600,    -1,   602,    -1,    -1,
      -1,    -1,   607,    -1,   171,    -1,    -1,    -1,    -1,    -1,
     666,    -1,    -1,    -1,   181,   671,    -1,    -1,    -1,    -1,
     187,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   695,
      -1,    -1,    -1,   699,    -1,   298,   213,    -1,   301,    -1,
      -1,   218,   219,   220,   221,    -1,    -1,    -1,    -1,    -1,
      -1,   666,    -1,    -1,   547,   548,   671,    -1,   235,    -1,
      -1,    -1,    -1,    -1,    -1,   558,    -1,   560,    -1,    -1,
      -1,    -1,   565,    -1,    -1,   568,    -1,    -1,   571,    -1,
     695,    -1,    -1,    -1,   699,   348,    -1,    -1,    -1,    -1,
     267,   268,    -1,    -1,    -1,    -1,   762,   763,   764,   765,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,   777,   778,    -1,    -1,   292,    -1,   294,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   312,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   321,    -1,    -1,   762,   763,   764,
     765,   328,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,   777,   778,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,   689,   100,   101,   102,
     463,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,
     723,   724,    -1,   726,   727,    -1,    -1,   500,    -1,    -1,
     503,    -1,    -1,    -1,    -1,    -1,    -1,   740,    -1,    -1,
      -1,    -1,   745,    -1,   747,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,   444,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,   779,    28,    29,    30,
     467,    32,    -1,   470,    -1,    -1,    37,    38,   561,    40,
     563,    42,    43,   566,    -1,    -1,   569,    -1,    49,   572,
      51,    -1,   489,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    62,    63,    64,    65,    -1,    -1,    -1,    21,   592,
      -1,    -1,    -1,   510,    -1,    -1,    -1,   600,    79,   602,
      -1,    -1,    -1,    -1,   607,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,    -1,   555,    -1,
     121,    -1,    -1,   124,    -1,   126,    79,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     577,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,    -1,   695,    -1,   127,    -1,   699,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,    -1,   625,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    -1,    -1,    74,    -1,    76,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,   702,   703,    -1,    -1,    -1,
      -1,    -1,   709,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,   113,    -1,   115,   116,    -1,   118,    -1,    -1,
     121,    -1,    -1,   124,    -1,   126,     1,   128,     3,     4,
       5,     6,     7,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,    74,
      -1,    76,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,   118,   119,   120,   121,    -1,    -1,   124,
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
      -1,    -1,    79,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   111,    32,   113,    -1,   115,   116,
      37,    38,    -1,    40,   121,    42,    43,   124,    -1,   126,
      -1,   128,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,    -1,    -1,    -1,   102,    -1,    -1,     3,     4,
       5,     6,     7,    -1,   111,    10,   113,    -1,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,    -1,   124,    23,   126,
      -1,   128,   129,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    -1,    -1,    -1,    -1,   102,   103,    -1,
       3,     4,     5,     6,     7,    -1,   111,    10,   113,    -1,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,    -1,   124,
      23,   126,    -1,   128,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   111,    10,
     113,    -1,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
      -1,   124,    23,   126,   127,   128,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     111,    32,   113,    -1,   115,   116,    37,    38,    -1,    40,
     121,    42,    43,   124,    -1,   126,    -1,   128,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    -1,    -1,    68,    -1,    -1,
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
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,   113,    -1,   115,   116,    -1,   118,
      -1,    -1,   121,    -1,    -1,   124,    -1,   126,    -1,   128,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    -1,    -1,    -1,
      -1,   102,    -1,    -1,     3,     4,     5,     6,     7,    -1,
     111,    10,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
     121,    -1,    -1,   124,    23,   126,    -1,   128,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,   102,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   111,    32,   113,    -1,   115,   116,    37,    38,
      -1,    40,   121,    42,    43,   124,    -1,   126,    -1,   128,
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
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,   102,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,   115,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   111,    32,
     113,    -1,    -1,   116,    37,    38,    -1,    40,   121,    42,
      43,   124,    -1,   126,    -1,   128,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    95,    -1,    -1,    -1,    99,    21,    -1,    -1,
     103,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    23,   126,   127,   128,    -1,    28,    29,    30,
      -1,    32,    56,    -1,    -1,    59,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,     3,
      -1,    62,    63,    64,    65,     9,    -1,    -1,    -1,    13,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,    95,   119,   120,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    82,    83,
      84,    85,    -1,    -1,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    -1,    -1,   100,    -1,    21,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,    -1,    -1,   119,   120,   121,   122,   123,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    79,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    56,    -1,    -1,
      59,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
      79,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,    56,    -1,    -1,    59,    94,    -1,    96,    97,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    79,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,    -1,    -1,    -1,    -1,    -1,    94,
     129,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     9,   114,
     115,   116,    13,    -1,   119,   120,   121,    -1,    -1,    -1,
      21,    22,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    21,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    94,    59,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     9,   114,   115,   116,    13,    -1,   119,   120,
     121,    -1,    -1,   124,    21,    22,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    31,   100,   101,   102,    -1,   104,
     105,   106,   107,    -1,   109,   110,   111,   112,    -1,   114,
      -1,   116,    -1,    -1,   119,   120,   121,    -1,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,    -1,   119,   120,   121,    -1,    56,   124,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,    -1,   119,
     120,   121,    -1,    56,   124,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      56,   124,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,    -1,   119,   120,   121,    -1,    56,   124,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
      -1,   119,   120,   121,    -1,    56,   124,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,    -1,    -1,   119,   120,   121,    -1,    56,   124,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      79,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,
      31,   100,   101,   102,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,    -1,    -1,
     119,   120,   121,    -1,   123,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,     9,    -1,    79,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    94,    -1,    96,    97,    98,    31,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,    -1,   119,   120,
     121,    -1,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    79,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    94,    -1,    96,    97,    98,    31,   100,   101,   102,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,   114,   115,   116,    -1,    -1,   119,   120,   121,    -1,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,    -1,   119,   120,   121,    -1,    56,    -1,
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
      -1,    78,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,     9,    -1,
      -1,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,    -1,
      -1,   119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      24,    -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,   114,   115,   116,    -1,   118,   119,   120,
     121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    79,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
     114,   115,   116,    -1,    -1,   119,   120,   121,    -1,    56,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    78,    79,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
      -1,    -1,   119,   120,   121,    -1,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    79,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,    -1,    -1,   119,
     120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     119,   120,   121,    -1,    56,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    79,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,   114,   115,   116,    -1,    -1,   119,   120,   121,
      -1,    56,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   119,   120,   121,    21,    56,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      56,    -1,    -1,    59,    94,    -1,    96,    97,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    79,   114,   115,   116,    -1,    -1,   119,
     120,   121,    56,    -1,    -1,    59,    -1,    -1,    94,    -1,
      96,    97,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,   107,    -1,   109,   110,   111,   112,    -1,   114,    -1,
     116,    -1,    -1,   119,   120,   121,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    98,    -1,   100,   101,   102,    -1,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,    -1,
      -1,    -1,   116,    -1,    -1,   119,   120,   121
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
     150,   151,   152,   154,   155,   158,   159,   160,   163,   166,
     169,   170,   190,   193,   194,   212,   213,   214,   215,   216,
     217,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   233,   234,   235,   236,   237,   138,   225,    75,   218,
     219,   145,   146,   218,    10,    28,    29,    30,    65,   124,
     128,   166,   212,   216,   223,   224,   225,   226,   228,   229,
      67,   145,   225,   138,   126,   146,     7,   145,   147,     8,
      67,   147,    48,    77,   153,   225,   225,   225,   126,   146,
     167,   126,   146,   195,   196,   138,   225,   225,   225,   225,
       7,   126,   118,   225,   225,    22,   141,   148,   225,   225,
      64,   126,   138,   225,    60,   146,   191,   117,   186,   206,
     225,   225,   225,   225,   225,   225,   225,   225,   125,   136,
     142,   206,    68,   103,   186,   207,   208,   225,   206,   225,
     232,    50,   138,    41,   146,    35,    53,   182,    18,    48,
      57,    71,   118,   123,     9,    13,    21,    56,    59,    79,
      94,    96,    97,    98,   100,   101,   102,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   114,   115,   116,   119,
     120,   121,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,   122,   126,   128,    56,
      59,   126,   138,   118,   124,   141,   225,   225,   225,   206,
      31,   218,   191,   118,   118,    74,   146,   186,   209,   210,
     211,   225,   124,   191,   160,   146,   126,   148,    22,    31,
     148,    22,    31,   148,   219,    66,   141,   209,   138,   126,
     177,    68,   126,   146,   197,   198,     1,    98,   200,   201,
      31,    99,   148,   209,   118,   124,   138,   148,   126,   209,
     206,    80,   192,   118,   146,    99,   118,   148,   118,   125,
     136,   125,   225,    99,   127,    99,   127,    31,   129,   219,
      78,    99,   129,     7,   146,    98,   161,   171,    57,   195,
     195,   195,   195,   225,   225,   225,   225,   153,   225,   153,
     225,   225,   225,   225,   225,   225,   225,    23,    67,   146,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   209,   209,   153,   225,   153,   225,    18,    31,    34,
      47,    48,    57,    67,   116,   180,   220,   221,   237,    22,
      31,    22,    31,    66,    31,   129,   153,   225,   148,   118,
     225,    78,    80,   127,    99,   146,   164,   165,   118,   146,
      31,   206,    30,   225,   153,   225,    30,   225,   153,   225,
     148,   138,   225,    24,   127,   178,   179,   180,   168,   198,
      99,   127,     1,   128,   202,   213,    78,    80,   199,   225,
     196,   127,   156,   209,   127,   124,   128,   202,   213,    99,
     186,   225,   127,   127,   207,   207,   153,   225,   138,   225,
     129,   225,   225,   124,   206,   124,    48,    57,    67,   173,
     181,   195,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   127,   129,    31,    57,   212,    99,   127,    56,    30,
     153,   225,    30,   153,   225,   153,   225,   218,   218,   118,
     225,   186,   225,   211,    80,    99,   125,   161,   225,   127,
     225,    22,   148,    22,   148,   225,    22,   148,   219,    22,
     148,   219,    24,    24,   138,    99,   127,   126,   146,    18,
      48,    57,    67,   183,   127,   198,    98,   201,   206,   225,
      44,   225,    46,    72,   125,   157,   127,   156,   206,   191,
     129,   219,   129,   219,   138,    78,   125,   142,   162,   172,
       9,    13,    80,    81,    82,    83,    84,    85,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,   100,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     116,   119,   120,   121,   122,   123,   146,   174,   175,   118,
     221,   212,   225,    22,    22,   225,    22,    22,   129,   129,
     148,   148,   225,   165,   124,   148,    66,    30,   225,    30,
     225,    66,    30,   225,   148,    30,   225,   148,   138,   225,
     179,   198,    98,   205,   201,   199,    31,   129,   138,   206,
     125,   129,    30,   138,   225,   129,    30,   138,   225,   129,
     225,   125,    27,    67,   125,   137,   150,   183,   146,   101,
     126,   176,   176,    66,    30,    30,    66,    30,    30,    30,
      30,   162,   225,   225,   225,   225,   225,   225,   127,    23,
      62,    65,   128,   186,   204,   213,   103,   188,   199,    73,
     189,   225,   202,   213,   148,   225,   138,   225,   138,   150,
     201,   146,   174,   175,   178,   225,   225,   225,   225,   225,
     225,   125,    66,    66,    66,    66,   205,   129,   206,   186,
     187,   225,   225,   141,   149,   185,   129,    66,    66,   189,
     176,   176,   127,    66,    66,    66,    66,    66,    66,   225,
     225,   225,   225,   188,   199,   186,   203,   204,   213,    31,
     129,   213,   225,   225,   118,   184,   185,   225,   203,   204,
     129,   203
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
#line 401 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 406 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 407 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 414 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 419 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 420 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 436 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 437 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 438 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 455 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 461 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 484 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 485 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 521 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
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

  case 76:

/* Line 1806 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 601 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { USR_FATAL((yyvsp[(3) - (6)].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 658 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 665 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 670 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 675 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 698 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 708 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 717 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 721 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 728 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 729 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 734 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 739 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 744 "chapel.ypp"
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

  case 128:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 770 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 777 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 785 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 796 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 802 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 809 "chapel.ypp"
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

  case 135:

/* Line 1806 of yacc.c  */
#line 831 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 835 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 840 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 844 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 853 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 855 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 856 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 857 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 858 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 965 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 967 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 991 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 996 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
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

  case 225:

/* Line 1806 of yacc.c  */
#line 1025 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1055 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
    }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1063 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1089 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1109 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1134 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1155 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1163 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1170 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1262 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1316 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1322 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1328 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1337 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1341 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1366 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[(1) - (2)].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[(2) - (2)].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[(2) - (2)].pexpr);
    }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1396 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1406 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1456 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7492 "bison-chapel.cpp"
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



