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
     TREQUIRE = 313,
     TRETURN = 314,
     TSCAN = 315,
     TSELECT = 316,
     TSERIAL = 317,
     TSINGLE = 318,
     TSPARSE = 319,
     TSUBDOMAIN = 320,
     TSYNC = 321,
     TTHEN = 322,
     TTYPE = 323,
     TUNDERSCORE = 324,
     TUNION = 325,
     TUSE = 326,
     TVAR = 327,
     TWHEN = 328,
     TWHERE = 329,
     TWHILE = 330,
     TWITH = 331,
     TYIELD = 332,
     TZIP = 333,
     TALIAS = 334,
     TAND = 335,
     TASSIGN = 336,
     TASSIGNBAND = 337,
     TASSIGNBOR = 338,
     TASSIGNBXOR = 339,
     TASSIGNDIVIDE = 340,
     TASSIGNEXP = 341,
     TASSIGNLAND = 342,
     TASSIGNLOR = 343,
     TASSIGNMINUS = 344,
     TASSIGNMOD = 345,
     TASSIGNMULTIPLY = 346,
     TASSIGNPLUS = 347,
     TASSIGNSL = 348,
     TASSIGNSR = 349,
     TBAND = 350,
     TBNOT = 351,
     TBOR = 352,
     TBXOR = 353,
     TCOLON = 354,
     TCOMMA = 355,
     TDIVIDE = 356,
     TDOT = 357,
     TDOTDOT = 358,
     TDOTDOTDOT = 359,
     TEQUAL = 360,
     TEXP = 361,
     TGREATER = 362,
     TGREATEREQUAL = 363,
     THASH = 364,
     TLESS = 365,
     TLESSEQUAL = 366,
     TMINUS = 367,
     TMOD = 368,
     TNOT = 369,
     TNOTEQUAL = 370,
     TOR = 371,
     TPLUS = 372,
     TQUESTION = 373,
     TSEMI = 374,
     TSHIFTLEFT = 375,
     TSHIFTRIGHT = 376,
     TSTAR = 377,
     TSWAP = 378,
     TIO = 379,
     TLCBR = 380,
     TRCBR = 381,
     TLP = 382,
     TRP = 383,
     TLSBR = 384,
     TRSBR = 385,
     TNOELSE = 386,
     TUMINUS = 387,
     TUPLUS = 388
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
#line 424 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 430 "bison-chapel.cpp"
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
#line 504 "bison-chapel.cpp"

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
#define YYLAST   10718

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  107
/* YYNRULES -- Number of rules.  */
#define YYNRULES  430
/* YYNRULES -- Number of states.  */
#define YYNSTATES  824

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   388

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
     125,   126,   127,   128,   129,   130,   131,   132,   133
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    48,    51,    55,    59,    63,    67,    71,    75,
      78,    82,    86,    89,    92,    96,    98,   104,   111,   112,
     114,   116,   119,   123,   125,   128,   132,   136,   141,   146,
     151,   156,   161,   166,   171,   176,   181,   186,   191,   196,
     201,   206,   211,   212,   214,   216,   218,   220,   223,   225,
     228,   232,   234,   236,   239,   242,   244,   246,   248,   250,
     252,   255,   261,   265,   272,   279,   284,   290,   296,   300,
     304,   311,   317,   324,   330,   337,   341,   346,   353,   361,
     368,   376,   381,   387,   392,   397,   401,   408,   414,   420,
     421,   424,   428,   431,   438,   445,   454,   456,   458,   460,
     461,   464,   465,   468,   472,   478,   480,   483,   487,   489,
     493,   494,   495,   504,   505,   507,   510,   513,   514,   515,
     525,   529,   533,   539,   545,   547,   550,   552,   554,   556,
     558,   560,   562,   564,   566,   568,   570,   572,   574,   576,
     578,   580,   582,   584,   586,   588,   590,   592,   594,   596,
     598,   600,   602,   604,   606,   608,   610,   612,   614,   616,
     618,   620,   622,   623,   627,   631,   632,   634,   638,   643,
     648,   655,   662,   663,   665,   667,   669,   671,   674,   677,
     679,   681,   683,   684,   686,   688,   690,   692,   694,   695,
     697,   699,   701,   703,   705,   707,   709,   711,   714,   716,
     717,   719,   722,   725,   726,   729,   733,   738,   743,   746,
     751,   752,   755,   758,   763,   768,   773,   779,   784,   785,
     787,   789,   791,   795,   799,   804,   810,   812,   814,   818,
     820,   823,   827,   828,   831,   834,   835,   840,   841,   844,
     847,   849,   854,   859,   866,   868,   869,   871,   873,   877,
     882,   886,   891,   898,   899,   902,   905,   908,   911,   914,
     917,   919,   921,   925,   929,   931,   933,   935,   939,   943,
     944,   946,   948,   952,   956,   960,   964,   966,   968,   970,
     972,   974,   976,   978,   980,   983,   988,   993,   998,  1004,
    1007,  1010,  1017,  1024,  1029,  1039,  1049,  1057,  1064,  1071,
    1076,  1086,  1096,  1104,  1109,  1116,  1123,  1133,  1143,  1150,
    1152,  1154,  1156,  1158,  1160,  1162,  1164,  1166,  1170,  1171,
    1173,  1178,  1180,  1184,  1187,  1191,  1193,  1197,  1200,  1205,
    1207,  1209,  1211,  1213,  1215,  1217,  1219,  1221,  1226,  1230,
    1234,  1237,  1240,  1242,  1244,  1246,  1248,  1250,  1252,  1254,
    1259,  1264,  1269,  1273,  1277,  1281,  1285,  1290,  1294,  1296,
    1298,  1300,  1302,  1306,  1310,  1314,  1318,  1324,  1328,  1332,
    1336,  1340,  1344,  1348,  1352,  1356,  1360,  1364,  1368,  1372,
    1376,  1380,  1384,  1388,  1392,  1396,  1400,  1404,  1408,  1412,
    1416,  1419,  1422,  1425,  1428,  1431,  1434,  1438,  1442,  1446,
    1450,  1454,  1458,  1462,  1466,  1468,  1470,  1472,  1474,  1476,
    1478
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     135,     0,    -1,   136,    -1,    -1,   136,   137,    -1,   139,
      -1,   138,   139,    -1,    50,     7,    -1,   138,    50,     7,
      -1,   140,    -1,   142,    -1,   144,    -1,   145,    -1,   152,
      -1,   146,    -1,   154,    -1,   157,    -1,   155,    -1,   158,
      -1,   161,    -1,   151,    -1,   220,   119,    -1,    10,   139,
      -1,    11,   221,   139,    -1,    12,   147,   119,    -1,    15,
     221,   142,    -1,    19,   147,   119,    -1,    20,   228,   119,
      -1,    36,   148,   139,    -1,    39,   139,    -1,    45,   228,
     150,    -1,    62,   228,   150,    -1,    62,   150,    -1,    66,
     139,    -1,    77,   228,   119,    -1,     1,    -1,   141,    41,
     148,   125,   126,    -1,   141,    41,   148,   125,   143,   126,
      -1,    -1,    54,    -1,    52,    -1,   125,   126,    -1,   125,
     143,   126,    -1,   137,    -1,   143,   137,    -1,    71,   209,
     119,    -1,    58,   209,   119,    -1,   229,    81,   228,   119,
      -1,   229,    92,   228,   119,    -1,   229,    89,   228,   119,
      -1,   229,    91,   228,   119,    -1,   229,    85,   228,   119,
      -1,   229,    90,   228,   119,    -1,   229,    86,   228,   119,
      -1,   229,    82,   228,   119,    -1,   229,    83,   228,   119,
      -1,   229,    84,   228,   119,    -1,   229,    94,   228,   119,
      -1,   229,    93,   228,   119,    -1,   229,   123,   228,   119,
      -1,   229,    87,   228,   119,    -1,   229,    88,   228,   119,
      -1,    -1,   148,    -1,     3,    -1,   147,    -1,     7,    -1,
      22,   139,    -1,   142,    -1,    59,   119,    -1,    59,   228,
     119,    -1,   119,    -1,   153,    -1,    54,   153,    -1,    52,
     153,    -1,   166,    -1,   193,    -1,   162,    -1,   173,    -1,
     196,    -1,    27,     8,    -1,    22,   139,    75,   228,   119,
      -1,    75,   228,   150,    -1,    16,   228,    31,   228,   221,
     150,    -1,    16,   228,    31,   156,   221,   150,    -1,    16,
     228,   221,   150,    -1,    28,   228,    31,   228,   150,    -1,
      28,   228,    31,   156,   150,    -1,    28,   228,   150,    -1,
      28,   156,   150,    -1,    28,    48,   148,    31,   228,   150,
      -1,    29,   228,    31,   228,   150,    -1,    29,   228,    31,
     228,   222,   150,    -1,    29,   228,    31,   156,   150,    -1,
      29,   228,    31,   156,   222,   150,    -1,    29,   228,   150,
      -1,    29,   228,   222,   150,    -1,   129,   209,    31,   228,
     130,   139,    -1,   129,   209,    31,   228,   222,   130,   139,
      -1,   129,   209,    31,   156,   130,   139,    -1,   129,   209,
      31,   156,   222,   130,   139,    -1,   129,   209,   130,   139,
      -1,   129,   209,   222,   130,   139,    -1,    78,   127,   209,
     128,    -1,    30,   228,    67,   139,    -1,    30,   228,   142,
      -1,    30,   228,    67,   139,    24,   139,    -1,    30,   228,
     142,    24,   139,    -1,    61,   228,   125,   159,   126,    -1,
      -1,   159,   160,    -1,    73,   209,   150,    -1,    46,   139,
      -1,    68,    61,   209,   125,   159,   126,    -1,   163,   148,
     164,   125,   165,   126,    -1,    27,   149,   163,   148,   164,
     125,   165,   126,    -1,    14,    -1,    55,    -1,    70,    -1,
      -1,    99,   209,    -1,    -1,   165,   152,    -1,   165,   138,
     152,    -1,    25,   148,   125,   167,   126,    -1,   168,    -1,
     167,   100,    -1,   167,   100,   168,    -1,   148,    -1,   148,
      81,   228,    -1,    -1,    -1,    37,   170,   180,   171,   186,
     204,   192,   188,    -1,    -1,    33,    -1,    26,   149,    -1,
      27,   149,    -1,    -1,    -1,   172,   185,   174,   176,   175,
     186,   204,   192,   187,    -1,   184,   177,   179,    -1,   184,
     178,   179,    -1,   184,   148,   102,   177,   179,    -1,   184,
     148,   102,   178,   179,    -1,   148,    -1,    96,   148,    -1,
      95,    -1,    97,    -1,    98,    -1,    96,    -1,   105,    -1,
     115,    -1,   111,    -1,   108,    -1,   110,    -1,   107,    -1,
     117,    -1,   112,    -1,   122,    -1,   101,    -1,   120,    -1,
     121,    -1,   113,    -1,   106,    -1,   114,    -1,    13,    -1,
     109,    -1,     9,    -1,   123,    -1,   124,    -1,    81,    -1,
      92,    -1,    89,    -1,    91,    -1,    85,    -1,    90,    -1,
      86,    -1,    82,    -1,    83,    -1,    84,    -1,    94,    -1,
      93,    -1,    -1,   127,   181,   128,    -1,   127,   181,   128,
      -1,    -1,   182,    -1,   181,   100,   182,    -1,   183,   148,
     208,   202,    -1,   183,   148,   208,   191,    -1,   183,   127,
     201,   128,   208,   202,    -1,   183,   127,   201,   128,   208,
     191,    -1,    -1,    31,    -1,    34,    -1,    47,    -1,    18,
      -1,    18,    31,    -1,    18,    57,    -1,    48,    -1,    57,
      -1,    68,    -1,    -1,    48,    -1,    57,    -1,    68,    -1,
      53,    -1,    35,    -1,    -1,    18,    -1,    57,    -1,    48,
      -1,    68,    -1,   119,    -1,   188,    -1,   142,    -1,   151,
      -1,   118,   148,    -1,   118,    -1,    -1,   189,    -1,   104,
     228,    -1,   104,   190,    -1,    -1,    74,   228,    -1,    68,
     194,   119,    -1,    17,    68,   194,   119,    -1,    27,    68,
     194,   119,    -1,   148,   195,    -1,   148,   195,   100,   194,
      -1,    -1,    81,   216,    -1,    81,   205,    -1,   197,    48,
     198,   119,    -1,   197,    18,   198,   119,    -1,   197,    57,
     198,   119,    -1,   197,    18,    57,   198,   119,    -1,   197,
      72,   198,   119,    -1,    -1,    17,    -1,    27,    -1,   199,
      -1,   198,   100,   199,    -1,   148,   204,   202,    -1,   148,
     203,    79,   228,    -1,   127,   201,   128,   204,   202,    -1,
      69,    -1,   148,    -1,   127,   201,   128,    -1,   200,    -1,
     200,   100,    -1,   200,   100,   201,    -1,    -1,    81,    44,
      -1,    81,   228,    -1,    -1,    99,   129,   209,   130,    -1,
      -1,    99,   216,    -1,    99,   205,    -1,     1,    -1,   129,
     209,   130,   216,    -1,   129,   209,   130,   205,    -1,   129,
     209,    31,   228,   130,   216,    -1,     1,    -1,    -1,   216,
      -1,   189,    -1,   129,   130,   206,    -1,   129,   209,   130,
     206,    -1,   129,   130,   207,    -1,   129,   209,   130,   207,
      -1,   129,   209,    31,   228,   130,   206,    -1,    -1,    99,
     216,    -1,    99,   189,    -1,    99,    23,    -1,    99,    63,
      -1,    99,    66,    -1,    99,   207,    -1,   228,    -1,   189,
      -1,   209,   100,   228,    -1,   209,   100,   189,    -1,    69,
      -1,   228,    -1,   189,    -1,   210,   100,   210,    -1,   211,
     100,   210,    -1,    -1,   213,    -1,   214,    -1,   213,   100,
     214,    -1,   148,    81,   189,    -1,   148,    81,   228,    -1,
     148,    79,   228,    -1,   189,    -1,   228,    -1,   148,    -1,
     219,    -1,   229,    -1,   218,    -1,   237,    -1,   236,    -1,
      63,   228,    -1,    32,   127,   212,   128,    -1,    23,   127,
     212,   128,    -1,    65,   127,   212,   128,    -1,    64,    65,
     127,   212,   128,    -1,    10,   228,    -1,    66,   228,    -1,
      28,   228,    31,   228,    22,   228,    -1,    28,   228,    31,
     156,    22,   228,    -1,    28,   228,    22,   228,    -1,    28,
     228,    31,   228,    22,    30,   228,    67,   228,    -1,    28,
     228,    31,   156,    22,    30,   228,    67,   228,    -1,    28,
     228,    22,    30,   228,    67,   228,    -1,    29,   228,    31,
     228,    22,   228,    -1,    29,   228,    31,   156,    22,   228,
      -1,    29,   228,    22,   228,    -1,    29,   228,    31,   228,
      22,    30,   228,    67,   228,    -1,    29,   228,    31,   156,
      22,    30,   228,    67,   228,    -1,    29,   228,    22,    30,
     228,    67,   228,    -1,   129,   209,   130,   228,    -1,   129,
     209,    31,   228,   130,   228,    -1,   129,   209,    31,   156,
     130,   228,    -1,   129,   209,    31,   228,   130,    30,   228,
      67,   228,    -1,   129,   209,    31,   156,   130,    30,   228,
      67,   228,    -1,    30,   228,    67,   228,    24,   228,    -1,
      43,    -1,   219,    -1,   215,    -1,   232,    -1,   231,    -1,
     169,    -1,   226,    -1,   227,    -1,   225,   124,   228,    -1,
      -1,   222,    -1,    76,   127,   223,   128,    -1,   224,    -1,
     223,   100,   224,    -1,   183,   215,    -1,   240,    56,   215,
      -1,   229,    -1,   225,   124,   228,    -1,    42,   228,    -1,
      38,   198,    31,   228,    -1,   234,    -1,   216,    -1,   217,
      -1,   238,    -1,   239,    -1,   169,    -1,   226,    -1,   227,
      -1,   127,   104,   228,   128,    -1,   228,    99,   228,    -1,
     228,   103,   228,    -1,   228,   103,    -1,   103,   228,    -1,
     103,    -1,   215,    -1,   231,    -1,   232,    -1,   233,    -1,
     229,    -1,   169,    -1,   230,   127,   212,   128,    -1,   230,
     129,   212,   130,    -1,    51,   127,   212,   128,    -1,   228,
     102,   148,    -1,   228,   102,    68,    -1,   228,   102,    23,
      -1,   127,   210,   128,    -1,   127,   210,   100,   128,    -1,
     127,   211,   128,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,   125,   209,   126,    -1,   129,   209,   130,    -1,
     129,   235,   130,    -1,   228,    79,   228,    -1,   235,   100,
     228,    79,   228,    -1,   228,   117,   228,    -1,   228,   112,
     228,    -1,   228,   122,   228,    -1,   228,   101,   228,    -1,
     228,   120,   228,    -1,   228,   121,   228,    -1,   228,   113,
     228,    -1,   228,   105,   228,    -1,   228,   115,   228,    -1,
     228,   111,   228,    -1,   228,   108,   228,    -1,   228,   110,
     228,    -1,   228,   107,   228,    -1,   228,    95,   228,    -1,
     228,    97,   228,    -1,   228,    98,   228,    -1,   228,    80,
     228,    -1,   228,   116,   228,    -1,   228,   106,   228,    -1,
     228,    13,   228,    -1,   228,     9,   228,    -1,   228,   109,
     228,    -1,   228,    21,   228,    -1,   117,   228,    -1,   112,
     228,    -1,    40,   228,    -1,    49,   228,    -1,   114,   228,
      -1,    96,   228,    -1,   228,    56,   228,    -1,   228,    56,
     156,    -1,   240,    56,   228,    -1,   240,    56,   156,    -1,
     228,    60,   228,    -1,   228,    60,   156,    -1,   240,    60,
     228,    -1,   240,    60,   156,    -1,   117,    -1,   122,    -1,
      80,    -1,   116,    -1,    95,    -1,    97,    -1,    98,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   402,   402,   407,   408,   414,   415,   420,   421,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   456,   458,   463,   464,
     465,   480,   481,   486,   487,   492,   496,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   518,   519,   523,   527,   528,   532,   533,   537,
     538,   542,   543,   544,   545,   546,   547,   548,   552,   553,
     557,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   594,   600,
     606,   612,   619,   629,   633,   634,   635,   636,   640,   645,
     646,   650,   652,   657,   664,   669,   677,   682,   687,   695,
     696,   701,   702,   704,   709,   719,   728,   732,   740,   741,
     746,   751,   745,   776,   782,   789,   797,   808,   814,   807,
     842,   846,   851,   855,   863,   864,   865,   866,   867,   868,
     869,   870,   871,   872,   873,   874,   875,   876,   877,   878,
     879,   880,   881,   882,   883,   884,   885,   886,   887,   888,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   903,   907,   908,   912,   916,   917,   918,   922,   924,
     926,   928,   933,   934,   935,   936,   937,   938,   939,   940,
     941,   942,   946,   947,   948,   949,   953,   954,   958,   959,
     960,   961,   962,   966,   967,   971,   972,   976,   978,   983,
     984,   988,   989,   993,   994,   998,  1000,  1002,  1007,  1020,
    1037,  1038,  1040,  1045,  1052,  1059,  1066,  1074,  1083,  1084,
    1085,  1089,  1090,  1098,  1100,  1106,  1111,  1113,  1115,  1120,
    1122,  1124,  1131,  1132,  1133,  1137,  1138,  1143,  1144,  1145,
    1146,  1166,  1170,  1174,  1182,  1186,  1187,  1188,  1192,  1194,
    1200,  1202,  1204,  1209,  1210,  1211,  1212,  1213,  1214,  1215,
    1221,  1222,  1223,  1224,  1228,  1229,  1230,  1234,  1235,  1239,
    1240,  1244,  1245,  1249,  1250,  1251,  1252,  1253,  1257,  1268,
    1269,  1270,  1271,  1272,  1273,  1275,  1277,  1279,  1281,  1283,
    1285,  1290,  1292,  1294,  1296,  1298,  1300,  1302,  1304,  1306,
    1308,  1310,  1312,  1314,  1321,  1327,  1333,  1339,  1348,  1353,
    1361,  1362,  1363,  1364,  1365,  1366,  1367,  1368,  1373,  1374,
    1378,  1382,  1384,  1392,  1402,  1406,  1407,  1412,  1417,  1425,
    1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1435,  1437,
    1439,  1441,  1443,  1448,  1449,  1450,  1451,  1462,  1463,  1467,
    1468,  1469,  1473,  1474,  1475,  1483,  1484,  1485,  1489,  1490,
    1491,  1492,  1493,  1494,  1495,  1502,  1503,  1507,  1508,  1509,
    1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,
    1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,
    1533,  1534,  1535,  1536,  1537,  1538,  1542,  1543,  1544,  1545,
    1549,  1550,  1551,  1552,  1557,  1558,  1559,  1560,  1561,  1562,
    1563
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
  "TPROC", "TPUBLIC", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE", "TVAR",
  "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
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
  "use_stmt", "require_stmt", "assignment_stmt", "opt_ident", "ident",
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
     385,   386,   387,   388
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   134,   135,   136,   136,   137,   137,   138,   138,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   140,   140,   141,   141,
     141,   142,   142,   143,   143,   144,   145,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   147,   147,   148,   149,   149,   150,   150,   151,
     151,   152,   152,   152,   152,   152,   152,   152,   153,   153,
     154,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   156,   157,   157,   157,   157,   158,   159,
     159,   160,   160,   161,   162,   162,   163,   163,   163,   164,
     164,   165,   165,   165,   166,   167,   167,   167,   168,   168,
     170,   171,   169,   172,   172,   172,   172,   174,   175,   173,
     176,   176,   176,   176,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   179,   179,   180,   181,   181,   181,   182,   182,
     182,   182,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   184,   184,   184,   184,   185,   185,   186,   186,
     186,   186,   186,   187,   187,   188,   188,   189,   189,   190,
     190,   191,   191,   192,   192,   193,   193,   193,   194,   194,
     195,   195,   195,   196,   196,   196,   196,   196,   197,   197,
     197,   198,   198,   199,   199,   199,   200,   200,   200,   201,
     201,   201,   202,   202,   202,   203,   203,   204,   204,   204,
     204,   205,   205,   205,   205,   206,   206,   206,   207,   207,
     207,   207,   207,   208,   208,   208,   208,   208,   208,   208,
     209,   209,   209,   209,   210,   210,   210,   211,   211,   212,
     212,   213,   213,   214,   214,   214,   214,   214,   215,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   218,   219,
     220,   220,   220,   220,   220,   220,   220,   220,   221,   221,
     222,   223,   223,   224,   224,   225,   225,   226,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   229,   229,   229,   229,   230,   230,   231,
     231,   231,   232,   232,   232,   233,   233,   233,   234,   234,
     234,   234,   234,   234,   234,   235,   235,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     237,   237,   237,   237,   237,   237,   238,   238,   238,   238,
     239,   239,   239,   239,   240,   240,   240,   240,   240,   240,
     240
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     3,     1,     5,     6,     0,     1,
       1,     2,     3,     1,     2,     3,     3,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     0,     1,     1,     1,     1,     2,     1,     2,
       3,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       2,     5,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     4,     3,     6,     5,     5,     0,
       2,     3,     2,     6,     6,     8,     1,     1,     1,     0,
       2,     0,     2,     3,     5,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     0,     0,     9,
       3,     3,     5,     5,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     4,
       6,     6,     0,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     0,
       1,     2,     2,     0,     2,     3,     4,     4,     2,     4,
       0,     2,     2,     4,     4,     4,     5,     4,     0,     1,
       1,     1,     3,     3,     4,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     0,     4,     0,     2,     2,
       1,     4,     4,     6,     1,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     2,     2,
       1,     1,     3,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     4,     5,     2,
       2,     6,     6,     4,     9,     9,     7,     6,     6,     4,
       9,     9,     7,     4,     6,     6,     9,     9,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       4,     1,     3,     2,     3,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     3,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     3,     3,     3,     3,     4,     3,     1,     1,
       1,     1,     3,     3,     3,     3,     5,     3,     3,     3,
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
       3,     0,     0,     1,    35,    64,   378,   379,   380,   381,
       0,   338,    62,   116,   338,     0,   239,    62,     0,     0,
       0,     0,    62,    62,     0,     0,     0,     0,   134,     0,
     130,     0,     0,     0,     0,   329,     0,     0,     0,     0,
     238,   238,   117,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   118,     0,     0,     0,   426,   428,     0,   429,
     430,   362,     0,     0,   427,   424,    71,   425,     0,     0,
       0,     4,     0,     5,     9,     0,    10,    11,    12,    14,
     298,    20,    13,    72,    15,    17,    16,    18,    19,    77,
       0,    75,   354,     0,    78,    76,    79,     0,   363,   350,
     351,   301,   299,     0,     0,   355,   356,     0,   300,     0,
     364,   365,   366,   349,   303,   302,   352,   353,     0,    22,
     309,     0,     0,   339,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,   354,   363,   299,   355,   356,   338,
     300,   364,   365,     0,     0,     0,     0,   289,     0,    66,
      65,   135,    80,     0,   136,     0,     0,     0,     0,     0,
       0,   289,     0,     0,     0,     0,     0,   241,    29,   412,
     347,     0,   413,     7,   289,   239,   240,    74,    73,   218,
     281,     0,   280,    69,     0,     0,     0,    68,    32,     0,
     304,     0,   289,    33,   310,     0,   230,     0,     0,     0,
       0,   415,   361,   411,   414,   410,    41,    43,     0,     0,
     284,     0,   286,     0,     0,   285,     0,   280,     0,     0,
       6,     0,   119,   207,   206,   137,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   360,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,   289,     0,     0,   192,
      23,    24,     0,    25,     0,     0,     0,     0,     0,     0,
       0,    26,    27,     0,   298,   296,     0,   290,   291,   297,
       0,     0,     0,     0,     0,    89,     0,     0,    88,     0,
       0,    95,     0,     0,   105,     0,    28,   185,   131,   246,
       0,   247,   249,     0,   260,     0,     0,   252,     0,     0,
      30,     0,   136,   217,     0,    46,    70,   109,    67,    31,
     289,     0,     0,     0,   228,   225,    45,    82,    34,    42,
      44,   382,     0,     0,   375,     0,   377,     0,     0,     0,
       0,     0,   384,     8,     0,     0,     0,   202,     0,     0,
       0,     0,     0,   337,   407,   406,   409,   417,   416,   421,
     420,   403,   400,   401,   402,   358,   390,   374,   373,   372,
     359,   394,   405,   399,   397,   408,   398,   396,   388,   393,
     395,   404,   387,   391,   392,   389,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   419,   418,   423,   422,   196,   193,   194,
     195,   199,   200,   201,   424,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,   383,   338,   338,    85,   226,
       0,     0,     0,   306,     0,   128,     0,   125,   227,   119,
       0,     0,     0,   313,     0,     0,     0,   319,     0,     0,
      96,   104,     0,     0,   305,     0,   186,     0,   208,     0,
     250,     0,   264,     0,   259,   350,     0,     0,   243,   348,
     242,   371,   283,   282,     0,     0,   307,   109,     0,   232,
     350,     0,   357,   376,   287,   288,     0,     0,   101,   323,
       0,   385,     0,     0,   120,   121,   203,   204,   205,   138,
       0,     0,   234,   233,   235,   237,    47,    54,    55,    56,
      51,    53,    60,    61,    49,    52,    50,    48,    58,    57,
      59,   369,   370,   197,   198,   343,   192,   340,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
     295,   293,   294,   292,     0,   126,   124,     0,     0,   103,
       0,     0,    87,     0,    86,     0,     0,    93,     0,     0,
      91,     0,     0,     0,   107,   192,   184,     0,   273,   209,
     211,   210,   212,     0,   248,   251,     0,   252,     0,   244,
     253,   254,     0,     0,   108,   110,   308,     0,     0,   229,
       0,     0,     0,     0,   102,     0,    36,     0,   238,   208,
     167,   165,   170,   177,   178,   179,   174,   176,   172,   175,
     173,   171,   181,   180,   146,   149,   147,   148,   159,   150,
     163,   155,   153,   166,   154,   152,   157,   162,   164,   151,
     156,   160,   161,   158,   168,   169,   144,   182,   182,   236,
     342,   344,     0,     0,     0,     0,     0,     0,     0,     0,
      84,    83,   129,   127,   121,    90,     0,     0,   312,     0,
     311,     0,     0,   318,    94,     0,   317,    92,   106,   328,
     187,     0,     0,   252,   223,   245,     0,     0,   112,     0,
     113,     0,     0,    99,   325,     0,     0,    97,   324,     0,
     386,    37,    62,   238,   238,     0,   114,   238,   122,     0,
     145,     0,   185,   140,   141,     0,     0,     0,     0,     0,
       0,     0,     0,   238,   316,     0,     0,   322,     0,     0,
     273,   276,   277,   278,     0,   275,   279,   350,   219,   189,
     188,     0,     0,     0,   262,   350,   111,     0,   100,     0,
      98,   123,   223,   144,   182,   182,     0,     0,     0,     0,
       0,     0,     0,   115,     0,     0,     0,     0,   252,   265,
       0,   220,   222,   221,   224,   215,   216,   132,     0,     0,
       0,     0,   142,   143,   183,     0,     0,     0,     0,     0,
       0,   315,   314,   321,   320,   191,   190,   267,   268,   270,
     350,     0,   383,   350,   327,   326,   213,   139,   214,     0,
     269,   271,   265,   272
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    72,   471,    74,    75,    76,   208,
      77,    78,    79,   150,    80,   154,   188,    81,    82,    83,
      84,    85,   555,    86,    87,   494,   605,    88,    89,    90,
     366,   618,    91,   456,   457,   134,   163,   478,    93,    94,
     367,   619,   519,   657,   658,   723,   318,   475,   476,   477,
     520,   225,   593,   817,   787,   180,   782,   749,   752,    95,
     197,   344,    96,    97,   166,   167,   322,   323,   488,   326,
     327,   484,   808,   746,   693,   209,   213,   214,   296,   297,
     298,   135,    99,   100,   101,   136,   103,   122,   123,   436,
     437,   104,   137,   138,   107,   140,   109,   141,   142,   112,
     113,   218,   114,   115,   116,   117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -711
static const yytype_int16 yypact[] =
{
    -711,    91,  2442,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    3465,   120,   109,  -711,   120,  6113,   140,   109,  6113,  3465,
      92,   109,   388,   476,  5125,  6113,  6113,   146,  -711,   109,
    -711,    16,  3465,  6113,  6113,  -711,  6113,  6113,   209,   149,
     422,   487,  -711,  5309,  5412,  6113,  5539,  6113,   228,   211,
    3465,    26,  -711,  5309,  6113,  6113,  -711,  -711,  6113,  -711,
    -711,  6842,  6113,  6113,  -711,  6113,  -711,  -711,  2703,  4919,
    5309,  -711,  3338,  -711,  -711,   300,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
     109,  -711,   290,    94,  -711,  -711,  -711,   121,   230,  -711,
    -711,  -711,   239,   263,   273,   292,   309, 10596,  1723,   237,
     318,   326,  -711,  -711,  -711,  -711,  -711,  -711,   364,  -711,
   10596,   319,  3465,  -711,   332,  -711,   334,  6113,  6113,  6113,
    6113,  6113,  5309,  5309,   311,  -711,  -711,  -711,  -711,  8374,
     341,  -711,  -711,   109,   350,  8564,   397,  5309,   360,  -711,
    -711,  -711,  -711,   109,   136,   109,   368,    35,  7429,  7324,
    7560,  5309,  3465,   369,    17,    80,    40,  -711,  -711,   404,
     321,  7623,   404,  -711,  5309,  -711,   180,  -711,  -711,   109,
    -711,   -25, 10596,  -711,  8622,  8253,  3465,  -711,  -711,  7623,
   10596,   372,  5309,  -711, 10596,  5309,   426,   390,   186,  7623,
    8680,   286,  1553,   404,   286,   404,  -711,  -711,  2830,   172,
    -711,  6113,  -711,   -24,    76, 10596,     8,  8738,   -17,   504,
    -711,   109,   417,  -711,  -711,  -711,    43,    16,    16,    16,
    -711,  6113,  6113,  6113,  6113,  5620,  5620,  6113,  6113,  6113,
    6113,  6113,  6113,    74,  6842,  6113,  6113,  6113,  6113,  6113,
    6113,  6113,  6113,  6113,  6113,  6113,  6113,  6113,  6113,  6113,
    6113,  6113,  6113,  6113,  6113,  6113,  6113,  6113,  6113,  6113,
    6113,  6113,  6113,  6113,  6113,  5309,  5309,  5620,  5620,   870,
    -711,  -711,  2957,  -711,  8443,  8506,  8801,    36,  5620,    35,
     400,  -711,  -711,  6113,   399,  -711,   393,   430,  -711, 10596,
     109,   415,   109,   507,  5309,  -711,  3592,  5620,  -711,  3719,
    5620,  -711,    35,  3465,   519,   421,  -711,    67,  -711,  -711,
      17,  -711,   445,   425,  -711,  2282,   471,   474,  6113,    16,
    -711,   428,  -711,  -711,  5309,  -711,  -711,  -711,  -711,  -711,
    5309,   429,   191,  4735,   458,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  7261,  5022,  -711,  5206,  -711,  5620,  2576,   432,
    6113,  6113,  -711,  -711,   435,  5309,   439,   265,    16,   221,
     225,   252,   294,  8311,  2020,  2020,   359,  -711,   359,  -711,
     359,  1882,  1065,  1225,  1370,   321,   286,  -711,  -711,  -711,
    1553,  1265,   359,  1136,  1136,  2020,  1136,  1136,   875,   286,
    1265, 10567,   875,   404,   404,   286,  8864,  8922,  8980,  9038,
    9096,  9154,  9212,  9270,  9328,  9386,  9444,  9502,  9560,  9618,
    9676,   437,   440,  -711,   359,  -711,   359,   143,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,   109,    81,  -711,   516,  6194,
    5620,  6275,  5620,  6113,  5620,  1529,   120,  9734,  -711,  -711,
    9792,  6113,  5309,  -711,  5309,   493,   208,  -711,  -711,   417,
    6113,    89,  6113, 10596,    52,  7686,  6113, 10596,    50,  7492,
    -711,   551,  9850,  3465,  -711,   174,  -711,    22,   271,   448,
      17,   280,  -711,  5309,  -711,   362,  6113,  5723,  -711, 10596,
    -711,  -711,  -711, 10596,    19,   450,  -711,  -711,  5309,  -711,
     296,   109,  -711,  -711,  -711,  -711,     4,  7088,  -711, 10596,
    3465, 10596,  9908,  3084,   482,  -711,  -711,  -711,  -711,  -711,
    6972,   308,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,   870,  -711,   109,  6113,
     561,  9971,  6113,   562, 10034,   455,  1951,    35,    35,  -711,
   10596,  -711, 10596,  -711,  6113,   109,  -711,   463,  7623,  -711,
    7749,  3846,  -711,  3973,  -711,  7812,  4100,  -711,    35,  4227,
    -711,    35,  3465,  6113,  -711,   484,  -711,    17,   491,  -711,
    -711,  -711,  -711,    31,  -711,  -711,  4735,   474,    68, 10596,
    -711, 10596,  3465,  5309,  -711,  -711,  -711,    79,   171,  -711,
    4354,   462,  4481,   466,  -711,  6113,  -711,  3211,   348,   271,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,   109,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,   495,   485,   485,  -711,
    -711,  -711, 10092,  6356,  6437, 10155,  6518,  6599,  6680,  6761,
    -711,  -711, 10596,  -711,  -711,  -711,  3465,  6113, 10596,  6113,
   10596,  3465,  6113, 10596,  -711,  6113, 10596,  -711,  -711, 10596,
    -711,   473,  5804,   114,   529,  -711,  5620,  2148,  -711,     6,
    -711,  4608,  6113,  -711, 10596,  3465,  6113,  -711, 10596,  3465,
   10596,  -711,   479,   351,   351,   109,  -711,   554,  -711,    29,
    -711,  6972,    67,  -711,  -711,  6113,  6113,  6113,  6113,  6113,
    6113,  6113,  6113,   998,  9850,  7875,  7938,  9850,  8001,  8064,
     491,    92,  6113,  6113,  4816,  -711,  -711,    84,  5309,  -711,
    -711,  6113,    30,  7122,  -711,   604,  -711,  8127,  -711,  8190,
    -711,  -711,   529,  -711,   485,   485,   192, 10218, 10281, 10344,
   10407, 10470, 10533,  -711,  3465,  3465,  3465,  3465,   114,  5907,
     182,  -711,  -711, 10596, 10596,  -711,  -711,  -711,  6761,  3465,
    3465,    -9,  -711,  -711,  -711,  6113,  6113,  6113,  6113,  6113,
    6113,  9850,  9850,  9850,  9850,  -711,  -711,  -711,  -711,  -711,
      86,  5620,  6923,   672,  9850,  9850,  -711,  -711,  -711,  7156,
    -711,  -711,  6010,  -711
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -711,  -711,  -711,    -1,  -571,  1727,  -711,  -711,  1119,    98,
    -711,  -711,  -711,   298,  1863,    -5,   461,  -698,  -576,   -34,
    -711,  -711,    49,  -711,  -711,   117,  -711,  -711,  -711,   467,
     156,   -51,  -711,  -711,    60,   285,  -711,  -711,  -711,  -711,
    -711,  -711,  -711,   -95,   -92,  -592,  -711,   -91,    45,  -269,
    -711,  -711,    14,  -711,  -157,   -65,  -711,  -142,  -125,  -711,
    -141,  -711,  -711,  -711,  -165,   310,  -711,  -317,  -586,  -711,
    -440,  -334,  -400,  -710,  -102,    25,   145,  -711,  -143,  -711,
     187,   304,  -287,  -711,  -711,   527,  -711,     7,  -137,  -711,
      96,  -711,   822,   841,   -10,  1151,  -711,  1172,  1320,  -711,
    -711,  -711,  -711,  -711,  -711,  -711,  -266
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -384
static const yytype_int16 yytable[] =
{
     120,    71,   290,   479,   212,   139,   177,   178,   145,   499,
     435,   695,   301,   438,   158,   159,   160,   151,   315,     5,
       5,   126,   312,   169,   170,     5,   171,   172,   186,     5,
     324,   331,   324,   182,   184,   185,   189,   190,   485,   357,
     194,   597,   718,   182,   199,   200,     5,   717,   201,   341,
      44,   202,   203,   204,   786,   205,   500,   186,   182,   215,
     217,   369,   370,   371,   372,   602,   724,   444,   181,   809,
    -192,   328,   576,   157,   571,   334,   353,     5,   198,   359,
     121,   324,   295,   361,   121,   427,   319,   195,  -257,    44,
    -257,     3,   603,   786,   335,   216,   295,   387,   428,   696,
     368,   429,   821,  -257,   354,  -257,   334,   750,   334,   295,
     816,  -257,     5,   362,   430,   431,   282,   120,   284,   285,
     286,   194,   182,   217,   432,   602,   121,   295,   596,   223,
     596,   282,   421,   422,   610,   433,   334,   299,   358,   226,
     329,   761,   388,   164,   320,   604,   289,   224,  -257,   587,
      13,   299,   603,   694,  -257,   282,  -257,   718,   287,  -255,
     282,  -257,   717,   595,   299,  -274,   445,  -266,   334,   227,
     164,   332,   792,   793,   543,   282,   355,   282,   228,   325,
    -257,   546,   299,     5,  -274,   182,  -266,   149,  -274,   334,
    -266,    42,   806,   229,  -192,   487,   121,   495,   697,  -257,
     544,   352,   696,   521,   356,   700,    52,   350,   143,   547,
     295,   295,  -274,   811,  -266,   -62,   173,   569,   748,   147,
     342,   373,   374,   375,   376,   378,   380,   381,   382,   383,
     384,   385,   386,   -62,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   299,   299,   424,   426,   492,
     691,   334,   334,   161,   585,   295,   174,   435,   447,   762,
     438,   324,   334,   450,   377,   379,   334,    92,   212,   589,
     212,   334,   585,   191,   182,    92,   463,   465,   351,   467,
     469,   701,   586,   472,    92,   346,    98,   234,   565,   485,
     124,  -257,   812,   516,    98,   144,   497,    92,   489,   590,
     794,   329,   517,    98,   493,   329,   423,   425,   591,   461,
     299,   578,   581,   518,   566,    92,    98,   446,   192,   592,
     522,   221,   235,   215,   523,   215,   236,   507,   509,  -331,
     511,   512,   329,    92,    98,   182,   464,    92,  -330,   468,
     609,  -257,    13,   754,   275,    16,   276,   754,   175,   611,
     613,   524,    98,    21,    22,   712,    98,    22,   176,   596,
    -257,    28,   230,  -133,    28,   241,  -133,   561,   243,   295,
     514,     5,   246,  -258,   329,   149,  -231,   231,    38,  -257,
     713,  -133,   714,    42,  -133,   747,   506,    92,   329,  -334,
     755,  -335,   820,   525,   755,  -231,   715,  -368,    52,  -368,
     277,  -258,   823,   243,   278,   234,    98,   659,  -336,   463,
     551,   467,   554,   472,   556,   509,  -258,  -333,  -368,   175,
    -368,   560,   562,  -258,   299,  -332,   279,    92,    22,   176,
     568,   281,   570,   557,   558,    28,   575,  -133,   241,   282,
     235,   243,  -258,   -40,   236,   246,    98,    66,  -367,   291,
    -367,    92,   293,   217,   716,  -133,   599,   601,   451,     5,
     452,  -258,     5,   149,   152,   300,   149,  -258,   217,   550,
      98,   553,   810,    92,  -240,   304,   317,  -240,   504,   340,
     505,   813,   427,   241,   175,   242,   243,   343,   598,   345,
     246,   363,    98,    22,   176,   428,   365,   253,   429,   449,
      28,   453,  -133,   608,  -240,   810,   259,  -240,   -39,   102,
     454,   430,   431,  -240,   458,   810,  -240,   102,   460,   662,
    -133,   432,   665,   473,   153,   480,   102,   153,  -240,   474,
     486,  -240,   433,   481,   672,   487,   491,   496,   501,   102,
     513,   678,   510,   680,   515,   541,   683,    92,    13,   686,
     542,    16,   548,   689,   564,   582,   594,   102,   606,    21,
      22,   712,   334,   663,   666,   668,    98,    28,   674,  -133,
     692,    92,   705,   182,    92,   102,   709,   721,    92,   102,
     704,   740,   708,   751,   219,   710,   713,  -133,   714,    42,
      98,   617,   722,    98,   607,   567,   350,    98,   305,   308,
     311,   302,   715,   733,    52,   673,   764,   745,   699,   765,
     690,   766,   330,   719,   818,  -261,   805,   791,   778,   490,
       0,   563,   660,    92,     0,     0,     0,     0,     0,   102,
     339,     0,     0,   678,   680,     0,   683,   686,   704,   708,
     347,     0,    98,  -261,     0,     0,   734,   735,     0,   736,
       0,   737,   738,    66,     0,   739,     0,     0,  -261,   177,
     178,     0,     0,   781,     0,  -261,   753,   509,     0,   102,
       0,   509,   757,     0,     0,     0,   759,     0,     0,     0,
       0,     0,     0,  -263,  -261,     0,     0,     0,     0,     0,
       0,     0,     0,   102,   807,   734,   767,   768,   737,   769,
     770,   771,   772,  -261,     0,     0,     0,     0,     0,  -261,
       0,  -263,   190,   194,   217,   102,     0,     0,   783,   545,
       0,   784,     0,     0,     0,     0,  -263,   807,     0,     0,
     448,     0,     0,  -263,     0,     0,     0,   807,    92,     0,
       0,     0,     0,     0,   801,   802,   803,   804,     0,   780,
       0,     0,  -263,   470,     0,     0,     0,    98,   708,   814,
     815,     0,     0,     0,     0,   801,   802,   803,   804,   814,
     815,  -263,     0,     0,     0,    92,     0,  -263,    92,     0,
       0,   819,   509,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   708,     0,    98,     0,     0,    98,     0,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,   105,   102,     0,     0,   102,     0,     0,     0,
     102,   105,     0,   106,     0,     0,     0,     0,     0,     0,
       0,   106,   661,     0,   105,     0,    92,     0,    92,     0,
     106,    92,     0,     0,    92,     0,     0,    92,     0,     0,
       0,     0,   105,   106,     0,    98,     0,    98,     0,     0,
      98,     0,     0,    98,     0,   102,    98,    92,   427,     0,
     105,   106,     0,     0,   105,    92,   234,    92,     0,     0,
       0,   428,    92,     0,   429,     0,    98,     0,     0,   106,
       0,     0,     0,   106,    98,     0,    98,   430,   431,     0,
       0,    98,     0,     0,     0,   572,   574,   432,     0,   577,
     580,   235,     0,     0,     0,   236,     0,     0,   433,     0,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,     0,   106,     0,    57,    92,    59,    60,     0,
     238,     0,   239,   240,   241,     0,   242,   243,     0,     0,
      98,   246,     0,     0,   105,    98,    64,   434,   253,     0,
      92,     0,    67,     0,    92,   257,   258,   259,     0,     0,
     102,     0,     0,   106,     0,     0,     0,     0,   105,    98,
       0,     0,    13,    98,     0,    16,     0,     0,   670,   671,
       0,     0,     0,    21,    22,   712,     0,   106,     0,   675,
     105,    28,     0,  -133,     0,     0,     0,   102,     0,   684,
     102,     0,   687,     0,     0,     0,     0,     0,    38,   106,
     713,  -133,   714,    42,     0,     0,     0,     0,     0,    92,
      92,    92,    92,     0,     0,     0,   715,     0,    52,     0,
       0,     0,     0,     0,    92,    92,     0,     0,    98,    98,
      98,    98,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,    98,    98,     0,     0,     0,   102,     0,
     102,     0,     0,   102,   105,     0,   102,     0,     0,   102,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,   235,     0,   106,   773,   236,     0,     0,   105,   102,
       0,   105,     0,     0,     0,   105,     0,   102,     0,   102,
       0,     0,     0,     0,   102,     0,     0,   106,     0,     0,
     106,     0,     0,   108,   106,     0,     0,   234,     0,     0,
     756,   108,     0,     0,   241,   187,   242,   243,     0,     0,
     108,   246,     0,     0,   110,     0,     0,     0,   253,     0,
     105,     0,   110,   108,     0,   257,   258,   259,     0,     0,
       0,   110,   235,     0,     0,     0,   236,     0,     0,   106,
       0,   108,     0,   102,   110,     0,     0,     0,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   108,
       0,     0,   110,   108,     0,     0,     0,     0,     0,     0,
       0,   238,   102,   239,   240,   241,   102,   242,   243,   244,
     110,     0,   246,     0,   110,   283,   234,     0,   252,   253,
       0,     0,     0,   256,     0,     0,   257,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,     0,     0,   187,   187,   187,   314,
       0,   235,     0,     0,     0,   236,   234,     0,     0,     0,
     187,     0,     0,     0,   110,   105,     0,     0,     0,     0,
       0,   102,   102,   102,   102,     0,     0,     0,   187,     0,
       0,     0,     0,   108,   106,     0,   102,   102,   187,     0,
     238,   235,   111,   240,   241,   236,   242,   243,     0,     0,
     111,   246,   105,     0,   110,   105,     0,   108,   253,   111,
       0,     0,     0,     0,     0,   257,   258,   259,     0,     0,
       0,   106,   111,     0,   106,     0,     0,     0,   110,   108,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     111,   246,   247,   248,     0,   250,   251,   252,   253,     0,
     110,     0,   256,     0,     0,   257,   258,   259,   111,     0,
       0,   234,   111,   105,     0,   105,     0,     0,   105,     0,
       0,   105,     0,     0,   105,     0,     0,     0,   187,     0,
       0,     0,   106,     0,   106,     0,     0,   106,     0,     0,
     106,     0,     0,   106,   105,     0,   235,     0,     0,     0,
     236,   187,   105,   108,   105,     0,     0,     0,     0,   105,
       0,     0,   111,   106,     0,     0,     0,     0,     0,     0,
       0,   106,     0,   106,   110,     0,     0,   108,   106,     0,
     108,     0,     0,     0,   108,   238,     0,     0,     0,   241,
       0,   242,   243,     0,     0,     0,   246,     0,   110,     0,
       0,   110,   111,   253,     0,   110,     0,     0,     0,     0,
     257,   258,   259,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,   105,     0,     0,   111,     0,     0,   108,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,   106,     0,     0,     0,     0,   105,   111,     0,
     110,   105,     5,     6,     7,     8,     9,     0,     0,   127,
       0,     0,     0,     0,     0,     0,   106,     0,     0,     0,
     106,     0,    20,     0,     0,     0,     0,   128,   129,   130,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,   234,     0,     0,     0,    37,     0,
      39,     0,     0,   187,   187,     0,     0,   187,   187,     0,
       0,     0,    47,    48,    49,   131,   105,   105,   105,   105,
       0,     0,   111,     0,     0,     0,     0,     0,     0,   235,
       0,   105,   105,   236,     0,   106,   106,   106,   106,     0,
       0,     0,     0,     0,   108,    58,   111,     0,     0,   111,
     106,   106,     0,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,   110,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,    69,     0,   133,   246,
       0,   108,     0,     0,   108,   252,   253,     0,     0,     0,
     256,     0,     0,   257,   258,   259,   187,   187,   111,     0,
       0,     0,   110,     0,     0,   110,     0,   187,     0,   314,
       0,     0,     0,     0,   314,     0,     0,   187,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   108,     0,   108,     0,     0,   108,     0,    73,
     108,     0,     0,   108,     0,     0,     0,   119,     0,     0,
       0,     0,     0,   110,     0,   110,   146,     0,   110,     0,
       0,   110,     0,   108,   110,     0,     0,     0,     0,   168,
       0,   108,     0,   108,     0,     0,     0,     0,   108,     0,
       0,     0,     0,     0,   110,     0,     0,   193,     0,     0,
       0,     0,   110,     0,   110,     0,     0,     0,     0,   110,
       0,     0,     0,   111,     0,    73,     0,     0,     0,   220,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   187,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
     111,     0,   108,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   274,  -345,   110,   280,
    -367,     0,  -367,   110,   314,   314,   108,   314,   314,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,     0,     0,     0,   125,   314,   110,   314,     0,
     125,   110,     0,     0,   148,   125,   125,     0,     0,   316,
       0,   111,   162,   111,   165,     0,   111,     0,     0,   111,
       0,     0,   111,   234,     0,     0,     0,     0,     0,     0,
     785,     0,     0,   338,   196,     0,     0,     0,     0,     0,
       0,     0,   111,     0,     0,   108,   108,   108,   108,     0,
     111,     0,   111,     0,     0,    73,     0,   111,   235,     0,
     108,   108,   236,     0,     0,     0,   110,   110,   110,   110,
       0,     0,     0,   222,     0,     0,     0,     0,     0,     0,
     232,   110,   110,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,     0,   250,   251,   252,   253,   111,   254,     0,   256,
       0,   111,   257,   258,   259,     0,   196,   235,     0,    73,
     294,   236,     0,     0,     0,     0,   196,     0,   303,     0,
       0,     0,     0,     0,   294,   111,     0,   321,     0,   111,
       0,   237,     0,   338,     0,     0,   338,   294,     0,   125,
       0,   234,   333,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,   294,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
       0,   257,   258,   259,     0,     0,   235,     0,     0,     0,
     236,   669,     0,     0,   364,   508,     0,     0,     0,   165,
     165,   165,   165,     0,   111,   111,   111,   111,     0,     0,
     237,     0,     0,     0,     0,     0,   389,     0,     0,   111,
     111,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,     0,
     250,   251,   252,   253,     0,   254,   255,   256,   294,   294,
     257,   258,   259,     0,     0,     0,     0,     0,     0,   482,
       0,     5,     6,     7,     8,     9,     0,  -383,   127,     0,
       0,  -383,     0,   455,     0,   459,     0,     0,     0,  -383,
       0,    20,     0,     0,     0,     0,   128,   129,   130,     0,
      27,     0,     0,   321,     0,    30,    31,     0,    33,     0,
      34,    35,   165,     0,     0,     0,     0,    37,     0,    39,
     584,     0,     0,   294,  -383,     0,     0,     0,  -383,     0,
       0,    47,    48,    49,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -256,  -383,     0,
       0,   165,     0,     0,     0,     0,     0,   614,     0,     0,
      73,     0,     0,  -383,    58,  -383,  -383,  -383,     0,  -383,
    -383,  -383,     0,  -383,  -383,  -383,  -383,  -383,  -383,  -383,
    -383,  -383,    63,  -383,  -383,  -383,     0,     0,  -383,  -383,
    -383,     0,     0,   132,     0,    69,     0,   498,     0,     0,
       0,     0,     0,   482,     0,     5,     6,     7,     8,     9,
       0,     0,   127,     0,     0,     0,     0,     0,   338,     0,
     338,     0,     0,   338,     0,    20,   338,     0,     0,   688,
     128,   129,   130,     0,    27,     0,     0,   294,     0,    30,
      31,     0,    33,     0,    34,    35,     0,     0,     0,   698,
       0,    37,     0,    39,     0,     0,     0,   703,     0,   707,
     588,     0,     0,   321,    73,    47,    48,    49,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,   656,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,     0,     0,     0,    67,     0,     0,   132,     0,    69,
       0,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,   758,     0,     0,     0,   760,     0,     0,     0,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
     321,     0,    10,    11,    12,     0,    13,    14,    15,    16,
    -238,    17,    18,     0,    19,    20,     0,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,  -133,    29,    30,
      31,    32,    33,   -38,    34,    35,     0,    36,     0,     0,
    -238,    37,    38,    39,    40,  -133,    41,    42,   720,  -238,
      43,    44,     0,    45,    46,    47,    48,    49,    50,     0,
      51,     0,    52,    53,  -238,     0,     0,    54,     0,    55,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,    66,     0,     0,    67,     0,     0,    68,     0,    69,
       0,    70,     0,     0,     0,   125,     0,     4,   196,     5,
       6,     7,     8,     9,   763,  -383,    10,    11,    12,  -383,
      13,    14,    15,    16,  -238,    17,    18,  -383,    19,    20,
    -383,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,  -133,    29,    30,    31,    32,    33,   -38,    34,    35,
       0,    36,     0,     0,  -238,    37,     0,    39,    40,  -133,
      41,    42,  -383,  -238,    43,    44,  -383,    45,    46,    47,
      48,    49,    50,     0,    51,     0,    52,    53,  -238,     0,
       0,    54,     0,    55,     0,     0,  -383,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -383,    58,  -383,  -383,  -383,  -383,  -383,  -383,  -383,
       0,  -383,  -383,  -383,  -383,  -383,  -383,  -383,  -383,  -383,
      63,  -383,  -383,  -383,     0,    66,  -383,  -383,  -383,     0,
       0,    68,  -383,    69,     4,    70,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -238,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,  -133,    29,
      30,    31,    32,    33,   -38,    34,    35,     0,    36,     0,
       0,  -238,    37,    38,    39,    40,  -133,    41,    42,     0,
    -238,    43,    44,     0,    45,    46,    47,    48,    49,    50,
       0,    51,     0,    52,    53,  -238,     0,     0,    54,     0,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,    63,     0,    64,
      65,   179,    66,     0,     0,    67,     0,     0,    68,   206,
      69,     4,    70,     5,     6,     7,     8,     9,     0,     0,
      10,    11,    12,     0,    13,    14,    15,    16,  -238,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
      26,     0,    27,    28,     0,  -133,    29,    30,    31,    32,
      33,   -38,    34,    35,     0,    36,     0,     0,  -238,    37,
      38,    39,    40,  -133,    41,    42,     0,  -238,    43,    44,
       0,    45,    46,    47,    48,    49,    50,     0,    51,     0,
      52,    53,  -238,     0,     0,    54,     0,    55,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,    63,     0,    64,    65,     0,    66,
       0,     0,    67,     0,     0,    68,   349,    69,     4,    70,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -238,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,    26,     0,    27,
      28,     0,  -133,    29,    30,    31,    32,    33,   -38,    34,
      35,     0,    36,     0,     0,  -238,    37,    38,    39,    40,
    -133,    41,    42,     0,  -238,    43,    44,     0,    45,    46,
      47,    48,    49,    50,     0,    51,     0,    52,    53,  -238,
       0,     0,    54,     0,    55,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,     0,    66,     0,     0,    67,
       0,     0,    68,   206,    69,     4,    70,     5,     6,     7,
       8,     9,     0,     0,    10,    11,    12,     0,    13,    14,
      15,    16,  -238,    17,    18,     0,    19,    20,     0,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,  -133,
      29,    30,    31,    32,    33,   -38,    34,    35,     0,    36,
       0,     0,  -238,    37,    38,    39,    40,  -133,    41,    42,
       0,  -238,    43,    44,     0,    45,    46,    47,    48,    49,
      50,     0,    51,     0,    52,    53,  -238,     0,     0,    54,
       0,    55,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,    63,     0,
      64,    65,     0,    66,     0,     0,    67,     0,     0,    68,
     616,    69,     4,    70,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -238,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,  -133,    29,    30,    31,
      32,    33,   -38,    34,    35,     0,    36,     0,     0,  -238,
      37,    38,    39,    40,  -133,    41,    42,     0,  -238,    43,
      44,     0,    45,    46,    47,    48,    49,    50,     0,    51,
       0,    52,    53,  -238,     0,     0,    54,     0,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    63,     0,    64,    65,     0,
      66,     0,     0,    67,     0,     0,    68,   711,    69,     4,
      70,     5,     6,     7,     8,     9,     0,     0,    10,    11,
      12,     0,    13,    14,    15,    16,  -238,    17,    18,     0,
      19,    20,     0,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,  -133,    29,    30,    31,    32,    33,   -38,
      34,    35,     0,    36,     0,     0,  -238,    37,   219,    39,
      40,  -133,    41,    42,     0,  -238,    43,    44,     0,    45,
      46,    47,    48,    49,    50,     0,    51,     0,    52,    53,
    -238,     0,     0,    54,     0,    55,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,    63,     0,    64,    65,     0,    66,     0,     0,
      67,     0,     0,    68,     0,    69,     4,    70,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -238,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -133,    29,    30,    31,    32,    33,   -38,    34,    35,     0,
      36,     0,     0,  -238,    37,     0,    39,    40,  -133,    41,
      42,     0,  -238,    43,    44,     0,    45,    46,    47,    48,
      49,    50,     0,    51,     0,    52,    53,  -238,     0,     0,
      54,     0,    55,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,    63,
       0,    64,    65,     0,    66,     0,     0,    67,     0,     0,
      68,     0,    69,     4,    70,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,    13,    14,    15,    16,
    -238,    17,    18,     0,    19,    20,     0,    21,    22,    23,
      24,    25,   462,     0,    27,    28,     0,  -133,    29,    30,
      31,    32,    33,   -38,    34,    35,     0,    36,     0,     0,
    -238,    37,     0,    39,    40,  -133,    41,    42,     0,  -238,
      43,    44,     0,    45,    46,    47,    48,    49,    50,     0,
      51,     0,    52,    53,  -238,     0,     0,    54,     0,    55,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,    66,     0,     0,    67,     0,     0,    68,     0,    69,
       4,    70,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -238,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,   466,
       0,    27,    28,     0,  -133,    29,    30,    31,    32,    33,
     -38,    34,    35,     0,    36,     0,     0,  -238,    37,     0,
      39,    40,  -133,    41,    42,     0,  -238,    43,    44,     0,
      45,    46,    47,    48,    49,    50,     0,    51,     0,    52,
      53,  -238,     0,     0,    54,     0,    55,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    63,     0,    64,    65,     0,    66,     0,
       0,    67,     0,     0,    68,     0,    69,     4,    70,     5,
       6,     7,     8,     9,     0,     0,    10,    11,    12,     0,
      13,    14,    15,    16,  -238,    17,    18,     0,    19,    20,
       0,    21,    22,    23,    24,    25,   677,     0,    27,    28,
       0,  -133,    29,    30,    31,    32,    33,   -38,    34,    35,
       0,    36,     0,     0,  -238,    37,     0,    39,    40,  -133,
      41,    42,     0,  -238,    43,    44,     0,    45,    46,    47,
      48,    49,    50,     0,    51,     0,    52,    53,  -238,     0,
       0,    54,     0,    55,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
      63,     0,    64,    65,     0,    66,     0,     0,    67,     0,
       0,    68,     0,    69,     4,    70,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -238,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,   679,     0,    27,    28,     0,  -133,    29,
      30,    31,    32,    33,   -38,    34,    35,     0,    36,     0,
       0,  -238,    37,     0,    39,    40,  -133,    41,    42,     0,
    -238,    43,    44,     0,    45,    46,    47,    48,    49,    50,
       0,    51,     0,    52,    53,  -238,     0,     0,    54,     0,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,    63,     0,    64,
      65,     0,    66,     0,     0,    67,     0,     0,    68,     0,
      69,     4,    70,     5,     6,     7,     8,     9,     0,     0,
      10,    11,    12,     0,    13,    14,    15,    16,  -238,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
     682,     0,    27,    28,     0,  -133,    29,    30,    31,    32,
      33,   -38,    34,    35,     0,    36,     0,     0,  -238,    37,
       0,    39,    40,  -133,    41,    42,     0,  -238,    43,    44,
       0,    45,    46,    47,    48,    49,    50,     0,    51,     0,
      52,    53,  -238,     0,     0,    54,     0,    55,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,    63,     0,    64,    65,     0,    66,
       0,     0,    67,     0,     0,    68,     0,    69,     4,    70,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -238,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,   685,     0,    27,
      28,     0,  -133,    29,    30,    31,    32,    33,   -38,    34,
      35,     0,    36,     0,     0,  -238,    37,     0,    39,    40,
    -133,    41,    42,     0,  -238,    43,    44,     0,    45,    46,
      47,    48,    49,    50,     0,    51,     0,    52,    53,  -238,
       0,     0,    54,     0,    55,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,     0,    66,     0,     0,    67,
       0,     0,    68,     0,    69,     4,    70,     5,     6,     7,
       8,     9,     0,     0,    10,    11,    12,     0,    13,    14,
      15,    16,  -238,    17,    18,     0,    19,    20,     0,    21,
      22,    23,    24,    25,   702,     0,    27,    28,     0,  -133,
      29,    30,    31,    32,    33,   -38,    34,    35,     0,    36,
       0,     0,  -238,    37,     0,    39,    40,  -133,    41,    42,
       0,  -238,    43,    44,     0,    45,    46,    47,    48,    49,
      50,     0,    51,     0,    52,    53,  -238,     0,     0,    54,
       0,    55,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,    63,     0,
      64,    65,     0,    66,     0,     0,    67,     0,     0,    68,
       0,    69,     4,    70,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -238,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,   706,     0,    27,    28,     0,  -133,    29,    30,    31,
      32,    33,   -38,    34,    35,     0,    36,     0,     0,  -238,
      37,     0,    39,    40,  -133,    41,    42,     0,  -238,    43,
      44,     0,    45,    46,    47,    48,    49,    50,     0,    51,
       0,    52,    53,  -238,     0,     0,    54,     0,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    63,     0,    64,    65,     0,
      66,     0,     0,    67,     0,     0,    68,     0,    69,   482,
      70,     5,     6,     7,     8,     9,     0,  -383,   127,     0,
       0,  -383,     0,     0,     0,     0,     0,     0,     0,  -383,
       0,    20,     0,     0,     0,     0,   128,   129,   130,     0,
      27,     0,     0,     0,     0,    30,    31,     0,    33,     0,
      34,    35,     0,     0,     0,     0,     0,    37,     0,    39,
       0,     0,     0,     0,  -383,     0,     0,     0,  -383,     0,
       0,    47,    48,    49,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -383,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -383,    58,  -383,  -383,  -383,     0,  -383,
    -383,  -383,     0,  -383,  -383,  -383,  -383,  -383,  -383,  -383,
    -383,  -383,    63,  -383,  -383,  -383,     0,     0,  -383,  -383,
    -383,     0,     0,   132,     0,    69,   482,   498,     5,     6,
       7,     8,     9,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,     0,
       0,     0,     0,   128,   129,   130,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     5,
       6,     7,     8,     9,     0,     0,   127,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,    20,
       0,     0,     0,     0,   128,   129,   130,    62,    27,    63,
       0,    64,    65,    30,    31,     0,    33,    67,    34,    35,
     132,     0,    69,     0,   498,    37,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
      48,    49,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
       0,     0,     5,     6,     7,     8,     9,     0,    62,   127,
      63,     0,    64,    65,   179,     0,     0,     0,    67,     0,
       0,   132,    20,    69,     0,   133,   779,   128,   129,   130,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   131,     0,     0,   210,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,   211,     0,     5,     6,     7,     8,     9,
       0,    62,   127,    63,     0,    64,    65,   179,     0,     0,
       0,    67,     0,     0,   132,    20,    69,     0,   133,     0,
     128,   129,   130,     0,    27,     0,     0,     0,     0,    30,
      31,     0,    33,     0,    34,    35,     0,     0,     0,     0,
       0,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,   131,     0,
       0,   210,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     5,     6,
       7,     8,     9,     0,    62,   127,    63,     0,    64,    65,
     179,     0,     0,     0,    67,     0,     0,   132,    20,    69,
     503,   133,     0,   128,   129,   130,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,   155,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   156,     0,    56,     0,     0,     0,     5,
       6,     7,     8,     9,     0,     0,   127,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,    20,
       0,     0,     0,     0,   128,   129,   130,    62,    27,    63,
       0,    64,    65,    30,    31,     0,    33,    67,    34,    35,
     132,     0,    69,     0,   133,    37,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
      48,    49,   131,     0,     0,   210,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
       0,     0,     5,     6,     7,     8,     9,     0,    62,   127,
      63,     0,    64,    65,   179,     0,     0,     0,    67,     0,
       0,   132,    20,    69,     0,   133,     0,   128,   129,   130,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     0,     5,     6,     7,     8,     9,
       0,    62,   127,    63,     0,    64,    65,   179,     0,     0,
       0,    67,     0,     0,   132,    20,    69,     0,   133,     0,
     128,   129,   130,     0,    27,     0,     0,     0,     0,    30,
      31,     0,    33,     0,    34,    35,     0,     0,     0,     0,
       0,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,   183,     0,     0,    67,     0,     0,   132,     0,    69,
       0,   133,     5,     6,     7,     8,     9,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   186,    20,     0,     0,     0,     0,   128,   129,   130,
       0,    27,     0,     0,     0,     0,    30,    31,     0,    33,
       0,    34,    35,     0,     0,     0,     0,     0,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     5,     6,     7,     8,     9,     0,     0,
     127,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,    20,     0,     0,     0,     0,   128,   129,
     130,    62,    27,    63,     0,    64,    65,    30,    31,     0,
      33,    67,    34,    35,    68,     0,    69,     0,   133,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   156,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,     5,     6,     7,     8,
       9,     0,    62,   127,    63,     0,    64,    65,     0,     0,
       0,     0,    67,     0,     0,   132,    20,    69,     0,   133,
       0,   128,   129,   130,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,   600,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    48,    49,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     5,     6,     7,
       8,     9,     0,     0,   127,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,   741,     0,     0,
       0,     0,   128,   129,   130,    62,    27,    63,     0,    64,
      65,    30,    31,     0,    33,    67,    34,    35,   132,     0,
      69,     0,   133,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   742,    48,    49,
     743,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       5,     6,     7,     8,     9,     0,    62,   127,    63,     0,
      64,    65,   179,     0,     0,     0,    67,     0,     0,   132,
      20,    69,     0,   744,     0,   128,   129,   130,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     5,     6,     7,     8,     9,     0,    62,
     127,    63,     0,    64,    65,   179,     0,     0,     0,    67,
       0,     0,   132,    20,    69,     0,   744,     0,   128,   129,
     732,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    34,    35,     0,     0,     0,     0,     0,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,     5,     6,     7,     8,
       9,     0,    62,   127,    63,     0,    64,    65,   179,     0,
       0,     0,    67,     0,     0,   132,    20,    69,     0,   133,
       0,   128,   129,   130,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    48,    49,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     5,     6,     7,
       8,     9,     0,     0,   127,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,    20,     0,     0,
       0,     0,   128,   129,   549,    62,    27,    63,     0,    64,
      65,    30,    31,     0,    33,    67,    34,    35,   132,     0,
      69,     0,   133,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   127,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    20,     0,
       0,     0,     0,   128,   129,   552,    62,    27,    63,     0,
      64,    65,    30,    31,     0,    33,    67,    34,    35,   132,
       0,    69,     0,   133,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     5,
       6,     7,     8,     9,     0,     0,   127,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,    20,
       0,     0,     0,     0,   128,   129,   726,    62,    27,    63,
       0,    64,    65,    30,    31,     0,    33,    67,    34,    35,
     132,     0,    69,     0,   133,    37,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
      48,    49,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   127,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
      20,     0,     0,     0,     0,   128,   129,   727,    62,    27,
      63,     0,    64,    65,    30,    31,     0,    33,    67,    34,
      35,   132,     0,    69,     0,   133,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     5,     6,     7,     8,     9,     0,     0,   127,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,    20,     0,     0,     0,     0,   128,   129,   729,    62,
      27,    63,     0,    64,    65,    30,    31,     0,    33,    67,
      34,    35,   132,     0,    69,     0,   133,    37,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    48,    49,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   127,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,    20,     0,     0,     0,     0,   128,   129,   730,
      62,    27,    63,     0,    64,    65,    30,    31,     0,    33,
      67,    34,    35,   132,     0,    69,     0,   133,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     5,     6,     7,     8,     9,     0,     0,
     127,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,    20,     0,     0,     0,     0,   128,   129,
     731,    62,    27,    63,     0,    64,    65,    30,    31,     0,
      33,    67,    34,    35,   132,     0,    69,     0,   133,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   127,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,    20,     0,     0,     0,     0,   128,
     129,   732,    62,    27,    63,     0,    64,    65,    30,    31,
       0,    33,    67,    34,    35,   132,     0,    69,     0,   133,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,    48,    49,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     5,     6,     7,     8,     9,
       0,     0,   127,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,    20,     0,     0,     0,     0,
     128,   129,   130,    62,    27,    63,     0,    64,    65,    30,
      31,     0,    33,    67,    34,    35,   132,     0,    69,     0,
     133,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   127,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,     0,    20,     0,     0,     0,
       0,   128,   129,   130,    62,    27,    63,     0,     0,    65,
      30,    31,     0,    33,    67,    34,    35,   132,     0,    69,
       0,   133,    37,     0,    39,     5,     0,     0,     0,     0,
       0,   620,     0,     0,     0,   621,    47,    48,    49,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -265,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,     0,     0,  -265,     0,     0,     0,  -265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,   179,     0,     0,     0,     0,     0,     0,   132,     0,
      69,  -265,   744,   622,   623,   624,   625,   626,   627,     0,
       0,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,     0,     0,   638,     0,     0,     0,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,   649,     0,   650,
       0,     0,   651,   652,   653,   654,   655,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   235,     0,     0,     0,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   232,     0,     0,   237,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   235,     0,
       0,     0,   236,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   237,   254,   255,   256,     0,     0,   257,   258,
     259,     0,   235,     0,     0,     0,   236,   238,   612,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   237,   254,   255,   256,
       0,     0,   257,   258,   259,     0,     0,     0,     0,     0,
       0,   238,   788,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     232,   254,   255,   256,   233,     0,   257,   258,   259,     0,
       0,     0,   234,     0,     0,     0,   822,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   235,     0,     0,
       0,   236,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,   309,     0,     0,     0,
       0,     0,     0,     0,     0,   310,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,     0,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   232,   254,
     255,   256,   233,     0,   257,   258,   259,     0,     0,   282,
     234,   306,     0,     0,     0,     0,     0,     0,     0,     0,
     307,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,   236,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,   237,
       0,     0,     0,   234,   579,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,   232,
       0,     0,   237,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,     0,   257,   258,   259,     0,   235,   282,     0,     0,
     236,     0,     0,     0,     0,     0,     0,   313,     0,     0,
       0,     0,   232,     0,     0,     0,   233,     0,     0,     0,
     237,     0,     0,     0,   234,   186,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,   237,     0,     0,     0,   234,   573,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   282,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,   233,     0,     0,     0,   237,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   282,     0,     0,     0,     0,   676,     0,     0,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,   237,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,     0,     0,     0,   681,
       0,     0,     0,     0,   232,     0,     0,     0,   233,     0,
       0,     0,   237,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,   282,     0,     0,
       0,     0,   774,     0,     0,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,   237,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
     282,     0,     0,     0,     0,   775,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,     0,   237,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,   282,     0,     0,     0,     0,   776,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,   282,     0,     0,     0,
       0,   777,     0,     0,     0,     0,   232,     0,     0,     0,
     233,     0,     0,     0,   237,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,   282,
       0,     0,     0,     0,   789,     0,     0,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,   237,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   282,     0,     0,     0,     0,   790,     0,     0,
       0,     0,   232,     0,     0,     0,   233,     0,     0,     0,
     237,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,   337,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   288,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,  -346,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,     0,   232,     0,   237,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,   439,     0,     0,     0,   238,
       0,   239,   240,   241,   440,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,     0,   257,   258,   259,     0,     0,   235,
       0,     0,     0,   236,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,   237,     0,     0,     0,   234,   441,     0,
       0,     0,     0,     0,     0,     0,     0,   442,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   292,   257,   258,   259,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   336,   257,   258,   259,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   348,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,   360,   237,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   526,   257,   258,   259,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   527,   257,   258,   259,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   528,
     257,   258,   259,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   529,   257,   258,
     259,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   530,   257,   258,   259,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   531,   257,   258,   259,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   532,   257,   258,   259,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   533,
     257,   258,   259,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   534,   257,   258,
     259,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   535,   257,   258,   259,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   536,   257,   258,   259,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   537,   257,   258,   259,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   538,
     257,   258,   259,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   539,   257,   258,
     259,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   540,   257,   258,   259,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
     121,     0,     0,   234,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   237,     0,   583,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   559,   257,   258,   259,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,   615,   237,     0,
       0,     0,   234,   664,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,   667,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   725,
       0,     0,     0,     0,   232,     0,     0,     0,   233,     0,
       0,     0,   237,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,     0,
       0,     0,   728,     0,     0,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,   237,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
       0,     0,     0,     0,     0,   795,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,     0,   237,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,     0,     0,     0,   796,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,   237,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,     0,     0,
       0,   797,     0,     0,     0,     0,   232,     0,     0,     0,
     233,     0,     0,     0,   237,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,     0,
       0,     0,     0,     0,   798,     0,     0,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,   237,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,     0,     0,     0,   799,     0,     0,
       0,     0,   232,     0,     0,     0,   233,     0,     0,     0,
     237,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   234,   235,
     257,   258,   259,   236,     0,     0,     0,     0,     0,     0,
     800,     0,     0,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,   237,     0,     0,     0,   234,     0,     0,
       0,     0,     0,   235,     0,     0,     0,   236,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   237,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   237,   250,   251,   252,
     253,     0,   254,     0,   256,     0,     0,   257,   258,   259,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,     0,   257,   258,   259
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-711))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,     2,   143,   320,    69,    15,    40,    41,    18,   343,
     279,   597,   153,   279,    24,    25,    26,    22,   161,     3,
       3,    14,   159,    33,    34,     3,    36,    37,    22,     3,
       1,   174,     1,    43,    44,    45,    46,    47,   325,    31,
      50,   481,   618,    53,    54,    55,     3,   618,    58,   192,
      59,    61,    62,    63,   752,    65,   343,    22,    68,    69,
      70,   226,   227,   228,   229,    46,   658,    31,    43,   779,
       3,    31,    22,    24,    22,   100,   100,     3,    53,   216,
      76,     1,   147,   100,    76,    18,    69,    61,    59,    59,
      59,     0,    73,   791,   119,    70,   161,    23,    31,    31,
      57,    34,   812,    74,   128,    74,   100,   693,   100,   174,
     119,    31,     3,   130,    47,    48,   125,   127,   128,   129,
     130,   131,   132,   133,    57,    46,    76,   192,    99,    35,
      99,   125,   275,   276,   130,    68,   100,   147,   130,    18,
     100,   717,    68,   127,   127,   126,   139,    53,   119,   127,
      14,   161,    73,   593,   125,   125,   125,   733,   133,    79,
     125,    81,   733,   480,   174,    81,   130,    81,   100,    48,
     127,   176,   764,   765,    31,   125,   100,   125,    57,    99,
     100,   100,   192,     3,   100,   195,   100,     7,   104,   100,
     104,    55,   778,    72,   127,    81,    76,   340,   130,   119,
      57,   211,    31,   368,   128,   126,    70,   208,    68,   128,
     275,   276,   128,    31,   128,    35,     7,   128,   104,   127,
     195,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,    53,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   334,
     587,   100,   100,   127,   100,   340,   127,   546,   288,   719,
     546,     1,   100,   293,   235,   236,   100,     2,   353,    18,
     355,   100,   100,    65,   304,    10,   306,   307,   126,   309,
     310,   130,   128,   313,    19,   119,     2,    21,   100,   596,
      12,    31,   130,    48,    10,    17,   125,    32,   328,    48,
     128,   100,    57,    19,   334,   100,   277,   278,    57,   304,
     340,   468,   469,    68,   126,    50,    32,   288,   127,    68,
     119,    41,    56,   353,   119,   355,    60,   357,   358,   119,
     360,   361,   100,    68,    50,   365,   307,    72,   119,   310,
     501,    81,    14,   697,   127,    17,   129,   701,    17,   506,
     507,   119,    68,    25,    26,    27,    72,    26,    27,    99,
     100,    33,   119,    35,    33,    99,    35,   452,   102,   454,
     365,     3,   106,    31,   100,     7,   100,   124,    50,   119,
      52,    53,    54,    55,    53,   692,   357,   122,   100,   119,
     697,   119,   812,   119,   701,   119,    68,   127,    70,   129,
      56,    59,   822,   102,    60,    21,   122,   119,   119,   439,
     440,   441,   442,   443,   444,   445,    74,   119,   127,    17,
     129,   451,   452,    81,   454,   119,   127,   162,    26,    27,
     460,   119,   462,   446,   447,    33,   466,    35,    99,   125,
      56,   102,   100,    41,    60,   106,   162,   119,   127,   119,
     129,   186,    75,   483,   126,    53,   486,   487,    79,     3,
      81,   119,     3,     7,     8,   125,     7,   125,   498,   440,
     186,   442,   779,   208,    18,   127,   127,    18,   353,   127,
     355,   788,    18,    99,    17,   101,   102,    81,   483,   119,
     106,     7,   208,    26,    27,    31,    99,   113,    34,   119,
      33,   128,    35,   498,    48,   812,   122,    48,    41,     2,
     100,    47,    48,    57,   119,   822,    57,    10,    31,   549,
      53,    57,   552,    24,    68,   100,    19,    68,    72,   128,
      79,    72,    68,   128,   564,    81,   128,   128,   100,    32,
     125,   571,   130,   573,   125,   128,   576,   282,    14,   579,
     130,    17,    56,   583,    81,    24,   128,    50,   128,    25,
      26,    27,   100,    22,    22,   130,   282,    33,   125,    35,
      99,   306,   130,   603,   309,    68,   130,   102,   313,    72,
     610,   128,   612,    74,    50,   615,    52,    53,    54,    55,
     306,   513,   127,   309,   497,   459,   617,   313,   157,   158,
     159,   154,    68,   674,    70,   565,   721,   692,   603,   721,
     585,   722,   171,   619,   791,    31,   778,   762,   740,   329,
      -1,   454,   546,   358,    -1,    -1,    -1,    -1,    -1,   122,
     189,    -1,    -1,   663,   664,    -1,   666,   667,   668,   669,
     199,    -1,   358,    59,    -1,    -1,   676,   677,    -1,   679,
      -1,   681,   682,   119,    -1,   685,    -1,    -1,    74,   713,
     714,    -1,    -1,   748,    -1,    81,   696,   697,    -1,   162,
      -1,   701,   702,    -1,    -1,    -1,   706,    -1,    -1,    -1,
      -1,    -1,    -1,    31,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   779,   725,   726,   727,   728,   729,
     730,   731,   732,   119,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    59,   742,   743,   744,   208,    -1,    -1,   748,   435,
      -1,   751,    -1,    -1,    -1,    -1,    74,   812,    -1,    -1,
     289,    -1,    -1,    81,    -1,    -1,    -1,   822,   473,    -1,
      -1,    -1,    -1,    -1,   774,   775,   776,   777,    -1,   744,
      -1,    -1,   100,   312,    -1,    -1,    -1,   473,   788,   789,
     790,    -1,    -1,    -1,    -1,   795,   796,   797,   798,   799,
     800,   119,    -1,    -1,    -1,   510,    -1,   125,   513,    -1,
      -1,   811,   812,    -1,    -1,    -1,    -1,    -1,    -1,   282,
      -1,    -1,   822,    -1,   510,    -1,    -1,   513,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,   306,    -1,    -1,   309,    -1,    -1,    -1,
     313,    19,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,   548,    -1,    32,    -1,   571,    -1,   573,    -1,
      19,   576,    -1,    -1,   579,    -1,    -1,   582,    -1,    -1,
      -1,    -1,    50,    32,    -1,   571,    -1,   573,    -1,    -1,
     576,    -1,    -1,   579,    -1,   358,   582,   602,    18,    -1,
      68,    50,    -1,    -1,    72,   610,    21,   612,    -1,    -1,
      -1,    31,   617,    -1,    34,    -1,   602,    -1,    -1,    68,
      -1,    -1,    -1,    72,   610,    -1,   612,    47,    48,    -1,
      -1,   617,    -1,    -1,    -1,   464,   465,    57,    -1,   468,
     469,    56,    -1,    -1,    -1,    60,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   676,    -1,   122,    -1,    95,   681,    97,    98,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,    -1,    -1,
     676,   106,    -1,    -1,   162,   681,   116,   117,   113,    -1,
     705,    -1,   122,    -1,   709,   120,   121,   122,    -1,    -1,
     473,    -1,    -1,   162,    -1,    -1,    -1,    -1,   186,   705,
      -1,    -1,    14,   709,    -1,    17,    -1,    -1,   557,   558,
      -1,    -1,    -1,    25,    26,    27,    -1,   186,    -1,   568,
     208,    33,    -1,    35,    -1,    -1,    -1,   510,    -1,   578,
     513,    -1,   581,    -1,    -1,    -1,    -1,    -1,    50,   208,
      52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,   774,
     775,   776,   777,    -1,    -1,    -1,    68,    -1,    70,    -1,
      -1,    -1,    -1,    -1,   789,   790,    -1,    -1,   774,   775,
     776,   777,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,   789,   790,    -1,    -1,    -1,   571,    -1,
     573,    -1,    -1,   576,   282,    -1,   579,    -1,    -1,   582,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    56,    -1,   282,   126,    60,    -1,    -1,   306,   602,
      -1,   309,    -1,    -1,    -1,   313,    -1,   610,    -1,   612,
      -1,    -1,    -1,    -1,   617,    -1,    -1,   306,    -1,    -1,
     309,    -1,    -1,     2,   313,    -1,    -1,    21,    -1,    -1,
     699,    10,    -1,    -1,    99,    46,   101,   102,    -1,    -1,
      19,   106,    -1,    -1,     2,    -1,    -1,    -1,   113,    -1,
     358,    -1,    10,    32,    -1,   120,   121,   122,    -1,    -1,
      -1,    19,    56,    -1,    -1,    -1,    60,    -1,    -1,   358,
      -1,    50,    -1,   676,    32,    -1,    -1,    -1,   681,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    50,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,   705,    97,    98,    99,   709,   101,   102,   103,
      68,    -1,   106,    -1,    72,   126,    21,    -1,   112,   113,
      -1,    -1,    -1,   117,    -1,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,   157,   158,   159,   160,
      -1,    56,    -1,    -1,    -1,    60,    21,    -1,    -1,    -1,
     171,    -1,    -1,    -1,   122,   473,    -1,    -1,    -1,    -1,
      -1,   774,   775,   776,   777,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,   162,   473,    -1,   789,   790,   199,    -1,
      95,    56,     2,    98,    99,    60,   101,   102,    -1,    -1,
      10,   106,   510,    -1,   162,   513,    -1,   186,   113,    19,
      -1,    -1,    -1,    -1,    -1,   120,   121,   122,    -1,    -1,
      -1,   510,    32,    -1,   513,    -1,    -1,    -1,   186,   208,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
      50,   106,   107,   108,    -1,   110,   111,   112,   113,    -1,
     208,    -1,   117,    -1,    -1,   120,   121,   122,    68,    -1,
      -1,    21,    72,   571,    -1,   573,    -1,    -1,   576,    -1,
      -1,   579,    -1,    -1,   582,    -1,    -1,    -1,   289,    -1,
      -1,    -1,   571,    -1,   573,    -1,    -1,   576,    -1,    -1,
     579,    -1,    -1,   582,   602,    -1,    56,    -1,    -1,    -1,
      60,   312,   610,   282,   612,    -1,    -1,    -1,    -1,   617,
      -1,    -1,   122,   602,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   610,    -1,   612,   282,    -1,    -1,   306,   617,    -1,
     309,    -1,    -1,    -1,   313,    95,    -1,    -1,    -1,    99,
      -1,   101,   102,    -1,    -1,    -1,   106,    -1,   306,    -1,
      -1,   309,   162,   113,    -1,   313,    -1,    -1,    -1,    -1,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,   676,    -1,
      -1,    -1,    -1,   681,    -1,    -1,   186,    -1,    -1,   358,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   676,    -1,    -1,
      -1,    -1,   681,    -1,    -1,    -1,    -1,   705,   208,    -1,
     358,   709,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,   705,    -1,    -1,    -1,
     709,    -1,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    21,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,   464,   465,    -1,    -1,   468,   469,    -1,
      -1,    -1,    63,    64,    65,    66,   774,   775,   776,   777,
      -1,    -1,   282,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,   789,   790,    60,    -1,   774,   775,   776,   777,    -1,
      -1,    -1,    -1,    -1,   473,    96,   306,    -1,    -1,   309,
     789,   790,    -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,   473,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   127,    -1,   129,   106,
      -1,   510,    -1,    -1,   513,   112,   113,    -1,    -1,    -1,
     117,    -1,    -1,   120,   121,   122,   557,   558,   358,    -1,
      -1,    -1,   510,    -1,    -1,   513,    -1,   568,    -1,   570,
      -1,    -1,    -1,    -1,   575,    -1,    -1,   578,    -1,    -1,
     581,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   571,    -1,   573,    -1,    -1,   576,    -1,     2,
     579,    -1,    -1,   582,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,   571,    -1,   573,    19,    -1,   576,    -1,
      -1,   579,    -1,   602,   582,    -1,    -1,    -1,    -1,    32,
      -1,   610,    -1,   612,    -1,    -1,    -1,    -1,   617,    -1,
      -1,    -1,    -1,    -1,   602,    -1,    -1,    50,    -1,    -1,
      -1,    -1,   610,    -1,   612,    -1,    -1,    -1,    -1,   617,
      -1,    -1,    -1,   473,    -1,    68,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,   699,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   676,    -1,    -1,
     510,    -1,   681,   513,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   676,   122,
     127,    -1,   129,   681,   735,   736,   705,   738,   739,    -1,
     709,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   752,    -1,    -1,    -1,    12,   757,   705,   759,    -1,
      17,   709,    -1,    -1,    21,    22,    23,    -1,    -1,   162,
      -1,   571,    29,   573,    31,    -1,   576,    -1,    -1,   579,
      -1,    -1,   582,    21,    -1,    -1,    -1,    -1,    -1,    -1,
     791,    -1,    -1,   186,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   602,    -1,    -1,   774,   775,   776,   777,    -1,
     610,    -1,   612,    -1,    -1,   208,    -1,   617,    56,    -1,
     789,   790,    60,    -1,    -1,    -1,   774,   775,   776,   777,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
       9,   789,   790,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,    -1,   110,   111,   112,   113,   676,   115,    -1,   117,
      -1,   681,   120,   121,   122,    -1,   143,    56,    -1,   282,
     147,    60,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,
      -1,    -1,    -1,    -1,   161,   705,    -1,   164,    -1,   709,
      -1,    80,    -1,   306,    -1,    -1,   309,   174,    -1,   176,
      -1,    21,   179,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,   192,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      -1,   120,   121,   122,    -1,    -1,    56,    -1,    -1,    -1,
      60,   130,    -1,    -1,   221,   358,    -1,    -1,    -1,   226,
     227,   228,   229,    -1,   774,   775,   776,   777,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,   789,
     790,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,    -1,
     110,   111,   112,   113,    -1,   115,   116,   117,   275,   276,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,    -1,     9,    10,    -1,
      -1,    13,    -1,   300,    -1,   302,    -1,    -1,    -1,    21,
      -1,    23,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    -1,   320,    -1,    37,    38,    -1,    40,    -1,
      42,    43,   329,    -1,    -1,    -1,    -1,    49,    -1,    51,
     473,    -1,    -1,   340,    56,    -1,    -1,    -1,    60,    -1,
      -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,   510,    -1,    -1,
     513,    -1,    -1,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,   571,    -1,
     573,    -1,    -1,   576,    -1,    23,   579,    -1,    -1,   582,
      28,    29,    30,    -1,    32,    -1,    -1,   454,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,   602,
      -1,    49,    -1,    51,    -1,    -1,    -1,   610,    -1,   612,
     477,    -1,    -1,   480,   617,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,   501,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,   520,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,    -1,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   565,    -1,
      -1,    -1,   705,    -1,    -1,    -1,   709,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
     587,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,   635,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,
      -1,   129,    -1,    -1,    -1,   712,    -1,     1,   715,     3,
       4,     5,     6,     7,   721,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    68,    -1,    70,    71,    72,    -1,
      -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,   122,    -1,
      -1,   125,   126,   127,     1,   129,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,
      77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,
     117,   118,   119,    -1,    -1,   122,    -1,    -1,   125,   126,
     127,     1,   129,     3,     4,     5,     6,     7,    -1,    -1,
      10,    11,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    -1,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,   125,   126,   127,     1,   129,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    70,    71,    72,
      -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,   125,   126,   127,     1,   129,     3,     4,     5,
       6,     7,    -1,    -1,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    -1,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    45,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,
     116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,
     126,   127,     1,   129,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,   125,   126,   127,     1,
     129,     3,     4,     5,     6,     7,    -1,    -1,    10,    11,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    -1,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    70,    71,
      72,    -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,   125,    -1,   127,     1,   129,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    -1,    51,    52,    53,    54,
      55,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,   116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
     125,    -1,   127,     1,   129,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    53,    54,    55,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,
       1,   129,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    -1,
      51,    52,    53,    54,    55,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    70,
      71,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,   125,    -1,   127,     1,   129,     3,
       4,     5,     6,     7,    -1,    -1,    10,    11,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      -1,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,    53,
      54,    55,    -1,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    -1,    70,    71,    72,    -1,
      -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,   125,    -1,   127,     1,   129,     3,     4,     5,     6,
       7,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    -1,    25,    26,
      27,    28,    29,    30,    -1,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    -1,    45,    -1,
      -1,    48,    49,    -1,    51,    52,    53,    54,    55,    -1,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,
      77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,
     127,     1,   129,     3,     4,     5,     6,     7,    -1,    -1,
      10,    11,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    -1,    25,    26,    27,    28,    29,
      30,    -1,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    -1,    45,    -1,    -1,    48,    49,
      -1,    51,    52,    53,    54,    55,    -1,    57,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,   125,    -1,   127,     1,   129,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    54,    55,    -1,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    70,    71,    72,
      -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,   125,    -1,   127,     1,   129,     3,     4,     5,
       6,     7,    -1,    -1,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    -1,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    45,
      -1,    -1,    48,    49,    -1,    51,    52,    53,    54,    55,
      -1,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,
     116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,
      -1,   127,     1,   129,     3,     4,     5,     6,     7,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    -1,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    -1,    45,    -1,    -1,    48,
      49,    -1,    51,    52,    53,    54,    55,    -1,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,     1,
     129,     3,     4,     5,     6,     7,    -1,     9,    10,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    23,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    60,    -1,
      -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    -1,   125,    -1,   127,     1,   129,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    23,
      -1,    -1,    -1,    -1,    28,    29,    30,   112,    32,   114,
      -1,   116,   117,    37,    38,    -1,    40,   122,    42,    43,
     125,    -1,   127,    -1,   129,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   112,    10,
     114,    -1,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
      -1,   125,    23,   127,    -1,   129,   130,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,   104,    -1,     3,     4,     5,     6,     7,
      -1,   112,    10,   114,    -1,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,    -1,   125,    23,   127,    -1,   129,    -1,
      28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,     3,     4,
       5,     6,     7,    -1,   112,    10,   114,    -1,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,    -1,   125,    23,   127,
     128,   129,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    48,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    23,
      -1,    -1,    -1,    -1,    28,    29,    30,   112,    32,   114,
      -1,   116,   117,    37,    38,    -1,    40,   122,    42,    43,
     125,    -1,   127,    -1,   129,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      -1,    -1,     3,     4,     5,     6,     7,    -1,   112,    10,
     114,    -1,   116,   117,   118,    -1,    -1,    -1,   122,    -1,
      -1,   125,    23,   127,    -1,   129,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    -1,    -1,     3,     4,     5,     6,     7,
      -1,   112,    10,   114,    -1,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,    -1,   125,    23,   127,    -1,   129,    -1,
      28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,
      -1,   129,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    23,    -1,    -1,    -1,    -1,    28,    29,
      30,   112,    32,   114,    -1,   116,   117,    37,    38,    -1,
      40,   122,    42,    43,   125,    -1,   127,    -1,   129,    49,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    -1,    -1,     3,     4,     5,     6,
       7,    -1,   112,    10,   114,    -1,   116,   117,    -1,    -1,
      -1,    -1,   122,    -1,    -1,   125,    23,   127,    -1,   129,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    10,    -1,    -1,    -1,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,    23,    -1,    -1,
      -1,    -1,    28,    29,    30,   112,    32,   114,    -1,   116,
     117,    37,    38,    -1,    40,   122,    42,    43,   125,    -1,
     127,    -1,   129,    49,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,
       3,     4,     5,     6,     7,    -1,   112,    10,   114,    -1,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,    -1,   125,
      23,   127,    -1,   129,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,     3,     4,     5,     6,     7,    -1,   112,
      10,   114,    -1,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,    -1,   125,    23,   127,    -1,   129,    -1,    28,    29,
      30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    -1,    -1,     3,     4,     5,     6,
       7,    -1,   112,    10,   114,    -1,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,    -1,   125,    23,   127,    -1,   129,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    10,    -1,    -1,    -1,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,    23,    -1,    -1,
      -1,    -1,    28,    29,    30,   112,    32,   114,    -1,   116,
     117,    37,    38,    -1,    40,   122,    42,    43,   125,    -1,
     127,    -1,   129,    49,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   112,    32,   114,    -1,
     116,   117,    37,    38,    -1,    40,   122,    42,    43,   125,
      -1,   127,    -1,   129,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    23,
      -1,    -1,    -1,    -1,    28,    29,    30,   112,    32,   114,
      -1,   116,   117,    37,    38,    -1,    40,   122,    42,    43,
     125,    -1,   127,    -1,   129,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   112,    32,
     114,    -1,   116,   117,    37,    38,    -1,    40,   122,    42,
      43,   125,    -1,   127,    -1,   129,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    23,    -1,    -1,    -1,    -1,    28,    29,    30,   112,
      32,   114,    -1,   116,   117,    37,    38,    -1,    40,   122,
      42,    43,   125,    -1,   127,    -1,   129,    49,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,   103,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     112,    32,   114,    -1,   116,   117,    37,    38,    -1,    40,
     122,    42,    43,   125,    -1,   127,    -1,   129,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    23,    -1,    -1,    -1,    -1,    28,    29,
      30,   112,    32,   114,    -1,   116,   117,    37,    38,    -1,
      40,   122,    42,    43,   125,    -1,   127,    -1,   129,    49,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   112,    32,   114,    -1,   116,   117,    37,    38,
      -1,    40,   122,    42,    43,   125,    -1,   127,    -1,   129,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    10,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    23,    -1,    -1,    -1,    -1,
      28,    29,    30,   112,    32,   114,    -1,   116,   117,    37,
      38,    -1,    40,   122,    42,    43,   125,    -1,   127,    -1,
     129,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   112,    32,   114,    -1,    -1,   117,
      37,    38,    -1,    40,   122,    42,    43,   125,    -1,   127,
      -1,   129,    49,    -1,    51,     3,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
     127,   128,   129,    81,    82,    83,    84,    85,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,   101,    -1,    -1,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,   117,
      -1,    -1,   120,   121,   122,   123,   124,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    56,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,     9,    -1,    -1,    80,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    56,    -1,
      -1,    -1,    60,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    80,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    56,    -1,    -1,    -1,    60,    95,   130,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    80,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    95,   130,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
       9,   115,   116,   117,    13,    -1,   120,   121,   122,    -1,
      -1,    -1,    21,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      56,   120,   121,   122,    60,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,     9,   115,
     116,   117,    13,    -1,   120,   121,   122,    -1,    -1,   125,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    56,   120,
     121,   122,    60,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,     9,
      -1,    -1,    80,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    56,   125,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    56,
     120,   121,   122,    60,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    56,   120,   121,   122,    60,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    56,   120,   121,   122,    60,
      -1,   125,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    56,   120,
     121,   122,    60,    -1,   125,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    56,   120,   121,   122,    60,    -1,   125,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    80,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,    56,   120,   121,   122,    60,    -1,
     125,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    56,   120,   121,
     122,    60,    -1,   125,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      56,   120,   121,   122,    60,    -1,   125,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    56,   120,   121,   122,    60,    -1,   125,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    56,   120,   121,   122,
      60,    -1,   125,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    56,
     120,   121,   122,    60,    -1,   125,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    56,   115,   116,
     117,    60,    -1,   120,   121,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      56,   120,   121,   122,    60,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,     9,    -1,    80,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    31,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,    -1,    -1,    56,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    56,   120,   121,   122,    60,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      56,   115,   116,   117,    60,    -1,   120,   121,   122,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    56,   115,
     116,   117,    60,   119,   120,   121,   122,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    56,   115,   116,   117,
      60,   119,   120,   121,   122,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,   119,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    79,    80,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    56,   120,   121,
     122,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      56,   120,   121,   122,    60,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    56,   115,
     116,   117,    60,   119,   120,   121,   122,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    56,   115,   116,   117,
      60,   119,   120,   121,   122,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,   119,
     120,   121,   122,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    56,   115,   116,   117,    60,   119,   120,   121,
     122,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      56,   115,   116,   117,    60,   119,   120,   121,   122,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    56,   115,
     116,   117,    60,   119,   120,   121,   122,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    56,   115,   116,   117,
      60,   119,   120,   121,   122,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,   119,
     120,   121,   122,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    56,   115,   116,   117,    60,   119,   120,   121,
     122,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      56,   115,   116,   117,    60,   119,   120,   121,   122,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    56,   115,
     116,   117,    60,   119,   120,   121,   122,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    56,   115,   116,   117,
      60,   119,   120,   121,   122,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,   119,
     120,   121,   122,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    56,   115,   116,   117,    60,   119,   120,   121,
     122,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      56,   115,   116,   117,    60,   119,   120,   121,   122,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    56,   115,
     116,   117,    60,    -1,   120,   121,   122,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    80,    -1,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    56,   115,   116,   117,
      60,   119,   120,   121,   122,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,    -1,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    79,    80,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    56,   120,   121,
     122,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      56,   120,   121,   122,    60,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    56,   115,
     116,   117,    60,    -1,   120,   121,   122,    -1,    -1,    67,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    56,   120,   121,   122,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    80,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,    56,   120,   121,   122,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    56,   120,   121,
     122,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      56,   120,   121,   122,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    56,   120,   121,   122,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    56,   120,   121,   122,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    21,    56,
     120,   121,   122,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    60,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    80,   115,   116,
     117,    -1,    56,   120,   121,   122,    60,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,    80,   110,   111,   112,
     113,    -1,   115,    -1,   117,    -1,    -1,   120,   121,   122,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    -1,   120,   121,   122
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   135,   136,     0,     1,     3,     4,     5,     6,     7,
      10,    11,    12,    14,    15,    16,    17,    19,    20,    22,
      23,    25,    26,    27,    28,    29,    30,    32,    33,    36,
      37,    38,    39,    40,    42,    43,    45,    49,    50,    51,
      52,    54,    55,    58,    59,    61,    62,    63,    64,    65,
      66,    68,    70,    71,    75,    77,    80,    95,    96,    97,
      98,   103,   112,   114,   116,   117,   119,   122,   125,   127,
     129,   137,   138,   139,   140,   141,   142,   144,   145,   146,
     148,   151,   152,   153,   154,   155,   157,   158,   161,   162,
     163,   166,   169,   172,   173,   193,   196,   197,   215,   216,
     217,   218,   219,   220,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   236,   237,   238,   239,   240,   139,
     228,    76,   221,   222,   147,   148,   221,    10,    28,    29,
      30,    66,   125,   129,   169,   215,   219,   226,   227,   228,
     229,   231,   232,    68,   147,   228,   139,   127,   148,     7,
     147,   149,     8,    68,   149,    48,    78,   156,   228,   228,
     228,   127,   148,   170,   127,   148,   198,   199,   139,   228,
     228,   228,   228,     7,   127,    17,    27,   153,   153,   118,
     189,   209,   228,   119,   228,   228,    22,   142,   150,   228,
     228,    65,   127,   139,   228,    61,   148,   194,   209,   228,
     228,   228,   228,   228,   228,   228,   126,   137,   143,   209,
      69,   104,   189,   210,   211,   228,   209,   228,   235,    50,
     139,    41,   148,    35,    53,   185,    18,    48,    57,    72,
     119,   124,     9,    13,    21,    56,    60,    80,    95,    97,
      98,    99,   101,   102,   103,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   115,   116,   117,   120,   121,   122,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,   123,   127,   129,    56,    60,   127,
     139,   119,   125,   142,   228,   228,   228,   209,    31,   221,
     194,   119,   119,    75,   148,   189,   212,   213,   214,   228,
     125,   194,   163,   148,   127,   150,    22,    31,   150,    22,
      31,   150,   222,    67,   142,   212,   139,   127,   180,    69,
     127,   148,   200,   201,     1,    99,   203,   204,    31,   100,
     150,   212,   149,   148,   100,   119,   119,   125,   139,   150,
     127,   212,   209,    81,   195,   119,   119,   150,   119,   126,
     137,   126,   228,   100,   128,   100,   128,    31,   130,   222,
      79,   100,   130,     7,   148,    99,   164,   174,    57,   198,
     198,   198,   198,   228,   228,   228,   228,   156,   228,   156,
     228,   228,   228,   228,   228,   228,   228,    23,    68,   148,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   212,   212,   156,   228,   156,   228,    18,    31,    34,
      47,    48,    57,    68,   117,   183,   223,   224,   240,    22,
      31,    22,    31,    67,    31,   130,   156,   228,   150,   119,
     228,    79,    81,   128,   100,   148,   167,   168,   119,   148,
      31,   209,    30,   228,   156,   228,    30,   228,   156,   228,
     150,   139,   228,    24,   128,   181,   182,   183,   171,   201,
     100,   128,     1,   129,   205,   216,    79,    81,   202,   228,
     199,   128,   189,   228,   159,   212,   128,   125,   129,   205,
     216,   100,   128,   128,   210,   210,   156,   228,   139,   228,
     130,   228,   228,   125,   209,   125,    48,    57,    68,   176,
     184,   198,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   128,   130,    31,    57,   215,   100,   128,    56,    30,
     156,   228,    30,   156,   228,   156,   228,   221,   221,   119,
     228,   189,   228,   214,    81,   100,   126,   164,   228,   128,
     228,    22,   150,    22,   150,   228,    22,   150,   222,    22,
     150,   222,    24,    24,   139,   100,   128,   127,   148,    18,
      48,    57,    68,   186,   128,   201,    99,   204,   209,   228,
      44,   228,    46,    73,   126,   160,   128,   159,   209,   194,
     130,   222,   130,   222,   139,    79,   126,   143,   165,   175,
       9,    13,    81,    82,    83,    84,    85,    86,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,   101,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     117,   120,   121,   122,   123,   124,   148,   177,   178,   119,
     224,   215,   228,    22,    22,   228,    22,    22,   130,   130,
     150,   150,   228,   168,   125,   150,    67,    30,   228,    30,
     228,    67,    30,   228,   150,    30,   228,   150,   139,   228,
     182,   201,    99,   208,   204,   202,    31,   130,   139,   209,
     126,   130,    30,   139,   228,   130,    30,   139,   228,   130,
     228,   126,    27,    52,    54,    68,   126,   138,   152,   186,
     148,   102,   127,   179,   179,    67,    30,    30,    67,    30,
      30,    30,    30,   165,   228,   228,   228,   228,   228,   228,
     128,    23,    63,    66,   129,   189,   207,   216,   104,   191,
     202,    74,   192,   228,   205,   216,   150,   228,   139,   228,
     139,   152,   204,   148,   177,   178,   181,   228,   228,   228,
     228,   228,   228,   126,    67,    67,    67,    67,   208,   130,
     209,   189,   190,   228,   228,   142,   151,   188,   130,    67,
      67,   192,   179,   179,   128,    67,    67,    67,    67,    67,
      67,   228,   228,   228,   228,   191,   202,   189,   206,   207,
     216,    31,   130,   216,   228,   228,   119,   187,   188,   228,
     206,   207,   130,   206
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

  case 21:

/* Line 1806 of yacc.c  */
#line 438 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 451 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 452 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 545 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 558 "chapel.ypp"
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

  case 81:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 601 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 607 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 613 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 658 "chapel.ypp"
    { USR_FATAL((yyvsp[(3) - (6)].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 665 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 670 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 677 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 682 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 687 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 710 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 720 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 729 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 733 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 756 "chapel.ypp"
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

  case 133:

/* Line 1806 of yacc.c  */
#line 776 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 782 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 789 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 797 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 808 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 814 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 821 "chapel.ypp"
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

  case 140:

/* Line 1806 of yacc.c  */
#line 843 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 852 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 856 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 962 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 966 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1003 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1008 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
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

  case 230:

/* Line 1806 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1046 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1053 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1060 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1067 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
    }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1075 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1091 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1099 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1101 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1114 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1123 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1146 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1171 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1175 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1182 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1193 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1286 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1315 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1322 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1328 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1340 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1383 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1385 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1393 "chapel.ypp"
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[(1) - (2)].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[(2) - (2)].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[(2) - (2)].pexpr);
    }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1408 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1442 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7460 "bison-chapel.cpp"
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



