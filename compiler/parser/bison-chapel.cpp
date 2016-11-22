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
     TTYPE = 327,
     TUNDERSCORE = 328,
     TUNION = 329,
     TUSE = 330,
     TVAR = 331,
     TWHEN = 332,
     TWHERE = 333,
     TWHILE = 334,
     TWITH = 335,
     TYIELD = 336,
     TZIP = 337,
     TALIAS = 338,
     TAND = 339,
     TASSIGN = 340,
     TASSIGNBAND = 341,
     TASSIGNBOR = 342,
     TASSIGNBXOR = 343,
     TASSIGNDIVIDE = 344,
     TASSIGNEXP = 345,
     TASSIGNLAND = 346,
     TASSIGNLOR = 347,
     TASSIGNMINUS = 348,
     TASSIGNMOD = 349,
     TASSIGNMULTIPLY = 350,
     TASSIGNPLUS = 351,
     TASSIGNSL = 352,
     TASSIGNSR = 353,
     TBAND = 354,
     TBNOT = 355,
     TBOR = 356,
     TBXOR = 357,
     TCOLON = 358,
     TCOMMA = 359,
     TDIVIDE = 360,
     TDOT = 361,
     TDOTDOT = 362,
     TDOTDOTDOT = 363,
     TEQUAL = 364,
     TEXP = 365,
     TGREATER = 366,
     TGREATEREQUAL = 367,
     THASH = 368,
     TLESS = 369,
     TLESSEQUAL = 370,
     TMINUS = 371,
     TMOD = 372,
     TNOT = 373,
     TNOTEQUAL = 374,
     TOR = 375,
     TPLUS = 376,
     TQUESTION = 377,
     TSEMI = 378,
     TSHIFTLEFT = 379,
     TSHIFTRIGHT = 380,
     TSTAR = 381,
     TSWAP = 382,
     TASSIGNREDUCE = 383,
     TIO = 384,
     TLCBR = 385,
     TRCBR = 386,
     TLP = 387,
     TRP = 388,
     TLSBR = 389,
     TRSBR = 390,
     TNOELSE = 391,
     TUMINUS = 392,
     TUPLUS = 393
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
#line 444 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 450 "bison-chapel.cpp"
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
#line 524 "bison-chapel.cpp"

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
#define YYLAST   12986

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  112
/* YYNRULES -- Number of rules.  */
#define YYNRULES  451
/* YYNRULES -- Number of states.  */
#define YYNSTATES  857

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   393

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
     135,   136,   137,   138
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
     114,   117,   121,   123,   126,   128,   132,   136,   142,   143,
     145,   147,   149,   153,   159,   165,   169,   174,   179,   184,
     189,   194,   199,   204,   209,   214,   219,   224,   229,   234,
     239,   244,   249,   250,   252,   254,   256,   258,   261,   263,
     266,   270,   272,   274,   277,   280,   282,   284,   286,   288,
     290,   293,   299,   303,   310,   317,   322,   328,   334,   338,
     342,   349,   355,   362,   368,   375,   379,   384,   391,   399,
     406,   414,   419,   425,   430,   435,   439,   446,   452,   458,
     459,   462,   466,   469,   473,   480,   489,   491,   493,   495,
     496,   499,   500,   503,   507,   513,   515,   518,   522,   524,
     528,   529,   530,   539,   540,   542,   545,   548,   549,   550,
     560,   564,   568,   574,   580,   582,   585,   587,   589,   591,
     593,   595,   597,   599,   601,   603,   605,   607,   609,   611,
     613,   615,   617,   619,   621,   623,   625,   627,   629,   631,
     633,   635,   637,   639,   641,   643,   645,   647,   649,   651,
     653,   655,   657,   658,   662,   666,   667,   669,   673,   678,
     683,   690,   697,   698,   700,   702,   704,   706,   708,   711,
     714,   716,   718,   720,   721,   723,   725,   727,   729,   731,
     732,   734,   737,   739,   741,   743,   745,   747,   749,   751,
     754,   756,   757,   759,   762,   765,   766,   769,   773,   778,
     783,   786,   791,   792,   795,   798,   803,   808,   813,   819,
     824,   825,   827,   829,   831,   835,   839,   844,   850,   852,
     854,   858,   860,   863,   867,   868,   871,   874,   875,   880,
     881,   884,   887,   889,   894,   899,   906,   908,   909,   911,
     913,   917,   922,   926,   931,   938,   939,   942,   945,   948,
     951,   954,   957,   959,   961,   965,   969,   971,   973,   975,
     979,   983,   984,   986,   988,   992,   996,  1000,  1004,  1006,
    1008,  1010,  1012,  1014,  1016,  1018,  1020,  1023,  1028,  1033,
    1038,  1044,  1047,  1050,  1057,  1064,  1069,  1079,  1089,  1097,
    1104,  1111,  1116,  1126,  1136,  1144,  1149,  1156,  1163,  1173,
    1183,  1190,  1192,  1194,  1196,  1198,  1200,  1202,  1204,  1206,
    1210,  1211,  1213,  1218,  1220,  1224,  1229,  1231,  1235,  1238,
    1242,  1246,  1248,  1252,  1255,  1260,  1262,  1264,  1266,  1268,
    1270,  1272,  1274,  1276,  1281,  1285,  1289,  1292,  1295,  1297,
    1299,  1301,  1303,  1305,  1307,  1309,  1314,  1319,  1324,  1328,
    1332,  1336,  1340,  1345,  1349,  1354,  1356,  1358,  1360,  1362,
    1364,  1368,  1373,  1377,  1382,  1386,  1391,  1395,  1401,  1405,
    1409,  1413,  1417,  1421,  1425,  1429,  1433,  1437,  1441,  1445,
    1449,  1453,  1457,  1461,  1465,  1469,  1473,  1477,  1481,  1485,
    1489,  1493,  1496,  1499,  1502,  1505,  1508,  1511,  1515,  1519,
    1523,  1527,  1531,  1535,  1539,  1543,  1545,  1547,  1549,  1551,
    1553,  1555
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     140,     0,    -1,   141,    -1,    -1,   141,   142,    -1,   144,
      -1,   143,   144,    -1,    54,     7,    -1,   143,    54,     7,
      -1,   145,    -1,   147,    -1,   152,    -1,   153,    -1,   160,
      -1,   154,    -1,   162,    -1,   165,    -1,   163,    -1,   166,
      -1,   159,    -1,   228,   123,    -1,    12,   144,    -1,    13,
     229,   144,    -1,    14,   155,   123,    -1,    17,   229,   147,
      -1,    21,   155,   123,    -1,    22,   238,   123,    -1,    39,
     156,   144,    -1,    42,   144,    -1,    48,   238,   158,    -1,
      66,   238,   158,    -1,    66,   158,    -1,    70,   144,    -1,
      81,   238,   123,    -1,     1,    -1,   146,    44,   156,   130,
     131,    -1,   146,    44,   156,   130,   148,   131,    -1,    -1,
      58,    -1,    56,    -1,   130,   131,    -1,   130,   148,   131,
      -1,   142,    -1,   148,   142,    -1,   238,    -1,   238,    11,
     238,    -1,   149,   104,   238,    -1,   149,   104,   238,    11,
     238,    -1,    -1,   149,    -1,   126,    -1,   149,    -1,    75,
     217,   123,    -1,    75,   238,    28,   151,   123,    -1,    75,
     238,    49,   150,   123,    -1,    62,   217,   123,    -1,   239,
      85,   238,   123,    -1,   239,    96,   238,   123,    -1,   239,
      93,   238,   123,    -1,   239,    95,   238,   123,    -1,   239,
      89,   238,   123,    -1,   239,    94,   238,   123,    -1,   239,
      90,   238,   123,    -1,   239,    86,   238,   123,    -1,   239,
      87,   238,   123,    -1,   239,    88,   238,   123,    -1,   239,
      98,   238,   123,    -1,   239,    97,   238,   123,    -1,   239,
     127,   238,   123,    -1,   239,   128,   238,   123,    -1,   239,
      91,   238,   123,    -1,   239,    92,   238,   123,    -1,    -1,
     156,    -1,     3,    -1,   155,    -1,     7,    -1,    24,   144,
      -1,   147,    -1,    63,   123,    -1,    63,   238,   123,    -1,
     123,    -1,   161,    -1,    58,   161,    -1,    56,   161,    -1,
     173,    -1,   201,    -1,   169,    -1,   180,    -1,   204,    -1,
      30,     9,    -1,    24,   144,    79,   238,   123,    -1,    79,
     238,   158,    -1,    18,   238,    34,   238,   229,   158,    -1,
      18,   238,    34,   164,   229,   158,    -1,    18,   238,   229,
     158,    -1,    31,   238,    34,   238,   158,    -1,    31,   238,
      34,   164,   158,    -1,    31,   238,   158,    -1,    31,   164,
     158,    -1,    31,    52,   156,    34,   238,   158,    -1,    32,
     238,    34,   238,   158,    -1,    32,   238,    34,   238,   232,
     158,    -1,    32,   238,    34,   164,   158,    -1,    32,   238,
      34,   164,   232,   158,    -1,    32,   238,   158,    -1,    32,
     238,   232,   158,    -1,   134,   217,    34,   238,   135,   144,
      -1,   134,   217,    34,   238,   232,   135,   144,    -1,   134,
     217,    34,   164,   135,   144,    -1,   134,   217,    34,   164,
     232,   135,   144,    -1,   134,   217,   135,   144,    -1,   134,
     217,   232,   135,   144,    -1,    82,   132,   217,   133,    -1,
      33,   238,    71,   144,    -1,    33,   238,   147,    -1,    33,
     238,    71,   144,    26,   144,    -1,    33,   238,   147,    26,
     144,    -1,    65,   238,   130,   167,   131,    -1,    -1,   167,
     168,    -1,    77,   217,   158,    -1,    50,   144,    -1,    50,
      24,   144,    -1,   170,   156,   171,   130,   172,   131,    -1,
      30,   157,   170,   156,   171,   130,   172,   131,    -1,    16,
      -1,    59,    -1,    74,    -1,    -1,   103,   217,    -1,    -1,
     172,   160,    -1,   172,   143,   160,    -1,    27,   156,   130,
     174,   131,    -1,   175,    -1,   174,   104,    -1,   174,   104,
     175,    -1,   156,    -1,   156,    85,   238,    -1,    -1,    -1,
      40,   177,   187,   178,   194,   212,   200,   196,    -1,    -1,
      36,    -1,    29,   157,    -1,    30,   157,    -1,    -1,    -1,
     179,   193,   181,   183,   182,   194,   212,   200,   195,    -1,
     192,   184,   186,    -1,   192,   185,   186,    -1,   192,   156,
     106,   184,   186,    -1,   192,   156,   106,   185,   186,    -1,
     156,    -1,   100,   156,    -1,    99,    -1,   101,    -1,   102,
      -1,   100,    -1,   109,    -1,   119,    -1,   115,    -1,   112,
      -1,   114,    -1,   111,    -1,   121,    -1,   116,    -1,   126,
      -1,   105,    -1,   124,    -1,   125,    -1,   117,    -1,   110,
      -1,   118,    -1,    15,    -1,   113,    -1,    10,    -1,   127,
      -1,   129,    -1,    85,    -1,    96,    -1,    93,    -1,    95,
      -1,    89,    -1,    94,    -1,    90,    -1,    86,    -1,    87,
      -1,    88,    -1,    98,    -1,    97,    -1,    -1,   132,   188,
     133,    -1,   132,   188,   133,    -1,    -1,   189,    -1,   188,
     104,   189,    -1,   190,   156,   216,   210,    -1,   190,   156,
     216,   199,    -1,   190,   132,   209,   133,   216,   210,    -1,
     190,   132,   209,   133,   216,   199,    -1,    -1,   191,    -1,
      34,    -1,    37,    -1,    51,    -1,    20,    -1,    20,    34,
      -1,    20,    61,    -1,    52,    -1,    61,    -1,    72,    -1,
      -1,    52,    -1,    61,    -1,    72,    -1,    57,    -1,    38,
      -1,    -1,    20,    -1,    20,    61,    -1,    61,    -1,    52,
      -1,    72,    -1,   123,    -1,   196,    -1,   147,    -1,   159,
      -1,   122,   156,    -1,   122,    -1,    -1,   197,    -1,   108,
     238,    -1,   108,   198,    -1,    -1,    78,   238,    -1,    72,
     202,   123,    -1,    19,    72,   202,   123,    -1,    30,    72,
     202,   123,    -1,   156,   203,    -1,   156,   203,   104,   202,
      -1,    -1,    85,   224,    -1,    85,   213,    -1,   205,    52,
     206,   123,    -1,   205,    20,   206,   123,    -1,   205,    61,
     206,   123,    -1,   205,    20,    61,   206,   123,    -1,   205,
      76,   206,   123,    -1,    -1,    19,    -1,    30,    -1,   207,
      -1,   206,   104,   207,    -1,   156,   212,   210,    -1,   156,
     211,    83,   238,    -1,   132,   209,   133,   212,   210,    -1,
      73,    -1,   156,    -1,   132,   209,   133,    -1,   208,    -1,
     208,   104,    -1,   208,   104,   209,    -1,    -1,    85,    47,
      -1,    85,   238,    -1,    -1,   103,   134,   217,   135,    -1,
      -1,   103,   224,    -1,   103,   213,    -1,     1,    -1,   134,
     217,   135,   224,    -1,   134,   217,   135,   213,    -1,   134,
     217,    34,   238,   135,   224,    -1,     1,    -1,    -1,   224,
      -1,   197,    -1,   134,   135,   214,    -1,   134,   217,   135,
     214,    -1,   134,   135,   215,    -1,   134,   217,   135,   215,
      -1,   134,   217,    34,   238,   135,   214,    -1,    -1,   103,
     224,    -1,   103,   197,    -1,   103,    25,    -1,   103,    67,
      -1,   103,    70,    -1,   103,   215,    -1,   238,    -1,   197,
      -1,   217,   104,   238,    -1,   217,   104,   197,    -1,    73,
      -1,   238,    -1,   197,    -1,   218,   104,   218,    -1,   219,
     104,   218,    -1,    -1,   221,    -1,   222,    -1,   221,   104,
     222,    -1,   156,    85,   197,    -1,   156,    85,   238,    -1,
     156,    83,   238,    -1,   197,    -1,   238,    -1,   156,    -1,
     227,    -1,   239,    -1,   226,    -1,   247,    -1,   246,    -1,
      67,   238,    -1,    35,   132,   220,   133,    -1,    25,   132,
     220,   133,    -1,    69,   132,   220,   133,    -1,    68,    69,
     132,   220,   133,    -1,    12,   238,    -1,    70,   238,    -1,
      31,   238,    34,   238,    24,   238,    -1,    31,   238,    34,
     164,    24,   238,    -1,    31,   238,    24,   238,    -1,    31,
     238,    34,   238,    24,    33,   238,    71,   238,    -1,    31,
     238,    34,   164,    24,    33,   238,    71,   238,    -1,    31,
     238,    24,    33,   238,    71,   238,    -1,    32,   238,    34,
     238,    24,   238,    -1,    32,   238,    34,   164,    24,   238,
      -1,    32,   238,    24,   238,    -1,    32,   238,    34,   238,
      24,    33,   238,    71,   238,    -1,    32,   238,    34,   164,
      24,    33,   238,    71,   238,    -1,    32,   238,    24,    33,
     238,    71,   238,    -1,   134,   217,   135,   238,    -1,   134,
     217,    34,   238,   135,   238,    -1,   134,   217,    34,   164,
     135,   238,    -1,   134,   217,    34,   238,   135,    33,   238,
      71,   238,    -1,   134,   217,    34,   164,   135,    33,   238,
      71,   238,    -1,    33,   238,    71,   238,    26,   238,    -1,
      46,    -1,   227,    -1,   223,    -1,   242,    -1,   241,    -1,
     176,    -1,   236,    -1,   237,    -1,   235,   129,   238,    -1,
      -1,   230,    -1,    80,   132,   231,   133,    -1,   234,    -1,
     231,   104,   234,    -1,    80,   132,   233,   133,    -1,   234,
      -1,   233,   104,   234,    -1,   191,   223,    -1,   238,    60,
     223,    -1,   250,    60,   223,    -1,   239,    -1,   235,   129,
     238,    -1,    45,   238,    -1,    41,   206,    34,   238,    -1,
     244,    -1,   224,    -1,   225,    -1,   248,    -1,   249,    -1,
     176,    -1,   236,    -1,   237,    -1,   132,   108,   238,   133,
      -1,   238,   103,   238,    -1,   238,   107,   238,    -1,   238,
     107,    -1,   107,   238,    -1,   107,    -1,   223,    -1,   241,
      -1,   242,    -1,   243,    -1,   239,    -1,   176,    -1,   240,
     132,   220,   133,    -1,   240,   134,   220,   135,    -1,    55,
     132,   220,   133,    -1,   238,   106,   156,    -1,   238,   106,
      72,    -1,   238,   106,    25,    -1,   132,   218,   133,    -1,
     132,   218,   104,   133,    -1,   132,   219,   133,    -1,   132,
     219,   104,   133,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,   130,   217,   131,    -1,   130,   217,
     104,   131,    -1,   134,   217,   135,    -1,   134,   217,   104,
     135,    -1,   134,   245,   135,    -1,   134,   245,   104,   135,
      -1,   238,    83,   238,    -1,   245,   104,   238,    83,   238,
      -1,   238,   121,   238,    -1,   238,   116,   238,    -1,   238,
     126,   238,    -1,   238,   105,   238,    -1,   238,   124,   238,
      -1,   238,   125,   238,    -1,   238,   117,   238,    -1,   238,
     109,   238,    -1,   238,   119,   238,    -1,   238,   115,   238,
      -1,   238,   112,   238,    -1,   238,   114,   238,    -1,   238,
     111,   238,    -1,   238,    99,   238,    -1,   238,   101,   238,
      -1,   238,   102,   238,    -1,   238,    84,   238,    -1,   238,
     120,   238,    -1,   238,   110,   238,    -1,   238,    15,   238,
      -1,   238,    10,   238,    -1,   238,   113,   238,    -1,   238,
      23,   238,    -1,   121,   238,    -1,   116,   238,    -1,    43,
     238,    -1,    53,   238,    -1,   118,   238,    -1,   100,   238,
      -1,   238,    60,   238,    -1,   238,    60,   164,    -1,   250,
      60,   238,    -1,   250,    60,   164,    -1,   238,    64,   238,
      -1,   238,    64,   164,    -1,   250,    64,   238,    -1,   250,
      64,   164,    -1,   121,    -1,   126,    -1,    84,    -1,   120,
      -1,    99,    -1,   101,    -1,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   421,   421,   426,   427,   433,   434,   439,   440,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   474,   476,   481,   482,   483,
     498,   499,   504,   505,   510,   515,   520,   524,   530,   535,
     539,   544,   548,   549,   550,   554,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   577,   578,   582,   586,   587,   591,   592,   596,
     597,   601,   602,   603,   604,   605,   606,   607,   611,   612,
     616,   631,   632,   633,   634,   635,   636,   637,   638,   639,
     640,   641,   642,   643,   644,   645,   646,   647,   653,   659,
     665,   671,   678,   688,   692,   693,   694,   695,   699,   704,
     705,   709,   711,   713,   720,   725,   733,   738,   743,   751,
     752,   757,   758,   760,   765,   775,   784,   788,   796,   797,
     802,   807,   801,   832,   838,   845,   853,   864,   870,   863,
     898,   902,   907,   911,   919,   920,   921,   922,   923,   924,
     925,   926,   927,   928,   929,   930,   931,   932,   933,   934,
     935,   936,   937,   938,   939,   940,   941,   942,   943,   944,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   963,   964,   968,   972,   973,   974,   978,   980,
     982,   984,   989,   990,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1002,  1006,  1007,  1008,  1009,  1013,  1014,  1018,
    1019,  1020,  1021,  1022,  1023,  1027,  1028,  1032,  1033,  1037,
    1039,  1044,  1045,  1049,  1050,  1054,  1055,  1059,  1061,  1063,
    1068,  1081,  1098,  1099,  1101,  1106,  1114,  1122,  1130,  1139,
    1149,  1150,  1151,  1155,  1156,  1164,  1166,  1172,  1177,  1179,
    1181,  1186,  1188,  1190,  1197,  1198,  1199,  1203,  1204,  1209,
    1210,  1211,  1212,  1232,  1236,  1240,  1248,  1252,  1253,  1254,
    1258,  1260,  1266,  1268,  1270,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1287,  1288,  1289,  1290,  1294,  1295,  1296,  1300,
    1301,  1305,  1306,  1310,  1311,  1315,  1316,  1317,  1318,  1319,
    1323,  1334,  1335,  1336,  1337,  1338,  1339,  1341,  1343,  1345,
    1347,  1349,  1351,  1356,  1358,  1360,  1362,  1364,  1366,  1368,
    1370,  1372,  1374,  1376,  1378,  1380,  1387,  1393,  1399,  1405,
    1414,  1419,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1434,
    1439,  1440,  1444,  1449,  1452,  1457,  1462,  1465,  1470,  1474,
    1475,  1479,  1480,  1485,  1490,  1498,  1499,  1500,  1501,  1502,
    1503,  1504,  1505,  1506,  1508,  1510,  1512,  1514,  1516,  1521,
    1522,  1523,  1524,  1535,  1536,  1540,  1541,  1542,  1546,  1547,
    1548,  1556,  1557,  1558,  1559,  1563,  1564,  1565,  1566,  1567,
    1568,  1569,  1570,  1571,  1572,  1576,  1584,  1585,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,
    1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,  1610,
    1611,  1615,  1616,  1617,  1618,  1619,  1620,  1624,  1625,  1626,
    1627,  1631,  1632,  1633,  1634,  1639,  1640,  1641,  1642,  1643,
    1644,  1645
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
  "TSTAR", "TSWAP", "TASSIGNREDUCE", "TIO", "TLCBR", "TRCBR", "TLP", "TRP",
  "TLSBR", "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "access_control", "block_stmt", "stmt_ls", "only_ls",
  "opt_only_ls", "except_ls", "use_stmt", "require_stmt",
  "assignment_stmt", "opt_ident", "ident", "opt_ident_or_string",
  "do_stmt", "return_stmt", "class_level_stmt", "private_decl",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_function_body_stmt",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   139,   140,   141,   141,   142,   142,   143,   143,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   145,   145,   146,   146,   146,
     147,   147,   148,   148,   149,   149,   149,   149,   150,   150,
     151,   151,   152,   152,   152,   153,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   155,   155,   156,   157,   157,   158,   158,   159,
     159,   160,   160,   160,   160,   160,   160,   160,   161,   161,
     162,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   164,   165,   165,   165,   165,   166,   167,
     167,   168,   168,   168,   169,   169,   170,   170,   170,   171,
     171,   172,   172,   172,   173,   174,   174,   174,   175,   175,
     177,   178,   176,   179,   179,   179,   179,   181,   182,   180,
     183,   183,   183,   183,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   187,   188,   188,   188,   189,   189,
     189,   189,   190,   190,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   192,   192,   192,   192,   193,   193,   194,
     194,   194,   194,   194,   194,   195,   195,   196,   196,   197,
     197,   198,   198,   199,   199,   200,   200,   201,   201,   201,
     202,   202,   203,   203,   203,   204,   204,   204,   204,   204,
     205,   205,   205,   206,   206,   207,   207,   207,   208,   208,
     208,   209,   209,   209,   210,   210,   210,   211,   211,   212,
     212,   212,   212,   213,   213,   213,   213,   214,   214,   214,
     215,   215,   215,   215,   215,   216,   216,   216,   216,   216,
     216,   216,   217,   217,   217,   217,   218,   218,   218,   219,
     219,   220,   220,   221,   221,   222,   222,   222,   222,   222,
     223,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     226,   227,   228,   228,   228,   228,   228,   228,   228,   228,
     229,   229,   230,   231,   231,   232,   233,   233,   234,   234,
     234,   235,   235,   236,   237,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   239,
     239,   239,   239,   240,   240,   241,   241,   241,   242,   242,
     242,   243,   243,   243,   243,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   245,   245,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   247,   247,   247,   247,   247,   247,   248,   248,   248,
     248,   249,   249,   249,   249,   250,   250,   250,   250,   250,
     250,   250
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     1,     3,     3,     5,     0,     1,
       1,     1,     3,     5,     5,     3,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     0,     1,     1,     1,     1,     2,     1,     2,
       3,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       2,     5,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     4,     3,     6,     5,     5,     0,
       2,     3,     2,     3,     6,     8,     1,     1,     1,     0,
       2,     0,     2,     3,     5,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     0,     0,     9,
       3,     3,     5,     5,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     0,     1,     3,     4,     4,
       6,     6,     0,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     0,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     0,     1,     2,     2,     0,     2,     3,     4,     4,
       2,     4,     0,     2,     2,     4,     4,     4,     5,     4,
       0,     1,     1,     1,     3,     3,     4,     5,     1,     1,
       3,     1,     2,     3,     0,     2,     2,     0,     4,     0,
       2,     2,     1,     4,     4,     6,     1,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     4,     1,     3,     4,     1,     3,     2,     3,
       3,     1,     3,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
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
       3,     0,     0,     1,    34,    74,   395,   396,   397,   398,
     399,     0,   350,    72,   126,   350,     0,   251,    72,     0,
       0,     0,     0,    72,    72,     0,     0,     0,     0,   144,
       0,   140,     0,     0,     0,     0,   341,     0,     0,     0,
       0,   250,   250,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   128,     0,     0,     0,   447,   449,     0,
     450,   451,   378,     0,     0,   448,   445,    81,   446,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   310,    19,    13,    82,    15,    17,    16,    18,    87,
       0,    85,   370,     0,    88,    86,    89,     0,   379,   366,
     367,   313,   311,     0,     0,   371,   372,     0,   312,     0,
     380,   381,   382,   365,   315,   314,   368,   369,     0,    21,
     321,     0,     0,   351,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,   370,   379,   311,   371,   372,   350,
     312,   380,   381,     0,     0,     0,     0,   301,     0,    76,
      75,   145,    90,     0,   146,     0,     0,     0,     0,     0,
       0,   301,     0,     0,     0,     0,     0,   253,    28,   433,
     363,     0,   434,     7,   301,   251,   252,    84,    83,   230,
     293,     0,   292,    79,     0,     0,     0,    78,    31,     0,
     316,     0,   301,    32,   322,   242,     0,     0,   292,     0,
       0,   436,   377,   432,   435,   431,    40,    42,     0,     0,
     296,     0,   298,     0,     0,   297,     0,   292,     0,     0,
       6,     0,   129,   218,   217,   147,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   376,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,   301,     0,     0,
       0,    22,    23,     0,    24,     0,     0,     0,     0,     0,
       0,     0,    25,    26,     0,   310,   308,     0,   302,   303,
     309,     0,     0,     0,     0,     0,    99,     0,     0,    98,
       0,     0,     0,   105,     0,     0,   115,     0,    27,   195,
     141,   258,     0,   259,   261,     0,   272,     0,     0,   264,
       0,     0,    29,     0,   146,   229,     0,    55,    80,   119,
      77,    30,   301,     0,     0,   240,   237,    52,     0,    48,
      92,    33,    41,    43,     0,   400,     0,     0,   391,     0,
     393,     0,     0,     0,     0,     0,     0,   404,     8,     0,
       0,     0,   213,     0,     0,     0,     0,     0,   349,   428,
     427,   430,   438,   437,   442,   441,   424,   421,   422,   423,
     374,   411,   390,   389,   388,   375,   415,   426,   420,   418,
     429,   419,   417,   409,   414,   416,   425,   408,   412,   413,
     410,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   440,
     439,   444,   443,   207,   204,   205,   206,   210,   211,   212,
       0,     0,   353,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   350,   350,    95,   238,     0,     0,     0,   318,
       0,   138,     0,   135,   239,   129,     0,     0,     0,   325,
       0,     0,     0,   331,     0,     0,     0,   106,   114,     0,
       0,   317,     0,   196,     0,   203,   219,     0,   262,     0,
     276,     0,   271,   366,     0,     0,   255,   364,   254,   387,
     295,   294,     0,     0,   319,     0,   244,   366,     0,   446,
      51,     0,    44,    49,     0,   401,   373,   392,   299,   394,
     300,     0,     0,   403,   111,   335,     0,   406,   405,     0,
       0,   130,   131,   214,   215,   216,   148,     0,     0,   246,
     245,   247,   249,    56,    63,    64,    65,    60,    62,    70,
      71,    58,    61,    59,    57,    67,    66,    68,    69,   385,
     386,   208,   209,   358,     0,   352,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,   307,
     305,   306,   304,     0,   136,   134,     0,     0,   113,     0,
       0,    97,     0,    96,     0,     0,   103,     0,     0,   101,
       0,     0,   356,     0,     0,   117,   202,   194,     0,   285,
     220,   223,   222,   224,     0,   260,   263,     0,   264,     0,
     256,   265,   266,     0,     0,   118,   120,   320,     0,   241,
       0,    53,     0,    54,     0,     0,     0,     0,   112,     0,
      35,     0,   250,   219,   177,   175,   180,   187,   188,   189,
     184,   186,   182,   185,   183,   181,   191,   190,   156,   159,
     157,   158,   169,   160,   173,   165,   163,   176,   164,   162,
     167,   172,   174,   161,   166,   170,   171,   168,   178,   179,
     154,   192,   192,   248,   354,   379,   379,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    93,   139,   137,   131,
     100,     0,     0,   324,     0,   323,     0,     0,   330,   104,
       0,   329,   102,     0,   355,   116,   340,   197,     0,     0,
     264,   221,   235,   257,     0,     0,     0,   122,     0,     0,
      46,    45,     0,   109,   337,     0,     0,   107,   336,     0,
     407,    36,    72,   250,   250,   124,   250,   132,     0,   155,
       0,   195,   150,   151,     0,     0,     0,     0,     0,     0,
       0,     0,   250,   328,     0,     0,   334,     0,     0,   357,
     285,   288,   289,   290,     0,   287,   291,   366,   231,   199,
     198,     0,     0,     0,   274,   366,   123,   121,     0,     0,
     110,     0,   108,   133,   235,   154,   192,   192,     0,     0,
       0,     0,     0,     0,     0,   125,     0,     0,     0,     0,
     264,   277,     0,   232,   234,   233,   236,   227,   228,   142,
       0,    47,     0,     0,     0,   152,   153,   193,     0,     0,
       0,     0,     0,     0,   327,   326,   333,   332,   201,   200,
     279,   280,   282,   366,     0,   402,   366,   339,   338,   225,
     149,   226,     0,   281,   283,   277,   284
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   478,    75,    76,    77,   208,
     510,   514,   511,    78,    79,    80,   150,    81,   154,   188,
      82,    83,    84,    85,    86,   574,    87,    88,   502,   626,
      89,    90,   371,   642,    91,   462,   463,   134,   163,   486,
      93,    94,   372,   643,   536,   681,   682,   752,   320,   482,
     483,   484,   440,   537,   225,   614,   850,   819,   180,   814,
     779,   782,    95,   196,   345,    96,    97,   166,   167,   324,
     325,   496,   328,   329,   492,   841,   776,   720,   209,   213,
     214,   297,   298,   299,   135,    99,   100,   101,   136,   103,
     122,   123,   441,   314,   601,   442,   104,   137,   138,   107,
     140,   109,   141,   142,   112,   113,   218,   114,   115,   116,
     117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -761
static const yytype_int16 yypact[] =
{
    -761,    82,  2355,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  3561,    18,   134,  -761,    18,  7524,    27,   134,  7524,
    3561,    10,   134,   394,   891,  6204,  7524,  7524,    20,  -761,
     134,  -761,    36,  3561,  7524,  7524,  -761,  7524,  7524,   186,
      69,   364,   619,  -761,  6468,  6600,  7524,  6732,  7524,   119,
      78,  3561,   134,  -761,  6468,  7524,  7524,  -761,  -761,  7524,
    -761,  -761,  8844,  7524,  7524,  -761,  7524,  -761,  -761,  2769,
    5808,  6468,  -761,  3429,  -761,  -761,   160,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
     134,  -761,   257,   251,  -761,  -761,  -761,    52,   109,  -761,
    -761,  -761,   150,   190,   200,   212,   213, 12686,  1436,   -43,
     219,   224,  -761,  -761,  -761,  -761,  -761,  -761,   352,  -761,
   12686,   209,  3561,  -761,   234,  -761,   238,  7524,  7524,  7524,
    7524,  7524,  6468,  6468,    64,  -761,  -761,  -761,  -761, 10223,
     286,  -761,  -761,   134,   246, 10517,   306,  6468,   266,  -761,
    -761,  -761,  -761,   134,    70,   134,   267,    43,  9268,  9234,
    9434,  6468,  3561,   274,    19,    56,    28,  -761,  -761,   541,
     301,  9497,   541,  -761,  6468,  -761,   273,  -761,  -761,   134,
    -761,   -23, 12686,  -761, 10551, 10102,  3561,  -761,  -761,  9497,
   12686,   285,  6468,  -761, 12686,   338,   304,   216, 10331,  9497,
   10636,   268,  1623,   541,   268,   541,  -761,  -761,  2901,   178,
    -761,  7524,  -761,   -25,    74, 12686,    54, 10680,   -34,   423,
    -761,   134,   340,  -761,  -761,  -761,    14,    36,    36,    36,
    -761,  7524,  7524,  7524,  7524,  6864,  6864,  7524,  7524,  7524,
    7524,  7524,  7524,    71,  8844,  7524,  7524,  7524,  7524,  7524,
    7524,  7524,  7524,  7524,  7524,  7524,  7524,  7524,  7524,  7524,
    7524,  7524,  7524,  7524,  7524,  7524,  7524,  7524,  7524,  7524,
    7524,  7524,  7524,  7524,  7524,  7524,  6468,  6468,  6864,  6864,
    5676,  -761,  -761,  3033,  -761, 10375, 10483, 10755,    60,  6864,
      43,   318,  -761,  -761,  7524,   339,  -761,   320,   347,  -761,
   12686,   134,   331,   134,   422,  6468,  -761,  3693,  6864,  -761,
    3825,  6864,   326,  -761,    43,  3561,   433,   329,  -761,   353,
    -761,  -761,    19,  -761,   360,   330,  -761,  2501,   383,   382,
    7524,    36,  -761,   337,  -761,  -761,  6468,  -761,  -761,  -761,
    -761,  -761,  6468,   342,  5145,   373,  -761,  -761,  7656,  7524,
    -761,  -761,  -761,  -761,  6336,  -761,  9126,  5940,  -761,  6072,
    -761,  6864,  5277,  2637,   351,  7524,  5543,  -761,  -761,   369,
    6468,   370,   288,    36,   230,   240,   241,   254, 10160, 12802,
   12802,    96,  -761,    96,  -761,    96,  9302,   663,  1392,   437,
     301,   268,  -761,  -761,  -761,  1623,  1119,    96,  1218,  1218,
   12802,  1218,  1218,   505,   268,  1119, 12860,   505,   541,   541,
     268, 10799, 10867, 10911, 10979, 11023, 11091, 11135, 11203, 11247,
   11315, 11359, 11427, 11471, 11539, 11583, 11651,   349,   375,  -761,
      96,  -761,    96,   157,  -761,  -761,  -761,  -761,  -761,  -761,
     134,    86,  -761, 12744,   355,  7788,  6864,  7920,  6864,  7524,
    6864,   984,    18, 11709,  -761,  -761, 11772,  7524,  6468,  -761,
    6468,   427,   202,  -761,  -761,   340,  7524,   177,  7524, 12686,
      47,  9555,  7524, 12686,    23,  9376,  5676,  -761,   482, 11839,
    3561,  -761,   181,  -761,    39,  -761,   142,   380,    19,    63,
    -761,  6468,  -761,   348,  7524,  6996,  -761, 12686,  -761,  -761,
    -761, 12686,    -1,   387,  -761,  6468,  -761,   258,   134,   399,
     419,   401, 11897,   419,   402,  -761,  -761,  -761,  -761,  -761,
    -761,   -30,  8965,  -761,  -761, 12686,  3561, 12686,  -761, 11960,
    3165,   434,  -761,  -761,  -761,  -761,  -761,  2219,   263,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  5676,  -761,  6864,  6864,  7524,   503,
   12018,  7524,   507, 12081,   406,  2079,    43,    43,  -761, 12686,
    -761, 12686,  -761,  7524,   134,  -761,   407,  9497,  -761,  9618,
    3957,  -761,  4089,  -761,  9676,  4221,  -761,    43,  4353,  -761,
      43,   182,  -761,  3561,  7524,  -761,   454,  -761,    19,   441,
     485,  -761,  -761,  -761,    62,  -761,  -761,  5145,   382,    76,
   12686,  -761, 12686,  4485,  6468,  -761,  -761,  -761,    80,  -761,
    7524,  -761,  7524,  -761,  4617,   413,  4749,   414,  -761,  7524,
    -761,  3297,   460,   142,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,   134,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,  -761,
     444,   420,   420,  -761,  -761,   183,   188, 12139,  8052,  8184,
   12202,  8316,  8448,  8580,  8712,  -761,  -761, 12686,  -761,  -761,
    -761,  3561,  7524, 12686,  7524, 12686,  3561,  7524, 12686,  -761,
    7524, 12686,  -761,  5676,  -761,  -761, 12686,  -761,   418,  7128,
     218,  -761,   480,  -761,  6864,  4881,  3561,  -761,    45,  5013,
   12260, 12686,  7524,  -761, 12686,  3561,  7524,  -761, 12686,  3561,
   12686,  -761,   589,   473,   473,  -761,   681,  -761,    24,  -761,
    2219,   353,  -761,  -761,  7524,  7524,  7524,  7524,  7524,  7524,
    7524,  7524,   874, 11839,  9739,  9797, 11839,  9860,  9918,  -761,
     441,    10,  7524,  7524,  5410,  -761,  -761,   -24,  6468,  -761,
    -761,  7524,     3,  9009,  -761,   722,   306,  -761,  7524,  9981,
    -761, 10039,  -761,  -761,   480,  -761,   420,   420,   191, 12323,
   12381, 12444, 12502, 12565, 12623,  -761,  3561,  3561,  3561,  3561,
     218,  7260,    81,  -761,  -761, 12686, 12686,  -761,  -761,  -761,
    8712, 12686,  3561,  3561,   154,  -761,  -761,  -761,  7524,  7524,
    7524,  7524,  7524,  7524, 11839, 11839, 11839, 11839,  -761,  -761,
    -761,  -761,  -761,   194,  6864,  8926,   747, 11839, 11839,  -761,
    -761,  -761,  9082,  -761,  -761,  7392,  -761
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -761,  -761,  -761,     4,  -588,  1989,  -761,  -761,  1503,    29,
     217,  -761,  -761,  -761,  -761,  -761,    65,  1980,    -4,    -2,
    -717,  -601,   -40,  -761,  -761,    87,  -761,  -761,  -761,  -761,
    -761,   416,   106,  -126,  -761,  -761,    -6,   566,  -761,  -761,
    -761,  -761,  -761,  -761,  -761,  -174,  -170,  -614,  -761,  -169,
     -18,  -761,  -301,  -761,  -761,   -54,  -761,  -234,  1126,  -761,
    -216,  -194,  -761,  -133,  -761,  -761,  -761,  -197,   271,  -761,
    -318,  -597,  -761,  -476,  -341,  -760,  -734,  -173,     2,    72,
    -761,   -68,  -761,   143,   359,  -316,  -761,  -761,   773,  -761,
      -8,  -761,  -761,  -204,  -761,  -438,  -761,   836,  1030,   -11,
    1075,  -761,  1237,  1431,  -761,  -761,  -761,  -761,  -761,  -761,
    -761,  -271
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -403
static const yytype_int16 yytable[] =
{
     120,   177,   178,   506,   487,   139,    72,   126,   145,   444,
     291,   493,   364,   618,   158,   159,   160,     5,   485,   151,
     302,   723,     5,   169,   170,   326,   171,   172,   507,   374,
     375,   376,   377,   182,   184,   185,   189,   190,   602,     5,
     194,   747,     5,   198,   199,   200,   181,   595,   201,   623,
     312,   202,   203,   204,   746,   205,   197,   326,   182,   215,
     217,  -286,   330,   326,   326,   818,    45,   186,   753,   186,
     366,   590,   226,   216,     5,   373,   624,   842,   124,   357,
    -286,   336,     3,   144,  -286,   853,    14,  -269,   361,   276,
    -269,   277,   321,   317,   450,   856,   392,  -269,   121,   143,
     337,   367,  -269,   312,   227,   634,   333,   818,   358,  -286,
     724,   854,   157,   228,   724,   844,   120,   285,   286,   287,
     194,   182,   217,   780,   343,  -269,   684,   617,   229,    43,
     625,   290,   331,   283,   312,   288,   300,     5,   722,  -267,
    -269,  -269,   147,   393,    53,   793,   164,  -269,  -269,   336,
     300,   322,   161,   283,  -269,   306,   309,   313,   362,   327,
    -269,   747,   610,   300,   362,   617,   617,  -269,   164,   332,
     616,   608,   334,   283,   746,   283,   538,   283,   359,  -269,
     362,   300,   825,   826,   362,   362,  -269,   341,   191,   363,
     564,   561,  -269,   173,   611,   451,  -384,   350,  -384,   241,
     356,   174,   243,   612,   221,   444,   246,   360,   427,   428,
     192,   725,   353,   839,   613,   729,   845,    45,   562,   565,
     378,   379,   380,   381,   383,   385,   386,   387,   388,   389,
     390,   391,  -343,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   300,   300,   430,   432,   443,
     597,   600,   794,  -342,   503,   769,     5,   849,   453,  -278,
     149,   336,   354,   456,   283,   606,   713,  -359,   454,   223,
     718,   234,  -360,   444,   182,   606,   469,   471,  -278,   473,
     475,   493,  -278,   495,   479,   485,   584,   467,   224,   355,
     588,   -72,   477,   230,   607,   714,  -359,   635,   637,   497,
     336,  -360,   382,   384,   827,   501,   778,  -278,   235,   231,
     -72,   300,   236,   585,   331,  -347,  -348,   512,   512,   347,
     533,   280,  -345,   501,   331,   331,   215,  -344,   215,   534,
     522,   501,   525,   539,   527,   529,  -202,   282,   331,   182,
     535,    98,  -243,   540,   541,   429,   431,   331,   283,   292,
      98,   241,   531,   433,   243,   629,   452,   542,   246,    98,
    -346,  -243,  -270,   175,   784,   294,   683,   434,   784,  -384,
     435,  -384,    98,    23,   176,   470,   301,     5,   474,   305,
      29,   149,  -143,   777,   436,   437,   319,   243,   -39,   785,
      98,  -270,   278,   785,   438,   567,   279,   342,  -383,   279,
    -383,  -143,   457,   344,   458,   439,  -270,   346,    98,   518,
     368,   520,    98,  -270,   469,   570,   473,   573,   479,   575,
     525,   455,   444,   370,   576,   577,   579,   581,   521,   300,
     485,   460,  -270,   459,   464,   587,   466,   589,   476,   480,
     234,   594,   481,   489,   488,   443,   494,   495,   591,   593,
     499,  -270,   596,   599,   433,   504,    14,   508,  -270,    17,
     217,    98,   559,   620,   622,  -202,   526,    22,   434,    23,
     742,   435,   175,   619,   217,   843,    29,   235,  -143,   530,
     532,   236,    23,   176,   846,   436,   437,   628,   603,    29,
     560,  -143,   583,   615,    39,   438,   743,  -143,   744,    43,
     627,    98,   -50,   630,   631,   633,   439,   688,   234,   843,
    -143,   691,    52,   569,    53,   572,   238,   699,   336,   843,
     241,   693,   242,   243,   719,    98,   721,   246,   735,   739,
     750,   770,   751,   443,   253,   383,   430,   687,   781,   641,
     690,   257,   258,   259,   234,   235,   513,    98,    92,   236,
     303,   586,   697,   762,   695,   696,   796,    92,   698,   703,
     797,   705,   798,    67,   708,   700,    92,   711,   717,   748,
     851,   745,     5,   716,   838,   709,   149,   810,   712,    92,
     824,   235,   498,   582,   238,   236,   239,   240,   241,  -252,
     242,   243,     0,   182,     0,   246,     0,    92,     0,   730,
       0,   731,   253,   734,     0,   738,   728,     0,   740,   257,
     258,   259,     0,     0,     0,    92,     0,     0,   175,    92,
       0,  -252,    98,     0,   241,   353,   242,   243,    23,   176,
    -252,   246,     0,   382,   429,    29,     0,  -143,   253,     0,
       0,   153,     0,   -38,     0,  -252,    98,   259,     0,    98,
       0,     0,     0,     0,    98,     0,  -143,   703,   705,     0,
     708,   711,   734,   738,     0,     0,   234,     0,    92,     0,
     763,   764,     0,   765,     0,   766,   767,    14,     0,   768,
      17,     0,   443,   177,   178,     0,     0,     0,    22,     0,
      23,   742,     0,   783,   525,     0,     0,    29,   525,  -143,
       0,   789,    98,   235,     0,   791,   787,   236,    92,     0,
       0,     0,     0,     0,     0,   219,     0,   743,  -143,   744,
      43,     0,     0,   763,   799,   800,   766,   801,   802,   803,
     804,     0,    92,    52,     0,    53,  -273,     0,     0,     0,
       0,   190,   194,   217,     0,     0,   241,   815,   242,   243,
     816,     0,     0,   246,    92,   102,   812,   821,     0,     0,
     253,  -275,     0,     0,   102,  -273,     0,   257,   258,   259,
       0,     0,     0,   102,     0,   834,   835,   836,   837,   563,
    -273,     0,     0,     0,    67,     0,   102,  -273,     0,   738,
    -275,   847,   848,     0,     0,     0,     0,   834,   835,   836,
     837,   847,   848,     0,   102,  -275,  -273,     0,     0,     0,
       0,     0,  -275,   852,   525,     0,     0,     0,   105,    98,
       0,     0,   102,     0,   738,  -273,   102,   105,     0,    92,
       0,  -275,  -273,     0,     0,     0,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
    -275,     0,     0,    92,     0,     0,    92,  -275,     0,     0,
       0,    92,     0,     0,     0,    98,     0,   105,     0,    98,
      14,     0,     0,    17,     5,   102,     0,     0,   149,     0,
     152,    22,     0,    23,   742,   105,     0,     0,     0,   105,
      29,  -252,  -143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   685,   686,     0,    39,    92,
     743,  -143,   744,    43,     0,   102,     0,     0,     0,     0,
       0,     0,     0,  -252,     0,     0,    52,     0,    53,    98,
       0,    98,  -252,     0,    98,     0,     0,    98,   105,   102,
       0,     0,    98,   153,     0,     0,     0,  -252,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,    98,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    98,     0,    98,   127,    67,   105,     0,
      98,     0,     0,     0,     0,   805,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,   105,     0,    31,    32,     0,    34,     0,    35,
      36,     0,   106,     0,     0,     0,     0,    38,     0,    40,
       0,   106,     0,     0,   105,     0,    92,     0,     0,     0,
     106,    48,    49,    50,   131,     0,   102,     0,     0,     0,
      98,     0,     0,   106,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
     102,   106,     0,   102,    59,    98,   108,     0,   102,     0,
       0,     0,    92,     0,    98,   108,    92,     0,    98,   106,
       0,     0,    64,   106,     0,     0,     0,     0,   108,     0,
       0,     0,     0,     0,     0,     0,    70,     0,   133,   105,
       0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,   234,   105,   108,     0,   105,     0,   108,     0,
       0,   105,   106,     0,     0,     0,    92,     0,    92,     0,
       0,    92,     0,     0,    92,    98,    98,    98,    98,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,    98,    98,   236,     0,     0,     0,     0,     0,    92,
       0,     0,   106,     0,     0,     0,   212,   108,     0,   105,
      92,     0,    92,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,     0,   246,
     247,   248,     0,   250,   251,   252,   253,   108,   106,   110,
     256,   234,     0,   257,   258,   259,     0,     0,   110,     0,
       0,     0,     0,   102,     0,     0,     0,   110,     0,     0,
       0,   108,     0,     0,     0,     0,     0,    92,     0,     0,
     110,     0,    92,   296,     0,     0,     0,     0,   235,     0,
       0,     0,   236,   108,     0,     0,     0,   296,   110,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,   102,
     296,    92,     0,   102,     0,    92,   110,     0,     0,     0,
     110,     0,     0,   106,     0,     0,   105,   238,   296,   239,
     240,   241,     0,   242,   243,   244,     0,     0,   246,     0,
       0,     0,     0,     0,   252,   253,     0,   106,     0,   256,
     106,     0,   257,   258,   259,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   108,   110,
       0,     0,   105,   102,     0,   102,   105,     0,   102,     0,
       0,   102,    92,    92,    92,    92,   102,     0,     0,     0,
       0,     0,   108,     0,     0,   108,     0,     0,    92,    92,
     108,     0,     0,   106,     0,     0,   102,     0,     0,   110,
       0,     0,   296,   296,     0,     0,     0,   102,     0,   102,
       0,     0,     0,     0,   102,   234,     0,     0,     0,     0,
       0,     0,     0,   110,     0,     0,   105,     0,   105,     0,
       0,   105,     0,   111,   105,     0,     0,     0,   108,   105,
       0,     0,   111,     0,     0,   110,     0,     0,     0,     0,
       0,   111,   235,     0,     0,     0,   236,     0,     0,   105,
       0,     0,   500,     0,   111,     0,     0,     0,   296,     0,
     105,     0,   105,     0,   102,     0,     0,   105,     0,   102,
     500,     0,   111,   212,     0,   212,     0,     0,   500,     0,
       0,   238,     0,     0,   240,   241,     0,   242,   243,   102,
     111,     0,   246,     0,   111,     0,     0,     0,   102,   253,
     106,     0,   102,     0,     0,     0,   257,   258,   259,     0,
     110,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,     0,     0,   105,     0,     0,
       0,     0,   105,     0,   110,     0,     0,   110,     0,     0,
     187,     0,   110,   111,     0,   108,   106,     0,     0,     0,
     106,     0,   105,   274,   275,  -361,     0,     0,  -383,     0,
    -383,   105,     0,     0,     0,   105,     0,     0,     0,   102,
     102,   102,   102,     0,   580,     0,   296,     0,     0,     0,
       0,     0,     0,   111,     0,   102,   102,     0,     0,     0,
     110,   108,     0,     0,     0,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
     106,     0,   106,     0,     0,   106,     0,     0,   106,   284,
       0,     0,     0,   106,     0,     0,     0,     0,     0,   111,
       0,     0,   105,   105,   105,   105,   234,     0,     0,     0,
       0,     0,     0,   106,     0,     0,     0,     0,   105,   105,
     187,   187,   187,   316,   106,   108,   106,   108,     0,     0,
     108,   106,     0,   108,   187,     0,     0,     0,   108,     0,
       0,     0,     0,   235,     0,     0,     0,   236,     0,     0,
       0,     0,   187,     0,     0,     0,     0,     0,   108,     0,
       0,     0,   187,     0,     0,     0,     0,     0,     0,   108,
       0,   108,     0,     0,   111,     0,   108,   110,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
       0,   106,     0,   246,     0,     0,   106,     0,   111,   252,
     253,   111,     0,     0,   256,     0,   111,   257,   258,   259,
       0,     0,     0,     0,     0,     0,   106,     0,     0,     0,
       0,     0,     0,   110,     0,   106,     0,   110,     0,   106,
       0,     0,     0,     0,     0,     0,   108,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   187,   111,     0,     0,     0,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
     108,     0,     0,     0,   108,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,     0,   110,
       0,     0,   110,     0,     0,   110,   106,   106,   106,   106,
     110,     0,     0,     0,     0,   775,     0,     0,     0,     0,
       0,     0,   106,   106,     0,     0,     0,     0,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,     0,   110,     0,     0,     0,     0,   110,     0,
       0,   108,   108,   108,   108,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,   108,     0,
       0,     0,     0,     0,   813,     0,     0,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   840,   110,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   111,     0,     0,
       0,   111,     0,   110,     0,     0,     0,     0,     0,     0,
       0,   840,   110,   187,   187,     0,   110,   187,   187,     0,
       0,   840,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,   125,     0,     0,     0,     0,   125,     0,
     119,     0,   148,   125,   125,     0,     0,     0,     0,   146,
     162,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,   111,   168,   111,     0,     0,   111,     0,     0,   111,
       0,     0,   195,     0,   111,     0,     0,     0,     0,     0,
     193,     0,     0,   110,   110,   110,   110,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,     0,    74,   110,
     110,     0,   220,     0,     0,   111,     0,   111,     0,     0,
     222,     0,   111,     0,     0,     0,     0,     0,     0,   187,
     187,     0,     0,     0,     0,     0,     0,     0,     0,   232,
     187,     0,   316,     0,   233,     0,     0,   316,     0,     0,
     187,     0,   234,   187,     0,     0,     0,     0,     0,     0,
       0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   195,     0,     0,     0,   295,     0,     0,
       0,     0,   111,   195,     0,   304,     0,   111,     0,   235,
       0,   295,     0,   236,   323,     0,     0,     0,     0,     0,
       0,   318,     0,     0,   295,     0,   125,   111,     0,   335,
       0,     0,     0,   237,     0,     0,   111,     0,     0,     0,
     111,     0,   295,     0,     0,   340,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,    74,   254,   255,
     256,   369,     0,   257,   258,   259,   165,   165,   165,   165,
       0,     0,     0,     0,   694,     0,     0,     0,     0,     0,
       0,     0,     5,   394,     0,     0,     0,     0,     0,   644,
       0,   187,     0,     0,   645,     0,     0,   111,   111,   111,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,   111,     0,   295,   295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   316,   316,     0,
     316,   316,    74,     0,     0,     0,     0,     0,     0,     0,
       0,   461,     0,   465,     0,   817,     0,     0,     0,     0,
       0,     0,   316,     0,   316,     0,   340,     0,     0,   340,
       0,     0,   323,     0,   646,   647,   648,   649,   650,   651,
       0,   165,   652,   653,   654,   655,   656,   657,   658,   659,
     660,   661,   295,     0,   662,     0,     0,   817,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,     0,
     674,     0,     0,   675,   676,   677,   678,     0,   679,     0,
       0,     0,   524,   165,     0,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -250,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -143,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -250,    38,    39,
      40,    41,  -143,    42,    43,     0,  -250,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -250,     0,     0,    55,     0,    56,     0,     0,    57,
     295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,   609,     0,     0,     0,   323,   605,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,     0,    69,     0,    70,   195,    71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,   638,     0,   680,     0,    74,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,   461,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,   340,
       0,   340,     0,     0,   340,    57,     0,   340,   323,     0,
       0,     0,   715,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,   727,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,   733,     0,   737,     0,    68,     0,     0,
      74,   132,     0,    70,     0,   491,     0,     0,     4,   749,
       5,     6,     7,     8,     9,    10,     0,  -402,     0,    11,
      12,    13,  -402,    14,    15,    16,    17,  -250,    18,    19,
    -402,    20,    21,  -402,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -143,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -250,
      38,     0,    40,    41,  -143,    42,    43,  -402,  -250,    44,
      45,  -402,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -250,     0,   786,    55,     0,    56,     0,
       0,  -402,   125,     0,   790,     0,     0,     0,   792,     0,
     795,     0,     0,     0,     0,     0,  -402,    59,  -402,  -402,
    -402,  -402,  -402,  -402,  -402,     0,  -402,  -402,  -402,  -402,
    -402,  -402,  -402,  -402,  -402,    64,  -402,  -402,  -402,     0,
      67,  -402,  -402,  -402,     0,     0,     0,    69,  -402,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -250,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -143,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -250,    38,    39,    40,    41,  -143,    42,    43,     0,
    -250,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -250,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,   179,    67,     0,     0,    68,     0,     0,     0,    69,
     206,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -250,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -143,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -250,    38,    39,    40,    41,  -143,    42,
      43,     0,  -250,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -250,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
       0,    69,   352,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -250,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,     0,    28,    29,
       0,  -143,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -250,    38,    39,    40,    41,
    -143,    42,    43,     0,  -250,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -250,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,     0,    69,   206,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -250,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -143,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -250,    38,    39,
      40,    41,  -143,    42,    43,     0,  -250,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -250,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,     0,    69,   640,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -250,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -143,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -250,
      38,    39,    40,    41,  -143,    42,    43,     0,  -250,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -250,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,     0,    69,   741,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -250,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,     0,    28,    29,     0,  -143,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -250,    38,   219,    40,    41,  -143,    42,    43,     0,
    -250,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -250,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,     0,    69,
       0,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -250,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,     0,    28,    29,     0,  -143,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -250,    38,     0,    40,    41,  -143,    42,
      43,     0,  -250,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -250,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -250,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   468,     0,    28,    29,
       0,  -143,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -250,    38,     0,    40,    41,
    -143,    42,    43,     0,  -250,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -250,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,     0,    69,     0,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -250,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,   472,     0,
      28,    29,     0,  -143,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -250,    38,     0,
      40,    41,  -143,    42,    43,     0,  -250,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -250,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,     0,    69,     0,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -250,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
     702,     0,    28,    29,     0,  -143,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -250,
      38,     0,    40,    41,  -143,    42,    43,     0,  -250,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -250,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -250,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   704,     0,    28,    29,     0,  -143,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -250,    38,     0,    40,    41,  -143,    42,    43,     0,
    -250,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -250,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,     0,    69,
       0,    70,     4,    71,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -250,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,   707,     0,    28,    29,     0,  -143,
      30,    31,    32,    33,    34,   -37,    35,    36,     0,    37,
       0,     0,     0,  -250,    38,     0,    40,    41,  -143,    42,
      43,     0,  -250,    44,    45,     0,    46,    47,    48,    49,
      50,    51,     0,    52,     0,    53,    54,  -250,     0,     0,
      55,     0,    56,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,    67,     0,     0,    68,     0,     0,
       0,    69,     0,    70,     4,    71,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -250,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,   710,     0,    28,    29,
       0,  -143,    30,    31,    32,    33,    34,   -37,    35,    36,
       0,    37,     0,     0,     0,  -250,    38,     0,    40,    41,
    -143,    42,    43,     0,  -250,    44,    45,     0,    46,    47,
      48,    49,    50,    51,     0,    52,     0,    53,    54,  -250,
       0,     0,    55,     0,    56,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,    67,     0,     0,    68,
       0,     0,     0,    69,     0,    70,     4,    71,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -250,    18,    19,     0,   726,
      21,     0,    22,     0,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -143,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -250,    38,     0,
      40,    41,  -143,    42,    43,     0,  -250,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -250,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,     0,    69,     0,    70,     4,    71,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -250,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
     732,     0,    28,    29,     0,  -143,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,     0,     0,     0,  -250,
      38,     0,    40,    41,  -143,    42,    43,     0,  -250,    44,
      45,     0,    46,    47,    48,    49,    50,    51,     0,    52,
       0,    53,    54,  -250,     0,     0,    55,     0,    56,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
      67,     0,     0,    68,     0,     0,     0,    69,     0,    70,
       4,    71,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -250,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,   736,     0,    28,    29,     0,  -143,    30,    31,
      32,    33,    34,   -37,    35,    36,     0,    37,     0,     0,
       0,  -250,    38,     0,    40,    41,  -143,    42,    43,     0,
    -250,    44,    45,     0,    46,    47,    48,    49,    50,    51,
       0,    52,     0,    53,    54,  -250,     0,     0,    55,     0,
      56,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,    67,     0,     0,    68,     0,     0,     0,    69,
       0,    70,   490,    71,     5,     6,     7,     8,     9,    10,
       0,  -402,     0,   127,     0,     0,  -402,     0,     0,     0,
       0,     0,     0,     0,  -402,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,  -402,     0,     0,     0,  -402,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -268,  -402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -402,    59,  -402,  -402,  -402,     0,  -402,  -402,  -402,     0,
    -402,  -402,  -402,  -402,  -402,  -402,  -402,  -402,  -402,    64,
    -402,  -402,  -402,     0,     0,  -402,  -402,  -402,     0,     0,
       0,   132,     0,    70,   490,   505,     5,     6,     7,     8,
       9,    10,     0,  -402,     0,   127,     0,     0,  -402,     0,
       0,     0,     0,     0,     0,     0,  -402,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,  -402,     0,     0,     0,  -402,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -402,    59,  -402,  -402,  -402,     0,  -402,  -402,
    -402,     0,  -402,  -402,  -402,  -402,  -402,  -402,  -402,  -402,
    -402,    64,  -402,  -402,  -402,     0,     0,  -402,  -402,  -402,
       0,     0,     0,   132,     0,    70,   490,   505,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,     0,   132,     0,    70,     0,   505,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,   179,
       0,     0,     0,    68,     0,     0,     0,   132,     0,    70,
       0,   133,   523,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,   179,     0,     0,     0,    68,     0,     0,     0,
     132,     0,    70,     0,   133,   811,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,     0,   132,     0,    70,     0,   133,   528,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
     434,    28,     0,   435,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,   436,   437,    38,
       0,    40,     0,     0,     0,     0,     0,   438,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,   439,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,     0,
       0,     0,    68,     0,     0,     0,   132,     0,    70,     0,
     133,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,   210,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,   211,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,     0,   132,     0,
      70,     0,   133,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,   210,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,   179,     0,     0,     0,    68,     0,     0,     0,
     132,     0,    70,   517,   133,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,   210,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,     0,   132,     0,    70,   519,   133,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,   155,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   156,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,     0,   132,     0,    70,     0,   133,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,   179,     0,
       0,     0,    68,     0,     0,     0,   132,   515,    70,     0,
     133,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,     0,   132,     0,
      70,     0,   133,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,   183,     0,     0,    68,     0,     0,     0,
     132,     0,    70,     0,   133,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   186,    21,     0,     0,
       0,     0,     0,   128,   129,   130,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,     0,    69,     0,    70,     0,   133,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   156,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,     0,   132,     0,    70,     0,   133,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,   621,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,     0,
       0,     0,    68,     0,     0,     0,   132,     0,    70,     0,
     133,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   771,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   772,    49,    50,   773,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
     179,     0,     0,     0,    68,     0,     0,     0,   132,     0,
      70,     0,   774,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   130,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,   179,     0,     0,     0,    68,     0,     0,     0,
     132,     0,    70,     0,   774,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   128,   129,   761,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,   179,     0,     0,     0,    68,     0,
       0,     0,   132,     0,    70,     0,   133,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,     0,   132,     0,    70,     0,   133,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,     0,
       0,     0,   509,     0,     0,     0,   132,     0,    70,     0,
     133,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   568,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,     0,   132,     0,
      70,     0,   133,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   571,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,     0,
     132,     0,    70,     0,   133,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   128,   129,   755,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,     0,   132,     0,    70,     0,   133,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   756,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,     0,     0,     0,
      68,     0,     0,     0,   132,     0,    70,     0,   133,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   758,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,     0,
       0,     0,    68,     0,     0,     0,   132,     0,    70,     0,
     133,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   759,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,     0,     0,     0,    68,     0,     0,     0,   132,     0,
      70,     0,   133,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   128,   129,   760,     0,    28,     0,     0,     0,     0,
      31,    32,     0,    34,     0,    35,    36,     0,     0,     0,
       0,     0,     0,    38,     0,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    49,    50,
     131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,     0,     0,     0,    68,     0,     0,     0,
     132,     0,    70,     0,   133,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   128,   129,   761,     0,    28,     0,     0,
       0,     0,    31,    32,     0,    34,     0,    35,    36,     0,
       0,     0,     0,     0,     0,    38,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,    50,   131,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,     0,     0,     0,    68,     0,
       0,     0,   132,     0,    70,     0,   133,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   127,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   128,   129,   130,     0,    28,
       0,     0,     0,     0,    31,    32,     0,    34,     0,    35,
      36,     0,     0,     0,     0,     0,     0,    38,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   127,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
      63,    28,    64,     0,     0,    66,    31,    32,     0,    34,
      68,    35,    36,     0,   132,   232,    70,     0,   133,    38,
     233,    40,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -277,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,   233,   235,    59,     0,     0,   236,
    -277,     0,   234,     0,  -277,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,   312,     0,     0,   179,   237,
       0,     0,     0,     0,     0,     0,   132,     0,    70,  -277,
     774,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,     0,   257,
     258,   259,   232,   237,     0,     0,     0,   233,     0,     0,
     636,     0,     0,     0,     0,   234,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,     0,   257,   258,   259,   232,     0,     0,     0,
       0,   233,   235,     0,   820,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,     0,   257,   258,   259,     0,
     237,     0,     0,     0,     0,     0,     0,   855,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   232,   254,   255,   256,     0,   233,
     257,   258,   259,     0,     0,     0,     0,   234,   310,   516,
       0,     0,     0,     0,     0,     0,     0,     0,   311,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   307,     0,   235,     0,     0,     0,   236,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   312,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   235,     0,
       0,     0,   236,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   237,   254,   255,   256,     0,     0,   257,   258,
     259,     0,   235,     0,   283,     0,   236,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   232,   254,   255,   256,
       0,   233,   257,   258,   259,     0,     0,     0,   283,   234,
     598,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,     0,   250,   251,   252,   253,
       0,   254,     0,   256,     0,     0,   257,   258,   259,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,   312,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,   315,   283,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,   283,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,   592,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,   283,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,     0,   283,   232,     0,     0,   701,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,   706,   283,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,     0,   283,   232,     0,     0,
     806,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,   807,   283,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,   283,   232,     0,
       0,   808,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   809,
     283,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,   283,   232,
       0,     0,   822,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
     823,   283,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,   283,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,     0,   339,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,  -362,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   121,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   232,   254,   255,   256,     0,   233,   257,   258,   259,
       0,     0,     0,     0,   234,     0,     0,     0,     0,   348,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     349,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,   235,     0,     0,     0,   236,     0,     0,   234,   445,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,   235,   242,   243,   244,   236,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,     0,   257,   258,   259,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   232,   254,   255,   256,     0,   233,   257,
     258,   259,     0,     0,     0,     0,   234,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   448,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   235,     0,     0,     0,   236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,   237,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   235,     0,     0,
       0,   236,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   237,   254,   255,   256,     0,     0,   257,   258,   259,
       0,   235,     0,     0,     0,   236,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   237,   254,   255,   256,     0,
     293,   257,   258,   259,     0,     0,   232,     0,     0,     0,
     238,   233,   239,   240,   241,     0,   242,   243,   244,   234,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   338,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   351,
     257,   258,   259,   365,   237,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,     0,   257,   258,   259,     0,     0,   232,
       0,     0,     0,     0,   233,   235,     0,     0,     0,   236,
       0,     0,   234,     0,     0,     0,   449,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,   232,     0,   257,
     258,   259,   233,   237,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   543,   257,   258,   259,   233,   235,     0,     0,
       0,   236,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,   232,
     544,   257,   258,   259,   233,   237,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,   545,   257,   258,   259,   233,   235,
       0,     0,     0,   236,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,   235,   242,   243,   244,   236,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   546,   257,   258,   259,   233,   237,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,   232,   547,   257,   258,   259,
     233,   235,     0,     0,     0,   236,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,   235,   242,   243,   244,   236,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,   548,   257,   258,   259,   233,   237,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,   232,   549,   257,
     258,   259,   233,   235,     0,     0,     0,   236,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,   235,   242,   243,
     244,   236,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,   232,   550,   257,   258,   259,
     233,   237,     0,     0,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,   232,
     551,   257,   258,   259,   233,   235,     0,     0,     0,   236,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,   235,
     242,   243,   244,   236,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,   232,   552,   257,
     258,   259,   233,   237,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   553,   257,   258,   259,   233,   235,     0,     0,
       0,   236,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,   232,
     554,   257,   258,   259,   233,   237,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,   232,   555,   257,   258,   259,   233,   235,
       0,     0,     0,   236,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,   235,   242,   243,   244,   236,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,   232,   556,   257,   258,   259,   233,   237,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   557,   257,   258,   259,
       0,   235,     0,     0,     0,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,   558,   257,   258,   259,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,     0,   121,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,   237,     0,     0,     0,
       0,     0,   234,     0,     0,   604,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   578,   257,   258,   259,   235,
       0,     0,     0,   236,     0,     0,     0,   232,   632,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,   689,   639,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,   692,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
     754,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,     0,
     232,   788,     0,   757,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,     0,
       0,   232,     0,     0,   828,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,     0,   257,   258,   259,
       0,     0,   829,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
       0,     0,   232,     0,     0,   830,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,   831,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,     0,     0,   232,     0,     0,   832,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,   833,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   566,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   234,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,     0,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,     0,   250,   251,   252,   253,     0,   254,
       0,   256,     0,     0,   257,   258,   259
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-761))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,    41,    42,   344,   322,    16,     2,    15,    19,   280,
     143,   327,   216,   489,    25,    26,    27,     3,   319,    23,
     153,   618,     3,    34,    35,     1,    37,    38,   344,   226,
     227,   228,   229,    44,    45,    46,    47,    48,   476,     3,
      51,   642,     3,    54,    55,    56,    44,    24,    59,    50,
      80,    62,    63,    64,   642,    66,    54,     1,    69,    70,
      71,    85,    34,     1,     1,   782,    63,    24,   682,    24,
     104,    24,    20,    71,     3,    61,    77,   811,    13,   104,
     104,   104,     0,    18,   108,   845,    16,    63,    34,   132,
      34,   134,    73,   161,    34,   855,    25,    34,    80,    72,
     123,   135,    78,    80,    52,   135,   174,   824,   133,   133,
      34,   845,    25,    61,    34,    34,   127,   128,   129,   130,
     131,   132,   133,   720,   192,    63,   564,   103,    76,    59,
     131,   139,   104,   130,    80,   133,   147,     3,   614,    83,
      78,    85,   132,    72,    74,   746,   132,   123,    85,   104,
     161,   132,   132,   130,   130,   157,   158,   159,   104,   103,
     104,   762,    20,   174,   104,   103,   103,   104,   132,   171,
     488,   132,   176,   130,   762,   130,   373,   130,   104,   123,
     104,   192,   796,   797,   104,   104,   123,   189,    69,   135,
     104,    34,   130,     7,    52,   135,   132,   199,   134,   103,
     211,   132,   106,    61,    44,   476,   110,   133,   276,   277,
     132,   135,   208,   810,    72,   135,   135,    63,    61,   133,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   123,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     474,   475,   748,   123,   342,   713,     3,   123,   289,    85,
       7,   104,   104,   294,   130,   104,   104,   104,   290,    38,
     608,    23,   104,   564,   305,   104,   307,   308,   104,   310,
     311,   617,   108,    85,   315,   606,   104,   305,    57,   131,
     133,    38,   314,   123,   133,   133,   133,   521,   522,   330,
     104,   133,   235,   236,   133,   336,   108,   133,    60,   129,
      57,   342,    64,   131,   104,   123,   123,   348,   349,   123,
      52,   132,   123,   354,   104,   104,   357,   123,   359,    61,
     361,   362,   363,   123,   365,   366,     3,   123,   104,   370,
      72,     2,   104,   123,   123,   278,   279,   104,   130,   123,
      11,   103,   370,    20,   106,   508,   289,   123,   110,    20,
     123,   123,    34,    19,   725,    79,   123,    34,   729,   132,
      37,   134,    33,    29,    30,   308,   130,     3,   311,   132,
      36,     7,    38,   719,    51,    52,   132,   106,    44,   725,
      51,    63,    60,   729,    61,    60,    64,   132,   132,    64,
     134,    57,    83,    85,    85,    72,    78,   123,    69,   357,
       7,   359,    73,    85,   445,   446,   447,   448,   449,   450,
     451,   123,   713,   103,   452,   453,   457,   458,   361,   460,
     751,   104,   104,   133,   123,   466,    34,   468,   132,    26,
      23,   472,   133,   133,   104,   476,    83,    85,   470,   471,
     133,   123,   474,   475,    20,   133,    16,   104,   130,    19,
     491,   122,   133,   494,   495,   132,   135,    27,    34,    29,
      30,    37,    19,   491,   505,   811,    36,    60,    38,   130,
     130,    64,    29,    30,   820,    51,    52,   505,    26,    36,
     135,    38,    85,   133,    54,    61,    56,    57,    58,    59,
     133,   162,   123,   104,   123,   123,    72,    24,    23,   845,
      57,    24,    72,   446,    74,   448,    99,   130,   104,   855,
     103,   135,   105,   106,   103,   186,    61,   110,   135,   135,
     106,   133,   132,   564,   117,   566,   567,   568,    78,   530,
     571,   124,   125,   126,    23,    60,   349,   208,     2,    64,
     154,   465,   583,   699,   576,   577,   750,    11,   584,   590,
     750,   592,   751,   123,   595,   587,    20,   598,   606,   643,
     824,   131,     3,   604,   810,   597,     7,   770,   600,    33,
     794,    60,   331,   460,    99,    64,   101,   102,   103,    20,
     105,   106,    -1,   624,    -1,   110,    -1,    51,    -1,   630,
      -1,   632,   117,   634,    -1,   636,   624,    -1,   639,   124,
     125,   126,    -1,    -1,    -1,    69,    -1,    -1,    19,    73,
      -1,    52,   283,    -1,   103,   641,   105,   106,    29,    30,
      61,   110,    -1,   566,   567,    36,    -1,    38,   117,    -1,
      -1,    72,    -1,    44,    -1,    76,   307,   126,    -1,   310,
      -1,    -1,    -1,    -1,   315,    -1,    57,   688,   689,    -1,
     691,   692,   693,   694,    -1,    -1,    23,    -1,   122,    -1,
     701,   702,    -1,   704,    -1,   706,   707,    16,    -1,   710,
      19,    -1,   713,   743,   744,    -1,    -1,    -1,    27,    -1,
      29,    30,    -1,   724,   725,    -1,    -1,    36,   729,    38,
      -1,   732,   363,    60,    -1,   736,   728,    64,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    57,    58,
      59,    -1,    -1,   754,   755,   756,   757,   758,   759,   760,
     761,    -1,   186,    72,    -1,    74,    34,    -1,    -1,    -1,
      -1,   772,   773,   774,    -1,    -1,   103,   778,   105,   106,
     781,    -1,    -1,   110,   208,     2,   774,   788,    -1,    -1,
     117,    34,    -1,    -1,    11,    63,    -1,   124,   125,   126,
      -1,    -1,    -1,    20,    -1,   806,   807,   808,   809,   440,
      78,    -1,    -1,    -1,   123,    -1,    33,    85,    -1,   820,
      63,   822,   823,    -1,    -1,    -1,    -1,   828,   829,   830,
     831,   832,   833,    -1,    51,    78,   104,    -1,    -1,    -1,
      -1,    -1,    85,   844,   845,    -1,    -1,    -1,     2,   480,
      -1,    -1,    69,    -1,   855,   123,    73,    11,    -1,   283,
      -1,   104,   130,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
     123,    -1,    -1,   307,    -1,    -1,   310,   130,    -1,    -1,
      -1,   315,    -1,    -1,    -1,   526,    -1,    51,    -1,   530,
      16,    -1,    -1,    19,     3,   122,    -1,    -1,     7,    -1,
       9,    27,    -1,    29,    30,    69,    -1,    -1,    -1,    73,
      36,    20,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   566,   567,    -1,    54,   363,
      56,    57,    58,    59,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    72,    -1,    74,   590,
      -1,   592,    61,    -1,   595,    -1,    -1,   598,   122,   186,
      -1,    -1,   603,    72,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,   623,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,     8,   634,    -1,   636,    12,   123,   162,    -1,
     641,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,   186,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,     2,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    11,    -1,    -1,   208,    -1,   480,    -1,    -1,    -1,
      20,    67,    68,    69,    70,    -1,   283,    -1,    -1,    -1,
     701,    -1,    -1,    33,    -1,   706,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
     307,    51,    -1,   310,   100,   726,    11,    -1,   315,    -1,
      -1,    -1,   526,    -1,   735,    20,   530,    -1,   739,    69,
      -1,    -1,   118,    73,    -1,    -1,    -1,    -1,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,   134,   283,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   363,    -1,    -1,    -1,
      -1,    -1,    23,   307,    69,    -1,   310,    -1,    73,    -1,
      -1,   315,   122,    -1,    -1,    -1,   590,    -1,   592,    -1,
      -1,   595,    -1,    -1,   598,   806,   807,   808,   809,   603,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,   822,   823,    64,    -1,    -1,    -1,    -1,    -1,   623,
      -1,    -1,   162,    -1,    -1,    -1,    70,   122,    -1,   363,
     634,    -1,   636,    -1,    -1,    -1,    -1,   641,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   162,   208,     2,
     121,    23,    -1,   124,   125,   126,    -1,    -1,    11,    -1,
      -1,    -1,    -1,   480,    -1,    -1,    -1,    20,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,   701,    -1,    -1,
      33,    -1,   706,   147,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,   208,    -1,    -1,    -1,   161,    51,    -1,
      -1,    -1,   726,    -1,    -1,    -1,    -1,    -1,    -1,   526,
     174,   735,    -1,   530,    -1,   739,    69,    -1,    -1,    -1,
      73,    -1,    -1,   283,    -1,    -1,   480,    99,   192,   101,
     102,   103,    -1,   105,   106,   107,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,   307,    -1,   121,
     310,    -1,   124,   125,   126,   315,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,   122,
      -1,    -1,   526,   590,    -1,   592,   530,    -1,   595,    -1,
      -1,   598,   806,   807,   808,   809,   603,    -1,    -1,    -1,
      -1,    -1,   307,    -1,    -1,   310,    -1,    -1,   822,   823,
     315,    -1,    -1,   363,    -1,    -1,   623,    -1,    -1,   162,
      -1,    -1,   276,   277,    -1,    -1,    -1,   634,    -1,   636,
      -1,    -1,    -1,    -1,   641,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,   590,    -1,   592,    -1,
      -1,   595,    -1,     2,   598,    -1,    -1,    -1,   363,   603,
      -1,    -1,    11,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    20,    60,    -1,    -1,    -1,    64,    -1,    -1,   623,
      -1,    -1,   336,    -1,    33,    -1,    -1,    -1,   342,    -1,
     634,    -1,   636,    -1,   701,    -1,    -1,   641,    -1,   706,
     354,    -1,    51,   357,    -1,   359,    -1,    -1,   362,    -1,
      -1,    99,    -1,    -1,   102,   103,    -1,   105,   106,   726,
      69,    -1,   110,    -1,    73,    -1,    -1,    -1,   735,   117,
     480,    -1,   739,    -1,    -1,    -1,   124,   125,   126,    -1,
     283,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,   701,    -1,    -1,
      -1,    -1,   706,    -1,   307,    -1,    -1,   310,    -1,    -1,
      47,    -1,   315,   122,    -1,   480,   526,    -1,    -1,    -1,
     530,    -1,   726,   127,   128,   129,    -1,    -1,   132,    -1,
     134,   735,    -1,    -1,    -1,   739,    -1,    -1,    -1,   806,
     807,   808,   809,    -1,   458,    -1,   460,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,   822,   823,    -1,    -1,    -1,
     363,   526,    -1,    -1,    -1,   530,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
     590,    -1,   592,    -1,    -1,   595,    -1,    -1,   598,   126,
      -1,    -1,    -1,   603,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,   806,   807,   808,   809,    23,    -1,    -1,    -1,
      -1,    -1,    -1,   623,    -1,    -1,    -1,    -1,   822,   823,
     157,   158,   159,   160,   634,   590,   636,   592,    -1,    -1,
     595,   641,    -1,   598,   171,    -1,    -1,    -1,   603,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,   623,    -1,
      -1,    -1,   199,    -1,    -1,    -1,    -1,    -1,    -1,   634,
      -1,   636,    -1,    -1,   283,    -1,   641,   480,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
      -1,   701,    -1,   110,    -1,    -1,   706,    -1,   307,   116,
     117,   310,    -1,    -1,   121,    -1,   315,   124,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,   726,    -1,    -1,    -1,
      -1,    -1,    -1,   526,    -1,   735,    -1,   530,    -1,   739,
      -1,    -1,    -1,    -1,    -1,    -1,   701,    -1,    -1,    -1,
      -1,   706,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,   363,    -1,    -1,    -1,    -1,    -1,
      -1,   726,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     735,    -1,    -1,    -1,   739,    -1,    -1,   314,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   590,    -1,   592,
      -1,    -1,   595,    -1,    -1,   598,   806,   807,   808,   809,
     603,    -1,    -1,    -1,    -1,   719,    -1,    -1,    -1,    -1,
      -1,    -1,   822,   823,    -1,    -1,    -1,    -1,    -1,    -1,
     623,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   634,    -1,   636,    -1,    -1,    -1,    -1,   641,    -1,
      -1,   806,   807,   808,   809,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   822,   823,    -1,
      -1,    -1,    -1,    -1,   778,    -1,    -1,    -1,    -1,    -1,
      -1,   480,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   811,   701,    -1,
      -1,    -1,    -1,   706,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   526,    -1,    -1,
      -1,   530,    -1,   726,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   845,   735,   470,   471,    -1,   739,   474,   475,    -1,
      -1,   855,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     2,    -1,    13,    -1,    -1,    -1,    -1,    18,    -1,
      11,    -1,    22,    23,    24,    -1,    -1,    -1,    -1,    20,
      30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   590,    33,   592,    -1,    -1,   595,    -1,    -1,   598,
      -1,    -1,    52,    -1,   603,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,   806,   807,   808,   809,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   623,    -1,    -1,    -1,    69,   822,
     823,    -1,    73,    -1,    -1,   634,    -1,   636,    -1,    -1,
      90,    -1,   641,    -1,    -1,    -1,    -1,    -1,    -1,   576,
     577,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
     587,    -1,   589,    -1,    15,    -1,    -1,   594,    -1,    -1,
     597,    -1,    23,   600,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,    -1,    -1,   147,    -1,    -1,
      -1,    -1,   701,   153,    -1,   155,    -1,   706,    -1,    60,
      -1,   161,    -1,    64,   164,    -1,    -1,    -1,    -1,    -1,
      -1,   162,    -1,    -1,   174,    -1,   176,   726,    -1,   179,
      -1,    -1,    -1,    84,    -1,    -1,   735,    -1,    -1,    -1,
     739,    -1,   192,    -1,    -1,   186,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   208,   119,   120,
     121,   221,    -1,   124,   125,   126,   226,   227,   228,   229,
      -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,   243,    -1,    -1,    -1,    -1,    -1,    10,
      -1,   728,    -1,    -1,    15,    -1,    -1,   806,   807,   808,
     809,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   822,   823,    -1,   276,   277,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   764,   765,    -1,
     767,   768,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   301,    -1,   303,    -1,   782,    -1,    -1,    -1,    -1,
      -1,    -1,   789,    -1,   791,    -1,   307,    -1,    -1,   310,
      -1,    -1,   322,    -1,    85,    86,    87,    88,    89,    90,
      -1,   331,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   342,    -1,   105,    -1,    -1,   824,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,    -1,    -1,   124,   125,   126,   127,    -1,   129,    -1,
      -1,    -1,   363,   373,    -1,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
     460,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,   484,    -1,    -1,    -1,   488,   480,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,   132,   508,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,   526,    -1,   537,    -1,   530,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   584,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   590,
      -1,   592,    -1,    -1,   595,    84,    -1,   598,   608,    -1,
      -1,    -1,   603,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,   623,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,   634,    -1,   636,    -1,   126,    -1,    -1,
     641,   130,    -1,   132,    -1,   134,    -1,    -1,     1,   659,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,   726,    79,    -1,    81,    -1,
      -1,    84,   742,    -1,   735,    -1,    -1,    -1,   739,    -1,
     750,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    -1,   130,   131,   132,
       1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,   122,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
     131,   132,     1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,   130,   131,   132,     1,   134,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,   130,   131,   132,     1,   134,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,   131,   132,     1,   134,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,
       1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,     1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,   132,     1,   134,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,   130,    -1,   132,     1,   134,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,   132,     1,   134,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,
       1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,     1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    -1,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,
      79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,   132,     1,   134,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    -1,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,
      57,    58,    59,    -1,    61,    62,    63,    -1,    65,    66,
      67,    68,    69,    70,    -1,    72,    -1,    74,    75,    76,
      -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,   130,    -1,   132,     1,   134,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,   132,     1,   134,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,
       1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    -1,    -1,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,
      81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,     1,   134,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,   124,   125,   126,    -1,    -1,
      -1,   130,    -1,   132,     1,   134,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    12,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
      -1,    -1,    -1,   130,    -1,   132,     1,   134,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,   122,
      -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,
      -1,   134,   135,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,    -1,   134,   135,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,   130,    -1,   132,    -1,   134,   135,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    -1,    37,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    51,    52,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,    -1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,   133,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,   133,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,    -1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,    -1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,    -1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,    -1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,    -1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,    -1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,    -1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,
      40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,    -1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,
      -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
     116,    35,   118,    -1,    -1,   121,    40,    41,    -1,    43,
     126,    45,    46,    -1,   130,    10,   132,    -1,   134,    53,
      15,    55,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    60,   100,    -1,    -1,    64,
     104,    -1,    23,    -1,   108,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    80,    -1,    -1,   122,    84,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   132,   133,
     134,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,
     105,   106,   107,    64,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    -1,   124,
     125,   126,    10,    84,    -1,    -1,    -1,    15,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    23,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    -1,   124,   125,   126,    10,    -1,    -1,    -1,
      -1,    15,    60,    -1,   135,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    -1,   124,   125,   126,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    10,   119,   120,   121,    -1,    15,
     124,   125,   126,    -1,    -1,    -1,    -1,    23,    24,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    60,    -1,
      -1,    -1,    64,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    84,   119,   120,   121,    -1,    -1,   124,   125,
     126,    -1,    60,    -1,   130,    -1,    64,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    10,   119,   120,   121,
      -1,    15,   124,   125,   126,    -1,    -1,    -1,   130,    23,
      24,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      -1,   119,    -1,   121,    -1,    -1,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    71,   130,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,   130,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,    -1,   130,    10,    -1,    -1,    71,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,    -1,    71,   130,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,    -1,   130,    10,    -1,    -1,
      71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    71,   130,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,    -1,   130,    10,    -1,
      -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    71,
     130,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,    -1,   130,    10,
      -1,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      71,   130,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,   130,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,    -1,   130,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    10,   119,   120,   121,    -1,    15,   124,   125,   126,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    60,   105,   106,   107,    64,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    -1,   124,   125,   126,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    10,   119,   120,   121,    -1,    15,   124,
     125,   126,    -1,    -1,    -1,    -1,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    60,    -1,    -1,
      -1,    64,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    84,   119,   120,   121,    -1,    -1,   124,   125,   126,
      -1,    60,    -1,    -1,    -1,    64,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    84,   119,   120,   121,    -1,
     123,   124,   125,   126,    -1,    -1,    10,    -1,    -1,    -1,
      99,    15,   101,   102,   103,    -1,   105,   106,   107,    23,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,   126,    83,    84,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    -1,   124,   125,   126,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    60,    -1,    -1,    -1,    64,
      -1,    -1,    23,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,
     105,   106,   107,    64,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    10,    -1,   124,
     125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    60,    -1,    -1,
      -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    60,   105,   106,   107,    64,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    10,
     123,   124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,   123,   124,   125,   126,    15,    60,
      -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    60,   105,   106,   107,    64,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    10,   123,   124,   125,   126,
      15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    60,   105,   106,   107,    64,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,   123,   124,   125,   126,    15,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    10,   123,   124,
     125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    60,   105,   106,
     107,    64,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    10,   123,   124,   125,   126,
      15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    10,
     123,   124,   125,   126,    15,    60,    -1,    -1,    -1,    64,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,
     105,   106,   107,    64,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    10,   123,   124,
     125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    60,    -1,    -1,
      -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    60,   105,   106,   107,    64,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    10,
     123,   124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    10,   123,   124,   125,   126,    15,    60,
      -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    60,   105,   106,   107,    64,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    10,   123,   124,   125,   126,    15,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,   123,   124,   125,   126,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,   123,   124,   125,   126,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    80,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,   123,   124,   125,   126,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    10,    11,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    83,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      71,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,    -1,
      10,    11,    -1,    71,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,    -1,
      -1,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    60,   119,   120,   121,    64,    -1,   124,   125,   126,
      -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    60,   124,   125,   126,    64,    -1,
      -1,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,    -1,    -1,    10,    -1,    -1,    71,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,    71,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,    -1,   119,
      -1,   121,    -1,    -1,   124,   125,   126
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   140,   141,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    35,    36,
      39,    40,    41,    42,    43,    45,    46,    48,    53,    54,
      55,    56,    58,    59,    62,    63,    65,    66,    67,    68,
      69,    70,    72,    74,    75,    79,    81,    84,    99,   100,
     101,   102,   107,   116,   118,   120,   121,   123,   126,   130,
     132,   134,   142,   143,   144,   145,   146,   147,   152,   153,
     154,   156,   159,   160,   161,   162,   163,   165,   166,   169,
     170,   173,   176,   179,   180,   201,   204,   205,   223,   224,
     225,   226,   227,   228,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   246,   247,   248,   249,   250,   144,
     238,    80,   229,   230,   155,   156,   229,    12,    31,    32,
      33,    70,   130,   134,   176,   223,   227,   236,   237,   238,
     239,   241,   242,    72,   155,   238,   144,   132,   156,     7,
     155,   157,     9,    72,   157,    52,    82,   164,   238,   238,
     238,   132,   156,   177,   132,   156,   206,   207,   144,   238,
     238,   238,   238,     7,   132,    19,    30,   161,   161,   122,
     197,   217,   238,   123,   238,   238,    24,   147,   158,   238,
     238,    69,   132,   144,   238,   156,   202,   217,   238,   238,
     238,   238,   238,   238,   238,   238,   131,   142,   148,   217,
      73,   108,   197,   218,   219,   238,   217,   238,   245,    54,
     144,    44,   156,    38,    57,   193,    20,    52,    61,    76,
     123,   129,    10,    15,    23,    60,    64,    84,    99,   101,
     102,   103,   105,   106,   107,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   119,   120,   121,   124,   125,   126,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   127,   128,   132,   134,    60,    64,
     132,   144,   123,   130,   147,   238,   238,   238,   217,    34,
     229,   202,   123,   123,    79,   156,   197,   220,   221,   222,
     238,   130,   202,   170,   156,   132,   158,    24,    34,   158,
      24,    34,    80,   158,   232,    71,   147,   220,   144,   132,
     187,    73,   132,   156,   208,   209,     1,   103,   211,   212,
      34,   104,   158,   220,   157,   156,   104,   123,   123,   130,
     144,   158,   132,   220,    85,   203,   123,   123,    28,    49,
     158,   123,   131,   142,   104,   131,   238,   104,   133,   104,
     133,    34,   104,   135,   232,    83,   104,   135,     7,   156,
     103,   171,   181,    61,   206,   206,   206,   206,   238,   238,
     238,   238,   164,   238,   164,   238,   238,   238,   238,   238,
     238,   238,    25,    72,   156,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   220,   220,   164,
     238,   164,   238,    20,    34,    37,    51,    52,    61,    72,
     191,   231,   234,   238,   250,    24,    34,    24,    34,    71,
      34,   135,   164,   238,   158,   123,   238,    83,    85,   133,
     104,   156,   174,   175,   123,   156,    34,   217,    33,   238,
     164,   238,    33,   238,   164,   238,   132,   158,   144,   238,
      26,   133,   188,   189,   190,   191,   178,   209,   104,   133,
       1,   134,   213,   224,    83,    85,   210,   238,   207,   133,
     197,   238,   167,   220,   133,   134,   213,   224,   104,   126,
     149,   151,   238,   149,   150,   131,   133,   133,   218,   133,
     218,   164,   238,   135,   144,   238,   135,   238,   135,   238,
     130,   217,   130,    52,    61,    72,   183,   192,   206,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   133,
     135,    34,    61,   223,   104,   133,    60,    60,    33,   164,
     238,    33,   164,   238,   164,   238,   229,   229,   123,   238,
     197,   238,   222,    85,   104,   131,   171,   238,   133,   238,
      24,   158,    24,   158,   238,    24,   158,   232,    24,   158,
     232,   233,   234,    26,    26,   144,   104,   133,   132,   156,
      20,    52,    61,    72,   194,   133,   209,   103,   212,   217,
     238,    47,   238,    50,    77,   131,   168,   133,   217,   202,
     104,   123,    11,   123,   135,   232,   135,   232,   144,    83,
     131,   148,   172,   182,    10,    15,    85,    86,    87,    88,
      89,    90,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   105,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   121,   124,   125,   126,   127,   129,
     156,   184,   185,   123,   234,   223,   223,   238,    24,    24,
     238,    24,    24,   135,   135,   158,   158,   238,   175,   130,
     158,    71,    33,   238,    33,   238,    71,    33,   238,   158,
      33,   238,   158,   104,   133,   144,   238,   189,   209,   103,
     216,    61,   212,   210,    34,   135,    24,   144,   217,   135,
     238,   238,    33,   144,   238,   135,    33,   144,   238,   135,
     238,   131,    30,    56,    58,   131,   143,   160,   194,   156,
     106,   132,   186,   186,    71,    33,    33,    71,    33,    33,
      33,    33,   172,   238,   238,   238,   238,   238,   238,   234,
     133,    25,    67,    70,   134,   197,   215,   224,   108,   199,
     210,    78,   200,   238,   213,   224,   144,   158,    11,   238,
     144,   238,   144,   160,   212,   156,   184,   185,   188,   238,
     238,   238,   238,   238,   238,   131,    71,    71,    71,    71,
     216,   135,   217,   197,   198,   238,   238,   147,   159,   196,
     135,   238,    71,    71,   200,   186,   186,   133,    71,    71,
      71,    71,    71,    71,   238,   238,   238,   238,   199,   210,
     197,   214,   215,   224,    34,   135,   224,   238,   238,   123,
     195,   196,   238,   214,   215,   135,   214
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
#line 421 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 426 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 427 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 434 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 439 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 440 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 461 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 466 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 467 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 468 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 469 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 470 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 475 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 477 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 482 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 483 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 498 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 499 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 520 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 524 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 530 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 554 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 558 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 562 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 563 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 564 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 567 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 568 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 586 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 587 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 591 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 592 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 601 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 617 "chapel.ypp"
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

  case 91:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 641 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 642 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 643 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), zipToTuple((yyvsp[(4) - (5)].pcallexpr)), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pForallIntents),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 654 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 660 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 666 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (7)].pcallexpr)), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 672 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 679 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 695 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 710 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 721 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 726 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 733 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 738 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 743 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 766 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 776 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 785 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 789 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 796 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 797 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 802 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 807 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 812 "chapel.ypp"
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

  case 143:

/* Line 1806 of yacc.c  */
#line 832 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 838 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 853 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
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

  case 150:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 908 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 925 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 939 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 948 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 949 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 950 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 963 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 964 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 985 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 994 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1008 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1014 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1018 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1020 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1044 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1049 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1050 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1055 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1060 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1064 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1069 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
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

  case 242:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1123 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1149 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1150 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1151 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1157 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1165 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1167 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1173 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1180 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1191 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1209 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1210 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1241 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1261 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1276 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1277 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1278 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1300 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1301 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1310 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1311 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1357 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1359 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1363 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1365 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1367 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1369 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1371 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1375 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1377 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1381 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1388 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1394 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1400 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1406 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1419 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1435 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1439 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1450 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1453 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pForallIntents) = (yyvsp[(3) - (4)].pForallIntents); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1463 "chapel.ypp"
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1466 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pForallIntents), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1471 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1474 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1475 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1486 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1507 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1511 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1517 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1546 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1548 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1573 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1577 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1589 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1615 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1616 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1617 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1618 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1619 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1620 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1624 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1625 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1626 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8128 "bison-chapel.cpp"
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



