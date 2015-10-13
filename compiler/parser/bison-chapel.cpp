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
#define YYLAST   10882

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  135
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  429
/* YYNRULES -- Number of states.  */
#define YYNSTATES  818

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
      43,    46,    49,    53,    57,    61,    65,    69,    73,    76,
      80,    84,    87,    90,    94,    96,   102,   109,   110,   112,
     114,   117,   121,   123,   126,   130,   134,   139,   144,   149,
     154,   159,   164,   169,   174,   179,   184,   189,   194,   199,
     204,   209,   210,   212,   214,   216,   218,   221,   223,   226,
     230,   232,   234,   237,   240,   242,   244,   246,   248,   250,
     253,   259,   263,   270,   277,   282,   288,   294,   298,   302,
     309,   315,   322,   328,   335,   339,   344,   351,   359,   366,
     374,   379,   385,   390,   395,   399,   406,   412,   418,   419,
     422,   426,   429,   436,   445,   447,   449,   451,   452,   455,
     456,   459,   463,   469,   471,   474,   478,   480,   484,   485,
     486,   495,   496,   498,   501,   504,   505,   506,   516,   520,
     524,   530,   536,   538,   541,   543,   545,   547,   549,   551,
     553,   555,   557,   559,   561,   563,   565,   567,   569,   571,
     573,   575,   577,   579,   581,   583,   585,   587,   589,   591,
     593,   595,   597,   599,   601,   603,   605,   607,   609,   611,
     613,   614,   618,   622,   623,   625,   629,   634,   639,   646,
     653,   654,   656,   658,   660,   662,   665,   668,   670,   672,
     674,   675,   677,   679,   681,   683,   685,   686,   688,   690,
     692,   694,   696,   698,   700,   702,   705,   707,   708,   710,
     713,   716,   717,   720,   724,   729,   734,   737,   742,   743,
     746,   749,   754,   759,   764,   770,   775,   776,   778,   780,
     782,   786,   790,   795,   801,   803,   805,   809,   811,   814,
     818,   819,   822,   825,   826,   831,   832,   835,   838,   840,
     845,   850,   857,   859,   860,   862,   864,   868,   873,   877,
     882,   889,   890,   893,   896,   899,   902,   905,   908,   910,
     912,   916,   920,   922,   924,   926,   930,   934,   935,   937,
     939,   943,   947,   951,   955,   957,   959,   961,   963,   965,
     967,   969,   971,   974,   979,   984,   989,   995,   998,  1001,
    1008,  1015,  1020,  1030,  1040,  1048,  1055,  1062,  1067,  1077,
    1087,  1095,  1100,  1107,  1114,  1124,  1134,  1141,  1143,  1145,
    1147,  1149,  1151,  1153,  1155,  1157,  1161,  1162,  1164,  1169,
    1171,  1175,  1178,  1182,  1184,  1188,  1191,  1196,  1198,  1200,
    1202,  1204,  1206,  1208,  1210,  1212,  1217,  1221,  1225,  1228,
    1231,  1233,  1235,  1237,  1239,  1241,  1243,  1245,  1250,  1255,
    1260,  1264,  1268,  1272,  1276,  1281,  1285,  1287,  1289,  1291,
    1293,  1295,  1299,  1303,  1307,  1311,  1317,  1321,  1325,  1329,
    1333,  1337,  1341,  1345,  1349,  1353,  1357,  1361,  1365,  1369,
    1373,  1377,  1381,  1385,  1389,  1393,  1397,  1401,  1405,  1409,
    1412,  1415,  1418,  1421,  1424,  1427,  1431,  1435,  1439,  1443,
    1447,  1451,  1455,  1459,  1461,  1463,  1465,  1467,  1469,  1471
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     136,     0,    -1,   137,    -1,    -1,   137,   138,    -1,   140,
      -1,   139,   140,    -1,    51,     7,    -1,   139,    51,     7,
      -1,   141,    -1,   143,    -1,   145,    -1,   146,    -1,   153,
      -1,   147,    -1,   155,    -1,   158,    -1,   156,    -1,   159,
      -1,   152,    -1,   220,   120,    -1,    11,   140,    -1,    12,
     221,   140,    -1,    13,   148,   120,    -1,    16,   221,   143,
      -1,    20,   148,   120,    -1,    21,   228,   120,    -1,    37,
     149,   140,    -1,    40,   140,    -1,    46,   228,   151,    -1,
      63,   228,   151,    -1,    63,   151,    -1,    67,   140,    -1,
      78,   228,   120,    -1,     1,    -1,   142,    42,   149,   126,
     127,    -1,   142,    42,   149,   126,   144,   127,    -1,    -1,
      55,    -1,    53,    -1,   126,   127,    -1,   126,   144,   127,
      -1,   138,    -1,   144,   138,    -1,    72,   209,   120,    -1,
      59,   209,   120,    -1,   229,    82,   228,   120,    -1,   229,
      93,   228,   120,    -1,   229,    90,   228,   120,    -1,   229,
      92,   228,   120,    -1,   229,    86,   228,   120,    -1,   229,
      91,   228,   120,    -1,   229,    87,   228,   120,    -1,   229,
      83,   228,   120,    -1,   229,    84,   228,   120,    -1,   229,
      85,   228,   120,    -1,   229,    95,   228,   120,    -1,   229,
      94,   228,   120,    -1,   229,   124,   228,   120,    -1,   229,
      88,   228,   120,    -1,   229,    89,   228,   120,    -1,    -1,
     149,    -1,     3,    -1,   148,    -1,     7,    -1,    23,   140,
      -1,   143,    -1,    60,   120,    -1,    60,   228,   120,    -1,
     120,    -1,   154,    -1,    55,   154,    -1,    53,   154,    -1,
     166,    -1,   193,    -1,   162,    -1,   173,    -1,   196,    -1,
      28,     9,    -1,    23,   140,    76,   228,   120,    -1,    76,
     228,   151,    -1,    17,   228,    32,   228,   221,   151,    -1,
      17,   228,    32,   157,   221,   151,    -1,    17,   228,   221,
     151,    -1,    29,   228,    32,   228,   151,    -1,    29,   228,
      32,   157,   151,    -1,    29,   228,   151,    -1,    29,   157,
     151,    -1,    29,    49,   149,    32,   228,   151,    -1,    30,
     228,    32,   228,   151,    -1,    30,   228,    32,   228,   222,
     151,    -1,    30,   228,    32,   157,   151,    -1,    30,   228,
      32,   157,   222,   151,    -1,    30,   228,   151,    -1,    30,
     228,   222,   151,    -1,   130,   209,    32,   228,   131,   140,
      -1,   130,   209,    32,   228,   222,   131,   140,    -1,   130,
     209,    32,   157,   131,   140,    -1,   130,   209,    32,   157,
     222,   131,   140,    -1,   130,   209,   131,   140,    -1,   130,
     209,   222,   131,   140,    -1,    79,   128,   209,   129,    -1,
      31,   228,    68,   140,    -1,    31,   228,   143,    -1,    31,
     228,    68,   140,    25,   140,    -1,    31,   228,   143,    25,
     140,    -1,    62,   228,   126,   160,   127,    -1,    -1,   160,
     161,    -1,    74,   209,   151,    -1,    47,   140,    -1,   163,
     149,   164,   126,   165,   127,    -1,    28,   150,   163,   149,
     164,   126,   165,   127,    -1,    15,    -1,    56,    -1,    71,
      -1,    -1,   100,   209,    -1,    -1,   165,   153,    -1,   165,
     139,   153,    -1,    26,   149,   126,   167,   127,    -1,   168,
      -1,   167,   101,    -1,   167,   101,   168,    -1,   149,    -1,
     149,    82,   228,    -1,    -1,    -1,    38,   170,   180,   171,
     186,   204,   192,   188,    -1,    -1,    34,    -1,    27,   150,
      -1,    28,   150,    -1,    -1,    -1,   172,   185,   174,   176,
     175,   186,   204,   192,   187,    -1,   184,   177,   179,    -1,
     184,   178,   179,    -1,   184,   149,   103,   177,   179,    -1,
     184,   149,   103,   178,   179,    -1,   149,    -1,    97,   149,
      -1,    96,    -1,    98,    -1,    99,    -1,    97,    -1,   106,
      -1,   116,    -1,   112,    -1,   109,    -1,   111,    -1,   108,
      -1,   118,    -1,   113,    -1,   123,    -1,   102,    -1,   121,
      -1,   122,    -1,   114,    -1,   107,    -1,   115,    -1,    14,
      -1,   110,    -1,    10,    -1,   124,    -1,   125,    -1,    82,
      -1,    93,    -1,    90,    -1,    92,    -1,    86,    -1,    91,
      -1,    87,    -1,    83,    -1,    84,    -1,    85,    -1,    95,
      -1,    94,    -1,    -1,   128,   181,   129,    -1,   128,   181,
     129,    -1,    -1,   182,    -1,   181,   101,   182,    -1,   183,
     149,   208,   202,    -1,   183,   149,   208,   191,    -1,   183,
     128,   201,   129,   208,   202,    -1,   183,   128,   201,   129,
     208,   191,    -1,    -1,    32,    -1,    35,    -1,    48,    -1,
      19,    -1,    19,    32,    -1,    19,    58,    -1,    49,    -1,
      58,    -1,    69,    -1,    -1,    49,    -1,    58,    -1,    69,
      -1,    54,    -1,    36,    -1,    -1,    19,    -1,    58,    -1,
      49,    -1,    69,    -1,   120,    -1,   188,    -1,   143,    -1,
     152,    -1,   119,   149,    -1,   119,    -1,    -1,   189,    -1,
     105,   228,    -1,   105,   190,    -1,    -1,    75,   228,    -1,
      69,   194,   120,    -1,    18,    69,   194,   120,    -1,    28,
      69,   194,   120,    -1,   149,   195,    -1,   149,   195,   101,
     194,    -1,    -1,    82,   216,    -1,    82,   205,    -1,   197,
      49,   198,   120,    -1,   197,    19,   198,   120,    -1,   197,
      58,   198,   120,    -1,   197,    19,    58,   198,   120,    -1,
     197,    73,   198,   120,    -1,    -1,    18,    -1,    28,    -1,
     199,    -1,   198,   101,   199,    -1,   149,   204,   202,    -1,
     149,   203,    80,   228,    -1,   128,   201,   129,   204,   202,
      -1,    70,    -1,   149,    -1,   128,   201,   129,    -1,   200,
      -1,   200,   101,    -1,   200,   101,   201,    -1,    -1,    82,
      45,    -1,    82,   228,    -1,    -1,   100,   130,   209,   131,
      -1,    -1,   100,   216,    -1,   100,   205,    -1,     1,    -1,
     130,   209,   131,   216,    -1,   130,   209,   131,   205,    -1,
     130,   209,    32,   228,   131,   216,    -1,     1,    -1,    -1,
     216,    -1,   189,    -1,   130,   131,   206,    -1,   130,   209,
     131,   206,    -1,   130,   131,   207,    -1,   130,   209,   131,
     207,    -1,   130,   209,    32,   228,   131,   206,    -1,    -1,
     100,   216,    -1,   100,   189,    -1,   100,    24,    -1,   100,
      64,    -1,   100,    67,    -1,   100,   207,    -1,   228,    -1,
     189,    -1,   209,   101,   228,    -1,   209,   101,   189,    -1,
      70,    -1,   228,    -1,   189,    -1,   210,   101,   210,    -1,
     211,   101,   210,    -1,    -1,   213,    -1,   214,    -1,   213,
     101,   214,    -1,   149,    82,   189,    -1,   149,    82,   228,
      -1,   149,    80,   228,    -1,   189,    -1,   228,    -1,   149,
      -1,   219,    -1,   229,    -1,   218,    -1,   237,    -1,   236,
      -1,    64,   228,    -1,    33,   128,   212,   129,    -1,    24,
     128,   212,   129,    -1,    66,   128,   212,   129,    -1,    65,
      66,   128,   212,   129,    -1,    11,   228,    -1,    67,   228,
      -1,    29,   228,    32,   228,    23,   228,    -1,    29,   228,
      32,   157,    23,   228,    -1,    29,   228,    23,   228,    -1,
      29,   228,    32,   228,    23,    31,   228,    68,   228,    -1,
      29,   228,    32,   157,    23,    31,   228,    68,   228,    -1,
      29,   228,    23,    31,   228,    68,   228,    -1,    30,   228,
      32,   228,    23,   228,    -1,    30,   228,    32,   157,    23,
     228,    -1,    30,   228,    23,   228,    -1,    30,   228,    32,
     228,    23,    31,   228,    68,   228,    -1,    30,   228,    32,
     157,    23,    31,   228,    68,   228,    -1,    30,   228,    23,
      31,   228,    68,   228,    -1,   130,   209,   131,   228,    -1,
     130,   209,    32,   228,   131,   228,    -1,   130,   209,    32,
     157,   131,   228,    -1,   130,   209,    32,   228,   131,    31,
     228,    68,   228,    -1,   130,   209,    32,   157,   131,    31,
     228,    68,   228,    -1,    31,   228,    68,   228,    25,   228,
      -1,    44,    -1,   219,    -1,   215,    -1,   232,    -1,   231,
      -1,   169,    -1,   226,    -1,   227,    -1,   225,   125,   228,
      -1,    -1,   222,    -1,    77,   128,   223,   129,    -1,   224,
      -1,   223,   101,   224,    -1,   183,   215,    -1,   240,    57,
     215,    -1,   229,    -1,   225,   125,   228,    -1,    43,   228,
      -1,    39,   198,    32,   228,    -1,   234,    -1,   216,    -1,
     217,    -1,   238,    -1,   239,    -1,   169,    -1,   226,    -1,
     227,    -1,   128,   105,   228,   129,    -1,   228,   100,   228,
      -1,   228,   104,   228,    -1,   228,   104,    -1,   104,   228,
      -1,   104,    -1,   215,    -1,   231,    -1,   232,    -1,   233,
      -1,   229,    -1,   169,    -1,   230,   128,   212,   129,    -1,
     230,   130,   212,   131,    -1,    52,   128,   212,   129,    -1,
     228,   103,   149,    -1,   228,   103,    69,    -1,   228,   103,
      24,    -1,   128,   210,   129,    -1,   128,   210,   101,   129,
      -1,   128,   211,   129,    -1,     4,    -1,     5,    -1,     6,
      -1,     7,    -1,     8,    -1,   126,   209,   127,    -1,   130,
     209,   131,    -1,   130,   235,   131,    -1,   228,    80,   228,
      -1,   235,   101,   228,    80,   228,    -1,   228,   118,   228,
      -1,   228,   113,   228,    -1,   228,   123,   228,    -1,   228,
     102,   228,    -1,   228,   121,   228,    -1,   228,   122,   228,
      -1,   228,   114,   228,    -1,   228,   106,   228,    -1,   228,
     116,   228,    -1,   228,   112,   228,    -1,   228,   109,   228,
      -1,   228,   111,   228,    -1,   228,   108,   228,    -1,   228,
      96,   228,    -1,   228,    98,   228,    -1,   228,    99,   228,
      -1,   228,    81,   228,    -1,   228,   117,   228,    -1,   228,
     107,   228,    -1,   228,    14,   228,    -1,   228,    10,   228,
      -1,   228,   110,   228,    -1,   228,    22,   228,    -1,   118,
     228,    -1,   113,   228,    -1,    41,   228,    -1,    50,   228,
      -1,   115,   228,    -1,    97,   228,    -1,   228,    57,   228,
      -1,   228,    57,   157,    -1,   240,    57,   228,    -1,   240,
      57,   157,    -1,   228,    61,   228,    -1,   228,    61,   157,
      -1,   240,    61,   228,    -1,   240,    61,   157,    -1,   118,
      -1,   123,    -1,    81,    -1,   117,    -1,    96,    -1,    98,
      -1,    99,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   403,   403,   408,   409,   415,   416,   421,   422,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   456,   458,   463,   464,   465,
     480,   481,   486,   487,   492,   496,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   518,   519,   523,   527,   528,   532,   533,   537,   538,
     542,   543,   544,   545,   546,   547,   548,   552,   553,   557,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   584,   585,   586,   587,   588,   594,   600,   606,
     612,   619,   629,   633,   634,   635,   636,   640,   645,   646,
     650,   652,   659,   664,   672,   677,   682,   690,   691,   696,
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
    1488,  1489,  1490,  1491,  1498,  1499,  1503,  1504,  1505,  1506,
    1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,
    1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1529,
    1530,  1531,  1532,  1533,  1534,  1538,  1539,  1540,  1541,  1545,
    1546,  1547,  1548,  1553,  1554,  1555,  1556,  1557,  1558,  1559
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
  "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
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
     385,   386,   387,   388,   389
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   135,   136,   137,   137,   138,   138,   139,   139,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   141,   141,   142,   142,   142,
     143,   143,   144,   144,   145,   146,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   148,   148,   149,   150,   150,   151,   151,   152,   152,
     153,   153,   153,   153,   153,   153,   153,   154,   154,   155,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   157,   158,   158,   158,   158,   159,   160,   160,
     161,   161,   162,   162,   163,   163,   163,   164,   164,   165,
     165,   165,   166,   167,   167,   167,   168,   168,   170,   171,
     169,   172,   172,   172,   172,   174,   175,   173,   176,   176,
     176,   176,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     179,   179,   180,   181,   181,   181,   182,   182,   182,   182,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     184,   184,   184,   184,   185,   185,   186,   186,   186,   186,
     186,   187,   187,   188,   188,   189,   189,   190,   190,   191,
     191,   192,   192,   193,   193,   193,   194,   194,   195,   195,
     195,   196,   196,   196,   196,   196,   197,   197,   197,   198,
     198,   199,   199,   199,   200,   200,   200,   201,   201,   201,
     202,   202,   202,   203,   203,   204,   204,   204,   204,   205,
     205,   205,   205,   206,   206,   206,   207,   207,   207,   207,
     207,   208,   208,   208,   208,   208,   208,   208,   209,   209,
     209,   209,   210,   210,   210,   211,   211,   212,   212,   213,
     213,   214,   214,   214,   214,   214,   215,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   218,   219,   220,   220,
     220,   220,   220,   220,   220,   220,   221,   221,   222,   223,
     223,   224,   224,   225,   225,   226,   227,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   229,   229,   229,   229,   230,   230,   231,   231,   231,
     232,   232,   232,   233,   233,   233,   234,   234,   234,   234,
     234,   234,   234,   234,   235,   235,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   237,
     237,   237,   237,   237,   237,   238,   238,   238,   238,   239,
     239,   239,   239,   240,   240,   240,   240,   240,   240,   240
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     3,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     0,     1,     1,     1,     1,     2,     1,     2,     3,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     2,
       5,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     4,     3,     6,     5,     5,     0,     2,
       3,     2,     6,     8,     1,     1,     1,     0,     2,     0,
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
       1,     3,     3,     3,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    63,   376,   377,   378,   379,
     380,     0,   336,    61,   114,   336,     0,   237,    61,     0,
       0,     0,     0,    61,    61,     0,     0,     0,     0,   132,
       0,   128,     0,     0,     0,     0,   327,     0,     0,     0,
       0,   236,   236,   115,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,     0,     0,     0,   425,   427,     0,
     428,   429,   360,     0,     0,   426,   423,    70,   424,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   296,    19,    13,    71,    15,    17,    16,    18,    76,
       0,    74,   352,     0,    77,    75,    78,     0,   361,   348,
     349,   299,   297,     0,     0,   353,   354,     0,   298,     0,
     362,   363,   364,   347,   301,   300,   350,   351,     0,    21,
     307,     0,     0,   337,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   361,   297,   353,   354,   336,
     298,   362,   363,     0,     0,     0,     0,   287,     0,    65,
      64,   133,    79,     0,   134,     0,     0,     0,     0,     0,
       0,   287,     0,     0,     0,     0,     0,   239,    28,   411,
     345,     0,   412,     7,   287,   237,   238,    73,    72,   216,
     279,     0,   278,    68,     0,     0,     0,    67,    31,     0,
     302,     0,   287,    32,   308,   228,     0,     0,     0,     0,
     414,   359,   410,   413,   409,    40,    42,     0,     0,   282,
       0,   284,     0,     0,   283,     0,   278,     0,     0,     6,
       0,   117,   205,   204,   135,     0,     0,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   358,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   287,   287,     0,     0,   190,    22,
      23,     0,    24,     0,     0,     0,     0,     0,     0,     0,
      25,    26,     0,   296,   294,     0,   288,   289,   295,     0,
       0,     0,     0,     0,    88,     0,     0,    87,     0,     0,
      94,     0,     0,   104,     0,    27,   183,   129,   244,     0,
     245,   247,     0,   258,     0,     0,   250,     0,     0,    29,
       0,   134,   215,     0,    45,    69,   108,    66,    30,   287,
       0,     0,   226,   223,    44,    81,    33,    41,    43,   381,
       0,     0,   373,     0,   375,     0,     0,     0,     0,     0,
     383,     8,     0,     0,     0,   200,     0,     0,     0,     0,
       0,   335,   406,   405,   408,   416,   415,   420,   419,   402,
     399,   400,   401,   356,   389,   372,   371,   370,   357,   393,
     404,   398,   396,   407,   397,   395,   387,   392,   394,   403,
     386,   390,   391,   388,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   418,   417,   422,   421,   194,   191,   192,   193,   197,
     198,   199,   423,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,   382,   336,   336,    84,   224,     0,     0,
       0,   304,     0,   126,     0,   123,   225,   117,     0,     0,
       0,   311,     0,     0,     0,   317,     0,     0,    95,   103,
       0,     0,   303,     0,   184,     0,   206,     0,   248,     0,
     262,     0,   257,   348,     0,     0,   241,   346,   240,   369,
     281,   280,     0,     0,   305,     0,   230,   348,     0,   355,
     374,   285,   286,     0,     0,   100,   321,     0,   384,     0,
       0,   118,   119,   201,   202,   203,   136,     0,     0,   232,
     231,   233,   235,    46,    53,    54,    55,    50,    52,    59,
      60,    48,    51,    49,    47,    57,    56,    58,   367,   368,
     195,   196,   341,   190,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,   293,   291,   292,
     290,     0,   124,   122,     0,     0,   102,     0,     0,    86,
       0,    85,     0,     0,    92,     0,     0,    90,     0,     0,
       0,   106,   190,   182,     0,   271,   207,   209,   208,   210,
       0,   246,   249,     0,   250,     0,   242,   251,   252,     0,
       0,   107,   109,   306,     0,   227,     0,     0,     0,     0,
     101,     0,    35,     0,   236,   206,   165,   163,   168,   175,
     176,   177,   172,   174,   170,   173,   171,   169,   179,   178,
     144,   147,   145,   146,   157,   148,   161,   153,   151,   164,
     152,   150,   155,   160,   162,   149,   154,   158,   159,   156,
     166,   167,   142,   180,   180,   234,   340,   342,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    82,   127,   125,
     119,    89,     0,     0,   310,     0,   309,     0,     0,   316,
      93,     0,   315,    91,   105,   326,   185,     0,     0,   250,
     221,   243,     0,     0,   111,     0,     0,     0,    98,   323,
       0,     0,    96,   322,     0,   385,    36,    61,   236,   236,
     112,   236,   120,     0,   143,     0,   183,   138,   139,     0,
       0,     0,     0,     0,     0,     0,     0,   236,   314,     0,
       0,   320,     0,     0,   271,   274,   275,   276,     0,   273,
     277,   348,   217,   187,   186,     0,     0,     0,   260,   348,
     110,     0,    99,     0,    97,   121,   221,   142,   180,   180,
       0,     0,     0,     0,     0,     0,     0,   113,     0,     0,
       0,     0,   250,   263,     0,   218,   220,   219,   222,   213,
     214,   130,     0,     0,     0,     0,   140,   141,   181,     0,
       0,     0,     0,     0,     0,   313,   312,   319,   318,   189,
     188,   265,   266,   268,   348,     0,   382,   348,   325,   324,
     211,   137,   212,     0,   267,   269,   263,   270
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   206,    73,   469,    75,    76,    77,   207,
      78,    79,    80,   150,    81,   154,   188,    82,    83,    84,
      85,    86,   552,    87,    88,   492,   602,    89,    90,   364,
     614,    91,   454,   455,   134,   163,   476,    93,    94,   365,
     615,   516,   653,   654,   717,   317,   473,   474,   475,   517,
     224,   590,   811,   781,   180,   776,   743,   746,    95,   196,
     342,    96,    97,   166,   167,   321,   322,   486,   325,   326,
     482,   802,   740,   689,   208,   212,   213,   295,   296,   297,
     135,    99,   100,   101,   136,   103,   122,   123,   434,   435,
     104,   137,   138,   107,   140,   109,   141,   142,   112,   113,
     217,   114,   115,   116,   117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -685
static const yytype_int16 yypact[] =
{
    -685,    63,  2168,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    -685,  3436,   -27,    74,  -685,   -27,  6223,    42,    74,  6223,
    3436,   -34,    74,   296,   544,  5234,  6223,  6223,   -20,  -685,
      74,  -685,    17,  3436,  6223,  6223,  -685,  6223,  6223,   134,
      16,   601,   791,  -685,  5418,  5521,  6223,  5649,  6223,    92,
      75,  3436,    74,  -685,  5418,  6223,  6223,  -685,  -685,  6223,
    -685,  -685,  1248,  6223,  6223,  -685,  6223,  -685,  -685,  2668,
    5028,  5418,  -685,  3308,  -685,  -685,   162,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
      74,  -685,    81,   164,  -685,  -685,  -685,   137,   150,  -685,
    -685,  -685,   159,   166,    87,   167,   191, 10759,  1938,   182,
     205,   223,  -685,  -685,  -685,  -685,  -685,  -685,   247,  -685,
   10759,   193,  3436,  -685,   230,  -685,   244,  6223,  6223,  6223,
    6223,  6223,  5418,  5418,   233,  -685,  -685,  -685,  -685,  8537,
     269,  -685,  -685,    74,   271,  8727,   338,  5418,   298,  -685,
    -685,  -685,  -685,    74,    68,    74,   295,    57,  7585,  7541,
    7723,  5418,  3436,   306,    36,    60,    33,  -685,  -685,   847,
     333,  7786,   847,  -685,  5418,  -685,   264,  -685,  -685,    74,
    -685,    93, 10759,  -685,  8785,  8416,  3436,  -685,  -685,  7786,
   10759,   316,  5418,  -685, 10759,   364,   330,    96,  7786,  8843,
     274,  1069,   847,   274,   847,  -685,  -685,  2796,    90,  -685,
    6223,  -685,    -6,    -3, 10759,   261,  8901,   -17,   444,  -685,
      74,   354,  -685,  -685,  -685,    18,    17,    17,    17,  -685,
    6223,  6223,  6223,  6223,  5730,  5730,  6223,  6223,  6223,  6223,
    6223,  6223,    88,  1248,  6223,  6223,  6223,  6223,  6223,  6223,
    6223,  6223,  6223,  6223,  6223,  6223,  6223,  6223,  6223,  6223,
    6223,  6223,  6223,  6223,  6223,  6223,  6223,  6223,  6223,  6223,
    6223,  6223,  6223,  6223,  5418,  5418,  5730,  5730,   670,  -685,
    -685,  2924,  -685,  8606,  8669,  8964,    58,  5730,    57,   335,
    -685,  -685,  6223,   326,  -685,   328,   357,  -685, 10759,    74,
     343,    74,   432,  5418,  -685,  3564,  5730,  -685,  3692,  5730,
    -685,    57,  3436,   441,   342,  -685,    38,  -685,  -685,    36,
    -685,   375,   348,  -685,  2331,   400,   403,  6223,    17,  -685,
     358,  -685,  -685,  5418,  -685,  -685,  -685,  -685,  -685,  5418,
     363,  4844,   396,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    7436,  5131,  -685,  5315,  -685,  5730,  2540,   367,  6223,  6223,
    -685,  -685,   374,  5418,   382,   219,    17,   189,   231,   287,
     289,  8474,  2364,  2364,   345,  -685,   345,  -685,   345,  2394,
     577,  1423,   411,   333,   274,  -685,  -685,  -685,  1069,  1548,
     345,  1365,  1365,  2364,  1365,  1365,  1101,   274,  1548,  7051,
    1101,   847,   847,   274,  9027,  9085,  9143,  9201,  9259,  9317,
    9375,  9433,  9491,  9549,  9607,  9665,  9723,  9781,  9839,   372,
     378,  -685,   345,  -685,   345,    67,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,    74,    46,  -685,   455,  6304,  5730,  6385,
    5730,  6223,  5730,  7055,   -27,  9897,  -685,  -685,  9955,  6223,
    5418,  -685,  5418,   434,   104,  -685,  -685,   354,  6223,    64,
    6223, 10759,    59,  7849,  6223, 10759,    56,  7655,  -685,   490,
   10013,  3436,  -685,    73,  -685,    39,   320,   388,    36,    70,
    -685,  5418,  -685,   259,  6223,  5833,  -685, 10759,  -685,  -685,
    -685, 10759,     0,   397,  -685,  5418,  -685,   293,    74,  -685,
    -685,  -685,  -685,   -31,  7229,  -685, 10759,  3436, 10759, 10071,
    3052,   426,  -685,  -685,  -685,  -685,  -685,  7113,   297,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,   670,  -685,    74,  6223,   505, 10134,  6223,
     506, 10197,   399,  7263,    57,    57,  -685, 10759,  -685, 10759,
    -685,  6223,    74,  -685,   405,  7786,  -685,  7912,  3820,  -685,
    3948,  -685,  7975,  4076,  -685,    57,  4204,  -685,    57,  3436,
    6223,  -685,   352,  -685,    36,   440,  -685,  -685,  -685,  -685,
      29,  -685,  -685,  4844,   403,    61, 10759,  -685, 10759,  3436,
    5418,  -685,  -685,  -685,    77,  -685,  4332,   410,  4460,   412,
    -685,  6223,  -685,  3180,   902,   320,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    -685,    74,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,   439,   416,   416,  -685,  -685,  -685, 10255,  6466,
    6547, 10318,  6628,  6709,  6790,  6871,  -685,  -685, 10759,  -685,
    -685,  -685,  3436,  6223, 10759,  6223, 10759,  3436,  6223, 10759,
    -685,  6223, 10759,  -685,  -685, 10759,  -685,   417,  5914,   201,
     473,  -685,  5730,  4588,  -685,     9,  4716,  6223,  -685, 10759,
    3436,  6223,  -685, 10759,  3436, 10759,  -685,   346,   596,   596,
    -685,   468,  -685,    28,  -685,  7113,    38,  -685,  -685,  6223,
    6223,  6223,  6223,  6223,  6223,  6223,  6223,  1024, 10013,  8038,
    8101, 10013,  8164,  8227,   440,   -34,  6223,  6223,  4925,  -685,
    -685,   311,  5418,  -685,  -685,  6223,    25,  7297,  -685,   336,
    -685,  8290,  -685,  8353,  -685,  -685,   473,  -685,   416,   416,
      86, 10381, 10444, 10507, 10570, 10633, 10696,  -685,  3436,  3436,
    3436,  3436,   201,  6017,    83,  -685,  -685, 10759, 10759,  -685,
    -685,  -685,  6871,  3436,  3436,    53,  -685,  -685,  -685,  6223,
    6223,  6223,  6223,  6223,  6223, 10013, 10013, 10013, 10013,  -685,
    -685,  -685,  -685,  -685,   377,  5730,  6952,   545, 10013, 10013,
    -685,  -685,  -685,  7402,  -685,  -685,  6120,  -685
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -685,  -685,  -685,    -1,  -589,  1809,  -685,  -685,  1170,    45,
    -685,  -685,  -685,   260,  1827,    -4,   433,  -684,  -558,   -35,
    -685,  -685,    50,  -685,  -685,  -685,  -685,  -685,   395,    95,
    -114,  -685,  -685,     2,   313,  -685,  -685,  -685,  -685,  -685,
    -685,  -685,  -157,  -155,  -582,  -685,  -150,   -21,  -274,  -685,
    -685,   -47,  -685,  -215,   128,  -685,  -201,  -184,  -685,  -141,
    -685,  -685,  -685,  -159,   245,  -685,  -310,  -584,  -685,  -441,
    -338,  -499,  -676,  -160,    10,    69,  -685,  -143,  -685,   123,
     303,  -313,  -685,  -685,   534,  -685,    -2,  -137,  -685,    40,
    -685,   824,   963,   -11,  1104,  -685,  1194,  1373,  -685,  -685,
    -685,  -685,  -685,  -685,  -685,  -261
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -383
static const yytype_int16 yytable[] =
{
     120,    72,   289,   496,   433,   139,   177,   178,   145,   477,
     691,   483,   300,   126,   158,   159,   160,   436,   314,   151,
       5,     5,   311,   169,   170,   711,   171,   172,   497,   323,
     323,   330,   186,   182,   184,   185,   189,   190,   594,     5,
     194,  -190,     5,   182,   198,   199,   121,   599,   200,   340,
     121,   201,   202,   203,   181,   204,   712,   425,   182,   214,
     216,   323,   780,     3,   197,   327,   367,   368,   369,   370,
     426,   323,   718,   427,   600,   157,   366,     5,   357,   573,
     186,   215,   568,    14,   359,    45,   428,   429,  -255,  -255,
     442,     5,  -255,   692,   147,   351,   430,   803,   353,   540,
     606,   780,  -255,  -255,  -255,   744,   318,   431,   161,   692,
     333,   143,   385,    45,   360,   805,   120,   283,   284,   285,
     194,   182,   216,   352,    43,   541,   354,   601,   593,   593,
     815,   419,   420,   121,   328,   281,   298,   288,   711,    53,
    -253,   173,  -255,   286,   174,   164,   164,   543,  -255,   690,
     298,   281,  -255,   755,  -255,  -255,   225,   386,   191,   333,
     324,  -255,   333,   298,   319,   333,  -190,   584,   592,   712,
     593,  -255,   331,   810,   582,   544,   786,   787,   333,   281,
    -255,   298,   281,   281,   333,   281,   226,   582,   800,   443,
    -255,   333,   693,   566,   333,   227,   493,   333,   211,   350,
     222,  -332,   583,   192,   220,   562,   348,   518,   696,  -366,
     228,  -366,   230,   334,   806,   788,   344,   349,   223,   371,
     372,   373,   374,   376,   378,   379,   380,   381,   382,   383,
     384,   563,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   298,   298,   422,   424,     5,   513,   433,
    -329,   149,   756,   124,   687,   294,   445,   514,   144,  -328,
     483,   448,   436,   485,   375,   377,   229,  -333,   515,   294,
     328,  -256,   182,   355,   461,   463,   233,   465,   467,     5,
     -61,   470,   294,   149,   276,    98,   742,   814,   277,   519,
     274,  -334,   275,   459,    98,    92,   487,   817,   -61,  -256,
     294,   278,   491,    98,    92,  -331,   421,   423,   298,   575,
     578,   234,   328,    92,  -256,   235,    98,   444,   121,   586,
     214,  -256,   214,  -330,   504,   506,    92,   508,   509,     5,
     280,   520,   182,   149,    98,   748,   462,   605,   748,   466,
    -256,  -366,   333,  -366,    92,  -238,   607,   609,  -259,   587,
     281,   425,    98,   511,   240,   741,    98,   242,   588,  -256,
     749,   245,    92,   749,   426,  -256,    92,   427,   328,   589,
     328,   290,   356,  -272,  -229,  -238,  -259,  -365,   328,  -365,
     428,   429,   294,   294,  -238,   503,   449,   521,   450,   522,
     430,  -259,  -272,  -229,   292,   153,  -272,   655,  -259,  -238,
     501,   431,   502,   303,   299,    98,   461,   548,   465,   551,
     470,   553,   506,   233,   316,    92,   242,  -259,   557,   559,
    -272,   298,   554,   555,   339,   240,   341,   565,   242,   567,
     343,   361,   245,   572,   363,   447,  -259,   451,   452,  -264,
     804,   490,  -259,   456,   458,    98,   471,   294,   234,   807,
     216,   472,   235,   596,   598,    92,   478,   479,  -264,   211,
     484,   211,  -264,    14,   216,   485,    17,   489,   547,    98,
     550,   595,   494,   804,    22,    23,   707,   498,   507,    92,
     510,   538,    29,   804,  -131,   604,  -264,   237,   512,   539,
      98,   240,   545,   241,   242,   579,   561,   591,   245,   218,
      92,   708,  -131,   709,    43,   252,   603,   333,   659,   662,
     664,   670,   256,   257,   258,   658,   102,    52,   661,    53,
     688,   700,   715,   704,   716,   102,   734,     5,   745,   301,
     668,   149,   564,   152,   102,   613,   727,   674,   758,   676,
     759,   686,   679,  -238,   669,   682,   760,   102,   713,   685,
     812,   799,   785,   488,   772,   560,     0,  -261,   558,     0,
     294,     0,     0,   656,    98,   102,     0,     0,    67,   182,
     304,   307,   310,  -238,    92,   699,     0,   703,     0,   233,
     705,     0,  -238,   102,   329,  -261,     0,   102,    98,     0,
     695,    98,   348,   153,   175,    98,     0,  -238,    92,   175,
    -261,    92,   338,    23,   176,    92,     0,  -261,    23,   176,
      29,   345,  -131,     0,   234,    29,     0,  -131,   235,     0,
       0,     0,     0,   -39,     0,     0,  -261,     0,   674,   676,
    -131,   679,   682,   699,   703,  -131,   102,     0,     0,    98,
       0,   728,   729,     0,   730,  -261,   731,   732,     0,    92,
     733,  -261,     0,   177,   178,     0,     0,   240,     0,   241,
     242,   747,   506,     0,   245,   506,   751,     0,     0,   425,
     753,   252,     0,     0,     0,     0,   102,     0,   256,   257,
     258,     0,   426,     0,     0,   427,     0,     0,   728,   761,
     762,   731,   763,   764,   765,   766,     0,     0,   428,   429,
     102,   446,     0,     0,     0,   190,   194,   216,   430,     0,
       0,   777,     0,     0,   778,     0,   542,     0,     0,   431,
       0,   102,     0,     0,   468,     0,     0,     0,   774,     0,
       0,    57,     0,     0,     0,     0,     0,   795,   796,   797,
     798,     0,     0,     0,     0,     0,    58,     0,    60,    61,
       0,   703,   808,   809,    98,     0,     0,     0,   795,   796,
     797,   798,   808,   809,    92,     0,     0,    65,   432,     0,
       0,     0,     0,    68,   813,   506,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   703,     0,     0,     0,   175,
      98,     0,     0,    98,     0,   102,   739,     0,    23,   176,
      92,     0,     0,    92,     0,    29,   105,  -131,     0,     0,
       0,     0,     0,   -38,     0,   105,     0,     0,     0,   102,
       0,     0,   102,     0,   105,  -131,   102,     0,   657,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   233,
     775,    98,     0,    98,     0,   105,    98,     0,     0,    98,
       0,    92,    98,    92,     0,     0,    92,     0,     0,    92,
     102,     0,    92,   105,     0,   569,   571,   105,     0,   574,
     577,   801,    98,     0,   234,     0,     0,     0,   235,    98,
       0,    98,    92,     0,     0,     0,    98,    14,     0,    92,
      17,    92,     0,     0,     0,     0,    92,     0,    22,    23,
     707,     0,     0,     0,   801,     0,    29,     0,  -131,     0,
       0,     0,     0,     0,   801,     0,   105,   240,     0,   241,
     242,     0,     0,    39,   245,   708,  -131,   709,    43,     0,
       0,   252,     0,     0,     0,   106,     0,     0,     0,     0,
     258,    52,     0,    53,   106,    98,     0,     0,     0,     0,
      98,     0,     0,   106,     0,    92,   105,   666,   667,     0,
      92,     0,     0,     0,     0,     0,   106,     0,   671,     0,
       0,     0,     0,    98,     0,   102,     0,    98,   680,     0,
     105,   683,     0,    92,   106,     0,     0,    92,     0,     0,
       0,     0,    67,     0,     0,     0,     0,     0,     0,   710,
       0,   105,   106,     0,     0,     0,   106,     0,     0,    14,
       0,   102,    17,     0,   102,     0,     0,     0,     0,     0,
      22,    23,   707,     0,     0,     0,     0,     0,    29,     0,
    -131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,    98,    98,    98,    39,     0,   708,  -131,   709,
      43,    92,    92,    92,    92,   106,    98,    98,     0,     0,
       0,   233,     0,    52,     0,    53,    92,    92,     0,     0,
       0,     0,   102,     0,   102,   105,   108,   102,     0,     0,
     102,     0,     0,   102,     0,   108,     0,     0,     0,     0,
       0,     0,     0,   233,   108,   106,   234,     0,   750,   105,
     235,     0,   105,   102,     0,     0,   105,   108,     0,     0,
     102,     0,   102,     0,    67,     0,     0,   102,     0,   106,
       0,   767,     0,     0,     0,   108,     0,     0,   234,     0,
       0,     0,   235,     0,     0,   237,     0,   238,   239,   240,
     106,   241,   242,   108,     0,     0,   245,   108,     0,     0,
     105,     0,   251,   252,     0,     0,     0,   255,     0,     0,
     256,   257,   258,     0,     0,     0,   110,   237,     0,   238,
     239,   240,     0,   241,   242,   110,   102,     0,   245,     0,
       0,   102,     0,     0,   110,   252,     0,   187,     0,     0,
       0,     0,   256,   257,   258,     0,   108,   110,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     0,   102,     0,
       0,     0,     0,     0,   106,   110,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   127,
       0,     0,     0,   110,     0,     0,   108,   110,   106,     0,
       0,   106,    21,     0,     0,   106,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
     108,    35,    36,     0,     0,   105,   282,     0,    38,     0,
      40,     0,   102,   102,   102,   102,     0,     0,     0,     0,
       0,   108,    48,    49,    50,   131,   110,   102,   102,   106,
       0,     0,     0,     0,     0,     0,     0,   187,   187,   187,
     313,   105,     0,     0,   105,     0,     0,     0,     0,     0,
       0,   187,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,     0,     0,     0,     0,   110,     0,     0,   187,
       0,    63,     0,    64,     0,     0,    66,     0,   187,     0,
       0,    68,     0,     0,   132,   111,    70,     0,   133,     0,
     110,     0,     0,     0,   111,   108,     0,   233,     0,     0,
       0,     0,   105,   111,   105,     0,     0,   105,     0,     0,
     105,   110,     0,   105,     0,     0,   111,     0,     0,   108,
       0,     0,   108,     0,     0,     0,   108,     0,     0,     0,
       0,     0,   234,   105,   111,     0,   235,     0,     0,     0,
     105,     0,   105,     0,   106,     0,     0,   105,     0,     0,
       0,     0,   111,     0,     0,   233,   111,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
     108,   237,     0,   238,   239,   240,     0,   241,   242,   243,
     106,     0,   245,   106,     0,   110,     0,     0,   251,   252,
     234,   187,     0,   255,   235,     0,   256,   257,   258,     0,
       0,     0,     0,     0,     0,   111,   105,     0,     0,   110,
       0,   105,   110,     0,     0,     0,   110,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,   239,   240,   105,   241,   242,     0,   105,     0,
     245,   106,     0,   106,     0,   111,   106,   252,     0,   106,
       0,     0,   106,     0,   256,   257,   258,     0,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,   106,     0,     0,     0,     0,     0,     0,   106,
     233,   106,     0,     0,     0,   108,   106,     0,     0,     0,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,   105,   105,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   234,     0,   105,   105,   235,
       0,   108,     0,     0,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,   187,     0,   106,   187,   187,     0,     0,
     106,     0,     0,     0,   237,     0,   238,   239,   240,     0,
     241,   242,   243,     0,   111,   245,   246,   247,     0,   249,
     250,   251,   252,   106,     0,   110,   255,   106,     0,   256,
     257,   258,   108,     0,   108,     0,     0,   108,   111,     0,
     108,   111,     0,   108,     0,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,     0,   108,   110,     0,     0,     0,     0,     0,
     108,     0,   108,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,   187,   187,     0,     0,     0,   111,
       0,   106,   106,   106,   106,   187,     0,   313,     0,     0,
       0,     0,   313,     0,     0,   187,   106,   106,   187,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,     0,   110,     0,     0,   110,     0,     0,
     110,     0,     0,   110,     0,     0,   108,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
     110,     0,   110,     0,   108,     0,     0,   110,   108,     0,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
     119,     0,     0,     0,     0,     0,     0,     0,     0,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     125,     0,   168,     0,   111,   125,     0,     0,     0,   148,
     125,   125,     0,     0,     0,     0,     0,   162,     0,   165,
     193,     0,     0,     0,     0,   187,   110,     0,     0,     0,
       0,   110,   108,   108,   108,   108,     0,     0,    74,   195,
     111,     0,   219,   111,     0,     0,     0,   108,   108,     0,
       0,     0,     0,     0,   110,     0,     0,     0,   110,   313,
     313,     0,   313,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   779,   221,     0,     0,
       0,   313,     0,   313,     0,     0,     0,     0,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,   111,     0,     0,   111,     0,     0,   111,
       0,     0,   111,     0,     0,   779,     0,     0,     0,     0,
       0,     0,   110,   110,   110,   110,     0,     0,     0,     0,
     195,   315,   111,     0,   293,     0,     0,   110,   110,   111,
     195,   111,   302,     0,     0,     0,   111,     0,   293,     0,
       0,   320,     0,     0,     0,   337,     0,     0,     0,     0,
       0,   293,     0,   125,     0,     0,   332,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,   293,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,   362,     0,     0,
     111,     0,   165,   165,   165,   165,     0,     0,     0,     0,
       0,     0,   273,  -343,     0,     0,  -365,     0,  -365,   387,
       0,     0,     0,   111,     0,     0,     0,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   293,   293,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   337,     0,     0,   337,     0,     0,
       0,     0,     0,     0,     0,     0,   453,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,   111,   111,   111,     0,   320,     0,     0,     0,
       0,     0,     0,     0,     0,   165,   111,   111,     0,     0,
       0,     0,     0,     0,     0,   505,   293,     0,    -2,     4,
       0,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -236,    18,    19,
       0,    20,    21,   165,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -131,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,  -236,    38,    39,
      40,    41,  -131,    42,    43,     0,  -236,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -236,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,   293,
     581,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     0,    71,     0,
       0,     0,   585,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   610,     0,     0,    74,
       0,     0,     0,     0,     0,   195,     0,     0,     0,     0,
       0,     0,   480,     0,     5,     6,     7,     8,     9,    10,
       0,     0,   127,     0,   652,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,   337,     0,   337,
       0,    38,   337,    40,     0,   337,   233,     0,   684,   453,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   694,     0,
       0,   320,    57,     0,     0,   698,   233,   702,     0,     0,
       0,   234,    74,     0,     0,   235,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,   236,    64,     0,    65,    66,
       0,   234,     0,     0,    68,   235,     0,   132,   714,    70,
     237,   481,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,     0,   249,   250,   251,   252,     0,
     253,   254,   255,     0,     0,   256,   257,   258,     0,     0,
     237,     0,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,     0,   249,   250,   251,   252,   752,
     253,     0,   255,   754,     0,   256,   257,   258,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   125,     0,     0,     0,     0,     0,
       0,     4,   757,     5,     6,     7,     8,     9,    10,     0,
    -382,    11,    12,    13,  -382,    14,    15,    16,    17,  -236,
      18,    19,  -382,    20,    21,  -382,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,     0,    40,    41,  -131,    42,    43,  -382,  -236,    44,
      45,  -382,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -236,     0,     0,    55,     0,    56,     0,
       0,  -382,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -382,    59,  -382,  -382,
    -382,  -382,  -382,  -382,  -382,     0,  -382,  -382,  -382,  -382,
    -382,  -382,  -382,  -382,  -382,    64,  -382,  -382,  -382,     0,
      67,  -382,  -382,  -382,     0,     0,    69,  -382,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -236,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -131,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,  -236,    38,    39,
      40,    41,  -131,    42,    43,     0,  -236,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -236,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,   179,    67,     0,
       0,    68,     0,     0,    69,   205,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -236,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -131,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,  -236,    38,    39,    40,    41,
    -131,    42,    43,     0,  -236,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -236,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,   347,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -236,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -131,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,  -236,    38,    39,    40,    41,  -131,    42,
      43,     0,  -236,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -236,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,   205,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -236,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -131,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,  -236,    38,    39,    40,    41,  -131,    42,    43,     0,
    -236,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -236,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,   612,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -236,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,    39,    40,    41,  -131,    42,    43,     0,  -236,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -236,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,   706,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -236,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -131,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,  -236,    38,   218,
      40,    41,  -131,    42,    43,     0,  -236,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -236,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -236,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -131,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,  -236,    38,     0,    40,    41,
    -131,    42,    43,     0,  -236,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -236,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -236,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,   460,     0,    28,    29,     0,
    -131,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,  -236,    38,     0,    40,    41,  -131,    42,
      43,     0,  -236,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -236,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -236,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,   464,     0,    28,    29,     0,  -131,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,  -236,    38,     0,    40,    41,  -131,    42,    43,     0,
    -236,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -236,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -236,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,   673,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,     0,    40,    41,  -131,    42,    43,     0,  -236,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -236,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -236,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,   675,
       0,    28,    29,     0,  -131,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,  -236,    38,     0,
      40,    41,  -131,    42,    43,     0,  -236,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -236,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -236,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   678,     0,    28,
      29,     0,  -131,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,  -236,    38,     0,    40,    41,
    -131,    42,    43,     0,  -236,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -236,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -236,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,   681,     0,    28,    29,     0,
    -131,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,  -236,    38,     0,    40,    41,  -131,    42,
      43,     0,  -236,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -236,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -236,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,   697,     0,    28,    29,     0,  -131,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,  -236,    38,     0,    40,    41,  -131,    42,    43,     0,
    -236,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -236,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -236,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,   701,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,     0,    40,    41,  -131,    42,    43,     0,  -236,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -236,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,    69,     0,    70,   480,
      71,     5,     6,     7,     8,     9,    10,     0,  -382,   127,
       0,     0,  -382,     0,     0,     0,     0,     0,     0,     0,
    -382,     0,    21,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,  -382,     0,     0,     0,  -382,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -254,  -382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -382,    59,  -382,  -382,  -382,     0,
    -382,  -382,  -382,     0,  -382,  -382,  -382,  -382,  -382,  -382,
    -382,  -382,  -382,    64,  -382,  -382,  -382,     0,     0,  -382,
    -382,  -382,     0,     0,   132,     0,    70,   480,   495,     5,
       6,     7,     8,     9,    10,     0,  -382,   127,     0,     0,
    -382,     0,     0,     0,     0,     0,     0,     0,  -382,     0,
      21,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,  -382,     0,     0,     0,  -382,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -382,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -382,    59,  -382,  -382,  -382,     0,  -382,  -382,
    -382,     0,  -382,  -382,  -382,  -382,  -382,  -382,  -382,  -382,
    -382,    64,  -382,  -382,  -382,     0,     0,  -382,  -382,  -382,
       0,     0,   132,     0,    70,   480,   495,     5,     6,     7,
       8,     9,    10,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   127,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,    21,
       0,     0,     0,     0,   128,   129,   130,    63,    28,    64,
       0,    65,    66,    31,    32,     0,    34,    68,    35,    36,
     132,     0,    70,     0,   495,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     5,     6,     7,     8,     9,    10,     0,    63,   127,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,   773,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,   209,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,   210,     5,     6,     7,     8,     9,    10,
       0,    63,   127,    64,     0,    65,    66,   179,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,   209,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
     500,   133,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,   155,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   156,     0,    57,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   127,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,    21,
       0,     0,     0,     0,   128,   129,   130,    63,    28,    64,
       0,    65,    66,    31,    32,     0,    34,    68,    35,    36,
     132,     0,    70,     0,   133,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,   209,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     5,     6,     7,     8,     9,    10,     0,    63,   127,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,   132,    21,    70,     0,   133,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     5,     6,     7,     8,     9,    10,
       0,    63,   127,    64,     0,    65,    66,   179,     0,     0,
       0,    68,     0,     0,   132,    21,    70,     0,   133,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,   183,     0,     0,    68,     0,     0,   132,     0,    70,
       0,   133,     5,     6,     7,     8,     9,    10,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,    21,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   127,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,    21,     0,     0,     0,     0,   128,
     129,   130,    63,    28,    64,     0,    65,    66,    31,    32,
       0,    34,    68,    35,    36,    69,     0,    70,     0,   133,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   156,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     5,     6,     7,     8,
       9,    10,     0,    63,   127,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,   597,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   127,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,   735,     0,
       0,     0,     0,   128,   129,   130,    63,    28,    64,     0,
      65,    66,    31,    32,     0,    34,    68,    35,    36,   132,
       0,    70,     0,   133,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   736,    49,
      50,   737,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       5,     6,     7,     8,     9,    10,     0,    63,   127,    64,
       0,    65,    66,   179,     0,     0,     0,    68,     0,     0,
     132,    21,    70,     0,   738,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     5,     6,     7,     8,     9,    10,     0,
      63,   127,    64,     0,    65,    66,   179,     0,     0,     0,
      68,     0,     0,   132,    21,    70,     0,   738,     0,   128,
     129,   726,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     5,     6,     7,     8,
       9,    10,     0,    63,   127,    64,     0,    65,    66,   179,
       0,     0,     0,    68,     0,     0,   132,    21,    70,     0,
     133,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   127,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,    21,     0,
       0,     0,     0,   128,   129,   546,    63,    28,    64,     0,
      65,    66,    31,    32,     0,    34,    68,    35,    36,   132,
       0,    70,     0,   133,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   127,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,    21,
       0,     0,     0,     0,   128,   129,   549,    63,    28,    64,
       0,    65,    66,    31,    32,     0,    34,    68,    35,    36,
     132,     0,    70,     0,   133,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,   127,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
      21,     0,     0,     0,     0,   128,   129,   720,    63,    28,
      64,     0,    65,    66,    31,    32,     0,    34,    68,    35,
      36,   132,     0,    70,     0,   133,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,   127,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,    21,     0,     0,     0,     0,   128,   129,   721,    63,
      28,    64,     0,    65,    66,    31,    32,     0,    34,    68,
      35,    36,   132,     0,    70,     0,   133,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   127,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,    21,     0,     0,     0,     0,   128,   129,   723,
      63,    28,    64,     0,    65,    66,    31,    32,     0,    34,
      68,    35,    36,   132,     0,    70,     0,   133,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     127,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,    21,     0,     0,     0,     0,   128,   129,
     724,    63,    28,    64,     0,    65,    66,    31,    32,     0,
      34,    68,    35,    36,   132,     0,    70,     0,   133,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     5,     6,     7,     8,     9,    10,     0,
       0,   127,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,    21,     0,     0,     0,     0,   128,
     129,   725,    63,    28,    64,     0,    65,    66,    31,    32,
       0,    34,    68,    35,    36,   132,     0,    70,     0,   133,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     5,     6,     7,     8,     9,    10,
       0,     0,   127,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,    21,     0,     0,     0,     0,
     128,   129,   726,    63,    28,    64,     0,    65,    66,    31,
      32,     0,    34,    68,    35,    36,   132,     0,    70,     0,
     133,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     5,     6,     7,     8,     9,
      10,     0,     0,   127,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,    21,     0,     0,     0,
       0,   128,   129,   130,    63,    28,    64,     0,    65,    66,
      31,    32,     0,    34,    68,    35,    36,   132,     0,    70,
       0,   133,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -263,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
       0,     0,     0,  -263,     0,     0,     0,  -263,     5,     6,
       7,     8,     9,    10,     0,     0,   127,    64,     0,     0,
       0,   179,     0,   233,     0,     0,     0,     0,   132,    21,
      70,  -263,   738,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,    38,     0,    40,   234,     0,
       0,     0,   235,     0,     0,     0,     5,     0,     0,    48,
      49,    50,   131,   616,     0,     0,     0,   617,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,    59,   241,   242,   243,     0,   244,   245,   246,
     247,     0,   249,   250,   251,   252,     0,   253,     0,   255,
      64,     0,   256,   257,   258,     0,     0,     0,     0,     0,
       0,     0,     0,    70,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   618,   619,   620,   621,   622,
     623,     0,     0,   624,   625,   626,   627,   628,   629,   630,
     631,   632,   633,     0,     0,   634,     0,     0,     0,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
       0,   646,     0,     0,   647,   648,   649,   650,   651,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   234,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   231,     0,     0,
     236,   232,     0,     0,     0,     0,     0,     0,     0,   233,
     234,     0,     0,     0,   235,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   236,   253,   254,   255,     0,     0,
     256,   257,   258,     0,   234,     0,     0,     0,   235,   237,
     608,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   236,   253,
     254,   255,     0,     0,   256,   257,   258,     0,     0,     0,
       0,     0,     0,   237,   665,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   231,   253,   254,   255,   232,     0,   256,   257,
     258,     0,     0,     0,   233,     0,     0,     0,   782,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   231,     0,     0,     0,
     232,     0,     0,     0,     0,     0,     0,     0,   233,   234,
       0,     0,     0,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,   235,   237,     0,
     238,   239,   240,     0,   241,   242,   243,     0,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   236,   253,   254,
     255,     0,     0,   256,   257,   258,     0,     0,     0,     0,
       0,     0,   237,   816,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   231,   253,   254,   255,   232,     0,   256,   257,   258,
       0,     0,     0,   233,   308,   499,     0,     0,     0,     0,
       0,     0,     0,   309,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,     0,     0,   234,   232,
       0,     0,   235,     0,     0,     0,     0,   233,   305,     0,
       0,     0,     0,     0,     0,     0,     0,   306,   121,     0,
       0,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,   234,   241,   242,   243,   235,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   253,   254,   255,
       0,     0,   256,   257,   258,   231,   236,   281,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,   576,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   253,   254,   255,     0,     0,   256,   257,   258,     0,
       0,   281,   234,     0,     0,     0,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   121,   231,     0,     0,   236,   232,     0,     0,
       0,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   253,   254,   255,     0,     0,   256,   257,   258,     0,
     234,   281,     0,     0,   235,     0,     0,     0,     0,     0,
       0,   312,     0,     0,     0,     0,   231,     0,     0,     0,
     232,     0,     0,     0,   236,     0,     0,     0,   233,   186,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,   253,
     254,   255,     0,   234,   256,   257,   258,   235,     0,   281,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,   236,     0,     0,
       0,   233,   570,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   253,   254,   255,     0,   234,   256,   257,   258,
     235,     0,   281,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   231,     0,     0,     0,   232,     0,     0,     0,
     236,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,     0,   253,   254,   255,     0,   234,
     256,   257,   258,   235,     0,   281,     0,     0,     0,     0,
     672,     0,     0,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,   236,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
     238,   239,   240,     0,   241,   242,   243,     0,   244,   245,
     246,   247,   248,   249,   250,   251,   252,     0,   253,   254,
     255,     0,   234,   256,   257,   258,   235,     0,   281,     0,
       0,     0,     0,   677,     0,     0,     0,     0,   231,     0,
       0,     0,   232,     0,     0,     0,   236,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   253,   254,   255,     0,   234,   256,   257,   258,   235,
       0,   281,     0,     0,     0,     0,   768,     0,     0,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,   236,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,   238,   239,   240,     0,
     241,   242,   243,     0,   244,   245,   246,   247,   248,   249,
     250,   251,   252,     0,   253,   254,   255,     0,   234,   256,
     257,   258,   235,     0,   281,     0,     0,     0,     0,   769,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
       0,     0,   236,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   253,   254,   255,
       0,   234,   256,   257,   258,   235,     0,   281,     0,     0,
       0,     0,   770,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,   236,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     253,   254,   255,     0,   234,   256,   257,   258,   235,     0,
     281,     0,     0,     0,     0,   771,     0,     0,     0,     0,
     231,     0,     0,     0,   232,     0,     0,     0,   236,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   253,   254,   255,     0,   234,   256,   257,
     258,   235,     0,   281,     0,     0,     0,     0,   783,     0,
       0,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,   236,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,   238,   239,
     240,     0,   241,   242,   243,     0,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,   253,   254,   255,     0,
     234,   256,   257,   258,   235,     0,   281,     0,     0,     0,
       0,   784,     0,     0,     0,     0,   231,     0,     0,     0,
     232,     0,     0,     0,   236,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,   253,
     254,   255,     0,   234,   256,   257,   258,   235,     0,   281,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,   233,   236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   234,   253,   254,   255,   235,     0,   256,   257,   258,
       0,     0,   336,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,   236,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   287,
     237,     0,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     253,   254,   255,     0,   234,   256,   257,   258,   235,  -344,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,     0,   231,     0,   236,     0,
     232,     0,     0,     0,     0,     0,     0,     0,   233,   437,
       0,     0,     0,   237,     0,   238,   239,   240,   438,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   253,   254,   255,     0,     0,   256,   257,
     258,     0,     0,   234,     0,     0,     0,   235,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,   236,     0,     0,
       0,   233,   439,     0,     0,     0,     0,     0,     0,     0,
       0,   440,   237,     0,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   253,   254,   255,     0,   234,   256,   257,   258,
     235,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,   233,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   234,   253,   254,   255,   235,     0,
     256,   257,   258,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   234,   253,   254,   255,   235,   291,   256,   257,
     258,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   236,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     234,   253,   254,   255,   235,   335,   256,   257,   258,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,   233,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   234,   253,
     254,   255,   235,   346,   256,   257,   258,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
       0,   358,   236,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   253,   254,   255,
       0,   234,   256,   257,   258,   235,     0,     0,     0,     0,
       0,     0,   441,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,   236,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     253,   254,   255,     0,   234,   256,   257,   258,   235,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   234,   253,   254,   255,   235,   523,   256,   257,
     258,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   236,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     234,   253,   254,   255,   235,   524,   256,   257,   258,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,   233,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   234,   253,
     254,   255,   235,   525,   256,   257,   258,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,   233,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   234,   253,   254,   255,
     235,   526,   256,   257,   258,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,   233,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   234,   253,   254,   255,   235,   527,
     256,   257,   258,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   234,   253,   254,   255,   235,   528,   256,   257,
     258,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   236,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     234,   253,   254,   255,   235,   529,   256,   257,   258,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,   233,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   234,   253,
     254,   255,   235,   530,   256,   257,   258,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,   233,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   234,   253,   254,   255,
     235,   531,   256,   257,   258,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,   233,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   234,   253,   254,   255,   235,   532,
     256,   257,   258,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   234,   253,   254,   255,   235,   533,   256,   257,
     258,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   236,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     234,   253,   254,   255,   235,   534,   256,   257,   258,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,   233,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   234,   253,
     254,   255,   235,   535,   256,   257,   258,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,     0,     0,     0,
       0,   233,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   234,   253,   254,   255,
     235,   536,   256,   257,   258,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,   233,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   234,   253,   254,   255,   235,   537,
     256,   257,   258,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,   121,     0,     0,   233,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   234,   253,   254,   255,   235,     0,   256,   257,
     258,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,   233,   236,     0,   580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     234,   253,   254,   255,   235,   556,   256,   257,   258,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,   233,   236,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   234,   253,
     254,   255,   235,     0,   256,   257,   258,     0,     0,     0,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
       0,   611,   236,     0,     0,     0,   233,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   253,   254,   255,
       0,   234,   256,   257,   258,   235,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,   236,     0,     0,     0,   233,
     663,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     253,   254,   255,     0,   234,   256,   257,   258,   235,     0,
       0,     0,     0,     0,     0,   231,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,   233,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   234,   253,   254,   255,   235,     0,   256,   257,
     258,     0,     0,   719,     0,     0,     0,     0,   231,     0,
       0,     0,   232,     0,     0,     0,   236,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,   238,   239,   240,     0,   241,   242,   243,
       0,   244,   245,   246,   247,   248,   249,   250,   251,   252,
       0,   253,   254,   255,     0,   234,   256,   257,   258,   235,
       0,     0,     0,     0,     0,     0,   722,     0,     0,     0,
       0,   231,     0,     0,     0,   232,     0,     0,     0,   236,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,   238,   239,   240,     0,
     241,   242,   243,     0,   244,   245,   246,   247,   248,   249,
     250,   251,   252,     0,   253,   254,   255,     0,   234,   256,
     257,   258,   235,     0,     0,     0,     0,     0,     0,   789,
       0,     0,     0,     0,   231,     0,     0,     0,   232,     0,
       0,     0,   236,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   238,
     239,   240,     0,   241,   242,   243,     0,   244,   245,   246,
     247,   248,   249,   250,   251,   252,     0,   253,   254,   255,
       0,   234,   256,   257,   258,   235,     0,     0,     0,     0,
       0,     0,   790,     0,     0,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,   236,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,   238,   239,   240,     0,   241,   242,   243,     0,
     244,   245,   246,   247,   248,   249,   250,   251,   252,     0,
     253,   254,   255,     0,   234,   256,   257,   258,   235,     0,
       0,     0,     0,     0,     0,   791,     0,     0,     0,     0,
     231,     0,     0,     0,   232,     0,     0,     0,   236,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,   238,   239,   240,     0,   241,
     242,   243,     0,   244,   245,   246,   247,   248,   249,   250,
     251,   252,     0,   253,   254,   255,     0,   234,   256,   257,
     258,   235,     0,     0,     0,     0,     0,     0,   792,     0,
       0,     0,     0,   231,     0,     0,     0,   232,     0,     0,
       0,   236,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,   238,   239,
     240,     0,   241,   242,   243,     0,   244,   245,   246,   247,
     248,   249,   250,   251,   252,     0,   253,   254,   255,     0,
     234,   256,   257,   258,   235,     0,     0,     0,     0,     0,
       0,   793,     0,     0,     0,     0,   231,     0,     0,     0,
     232,     0,     0,     0,   236,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,   238,   239,   240,     0,   241,   242,   243,     0,   244,
     245,   246,   247,   248,   249,   250,   251,   252,     0,   253,
     254,   255,     0,   234,   256,   257,   258,   235,     0,     0,
       0,     0,     0,     0,   794,     0,     0,     0,     0,   231,
       0,     0,     0,   232,     0,     0,     0,   236,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,   238,   239,   240,     0,   241,   242,
     243,     0,   244,   245,   246,   247,   248,   249,   250,   251,
     252,     0,   253,   254,   255,     0,   234,   256,   257,   258,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   238,   239,   240,
       0,   241,   242,   243,     0,   244,   245,   246,   247,   248,
     249,   250,   251,   252,     0,   253,   254,   255,     0,     0,
     256,   257,   258
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-685))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   143,   341,   278,    16,    41,    42,    19,   319,
     594,   324,   153,    15,    25,    26,    27,   278,   161,    23,
       3,     3,   159,    34,    35,   614,    37,    38,   341,     1,
       1,   174,    23,    44,    45,    46,    47,    48,   479,     3,
      51,     3,     3,    54,    55,    56,    77,    47,    59,   192,
      77,    62,    63,    64,    44,    66,   614,    19,    69,    70,
      71,     1,   746,     0,    54,    32,   225,   226,   227,   228,
      32,     1,   654,    35,    74,    25,    58,     3,   215,    23,
      23,    71,    23,    15,   101,    60,    48,    49,    60,    60,
      32,     3,    32,    32,   128,   101,    58,   773,   101,    32,
     131,   785,    32,    75,    75,   689,    70,    69,   128,    32,
     101,    69,    24,    60,   131,    32,   127,   128,   129,   130,
     131,   132,   133,   129,    56,    58,   129,   127,   100,   100,
     806,   274,   275,    77,   101,   126,   147,   139,   727,    71,
      80,     7,    82,   133,   128,   128,   128,   101,   120,   590,
     161,   126,    82,   711,   126,   126,    19,    69,    66,   101,
     100,   101,   101,   174,   128,   101,   128,   128,   478,   727,
     100,   101,   176,   120,   101,   129,   758,   759,   101,   126,
     120,   192,   126,   126,   101,   126,    49,   101,   772,   131,
     120,   101,   131,   129,   101,    58,   339,   101,    70,   210,
      36,   120,   129,   128,    42,   101,   207,   366,   131,   128,
      73,   130,   125,   120,   131,   129,   120,   127,    54,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   127,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,     3,    49,   543,
     120,     7,   713,    13,   584,   147,   287,    58,    18,   120,
     593,   292,   543,    82,   234,   235,   120,   120,    69,   161,
     101,    32,   303,    32,   305,   306,    22,   308,   309,     3,
      36,   312,   174,     7,    57,     2,   105,   806,    61,   120,
     128,   120,   130,   303,    11,     2,   327,   816,    54,    60,
     192,   128,   333,    20,    11,   120,   276,   277,   339,   466,
     467,    57,   101,    20,    75,    61,    33,   287,    77,    19,
     351,    82,   353,   120,   355,   356,    33,   358,   359,     3,
     120,   120,   363,     7,    51,   693,   306,   498,   696,   309,
     101,   128,   101,   130,    51,    19,   503,   504,    32,    49,
     126,    19,    69,   363,   100,   688,    73,   103,    58,   120,
     693,   107,    69,   696,    32,   126,    73,    35,   101,    69,
     101,   120,   131,    82,   101,    49,    60,   128,   101,   130,
      48,    49,   274,   275,    58,   355,    80,   120,    82,   120,
      58,    75,   101,   120,    76,    69,   105,   120,    82,    73,
     351,    69,   353,   128,   126,   122,   437,   438,   439,   440,
     441,   442,   443,    22,   128,   122,   103,   101,   449,   450,
     129,   452,   444,   445,   128,   100,    82,   458,   103,   460,
     120,     7,   107,   464,   100,   120,   120,   129,   101,    82,
     773,   333,   126,   120,    32,   162,    25,   339,    57,   782,
     481,   129,    61,   484,   485,   162,   101,   129,   101,   351,
      80,   353,   105,    15,   495,    82,    18,   129,   438,   186,
     440,   481,   129,   806,    26,    27,    28,   101,   131,   186,
     126,   129,    34,   816,    36,   495,   129,    96,   126,   131,
     207,   100,    57,   102,   103,    25,    82,   129,   107,    51,
     207,    53,    54,    55,    56,   114,   129,   101,    23,    23,
     131,   126,   121,   122,   123,   546,     2,    69,   549,    71,
     100,   131,   103,   131,   128,    11,   129,     3,    75,   154,
     561,     7,   457,     9,    20,   510,   670,   568,   715,   570,
     715,   582,   573,    19,   562,   576,   716,    33,   615,   580,
     785,   772,   756,   328,   734,   452,    -1,    32,   450,    -1,
     452,    -1,    -1,   543,   281,    51,    -1,    -1,   120,   600,
     157,   158,   159,    49,   281,   606,    -1,   608,    -1,    22,
     611,    -1,    58,    69,   171,    60,    -1,    73,   305,    -1,
     600,   308,   613,    69,    18,   312,    -1,    73,   305,    18,
      75,   308,   189,    27,    28,   312,    -1,    82,    27,    28,
      34,   198,    36,    -1,    57,    34,    -1,    36,    61,    -1,
      -1,    -1,    -1,    42,    -1,    -1,   101,    -1,   659,   660,
      54,   662,   663,   664,   665,    54,   122,    -1,    -1,   356,
      -1,   672,   673,    -1,   675,   120,   677,   678,    -1,   356,
     681,   126,    -1,   708,   709,    -1,    -1,   100,    -1,   102,
     103,   692,   693,    -1,   107,   696,   697,    -1,    -1,    19,
     701,   114,    -1,    -1,    -1,    -1,   162,    -1,   121,   122,
     123,    -1,    32,    -1,    -1,    35,    -1,    -1,   719,   720,
     721,   722,   723,   724,   725,   726,    -1,    -1,    48,    49,
     186,   288,    -1,    -1,    -1,   736,   737,   738,    58,    -1,
      -1,   742,    -1,    -1,   745,    -1,   433,    -1,    -1,    69,
      -1,   207,    -1,    -1,   311,    -1,    -1,    -1,   738,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,   768,   769,   770,
     771,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,
      -1,   782,   783,   784,   471,    -1,    -1,    -1,   789,   790,
     791,   792,   793,   794,   471,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,   123,   805,   806,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   816,    -1,    -1,    -1,    18,
     507,    -1,    -1,   510,    -1,   281,   688,    -1,    27,    28,
     507,    -1,    -1,   510,    -1,    34,     2,    36,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    11,    -1,    -1,    -1,   305,
      -1,    -1,   308,    -1,    20,    54,   312,    -1,   545,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
     742,   568,    -1,   570,    -1,    51,   573,    -1,    -1,   576,
      -1,   568,   579,   570,    -1,    -1,   573,    -1,    -1,   576,
     356,    -1,   579,    69,    -1,   462,   463,    73,    -1,   466,
     467,   773,   599,    -1,    57,    -1,    -1,    -1,    61,   606,
      -1,   608,   599,    -1,    -1,    -1,   613,    15,    -1,   606,
      18,   608,    -1,    -1,    -1,    -1,   613,    -1,    26,    27,
      28,    -1,    -1,    -1,   806,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    -1,   816,    -1,   122,   100,    -1,   102,
     103,    -1,    -1,    51,   107,    53,    54,    55,    56,    -1,
      -1,   114,    -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,
     123,    69,    -1,    71,    11,   672,    -1,    -1,    -1,    -1,
     677,    -1,    -1,    20,    -1,   672,   162,   554,   555,    -1,
     677,    -1,    -1,    -1,    -1,    -1,    33,    -1,   565,    -1,
      -1,    -1,    -1,   700,    -1,   471,    -1,   704,   575,    -1,
     186,   578,    -1,   700,    51,    -1,    -1,   704,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,   127,
      -1,   207,    69,    -1,    -1,    -1,    73,    -1,    -1,    15,
      -1,   507,    18,    -1,   510,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   768,   769,   770,   771,    51,    -1,    53,    54,    55,
      56,   768,   769,   770,   771,   122,   783,   784,    -1,    -1,
      -1,    22,    -1,    69,    -1,    71,   783,   784,    -1,    -1,
      -1,    -1,   568,    -1,   570,   281,     2,   573,    -1,    -1,
     576,    -1,    -1,   579,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    20,   162,    57,    -1,   695,   305,
      61,    -1,   308,   599,    -1,    -1,   312,    33,    -1,    -1,
     606,    -1,   608,    -1,   120,    -1,    -1,   613,    -1,   186,
      -1,   127,    -1,    -1,    -1,    51,    -1,    -1,    57,    -1,
      -1,    -1,    61,    -1,    -1,    96,    -1,    98,    99,   100,
     207,   102,   103,    69,    -1,    -1,   107,    73,    -1,    -1,
     356,    -1,   113,   114,    -1,    -1,    -1,   118,    -1,    -1,
     121,   122,   123,    -1,    -1,    -1,     2,    96,    -1,    98,
      99,   100,    -1,   102,   103,    11,   672,    -1,   107,    -1,
      -1,   677,    -1,    -1,    20,   114,    -1,    47,    -1,    -1,
      -1,    -1,   121,   122,   123,    -1,   122,    33,    -1,    -1,
      -1,    -1,    -1,    -1,   700,    -1,    -1,    -1,   704,    -1,
      -1,    -1,    -1,    -1,   281,    51,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    69,    -1,    -1,   162,    73,   305,    -1,
      -1,   308,    24,    -1,    -1,   312,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
     186,    43,    44,    -1,    -1,   471,   126,    -1,    50,    -1,
      52,    -1,   768,   769,   770,   771,    -1,    -1,    -1,    -1,
      -1,   207,    64,    65,    66,    67,   122,   783,   784,   356,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   159,
     160,   507,    -1,    -1,   510,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,   189,
      -1,   113,    -1,   115,    -1,    -1,   118,    -1,   198,    -1,
      -1,   123,    -1,    -1,   126,     2,   128,    -1,   130,    -1,
     186,    -1,    -1,    -1,    11,   281,    -1,    22,    -1,    -1,
      -1,    -1,   568,    20,   570,    -1,    -1,   573,    -1,    -1,
     576,   207,    -1,   579,    -1,    -1,    33,    -1,    -1,   305,
      -1,    -1,   308,    -1,    -1,    -1,   312,    -1,    -1,    -1,
      -1,    -1,    57,   599,    51,    -1,    61,    -1,    -1,    -1,
     606,    -1,   608,    -1,   471,    -1,    -1,   613,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    22,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
     356,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
     507,    -1,   107,   510,    -1,   281,    -1,    -1,   113,   114,
      57,   311,    -1,   118,    61,    -1,   121,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   672,    -1,    -1,   305,
      -1,   677,   308,    -1,    -1,    -1,   312,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    99,   100,   700,   102,   103,    -1,   704,    -1,
     107,   568,    -1,   570,    -1,   162,   573,   114,    -1,   576,
      -1,    -1,   579,    -1,   121,   122,   123,    -1,    -1,    -1,
     356,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,   599,    -1,    -1,    -1,    -1,    -1,    -1,   606,
      22,   608,    -1,    -1,    -1,   471,   613,    -1,    -1,    -1,
     207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   768,   769,   770,   771,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,   783,   784,    61,
      -1,   507,    -1,    -1,   510,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   462,   463,    -1,   672,   466,   467,    -1,    -1,
     677,    -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,
     102,   103,   104,    -1,   281,   107,   108,   109,    -1,   111,
     112,   113,   114,   700,    -1,   471,   118,   704,    -1,   121,
     122,   123,   568,    -1,   570,    -1,    -1,   573,   305,    -1,
     576,   308,    -1,   579,    -1,   312,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   507,    -1,   599,   510,    -1,    -1,    -1,    -1,    -1,
     606,    -1,   608,    -1,    -1,    -1,    -1,   613,    -1,    -1,
      -1,    -1,    -1,    -1,   554,   555,    -1,    -1,    -1,   356,
      -1,   768,   769,   770,   771,   565,    -1,   567,    -1,    -1,
      -1,    -1,   572,    -1,    -1,   575,   783,   784,   578,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   568,    -1,   570,    -1,    -1,   573,    -1,    -1,
     576,    -1,    -1,   579,    -1,    -1,   672,    -1,    -1,    -1,
      -1,   677,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   599,    -1,    -1,    -1,    -1,    -1,    -1,
     606,    -1,   608,    -1,   700,    -1,    -1,   613,   704,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    33,    -1,   471,    18,    -1,    -1,    -1,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    32,
      51,    -1,    -1,    -1,    -1,   695,   672,    -1,    -1,    -1,
      -1,   677,   768,   769,   770,   771,    -1,    -1,    69,    52,
     507,    -1,    73,   510,    -1,    -1,    -1,   783,   784,    -1,
      -1,    -1,    -1,    -1,   700,    -1,    -1,    -1,   704,   729,
     730,    -1,   732,   733,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   746,    90,    -1,    -1,
      -1,   751,    -1,   753,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   568,    -1,   570,    -1,    -1,   573,    -1,    -1,   576,
      -1,    -1,   579,    -1,    -1,   785,    -1,    -1,    -1,    -1,
      -1,    -1,   768,   769,   770,   771,    -1,    -1,    -1,    -1,
     143,   162,   599,    -1,   147,    -1,    -1,   783,   784,   606,
     153,   608,   155,    -1,    -1,    -1,   613,    -1,   161,    -1,
      -1,   164,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,   174,    -1,   176,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,   192,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   672,    -1,   220,    -1,    -1,
     677,    -1,   225,   226,   227,   228,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,    -1,    -1,   128,    -1,   130,   242,
      -1,    -1,    -1,   700,    -1,    -1,    -1,   704,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     281,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   274,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,   308,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   299,    -1,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   768,   769,   770,   771,    -1,   319,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   328,   783,   784,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   356,   339,    -1,     0,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,   366,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    -1,    71,
      72,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,   452,
     471,   113,    -1,   115,    -1,   117,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,   126,    -1,   128,    -1,   130,    -1,
      -1,    -1,   475,    -1,    -1,   478,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   507,    -1,    -1,   510,
      -1,    -1,    -1,    -1,    -1,   498,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,   517,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,   568,    -1,   570,
      -1,    50,   573,    52,    -1,   576,    22,    -1,   579,   562,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   599,    -1,
      -1,   584,    81,    -1,    -1,   606,    22,   608,    -1,    -1,
      -1,    57,   613,    -1,    -1,    61,    -1,    96,    97,    98,
      99,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    81,   115,    -1,   117,   118,
      -1,    57,    -1,    -1,   123,    61,    -1,   126,   631,   128,
      96,   130,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,    -1,
     116,   117,   118,    -1,    -1,   121,   122,   123,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   700,
     116,    -1,   118,   704,    -1,   121,   122,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   707,    -1,    -1,    -1,    -1,    -1,
      -1,     1,   715,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    69,
      -1,    71,    72,    73,    -1,    -1,    76,    -1,    78,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
     120,   121,   122,   123,    -1,    -1,   126,   127,   128,     1,
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
      -1,   113,    -1,   115,    -1,   117,   118,   119,   120,    -1,
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
     130,     3,     4,     5,     6,     7,     8,    -1,    10,    11,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    61,
      -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,    -1,
     102,   103,   104,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,    -1,   121,
     122,   123,    -1,    -1,   126,    -1,   128,     1,   130,     3,
       4,     5,     6,     7,     8,    -1,    10,    11,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    -1,    -1,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
      -1,    -1,   126,    -1,   128,     1,   130,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,
      -1,     3,     4,     5,     6,     7,     8,    -1,   113,    11,
     115,    -1,   117,   118,   119,    -1,    -1,    -1,   123,    -1,
      -1,   126,    24,   128,    -1,   130,   131,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,   105,     3,     4,     5,     6,     7,     8,
      -1,   113,    11,   115,    -1,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,    -1,   126,    24,   128,    -1,   130,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,    -1,    -1,    -1,    -1,   104,    -1,     3,     4,     5,
       6,     7,     8,    -1,   113,    11,   115,    -1,   117,   118,
     119,    -1,    -1,    -1,   123,    -1,    -1,   126,    24,   128,
     129,   130,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    49,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    -1,    81,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,
      96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,   113,    33,   115,
      -1,   117,   118,    38,    39,    -1,    41,   123,    43,    44,
     126,    -1,   128,    -1,   130,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,    -1,    -1,    -1,    -1,   104,
      -1,     3,     4,     5,     6,     7,     8,    -1,   113,    11,
     115,    -1,   117,   118,   119,    -1,    -1,    -1,   123,    -1,
      -1,   126,    24,   128,    -1,   130,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,    -1,    -1,
      -1,    -1,   104,    -1,     3,     4,     5,     6,     7,     8,
      -1,   113,    11,   115,    -1,   117,   118,   119,    -1,    -1,
      -1,   123,    -1,    -1,   126,    24,   128,    -1,   130,    -1,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,   117,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,   126,    -1,   128,
      -1,   130,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,   104,    -1,     3,     4,     5,     6,
       7,     8,    -1,   113,    11,   115,    -1,   117,   118,    -1,
      -1,    -1,    -1,   123,    -1,    -1,   126,    24,   128,    -1,
     130,    -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    45,    -1,
      -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   104,    -1,     3,     4,     5,     6,
       7,     8,    -1,   113,    11,   115,    -1,   117,   118,   119,
      -1,    -1,    -1,   123,    -1,    -1,   126,    24,   128,    -1,
     130,    -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
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
      -1,    -1,    81,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    -1,    -1,    -1,    96,    97,    98,
      99,    -1,    -1,    -1,    -1,   104,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,   113,    33,   115,    -1,   117,   118,
      38,    39,    -1,    41,   123,    43,    44,   126,    -1,   128,
      -1,   130,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,   101,    -1,    -1,    -1,   105,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,   115,    -1,    -1,
      -1,   119,    -1,    22,    -1,    -1,    -1,    -1,   126,    24,
     128,   129,   130,    -1,    29,    30,    31,    -1,    33,    -1,
      -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    57,    -1,
      -1,    -1,    61,    -1,    -1,    -1,     3,    -1,    -1,    64,
      65,    66,    67,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    97,   102,   103,   104,    -1,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,    -1,   116,    -1,   118,
     115,    -1,   121,   122,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    -1,    -1,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,    -1,    -1,   102,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,   118,    -1,    -1,   121,   122,   123,   124,   125,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    57,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    10,    -1,    -1,
      81,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      57,    -1,    -1,    -1,    61,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    81,   116,   117,   118,    -1,    -1,
     121,   122,   123,    -1,    57,    -1,    -1,    -1,    61,    96,
     131,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    81,   116,
     117,   118,    -1,    -1,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    96,   131,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    10,   116,   117,   118,    14,    -1,   121,   122,
     123,    -1,    -1,    -1,    22,    -1,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    57,
      -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    61,    96,    -1,
      98,    99,   100,    -1,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    81,   116,   117,
     118,    -1,    -1,   121,   122,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    96,   131,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    10,   116,   117,   118,    14,    -1,   121,   122,   123,
      -1,    -1,    -1,    22,    23,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    57,    14,
      -1,    -1,    61,    -1,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    77,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    57,   102,   103,   104,    61,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    -1,   121,   122,   123,    10,    81,   126,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,   116,   117,   118,    -1,    -1,   121,   122,   123,    -1,
      -1,   126,    57,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    10,    -1,    -1,    81,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,   116,   117,   118,    -1,    -1,   121,   122,   123,    -1,
      57,   126,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,    -1,    57,   121,   122,   123,    61,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,    -1,    57,   121,   122,   123,
      61,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    68,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,   116,
     117,   118,    -1,    57,   121,   122,   123,    61,    -1,   126,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    57,   116,   117,   118,    61,    -1,   121,   122,   123,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,    -1,    57,   121,   122,   123,    61,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    10,    -1,    81,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    32,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,   116,   117,   118,    -1,    -1,   121,   122,
     123,    -1,    -1,    57,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,    -1,    57,   121,   122,   123,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    57,   116,   117,   118,    61,    -1,
     121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    57,   116,   117,   118,    61,   120,   121,   122,
     123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      57,   116,   117,   118,    61,   120,   121,   122,   123,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    57,   116,
     117,   118,    61,   120,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    80,    81,    -1,    -1,    -1,    22,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    57,   116,   117,   118,    61,   120,   121,   122,
     123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      57,   116,   117,   118,    61,   120,   121,   122,   123,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    57,   116,
     117,   118,    61,   120,   121,   122,   123,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    57,   116,   117,   118,
      61,   120,   121,   122,   123,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    57,   116,   117,   118,    61,   120,
     121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    57,   116,   117,   118,    61,   120,   121,   122,
     123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      57,   116,   117,   118,    61,   120,   121,   122,   123,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    57,   116,
     117,   118,    61,   120,   121,   122,   123,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    57,   116,   117,   118,
      61,   120,   121,   122,   123,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    57,   116,   117,   118,    61,   120,
     121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    57,   116,   117,   118,    61,   120,   121,   122,
     123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      57,   116,   117,   118,    61,   120,   121,   122,   123,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    57,   116,
     117,   118,    61,   120,   121,   122,   123,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    57,   116,   117,   118,
      61,   120,   121,   122,   123,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    57,   116,   117,   118,    61,   120,
     121,   122,   123,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    22,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    57,   116,   117,   118,    61,    -1,   121,   122,
     123,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    81,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    99,   100,    -1,   102,   103,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      57,   116,   117,   118,    61,   120,   121,   122,   123,    -1,
      -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    98,    99,   100,    -1,   102,   103,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    57,   116,
     117,   118,    61,    -1,   121,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      -1,    80,    81,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,
      99,   100,    -1,   102,   103,   104,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
      -1,    57,   121,   122,   123,    61,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    81,    -1,    -1,    -1,    22,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    98,    99,   100,    -1,   102,   103,   104,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
     116,   117,   118,    -1,    57,   121,   122,   123,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    57,   116,   117,   118,    61,    -1,   121,   122,
     123,    -1,    -1,    68,    -1,    -1,    -1,    -1,    10,    -1,
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
      -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    99,   100,    -1,   102,   103,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,   116,   117,   118,    -1,    57,   121,   122,   123,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    99,   100,
      -1,   102,   103,   104,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,   116,   117,   118,    -1,    -1,
     121,   122,   123
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
     163,   166,   169,   172,   173,   193,   196,   197,   215,   216,
     217,   218,   219,   220,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   236,   237,   238,   239,   240,   140,
     228,    77,   221,   222,   148,   149,   221,    11,    29,    30,
      31,    67,   126,   130,   169,   215,   219,   226,   227,   228,
     229,   231,   232,    69,   148,   228,   140,   128,   149,     7,
     148,   150,     9,    69,   150,    49,    79,   157,   228,   228,
     228,   128,   149,   170,   128,   149,   198,   199,   140,   228,
     228,   228,   228,     7,   128,    18,    28,   154,   154,   119,
     189,   209,   228,   120,   228,   228,    23,   143,   151,   228,
     228,    66,   128,   140,   228,   149,   194,   209,   228,   228,
     228,   228,   228,   228,   228,   127,   138,   144,   209,    70,
     105,   189,   210,   211,   228,   209,   228,   235,    51,   140,
      42,   149,    36,    54,   185,    19,    49,    58,    73,   120,
     125,    10,    14,    22,    57,    61,    81,    96,    98,    99,
     100,   102,   103,   104,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   116,   117,   118,   121,   122,   123,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   124,   128,   130,    57,    61,   128,   140,
     120,   126,   143,   228,   228,   228,   209,    32,   221,   194,
     120,   120,    76,   149,   189,   212,   213,   214,   228,   126,
     194,   163,   149,   128,   151,    23,    32,   151,    23,    32,
     151,   222,    68,   143,   212,   140,   128,   180,    70,   128,
     149,   200,   201,     1,   100,   203,   204,    32,   101,   151,
     212,   150,   149,   101,   120,   120,   126,   140,   151,   128,
     212,    82,   195,   120,   120,   151,   120,   127,   138,   127,
     228,   101,   129,   101,   129,    32,   131,   222,    80,   101,
     131,     7,   149,   100,   164,   174,    58,   198,   198,   198,
     198,   228,   228,   228,   228,   157,   228,   157,   228,   228,
     228,   228,   228,   228,   228,    24,    69,   149,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   212,
     212,   157,   228,   157,   228,    19,    32,    35,    48,    49,
      58,    69,   118,   183,   223,   224,   240,    23,    32,    23,
      32,    68,    32,   131,   157,   228,   151,   120,   228,    80,
      82,   129,   101,   149,   167,   168,   120,   149,    32,   209,
      31,   228,   157,   228,    31,   228,   157,   228,   151,   140,
     228,    25,   129,   181,   182,   183,   171,   201,   101,   129,
       1,   130,   205,   216,    80,    82,   202,   228,   199,   129,
     189,   228,   160,   212,   129,   130,   205,   216,   101,   129,
     129,   210,   210,   157,   228,   140,   228,   131,   228,   228,
     126,   209,   126,    49,    58,    69,   176,   184,   198,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   129,   131,
      32,    58,   215,   101,   129,    57,    31,   157,   228,    31,
     157,   228,   157,   228,   221,   221,   120,   228,   189,   228,
     214,    82,   101,   127,   164,   228,   129,   228,    23,   151,
      23,   151,   228,    23,   151,   222,    23,   151,   222,    25,
      25,   140,   101,   129,   128,   149,    19,    49,    58,    69,
     186,   129,   201,   100,   204,   209,   228,    45,   228,    47,
      74,   127,   161,   129,   209,   194,   131,   222,   131,   222,
     140,    80,   127,   144,   165,   175,    10,    14,    82,    83,
      84,    85,    86,    87,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   102,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   118,   121,   122,   123,
     124,   125,   149,   177,   178,   120,   224,   215,   228,    23,
      23,   228,    23,    23,   131,   131,   151,   151,   228,   168,
     126,   151,    68,    31,   228,    31,   228,    68,    31,   228,
     151,    31,   228,   151,   140,   228,   182,   201,   100,   208,
     204,   202,    32,   131,   140,   209,   131,    31,   140,   228,
     131,    31,   140,   228,   131,   228,   127,    28,    53,    55,
     127,   139,   153,   186,   149,   103,   128,   179,   179,    68,
      31,    31,    68,    31,    31,    31,    31,   165,   228,   228,
     228,   228,   228,   228,   129,    24,    64,    67,   130,   189,
     207,   216,   105,   191,   202,    75,   192,   228,   205,   216,
     151,   228,   140,   228,   140,   153,   204,   149,   177,   178,
     181,   228,   228,   228,   228,   228,   228,   127,    68,    68,
      68,    68,   208,   131,   209,   189,   190,   228,   228,   143,
     152,   188,   131,    68,    68,   192,   179,   179,   129,    68,
      68,    68,    68,    68,    68,   228,   228,   228,   228,   191,
     202,   189,   206,   207,   216,    32,   131,   216,   228,   228,
     120,   187,   188,   228,   206,   207,   131,   206
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

  case 20:

/* Line 1806 of yacc.c  */
#line 438 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 451 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 452 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 518 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 545 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 79:

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

  case 80:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 601 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 607 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 613 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
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
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1492 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1498 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7488 "bison-chapel.cpp"
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



