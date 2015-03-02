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
    ArgSymbol* first;
    Expr*      second;
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
     TPROC = 307,
     TRECORD = 308,
     TREDUCE = 309,
     TREF = 310,
     TRETURN = 311,
     TSCAN = 312,
     TSELECT = 313,
     TSERIAL = 314,
     TSINGLE = 315,
     TSPARSE = 316,
     TSUBDOMAIN = 317,
     TSYNC = 318,
     TTHEN = 319,
     TTYPE = 320,
     TUNDERSCORE = 321,
     TUNION = 322,
     TUSE = 323,
     TVAR = 324,
     TWHEN = 325,
     TWHERE = 326,
     TWHILE = 327,
     TWITH = 328,
     TYIELD = 329,
     TZIP = 330,
     TALIAS = 331,
     TAND = 332,
     TASSIGN = 333,
     TASSIGNBAND = 334,
     TASSIGNBOR = 335,
     TASSIGNBXOR = 336,
     TASSIGNDIVIDE = 337,
     TASSIGNEXP = 338,
     TASSIGNLAND = 339,
     TASSIGNLOR = 340,
     TASSIGNMINUS = 341,
     TASSIGNMOD = 342,
     TASSIGNMULTIPLY = 343,
     TASSIGNPLUS = 344,
     TASSIGNSL = 345,
     TASSIGNSR = 346,
     TBAND = 347,
     TBNOT = 348,
     TBOR = 349,
     TBXOR = 350,
     TCOLON = 351,
     TCOMMA = 352,
     TDIVIDE = 353,
     TDOT = 354,
     TDOTDOT = 355,
     TDOTDOTDOT = 356,
     TEQUAL = 357,
     TEXP = 358,
     TGREATER = 359,
     TGREATEREQUAL = 360,
     THASH = 361,
     TLESS = 362,
     TLESSEQUAL = 363,
     TMINUS = 364,
     TMOD = 365,
     TNOT = 366,
     TNOTEQUAL = 367,
     TOR = 368,
     TPLUS = 369,
     TQUESTION = 370,
     TSEMI = 371,
     TSHIFTLEFT = 372,
     TSHIFTRIGHT = 373,
     TSTAR = 374,
     TSWAP = 375,
     TIO = 376,
     TLCBR = 377,
     TRCBR = 378,
     TLP = 379,
     TRP = 380,
     TLSBR = 381,
     TRSBR = 382,
     TNOELSE = 383,
     TUMINUS = 384,
     TUPLUS = 385
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
#line 421 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 427 "bison-chapel.cpp"
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
#line 500 "bison-chapel.cpp"

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
#define YYLAST   10720

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  131
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  422
/* YYNRULES -- Number of states.  */
#define YYNSTATES  798

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   385

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
     125,   126,   127,   128,   129,   130
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    46,    49,    53,    57,    61,    65,    69,    73,    76,
      80,    84,    87,    90,    94,    96,   101,   107,   109,   112,
     116,   118,   121,   125,   130,   135,   140,   145,   150,   155,
     160,   165,   170,   175,   180,   185,   190,   195,   200,   201,
     203,   205,   208,   210,   213,   217,   219,   221,   223,   225,
     227,   229,   232,   238,   242,   249,   256,   261,   267,   273,
     277,   281,   288,   294,   301,   307,   314,   318,   323,   330,
     338,   345,   353,   358,   364,   369,   374,   378,   385,   391,
     397,   398,   401,   405,   408,   415,   423,   424,   426,   428,
     430,   432,   433,   436,   437,   440,   444,   450,   452,   455,
     459,   461,   465,   466,   467,   476,   477,   479,   481,   484,
     486,   489,   490,   491,   501,   505,   509,   515,   521,   523,
     526,   528,   530,   532,   534,   536,   538,   540,   542,   544,
     546,   548,   550,   552,   554,   556,   558,   560,   562,   564,
     566,   568,   570,   572,   574,   576,   578,   580,   582,   584,
     586,   588,   590,   592,   594,   596,   598,   599,   603,   607,
     608,   610,   614,   619,   624,   631,   638,   639,   641,   643,
     645,   647,   650,   653,   655,   657,   659,   660,   662,   664,
     666,   668,   669,   671,   673,   675,   677,   679,   681,   683,
     685,   688,   690,   691,   693,   696,   699,   700,   703,   707,
     712,   717,   720,   725,   726,   729,   732,   737,   742,   747,
     753,   758,   759,   761,   763,   765,   769,   773,   778,   784,
     786,   788,   792,   794,   797,   801,   802,   805,   808,   809,
     814,   815,   818,   821,   823,   828,   833,   840,   842,   843,
     845,   847,   851,   856,   860,   865,   872,   873,   876,   879,
     882,   885,   888,   891,   893,   895,   899,   903,   905,   907,
     909,   913,   917,   918,   920,   922,   926,   930,   934,   938,
     940,   942,   944,   946,   948,   950,   952,   954,   957,   962,
     967,   972,   978,   981,   984,   991,   998,  1003,  1013,  1023,
    1031,  1038,  1045,  1050,  1060,  1070,  1078,  1083,  1090,  1097,
    1107,  1117,  1124,  1126,  1128,  1130,  1132,  1134,  1136,  1138,
    1140,  1144,  1145,  1147,  1152,  1154,  1158,  1161,  1163,  1167,
    1170,  1175,  1177,  1179,  1181,  1183,  1185,  1187,  1189,  1191,
    1196,  1200,  1204,  1207,  1210,  1212,  1214,  1216,  1218,  1220,
    1222,  1224,  1229,  1234,  1239,  1243,  1247,  1251,  1255,  1260,
    1264,  1266,  1268,  1270,  1272,  1276,  1280,  1284,  1288,  1294,
    1298,  1302,  1306,  1310,  1314,  1318,  1322,  1326,  1330,  1334,
    1338,  1342,  1346,  1350,  1354,  1358,  1362,  1366,  1370,  1374,
    1378,  1382,  1386,  1389,  1392,  1395,  1398,  1401,  1404,  1408,
    1412,  1416,  1420,  1424,  1428,  1432,  1436,  1438,  1440,  1442,
    1444,  1446,  1448
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     132,     0,    -1,   133,    -1,    -1,   133,   134,    -1,   136,
      -1,   135,   136,    -1,    50,     7,    -1,   135,    50,     7,
      -1,   137,    -1,   139,    -1,   141,    -1,   147,    -1,   142,
      -1,   148,    -1,   151,    -1,   149,    -1,   152,    -1,   155,
      -1,   146,    -1,   215,   116,    -1,    10,   136,    -1,    11,
     216,   136,    -1,    12,   143,   116,    -1,    15,   216,   139,
      -1,    19,   143,   116,    -1,    20,   223,   116,    -1,    36,
     144,   136,    -1,    39,   136,    -1,    45,   223,   145,    -1,
      59,   223,   145,    -1,    59,   145,    -1,    63,   136,    -1,
      74,   223,   116,    -1,     1,    -1,   138,   144,   122,   123,
      -1,   138,   144,   122,   140,   123,    -1,    41,    -1,   122,
     123,    -1,   122,   140,   123,    -1,   134,    -1,   140,   134,
      -1,    68,   204,   116,    -1,   224,    78,   223,   116,    -1,
     224,    89,   223,   116,    -1,   224,    86,   223,   116,    -1,
     224,    88,   223,   116,    -1,   224,    82,   223,   116,    -1,
     224,    87,   223,   116,    -1,   224,    83,   223,   116,    -1,
     224,    79,   223,   116,    -1,   224,    80,   223,   116,    -1,
     224,    81,   223,   116,    -1,   224,    91,   223,   116,    -1,
     224,    90,   223,   116,    -1,   224,   120,   223,   116,    -1,
     224,    84,   223,   116,    -1,   224,    85,   223,   116,    -1,
      -1,   144,    -1,     3,    -1,    22,   136,    -1,   139,    -1,
      56,   116,    -1,    56,   223,   116,    -1,   116,    -1,   168,
      -1,   156,    -1,   161,    -1,   188,    -1,   191,    -1,    27,
       8,    -1,    22,   136,    72,   223,   116,    -1,    72,   223,
     145,    -1,    16,   223,    31,   223,   216,   145,    -1,    16,
     223,    31,   150,   216,   145,    -1,    16,   223,   216,   145,
      -1,    28,   223,    31,   223,   145,    -1,    28,   223,    31,
     150,   145,    -1,    28,   223,   145,    -1,    28,   150,   145,
      -1,    28,    48,   144,    31,   223,   145,    -1,    29,   223,
      31,   223,   145,    -1,    29,   223,    31,   223,   217,   145,
      -1,    29,   223,    31,   150,   145,    -1,    29,   223,    31,
     150,   217,   145,    -1,    29,   223,   145,    -1,    29,   223,
     217,   145,    -1,   126,   204,    31,   223,   127,   136,    -1,
     126,   204,    31,   223,   217,   127,   136,    -1,   126,   204,
      31,   150,   127,   136,    -1,   126,   204,    31,   150,   217,
     127,   136,    -1,   126,   204,   127,   136,    -1,   126,   204,
     217,   127,   136,    -1,    75,   124,   204,   125,    -1,    30,
     223,    64,   136,    -1,    30,   223,   139,    -1,    30,   223,
      64,   136,    24,   136,    -1,    30,   223,   139,    24,   136,
      -1,    58,   223,   122,   153,   123,    -1,    -1,   153,   154,
      -1,    70,   204,   145,    -1,    46,   136,    -1,    65,    58,
     204,   122,   153,   123,    -1,   157,   158,   144,   159,   122,
     160,   123,    -1,    -1,    27,    -1,    14,    -1,    53,    -1,
      67,    -1,    -1,    96,   204,    -1,    -1,   160,   147,    -1,
     160,   135,   147,    -1,    25,   144,   122,   162,   123,    -1,
     163,    -1,   162,    97,    -1,   162,    97,   163,    -1,   144,
      -1,   144,    78,   223,    -1,    -1,    -1,    37,   165,   175,
     166,   181,   199,   187,   183,    -1,    -1,    33,    -1,    26,
      -1,    26,   144,    -1,    27,    -1,    27,   144,    -1,    -1,
      -1,   167,   180,   169,   171,   170,   181,   199,   187,   182,
      -1,   179,   172,   174,    -1,   179,   173,   174,    -1,   179,
     144,    99,   172,   174,    -1,   179,   144,    99,   173,   174,
      -1,   144,    -1,    93,   144,    -1,    92,    -1,    94,    -1,
      95,    -1,    93,    -1,   102,    -1,   112,    -1,   108,    -1,
     105,    -1,   107,    -1,   104,    -1,   114,    -1,   109,    -1,
     119,    -1,    98,    -1,   117,    -1,   118,    -1,   110,    -1,
     103,    -1,   111,    -1,    13,    -1,   106,    -1,     9,    -1,
     120,    -1,   121,    -1,    78,    -1,    89,    -1,    86,    -1,
      88,    -1,    82,    -1,    87,    -1,    83,    -1,    79,    -1,
      80,    -1,    81,    -1,    91,    -1,    90,    -1,    -1,   124,
     176,   125,    -1,   124,   176,   125,    -1,    -1,   177,    -1,
     176,    97,   177,    -1,   178,   144,   203,   197,    -1,   178,
     144,   203,   186,    -1,   178,   124,   196,   125,   203,   197,
      -1,   178,   124,   196,   125,   203,   186,    -1,    -1,    31,
      -1,    34,    -1,    47,    -1,    18,    -1,    18,    31,    -1,
      18,    55,    -1,    48,    -1,    55,    -1,    65,    -1,    -1,
      48,    -1,    55,    -1,    52,    -1,    35,    -1,    -1,    18,
      -1,    55,    -1,    48,    -1,    65,    -1,   116,    -1,   183,
      -1,   139,    -1,   146,    -1,   115,   144,    -1,   115,    -1,
      -1,   184,    -1,   101,   223,    -1,   101,   185,    -1,    -1,
      71,   223,    -1,    65,   189,   116,    -1,    17,    65,   189,
     116,    -1,    27,    65,   189,   116,    -1,   144,   190,    -1,
     144,   190,    97,   189,    -1,    -1,    78,   211,    -1,    78,
     200,    -1,   192,    48,   193,   116,    -1,   192,    18,   193,
     116,    -1,   192,    55,   193,   116,    -1,   192,    18,    55,
     193,   116,    -1,   192,    69,   193,   116,    -1,    -1,    17,
      -1,    27,    -1,   194,    -1,   193,    97,   194,    -1,   144,
     199,   197,    -1,   144,   198,    76,   223,    -1,   124,   196,
     125,   199,   197,    -1,    66,    -1,   144,    -1,   124,   196,
     125,    -1,   195,    -1,   195,    97,    -1,   195,    97,   196,
      -1,    -1,    78,    44,    -1,    78,   223,    -1,    -1,    96,
     126,   204,   127,    -1,    -1,    96,   211,    -1,    96,   200,
      -1,     1,    -1,   126,   204,   127,   211,    -1,   126,   204,
     127,   200,    -1,   126,   204,    31,   223,   127,   211,    -1,
       1,    -1,    -1,   211,    -1,   184,    -1,   126,   127,   201,
      -1,   126,   204,   127,   201,    -1,   126,   127,   202,    -1,
     126,   204,   127,   202,    -1,   126,   204,    31,   223,   127,
     201,    -1,    -1,    96,   211,    -1,    96,   184,    -1,    96,
      23,    -1,    96,    60,    -1,    96,    63,    -1,    96,   202,
      -1,   223,    -1,   184,    -1,   204,    97,   223,    -1,   204,
      97,   184,    -1,    66,    -1,   223,    -1,   184,    -1,   205,
      97,   205,    -1,   206,    97,   205,    -1,    -1,   208,    -1,
     209,    -1,   208,    97,   209,    -1,   144,    78,   184,    -1,
     144,    78,   223,    -1,   144,    76,   223,    -1,   184,    -1,
     223,    -1,   144,    -1,   214,    -1,   224,    -1,   213,    -1,
     232,    -1,   231,    -1,    60,   223,    -1,    32,   124,   207,
     125,    -1,    23,   124,   207,   125,    -1,    62,   124,   207,
     125,    -1,    61,    62,   124,   207,   125,    -1,    10,   223,
      -1,    63,   223,    -1,    28,   223,    31,   223,    22,   223,
      -1,    28,   223,    31,   150,    22,   223,    -1,    28,   223,
      22,   223,    -1,    28,   223,    31,   223,    22,    30,   223,
      64,   223,    -1,    28,   223,    31,   150,    22,    30,   223,
      64,   223,    -1,    28,   223,    22,    30,   223,    64,   223,
      -1,    29,   223,    31,   223,    22,   223,    -1,    29,   223,
      31,   150,    22,   223,    -1,    29,   223,    22,   223,    -1,
      29,   223,    31,   223,    22,    30,   223,    64,   223,    -1,
      29,   223,    31,   150,    22,    30,   223,    64,   223,    -1,
      29,   223,    22,    30,   223,    64,   223,    -1,   126,   204,
     127,   223,    -1,   126,   204,    31,   223,   127,   223,    -1,
     126,   204,    31,   150,   127,   223,    -1,   126,   204,    31,
     223,   127,    30,   223,    64,   223,    -1,   126,   204,    31,
     150,   127,    30,   223,    64,   223,    -1,    30,   223,    64,
     223,    24,   223,    -1,    43,    -1,   214,    -1,   210,    -1,
     227,    -1,   226,    -1,   164,    -1,   221,    -1,   222,    -1,
     220,   121,   223,    -1,    -1,   217,    -1,    73,   124,   218,
     125,    -1,   219,    -1,   218,    97,   219,    -1,   178,   210,
      -1,   224,    -1,   220,   121,   223,    -1,    42,   223,    -1,
      38,   193,    31,   223,    -1,   229,    -1,   211,    -1,   212,
      -1,   233,    -1,   234,    -1,   164,    -1,   221,    -1,   222,
      -1,   124,   101,   223,   125,    -1,   223,    96,   223,    -1,
     223,   100,   223,    -1,   223,   100,    -1,   100,   223,    -1,
     100,    -1,   210,    -1,   226,    -1,   227,    -1,   228,    -1,
     224,    -1,   164,    -1,   225,   124,   207,   125,    -1,   225,
     126,   207,   127,    -1,    51,   124,   207,   125,    -1,   223,
      99,   144,    -1,   223,    99,    65,    -1,   223,    99,    23,
      -1,   124,   205,   125,    -1,   124,   205,    97,   125,    -1,
     124,   206,   125,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,   122,   204,   123,    -1,   126,   204,   127,    -1,
     126,   230,   127,    -1,   223,    76,   223,    -1,   230,    97,
     223,    76,   223,    -1,   223,   114,   223,    -1,   223,   109,
     223,    -1,   223,   119,   223,    -1,   223,    98,   223,    -1,
     223,   117,   223,    -1,   223,   118,   223,    -1,   223,   110,
     223,    -1,   223,   102,   223,    -1,   223,   112,   223,    -1,
     223,   108,   223,    -1,   223,   105,   223,    -1,   223,   107,
     223,    -1,   223,   104,   223,    -1,   223,    92,   223,    -1,
     223,    94,   223,    -1,   223,    95,   223,    -1,   223,    77,
     223,    -1,   223,   113,   223,    -1,   223,   103,   223,    -1,
     223,    13,   223,    -1,   223,     9,   223,    -1,   223,   106,
     223,    -1,   223,    21,   223,    -1,   114,   223,    -1,   109,
     223,    -1,    40,   223,    -1,    49,   223,    -1,   111,   223,
      -1,    93,   223,    -1,   223,    54,   223,    -1,   223,    54,
     150,    -1,   235,    54,   223,    -1,   235,    54,   150,    -1,
     223,    57,   223,    -1,   223,    57,   150,    -1,   235,    57,
     223,    -1,   235,    57,   150,    -1,   114,    -1,   119,    -1,
      77,    -1,   113,    -1,    92,    -1,    94,    -1,    95,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   399,   399,   404,   405,   411,   412,   417,   418,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   452,   454,   459,   474,   475,
     480,   481,   486,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   508,   509,
     513,   517,   518,   522,   523,   527,   528,   529,   530,   531,
     532,   536,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,   567,   573,
     579,   585,   591,   598,   608,   612,   613,   614,   615,   619,
     624,   625,   629,   631,   636,   643,   648,   649,   653,   658,
     663,   671,   672,   677,   678,   680,   685,   695,   702,   706,
     714,   715,   720,   725,   719,   750,   756,   763,   771,   779,
     787,   798,   804,   797,   832,   836,   841,   845,   853,   854,
     855,   856,   857,   858,   859,   860,   861,   862,   863,   864,
     865,   866,   867,   868,   869,   870,   871,   872,   873,   874,
     875,   876,   877,   878,   882,   883,   884,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   897,   898,   902,   906,
     907,   908,   912,   914,   916,   918,   923,   924,   925,   926,
     927,   928,   929,   930,   931,   932,   936,   937,   938,   942,
     943,   947,   948,   949,   950,   951,   955,   956,   960,   961,
     965,   967,   972,   973,   977,   978,   982,   983,   987,   989,
     991,   996,  1009,  1026,  1027,  1029,  1034,  1041,  1048,  1055,
    1063,  1072,  1073,  1074,  1078,  1079,  1087,  1089,  1095,  1100,
    1102,  1104,  1109,  1111,  1113,  1120,  1121,  1122,  1126,  1127,
    1132,  1133,  1134,  1135,  1155,  1159,  1163,  1171,  1175,  1176,
    1177,  1181,  1183,  1189,  1191,  1193,  1198,  1199,  1200,  1201,
    1202,  1203,  1204,  1210,  1211,  1212,  1213,  1217,  1218,  1219,
    1223,  1224,  1228,  1229,  1233,  1234,  1238,  1239,  1240,  1241,
    1242,  1246,  1257,  1258,  1259,  1260,  1261,  1262,  1264,  1266,
    1268,  1270,  1272,  1274,  1279,  1281,  1283,  1285,  1287,  1289,
    1291,  1293,  1295,  1297,  1299,  1301,  1303,  1310,  1316,  1322,
    1328,  1337,  1342,  1350,  1351,  1352,  1353,  1354,  1355,  1356,
    1357,  1362,  1363,  1367,  1371,  1373,  1381,  1392,  1393,  1398,
    1403,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,
    1421,  1423,  1425,  1427,  1429,  1434,  1435,  1436,  1437,  1448,
    1449,  1453,  1454,  1455,  1459,  1460,  1461,  1469,  1470,  1471,
    1475,  1476,  1477,  1478,  1479,  1480,  1481,  1488,  1489,  1493,
    1494,  1495,  1496,  1497,  1498,  1499,  1500,  1501,  1502,  1503,
    1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,
    1514,  1515,  1519,  1520,  1521,  1522,  1523,  1524,  1528,  1529,
    1530,  1531,  1535,  1536,  1537,  1538,  1543,  1544,  1545,  1546,
    1547,  1548,  1549
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
  "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPROC",
  "TRECORD", "TREDUCE", "TREF", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE",
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
  "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "module", "block_stmt", "stmt_ls", "use_stmt",
  "assignment_stmt", "opt_ident", "ident", "do_stmt", "return_stmt",
  "class_level_stmt", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "select_stmt", "when_stmt_ls",
  "when_stmt", "type_select_stmt", "class_decl_stmt", "opt_extern",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_query_expr", "var_arg_expr", "opt_where_part",
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
     385
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   131,   132,   133,   133,   134,   134,   135,   135,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   137,   137,   138,   139,   139,
     140,   140,   141,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   143,   143,
     144,   145,   145,   146,   146,   147,   147,   147,   147,   147,
     147,   148,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   150,   151,   151,   151,   151,   152,
     153,   153,   154,   154,   155,   156,   157,   157,   158,   158,
     158,   159,   159,   160,   160,   160,   161,   162,   162,   162,
     163,   163,   165,   166,   164,   167,   167,   167,   167,   167,
     167,   169,   170,   168,   171,   171,   171,   171,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   174,   174,   175,   176,
     176,   176,   177,   177,   177,   177,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   179,   179,   179,   180,
     180,   181,   181,   181,   181,   181,   182,   182,   183,   183,
     184,   184,   185,   185,   186,   186,   187,   187,   188,   188,
     188,   189,   189,   190,   190,   190,   191,   191,   191,   191,
     191,   192,   192,   192,   193,   193,   194,   194,   194,   195,
     195,   195,   196,   196,   196,   197,   197,   197,   198,   198,
     199,   199,   199,   199,   200,   200,   200,   200,   201,   201,
     201,   202,   202,   202,   202,   202,   203,   203,   203,   203,
     203,   203,   203,   204,   204,   204,   204,   205,   205,   205,
     206,   206,   207,   207,   208,   208,   209,   209,   209,   209,
     209,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   213,   214,   215,   215,   215,   215,   215,   215,   215,
     215,   216,   216,   217,   218,   218,   219,   220,   220,   221,
     222,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   224,   224,   224,   224,   225,
     225,   226,   226,   226,   227,   227,   227,   228,   228,   228,
     229,   229,   229,   229,   229,   229,   229,   230,   230,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   232,   232,   232,   232,   232,   232,   233,   233,
     233,   233,   234,   234,   234,   234,   235,   235,   235,   235,
     235,   235,   235
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     4,     5,     1,     2,     3,
       1,     2,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     2,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     5,
       0,     2,     3,     2,     6,     7,     0,     1,     1,     1,
       1,     0,     2,     0,     2,     3,     5,     1,     2,     3,
       1,     3,     0,     0,     8,     0,     1,     1,     2,     1,
       2,     0,     0,     9,     3,     3,     5,     5,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     3,     0,
       1,     3,     4,     4,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     0,     1,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     0,     1,     2,     2,     0,     2,     3,     4,
       4,     2,     4,     0,     2,     2,     4,     4,     4,     5,
       4,     0,     1,     1,     1,     3,     3,     4,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     0,     4,
       0,     2,     2,     1,     4,     4,     6,     1,     0,     1,
       1,     3,     4,     3,     4,     6,     0,     2,     2,     2,
       2,     2,     2,     1,     1,     3,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     2,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     4,     3,
       1,     1,     1,     1,     3,     3,     3,     3,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    60,   370,   371,   372,   373,
       0,   331,    58,   331,     0,   232,    58,     0,     0,     0,
       0,   127,   233,     0,     0,     0,     0,   126,     0,   122,
       0,     0,     0,    37,     0,   322,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   418,   420,     0,   421,   422,   354,     0,     0,   419,
     416,    65,   417,     0,     0,     0,     4,     0,     5,     9,
       0,    10,    11,    13,   291,    19,    12,    14,    16,    15,
      17,    18,    67,     0,    68,   346,     0,    66,    69,    70,
       0,   355,   342,   343,   294,   292,     0,     0,   347,   348,
       0,   293,     0,   356,   357,   358,   341,   296,   295,   344,
     345,     0,    21,   302,     0,     0,   332,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,   346,   355,   292,
     347,   348,   331,   293,   356,   357,     0,     0,     0,     0,
     282,     0,   128,    71,     0,   130,     0,     0,     0,     0,
       0,     0,   282,     0,     0,     0,     0,     0,   234,    28,
     404,   339,     0,   405,     7,   282,    63,     0,     0,     0,
      62,    31,     0,   297,     0,   282,    32,   303,     0,   223,
       0,   211,   274,     0,   273,     0,     0,   407,   353,   403,
     406,   402,    38,    40,     0,     0,   277,     0,   279,     0,
       0,   278,     0,   273,     0,     0,     6,     0,   108,   109,
     110,     0,   200,   199,   131,     0,     0,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   352,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   282,   282,     0,     0,   186,    22,
      23,     0,    24,     0,     0,     0,     0,     0,     0,     0,
      25,    26,     0,   291,   289,     0,   283,   284,   290,     0,
       0,     0,     0,    80,     0,     0,    79,     0,     0,    86,
       0,     0,    96,     0,    27,   179,   123,   239,     0,   240,
     242,     0,   253,     0,     0,   245,     0,     0,    29,     0,
      64,   100,    61,    30,   282,     0,     0,     0,   221,   218,
     210,     0,    42,    73,    33,    39,    41,   374,     0,     0,
     367,     0,   369,     0,     0,     0,     0,     0,   376,     8,
       0,   111,   196,     0,     0,     0,     0,     0,   330,   399,
     398,   401,   409,   408,   413,   412,   395,   392,   393,   394,
     350,   382,   366,   365,   364,   351,   386,   397,   391,   389,
     400,   390,   388,   380,   385,   387,   396,   379,   383,   384,
     381,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   411,   410,
     415,   414,   190,   187,   188,   189,   193,   194,   195,     0,
       0,   334,     0,     0,     0,     0,     0,     0,   375,   331,
     331,    76,   219,     0,     0,     0,   299,     0,   120,     0,
     117,   220,     0,     0,     0,   306,     0,     0,     0,   312,
       0,     0,    87,    95,     0,     0,   298,     0,   180,     0,
     201,     0,   243,     0,   257,     0,   252,   342,     0,     0,
     236,   340,   235,   363,     0,     0,   300,   100,     0,   225,
     342,     0,   276,   275,   349,   368,   280,   281,     0,     0,
      92,   316,     0,   377,     0,    35,     0,     0,     0,   197,
     198,   132,     0,     0,   227,   226,   228,   230,    43,    50,
      51,    52,    47,    49,    56,    57,    45,    48,    46,    44,
      54,    53,    55,   361,   362,   191,   192,   336,   186,   333,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      72,   288,   286,   287,   285,     0,   118,   116,     0,    94,
       0,     0,    78,     0,    77,     0,     0,    84,     0,     0,
      82,     0,     0,     0,    98,   186,   178,     0,   266,   202,
     204,   203,   205,     0,   241,   244,     0,   245,     0,   237,
     246,   247,     0,     0,    99,   101,   301,     0,     0,   222,
       0,     0,     0,     0,    93,     0,    36,   112,   113,   201,
     161,   159,   164,   171,   172,   173,   168,   170,   166,   169,
     167,   165,   175,   174,   140,   143,   141,   142,   153,   144,
     157,   149,   147,   160,   148,   146,   151,   156,   158,   145,
     150,   154,   155,   152,   162,   163,   138,   176,   176,   229,
     335,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      74,   121,   119,    81,     0,     0,   305,     0,   304,     0,
       0,   311,    85,     0,   310,    83,    97,   321,   181,     0,
       0,   245,   216,   238,     0,     0,   103,     0,   104,     0,
       0,    90,   318,     0,     0,    88,   317,     0,   378,   231,
       0,   139,     0,   179,   134,   135,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,   315,     0,     0,
     266,   269,   270,   271,     0,   268,   272,   342,   212,   183,
     182,     0,     0,     0,   255,   342,   102,     0,    91,     0,
      89,   233,     0,   105,   231,   114,   216,   138,   176,   176,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,   258,     0,   213,   215,   214,   217,   208,   209,
     124,     0,     0,     0,   115,     0,   136,   137,   177,     0,
       0,     0,     0,     0,     0,   308,   307,   314,   313,   185,
     184,   260,   261,   263,   342,     0,   375,   342,   320,   319,
     206,   133,   207,     0,   262,   264,   258,   265
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   193,    67,   453,    69,    70,    71,   194,
      72,    73,   117,    74,   171,    75,    76,    77,    78,   536,
      79,    80,   474,   585,    81,    82,    83,   211,   498,   689,
      84,   439,   440,   127,   154,   460,    86,    87,   352,   599,
     501,   637,   638,   694,   306,   457,   458,   459,   502,   214,
     573,   791,   760,   182,   755,   719,   722,    88,   180,   328,
      89,    90,   157,   158,   310,   311,   470,   314,   315,   466,
     782,   716,   671,   195,   199,   200,   285,   286,   287,   128,
      92,    93,    94,   129,    96,   115,   116,   420,   421,    97,
     130,   131,   100,   133,   102,   134,   135,   105,   106,   204,
     107,   108,   109,   110,   111
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -688
static const yytype_int16 yypact[] =
{
    -688,    73,  2261,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    3303,    11,   128,    11,  5957,    40,   128,  5957,  3303,   150,
     128,   128,   480,  4999,  5957,  5957,   155,  -688,   128,  -688,
       9,  3303,  5957,  -688,  5957,  -688,  5957,  5957,   273,   159,
    5177,  5957,  5301,  5957,   245,   191,  3303,    64,  5379,  5957,
    5957,  -688,  -688,  5957,  -688,  -688,  1241,  5957,  5957,  -688,
    5957,  -688,  -688,  2559,  4799,  5379,  -688,  3179,  -688,  -688,
     128,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,   103,  -688,   247,    50,  -688,  -688,  -688,
     354,   197,  -688,  -688,  -688,   206,   223,   221,   227,   231,
   10601,  1982,    80,   232,   243,  -688,  -688,  -688,  -688,  -688,
    -688,   107,  -688, 10601,   240,  3303,  -688,   251,  -688,   248,
    5957,  5957,  5957,  5957,  5957,  5379,  5379,   260,  -688,  -688,
    -688,  -688,  8159,   268,  -688,  -688,   128,   265,  8357,   315,
    5379,   271,  -688,  -688,   128,  -688,   128,   275,     1,  7133,
    7099,  7301,  5379,  3303,   276,     8,   365,    48,  -688,  -688,
     625,   304,  7367,   625,  -688,  5379,  -688,  8423,  8027,  3303,
    -688,  -688,  7367, 10601,   280,  5379,  -688, 10601,  5379,   327,
     290,   128,  -688,   162, 10601,  7367,  8489,   242,  1867,   625,
     242,   625,  -688,  -688,  2683,   -81,  -688,  5957,  -688,    66,
      77, 10601,   229,  8555,   -28,   400,  -688,   288,  -688,  -688,
    -688,   128,  -688,  -688,  -688,    34,     9,     9,     9,  -688,
    5957,  5957,  5957,  5957,  5479,  5479,  5957,  5957,  5957,  5957,
    5957,  5957,    69,  1241,  5957,  5957,  5957,  5957,  5957,  5957,
    5957,  5957,  5957,  5957,  5957,  5957,  5957,  5957,  5957,  5957,
    5957,  5957,  5957,  5957,  5957,  5957,  5957,  5957,  5957,  5957,
    5957,  5957,  5957,  5957,  5379,  5379,  5479,  5479,   574,  -688,
    -688,  2807,  -688,  8225,  8291,  8621,    62,  5479,     1,   303,
    -688,  -688,  5957,   102,  -688,   295,   332,  -688, 10601,   128,
     314,   402,  5379,  -688,  3427,  5479,  -688,  3551,  5479,  -688,
       1,  3303,   407,   310,  -688,   343,  -688,  -688,     8,  -688,
     339,   321,  -688,  4543,   363,   362,  5957,     9,  -688,   323,
    -688,  -688,  -688,  -688,  5379,   324,   176,  4621,   350,  -688,
    -688,  5379,  -688,  -688,  -688,  -688,  -688,  -688,  6997,  4899,
    -688,  5077,  -688,  5479,  2435,   325,  5957,  5957,  -688,  -688,
    2931,   357,    -3,     9,   174,   179,   211,   214,  8093,   752,
     752,   279,  -688,   279,  -688,   279,  2311,  1323,   938,  1043,
     304,   242,  -688,  -688,  -688,  1867,  1483,   279,  1671,  1671,
     752,  1671,  1671,  1921,   242,  1483,  1643,  1921,   625,   625,
     242,  8687,  8753,  8819,  8885,  8951,  9017,  9083,  9149,  9215,
    9281,  9347,  9413,  9479,  9545,  9611,   329,   330,  -688,   279,
    -688,   279,   131,  -688,  -688,  -688,  -688,  -688,  -688,   128,
      82,  -688,  6035,  5479,  6113,  5479,  5957,  5479,    58,    11,
    9677,  -688,  -688,  9743,  5957,  5379,  -688,  5379,   372,   189,
    -688,  -688,  5957,    84,  5957, 10601,    27,  7433,  5957, 10601,
      54,  7235,  -688,   432,  9809,  3303,  -688,   172,  -688,    22,
      88,   335,     8,   348,  -688,  5379,  -688,   441,  5957,  5579,
    -688, 10601,  -688,  -688,   -26,   340,  -688,  -688,  5379,  -688,
     219,   128,  -688, 10601,  -688,  -688,  -688,  -688,   -52,  6827,
    -688, 10601,  3303, 10601,  9875,  -688,  3055,  5379,   344,  -688,
    -688,  -688,  6714,   228,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,   574,  -688,
    5957,   456,  9941,  5957,   458, 10007,   355,  1795,     1,     1,
    -688, 10601,  -688, 10601,  -688,  5957,   128,  -688,  7367,  -688,
    7499,  3675,  -688,  3799,  -688,  7565,  3923,  -688,     1,  4047,
    -688,     1,  3303,  5957,  -688,   574,  -688,     8,   388,  -688,
    -688,  -688,  -688,    79,  -688,  -688,  4621,   362,    95, 10601,
    -688, 10601,  3303,  5379,  -688,  -688,  -688,    24,   165,  -688,
    4171,   358,  4295,   366,  -688,  5957,  -688,   392,  -688,    88,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,   128,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,  -688,   397,   374,   374,  -688,
    -688, 10073,  6191,  6269, 10139,  6347,  6425,  6503,  6581,  -688,
    -688, 10601,  -688,  -688,  3303,  5957, 10601,  5957, 10601,  3303,
    5957, 10601,  -688,  5957, 10601,  -688,  -688, 10601,  -688,   377,
    5657,    47,   428,  -688,  5479,  2115,  -688,     7,  -688,  4419,
    5957,  -688, 10601,  3303,  5957,  -688, 10601,  3303, 10601,   915,
      81,  -688,  6714,   343,  -688,  -688,  5957,  5957,  5957,  5957,
    5957,  5957,  5957,  5957,  9809,  7631,  7697,  9809,  7763,  7829,
     388,   150,  5957,  5957,  4699,  -688,  -688,   180,  5379,  -688,
    -688,  5957,    35,  6861,  -688,   649,  -688,  7895,  -688,  7961,
    -688,   534,   128,  -688,  1171,  -688,   428,  -688,   374,   374,
     178, 10205, 10271, 10337, 10403, 10469, 10535,  3303,  3303,  3303,
    3303,    47,  5757,   167,  -688,  -688, 10601, 10601,  -688,  -688,
    -688,  6581,  3303,  3303,  -688,    83,  -688,  -688,  -688,  5957,
    5957,  5957,  5957,  5957,  5957,  9809,  9809,  9809,  9809,  -688,
    -688,  -688,  -688,  -688,   192,  5479,  6659,   874,  9809,  9809,
    -688,  -688,  -688,  6963,  -688,  -688,  5857,  -688
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -688,  -688,  -688,    -1,  -186,   461,  -688,  -688,  1007,   154,
    -688,  -688,   489,  1793,    23,  -687,  -606,  -688,  -688,    85,
    -688,  -688,    29,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,   -35,   548,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -179,  -178,  -572,  -688,  -176,   -47,  -260,  -688,  -688,
     -78,  -688,  -243,   236,  -688,  -226,  -210,  -688,  -126,  -688,
    -688,  -688,  -159,   210,  -688,  -302,  -568,  -688,  -429,  -325,
    -468,  -598,  -181,    12,    56,  -688,  -124,  -688,    93,   322,
    -310,  -688,  -688,   764,  -688,    -8,  -131,  -688,     3,  -688,
     822,   893,   -10,  1119,  -688,  1364,  1497,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -376
static const yytype_int16 yytable[] =
{
     113,    66,   479,   467,   132,   119,   461,   138,   419,   673,
     279,     5,     5,   149,   150,   151,   331,   480,   290,   300,
     582,   114,   160,   169,   161,     5,   162,   163,   303,   169,
     167,   168,   172,   173,   577,   759,   177,     5,   184,   185,
     186,   319,   337,   187,   583,   499,   188,   189,   190,   551,
     191,   325,   500,   184,   201,   203,   354,   355,   356,   357,
     183,     5,     6,     7,     8,     9,   695,     5,   120,   347,
     582,   345,     5,     3,   307,   590,   556,   202,   759,   316,
     312,    19,   312,   735,   114,   212,   121,   122,   123,   353,
      26,    40,   372,   427,   583,    29,    30,   584,    32,   348,
      34,    35,   213,   720,   331,   136,   569,    37,   148,    39,
     113,   273,   274,   275,   177,   184,   203,   208,    43,    44,
      45,   124,   178,   271,   278,   469,   674,   114,   764,   271,
     288,     5,   308,   155,   373,  -250,   570,  -250,   276,    40,
     406,   407,   288,   571,   672,   317,   567,   678,   718,   271,
    -250,    53,  -250,   572,   783,   288,   209,   271,   155,   331,
     575,   266,   525,   339,   267,   288,   766,   767,   184,    58,
     210,   293,   296,   299,   341,   576,   271,   576,   434,   528,
     435,   331,    64,   780,   126,   318,   526,   338,   795,   428,
     326,   340,   331,   336,   503,   323,   674,  -250,   785,   790,
     475,  -250,   342,  -250,   264,   271,   265,   529,   333,   549,
     358,   359,   360,   361,   363,   365,   366,   367,   368,   369,
     370,   371,   675,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   288,   288,   409,   411,  -267,   331,
     343,   736,   331,   223,   331,   669,   467,   430,   419,   565,
    -259,   317,   433,   331,   140,   565,   317,  -267,   332,   152,
     164,  -267,   184,   165,   445,   447,   546,   449,   451,  -259,
     504,   454,   679,  -259,   786,   505,   224,   566,   477,   225,
     198,   431,   114,   768,   443,  -267,   471,   174,   317,   362,
     364,   317,   547,  -324,   288,   175,  -224,  -259,   794,   558,
     561,   483,  -323,   452,    91,   317,   331,   506,   797,   201,
     507,   201,    91,   489,   491,  -224,   493,   494,   230,   219,
      91,   232,   220,  -328,   639,   235,  -186,  -329,  -326,   312,
     724,   408,   410,    91,   724,   589,   344,   591,   593,  -325,
     717,   412,   429,  -327,   268,   725,   312,   270,    91,   725,
     271,  -360,   215,  -360,   413,   230,   284,   414,   232,  -250,
     446,   280,   235,   450,  -360,    91,  -360,   282,   284,    91,
     415,   416,  -359,   289,  -359,   486,  -250,   487,   417,   292,
     305,   284,   216,   232,   324,   327,   329,   349,   418,   217,
     350,   284,   445,   532,   449,   535,   454,   537,   491,   432,
     436,   538,   539,   218,   541,   543,  -250,   288,   488,   437,
     441,   455,   548,   442,   550,   456,   462,    91,   555,   468,
     469,  -248,   784,  -250,   576,  -250,   463,   481,   473,   476,
     545,   787,   492,   497,   523,   203,   562,   524,   579,   581,
     574,   313,  -250,    68,  -250,   586,   598,  -186,   203,   552,
     554,   112,  -251,   557,   560,    91,   784,   578,   642,   139,
     645,  -250,   647,     5,   670,   683,   784,   184,   143,   331,
     588,    91,   159,   687,  -107,   336,   692,  -251,   693,   721,
     284,   284,   710,   734,   496,   137,   587,   176,   531,   597,
     534,   652,  -251,   738,   739,  -129,    91,   740,   668,  -251,
     641,   690,   792,   644,    68,   779,   765,   472,   206,   751,
     544,   640,  -129,  -107,     0,   651,     0,     5,  -251,     0,
       0,   656,     0,   658,     0,   144,   661,  -107,  -107,   664,
      85,     0,     0,   667,     0,     0,     0,  -251,    85,     0,
     284,   649,   650,  -251,     0,     0,    85,   482,     0,  -129,
       0,   653,     0,   184,     0,   198,   269,   198,     0,    85,
     682,   662,   686,     0,   665,   688,  -129,  -107,     0,     0,
       0,     0,   412,    91,    85,   677,     0,     0,     0,   144,
       0,  -107,     0,     0,     0,   413,     0,     0,   414,     0,
       0,    85,     0,     0,   304,    85,    91,     0,     0,    91,
       0,   415,   416,    91,     0,     0,     0,     0,     0,   417,
     322,     0,   656,   658,     0,   661,   664,   682,   686,   418,
       0,     0,     0,     0,   704,   705,   223,   706,     0,   707,
     708,     0,     0,   709,     0,    68,     0,     0,     0,     0,
       0,     0,     0,    85,   723,   491,    91,     0,     0,   491,
     727,   542,    91,   284,   729,     0,     0,     0,     0,   224,
    -254,     0,   225,     0,     0,     0,   704,   741,   742,   707,
     743,   744,   745,   746,     0,     0,     0,     0,     0,     0,
     726,    85,   173,   177,   203,  -254,     0,     0,   756,     0,
       0,   757,     0,     0,     0,     0,     0,    85,     0,     0,
    -254,   230,     0,   231,   232,     0,   753,  -254,   235,     0,
       0,     0,    68,     0,     0,   242,     0,   775,   776,   777,
     778,   527,    85,     0,   248,     0,  -254,     0,     0,     0,
       0,   686,   788,   789,     0,   322,     0,     0,   322,   775,
     776,   777,   778,   788,   789,  -254,    95,     0,     0,     0,
       0,  -254,     0,   223,    95,   793,   491,    91,     0,     0,
       0,     0,    95,     0,     0,     0,   686,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   224,     0,     0,   225,
      95,    68,     0,     0,    91,     0,     0,     0,    91,    85,
       0,     0,     0,     0,    98,     0,     0,    95,     0,   226,
       0,    95,    98,     0,     0,     0,     0,     0,     0,     0,
      98,     0,    85,     0,   227,    85,   228,   229,   230,    85,
     231,   232,   233,    98,   234,   235,   236,   237,     0,   239,
     240,   241,   242,     0,   243,   244,   245,     0,    98,   246,
     247,   248,     0,    91,     0,    91,     0,     0,    91,    95,
       0,    91,     0,     0,    91,    98,     0,     0,     0,    98,
       0,     0,    85,     0,     0,    99,     0,     0,    85,     0,
       0,     0,     0,    99,    91,  -256,   715,     0,     0,     0,
       0,    99,    91,     0,    91,     0,   564,    95,     0,     0,
       0,     0,     0,     0,    99,     0,     0,     0,     0,  -106,
    -256,     0,    15,    95,     0,     0,     0,    98,     0,    99,
      20,    21,   731,     0,     0,  -256,     0,     0,    27,     0,
    -125,     0,  -256,   594,   754,     0,    99,    68,    95,   223,
      99,     0,     0,     0,     0,    38,     0,  -125,  -106,     0,
       0,  -256,     0,     0,     0,    98,    91,     0,     0,     0,
     732,    91,  -106,     0,     0,     0,     0,     0,   781,     0,
    -256,    98,   224,     0,     0,   225,  -256,     0,     0,     0,
       0,     0,     0,    85,     0,    91,     0,     0,    99,    91,
       0,     0,   322,     0,   322,     0,    98,   322,     0,     0,
     322,     0,   781,   666,     0,     0,     0,     0,     0,     0,
     227,    61,   781,   229,   230,    95,   231,   232,   733,     0,
      85,   235,     0,   676,    85,     0,    99,     0,   242,   170,
       0,   681,     0,   685,     0,   246,   247,   248,    95,     0,
       0,    95,    99,     0,   223,    95,     0,     0,     0,    91,
      91,    91,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    91,    91,     0,    99,     0,     0,
       0,     0,     0,    98,     0,     0,     0,   224,     0,    85,
     225,    85,     0,     0,    85,     0,     0,    85,    95,     0,
      85,     0,     0,     0,    95,     0,    98,     0,     0,    98,
       0,   101,     0,    98,     0,     0,   272,     0,     0,   101,
      85,     0,     0,     0,     0,   227,     0,   101,    85,   230,
      85,   231,   232,     0,   728,     0,   235,     0,   730,     0,
     101,     0,     0,   242,     0,   170,   170,   170,   302,     0,
     246,   247,   248,     0,    99,   101,    98,     0,     0,   170,
       0,     0,    98,     0,     0,     0,     0,     0,     0,   170,
       0,     0,   101,     0,     0,  -106,   101,    99,    15,     0,
      99,     0,   170,     0,    99,     0,    20,    21,   731,     0,
       0,     0,    85,     0,    27,     0,  -125,    85,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
       0,   205,     0,  -125,  -106,     0,     0,     0,     0,     0,
       0,    85,     0,     0,   101,    85,   732,    99,  -106,     0,
       0,     0,     0,    99,     5,     6,     7,     8,     9,     0,
       0,   120,     0,     0,     0,     0,    95,     0,     0,     0,
      95,     0,     0,     0,    19,     0,     0,     0,     0,   121,
     122,   123,   101,    26,     0,     0,     0,    98,    29,    30,
       0,    32,     0,    34,    35,   170,     0,    61,   101,     0,
      37,     0,    39,     0,     0,    85,    85,    85,    85,     0,
       0,    43,    44,    45,   124,     0,     0,   170,     0,     0,
      85,    85,     0,   101,    98,    95,     0,    95,    98,     0,
      95,     0,     0,    95,     0,     0,    95,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,     0,     0,     0,   223,     0,    95,     0,    99,     0,
      57,     0,    58,     0,    95,    60,    95,     0,     0,     0,
      62,     0,     0,   125,     0,    64,   103,   126,     0,     0,
       0,     0,     0,    98,   103,    98,     0,   224,    98,     0,
     225,    98,   103,     0,    98,    99,     0,     0,     0,    99,
     101,     0,     0,     0,     0,   103,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
     103,     0,    98,   101,    98,     0,   101,     0,    95,   230,
     101,   231,   232,    95,     0,     0,   235,   103,     0,     0,
       0,   103,     0,   242,     0,     0,     0,     0,     0,     0,
     246,   247,   248,     0,    99,     0,    99,    95,     0,    99,
       0,    95,    99,   170,   170,    99,     0,   170,   170,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,    99,    98,     0,     0,   103,
       0,    98,     0,    99,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,   223,    98,     0,   104,     0,    98,
       0,    95,    95,    95,    95,   104,     0,   103,     0,     0,
       0,     0,     0,     0,     0,     0,    95,    95,   104,     0,
       0,     0,     0,   103,     0,     0,     0,   224,     0,     0,
     225,     0,     0,   104,     0,   170,   170,    99,     0,     0,
       0,     0,    99,     0,     0,   170,     0,   302,   103,     0,
     104,     0,   302,     0,   104,   170,     0,     0,   170,    98,
      98,    98,    98,     0,   101,   227,    99,   228,   229,   230,
      99,   231,   232,   233,    98,    98,   235,   236,   237,     0,
     239,   240,   241,   242,     0,     0,     0,   245,     0,     0,
     246,   247,   248,     0,     0,     0,     0,     0,     0,     0,
       0,   101,   104,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103,     0,     0,     0,     0,
      99,    99,    99,    99,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,     0,    99,    99,     0,   103,     0,
       0,   103,     0,     0,   223,   103,   104,     0,     0,     0,
     101,     0,   101,     0,     0,   101,     0,     0,   101,     0,
       0,   101,     0,     0,   170,     0,     0,     0,     0,     0,
       0,   104,   223,     0,     0,     0,     0,   224,     0,     0,
     225,   101,     0,     0,     0,     0,     0,     0,   103,   101,
       0,   101,   302,   302,   103,   302,   302,     0,     0,     0,
     226,     0,     0,     0,     0,   224,     0,     0,   225,   758,
       0,     0,     0,     0,   302,   227,   302,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,     0,
     239,   240,   241,   242,     0,   243,     0,   245,     0,     0,
     246,   247,   248,   227,     0,   228,   229,   230,   104,   231,
     232,   233,   758,   101,   235,     0,     0,     0,   101,     0,
     241,   242,     0,     0,     0,   245,     0,     0,   246,   247,
     248,   104,     0,     0,   104,     0,     0,     0,   104,     0,
       0,     0,   101,     0,   221,   118,   101,     0,   222,   118,
       0,     0,     0,   141,   142,   145,   223,     0,     0,   103,
       0,   153,     0,   156,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     179,   104,     0,     0,     0,     0,     0,   104,     0,   224,
       0,     0,   225,     0,     0,     0,   103,     0,     0,     0,
     103,     0,     0,   207,     0,     0,   101,   101,   101,   101,
       0,     0,   226,     0,     0,     0,     0,     0,     0,     0,
       0,   101,   101,     0,     0,     0,     0,   227,   223,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   103,     0,   103,     0,     0,
     103,   224,   648,   103,   225,     0,   103,     0,     0,   179,
       0,     0,     0,   283,     0,     0,     0,   179,     0,   291,
       0,     0,   223,     0,     0,   283,   103,     0,   309,     0,
       0,     0,   104,     0,   103,     0,   103,     0,   283,   227,
       0,   228,   229,   230,     0,   231,   232,     0,   283,     0,
     235,     0,     0,     0,   330,   224,   241,   242,   225,     0,
       0,   245,     0,     0,   246,   247,   248,     0,     0,   104,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   351,     0,     0,     0,   156,   156,
     156,   156,     0,   227,     0,   228,   229,   230,   103,   231,
     232,     0,     0,   103,   235,   374,     0,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,     0,   246,   247,
     248,     0,     0,     0,     0,     0,     0,   103,   104,     0,
     104,   103,     0,   104,     0,     0,   104,   283,   283,   104,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,     0,     0,     0,     0,     0,   104,
       0,     0,   438,     0,     0,     0,     0,   104,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   309,   263,  -337,     0,     0,  -359,     0,  -359,     0,
     156,   103,   103,   103,   103,     0,   464,   283,     5,     6,
       7,     8,     9,     0,  -375,   120,   103,   103,  -375,     0,
       0,     0,     0,     0,     0,     0,  -375,     0,    19,     0,
       0,     0,     0,   121,   122,   123,   156,    26,     0,     0,
       0,   104,    29,    30,     0,    32,   104,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,  -375,
       0,     0,  -375,     0,     0,    43,    44,    45,   124,     0,
     104,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,  -249,  -375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -375,    53,  -375,
    -375,  -375,     0,  -375,  -375,  -375,     0,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,    58,  -375,  -375,  -375,
     283,     0,  -375,  -375,  -375,     0,     0,   125,     0,    64,
       0,   478,     0,     0,   104,   104,   104,   104,     0,     0,
       0,     0,   568,     0,     0,   309,     0,     0,     0,   104,
     104,    -2,     4,     0,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,   179,  -106,    13,    14,    15,  -231,
      16,    17,     0,    18,    19,     0,    20,    21,    22,    23,
      24,    25,     0,    26,    27,   636,  -125,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,     0,     0,  -231,
      37,    38,    39,  -125,  -106,     0,  -231,    40,     0,    41,
      42,    43,    44,    45,    46,     0,    47,     0,  -106,    48,
    -231,     0,   223,    49,     0,    50,     0,     0,    51,   438,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
     309,    56,     0,     0,     0,   224,     0,     0,   225,     0,
      57,     0,    58,     0,    59,    60,     0,    61,     0,     0,
      62,     0,     0,    63,     0,    64,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   227,     0,   228,   229,   230,   691,   231,
     232,   233,     0,   234,   235,   236,   237,     0,   239,   240,
     241,   242,     0,   243,     0,   245,     0,     0,   246,   247,
     248,     0,     0,     0,     0,     0,     4,     0,     5,     6,
       7,     8,     9,     0,  -375,    10,    11,    12,  -375,  -106,
      13,    14,    15,  -231,    16,    17,  -375,    18,    19,  -375,
      20,    21,    22,    23,    24,    25,     0,    26,    27,     0,
    -125,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,     0,     0,  -231,    37,   737,    39,  -125,  -106,  -375,
    -231,    40,  -375,    41,    42,    43,    44,    45,    46,     0,
      47,     0,  -106,    48,  -231,     0,     0,    49,     0,    50,
       0,     0,  -375,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,   179,     0,  -375,    53,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,     0,  -375,  -375,  -375,
    -375,  -375,  -375,  -375,  -375,  -375,    58,  -375,  -375,  -375,
       0,    61,  -375,  -375,  -375,     0,     0,    63,  -375,    64,
       4,    65,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,  -106,    13,    14,    15,  -231,    16,    17,
       0,    18,    19,     0,    20,    21,    22,    23,    24,    25,
       0,    26,    27,     0,  -125,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,     0,     0,  -231,    37,    38,
      39,  -125,  -106,     0,  -231,    40,     0,    41,    42,    43,
      44,    45,    46,     0,    47,     0,  -106,    48,  -231,     0,
       0,    49,     0,    50,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      58,     0,    59,    60,   181,    61,     0,     0,    62,     0,
       0,    63,   192,    64,     4,    65,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,  -106,    13,    14,
      15,  -231,    16,    17,     0,    18,    19,     0,    20,    21,
      22,    23,    24,    25,     0,    26,    27,     0,  -125,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,     0,
       0,  -231,    37,    38,    39,  -125,  -106,     0,  -231,    40,
       0,    41,    42,    43,    44,    45,    46,     0,    47,     0,
    -106,    48,  -231,     0,     0,    49,     0,    50,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,   335,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -106,    13,    14,    15,  -231,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,    25,     0,    26,
      27,     0,  -125,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -231,    37,    38,    39,  -125,
    -106,     0,  -231,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -106,    48,  -231,     0,     0,    49,
       0,    50,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    58,     0,
      59,    60,     0,    61,     0,     0,    62,     0,     0,    63,
     192,    64,     4,    65,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,  -106,    13,    14,    15,  -231,
      16,    17,     0,    18,    19,     0,    20,    21,    22,    23,
      24,    25,     0,    26,    27,     0,  -125,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,     0,     0,  -231,
      37,    38,    39,  -125,  -106,     0,  -231,    40,     0,    41,
      42,    43,    44,    45,    46,     0,    47,     0,  -106,    48,
    -231,     0,     0,    49,     0,    50,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    58,     0,    59,    60,     0,    61,     0,     0,
      62,     0,     0,    63,   495,    64,     4,    65,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,  -106,
      13,    14,    15,  -231,    16,    17,     0,    18,    19,     0,
      20,    21,    22,    23,    24,    25,     0,    26,    27,     0,
    -125,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,     0,     0,  -231,    37,    38,    39,  -125,  -106,     0,
    -231,    40,     0,    41,    42,    43,    44,    45,    46,     0,
      47,     0,  -106,    48,  -231,     0,     0,    49,     0,    50,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    58,     0,    59,    60,
       0,    61,     0,     0,    62,     0,     0,    63,   596,    64,
       4,    65,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,  -106,    13,    14,    15,  -231,    16,    17,
       0,    18,    19,     0,    20,    21,    22,    23,    24,    25,
       0,    26,    27,     0,  -125,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,     0,     0,  -231,    37,   205,
      39,  -125,  -106,     0,  -231,    40,     0,    41,    42,    43,
      44,    45,    46,     0,    47,     0,  -106,    48,  -231,     0,
       0,    49,     0,    50,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      58,     0,    59,    60,     0,    61,     0,     0,    62,     0,
       0,    63,     0,    64,     4,    65,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,  -106,    13,    14,
      15,  -231,    16,    17,     0,    18,    19,     0,    20,    21,
      22,    23,    24,    25,     0,    26,    27,     0,  -125,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,     0,
       0,  -231,    37,     0,    39,  -125,  -106,     0,  -231,    40,
       0,    41,    42,    43,    44,    45,    46,     0,    47,     0,
    -106,    48,  -231,     0,     0,    49,     0,    50,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -106,    13,    14,    15,  -231,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,   444,     0,    26,
      27,     0,  -125,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -231,    37,     0,    39,  -125,
    -106,     0,  -231,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -106,    48,  -231,     0,     0,    49,
       0,    50,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    58,     0,
      59,    60,     0,    61,     0,     0,    62,     0,     0,    63,
       0,    64,     4,    65,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,  -106,    13,    14,    15,  -231,
      16,    17,     0,    18,    19,     0,    20,    21,    22,    23,
      24,   448,     0,    26,    27,     0,  -125,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,     0,     0,  -231,
      37,     0,    39,  -125,  -106,     0,  -231,    40,     0,    41,
      42,    43,    44,    45,    46,     0,    47,     0,  -106,    48,
    -231,     0,     0,    49,     0,    50,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    58,     0,    59,    60,     0,    61,     0,     0,
      62,     0,     0,    63,     0,    64,     4,    65,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,  -106,
      13,    14,    15,  -231,    16,    17,     0,    18,    19,     0,
      20,    21,    22,    23,    24,   655,     0,    26,    27,     0,
    -125,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,     0,     0,  -231,    37,     0,    39,  -125,  -106,     0,
    -231,    40,     0,    41,    42,    43,    44,    45,    46,     0,
      47,     0,  -106,    48,  -231,     0,     0,    49,     0,    50,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    58,     0,    59,    60,
       0,    61,     0,     0,    62,     0,     0,    63,     0,    64,
       4,    65,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,  -106,    13,    14,    15,  -231,    16,    17,
       0,    18,    19,     0,    20,    21,    22,    23,    24,   657,
       0,    26,    27,     0,  -125,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,     0,     0,  -231,    37,     0,
      39,  -125,  -106,     0,  -231,    40,     0,    41,    42,    43,
      44,    45,    46,     0,    47,     0,  -106,    48,  -231,     0,
       0,    49,     0,    50,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      58,     0,    59,    60,     0,    61,     0,     0,    62,     0,
       0,    63,     0,    64,     4,    65,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,  -106,    13,    14,
      15,  -231,    16,    17,     0,    18,    19,     0,    20,    21,
      22,    23,    24,   660,     0,    26,    27,     0,  -125,    28,
      29,    30,    31,    32,    33,    34,    35,     0,    36,     0,
       0,  -231,    37,     0,    39,  -125,  -106,     0,  -231,    40,
       0,    41,    42,    43,    44,    45,    46,     0,    47,     0,
    -106,    48,  -231,     0,     0,    49,     0,    50,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -106,    13,    14,    15,  -231,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,   663,     0,    26,
      27,     0,  -125,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -231,    37,     0,    39,  -125,
    -106,     0,  -231,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -106,    48,  -231,     0,     0,    49,
       0,    50,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    58,     0,
      59,    60,     0,    61,     0,     0,    62,     0,     0,    63,
       0,    64,     4,    65,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,  -106,    13,    14,    15,  -231,
      16,    17,     0,    18,    19,     0,    20,    21,    22,    23,
      24,   680,     0,    26,    27,     0,  -125,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    36,     0,     0,  -231,
      37,     0,    39,  -125,  -106,     0,  -231,    40,     0,    41,
      42,    43,    44,    45,    46,     0,    47,     0,  -106,    48,
    -231,     0,     0,    49,     0,    50,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    58,     0,    59,    60,     0,    61,     0,     0,
      62,     0,     0,    63,     0,    64,     4,    65,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,  -106,
      13,    14,    15,  -231,    16,    17,     0,    18,    19,     0,
      20,    21,    22,    23,    24,   684,     0,    26,    27,     0,
    -125,    28,    29,    30,    31,    32,    33,    34,    35,     0,
      36,     0,     0,  -231,    37,     0,    39,  -125,  -106,     0,
    -231,    40,     0,    41,    42,    43,    44,    45,    46,     0,
      47,     0,  -106,    48,  -231,     0,     0,    49,     0,    50,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    58,     0,    59,    60,
       0,    61,     0,     0,    62,     0,     0,    63,     0,    64,
     464,    65,     5,     6,     7,     8,     9,     0,  -375,   120,
       0,     0,  -375,     0,     0,     0,     0,     0,     0,     0,
    -375,     0,    19,     0,     0,     0,     0,   121,   122,   123,
       0,    26,     0,     0,     0,     0,    29,    30,     0,    32,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,  -375,     0,     0,  -375,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -375,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -375,    53,  -375,  -375,  -375,     0,  -375,  -375,  -375,
       0,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,  -375,
      58,  -375,  -375,  -375,     0,     0,  -375,  -375,  -375,     0,
       0,   125,     0,    64,   464,   478,     5,     6,     7,     8,
       9,     0,     0,   120,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,   121,   122,   123,     0,    26,     0,     0,     0,     0,
      29,    30,     0,    32,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    45,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,   464,     0,     5,     6,     7,     8,     9,     0,
       0,   120,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,    19,     0,     0,     0,     0,   121,
     122,   123,    57,    26,    58,     0,    59,    60,    29,    30,
       0,    32,    62,    34,    35,   125,     0,    64,     0,   465,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,    45,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   120,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,    19,     0,     0,     0,     0,   121,   122,   123,
      57,    26,    58,     0,    59,    60,    29,    30,     0,    32,
      62,    34,    35,   125,     0,    64,     0,   478,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     5,     6,     7,     8,     9,     0,    57,   120,
      58,     0,    59,    60,   181,     0,     0,     0,    62,     0,
       0,   125,    19,    64,     0,   126,   752,   121,   122,   123,
       0,    26,     0,     0,     0,     0,    29,    30,     0,    32,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,   196,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
     197,     0,     5,     6,     7,     8,     9,     0,    57,   120,
      58,     0,    59,    60,   181,     0,     0,     0,    62,     0,
       0,   125,    19,    64,     0,   126,     0,   121,   122,   123,
       0,    26,     0,     0,     0,     0,    29,    30,     0,    32,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,   196,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     5,     6,     7,     8,     9,     0,    57,   120,
      58,     0,    59,    60,   181,     0,     0,     0,    62,     0,
       0,   125,    19,    64,   485,   126,     0,   121,   122,   123,
       0,    26,     0,     0,     0,     0,    29,    30,     0,    32,
       0,    34,    35,     0,     0,     0,     0,   146,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,     0,    51,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   120,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
      19,     0,     0,     0,     0,   121,   122,   123,    57,    26,
      58,     0,    59,    60,    29,    30,     0,    32,    62,    34,
      35,   125,     0,    64,     0,   126,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,   196,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       5,     6,     7,     8,     9,     0,    57,   120,    58,     0,
      59,    60,   181,     0,     0,     0,    62,     0,     0,   125,
      19,    64,     0,   126,     0,   121,   122,   123,     0,    26,
       0,     0,     0,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    58,     0,
      59,    60,     0,   166,     0,     0,    62,     0,     0,   125,
       0,    64,     0,   126,     5,     6,     7,     8,     9,     0,
       0,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,    19,     0,     0,     0,     0,   121,
     122,   123,     0,    26,     0,     0,     0,     0,    29,    30,
       0,    32,     0,    34,    35,     0,     0,     0,     0,     0,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,    45,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   120,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,    19,     0,     0,     0,     0,   121,   122,   123,
      57,    26,    58,     0,    59,    60,    29,    30,     0,    32,
      62,    34,    35,    63,     0,    64,     0,   126,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     5,     6,     7,     8,     9,     0,    57,   120,
      58,     0,    59,    60,   181,     0,     0,     0,    62,     0,
       0,   125,    19,    64,     0,   126,     0,   121,   122,   123,
       0,    26,     0,     0,     0,     0,    29,    30,     0,    32,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     5,     6,     7,     8,     9,     0,    57,   120,
      58,     0,    59,    60,     0,     0,     0,     0,    62,     0,
       0,   125,    19,    64,     0,   126,     0,   121,   122,   123,
       0,    26,     0,     0,     0,     0,    29,    30,     0,    32,
       0,    34,    35,   580,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   120,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
     711,     0,     0,     0,     0,   121,   122,   123,    57,    26,
      58,     0,    59,    60,    29,    30,     0,    32,    62,    34,
      35,   125,     0,    64,     0,   126,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,   712,    44,    45,
     713,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       5,     6,     7,     8,     9,     0,    57,   120,    58,     0,
      59,    60,   181,     0,     0,     0,    62,     0,     0,   125,
      19,    64,     0,   714,     0,   121,   122,   123,     0,    26,
       0,     0,     0,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       5,     6,     7,     8,     9,     0,    57,   120,    58,     0,
      59,    60,   181,     0,     0,     0,    62,     0,     0,   125,
      19,    64,     0,   714,     0,   121,   122,   703,     0,    26,
       0,     0,     0,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,     0,     0,
       5,     6,     7,     8,     9,     0,    57,   120,    58,     0,
      59,    60,   181,     0,     0,     0,    62,     0,     0,   125,
      19,    64,     0,   126,     0,   121,   122,   123,     0,    26,
       0,     0,     0,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   120,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,    19,     0,
       0,     0,     0,   121,   122,   530,    57,    26,    58,     0,
      59,    60,    29,    30,     0,    32,    62,    34,    35,   125,
       0,    64,     0,   126,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    45,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   120,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,    19,     0,     0,     0,
       0,   121,   122,   533,    57,    26,    58,     0,    59,    60,
      29,    30,     0,    32,    62,    34,    35,   125,     0,    64,
       0,   126,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    45,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   120,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,    19,     0,     0,     0,     0,   121,
     122,   697,    57,    26,    58,     0,    59,    60,    29,    30,
       0,    32,    62,    34,    35,   125,     0,    64,     0,   126,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,    45,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   120,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,    19,     0,     0,     0,     0,   121,   122,   698,
      57,    26,    58,     0,    59,    60,    29,    30,     0,    32,
      62,    34,    35,   125,     0,    64,     0,   126,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   120,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
      19,     0,     0,     0,     0,   121,   122,   700,    57,    26,
      58,     0,    59,    60,    29,    30,     0,    32,    62,    34,
      35,   125,     0,    64,     0,   126,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   120,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,    19,     0,
       0,     0,     0,   121,   122,   701,    57,    26,    58,     0,
      59,    60,    29,    30,     0,    32,    62,    34,    35,   125,
       0,    64,     0,   126,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    45,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   120,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,    19,     0,     0,     0,
       0,   121,   122,   702,    57,    26,    58,     0,    59,    60,
      29,    30,     0,    32,    62,    34,    35,   125,     0,    64,
       0,   126,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    45,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   120,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,    19,     0,     0,     0,     0,   121,
     122,   703,    57,    26,    58,     0,    59,    60,    29,    30,
       0,    32,    62,    34,    35,   125,     0,    64,     0,   126,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,    45,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   120,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,    19,     0,     0,     0,     0,   121,   122,   123,
      57,    26,    58,     0,    59,    60,    29,    30,     0,    32,
      62,    34,    35,   125,     0,    64,     0,   126,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     5,     0,    43,
      44,    45,   124,   600,     0,     0,     0,   601,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,  -258,     0,     0,     0,
    -258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,     0,     0,   181,     0,     0,     0,     0,     0,
       0,   125,     0,    64,  -258,   714,     0,     0,     0,     0,
       0,     0,   602,   603,   604,   605,   606,   607,     0,     0,
     608,   609,   610,   611,   612,   613,   614,   615,   616,   617,
       0,     0,   618,     0,     0,     0,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,     0,   630,     0,
       0,   631,   632,   633,   634,   635,   221,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,     0,     0,   222,     0,     0,     0,     0,     0,
       0,   224,   223,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,     0,   226,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,     0,   225,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   226,   243,
     244,   245,     0,     0,   246,   247,   248,     0,     0,     0,
       0,     0,     0,   227,   592,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   221,   243,   244,   245,   222,     0,   246,   247,
     248,     0,     0,     0,   223,     0,     0,     0,   761,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,     0,     0,
     222,     0,     0,     0,     0,     0,     0,   224,   223,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,     0,     0,   225,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   226,   243,   244,   245,     0,     0,
     246,   247,   248,     0,     0,     0,     0,     0,     0,   227,
     796,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   221,   243,
     244,   245,   222,     0,   246,   247,   248,     0,     0,     0,
     223,   297,   484,     0,     0,     0,     0,     0,     0,     0,
     298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,     0,     0,   222,     0,     0,     0,
       0,     0,     0,   224,   223,   294,   225,     0,     0,     0,
       0,     0,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,     0,   226,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,     0,     0,
     225,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     226,   243,   244,   245,     0,     0,   246,   247,   248,     0,
       0,   271,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   221,   243,   244,   245,   222,     0,
     246,   247,   248,     0,     0,   271,   223,   559,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,   271,   225,     0,
       0,     0,     0,     0,     0,   301,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,   169,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,   271,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,   553,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,   271,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,   271,   225,     0,     0,     0,
       0,     0,     0,   654,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,   271,   225,     0,     0,     0,     0,     0,     0,   659,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,   271,   225,     0,
       0,     0,     0,     0,     0,   747,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,   271,   225,     0,     0,     0,     0,     0,
       0,   748,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,   271,
     225,     0,     0,     0,     0,     0,     0,   749,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,   271,   225,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,   271,   225,     0,     0,     0,     0,     0,     0,   762,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,   271,   225,     0,
       0,     0,     0,     0,     0,   763,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,   271,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,   321,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
     277,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,  -338,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,   422,     0,     0,
       0,   227,     0,   228,   229,   230,   423,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,   424,     0,     0,     0,   227,     0,   228,
     229,   230,   425,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,   281,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   320,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,   334,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,   346,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,   508,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   509,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,   510,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,   511,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,   512,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,   513,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   514,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,   515,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,   516,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,   517,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,   518,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   519,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,   520,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,   521,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,   522,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,   563,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   540,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,   595,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,   643,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,   646,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,   696,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,   699,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,   769,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,   770,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,   771,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,   772,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,   773,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,   774,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-688))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,     2,   327,   313,    14,    13,   308,    17,   268,   577,
     136,     3,     3,    23,    24,    25,    97,   327,   144,   150,
      46,    73,    32,    22,    34,     3,    36,    37,   152,    22,
      40,    41,    42,    43,   463,   722,    46,     3,    48,    49,
      50,   165,   123,    53,    70,    48,    56,    57,    58,    22,
      60,   175,    55,    63,    64,    65,   215,   216,   217,   218,
      48,     3,     4,     5,     6,     7,   638,     3,    10,    97,
      46,   202,     3,     0,    66,   127,    22,    65,   765,    31,
       1,    23,     1,   689,    73,    35,    28,    29,    30,    55,
      32,    56,    23,    31,    70,    37,    38,   123,    40,   127,
      42,    43,    52,   671,    97,    65,    18,    49,    23,    51,
     120,   121,   122,   123,   124,   125,   126,    14,    60,    61,
      62,    63,    58,   122,   132,    78,    31,    73,   734,   122,
     140,     3,   124,   124,    65,    56,    48,    56,   126,    56,
     264,   265,   152,    55,   573,    97,   124,   123,   101,   122,
      71,    93,    71,    65,   752,   165,    53,   122,   124,    97,
     462,    54,    31,    97,    57,   175,   738,   739,   178,   111,
      67,   148,   149,   150,    97,    96,   122,    96,    76,    97,
      78,    97,   124,   751,   126,   162,    55,   197,   786,   127,
     178,   125,    97,   194,   353,   172,    31,   116,    31,   116,
     324,   122,   125,   122,   124,   122,   126,   125,   185,   125,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   127,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,    78,    97,
      31,   690,    97,    21,    97,   567,   576,   277,   528,    97,
      78,    97,   282,    97,   124,    97,    97,    97,   116,   124,
       7,   101,   292,   124,   294,   295,    97,   297,   298,    97,
     116,   301,   127,   101,   127,   116,    54,   125,   122,    57,
      64,   278,    73,   125,   292,   125,   316,    62,    97,   224,
     225,    97,   123,   116,   324,   124,    97,   125,   786,   450,
     451,   331,   116,   300,     2,    97,    97,   116,   796,   339,
     116,   341,    10,   343,   344,   116,   346,   347,    96,   116,
      18,    99,   121,   116,   116,   103,     3,   116,   116,     1,
     675,   266,   267,    31,   679,   481,   127,   488,   489,   116,
     670,    18,   277,   116,   124,   675,     1,   116,    46,   679,
     122,   124,    18,   126,    31,    96,   140,    34,    99,    31,
     295,   116,   103,   298,   124,    63,   126,    72,   152,    67,
      47,    48,   124,   122,   126,   339,    31,   341,    55,   124,
     124,   165,    48,    99,   124,    78,   116,     7,    65,    55,
     122,   175,   422,   423,   424,   425,   426,   427,   428,   116,
     125,   429,   430,    69,   434,   435,    78,   437,   343,    97,
     116,    24,   442,    31,   444,   125,    97,   115,   448,    76,
      78,    76,   752,    78,    96,    97,   125,    97,   125,   125,
      78,   761,   127,    96,   125,   465,    24,   127,   468,   469,
     125,    96,    97,     2,   116,   125,   122,   124,   478,   446,
     447,    10,    31,   450,   451,   153,   786,   465,    22,    18,
      22,   116,   127,     3,    96,   127,   796,   497,     8,    97,
     478,   169,    31,   127,    14,   496,    99,    56,   124,    71,
     264,   265,   125,   689,   350,    16,   477,    46,   423,   497,
     425,   546,    71,   692,   692,    35,   194,   693,   565,    78,
     530,   599,   765,   533,    63,   751,   736,   317,    67,   710,
     437,   528,    52,    53,    -1,   545,    -1,     3,    97,    -1,
      -1,   551,    -1,   553,    -1,    65,   556,    67,    14,   559,
       2,    -1,    -1,   563,    -1,    -1,    -1,   116,    10,    -1,
     324,   538,   539,   122,    -1,    -1,    18,   331,    -1,    35,
      -1,   548,    -1,   583,    -1,   339,   115,   341,    -1,    31,
     590,   558,   592,    -1,   561,   595,    52,    53,    -1,    -1,
      -1,    -1,    18,   271,    46,   583,    -1,    -1,    -1,    65,
      -1,    67,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    63,    -1,    -1,   153,    67,   294,    -1,    -1,   297,
      -1,    47,    48,   301,    -1,    -1,    -1,    -1,    -1,    55,
     169,    -1,   642,   643,    -1,   645,   646,   647,   648,    65,
      -1,    -1,    -1,    -1,   654,   655,    21,   657,    -1,   659,
     660,    -1,    -1,   663,    -1,   194,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,   674,   675,   344,    -1,    -1,   679,
     680,   435,   350,   437,   684,    -1,    -1,    -1,    -1,    54,
      31,    -1,    57,    -1,    -1,    -1,   696,   697,   698,   699,
     700,   701,   702,   703,    -1,    -1,    -1,    -1,    -1,    -1,
     677,   153,   712,   713,   714,    56,    -1,    -1,   718,    -1,
      -1,   721,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      71,    96,    -1,    98,    99,    -1,   714,    78,   103,    -1,
      -1,    -1,   271,    -1,    -1,   110,    -1,   747,   748,   749,
     750,   419,   194,    -1,   119,    -1,    97,    -1,    -1,    -1,
      -1,   761,   762,   763,    -1,   294,    -1,    -1,   297,   769,
     770,   771,   772,   773,   774,   116,     2,    -1,    -1,    -1,
      -1,   122,    -1,    21,    10,   785,   786,   455,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    -1,   796,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   344,    54,    -1,    -1,    57,
      46,   350,    -1,    -1,   492,    -1,    -1,    -1,   496,   271,
      -1,    -1,    -1,    -1,     2,    -1,    -1,    63,    -1,    77,
      -1,    67,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    -1,   294,    -1,    92,   297,    94,    95,    96,   301,
      98,    99,   100,    31,   102,   103,   104,   105,    -1,   107,
     108,   109,   110,    -1,   112,   113,   114,    -1,    46,   117,
     118,   119,    -1,   551,    -1,   553,    -1,    -1,   556,   115,
      -1,   559,    -1,    -1,   562,    63,    -1,    -1,    -1,    67,
      -1,    -1,   344,    -1,    -1,     2,    -1,    -1,   350,    -1,
      -1,    -1,    -1,    10,   582,    31,   670,    -1,    -1,    -1,
      -1,    18,   590,    -1,   592,    -1,   455,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    14,
      56,    -1,    17,   169,    -1,    -1,    -1,   115,    -1,    46,
      25,    26,    27,    -1,    -1,    71,    -1,    -1,    33,    -1,
      35,    -1,    78,   492,   718,    -1,    63,   496,   194,    21,
      67,    -1,    -1,    -1,    -1,    50,    -1,    52,    53,    -1,
      -1,    97,    -1,    -1,    -1,   153,   654,    -1,    -1,    -1,
      65,   659,    67,    -1,    -1,    -1,    -1,    -1,   752,    -1,
     116,   169,    54,    -1,    -1,    57,   122,    -1,    -1,    -1,
      -1,    -1,    -1,   455,    -1,   683,    -1,    -1,   115,   687,
      -1,    -1,   551,    -1,   553,    -1,   194,   556,    -1,    -1,
     559,    -1,   786,   562,    -1,    -1,    -1,    -1,    -1,    -1,
      92,   116,   796,    95,    96,   271,    98,    99,   123,    -1,
     492,   103,    -1,   582,   496,    -1,   153,    -1,   110,    42,
      -1,   590,    -1,   592,    -1,   117,   118,   119,   294,    -1,
      -1,   297,   169,    -1,    21,   301,    -1,    -1,    -1,   747,
     748,   749,   750,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   762,   763,    -1,   194,    -1,    -1,
      -1,    -1,    -1,   271,    -1,    -1,    -1,    54,    -1,   551,
      57,   553,    -1,    -1,   556,    -1,    -1,   559,   344,    -1,
     562,    -1,    -1,    -1,   350,    -1,   294,    -1,    -1,   297,
      -1,     2,    -1,   301,    -1,    -1,   119,    -1,    -1,    10,
     582,    -1,    -1,    -1,    -1,    92,    -1,    18,   590,    96,
     592,    98,    99,    -1,   683,    -1,   103,    -1,   687,    -1,
      31,    -1,    -1,   110,    -1,   148,   149,   150,   151,    -1,
     117,   118,   119,    -1,   271,    46,   344,    -1,    -1,   162,
      -1,    -1,   350,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    63,    -1,    -1,    14,    67,   294,    17,    -1,
     297,    -1,   185,    -1,   301,    -1,    25,    26,    27,    -1,
      -1,    -1,   654,    -1,    33,    -1,    35,   659,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   455,
      -1,    50,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,   683,    -1,    -1,   115,   687,    65,   344,    67,    -1,
      -1,    -1,    -1,   350,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    -1,   492,    -1,    -1,    -1,
     496,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   153,    32,    -1,    -1,    -1,   455,    37,    38,
      -1,    40,    -1,    42,    43,   278,    -1,   116,   169,    -1,
      49,    -1,    51,    -1,    -1,   747,   748,   749,   750,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,   300,    -1,    -1,
     762,   763,    -1,   194,   492,   551,    -1,   553,   496,    -1,
     556,    -1,    -1,   559,    -1,    -1,   562,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,   582,    -1,   455,    -1,
     109,    -1,   111,    -1,   590,   114,   592,    -1,    -1,    -1,
     119,    -1,    -1,   122,    -1,   124,     2,   126,    -1,    -1,
      -1,    -1,    -1,   551,    10,   553,    -1,    54,   556,    -1,
      57,   559,    18,    -1,   562,   492,    -1,    -1,    -1,   496,
     271,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   582,    -1,    -1,    -1,    -1,    -1,
      46,    -1,   590,   294,   592,    -1,   297,    -1,   654,    96,
     301,    98,    99,   659,    -1,    -1,   103,    63,    -1,    -1,
      -1,    67,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,   118,   119,    -1,   551,    -1,   553,   683,    -1,   556,
      -1,   687,   559,   446,   447,   562,    -1,   450,   451,    -1,
      -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,    -1,   350,
      -1,    -1,    -1,    -1,    -1,   582,   654,    -1,    -1,   115,
      -1,   659,    -1,   590,    -1,   592,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    21,   683,    -1,    10,    -1,   687,
      -1,   747,   748,   749,   750,    18,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   762,   763,    31,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    54,    -1,    -1,
      57,    -1,    -1,    46,    -1,   538,   539,   654,    -1,    -1,
      -1,    -1,   659,    -1,    -1,   548,    -1,   550,   194,    -1,
      63,    -1,   555,    -1,    67,   558,    -1,    -1,   561,   747,
     748,   749,   750,    -1,   455,    92,   683,    94,    95,    96,
     687,    98,    99,   100,   762,   763,   103,   104,   105,    -1,
     107,   108,   109,   110,    -1,    -1,    -1,   114,    -1,    -1,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   492,   115,    -1,    -1,   496,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,    -1,
     747,   748,   749,   750,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,    -1,    -1,   762,   763,    -1,   294,    -1,
      -1,   297,    -1,    -1,    21,   301,   169,    -1,    -1,    -1,
     551,    -1,   553,    -1,    -1,   556,    -1,    -1,   559,    -1,
      -1,   562,    -1,    -1,   677,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    21,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,   582,    -1,    -1,    -1,    -1,    -1,    -1,   344,   590,
      -1,   592,   705,   706,   350,   708,   709,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,   722,
      -1,    -1,    -1,    -1,   727,    92,   729,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,    -1,
     107,   108,   109,   110,    -1,   112,    -1,   114,    -1,    -1,
     117,   118,   119,    92,    -1,    94,    95,    96,   271,    98,
      99,   100,   765,   654,   103,    -1,    -1,    -1,   659,    -1,
     109,   110,    -1,    -1,    -1,   114,    -1,    -1,   117,   118,
     119,   294,    -1,    -1,   297,    -1,    -1,    -1,   301,    -1,
      -1,    -1,   683,    -1,     9,    12,   687,    -1,    13,    16,
      -1,    -1,    -1,    20,    21,    22,    21,    -1,    -1,   455,
      -1,    28,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      47,   344,    -1,    -1,    -1,    -1,    -1,   350,    -1,    54,
      -1,    -1,    57,    -1,    -1,    -1,   492,    -1,    -1,    -1,
     496,    -1,    -1,    70,    -1,    -1,   747,   748,   749,   750,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   762,   763,    -1,    -1,    -1,    -1,    92,    21,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,   551,    -1,   553,    -1,    -1,
     556,    54,   127,   559,    57,    -1,   562,    -1,    -1,   136,
      -1,    -1,    -1,   140,    -1,    -1,    -1,   144,    -1,   146,
      -1,    -1,    21,    -1,    -1,   152,   582,    -1,   155,    -1,
      -1,    -1,   455,    -1,   590,    -1,   592,    -1,   165,    92,
      -1,    94,    95,    96,    -1,    98,    99,    -1,   175,    -1,
     103,    -1,    -1,    -1,   181,    54,   109,   110,    57,    -1,
      -1,   114,    -1,    -1,   117,   118,   119,    -1,    -1,   492,
      -1,    -1,    -1,   496,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   211,    -1,    -1,    -1,   215,   216,
     217,   218,    -1,    92,    -1,    94,    95,    96,   654,    98,
      99,    -1,    -1,   659,   103,   232,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,   683,   551,    -1,
     553,   687,    -1,   556,    -1,    -1,   559,   264,   265,   562,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,   582,
      -1,    -1,   289,    -1,    -1,    -1,    -1,   590,    -1,   592,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   308,   120,   121,    -1,    -1,   124,    -1,   126,    -1,
     317,   747,   748,   749,   750,    -1,     1,   324,     3,     4,
       5,     6,     7,    -1,     9,    10,   762,   763,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   353,    32,    -1,    -1,
      -1,   654,    37,    38,    -1,    40,   659,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    54,
      -1,    -1,    57,    -1,    -1,    60,    61,    62,    63,    -1,
     683,    -1,    -1,    -1,   687,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     437,    -1,   117,   118,   119,    -1,    -1,   122,    -1,   124,
      -1,   126,    -1,    -1,   747,   748,   749,   750,    -1,    -1,
      -1,    -1,   459,    -1,    -1,   462,    -1,    -1,    -1,   762,
     763,     0,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,   481,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,   502,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    -1,    67,    68,
      69,    -1,    21,    72,    -1,    74,    -1,    -1,    77,   546,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
     567,   100,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
     109,    -1,   111,    -1,   113,   114,    -1,   116,    -1,    -1,
     119,    -1,    -1,   122,    -1,   124,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,   615,    98,
      99,   100,    -1,   102,   103,   104,   105,    -1,   107,   108,
     109,   110,    -1,   112,    -1,   114,    -1,    -1,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,   692,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    -1,
      65,    -1,    67,    68,    69,    -1,    -1,    72,    -1,    74,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   731,   732,    -1,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,   116,   117,   118,   119,    -1,    -1,   122,   123,   124,
       1,   126,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    -1,    67,    68,    69,    -1,
      -1,    72,    -1,    74,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
     111,    -1,   113,   114,   115,   116,    -1,    -1,   119,    -1,
      -1,   122,   123,   124,     1,   126,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    -1,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    -1,
      67,    68,    69,    -1,    -1,    72,    -1,    74,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,   111,    -1,   113,   114,    -1,   116,
      -1,    -1,   119,    -1,    -1,   122,   123,   124,     1,   126,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    -1,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,
     113,   114,    -1,   116,    -1,    -1,   119,    -1,    -1,   122,
     123,   124,     1,   126,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    -1,    67,    68,
      69,    -1,    -1,    72,    -1,    74,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   111,    -1,   113,   114,    -1,   116,    -1,    -1,
     119,    -1,    -1,   122,   123,   124,     1,   126,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    -1,    67,    68,    69,    -1,    -1,    72,    -1,    74,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,   113,   114,
      -1,   116,    -1,    -1,   119,    -1,    -1,   122,   123,   124,
       1,   126,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    -1,    67,    68,    69,    -1,
      -1,    72,    -1,    74,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
     111,    -1,   113,   114,    -1,   116,    -1,    -1,   119,    -1,
      -1,   122,    -1,   124,     1,   126,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    -1,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    -1,
      67,    68,    69,    -1,    -1,    72,    -1,    74,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,   111,    -1,   113,   114,    -1,   116,
      -1,    -1,   119,    -1,    -1,   122,    -1,   124,     1,   126,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,
     113,   114,    -1,   116,    -1,    -1,   119,    -1,    -1,   122,
      -1,   124,     1,   126,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    -1,    67,    68,
      69,    -1,    -1,    72,    -1,    74,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   111,    -1,   113,   114,    -1,   116,    -1,    -1,
     119,    -1,    -1,   122,    -1,   124,     1,   126,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    53,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    -1,    67,    68,    69,    -1,    -1,    72,    -1,    74,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,   113,   114,
      -1,   116,    -1,    -1,   119,    -1,    -1,   122,    -1,   124,
       1,   126,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    -1,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    -1,    67,    68,    69,    -1,
      -1,    72,    -1,    74,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
     111,    -1,   113,   114,    -1,   116,    -1,    -1,   119,    -1,
      -1,   122,    -1,   124,     1,   126,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    -1,    55,    56,
      -1,    58,    59,    60,    61,    62,    63,    -1,    65,    -1,
      67,    68,    69,    -1,    -1,    72,    -1,    74,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,   111,    -1,   113,   114,    -1,   116,
      -1,    -1,   119,    -1,    -1,   122,    -1,   124,     1,   126,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    55,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,
     113,   114,    -1,   116,    -1,    -1,   119,    -1,    -1,   122,
      -1,   124,     1,   126,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    -1,    55,    56,    -1,    58,
      59,    60,    61,    62,    63,    -1,    65,    -1,    67,    68,
      69,    -1,    -1,    72,    -1,    74,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,   111,    -1,   113,   114,    -1,   116,    -1,    -1,
     119,    -1,    -1,   122,    -1,   124,     1,   126,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    53,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    -1,
      65,    -1,    67,    68,    69,    -1,    -1,    72,    -1,    74,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,    -1,   113,   114,
      -1,   116,    -1,    -1,   119,    -1,    -1,   122,    -1,   124,
       1,   126,     3,     4,     5,     6,     7,    -1,     9,    10,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,   118,   119,    -1,
      -1,   122,    -1,   124,     1,   126,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,   100,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   109,    32,   111,    -1,   113,   114,    37,    38,
      -1,    40,   119,    42,    43,   122,    -1,   124,    -1,   126,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     109,    32,   111,    -1,   113,   114,    37,    38,    -1,    40,
     119,    42,    43,   122,    -1,   124,    -1,   126,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   109,    10,
     111,    -1,   113,   114,   115,    -1,    -1,    -1,   119,    -1,
      -1,   122,    23,   124,    -1,   126,   127,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
     101,    -1,     3,     4,     5,     6,     7,    -1,   109,    10,
     111,    -1,   113,   114,   115,    -1,    -1,    -1,   119,    -1,
      -1,   122,    23,   124,    -1,   126,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   109,    10,
     111,    -1,   113,   114,   115,    -1,    -1,    -1,   119,    -1,
      -1,   122,    23,   124,   125,   126,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    48,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   109,    32,
     111,    -1,   113,   114,    37,    38,    -1,    40,   119,    42,
      43,   122,    -1,   124,    -1,   126,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   109,    10,   111,    -1,
     113,   114,   115,    -1,    -1,    -1,   119,    -1,    -1,   122,
      23,   124,    -1,   126,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,
     113,   114,    -1,   116,    -1,    -1,   119,    -1,    -1,   122,
      -1,   124,    -1,   126,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     109,    32,   111,    -1,   113,   114,    37,    38,    -1,    40,
     119,    42,    43,   122,    -1,   124,    -1,   126,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   109,    10,
     111,    -1,   113,   114,   115,    -1,    -1,    -1,   119,    -1,
      -1,   122,    23,   124,    -1,   126,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   109,    10,
     111,    -1,   113,   114,    -1,    -1,    -1,    -1,   119,    -1,
      -1,   122,    23,   124,    -1,   126,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   109,    32,
     111,    -1,   113,   114,    37,    38,    -1,    40,   119,    42,
      43,   122,    -1,   124,    -1,   126,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   109,    10,   111,    -1,
     113,   114,   115,    -1,    -1,    -1,   119,    -1,    -1,   122,
      23,   124,    -1,   126,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   109,    10,   111,    -1,
     113,   114,   115,    -1,    -1,    -1,   119,    -1,    -1,   122,
      23,   124,    -1,   126,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   109,    10,   111,    -1,
     113,   114,   115,    -1,    -1,    -1,   119,    -1,    -1,   122,
      23,   124,    -1,   126,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   109,    32,   111,    -1,
     113,   114,    37,    38,    -1,    40,   119,    42,    43,   122,
      -1,   124,    -1,   126,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,   100,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   109,    32,   111,    -1,   113,   114,
      37,    38,    -1,    40,   119,    42,    43,   122,    -1,   124,
      -1,   126,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,   100,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   109,    32,   111,    -1,   113,   114,    37,    38,
      -1,    40,   119,    42,    43,   122,    -1,   124,    -1,   126,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     109,    32,   111,    -1,   113,   114,    37,    38,    -1,    40,
     119,    42,    43,   122,    -1,   124,    -1,   126,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    92,    93,    94,    95,    -1,    -1,    -1,    -1,   100,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   109,    32,
     111,    -1,   113,   114,    37,    38,    -1,    40,   119,    42,
      43,   122,    -1,   124,    -1,   126,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    92,
      93,    94,    95,    -1,    -1,    -1,    -1,   100,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   109,    32,   111,    -1,
     113,   114,    37,    38,    -1,    40,   119,    42,    43,   122,
      -1,   124,    -1,   126,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    92,    93,    94,
      95,    -1,    -1,    -1,    -1,   100,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   109,    32,   111,    -1,   113,   114,
      37,    38,    -1,    40,   119,    42,    43,   122,    -1,   124,
      -1,   126,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    92,    93,    94,    95,    -1,
      -1,    -1,    -1,   100,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   109,    32,   111,    -1,   113,   114,    37,    38,
      -1,    40,   119,    42,    43,   122,    -1,   124,    -1,   126,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    92,    93,    94,    95,    -1,    -1,    -1,
      -1,   100,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     109,    32,   111,    -1,   113,   114,    37,    38,    -1,    40,
     119,    42,    43,   122,    -1,   124,    -1,   126,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    60,
      61,    62,    63,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    97,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    81,    82,    83,    -1,    -1,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    98,    -1,    -1,    -1,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,    -1,
      -1,   117,   118,   119,   120,   121,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    21,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    77,   112,
     113,   114,    -1,    -1,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    92,   127,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,     9,   112,   113,   114,    13,    -1,   117,   118,
     119,    -1,    -1,    -1,    21,    -1,    -1,    -1,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    54,    21,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    -1,    57,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    77,   112,   113,   114,    -1,    -1,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    92,
     127,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,     9,   112,
     113,   114,    13,    -1,   117,   118,   119,    -1,    -1,    -1,
      21,    22,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    21,    22,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      57,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      77,   112,   113,   114,    -1,    -1,   117,   118,   119,    -1,
      -1,   122,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,     9,   112,   113,   114,    13,    -1,
     117,   118,   119,    -1,    -1,   122,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,   122,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,   122,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,   118,   119,    54,    -1,   122,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,   118,   119,    54,    -1,   122,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    54,
      -1,   122,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,   122,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,   122,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,   118,   119,    54,    -1,   122,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,   118,   119,    54,    -1,   122,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    54,
      -1,   122,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,   122,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,   122,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,   118,   119,    54,    -1,   122,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      31,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,   118,   119,    54,   121,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    31,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    31,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    76,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,   116,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,   116,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,   116,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,   116,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    24,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,   116,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    76,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    77,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    77,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    92,
      -1,    94,    95,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,    -1,    -1,   117,   118,   119,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      77,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    92,    -1,    94,    95,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,   105,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,    -1,    -1,
     117,   118,   119,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    77,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    92,    -1,    94,    95,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,   105,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,    -1,    -1,   117,   118,   119,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    77,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    92,    -1,    94,
      95,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
      -1,    -1,   117,   118,   119,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    94,    95,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,    -1,    -1,   117,   118,
     119
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   132,   133,     0,     1,     3,     4,     5,     6,     7,
      10,    11,    12,    15,    16,    17,    19,    20,    22,    23,
      25,    26,    27,    28,    29,    30,    32,    33,    36,    37,
      38,    39,    40,    41,    42,    43,    45,    49,    50,    51,
      56,    58,    59,    60,    61,    62,    63,    65,    68,    72,
      74,    77,    92,    93,    94,    95,   100,   109,   111,   113,
     114,   116,   119,   122,   124,   126,   134,   135,   136,   137,
     138,   139,   141,   142,   144,   146,   147,   148,   149,   151,
     152,   155,   156,   157,   161,   164,   167,   168,   188,   191,
     192,   210,   211,   212,   213,   214,   215,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   231,   232,   233,
     234,   235,   136,   223,    73,   216,   217,   143,   144,   216,
      10,    28,    29,    30,    63,   122,   126,   164,   210,   214,
     221,   222,   223,   224,   226,   227,    65,   143,   223,   136,
     124,   144,   144,     8,    65,   144,    48,    75,   150,   223,
     223,   223,   124,   144,   165,   124,   144,   193,   194,   136,
     223,   223,   223,   223,     7,   124,   116,   223,   223,    22,
     139,   145,   223,   223,    62,   124,   136,   223,    58,   144,
     189,   115,   184,   204,   223,   223,   223,   223,   223,   223,
     223,   223,   123,   134,   140,   204,    66,   101,   184,   205,
     206,   223,   204,   223,   230,    50,   136,   144,    14,    53,
      67,   158,    35,    52,   180,    18,    48,    55,    69,   116,
     121,     9,    13,    21,    54,    57,    77,    92,    94,    95,
      96,    98,    99,   100,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   112,   113,   114,   117,   118,   119,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   120,   124,   126,    54,    57,   124,   136,
     116,   122,   139,   223,   223,   223,   204,    31,   216,   189,
     116,   116,    72,   144,   184,   207,   208,   209,   223,   122,
     189,   144,   124,   145,    22,    31,   145,    22,    31,   145,
     217,    64,   139,   207,   136,   124,   175,    66,   124,   144,
     195,   196,     1,    96,   198,   199,    31,    97,   145,   207,
     116,   122,   136,   145,   124,   207,   204,    78,   190,   116,
     144,    97,   116,   145,   116,   123,   134,   123,   223,    97,
     125,    97,   125,    31,   127,   217,    76,    97,   127,     7,
     122,   144,   169,    55,   193,   193,   193,   193,   223,   223,
     223,   223,   150,   223,   150,   223,   223,   223,   223,   223,
     223,   223,    23,    65,   144,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   207,   207,   150,   223,
     150,   223,    18,    31,    34,    47,    48,    55,    65,   178,
     218,   219,    22,    31,    22,    31,    64,    31,   127,   150,
     223,   145,   116,   223,    76,    78,   125,    97,   144,   162,
     163,   116,    31,   204,    30,   223,   150,   223,    30,   223,
     150,   223,   145,   136,   223,    24,   125,   176,   177,   178,
     166,   196,    97,   125,     1,   126,   200,   211,    76,    78,
     197,   223,   194,   125,   153,   207,   125,   122,   126,   200,
     211,    97,   184,   223,   125,   125,   205,   205,   150,   223,
     136,   223,   127,   223,   223,   123,   140,    96,   159,    48,
      55,   171,   179,   193,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   125,   127,    31,    55,   210,    97,   125,
      30,   150,   223,    30,   150,   223,   150,   223,   216,   216,
     116,   223,   184,   223,   209,    78,    97,   123,   223,   125,
     223,    22,   145,    22,   145,   223,    22,   145,   217,    22,
     145,   217,    24,    24,   136,    97,   125,   124,   144,    18,
      48,    55,    65,   181,   125,   196,    96,   199,   204,   223,
      44,   223,    46,    70,   123,   154,   125,   153,   204,   189,
     127,   217,   127,   217,   136,    76,   123,   204,   122,   170,
       9,    13,    78,    79,    80,    81,    82,    83,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    98,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     114,   117,   118,   119,   120,   121,   144,   172,   173,   116,
     219,   223,    22,    22,   223,    22,    22,   127,   127,   145,
     145,   223,   163,   145,    64,    30,   223,    30,   223,    64,
      30,   223,   145,    30,   223,   145,   136,   223,   177,   196,
      96,   203,   199,   197,    31,   127,   136,   204,   123,   127,
      30,   136,   223,   127,    30,   136,   223,   127,   223,   160,
     181,   144,    99,   124,   174,   174,    64,    30,    30,    64,
      30,    30,    30,    30,   223,   223,   223,   223,   223,   223,
     125,    23,    60,    63,   126,   184,   202,   211,   101,   186,
     197,    71,   187,   223,   200,   211,   145,   223,   136,   223,
     136,    27,    65,   123,   135,   147,   199,   144,   172,   173,
     176,   223,   223,   223,   223,   223,   223,    64,    64,    64,
      64,   203,   127,   204,   184,   185,   223,   223,   139,   146,
     183,   127,    64,    64,   147,   187,   174,   174,   125,    64,
      64,    64,    64,    64,    64,   223,   223,   223,   223,   186,
     197,   184,   201,   202,   211,    31,   127,   211,   223,   223,
     116,   182,   183,   223,   201,   202,   127,   201
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

/* Line 1821 of yacc.c  */
#line 399 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1821 of yacc.c  */
#line 404 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1821 of yacc.c  */
#line 405 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1821 of yacc.c  */
#line 412 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1821 of yacc.c  */
#line 417 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1821 of yacc.c  */
#line 418 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1821 of yacc.c  */
#line 434 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1821 of yacc.c  */
#line 435 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1821 of yacc.c  */
#line 436 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1821 of yacc.c  */
#line 437 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1821 of yacc.c  */
#line 438 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1821 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1821 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1821 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1821 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1821 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1821 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1821 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1821 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1821 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1821 of yacc.c  */
#line 448 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1821 of yacc.c  */
#line 453 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(2) - (4)].pch), new BlockStmt(), yyfilename, (yylsp[(1) - (4)]).comment))); }
    break;

  case 36:

/* Line 1821 of yacc.c  */
#line 455 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pblockstmt), yyfilename, (yylsp[(1) - (5)]).comment))); }
    break;

  case 37:

/* Line 1821 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1821 of yacc.c  */
#line 474 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 39:

/* Line 1821 of yacc.c  */
#line 475 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 40:

/* Line 1821 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 41:

/* Line 1821 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 42:

/* Line 1821 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 43:

/* Line 1821 of yacc.c  */
#line 490 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 44:

/* Line 1821 of yacc.c  */
#line 491 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 45:

/* Line 1821 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 46:

/* Line 1821 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 47:

/* Line 1821 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 48:

/* Line 1821 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 49:

/* Line 1821 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 50:

/* Line 1821 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 51:

/* Line 1821 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 52:

/* Line 1821 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 53:

/* Line 1821 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 54:

/* Line 1821 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 55:

/* Line 1821 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 56:

/* Line 1821 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 57:

/* Line 1821 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 58:

/* Line 1821 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 60:

/* Line 1821 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 61:

/* Line 1821 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 62:

/* Line 1821 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 63:

/* Line 1821 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 64:

/* Line 1821 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 65:

/* Line 1821 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 71:

/* Line 1821 of yacc.c  */
#line 537 "chapel.ypp"
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

  case 72:

/* Line 1821 of yacc.c  */
#line 551 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 73:

/* Line 1821 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 74:

/* Line 1821 of yacc.c  */
#line 553 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 75:

/* Line 1821 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 76:

/* Line 1821 of yacc.c  */
#line 555 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 77:

/* Line 1821 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 78:

/* Line 1821 of yacc.c  */
#line 557 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 79:

/* Line 1821 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 80:

/* Line 1821 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 81:

/* Line 1821 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 82:

/* Line 1821 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 83:

/* Line 1821 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 84:

/* Line 1821 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 85:

/* Line 1821 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 86:

/* Line 1821 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 87:

/* Line 1821 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 88:

/* Line 1821 of yacc.c  */
#line 568 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 89:

/* Line 1821 of yacc.c  */
#line 574 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 90:

/* Line 1821 of yacc.c  */
#line 580 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 91:

/* Line 1821 of yacc.c  */
#line 586 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 92:

/* Line 1821 of yacc.c  */
#line 592 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 93:

/* Line 1821 of yacc.c  */
#line 599 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 94:

/* Line 1821 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 95:

/* Line 1821 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 96:

/* Line 1821 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 97:

/* Line 1821 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 98:

/* Line 1821 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 99:

/* Line 1821 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 100:

/* Line 1821 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 101:

/* Line 1821 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 102:

/* Line 1821 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 103:

/* Line 1821 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 104:

/* Line 1821 of yacc.c  */
#line 637 "chapel.ypp"
    { USR_FATAL((yyvsp[(3) - (6)].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
    break;

  case 105:

/* Line 1821 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(3) - (7)].pch), (yyvsp[(2) - (7)].ptype), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(6) - (7)].pblockstmt), (yyvsp[(1) - (7)].flag), (yylsp[(1) - (7)]).comment)); }
    break;

  case 106:

/* Line 1821 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 107:

/* Line 1821 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 108:

/* Line 1821 of yacc.c  */
#line 653 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 109:

/* Line 1821 of yacc.c  */
#line 658 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 110:

/* Line 1821 of yacc.c  */
#line 663 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 111:

/* Line 1821 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 112:

/* Line 1821 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 113:

/* Line 1821 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 114:

/* Line 1821 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 115:

/* Line 1821 of yacc.c  */
#line 681 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 116:

/* Line 1821 of yacc.c  */
#line 686 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 117:

/* Line 1821 of yacc.c  */
#line 696 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
    }
    break;

  case 118:

/* Line 1821 of yacc.c  */
#line 703 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 119:

/* Line 1821 of yacc.c  */
#line 707 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 120:

/* Line 1821 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 121:

/* Line 1821 of yacc.c  */
#line 715 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 122:

/* Line 1821 of yacc.c  */
#line 720 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 123:

/* Line 1821 of yacc.c  */
#line 725 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 124:

/* Line 1821 of yacc.c  */
#line 730 "chapel.ypp"
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

  case 125:

/* Line 1821 of yacc.c  */
#line 750 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 126:

/* Line 1821 of yacc.c  */
#line 756 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 127:

/* Line 1821 of yacc.c  */
#line 763 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 128:

/* Line 1821 of yacc.c  */
#line 771 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 129:

/* Line 1821 of yacc.c  */
#line 779 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 130:

/* Line 1821 of yacc.c  */
#line 787 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 131:

/* Line 1821 of yacc.c  */
#line 798 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 132:

/* Line 1821 of yacc.c  */
#line 804 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 133:

/* Line 1821 of yacc.c  */
#line 811 "chapel.ypp"
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

  case 134:

/* Line 1821 of yacc.c  */
#line 833 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 135:

/* Line 1821 of yacc.c  */
#line 837 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 136:

/* Line 1821 of yacc.c  */
#line 842 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 137:

/* Line 1821 of yacc.c  */
#line 846 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 139:

/* Line 1821 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 140:

/* Line 1821 of yacc.c  */
#line 855 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 141:

/* Line 1821 of yacc.c  */
#line 856 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 142:

/* Line 1821 of yacc.c  */
#line 857 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 143:

/* Line 1821 of yacc.c  */
#line 858 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 144:

/* Line 1821 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 145:

/* Line 1821 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 146:

/* Line 1821 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 147:

/* Line 1821 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 148:

/* Line 1821 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 149:

/* Line 1821 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 150:

/* Line 1821 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 151:

/* Line 1821 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 152:

/* Line 1821 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 153:

/* Line 1821 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 154:

/* Line 1821 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 155:

/* Line 1821 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 156:

/* Line 1821 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 157:

/* Line 1821 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 158:

/* Line 1821 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 159:

/* Line 1821 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 160:

/* Line 1821 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 161:

/* Line 1821 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 162:

/* Line 1821 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 163:

/* Line 1821 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 164:

/* Line 1821 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 165:

/* Line 1821 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 166:

/* Line 1821 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 167:

/* Line 1821 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 168:

/* Line 1821 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 169:

/* Line 1821 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 170:

/* Line 1821 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 171:

/* Line 1821 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 172:

/* Line 1821 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 173:

/* Line 1821 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 174:

/* Line 1821 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 175:

/* Line 1821 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 176:

/* Line 1821 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 177:

/* Line 1821 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 178:

/* Line 1821 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 179:

/* Line 1821 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 180:

/* Line 1821 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 181:

/* Line 1821 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 182:

/* Line 1821 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 183:

/* Line 1821 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 184:

/* Line 1821 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 185:

/* Line 1821 of yacc.c  */
#line 919 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 186:

/* Line 1821 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 187:

/* Line 1821 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 188:

/* Line 1821 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 189:

/* Line 1821 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 190:

/* Line 1821 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 191:

/* Line 1821 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 192:

/* Line 1821 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 193:

/* Line 1821 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 194:

/* Line 1821 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 195:

/* Line 1821 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 196:

/* Line 1821 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 197:

/* Line 1821 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 198:

/* Line 1821 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 199:

/* Line 1821 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 200:

/* Line 1821 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 201:

/* Line 1821 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 202:

/* Line 1821 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 203:

/* Line 1821 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 204:

/* Line 1821 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 205:

/* Line 1821 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 206:

/* Line 1821 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 209:

/* Line 1821 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 210:

/* Line 1821 of yacc.c  */
#line 966 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 211:

/* Line 1821 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 212:

/* Line 1821 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 214:

/* Line 1821 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 215:

/* Line 1821 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 216:

/* Line 1821 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 217:

/* Line 1821 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 218:

/* Line 1821 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 219:

/* Line 1821 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 220:

/* Line 1821 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 221:

/* Line 1821 of yacc.c  */
#line 997 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 222:

/* Line 1821 of yacc.c  */
#line 1010 "chapel.ypp"
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

  case 223:

/* Line 1821 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 224:

/* Line 1821 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 225:

/* Line 1821 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 226:

/* Line 1821 of yacc.c  */
#line 1035 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 227:

/* Line 1821 of yacc.c  */
#line 1042 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 228:

/* Line 1821 of yacc.c  */
#line 1049 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 229:

/* Line 1821 of yacc.c  */
#line 1056 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
    }
    break;

  case 230:

/* Line 1821 of yacc.c  */
#line 1064 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 231:

/* Line 1821 of yacc.c  */
#line 1072 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 232:

/* Line 1821 of yacc.c  */
#line 1073 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 233:

/* Line 1821 of yacc.c  */
#line 1074 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 235:

/* Line 1821 of yacc.c  */
#line 1080 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 236:

/* Line 1821 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 237:

/* Line 1821 of yacc.c  */
#line 1090 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 238:

/* Line 1821 of yacc.c  */
#line 1096 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 239:

/* Line 1821 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 240:

/* Line 1821 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 241:

/* Line 1821 of yacc.c  */
#line 1105 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 242:

/* Line 1821 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 243:

/* Line 1821 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 244:

/* Line 1821 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 245:

/* Line 1821 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 246:

/* Line 1821 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 247:

/* Line 1821 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 248:

/* Line 1821 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 249:

/* Line 1821 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 250:

/* Line 1821 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 251:

/* Line 1821 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 252:

/* Line 1821 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 253:

/* Line 1821 of yacc.c  */
#line 1135 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 254:

/* Line 1821 of yacc.c  */
#line 1156 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 255:

/* Line 1821 of yacc.c  */
#line 1160 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 256:

/* Line 1821 of yacc.c  */
#line 1164 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 257:

/* Line 1821 of yacc.c  */
#line 1171 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 258:

/* Line 1821 of yacc.c  */
#line 1175 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 259:

/* Line 1821 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 260:

/* Line 1821 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 261:

/* Line 1821 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 262:

/* Line 1821 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 263:

/* Line 1821 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 264:

/* Line 1821 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 265:

/* Line 1821 of yacc.c  */
#line 1194 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 266:

/* Line 1821 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 267:

/* Line 1821 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 268:

/* Line 1821 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 269:

/* Line 1821 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 270:

/* Line 1821 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 271:

/* Line 1821 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 272:

/* Line 1821 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 273:

/* Line 1821 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 274:

/* Line 1821 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 275:

/* Line 1821 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 276:

/* Line 1821 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 277:

/* Line 1821 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 278:

/* Line 1821 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 279:

/* Line 1821 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 280:

/* Line 1821 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 281:

/* Line 1821 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 282:

/* Line 1821 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 284:

/* Line 1821 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 285:

/* Line 1821 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 286:

/* Line 1821 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 287:

/* Line 1821 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 288:

/* Line 1821 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 289:

/* Line 1821 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 291:

/* Line 1821 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 297:

/* Line 1821 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 298:

/* Line 1821 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 299:

/* Line 1821 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 300:

/* Line 1821 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 301:

/* Line 1821 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 302:

/* Line 1821 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 303:

/* Line 1821 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 304:

/* Line 1821 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 305:

/* Line 1821 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 306:

/* Line 1821 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 307:

/* Line 1821 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 308:

/* Line 1821 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 309:

/* Line 1821 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 310:

/* Line 1821 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 311:

/* Line 1821 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 312:

/* Line 1821 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 313:

/* Line 1821 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 314:

/* Line 1821 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 315:

/* Line 1821 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 316:

/* Line 1821 of yacc.c  */
#line 1304 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 317:

/* Line 1821 of yacc.c  */
#line 1311 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 318:

/* Line 1821 of yacc.c  */
#line 1317 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 319:

/* Line 1821 of yacc.c  */
#line 1323 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 320:

/* Line 1821 of yacc.c  */
#line 1329 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 321:

/* Line 1821 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 322:

/* Line 1821 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 330:

/* Line 1821 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:

/* Line 1821 of yacc.c  */
#line 1362 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 333:

/* Line 1821 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 334:

/* Line 1821 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 335:

/* Line 1821 of yacc.c  */
#line 1374 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 336:

/* Line 1821 of yacc.c  */
#line 1382 "chapel.ypp"
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[(1) - (2)].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[(2) - (2)].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = tiMark;
      (yyval.pIntentExpr).second = (yyvsp[(2) - (2)].pexpr);
    }
    break;

  case 338:

/* Line 1821 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:

/* Line 1821 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 340:

/* Line 1821 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 349:

/* Line 1821 of yacc.c  */
#line 1420 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 350:

/* Line 1821 of yacc.c  */
#line 1422 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 351:

/* Line 1821 of yacc.c  */
#line 1424 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 352:

/* Line 1821 of yacc.c  */
#line 1426 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 353:

/* Line 1821 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 354:

/* Line 1821 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 361:

/* Line 1821 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 362:

/* Line 1821 of yacc.c  */
#line 1454 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 363:

/* Line 1821 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 364:

/* Line 1821 of yacc.c  */
#line 1459 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 365:

/* Line 1821 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 366:

/* Line 1821 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 367:

/* Line 1821 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 368:

/* Line 1821 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 369:

/* Line 1821 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 370:

/* Line 1821 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 371:

/* Line 1821 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 372:

/* Line 1821 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 373:

/* Line 1821 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 374:

/* Line 1821 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 375:

/* Line 1821 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 376:

/* Line 1821 of yacc.c  */
#line 1482 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 377:

/* Line 1821 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 378:

/* Line 1821 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 379:

/* Line 1821 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 380:

/* Line 1821 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:

/* Line 1821 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 382:

/* Line 1821 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 383:

/* Line 1821 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:

/* Line 1821 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:

/* Line 1821 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:

/* Line 1821 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1821 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1821 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1821 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1821 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1821 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1821 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1821 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1821 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1821 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1821 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1821 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1821 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1821 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1821 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1821 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1821 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 403:

/* Line 1821 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 404:

/* Line 1821 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 405:

/* Line 1821 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 406:

/* Line 1821 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 407:

/* Line 1821 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 408:

/* Line 1821 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1821 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 410:

/* Line 1821 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 411:

/* Line 1821 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 412:

/* Line 1821 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:

/* Line 1821 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 414:

/* Line 1821 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:

/* Line 1821 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 416:

/* Line 1821 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 417:

/* Line 1821 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 418:

/* Line 1821 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 419:

/* Line 1821 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 420:

/* Line 1821 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 421:

/* Line 1821 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 422:

/* Line 1821 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1821 of yacc.c  */
#line 7405 "bison-chapel.cpp"
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



