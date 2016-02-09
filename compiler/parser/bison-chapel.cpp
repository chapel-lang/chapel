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
#line 120 "chapel.ypp"

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
#line 142 "chapel.ypp"

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
#line 224 "bison-chapel.cpp"

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
     TEXCEPT = 282,
     TEXPORT = 283,
     TEXTERN = 284,
     TFOR = 285,
     TFORALL = 286,
     TIF = 287,
     TIN = 288,
     TINDEX = 289,
     TINLINE = 290,
     TINOUT = 291,
     TITER = 292,
     TLABEL = 293,
     TLAMBDA = 294,
     TLET = 295,
     TLOCAL = 296,
     TMINUSMINUS = 297,
     TMODULE = 298,
     TNEW = 299,
     TNIL = 300,
     TNOINIT = 301,
     TON = 302,
     TONLY = 303,
     TOTHERWISE = 304,
     TOUT = 305,
     TPARAM = 306,
     TPLUSPLUS = 307,
     TPRAGMA = 308,
     TPRIMITIVE = 309,
     TPRIVATE = 310,
     TPROC = 311,
     TPUBLIC = 312,
     TRECORD = 313,
     TREDUCE = 314,
     TREF = 315,
     TREQUIRE = 316,
     TRETURN = 317,
     TSCAN = 318,
     TSELECT = 319,
     TSERIAL = 320,
     TSINGLE = 321,
     TSPARSE = 322,
     TSUBDOMAIN = 323,
     TSYNC = 324,
     TTHEN = 325,
     TTYPE = 326,
     TUNDERSCORE = 327,
     TUNION = 328,
     TUSE = 329,
     TVAR = 330,
     TWHEN = 331,
     TWHERE = 332,
     TWHILE = 333,
     TWITH = 334,
     TYIELD = 335,
     TZIP = 336,
     TALIAS = 337,
     TAND = 338,
     TASSIGN = 339,
     TASSIGNBAND = 340,
     TASSIGNBOR = 341,
     TASSIGNBXOR = 342,
     TASSIGNDIVIDE = 343,
     TASSIGNEXP = 344,
     TASSIGNLAND = 345,
     TASSIGNLOR = 346,
     TASSIGNMINUS = 347,
     TASSIGNMOD = 348,
     TASSIGNMULTIPLY = 349,
     TASSIGNPLUS = 350,
     TASSIGNSL = 351,
     TASSIGNSR = 352,
     TBAND = 353,
     TBNOT = 354,
     TBOR = 355,
     TBXOR = 356,
     TCOLON = 357,
     TCOMMA = 358,
     TDIVIDE = 359,
     TDOT = 360,
     TDOTDOT = 361,
     TDOTDOTDOT = 362,
     TEQUAL = 363,
     TEXP = 364,
     TGREATER = 365,
     TGREATEREQUAL = 366,
     THASH = 367,
     TLESS = 368,
     TLESSEQUAL = 369,
     TMINUS = 370,
     TMOD = 371,
     TNOT = 372,
     TNOTEQUAL = 373,
     TOR = 374,
     TPLUS = 375,
     TQUESTION = 376,
     TSEMI = 377,
     TSHIFTLEFT = 378,
     TSHIFTRIGHT = 379,
     TSTAR = 380,
     TSWAP = 381,
     TIO = 382,
     TLCBR = 383,
     TRCBR = 384,
     TLP = 385,
     TRP = 386,
     TLSBR = 387,
     TRSBR = 388,
     TNOELSE = 389,
     TUMINUS = 390,
     TUPLUS = 391
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
#line 173 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 428 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 434 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  int         captureTokens;
  char        captureString[1024];


/* Line 344 of yacc.c  */
#line 181 "chapel.ypp"

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
#line 508 "bison-chapel.cpp"

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
#define YYLAST   11132

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  106
/* YYNRULES -- Number of rules.  */
#define YYNRULES  432
/* YYNRULES -- Number of states.  */
#define YYNSTATES  826

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   391

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
     135,   136
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
     114,   117,   121,   123,   126,   130,   136,   142,   146,   151,
     156,   161,   166,   171,   176,   181,   186,   191,   196,   201,
     206,   211,   216,   221,   222,   224,   226,   228,   230,   233,
     235,   238,   242,   244,   246,   249,   252,   254,   256,   258,
     260,   262,   265,   271,   275,   282,   289,   294,   300,   306,
     310,   314,   321,   327,   334,   340,   347,   351,   356,   363,
     371,   378,   386,   391,   397,   402,   407,   411,   418,   424,
     430,   431,   434,   438,   441,   448,   457,   459,   461,   463,
     464,   467,   468,   471,   475,   481,   483,   486,   490,   492,
     496,   497,   498,   507,   508,   510,   513,   516,   517,   518,
     528,   532,   536,   542,   548,   550,   553,   555,   557,   559,
     561,   563,   565,   567,   569,   571,   573,   575,   577,   579,
     581,   583,   585,   587,   589,   591,   593,   595,   597,   599,
     601,   603,   605,   607,   609,   611,   613,   615,   617,   619,
     621,   623,   625,   626,   630,   634,   635,   637,   641,   646,
     651,   658,   665,   666,   668,   670,   672,   674,   677,   680,
     682,   684,   686,   687,   689,   691,   693,   695,   697,   698,
     700,   703,   705,   707,   709,   711,   713,   715,   717,   720,
     722,   723,   725,   728,   731,   732,   735,   739,   744,   749,
     752,   757,   758,   761,   764,   769,   774,   779,   785,   790,
     791,   793,   795,   797,   801,   805,   810,   816,   818,   820,
     824,   826,   829,   833,   834,   837,   840,   841,   846,   847,
     850,   853,   855,   860,   865,   872,   874,   875,   877,   879,
     883,   888,   892,   897,   904,   905,   908,   911,   914,   917,
     920,   923,   925,   927,   931,   935,   937,   939,   941,   945,
     949,   950,   952,   954,   958,   962,   966,   970,   972,   974,
     976,   978,   980,   982,   984,   986,   989,   994,   999,  1004,
    1010,  1013,  1016,  1023,  1030,  1035,  1045,  1055,  1063,  1070,
    1077,  1082,  1092,  1102,  1110,  1115,  1122,  1129,  1139,  1149,
    1156,  1158,  1160,  1162,  1164,  1166,  1168,  1170,  1172,  1176,
    1177,  1179,  1184,  1186,  1190,  1193,  1197,  1199,  1203,  1206,
    1211,  1213,  1215,  1217,  1219,  1221,  1223,  1225,  1227,  1232,
    1236,  1240,  1243,  1246,  1248,  1250,  1252,  1254,  1256,  1258,
    1260,  1265,  1270,  1275,  1279,  1283,  1287,  1291,  1296,  1300,
    1302,  1304,  1306,  1308,  1310,  1314,  1318,  1322,  1326,  1332,
    1336,  1340,  1344,  1348,  1352,  1356,  1360,  1364,  1368,  1372,
    1376,  1380,  1384,  1388,  1392,  1396,  1400,  1404,  1408,  1412,
    1416,  1420,  1424,  1427,  1430,  1433,  1436,  1439,  1442,  1446,
    1450,  1454,  1458,  1462,  1466,  1470,  1474,  1476,  1478,  1480,
    1482,  1484,  1486
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     138,     0,    -1,   139,    -1,    -1,   139,   140,    -1,   142,
      -1,   141,   142,    -1,    53,     7,    -1,   141,    53,     7,
      -1,   143,    -1,   145,    -1,   147,    -1,   148,    -1,   155,
      -1,   149,    -1,   157,    -1,   160,    -1,   158,    -1,   161,
      -1,   154,    -1,   222,   122,    -1,    11,   142,    -1,    12,
     223,   142,    -1,    13,   150,   122,    -1,    16,   223,   145,
      -1,    20,   150,   122,    -1,    21,   230,   122,    -1,    38,
     151,   142,    -1,    41,   142,    -1,    47,   230,   153,    -1,
      65,   230,   153,    -1,    65,   153,    -1,    69,   142,    -1,
      80,   230,   122,    -1,     1,    -1,   144,    43,   151,   128,
     129,    -1,   144,    43,   151,   128,   146,   129,    -1,    -1,
      57,    -1,    55,    -1,   128,   129,    -1,   128,   146,   129,
      -1,   140,    -1,   146,   140,    -1,    74,   211,   122,    -1,
      74,   230,    27,   211,   122,    -1,    74,   230,    48,   211,
     122,    -1,    61,   211,   122,    -1,   231,    84,   230,   122,
      -1,   231,    95,   230,   122,    -1,   231,    92,   230,   122,
      -1,   231,    94,   230,   122,    -1,   231,    88,   230,   122,
      -1,   231,    93,   230,   122,    -1,   231,    89,   230,   122,
      -1,   231,    85,   230,   122,    -1,   231,    86,   230,   122,
      -1,   231,    87,   230,   122,    -1,   231,    97,   230,   122,
      -1,   231,    96,   230,   122,    -1,   231,   126,   230,   122,
      -1,   231,    90,   230,   122,    -1,   231,    91,   230,   122,
      -1,    -1,   151,    -1,     3,    -1,   150,    -1,     7,    -1,
      23,   142,    -1,   145,    -1,    62,   122,    -1,    62,   230,
     122,    -1,   122,    -1,   156,    -1,    57,   156,    -1,    55,
     156,    -1,   168,    -1,   195,    -1,   164,    -1,   175,    -1,
     198,    -1,    29,     9,    -1,    23,   142,    78,   230,   122,
      -1,    78,   230,   153,    -1,    17,   230,    33,   230,   223,
     153,    -1,    17,   230,    33,   159,   223,   153,    -1,    17,
     230,   223,   153,    -1,    30,   230,    33,   230,   153,    -1,
      30,   230,    33,   159,   153,    -1,    30,   230,   153,    -1,
      30,   159,   153,    -1,    30,    51,   151,    33,   230,   153,
      -1,    31,   230,    33,   230,   153,    -1,    31,   230,    33,
     230,   224,   153,    -1,    31,   230,    33,   159,   153,    -1,
      31,   230,    33,   159,   224,   153,    -1,    31,   230,   153,
      -1,    31,   230,   224,   153,    -1,   132,   211,    33,   230,
     133,   142,    -1,   132,   211,    33,   230,   224,   133,   142,
      -1,   132,   211,    33,   159,   133,   142,    -1,   132,   211,
      33,   159,   224,   133,   142,    -1,   132,   211,   133,   142,
      -1,   132,   211,   224,   133,   142,    -1,    81,   130,   211,
     131,    -1,    32,   230,    70,   142,    -1,    32,   230,   145,
      -1,    32,   230,    70,   142,    25,   142,    -1,    32,   230,
     145,    25,   142,    -1,    64,   230,   128,   162,   129,    -1,
      -1,   162,   163,    -1,    76,   211,   153,    -1,    49,   142,
      -1,   165,   151,   166,   128,   167,   129,    -1,    29,   152,
     165,   151,   166,   128,   167,   129,    -1,    15,    -1,    58,
      -1,    73,    -1,    -1,   102,   211,    -1,    -1,   167,   155,
      -1,   167,   141,   155,    -1,    26,   151,   128,   169,   129,
      -1,   170,    -1,   169,   103,    -1,   169,   103,   170,    -1,
     151,    -1,   151,    84,   230,    -1,    -1,    -1,    39,   172,
     182,   173,   188,   206,   194,   190,    -1,    -1,    35,    -1,
      28,   152,    -1,    29,   152,    -1,    -1,    -1,   174,   187,
     176,   178,   177,   188,   206,   194,   189,    -1,   186,   179,
     181,    -1,   186,   180,   181,    -1,   186,   151,   105,   179,
     181,    -1,   186,   151,   105,   180,   181,    -1,   151,    -1,
      99,   151,    -1,    98,    -1,   100,    -1,   101,    -1,    99,
      -1,   108,    -1,   118,    -1,   114,    -1,   111,    -1,   113,
      -1,   110,    -1,   120,    -1,   115,    -1,   125,    -1,   104,
      -1,   123,    -1,   124,    -1,   116,    -1,   109,    -1,   117,
      -1,    14,    -1,   112,    -1,    10,    -1,   126,    -1,   127,
      -1,    84,    -1,    95,    -1,    92,    -1,    94,    -1,    88,
      -1,    93,    -1,    89,    -1,    85,    -1,    86,    -1,    87,
      -1,    97,    -1,    96,    -1,    -1,   130,   183,   131,    -1,
     130,   183,   131,    -1,    -1,   184,    -1,   183,   103,   184,
      -1,   185,   151,   210,   204,    -1,   185,   151,   210,   193,
      -1,   185,   130,   203,   131,   210,   204,    -1,   185,   130,
     203,   131,   210,   193,    -1,    -1,    33,    -1,    36,    -1,
      50,    -1,    19,    -1,    19,    33,    -1,    19,    60,    -1,
      51,    -1,    60,    -1,    71,    -1,    -1,    51,    -1,    60,
      -1,    71,    -1,    56,    -1,    37,    -1,    -1,    19,    -1,
      19,    60,    -1,    60,    -1,    51,    -1,    71,    -1,   122,
      -1,   190,    -1,   145,    -1,   154,    -1,   121,   151,    -1,
     121,    -1,    -1,   191,    -1,   107,   230,    -1,   107,   192,
      -1,    -1,    77,   230,    -1,    71,   196,   122,    -1,    18,
      71,   196,   122,    -1,    29,    71,   196,   122,    -1,   151,
     197,    -1,   151,   197,   103,   196,    -1,    -1,    84,   218,
      -1,    84,   207,    -1,   199,    51,   200,   122,    -1,   199,
      19,   200,   122,    -1,   199,    60,   200,   122,    -1,   199,
      19,    60,   200,   122,    -1,   199,    75,   200,   122,    -1,
      -1,    18,    -1,    29,    -1,   201,    -1,   200,   103,   201,
      -1,   151,   206,   204,    -1,   151,   205,    82,   230,    -1,
     130,   203,   131,   206,   204,    -1,    72,    -1,   151,    -1,
     130,   203,   131,    -1,   202,    -1,   202,   103,    -1,   202,
     103,   203,    -1,    -1,    84,    46,    -1,    84,   230,    -1,
      -1,   102,   132,   211,   133,    -1,    -1,   102,   218,    -1,
     102,   207,    -1,     1,    -1,   132,   211,   133,   218,    -1,
     132,   211,   133,   207,    -1,   132,   211,    33,   230,   133,
     218,    -1,     1,    -1,    -1,   218,    -1,   191,    -1,   132,
     133,   208,    -1,   132,   211,   133,   208,    -1,   132,   133,
     209,    -1,   132,   211,   133,   209,    -1,   132,   211,    33,
     230,   133,   208,    -1,    -1,   102,   218,    -1,   102,   191,
      -1,   102,    24,    -1,   102,    66,    -1,   102,    69,    -1,
     102,   209,    -1,   230,    -1,   191,    -1,   211,   103,   230,
      -1,   211,   103,   191,    -1,    72,    -1,   230,    -1,   191,
      -1,   212,   103,   212,    -1,   213,   103,   212,    -1,    -1,
     215,    -1,   216,    -1,   215,   103,   216,    -1,   151,    84,
     191,    -1,   151,    84,   230,    -1,   151,    82,   230,    -1,
     191,    -1,   230,    -1,   151,    -1,   221,    -1,   231,    -1,
     220,    -1,   239,    -1,   238,    -1,    66,   230,    -1,    34,
     130,   214,   131,    -1,    24,   130,   214,   131,    -1,    68,
     130,   214,   131,    -1,    67,    68,   130,   214,   131,    -1,
      11,   230,    -1,    69,   230,    -1,    30,   230,    33,   230,
      23,   230,    -1,    30,   230,    33,   159,    23,   230,    -1,
      30,   230,    23,   230,    -1,    30,   230,    33,   230,    23,
      32,   230,    70,   230,    -1,    30,   230,    33,   159,    23,
      32,   230,    70,   230,    -1,    30,   230,    23,    32,   230,
      70,   230,    -1,    31,   230,    33,   230,    23,   230,    -1,
      31,   230,    33,   159,    23,   230,    -1,    31,   230,    23,
     230,    -1,    31,   230,    33,   230,    23,    32,   230,    70,
     230,    -1,    31,   230,    33,   159,    23,    32,   230,    70,
     230,    -1,    31,   230,    23,    32,   230,    70,   230,    -1,
     132,   211,   133,   230,    -1,   132,   211,    33,   230,   133,
     230,    -1,   132,   211,    33,   159,   133,   230,    -1,   132,
     211,    33,   230,   133,    32,   230,    70,   230,    -1,   132,
     211,    33,   159,   133,    32,   230,    70,   230,    -1,    32,
     230,    70,   230,    25,   230,    -1,    45,    -1,   221,    -1,
     217,    -1,   234,    -1,   233,    -1,   171,    -1,   228,    -1,
     229,    -1,   227,   127,   230,    -1,    -1,   224,    -1,    79,
     130,   225,   131,    -1,   226,    -1,   225,   103,   226,    -1,
     185,   217,    -1,   242,    59,   217,    -1,   231,    -1,   227,
     127,   230,    -1,    44,   230,    -1,    40,   200,    33,   230,
      -1,   236,    -1,   218,    -1,   219,    -1,   240,    -1,   241,
      -1,   171,    -1,   228,    -1,   229,    -1,   130,   107,   230,
     131,    -1,   230,   102,   230,    -1,   230,   106,   230,    -1,
     230,   106,    -1,   106,   230,    -1,   106,    -1,   217,    -1,
     233,    -1,   234,    -1,   235,    -1,   231,    -1,   171,    -1,
     232,   130,   214,   131,    -1,   232,   132,   214,   133,    -1,
      54,   130,   214,   131,    -1,   230,   105,   151,    -1,   230,
     105,    71,    -1,   230,   105,    24,    -1,   130,   212,   131,
      -1,   130,   212,   103,   131,    -1,   130,   213,   131,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
     128,   211,   129,    -1,   132,   211,   133,    -1,   132,   237,
     133,    -1,   230,    82,   230,    -1,   237,   103,   230,    82,
     230,    -1,   230,   120,   230,    -1,   230,   115,   230,    -1,
     230,   125,   230,    -1,   230,   104,   230,    -1,   230,   123,
     230,    -1,   230,   124,   230,    -1,   230,   116,   230,    -1,
     230,   108,   230,    -1,   230,   118,   230,    -1,   230,   114,
     230,    -1,   230,   111,   230,    -1,   230,   113,   230,    -1,
     230,   110,   230,    -1,   230,    98,   230,    -1,   230,   100,
     230,    -1,   230,   101,   230,    -1,   230,    83,   230,    -1,
     230,   119,   230,    -1,   230,   109,   230,    -1,   230,    14,
     230,    -1,   230,    10,   230,    -1,   230,   112,   230,    -1,
     230,    22,   230,    -1,   120,   230,    -1,   115,   230,    -1,
      42,   230,    -1,    52,   230,    -1,   117,   230,    -1,    99,
     230,    -1,   230,    59,   230,    -1,   230,    59,   159,    -1,
     242,    59,   230,    -1,   242,    59,   159,    -1,   230,    63,
     230,    -1,   230,    63,   159,    -1,   242,    63,   230,    -1,
     242,    63,   159,    -1,   120,    -1,   125,    -1,    83,    -1,
     119,    -1,    98,    -1,   100,    -1,   101,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   404,   404,   409,   410,   416,   417,   422,   423,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   457,   459,   464,   465,   466,
     481,   482,   487,   488,   493,   494,   495,   499,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   521,   522,   526,   530,   531,   535,   536,
     540,   541,   545,   546,   547,   548,   549,   550,   551,   555,
     556,   560,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   597,
     603,   609,   615,   622,   632,   636,   637,   638,   639,   643,
     648,   649,   653,   655,   662,   667,   675,   680,   685,   693,
     694,   699,   700,   702,   707,   717,   726,   730,   738,   739,
     744,   749,   743,   774,   780,   787,   795,   806,   812,   805,
     840,   844,   849,   853,   861,   862,   863,   864,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   901,   905,   906,   910,   914,   915,   916,   920,   922,
     924,   926,   931,   932,   933,   934,   935,   936,   937,   938,
     939,   940,   944,   945,   946,   947,   951,   952,   956,   957,
     958,   959,   960,   961,   965,   966,   970,   971,   975,   977,
     982,   983,   987,   988,   992,   993,   997,   999,  1001,  1006,
    1019,  1036,  1037,  1039,  1044,  1051,  1058,  1065,  1073,  1082,
    1083,  1084,  1088,  1089,  1097,  1099,  1105,  1110,  1112,  1114,
    1119,  1121,  1123,  1130,  1131,  1132,  1136,  1137,  1142,  1143,
    1144,  1145,  1165,  1169,  1173,  1181,  1185,  1186,  1187,  1191,
    1193,  1199,  1201,  1203,  1208,  1209,  1210,  1211,  1212,  1213,
    1214,  1220,  1221,  1222,  1223,  1227,  1228,  1229,  1233,  1234,
    1238,  1239,  1243,  1244,  1248,  1249,  1250,  1251,  1252,  1256,
    1267,  1268,  1269,  1270,  1271,  1272,  1274,  1276,  1278,  1280,
    1282,  1284,  1289,  1291,  1293,  1295,  1297,  1299,  1301,  1303,
    1305,  1307,  1309,  1311,  1313,  1320,  1326,  1332,  1338,  1347,
    1352,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1367,  1372,
    1373,  1377,  1381,  1383,  1391,  1401,  1405,  1406,  1411,  1416,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1434,
    1436,  1438,  1440,  1442,  1447,  1448,  1449,  1450,  1461,  1462,
    1466,  1467,  1468,  1472,  1473,  1474,  1482,  1483,  1484,  1488,
    1489,  1490,  1491,  1492,  1493,  1494,  1495,  1502,  1503,  1507,
    1508,  1509,  1510,  1511,  1512,  1513,  1514,  1515,  1516,  1517,
    1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,  1526,  1527,
    1528,  1529,  1533,  1534,  1535,  1536,  1537,  1538,  1542,  1543,
    1544,  1545,  1549,  1550,  1551,  1552,  1557,  1558,  1559,  1560,
    1561,  1562,  1563
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
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFOR",
  "TFORALL", "TIF", "TIN", "TINDEX", "TINLINE", "TINOUT", "TITER",
  "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS", "TMODULE", "TNEW",
  "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE", "TOUT", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPUBLIC",
  "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT",
  "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE",
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
     385,   386,   387,   388,   389,   390,   391
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   137,   138,   139,   139,   140,   140,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   143,   143,   144,   144,   144,
     145,   145,   146,   146,   147,   147,   147,   148,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   150,   150,   151,   152,   152,   153,   153,
     154,   154,   155,   155,   155,   155,   155,   155,   155,   156,
     156,   157,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   159,   160,   160,   160,   160,   161,
     162,   162,   163,   163,   164,   164,   165,   165,   165,   166,
     166,   167,   167,   167,   168,   169,   169,   169,   170,   170,
     172,   173,   171,   174,   174,   174,   174,   176,   177,   175,
     178,   178,   178,   178,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   181,   181,   182,   183,   183,   183,   184,   184,
     184,   184,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   186,   186,   187,   187,   188,   188,
     188,   188,   188,   188,   189,   189,   190,   190,   191,   191,
     192,   192,   193,   193,   194,   194,   195,   195,   195,   196,
     196,   197,   197,   197,   198,   198,   198,   198,   198,   199,
     199,   199,   200,   200,   201,   201,   201,   202,   202,   202,
     203,   203,   203,   204,   204,   204,   205,   205,   206,   206,
     206,   206,   207,   207,   207,   207,   208,   208,   208,   209,
     209,   209,   209,   209,   210,   210,   210,   210,   210,   210,
     210,   211,   211,   211,   211,   212,   212,   212,   213,   213,
     214,   214,   215,   215,   216,   216,   216,   216,   216,   217,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   220,
     221,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     223,   224,   225,   225,   226,   226,   227,   227,   228,   229,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   231,   231,   231,   231,   232,   232,
     233,   233,   233,   234,   234,   234,   235,   235,   235,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   237,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   239,   239,   239,   239,   239,   239,   240,   240,
     240,   240,   241,   241,   241,   241,   242,   242,   242,   242,
     242,   242,   242
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     3,     5,     5,     3,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     4,     3,     6,     5,     5,
       0,     2,     3,     2,     6,     8,     1,     1,     1,     0,
       2,     0,     2,     3,     5,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     0,     0,     9,
       3,     3,     5,     5,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     4,
       6,     6,     0,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     0,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     1,
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
       1,     4,     1,     3,     2,     3,     1,     3,     2,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       3,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     4,     3,     1,
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
       3,     0,     0,     1,    34,    65,   379,   380,   381,   382,
     383,     0,   339,    63,   116,   339,     0,   240,    63,     0,
       0,     0,     0,    63,    63,     0,     0,     0,     0,   134,
       0,   130,     0,     0,     0,     0,   330,     0,     0,     0,
       0,   239,   239,   117,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,   428,   430,     0,
     431,   432,   363,     0,     0,   429,   426,    72,   427,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   299,    19,    13,    73,    15,    17,    16,    18,    78,
       0,    76,   355,     0,    79,    77,    80,     0,   364,   351,
     352,   302,   300,     0,     0,   356,   357,     0,   301,     0,
     365,   366,   367,   350,   304,   303,   353,   354,     0,    21,
     310,     0,     0,   340,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,   355,   364,   300,   356,   357,   339,
     301,   365,   366,     0,     0,     0,     0,   290,     0,    67,
      66,   135,    81,     0,   136,     0,     0,     0,     0,     0,
       0,   290,     0,     0,     0,     0,     0,   242,    28,   414,
     348,     0,   415,     7,   290,   240,   241,    75,    74,   219,
     282,     0,   281,    70,     0,     0,     0,    69,    31,     0,
     305,     0,   290,    32,   311,   231,     0,     0,   281,     0,
       0,   417,   362,   413,   416,   412,    40,    42,     0,     0,
     285,     0,   287,     0,     0,   286,     0,   281,     0,     0,
       6,     0,   119,   207,   206,   137,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   290,   290,     0,     0,   192,
      22,    23,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,   299,   297,     0,   291,   292,   298,
       0,     0,     0,     0,     0,    90,     0,     0,    89,     0,
       0,    96,     0,     0,   106,     0,    27,   185,   131,   247,
       0,   248,   250,     0,   261,     0,     0,   253,     0,     0,
      29,     0,   136,   218,     0,    47,    71,   110,    68,    30,
     290,     0,     0,   229,   226,    44,     0,     0,    83,    33,
      41,    43,   384,     0,     0,   376,     0,   378,     0,     0,
       0,     0,     0,   386,     8,     0,     0,     0,   202,     0,
       0,     0,     0,     0,   338,   409,   408,   411,   419,   418,
     423,   422,   405,   402,   403,   404,   359,   392,   375,   374,
     373,   360,   396,   407,   401,   399,   410,   400,   398,   390,
     395,   397,   406,   389,   393,   394,   391,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   421,   420,   425,   424,   196,   193,
     194,   195,   199,   200,   201,   426,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,   385,   339,   339,    86,
     227,     0,     0,     0,   307,     0,   128,     0,   125,   228,
     119,     0,     0,     0,   314,     0,     0,     0,   320,     0,
       0,    97,   105,     0,     0,   306,     0,   186,     0,   208,
       0,   251,     0,   265,     0,   260,   351,     0,     0,   244,
     349,   243,   372,   284,   283,     0,     0,   308,     0,   233,
     351,     0,     0,     0,   358,   377,   288,   289,     0,     0,
     102,   324,     0,   387,     0,     0,   120,   121,   203,   204,
     205,   138,     0,     0,   235,   234,   236,   238,    48,    55,
      56,    57,    52,    54,    61,    62,    50,    53,    51,    49,
      59,    58,    60,   370,   371,   197,   198,   344,   192,   341,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,   296,   294,   295,   293,     0,   126,   124,     0,
       0,   104,     0,     0,    88,     0,    87,     0,     0,    94,
       0,     0,    92,     0,     0,     0,   108,   192,   184,     0,
     274,   209,   212,   211,   213,     0,   249,   252,     0,   253,
       0,   245,   254,   255,     0,     0,   109,   111,   309,     0,
     230,    45,    46,     0,     0,     0,     0,   103,     0,    35,
       0,   239,   208,   167,   165,   170,   177,   178,   179,   174,
     176,   172,   175,   173,   171,   181,   180,   146,   149,   147,
     148,   159,   150,   163,   155,   153,   166,   154,   152,   157,
     162,   164,   151,   156,   160,   161,   158,   168,   169,   144,
     182,   182,   237,   343,   345,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    84,   129,   127,   121,    91,     0,
       0,   313,     0,   312,     0,     0,   319,    95,     0,   318,
      93,   107,   329,   187,     0,     0,   253,   210,   224,   246,
       0,     0,   113,     0,     0,     0,   100,   326,     0,     0,
      98,   325,     0,   388,    36,    63,   239,   239,   114,   239,
     122,     0,   145,     0,   185,   140,   141,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   317,     0,     0,   323,
       0,     0,   274,   277,   278,   279,     0,   276,   280,   351,
     220,   189,   188,     0,     0,     0,   263,   351,   112,     0,
     101,     0,    99,   123,   224,   144,   182,   182,     0,     0,
       0,     0,     0,     0,     0,   115,     0,     0,     0,     0,
     253,   266,     0,   221,   223,   222,   225,   216,   217,   132,
       0,     0,     0,     0,   142,   143,   183,     0,     0,     0,
       0,     0,     0,   316,   315,   322,   321,   191,   190,   268,
     269,   271,   351,     0,   385,   351,   328,   327,   214,   139,
     215,     0,   270,   272,   266,   273
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   472,    75,    76,    77,   208,
      78,    79,    80,   150,    81,   154,   188,    82,    83,    84,
      85,    86,   557,    87,    88,   495,   607,    89,    90,   367,
     621,    91,   457,   458,   134,   163,   479,    93,    94,   368,
     622,   521,   660,   661,   725,   318,   476,   477,   478,   522,
     225,   595,   819,   789,   180,   784,   751,   754,    95,   196,
     343,    96,    97,   166,   167,   322,   323,   489,   326,   327,
     485,   810,   748,   696,   209,   213,   214,   296,   297,   298,
     135,    99,   100,   101,   136,   103,   122,   123,   437,   438,
     104,   137,   138,   107,   140,   109,   141,   142,   112,   113,
     218,   114,   115,   116,   117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -728
static const yytype_int16 yypact[] =
{
    -728,    84,  2361,  -728,  -728,  -728,  -728,  -728,  -728,  -728,
    -728,  3439,    78,    96,  -728,    78,  6259,   140,    96,  6259,
    3439,   102,    96,   212,   865,  5184,  6259,  6259,   153,  -728,
      96,  -728,    29,  3439,  6259,  6259,  -728,  6259,  6259,   304,
     167,   668,   726,  -728,  5394,  5499,  6259,  5629,  6259,   202,
     188,  3439,    96,  -728,  5394,  6259,  6259,  -728,  -728,  6259,
    -728,  -728,  7204,  6259,  6259,  -728,  6259,  -728,  -728,  2659,
    4974,  5394,  -728,  3309,  -728,  -728,   315,  -728,  -728,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,
      96,  -728,   243,   270,  -728,  -728,  -728,   128,   164,  -728,
    -728,  -728,   197,   210,   235,   264,   274, 10828,  1271,   -39,
     275,   280,  -728,  -728,  -728,  -728,  -728,  -728,   261,  -728,
   10828,   287,  3439,  -728,   292,  -728,   299,  6259,  6259,  6259,
    6259,  6259,  5394,  5394,     9,  -728,  -728,  -728,  -728,  8623,
     278,  -728,  -728,    96,   306,  8837,   347,  5394,   308,  -728,
    -728,  -728,  -728,    96,    67,    96,   310,    56,  7726,  7668,
    7847,  5394,  3439,   316,    22,    69,    34,  -728,  -728,   404,
     344,  7891,   404,  -728,  5394,  -728,   284,  -728,  -728,    96,
    -728,   205, 10828,  -728,  8905,  8502,  3439,  -728,  -728,  7891,
   10828,   323,  5394,  -728, 10828,   373,   337,   222,  8730,  7891,
    8963,   448,  1199,   404,   448,   404,  -728,  -728,  2789,    77,
    -728,  6259,  -728,   -25,   -17, 10828,    32,  9021,   -20,   453,
    -728,    96,   360,  -728,  -728,  -728,    25,    29,    29,    29,
    -728,  6259,  6259,  6259,  6259,  5734,  5734,  6259,  6259,  6259,
    6259,  6259,  6259,   142,  7204,  6259,  6259,  6259,  6259,  6259,
    6259,  6259,  6259,  6259,  6259,  6259,  6259,  6259,  6259,  6259,
    6259,  6259,  6259,  6259,  6259,  6259,  6259,  6259,  6259,  6259,
    6259,  6259,  6259,  6259,  6259,  5394,  5394,  5734,  5734,  1227,
    -728,  -728,  2919,  -728,  8686,  8793,  9079,    21,  5734,    56,
     346,  -728,  -728,  6259,   117,  -728,   335,   371,  -728, 10828,
      96,   353,    96,   446,  5394,  -728,  3569,  5734,  -728,  3699,
    5734,  -728,    56,  3439,   456,   351,  -728,    38,  -728,  -728,
      22,  -728,   381,   354,  -728,  1972,   407,   406,  6259,    29,
    -728,   362,  -728,  -728,  5394,  -728,  -728,  -728,  -728,  -728,
    5394,   367,  2228,   397,  -728,  -728,  5394,  5394,  -728,  -728,
    -728,  -728,  -728,  7610,  5079,  -728,  5289,  -728,  5734,  2529,
     368,  6259,  6259,  -728,  -728,   374,  5394,   382,   229,    29,
     246,   254,   263,   279,  8565, 10886, 10886,    93,  -728,    93,
    -728,    93, 10976,   669,  1290,   522,   344,   448,  -728,  -728,
    -728,  1199, 11007,    93,  1453,  1453, 10886,  1453,  1453,  1028,
     448, 11007, 10944,  1028,   404,   404,   448,  9137,  9195,  9253,
    9311,  9369,  9427,  9485,  9543,  9601,  9659,  9717,  9775,  9833,
    9891,  9949,   383,   385,  -728,    93,  -728,    93,   115,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,    96,    83,  -728,   457,
    6364,  5734,  6469,  5734,  6259,  5734,  7353,    78, 10007,  -728,
    -728, 10065,  6259,  5394,  -728,  5394,   431,    87,  -728,  -728,
     360,  6259,    86,  6259, 10828,    57,  7954,  6259, 10828,    50,
    7784,  -728,   494, 10132,  3439,  -728,   170,  -728,    39,   320,
     391,    22,    74,  -728,  5394,  -728,   276,  6259,  5839,  -728,
   10828,  -728,  -728,  -728, 10828,    20,   396,  -728,  5394,  -728,
     285,    96,   289,   290,  -728,  -728,  -728,  -728,   -29,  7396,
    -728, 10828,  3439, 10828, 10190,  3049,   427,  -728,  -728,  -728,
    -728,  -728,  2046,   297,  -728,  -728,  -728,  -728,  -728,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  1227,  -728,
      96,  6259,   508, 10248,  6259,   510, 10306,   403,  7440,    56,
      56,  -728, 10828,  -728, 10828,  -728,  6259,    96,  -728,   409,
    7891,  -728,  8017,  3829,  -728,  3959,  -728,  8080,  4089,  -728,
      56,  4219,  -728,    56,  3439,  6259,  -728,   387,  -728,    22,
     440,   485,  -728,  -728,  -728,    28,  -728,  -728,  2228,   406,
      59, 10828,  -728, 10828,  3439,  5394,  -728,  -728,  -728,    79,
    -728,  -728,  -728,  4349,   415,  4479,   418,  -728,  6259,  -728,
    3179,   468,   320,  -728,  -728,  -728,  -728,  -728,  -728,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,    96,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,
    -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,  -728,   447,
     429,   429,  -728,  -728,  -728, 10364,  6574,  6679, 10422,  6784,
    6889,  6994,  7099,  -728,  -728, 10828,  -728,  -728,  -728,  3439,
    6259, 10828,  6259, 10828,  3439,  6259, 10828,  -728,  6259, 10828,
    -728,  -728, 10828,  -728,   432,  5944,   208,  -728,   483,  -728,
    5734,  4609,  -728,    80,  4739,  6259,  -728, 10828,  3439,  6259,
    -728, 10828,  3439, 10828,  -728,   303,   443,   443,  -728,   792,
    -728,    65,  -728,  2046,    38,  -728,  -728,  6259,  6259,  6259,
    6259,  6259,  6259,  6259,  6259,  1167, 10132,  8143,  8206, 10132,
    8269,  8332,   440,   102,  6259,  6259,  4869,  -728,  -728,   451,
    5394,  -728,  -728,  6259,    10,  7503,  -728,   484,  -728,  8395,
    -728,  8458,  -728,  -728,   483,  -728,   429,   429,   173, 10480,
   10538, 10596, 10654, 10712, 10770,  -728,  3439,  3439,  3439,  3439,
     208,  6049,   172,  -728,  -728, 10828, 10828,  -728,  -728,  -728,
    7099,  3439,  3439,   156,  -728,  -728,  -728,  6259,  6259,  6259,
    6259,  6259,  6259, 10132, 10132, 10132, 10132,  -728,  -728,  -728,
    -728,  -728,   541,  5734,  7283,   603, 10132, 10132,  -728,  -728,
    -728,  7547,  -728,  -728,  6154,  -728
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -728,  -728,  -728,    -1,  -574,  1608,  -728,  -728,   868,    51,
    -728,  -728,  -728,   110,  1808,    -6,   452,  -678,  -575,   -35,
    -728,  -728,   106,  -728,  -728,  -728,  -728,  -728,   417,   109,
    -104,  -728,  -728,     8,   283,  -728,  -728,  -728,  -728,  -728,
    -728,  -728,  -147,  -146,  -593,  -728,  -145,    -7,  -269,  -728,
    -728,   -38,  -728,  -210,   -66,  -728,  -194,  -176,  -728,  -134,
    -728,  -728,  -728,  -165,   262,  -728,  -317,  -586,  -728,  -452,
    -340,  -727,  -680,  -149,   -33,    68,  -728,  -143,  -728,   143,
     326,  -286,  -728,  -728,   370,  -728,     7,  -139,  -728,    47,
    -728,   806,   858,   -11,  1087,  -728,  1143,  1168,  -728,  -728,
    -728,  -728,  -728,  -728,  -728,  -267
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -386
static const yytype_int16 yytable[] =
{
     120,    72,   499,   480,   212,   139,   177,   178,   145,   290,
     436,   181,   439,   699,   158,   159,   160,   151,   315,   301,
     312,   197,   126,   169,   170,     5,   171,   172,     5,   324,
     599,   331,     5,   182,   184,   185,   189,   190,   216,   486,
     194,  -192,     5,   198,   199,   200,   720,   719,   201,   341,
     121,   202,   203,   204,   445,   205,   500,   428,   182,   215,
     217,   370,   371,   372,   373,   358,   324,   328,   726,   604,
     324,   429,    45,   578,   430,   324,   788,   360,   354,   186,
     573,   295,    14,   362,     3,   369,   356,   822,   431,   432,
    -258,   275,   700,   276,   319,   295,   605,   825,   433,     5,
     287,   811,  -258,   186,   613,  -258,   355,  -258,   295,   434,
     752,   121,   700,   363,   357,   788,   120,   284,   285,   286,
     194,   182,   217,   124,   334,    43,   295,  -258,   144,   121,
     598,   157,   422,   423,   823,   334,   299,   329,   282,  -369,
      53,  -369,  -258,   698,   763,     5,   289,   226,   545,   606,
     299,  -256,   320,  -258,   446,   164,  -258,   121,  -258,   164,
     720,   719,   334,   299,   597,   359,   388,   598,  -192,   589,
     332,   325,  -258,   794,   795,   546,   598,  -258,   282,   227,
     334,   299,   334,   334,   282,   282,   548,  -258,   228,   334,
     567,  -258,   701,  -258,   808,   241,  -258,   496,   243,   452,
     353,   453,   246,   229,   523,   813,   352,   351,   282,   295,
     295,   143,   704,   389,   549,     5,   568,   571,    45,   149,
     374,   375,   376,   377,   379,   381,   382,   383,   384,   385,
     386,   387,   147,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   299,   299,   425,   427,   493,   764,
     191,   462,   694,   587,   295,   334,   587,   448,   818,   436,
     518,   439,   451,   161,   282,    92,  -332,     5,   212,   519,
     212,   149,   488,   182,    92,   464,   466,   174,   468,   470,
     520,   588,   473,    92,   796,   814,     5,   223,   334,  -259,
     149,   173,   486,   502,   503,   750,    92,   490,   192,  -331,
     277,   -63,  -241,   494,   278,   334,   224,   335,    98,   299,
     580,   583,   230,   516,    92,   182,   182,    98,  -259,   591,
     -63,   378,   380,   215,   345,   215,    98,   509,   511,   329,
     513,   514,    92,  -259,  -241,   182,    92,   329,   221,    98,
    -259,   756,   231,  -241,   756,  -335,   329,   610,   524,   614,
     616,   592,   102,  -369,   153,  -369,   525,    98,  -241,  -259,
     593,   102,   329,   424,   426,   526,  -336,   563,  -232,   295,
     102,   594,   334,   334,   447,    98,  -337,  -334,  -259,    98,
     329,   527,  -333,   102,  -259,    92,   428,  -232,  -368,   749,
    -368,   611,   612,   465,   281,   757,   469,   279,   757,   662,
     429,   102,   506,   430,   507,   293,   234,   282,   291,   464,
     553,   468,   556,   473,   558,   511,   300,   431,   432,   102,
     304,   562,   564,   102,   299,    92,   317,   433,    98,   243,
     570,   600,   572,   340,   559,   560,   577,   342,   434,   344,
     364,   175,   366,   235,   508,   609,   454,   236,   450,    92,
     234,    23,   176,   217,   455,   459,   601,   603,    29,   461,
    -133,   474,   475,    14,   481,   482,    17,   217,    98,   487,
     488,    92,   102,   492,    22,   812,    23,   715,   497,  -133,
     501,   512,   515,    29,   815,  -133,   241,   235,   242,   243,
     517,   236,    98,   246,   543,   566,   550,  -262,   544,   584,
     253,    39,   596,   716,  -133,   717,    43,   608,   812,   259,
     334,   666,   102,   669,    98,  -275,   671,   677,   812,    52,
     665,    53,   695,   668,   234,   697,  -262,   552,   708,   555,
     241,   712,   723,   243,  -275,   675,   102,   246,  -275,   724,
     753,  -262,   681,   742,   683,    92,   620,   686,  -262,   569,
     689,   302,   703,   735,   692,   676,   766,   767,   102,   768,
     693,   235,  -275,   820,   721,   236,   807,  -262,   793,    92,
      67,   491,    92,   780,   182,   663,    92,   718,   565,     0,
       0,     0,   707,     0,   711,     0,  -262,   713,    98,   305,
     308,   311,  -262,     0,     0,     0,     0,     0,     0,   351,
     238,     0,     0,   330,   241,  -267,   242,   243,     0,   747,
       0,   246,    98,     0,     0,    98,  -264,     0,   253,    98,
       0,   339,    92,     0,  -267,   257,   258,   259,  -267,     0,
       0,   348,   102,     0,     0,   681,   683,     0,   686,   689,
     707,   711,     0,     0,     0,  -264,     0,     0,   736,   737,
       0,   738,  -267,   739,   740,     0,   102,   741,     0,   102,
    -264,   177,   178,   102,   783,    98,   175,  -264,     0,   755,
     511,   234,     0,   511,   759,     0,    23,   176,   761,     0,
       0,     0,     0,    29,     0,  -133,  -264,     0,     0,     0,
       0,   -39,     0,   782,     0,   809,   736,   769,   770,   739,
     771,   772,   773,   774,  -133,  -264,     0,     0,   235,   102,
       0,  -264,   236,   190,   194,   217,     0,     0,     0,   785,
       0,   449,   786,     0,   175,     0,     0,     0,   809,     0,
       0,     0,     0,     0,    23,   176,     0,    92,   809,     0,
       0,    29,   547,  -133,   471,   803,   804,   805,   806,   -38,
       0,   241,     0,   242,   243,     0,     0,     0,   246,   711,
     816,   817,  -133,     0,     0,   253,   803,   804,   805,   806,
     816,   817,   257,   258,   259,    92,     0,     0,    92,     0,
      98,     0,   821,   511,     0,     0,     0,    14,   105,     0,
      17,     0,     0,   711,     0,     0,     0,   105,    22,     0,
      23,   715,     0,     0,     0,     0,   105,    29,     0,  -133,
       0,     0,     0,     0,     0,     0,     0,     0,    98,   105,
       0,    98,     0,     0,   102,   219,     0,   716,  -133,   717,
      43,     0,     0,     0,     0,     0,    92,   105,    92,     0,
     106,    92,     0,    52,    92,    53,     0,    92,     5,   106,
       0,     0,   149,     0,   152,   105,   664,     0,   106,   105,
       0,     0,   102,     0,  -241,   102,     0,    92,     0,     0,
       0,   106,     0,     0,     0,     0,    92,     0,    92,    98,
       0,    98,     0,    92,    98,     0,     0,    98,     0,   106,
      98,     0,     0,     0,    67,   187,  -241,   574,   576,     0,
       0,   579,   582,     0,     0,  -241,     0,   106,   105,     0,
      98,   106,     0,     0,     0,     0,   153,     0,     0,    98,
    -241,    98,     0,   102,     0,   102,    98,     0,   102,     0,
       0,   102,     0,     0,   102,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,    92,   105,     0,
       0,     0,     0,     0,   102,     0,     0,     0,     0,     0,
     106,     0,     0,   102,     0,   102,     0,     0,     0,     0,
     102,    92,   105,     0,   283,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,     0,     0,
      98,   673,   674,     0,   105,     0,     0,     0,     0,     0,
     106,     0,   678,     0,     0,   187,   187,   187,   314,     0,
       0,     0,   687,     0,    98,   690,     0,     0,    98,   187,
       0,     0,     0,     0,   106,     0,     0,     0,     0,   102,
     234,     0,     0,     0,   102,     0,     0,   187,     0,    92,
      92,    92,    92,     0,     0,     0,   106,   187,     0,     0,
       0,     0,     0,     0,    92,    92,     0,     0,   102,     0,
       0,     0,   102,     0,     0,     0,     0,   235,   105,   108,
       0,   236,     0,     0,     0,     0,     0,     0,   108,     0,
       0,     0,    98,    98,    98,    98,     0,   108,     0,     0,
       0,     0,   105,     0,     0,   105,     0,    98,    98,   105,
     108,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,     0,     0,     0,   246,   108,     0,
     106,     0,     0,     0,   253,   110,   102,   102,   102,   102,
       0,   257,   258,   259,   110,   758,   108,   187,     0,     0,
     108,   102,   102,   110,   106,   105,     0,   106,     0,     0,
     111,   106,     0,     0,     0,     0,   110,     0,     0,   111,
     187,     0,    14,     0,     0,    17,     0,     0,   111,     0,
       0,     0,     0,    22,   110,    23,   715,     0,     0,     0,
       0,   111,    29,     0,  -133,     0,     0,     0,     0,   108,
       0,     0,   110,     0,     0,     0,   110,   106,     0,   111,
      39,   234,   716,  -133,   717,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,    52,     0,
      53,   111,     0,     0,     0,     0,   428,     0,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,   235,     0,
     429,     0,   236,   430,     0,   110,     0,     0,     0,     0,
       0,     0,     0,   108,     0,     0,     0,   431,   432,     0,
     105,     0,     0,     0,     0,     0,     0,   433,     0,    67,
     111,     0,     0,     0,     0,   108,   775,   238,   434,   239,
     240,   241,     0,   242,   243,   110,     0,     0,   246,     0,
      57,     0,   234,     0,   252,   253,     0,     0,   105,   256,
       0,   105,   257,   258,   259,    58,     0,    60,    61,   110,
     111,     0,   106,   187,   187,     0,     0,   187,   187,     0,
       0,     0,     0,     0,     0,     0,    65,   435,     0,   235,
       0,   110,    68,   236,   111,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   108,
     106,     0,     0,   106,     0,     0,   111,     0,     0,   105,
       0,   105,     0,     0,   105,     0,     0,   105,   238,     0,
     105,   240,   241,   108,   242,   243,   108,   274,  -346,   246,
     108,  -368,     0,  -368,     0,     0,   253,     0,     0,     0,
     105,     0,     0,   257,   258,   259,     0,     0,     0,   105,
       0,   105,     0,     0,     0,   110,   105,   187,   187,     0,
       0,   106,     0,   106,     0,     0,   106,     0,   187,   106,
     314,     0,   106,     0,     0,   314,   108,     0,   187,   110,
     111,   187,   110,     0,     0,     0,   110,     0,     0,     0,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,   106,     0,   106,   111,   234,     0,   111,   106,     0,
       0,   111,     0,     0,     0,   105,     0,     0,     0,     0,
     105,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,     0,   105,     0,   236,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   108,   246,     0,     0,     0,   106,     0,   252,   253,
     106,   187,     0,   256,     0,     0,   257,   258,   259,     0,
       0,     0,   105,   105,   105,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,   105,   108,
       0,     0,   108,     0,     0,   314,   314,     0,   314,   314,
      74,     0,     0,     0,     0,     0,     0,   110,     0,   119,
       0,     0,   787,     0,     0,     0,     0,   314,   146,   314,
       0,     0,     0,     0,   106,   106,   106,   106,     0,     0,
       0,   168,   111,     0,     0,     0,     0,     0,     0,   106,
     106,     0,     0,     0,     0,   110,     0,     0,   110,   193,
     108,   787,   108,     0,     0,   108,     0,     0,   108,     0,
       0,   108,     0,     0,     0,     0,     0,    74,     0,     0,
     111,   220,     0,   111,     0,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
     108,     0,   108,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,   110,     0,   110,     0,
       0,   110,     0,     0,   110,     0,     0,   110,     0,     0,
     280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,   111,     0,     0,   111,   110,     0,   111,
       0,     0,   111,     0,     0,     0,   110,     0,   110,     0,
       0,     0,     0,   110,     0,     0,   108,     0,     0,     0,
     316,   108,   111,     0,     0,     0,     0,     0,     0,     0,
       0,   111,     0,   111,     0,     0,     0,     0,   111,     0,
       0,     0,     0,     0,   338,   108,     0,     0,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,   125,   110,     0,     0,     0,   125,   110,     0,     0,
     148,   125,   125,     0,     0,     0,     0,     0,   162,     0,
     165,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,   110,   111,     0,     0,   110,     0,     0,     0,     0,
     195,     0,     0,   108,   108,   108,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,     0,   108,   108,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,   222,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   338,     0,     0,   338,     0,   110,
     110,   110,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,   110,     0,     0,     0,     0,
       0,     0,     0,     0,   111,   111,   111,   111,     0,     0,
       0,   195,     0,     0,     0,   294,     0,     0,     0,   111,
     111,   195,     0,   303,     0,     0,     0,   510,     0,   294,
       0,     0,   321,   483,     0,     5,     6,     7,     8,     9,
      10,     0,   294,   127,   125,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
     294,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,   365,
       0,     0,     0,     0,   165,   165,   165,   165,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     5,
       0,   390,     0,     0,     0,    57,   623,     0,     0,     0,
     624,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,   586,   294,   294,     0,     0,    63,     0,    64,
       0,    65,    66,     0,     0,     0,     0,    68,     0,     0,
     132,     0,    70,     0,   484,     0,     0,     0,   456,     0,
     460,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     617,     0,     0,    74,     0,     0,     0,     0,   321,     0,
     625,   626,   627,   628,   629,   630,     0,   165,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   294,     0,
     641,     0,     0,     0,   642,   643,   644,   645,   646,   647,
     648,   649,   650,   651,   652,     0,   653,     0,     0,   654,
     655,   656,   657,   658,     0,     0,     0,   165,     0,     0,
       0,   338,     0,   338,     0,     0,   338,     0,     0,   338,
       0,     0,   691,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   702,     0,     0,     0,     0,     0,     0,     0,
       0,   706,     0,   710,     0,     0,     0,     0,    74,   483,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,   294,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,   590,     0,     0,   321,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   195,
       0,    57,     0,     0,     0,     0,   760,     0,     0,     0,
     762,     0,     0,     0,     0,     0,    58,    59,    60,    61,
     659,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,   132,     0,    70,     0,
     498,    -2,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,   456,    14,    15,    16,    17,
    -239,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,   321,  -133,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -239,    38,    39,    40,    41,  -133,    42,    43,
       0,  -239,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -239,     0,     0,    55,
       0,    56,     0,     0,    57,     0,   722,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,    69,
       0,    70,     0,    71,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   125,     0,     0,     0,     0,     0,     0,
       4,   765,     5,     6,     7,     8,     9,    10,     0,  -385,
      11,    12,    13,  -385,    14,    15,    16,    17,  -239,    18,
      19,  -385,    20,    21,  -385,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,  -385,  -239,
      44,    45,  -385,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,  -385,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -385,    59,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,     0,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,    64,  -385,  -385,  -385,
       0,    67,  -385,  -385,  -385,     0,     0,    69,  -385,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,    39,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
     179,    67,     0,     0,    68,     0,     0,    69,   206,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,    39,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,   350,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,    39,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,   206,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,    39,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,   619,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,    39,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,   714,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,   219,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   463,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   467,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   680,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   682,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   685,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   688,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   705,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -239,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   709,     0,    28,    29,     0,  -133,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -239,    38,     0,    40,    41,  -133,    42,    43,     0,  -239,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -239,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,    69,     0,    70,
     483,    71,     5,     6,     7,     8,     9,    10,     0,  -385,
     127,     0,     0,  -385,     0,     0,     0,     0,     0,     0,
       0,  -385,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,  -385,     0,
       0,     0,  -385,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -257,  -385,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -385,    59,  -385,
    -385,  -385,     0,  -385,  -385,  -385,     0,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,    64,  -385,  -385,  -385,
       0,     0,  -385,  -385,  -385,     0,     0,   132,     0,    70,
     483,   498,     5,     6,     7,     8,     9,    10,     0,  -385,
     127,     0,     0,  -385,     0,     0,     0,     0,     0,     0,
       0,  -385,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,  -385,     0,
       0,     0,  -385,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -385,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -385,    59,  -385,
    -385,  -385,     0,  -385,  -385,  -385,     0,  -385,  -385,  -385,
    -385,  -385,  -385,  -385,  -385,  -385,    64,  -385,  -385,  -385,
       0,     0,  -385,  -385,  -385,     0,     0,   132,     0,    70,
       0,   498,     5,     6,     7,     8,     9,    10,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,   781,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,   210,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,   211,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,   210,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
     505,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,   155,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,   210,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
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
       0,     0,   186,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,    69,    21,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,   602,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,   743,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     744,    49,    50,   745,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   746,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   746,     0,     0,   128,   129,   734,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,   179,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   551,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   554,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   728,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   729,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   731,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   732,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   733,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     5,     6,     7,     8,     9,    10,     0,    63,
     127,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,   132,    21,    70,     0,   133,     0,     0,   128,
     129,   734,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     5,     6,     7,
       8,     9,    10,     0,    63,   127,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,   132,    21,    70,
       0,   133,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,    21,     0,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,    63,
       0,    64,    31,    32,    66,    34,     0,    35,    36,    68,
       0,     0,   132,     0,    70,    38,   133,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,   127,     0,     0,  -266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,    59,   128,   129,   130,  -266,    28,     0,     0,
    -266,     0,    31,    32,     0,    34,     0,    35,    36,     0,
      64,     0,     0,     0,   179,    38,   232,    40,     0,     0,
     233,   132,     0,    70,  -266,   746,     0,     0,   234,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,    59,     0,   233,   235,     0,     0,     0,   236,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,   121,     0,     0,     0,   237,
       0,     0,     0,    70,     0,   133,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   232,   254,   255,   256,   233,     0,   257,
     258,   259,     0,   237,     0,   234,     0,     0,     0,   615,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   232,   254,   255,
     256,   233,   235,   257,   258,   259,   236,     0,     0,   234,
       0,     0,     0,   672,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     232,   254,   255,   256,   233,     0,   257,   258,   259,     0,
     237,     0,   234,     0,     0,     0,   790,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,     0,     0,     0,   232,     0,
     824,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,   309,     0,   237,     0,     0,     0,     0,     0,     0,
       0,   310,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,   232,     0,     0,     0,
     233,   504,     0,     0,     0,     0,     0,   121,   234,   306,
       0,   237,     0,     0,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,   232,     0,   282,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,   581,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,   282,     0,     0,   232,     0,     0,
       0,   233,     0,   121,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   232,   254,   255,   256,   233,   235,   257,   258,   259,
     236,     0,   282,   234,   186,     0,     0,   313,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   232,   254,   255,   256,   233,     0,
     257,   258,   259,     0,   237,   282,   234,   575,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,   282,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,   282,     0,     0,     0,     0,   679,     0,     0,
     232,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,   282,     0,     0,     0,     0,
     684,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,   282,     0,
       0,     0,     0,   776,     0,     0,   232,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,   282,     0,     0,     0,     0,   777,     0,     0,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,   282,     0,     0,     0,     0,   778,
       0,     0,   232,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,   282,     0,     0,
       0,     0,   779,     0,     0,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
     282,     0,     0,     0,     0,   791,     0,     0,   232,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   232,   254,   255,   256,   233,   235,   257,   258,
     259,   236,     0,   282,   234,     0,     0,     0,   792,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   232,   254,   255,   256,   233,
       0,   257,   258,   259,     0,   237,   282,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
     337,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,   288,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,  -347,     0,     0,     0,   232,     0,     0,     0,
     233,     0,   121,     0,     0,     0,   237,     0,   234,   440,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   441,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     232,   254,   255,   256,   233,   235,   257,   258,   259,   236,
       0,     0,   234,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,   347,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   232,   254,   255,   256,   233,     0,   257,
     258,   259,     0,   237,     0,   234,   442,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   443,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   232,   254,   255,
     256,   233,   235,   257,   258,   259,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   232,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   292,
     257,   258,   259,     0,   235,     0,     0,     0,   236,     0,
       0,     0,     0,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   336,   257,   258,
     259,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   349,   257,   258,   259,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,   361,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,   232,     0,   444,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   528,
     257,   258,   259,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   529,   257,   258,
     259,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   530,   257,   258,   259,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   531,   257,   258,   259,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   532,   257,   258,   259,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   533,
     257,   258,   259,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   534,   257,   258,
     259,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   535,   257,   258,   259,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   536,   257,   258,   259,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   537,   257,   258,   259,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   538,
     257,   258,   259,   232,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,   539,   257,   258,
     259,   232,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,   540,   257,   258,   259,   232,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,   541,   257,   258,   259,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,   542,   257,   258,   259,   232,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,   121,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,   233,     0,   237,     0,
       0,     0,     0,     0,   234,     0,     0,   585,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   561,   257,   258,
     259,   235,     0,     0,     0,   236,     0,     0,     0,     0,
     232,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,   232,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,   667,   618,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,   232,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,   670,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,   232,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,   232,     0,   727,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,     0,   257,   258,   259,
     232,     0,   730,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,   232,     0,
     797,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,   232,     0,   798,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,   232,     0,   799,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,   232,     0,   800,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,     0,   257,   258,   259,
     232,     0,   801,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,   232,     0,
     802,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   234,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,   235,     0,     0,     0,   236,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,     0,   250,   251,   252,
     253,     0,   254,     0,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,     0,   250,
     251,   252,   253,     0,   254,     0,   256,     0,     0,   257,
     258,   259,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,     0,   246,   247,   248,     0,
     250,   251,   252,   253,     0,     0,     0,   256,     0,     0,
     257,   258,   259
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-728))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   342,   320,    70,    16,    41,    42,    19,   143,
     279,    44,   279,   599,    25,    26,    27,    23,   161,   153,
     159,    54,    15,    34,    35,     3,    37,    38,     3,     1,
     482,   174,     3,    44,    45,    46,    47,    48,    71,   325,
      51,     3,     3,    54,    55,    56,   621,   621,    59,   192,
      79,    62,    63,    64,    33,    66,   342,    19,    69,    70,
      71,   226,   227,   228,   229,    33,     1,    33,   661,    49,
       1,    33,    62,    23,    36,     1,   754,   216,   103,    23,
      23,   147,    15,   103,     0,    60,   103,   814,    50,    51,
      62,   130,    33,   132,    72,   161,    76,   824,    60,     3,
     133,   781,    33,    23,   133,    77,   131,    33,   174,    71,
     696,    79,    33,   133,   131,   793,   127,   128,   129,   130,
     131,   132,   133,    13,   103,    58,   192,    62,    18,    79,
     102,    25,   275,   276,   814,   103,   147,   103,   128,   130,
      73,   132,    77,   595,   719,     3,   139,    19,    33,   129,
     161,    82,   130,    84,   133,   130,   128,    79,    84,   130,
     735,   735,   103,   174,   481,   133,    24,   102,   130,   130,
     176,   102,   103,   766,   767,    60,   102,   103,   128,    51,
     103,   192,   103,   103,   128,   128,   103,   122,    60,   103,
     103,   122,   133,   128,   780,   102,   122,   340,   105,    82,
     211,    84,   109,    75,   369,    33,   129,   208,   128,   275,
     276,    71,   133,    71,   131,     3,   129,   131,    62,     7,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   130,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   334,   721,
      68,   304,   589,   103,   340,   103,   103,   288,   122,   548,
      51,   548,   293,   130,   128,     2,   122,     3,   354,    60,
     356,     7,    84,   304,    11,   306,   307,   130,   309,   310,
      71,   131,   313,    20,   131,   133,     3,    37,   103,    33,
       7,     7,   598,   346,   347,   107,    33,   328,   130,   122,
      59,    37,    19,   334,    63,   103,    56,   122,     2,   340,
     469,   470,   122,   366,    51,   346,   347,    11,    62,    19,
      56,   235,   236,   354,   122,   356,    20,   358,   359,   103,
     361,   362,    69,    77,    51,   366,    73,   103,    43,    33,
      84,   701,   127,    60,   704,   122,   103,   501,   122,   508,
     509,    51,     2,   130,    71,   132,   122,    51,    75,   103,
      60,    11,   103,   277,   278,   122,   122,   453,   103,   455,
      20,    71,   103,   103,   288,    69,   122,   122,   122,    73,
     103,   122,   122,    33,   128,   122,    19,   122,   130,   695,
     132,   122,   122,   307,   122,   701,   310,   130,   704,   122,
      33,    51,   354,    36,   356,    78,    22,   128,   122,   440,
     441,   442,   443,   444,   445,   446,   128,    50,    51,    69,
     130,   452,   453,    73,   455,   162,   130,    60,   122,   105,
     461,   484,   463,   130,   447,   448,   467,    84,    71,   122,
       7,    18,   102,    59,   358,   498,   131,    63,   122,   186,
      22,    28,    29,   484,   103,   122,   487,   488,    35,    33,
      37,    25,   131,    15,   103,   131,    18,   498,   162,    82,
      84,   208,   122,   131,    26,   781,    28,    29,   131,    56,
     103,   133,   128,    35,   790,    37,   102,    59,   104,   105,
     128,    63,   186,   109,   131,    84,    59,    33,   133,    25,
     116,    53,   131,    55,    56,    57,    58,   131,   814,   125,
     103,    23,   162,    23,   208,    84,   133,   128,   824,    71,
     551,    73,   102,   554,    22,    60,    62,   441,   133,   443,
     102,   133,   105,   105,   103,   566,   186,   109,   107,   130,
      77,    77,   573,   131,   575,   282,   515,   578,    84,   460,
     581,   154,   605,   677,   585,   567,   723,   723,   208,   724,
     587,    59,   131,   793,   622,    63,   780,   103,   764,   306,
     122,   329,   309,   742,   605,   548,   313,   129,   455,    -1,
      -1,    -1,   613,    -1,   615,    -1,   122,   618,   282,   157,
     158,   159,   128,    -1,    -1,    -1,    -1,    -1,    -1,   620,
      98,    -1,    -1,   171,   102,    84,   104,   105,    -1,   695,
      -1,   109,   306,    -1,    -1,   309,    33,    -1,   116,   313,
      -1,   189,   359,    -1,   103,   123,   124,   125,   107,    -1,
      -1,   199,   282,    -1,    -1,   666,   667,    -1,   669,   670,
     671,   672,    -1,    -1,    -1,    62,    -1,    -1,   679,   680,
      -1,   682,   131,   684,   685,    -1,   306,   688,    -1,   309,
      77,   716,   717,   313,   750,   359,    18,    84,    -1,   700,
     701,    22,    -1,   704,   705,    -1,    28,    29,   709,    -1,
      -1,    -1,    -1,    35,    -1,    37,   103,    -1,    -1,    -1,
      -1,    43,    -1,   746,    -1,   781,   727,   728,   729,   730,
     731,   732,   733,   734,    56,   122,    -1,    -1,    59,   359,
      -1,   128,    63,   744,   745,   746,    -1,    -1,    -1,   750,
      -1,   289,   753,    -1,    18,    -1,    -1,    -1,   814,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,   474,   824,    -1,
      -1,    35,   436,    37,   312,   776,   777,   778,   779,    43,
      -1,   102,    -1,   104,   105,    -1,    -1,    -1,   109,   790,
     791,   792,    56,    -1,    -1,   116,   797,   798,   799,   800,
     801,   802,   123,   124,   125,   512,    -1,    -1,   515,    -1,
     474,    -1,   813,   814,    -1,    -1,    -1,    15,     2,    -1,
      18,    -1,    -1,   824,    -1,    -1,    -1,    11,    26,    -1,
      28,    29,    -1,    -1,    -1,    -1,    20,    35,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   512,    33,
      -1,   515,    -1,    -1,   474,    53,    -1,    55,    56,    57,
      58,    -1,    -1,    -1,    -1,    -1,   573,    51,   575,    -1,
       2,   578,    -1,    71,   581,    73,    -1,   584,     3,    11,
      -1,    -1,     7,    -1,     9,    69,   550,    -1,    20,    73,
      -1,    -1,   512,    -1,    19,   515,    -1,   604,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,   613,    -1,   615,   573,
      -1,   575,    -1,   620,   578,    -1,    -1,   581,    -1,    51,
     584,    -1,    -1,    -1,   122,    47,    51,   465,   466,    -1,
      -1,   469,   470,    -1,    -1,    60,    -1,    69,   122,    -1,
     604,    73,    -1,    -1,    -1,    -1,    71,    -1,    -1,   613,
      75,   615,    -1,   573,    -1,   575,   620,    -1,   578,    -1,
      -1,   581,    -1,    -1,   584,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   679,    -1,    -1,    -1,    -1,   684,   162,    -1,
      -1,    -1,    -1,    -1,   604,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,   613,    -1,   615,    -1,    -1,    -1,    -1,
     620,   708,   186,    -1,   126,   712,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   679,    -1,    -1,    -1,    -1,
     684,   559,   560,    -1,   208,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   570,    -1,    -1,   157,   158,   159,   160,    -1,
      -1,    -1,   580,    -1,   708,   583,    -1,    -1,   712,   171,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,   679,
      22,    -1,    -1,    -1,   684,    -1,    -1,   189,    -1,   776,
     777,   778,   779,    -1,    -1,    -1,   208,   199,    -1,    -1,
      -1,    -1,    -1,    -1,   791,   792,    -1,    -1,   708,    -1,
      -1,    -1,   712,    -1,    -1,    -1,    -1,    59,   282,     2,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      -1,    -1,   776,   777,   778,   779,    -1,    20,    -1,    -1,
      -1,    -1,   306,    -1,    -1,   309,    -1,   791,   792,   313,
      33,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,   105,    -1,    -1,    -1,   109,    51,    -1,
     282,    -1,    -1,    -1,   116,     2,   776,   777,   778,   779,
      -1,   123,   124,   125,    11,   703,    69,   289,    -1,    -1,
      73,   791,   792,    20,   306,   359,    -1,   309,    -1,    -1,
       2,   313,    -1,    -1,    -1,    -1,    33,    -1,    -1,    11,
     312,    -1,    15,    -1,    -1,    18,    -1,    -1,    20,    -1,
      -1,    -1,    -1,    26,    51,    28,    29,    -1,    -1,    -1,
      -1,    33,    35,    -1,    37,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    69,    -1,    -1,    -1,    73,   359,    -1,    51,
      53,    22,    55,    56,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    71,    -1,
      73,    73,    -1,    -1,    -1,    -1,    19,    -1,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      33,    -1,    63,    36,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    50,    51,    -1,
     474,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,   122,
     122,    -1,    -1,    -1,    -1,   208,   129,    98,    71,   100,
     101,   102,    -1,   104,   105,   162,    -1,    -1,   109,    -1,
      83,    -1,    22,    -1,   115,   116,    -1,    -1,   512,   120,
      -1,   515,   123,   124,   125,    98,    -1,   100,   101,   186,
     162,    -1,   474,   465,   466,    -1,    -1,   469,   470,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,    59,
      -1,   208,   125,    63,   186,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,   282,
     512,    -1,    -1,   515,    -1,    -1,   208,    -1,    -1,   573,
      -1,   575,    -1,    -1,   578,    -1,    -1,   581,    98,    -1,
     584,   101,   102,   306,   104,   105,   309,   126,   127,   109,
     313,   130,    -1,   132,    -1,    -1,   116,    -1,    -1,    -1,
     604,    -1,    -1,   123,   124,   125,    -1,    -1,    -1,   613,
      -1,   615,    -1,    -1,    -1,   282,   620,   559,   560,    -1,
      -1,   573,    -1,   575,    -1,    -1,   578,    -1,   570,   581,
     572,    -1,   584,    -1,    -1,   577,   359,    -1,   580,   306,
     282,   583,   309,    -1,    -1,    -1,   313,    -1,    -1,    -1,
      -1,    -1,   604,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   613,    -1,   615,   306,    22,    -1,   309,   620,    -1,
      -1,   313,    -1,    -1,    -1,   679,    -1,    -1,    -1,    -1,
     684,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    -1,   708,    -1,    63,    -1,   712,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   359,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   679,    -1,    -1,
      -1,    -1,   684,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   474,   109,    -1,    -1,    -1,   708,    -1,   115,   116,
     712,   703,    -1,   120,    -1,    -1,   123,   124,   125,    -1,
      -1,    -1,   776,   777,   778,   779,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   791,   792,   512,
      -1,    -1,   515,    -1,    -1,   737,   738,    -1,   740,   741,
       2,    -1,    -1,    -1,    -1,    -1,    -1,   474,    -1,    11,
      -1,    -1,   754,    -1,    -1,    -1,    -1,   759,    20,   761,
      -1,    -1,    -1,    -1,   776,   777,   778,   779,    -1,    -1,
      -1,    33,   474,    -1,    -1,    -1,    -1,    -1,    -1,   791,
     792,    -1,    -1,    -1,    -1,   512,    -1,    -1,   515,    51,
     573,   793,   575,    -1,    -1,   578,    -1,    -1,   581,    -1,
      -1,   584,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
     512,    73,    -1,   515,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   604,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     613,    -1,   615,    -1,    -1,    -1,    -1,   620,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   573,    -1,   575,    -1,
      -1,   578,    -1,    -1,   581,    -1,    -1,   584,    -1,    -1,
     122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   573,    -1,   575,    -1,    -1,   578,   604,    -1,   581,
      -1,    -1,   584,    -1,    -1,    -1,   613,    -1,   615,    -1,
      -1,    -1,    -1,   620,    -1,    -1,   679,    -1,    -1,    -1,
     162,   684,   604,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   613,    -1,   615,    -1,    -1,    -1,    -1,   620,    -1,
      -1,    -1,    -1,    -1,   186,   708,    -1,    -1,    -1,   712,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,
      -1,    13,   679,    -1,    -1,    -1,    18,   684,    -1,    -1,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   679,    -1,    -1,
      -1,   708,   684,    -1,    -1,   712,    -1,    -1,    -1,    -1,
      52,    -1,    -1,   776,   777,   778,   779,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   708,    -1,   791,   792,
     712,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     282,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   306,    -1,    -1,   309,    -1,   776,
     777,   778,   779,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   791,   792,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   776,   777,   778,   779,    -1,    -1,
      -1,   143,    -1,    -1,    -1,   147,    -1,    -1,    -1,   791,
     792,   153,    -1,   155,    -1,    -1,    -1,   359,    -1,   161,
      -1,    -1,   164,     1,    -1,     3,     4,     5,     6,     7,
       8,    -1,   174,    11,   176,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
     192,    -1,    30,    31,    32,    -1,    34,    -1,    -1,    -1,
      -1,    39,    40,    -1,    42,    -1,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    54,    -1,    -1,   221,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,   243,    -1,    -1,    -1,    83,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   474,   275,   276,    -1,    -1,   115,    -1,   117,
      -1,   119,   120,    -1,    -1,    -1,    -1,   125,    -1,    -1,
     128,    -1,   130,    -1,   132,    -1,    -1,    -1,   300,    -1,
     302,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     512,    -1,    -1,   515,    -1,    -1,    -1,    -1,   320,    -1,
      84,    85,    86,    87,    88,    89,    -1,   329,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   340,    -1,
     104,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,   120,    -1,    -1,   123,
     124,   125,   126,   127,    -1,    -1,    -1,   369,    -1,    -1,
      -1,   573,    -1,   575,    -1,    -1,   578,    -1,    -1,   581,
      -1,    -1,   584,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   604,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   613,    -1,   615,    -1,    -1,    -1,    -1,   620,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,    31,
      32,    -1,    34,   455,    -1,    -1,    -1,    39,    40,    -1,
      42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    -1,    -1,    -1,   478,    -1,    -1,   481,
      -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   501,
      -1,    83,    -1,    -1,    -1,    -1,   708,    -1,    -1,    -1,
     712,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     522,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,    -1,
      -1,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,    -1,
     132,     0,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    11,    12,    13,   567,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    -1,    26,    -1,    28,
      29,    30,    31,    32,    -1,    34,    35,   589,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    -1,
      -1,    -1,    51,    52,    53,    54,    55,    56,    57,    58,
      -1,    60,    61,    62,    -1,    64,    65,    66,    67,    68,
      69,    -1,    71,    -1,    73,    74,    75,    -1,    -1,    78,
      -1,    80,    -1,    -1,    83,    -1,   638,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,   117,    -1,
     119,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,   128,
      -1,   130,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   715,    -1,    -1,    -1,    -1,    -1,    -1,
       1,   723,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,   123,   124,   125,    -1,    -1,   128,   129,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
     121,   122,    -1,    -1,   125,    -1,    -1,   128,   129,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,   129,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,   129,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,   129,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,   129,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    -1,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    32,    -1,    34,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    -1,    -1,    -1,
      51,    52,    -1,    54,    55,    56,    57,    58,    -1,    60,
      61,    62,    -1,    64,    65,    66,    67,    68,    69,    -1,
      71,    -1,    73,    74,    75,    -1,    -1,    78,    -1,    80,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,    -1,   128,    -1,   130,
       1,   132,     3,     4,     5,     6,     7,     8,    -1,    10,
      11,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    63,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,    -1,   123,   124,   125,    -1,    -1,   128,    -1,   130,
      -1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,   133,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,   107,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
     131,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    51,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,   117,    -1,   119,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,   128,    -1,   130,
      -1,   132,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
     121,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,   121,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    -1,    -1,    -1,
     106,    -1,     3,     4,     5,     6,     7,     8,    -1,   115,
      11,   117,    -1,   119,   120,    -1,    -1,    -1,    -1,   125,
      -1,    -1,   128,    24,   130,    -1,   132,    -1,    -1,    30,
      31,    32,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,    -1,    -1,    -1,    -1,   106,    -1,     3,     4,     5,
       6,     7,     8,    -1,   115,    11,   117,    -1,   119,   120,
      -1,    -1,    -1,    -1,   125,    -1,    -1,   128,    24,   130,
      -1,   132,    -1,    -1,    30,    31,    32,    -1,    34,    -1,
      -1,    -1,    -1,    39,    40,    -1,    42,    -1,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,   100,   101,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    -1,    34,    -1,   115,
      -1,   117,    39,    40,   120,    42,    -1,    44,    45,   125,
      -1,    -1,   128,    -1,   130,    52,   132,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    68,    69,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    11,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    99,    30,    31,    32,   103,    34,    -1,    -1,
     107,    -1,    39,    40,    -1,    42,    -1,    44,    45,    -1,
     117,    -1,    -1,    -1,   121,    52,    10,    54,    -1,    -1,
      14,   128,    -1,   130,   131,   132,    -1,    -1,    22,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    99,    -1,    14,    59,    -1,    -1,    -1,    63,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    59,
     104,   105,   106,    63,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    10,   118,   119,   120,    14,    -1,   123,
     124,   125,    -1,    83,    -1,    22,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    10,   118,   119,
     120,    14,    59,   123,   124,   125,    63,    -1,    -1,    22,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    59,   104,   105,   106,
      63,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      10,   118,   119,   120,    14,    -1,   123,   124,   125,    -1,
      83,    -1,    22,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,   118,   119,   120,    -1,    59,
     123,   124,   125,    63,    -1,    -1,    -1,    -1,    10,    -1,
     133,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    59,   118,   119,
     120,    63,    -1,   123,   124,   125,    10,    -1,    -1,    -1,
      14,   131,    -1,    -1,    -1,    -1,    -1,    79,    22,    23,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    59,   118,   119,   120,    63,
      -1,   123,   124,   125,    10,    -1,   128,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    59,   118,   119,   120,    63,    -1,   123,
     124,   125,    -1,    -1,   128,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    79,    -1,    -1,    -1,    83,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    10,   118,   119,   120,    14,    59,   123,   124,   125,
      63,    -1,   128,    22,    23,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      59,   104,   105,   106,    63,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    10,   118,   119,   120,    14,    -1,
     123,   124,   125,    -1,    83,   128,    22,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    -1,   118,
     119,   120,    -1,    59,   123,   124,   125,    63,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    83,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    -1,   118,   119,   120,    -1,    59,   123,   124,   125,
      63,    -1,   128,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,   118,   119,   120,    -1,    59,
     123,   124,   125,    63,    -1,   128,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    -1,   118,   119,
     120,    -1,    59,   123,   124,   125,    63,    -1,   128,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    83,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,   118,   119,   120,    -1,    59,   123,   124,   125,    63,
      -1,   128,    -1,    -1,    -1,    -1,    70,    -1,    -1,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    -1,   118,   119,   120,    -1,    59,   123,
     124,   125,    63,    -1,   128,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    -1,   118,   119,   120,
      -1,    59,   123,   124,   125,    63,    -1,   128,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
     118,   119,   120,    -1,    59,   123,   124,   125,    63,    -1,
     128,    -1,    -1,    -1,    -1,    70,    -1,    -1,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    10,   118,   119,   120,    14,    59,   123,   124,
     125,    63,    -1,   128,    22,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    59,   104,   105,   106,    63,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    10,   118,   119,   120,    14,
      -1,   123,   124,   125,    -1,    83,   128,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    -1,
     118,   119,   120,    -1,    59,   123,   124,   125,    63,    -1,
     128,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    59,   118,   119,   120,    63,    -1,   123,   124,
     125,    -1,   127,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      14,    -1,    79,    -1,    -1,    -1,    83,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      10,   118,   119,   120,    14,    59,   123,   124,   125,    63,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    59,
     104,   105,   106,    63,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    10,   118,   119,   120,    14,    -1,   123,
     124,   125,    -1,    83,    -1,    22,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    10,   118,   119,
     120,    14,    59,   123,   124,   125,    63,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    59,   104,   105,   106,
      63,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,   118,   119,   120,    -1,    10,   123,   124,   125,    14,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,   125,    -1,    59,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    59,   118,   119,   120,    63,   122,   123,   124,
     125,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      59,   118,   119,   120,    63,   122,   123,   124,   125,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    59,   118,
     119,   120,    63,    -1,   123,   124,   125,    10,    -1,    70,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    59,   118,   119,   120,
      63,    -1,   123,   124,   125,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    59,   118,   119,   120,    63,   122,
     123,   124,   125,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    59,   118,   119,   120,    63,   122,   123,   124,
     125,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      59,   118,   119,   120,    63,   122,   123,   124,   125,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    59,   118,
     119,   120,    63,   122,   123,   124,   125,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    59,   118,   119,   120,
      63,   122,   123,   124,   125,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    59,   118,   119,   120,    63,   122,
     123,   124,   125,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    59,   118,   119,   120,    63,   122,   123,   124,
     125,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      59,   118,   119,   120,    63,   122,   123,   124,   125,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    59,   118,
     119,   120,    63,   122,   123,   124,   125,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    59,   118,   119,   120,
      63,   122,   123,   124,   125,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    59,   118,   119,   120,    63,   122,
     123,   124,   125,    10,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    59,   118,   119,   120,    63,   122,   123,   124,
     125,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,   100,   101,   102,    -1,   104,   105,   106,
      -1,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      59,   118,   119,   120,    63,   122,   123,   124,   125,    10,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,   102,    -1,   104,   105,   106,    -1,   108,
     109,   110,   111,   112,   113,   114,   115,   116,    59,   118,
     119,   120,    63,   122,   123,   124,   125,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,
     101,   102,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    59,   118,   119,   120,
      63,   122,   123,   124,   125,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    22,    -1,    -1,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    59,   118,   119,   120,    63,    -1,
     123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    83,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,
     105,   106,    -1,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,   118,   119,   120,    -1,   122,   123,   124,
     125,    59,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    59,
     118,   119,   120,    63,    -1,   123,   124,   125,    10,    -1,
      -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    59,   118,   119,
     120,    63,    -1,   123,   124,   125,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    59,   118,   119,   120,    63,
      -1,   123,   124,   125,    10,    -1,    -1,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    59,   118,   119,   120,    63,    -1,   123,
     124,   125,    10,    -1,    70,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    59,   118,   119,   120,    63,    -1,   123,   124,   125,
      10,    -1,    70,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    59,
     118,   119,   120,    63,    -1,   123,   124,   125,    10,    -1,
      70,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    59,   118,   119,
     120,    63,    -1,   123,   124,   125,    10,    -1,    70,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    59,   118,   119,   120,    63,
      -1,   123,   124,   125,    10,    -1,    70,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,    59,   118,   119,   120,    63,    -1,   123,
     124,   125,    10,    -1,    70,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,   112,   113,   114,   115,
     116,    59,   118,   119,   120,    63,    -1,   123,   124,   125,
      10,    -1,    70,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,   102,    -1,   104,   105,   106,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,    59,
     118,   119,   120,    63,    -1,   123,   124,   125,    10,    -1,
      70,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
     100,   101,   102,    -1,   104,   105,   106,    -1,   108,   109,
     110,   111,   112,   113,   114,   115,   116,    59,   118,   119,
     120,    63,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
     102,    -1,   104,   105,   106,    -1,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    59,   118,   119,   120,    63,
      -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,    22,   113,
     114,   115,   116,    59,   118,   119,   120,    63,    -1,   123,
     124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    63,
      -1,    -1,    98,    -1,   100,   101,   102,    -1,   104,   105,
     106,    -1,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,    -1,   118,    -1,   120,    -1,    59,   123,   124,   125,
      63,    -1,    -1,    -1,    98,    -1,   100,   101,   102,    -1,
     104,   105,   106,    -1,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,    -1,   118,    -1,   120,    -1,    -1,   123,
     124,   125,    -1,    -1,    -1,    98,    -1,   100,   101,   102,
      -1,   104,   105,   106,    -1,    -1,   109,   110,   111,    -1,
     113,   114,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,
     123,   124,   125
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   138,   139,     0,     1,     3,     4,     5,     6,     7,
       8,    11,    12,    13,    15,    16,    17,    18,    20,    21,
      23,    24,    26,    28,    29,    30,    31,    32,    34,    35,
      38,    39,    40,    41,    42,    44,    45,    47,    52,    53,
      54,    55,    57,    58,    61,    62,    64,    65,    66,    67,
      68,    69,    71,    73,    74,    78,    80,    83,    98,    99,
     100,   101,   106,   115,   117,   119,   120,   122,   125,   128,
     130,   132,   140,   141,   142,   143,   144,   145,   147,   148,
     149,   151,   154,   155,   156,   157,   158,   160,   161,   164,
     165,   168,   171,   174,   175,   195,   198,   199,   217,   218,
     219,   220,   221,   222,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   238,   239,   240,   241,   242,   142,
     230,    79,   223,   224,   150,   151,   223,    11,    30,    31,
      32,    69,   128,   132,   171,   217,   221,   228,   229,   230,
     231,   233,   234,    71,   150,   230,   142,   130,   151,     7,
     150,   152,     9,    71,   152,    51,    81,   159,   230,   230,
     230,   130,   151,   172,   130,   151,   200,   201,   142,   230,
     230,   230,   230,     7,   130,    18,    29,   156,   156,   121,
     191,   211,   230,   122,   230,   230,    23,   145,   153,   230,
     230,    68,   130,   142,   230,   151,   196,   211,   230,   230,
     230,   230,   230,   230,   230,   230,   129,   140,   146,   211,
      72,   107,   191,   212,   213,   230,   211,   230,   237,    53,
     142,    43,   151,    37,    56,   187,    19,    51,    60,    75,
     122,   127,    10,    14,    22,    59,    63,    83,    98,   100,
     101,   102,   104,   105,   106,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   118,   119,   120,   123,   124,   125,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,   126,   130,   132,    59,    63,   130,
     142,   122,   128,   145,   230,   230,   230,   211,    33,   223,
     196,   122,   122,    78,   151,   191,   214,   215,   216,   230,
     128,   196,   165,   151,   130,   153,    23,    33,   153,    23,
      33,   153,   224,    70,   145,   214,   142,   130,   182,    72,
     130,   151,   202,   203,     1,   102,   205,   206,    33,   103,
     153,   214,   152,   151,   103,   122,   122,   128,   142,   153,
     130,   214,    84,   197,   122,   122,    27,    48,   153,   122,
     129,   140,   129,   230,   103,   131,   103,   131,    33,   133,
     224,    82,   103,   133,     7,   151,   102,   166,   176,    60,
     200,   200,   200,   200,   230,   230,   230,   230,   159,   230,
     159,   230,   230,   230,   230,   230,   230,   230,    24,    71,
     151,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   214,   214,   159,   230,   159,   230,    19,    33,
      36,    50,    51,    60,    71,   120,   185,   225,   226,   242,
      23,    33,    23,    33,    70,    33,   133,   159,   230,   153,
     122,   230,    82,    84,   131,   103,   151,   169,   170,   122,
     151,    33,   211,    32,   230,   159,   230,    32,   230,   159,
     230,   153,   142,   230,    25,   131,   183,   184,   185,   173,
     203,   103,   131,     1,   132,   207,   218,    82,    84,   204,
     230,   201,   131,   191,   230,   162,   214,   131,   132,   207,
     218,   103,   211,   211,   131,   131,   212,   212,   159,   230,
     142,   230,   133,   230,   230,   128,   211,   128,    51,    60,
      71,   178,   186,   200,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   131,   133,    33,    60,   217,   103,   131,
      59,    32,   159,   230,    32,   159,   230,   159,   230,   223,
     223,   122,   230,   191,   230,   216,    84,   103,   129,   166,
     230,   131,   230,    23,   153,    23,   153,   230,    23,   153,
     224,    23,   153,   224,    25,    25,   142,   103,   131,   130,
     151,    19,    51,    60,    71,   188,   131,   203,   102,   206,
     211,   230,    46,   230,    49,    76,   129,   163,   131,   211,
     196,   122,   122,   133,   224,   133,   224,   142,    82,   129,
     146,   167,   177,    10,    14,    84,    85,    86,    87,    88,
      89,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   104,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   120,   123,   124,   125,   126,   127,   151,
     179,   180,   122,   226,   217,   230,    23,    23,   230,    23,
      23,   133,   133,   153,   153,   230,   170,   128,   153,    70,
      32,   230,    32,   230,    70,    32,   230,   153,    32,   230,
     153,   142,   230,   184,   203,   102,   210,    60,   206,   204,
      33,   133,   142,   211,   133,    32,   142,   230,   133,    32,
     142,   230,   133,   230,   129,    29,    55,    57,   129,   141,
     155,   188,   151,   105,   130,   181,   181,    70,    32,    32,
      70,    32,    32,    32,    32,   167,   230,   230,   230,   230,
     230,   230,   131,    24,    66,    69,   132,   191,   209,   218,
     107,   193,   204,    77,   194,   230,   207,   218,   153,   230,
     142,   230,   142,   155,   206,   151,   179,   180,   183,   230,
     230,   230,   230,   230,   230,   129,    70,    70,    70,    70,
     210,   133,   211,   191,   192,   230,   230,   145,   154,   190,
     133,    70,    70,   194,   181,   181,   131,    70,    70,    70,
      70,    70,    70,   230,   230,   230,   230,   193,   204,   191,
     208,   209,   218,    33,   133,   218,   230,   230,   122,   189,
     190,   230,   208,   209,   133,   208
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
#line 404 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 409 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 410 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 417 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 422 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 423 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 441 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 444 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 445 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 446 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 450 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 451 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 453 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 466 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 487 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), true); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), false); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 503 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 506 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 508 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 511 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 513 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 514 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 517 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 521 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 535 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 536 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 548 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
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

  case 82:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 585 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 589 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 590 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 598 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 610 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 623 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 668 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 675 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 680 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 685 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 701 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 703 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 708 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 718 "chapel.ypp"
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
#line 727 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 731 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 744 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 749 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 754 "chapel.ypp"
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

  case 133:

/* Line 1806 of yacc.c  */
#line 774 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 780 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 787 "chapel.ypp"
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
#line 795 "chapel.ypp"
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
#line 806 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 812 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 819 "chapel.ypp"
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
#line 841 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 850 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 854 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 866 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 867 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 869 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 871 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 873 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 879 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 881 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 883 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 884 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 885 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 886 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 890 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 892 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 893 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 894 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 895 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 896 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 965 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1007 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
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

  case 231:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1045 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1052 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1059 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1066 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
    }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1074 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1083 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1090 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1106 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1113 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1120 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1122 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1124 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1142 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1181 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1185 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1186 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1192 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1194 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1200 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1202 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1208 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1228 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1234 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1283 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1285 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1298 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1304 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1306 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1308 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1312 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1314 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1321 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1327 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1333 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1339 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1368 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1372 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1384 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1392 "chapel.ypp"
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[(1) - (2)].pt));
      if (!tiMark)
        USR_FATAL_CONT((yyvsp[(2) - (2)].pexpr), "Unsupported intent in with clause");
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[(2) - (2)].pexpr);
    }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1412 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1433 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1437 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1441 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1443 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1466 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1467 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1468 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1472 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1484 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1490 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1492 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1493 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1494 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1496 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1502 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1503 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1508 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1510 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1518 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1523 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1525 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1526 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1527 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1528 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1533 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1534 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1535 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1543 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1552 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7571 "bison-chapel.cpp"
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



