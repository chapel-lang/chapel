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
#define YYLAST   11054

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  429
/* YYNRULES -- Number of states.  */
#define YYNSTATES  821

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
    1296,  1298,  1302,  1306,  1310,  1314,  1320,  1324,  1328,  1332,
    1336,  1340,  1344,  1348,  1352,  1356,  1360,  1364,  1368,  1372,
    1376,  1380,  1384,  1388,  1392,  1396,  1400,  1404,  1408,  1412,
    1415,  1418,  1421,  1424,  1427,  1430,  1434,  1438,  1442,  1446,
    1450,  1454,  1458,  1462,  1464,  1466,  1468,  1470,  1472,  1474
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     135,     0,    -1,   136,    -1,    -1,   136,   137,    -1,   139,
      -1,   138,   139,    -1,    51,     7,    -1,   138,    51,     7,
      -1,   140,    -1,   142,    -1,   144,    -1,   151,    -1,   145,
      -1,   153,    -1,   156,    -1,   154,    -1,   157,    -1,   160,
      -1,   150,    -1,   219,   119,    -1,    11,   139,    -1,    12,
     220,   139,    -1,    13,   146,   119,    -1,    16,   220,   142,
      -1,    20,   146,   119,    -1,    21,   227,   119,    -1,    37,
     147,   139,    -1,    40,   139,    -1,    46,   227,   149,    -1,
      62,   227,   149,    -1,    62,   149,    -1,    66,   139,    -1,
      77,   227,   119,    -1,     1,    -1,   141,    42,   147,   125,
     126,    -1,   141,    42,   147,   125,   143,   126,    -1,    -1,
      55,    -1,    53,    -1,   125,   126,    -1,   125,   143,   126,
      -1,   137,    -1,   143,   137,    -1,    71,   208,   119,    -1,
     228,    81,   227,   119,    -1,   228,    92,   227,   119,    -1,
     228,    89,   227,   119,    -1,   228,    91,   227,   119,    -1,
     228,    85,   227,   119,    -1,   228,    90,   227,   119,    -1,
     228,    86,   227,   119,    -1,   228,    82,   227,   119,    -1,
     228,    83,   227,   119,    -1,   228,    84,   227,   119,    -1,
     228,    94,   227,   119,    -1,   228,    93,   227,   119,    -1,
     228,   123,   227,   119,    -1,   228,    87,   227,   119,    -1,
     228,    88,   227,   119,    -1,    -1,   147,    -1,     3,    -1,
     146,    -1,     7,    -1,    23,   139,    -1,   142,    -1,    59,
     119,    -1,    59,   227,   119,    -1,   119,    -1,   152,    -1,
      55,   152,    -1,    53,   152,    -1,   165,    -1,   192,    -1,
     161,    -1,   172,    -1,   195,    -1,    28,     9,    -1,    23,
     139,    75,   227,   119,    -1,    75,   227,   149,    -1,    17,
     227,    32,   227,   220,   149,    -1,    17,   227,    32,   155,
     220,   149,    -1,    17,   227,   220,   149,    -1,    29,   227,
      32,   227,   149,    -1,    29,   227,    32,   155,   149,    -1,
      29,   227,   149,    -1,    29,   155,   149,    -1,    29,    49,
     147,    32,   227,   149,    -1,    30,   227,    32,   227,   149,
      -1,    30,   227,    32,   227,   221,   149,    -1,    30,   227,
      32,   155,   149,    -1,    30,   227,    32,   155,   221,   149,
      -1,    30,   227,   149,    -1,    30,   227,   221,   149,    -1,
     129,   208,    32,   227,   130,   139,    -1,   129,   208,    32,
     227,   221,   130,   139,    -1,   129,   208,    32,   155,   130,
     139,    -1,   129,   208,    32,   155,   221,   130,   139,    -1,
     129,   208,   130,   139,    -1,   129,   208,   221,   130,   139,
      -1,    78,   127,   208,   128,    -1,    31,   227,    67,   139,
      -1,    31,   227,   142,    -1,    31,   227,    67,   139,    25,
     139,    -1,    31,   227,   142,    25,   139,    -1,    61,   227,
     125,   158,   126,    -1,    -1,   158,   159,    -1,    73,   208,
     149,    -1,    47,   139,    -1,    68,    61,   208,   125,   158,
     126,    -1,   162,   147,   163,   125,   164,   126,    -1,    28,
     148,   162,   147,   163,   125,   164,   126,    -1,    15,    -1,
      56,    -1,    70,    -1,    -1,    99,   208,    -1,    -1,   164,
     151,    -1,   164,   138,   151,    -1,    26,   147,   125,   166,
     126,    -1,   167,    -1,   166,   100,    -1,   166,   100,   167,
      -1,   147,    -1,   147,    81,   227,    -1,    -1,    -1,    38,
     169,   179,   170,   185,   203,   191,   187,    -1,    -1,    34,
      -1,    27,   148,    -1,    28,   148,    -1,    -1,    -1,   171,
     184,   173,   175,   174,   185,   203,   191,   186,    -1,   183,
     176,   178,    -1,   183,   177,   178,    -1,   183,   147,   102,
     176,   178,    -1,   183,   147,   102,   177,   178,    -1,   147,
      -1,    96,   147,    -1,    95,    -1,    97,    -1,    98,    -1,
      96,    -1,   105,    -1,   115,    -1,   111,    -1,   108,    -1,
     110,    -1,   107,    -1,   117,    -1,   112,    -1,   122,    -1,
     101,    -1,   120,    -1,   121,    -1,   113,    -1,   106,    -1,
     114,    -1,    14,    -1,   109,    -1,    10,    -1,   123,    -1,
     124,    -1,    81,    -1,    92,    -1,    89,    -1,    91,    -1,
      85,    -1,    90,    -1,    86,    -1,    82,    -1,    83,    -1,
      84,    -1,    94,    -1,    93,    -1,    -1,   127,   180,   128,
      -1,   127,   180,   128,    -1,    -1,   181,    -1,   180,   100,
     181,    -1,   182,   147,   207,   201,    -1,   182,   147,   207,
     190,    -1,   182,   127,   200,   128,   207,   201,    -1,   182,
     127,   200,   128,   207,   190,    -1,    -1,    32,    -1,    35,
      -1,    48,    -1,    19,    -1,    19,    32,    -1,    19,    58,
      -1,    49,    -1,    58,    -1,    68,    -1,    -1,    49,    -1,
      58,    -1,    68,    -1,    54,    -1,    36,    -1,    -1,    19,
      -1,    58,    -1,    49,    -1,    68,    -1,   119,    -1,   187,
      -1,   142,    -1,   150,    -1,   118,   147,    -1,   118,    -1,
      -1,   188,    -1,   104,   227,    -1,   104,   189,    -1,    -1,
      74,   227,    -1,    68,   193,   119,    -1,    18,    68,   193,
     119,    -1,    28,    68,   193,   119,    -1,   147,   194,    -1,
     147,   194,   100,   193,    -1,    -1,    81,   215,    -1,    81,
     204,    -1,   196,    49,   197,   119,    -1,   196,    19,   197,
     119,    -1,   196,    58,   197,   119,    -1,   196,    19,    58,
     197,   119,    -1,   196,    72,   197,   119,    -1,    -1,    18,
      -1,    28,    -1,   198,    -1,   197,   100,   198,    -1,   147,
     203,   201,    -1,   147,   202,    79,   227,    -1,   127,   200,
     128,   203,   201,    -1,    69,    -1,   147,    -1,   127,   200,
     128,    -1,   199,    -1,   199,   100,    -1,   199,   100,   200,
      -1,    -1,    81,    45,    -1,    81,   227,    -1,    -1,    99,
     129,   208,   130,    -1,    -1,    99,   215,    -1,    99,   204,
      -1,     1,    -1,   129,   208,   130,   215,    -1,   129,   208,
     130,   204,    -1,   129,   208,    32,   227,   130,   215,    -1,
       1,    -1,    -1,   215,    -1,   188,    -1,   129,   130,   205,
      -1,   129,   208,   130,   205,    -1,   129,   130,   206,    -1,
     129,   208,   130,   206,    -1,   129,   208,    32,   227,   130,
     205,    -1,    -1,    99,   215,    -1,    99,   188,    -1,    99,
      24,    -1,    99,    63,    -1,    99,    66,    -1,    99,   206,
      -1,   227,    -1,   188,    -1,   208,   100,   227,    -1,   208,
     100,   188,    -1,    69,    -1,   227,    -1,   188,    -1,   209,
     100,   209,    -1,   210,   100,   209,    -1,    -1,   212,    -1,
     213,    -1,   212,   100,   213,    -1,   147,    81,   188,    -1,
     147,    81,   227,    -1,   147,    79,   227,    -1,   188,    -1,
     227,    -1,   147,    -1,   218,    -1,   228,    -1,   217,    -1,
     236,    -1,   235,    -1,    63,   227,    -1,    33,   127,   211,
     128,    -1,    24,   127,   211,   128,    -1,    65,   127,   211,
     128,    -1,    64,    65,   127,   211,   128,    -1,    11,   227,
      -1,    66,   227,    -1,    29,   227,    32,   227,    23,   227,
      -1,    29,   227,    32,   155,    23,   227,    -1,    29,   227,
      23,   227,    -1,    29,   227,    32,   227,    23,    31,   227,
      67,   227,    -1,    29,   227,    32,   155,    23,    31,   227,
      67,   227,    -1,    29,   227,    23,    31,   227,    67,   227,
      -1,    30,   227,    32,   227,    23,   227,    -1,    30,   227,
      32,   155,    23,   227,    -1,    30,   227,    23,   227,    -1,
      30,   227,    32,   227,    23,    31,   227,    67,   227,    -1,
      30,   227,    32,   155,    23,    31,   227,    67,   227,    -1,
      30,   227,    23,    31,   227,    67,   227,    -1,   129,   208,
     130,   227,    -1,   129,   208,    32,   227,   130,   227,    -1,
     129,   208,    32,   155,   130,   227,    -1,   129,   208,    32,
     227,   130,    31,   227,    67,   227,    -1,   129,   208,    32,
     155,   130,    31,   227,    67,   227,    -1,    31,   227,    67,
     227,    25,   227,    -1,    44,    -1,   218,    -1,   214,    -1,
     231,    -1,   230,    -1,   168,    -1,   225,    -1,   226,    -1,
     224,   124,   227,    -1,    -1,   221,    -1,    76,   127,   222,
     128,    -1,   223,    -1,   222,   100,   223,    -1,   182,   214,
      -1,   239,    57,   214,    -1,   228,    -1,   224,   124,   227,
      -1,    43,   227,    -1,    39,   197,    32,   227,    -1,   233,
      -1,   215,    -1,   216,    -1,   237,    -1,   238,    -1,   168,
      -1,   225,    -1,   226,    -1,   127,   104,   227,   128,    -1,
     227,    99,   227,    -1,   227,   103,   227,    -1,   227,   103,
      -1,   103,   227,    -1,   103,    -1,   214,    -1,   230,    -1,
     231,    -1,   232,    -1,   228,    -1,   168,    -1,   229,   127,
     211,   128,    -1,   229,   129,   211,   130,    -1,    52,   127,
     211,   128,    -1,   227,   102,   147,    -1,   227,   102,    68,
      -1,   227,   102,    24,    -1,   127,   209,   128,    -1,   127,
     209,   100,   128,    -1,   127,   210,   128,    -1,     4,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,   125,   208,
     126,    -1,   129,   208,   130,    -1,   129,   234,   130,    -1,
     227,    79,   227,    -1,   234,   100,   227,    79,   227,    -1,
     227,   117,   227,    -1,   227,   112,   227,    -1,   227,   122,
     227,    -1,   227,   101,   227,    -1,   227,   120,   227,    -1,
     227,   121,   227,    -1,   227,   113,   227,    -1,   227,   105,
     227,    -1,   227,   115,   227,    -1,   227,   111,   227,    -1,
     227,   108,   227,    -1,   227,   110,   227,    -1,   227,   107,
     227,    -1,   227,    95,   227,    -1,   227,    97,   227,    -1,
     227,    98,   227,    -1,   227,    80,   227,    -1,   227,   116,
     227,    -1,   227,   106,   227,    -1,   227,    14,   227,    -1,
     227,    10,   227,    -1,   227,   109,   227,    -1,   227,    22,
     227,    -1,   117,   227,    -1,   112,   227,    -1,    41,   227,
      -1,    50,   227,    -1,   114,   227,    -1,    96,   227,    -1,
     227,    57,   227,    -1,   227,    57,   155,    -1,   239,    57,
     227,    -1,   239,    57,   155,    -1,   227,    60,   227,    -1,
     227,    60,   155,    -1,   239,    60,   227,    -1,   239,    60,
     155,    -1,   117,    -1,   122,    -1,    80,    -1,   116,    -1,
      95,    -1,    97,    -1,    98,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   403,   403,   408,   409,   415,   416,   421,   422,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   456,   458,   463,   464,   465,
     480,   481,   486,   487,   492,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     514,   515,   519,   523,   524,   528,   529,   533,   534,   538,
     539,   540,   541,   542,   543,   544,   548,   549,   553,   568,
     569,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   590,   596,   602,   608,
     615,   625,   629,   630,   631,   632,   636,   641,   642,   646,
     648,   653,   660,   665,   673,   678,   683,   691,   692,   697,
     698,   700,   705,   715,   724,   728,   736,   737,   742,   747,
     741,   772,   778,   785,   793,   804,   810,   803,   838,   842,
     847,   851,   859,   860,   861,   862,   863,   864,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   888,   889,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     903,   904,   908,   912,   913,   914,   918,   920,   922,   924,
     929,   930,   931,   932,   933,   934,   935,   936,   937,   938,
     942,   943,   944,   945,   949,   950,   954,   955,   956,   957,
     958,   962,   963,   967,   968,   972,   974,   979,   980,   984,
     985,   989,   990,   994,   996,   998,  1003,  1016,  1033,  1034,
    1036,  1041,  1048,  1055,  1062,  1070,  1079,  1080,  1081,  1085,
    1086,  1094,  1096,  1102,  1107,  1109,  1111,  1116,  1118,  1120,
    1127,  1128,  1129,  1133,  1134,  1139,  1140,  1141,  1142,  1162,
    1166,  1170,  1178,  1182,  1183,  1184,  1188,  1190,  1196,  1198,
    1200,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1217,  1218,
    1219,  1220,  1224,  1225,  1226,  1230,  1231,  1235,  1236,  1240,
    1241,  1245,  1246,  1247,  1248,  1249,  1253,  1264,  1265,  1266,
    1267,  1268,  1269,  1271,  1273,  1275,  1277,  1279,  1281,  1286,
    1288,  1290,  1292,  1294,  1296,  1298,  1300,  1302,  1304,  1306,
    1308,  1310,  1317,  1323,  1329,  1335,  1344,  1349,  1357,  1358,
    1359,  1360,  1361,  1362,  1363,  1364,  1369,  1370,  1374,  1378,
    1380,  1388,  1398,  1402,  1403,  1408,  1413,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1431,  1433,  1435,  1437,
    1439,  1444,  1445,  1446,  1447,  1458,  1459,  1463,  1464,  1465,
    1469,  1470,  1471,  1479,  1480,  1481,  1485,  1486,  1487,  1488,
    1489,  1490,  1491,  1492,  1499,  1500,  1504,  1505,  1506,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,
    1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1530,
    1531,  1532,  1533,  1534,  1535,  1539,  1540,  1541,  1542,  1546,
    1547,  1548,  1549,  1554,  1555,  1556,  1557,  1558,  1559,  1560
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
  "TREF", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSINGLE", "TSPARSE",
  "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE", "TUNDERSCORE", "TUNION", "TUSE",
  "TVAR", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS",
  "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNSL", "TASSIGNSR", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL", "TMINUS",
  "TMOD", "TNOT", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR",
  "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS",
  "$accept", "program", "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls",
  "stmt", "module_decl_stmt", "access_control", "block_stmt", "stmt_ls",
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
     385,   386,   387,   388
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   134,   135,   136,   136,   137,   137,   138,   138,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   140,   140,   141,   141,   141,
     142,   142,   143,   143,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     146,   146,   147,   148,   148,   149,   149,   150,   150,   151,
     151,   151,   151,   151,   151,   151,   152,   152,   153,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   155,   156,   156,   156,   156,   157,   158,   158,   159,
     159,   160,   161,   161,   162,   162,   162,   163,   163,   164,
     164,   164,   165,   166,   166,   166,   167,   167,   169,   170,
     168,   171,   171,   171,   171,   173,   174,   172,   175,   175,
     175,   175,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     178,   178,   179,   180,   180,   180,   181,   181,   181,   181,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     183,   183,   183,   183,   184,   184,   185,   185,   185,   185,
     185,   186,   186,   187,   187,   188,   188,   189,   189,   190,
     190,   191,   191,   192,   192,   192,   193,   193,   194,   194,
     194,   195,   195,   195,   195,   195,   196,   196,   196,   197,
     197,   198,   198,   198,   199,   199,   199,   200,   200,   200,
     201,   201,   201,   202,   202,   203,   203,   203,   203,   204,
     204,   204,   204,   205,   205,   205,   206,   206,   206,   206,
     206,   207,   207,   207,   207,   207,   207,   207,   208,   208,
     208,   208,   209,   209,   209,   210,   210,   211,   211,   212,
     212,   213,   213,   213,   213,   213,   214,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   217,   218,   219,   219,
     219,   219,   219,   219,   219,   219,   220,   220,   221,   222,
     222,   223,   223,   224,   224,   225,   226,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   228,   228,   228,   228,   229,   229,   230,   230,   230,
     231,   231,   231,   232,   232,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   236,
     236,   236,   236,   236,   236,   237,   237,   237,   237,   238,
     238,   238,   238,   239,   239,   239,   239,   239,   239,   239
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
       3,     0,     0,     1,    34,    62,   376,   377,   378,   379,
     380,     0,   336,    60,   114,   336,     0,   237,    60,     0,
       0,     0,     0,    60,    60,     0,     0,     0,     0,   132,
       0,   128,     0,     0,     0,     0,   327,     0,     0,     0,
       0,   236,   236,   115,     0,     0,     0,     0,     0,     0,
       0,     0,   116,     0,     0,     0,   425,   427,     0,   428,
     429,   360,     0,     0,   426,   423,    69,   424,     0,     0,
       0,     4,     0,     5,     9,     0,    10,    11,    13,   296,
      19,    12,    70,    14,    16,    15,    17,    18,    75,     0,
      73,   352,     0,    76,    74,    77,     0,   361,   348,   349,
     299,   297,     0,     0,   353,   354,     0,   298,     0,   362,
     363,   364,   347,   301,   300,   350,   351,     0,    21,   307,
       0,     0,   337,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,   352,   361,   297,   353,   354,   336,   298,
     362,   363,     0,     0,     0,     0,   287,     0,    64,    63,
     133,    78,     0,   134,     0,     0,     0,     0,     0,     0,
     287,     0,     0,     0,     0,     0,   239,    28,   411,   345,
       0,   412,     7,   287,   237,   238,    72,    71,    67,     0,
       0,     0,    66,    31,     0,   302,     0,   287,    32,   308,
       0,   228,     0,   216,   279,     0,   278,     0,     0,   414,
     359,   410,   413,   409,    40,    42,     0,     0,   282,     0,
     284,     0,     0,   283,     0,   278,     0,     0,     6,     0,
     117,   205,   204,   135,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   287,   287,     0,     0,   190,    22,    23,
       0,    24,     0,     0,     0,     0,     0,     0,     0,    25,
      26,     0,   296,   294,     0,   288,   289,   295,     0,     0,
       0,     0,     0,    87,     0,     0,    86,     0,     0,    93,
       0,     0,   103,     0,    27,   183,   129,   244,     0,   245,
     247,     0,   258,     0,     0,   250,     0,     0,    29,     0,
     134,    68,   107,    65,    30,   287,     0,     0,     0,   226,
     223,   215,     0,    44,    80,    33,    41,    43,   381,     0,
       0,   373,     0,   375,     0,     0,     0,     0,     0,   383,
       8,     0,     0,     0,   200,     0,     0,     0,     0,     0,
     335,   406,   405,   408,   416,   415,   420,   419,   402,   399,
     400,   401,   356,   389,   372,   371,   370,   357,   393,   404,
     398,   396,   407,   397,   395,   387,   392,   394,   403,   386,
     390,   391,   388,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     418,   417,   422,   421,   194,   191,   192,   193,   197,   198,
     199,   423,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,   382,   336,   336,    83,   224,     0,     0,     0,
     304,     0,   126,     0,   123,   225,   117,     0,     0,     0,
     311,     0,     0,     0,   317,     0,     0,    94,   102,     0,
       0,   303,     0,   184,     0,   206,     0,   248,     0,   262,
       0,   257,   348,     0,     0,   241,   346,   240,   369,     0,
       0,   305,   107,     0,   230,   348,     0,   281,   280,   355,
     374,   285,   286,     0,     0,    99,   321,     0,   384,     0,
       0,   118,   119,   201,   202,   203,   136,     0,     0,   232,
     231,   233,   235,    45,    52,    53,    54,    49,    51,    58,
      59,    47,    50,    48,    46,    56,    55,    57,   367,   368,
     195,   196,   341,   190,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,   293,   291,   292,
     290,     0,   124,   122,     0,     0,   101,     0,     0,    85,
       0,    84,     0,     0,    91,     0,     0,    89,     0,     0,
       0,   105,   190,   182,     0,   271,   207,   209,   208,   210,
       0,   246,   249,     0,   250,     0,   242,   251,   252,     0,
       0,   106,   108,   306,     0,     0,   227,     0,     0,     0,
       0,   100,     0,    35,     0,   236,   206,   165,   163,   168,
     175,   176,   177,   172,   174,   170,   173,   171,   169,   179,
     178,   144,   147,   145,   146,   157,   148,   161,   153,   151,
     164,   152,   150,   155,   160,   162,   149,   154,   158,   159,
     156,   166,   167,   142,   180,   180,   234,   340,   342,     0,
       0,     0,     0,     0,     0,     0,     0,    82,    81,   127,
     125,   119,    88,     0,     0,   310,     0,   309,     0,     0,
     316,    92,     0,   315,    90,   104,   326,   185,     0,     0,
     250,   221,   243,     0,     0,   110,     0,   111,     0,     0,
      97,   323,     0,     0,    95,   322,     0,   385,    36,    60,
     236,   236,     0,   112,   236,   120,     0,   143,     0,   183,
     138,   139,     0,     0,     0,     0,     0,     0,     0,     0,
     236,   314,     0,     0,   320,     0,     0,   271,   274,   275,
     276,     0,   273,   277,   348,   217,   187,   186,     0,     0,
       0,   260,   348,   109,     0,    98,     0,    96,   121,   221,
     142,   180,   180,     0,     0,     0,     0,     0,     0,     0,
     113,     0,     0,     0,     0,   250,   263,     0,   218,   220,
     219,   222,   213,   214,   130,     0,     0,     0,     0,   140,
     141,   181,     0,     0,     0,     0,     0,     0,   313,   312,
     319,   318,   189,   188,   265,   266,   268,   348,     0,   382,
     348,   325,   324,   211,   137,   212,     0,   267,   269,   263,
     270
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   205,    72,   468,    74,    75,    76,   206,
      77,    78,   149,    79,   153,   183,    80,    81,    82,    83,
      84,   552,    85,    86,   489,   602,    87,    88,    89,   363,
     615,    90,   453,   454,   133,   162,   475,    92,    93,   364,
     616,   516,   654,   655,   720,   316,   472,   473,   474,   517,
     223,   590,   814,   784,   194,   779,   746,   749,    94,   192,
     339,    95,    96,   165,   166,   320,   321,   485,   324,   325,
     481,   805,   743,   690,   207,   211,   212,   294,   295,   296,
     134,    98,    99,   100,   135,   102,   121,   122,   433,   434,
     103,   136,   137,   106,   139,   108,   140,   141,   111,   112,
     216,   113,   114,   115,   116,   117
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -721
static const yytype_int16 yypact[] =
{
    -721,    75,  2210,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  3359,     6,    80,  -721,     6,  6074,    65,    80,  6074,
    3359,    12,    80,   399,   413,  5095,  6074,  6074,    90,  -721,
      80,  -721,    19,  3359,  6074,  6074,  -721,  6074,  6074,   161,
     140,   807,   970,  -721,  5277,  6074,  5404,  6074,   209,   176,
    3359,    63,  -721,  5484,  6074,  6074,  -721,  -721,  6074,  -721,
    -721,  2065,  6074,  6074,  -721,  6074,  -721,  -721,  2597,  4891,
    5484,  -721,  3232,  -721,  -721,   298,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,    80,
    -721,    77,   159,  -721,  -721,  -721,   310,   199,  -721,  -721,
    -721,   235,   241,   237,   248,   259, 10831,  7010,   314,   269,
     275,  -721,  -721,  -721,  -721,  -721,  -721,   326,  -721, 10831,
     274,  3359,  -721,   277,  -721,   279,  6074,  6074,  6074,  6074,
    6074,  5484,  5484,   320,  -721,  -721,  -721,  -721,  8350,   324,
    -721,  -721,    80,   291,  8553,   338,  5484,   292,  -721,  -721,
    -721,  -721,    80,   160,    80,   296,    23,  7344,  7240,  7478,
    5484,  3359,   306,    16,    55,    31,  -721,  -721,   397,   316,
    7545,   397,  -721,  5484,  -721,   319,  -721,  -721,  -721,  8620,
    8215,  3359,  -721,  -721,  7545, 10831,   307,  5484,  -721, 10831,
    5484,   361,   325,    80,  -721,   -33, 10831,  7545,  8687,   285,
    1287,   397,   285,   397,  -721,  -721,  2724,    88,  -721,  6074,
    -721,   -22,   -16, 10831,    28,  8754,   -28,   443,  -721,    80,
     356,  -721,  -721,  -721,    38,    19,    19,    19,  -721,  6074,
    6074,  6074,  6074,  5586,  5586,  6074,  6074,  6074,  6074,  6074,
    6074,   142,  2065,  6074,  6074,  6074,  6074,  6074,  6074,  6074,
    6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,
    6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,
    6074,  6074,  6074,  5484,  5484,  5586,  5586,   580,  -721,  -721,
    2851,  -721,  8418,  8486,  8821,    41,  5586,    23,   337,  -721,
    -721,  6074,   121,  -721,   333,   363,  -721, 10831,    80,   347,
      80,   438,  5484,  -721,  3486,  5586,  -721,  3613,  5586,  -721,
      23,  3359,   449,   350,  -721,    45,  -721,  -721,    16,  -721,
     383,   358,  -721,  4629,   415,   419,  6074,    19,  -721,   373,
    -721,  -721,  -721,  -721,  -721,  5484,   377,    82,  4709,   406,
    -721,  -721,  5484,  -721,  -721,  -721,  -721,  -721,  -721,  7136,
    4993,  -721,  5175,  -721,  5586,  2470,   378,  6074,  6074,  -721,
    -721,   386,  5484,   388,   267,    19,    92,   192,   213,   230,
    8282, 10865, 10865,   301,  -721,   301,  -721,   301,  7276,   503,
     644,  1615,   316,   285,  -721,  -721,  -721,  1287, 10932,   301,
    1160,  1160, 10865,  1160,  1160,  1598,   285, 10932, 10899,  1598,
     397,   397,   285,  8888,  8955,  9022,  9089,  9156,  9223,  9290,
    9357,  9424,  9491,  9558,  9625,  9692,  9759,  9826,   390,   392,
    -721,   301,  -721,   301,   251,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,    80,    84,  -721,   466,  6154,  5586,  6234,  5586,
    6074,  5586,  6896,     6,  9893,  -721,  -721,  9960,  6074,  5484,
    -721,  5484,   445,   184,  -721,  -721,   356,  6074,    87,  6074,
   10831,    32,  7612,  6074, 10831,    22,  7411,  -721,   507, 10027,
    3359,  -721,   173,  -721,    37,    43,   400,    16,    78,  -721,
    5484,  -721,   289,  6074,  5688,  -721, 10831,  -721,  -721,    24,
     405,  -721,  -721,  5484,  -721,   266,    80,  -721, 10831,  -721,
    -721,  -721,  -721,    14,  1460,  -721, 10831,  3359, 10831, 10094,
    2978,   434,  -721,  -721,  -721,  -721,  -721,  6953,   276,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,   580,  -721,    80,  6074,   514, 10161,  6074,
     516, 10228,   412,  6873,    23,    23,  -721, 10831,  -721, 10831,
    -721,  6074,    80,  -721,   421,  7545,  -721,  7679,  3740,  -721,
    3867,  -721,  7746,  3994,  -721,    23,  4121,  -721,    23,  3359,
    6074,  -721,   472,  -721,    16,   448,  -721,  -721,  -721,  -721,
      68,  -721,  -721,  4709,   419,    56, 10831,  -721, 10831,  3359,
    5484,  -721,  -721,  -721,    34,    73,  -721,  4248,   414,  4375,
     418,  -721,  6074,  -721,  3105,   461,    43,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,    80,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -721,   447,   425,   425,  -721,  -721,  -721, 10295,
    6314,  6394, 10362,  6474,  6554,  6634,  6714,  -721,  -721, 10831,
    -721,  -721,  -721,  3359,  6074, 10831,  6074, 10831,  3359,  6074,
   10831,  -721,  6074, 10831,  -721,  -721, 10831,  -721,   426,  5768,
     246,   479,  -721,  5586,  2340,  -721,    26,  -721,  4502,  6074,
    -721, 10831,  3359,  6074,  -721, 10831,  3359, 10831,  -721,   809,
     254,   254,    80,  -721,   993,  -721,    64,  -721,  6953,    45,
    -721,  -721,  6074,  6074,  6074,  6074,  6074,  6074,  6074,  6074,
     896, 10027,  7813,  7880, 10027,  7947,  8014,   448,    12,  6074,
    6074,  4789,  -721,  -721,   293,  5484,  -721,  -721,  6074,   -34,
    7068,  -721,   492,  -721,  8081,  -721,  8148,  -721,  -721,   479,
    -721,   425,   425,   178, 10429, 10496, 10563, 10630, 10697, 10764,
    -721,  3359,  3359,  3359,  3359,   246,  5870,   169,  -721,  -721,
   10831, 10831,  -721,  -721,  -721,  6714,  3359,  3359,    66,  -721,
    -721,  -721,  6074,  6074,  6074,  6074,  6074,  6074, 10027, 10027,
   10027, 10027,  -721,  -721,  -721,  -721,  -721,   311,  5586,  6794,
     600, 10027, 10027,  -721,  -721,  -721,  7102,  -721,  -721,  5972,
    -721
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -721,  -721,  -721,    -1,  -578,  1577,  -721,  -721,  1221,    46,
    -721,  -721,   315,  1840,    -6,   552,  -688,  -577,   -38,  -721,
    -721,   104,  -721,  -721,    69,  -721,  -721,  -721,   402,   111,
    -103,  -721,  -721,     8,   284,  -721,  -721,  -721,  -721,  -721,
    -721,  -721,  -147,  -141,  -581,  -721,  -140,   -10,  -266,  -721,
    -721,   -35,  -721,  -210,   125,  -721,  -193,  -175,  -721,  -132,
    -721,  -721,  -721,  -195,   258,  -721,  -316,  -576,  -721,  -450,
    -329,  -720,  -700,  -151,     0,   -63,  -721,   -65,  -721,   139,
     303,  -317,  -721,  -721,   525,  -721,    -8,  -146,  -721,    51,
    -721,   822,   905,   -11,  1133,  -721,  1154,  1245,  -721,  -721,
    -721,  -721,  -721,  -721,  -721,  -264
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -383
static const yytype_int16 yytable[] =
{
     119,    71,   476,   176,   177,   138,   482,   125,   144,   494,
     288,   432,   310,   435,   157,   158,   159,   150,   692,     5,
     299,   495,     5,   168,   169,    44,   170,   171,   594,   366,
     367,   368,   369,   179,   180,   184,   185,   714,   715,   189,
       5,     5,   196,   197,   198,   573,   181,   199,  -190,   181,
     200,   201,   202,   195,   203,   568,   322,   196,   213,   215,
     354,   783,   586,   326,   424,   322,     5,   342,   356,   322,
     214,   599,   358,   441,   721,     3,   806,   425,   350,   322,
     426,   599,   120,     5,   352,   317,   343,  -255,   693,   817,
     120,   280,   587,   427,   428,   313,   365,   600,   120,   820,
     783,   588,   359,   429,   120,   693,   351,   600,   329,   818,
    -255,   589,   353,   430,   747,   119,   282,   283,   284,   189,
     196,   215,   336,  -255,   190,    44,   342,  -255,   342,   156,
     287,   327,   285,   142,  -253,   297,  -255,   758,  -255,   146,
     691,   342,  -255,   318,   607,     5,   163,   280,   280,   297,
     601,   280,   714,   715,   323,  -255,   342,   280,   355,  -255,
     697,   592,   297,   593,   584,   163,   384,   593,   172,   330,
     518,   442,  -190,   342,  -255,    14,   297,   593,  -255,   196,
     789,   790,   342,  -255,   543,   813,   694,   342,   342,  -255,
     337,   280,   327,  -255,   210,   221,  -332,  -255,   349,   803,
     448,   808,   449,   698,  -366,   347,  -366,   492,   418,   419,
     385,   519,   544,   222,   348,   566,    43,   160,   370,   371,
     372,   373,   375,   377,   378,   379,   380,   381,   382,   383,
      52,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   297,   297,   421,   423,   759,   173,   688,   342,
     490,   293,   174,   582,   186,   444,   482,   432,   582,   435,
     447,    23,   175,   540,   562,   293,    91,   501,    29,   502,
    -131,   196,   327,   460,   462,    91,   464,   466,   293,   809,
     469,   583,   458,   187,    91,    97,   791,   232,  -131,   541,
     563,   520,   293,   327,    97,   486,   513,    91,  -329,   575,
     578,  -256,     5,    97,   297,   514,   148,   484,   123,   224,
     327,   498,   521,   143,    91,   515,    97,   374,   376,   213,
     219,   213,   233,   504,   506,   234,   508,   509,  -256,   522,
     745,   196,    91,    97,  -328,   -60,    91,   608,   610,   225,
     228,   229,   511,  -256,   606,   751,  -229,  -333,   226,   751,
    -256,    97,   744,   -60,  -272,    97,   327,   752,  -334,   420,
     422,   752,   227,   275,   239,  -229,   276,   241,  -331,  -256,
     443,   244,  -264,  -272,  -330,   656,   279,  -272,   293,   293,
     239,   277,     5,   241,   280,    91,   148,   244,  -256,   461,
     289,  -264,   465,   291,  -256,  -264,     5,   298,   241,   232,
     148,  -272,   151,   302,    97,   460,   548,   464,   551,   469,
     553,   506,  -238,   315,   335,   554,   555,   557,   559,  -264,
     297,   273,   338,   274,   340,    91,   565,  -366,   567,  -366,
     360,  -365,   572,  -365,   233,   362,   446,   234,   503,   807,
     293,   450,  -238,   451,    97,    91,   455,   497,   810,   215,
     457,  -238,   596,   598,   470,   210,    14,   210,   471,    17,
     595,   152,   215,   477,    97,  -238,   478,    22,    23,   709,
      91,   424,   807,   605,   483,    29,   239,  -131,   240,   241,
     484,   488,   807,   244,   425,   491,   496,   426,   507,    97,
     251,   510,    39,   512,   710,  -131,   711,    43,   538,   257,
     427,   428,   539,   545,  -259,   232,   561,   101,   591,   712,
     429,    52,   579,   603,   342,   659,   101,   660,   662,   663,
     430,   547,   665,   550,   702,   101,   671,   689,   706,   718,
     669,  -259,   719,   748,   737,   300,   614,   675,   101,   677,
     233,   604,   680,   234,    91,   683,  -259,   564,   730,   686,
     670,   761,   687,  -259,   558,   101,   293,   762,   815,   763,
      66,   716,   802,    97,   788,   487,   775,   713,    91,   196,
     560,    91,  -259,   101,   657,    91,   701,   101,   705,   424,
     696,   707,   239,     0,   240,   241,     0,    97,     0,   244,
      97,  -259,   425,   347,    97,   426,   251,  -259,     0,     0,
       0,     0,     0,   255,   256,   257,     0,     0,   427,   428,
       0,     0,  -261,     0,     0,     0,     0,     0,   429,    91,
       0,     0,     0,     0,     0,     0,   101,     0,   430,   675,
     677,     0,   680,   683,   701,   705,     0,     0,    97,  -261,
      56,     0,   731,   732,     0,   733,   232,   734,   735,     0,
       0,   736,   176,   177,  -261,    57,     0,    59,    60,     0,
       0,  -261,   750,   506,     0,     0,   101,   506,   754,     0,
       0,     0,   756,     0,     0,     0,    64,   431,     0,     0,
    -261,   233,    67,     0,   234,     0,   101,     0,   303,   306,
     309,   731,   764,   765,   734,   766,   767,   768,   769,  -261,
       0,     0,   328,     0,     0,  -261,     0,     0,   185,   189,
     215,   101,     0,     0,   780,   542,   334,   781,     0,   236,
       0,   777,   238,   239,     0,   240,   241,     0,     0,   344,
     244,     0,     0,     0,    91,     0,     0,   251,     0,     0,
     798,   799,   800,   801,   255,   256,   257,     0,     0,     0,
       0,     0,     0,    97,   705,   811,   812,     0,     0,     0,
       0,   798,   799,   800,   801,   811,   812,     0,     0,     0,
       0,    91,     0,     0,    91,     0,     0,   816,   506,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   705,     0,
      97,     0,     5,    97,   742,     0,   148,     0,     0,     0,
       0,     0,     0,     0,   104,   174,     0,     0,  -238,   101,
       0,     0,   101,   104,    23,   175,   101,     0,     0,   445,
       0,    29,   104,  -131,     0,     0,     0,     0,   658,   -39,
       0,     0,    91,     0,    91,   104,     0,    91,  -238,     0,
      91,  -131,   467,    91,     0,     0,     0,  -238,     0,     0,
     778,    97,   104,    97,     0,     0,    97,   152,     0,    97,
     101,  -238,    97,    91,     0,     0,     0,     0,     0,     0,
     104,    91,     0,    91,   104,     0,     0,     0,    91,     0,
       0,   804,    97,     0,     0,     0,     0,   105,     0,     0,
      97,    14,    97,     0,    17,     0,   105,    97,     0,     0,
       0,     0,    22,    23,   709,   105,     0,     0,     0,     0,
      29,     0,  -131,     0,   804,     0,     0,     0,   105,     0,
       0,     0,     0,   104,   804,     0,     0,    39,     0,   710,
    -131,   711,    43,     0,     0,   105,     0,    91,     0,     0,
       0,     0,    91,     0,   712,     0,    52,     0,     0,     0,
       0,     0,     0,   105,     0,     0,    97,   105,     0,     0,
       0,    97,     0,   104,     0,     0,    91,     0,   174,     0,
      91,     0,     0,     0,     0,   101,     0,    23,   175,     0,
       0,     0,     0,   104,    29,    97,  -131,     0,    14,    97,
       0,    17,   -38,   569,   571,    66,     0,   574,   577,    22,
      23,   709,   770,     0,  -131,     0,   105,    29,   104,  -131,
       0,     0,   101,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,   710,  -131,   711,    43,
       0,     0,     0,     0,     0,    91,    91,    91,    91,     0,
       0,   712,     0,    52,     0,     0,   105,     0,     0,     0,
      91,    91,     0,     0,    97,    97,    97,    97,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,    97,
      97,     0,     0,   101,     0,   101,     0,     0,   101,     0,
       0,   101,   104,     0,   101,     0,   667,   668,     0,     0,
       0,   105,    66,     0,     0,     0,     0,   672,     0,     0,
       0,     0,     0,     0,   101,     0,   104,   681,     0,   104,
     684,     0,   101,   104,   101,   107,     0,     0,     0,   101,
       0,     0,     0,     0,   107,     0,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,   109,     0,     0,     0,
       0,     0,     0,     0,     0,   109,   107,     0,     0,     0,
       0,     0,     0,     0,   109,     0,     0,   104,     0,     0,
       0,     0,   232,   107,     0,   105,     0,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   107,     0,   101,   109,   107,     0,     0,     0,   105,
       0,     0,   105,     0,     0,     0,   105,   233,     0,     0,
     234,     0,   109,     0,     0,     0,   109,   101,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,   753,     0,
       0,     0,     0,     0,   107,   236,   110,   237,   238,   239,
     105,   240,   241,   242,     0,   110,   244,   182,     0,     0,
       0,     0,   250,   251,     0,   109,     0,   254,   110,     0,
     255,   256,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,   107,   110,   101,   101,   101,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,   101,   101,   110,   107,   109,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,   104,     0,     0,   109,     0,     0,     0,   107,
       0,     0,     0,     0,   233,     0,   281,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     109,     0,     0,     0,     0,     0,   110,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,   182,   182,   182,
     312,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     104,   182,   104,   244,     0,   104,     0,     0,   104,   250,
     251,   104,     0,     0,   254,   182,   110,   255,   256,   257,
       0,     0,   105,   107,     0,   105,     0,     0,   182,     0,
       0,   104,     0,     0,     0,     0,   110,     0,     0,   104,
       0,   104,     0,     0,   109,     0,   104,   107,     0,     0,
     107,     0,     0,     0,   107,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,     0,     0,     0,   109,     0,
       0,   109,     0,     0,     0,   109,     0,     0,     0,     0,
     230,     0,     0,   105,   231,   105,     0,     0,   105,     0,
       0,   105,   232,     0,   105,     0,     0,     0,   107,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,     0,
     104,     0,     0,     0,   105,     0,     0,     0,   182,   109,
       0,     0,   105,     0,   105,     0,     0,   233,     0,   105,
     234,     0,     0,     0,   104,   110,     0,     0,   104,     0,
       0,   182,     0,     0,     0,     0,   120,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,     0,   110,
       0,     0,   110,     0,     0,   236,   110,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,   105,    73,
     255,   256,   257,   105,     0,     0,     0,     0,   118,     0,
     609,     0,     0,   104,   104,   104,   104,   145,     0,     0,
     110,     0,     0,   107,     0,     0,     0,   105,   104,   104,
     167,   105,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,   109,     0,     0,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
     107,     0,     0,   107,     0,    73,     0,     0,     0,   218,
       0,     0,     0,     0,     0,   233,     0,     0,   234,     0,
       0,   109,     0,     0,   109,     0,     0,     0,     0,     0,
       0,     0,   233,     0,     0,   234,   105,   105,   105,   105,
       0,     0,   182,   182,     0,     0,   182,   182,     0,     0,
       0,   105,   105,   236,     0,   237,   238,   239,   278,   240,
     241,   107,     0,   107,   244,     0,   107,     0,     0,   107,
     236,   251,   107,     0,   239,   110,   240,   241,   255,   256,
     257,   244,   109,     0,   109,     0,     0,   109,   251,     0,
     109,     0,   107,   109,     0,   255,   256,   257,   314,     0,
     107,     0,   107,     0,     0,     0,     0,   107,     0,     0,
       0,     0,   110,   109,     0,   110,     0,     0,   333,     0,
       0,   109,     0,   109,     0,     0,     0,     0,   109,     0,
       0,     0,     0,     0,     0,   182,   182,     0,     0,     0,
       0,     0,     0,    73,     0,     0,   182,     0,   312,     0,
       0,     0,     0,   312,     0,     0,   182,     0,     0,   182,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
       0,   107,     0,   110,     0,   110,     0,     0,   110,     0,
       0,   110,     0,     0,   110,     0,     0,   109,     0,     0,
       0,     0,   109,     0,     0,   107,     0,     0,     0,   107,
       0,     0,     0,     0,   110,     0,     0,     0,     0,     0,
       0,     0,   110,   124,   110,     0,   109,    73,   124,   110,
     109,     0,   147,   124,   124,     0,     0,     0,     0,     0,
     161,     0,   164,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,   333,     0,     0,     0,     0,     0,
       0,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   107,   107,   107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   182,   110,   107,
     107,     0,     0,   110,     0,   109,   109,   109,   109,   220,
       0,     0,   505,     0,     0,     0,     0,     0,     0,     0,
     109,   109,     0,     0,     0,     0,     0,   110,     0,     0,
       0,   110,     0,   312,   312,     0,   312,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     782,     0,     0,     0,     0,   312,     0,   312,     0,     0,
       0,     0,   191,     0,     0,     0,   292,     0,     0,     0,
       0,     0,   191,     0,   301,     0,     0,     0,     0,     0,
     292,     0,     0,   319,     0,     0,     0,     0,     0,   782,
       0,     0,     0,   292,     0,   124,   110,   110,   110,   110,
       0,     0,     0,     0,     0,     0,     0,   292,     0,     0,
       0,   110,   110,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   581,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   361,
       0,     0,     0,     0,   164,   164,   164,   164,     5,     6,
       7,     8,     9,    10,     0,     0,   126,     0,     0,     0,
       0,   386,     0,     0,   611,     0,     0,    73,     0,    21,
       0,     0,     0,     0,   127,   128,   129,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,   292,   292,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,     0,     0,     0,     0,     0,     0,   452,     0,
     456,     0,     0,     0,     0,   333,     0,   333,     0,     0,
     333,     0,     0,   333,     0,     0,   685,     0,   319,     0,
      57,    58,    59,    60,     0,     0,     0,   164,     0,     0,
       0,     0,     0,     0,     0,   292,   695,    62,     0,    63,
       0,     0,    65,     0,   700,     0,   704,    67,     0,     0,
     131,    73,    69,     0,   132,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,     0,     0,     0,     0,
      -2,     4,     0,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -236,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,    39,    40,    41,  -131,    42,    43,     0,  -236,    44,
       0,    45,    46,    47,    48,    49,    50,     0,    51,   755,
      52,    53,  -236,   757,     0,    54,     0,    55,     0,     0,
      56,   292,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,   585,     0,     0,   319,     0,     0,
       0,     0,    62,     0,    63,     0,    64,    65,     0,    66,
       0,     0,    67,     0,     0,    68,   191,    69,     0,    70,
       0,   479,     0,     5,     6,     7,     8,     9,    10,     0,
    -382,   126,     0,     0,  -382,     0,     0,   653,     0,     0,
       0,     0,  -382,     0,    21,     0,     0,     0,     0,   127,
     128,   129,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,  -382,     0,     0,
    -382,     0,   452,    47,    48,    49,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -254,
    -382,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -382,    58,  -382,  -382,  -382,
       0,  -382,  -382,  -382,     0,  -382,  -382,  -382,  -382,  -382,
    -382,  -382,  -382,  -382,    63,  -382,  -382,  -382,     0,     0,
    -382,  -382,  -382,     0,     0,   131,     0,    69,     0,   493,
       0,     4,   717,     5,     6,     7,     8,     9,    10,     0,
    -382,    11,    12,    13,  -382,    14,    15,    16,    17,  -236,
      18,    19,  -382,    20,    21,  -382,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,     0,    40,    41,  -131,    42,    43,  -382,  -236,    44,
    -382,    45,    46,    47,    48,    49,    50,     0,    51,     0,
      52,    53,  -236,     0,     0,    54,     0,    55,     0,   124,
    -382,     0,   191,     0,     0,     0,     0,     0,   760,     0,
       0,     0,     0,     0,     0,  -382,    58,  -382,  -382,  -382,
    -382,  -382,  -382,  -382,     0,  -382,  -382,  -382,  -382,  -382,
    -382,  -382,  -382,  -382,    63,  -382,  -382,  -382,     0,    66,
    -382,  -382,  -382,     0,     0,    68,  -382,    69,     4,    70,
       5,     6,     7,     8,     9,    10,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -236,    18,    19,     0,
      20,    21,     0,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -131,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,  -236,    38,    39,    40,
      41,  -131,    42,    43,     0,  -236,    44,     0,    45,    46,
      47,    48,    49,    50,     0,    51,     0,    52,    53,  -236,
       0,     0,    54,     0,    55,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,   193,    66,     0,     0,    67,
       0,     0,    68,   204,    69,     4,    70,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -236,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -131,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,  -236,    38,    39,    40,    41,  -131,    42,
      43,     0,  -236,    44,     0,    45,    46,    47,    48,    49,
      50,     0,    51,     0,    52,    53,  -236,     0,     0,    54,
       0,    55,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,    63,     0,
      64,    65,     0,    66,     0,     0,    67,     0,     0,    68,
     346,    69,     4,    70,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -236,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -131,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
    -236,    38,    39,    40,    41,  -131,    42,    43,     0,  -236,
      44,     0,    45,    46,    47,    48,    49,    50,     0,    51,
       0,    52,    53,  -236,     0,     0,    54,     0,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    63,     0,    64,    65,     0,
      66,     0,     0,    67,     0,     0,    68,   204,    69,     4,
      70,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -236,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -131,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,  -236,    38,    39,
      40,    41,  -131,    42,    43,     0,  -236,    44,     0,    45,
      46,    47,    48,    49,    50,     0,    51,     0,    52,    53,
    -236,     0,     0,    54,     0,    55,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,    63,     0,    64,    65,     0,    66,     0,     0,
      67,     0,     0,    68,   613,    69,     4,    70,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -236,    18,    19,     0,    20,    21,
       0,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -131,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,  -236,    38,    39,    40,    41,  -131,
      42,    43,     0,  -236,    44,     0,    45,    46,    47,    48,
      49,    50,     0,    51,     0,    52,    53,  -236,     0,     0,
      54,     0,    55,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,    63,
       0,    64,    65,     0,    66,     0,     0,    67,     0,     0,
      68,   708,    69,     4,    70,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -236,    18,    19,     0,    20,    21,     0,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -131,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,  -236,    38,   217,    40,    41,  -131,    42,    43,     0,
    -236,    44,     0,    45,    46,    47,    48,    49,    50,     0,
      51,     0,    52,    53,  -236,     0,     0,    54,     0,    55,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,    66,     0,     0,    67,     0,     0,    68,     0,    69,
       4,    70,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -236,    18,
      19,     0,    20,    21,     0,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -131,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,  -236,    38,
       0,    40,    41,  -131,    42,    43,     0,  -236,    44,     0,
      45,    46,    47,    48,    49,    50,     0,    51,     0,    52,
      53,  -236,     0,     0,    54,     0,    55,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    63,     0,    64,    65,     0,    66,     0,
       0,    67,     0,     0,    68,     0,    69,     4,    70,     5,
       6,     7,     8,     9,    10,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -236,    18,    19,     0,    20,
      21,     0,    22,    23,    24,    25,    26,   459,     0,    28,
      29,     0,  -131,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,  -236,    38,     0,    40,    41,
    -131,    42,    43,     0,  -236,    44,     0,    45,    46,    47,
      48,    49,    50,     0,    51,     0,    52,    53,  -236,     0,
       0,    54,     0,    55,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
      63,     0,    64,    65,     0,    66,     0,     0,    67,     0,
       0,    68,     0,    69,     4,    70,     5,     6,     7,     8,
       9,    10,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -236,    18,    19,     0,    20,    21,     0,    22,
      23,    24,    25,    26,   463,     0,    28,    29,     0,  -131,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,  -236,    38,     0,    40,    41,  -131,    42,    43,
       0,  -236,    44,     0,    45,    46,    47,    48,    49,    50,
       0,    51,     0,    52,    53,  -236,     0,     0,    54,     0,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,    63,     0,    64,
      65,     0,    66,     0,     0,    67,     0,     0,    68,     0,
      69,     4,    70,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -236,
      18,    19,     0,    20,    21,     0,    22,    23,    24,    25,
      26,   674,     0,    28,    29,     0,  -131,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,  -236,
      38,     0,    40,    41,  -131,    42,    43,     0,  -236,    44,
       0,    45,    46,    47,    48,    49,    50,     0,    51,     0,
      52,    53,  -236,     0,     0,    54,     0,    55,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,    63,     0,    64,    65,     0,    66,
       0,     0,    67,     0,     0,    68,     0,    69,     4,    70,
       5,     6,     7,     8,     9,    10,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -236,    18,    19,     0,
      20,    21,     0,    22,    23,    24,    25,    26,   676,     0,
      28,    29,     0,  -131,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,  -236,    38,     0,    40,
      41,  -131,    42,    43,     0,  -236,    44,     0,    45,    46,
      47,    48,    49,    50,     0,    51,     0,    52,    53,  -236,
       0,     0,    54,     0,    55,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,     0,    66,     0,     0,    67,
       0,     0,    68,     0,    69,     4,    70,     5,     6,     7,
       8,     9,    10,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -236,    18,    19,     0,    20,    21,     0,
      22,    23,    24,    25,    26,   679,     0,    28,    29,     0,
    -131,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,  -236,    38,     0,    40,    41,  -131,    42,
      43,     0,  -236,    44,     0,    45,    46,    47,    48,    49,
      50,     0,    51,     0,    52,    53,  -236,     0,     0,    54,
       0,    55,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,    63,     0,
      64,    65,     0,    66,     0,     0,    67,     0,     0,    68,
       0,    69,     4,    70,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -236,    18,    19,     0,    20,    21,     0,    22,    23,    24,
      25,    26,   682,     0,    28,    29,     0,  -131,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
    -236,    38,     0,    40,    41,  -131,    42,    43,     0,  -236,
      44,     0,    45,    46,    47,    48,    49,    50,     0,    51,
       0,    52,    53,  -236,     0,     0,    54,     0,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    63,     0,    64,    65,     0,
      66,     0,     0,    67,     0,     0,    68,     0,    69,     4,
      70,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -236,    18,    19,
       0,    20,    21,     0,    22,    23,    24,    25,    26,   699,
       0,    28,    29,     0,  -131,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,  -236,    38,     0,
      40,    41,  -131,    42,    43,     0,  -236,    44,     0,    45,
      46,    47,    48,    49,    50,     0,    51,     0,    52,    53,
    -236,     0,     0,    54,     0,    55,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,    63,     0,    64,    65,     0,    66,     0,     0,
      67,     0,     0,    68,     0,    69,     4,    70,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -236,    18,    19,     0,    20,    21,
       0,    22,    23,    24,    25,    26,   703,     0,    28,    29,
       0,  -131,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,  -236,    38,     0,    40,    41,  -131,
      42,    43,     0,  -236,    44,     0,    45,    46,    47,    48,
      49,    50,     0,    51,     0,    52,    53,  -236,     0,     0,
      54,     0,    55,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,    63,
       0,    64,    65,     0,    66,     0,     0,    67,     0,     0,
      68,     0,    69,   479,    70,     5,     6,     7,     8,     9,
      10,     0,  -382,   126,     0,     0,  -382,     0,     0,     0,
       0,     0,     0,     0,  -382,     0,    21,     0,     0,     0,
       0,   127,   128,   129,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,  -382,
       0,     0,  -382,     0,     0,    47,    48,    49,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -382,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -382,    58,  -382,
    -382,  -382,     0,  -382,  -382,  -382,     0,  -382,  -382,  -382,
    -382,  -382,  -382,  -382,  -382,  -382,    63,  -382,  -382,  -382,
       0,     0,  -382,  -382,  -382,     0,     0,   131,     0,    69,
     479,   493,     5,     6,     7,     8,     9,    10,     0,     0,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,   127,   128,
     129,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
     479,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     126,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,    21,     0,     0,     0,     0,   127,   128,
     129,    62,    28,    63,     0,    64,    65,    31,    32,     0,
      34,    67,    35,    36,   131,     0,    69,     0,   480,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
     126,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,    21,     0,     0,     0,     0,   127,   128,
     129,    62,    28,    63,     0,    64,    65,    31,    32,     0,
      34,    67,    35,    36,   131,     0,    69,     0,   493,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     5,     6,     7,     8,     9,    10,
       0,    62,   126,    63,     0,    64,    65,   193,     0,     0,
       0,    67,     0,     0,   131,    21,    69,     0,   132,   776,
     127,   128,   129,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,    48,    49,   130,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,   209,     5,     6,     7,     8,
       9,    10,     0,    62,   126,    63,     0,    64,    65,   193,
       0,     0,     0,    67,     0,     0,   131,    21,    69,     0,
     132,     0,   127,   128,   129,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    48,    49,   130,
       0,     0,   208,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,     0,     5,     6,
       7,     8,     9,    10,     0,    62,   126,    63,     0,    64,
      65,   193,     0,     0,     0,    67,     0,     0,   131,    21,
      69,   500,   132,     0,   127,   128,   129,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,   154,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,     0,    56,     0,     0,     5,     6,
       7,     8,     9,    10,     0,     0,   126,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,    21,
       0,     0,     0,     0,   127,   128,   129,    62,    28,    63,
       0,    64,    65,    31,    32,     0,    34,    67,    35,    36,
     131,     0,    69,     0,   132,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,     0,     0,   208,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       5,     6,     7,     8,     9,    10,     0,    62,   126,    63,
       0,    64,    65,   193,     0,     0,     0,    67,     0,     0,
     131,    21,    69,     0,   132,     0,   127,   128,   129,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,     0,   178,     0,     0,    67,
       0,     0,   131,     0,    69,     0,   132,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   181,    21,     0,
       0,     0,     0,   127,   128,   129,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   129,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,    68,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     5,
       6,     7,     8,     9,    10,     0,    62,   126,    63,     0,
      64,    65,   193,     0,     0,     0,    67,     0,     0,   131,
      21,    69,     0,   132,     0,   127,   128,   129,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
      48,    49,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
       0,     5,     6,     7,     8,     9,    10,     0,    62,   126,
      63,     0,    64,    65,     0,     0,     0,     0,    67,     0,
       0,   131,    21,    69,     0,   132,     0,   127,   128,   129,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,   597,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    48,    49,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   126,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,   738,     0,     0,     0,     0,   127,   128,   129,
      62,    28,    63,     0,    64,    65,    31,    32,     0,    34,
      67,    35,    36,   131,     0,    69,     0,   132,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   739,    48,    49,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     5,     6,     7,     8,     9,    10,     0,
      62,   126,    63,     0,    64,    65,   193,     0,     0,     0,
      67,     0,     0,   131,    21,    69,     0,   741,     0,   127,
     128,   129,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     5,     6,     7,     8,     9,
      10,     0,    62,   126,    63,     0,    64,    65,   193,     0,
       0,     0,    67,     0,     0,   131,    21,    69,     0,   741,
       0,   127,   128,   729,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     5,     6,     7,
       8,     9,    10,     0,    62,   126,    63,     0,    64,    65,
     193,     0,     0,     0,    67,     0,     0,   131,    21,    69,
       0,   132,     0,   127,   128,   129,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   546,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   549,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   723,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   724,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   726,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   727,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   728,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   729,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,    10,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    21,     0,
       0,     0,     0,   127,   128,   129,    62,    28,    63,     0,
      64,    65,    31,    32,     0,    34,    67,    35,    36,   131,
       0,    69,     0,   132,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -263,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,     0,   231,     0,     0,
      58,     0,     0,     0,  -263,   232,     0,     0,  -263,     5,
       6,     7,     8,     9,    10,     0,     0,   126,    63,     0,
       0,     0,   193,     0,     0,     0,     0,     0,     0,   131,
      21,    69,  -263,   741,     0,   127,   128,   129,     0,    28,
     233,     0,     0,   234,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,   235,     0,     0,     5,     0,     0,    47,
      48,    49,   130,   617,     0,     0,     0,   618,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,    58,   255,   256,   257,     0,     0,     0,     0,
       0,     0,     0,   666,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,   132,     0,     0,     0,     0,
       0,     0,     0,     0,   619,   620,   621,   622,   623,   624,
       0,     0,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,     0,     0,   635,     0,     0,     0,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,     0,
     647,     0,     0,   648,   649,   650,   651,   652,   230,     0,
       0,     0,   231,     0,     0,     0,     0,     0,     0,     0,
     232,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,     0,   231,     0,     0,     0,
       0,     0,     0,     0,   232,   233,     0,     0,   234,     0,
       0,     0,     0,   272,  -343,     0,     0,  -365,     0,  -365,
       0,     0,     0,     0,     0,     0,   230,     0,   235,     0,
     231,     0,     0,     0,     0,     0,     0,     0,   232,   233,
       0,     0,   234,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   235,   252,   253,   254,     0,     0,   255,   256,
     257,     0,     0,   233,     0,     0,   234,   236,   785,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   235,   252,   253,   254,
       0,     0,   255,   256,   257,     0,     0,     0,     0,     0,
       0,   236,   819,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     230,   252,   253,   254,   231,     0,   255,   256,   257,     0,
       0,     0,   232,   307,   499,     0,     0,     0,     0,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   233,   232,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   233,     0,   236,   234,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   230,   252,   253,   254,   231,     0,
     255,   256,   257,     0,     0,   280,   232,   304,     0,     0,
       0,   236,     0,   237,   238,   239,   305,   240,   241,   242,
       0,   243,   244,   245,   246,     0,   248,   249,   250,   251,
       0,   252,     0,   254,     0,     0,   255,   256,   257,     0,
       0,   233,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,   235,   231,     0,     0,     0,     0,
       0,     0,     0,   232,   576,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,     0,   255,   256,   257,     0,   233,   280,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   120,   230,     0,
       0,   235,   231,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
       0,   255,   256,   257,     0,   233,   280,     0,   234,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,     0,     0,   235,   231,
       0,     0,     0,     0,     0,     0,     0,   232,   181,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,     0,   255,   256,
     257,     0,   233,   280,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,   235,   231,     0,     0,     0,
       0,     0,     0,     0,   232,   570,     0,     0,     0,     0,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,     0,   255,   256,   257,     0,   233,
     280,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,   235,   231,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,     0,   255,   256,   257,     0,   233,   280,     0,   234,
       0,     0,     0,     0,     0,     0,   673,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,   235,
     231,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,     0,   255,
     256,   257,     0,   233,   280,     0,   234,     0,     0,     0,
       0,     0,     0,   678,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,   235,   231,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,     0,   255,   256,   257,     0,
     233,   280,     0,   234,     0,     0,     0,     0,     0,     0,
     771,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,   235,   231,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,     0,   255,   256,   257,     0,   233,   280,     0,
     234,     0,     0,     0,     0,     0,     0,   772,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
     235,   231,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,     0,
     255,   256,   257,     0,   233,   280,     0,   234,     0,     0,
       0,     0,     0,     0,   773,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   235,   231,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,     0,   255,   256,   257,
       0,   233,   280,     0,   234,     0,     0,     0,     0,     0,
       0,   774,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,   235,   231,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,     0,   255,   256,   257,     0,   233,   280,
       0,   234,     0,     0,     0,     0,     0,     0,   786,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,     0,
       0,   235,   231,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
       0,   255,   256,   257,     0,   233,   280,     0,   234,     0,
       0,     0,     0,     0,     0,   787,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,     0,     0,   235,   231,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,     0,   255,   256,
     257,     0,   233,   280,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,   235,   231,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,     0,   255,   256,   257,     0,   233,
     332,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,   235,     0,   231,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   236,     0,   237,
     238,   239,   286,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,     0,   255,   256,   257,     0,  -344,   233,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,   230,     0,
     235,     0,   231,     0,     0,     0,     0,     0,     0,     0,
     232,   436,     0,     0,     0,   236,     0,   237,   238,   239,
     437,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,     0,
     255,   256,   257,     0,     0,   233,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,   235,     0,
     231,     0,     0,     0,     0,     0,     0,     0,   232,   438,
       0,     0,     0,   236,     0,   237,   238,   239,   439,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,     0,   255,   256,
     257,     0,     0,   233,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,   235,   231,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,     0,   255,   256,   257,     0,
     233,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,   235,   231,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,   290,   255,   256,   257,     0,   233,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
     235,   231,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,   331,
     255,   256,   257,     0,   233,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   235,   231,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   345,   255,   256,   257,
       0,   233,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,   357,   235,   231,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,     0,   255,   256,   257,     0,   233,     0,
       0,   234,     0,     0,     0,     0,     0,     0,   440,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,     0,
       0,   235,   231,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
       0,   255,   256,   257,     0,   233,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,     0,     0,   235,   231,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,   523,   255,   256,
     257,     0,   233,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,   235,   231,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,   524,   255,   256,   257,     0,   233,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,   235,   231,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,   525,   255,   256,   257,     0,   233,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,   235,
     231,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,   526,   255,
     256,   257,     0,   233,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,   235,   231,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,   527,   255,   256,   257,     0,
     233,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,   235,   231,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,   528,   255,   256,   257,     0,   233,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
     235,   231,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,   529,
     255,   256,   257,     0,   233,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   235,   231,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   530,   255,   256,   257,
       0,   233,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,   235,   231,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,   531,   255,   256,   257,     0,   233,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,     0,
       0,   235,   231,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
     532,   255,   256,   257,     0,   233,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,     0,     0,   235,   231,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,   533,   255,   256,
     257,     0,   233,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,   235,   231,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,   534,   255,   256,   257,     0,   233,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,   235,   231,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,   535,   255,   256,   257,     0,   233,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,   235,
     231,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,   536,   255,
     256,   257,     0,   233,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,   235,   231,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,   537,   255,   256,   257,     0,
     233,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120,
     230,     0,     0,   235,   231,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,     0,   255,   256,   257,     0,   233,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
     235,   231,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,   580,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,   556,
     255,   256,   257,     0,   233,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   235,   231,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,     0,   255,   256,   257,
       0,   233,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,   612,   235,   231,     0,     0,     0,     0,
       0,     0,     0,   232,   661,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,     0,   255,   256,   257,     0,   233,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   230,     0,
       0,   235,   231,     0,     0,     0,     0,     0,     0,     0,
     232,   664,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
       0,   255,   256,   257,     0,   233,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   230,     0,     0,   235,   231,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,     0,   255,   256,
     257,     0,   233,     0,     0,   234,     0,     0,     0,     0,
       0,     0,   722,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,     0,   235,   231,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,     0,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,     0,   255,   256,   257,     0,   233,
       0,     0,   234,     0,     0,     0,     0,     0,     0,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,   235,   231,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,     0,   255,   256,   257,     0,   233,     0,     0,   234,
       0,     0,     0,     0,     0,     0,   792,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,   235,
     231,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,     0,   255,
     256,   257,     0,   233,     0,     0,   234,     0,     0,     0,
       0,     0,     0,   793,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,     0,     0,   235,   231,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,     0,   255,   256,   257,     0,
     233,     0,     0,   234,     0,     0,     0,     0,     0,     0,
     794,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     230,     0,     0,   235,   231,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,     0,   255,   256,   257,     0,   233,     0,     0,
     234,     0,     0,     0,     0,     0,     0,   795,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
     235,   231,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,     0,
     255,   256,   257,     0,   233,     0,     0,   234,     0,     0,
       0,     0,     0,     0,   796,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,   235,   231,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,     0,   255,   256,   257,
       0,   233,     0,     0,   234,     0,     0,     0,     0,     0,
       0,   797,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,   235,   231,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,     0,   255,   256,   257,   232,   233,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,   233,     0,     0,   234,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   235,   252,   253,   254,     0,
       0,   255,   256,   257,   232,     0,   233,     0,     0,   234,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,     0,   248,   249,   250,   251,   235,
     252,   253,   254,     0,     0,   255,   256,   257,     0,   233,
       0,     0,   234,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,     0,   248,
     249,   250,   251,     0,   252,     0,   254,     0,     0,   255,
     256,   257,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,     0,   244,   245,
     246,     0,   248,   249,   250,   251,     0,     0,     0,   254,
       0,     0,   255,   256,   257
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-721))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   318,    41,    42,    16,   323,    15,    19,   338,
     142,   277,   158,   277,    25,    26,    27,    23,   594,     3,
     152,   338,     3,    34,    35,    59,    37,    38,   478,   224,
     225,   226,   227,    44,    45,    46,    47,   615,   615,    50,
       3,     3,    53,    54,    55,    23,    23,    58,     3,    23,
      61,    62,    63,    53,    65,    23,     1,    68,    69,    70,
      32,   749,    19,    32,    19,     1,     3,   100,   214,     1,
      70,    47,   100,    32,   655,     0,   776,    32,   100,     1,
      35,    47,    76,     3,   100,    69,   119,    32,    32,   809,
      76,   125,    49,    48,    49,   160,    58,    73,    76,   819,
     788,    58,   130,    58,    76,    32,   128,    73,   173,   809,
      32,    68,   128,    68,   690,   126,   127,   128,   129,   130,
     131,   132,   187,    59,    61,    59,   100,    59,   100,    25,
     138,   100,   132,    68,    79,   146,    81,   714,    74,   127,
     590,   100,    74,   127,   130,     3,   127,   125,   125,   160,
     126,   125,   730,   730,    99,   100,   100,   125,   130,    81,
     126,   477,   173,    99,   127,   127,    24,    99,     7,   175,
     365,   130,   127,   100,   119,    15,   187,    99,   100,   190,
     761,   762,   100,   119,   100,   119,   130,   100,   100,   125,
     190,   125,   100,   125,    69,    36,   119,   119,   209,   775,
      79,    32,    81,   130,   127,   206,   129,   125,   273,   274,
      68,   119,   128,    54,   126,   128,    56,   127,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
      70,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   716,   127,   584,   100,
     335,   146,    18,   100,    65,   286,   593,   543,   100,   543,
     291,    27,    28,    32,   100,   160,     2,   350,    34,   352,
      36,   302,   100,   304,   305,    11,   307,   308,   173,   130,
     311,   128,   302,   127,    20,     2,   128,    22,    54,    58,
     126,   119,   187,   100,    11,   326,    49,    33,   119,   465,
     466,    32,     3,    20,   335,    58,     7,    81,    13,    19,
     100,   342,   119,    18,    50,    68,    33,   233,   234,   350,
      42,   352,    57,   354,   355,    60,   357,   358,    59,   119,
     104,   362,    68,    50,   119,    36,    72,   503,   504,    49,
     119,   124,   362,    74,   496,   694,   100,   119,    58,   698,
      81,    68,   689,    54,    81,    72,   100,   694,   119,   275,
     276,   698,    72,    57,    99,   119,    60,   102,   119,   100,
     286,   106,    81,   100,   119,   119,   119,   104,   273,   274,
      99,   127,     3,   102,   125,   121,     7,   106,   119,   305,
     119,   100,   308,    75,   125,   104,     3,   125,   102,    22,
       7,   128,     9,   127,   121,   436,   437,   438,   439,   440,
     441,   442,    19,   127,   127,   443,   444,   448,   449,   128,
     451,   127,    81,   129,   119,   161,   457,   127,   459,   129,
       7,   127,   463,   129,    57,    99,   119,    60,   354,   776,
     335,   128,    49,   100,   161,   181,   119,   342,   785,   480,
      32,    58,   483,   484,    25,   350,    15,   352,   128,    18,
     480,    68,   493,   100,   181,    72,   128,    26,    27,    28,
     206,    19,   809,   493,    79,    34,    99,    36,   101,   102,
      81,   128,   819,   106,    32,   128,   100,    35,   130,   206,
     113,   125,    51,   125,    53,    54,    55,    56,   128,   122,
      48,    49,   130,    57,    32,    22,    81,     2,   128,    68,
      58,    70,    25,   128,   100,   546,    11,    23,   549,    23,
      68,   437,   130,   439,   130,    20,   125,    99,   130,   102,
     561,    59,   127,    74,   128,   153,   510,   568,    33,   570,
      57,   492,   573,    60,   280,   576,    74,   456,   671,   580,
     562,   718,   582,    81,   449,    50,   451,   718,   788,   719,
     119,   616,   775,   280,   759,   327,   737,   126,   304,   600,
     451,   307,   100,    68,   543,   311,   607,    72,   609,    19,
     600,   612,    99,    -1,   101,   102,    -1,   304,    -1,   106,
     307,   119,    32,   614,   311,    35,   113,   125,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,    -1,    -1,    48,    49,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    58,   355,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    68,   660,
     661,    -1,   663,   664,   665,   666,    -1,    -1,   355,    59,
      80,    -1,   673,   674,    -1,   676,    22,   678,   679,    -1,
      -1,   682,   710,   711,    74,    95,    -1,    97,    98,    -1,
      -1,    81,   693,   694,    -1,    -1,   161,   698,   699,    -1,
      -1,    -1,   703,    -1,    -1,    -1,   116,   117,    -1,    -1,
     100,    57,   122,    -1,    60,    -1,   181,    -1,   156,   157,
     158,   722,   723,   724,   725,   726,   727,   728,   729,   119,
      -1,    -1,   170,    -1,    -1,   125,    -1,    -1,   739,   740,
     741,   206,    -1,    -1,   745,   432,   184,   748,    -1,    95,
      -1,   741,    98,    99,    -1,   101,   102,    -1,    -1,   197,
     106,    -1,    -1,    -1,   470,    -1,    -1,   113,    -1,    -1,
     771,   772,   773,   774,   120,   121,   122,    -1,    -1,    -1,
      -1,    -1,    -1,   470,   785,   786,   787,    -1,    -1,    -1,
      -1,   792,   793,   794,   795,   796,   797,    -1,    -1,    -1,
      -1,   507,    -1,    -1,   510,    -1,    -1,   808,   809,    -1,
      -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,   819,    -1,
     507,    -1,     3,   510,   689,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    18,    -1,    -1,    19,   304,
      -1,    -1,   307,    11,    27,    28,   311,    -1,    -1,   287,
      -1,    34,    20,    36,    -1,    -1,    -1,    -1,   545,    42,
      -1,    -1,   568,    -1,   570,    33,    -1,   573,    49,    -1,
     576,    54,   310,   579,    -1,    -1,    -1,    58,    -1,    -1,
     745,   568,    50,   570,    -1,    -1,   573,    68,    -1,   576,
     355,    72,   579,   599,    -1,    -1,    -1,    -1,    -1,    -1,
      68,   607,    -1,   609,    72,    -1,    -1,    -1,   614,    -1,
      -1,   776,   599,    -1,    -1,    -1,    -1,     2,    -1,    -1,
     607,    15,   609,    -1,    18,    -1,    11,   614,    -1,    -1,
      -1,    -1,    26,    27,    28,    20,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,   809,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,   121,   819,    -1,    -1,    51,    -1,    53,
      54,    55,    56,    -1,    -1,    50,    -1,   673,    -1,    -1,
      -1,    -1,   678,    -1,    68,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,   673,    72,    -1,    -1,
      -1,   678,    -1,   161,    -1,    -1,   702,    -1,    18,    -1,
     706,    -1,    -1,    -1,    -1,   470,    -1,    27,    28,    -1,
      -1,    -1,    -1,   181,    34,   702,    36,    -1,    15,   706,
      -1,    18,    42,   461,   462,   119,    -1,   465,   466,    26,
      27,    28,   126,    -1,    54,    -1,   121,    34,   206,    36,
      -1,    -1,   507,    -1,    -1,   510,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    53,    54,    55,    56,
      -1,    -1,    -1,    -1,    -1,   771,   772,   773,   774,    -1,
      -1,    68,    -1,    70,    -1,    -1,   161,    -1,    -1,    -1,
     786,   787,    -1,    -1,   771,   772,   773,   774,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,   786,
     787,    -1,    -1,   568,    -1,   570,    -1,    -1,   573,    -1,
      -1,   576,   280,    -1,   579,    -1,   554,   555,    -1,    -1,
      -1,   206,   119,    -1,    -1,    -1,    -1,   565,    -1,    -1,
      -1,    -1,    -1,    -1,   599,    -1,   304,   575,    -1,   307,
     578,    -1,   607,   311,   609,     2,    -1,    -1,    -1,   614,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    -1,    -1,   355,    -1,    -1,
      -1,    -1,    22,    50,    -1,   280,    -1,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   673,    -1,
      -1,    68,    -1,   678,    50,    72,    -1,    -1,    -1,   304,
      -1,    -1,   307,    -1,    -1,    -1,   311,    57,    -1,    -1,
      60,    -1,    68,    -1,    -1,    -1,    72,   702,    -1,    -1,
      -1,   706,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,   696,    -1,
      -1,    -1,    -1,    -1,   121,    95,    11,    97,    98,    99,
     355,   101,   102,   103,    -1,    20,   106,    46,    -1,    -1,
      -1,    -1,   112,   113,    -1,   121,    -1,   117,    33,    -1,
     120,   121,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   470,    -1,   161,    50,   771,   772,   773,   774,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   786,   787,    68,   181,   161,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   507,
      -1,    -1,   510,    -1,    -1,   181,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    57,    -1,   125,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   470,    -1,   156,   157,   158,
     159,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     568,   170,   570,   106,    -1,   573,    -1,    -1,   576,   112,
     113,   579,    -1,    -1,   117,   184,   161,   120,   121,   122,
      -1,    -1,   507,   280,    -1,   510,    -1,    -1,   197,    -1,
      -1,   599,    -1,    -1,    -1,    -1,   181,    -1,    -1,   607,
      -1,   609,    -1,    -1,   280,    -1,   614,   304,    -1,    -1,
     307,    -1,    -1,    -1,   311,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,
      -1,   307,    -1,    -1,    -1,   311,    -1,    -1,    -1,    -1,
      10,    -1,    -1,   568,    14,   570,    -1,    -1,   573,    -1,
      -1,   576,    22,    -1,   579,    -1,    -1,    -1,   355,    -1,
      -1,    -1,    -1,    -1,    -1,   673,    -1,    -1,    -1,    -1,
     678,    -1,    -1,    -1,   599,    -1,    -1,    -1,   287,   355,
      -1,    -1,   607,    -1,   609,    -1,    -1,    57,    -1,   614,
      60,    -1,    -1,    -1,   702,   280,    -1,    -1,   706,    -1,
      -1,   310,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   304,
      -1,    -1,   307,    -1,    -1,    95,   311,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,   673,     2,
     120,   121,   122,   678,    -1,    -1,    -1,    -1,    11,    -1,
     130,    -1,    -1,   771,   772,   773,   774,    20,    -1,    -1,
     355,    -1,    -1,   470,    -1,    -1,    -1,   702,   786,   787,
      33,   706,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,   470,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     507,    -1,    -1,   510,    -1,    68,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    60,    -1,
      -1,   507,    -1,    -1,   510,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    60,   771,   772,   773,   774,
      -1,    -1,   461,   462,    -1,    -1,   465,   466,    -1,    -1,
      -1,   786,   787,    95,    -1,    97,    98,    99,   121,   101,
     102,   568,    -1,   570,   106,    -1,   573,    -1,    -1,   576,
      95,   113,   579,    -1,    99,   470,   101,   102,   120,   121,
     122,   106,   568,    -1,   570,    -1,    -1,   573,   113,    -1,
     576,    -1,   599,   579,    -1,   120,   121,   122,   161,    -1,
     607,    -1,   609,    -1,    -1,    -1,    -1,   614,    -1,    -1,
      -1,    -1,   507,   599,    -1,   510,    -1,    -1,   181,    -1,
      -1,   607,    -1,   609,    -1,    -1,    -1,    -1,   614,    -1,
      -1,    -1,    -1,    -1,    -1,   554,   555,    -1,    -1,    -1,
      -1,    -1,    -1,   206,    -1,    -1,   565,    -1,   567,    -1,
      -1,    -1,    -1,   572,    -1,    -1,   575,    -1,    -1,   578,
      -1,    -1,    -1,    -1,    -1,    -1,   673,    -1,    -1,    -1,
      -1,   678,    -1,   568,    -1,   570,    -1,    -1,   573,    -1,
      -1,   576,    -1,    -1,   579,    -1,    -1,   673,    -1,    -1,
      -1,    -1,   678,    -1,    -1,   702,    -1,    -1,    -1,   706,
      -1,    -1,    -1,    -1,   599,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   607,    13,   609,    -1,   702,   280,    18,   614,
     706,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   304,    -1,    -1,   307,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   771,   772,   773,   774,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   696,   673,   786,
     787,    -1,    -1,   678,    -1,   771,   772,   773,   774,    89,
      -1,    -1,   355,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     786,   787,    -1,    -1,    -1,    -1,    -1,   702,    -1,    -1,
      -1,   706,    -1,   732,   733,    -1,   735,   736,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     749,    -1,    -1,    -1,    -1,   754,    -1,   756,    -1,    -1,
      -1,    -1,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,
      -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,   788,
      -1,    -1,    -1,   173,    -1,   175,   771,   772,   773,   774,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,
      -1,   786,   787,   193,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   470,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,   224,   225,   226,   227,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,
      -1,   241,    -1,    -1,   507,    -1,    -1,   510,    -1,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,
      -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,   273,   274,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,   298,    -1,
     300,    -1,    -1,    -1,    -1,   568,    -1,   570,    -1,    -1,
     573,    -1,    -1,   576,    -1,    -1,   579,    -1,   318,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,   327,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   335,   599,   112,    -1,   114,
      -1,    -1,   117,    -1,   607,    -1,   609,   122,    -1,    -1,
     125,   614,   127,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   365,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    12,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,   702,
      70,    71,    72,   706,    -1,    75,    -1,    77,    -1,    -1,
      80,   451,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,   474,    -1,    -1,   477,    -1,    -1,
      -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,   125,   496,   127,    -1,   129,
      -1,     1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    -1,    -1,    14,    -1,    -1,   517,    -1,    -1,
      -1,    -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    57,    -1,    -1,
      60,    -1,   562,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    -1,    -1,    -1,   584,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,   121,   122,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,     1,   632,     3,     4,     5,     6,     7,     8,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,   709,
      80,    -1,   712,    -1,    -1,    -1,    -1,    -1,   718,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,   122,    -1,    -1,   125,   126,   127,     1,   129,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    12,
      13,    -1,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    24,    -1,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    70,    71,    72,
      -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,   118,   119,    -1,    -1,   122,
      -1,    -1,   125,   126,   127,     1,   129,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,
     116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,
     126,   127,     1,   129,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,   125,   126,   127,     1,
     129,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    70,    71,
      72,    -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,   125,   126,   127,     1,   129,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    12,    13,    -1,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    24,
      -1,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,   116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
     125,   126,   127,     1,   129,     3,     4,     5,     6,     7,
       8,    -1,    -1,    11,    12,    13,    -1,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    -1,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,
       1,   129,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    46,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    55,    56,    -1,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    70,
      71,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,   125,    -1,   127,     1,   129,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    -1,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    55,    56,    -1,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    -1,    68,    -1,    70,    71,    72,    -1,
      -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,    -1,
      -1,   125,    -1,   127,     1,   129,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    12,    13,    -1,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    24,    -1,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    46,
      -1,    -1,    49,    50,    -1,    52,    53,    54,    55,    56,
      -1,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,
      77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,
     117,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,
     127,     1,   129,     3,     4,     5,     6,     7,     8,    -1,
      -1,    11,    12,    13,    -1,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    24,    -1,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    46,    -1,    -1,    49,
      50,    -1,    52,    53,    54,    55,    56,    -1,    58,    59,
      -1,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,
      -1,    -1,   122,    -1,    -1,   125,    -1,   127,     1,   129,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    12,
      13,    -1,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    24,    -1,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    -1,    -1,    49,    50,    -1,    52,
      53,    54,    55,    56,    -1,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    70,    71,    72,
      -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,   125,    -1,   127,     1,   129,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    12,    13,    -1,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    -1,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      46,    -1,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    -1,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,    -1,
     116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,   125,
      -1,   127,     1,   129,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,    -1,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    46,    -1,    -1,
      49,    50,    -1,    52,    53,    54,    55,    56,    -1,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,    -1,
     119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,     1,
     129,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      12,    13,    -1,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    24,    -1,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    46,    -1,    -1,    49,    50,    -1,
      52,    53,    54,    55,    56,    -1,    58,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    70,    71,
      72,    -1,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,
     122,    -1,    -1,   125,    -1,   127,     1,   129,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    12,    13,    -1,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    24,
      -1,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      -1,    46,    -1,    -1,    49,    50,    -1,    52,    53,    54,
      55,    56,    -1,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,   116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
     125,    -1,   127,     1,   129,     3,     4,     5,     6,     7,
       8,    -1,    10,    11,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    60,    -1,    -1,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    -1,   125,    -1,   127,
       1,   129,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,
      41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
       1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,   112,    33,   114,    -1,   116,   117,    38,    39,    -1,
      41,   122,    43,    44,   125,    -1,   127,    -1,   129,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,   112,    33,   114,    -1,   116,   117,    38,    39,    -1,
      41,   122,    43,    44,   125,    -1,   127,    -1,   129,    50,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    -1,     3,     4,     5,     6,     7,     8,
      -1,   112,    11,   114,    -1,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,    -1,   125,    24,   127,    -1,   129,   130,
      29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,
      39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,   104,     3,     4,     5,     6,
       7,     8,    -1,   112,    11,   114,    -1,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,    -1,   125,    24,   127,    -1,
     129,    -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,
      -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,   103,    -1,     3,     4,
       5,     6,     7,     8,    -1,   112,    11,   114,    -1,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,    -1,   125,    24,
     127,   128,   129,    -1,    29,    30,    31,    -1,    33,    -1,
      -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,
      -1,    -1,    -1,    -1,    49,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,   112,    33,   114,
      -1,   116,   117,    38,    39,    -1,    41,   122,    43,    44,
     125,    -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
       3,     4,     5,     6,     7,     8,    -1,   112,    11,   114,
      -1,   116,   117,   118,    -1,    -1,    -1,   122,    -1,    -1,
     125,    24,   127,    -1,   129,    -1,    29,    30,    31,    -1,
      33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,   125,    -1,   127,    -1,   129,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,     3,
       4,     5,     6,     7,     8,    -1,   112,    11,   114,    -1,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,    -1,   125,
      24,   127,    -1,   129,    -1,    29,    30,    31,    -1,    33,
      -1,    -1,    -1,    -1,    38,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      -1,     3,     4,     5,     6,     7,     8,    -1,   112,    11,
     114,    -1,   116,   117,    -1,    -1,    -1,    -1,   122,    -1,
      -1,   125,    24,   127,    -1,   129,    -1,    29,    30,    31,
      -1,    33,    -1,    -1,    -1,    -1,    38,    39,    -1,    41,
      -1,    43,    44,    45,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,   103,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
     112,    33,   114,    -1,   116,   117,    38,    39,    -1,    41,
     122,    43,    44,   125,    -1,   127,    -1,   129,    50,    -1,
      52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,   103,    -1,     3,     4,     5,     6,     7,     8,    -1,
     112,    11,   114,    -1,   116,   117,   118,    -1,    -1,    -1,
     122,    -1,    -1,   125,    24,   127,    -1,   129,    -1,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    38,    39,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    -1,     3,     4,     5,     6,     7,
       8,    -1,   112,    11,   114,    -1,   116,   117,   118,    -1,
      -1,    -1,   122,    -1,    -1,   125,    24,   127,    -1,   129,
      -1,    29,    30,    31,    -1,    33,    -1,    -1,    -1,    -1,
      38,    39,    -1,    41,    -1,    43,    44,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,     3,     4,     5,
       6,     7,     8,    -1,   112,    11,   114,    -1,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,    -1,   125,    24,   127,
      -1,   129,    -1,    29,    30,    31,    -1,    33,    -1,    -1,
      -1,    -1,    38,    39,    -1,    41,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    11,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,   112,    33,   114,    -1,
     116,   117,    38,    39,    -1,    41,   122,    43,    44,   125,
      -1,   127,    -1,   129,    50,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      96,    -1,    -1,    -1,   100,    22,    -1,    -1,   104,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      24,   127,   128,   129,    -1,    29,    30,    31,    -1,    33,
      57,    -1,    -1,    60,    38,    39,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    -1,    80,    -1,    -1,     3,    -1,    -1,    63,
      64,    65,    66,    10,    -1,    -1,    -1,    14,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    96,   120,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    84,    85,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,    -1,    -1,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
     117,    -1,    -1,   120,   121,   122,   123,   124,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,   123,   124,    -1,    -1,   127,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    80,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    57,
      -1,    -1,    60,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    80,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    -1,    57,    -1,    -1,    60,    95,   130,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    80,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    95,   130,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      10,   115,   116,   117,    14,    -1,   120,   121,   122,    -1,
      -1,    -1,    22,    23,   128,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    22,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    95,    60,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    10,   115,   116,   117,    14,    -1,
     120,   121,   122,    -1,    -1,   125,    22,    23,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    32,   101,   102,   103,
      -1,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
      -1,   115,    -1,   117,    -1,    -1,   120,   121,   122,    -1,
      -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,    -1,    57,   125,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    10,    -1,
      -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      -1,   120,   121,   122,    -1,    57,   125,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    57,   125,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,    -1,   120,   121,   122,    -1,    57,
     125,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    57,   125,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    -1,   120,
     121,   122,    -1,    57,   125,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    -1,   120,   121,   122,    -1,
      57,   125,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    -1,   120,   121,   122,    -1,    57,   125,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    -1,
     120,   121,   122,    -1,    57,   125,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    -1,   120,   121,   122,
      -1,    57,   125,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,    -1,    57,   125,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      -1,   120,   121,   122,    -1,    57,   125,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    57,   125,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,    -1,   120,   121,   122,    -1,    57,
     125,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    80,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    32,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,   124,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    10,    -1,
      80,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      32,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    -1,
     120,   121,   122,    -1,    -1,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    80,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    32,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    -1,   120,   121,   122,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,   119,
     120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    79,    80,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,    -1,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      -1,   120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,   119,   120,   121,
     122,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,   119,   120,   121,   122,    -1,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,   119,   120,   121,   122,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,   119,   120,   121,   122,    -1,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,   119,
     120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,   119,   120,   121,   122,    -1,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
     119,   120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,   119,   120,   121,
     122,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,   119,   120,   121,   122,    -1,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,   119,   120,   121,   122,    -1,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,   119,   120,
     121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,   119,   120,   121,   122,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    -1,   120,   121,   122,    -1,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    25,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,   119,
     120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    -1,   120,   121,   122,
      -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    79,    80,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,    -1,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    -1,   115,   116,   117,    -1,
      -1,   120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,    -1,   120,   121,   122,    -1,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    80,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    -1,   120,
     121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    -1,   120,   121,   122,    -1,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    -1,   120,   121,   122,    -1,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      80,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    -1,
     120,   121,   122,    -1,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    80,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    -1,   120,   121,   122,
      -1,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    80,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,    22,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    57,    -1,    -1,    60,    95,    -1,    97,    98,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,   108,
     109,   110,   111,   112,   113,    80,   115,   116,   117,    -1,
      -1,   120,   121,   122,    22,    -1,    57,    -1,    -1,    60,
      95,    -1,    97,    98,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,   108,    -1,   110,   111,   112,   113,    80,
     115,   116,   117,    -1,    -1,   120,   121,   122,    -1,    57,
      -1,    -1,    60,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,    -1,   110,
     111,   112,   113,    -1,   115,    -1,   117,    -1,    -1,   120,
     121,   122,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,   112,   113,    -1,    -1,    -1,   117,
      -1,    -1,   120,   121,   122
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   135,   136,     0,     1,     3,     4,     5,     6,     7,
       8,    11,    12,    13,    15,    16,    17,    18,    20,    21,
      23,    24,    26,    27,    28,    29,    30,    31,    33,    34,
      37,    38,    39,    40,    41,    43,    44,    46,    50,    51,
      52,    53,    55,    56,    59,    61,    62,    63,    64,    65,
      66,    68,    70,    71,    75,    77,    80,    95,    96,    97,
      98,   103,   112,   114,   116,   117,   119,   122,   125,   127,
     129,   137,   138,   139,   140,   141,   142,   144,   145,   147,
     150,   151,   152,   153,   154,   156,   157,   160,   161,   162,
     165,   168,   171,   172,   192,   195,   196,   214,   215,   216,
     217,   218,   219,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   235,   236,   237,   238,   239,   139,   227,
      76,   220,   221,   146,   147,   220,    11,    29,    30,    31,
      66,   125,   129,   168,   214,   218,   225,   226,   227,   228,
     230,   231,    68,   146,   227,   139,   127,   147,     7,   146,
     148,     9,    68,   148,    49,    78,   155,   227,   227,   227,
     127,   147,   169,   127,   147,   197,   198,   139,   227,   227,
     227,   227,     7,   127,    18,    28,   152,   152,   119,   227,
     227,    23,   142,   149,   227,   227,    65,   127,   139,   227,
      61,   147,   193,   118,   188,   208,   227,   227,   227,   227,
     227,   227,   227,   227,   126,   137,   143,   208,    69,   104,
     188,   209,   210,   227,   208,   227,   234,    51,   139,    42,
     147,    36,    54,   184,    19,    49,    58,    72,   119,   124,
      10,    14,    22,    57,    60,    80,    95,    97,    98,    99,
     101,   102,   103,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   115,   116,   117,   120,   121,   122,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   123,   127,   129,    57,    60,   127,   139,   119,
     125,   142,   227,   227,   227,   208,    32,   220,   193,   119,
     119,    75,   147,   188,   211,   212,   213,   227,   125,   193,
     162,   147,   127,   149,    23,    32,   149,    23,    32,   149,
     221,    67,   142,   211,   139,   127,   179,    69,   127,   147,
     199,   200,     1,    99,   202,   203,    32,   100,   149,   211,
     148,   119,   125,   139,   149,   127,   211,   208,    81,   194,
     119,   147,   100,   119,   149,   119,   126,   137,   126,   227,
     100,   128,   100,   128,    32,   130,   221,    79,   100,   130,
       7,   147,    99,   163,   173,    58,   197,   197,   197,   197,
     227,   227,   227,   227,   155,   227,   155,   227,   227,   227,
     227,   227,   227,   227,    24,    68,   147,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   211,   211,
     155,   227,   155,   227,    19,    32,    35,    48,    49,    58,
      68,   117,   182,   222,   223,   239,    23,    32,    23,    32,
      67,    32,   130,   155,   227,   149,   119,   227,    79,    81,
     128,   100,   147,   166,   167,   119,   147,    32,   208,    31,
     227,   155,   227,    31,   227,   155,   227,   149,   139,   227,
      25,   128,   180,   181,   182,   170,   200,   100,   128,     1,
     129,   204,   215,    79,    81,   201,   227,   198,   128,   158,
     211,   128,   125,   129,   204,   215,   100,   188,   227,   128,
     128,   209,   209,   155,   227,   139,   227,   130,   227,   227,
     125,   208,   125,    49,    58,    68,   175,   183,   197,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   128,   130,
      32,    58,   214,   100,   128,    57,    31,   155,   227,    31,
     155,   227,   155,   227,   220,   220,   119,   227,   188,   227,
     213,    81,   100,   126,   163,   227,   128,   227,    23,   149,
      23,   149,   227,    23,   149,   221,    23,   149,   221,    25,
      25,   139,   100,   128,   127,   147,    19,    49,    58,    68,
     185,   128,   200,    99,   203,   208,   227,    45,   227,    47,
      73,   126,   159,   128,   158,   208,   193,   130,   221,   130,
     221,   139,    79,   126,   143,   164,   174,    10,    14,    81,
      82,    83,    84,    85,    86,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,   101,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   117,   120,   121,
     122,   123,   124,   147,   176,   177,   119,   223,   214,   227,
      23,    23,   227,    23,    23,   130,   130,   149,   149,   227,
     167,   125,   149,    67,    31,   227,    31,   227,    67,    31,
     227,   149,    31,   227,   149,   139,   227,   181,   200,    99,
     207,   203,   201,    32,   130,   139,   208,   126,   130,    31,
     139,   227,   130,    31,   139,   227,   130,   227,   126,    28,
      53,    55,    68,   126,   138,   151,   185,   147,   102,   127,
     178,   178,    67,    31,    31,    67,    31,    31,    31,    31,
     164,   227,   227,   227,   227,   227,   227,   128,    24,    63,
      66,   129,   188,   206,   215,   104,   190,   201,    74,   191,
     227,   204,   215,   149,   227,   139,   227,   139,   151,   203,
     147,   176,   177,   180,   227,   227,   227,   227,   227,   227,
     126,    67,    67,    67,    67,   207,   130,   208,   188,   189,
     227,   227,   142,   150,   187,   130,    67,    67,   191,   178,
     178,   128,    67,    67,    67,    67,    67,    67,   227,   227,
     227,   227,   190,   201,   188,   205,   206,   215,    32,   130,
     215,   227,   227,   119,   186,   187,   227,   205,   206,   130,
     205
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
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 497 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 519 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 523 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 524 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 534 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 554 "chapel.ypp"
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
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 591 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 597 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 603 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 609 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 654 "chapel.ypp"
    { USR_FATAL((yyvsp[(3) - (6)].pcallexpr), "'type select' is no longer supported. Use 'select'"); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 666 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 673 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 678 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 683 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 706 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 716 "chapel.ypp"
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
#line 725 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 729 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 742 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 747 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
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
#line 772 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 778 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 785 "chapel.ypp"
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
#line 793 "chapel.ypp"
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
#line 804 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 810 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 817 "chapel.ypp"
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
#line 839 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 843 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 848 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 852 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 925 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 962 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 984 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 985 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1004 "chapel.ypp"
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
#line 1017 "chapel.ypp"
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
#line 1033 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1037 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1056 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1063 "chapel.ypp"
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
#line 1071 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1097 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1103 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1112 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1135 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1141 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1142 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1163 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1167 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1171 "chapel.ypp"
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
#line 1178 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1183 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1201 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1226 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1230 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1318 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1324 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1330 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1336 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1345 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1374 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1389 "chapel.ypp"
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
#line 1398 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1404 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1440 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1464 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1470 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1480 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1493 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1499 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1500 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7524 "bison-chapel.cpp"
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



