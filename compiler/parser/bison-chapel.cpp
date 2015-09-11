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
     CSTRINGLITERAL = 263,
     EXTERNCODE = 264,
     TALIGN = 265,
     TATOMIC = 266,
     TBEGIN = 267,
     TBREAK = 268,
     TBY = 269,
     TCLASS = 270,
     TCOBEGIN = 271,
     TCOFORALL = 272,
     TCONFIG = 273,
     TCONST = 274,
     TCONTINUE = 275,
     TDELETE = 276,
     TDMAPPED = 277,
     TDO = 278,
     TDOMAIN = 279,
     TELSE = 280,
     TENUM = 281,
     TEXPORT = 282,
     TEXTERN = 283,
     TFOR = 284,
     TFORALL = 285,
     TIF = 286,
     TIN = 287,
     TINDEX = 288,
     TINLINE = 289,
     TINOUT = 290,
     TITER = 291,
     TLABEL = 292,
     TLAMBDA = 293,
     TLET = 294,
     TLOCAL = 295,
     TMINUSMINUS = 296,
     TMODULE = 297,
     TNEW = 298,
     TNIL = 299,
     TNOINIT = 300,
     TON = 301,
     TOTHERWISE = 302,
     TOUT = 303,
     TPARAM = 304,
     TPLUSPLUS = 305,
     TPRAGMA = 306,
     TPRIMITIVE = 307,
     TPRIVATE = 308,
     TPROC = 309,
     TPUBLIC = 310,
     TRECORD = 311,
     TREDUCE = 312,
     TREF = 313,
     TREQUIRE = 314,
     TRETURN = 315,
     TSCAN = 316,
     TSELECT = 317,
     TSERIAL = 318,
     TSINGLE = 319,
     TSPARSE = 320,
     TSUBDOMAIN = 321,
     TSYNC = 322,
     TTHEN = 323,
     TTYPE = 324,
     TUNDERSCORE = 325,
     TUNION = 326,
     TUSE = 327,
     TVAR = 328,
     TWHEN = 329,
     TWHERE = 330,
     TWHILE = 331,
     TWITH = 332,
     TYIELD = 333,
     TZIP = 334,
     TALIAS = 335,
     TAND = 336,
     TASSIGN = 337,
     TASSIGNBAND = 338,
     TASSIGNBOR = 339,
     TASSIGNBXOR = 340,
     TASSIGNDIVIDE = 341,
     TASSIGNEXP = 342,
     TASSIGNLAND = 343,
     TASSIGNLOR = 344,
     TASSIGNMINUS = 345,
     TASSIGNMOD = 346,
     TASSIGNMULTIPLY = 347,
     TASSIGNPLUS = 348,
     TASSIGNSL = 349,
     TASSIGNSR = 350,
     TBAND = 351,
     TBNOT = 352,
     TBOR = 353,
     TBXOR = 354,
     TCOLON = 355,
     TCOMMA = 356,
     TDIVIDE = 357,
     TDOT = 358,
     TDOTDOT = 359,
     TDOTDOTDOT = 360,
     TEQUAL = 361,
     TEXP = 362,
     TGREATER = 363,
     TGREATEREQUAL = 364,
     THASH = 365,
     TLESS = 366,
     TLESSEQUAL = 367,
     TMINUS = 368,
     TMOD = 369,
     TNOT = 370,
     TNOTEQUAL = 371,
     TOR = 372,
     TPLUS = 373,
     TQUESTION = 374,
     TSEMI = 375,
     TSHIFTLEFT = 376,
     TSHIFTRIGHT = 377,
     TSTAR = 378,
     TSWAP = 379,
     TIO = 380,
     TLCBR = 381,
     TRCBR = 382,
     TLP = 383,
     TRP = 384,
     TLSBR = 385,
     TRSBR = 386,
     TNOELSE = 387,
     TUMINUS = 388,
     TUPLUS = 389
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
#line 425 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 431 "bison-chapel.cpp"
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
#line 505 "bison-chapel.cpp"

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
#define YYLAST   10949

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  135
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  107
/* YYNRULES -- Number of rules.  */
#define YYNRULES  431
/* YYNRULES -- Number of states.  */
#define YYNSTATES  825

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   389

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
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134
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
    1298,  1300,  1302,  1304,  1308,  1312,  1316,  1320,  1326,  1330,
    1334,  1338,  1342,  1346,  1350,  1354,  1358,  1362,  1366,  1370,
    1374,  1378,  1382,  1386,  1390,  1394,  1398,  1402,  1406,  1410,
    1414,  1418,  1421,  1424,  1427,  1430,  1433,  1436,  1440,  1444,
    1448,  1452,  1456,  1460,  1464,  1468,  1470,  1472,  1474,  1476,
    1478,  1480
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     136,     0,    -1,   137,    -1,    -1,   137,   138,    -1,   140,
      -1,   139,   140,    -1,    51,     7,    -1,   139,    51,     7,
      -1,   141,    -1,   143,    -1,   145,    -1,   146,    -1,   153,
      -1,   147,    -1,   155,    -1,   158,    -1,   156,    -1,   159,
      -1,   162,    -1,   152,    -1,   221,   120,    -1,    11,   140,
      -1,    12,   222,   140,    -1,    13,   148,   120,    -1,    16,
     222,   143,    -1,    20,   148,   120,    -1,    21,   229,   120,
      -1,    37,   149,   140,    -1,    40,   140,    -1,    46,   229,
     151,    -1,    63,   229,   151,    -1,    63,   151,    -1,    67,
     140,    -1,    78,   229,   120,    -1,     1,    -1,   142,    42,
     149,   126,   127,    -1,   142,    42,   149,   126,   144,   127,
      -1,    -1,    55,    -1,    53,    -1,   126,   127,    -1,   126,
     144,   127,    -1,   138,    -1,   144,   138,    -1,    72,   210,
     120,    -1,    59,   210,   120,    -1,   230,    82,   229,   120,
      -1,   230,    93,   229,   120,    -1,   230,    90,   229,   120,
      -1,   230,    92,   229,   120,    -1,   230,    86,   229,   120,
      -1,   230,    91,   229,   120,    -1,   230,    87,   229,   120,
      -1,   230,    83,   229,   120,    -1,   230,    84,   229,   120,
      -1,   230,    85,   229,   120,    -1,   230,    95,   229,   120,
      -1,   230,    94,   229,   120,    -1,   230,   124,   229,   120,
      -1,   230,    88,   229,   120,    -1,   230,    89,   229,   120,
      -1,    -1,   149,    -1,     3,    -1,   148,    -1,     7,    -1,
      23,   140,    -1,   143,    -1,    60,   120,    -1,    60,   229,
     120,    -1,   120,    -1,   154,    -1,    55,   154,    -1,    53,
     154,    -1,   167,    -1,   194,    -1,   163,    -1,   174,    -1,
     197,    -1,    28,     9,    -1,    23,   140,    76,   229,   120,
      -1,    76,   229,   151,    -1,    17,   229,    32,   229,   222,
     151,    -1,    17,   229,    32,   157,   222,   151,    -1,    17,
     229,   222,   151,    -1,    29,   229,    32,   229,   151,    -1,
      29,   229,    32,   157,   151,    -1,    29,   229,   151,    -1,
      29,   157,   151,    -1,    29,    49,   149,    32,   229,   151,
      -1,    30,   229,    32,   229,   151,    -1,    30,   229,    32,
     229,   223,   151,    -1,    30,   229,    32,   157,   151,    -1,
      30,   229,    32,   157,   223,   151,    -1,    30,   229,   151,
      -1,    30,   229,   223,   151,    -1,   130,   210,    32,   229,
     131,   140,    -1,   130,   210,    32,   229,   223,   131,   140,
      -1,   130,   210,    32,   157,   131,   140,    -1,   130,   210,
      32,   157,   223,   131,   140,    -1,   130,   210,   131,   140,
      -1,   130,   210,   223,   131,   140,    -1,    79,   128,   210,
     129,    -1,    31,   229,    68,   140,    -1,    31,   229,   143,
      -1,    31,   229,    68,   140,    25,   140,    -1,    31,   229,
     143,    25,   140,    -1,    62,   229,   126,   160,   127,    -1,
      -1,   160,   161,    -1,    74,   210,   151,    -1,    47,   140,
      -1,    69,    62,   210,   126,   160,   127,    -1,   164,   149,
     165,   126,   166,   127,    -1,    28,   150,   164,   149,   165,
     126,   166,   127,    -1,    15,    -1,    56,    -1,    71,    -1,
      -1,   100,   210,    -1,    -1,   166,   153,    -1,   166,   139,
     153,    -1,    26,   149,   126,   168,   127,    -1,   169,    -1,
     168,   101,    -1,   168,   101,   169,    -1,   149,    -1,   149,
      82,   229,    -1,    -1,    -1,    38,   171,   181,   172,   187,
     205,   193,   189,    -1,    -1,    34,    -1,    27,   150,    -1,
      28,   150,    -1,    -1,    -1,   173,   186,   175,   177,   176,
     187,   205,   193,   188,    -1,   185,   178,   180,    -1,   185,
     179,   180,    -1,   185,   149,   103,   178,   180,    -1,   185,
     149,   103,   179,   180,    -1,   149,    -1,    97,   149,    -1,
      96,    -1,    98,    -1,    99,    -1,    97,    -1,   106,    -1,
     116,    -1,   112,    -1,   109,    -1,   111,    -1,   108,    -1,
     118,    -1,   113,    -1,   123,    -1,   102,    -1,   121,    -1,
     122,    -1,   114,    -1,   107,    -1,   115,    -1,    14,    -1,
     110,    -1,    10,    -1,   124,    -1,   125,    -1,    82,    -1,
      93,    -1,    90,    -1,    92,    -1,    86,    -1,    91,    -1,
      87,    -1,    83,    -1,    84,    -1,    85,    -1,    95,    -1,
      94,    -1,    -1,   128,   182,   129,    -1,   128,   182,   129,
      -1,    -1,   183,    -1,   182,   101,   183,    -1,   184,   149,
     209,   203,    -1,   184,   149,   209,   192,    -1,   184,   128,
     202,   129,   209,   203,    -1,   184,   128,   202,   129,   209,
     192,    -1,    -1,    32,    -1,    35,    -1,    48,    -1,    19,
      -1,    19,    32,    -1,    19,    58,    -1,    49,    -1,    58,
      -1,    69,    -1,    -1,    49,    -1,    58,    -1,    69,    -1,
      54,    -1,    36,    -1,    -1,    19,    -1,    58,    -1,    49,
      -1,    69,    -1,   120,    -1,   189,    -1,   143,    -1,   152,
      -1,   119,   149,    -1,   119,    -1,    -1,   190,    -1,   105,
     229,    -1,   105,   191,    -1,    -1,    75,   229,    -1,    69,
     195,   120,    -1,    18,    69,   195,   120,    -1,    28,    69,
     195,   120,    -1,   149,   196,    -1,   149,   196,   101,   195,
      -1,    -1,    82,   217,    -1,    82,   206,    -1,   198,    49,
     199,   120,    -1,   198,    19,   199,   120,    -1,   198,    58,
     199,   120,    -1,   198,    19,    58,   199,   120,    -1,   198,
      73,   199,   120,    -1,    -1,    18,    -1,    28,    -1,   200,
      -1,   199,   101,   200,    -1,   149,   205,   203,    -1,   149,
     204,    80,   229,    -1,   128,   202,   129,   205,   203,    -1,
      70,    -1,   149,    -1,   128,   202,   129,    -1,   201,    -1,
     201,   101,    -1,   201,   101,   202,    -1,    -1,    82,    45,
      -1,    82,   229,    -1,    -1,   100,   130,   210,   131,    -1,
      -1,   100,   217,    -1,   100,   206,    -1,     1,    -1,   130,
     210,   131,   217,    -1,   130,   210,   131,   206,    -1,   130,
     210,    32,   229,   131,   217,    -1,     1,    -1,    -1,   217,
      -1,   190,    -1,   130,   131,   207,    -1,   130,   210,   131,
     207,    -1,   130,   131,   208,    -1,   130,   210,   131,   208,
      -1,   130,   210,    32,   229,   131,   207,    -1,    -1,   100,
     217,    -1,   100,   190,    -1,   100,    24,    -1,   100,    64,
      -1,   100,    67,    -1,   100,   208,    -1,   229,    -1,   190,
      -1,   210,   101,   229,    -1,   210,   101,   190,    -1,    70,
      -1,   229,    -1,   190,    -1,   211,   101,   211,    -1,   212,
     101,   211,    -1,    -1,   214,    -1,   215,    -1,   214,   101,
     215,    -1,   149,    82,   190,    -1,   149,    82,   229,    -1,
     149,    80,   229,    -1,   190,    -1,   229,    -1,   149,    -1,
     220,    -1,   230,    -1,   219,    -1,   238,    -1,   237,    -1,
      64,   229,    -1,    33,   128,   213,   129,    -1,    24,   128,
     213,   129,    -1,    66,   128,   213,   129,    -1,    65,    66,
     128,   213,   129,    -1,    11,   229,    -1,    67,   229,    -1,
      29,   229,    32,   229,    23,   229,    -1,    29,   229,    32,
     157,    23,   229,    -1,    29,   229,    23,   229,    -1,    29,
     229,    32,   229,    23,    31,   229,    68,   229,    -1,    29,
     229,    32,   157,    23,    31,   229,    68,   229,    -1,    29,
     229,    23,    31,   229,    68,   229,    -1,    30,   229,    32,
     229,    23,   229,    -1,    30,   229,    32,   157,    23,   229,
      -1,    30,   229,    23,   229,    -1,    30,   229,    32,   229,
      23,    31,   229,    68,   229,    -1,    30,   229,    32,   157,
      23,    31,   229,    68,   229,    -1,    30,   229,    23,    31,
     229,    68,   229,    -1,   130,   210,   131,   229,    -1,   130,
     210,    32,   229,   131,   229,    -1,   130,   210,    32,   157,
     131,   229,    -1,   130,   210,    32,   229,   131,    31,   229,
      68,   229,    -1,   130,   210,    32,   157,   131,    31,   229,
      68,   229,    -1,    31,   229,    68,   229,    25,   229,    -1,
      44,    -1,   220,    -1,   216,    -1,   233,    -1,   232,    -1,
     170,    -1,   227,    -1,   228,    -1,   226,   125,   229,    -1,
      -1,   223,    -1,    77,   128,   224,   129,    -1,   225,    -1,
     224,   101,   225,    -1,   184,   216,    -1,   241,    57,   216,
      -1,   230,    -1,   226,   125,   229,    -1,    43,   229,    -1,
      39,   199,    32,   229,    -1,   235,    -1,   217,    -1,   218,
      -1,   239,    -1,   240,    -1,   170,    -1,   227,    -1,   228,
      -1,   128,   105,   229,   129,    -1,   229,   100,   229,    -1,
     229,   104,   229,    -1,   229,   104,    -1,   104,   229,    -1,
     104,    -1,   216,    -1,   232,    -1,   233,    -1,   234,    -1,
     230,    -1,   170,    -1,   231,   128,   213,   129,    -1,   231,
     130,   213,   131,    -1,    52,   128,   213,   129,    -1,   229,
     103,   149,    -1,   229,   103,    69,    -1,   229,   103,    24,
      -1,   128,   211,   129,    -1,   128,   211,   101,   129,    -1,
     128,   212,   129,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,   126,   210,   127,    -1,   130,   210,
     131,    -1,   130,   236,   131,    -1,   229,    80,   229,    -1,
     236,   101,   229,    80,   229,    -1,   229,   118,   229,    -1,
     229,   113,   229,    -1,   229,   123,   229,    -1,   229,   102,
     229,    -1,   229,   121,   229,    -1,   229,   122,   229,    -1,
     229,   114,   229,    -1,   229,   106,   229,    -1,   229,   116,
     229,    -1,   229,   112,   229,    -1,   229,   109,   229,    -1,
     229,   111,   229,    -1,   229,   108,   229,    -1,   229,    96,
     229,    -1,   229,    98,   229,    -1,   229,    99,   229,    -1,
     229,    81,   229,    -1,   229,   117,   229,    -1,   229,   107,
     229,    -1,   229,    14,   229,    -1,   229,    10,   229,    -1,
     229,   110,   229,    -1,   229,    22,   229,    -1,   118,   229,
      -1,   113,   229,    -1,    41,   229,    -1,    50,   229,    -1,
     115,   229,    -1,    97,   229,    -1,   229,    57,   229,    -1,
     229,    57,   157,    -1,   241,    57,   229,    -1,   241,    57,
     157,    -1,   229,    61,   229,    -1,   229,    61,   157,    -1,
     241,    61,   229,    -1,   241,    61,   157,    -1,   118,    -1,
     123,    -1,    81,    -1,   117,    -1,    96,    -1,    98,    -1,
      99,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   403,   403,   408,   409,   415,   416,   421,   422,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   457,   459,   464,   465,
     466,   481,   482,   487,   488,   493,   497,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   519,   520,   524,   528,   529,   533,   534,   538,
     539,   543,   544,   545,   546,   547,   548,   549,   553,   554,
     558,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   584,   585,   586,   587,   588,   589,   595,   601,
     607,   613,   620,   630,   634,   635,   636,   637,   641,   646,
     647,   651,   653,   658,   665,   670,   678,   683,   688,   696,
     697,   702,   703,   705,   710,   720,   729,   733,   741,   742,
     747,   752,   746,   777,   783,   790,   798,   809,   815,   808,
     843,   847,   852,   856,   864,   865,   866,   867,   868,   869,
     870,   871,   872,   873,   874,   875,   876,   877,   878,   879,
     880,   881,   882,   883,   884,   885,   886,   887,   888,   889,
     893,   894,   895,   896,   897,   898,   899,   900,   901,   902,
     903,   904,   908,   909,   913,   917,   918,   919,   923,   925,
     927,   929,   934,   935,   936,   937,   938,   939,   940,   941,
     942,   943,   947,   948,   949,   950,   954,   955,   959,   960,
     961,   962,   963,   967,   968,   972,   973,   977,   979,   984,
     985,   989,   990,   994,   995,   999,  1001,  1003,  1008,  1021,
    1038,  1039,  1041,  1046,  1053,  1060,  1067,  1075,  1084,  1085,
    1086,  1090,  1091,  1099,  1101,  1107,  1112,  1114,  1116,  1121,
    1123,  1125,  1132,  1133,  1134,  1138,  1139,  1144,  1145,  1146,
    1147,  1167,  1171,  1175,  1183,  1187,  1188,  1189,  1193,  1195,
    1201,  1203,  1205,  1210,  1211,  1212,  1213,  1214,  1215,  1216,
    1222,  1223,  1224,  1225,  1229,  1230,  1231,  1235,  1236,  1240,
    1241,  1245,  1246,  1250,  1251,  1252,  1253,  1254,  1258,  1269,
    1270,  1271,  1272,  1273,  1274,  1276,  1278,  1280,  1282,  1284,
    1286,  1291,  1293,  1295,  1297,  1299,  1301,  1303,  1305,  1307,
    1309,  1311,  1313,  1315,  1322,  1328,  1334,  1340,  1349,  1354,
    1362,  1363,  1364,  1365,  1366,  1367,  1368,  1369,  1374,  1375,
    1379,  1383,  1385,  1393,  1403,  1407,  1408,  1413,  1418,  1426,
    1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,  1436,  1438,
    1440,  1442,  1444,  1449,  1450,  1451,  1452,  1463,  1464,  1468,
    1469,  1470,  1474,  1475,  1476,  1484,  1485,  1486,  1490,  1491,
    1492,  1493,  1494,  1495,  1496,  1497,  1504,  1505,  1509,  1510,
    1511,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,  1520,
    1521,  1522,  1523,  1524,  1525,  1526,  1527,  1528,  1529,  1530,
    1531,  1535,  1536,  1537,  1538,  1539,  1540,  1544,  1545,  1546,
    1547,  1551,  1552,  1553,  1554,  1559,  1560,  1561,  1562,  1563,
    1564,  1565
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "INTLITERAL", "REALLITERAL",
  "IMAGLITERAL", "STRINGLITERAL", "CSTRINGLITERAL", "EXTERNCODE", "TALIGN",
  "TATOMIC", "TBEGIN", "TBREAK", "TBY", "TCLASS", "TCOBEGIN", "TCOFORALL",
  "TCONFIG", "TCONST", "TCONTINUE", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXPORT", "TEXTERN", "TFOR", "TFORALL",
  "TIF", "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER", "TLABEL",
  "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW", "TNIL",
  "TNOINIT", "TON", "TOTHERWISE", "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA",
  "TPRIMITIVE", "TPRIVATE", "TPROC", "TPUBLIC", "TRECORD", "TREDUCE",
  "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSINGLE",
  "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE", "TUNDERSCORE",
  "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD",
  "TZIP", "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR",
  "TASSIGNBXOR", "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND",
  "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY",
  "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR",
  "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT",
  "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH", "TLESS",
  "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS",
  "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP",
  "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE",
  "TUMINUS", "TUPLUS", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "module_decl_stmt",
  "access_control", "block_stmt", "stmt_ls", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_ident", "ident", "opt_ident_or_string",
  "do_stmt", "return_stmt", "class_level_stmt", "private_decl",
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
     385,   386,   387,   388,   389
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   135,   136,   137,   137,   138,   138,   139,   139,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   141,   141,   142,   142,
     142,   143,   143,   144,   144,   145,   146,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   148,   148,   149,   150,   150,   151,   151,   152,
     152,   153,   153,   153,   153,   153,   153,   153,   154,   154,
     155,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   157,   158,   158,   158,   158,   159,   160,
     160,   161,   161,   162,   163,   163,   164,   164,   164,   165,
     165,   166,   166,   166,   167,   168,   168,   168,   169,   169,
     171,   172,   170,   173,   173,   173,   173,   175,   176,   174,
     177,   177,   177,   177,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   180,   180,   181,   182,   182,   182,   183,   183,
     183,   183,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   185,   185,   185,   185,   186,   186,   187,   187,
     187,   187,   187,   188,   188,   189,   189,   190,   190,   191,
     191,   192,   192,   193,   193,   194,   194,   194,   195,   195,
     196,   196,   196,   197,   197,   197,   197,   197,   198,   198,
     198,   199,   199,   200,   200,   200,   201,   201,   201,   202,
     202,   202,   203,   203,   203,   204,   204,   205,   205,   205,
     205,   206,   206,   206,   206,   207,   207,   207,   208,   208,
     208,   208,   208,   209,   209,   209,   209,   209,   209,   209,
     210,   210,   210,   210,   211,   211,   211,   212,   212,   213,
     213,   214,   214,   215,   215,   215,   215,   215,   216,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   219,   220,
     221,   221,   221,   221,   221,   221,   221,   221,   222,   222,
     223,   224,   224,   225,   225,   226,   226,   227,   228,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   230,   230,   230,   230,   231,   231,   232,
     232,   232,   233,   233,   233,   234,   234,   234,   235,   235,
     235,   235,   235,   235,   235,   235,   236,   236,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   238,   238,   238,   238,   238,   238,   239,   239,   239,
     239,   240,   240,   240,   240,   241,   241,   241,   241,   241,
     241,   241
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
       1,     1,     1,     3,     3,     3,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    35,    64,   378,   379,   380,   381,
     382,     0,   338,    62,   116,   338,     0,   239,    62,     0,
       0,     0,     0,    62,    62,     0,     0,     0,     0,   134,
       0,   130,     0,     0,     0,     0,   329,     0,     0,     0,
       0,   238,   238,   117,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,   427,   429,     0,
     430,   431,   362,     0,     0,   428,   425,    71,   426,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   298,    20,    13,    72,    15,    17,    16,    18,    19,
      77,     0,    75,   354,     0,    78,    76,    79,     0,   363,
     350,   351,   301,   299,     0,     0,   355,   356,     0,   300,
       0,   364,   365,   366,   349,   303,   302,   352,   353,     0,
      22,   309,     0,     0,   339,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,   354,   363,   299,   355,   356,
     338,   300,   364,   365,     0,     0,     0,     0,   289,     0,
      66,    65,   135,    80,     0,   136,     0,     0,     0,     0,
       0,     0,   289,     0,     0,     0,     0,     0,   241,    29,
     413,   347,     0,   414,     7,   289,   239,   240,    74,    73,
     218,   281,     0,   280,    69,     0,     0,     0,    68,    32,
       0,   304,     0,   289,    33,   310,     0,   230,     0,     0,
       0,     0,   416,   361,   412,   415,   411,    41,    43,     0,
       0,   284,     0,   286,     0,     0,   285,     0,   280,     0,
       0,     6,     0,   119,   207,   206,   137,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   360,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   289,     0,     0,
     192,    23,    24,     0,    25,     0,     0,     0,     0,     0,
       0,     0,    26,    27,     0,   298,   296,     0,   290,   291,
     297,     0,     0,     0,     0,     0,    89,     0,     0,    88,
       0,     0,    95,     0,     0,   105,     0,    28,   185,   131,
     246,     0,   247,   249,     0,   260,     0,     0,   252,     0,
       0,    30,     0,   136,   217,     0,    46,    70,   109,    67,
      31,   289,     0,     0,     0,   228,   225,    45,    82,    34,
      42,    44,   383,     0,     0,   375,     0,   377,     0,     0,
       0,     0,     0,   385,     8,     0,     0,     0,   202,     0,
       0,     0,     0,     0,   337,   408,   407,   410,   418,   417,
     422,   421,   404,   401,   402,   403,   358,   391,   374,   373,
     372,   359,   395,   406,   400,   398,   409,   399,   397,   389,
     394,   396,   405,   388,   392,   393,   390,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   420,   419,   424,   423,   196,   193,
     194,   195,   199,   200,   201,   425,     0,     0,   341,     0,
       0,     0,     0,     0,     0,     0,   384,   338,   338,    85,
     226,     0,     0,     0,   306,     0,   128,     0,   125,   227,
     119,     0,     0,     0,   313,     0,     0,     0,   319,     0,
       0,    96,   104,     0,     0,   305,     0,   186,     0,   208,
       0,   250,     0,   264,     0,   259,   350,     0,     0,   243,
     348,   242,   371,   283,   282,     0,     0,   307,   109,     0,
     232,   350,     0,   357,   376,   287,   288,     0,     0,   101,
     323,     0,   386,     0,     0,   120,   121,   203,   204,   205,
     138,     0,     0,   234,   233,   235,   237,    47,    54,    55,
      56,    51,    53,    60,    61,    49,    52,    50,    48,    58,
      57,    59,   369,   370,   197,   198,   343,   192,   340,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,   295,   293,   294,   292,     0,   126,   124,     0,     0,
     103,     0,     0,    87,     0,    86,     0,     0,    93,     0,
       0,    91,     0,     0,     0,   107,   192,   184,     0,   273,
     209,   211,   210,   212,     0,   248,   251,     0,   252,     0,
     244,   253,   254,     0,     0,   108,   110,   308,     0,     0,
     229,     0,     0,     0,     0,   102,     0,    36,     0,   238,
     208,   167,   165,   170,   177,   178,   179,   174,   176,   172,
     175,   173,   171,   181,   180,   146,   149,   147,   148,   159,
     150,   163,   155,   153,   166,   154,   152,   157,   162,   164,
     151,   156,   160,   161,   158,   168,   169,   144,   182,   182,
     236,   342,   344,     0,     0,     0,     0,     0,     0,     0,
       0,    84,    83,   129,   127,   121,    90,     0,     0,   312,
       0,   311,     0,     0,   318,    94,     0,   317,    92,   106,
     328,   187,     0,     0,   252,   223,   245,     0,     0,   112,
       0,   113,     0,     0,    99,   325,     0,     0,    97,   324,
       0,   387,    37,    62,   238,   238,     0,   114,   238,   122,
       0,   145,     0,   185,   140,   141,     0,     0,     0,     0,
       0,     0,     0,     0,   238,   316,     0,     0,   322,     0,
       0,   273,   276,   277,   278,     0,   275,   279,   350,   219,
     189,   188,     0,     0,     0,   262,   350,   111,     0,   100,
       0,    98,   123,   223,   144,   182,   182,     0,     0,     0,
       0,     0,     0,     0,   115,     0,     0,     0,     0,   252,
     265,     0,   220,   222,   221,   224,   215,   216,   132,     0,
       0,     0,     0,   142,   143,   183,     0,     0,     0,     0,
       0,     0,   315,   314,   321,   320,   191,   190,   267,   268,
     270,   350,     0,   384,   350,   327,   326,   213,   139,   214,
       0,   269,   271,   265,   272
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   208,    73,   472,    75,    76,    77,   209,
      78,    79,    80,   151,    81,   155,   189,    82,    83,    84,
      85,    86,   556,    87,    88,   495,   606,    89,    90,    91,
     367,   619,    92,   457,   458,   135,   164,   479,    94,    95,
     368,   620,   520,   658,   659,   724,   319,   476,   477,   478,
     521,   226,   594,   818,   788,   181,   783,   750,   753,    96,
     198,   345,    97,    98,   167,   168,   323,   324,   489,   327,
     328,   485,   809,   747,   694,   210,   214,   215,   297,   298,
     299,   136,   100,   101,   102,   137,   104,   123,   124,   437,
     438,   105,   138,   139,   108,   141,   110,   142,   143,   113,
     114,   219,   115,   116,   117,   118,   119
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -725
static const yytype_int16 yypact[] =
{
    -725,    92,  2359,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  3444,   118,    99,  -725,   118,  6312,    87,    99,  6312,
    3444,   105,    99,   392,   612,  5323,  6312,  6312,   149,  -725,
      99,  -725,    25,  3444,  6312,  6312,  -725,  6312,  6312,   189,
     184,   817,   961,  -725,  5507,  5610,  6312,  5738,  6312,   247,
     192,  3444,    85,  -725,  5507,  6312,  6312,  -725,  -725,  6312,
    -725,  -725,  7041,  6312,  6312,  -725,  6312,  -725,  -725,  2676,
    5117,  5507,  -725,  3316,  -725,  -725,   329,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,    99,  -725,   -27,    50,  -725,  -725,  -725,   451,   208,
    -725,  -725,  -725,   258,   276,   312,   308,   318, 10826,  1597,
     146,   349,   362,  -725,  -725,  -725,  -725,  -725,  -725,   366,
    -725, 10826,   327,  3444,  -725,   363,  -725,   352,  6312,  6312,
    6312,  6312,  6312,  5507,  5507,   292,  -725,  -725,  -725,  -725,
    8604,   321,  -725,  -725,    99,   364,  8794,   409,  5507,   360,
    -725,  -725,  -725,  -725,    99,    70,    99,   359,    49,  7652,
    7608,  7790,  5507,  3444,   367,    14,    80,     7,  -725,  -725,
     441,   394,  7853,   441,  -725,  5507,  -725,   286,  -725,  -725,
      99,  -725,   -14, 10826,  -725,  8852,  8483,  3444,  -725,  -725,
    7853, 10826,   376,  5507,  -725, 10826,  5507,   423,   386,    97,
    7853,  8910,   112,  2405,   441,   112,   441,  -725,  -725,  2804,
      60,  -725,  6312,  -725,   -18,    78, 10826,    58,  8968,    -7,
     503,  -725,    99,   407,  -725,  -725,  -725,    18,    25,    25,
      25,  -725,  6312,  6312,  6312,  6312,  5819,  5819,  6312,  6312,
    6312,  6312,  6312,  6312,   107,  7041,  6312,  6312,  6312,  6312,
    6312,  6312,  6312,  6312,  6312,  6312,  6312,  6312,  6312,  6312,
    6312,  6312,  6312,  6312,  6312,  6312,  6312,  6312,  6312,  6312,
    6312,  6312,  6312,  6312,  6312,  6312,  5507,  5507,  5819,  5819,
    1231,  -725,  -725,  2932,  -725,  8673,  8736,  9031,    73,  5819,
      49,   395,  -725,  -725,  6312,   126,  -725,   385,   415,  -725,
   10826,    99,   397,    99,   486,  5507,  -725,  3572,  5819,  -725,
    3700,  5819,  -725,    49,  3444,   497,   401,  -725,    22,  -725,
    -725,    14,  -725,   425,   402,  -725,  4852,   453,   452,  6312,
      25,  -725,   411,  -725,  -725,  5507,  -725,  -725,  -725,  -725,
    -725,  5507,   416,   251,  4933,   435,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  7503,  5220,  -725,  5404,  -725,  5819,  2548,
     418,  6312,  6312,  -725,  -725,   412,  5507,   424,   335,    25,
     262,   291,   293,   316,  8541,   846,   846,   396,  -725,   396,
    -725,   396,  1985,  1250,   358,   815,   394,   112,  -725,  -725,
    -725,  2405,  2091,   396,  1356,  1356,   846,  1356,  1356,  1302,
     112,  2091,  1763,  1302,   441,   441,   112,  9094,  9152,  9210,
    9268,  9326,  9384,  9442,  9500,  9558,  9616,  9674,  9732,  9790,
    9848,  9906,   422,   421,  -725,   396,  -725,   396,   240,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,    99,    89,  -725,   496,
    6393,  5819,  6474,  5819,  6312,  5819,  2223,   118,  9964,  -725,
    -725, 10022,  6312,  5507,  -725,  5507,   474,   206,  -725,  -725,
     407,  6312,   181,  6312, 10826,    52,  7916,  6312, 10826,    23,
    7722,  -725,   532, 10080,  3444,  -725,   215,  -725,    29,   390,
     429,    14,    66,  -725,  5507,  -725,   259,  6312,  5922,  -725,
   10826,  -725,  -725,  -725, 10826,     3,   433,  -725,  -725,  5507,
    -725,   320,    99,  -725,  -725,  -725,  -725,    39,  7296,  -725,
   10826,  3444, 10826, 10138,  3060,   464,  -725,  -725,  -725,  -725,
    -725,  7180,   323,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  1231,  -725,    99,
    6312,   547, 10201,  6312,   549, 10264,   443,  7330,    49,    49,
    -725, 10826,  -725, 10826,  -725,  6312,    99,  -725,   450,  7853,
    -725,  7979,  3828,  -725,  3956,  -725,  8042,  4084,  -725,    49,
    4212,  -725,    49,  3444,  6312,  -725,   705,  -725,    14,   478,
    -725,  -725,  -725,  -725,    65,  -725,  -725,  4933,   452,    82,
   10826,  -725, 10826,  3444,  5507,  -725,  -725,  -725,   234,    83,
    -725,  4340,   454,  4468,   457,  -725,  6312,  -725,  3188,   347,
     390,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,    99,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   480,   456,   456,
    -725,  -725,  -725, 10322,  6555,  6636, 10385,  6717,  6798,  6879,
    6960,  -725,  -725, 10826,  -725,  -725,  -725,  3444,  6312, 10826,
    6312, 10826,  3444,  6312, 10826,  -725,  6312, 10826,  -725,  -725,
   10826,  -725,   461,  6003,   111,   517,  -725,  5819,  4596,  -725,
      51,  -725,  4724,  6312,  -725, 10826,  3444,  6312,  -725, 10826,
    3444, 10826,  -725,   339,   493,   493,    99,  -725,   553,  -725,
      68,  -725,  7180,    22,  -725,  -725,  6312,  6312,  6312,  6312,
    6312,  6312,  6312,  6312,  1131, 10080,  8105,  8168, 10080,  8231,
    8294,   478,   105,  6312,  6312,  5014,  -725,  -725,   210,  5507,
    -725,  -725,  6312,   -13,  7364,  -725,   393,  -725,  8357,  -725,
    8420,  -725,  -725,   517,  -725,   456,   456,   237, 10448, 10511,
   10574, 10637, 10700, 10763,  -725,  3444,  3444,  3444,  3444,   111,
    6106,   170,  -725,  -725, 10826, 10826,  -725,  -725,  -725,  6960,
    3444,  3444,    79,  -725,  -725,  -725,  6312,  6312,  6312,  6312,
    6312,  6312, 10080, 10080, 10080, 10080,  -725,  -725,  -725,  -725,
    -725,   220,  5819,  7122,   500, 10080, 10080,  -725,  -725,  -725,
    7469,  -725,  -725,  6209,  -725
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -725,  -725,  -725,     0,  -590,  1605,  -725,  -725,  1207,    98,
    -725,  -725,  -725,   373,  1828,   -22,   740,  -656,  -589,   -35,
    -725,  -725,    48,  -725,  -725,   113,  -725,  -725,  -725,   442,
     136,   -72,  -725,  -725,    57,   284,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -108,  -106,  -594,  -725,   -98,    43,  -268,
    -725,  -725,    10,  -725,  -160,   -66,  -725,  -146,  -129,  -725,
    -135,  -725,  -725,  -725,  -166,   305,  -725,  -318,  -587,  -725,
    -440,  -334,  -724,  -507,  -105,    24,   108,  -725,  -144,  -725,
     182,   303,  -288,  -725,  -725,   526,  -725,    -2,  -138,  -725,
      91,  -725,   809,   954,   -11,  1032,  -725,  1123,  1460,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -260
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -385
static const yytype_int16 yytable[] =
{
     121,   152,    72,   480,   213,   140,   178,   179,   146,   291,
     500,   696,   436,   127,   159,   160,   161,     5,   316,   302,
     439,     5,   313,   170,   171,  -192,   172,   173,     5,   718,
     719,   332,     5,   183,   185,   186,   190,   191,   486,   329,
     195,   428,   598,   183,   200,   201,   577,    45,   202,   342,
     603,   203,   204,   205,   429,   206,   501,   430,   183,   216,
     218,   370,   371,   372,   373,   725,   325,   325,   182,   325,
     431,   432,   187,   158,   187,   572,   369,   604,   199,   360,
     433,   325,   296,   354,   320,    14,   224,   335,     5,   821,
     358,   434,     3,  -334,   362,   217,   296,   787,  -257,   824,
     122,  -368,     5,  -368,   225,   445,   336,   751,   330,   296,
       5,   355,  -257,   283,   697,   697,   122,   121,   285,   286,
     287,   195,   183,   218,   363,  -257,    43,   296,  -257,   762,
     605,   388,   422,   423,   235,   122,   787,   300,   290,    45,
    -257,    53,   321,  -257,   718,   719,   165,   196,  -257,   283,
    -192,   300,   335,   165,   695,   333,   144,   588,   288,   335,
    -255,   335,  -257,   596,   300,   597,   597,  -257,   597,   236,
     611,   793,   794,   237,   335,   283,   389,   283,   283,   356,
     326,  -257,   300,   335,   335,   183,  -257,   352,  -257,   359,
     547,  -257,   807,   488,  -257,   122,   174,   496,   335,   817,
    -257,   353,   812,   522,   446,   283,   452,   357,   453,   351,
     296,   296,   242,   698,   702,   244,   749,   347,   548,   247,
     343,   374,   375,   376,   377,   379,   381,   382,   383,   384,
     385,   386,   387,   148,   391,   392,   393,   394,   395,   396,
     397,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   300,   300,   425,   427,   493,
     692,   335,   544,   810,   276,   296,   277,   162,   448,   436,
     763,   603,   335,   451,   378,   380,    93,   439,   213,     5,
     213,  -258,  -274,   150,   183,    93,   464,   466,   545,   468,
     470,   813,  -266,   473,    93,    99,   822,   566,   604,   486,
     570,  -274,   175,   192,    99,  -274,   586,    93,   490,  -258,
     193,  -266,   -62,    99,   494,  -266,   424,   426,  -331,   462,
     300,   579,   582,   567,  -258,    93,    99,   447,   586,  -274,
     -62,  -258,     5,   216,   587,   216,   150,   508,   510,  -266,
     512,   513,   335,    93,    99,   183,   465,    93,  -240,   469,
    -258,   701,    14,   330,   755,    17,   795,   610,   755,   612,
     614,   222,    99,    22,    23,   713,    99,   498,  -330,  -258,
     235,    29,   523,  -133,   517,  -258,   125,   562,  -240,   296,
     515,   145,   330,   518,   330,     5,   231,  -240,    39,   150,
     714,  -133,   715,    43,   519,   748,   507,    93,   154,   590,
     756,   524,  -240,   525,   756,   236,   716,   330,    53,   237,
    -368,  -231,  -368,   278,   330,  -261,    99,   279,  -335,   464,
     552,   468,   555,   473,   557,   510,   526,   232,  -336,   591,
    -231,   561,   563,   660,   300,   558,   559,    93,   592,  -367,
     569,  -367,   571,  -261,   239,   280,   576,   241,   242,   593,
     243,   244,   505,   235,   506,   247,    99,    67,  -261,  -333,
     227,    93,   254,   218,   717,  -261,   600,   602,   283,   258,
     259,   260,  -332,   282,   292,   294,   301,   305,   218,   551,
      99,   554,   811,    93,  -261,   318,   242,   244,   236,   244,
     228,   814,   237,   247,   341,   344,   346,   366,   599,   229,
     364,   176,    99,  -261,   454,   450,   455,   459,   461,  -261,
      23,   177,   474,   609,   230,   811,   481,    29,   103,  -133,
     475,   482,  -263,   487,   488,   811,   502,   103,   514,   663,
     492,   242,   666,   243,   244,   497,   103,  -133,   247,   511,
     516,   542,   543,   549,   673,   254,   565,   583,   595,   103,
    -263,   679,   607,   681,   260,   335,   684,    93,    14,   687,
     664,    17,   667,   690,   669,  -263,   675,   103,   693,    22,
      23,   713,  -263,   722,   723,   706,    99,    29,   710,  -133,
     741,    93,   752,   183,    93,   103,   568,   303,    93,   103,
     705,  -263,   709,   734,   220,   711,   714,  -133,   715,    43,
      99,   608,   618,    99,   765,     5,   766,    99,   351,   150,
    -263,   153,   716,   674,    53,   767,  -263,   746,   700,   691,
     720,  -240,   819,   806,   792,   491,   779,   564,   661,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,   103,
       0,     0,     0,   679,   681,     0,   684,   687,   705,   709,
       0,  -240,    99,     0,     0,     0,   735,   736,     0,   737,
    -240,   738,   739,    67,     0,   740,     0,     0,     0,   178,
     179,   154,     0,   782,     0,  -240,   754,   510,     0,   103,
       0,   510,   758,     0,     0,     0,   760,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,   808,   735,   768,   769,   738,   770,
     771,   772,   773,     0,   428,     0,     0,     0,     0,     0,
       0,     0,   191,   195,   218,   103,     0,   429,   784,   546,
     430,   785,     0,     0,     0,     0,     0,   808,     0,     0,
       0,     0,     0,   431,   432,     0,     0,   808,    93,     0,
       0,     0,     0,   433,   802,   803,   804,   805,     0,   781,
       0,     0,     0,     0,   434,     0,     0,    99,   709,   815,
     816,     0,     0,     0,     0,   802,   803,   804,   805,   815,
     816,     0,     0,     0,     0,    93,     0,     0,    93,     0,
       0,   820,   510,     0,     0,     0,     0,     0,     0,   103,
       0,   106,   709,     0,    99,     0,     0,    99,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,   103,     0,   176,   103,   235,     0,     0,
     103,     0,   106,     0,    23,   177,     0,     0,     0,     0,
       0,    29,   662,  -133,     0,     0,    93,     0,    93,   -40,
     106,    93,     0,     0,    93,     0,     0,    93,   235,     0,
       0,  -133,   236,     0,     0,    99,   237,    99,   106,     0,
      99,     0,   106,    99,     0,   103,    99,    93,     0,     0,
       0,     0,     0,     0,     0,    93,     0,    93,   306,   309,
     312,     0,    93,   236,     0,     0,    99,   237,     0,     0,
       0,   239,   331,     0,    99,   242,    99,   243,   244,     0,
       0,    99,   247,     0,     0,     0,     0,   238,     0,   254,
     340,     0,   106,     0,     0,     0,   258,   259,   260,     0,
     348,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   107,   251,   252,   253,
     254,    93,   255,   256,   257,   107,    93,   258,   259,   260,
       0,     0,   106,     0,   107,     0,     0,     0,     0,   176,
      99,     0,     0,     0,     0,    99,     0,   107,    23,   177,
      93,     0,     0,     0,    93,    29,   106,  -133,     0,     0,
     103,     0,     0,   -39,     0,   107,     0,     0,     0,    99,
       0,     0,     0,    99,     0,  -133,     0,     0,   106,     0,
       0,     0,     0,   107,     0,     0,     0,   107,     0,     0,
     449,     0,     0,     0,   109,     0,     0,   103,     0,     0,
     103,     0,     0,   109,     0,     0,     0,     0,     0,     0,
       0,     0,   109,   471,     0,     0,     0,     0,     0,    93,
      93,    93,    93,     0,     0,   109,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    93,     0,   107,    99,    99,
      99,    99,     0,   109,     0,     0,     0,     0,     0,     0,
       0,     0,   106,    99,    99,     0,     0,     0,   103,     0,
     103,   109,     0,   103,     0,   109,   103,     0,     0,   103,
       0,     0,     0,     0,     0,     0,   106,   107,     0,   106,
       0,     0,     0,   106,     0,   111,     0,     0,     0,   103,
       0,     0,     0,     0,   111,     0,     0,   103,     0,   103,
       0,   107,     0,   111,   103,     0,    14,     0,     0,    17,
       0,     0,     0,     0,     0,   109,   111,    22,    23,   713,
       0,     0,     0,   107,     0,    29,     0,  -133,   106,     0,
       0,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,     0,    39,     0,   714,  -133,   715,    43,     0,     0,
       0,     0,   111,     0,     0,   109,   111,     0,     0,     0,
     716,     0,    53,   103,     0,   573,   575,     0,   103,   578,
     581,     0,     0,     0,     0,     0,     0,     0,     0,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,     0,   103,   107,     0,     0,
       0,   109,     0,     0,     0,     0,   111,     0,     0,     0,
     428,    67,     0,     0,   188,     0,     0,     0,   774,     0,
       0,   107,     0,   429,   107,     0,   430,     0,   107,     0,
       0,     0,   235,     0,     0,     0,     0,     0,     0,   431,
     432,     0,     0,   106,     0,     0,   111,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,   671,   672,
     434,   103,   103,   103,   103,     0,     0,   236,     0,   676,
     111,   237,    57,   107,     0,   109,   103,   103,     0,   685,
     106,     0,   688,   106,   235,     0,     0,    58,     0,    60,
      61,     0,   111,     0,   284,     0,     0,     0,     0,   109,
       0,     0,   109,     0,     0,     0,   109,     0,    65,   435,
     242,     0,   243,   244,    68,     0,     0,   247,     0,   236,
       0,     0,     0,   237,   254,   188,   188,   188,   315,     0,
       0,   258,   259,   260,     0,     0,     0,     0,   235,   188,
       0,   106,     0,   106,     0,     0,   106,     0,     0,   106,
       0,   109,   106,     0,     0,     0,     0,   188,   239,     0,
     240,   241,   242,     0,   243,   244,   111,   188,     0,   247,
       0,     0,   106,   236,     0,     0,   254,   237,     0,     0,
     106,     0,   106,   258,   259,   260,     0,   106,   107,     0,
     111,     0,     0,   111,     0,     0,     0,   111,     0,     0,
     757,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   112,   247,     0,   107,     0,     0,   107,   253,
     254,   112,     0,     0,   257,     0,     0,   258,   259,   260,
     112,     0,   111,     0,     0,     0,   106,     0,     0,     0,
       0,   106,     0,   112,     0,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,   109,     0,     0,     0,
       0,   112,     0,     0,     0,   106,     0,     0,     0,   106,
     188,     0,     0,     0,     0,     0,   107,     0,   107,   112,
       0,   107,     0,   112,   107,     0,     0,   107,     0,     0,
       0,     0,     0,   109,     0,     0,   109,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,     0,   107,     0,   107,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,   106,   106,   106,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,   106,
     106,     0,     0,     0,   109,     0,   109,    74,     0,   109,
       0,     0,   109,     0,     0,   109,   120,     0,     0,     0,
       0,     0,     0,   112,     0,   147,     0,     0,     0,     0,
       0,   107,     0,     0,   111,   109,   107,   111,   169,     0,
       0,     0,     0,   109,     0,   109,     0,   112,     0,     0,
     109,     0,     0,     0,     0,     0,   194,     0,     0,     0,
     107,     0,     0,     0,   107,     0,     0,     0,     0,   112,
       0,     0,   188,   188,    74,     0,   188,   188,   221,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,     0,     0,   111,     0,   111,     0,     0,
     111,     0,     0,   111,     0,     0,   111,     0,     0,   109,
       0,     0,     0,     0,   109,     0,     0,     0,     0,     0,
       0,   275,  -345,     0,     0,  -367,   111,  -367,   281,   107,
     107,   107,   107,     0,   111,     0,   111,     0,   109,     0,
       0,   111,   109,   112,   107,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   188,   188,   112,   317,     0,
     112,     0,     0,     0,   112,     0,   188,     0,   315,     0,
       0,     0,     0,   315,     0,   235,   188,     0,     0,   188,
       0,     0,   339,     0,     0,     0,     0,     0,     0,     0,
     111,     0,     0,     0,     0,   111,     0,   109,   109,   109,
     109,     0,     0,     0,    74,     0,     0,     0,     0,   112,
     236,     0,   109,   109,   237,     0,     0,     0,     0,   111,
       0,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,   126,     0,     0,   238,     0,   126,     0,     0,     0,
     149,   126,   126,     0,     0,     0,     0,     0,   163,   239,
     166,   240,   241,   242,     0,   243,   244,   245,     0,   246,
     247,   248,   249,     0,   251,   252,   253,   254,     0,   255,
     197,   257,     0,     0,   258,   259,   260,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,   111,
     111,   111,     0,     0,     0,     0,     0,   188,     0,     0,
       0,     0,   339,   111,   111,   339,     0,     0,     0,   223,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,     0,     0,     0,     0,     0,
       0,     0,     0,   315,   315,     0,   315,   315,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     786,     0,     0,     0,   509,   315,     0,   315,     0,     0,
       0,   112,   197,     0,   112,     0,   295,     0,     0,     0,
       0,     0,   197,     0,   304,     0,     0,     0,     0,     0,
     295,     0,     0,   322,     0,     0,     0,     0,     0,   786,
       0,     0,     0,   295,     0,   126,     0,   235,   334,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   295,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,     0,   112,     0,     0,   112,     0,     0,
     112,     0,   236,   112,     0,     0,   237,     0,     0,     0,
     365,     0,     0,     0,     0,   166,   166,   166,   166,     0,
       0,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,   112,   390,   112,     0,     0,     0,     0,   112,   585,
       0,   239,     0,   240,   241,   242,     0,   243,   244,   245,
       0,   246,   247,   248,   249,     0,   251,   252,   253,   254,
       0,   255,     0,   257,   295,   295,   258,   259,   260,     0,
       0,     0,     0,   235,     0,     0,   615,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   456,
       0,   460,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,   112,     0,     0,     0,     0,     0,   236,   322,
       0,     0,   237,     0,     0,     0,     0,     0,   166,     0,
       0,     0,     0,     0,     0,     0,   112,     0,     0,   295,
     112,     0,     0,     0,     0,     0,     0,   339,     0,   339,
       0,     0,   339,     0,     0,   339,     0,   239,   689,   240,
     241,   242,     0,   243,   244,   245,     0,   166,   247,   248,
     249,     0,   251,   252,   253,   254,     0,     0,   699,   257,
       0,     0,   258,   259,   260,     0,   704,     0,   708,     0,
       0,     0,     0,    74,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   128,   112,   112,   112,   112,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
     112,   112,   129,   130,   131,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,   295,     0,     0,     0,    48,    49,    50,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   589,     0,     0,   322,
       0,   759,     0,     0,     0,   761,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   657,
       0,    70,     0,   134,     0,     0,     0,     0,     0,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -238,    18,
      19,     0,    20,    21,     0,    22,    23,    24,    25,    26,
      27,     0,    28,    29,   456,  -133,    30,    31,    32,    33,
      34,   -38,    35,    36,     0,    37,     0,     0,  -238,    38,
      39,    40,    41,  -133,    42,    43,   322,  -238,    44,    45,
       0,    46,    47,    48,    49,    50,    51,   235,    52,     0,
      53,    54,  -238,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,   236,    62,   721,     0,   237,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,    69,     0,    70,     0,    71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,   240,   241,   242,     0,   243,   244,     0,
       0,     0,   247,     0,     0,     0,     0,     0,   253,   254,
       0,     0,     0,   257,     0,     0,   258,   259,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,     0,     0,   197,     0,     0,     0,     0,     4,
     764,     5,     6,     7,     8,     9,    10,     0,  -384,    11,
      12,    13,  -384,    14,    15,    16,    17,  -238,    18,    19,
    -384,    20,    21,  -384,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -133,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,  -238,    38,     0,
      40,    41,  -133,    42,    43,  -384,  -238,    44,    45,  -384,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -238,     0,     0,    55,     0,    56,     0,     0,  -384,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -384,    59,  -384,  -384,  -384,  -384,
    -384,  -384,  -384,     0,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,  -384,  -384,    64,  -384,  -384,  -384,     0,    67,  -384,
    -384,  -384,     0,     0,    69,  -384,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -238,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -133,    30,    31,    32,    33,    34,   -38,    35,
      36,     0,    37,     0,     0,  -238,    38,    39,    40,    41,
    -133,    42,    43,     0,  -238,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -238,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,   180,    67,     0,     0,    68,
       0,     0,    69,   207,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -238,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -133,    30,    31,    32,    33,    34,   -38,    35,    36,     0,
      37,     0,     0,  -238,    38,    39,    40,    41,  -133,    42,
      43,     0,  -238,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -238,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,   350,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -238,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -133,    30,
      31,    32,    33,    34,   -38,    35,    36,     0,    37,     0,
       0,  -238,    38,    39,    40,    41,  -133,    42,    43,     0,
    -238,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -238,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,   207,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -238,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -38,    35,    36,     0,    37,     0,     0,  -238,
      38,    39,    40,    41,  -133,    42,    43,     0,  -238,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -238,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,   617,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -238,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -133,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,  -238,    38,    39,
      40,    41,  -133,    42,    43,     0,  -238,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -238,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,   712,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -238,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -133,    30,    31,    32,    33,    34,   -38,    35,
      36,     0,    37,     0,     0,  -238,    38,   220,    40,    41,
    -133,    42,    43,     0,  -238,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -238,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -238,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -133,    30,    31,    32,    33,    34,   -38,    35,    36,     0,
      37,     0,     0,  -238,    38,     0,    40,    41,  -133,    42,
      43,     0,  -238,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -238,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -238,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,   463,     0,    28,    29,     0,  -133,    30,
      31,    32,    33,    34,   -38,    35,    36,     0,    37,     0,
       0,  -238,    38,     0,    40,    41,  -133,    42,    43,     0,
    -238,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -238,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -238,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,   467,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -38,    35,    36,     0,    37,     0,     0,  -238,
      38,     0,    40,    41,  -133,    42,    43,     0,  -238,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -238,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -238,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,   678,
       0,    28,    29,     0,  -133,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,  -238,    38,     0,
      40,    41,  -133,    42,    43,     0,  -238,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -238,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -238,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   680,     0,    28,
      29,     0,  -133,    30,    31,    32,    33,    34,   -38,    35,
      36,     0,    37,     0,     0,  -238,    38,     0,    40,    41,
    -133,    42,    43,     0,  -238,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -238,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -238,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,   683,     0,    28,    29,     0,
    -133,    30,    31,    32,    33,    34,   -38,    35,    36,     0,
      37,     0,     0,  -238,    38,     0,    40,    41,  -133,    42,
      43,     0,  -238,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -238,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -238,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,   686,     0,    28,    29,     0,  -133,    30,
      31,    32,    33,    34,   -38,    35,    36,     0,    37,     0,
       0,  -238,    38,     0,    40,    41,  -133,    42,    43,     0,
    -238,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -238,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -238,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,   703,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -38,    35,    36,     0,    37,     0,     0,  -238,
      38,     0,    40,    41,  -133,    42,    43,     0,  -238,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -238,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -238,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,   707,
       0,    28,    29,     0,  -133,    30,    31,    32,    33,    34,
     -38,    35,    36,     0,    37,     0,     0,  -238,    38,     0,
      40,    41,  -133,    42,    43,     0,  -238,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -238,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,   483,    71,     5,
       6,     7,     8,     9,    10,     0,  -384,   128,     0,     0,
    -384,     0,     0,     0,     0,     0,     0,     0,  -384,     0,
      21,     0,     0,     0,     0,   129,   130,   131,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,  -384,     0,     0,     0,  -384,     0,     0,
      48,    49,    50,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -256,  -384,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -384,    59,  -384,  -384,  -384,     0,  -384,  -384,
    -384,     0,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,
    -384,    64,  -384,  -384,  -384,     0,     0,  -384,  -384,  -384,
       0,     0,   133,     0,    70,   483,   499,     5,     6,     7,
       8,     9,    10,     0,  -384,   128,     0,     0,  -384,     0,
       0,     0,     0,     0,     0,     0,  -384,     0,    21,     0,
       0,     0,     0,   129,   130,   131,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,  -384,     0,     0,     0,  -384,     0,     0,    48,    49,
      50,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -384,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -384,    59,  -384,  -384,  -384,     0,  -384,  -384,  -384,     0,
    -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,  -384,    64,
    -384,  -384,  -384,     0,     0,  -384,  -384,  -384,     0,     0,
     133,     0,    70,   483,   499,     5,     6,     7,     8,     9,
      10,     0,     0,   128,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,   129,   130,   131,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,   483,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   128,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,    21,     0,     0,
       0,     0,   129,   130,   131,    63,    28,    64,     0,    65,
      66,    31,    32,     0,    34,    68,    35,    36,   133,     0,
      70,     0,   484,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   128,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,    21,     0,
       0,     0,     0,   129,   130,   131,    63,    28,    64,     0,
      65,    66,    31,    32,     0,    34,    68,    35,    36,   133,
       0,    70,     0,   499,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       5,     6,     7,     8,     9,    10,     0,    63,   128,    64,
       0,    65,    66,   180,     0,     0,     0,    68,     0,     0,
     133,    21,    70,     0,   134,   780,   129,   130,   131,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   132,     0,     0,   211,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,   212,     5,     6,     7,     8,     9,    10,     0,
      63,   128,    64,     0,    65,    66,   180,     0,     0,     0,
      68,     0,     0,   133,    21,    70,     0,   134,     0,   129,
     130,   131,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   132,     0,     0,
     211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     5,     6,     7,     8,
       9,    10,     0,    63,   128,    64,     0,    65,    66,   180,
       0,     0,     0,    68,     0,     0,   133,    21,    70,   504,
     134,     0,   129,   130,   131,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,   156,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,    57,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   128,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,    21,     0,
       0,     0,     0,   129,   130,   131,    63,    28,    64,     0,
      65,    66,    31,    32,     0,    34,    68,    35,    36,   133,
       0,    70,     0,   134,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   132,     0,     0,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       5,     6,     7,     8,     9,    10,     0,    63,   128,    64,
       0,    65,    66,   180,     0,     0,     0,    68,     0,     0,
     133,    21,    70,     0,   134,     0,   129,   130,   131,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     5,     6,     7,     8,     9,    10,     0,
      63,   128,    64,     0,    65,    66,   180,     0,     0,     0,
      68,     0,     0,   133,    21,    70,     0,   134,     0,   129,
     130,   131,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
     184,     0,     0,    68,     0,     0,   133,     0,    70,     0,
     134,     5,     6,     7,     8,     9,    10,     0,     0,   128,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,    21,     0,     0,     0,     0,   129,   130,   131,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     128,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,    21,     0,     0,     0,     0,   129,   130,
     131,    63,    28,    64,     0,    65,    66,    31,    32,     0,
      34,    68,    35,    36,    69,     0,    70,     0,   134,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   157,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     5,     6,     7,     8,     9,
      10,     0,    63,   128,    64,     0,    65,    66,     0,     0,
       0,     0,    68,     0,     0,   133,    21,    70,     0,   134,
       0,   129,   130,   131,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,   601,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   128,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   742,     0,     0,
       0,     0,   129,   130,   131,    63,    28,    64,     0,    65,
      66,    31,    32,     0,    34,    68,    35,    36,   133,     0,
      70,     0,   134,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   743,    49,    50,
     744,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     5,
       6,     7,     8,     9,    10,     0,    63,   128,    64,     0,
      65,    66,   180,     0,     0,     0,    68,     0,     0,   133,
      21,    70,     0,   745,     0,   129,   130,   131,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     128,    64,     0,    65,    66,   180,     0,     0,     0,    68,
       0,     0,   133,    21,    70,     0,   745,     0,   129,   130,
     733,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     5,     6,     7,     8,     9,
      10,     0,    63,   128,    64,     0,    65,    66,   180,     0,
       0,     0,    68,     0,     0,   133,    21,    70,     0,   134,
       0,   129,   130,   131,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   132,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   128,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,    21,     0,     0,
       0,     0,   129,   130,   550,    63,    28,    64,     0,    65,
      66,    31,    32,     0,    34,    68,    35,    36,   133,     0,
      70,     0,   134,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     132,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   128,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,    21,     0,
       0,     0,     0,   129,   130,   553,    63,    28,    64,     0,
      65,    66,    31,    32,     0,    34,    68,    35,    36,   133,
       0,    70,     0,   134,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   132,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   128,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,    21,
       0,     0,     0,     0,   129,   130,   727,    63,    28,    64,
       0,    65,    66,    31,    32,     0,    34,    68,    35,    36,
     133,     0,    70,     0,   134,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   132,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,   128,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
      21,     0,     0,     0,     0,   129,   130,   728,    63,    28,
      64,     0,    65,    66,    31,    32,     0,    34,    68,    35,
      36,   133,     0,    70,     0,   134,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   132,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   128,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,    21,     0,     0,     0,     0,   129,   130,   730,    63,
      28,    64,     0,    65,    66,    31,    32,     0,    34,    68,
      35,    36,   133,     0,    70,     0,   134,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   128,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,    21,     0,     0,     0,     0,   129,   130,   731,
      63,    28,    64,     0,    65,    66,    31,    32,     0,    34,
      68,    35,    36,   133,     0,    70,     0,   134,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   132,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     128,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,    21,     0,     0,     0,     0,   129,   130,
     732,    63,    28,    64,     0,    65,    66,    31,    32,     0,
      34,    68,    35,    36,   133,     0,    70,     0,   134,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   132,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   128,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,    21,     0,     0,     0,     0,   129,
     130,   733,    63,    28,    64,     0,    65,    66,    31,    32,
       0,    34,    68,    35,    36,   133,     0,    70,     0,   134,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   132,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,   128,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,    21,     0,     0,     0,     0,
     129,   130,   131,    63,    28,    64,     0,    65,    66,    31,
      32,     0,    34,    68,    35,    36,   133,     0,    70,     0,
     134,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   132,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,   128,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,   129,   130,   131,    63,    28,    64,     0,     0,    66,
      31,    32,     0,    34,    68,    35,    36,   133,     0,    70,
       0,   134,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     5,     0,     0,    48,    49,    50,   132,
     621,     0,     0,     0,   622,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -265,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,  -265,     0,     0,     0,  -265,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,     0,     0,
       0,   180,     0,     0,     0,     0,     0,     0,   133,     0,
      70,  -265,   745,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   623,   624,   625,   626,   627,   628,     0,     0,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
       0,     0,   639,     0,     0,     0,   640,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,     0,   651,     0,
       0,   652,   653,   654,   655,   656,   233,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,   236,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,   233,     0,     0,   238,   234,     0,
       0,     0,     0,     0,     0,     0,   235,   236,     0,     0,
       0,   237,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   238,   255,   256,   257,     0,     0,   258,   259,   260,
       0,   236,     0,     0,     0,   237,   239,   613,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   238,   255,   256,   257,     0,
       0,   258,   259,   260,     0,     0,     0,     0,     0,     0,
     239,   670,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   233,
     255,   256,   257,   234,     0,   258,   259,   260,     0,     0,
       0,   235,     0,     0,     0,   789,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   235,   236,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,     0,     0,     0,   237,   239,     0,   240,   241,   242,
       0,   243,   244,   245,     0,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   238,   255,   256,   257,     0,     0,
     258,   259,   260,     0,     0,     0,     0,     0,     0,   239,
     823,   240,   241,   242,     0,   243,   244,   245,     0,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   233,   255,
     256,   257,   234,     0,   258,   259,   260,     0,     0,     0,
     235,   310,   503,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   233,     0,     0,   236,   234,     0,     0,   237,
       0,     0,     0,     0,   235,   307,     0,     0,     0,     0,
       0,     0,     0,     0,   308,   122,     0,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,   236,
     243,   244,   245,   237,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   255,   256,   257,     0,     0,   258,
     259,   260,   233,   238,   283,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   580,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   255,   256,
     257,     0,     0,   258,   259,   260,     0,     0,   283,   236,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   122,
     233,     0,     0,   238,   234,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   255,   256,
     257,     0,     0,   258,   259,   260,     0,   236,   283,     0,
       0,   237,     0,     0,     0,     0,     0,     0,   314,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
       0,   238,     0,     0,     0,   235,   187,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   255,   256,   257,     0,
     236,   258,   259,   260,   237,     0,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,   238,     0,     0,     0,   235,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
       0,   240,   241,   242,     0,   243,   244,   245,     0,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   255,
     256,   257,     0,   236,   258,   259,   260,   237,     0,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   233,
       0,     0,     0,   234,     0,     0,     0,   238,     0,     0,
       0,   235,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   255,   256,   257,     0,   236,   258,   259,   260,
     237,     0,   283,     0,     0,     0,     0,   677,     0,     0,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
     238,     0,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,     0,   240,   241,   242,
       0,   243,   244,   245,     0,   246,   247,   248,   249,   250,
     251,   252,   253,   254,     0,   255,   256,   257,     0,   236,
     258,   259,   260,   237,     0,   283,     0,     0,     0,     0,
     682,     0,     0,     0,     0,   233,     0,     0,     0,   234,
       0,     0,     0,   238,     0,     0,     0,   235,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   255,   256,
     257,     0,   236,   258,   259,   260,   237,     0,   283,     0,
       0,     0,     0,   775,     0,     0,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,   238,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,   240,   241,   242,     0,   243,   244,   245,
       0,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   255,   256,   257,     0,   236,   258,   259,   260,   237,
       0,   283,     0,     0,     0,     0,   776,     0,     0,     0,
       0,   233,     0,     0,     0,   234,     0,     0,     0,   238,
       0,     0,     0,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   255,   256,   257,     0,   236,   258,
     259,   260,   237,     0,   283,     0,     0,     0,     0,   777,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,   238,     0,     0,     0,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,     0,   240,
     241,   242,     0,   243,   244,   245,     0,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   255,   256,   257,
       0,   236,   258,   259,   260,   237,     0,   283,     0,     0,
       0,     0,   778,     0,     0,     0,     0,   233,     0,     0,
       0,   234,     0,     0,     0,   238,     0,     0,     0,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     255,   256,   257,     0,   236,   258,   259,   260,   237,     0,
     283,     0,     0,     0,     0,   790,     0,     0,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,   238,     0,
       0,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,     0,   240,   241,   242,     0,   243,
     244,   245,     0,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   255,   256,   257,     0,   236,   258,   259,
     260,   237,     0,   283,     0,     0,     0,     0,   791,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
       0,   238,     0,     0,     0,   235,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   255,   256,   257,     0,
     236,   258,   259,   260,   237,     0,   283,     0,     0,     0,
       0,   233,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   235,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
       0,   240,   241,   242,     0,   243,   244,   245,     0,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   236,   255,
     256,   257,   237,     0,   258,   259,   260,     0,     0,   338,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,   238,     0,     0,     0,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,   239,     0,   240,
     241,   242,     0,   243,   244,   245,     0,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   255,   256,   257,
       0,   236,   258,   259,   260,   237,  -346,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   122,     0,   233,     0,   238,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   235,   440,     0,     0,     0,
     239,     0,   240,   241,   242,   441,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     255,   256,   257,     0,     0,   258,   259,   260,     0,     0,
     236,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,   238,     0,     0,     0,   235,   442,
       0,     0,     0,     0,     0,     0,     0,     0,   443,   239,
       0,   240,   241,   242,     0,   243,   244,   245,     0,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   255,
     256,   257,     0,   236,   258,   259,   260,   237,     0,     0,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   236,   255,   256,   257,   237,     0,   258,   259,   260,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   236,
     255,   256,   257,   237,   293,   258,   259,   260,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   236,   255,   256,
     257,   237,   337,   258,   259,   260,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   236,   255,   256,   257,   237,
     349,   258,   259,   260,     0,     0,     0,     0,     0,     0,
       0,   233,     0,     0,     0,   234,     0,     0,   361,   238,
       0,     0,     0,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   255,   256,   257,     0,   236,   258,
     259,   260,   237,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,   238,     0,     0,     0,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,     0,   240,
     241,   242,     0,   243,   244,   245,     0,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   255,   256,   257,
       0,   236,   258,   259,   260,   237,     0,     0,     0,     0,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   236,
     255,   256,   257,   237,   527,   258,   259,   260,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   236,   255,   256,
     257,   237,   528,   258,   259,   260,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   236,   255,   256,   257,   237,
     529,   258,   259,   260,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   236,   255,   256,   257,   237,   530,   258,
     259,   260,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   236,   255,   256,   257,   237,   531,   258,   259,   260,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   236,
     255,   256,   257,   237,   532,   258,   259,   260,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   236,   255,   256,
     257,   237,   533,   258,   259,   260,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   236,   255,   256,   257,   237,
     534,   258,   259,   260,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   236,   255,   256,   257,   237,   535,   258,
     259,   260,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   236,   255,   256,   257,   237,   536,   258,   259,   260,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   236,
     255,   256,   257,   237,   537,   258,   259,   260,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   236,   255,   256,
     257,   237,   538,   258,   259,   260,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   236,   255,   256,   257,   237,
     539,   258,   259,   260,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   236,   255,   256,   257,   237,   540,   258,
     259,   260,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   235,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   236,   255,   256,   257,   237,   541,   258,   259,   260,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,   122,     0,     0,   235,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   236,
     255,   256,   257,   237,     0,   258,   259,   260,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   235,   238,     0,   584,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   236,   255,   256,
     257,   237,   560,   258,   259,   260,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     235,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   236,   255,   256,   257,   237,
       0,   258,   259,   260,     0,     0,     0,     0,     0,     0,
       0,   233,     0,     0,     0,   234,     0,     0,   616,   238,
       0,     0,     0,   235,   665,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   255,   256,   257,     0,   236,   258,
     259,   260,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,   238,     0,     0,     0,   235,   668,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,     0,   240,
     241,   242,     0,   243,   244,   245,     0,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   255,   256,   257,
       0,   236,   258,   259,   260,   237,     0,     0,     0,     0,
       0,     0,   233,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   235,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   236,
     255,   256,   257,   237,     0,   258,   259,   260,     0,     0,
     726,     0,     0,     0,     0,   233,     0,     0,     0,   234,
       0,     0,     0,   238,     0,     0,     0,   235,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
     240,   241,   242,     0,   243,   244,   245,     0,   246,   247,
     248,   249,   250,   251,   252,   253,   254,     0,   255,   256,
     257,     0,   236,   258,   259,   260,   237,     0,     0,     0,
       0,     0,     0,   729,     0,     0,     0,     0,   233,     0,
       0,     0,   234,     0,     0,     0,   238,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,   240,   241,   242,     0,   243,   244,   245,
       0,   246,   247,   248,   249,   250,   251,   252,   253,   254,
       0,   255,   256,   257,     0,   236,   258,   259,   260,   237,
       0,     0,     0,     0,     0,     0,   796,     0,     0,     0,
       0,   233,     0,     0,     0,   234,     0,     0,     0,   238,
       0,     0,     0,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   240,   241,   242,     0,
     243,   244,   245,     0,   246,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   255,   256,   257,     0,   236,   258,
     259,   260,   237,     0,     0,     0,     0,     0,     0,   797,
       0,     0,     0,     0,   233,     0,     0,     0,   234,     0,
       0,     0,   238,     0,     0,     0,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,     0,   240,
     241,   242,     0,   243,   244,   245,     0,   246,   247,   248,
     249,   250,   251,   252,   253,   254,     0,   255,   256,   257,
       0,   236,   258,   259,   260,   237,     0,     0,     0,     0,
       0,     0,   798,     0,     0,     0,     0,   233,     0,     0,
       0,   234,     0,     0,     0,   238,     0,     0,     0,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,   240,   241,   242,     0,   243,   244,   245,     0,
     246,   247,   248,   249,   250,   251,   252,   253,   254,     0,
     255,   256,   257,     0,   236,   258,   259,   260,   237,     0,
       0,     0,     0,     0,     0,   799,     0,     0,     0,     0,
     233,     0,     0,     0,   234,     0,     0,     0,   238,     0,
       0,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,     0,   240,   241,   242,     0,   243,
     244,   245,     0,   246,   247,   248,   249,   250,   251,   252,
     253,   254,     0,   255,   256,   257,     0,   236,   258,   259,
     260,   237,     0,     0,     0,     0,     0,     0,   800,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
       0,   238,     0,     0,     0,   235,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   239,     0,   240,   241,
     242,     0,   243,   244,   245,     0,   246,   247,   248,   249,
     250,   251,   252,   253,   254,     0,   255,   256,   257,     0,
     236,   258,   259,   260,   237,     0,     0,     0,     0,     0,
       0,   801,     0,     0,     0,     0,   233,     0,     0,     0,
     234,     0,     0,     0,   238,     0,     0,     0,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
       0,   240,   241,   242,     0,   243,   244,   245,     0,   246,
     247,   248,   249,   250,   251,   252,   253,   254,     0,   255,
     256,   257,     0,   236,   258,   259,   260,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,   240,   241,   242,     0,   243,   244,
     245,     0,   246,   247,   248,   249,   250,   251,   252,   253,
     254,     0,   255,   256,   257,     0,     0,   258,   259,   260
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-725))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,    23,     2,   321,    70,    16,    41,    42,    19,   144,
     344,   598,   280,    15,    25,    26,    27,     3,   162,   154,
     280,     3,   160,    34,    35,     3,    37,    38,     3,   619,
     619,   175,     3,    44,    45,    46,    47,    48,   326,    32,
      51,    19,   482,    54,    55,    56,    23,    60,    59,   193,
      47,    62,    63,    64,    32,    66,   344,    35,    69,    70,
      71,   227,   228,   229,   230,   659,     1,     1,    44,     1,
      48,    49,    23,    25,    23,    23,    58,    74,    54,   217,
      58,     1,   148,   101,    70,    15,    36,   101,     3,   813,
      32,    69,     0,   120,   101,    71,   162,   753,    32,   823,
      77,   128,     3,   130,    54,    32,   120,   694,   101,   175,
       3,   129,    32,   126,    32,    32,    77,   128,   129,   130,
     131,   132,   133,   134,   131,    60,    56,   193,    60,   718,
     127,    24,   276,   277,    22,    77,   792,   148,   140,    60,
      75,    71,   128,    75,   734,   734,   128,    62,    82,   126,
     128,   162,   101,   128,   594,   177,    69,   128,   134,   101,
      80,   101,    82,   481,   175,   100,   100,   101,   100,    57,
     131,   765,   766,    61,   101,   126,    69,   126,   126,   101,
     100,   101,   193,   101,   101,   196,   120,   127,   120,   131,
     101,   126,   779,    82,   126,    77,     7,   341,   101,   120,
     120,   212,    32,   369,   131,   126,    80,   129,    82,   209,
     276,   277,   100,   131,   131,   103,   105,   120,   129,   107,
     196,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   128,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   335,
     588,   101,    32,   780,   128,   341,   130,   128,   289,   547,
     720,    47,   101,   294,   236,   237,     2,   547,   354,     3,
     356,    32,    82,     7,   305,    11,   307,   308,    58,   310,
     311,   131,    82,   314,    20,     2,   813,   101,    74,   597,
     129,   101,   128,    66,    11,   105,   101,    33,   329,    60,
     128,   101,    36,    20,   335,   105,   278,   279,   120,   305,
     341,   469,   470,   127,    75,    51,    33,   289,   101,   129,
      54,    82,     3,   354,   129,   356,     7,   358,   359,   129,
     361,   362,   101,    69,    51,   366,   308,    73,    19,   311,
     101,   127,    15,   101,   698,    18,   129,   502,   702,   507,
     508,    42,    69,    26,    27,    28,    73,   126,   120,   120,
      22,    34,   120,    36,    49,   126,    13,   453,    49,   455,
     366,    18,   101,    58,   101,     3,   120,    58,    51,     7,
      53,    54,    55,    56,    69,   693,   358,   123,    69,    19,
     698,   120,    73,   120,   702,    57,    69,   101,    71,    61,
     128,   101,   130,    57,   101,    32,   123,    61,   120,   440,
     441,   442,   443,   444,   445,   446,   120,   125,   120,    49,
     120,   452,   453,   120,   455,   447,   448,   163,    58,   128,
     461,   130,   463,    60,    96,   128,   467,    99,   100,    69,
     102,   103,   354,    22,   356,   107,   163,   120,    75,   120,
      19,   187,   114,   484,   127,    82,   487,   488,   126,   121,
     122,   123,   120,   120,   120,    76,   126,   128,   499,   441,
     187,   443,   780,   209,   101,   128,   100,   103,    57,   103,
      49,   789,    61,   107,   128,    82,   120,   100,   484,    58,
       7,    18,   209,   120,   129,   120,   101,   120,    32,   126,
      27,    28,    25,   499,    73,   813,   101,    34,     2,    36,
     129,   129,    32,    80,    82,   823,   101,    11,   126,   550,
     129,   100,   553,   102,   103,   129,    20,    54,   107,   131,
     126,   129,   131,    57,   565,   114,    82,    25,   129,    33,
      60,   572,   129,   574,   123,   101,   577,   283,    15,   580,
      23,    18,    23,   584,   131,    75,   126,    51,   100,    26,
      27,    28,    82,   103,   128,   131,   283,    34,   131,    36,
     129,   307,    75,   604,   310,    69,   460,   155,   314,    73,
     611,   101,   613,   675,    51,   616,    53,    54,    55,    56,
     307,   498,   514,   310,   722,     3,   722,   314,   618,     7,
     120,     9,    69,   566,    71,   723,   126,   693,   604,   586,
     620,    19,   792,   779,   763,   330,   741,   455,   547,    -1,
      -1,    -1,    -1,   359,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,   664,   665,    -1,   667,   668,   669,   670,
      -1,    49,   359,    -1,    -1,    -1,   677,   678,    -1,   680,
      58,   682,   683,   120,    -1,   686,    -1,    -1,    -1,   714,
     715,    69,    -1,   749,    -1,    73,   697,   698,    -1,   163,
      -1,   702,   703,    -1,    -1,    -1,   707,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   187,   780,   726,   727,   728,   729,   730,
     731,   732,   733,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   743,   744,   745,   209,    -1,    32,   749,   436,
      35,   752,    -1,    -1,    -1,    -1,    -1,   813,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    -1,   823,   474,    -1,
      -1,    -1,    -1,    58,   775,   776,   777,   778,    -1,   745,
      -1,    -1,    -1,    -1,    69,    -1,    -1,   474,   789,   790,
     791,    -1,    -1,    -1,    -1,   796,   797,   798,   799,   800,
     801,    -1,    -1,    -1,    -1,   511,    -1,    -1,   514,    -1,
      -1,   812,   813,    -1,    -1,    -1,    -1,    -1,    -1,   283,
      -1,     2,   823,    -1,   511,    -1,    -1,   514,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,   307,    -1,    18,   310,    22,    -1,    -1,
     314,    -1,    33,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    34,   549,    36,    -1,    -1,   572,    -1,   574,    42,
      51,   577,    -1,    -1,   580,    -1,    -1,   583,    22,    -1,
      -1,    54,    57,    -1,    -1,   572,    61,   574,    69,    -1,
     577,    -1,    73,   580,    -1,   359,   583,   603,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   611,    -1,   613,   158,   159,
     160,    -1,   618,    57,    -1,    -1,   603,    61,    -1,    -1,
      -1,    96,   172,    -1,   611,   100,   613,   102,   103,    -1,
      -1,   618,   107,    -1,    -1,    -1,    -1,    81,    -1,   114,
     190,    -1,   123,    -1,    -1,    -1,   121,   122,   123,    -1,
     200,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,     2,   111,   112,   113,
     114,   677,   116,   117,   118,    11,   682,   121,   122,   123,
      -1,    -1,   163,    -1,    20,    -1,    -1,    -1,    -1,    18,
     677,    -1,    -1,    -1,    -1,   682,    -1,    33,    27,    28,
     706,    -1,    -1,    -1,   710,    34,   187,    36,    -1,    -1,
     474,    -1,    -1,    42,    -1,    51,    -1,    -1,    -1,   706,
      -1,    -1,    -1,   710,    -1,    54,    -1,    -1,   209,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    73,    -1,    -1,
     290,    -1,    -1,    -1,     2,    -1,    -1,   511,    -1,    -1,
     514,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,   313,    -1,    -1,    -1,    -1,    -1,   775,
     776,   777,   778,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   790,   791,    -1,   123,   775,   776,
     777,   778,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   283,   790,   791,    -1,    -1,    -1,   572,    -1,
     574,    69,    -1,   577,    -1,    73,   580,    -1,    -1,   583,
      -1,    -1,    -1,    -1,    -1,    -1,   307,   163,    -1,   310,
      -1,    -1,    -1,   314,    -1,     2,    -1,    -1,    -1,   603,
      -1,    -1,    -1,    -1,    11,    -1,    -1,   611,    -1,   613,
      -1,   187,    -1,    20,   618,    -1,    15,    -1,    -1,    18,
      -1,    -1,    -1,    -1,    -1,   123,    33,    26,    27,    28,
      -1,    -1,    -1,   209,    -1,    34,    -1,    36,   359,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    53,    54,    55,    56,    -1,    -1,
      -1,    -1,    69,    -1,    -1,   163,    73,    -1,    -1,    -1,
      69,    -1,    71,   677,    -1,   465,   466,    -1,   682,   469,
     470,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   706,    -1,    -1,    -1,   710,   283,    -1,    -1,
      -1,   209,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      19,   120,    -1,    -1,    47,    -1,    -1,    -1,   127,    -1,
      -1,   307,    -1,    32,   310,    -1,    35,    -1,   314,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    -1,    -1,   474,    -1,    -1,   163,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   558,   559,
      69,   775,   776,   777,   778,    -1,    -1,    57,    -1,   569,
     187,    61,    81,   359,    -1,   283,   790,   791,    -1,   579,
     511,    -1,   582,   514,    22,    -1,    -1,    96,    -1,    98,
      99,    -1,   209,    -1,   127,    -1,    -1,    -1,    -1,   307,
      -1,    -1,   310,    -1,    -1,    -1,   314,    -1,   117,   118,
     100,    -1,   102,   103,   123,    -1,    -1,   107,    -1,    57,
      -1,    -1,    -1,    61,   114,   158,   159,   160,   161,    -1,
      -1,   121,   122,   123,    -1,    -1,    -1,    -1,    22,   172,
      -1,   572,    -1,   574,    -1,    -1,   577,    -1,    -1,   580,
      -1,   359,   583,    -1,    -1,    -1,    -1,   190,    96,    -1,
      98,    99,   100,    -1,   102,   103,   283,   200,    -1,   107,
      -1,    -1,   603,    57,    -1,    -1,   114,    61,    -1,    -1,
     611,    -1,   613,   121,   122,   123,    -1,   618,   474,    -1,
     307,    -1,    -1,   310,    -1,    -1,    -1,   314,    -1,    -1,
     700,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,     2,   107,    -1,   511,    -1,    -1,   514,   113,
     114,    11,    -1,    -1,   118,    -1,    -1,   121,   122,   123,
      20,    -1,   359,    -1,    -1,    -1,   677,    -1,    -1,    -1,
      -1,   682,    -1,    33,    -1,    -1,    -1,   290,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   474,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,   706,    -1,    -1,    -1,   710,
     313,    -1,    -1,    -1,    -1,    -1,   572,    -1,   574,    69,
      -1,   577,    -1,    73,   580,    -1,    -1,   583,    -1,    -1,
      -1,    -1,    -1,   511,    -1,    -1,   514,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   603,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   611,    -1,   613,    -1,    -1,
      -1,    -1,   618,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   775,   776,   777,   778,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   474,    -1,   790,
     791,    -1,    -1,    -1,   572,    -1,   574,     2,    -1,   577,
      -1,    -1,   580,    -1,    -1,   583,    11,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,    20,    -1,    -1,    -1,    -1,
      -1,   677,    -1,    -1,   511,   603,   682,   514,    33,    -1,
      -1,    -1,    -1,   611,    -1,   613,    -1,   187,    -1,    -1,
     618,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
     706,    -1,    -1,    -1,   710,    -1,    -1,    -1,    -1,   209,
      -1,    -1,   465,   466,    69,    -1,   469,   470,    73,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,   572,    -1,   574,    -1,    -1,
     577,    -1,    -1,   580,    -1,    -1,   583,    -1,    -1,   677,
      -1,    -1,    -1,    -1,   682,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,   128,   603,   130,   123,   775,
     776,   777,   778,    -1,   611,    -1,   613,    -1,   706,    -1,
      -1,   618,   710,   283,   790,   791,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   558,   559,   307,   163,    -1,
     310,    -1,    -1,    -1,   314,    -1,   569,    -1,   571,    -1,
      -1,    -1,    -1,   576,    -1,    22,   579,    -1,    -1,   582,
      -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     677,    -1,    -1,    -1,    -1,   682,    -1,   775,   776,   777,
     778,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,   359,
      57,    -1,   790,   791,    61,    -1,    -1,    -1,    -1,   706,
      -1,    -1,    -1,   710,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    81,    -1,    18,    -1,    -1,    -1,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    96,
      32,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,    -1,   116,
      52,   118,    -1,    -1,   121,   122,   123,    -1,   283,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   775,   776,
     777,   778,    -1,    -1,    -1,    -1,    -1,   700,    -1,    -1,
      -1,    -1,   307,   790,   791,   310,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   474,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   736,   737,    -1,   739,   740,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     753,    -1,    -1,    -1,   359,   758,    -1,   760,    -1,    -1,
      -1,   511,   144,    -1,   514,    -1,   148,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,    -1,    -1,    -1,    -1,    -1,
     162,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   792,
      -1,    -1,    -1,   175,    -1,   177,    -1,    22,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   572,    -1,   574,    -1,    -1,   577,    -1,    -1,
     580,    -1,    57,   583,    -1,    -1,    61,    -1,    -1,    -1,
     222,    -1,    -1,    -1,    -1,   227,   228,   229,   230,    -1,
      -1,    -1,    -1,   603,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   611,   244,   613,    -1,    -1,    -1,    -1,   618,   474,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
      -1,   116,    -1,   118,   276,   277,   121,   122,   123,    -1,
      -1,    -1,    -1,    22,    -1,    -1,   511,    -1,    -1,   514,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   301,
      -1,   303,    -1,    -1,    -1,    -1,    -1,   677,    -1,    -1,
      -1,    -1,   682,    -1,    -1,    -1,    -1,    -1,    57,   321,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,   330,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,    -1,   341,
     710,    -1,    -1,    -1,    -1,    -1,    -1,   572,    -1,   574,
      -1,    -1,   577,    -1,    -1,   580,    -1,    96,   583,    98,
      99,   100,    -1,   102,   103,   104,    -1,   369,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,    -1,   603,   118,
      -1,    -1,   121,   122,   123,    -1,   611,    -1,   613,    -1,
      -1,    -1,    -1,   618,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,   775,   776,   777,   778,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
     790,   791,    29,    30,    31,    -1,    33,    -1,    -1,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   455,    -1,    -1,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,   481,
      -1,   706,    -1,    -1,    -1,   710,    -1,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     502,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   521,
      -1,   128,    -1,   130,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,   566,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,   588,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    22,    69,    -1,
      71,    72,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    57,   104,   636,    -1,    61,    -1,    -1,    -1,
      -1,    -1,   113,    -1,   115,    -1,   117,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,   126,    -1,   128,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,   118,    -1,    -1,   121,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   713,    -1,    -1,   716,    -1,    -1,    -1,    -1,     1,
     722,     3,     4,     5,     6,     7,     8,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    -1,    69,    -1,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
     122,   123,    -1,    -1,   126,   127,   128,     1,   130,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    -1,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,   115,    -1,   117,   118,   119,   120,    -1,    -1,   123,
      -1,    -1,   126,   127,   128,     1,   130,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    -1,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
      -1,   117,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
     126,   127,   128,     1,   130,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    12,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    -1,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,   117,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,   126,   127,
     128,     1,   130,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    12,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      -1,    71,    72,    73,    -1,    -1,    76,    -1,    78,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    -1,   117,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,   126,   127,   128,     1,
     130,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    -1,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    -1,   117,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,   126,   127,   128,     1,   130,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    -1,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,   115,    -1,   117,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,   126,    -1,   128,     1,   130,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    -1,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
      -1,   117,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
     126,    -1,   128,     1,   130,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    12,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    -1,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,   117,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,   126,    -1,
     128,     1,   130,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    12,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      -1,    71,    72,    73,    -1,    -1,    76,    -1,    78,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    -1,   117,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,   126,    -1,   128,     1,
     130,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    -1,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    -1,   117,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,   126,    -1,   128,     1,   130,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    55,    56,    -1,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    -1,    71,    72,    73,
      -1,    -1,    76,    -1,    78,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,   115,    -1,   117,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,   126,    -1,   128,     1,   130,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    -1,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    -1,    71,    72,    73,    -1,    -1,
      76,    -1,    78,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
      -1,   117,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
     126,    -1,   128,     1,   130,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    12,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    -1,
      -1,    49,    50,    -1,    52,    53,    54,    55,    56,    -1,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    -1,    71,    72,    73,    -1,    -1,    76,    -1,
      78,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    -1,   117,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,   126,    -1,
     128,     1,   130,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    12,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    54,    55,    56,    -1,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      -1,    71,    72,    73,    -1,    -1,    76,    -1,    78,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    -1,   117,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,   126,    -1,   128,     1,
     130,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    -1,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    -1,   117,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,   126,    -1,   128,     1,   130,     3,
       4,     5,     6,     7,     8,    -1,    10,    11,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    -1,    -1,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
      -1,    -1,   126,    -1,   128,     1,   130,     3,     4,     5,
       6,     7,     8,    -1,    10,    11,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    61,    -1,    -1,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,    -1,   121,   122,   123,    -1,    -1,
     126,    -1,   128,     1,   130,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,     1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,   104,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,   113,    33,   115,    -1,   117,
     118,    38,    39,    -1,    41,   123,    43,    44,   126,    -1,
     128,    -1,   130,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,   104,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   113,    33,   115,    -1,
     117,   118,    38,    39,    -1,    41,   123,    43,    44,   126,
      -1,   128,    -1,   130,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    -1,
       3,     4,     5,     6,     7,     8,    -1,   113,    11,   115,
      -1,   117,   118,   119,    -1,    -1,    -1,   123,    -1,    -1,
     126,    24,   128,    -1,   130,   131,    29,    30,    31,    -1,
      33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,   104,   105,     3,     4,     5,     6,     7,     8,    -1,
     113,    11,   115,    -1,   117,   118,   119,    -1,    -1,    -1,
     123,    -1,    -1,   126,    24,   128,    -1,   130,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    -1,     3,     4,     5,     6,
       7,     8,    -1,   113,    11,   115,    -1,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,    -1,   126,    24,   128,   129,
     130,    -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    81,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,   104,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   113,    33,   115,    -1,
     117,   118,    38,    39,    -1,    41,   123,    43,    44,   126,
      -1,   128,    -1,   130,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    -1,
       3,     4,     5,     6,     7,     8,    -1,   113,    11,   115,
      -1,   117,   118,   119,    -1,    -1,    -1,   123,    -1,    -1,
     126,    24,   128,    -1,   130,    -1,    29,    30,    31,    -1,
      33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,   104,    -1,     3,     4,     5,     6,     7,     8,    -1,
     113,    11,   115,    -1,   117,   118,   119,    -1,    -1,    -1,
     123,    -1,    -1,   126,    24,   128,    -1,   130,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    -1,   117,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,   126,    -1,   128,    -1,
     130,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,   113,    33,   115,    -1,   117,   118,    38,    39,    -1,
      41,   123,    43,    44,   126,    -1,   128,    -1,   130,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,   104,    -1,     3,     4,     5,     6,     7,
       8,    -1,   113,    11,   115,    -1,   117,   118,    -1,    -1,
      -1,    -1,   123,    -1,    -1,   126,    24,   128,    -1,   130,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    45,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,   104,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,   113,    33,   115,    -1,   117,
     118,    38,    39,    -1,    41,   123,    43,    44,   126,    -1,
     128,    -1,   130,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,   104,    -1,     3,
       4,     5,     6,     7,     8,    -1,   113,    11,   115,    -1,
     117,   118,   119,    -1,    -1,    -1,   123,    -1,    -1,   126,
      24,   128,    -1,   130,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
     104,    -1,     3,     4,     5,     6,     7,     8,    -1,   113,
      11,   115,    -1,   117,   118,   119,    -1,    -1,    -1,   123,
      -1,    -1,   126,    24,   128,    -1,   130,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,   104,    -1,     3,     4,     5,     6,     7,
       8,    -1,   113,    11,   115,    -1,   117,   118,   119,    -1,
      -1,    -1,   123,    -1,    -1,   126,    24,   128,    -1,   130,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    96,    97,
      98,    99,    -1,    -1,    -1,    -1,   104,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,   113,    33,   115,    -1,   117,
     118,    38,    39,    -1,    41,   123,    43,    44,   126,    -1,
     128,    -1,   130,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    96,
      97,    98,    99,    -1,    -1,    -1,    -1,   104,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   113,    33,   115,    -1,
     117,   118,    38,    39,    -1,    41,   123,    43,    44,   126,
      -1,   128,    -1,   130,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,   113,    33,   115,
      -1,   117,   118,    38,    39,    -1,    41,   123,    43,    44,
     126,    -1,   128,    -1,   130,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,
      -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,   113,    33,
     115,    -1,   117,   118,    38,    39,    -1,    41,   123,    43,
      44,   126,    -1,   128,    -1,   130,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    -1,
      -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,
     104,    24,    -1,    -1,    -1,    -1,    29,    30,    31,   113,
      33,   115,    -1,   117,   118,    38,    39,    -1,    41,   123,
      43,    44,   126,    -1,   128,    -1,   130,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,    -1,
      -1,   104,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
     113,    33,   115,    -1,   117,   118,    38,    39,    -1,    41,
     123,    43,    44,   126,    -1,   128,    -1,   130,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,   113,    33,   115,    -1,   117,   118,    38,    39,    -1,
      41,   123,    43,    44,   126,    -1,   128,    -1,   130,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    -1,    -1,    -1,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,   104,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,   113,    33,   115,    -1,   117,   118,    38,    39,
      -1,    41,   123,    43,    44,   126,    -1,   128,    -1,   130,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,   113,    33,   115,    -1,   117,   118,    38,
      39,    -1,    41,   123,    43,    44,   126,    -1,   128,    -1,
     130,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    -1,    -1,    -1,    96,    97,    98,
      99,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,   113,    33,   115,    -1,    -1,   118,
      38,    39,    -1,    41,   123,    43,    44,   126,    -1,   128,
      -1,   130,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    64,    65,    66,    67,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     128,   129,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    -1,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
      -1,    -1,   102,    -1,    -1,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,   118,    -1,
      -1,   121,   122,   123,   124,   125,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    57,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    10,    -1,    -1,    81,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    57,    -1,    -1,
      -1,    61,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    81,   116,   117,   118,    -1,    -1,   121,   122,   123,
      -1,    57,    -1,    -1,    -1,    61,    96,   131,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    81,   116,   117,   118,    -1,
      -1,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      96,   131,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    10,
     116,   117,   118,    14,    -1,   121,   122,   123,    -1,    -1,
      -1,    22,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    57,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    61,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    81,   116,   117,   118,    -1,    -1,
     121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    96,
     131,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    10,   116,
     117,   118,    14,    -1,   121,   122,   123,    -1,    -1,    -1,
      22,    23,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    57,    14,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    77,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    57,
     102,   103,   104,    61,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,    -1,    -1,   121,
     122,   123,    10,    81,   126,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,    -1,    -1,   121,   122,   123,    -1,    -1,   126,    57,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      10,    -1,    -1,    81,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,    -1,    -1,   121,   122,   123,    -1,    57,   126,    -1,
      -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,    -1,
      57,   121,   122,   123,    61,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,    -1,    57,   121,   122,   123,    61,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,    -1,    57,   121,   122,   123,
      61,    -1,   126,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,   116,   117,   118,    -1,    57,
     121,   122,   123,    61,    -1,   126,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,    -1,    57,   121,   122,   123,    61,    -1,   126,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,   116,   117,   118,    -1,    57,   121,   122,   123,    61,
      -1,   126,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,    -1,    57,   121,
     122,   123,    61,    -1,   126,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    57,   121,   122,   123,    61,    -1,   126,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,    -1,    57,   121,   122,   123,    61,    -1,
     126,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,   116,   117,   118,    -1,    57,   121,   122,
     123,    61,    -1,   126,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,    -1,
      57,   121,   122,   123,    61,    -1,   126,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    57,   116,
     117,   118,    61,    -1,   121,   122,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    57,   121,   122,   123,    61,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    10,    -1,    81,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    32,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,    -1,    -1,   121,   122,   123,    -1,    -1,
      57,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,    -1,    57,   121,   122,   123,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    57,   116,   117,   118,    61,    -1,   121,   122,   123,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    57,
     116,   117,   118,    61,   120,   121,   122,   123,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    57,   116,   117,
     118,    61,   120,   121,   122,   123,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    57,   116,   117,   118,    61,
     120,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    80,    81,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,    -1,    57,   121,
     122,   123,    61,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    57,   121,   122,   123,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    57,
     116,   117,   118,    61,   120,   121,   122,   123,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    57,   116,   117,
     118,    61,   120,   121,   122,   123,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    57,   116,   117,   118,    61,
     120,   121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    57,   116,   117,   118,    61,   120,   121,
     122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    57,   116,   117,   118,    61,   120,   121,   122,   123,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    57,
     116,   117,   118,    61,   120,   121,   122,   123,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    57,   116,   117,
     118,    61,   120,   121,   122,   123,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    57,   116,   117,   118,    61,
     120,   121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    57,   116,   117,   118,    61,   120,   121,
     122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    57,   116,   117,   118,    61,   120,   121,   122,   123,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    57,
     116,   117,   118,    61,   120,   121,   122,   123,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    57,   116,   117,
     118,    61,   120,   121,   122,   123,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    57,   116,   117,   118,    61,
     120,   121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    57,   116,   117,   118,    61,   120,   121,
     122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    57,   116,   117,   118,    61,   120,   121,   122,   123,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    57,
     116,   117,   118,    61,    -1,   121,   122,   123,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    81,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    57,   116,   117,
     118,    61,   120,   121,   122,   123,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    57,   116,   117,   118,    61,
      -1,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    80,    81,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,    -1,    57,   121,
     122,   123,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    57,   121,   122,   123,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    57,
     116,   117,   118,    61,    -1,   121,   122,   123,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,    -1,    57,   121,   122,   123,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,   116,   117,   118,    -1,    57,   121,   122,   123,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,    -1,    57,   121,
     122,   123,    61,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    57,   121,   122,   123,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,    -1,    57,   121,   122,   123,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,   116,   117,   118,    -1,    57,   121,   122,
     123,    61,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
     100,    -1,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,    -1,
      57,   121,   122,   123,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,    -1,    57,   121,   122,   123,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,    -1,    -1,   121,   122,   123
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   136,   137,     0,     1,     3,     4,     5,     6,     7,
       8,    11,    12,    13,    15,    16,    17,    18,    20,    21,
      23,    24,    26,    27,    28,    29,    30,    31,    33,    34,
      37,    38,    39,    40,    41,    43,    44,    46,    50,    51,
      52,    53,    55,    56,    59,    60,    62,    63,    64,    65,
      66,    67,    69,    71,    72,    76,    78,    81,    96,    97,
      98,    99,   104,   113,   115,   117,   118,   120,   123,   126,
     128,   130,   138,   139,   140,   141,   142,   143,   145,   146,
     147,   149,   152,   153,   154,   155,   156,   158,   159,   162,
     163,   164,   167,   170,   173,   174,   194,   197,   198,   216,
     217,   218,   219,   220,   221,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   237,   238,   239,   240,   241,
     140,   229,    77,   222,   223,   148,   149,   222,    11,    29,
      30,    31,    67,   126,   130,   170,   216,   220,   227,   228,
     229,   230,   232,   233,    69,   148,   229,   140,   128,   149,
       7,   148,   150,     9,    69,   150,    49,    79,   157,   229,
     229,   229,   128,   149,   171,   128,   149,   199,   200,   140,
     229,   229,   229,   229,     7,   128,    18,    28,   154,   154,
     119,   190,   210,   229,   120,   229,   229,    23,   143,   151,
     229,   229,    66,   128,   140,   229,    62,   149,   195,   210,
     229,   229,   229,   229,   229,   229,   229,   127,   138,   144,
     210,    70,   105,   190,   211,   212,   229,   210,   229,   236,
      51,   140,    42,   149,    36,    54,   186,    19,    49,    58,
      73,   120,   125,    10,    14,    22,    57,    61,    81,    96,
      98,    99,   100,   102,   103,   104,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   116,   117,   118,   121,   122,
     123,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,   124,   128,   130,    57,    61,
     128,   140,   120,   126,   143,   229,   229,   229,   210,    32,
     222,   195,   120,   120,    76,   149,   190,   213,   214,   215,
     229,   126,   195,   164,   149,   128,   151,    23,    32,   151,
      23,    32,   151,   223,    68,   143,   213,   140,   128,   181,
      70,   128,   149,   201,   202,     1,   100,   204,   205,    32,
     101,   151,   213,   150,   149,   101,   120,   120,   126,   140,
     151,   128,   213,   210,    82,   196,   120,   120,   151,   120,
     127,   138,   127,   229,   101,   129,   101,   129,    32,   131,
     223,    80,   101,   131,     7,   149,   100,   165,   175,    58,
     199,   199,   199,   199,   229,   229,   229,   229,   157,   229,
     157,   229,   229,   229,   229,   229,   229,   229,    24,    69,
     149,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   213,   213,   157,   229,   157,   229,    19,    32,
      35,    48,    49,    58,    69,   118,   184,   224,   225,   241,
      23,    32,    23,    32,    68,    32,   131,   157,   229,   151,
     120,   229,    80,    82,   129,   101,   149,   168,   169,   120,
     149,    32,   210,    31,   229,   157,   229,    31,   229,   157,
     229,   151,   140,   229,    25,   129,   182,   183,   184,   172,
     202,   101,   129,     1,   130,   206,   217,    80,    82,   203,
     229,   200,   129,   190,   229,   160,   213,   129,   126,   130,
     206,   217,   101,   129,   129,   211,   211,   157,   229,   140,
     229,   131,   229,   229,   126,   210,   126,    49,    58,    69,
     177,   185,   199,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   129,   131,    32,    58,   216,   101,   129,    57,
      31,   157,   229,    31,   157,   229,   157,   229,   222,   222,
     120,   229,   190,   229,   215,    82,   101,   127,   165,   229,
     129,   229,    23,   151,    23,   151,   229,    23,   151,   223,
      23,   151,   223,    25,    25,   140,   101,   129,   128,   149,
      19,    49,    58,    69,   187,   129,   202,   100,   205,   210,
     229,    45,   229,    47,    74,   127,   161,   129,   160,   210,
     195,   131,   223,   131,   223,   140,    80,   127,   144,   166,
     176,    10,    14,    82,    83,    84,    85,    86,    87,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   102,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   118,   121,   122,   123,   124,   125,   149,   178,   179,
     120,   225,   216,   229,    23,    23,   229,    23,    23,   131,
     131,   151,   151,   229,   169,   126,   151,    68,    31,   229,
      31,   229,    68,    31,   229,   151,    31,   229,   151,   140,
     229,   183,   202,   100,   209,   205,   203,    32,   131,   140,
     210,   127,   131,    31,   140,   229,   131,    31,   140,   229,
     131,   229,   127,    28,    53,    55,    69,   127,   139,   153,
     187,   149,   103,   128,   180,   180,    68,    31,    31,    68,
      31,    31,    31,    31,   166,   229,   229,   229,   229,   229,
     229,   129,    24,    64,    67,   130,   190,   208,   217,   105,
     192,   203,    75,   193,   229,   206,   217,   151,   229,   140,
     229,   140,   153,   205,   149,   178,   179,   182,   229,   229,
     229,   229,   229,   229,   127,    68,    68,    68,    68,   209,
     131,   210,   190,   191,   229,   229,   143,   152,   189,   131,
      68,    68,   193,   180,   180,   129,    68,    68,    68,    68,
      68,    68,   229,   229,   229,   229,   192,   203,   190,   207,
     208,   217,    32,   131,   217,   229,   229,   120,   188,   189,
     229,   207,   208,   131,   207
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
#line 403 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 408 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 409 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 416 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 421 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 422 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 451 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 453 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 466 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 519 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 559 "chapel.ypp"
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
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 659 "chapel.ypp"
    { USR_FATAL((yyvsp[(3) - (6)].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 666 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 671 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 678 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 683 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 696 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 702 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 706 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 711 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 721 "chapel.ypp"
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
#line 730 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 734 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 742 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 747 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
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
#line 777 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 790 "chapel.ypp"
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
#line 798 "chapel.ypp"
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
#line 809 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 815 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 822 "chapel.ypp"
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
#line 844 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 848 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 853 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 857 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 962 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 963 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 967 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 980 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 984 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
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
#line 1022 "chapel.ypp"
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
#line 1038 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1061 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1068 "chapel.ypp"
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
#line 1076 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1092 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1134 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1146 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1147 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1176 "chapel.ypp"
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
#line 1183 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1194 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1196 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1316 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1323 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1329 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1335 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1341 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1370 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1386 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1394 "chapel.ypp"
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
#line 1403 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1445 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1476 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1498 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7516 "bison-chapel.cpp"
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



