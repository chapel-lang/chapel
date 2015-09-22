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
#define YYLAST   10642

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  428
/* YYNRULES -- Number of states.  */
#define YYNSTATES  817

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
    1293,  1297,  1301,  1305,  1309,  1315,  1319,  1323,  1327,  1331,
    1335,  1339,  1343,  1347,  1351,  1355,  1359,  1363,  1367,  1371,
    1375,  1379,  1383,  1387,  1391,  1395,  1399,  1403,  1407,  1410,
    1413,  1416,  1419,  1422,  1425,  1429,  1433,  1437,  1441,  1445,
    1449,  1453,  1457,  1459,  1461,  1463,  1465,  1467,  1469
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     135,     0,    -1,   136,    -1,    -1,   136,   137,    -1,   139,
      -1,   138,   139,    -1,    50,     7,    -1,   138,    50,     7,
      -1,   140,    -1,   142,    -1,   144,    -1,   145,    -1,   152,
      -1,   146,    -1,   154,    -1,   157,    -1,   155,    -1,   158,
      -1,   151,    -1,   219,   119,    -1,    10,   139,    -1,    11,
     220,   139,    -1,    12,   147,   119,    -1,    15,   220,   142,
      -1,    19,   147,   119,    -1,    20,   227,   119,    -1,    36,
     148,   139,    -1,    39,   139,    -1,    45,   227,   150,    -1,
      62,   227,   150,    -1,    62,   150,    -1,    66,   139,    -1,
      77,   227,   119,    -1,     1,    -1,   141,    41,   148,   125,
     126,    -1,   141,    41,   148,   125,   143,   126,    -1,    -1,
      54,    -1,    52,    -1,   125,   126,    -1,   125,   143,   126,
      -1,   137,    -1,   143,   137,    -1,    71,   208,   119,    -1,
      58,   208,   119,    -1,   228,    81,   227,   119,    -1,   228,
      92,   227,   119,    -1,   228,    89,   227,   119,    -1,   228,
      91,   227,   119,    -1,   228,    85,   227,   119,    -1,   228,
      90,   227,   119,    -1,   228,    86,   227,   119,    -1,   228,
      82,   227,   119,    -1,   228,    83,   227,   119,    -1,   228,
      84,   227,   119,    -1,   228,    94,   227,   119,    -1,   228,
      93,   227,   119,    -1,   228,   123,   227,   119,    -1,   228,
      87,   227,   119,    -1,   228,    88,   227,   119,    -1,    -1,
     148,    -1,     3,    -1,   147,    -1,     7,    -1,    22,   139,
      -1,   142,    -1,    59,   119,    -1,    59,   227,   119,    -1,
     119,    -1,   153,    -1,    54,   153,    -1,    52,   153,    -1,
     165,    -1,   192,    -1,   161,    -1,   172,    -1,   195,    -1,
      27,     8,    -1,    22,   139,    75,   227,   119,    -1,    75,
     227,   150,    -1,    16,   227,    31,   227,   220,   150,    -1,
      16,   227,    31,   156,   220,   150,    -1,    16,   227,   220,
     150,    -1,    28,   227,    31,   227,   150,    -1,    28,   227,
      31,   156,   150,    -1,    28,   227,   150,    -1,    28,   156,
     150,    -1,    28,    48,   148,    31,   227,   150,    -1,    29,
     227,    31,   227,   150,    -1,    29,   227,    31,   227,   221,
     150,    -1,    29,   227,    31,   156,   150,    -1,    29,   227,
      31,   156,   221,   150,    -1,    29,   227,   150,    -1,    29,
     227,   221,   150,    -1,   129,   208,    31,   227,   130,   139,
      -1,   129,   208,    31,   227,   221,   130,   139,    -1,   129,
     208,    31,   156,   130,   139,    -1,   129,   208,    31,   156,
     221,   130,   139,    -1,   129,   208,   130,   139,    -1,   129,
     208,   221,   130,   139,    -1,    78,   127,   208,   128,    -1,
      30,   227,    67,   139,    -1,    30,   227,   142,    -1,    30,
     227,    67,   139,    24,   139,    -1,    30,   227,   142,    24,
     139,    -1,    61,   227,   125,   159,   126,    -1,    -1,   159,
     160,    -1,    73,   208,   150,    -1,    46,   139,    -1,   162,
     148,   163,   125,   164,   126,    -1,    27,   149,   162,   148,
     163,   125,   164,   126,    -1,    14,    -1,    55,    -1,    70,
      -1,    -1,    99,   208,    -1,    -1,   164,   152,    -1,   164,
     138,   152,    -1,    25,   148,   125,   166,   126,    -1,   167,
      -1,   166,   100,    -1,   166,   100,   167,    -1,   148,    -1,
     148,    81,   227,    -1,    -1,    -1,    37,   169,   179,   170,
     185,   203,   191,   187,    -1,    -1,    33,    -1,    26,   149,
      -1,    27,   149,    -1,    -1,    -1,   171,   184,   173,   175,
     174,   185,   203,   191,   186,    -1,   183,   176,   178,    -1,
     183,   177,   178,    -1,   183,   148,   102,   176,   178,    -1,
     183,   148,   102,   177,   178,    -1,   148,    -1,    96,   148,
      -1,    95,    -1,    97,    -1,    98,    -1,    96,    -1,   105,
      -1,   115,    -1,   111,    -1,   108,    -1,   110,    -1,   107,
      -1,   117,    -1,   112,    -1,   122,    -1,   101,    -1,   120,
      -1,   121,    -1,   113,    -1,   106,    -1,   114,    -1,    13,
      -1,   109,    -1,     9,    -1,   123,    -1,   124,    -1,    81,
      -1,    92,    -1,    89,    -1,    91,    -1,    85,    -1,    90,
      -1,    86,    -1,    82,    -1,    83,    -1,    84,    -1,    94,
      -1,    93,    -1,    -1,   127,   180,   128,    -1,   127,   180,
     128,    -1,    -1,   181,    -1,   180,   100,   181,    -1,   182,
     148,   207,   201,    -1,   182,   148,   207,   190,    -1,   182,
     127,   200,   128,   207,   201,    -1,   182,   127,   200,   128,
     207,   190,    -1,    -1,    31,    -1,    34,    -1,    47,    -1,
      18,    -1,    18,    31,    -1,    18,    57,    -1,    48,    -1,
      57,    -1,    68,    -1,    -1,    48,    -1,    57,    -1,    68,
      -1,    53,    -1,    35,    -1,    -1,    18,    -1,    57,    -1,
      48,    -1,    68,    -1,   119,    -1,   187,    -1,   142,    -1,
     151,    -1,   118,   148,    -1,   118,    -1,    -1,   188,    -1,
     104,   227,    -1,   104,   189,    -1,    -1,    74,   227,    -1,
      68,   193,   119,    -1,    17,    68,   193,   119,    -1,    27,
      68,   193,   119,    -1,   148,   194,    -1,   148,   194,   100,
     193,    -1,    -1,    81,   215,    -1,    81,   204,    -1,   196,
      48,   197,   119,    -1,   196,    18,   197,   119,    -1,   196,
      57,   197,   119,    -1,   196,    18,    57,   197,   119,    -1,
     196,    72,   197,   119,    -1,    -1,    17,    -1,    27,    -1,
     198,    -1,   197,   100,   198,    -1,   148,   203,   201,    -1,
     148,   202,    79,   227,    -1,   127,   200,   128,   203,   201,
      -1,    69,    -1,   148,    -1,   127,   200,   128,    -1,   199,
      -1,   199,   100,    -1,   199,   100,   200,    -1,    -1,    81,
      44,    -1,    81,   227,    -1,    -1,    99,   129,   208,   130,
      -1,    -1,    99,   215,    -1,    99,   204,    -1,     1,    -1,
     129,   208,   130,   215,    -1,   129,   208,   130,   204,    -1,
     129,   208,    31,   227,   130,   215,    -1,     1,    -1,    -1,
     215,    -1,   188,    -1,   129,   130,   205,    -1,   129,   208,
     130,   205,    -1,   129,   130,   206,    -1,   129,   208,   130,
     206,    -1,   129,   208,    31,   227,   130,   205,    -1,    -1,
      99,   215,    -1,    99,   188,    -1,    99,    23,    -1,    99,
      63,    -1,    99,    66,    -1,    99,   206,    -1,   227,    -1,
     188,    -1,   208,   100,   227,    -1,   208,   100,   188,    -1,
      69,    -1,   227,    -1,   188,    -1,   209,   100,   209,    -1,
     210,   100,   209,    -1,    -1,   212,    -1,   213,    -1,   212,
     100,   213,    -1,   148,    81,   188,    -1,   148,    81,   227,
      -1,   148,    79,   227,    -1,   188,    -1,   227,    -1,   148,
      -1,   218,    -1,   228,    -1,   217,    -1,   236,    -1,   235,
      -1,    63,   227,    -1,    32,   127,   211,   128,    -1,    23,
     127,   211,   128,    -1,    65,   127,   211,   128,    -1,    64,
      65,   127,   211,   128,    -1,    10,   227,    -1,    66,   227,
      -1,    28,   227,    31,   227,    22,   227,    -1,    28,   227,
      31,   156,    22,   227,    -1,    28,   227,    22,   227,    -1,
      28,   227,    31,   227,    22,    30,   227,    67,   227,    -1,
      28,   227,    31,   156,    22,    30,   227,    67,   227,    -1,
      28,   227,    22,    30,   227,    67,   227,    -1,    29,   227,
      31,   227,    22,   227,    -1,    29,   227,    31,   156,    22,
     227,    -1,    29,   227,    22,   227,    -1,    29,   227,    31,
     227,    22,    30,   227,    67,   227,    -1,    29,   227,    31,
     156,    22,    30,   227,    67,   227,    -1,    29,   227,    22,
      30,   227,    67,   227,    -1,   129,   208,   130,   227,    -1,
     129,   208,    31,   227,   130,   227,    -1,   129,   208,    31,
     156,   130,   227,    -1,   129,   208,    31,   227,   130,    30,
     227,    67,   227,    -1,   129,   208,    31,   156,   130,    30,
     227,    67,   227,    -1,    30,   227,    67,   227,    24,   227,
      -1,    43,    -1,   218,    -1,   214,    -1,   231,    -1,   230,
      -1,   168,    -1,   225,    -1,   226,    -1,   224,   124,   227,
      -1,    -1,   221,    -1,    76,   127,   222,   128,    -1,   223,
      -1,   222,   100,   223,    -1,   182,   214,    -1,   239,    56,
     214,    -1,   228,    -1,   224,   124,   227,    -1,    42,   227,
      -1,    38,   197,    31,   227,    -1,   233,    -1,   215,    -1,
     216,    -1,   237,    -1,   238,    -1,   168,    -1,   225,    -1,
     226,    -1,   127,   104,   227,   128,    -1,   227,    99,   227,
      -1,   227,   103,   227,    -1,   227,   103,    -1,   103,   227,
      -1,   103,    -1,   214,    -1,   230,    -1,   231,    -1,   232,
      -1,   228,    -1,   168,    -1,   229,   127,   211,   128,    -1,
     229,   129,   211,   130,    -1,    51,   127,   211,   128,    -1,
     227,   102,   148,    -1,   227,   102,    68,    -1,   227,   102,
      23,    -1,   127,   209,   128,    -1,   127,   209,   100,   128,
      -1,   127,   210,   128,    -1,     4,    -1,     5,    -1,     6,
      -1,     7,    -1,   125,   208,   126,    -1,   129,   208,   130,
      -1,   129,   234,   130,    -1,   227,    79,   227,    -1,   234,
     100,   227,    79,   227,    -1,   227,   117,   227,    -1,   227,
     112,   227,    -1,   227,   122,   227,    -1,   227,   101,   227,
      -1,   227,   120,   227,    -1,   227,   121,   227,    -1,   227,
     113,   227,    -1,   227,   105,   227,    -1,   227,   115,   227,
      -1,   227,   111,   227,    -1,   227,   108,   227,    -1,   227,
     110,   227,    -1,   227,   107,   227,    -1,   227,    95,   227,
      -1,   227,    97,   227,    -1,   227,    98,   227,    -1,   227,
      80,   227,    -1,   227,   116,   227,    -1,   227,   106,   227,
      -1,   227,    13,   227,    -1,   227,     9,   227,    -1,   227,
     109,   227,    -1,   227,    21,   227,    -1,   117,   227,    -1,
     112,   227,    -1,    40,   227,    -1,    49,   227,    -1,   114,
     227,    -1,    96,   227,    -1,   227,    56,   227,    -1,   227,
      56,   156,    -1,   239,    56,   227,    -1,   239,    56,   156,
      -1,   227,    60,   227,    -1,   227,    60,   156,    -1,   239,
      60,   227,    -1,   239,    60,   156,    -1,   117,    -1,   122,
      -1,    80,    -1,   116,    -1,    95,    -1,    97,    -1,    98,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   402,   402,   407,   408,   414,   415,   420,   421,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   455,   457,   462,   463,   464,
     479,   480,   485,   486,   491,   495,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   517,   518,   522,   526,   527,   531,   532,   536,   537,
     541,   542,   543,   544,   545,   546,   547,   551,   552,   556,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   593,   599,   605,
     611,   618,   628,   632,   633,   634,   635,   639,   644,   645,
     649,   651,   658,   663,   671,   676,   681,   689,   690,   695,
     696,   698,   703,   713,   722,   726,   734,   735,   740,   745,
     739,   770,   776,   783,   791,   802,   808,   801,   836,   840,
     845,   849,   857,   858,   859,   860,   861,   862,   863,   864,
     865,   866,   867,   868,   869,   870,   871,   872,   873,   874,
     875,   876,   877,   878,   879,   880,   881,   882,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     901,   902,   906,   910,   911,   912,   916,   918,   920,   922,
     927,   928,   929,   930,   931,   932,   933,   934,   935,   936,
     940,   941,   942,   943,   947,   948,   952,   953,   954,   955,
     956,   960,   961,   965,   966,   970,   972,   977,   978,   982,
     983,   987,   988,   992,   994,   996,  1001,  1014,  1031,  1032,
    1034,  1039,  1046,  1053,  1060,  1068,  1077,  1078,  1079,  1083,
    1084,  1092,  1094,  1100,  1105,  1107,  1109,  1114,  1116,  1118,
    1125,  1126,  1127,  1131,  1132,  1137,  1138,  1139,  1140,  1160,
    1164,  1168,  1176,  1180,  1181,  1182,  1186,  1188,  1194,  1196,
    1198,  1203,  1204,  1205,  1206,  1207,  1208,  1209,  1215,  1216,
    1217,  1218,  1222,  1223,  1224,  1228,  1229,  1233,  1234,  1238,
    1239,  1243,  1244,  1245,  1246,  1247,  1251,  1262,  1263,  1264,
    1265,  1266,  1267,  1269,  1271,  1273,  1275,  1277,  1279,  1284,
    1286,  1288,  1290,  1292,  1294,  1296,  1298,  1300,  1302,  1304,
    1306,  1308,  1315,  1321,  1327,  1333,  1342,  1347,  1355,  1356,
    1357,  1358,  1359,  1360,  1361,  1362,  1367,  1368,  1372,  1376,
    1378,  1386,  1396,  1400,  1401,  1406,  1411,  1419,  1420,  1421,
    1422,  1423,  1424,  1425,  1426,  1427,  1429,  1431,  1433,  1435,
    1437,  1442,  1443,  1444,  1445,  1456,  1457,  1461,  1462,  1463,
    1467,  1468,  1469,  1477,  1478,  1479,  1483,  1484,  1485,  1486,
    1487,  1488,  1489,  1496,  1497,  1501,  1502,  1503,  1504,  1505,
    1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,
    1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1527,  1528,
    1529,  1530,  1531,  1532,  1536,  1537,  1538,  1539,  1543,  1544,
    1545,  1546,  1551,  1552,  1553,  1554,  1555,  1556,  1557
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
  "if_stmt", "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
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
     142,   142,   143,   143,   144,   145,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   147,   147,   148,   149,   149,   150,   150,   151,   151,
     152,   152,   152,   152,   152,   152,   152,   153,   153,   154,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   156,   157,   157,   157,   157,   158,   159,   159,
     160,   160,   161,   161,   162,   162,   162,   163,   163,   164,
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
     233,   233,   233,   234,   234,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   236,   236,
     236,   236,   236,   236,   237,   237,   237,   237,   238,   238,
     238,   238,   239,   239,   239,   239,   239,   239,   239
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
       3,     0,     0,     1,    34,    63,   376,   377,   378,   379,
       0,   336,    61,   114,   336,     0,   237,    61,     0,     0,
       0,     0,    61,    61,     0,     0,     0,     0,   132,     0,
     128,     0,     0,     0,     0,   327,     0,     0,     0,     0,
     236,   236,   115,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,     0,     0,     0,   424,   426,     0,   427,
     428,   360,     0,     0,   425,   422,    70,   423,     0,     0,
       0,     4,     0,     5,     9,     0,    10,    11,    12,    14,
     296,    19,    13,    71,    15,    17,    16,    18,    76,     0,
      74,   352,     0,    77,    75,    78,     0,   361,   348,   349,
     299,   297,     0,     0,   353,   354,     0,   298,     0,   362,
     363,   364,   347,   301,   300,   350,   351,     0,    21,   307,
       0,     0,   337,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,   352,   361,   297,   353,   354,   336,   298,
     362,   363,     0,     0,     0,     0,   287,     0,    65,    64,
     133,    79,     0,   134,     0,     0,     0,     0,     0,     0,
     287,     0,     0,     0,     0,     0,   239,    28,   410,   345,
       0,   411,     7,   287,   237,   238,    73,    72,   216,   279,
       0,   278,    68,     0,     0,     0,    67,    31,     0,   302,
       0,   287,    32,   308,   228,     0,     0,     0,     0,   413,
     359,   409,   412,   408,    40,    42,     0,     0,   282,     0,
     284,     0,     0,   283,     0,   278,     0,     0,     6,     0,
     117,   205,   204,   135,     0,     0,     0,     0,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   358,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   287,   287,     0,     0,   190,    22,    23,
       0,    24,     0,     0,     0,     0,     0,     0,     0,    25,
      26,     0,   296,   294,     0,   288,   289,   295,     0,     0,
       0,     0,     0,    88,     0,     0,    87,     0,     0,    94,
       0,     0,   104,     0,    27,   183,   129,   244,     0,   245,
     247,     0,   258,     0,     0,   250,     0,     0,    29,     0,
     134,   215,     0,    45,    69,   108,    66,    30,   287,     0,
       0,   226,   223,    44,    81,    33,    41,    43,   380,     0,
       0,   373,     0,   375,     0,     0,     0,     0,     0,   382,
       8,     0,     0,     0,   200,     0,     0,     0,     0,     0,
     335,   405,   404,   407,   415,   414,   419,   418,   401,   398,
     399,   400,   356,   388,   372,   371,   370,   357,   392,   403,
     397,   395,   406,   396,   394,   386,   391,   393,   402,   385,
     389,   390,   387,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     417,   416,   421,   420,   194,   191,   192,   193,   197,   198,
     199,   422,     0,     0,   339,     0,     0,     0,     0,     0,
       0,     0,   381,   336,   336,    84,   224,     0,     0,     0,
     304,     0,   126,     0,   123,   225,   117,     0,     0,     0,
     311,     0,     0,     0,   317,     0,     0,    95,   103,     0,
       0,   303,     0,   184,     0,   206,     0,   248,     0,   262,
       0,   257,   348,     0,     0,   241,   346,   240,   369,   281,
     280,     0,     0,   305,     0,   230,   348,     0,   355,   374,
     285,   286,     0,     0,   100,   321,     0,   383,     0,     0,
     118,   119,   201,   202,   203,   136,     0,     0,   232,   231,
     233,   235,    46,    53,    54,    55,    50,    52,    59,    60,
      48,    51,    49,    47,    57,    56,    58,   367,   368,   195,
     196,   341,   190,   338,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    80,   293,   291,   292,   290,
       0,   124,   122,     0,     0,   102,     0,     0,    86,     0,
      85,     0,     0,    92,     0,     0,    90,     0,     0,     0,
     106,   190,   182,     0,   271,   207,   209,   208,   210,     0,
     246,   249,     0,   250,     0,   242,   251,   252,     0,     0,
     107,   109,   306,     0,   227,     0,     0,     0,     0,   101,
       0,    35,     0,   236,   206,   165,   163,   168,   175,   176,
     177,   172,   174,   170,   173,   171,   169,   179,   178,   144,
     147,   145,   146,   157,   148,   161,   153,   151,   164,   152,
     150,   155,   160,   162,   149,   154,   158,   159,   156,   166,
     167,   142,   180,   180,   234,   340,   342,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    82,   127,   125,   119,
      89,     0,     0,   310,     0,   309,     0,     0,   316,    93,
       0,   315,    91,   105,   326,   185,     0,     0,   250,   221,
     243,     0,     0,   111,     0,     0,     0,    98,   323,     0,
       0,    96,   322,     0,   384,    36,    61,   236,   236,   112,
     236,   120,     0,   143,     0,   183,   138,   139,     0,     0,
       0,     0,     0,     0,     0,     0,   236,   314,     0,     0,
     320,     0,     0,   271,   274,   275,   276,     0,   273,   277,
     348,   217,   187,   186,     0,     0,     0,   260,   348,   110,
       0,    99,     0,    97,   121,   221,   142,   180,   180,     0,
       0,     0,     0,     0,     0,     0,   113,     0,     0,     0,
       0,   250,   263,     0,   218,   220,   219,   222,   213,   214,
     130,     0,     0,     0,     0,   140,   141,   181,     0,     0,
       0,     0,     0,     0,   313,   312,   319,   318,   189,   188,
     265,   266,   268,   348,     0,   381,   348,   325,   324,   211,
     137,   212,     0,   267,   269,   263,   270
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   205,    72,   468,    74,    75,    76,   206,
      77,    78,    79,   149,    80,   153,   187,    81,    82,    83,
      84,    85,   551,    86,    87,   491,   601,    88,    89,   363,
     613,    90,   453,   454,   133,   162,   475,    92,    93,   364,
     614,   515,   652,   653,   716,   316,   472,   473,   474,   516,
     223,   589,   810,   780,   179,   775,   742,   745,    94,   195,
     341,    95,    96,   165,   166,   320,   321,   485,   324,   325,
     481,   801,   739,   688,   207,   211,   212,   294,   295,   296,
     134,    98,    99,   100,   135,   102,   121,   122,   433,   434,
     103,   136,   137,   106,   139,   108,   140,   141,   111,   112,
     216,   113,   114,   115,   116,   117
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -726
static const yytype_int16 yypact[] =
{
    -726,    32,  2162,  -726,  -726,  -726,  -726,  -726,  -726,  -726,
    3343,    14,    65,  -726,    14,  6118,    38,    65,  6118,  3343,
     -30,    65,   304,   317,  5130,  6118,  6118,     6,  -726,    65,
    -726,    17,  3343,  6118,  6118,  -726,  6118,  6118,   157,    83,
     493,   808,  -726,  5314,  5417,  6118,  5544,  6118,   136,   147,
    3343,    65,  -726,  5314,  6118,  6118,  -726,  -726,  6118,  -726,
    -726,  6847,  6118,  6118,  -726,  6118,  -726,  -726,  2581,  4924,
    5314,  -726,  3216,  -726,  -726,   248,  -726,  -726,  -726,  -726,
    -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,    65,
    -726,   177,   133,  -726,  -726,  -726,   269,   190,  -726,  -726,
    -726,   227,   274,   188,   275,   278, 10452,  6914,   328,   305,
     321,  -726,  -726,  -726,  -726,  -726,  -726,   326,  -726, 10452,
     280,  3343,  -726,   325,  -726,   294,  6118,  6118,  6118,  6118,
    6118,  5314,  5314,   336,  -726,  -726,  -726,  -726,  8230,   348,
    -726,  -726,    65,   332,  8420,   377,  5314,   298,  -726,  -726,
    -726,  -726,    65,   127,    65,   337,    50,  7278,  7234,  7416,
    5314,  3343,   341,    22,    48,    61,  -726,  -726,    78,   369,
    7479,    78,  -726,  5314,  -726,   283,  -726,  -726,    65,  -726,
      69, 10452,  -726,  8478,  8109,  3343,  -726,  -726,  7479, 10452,
     345,  5314,  -726, 10452,   319,   361,   169,  7479,  8536,   477,
    1282,    78,   477,    78,  -726,  -726,  2708,   -43,  -726,  6118,
    -726,   -19,    70, 10452,   251,  8594,   -23,   474,  -726,    65,
     390,  -726,  -726,  -726,    19,    17,    17,    17,  -726,  6118,
    6118,  6118,  6118,  5625,  5625,  6118,  6118,  6118,  6118,  6118,
    6118,   122,  6847,  6118,  6118,  6118,  6118,  6118,  6118,  6118,
    6118,  6118,  6118,  6118,  6118,  6118,  6118,  6118,  6118,  6118,
    6118,  6118,  6118,  6118,  6118,  6118,  6118,  6118,  6118,  6118,
    6118,  6118,  6118,  5314,  5314,  5625,  5625,   706,  -726,  -726,
    2835,  -726,  8299,  8362,  8657,    10,  5625,    50,   371,  -726,
    -726,  6118,   339,  -726,   363,   392,  -726, 10452,    65,   374,
      65,   463,  5314,  -726,  3470,  5625,  -726,  3597,  5625,  -726,
      50,  3343,   471,   368,  -726,    67,  -726,  -726,    22,  -726,
     399,   372,  -726,  2295,   422,   421,  6118,    17,  -726,   375,
    -726,  -726,  5314,  -726,  -726,  -726,  -726,  -726,  5314,   380,
    4740,   409,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  7129,
    5027,  -726,  5211,  -726,  5625,  2454,   382,  6118,  6118,  -726,
    -726,   388,  5314,   389,   330,    17,   260,   261,   290,   291,
    8167,  1437,  1437,   344,  -726,   344,  -726,   344, 10486,   518,
    1589,  1736,   369,   477,  -726,  -726,  -726,  1282, 10520,   344,
     866,   866,  1437,   866,   866,  1312,   477, 10520, 10423,  1312,
      78,    78,   477,  8720,  8778,  8836,  8894,  8952,  9010,  9068,
    9126,  9184,  9242,  9300,  9358,  9416,  9474,  9532,   394,   400,
    -726,   344,  -726,   344,   272,  -726,  -726,  -726,  -726,  -726,
    -726,  -726,    65,    89,  -726,   462,  6199,  5625,  6280,  5625,
    6118,  5625,  6928,    14,  9590,  -726,  -726,  9648,  6118,  5314,
    -726,  5314,   448,   258,  -726,  -726,   390,  6118,   171,  6118,
   10452,    51,  7542,  6118, 10452,    47,  7348,  -726,   512,  9706,
    3343,  -726,   193,  -726,    25,   266,   419,    22,   112,  -726,
    5314,  -726,   386,  6118,  5728,  -726, 10452,  -726,  -726,  -726,
   10452,   141,   420,  -726,  5314,  -726,   295,    65,  -726,  -726,
    -726,  -726,     2,  1617,  -726, 10452,  3343, 10452,  9764,  2962,
     449,  -726,  -726,  -726,  -726,  -726,  2028,   306,  -726,  -726,
    -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,
    -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,
    -726,  -726,   706,  -726,    65,  6118,   529,  9827,  6118,   530,
    9890,   423,  6969,    50,    50,  -726, 10452,  -726, 10452,  -726,
    6118,    65,  -726,   431,  7479,  -726,  7605,  3724,  -726,  3851,
    -726,  7668,  3978,  -726,    50,  4105,  -726,    50,  3343,  6118,
    -726,   365,  -726,    22,   464,  -726,  -726,  -726,  -726,    29,
    -726,  -726,  4740,   421,    73, 10452,  -726, 10452,  3343,  5314,
    -726,  -726,  -726,    74,  -726,  4232,   434,  4359,   441,  -726,
    6118,  -726,  3089,   490,   266,  -726,  -726,  -726,  -726,  -726,
    -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,
      65,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,
    -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,  -726,
    -726,   473,   446,   446,  -726,  -726,  -726,  9948,  6361,  6442,
   10011,  6523,  6604,  6685,  6766,  -726,  -726, 10452,  -726,  -726,
    -726,  3343,  6118, 10452,  6118, 10452,  3343,  6118, 10452,  -726,
    6118, 10452,  -726,  -726, 10452,  -726,   453,  5809,   102,   508,
    -726,  5625,  4486,  -726,    60,  4613,  6118,  -726, 10452,  3343,
    6118,  -726, 10452,  3343, 10452,  -726,   627,   868,   868,  -726,
     991,  -726,    12,  -726,  2028,    67,  -726,  -726,  6118,  6118,
    6118,  6118,  6118,  6118,  6118,  6118,   883,  9706,  7731,  7794,
    9706,  7857,  7920,   464,   -30,  6118,  6118,  4821,  -726,  -726,
     273,  5314,  -726,  -726,  6118,   143,  7013,  -726,   496,  -726,
    7983,  -726,  8046,  -726,  -726,   508,  -726,   446,   446,   205,
   10074, 10137, 10200, 10263, 10326, 10389,  -726,  3343,  3343,  3343,
    3343,   102,  5912,   185,  -726,  -726, 10452, 10452,  -726,  -726,
    -726,  6766,  3343,  3343,    37,  -726,  -726,  -726,  6118,  6118,
    6118,  6118,  6118,  6118,  9706,  9706,  9706,  9706,  -726,  -726,
    -726,  -726,  -726,   288,  5625,  1938,   597,  9706,  9706,  -726,
    -726,  -726,  7085,  -726,  -726,  6015,  -726
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -726,  -726,  -726,     1,  -584,  1801,  -726,  -726,  1014,    75,
    -726,  -726,  -726,   201,  1732,    -4,   -95,  -695,  -571,   -39,
    -726,  -726,   129,  -726,  -726,  -726,  -726,  -726,   433,   131,
     -78,  -726,  -726,    40,   281,  -726,  -726,  -726,  -726,  -726,
    -726,  -726,  -121,  -116,  -599,  -726,  -113,    24,  -267,  -726,
    -726,   -11,  -726,  -178,     5,  -726,  -163,  -145,  -726,  -133,
    -726,  -726,  -726,  -160,   285,  -726,  -311,  -576,  -726,  -432,
    -336,  -721,  -725,  -119,   -32,   126,  -726,   -65,  -726,   167,
     300,  -284,  -726,  -726,   522,  -726,    -8,  -127,  -726,    80,
    -726,   670,   852,   -10,  1004,  -726,  1094,  1321,  -726,  -726,
    -726,  -726,  -726,  -726,  -726,  -265
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -382
static const yytype_int16 yytable[] =
{
     119,   176,   177,    71,   495,   138,   125,   476,   144,   288,
     432,   180,   435,   322,   157,   158,   159,   690,   150,   299,
       5,   196,     5,   168,   169,     5,   170,   171,     5,   710,
     322,   310,     3,   181,   183,   184,   188,   189,   214,   482,
     193,   441,   711,   181,   197,   198,   593,   802,   199,   322,
     779,   200,   201,   202,   717,   203,   496,   332,   181,   213,
     215,   303,   306,   309,   366,   367,   368,   369,     5,   572,
    -190,  -255,   185,   567,   210,   328,   365,   358,   120,  -255,
     814,   350,   185,   348,   813,   424,  -255,   356,  -255,   779,
     120,   317,   326,   337,   816,   313,    44,   146,   425,   232,
     285,   426,   344,  -255,   691,   691,   142,   359,   329,   351,
     332,   592,   743,   322,   427,   428,   119,   282,   283,   284,
     193,   181,   215,   120,   429,     5,   339,  -253,   592,  -255,
     287,  -255,   605,   160,   233,   430,   297,  -255,   234,   754,
     442,    13,   710,  -255,   163,   384,   163,   323,  -255,   318,
     297,   293,   583,   156,  -255,   711,   809,   689,   785,   786,
     332,   327,   280,   297,   172,   293,   591,  -255,   221,   332,
     352,   330,   280,   332,   332,   280,   280,   239,   293,   240,
     241,   297,    42,   484,   244,   280,   222,   598,   333,   542,
     385,   251,   445,  -255,  -190,   799,   293,    52,   353,   349,
     257,   190,    44,   692,   695,   517,   741,   347,   418,   419,
     173,   592,  -255,   123,   599,   467,   804,   543,   143,   370,
     371,   372,   373,   375,   377,   378,   379,   380,   381,   382,
     383,  -255,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   297,   297,   421,   423,   600,   280,   332,
     458,   332,   686,   492,   191,   432,   444,   435,   293,   293,
     755,   447,   354,    91,   585,   332,     5,   224,   343,   219,
     148,    91,   181,   581,   460,   462,  -332,   464,   466,   565,
      91,   469,    97,   539,  -366,   581,  -366,     5,   482,  -329,
      97,   148,   229,    91,   586,   805,   486,   225,   -61,    97,
       5,   582,   490,   587,   148,   151,   226,   120,   297,   540,
     510,    91,    97,   787,   588,  -238,   -61,   489,   574,   577,
     213,   227,   213,   293,   503,   505,  -328,   507,   508,    91,
      97,   332,   181,    91,  -272,   210,   747,   210,   561,   747,
     327,   327,   374,   376,   604,  -238,   568,   570,    97,  -264,
     573,   576,    97,  -272,  -238,   606,   608,  -272,   512,   518,
     519,   355,   275,   424,   562,   152,   276,   513,  -264,  -238,
     327,   327,  -264,   228,  -333,  -229,   425,  -334,   514,   426,
     340,  -272,    91,   740,   420,   422,   327,   277,   748,   520,
     521,   748,   427,   428,  -229,   443,  -264,  -256,   448,   280,
     449,    97,   429,   298,  -331,   654,   460,   547,   464,   550,
     469,   552,   505,   430,   461,   553,   554,   465,   556,   558,
    -330,   297,    91,   239,   279,  -256,   241,   564,   594,   566,
     244,   289,   291,   571,   557,   273,   293,   274,   665,   666,
    -256,    97,   603,  -366,   302,  -366,    91,  -256,   315,   670,
     215,   241,   338,   595,   597,  -365,   500,  -365,   501,   679,
     342,   360,   682,   502,   215,    97,  -256,    91,   803,   362,
     446,   450,   451,   455,   457,   470,   471,   806,   232,   477,
     478,   483,   484,   488,    13,  -256,    97,    16,   493,   497,
     174,  -256,   506,   509,   511,    21,    22,   706,   544,    22,
     175,   803,   537,    28,   101,  -131,    28,  -259,  -131,   560,
     538,   803,   101,   233,   -39,   657,   578,   234,   660,   232,
      38,   101,   707,  -131,   708,    42,  -131,   590,   602,   332,
     667,   658,   661,   663,   101,  -259,   669,   673,    51,   675,
      52,    91,   678,   687,   699,   681,   546,   694,   549,   684,
    -259,   703,   101,   715,   233,   714,   239,  -259,   234,   241,
      97,   733,   744,   244,   612,    91,   300,   563,    91,   181,
     101,   726,    91,   757,   101,   698,  -259,   702,   758,   749,
     704,   668,   759,   712,    97,   685,   811,    97,   798,    66,
     784,    97,   487,   347,   771,  -259,   709,   239,   559,   240,
     241,  -259,   655,     0,   244,     0,     0,     0,  -261,     0,
       5,   251,     0,     0,   148,     0,    91,     0,   255,   256,
     257,     0,     0,   101,     0,  -238,     0,     0,   673,   675,
       0,   678,   681,   698,   702,    97,  -261,     0,     0,     0,
       0,   727,   728,     0,   729,     0,   730,   731,   176,   177,
     732,  -261,   104,     0,     0,  -238,     0,     0,  -261,     0,
     104,   746,   505,   101,  -238,   505,   750,     0,     0,   104,
     752,     0,   738,     0,     0,   152,     0,  -261,     0,  -238,
       0,     0,   104,     0,     0,   773,     0,   101,   727,   760,
     761,   730,   762,   763,   764,   765,  -261,     0,     0,     0,
     104,     0,  -261,     0,   424,   189,   193,   215,   101,     0,
       0,   776,   541,     0,   777,     0,     0,   425,   104,     0,
     426,     0,   104,     0,     0,     0,   774,     0,     0,     0,
       0,    91,     0,   427,   428,     0,     0,   794,   795,   796,
     797,     0,     0,   429,     0,     0,     0,     0,     0,     0,
      97,   702,   807,   808,   430,     0,     0,   800,   794,   795,
     796,   797,   807,   808,     0,     0,    56,    91,     0,     0,
      91,   104,     0,     0,   812,   505,     0,     0,     0,     0,
       0,    57,   101,    59,    60,   702,    97,     0,     0,    97,
     800,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     800,     0,    64,   431,     0,   174,   101,     0,    67,   101,
       0,   104,     0,   101,    22,   175,     0,     0,     0,     0,
       0,    28,     0,  -131,   656,     0,     0,     0,    91,   -38,
      91,     0,     0,    91,   105,   104,    91,     0,     0,    91,
       0,  -131,   105,     0,     0,     0,     0,    97,     0,    97,
       0,   105,    97,     0,     0,    97,   104,   101,    97,    91,
       0,     0,     0,     0,   105,   174,    91,   232,    91,     0,
       0,     0,     0,    91,    22,   175,     0,    13,    97,     0,
      16,    28,   105,  -131,     0,    97,     0,    97,    21,    22,
     706,     0,    97,     0,     0,     0,    28,     0,  -131,     0,
     105,  -131,   233,     0,   105,     0,   234,     0,     0,     0,
       0,     0,     0,    38,     0,   707,  -131,   708,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,    51,    91,    52,     0,     0,     0,    91,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,    97,   244,   105,   104,     0,    97,   104,   250,   251,
      91,   104,     0,   254,    91,     0,   255,   256,   257,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,    97,
       0,     0,    66,    97,     0,    13,   107,     0,    16,   766,
       0,     0,     0,   105,   107,     0,    21,    22,   706,     0,
       0,     0,     0,   107,    28,   104,  -131,     0,   101,     0,
       0,   101,     0,     0,     0,     0,   107,   105,     0,     0,
       0,   217,     0,   707,  -131,   708,    42,     0,    91,    91,
      91,    91,     0,     0,   107,     0,     0,     0,   105,    51,
     186,    52,     0,    91,    91,     0,     0,    97,    97,    97,
      97,     0,   107,     0,     0,     0,   107,     0,     0,     0,
       0,     0,    97,    97,     0,     0,     0,     0,     0,   101,
       0,   101,     0,     0,   101,     0,   109,   101,     0,     0,
     101,     0,     0,     0,   109,     0,     0,     0,     0,     0,
      66,     0,     0,   109,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,   107,   109,   101,     0,   101,
       0,     0,   105,     0,   101,     0,     0,     0,     0,   281,
     104,     0,     0,     0,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,   105,
       0,     0,   109,   105,     0,   107,   109,     0,     0,     0,
     186,   186,   186,   312,     0,     0,   104,     0,     0,   104,
       0,     0,     0,     0,   186,     0,     0,     0,     0,   107,
       0,     0,     0,   101,     0,     0,     0,     0,   101,     0,
       0,     0,   186,     0,     0,     0,     0,   105,     0,     0,
     107,   186,     0,     0,     0,   109,     0,     0,     0,     0,
       0,   101,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   104,     0,   104,
       0,     0,   104,     0,     0,   104,     0,     0,   104,     0,
       0,     0,     0,     0,     0,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,   104,     0,   104,     0,   109,
       0,     0,   104,     0,   107,     0,     0,     0,     0,   101,
     101,   101,   101,     0,     0,     0,     0,     0,     0,     0,
     109,   186,     0,   232,   101,   101,     0,     0,   107,     0,
       0,   107,     0,     0,     0,   107,     0,     0,     0,     0,
       0,     0,   105,   110,   186,     0,     0,     0,     0,     0,
       0,   110,     0,   232,     0,     0,     0,     0,   233,     0,
     110,   104,   234,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,   105,   107,
       0,   105,     0,     0,     0,     0,     0,     0,   233,   104,
       0,   110,   234,   104,   109,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,     0,     0,     0,   244,   110,
       0,     0,     0,   110,   250,   251,     0,     0,   109,   254,
       0,   109,   255,   256,   257,   109,     0,   236,     0,   237,
     238,   239,     0,   240,   241,     0,     0,     0,   244,   105,
       0,   105,     0,     0,   105,   251,     0,   105,     0,     0,
     105,     0,   255,   256,   257,     0,     0,   104,   104,   104,
     104,     0,   110,     0,     0,     0,     0,     0,     0,   109,
     105,     0,   104,   104,     0,     0,     0,   105,   232,   105,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   107,   186,   186,     0,     0,   186,
     186,     0,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   233,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,   110,     0,     0,     0,
     107,     0,     0,   107,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   105,     0,     0,     0,   110,   105,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,     0,   248,   249,   250,
     251,   105,   252,   253,   254,   105,     0,   255,   256,   257,
       0,     0,     0,     0,   109,     0,     0,   186,   186,     0,
       0,   107,     0,   107,     0,     0,   107,     0,   186,   107,
     312,     0,   107,     0,     0,   312,     0,     0,   186,     0,
       0,   186,     0,     0,     0,     0,     0,     0,     0,     0,
     109,   110,   107,   109,     0,     0,     0,     0,     0,   107,
     232,   107,     0,     0,     0,     0,   107,     0,     0,   105,
     105,   105,   105,     0,     0,   110,   230,     0,   110,     0,
     231,     0,   110,     0,   105,   105,     0,     0,   232,     0,
       0,     0,     0,     0,     0,   233,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   109,     0,   109,     0,     0,   109,     0,     0,   109,
       0,     0,   109,   233,     0,   107,   110,   234,     0,     0,
     107,     0,     0,     0,   236,     0,     0,   238,   239,     0,
     240,   241,   109,   120,     0,   244,     0,   235,     0,   109,
       0,   109,   251,   107,     0,     0,   109,   107,   186,   255,
     256,   257,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,     0,   255,   256,   257,
       0,     0,   312,   312,   124,   312,   312,   607,     0,   124,
       0,     0,     0,   147,   124,   124,     0,   232,     0,   778,
       0,   161,     0,   164,   312,   109,   312,     0,     0,     0,
     109,   107,   107,   107,   107,     0,     0,     0,     0,     0,
       0,     0,     0,   194,     0,     0,   107,   107,     0,     0,
       0,   110,   233,   109,     0,     0,   234,   109,   778,     0,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
     145,   220,     0,     0,     0,     0,     0,   110,     0,     0,
     110,   236,     0,   167,     0,   239,     0,   240,   241,     0,
       0,     0,   244,     0,     0,     0,     0,     0,     0,   251,
       0,   192,     0,     0,     0,     0,   255,   256,   257,     0,
       0,   109,   109,   109,   109,     0,     0,     0,     0,    73,
       0,     0,     0,   218,   194,     0,   109,   109,   292,     0,
       0,     0,     0,     0,   194,     0,   301,     0,   110,     0,
     110,     0,   292,   110,     0,   319,   110,     0,     0,   110,
       0,     0,     0,     0,     0,   292,     0,   124,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,   110,
       0,     0,   278,   292,     0,     0,   110,     0,   110,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,     0,     0,   126,     0,
       0,   361,     0,     0,     0,     0,   164,   164,   164,   164,
       0,    20,   314,     0,     0,     0,   127,   128,   129,     0,
      27,     0,     0,   386,     0,    30,    31,     0,    33,     0,
      34,    35,     0,     0,     0,     0,   336,    37,     0,    39,
       0,     0,   110,     0,     0,     0,     0,   110,     0,     0,
       0,    47,    48,    49,   130,   292,   292,    73,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -263,
     110,     0,     0,     0,   110,     0,     0,     0,     0,     0,
     452,     5,   456,     0,    58,     0,     0,   615,  -263,     0,
       0,   616,  -263,     0,     0,     0,     0,     0,     0,     0,
     319,     0,    63,     0,     0,     0,   178,     0,     0,   164,
       0,     0,     0,   131,     0,    69,  -263,   737,     0,     0,
     292,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,     0,     0,     0,     0,     0,     0,   110,   110,
     110,   110,     0,     0,     0,     0,     0,   164,     0,     0,
       0,     0,     0,   110,   110,   336,     0,     0,   336,   617,
     618,   619,   620,   621,   622,     0,     0,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,     0,     0,   633,
       0,     0,     0,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,     0,   645,     0,     0,   646,   647,
     648,   649,   650,     0,     0,     0,   504,     0,     0,     0,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,    13,    14,    15,    16,
    -236,    17,    18,   292,    19,    20,     0,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,  -131,    29,    30,
      31,    32,    33,   -37,    34,    35,   584,    36,     0,   319,
    -236,    37,    38,    39,    40,  -131,    41,    42,     0,  -236,
      43,    44,     0,    45,    46,    47,    48,    49,    50,   194,
      51,     0,    52,    53,  -236,     0,     0,    54,     0,    55,
       0,     0,    56,     0,     0,     0,     0,     0,   651,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,   580,     0,     0,    62,     0,    63,     0,    64,    65,
       0,    66,     0,     0,    67,     0,     0,    68,     0,    69,
       0,    70,     0,   452,     0,     0,   479,     0,     5,     6,
       7,     8,     9,     0,     0,   126,     0,   609,     0,     0,
      73,     0,     0,     0,     0,   319,     0,     0,    20,     0,
       0,     0,     0,   127,   128,   129,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,   713,     0,     0,     0,     0,     0,   336,     0,
     336,     0,     0,   336,     0,    56,   336,     0,     0,   683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,   693,
       0,     0,     0,     0,     0,     0,   697,    62,   701,    63,
       0,    64,    65,    73,     0,     0,     0,    67,     0,     0,
     131,     0,    69,     0,   480,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   124,     0,
       0,     0,     0,     0,     0,     0,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     0,     5,     6,     7,
       8,     9,     0,  -381,    10,    11,    12,  -381,    13,    14,
      15,    16,  -236,    17,    18,  -381,    19,    20,  -381,    21,
      22,    23,    24,    25,    26,     0,    27,    28,     0,  -131,
      29,    30,    31,    32,    33,   -37,    34,    35,     0,    36,
     751,     0,  -236,    37,   753,    39,    40,  -131,    41,    42,
    -381,  -236,    43,    44,  -381,    45,    46,    47,    48,    49,
      50,     0,    51,     0,    52,    53,  -236,     0,     0,    54,
       0,    55,     0,     0,  -381,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -381,
      58,  -381,  -381,  -381,  -381,  -381,  -381,  -381,     0,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,    63,  -381,
    -381,  -381,     0,    66,  -381,  -381,  -381,     0,     0,    68,
    -381,    69,     4,    70,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -236,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,    26,     0,    27,    28,     0,  -131,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -236,
      37,    38,    39,    40,  -131,    41,    42,     0,  -236,    43,
      44,     0,    45,    46,    47,    48,    49,    50,     0,    51,
       0,    52,    53,  -236,     0,     0,    54,     0,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    63,     0,    64,    65,   178,
      66,     0,     0,    67,     0,     0,    68,   204,    69,     4,
      70,     5,     6,     7,     8,     9,     0,     0,    10,    11,
      12,     0,    13,    14,    15,    16,  -236,    17,    18,     0,
      19,    20,     0,    21,    22,    23,    24,    25,    26,     0,
      27,    28,     0,  -131,    29,    30,    31,    32,    33,   -37,
      34,    35,     0,    36,     0,     0,  -236,    37,    38,    39,
      40,  -131,    41,    42,     0,  -236,    43,    44,     0,    45,
      46,    47,    48,    49,    50,     0,    51,     0,    52,    53,
    -236,     0,     0,    54,     0,    55,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,    63,     0,    64,    65,     0,    66,     0,     0,
      67,     0,     0,    68,   346,    69,     4,    70,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -236,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,    26,     0,    27,    28,     0,
    -131,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -236,    37,    38,    39,    40,  -131,    41,
      42,     0,  -236,    43,    44,     0,    45,    46,    47,    48,
      49,    50,     0,    51,     0,    52,    53,  -236,     0,     0,
      54,     0,    55,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,    63,
       0,    64,    65,     0,    66,     0,     0,    67,     0,     0,
      68,   204,    69,     4,    70,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,    13,    14,    15,    16,
    -236,    17,    18,     0,    19,    20,     0,    21,    22,    23,
      24,    25,    26,     0,    27,    28,     0,  -131,    29,    30,
      31,    32,    33,   -37,    34,    35,     0,    36,     0,     0,
    -236,    37,    38,    39,    40,  -131,    41,    42,     0,  -236,
      43,    44,     0,    45,    46,    47,    48,    49,    50,     0,
      51,     0,    52,    53,  -236,     0,     0,    54,     0,    55,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,    66,     0,     0,    67,     0,     0,    68,   611,    69,
       4,    70,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -236,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,    26,
       0,    27,    28,     0,  -131,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -236,    37,    38,
      39,    40,  -131,    41,    42,     0,  -236,    43,    44,     0,
      45,    46,    47,    48,    49,    50,     0,    51,     0,    52,
      53,  -236,     0,     0,    54,     0,    55,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    63,     0,    64,    65,     0,    66,     0,
       0,    67,     0,     0,    68,   705,    69,     4,    70,     5,
       6,     7,     8,     9,     0,     0,    10,    11,    12,     0,
      13,    14,    15,    16,  -236,    17,    18,     0,    19,    20,
       0,    21,    22,    23,    24,    25,    26,     0,    27,    28,
       0,  -131,    29,    30,    31,    32,    33,   -37,    34,    35,
       0,    36,     0,     0,  -236,    37,   217,    39,    40,  -131,
      41,    42,     0,  -236,    43,    44,     0,    45,    46,    47,
      48,    49,    50,     0,    51,     0,    52,    53,  -236,     0,
       0,    54,     0,    55,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
      63,     0,    64,    65,     0,    66,     0,     0,    67,     0,
       0,    68,     0,    69,     4,    70,     5,     6,     7,     8,
       9,     0,     0,    10,    11,    12,     0,    13,    14,    15,
      16,  -236,    17,    18,     0,    19,    20,     0,    21,    22,
      23,    24,    25,    26,     0,    27,    28,     0,  -131,    29,
      30,    31,    32,    33,   -37,    34,    35,     0,    36,     0,
       0,  -236,    37,     0,    39,    40,  -131,    41,    42,     0,
    -236,    43,    44,     0,    45,    46,    47,    48,    49,    50,
       0,    51,     0,    52,    53,  -236,     0,     0,    54,     0,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,    62,     0,    63,     0,    64,
      65,     0,    66,     0,     0,    67,     0,     0,    68,     0,
      69,     4,    70,     5,     6,     7,     8,     9,     0,     0,
      10,    11,    12,     0,    13,    14,    15,    16,  -236,    17,
      18,     0,    19,    20,     0,    21,    22,    23,    24,    25,
     459,     0,    27,    28,     0,  -131,    29,    30,    31,    32,
      33,   -37,    34,    35,     0,    36,     0,     0,  -236,    37,
       0,    39,    40,  -131,    41,    42,     0,  -236,    43,    44,
       0,    45,    46,    47,    48,    49,    50,     0,    51,     0,
      52,    53,  -236,     0,     0,    54,     0,    55,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    62,     0,    63,     0,    64,    65,     0,    66,
       0,     0,    67,     0,     0,    68,     0,    69,     4,    70,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,    15,    16,  -236,    17,    18,     0,    19,
      20,     0,    21,    22,    23,    24,    25,   463,     0,    27,
      28,     0,  -131,    29,    30,    31,    32,    33,   -37,    34,
      35,     0,    36,     0,     0,  -236,    37,     0,    39,    40,
    -131,    41,    42,     0,  -236,    43,    44,     0,    45,    46,
      47,    48,    49,    50,     0,    51,     0,    52,    53,  -236,
       0,     0,    54,     0,    55,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,     0,    66,     0,     0,    67,
       0,     0,    68,     0,    69,     4,    70,     5,     6,     7,
       8,     9,     0,     0,    10,    11,    12,     0,    13,    14,
      15,    16,  -236,    17,    18,     0,    19,    20,     0,    21,
      22,    23,    24,    25,   672,     0,    27,    28,     0,  -131,
      29,    30,    31,    32,    33,   -37,    34,    35,     0,    36,
       0,     0,  -236,    37,     0,    39,    40,  -131,    41,    42,
       0,  -236,    43,    44,     0,    45,    46,    47,    48,    49,
      50,     0,    51,     0,    52,    53,  -236,     0,     0,    54,
       0,    55,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,    62,     0,    63,     0,
      64,    65,     0,    66,     0,     0,    67,     0,     0,    68,
       0,    69,     4,    70,     5,     6,     7,     8,     9,     0,
       0,    10,    11,    12,     0,    13,    14,    15,    16,  -236,
      17,    18,     0,    19,    20,     0,    21,    22,    23,    24,
      25,   674,     0,    27,    28,     0,  -131,    29,    30,    31,
      32,    33,   -37,    34,    35,     0,    36,     0,     0,  -236,
      37,     0,    39,    40,  -131,    41,    42,     0,  -236,    43,
      44,     0,    45,    46,    47,    48,    49,    50,     0,    51,
       0,    52,    53,  -236,     0,     0,    54,     0,    55,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    62,     0,    63,     0,    64,    65,     0,
      66,     0,     0,    67,     0,     0,    68,     0,    69,     4,
      70,     5,     6,     7,     8,     9,     0,     0,    10,    11,
      12,     0,    13,    14,    15,    16,  -236,    17,    18,     0,
      19,    20,     0,    21,    22,    23,    24,    25,   677,     0,
      27,    28,     0,  -131,    29,    30,    31,    32,    33,   -37,
      34,    35,     0,    36,     0,     0,  -236,    37,     0,    39,
      40,  -131,    41,    42,     0,  -236,    43,    44,     0,    45,
      46,    47,    48,    49,    50,     0,    51,     0,    52,    53,
    -236,     0,     0,    54,     0,    55,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,    63,     0,    64,    65,     0,    66,     0,     0,
      67,     0,     0,    68,     0,    69,     4,    70,     5,     6,
       7,     8,     9,     0,     0,    10,    11,    12,     0,    13,
      14,    15,    16,  -236,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,    25,   680,     0,    27,    28,     0,
    -131,    29,    30,    31,    32,    33,   -37,    34,    35,     0,
      36,     0,     0,  -236,    37,     0,    39,    40,  -131,    41,
      42,     0,  -236,    43,    44,     0,    45,    46,    47,    48,
      49,    50,     0,    51,     0,    52,    53,  -236,     0,     0,
      54,     0,    55,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,    62,     0,    63,
       0,    64,    65,     0,    66,     0,     0,    67,     0,     0,
      68,     0,    69,     4,    70,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,    13,    14,    15,    16,
    -236,    17,    18,     0,    19,    20,     0,    21,    22,    23,
      24,    25,   696,     0,    27,    28,     0,  -131,    29,    30,
      31,    32,    33,   -37,    34,    35,     0,    36,     0,     0,
    -236,    37,     0,    39,    40,  -131,    41,    42,     0,  -236,
      43,    44,     0,    45,    46,    47,    48,    49,    50,     0,
      51,     0,    52,    53,  -236,     0,     0,    54,     0,    55,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,    62,     0,    63,     0,    64,    65,
       0,    66,     0,     0,    67,     0,     0,    68,     0,    69,
       4,    70,     5,     6,     7,     8,     9,     0,     0,    10,
      11,    12,     0,    13,    14,    15,    16,  -236,    17,    18,
       0,    19,    20,     0,    21,    22,    23,    24,    25,   700,
       0,    27,    28,     0,  -131,    29,    30,    31,    32,    33,
     -37,    34,    35,     0,    36,     0,     0,  -236,    37,     0,
      39,    40,  -131,    41,    42,     0,  -236,    43,    44,     0,
      45,    46,    47,    48,    49,    50,     0,    51,     0,    52,
      53,  -236,     0,     0,    54,     0,    55,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    63,     0,    64,    65,     0,    66,     0,
       0,    67,     0,     0,    68,     0,    69,   479,    70,     5,
       6,     7,     8,     9,     0,  -381,   126,     0,     0,  -381,
       0,     0,     0,     0,     0,     0,     0,  -381,     0,    20,
       0,     0,     0,     0,   127,   128,   129,     0,    27,     0,
       0,     0,     0,    30,    31,     0,    33,     0,    34,    35,
       0,     0,     0,     0,     0,    37,     0,    39,     0,     0,
       0,     0,  -381,     0,     0,     0,  -381,     0,     0,    47,
      48,    49,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -254,  -381,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -381,    58,  -381,  -381,  -381,     0,  -381,  -381,  -381,
       0,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,  -381,
      63,  -381,  -381,  -381,     0,     0,  -381,  -381,  -381,     0,
       0,   131,     0,    69,   479,   494,     5,     6,     7,     8,
       9,     0,  -381,   126,     0,     0,  -381,     0,     0,     0,
       0,     0,     0,     0,  -381,     0,    20,     0,     0,     0,
       0,   127,   128,   129,     0,    27,     0,     0,     0,     0,
      30,    31,     0,    33,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,  -381,
       0,     0,     0,  -381,     0,     0,    47,    48,    49,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -381,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -381,    58,
    -381,  -381,  -381,     0,  -381,  -381,  -381,     0,  -381,  -381,
    -381,  -381,  -381,  -381,  -381,  -381,  -381,    63,  -381,  -381,
    -381,     0,     0,  -381,  -381,  -381,     0,     0,   131,     0,
      69,   479,   494,     5,     6,     7,     8,     9,     0,     0,
     126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,     0,     0,     0,     0,   127,   128,
     129,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    34,    35,     0,     0,     0,     0,     0,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   126,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,    20,     0,     0,     0,     0,   127,
     128,   129,    62,    27,    63,     0,    64,    65,    30,    31,
       0,    33,    67,    34,    35,   131,     0,    69,     0,   494,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,    48,    49,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     5,     6,     7,
       8,     9,     0,    62,   126,    63,     0,    64,    65,   178,
       0,     0,     0,    67,     0,     0,   131,    20,    69,     0,
     132,   772,   127,   128,   129,     0,    27,     0,     0,     0,
       0,    30,    31,     0,    33,     0,    34,    35,     0,     0,
       0,     0,     0,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,   208,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,   209,     0,
       5,     6,     7,     8,     9,     0,    62,   126,    63,     0,
      64,    65,   178,     0,     0,     0,    67,     0,     0,   131,
      20,    69,     0,   132,     0,   127,   128,   129,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,   130,     0,     0,   208,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     5,     6,     7,     8,     9,     0,    62,
     126,    63,     0,    64,    65,   178,     0,     0,     0,    67,
       0,     0,   131,    20,    69,   499,   132,     0,   127,   128,
     129,     0,    27,     0,     0,     0,     0,    30,    31,     0,
      33,     0,    34,    35,     0,     0,     0,     0,   154,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,     0,
      56,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   126,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,    20,     0,     0,     0,     0,   127,
     128,   129,    62,    27,    63,     0,    64,    65,    30,    31,
       0,    33,    67,    34,    35,   131,     0,    69,     0,   132,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,    48,    49,   130,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,     0,     0,     5,     6,     7,
       8,     9,     0,    62,   126,    63,     0,    64,    65,   178,
       0,     0,     0,    67,     0,     0,   131,    20,    69,     0,
     132,     0,   127,   128,   129,     0,    27,     0,     0,     0,
       0,    30,    31,     0,    33,     0,    34,    35,     0,     0,
       0,     0,     0,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,     0,     0,
       5,     6,     7,     8,     9,     0,    62,   126,    63,     0,
      64,    65,   178,     0,     0,     0,    67,     0,     0,   131,
      20,    69,     0,   132,     0,   127,   128,   129,     0,    27,
       0,     0,     0,     0,    30,    31,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,    65,     0,   182,     0,     0,    67,
       0,     0,   131,     0,    69,     0,   132,     5,     6,     7,
       8,     9,     0,     0,   126,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   185,    20,     0,     0,
       0,     0,   127,   128,   129,     0,    27,     0,     0,     0,
       0,    30,    31,     0,    33,     0,    34,    35,     0,     0,
       0,     0,     0,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    20,     0,
       0,     0,     0,   127,   128,   129,    62,    27,    63,     0,
      64,    65,    30,    31,     0,    33,    67,    34,    35,    68,
       0,    69,     0,   132,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     5,     6,     7,     8,     9,     0,    62,   126,    63,
       0,    64,    65,     0,     0,     0,     0,    67,     0,     0,
     131,    20,    69,     0,   132,     0,   127,   128,   129,     0,
      27,     0,     0,     0,     0,    30,    31,     0,    33,     0,
      34,    35,   596,     0,     0,     0,     0,    37,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    48,    49,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   126,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,   734,     0,     0,     0,     0,   127,   128,   129,
      62,    27,    63,     0,    64,    65,    30,    31,     0,    33,
      67,    34,    35,   131,     0,    69,     0,   132,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   735,    48,    49,   736,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,     0,     0,     5,     6,     7,     8,     9,
       0,    62,   126,    63,     0,    64,    65,   178,     0,     0,
       0,    67,     0,     0,   131,    20,    69,     0,   737,     0,
     127,   128,   129,     0,    27,     0,     0,     0,     0,    30,
      31,     0,    33,     0,    34,    35,     0,     0,     0,     0,
       0,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,     0,     0,     5,     6,
       7,     8,     9,     0,    62,   126,    63,     0,    64,    65,
     178,     0,     0,     0,    67,     0,     0,   131,    20,    69,
       0,   737,     0,   127,   128,   725,     0,    27,     0,     0,
       0,     0,    30,    31,     0,    33,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,     0,
       0,     5,     6,     7,     8,     9,     0,    62,   126,    63,
       0,    64,    65,   178,     0,     0,     0,    67,     0,     0,
     131,    20,    69,     0,   132,     0,   127,   128,   129,     0,
      27,     0,     0,     0,     0,    30,    31,     0,    33,     0,
      34,    35,     0,     0,     0,     0,     0,    37,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    48,    49,   130,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   126,
       0,     0,     0,    57,    58,    59,    60,     0,     0,     0,
       0,    61,    20,     0,     0,     0,     0,   127,   128,   545,
      62,    27,    63,     0,    64,    65,    30,    31,     0,    33,
      67,    34,    35,   131,     0,    69,     0,   132,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     5,     6,     7,     8,     9,     0,     0,
     126,     0,     0,     0,    57,    58,    59,    60,     0,     0,
       0,     0,    61,    20,     0,     0,     0,     0,   127,   128,
     548,    62,    27,    63,     0,    64,    65,    30,    31,     0,
      33,    67,    34,    35,   131,     0,    69,     0,   132,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    47,    48,    49,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   126,     0,     0,     0,    57,    58,    59,    60,     0,
       0,     0,     0,    61,    20,     0,     0,     0,     0,   127,
     128,   719,    62,    27,    63,     0,    64,    65,    30,    31,
       0,    33,    67,    34,    35,   131,     0,    69,     0,   132,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    47,    48,    49,   130,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     5,     6,     7,     8,     9,
       0,     0,   126,     0,     0,     0,    57,    58,    59,    60,
       0,     0,     0,     0,    61,    20,     0,     0,     0,     0,
     127,   128,   720,    62,    27,    63,     0,    64,    65,    30,
      31,     0,    33,    67,    34,    35,   131,     0,    69,     0,
     132,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,   130,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   126,     0,     0,     0,    57,    58,    59,
      60,     0,     0,     0,     0,    61,    20,     0,     0,     0,
       0,   127,   128,   722,    62,    27,    63,     0,    64,    65,
      30,    31,     0,    33,    67,    34,    35,   131,     0,    69,
       0,   132,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    48,    49,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     5,     6,     7,
       8,     9,     0,     0,   126,     0,     0,     0,    57,    58,
      59,    60,     0,     0,     0,     0,    61,    20,     0,     0,
       0,     0,   127,   128,   723,    62,    27,    63,     0,    64,
      65,    30,    31,     0,    33,    67,    34,    35,   131,     0,
      69,     0,   132,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    48,    49,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   126,     0,     0,     0,    57,
      58,    59,    60,     0,     0,     0,     0,    61,    20,     0,
       0,     0,     0,   127,   128,   724,    62,    27,    63,     0,
      64,    65,    30,    31,     0,    33,    67,    34,    35,   131,
       0,    69,     0,   132,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     5,
       6,     7,     8,     9,     0,     0,   126,     0,     0,     0,
      57,    58,    59,    60,     0,     0,     0,     0,    61,    20,
       0,     0,     0,     0,   127,   128,   725,    62,    27,    63,
       0,    64,    65,    30,    31,     0,    33,    67,    34,    35,
     131,     0,    69,     0,   132,    37,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    47,
      48,    49,   130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   126,     0,     0,
       0,    57,    58,    59,    60,     0,     0,     0,     0,    61,
      20,     0,     0,     0,     0,   127,   128,   129,    62,    27,
      63,     0,    64,    65,    30,    31,     0,    33,    67,    34,
      35,   131,     0,    69,     0,   132,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,     0,     0,   126,     0,
       0,     0,    57,    58,    59,    60,     0,     0,     0,     0,
       0,    20,     0,     0,     0,     0,   127,   128,   129,    62,
      27,    63,     0,     0,    65,    30,    31,     0,    33,    67,
      34,    35,   131,     0,    69,     0,   132,    37,   230,    39,
       0,     0,   231,     0,     0,     0,     0,     0,     0,     0,
     232,    47,    48,    49,   130,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   230,     0,    58,   233,   231,     0,     0,   234,
       0,     0,     0,     0,   232,     0,     0,   272,  -343,     0,
       0,  -365,    63,  -365,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,    69,     0,   132,     0,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,   233,
     240,   241,   242,   234,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,     0,   255,
     256,   257,     0,   235,   230,     0,     0,     0,   231,   664,
       0,     0,     0,     0,     0,     0,   232,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,     0,   255,   256,   257,     0,     0,   230,     0,
       0,   233,   231,   781,     0,   234,     0,     0,     0,     0,
     232,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,     0,   237,   238,   239,   233,   240,   241,   242,   234,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,     0,   255,   256,   257,     0,   235,
       0,     0,     0,     0,     0,   815,     0,     0,     0,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   230,   252,   253,   254,   231,     0,   255,
     256,   257,     0,     0,     0,   232,   307,   498,     0,     0,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,     0,     0,
     233,   231,     0,     0,   234,     0,     0,     0,     0,   232,
     304,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     120,     0,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,   233,   240,   241,   242,   234,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,     0,   255,   256,   257,   230,   235,   280,
       0,   231,     0,     0,     0,     0,     0,     0,     0,   232,
     575,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,     0,   255,   256,
     257,     0,     0,   280,   233,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,   230,     0,     0,   235,   231,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,     0,   255,   256,
     257,     0,   233,   280,     0,     0,   234,     0,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,   230,     0,
       0,     0,   231,     0,     0,     0,   235,     0,     0,     0,
     232,   185,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,   233,   255,   256,   257,   234,
       0,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,   235,
       0,     0,     0,   232,   569,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,   233,   255,
     256,   257,   234,     0,   280,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,     0,   231,     0,
       0,     0,   235,     0,     0,     0,   232,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,   233,   255,   256,   257,   234,     0,   280,     0,     0,
       0,     0,   671,     0,     0,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,   235,     0,     0,     0,   232,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     236,     0,   237,   238,   239,     0,   240,   241,   242,     0,
     243,   244,   245,   246,   247,   248,   249,   250,   251,     0,
     252,   253,   254,     0,   233,   255,   256,   257,   234,     0,
     280,     0,     0,     0,     0,   676,     0,     0,     0,     0,
     230,     0,     0,     0,   231,     0,     0,     0,   235,     0,
       0,     0,   232,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,   233,   255,   256,
     257,   234,     0,   280,     0,     0,     0,     0,   767,     0,
       0,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,   235,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
     233,   255,   256,   257,   234,     0,   280,     0,     0,     0,
       0,   768,     0,     0,     0,     0,   230,     0,     0,     0,
     231,     0,     0,     0,   235,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,   233,   255,   256,   257,   234,     0,   280,
       0,     0,     0,     0,   769,     0,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,   235,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   233,   255,   256,   257,
     234,     0,   280,     0,     0,     0,     0,   770,     0,     0,
       0,     0,   230,     0,     0,     0,   231,     0,     0,     0,
     235,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,   233,
     255,   256,   257,   234,     0,   280,     0,     0,     0,     0,
     782,     0,     0,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,   235,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,   233,   255,   256,   257,   234,     0,   280,     0,
       0,     0,     0,   783,     0,     0,     0,     0,   230,     0,
       0,     0,   231,     0,     0,     0,   235,     0,     0,     0,
     232,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,     0,   233,   255,   256,   257,   234,
       0,   280,     0,     0,     0,     0,   230,     0,     0,     0,
     231,     0,     0,     0,     0,     0,     0,     0,   232,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   233,   252,   253,   254,   234,     0,   255,
     256,   257,     0,     0,   335,     0,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,   235,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   233,   255,   256,   257,
     234,  -344,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,     0,   230,     0,
     235,     0,   231,     0,     0,     0,     0,     0,     0,     0,
     232,   436,     0,     0,     0,   236,     0,   237,   238,   239,
     437,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,     0,
     255,   256,   257,     0,     0,   233,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,   235,
       0,     0,     0,   232,   438,     0,     0,     0,     0,     0,
       0,     0,     0,   439,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,     0,   233,   255,
     256,   257,   234,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,   232,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   233,   252,   253,   254,
     234,     0,   255,   256,   257,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,   232,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   233,   252,   253,   254,   234,   290,
     255,   256,   257,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,   232,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   233,   252,   253,   254,   234,   334,   255,   256,
     257,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,   232,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     233,   252,   253,   254,   234,   345,   255,   256,   257,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,     0,
     231,     0,     0,   357,   235,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,   233,   255,   256,   257,   234,     0,     0,
       0,     0,     0,     0,   440,     0,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,   235,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   233,   255,   256,   257,
     234,     0,     0,     0,     0,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,   232,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   233,   252,   253,   254,   234,   522,
     255,   256,   257,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,   232,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   233,   252,   253,   254,   234,   523,   255,   256,
     257,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,   232,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     233,   252,   253,   254,   234,   524,   255,   256,   257,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,     0,
       0,     0,     0,   232,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   233,   252,
     253,   254,   234,   525,   255,   256,   257,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,   232,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   233,   252,   253,   254,
     234,   526,   255,   256,   257,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,   232,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   233,   252,   253,   254,   234,   527,
     255,   256,   257,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,   232,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   233,   252,   253,   254,   234,   528,   255,   256,
     257,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,   232,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     233,   252,   253,   254,   234,   529,   255,   256,   257,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,     0,
       0,     0,     0,   232,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   233,   252,
     253,   254,   234,   530,   255,   256,   257,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,   232,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   233,   252,   253,   254,
     234,   531,   255,   256,   257,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,   232,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   233,   252,   253,   254,   234,   532,
     255,   256,   257,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,   232,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   233,   252,   253,   254,   234,   533,   255,   256,
     257,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,   232,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     233,   252,   253,   254,   234,   534,   255,   256,   257,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,     0,
       0,     0,     0,   232,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   233,   252,
     253,   254,   234,   535,   255,   256,   257,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,     0,     0,     0,
       0,   232,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   233,   252,   253,   254,
     234,   536,   255,   256,   257,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,     0,   120,     0,     0,   232,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   233,   252,   253,   254,   234,     0,
     255,   256,   257,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,     0,     0,     0,     0,   232,   235,     0,
     579,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   233,   252,   253,   254,   234,   555,   255,   256,
     257,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,     0,     0,     0,     0,   232,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     233,   252,   253,   254,   234,     0,   255,   256,   257,     0,
       0,     0,     0,     0,     0,     0,   230,     0,     0,     0,
     231,     0,     0,   610,   235,     0,     0,     0,   232,   659,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,   233,   255,   256,   257,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,   235,     0,     0,
       0,   232,   662,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   233,   255,   256,   257,
     234,     0,     0,     0,     0,     0,     0,   230,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,   232,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   233,   252,   253,   254,   234,     0,
     255,   256,   257,     0,     0,   718,     0,     0,     0,     0,
     230,     0,     0,     0,   231,     0,     0,     0,   235,     0,
       0,     0,   232,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,   237,   238,   239,     0,   240,
     241,   242,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,     0,   252,   253,   254,     0,   233,   255,   256,
     257,   234,     0,     0,     0,     0,     0,     0,   721,     0,
       0,     0,     0,   230,     0,     0,     0,   231,     0,     0,
       0,   235,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   236,     0,   237,   238,
     239,     0,   240,   241,   242,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,     0,   252,   253,   254,     0,
     233,   255,   256,   257,   234,     0,     0,     0,     0,     0,
       0,   788,     0,     0,     0,     0,   230,     0,     0,     0,
     231,     0,     0,     0,   235,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   236,
       0,   237,   238,   239,     0,   240,   241,   242,     0,   243,
     244,   245,   246,   247,   248,   249,   250,   251,     0,   252,
     253,   254,     0,   233,   255,   256,   257,   234,     0,     0,
       0,     0,     0,     0,   789,     0,     0,     0,     0,   230,
       0,     0,     0,   231,     0,     0,     0,   235,     0,     0,
       0,   232,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,     0,   237,   238,   239,     0,   240,   241,
     242,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,     0,   252,   253,   254,     0,   233,   255,   256,   257,
     234,     0,     0,     0,     0,     0,     0,   790,     0,     0,
       0,     0,   230,     0,     0,     0,   231,     0,     0,     0,
     235,     0,     0,     0,   232,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,   243,   244,   245,   246,   247,
     248,   249,   250,   251,     0,   252,   253,   254,     0,   233,
     255,   256,   257,   234,     0,     0,     0,     0,     0,     0,
     791,     0,     0,     0,     0,   230,     0,     0,     0,   231,
       0,     0,     0,   235,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   247,   248,   249,   250,   251,     0,   252,   253,
     254,     0,   233,   255,   256,   257,   234,     0,     0,     0,
       0,     0,     0,   792,     0,     0,     0,     0,   230,     0,
       0,     0,   231,     0,     0,     0,   235,     0,     0,     0,
     232,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,   247,   248,   249,   250,   251,
       0,   252,   253,   254,   232,   233,   255,   256,   257,   234,
       0,     0,     0,     0,     0,     0,   793,     0,     0,     0,
       0,   230,     0,     0,     0,   231,     0,     0,     0,   235,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   233,
       0,     0,     0,   234,   236,     0,   237,   238,   239,     0,
     240,   241,   242,     0,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   235,   252,   253,   254,   232,   233,   255,
     256,   257,   234,     0,     0,     0,     0,     0,   236,     0,
     237,   238,   239,     0,   240,   241,   242,     0,   243,   244,
     245,   246,   235,   248,   249,   250,   251,     0,   252,     0,
     254,   232,   233,   255,   256,   257,   234,   236,     0,   237,
     238,   239,     0,   240,   241,   242,     0,   243,   244,   245,
     246,   247,   248,   249,   250,   251,     0,   252,   253,   254,
       0,     0,   255,   256,   257,     0,   233,     0,     0,     0,
     234,   236,     0,   237,   238,   239,     0,   240,   241,   242,
       0,   243,   244,   245,   246,     0,   248,   249,   250,   251,
       0,   252,     0,   254,     0,     0,   255,   256,   257,     0,
       0,     0,     0,     0,     0,   236,     0,   237,   238,   239,
       0,   240,   241,   242,     0,     0,   244,   245,   246,     0,
     248,   249,   250,   251,     0,     0,     0,   254,     0,     0,
     255,   256,   257
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-726))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,    40,    41,     2,   340,    15,    14,   318,    18,   142,
     277,    43,   277,     1,    24,    25,    26,   593,    22,   152,
       3,    53,     3,    33,    34,     3,    36,    37,     3,   613,
       1,   158,     0,    43,    44,    45,    46,    47,    70,   323,
      50,    31,   613,    53,    54,    55,   478,   772,    58,     1,
     745,    61,    62,    63,   653,    65,   340,   100,    68,    69,
      70,   156,   157,   158,   224,   225,   226,   227,     3,    22,
       3,    59,    22,    22,    69,   170,    57,   100,    76,    31,
     805,   100,    22,   126,   805,    18,    74,   214,    59,   784,
      76,    69,    31,   188,   815,   160,    59,   127,    31,    21,
     132,    34,   197,    74,    31,    31,    68,   130,   173,   128,
     100,    99,   688,     1,    47,    48,   126,   127,   128,   129,
     130,   131,   132,    76,    57,     3,   191,    79,    99,    81,
     138,   119,   130,   127,    56,    68,   146,   125,    60,   710,
     130,    14,   726,    31,   127,    23,   127,    99,   100,   127,
     160,   146,   127,    24,   125,   726,   119,   589,   757,   758,
     100,   100,   125,   173,     7,   160,   477,   119,    35,   100,
     100,   175,   125,   100,   100,   125,   125,    99,   173,   101,
     102,   191,    55,    81,   106,   125,    53,    46,   119,   100,
      68,   113,   287,    81,   127,   771,   191,    70,   128,   209,
     122,    65,    59,   130,   130,   365,   104,   206,   273,   274,
     127,    99,   100,    12,    73,   310,    31,   128,    17,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   119,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   126,   125,   100,
     302,   100,   583,   338,   127,   542,   286,   542,   273,   274,
     712,   291,    31,     2,    18,   100,     3,    18,   119,    41,
       7,    10,   302,   100,   304,   305,   119,   307,   308,   128,
      19,   311,     2,    31,   127,   100,   129,     3,   592,   119,
      10,     7,   124,    32,    48,   130,   326,    48,    35,    19,
       3,   128,   332,    57,     7,     8,    57,    76,   338,    57,
     362,    50,    32,   128,    68,    18,    53,   332,   465,   466,
     350,    72,   352,   338,   354,   355,   119,   357,   358,    68,
      50,   100,   362,    72,    81,   350,   692,   352,   100,   695,
     100,   100,   233,   234,   497,    48,   461,   462,    68,    81,
     465,   466,    72,   100,    57,   502,   503,   104,    48,   119,
     119,   130,    56,    18,   126,    68,    60,    57,   100,    72,
     100,   100,   104,   119,   119,   100,    31,   119,    68,    34,
      81,   128,   121,   687,   275,   276,   100,   127,   692,   119,
     119,   695,    47,    48,   119,   286,   128,    31,    79,   125,
      81,   121,    57,   125,   119,   119,   436,   437,   438,   439,
     440,   441,   442,    68,   305,   443,   444,   308,   448,   449,
     119,   451,   161,    99,   119,    59,   102,   457,   480,   459,
     106,   119,    75,   463,   449,   127,   451,   129,   553,   554,
      74,   161,   494,   127,   127,   129,   185,    81,   127,   564,
     480,   102,   127,   483,   484,   127,   350,   129,   352,   574,
     119,     7,   577,   354,   494,   185,   100,   206,   772,    99,
     119,   128,   100,   119,    31,    24,   128,   781,    21,   100,
     128,    79,    81,   128,    14,   119,   206,    17,   128,   100,
      17,   125,   130,   125,   125,    25,    26,    27,    56,    26,
      27,   805,   128,    33,     2,    35,    33,    31,    35,    81,
     130,   815,    10,    56,    41,   545,    24,    60,   548,    21,
      50,    19,    52,    53,    54,    55,    53,   128,   128,   100,
     560,    22,    22,   130,    32,    59,   125,   567,    68,   569,
      70,   280,   572,    99,   130,   575,   437,   599,   439,   579,
      74,   130,    50,   127,    56,   102,    99,    81,    60,   102,
     280,   128,    74,   106,   509,   304,   153,   456,   307,   599,
      68,   669,   311,   714,    72,   605,   100,   607,   714,   694,
     610,   561,   715,   614,   304,   581,   784,   307,   771,   119,
     755,   311,   327,   612,   733,   119,   126,    99,   451,   101,
     102,   125,   542,    -1,   106,    -1,    -1,    -1,    31,    -1,
       3,   113,    -1,    -1,     7,    -1,   355,    -1,   120,   121,
     122,    -1,    -1,   121,    -1,    18,    -1,    -1,   658,   659,
      -1,   661,   662,   663,   664,   355,    59,    -1,    -1,    -1,
      -1,   671,   672,    -1,   674,    -1,   676,   677,   707,   708,
     680,    74,     2,    -1,    -1,    48,    -1,    -1,    81,    -1,
      10,   691,   692,   161,    57,   695,   696,    -1,    -1,    19,
     700,    -1,   687,    -1,    -1,    68,    -1,   100,    -1,    72,
      -1,    -1,    32,    -1,    -1,   737,    -1,   185,   718,   719,
     720,   721,   722,   723,   724,   725,   119,    -1,    -1,    -1,
      50,    -1,   125,    -1,    18,   735,   736,   737,   206,    -1,
      -1,   741,   432,    -1,   744,    -1,    -1,    31,    68,    -1,
      34,    -1,    72,    -1,    -1,    -1,   741,    -1,    -1,    -1,
      -1,   470,    -1,    47,    48,    -1,    -1,   767,   768,   769,
     770,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
     470,   781,   782,   783,    68,    -1,    -1,   772,   788,   789,
     790,   791,   792,   793,    -1,    -1,    80,   506,    -1,    -1,
     509,   121,    -1,    -1,   804,   805,    -1,    -1,    -1,    -1,
      -1,    95,   280,    97,    98,   815,   506,    -1,    -1,   509,
     805,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     815,    -1,   116,   117,    -1,    17,   304,    -1,   122,   307,
      -1,   161,    -1,   311,    26,    27,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,   544,    -1,    -1,    -1,   567,    41,
     569,    -1,    -1,   572,     2,   185,   575,    -1,    -1,   578,
      -1,    53,    10,    -1,    -1,    -1,    -1,   567,    -1,   569,
      -1,    19,   572,    -1,    -1,   575,   206,   355,   578,   598,
      -1,    -1,    -1,    -1,    32,    17,   605,    21,   607,    -1,
      -1,    -1,    -1,   612,    26,    27,    -1,    14,   598,    -1,
      17,    33,    50,    35,    -1,   605,    -1,   607,    25,    26,
      27,    -1,   612,    -1,    -1,    -1,    33,    -1,    35,    -1,
      68,    53,    56,    -1,    72,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    52,    53,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     280,    68,   671,    70,    -1,    -1,    -1,   676,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   671,   106,   121,   304,    -1,   676,   307,   112,   113,
     699,   311,    -1,   117,   703,    -1,   120,   121,   122,    -1,
      -1,    -1,   470,    -1,    -1,    -1,    -1,    -1,    -1,   699,
      -1,    -1,   119,   703,    -1,    14,     2,    -1,    17,   126,
      -1,    -1,    -1,   161,    10,    -1,    25,    26,    27,    -1,
      -1,    -1,    -1,    19,    33,   355,    35,    -1,   506,    -1,
      -1,   509,    -1,    -1,    -1,    -1,    32,   185,    -1,    -1,
      -1,    50,    -1,    52,    53,    54,    55,    -1,   767,   768,
     769,   770,    -1,    -1,    50,    -1,    -1,    -1,   206,    68,
      46,    70,    -1,   782,   783,    -1,    -1,   767,   768,   769,
     770,    -1,    68,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,   782,   783,    -1,    -1,    -1,    -1,    -1,   567,
      -1,   569,    -1,    -1,   572,    -1,     2,   575,    -1,    -1,
     578,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
     598,    -1,    -1,    -1,    -1,   121,    32,   605,    -1,   607,
      -1,    -1,   280,    -1,   612,    -1,    -1,    -1,    -1,   125,
     470,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,    -1,   307,
      -1,    -1,    68,   311,    -1,   161,    72,    -1,    -1,    -1,
     156,   157,   158,   159,    -1,    -1,   506,    -1,    -1,   509,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,   185,
      -1,    -1,    -1,   671,    -1,    -1,    -1,    -1,   676,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,   355,    -1,    -1,
     206,   197,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,   699,    -1,    -1,    -1,   703,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   567,    -1,   569,
      -1,    -1,   572,    -1,    -1,   575,    -1,    -1,   578,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   598,    -1,
      -1,    -1,    -1,    -1,    -1,   605,    -1,   607,    -1,   185,
      -1,    -1,   612,    -1,   280,    -1,    -1,    -1,    -1,   767,
     768,   769,   770,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   287,    -1,    21,   782,   783,    -1,    -1,   304,    -1,
      -1,   307,    -1,    -1,    -1,   311,    -1,    -1,    -1,    -1,
      -1,    -1,   470,     2,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    21,    -1,    -1,    -1,    -1,    56,    -1,
      19,   671,    60,    -1,    -1,    -1,   676,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,   506,   355,
      -1,   509,    -1,    -1,    -1,    -1,    -1,    -1,    56,   699,
      -1,    50,    60,   703,   280,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,    -1,    -1,    -1,   106,    68,
      -1,    -1,    -1,    72,   112,   113,    -1,    -1,   304,   117,
      -1,   307,   120,   121,   122,   311,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,    -1,    -1,    -1,   106,   567,
      -1,   569,    -1,    -1,   572,   113,    -1,   575,    -1,    -1,
     578,    -1,   120,   121,   122,    -1,    -1,   767,   768,   769,
     770,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,   355,
     598,    -1,   782,   783,    -1,    -1,    -1,   605,    21,   607,
      -1,    -1,    -1,    -1,   612,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   470,   461,   462,    -1,    -1,   465,
     466,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,
     506,    -1,    -1,   509,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,   671,    -1,    -1,    -1,   206,   676,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,    -1,   110,   111,   112,
     113,   699,   115,   116,   117,   703,    -1,   120,   121,   122,
      -1,    -1,    -1,    -1,   470,    -1,    -1,   553,   554,    -1,
      -1,   567,    -1,   569,    -1,    -1,   572,    -1,   564,   575,
     566,    -1,   578,    -1,    -1,   571,    -1,    -1,   574,    -1,
      -1,   577,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     506,   280,   598,   509,    -1,    -1,    -1,    -1,    -1,   605,
      21,   607,    -1,    -1,    -1,    -1,   612,    -1,    -1,   767,
     768,   769,   770,    -1,    -1,   304,     9,    -1,   307,    -1,
      13,    -1,   311,    -1,   782,   783,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   567,    -1,   569,    -1,    -1,   572,    -1,    -1,   575,
      -1,    -1,   578,    56,    -1,   671,   355,    60,    -1,    -1,
     676,    -1,    -1,    -1,    95,    -1,    -1,    98,    99,    -1,
     101,   102,   598,    76,    -1,   106,    -1,    80,    -1,   605,
      -1,   607,   113,   699,    -1,    -1,   612,   703,   694,   120,
     121,   122,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    -1,   120,   121,   122,
      -1,    -1,   728,   729,    12,   731,   732,   130,    -1,    17,
      -1,    -1,    -1,    21,    22,    23,    -1,    21,    -1,   745,
      -1,    29,    -1,    31,   750,   671,   752,    -1,    -1,    -1,
     676,   767,   768,   769,   770,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,   782,   783,    -1,    -1,
      -1,   470,    56,   699,    -1,    -1,    60,   703,   784,    -1,
      -1,    -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    89,    -1,    -1,    -1,    -1,    -1,   506,    -1,    -1,
     509,    95,    -1,    32,    -1,    99,    -1,   101,   102,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    50,    -1,    -1,    -1,    -1,   120,   121,   122,    -1,
      -1,   767,   768,   769,   770,    -1,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    72,   142,    -1,   782,   783,   146,    -1,
      -1,    -1,    -1,    -1,   152,    -1,   154,    -1,   567,    -1,
     569,    -1,   160,   572,    -1,   163,   575,    -1,    -1,   578,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   598,
      -1,    -1,   121,   191,    -1,    -1,   605,    -1,   607,    -1,
      -1,    -1,    -1,   612,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      -1,   219,    -1,    -1,    -1,    -1,   224,   225,   226,   227,
      -1,    23,   161,    -1,    -1,    -1,    28,    29,    30,    -1,
      32,    -1,    -1,   241,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    -1,    -1,    -1,    -1,   185,    49,    -1,    51,
      -1,    -1,   671,    -1,    -1,    -1,    -1,   676,    -1,    -1,
      -1,    63,    64,    65,    66,   273,   274,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
     699,    -1,    -1,    -1,   703,    -1,    -1,    -1,    -1,    -1,
     298,     3,   300,    -1,    96,    -1,    -1,     9,   100,    -1,
      -1,    13,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     318,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,   327,
      -1,    -1,    -1,   125,    -1,   127,   128,   129,    -1,    -1,
     338,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   280,    -1,    -1,    -1,    -1,    -1,    -1,   767,   768,
     769,   770,    -1,    -1,    -1,    -1,    -1,   365,    -1,    -1,
      -1,    -1,    -1,   782,   783,   304,    -1,    -1,   307,    81,
      82,    83,    84,    85,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,    -1,   101,
      -1,    -1,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,   117,    -1,    -1,   120,   121,
     122,   123,   124,    -1,    -1,    -1,   355,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,   451,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,   474,    45,    -1,   477,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,   497,
      68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,   470,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,   125,    -1,   127,
      -1,   129,    -1,   561,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,   506,    -1,    -1,
     509,    -1,    -1,    -1,    -1,   583,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,   630,    -1,    -1,    -1,    -1,    -1,   567,    -1,
     569,    -1,    -1,   572,    -1,    80,   575,    -1,    -1,   578,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,   598,
      -1,    -1,    -1,    -1,    -1,    -1,   605,   112,   607,   114,
      -1,   116,   117,   612,    -1,    -1,    -1,   122,    -1,    -1,
     125,    -1,   127,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   706,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   714,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    -1,    45,
     699,    -1,    48,    49,   703,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,   122,    -1,    -1,   125,
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
      -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,   118,
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
     122,    -1,    -1,   125,   126,   127,     1,   129,     3,     4,
       5,     6,     7,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    -1,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    59,    -1,    61,    62,    63,    64,
      65,    66,    -1,    68,    -1,    70,    71,    72,    -1,    -1,
      75,    -1,    77,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,   114,
      -1,   116,   117,    -1,   119,    -1,    -1,   122,    -1,    -1,
     125,   126,   127,     1,   129,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    59,    -1,    61,    62,    63,    64,    65,    66,    -1,
      68,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    77,
      -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,   114,    -1,   116,   117,
      -1,   119,    -1,    -1,   122,    -1,    -1,   125,   126,   127,
       1,   129,     3,     4,     5,     6,     7,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    70,
      71,    72,    -1,    -1,    75,    -1,    77,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,   114,    -1,   116,   117,    -1,   119,    -1,
      -1,   122,    -1,    -1,   125,   126,   127,     1,   129,     3,
       4,     5,     6,     7,    -1,    -1,    10,    11,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      -1,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    45,    -1,    -1,    48,    49,    50,    51,    52,    53,
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
     129,     3,     4,     5,     6,     7,    -1,    -1,    10,    11,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    -1,    25,    26,    27,    28,    29,    30,    -1,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,
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
       4,     5,     6,     7,    -1,     9,    10,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    23,
      -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,
      -1,    -1,    56,    -1,    -1,    -1,    60,    -1,    -1,    63,
      64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,    -1,   120,   121,   122,    -1,
      -1,   125,    -1,   127,     1,   129,     3,     4,     5,     6,
       7,    -1,     9,    10,    -1,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    60,    -1,    -1,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,   121,   122,    -1,    -1,   125,    -1,
     127,     1,   129,     3,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,    29,
      30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,
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
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    -1,    -1,     3,     4,     5,
       6,     7,    -1,   112,    10,   114,    -1,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,    -1,   125,    23,   127,    -1,
     129,   130,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,   103,   104,    -1,
       3,     4,     5,     6,     7,    -1,   112,    10,   114,    -1,
     116,   117,   118,    -1,    -1,    -1,   122,    -1,    -1,   125,
      23,   127,    -1,   129,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
     103,    -1,    -1,     3,     4,     5,     6,     7,    -1,   112,
      10,   114,    -1,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,    -1,   125,    23,   127,   128,   129,    -1,    28,    29,
      30,    -1,    32,    -1,    -1,    -1,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    -1,    -1,    -1,    -1,    48,    49,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,   103,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   112,    32,   114,    -1,   116,   117,    37,    38,
      -1,    40,   122,    42,    43,   125,    -1,   127,    -1,   129,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,   103,    -1,    -1,     3,     4,     5,
       6,     7,    -1,   112,    10,   114,    -1,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,    -1,   125,    23,   127,    -1,
     129,    -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,
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
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   114,    -1,   116,   117,    -1,   119,    -1,    -1,   122,
      -1,    -1,   125,    -1,   127,    -1,   129,     3,     4,     5,
       6,     7,    -1,    -1,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,
      -1,    -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    78,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
      -1,     3,     4,     5,     6,     7,    -1,   112,    10,   114,
      -1,   116,   117,    -1,    -1,    -1,    -1,   122,    -1,    -1,
     125,    23,   127,    -1,   129,    -1,    28,    29,    30,    -1,
      32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    49,    -1,    51,
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
      98,    -1,    -1,    -1,    -1,   103,    -1,    -1,     3,     4,
       5,     6,     7,    -1,   112,    10,   114,    -1,   116,   117,
     118,    -1,    -1,    -1,   122,    -1,    -1,   125,    23,   127,
      -1,   129,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,    -1,
      -1,     3,     4,     5,     6,     7,    -1,   112,    10,   114,
      -1,   116,   117,   118,    -1,    -1,    -1,   122,    -1,    -1,
     125,    23,   127,    -1,   129,    -1,    28,    29,    30,    -1,
      32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,
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
      -1,    -1,    80,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,   103,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   112,    32,   114,    -1,   116,   117,
      37,    38,    -1,    40,   122,    42,    43,   125,    -1,   127,
      -1,   129,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      -1,    -1,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    28,    29,    30,   112,
      32,   114,    -1,    -1,   117,    37,    38,    -1,    40,   122,
      42,    43,   125,    -1,   127,    -1,   129,    49,     9,    51,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    63,    64,    65,    66,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    96,    56,    13,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    21,    -1,    -1,   123,   124,    -1,
      -1,   127,   114,   129,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,   127,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    56,
     101,   102,   103,    60,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    -1,   120,
     121,   122,    -1,    80,     9,    -1,    -1,    -1,    13,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    -1,   120,   121,   122,    -1,    -1,     9,    -1,
      -1,    56,    13,   130,    -1,    60,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    -1,    97,    98,    99,    56,   101,   102,   103,    60,
     105,   106,   107,   108,   109,   110,   111,   112,   113,    -1,
     115,   116,   117,    -1,    -1,   120,   121,   122,    -1,    80,
      -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,     9,   115,   116,   117,    13,    -1,   120,
     121,   122,    -1,    -1,    -1,    21,    22,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      56,    13,    -1,    -1,    60,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    56,   101,   102,   103,    60,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    -1,   120,   121,   122,     9,    80,   125,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    -1,   125,    56,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,     9,    -1,    -1,    80,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    -1,   115,   116,   117,    -1,    -1,   120,   121,
     122,    -1,    56,   125,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    56,   120,   121,   122,    60,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    56,   120,
     121,   122,    60,    -1,   125,    -1,    -1,    -1,    -1,    -1,
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
      67,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    56,   120,   121,   122,    60,    -1,   125,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    -1,    56,   120,   121,   122,    60,
      -1,   125,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    56,   115,   116,   117,    60,    -1,   120,
     121,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    56,   120,   121,   122,
      60,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,     9,    -1,
      80,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      31,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    -1,
     120,   121,   122,    -1,    -1,    56,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    -1,   115,   116,   117,    -1,    56,   120,
     121,   122,    60,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    56,   115,   116,   117,
      60,    -1,   120,   121,   122,    -1,    -1,     9,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    79,    80,    -1,    -1,    -1,    21,    -1,
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
      60,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
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
      -1,    13,    -1,    -1,    -1,    -1,    76,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,    -1,
     120,   121,   122,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    80,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,   108,   109,   110,   111,
     112,   113,    56,   115,   116,   117,    60,   119,   120,   121,
     122,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      56,   115,   116,   117,    60,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    79,    80,    -1,    -1,    -1,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    97,    98,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,   108,   109,   110,   111,   112,   113,    -1,   115,
     116,   117,    -1,    56,   120,   121,   122,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    97,    98,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    -1,   115,   116,   117,    -1,    56,   120,   121,   122,
      60,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    56,   115,   116,   117,    60,    -1,
     120,   121,   122,    -1,    -1,    67,    -1,    -1,    -1,    -1,
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
     110,   111,   112,   113,    -1,   115,   116,   117,    -1,    56,
     120,   121,   122,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,   115,   116,
     117,    -1,    56,   120,   121,   122,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,   109,   110,   111,   112,   113,
      -1,   115,   116,   117,    21,    56,   120,   121,   122,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    60,    95,    -1,    97,    98,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,   108,   109,   110,
     111,   112,   113,    80,   115,   116,   117,    21,    56,   120,
     121,   122,    60,    -1,    -1,    -1,    -1,    -1,    95,    -1,
      97,    98,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,   108,    80,   110,   111,   112,   113,    -1,   115,    -1,
     117,    21,    56,   120,   121,   122,    60,    95,    -1,    97,
      98,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
     108,   109,   110,   111,   112,   113,    -1,   115,   116,   117,
      -1,    -1,   120,   121,   122,    -1,    56,    -1,    -1,    -1,
      60,    95,    -1,    97,    98,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,   108,    -1,   110,   111,   112,   113,
      -1,   115,    -1,   117,    -1,    -1,   120,   121,   122,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    97,    98,    99,
      -1,   101,   102,   103,    -1,    -1,   106,   107,   108,    -1,
     110,   111,   112,   113,    -1,    -1,    -1,   117,    -1,    -1,
     120,   121,   122
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
     165,   168,   171,   172,   192,   195,   196,   214,   215,   216,
     217,   218,   219,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   235,   236,   237,   238,   239,   139,   227,
      76,   220,   221,   147,   148,   220,    10,    28,    29,    30,
      66,   125,   129,   168,   214,   218,   225,   226,   227,   228,
     230,   231,    68,   147,   227,   139,   127,   148,     7,   147,
     149,     8,    68,   149,    48,    78,   156,   227,   227,   227,
     127,   148,   169,   127,   148,   197,   198,   139,   227,   227,
     227,   227,     7,   127,    17,    27,   153,   153,   118,   188,
     208,   227,   119,   227,   227,    22,   142,   150,   227,   227,
      65,   127,   139,   227,   148,   193,   208,   227,   227,   227,
     227,   227,   227,   227,   126,   137,   143,   208,    69,   104,
     188,   209,   210,   227,   208,   227,   234,    50,   139,    41,
     148,    35,    53,   184,    18,    48,    57,    72,   119,   124,
       9,    13,    21,    56,    60,    80,    95,    97,    98,    99,
     101,   102,   103,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   115,   116,   117,   120,   121,   122,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,   123,   127,   129,    56,    60,   127,   139,   119,
     125,   142,   227,   227,   227,   208,    31,   220,   193,   119,
     119,    75,   148,   188,   211,   212,   213,   227,   125,   193,
     162,   148,   127,   150,    22,    31,   150,    22,    31,   150,
     221,    67,   142,   211,   139,   127,   179,    69,   127,   148,
     199,   200,     1,    99,   202,   203,    31,   100,   150,   211,
     149,   148,   100,   119,   119,   125,   139,   150,   127,   211,
      81,   194,   119,   119,   150,   119,   126,   137,   126,   227,
     100,   128,   100,   128,    31,   130,   221,    79,   100,   130,
       7,   148,    99,   163,   173,    57,   197,   197,   197,   197,
     227,   227,   227,   227,   156,   227,   156,   227,   227,   227,
     227,   227,   227,   227,    23,    68,   148,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   211,   211,
     156,   227,   156,   227,    18,    31,    34,    47,    48,    57,
      68,   117,   182,   222,   223,   239,    22,    31,    22,    31,
      67,    31,   130,   156,   227,   150,   119,   227,    79,    81,
     128,   100,   148,   166,   167,   119,   148,    31,   208,    30,
     227,   156,   227,    30,   227,   156,   227,   150,   139,   227,
      24,   128,   180,   181,   182,   170,   200,   100,   128,     1,
     129,   204,   215,    79,    81,   201,   227,   198,   128,   188,
     227,   159,   211,   128,   129,   204,   215,   100,   128,   128,
     209,   209,   156,   227,   139,   227,   130,   227,   227,   125,
     208,   125,    48,    57,    68,   175,   183,   197,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   128,   130,    31,
      57,   214,   100,   128,    56,    30,   156,   227,    30,   156,
     227,   156,   227,   220,   220,   119,   227,   188,   227,   213,
      81,   100,   126,   163,   227,   128,   227,    22,   150,    22,
     150,   227,    22,   150,   221,    22,   150,   221,    24,    24,
     139,   100,   128,   127,   148,    18,    48,    57,    68,   185,
     128,   200,    99,   203,   208,   227,    44,   227,    46,    73,
     126,   160,   128,   208,   193,   130,   221,   130,   221,   139,
      79,   126,   143,   164,   174,     9,    13,    81,    82,    83,
      84,    85,    86,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   101,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   117,   120,   121,   122,   123,
     124,   148,   176,   177,   119,   223,   214,   227,    22,    22,
     227,    22,    22,   130,   130,   150,   150,   227,   167,   125,
     150,    67,    30,   227,    30,   227,    67,    30,   227,   150,
      30,   227,   150,   139,   227,   181,   200,    99,   207,   203,
     201,    31,   130,   139,   208,   130,    30,   139,   227,   130,
      30,   139,   227,   130,   227,   126,    27,    52,    54,   126,
     138,   152,   185,   148,   102,   127,   178,   178,    67,    30,
      30,    67,    30,    30,    30,    30,   164,   227,   227,   227,
     227,   227,   227,   128,    23,    63,    66,   129,   188,   206,
     215,   104,   190,   201,    74,   191,   227,   204,   215,   150,
     227,   139,   227,   139,   152,   203,   148,   176,   177,   180,
     227,   227,   227,   227,   227,   227,   126,    67,    67,    67,
      67,   207,   130,   208,   188,   189,   227,   227,   142,   151,
     187,   130,    67,    67,   191,   178,   178,   128,    67,    67,
      67,    67,    67,    67,   227,   227,   227,   227,   190,   201,
     188,   205,   206,   215,    31,   130,   215,   227,   227,   119,
     186,   187,   227,   205,   206,   130,   205
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
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 500 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 522 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 532 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 544 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 557 "chapel.ypp"
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
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 594 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 600 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 606 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 619 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 659 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 671 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 676 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 681 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 697 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 704 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 714 "chapel.ypp"
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
#line 723 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 727 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 734 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 740 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 745 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 750 "chapel.ypp"
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
#line 770 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 776 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
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
#line 791 "chapel.ypp"
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
#line 802 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 808 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 815 "chapel.ypp"
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
#line 837 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 841 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 846 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 850 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 858 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 859 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 860 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 861 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 888 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 889 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 966 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
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
#line 1015 "chapel.ypp"
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
#line 1031 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1035 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1061 "chapel.ypp"
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
#line 1069 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1085 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1095 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1101 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1108 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1117 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1125 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1126 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1127 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1133 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1137 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1139 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1169 "chapel.ypp"
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
#line 1176 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1195 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1206 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1207 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1217 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1270 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1303 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1309 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1316 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1322 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1328 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1347 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1387 "chapel.ypp"
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
#line 1396 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1402 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1430 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1432 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1434 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1436 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_partially_bounded_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1461 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1462 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1469 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1478 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1479 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1497 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1505 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1506 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1554 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7431 "bison-chapel.cpp"
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



