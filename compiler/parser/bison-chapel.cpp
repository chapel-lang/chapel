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
    Expr*  first;
    Expr*  second;
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
    std::vector<OnlyRename*>* ponlylist;
  };

  #endif


/* Line 288 of yacc.c  */
#line 132 "chapel.ypp"

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
#line 154 "chapel.ypp"

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
#line 236 "bison-chapel.cpp"

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
#line 185 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 442 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 448 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  int         captureTokens;
  char        captureString[1024];


/* Line 344 of yacc.c  */
#line 193 "chapel.ypp"

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
#line 522 "bison-chapel.cpp"

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
#define YYLAST   12571

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  139
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  108
/* YYNRULES -- Number of rules.  */
#define YYNRULES  440
/* YYNRULES -- Number of states.  */
#define YYNSTATES  841

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
     114,   117,   121,   123,   126,   128,   132,   136,   142,   146,
     152,   158,   162,   167,   172,   177,   182,   187,   192,   197,
     202,   207,   212,   217,   222,   227,   232,   237,   242,   243,
     245,   247,   249,   251,   254,   256,   259,   263,   265,   267,
     270,   273,   275,   277,   279,   281,   283,   286,   292,   296,
     303,   310,   315,   321,   327,   331,   335,   342,   348,   355,
     361,   368,   372,   377,   384,   392,   399,   407,   412,   418,
     423,   428,   432,   439,   445,   451,   452,   455,   459,   462,
     466,   473,   482,   484,   486,   488,   489,   492,   493,   496,
     500,   506,   508,   511,   515,   517,   521,   522,   523,   532,
     533,   535,   538,   541,   542,   543,   553,   557,   561,   567,
     573,   575,   578,   580,   582,   584,   586,   588,   590,   592,
     594,   596,   598,   600,   602,   604,   606,   608,   610,   612,
     614,   616,   618,   620,   622,   624,   626,   628,   630,   632,
     634,   636,   638,   640,   642,   644,   646,   648,   650,   651,
     655,   659,   660,   662,   666,   671,   676,   683,   690,   691,
     693,   695,   697,   699,   701,   704,   707,   709,   711,   713,
     714,   716,   718,   720,   722,   724,   725,   727,   730,   732,
     734,   736,   738,   740,   742,   744,   747,   749,   750,   752,
     755,   758,   759,   762,   766,   771,   776,   779,   784,   785,
     788,   791,   796,   801,   806,   812,   817,   818,   820,   822,
     824,   828,   832,   837,   843,   845,   847,   851,   853,   856,
     860,   861,   864,   867,   868,   873,   874,   877,   880,   882,
     887,   892,   899,   901,   902,   904,   906,   910,   915,   919,
     924,   931,   932,   935,   938,   941,   944,   947,   950,   952,
     954,   958,   962,   964,   966,   968,   972,   976,   977,   979,
     981,   985,   989,   993,   997,   999,  1001,  1003,  1005,  1007,
    1009,  1011,  1013,  1016,  1021,  1026,  1031,  1037,  1040,  1043,
    1050,  1057,  1062,  1072,  1082,  1090,  1097,  1104,  1109,  1119,
    1129,  1137,  1142,  1149,  1156,  1166,  1176,  1183,  1185,  1187,
    1189,  1191,  1193,  1195,  1197,  1199,  1203,  1204,  1206,  1211,
    1213,  1217,  1220,  1224,  1228,  1230,  1234,  1237,  1242,  1244,
    1246,  1248,  1250,  1252,  1254,  1256,  1258,  1263,  1267,  1271,
    1274,  1277,  1279,  1281,  1283,  1285,  1287,  1289,  1291,  1296,
    1301,  1306,  1310,  1314,  1318,  1322,  1327,  1331,  1333,  1335,
    1337,  1339,  1341,  1345,  1349,  1353,  1357,  1363,  1367,  1371,
    1375,  1379,  1383,  1387,  1391,  1395,  1399,  1403,  1407,  1411,
    1415,  1419,  1423,  1427,  1431,  1435,  1439,  1443,  1447,  1451,
    1455,  1458,  1461,  1464,  1467,  1470,  1473,  1477,  1481,  1485,
    1489,  1493,  1497,  1501,  1505,  1507,  1509,  1511,  1513,  1515,
    1517
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     140,     0,    -1,   141,    -1,    -1,   141,   142,    -1,   144,
      -1,   143,   144,    -1,    54,     7,    -1,   143,    54,     7,
      -1,   145,    -1,   147,    -1,   150,    -1,   151,    -1,   158,
      -1,   152,    -1,   160,    -1,   163,    -1,   161,    -1,   164,
      -1,   157,    -1,   226,   123,    -1,    12,   144,    -1,    13,
     227,   144,    -1,    14,   153,   123,    -1,    17,   227,   147,
      -1,    21,   153,   123,    -1,    22,   234,   123,    -1,    39,
     154,   144,    -1,    42,   144,    -1,    48,   234,   156,    -1,
      66,   234,   156,    -1,    66,   156,    -1,    70,   144,    -1,
      81,   234,   123,    -1,     1,    -1,   146,    44,   154,   130,
     131,    -1,   146,    44,   154,   130,   148,   131,    -1,    -1,
      58,    -1,    56,    -1,   130,   131,    -1,   130,   148,   131,
      -1,   142,    -1,   148,   142,    -1,   234,    -1,   234,    11,
     234,    -1,   149,   104,   234,    -1,   149,   104,   234,    11,
     234,    -1,    75,   215,   123,    -1,    75,   234,    28,   149,
     123,    -1,    75,   234,    49,   149,   123,    -1,    62,   215,
     123,    -1,   235,    85,   234,   123,    -1,   235,    96,   234,
     123,    -1,   235,    93,   234,   123,    -1,   235,    95,   234,
     123,    -1,   235,    89,   234,   123,    -1,   235,    94,   234,
     123,    -1,   235,    90,   234,   123,    -1,   235,    86,   234,
     123,    -1,   235,    87,   234,   123,    -1,   235,    88,   234,
     123,    -1,   235,    98,   234,   123,    -1,   235,    97,   234,
     123,    -1,   235,   127,   234,   123,    -1,   235,   128,   234,
     123,    -1,   235,    91,   234,   123,    -1,   235,    92,   234,
     123,    -1,    -1,   154,    -1,     3,    -1,   153,    -1,     7,
      -1,    24,   144,    -1,   147,    -1,    63,   123,    -1,    63,
     234,   123,    -1,   123,    -1,   159,    -1,    58,   159,    -1,
      56,   159,    -1,   171,    -1,   199,    -1,   167,    -1,   178,
      -1,   202,    -1,    30,     9,    -1,    24,   144,    79,   234,
     123,    -1,    79,   234,   156,    -1,    18,   234,    34,   234,
     227,   156,    -1,    18,   234,    34,   162,   227,   156,    -1,
      18,   234,   227,   156,    -1,    31,   234,    34,   234,   156,
      -1,    31,   234,    34,   162,   156,    -1,    31,   234,   156,
      -1,    31,   162,   156,    -1,    31,    52,   154,    34,   234,
     156,    -1,    32,   234,    34,   234,   156,    -1,    32,   234,
      34,   234,   228,   156,    -1,    32,   234,    34,   162,   156,
      -1,    32,   234,    34,   162,   228,   156,    -1,    32,   234,
     156,    -1,    32,   234,   228,   156,    -1,   134,   215,    34,
     234,   135,   144,    -1,   134,   215,    34,   234,   228,   135,
     144,    -1,   134,   215,    34,   162,   135,   144,    -1,   134,
     215,    34,   162,   228,   135,   144,    -1,   134,   215,   135,
     144,    -1,   134,   215,   228,   135,   144,    -1,    82,   132,
     215,   133,    -1,    33,   234,    71,   144,    -1,    33,   234,
     147,    -1,    33,   234,    71,   144,    26,   144,    -1,    33,
     234,   147,    26,   144,    -1,    65,   234,   130,   165,   131,
      -1,    -1,   165,   166,    -1,    77,   215,   156,    -1,    50,
     144,    -1,    50,    24,   144,    -1,   168,   154,   169,   130,
     170,   131,    -1,    30,   155,   168,   154,   169,   130,   170,
     131,    -1,    16,    -1,    59,    -1,    74,    -1,    -1,   103,
     215,    -1,    -1,   170,   158,    -1,   170,   143,   158,    -1,
      27,   154,   130,   172,   131,    -1,   173,    -1,   172,   104,
      -1,   172,   104,   173,    -1,   154,    -1,   154,    85,   234,
      -1,    -1,    -1,    40,   175,   185,   176,   192,   210,   198,
     194,    -1,    -1,    36,    -1,    29,   155,    -1,    30,   155,
      -1,    -1,    -1,   177,   191,   179,   181,   180,   192,   210,
     198,   193,    -1,   190,   182,   184,    -1,   190,   183,   184,
      -1,   190,   154,   106,   182,   184,    -1,   190,   154,   106,
     183,   184,    -1,   154,    -1,   100,   154,    -1,    99,    -1,
     101,    -1,   102,    -1,   100,    -1,   109,    -1,   119,    -1,
     115,    -1,   112,    -1,   114,    -1,   111,    -1,   121,    -1,
     116,    -1,   126,    -1,   105,    -1,   124,    -1,   125,    -1,
     117,    -1,   110,    -1,   118,    -1,    15,    -1,   113,    -1,
      10,    -1,   127,    -1,   129,    -1,    85,    -1,    96,    -1,
      93,    -1,    95,    -1,    89,    -1,    94,    -1,    90,    -1,
      86,    -1,    87,    -1,    88,    -1,    98,    -1,    97,    -1,
      -1,   132,   186,   133,    -1,   132,   186,   133,    -1,    -1,
     187,    -1,   186,   104,   187,    -1,   188,   154,   214,   208,
      -1,   188,   154,   214,   197,    -1,   188,   132,   207,   133,
     214,   208,    -1,   188,   132,   207,   133,   214,   197,    -1,
      -1,   189,    -1,    34,    -1,    37,    -1,    51,    -1,    20,
      -1,    20,    34,    -1,    20,    61,    -1,    52,    -1,    61,
      -1,    72,    -1,    -1,    52,    -1,    61,    -1,    72,    -1,
      57,    -1,    38,    -1,    -1,    20,    -1,    20,    61,    -1,
      61,    -1,    52,    -1,    72,    -1,   123,    -1,   194,    -1,
     147,    -1,   157,    -1,   122,   154,    -1,   122,    -1,    -1,
     195,    -1,   108,   234,    -1,   108,   196,    -1,    -1,    78,
     234,    -1,    72,   200,   123,    -1,    19,    72,   200,   123,
      -1,    30,    72,   200,   123,    -1,   154,   201,    -1,   154,
     201,   104,   200,    -1,    -1,    85,   222,    -1,    85,   211,
      -1,   203,    52,   204,   123,    -1,   203,    20,   204,   123,
      -1,   203,    61,   204,   123,    -1,   203,    20,    61,   204,
     123,    -1,   203,    76,   204,   123,    -1,    -1,    19,    -1,
      30,    -1,   205,    -1,   204,   104,   205,    -1,   154,   210,
     208,    -1,   154,   209,    83,   234,    -1,   132,   207,   133,
     210,   208,    -1,    73,    -1,   154,    -1,   132,   207,   133,
      -1,   206,    -1,   206,   104,    -1,   206,   104,   207,    -1,
      -1,    85,    47,    -1,    85,   234,    -1,    -1,   103,   134,
     215,   135,    -1,    -1,   103,   222,    -1,   103,   211,    -1,
       1,    -1,   134,   215,   135,   222,    -1,   134,   215,   135,
     211,    -1,   134,   215,    34,   234,   135,   222,    -1,     1,
      -1,    -1,   222,    -1,   195,    -1,   134,   135,   212,    -1,
     134,   215,   135,   212,    -1,   134,   135,   213,    -1,   134,
     215,   135,   213,    -1,   134,   215,    34,   234,   135,   212,
      -1,    -1,   103,   222,    -1,   103,   195,    -1,   103,    25,
      -1,   103,    67,    -1,   103,    70,    -1,   103,   213,    -1,
     234,    -1,   195,    -1,   215,   104,   234,    -1,   215,   104,
     195,    -1,    73,    -1,   234,    -1,   195,    -1,   216,   104,
     216,    -1,   217,   104,   216,    -1,    -1,   219,    -1,   220,
      -1,   219,   104,   220,    -1,   154,    85,   195,    -1,   154,
      85,   234,    -1,   154,    83,   234,    -1,   195,    -1,   234,
      -1,   154,    -1,   225,    -1,   235,    -1,   224,    -1,   243,
      -1,   242,    -1,    67,   234,    -1,    35,   132,   218,   133,
      -1,    25,   132,   218,   133,    -1,    69,   132,   218,   133,
      -1,    68,    69,   132,   218,   133,    -1,    12,   234,    -1,
      70,   234,    -1,    31,   234,    34,   234,    24,   234,    -1,
      31,   234,    34,   162,    24,   234,    -1,    31,   234,    24,
     234,    -1,    31,   234,    34,   234,    24,    33,   234,    71,
     234,    -1,    31,   234,    34,   162,    24,    33,   234,    71,
     234,    -1,    31,   234,    24,    33,   234,    71,   234,    -1,
      32,   234,    34,   234,    24,   234,    -1,    32,   234,    34,
     162,    24,   234,    -1,    32,   234,    24,   234,    -1,    32,
     234,    34,   234,    24,    33,   234,    71,   234,    -1,    32,
     234,    34,   162,    24,    33,   234,    71,   234,    -1,    32,
     234,    24,    33,   234,    71,   234,    -1,   134,   215,   135,
     234,    -1,   134,   215,    34,   234,   135,   234,    -1,   134,
     215,    34,   162,   135,   234,    -1,   134,   215,    34,   234,
     135,    33,   234,    71,   234,    -1,   134,   215,    34,   162,
     135,    33,   234,    71,   234,    -1,    33,   234,    71,   234,
      26,   234,    -1,    46,    -1,   225,    -1,   221,    -1,   238,
      -1,   237,    -1,   174,    -1,   232,    -1,   233,    -1,   231,
     129,   234,    -1,    -1,   228,    -1,    80,   132,   229,   133,
      -1,   230,    -1,   229,   104,   230,    -1,   189,   221,    -1,
     234,    60,   221,    -1,   246,    60,   221,    -1,   235,    -1,
     231,   129,   234,    -1,    45,   234,    -1,    41,   204,    34,
     234,    -1,   240,    -1,   222,    -1,   223,    -1,   244,    -1,
     245,    -1,   174,    -1,   232,    -1,   233,    -1,   132,   108,
     234,   133,    -1,   234,   103,   234,    -1,   234,   107,   234,
      -1,   234,   107,    -1,   107,   234,    -1,   107,    -1,   221,
      -1,   237,    -1,   238,    -1,   239,    -1,   235,    -1,   174,
      -1,   236,   132,   218,   133,    -1,   236,   134,   218,   135,
      -1,    55,   132,   218,   133,    -1,   234,   106,   154,    -1,
     234,   106,    72,    -1,   234,   106,    25,    -1,   132,   216,
     133,    -1,   132,   216,   104,   133,    -1,   132,   217,   133,
      -1,     4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,
      -1,   130,   215,   131,    -1,   134,   215,   135,    -1,   134,
     241,   135,    -1,   234,    83,   234,    -1,   241,   104,   234,
      83,   234,    -1,   234,   121,   234,    -1,   234,   116,   234,
      -1,   234,   126,   234,    -1,   234,   105,   234,    -1,   234,
     124,   234,    -1,   234,   125,   234,    -1,   234,   117,   234,
      -1,   234,   109,   234,    -1,   234,   119,   234,    -1,   234,
     115,   234,    -1,   234,   112,   234,    -1,   234,   114,   234,
      -1,   234,   111,   234,    -1,   234,    99,   234,    -1,   234,
     101,   234,    -1,   234,   102,   234,    -1,   234,    84,   234,
      -1,   234,   120,   234,    -1,   234,   110,   234,    -1,   234,
      15,   234,    -1,   234,    10,   234,    -1,   234,   113,   234,
      -1,   234,    23,   234,    -1,   121,   234,    -1,   116,   234,
      -1,    43,   234,    -1,    53,   234,    -1,   118,   234,    -1,
     100,   234,    -1,   234,    60,   234,    -1,   234,    60,   162,
      -1,   246,    60,   234,    -1,   246,    60,   162,    -1,   234,
      64,   234,    -1,   234,    64,   162,    -1,   246,    64,   234,
      -1,   246,    64,   162,    -1,   121,    -1,   126,    -1,    84,
      -1,   120,    -1,    99,    -1,   101,    -1,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   418,   418,   423,   424,   430,   431,   436,   437,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   471,   473,   478,   479,   480,
     495,   496,   501,   502,   507,   512,   517,   521,   527,   528,
     529,   533,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   556,   557,
     561,   565,   566,   570,   571,   575,   576,   580,   581,   582,
     583,   584,   585,   586,   590,   591,   595,   610,   611,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   632,   638,   644,   650,   657,   667,
     671,   672,   673,   674,   678,   683,   684,   688,   690,   692,
     699,   704,   712,   717,   722,   730,   731,   736,   737,   739,
     744,   754,   763,   767,   775,   776,   781,   786,   780,   811,
     817,   824,   832,   843,   849,   842,   877,   881,   886,   890,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   936,   937,   938,   942,   943,
     947,   951,   952,   953,   957,   959,   961,   963,   968,   969,
     973,   974,   975,   976,   977,   978,   979,   980,   981,   985,
     986,   987,   988,   992,   993,   997,   998,   999,  1000,  1001,
    1002,  1006,  1007,  1011,  1012,  1016,  1018,  1023,  1024,  1028,
    1029,  1033,  1034,  1038,  1040,  1042,  1047,  1060,  1077,  1078,
    1080,  1085,  1093,  1101,  1109,  1118,  1128,  1129,  1130,  1134,
    1135,  1143,  1145,  1151,  1156,  1158,  1160,  1165,  1167,  1169,
    1176,  1177,  1178,  1182,  1183,  1188,  1189,  1190,  1191,  1211,
    1215,  1219,  1227,  1231,  1232,  1233,  1237,  1239,  1245,  1247,
    1249,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1266,  1267,
    1268,  1269,  1273,  1274,  1275,  1279,  1280,  1284,  1285,  1289,
    1290,  1294,  1295,  1296,  1297,  1298,  1302,  1313,  1314,  1315,
    1316,  1317,  1318,  1320,  1322,  1324,  1326,  1328,  1330,  1335,
    1337,  1339,  1341,  1343,  1345,  1347,  1349,  1351,  1353,  1355,
    1357,  1359,  1366,  1372,  1378,  1384,  1393,  1398,  1406,  1407,
    1408,  1409,  1410,  1411,  1412,  1413,  1418,  1419,  1423,  1427,
    1429,  1437,  1448,  1449,  1453,  1454,  1459,  1464,  1472,  1473,
    1474,  1475,  1476,  1477,  1478,  1479,  1480,  1482,  1484,  1486,
    1488,  1490,  1495,  1496,  1497,  1498,  1509,  1510,  1514,  1515,
    1516,  1520,  1521,  1522,  1530,  1531,  1532,  1536,  1537,  1538,
    1539,  1540,  1541,  1542,  1543,  1550,  1551,  1555,  1556,  1557,
    1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1566,  1567,
    1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,  1576,  1577,
    1581,  1582,  1583,  1584,  1585,  1586,  1590,  1591,  1592,  1593,
    1597,  1598,  1599,  1600,  1605,  1606,  1607,  1608,  1609,  1610,
    1611
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
  "use_stmt", "require_stmt", "assignment_stmt", "opt_ident", "ident",
  "opt_ident_or_string", "do_stmt", "return_stmt", "class_level_stmt",
  "private_decl", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
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
     150,   151,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   153,   153,
     154,   155,   155,   156,   156,   157,   157,   158,   158,   158,
     158,   158,   158,   158,   159,   159,   160,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   162,
     163,   163,   163,   163,   164,   165,   165,   166,   166,   166,
     167,   167,   168,   168,   168,   169,   169,   170,   170,   170,
     171,   172,   172,   172,   173,   173,   175,   176,   174,   177,
     177,   177,   177,   179,   180,   178,   181,   181,   181,   181,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   184,   184,
     185,   186,   186,   186,   187,   187,   187,   187,   188,   188,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   190,
     190,   190,   190,   191,   191,   192,   192,   192,   192,   192,
     192,   193,   193,   194,   194,   195,   195,   196,   196,   197,
     197,   198,   198,   199,   199,   199,   200,   200,   201,   201,
     201,   202,   202,   202,   202,   202,   203,   203,   203,   204,
     204,   205,   205,   205,   206,   206,   206,   207,   207,   207,
     208,   208,   208,   209,   209,   210,   210,   210,   210,   211,
     211,   211,   211,   212,   212,   212,   213,   213,   213,   213,
     213,   214,   214,   214,   214,   214,   214,   214,   215,   215,
     215,   215,   216,   216,   216,   217,   217,   218,   218,   219,
     219,   220,   220,   220,   220,   220,   221,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   224,   225,   226,   226,
     226,   226,   226,   226,   226,   226,   227,   227,   228,   229,
     229,   230,   230,   230,   231,   231,   232,   233,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   235,   235,   235,   235,   236,   236,   237,   237,
     237,   238,   238,   238,   239,   239,   239,   240,   240,   240,
     240,   240,   240,   240,   240,   241,   241,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     243,   243,   243,   243,   243,   243,   244,   244,   244,   244,
     245,   245,   245,   245,   246,   246,   246,   246,   246,   246,
     246
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     5,     6,     0,     1,     1,
       2,     3,     1,     2,     1,     3,     3,     5,     3,     5,
       5,     3,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     1,     1,     2,     1,     2,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     2,     5,     3,     6,
       6,     4,     5,     5,     3,     3,     6,     5,     6,     5,
       6,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       4,     3,     6,     5,     5,     0,     2,     3,     2,     3,
       6,     8,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     2,     2,     0,     0,     9,     3,     3,     5,     5,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     3,
       3,     0,     1,     3,     4,     4,     6,     6,     0,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     1,
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
       3,     2,     3,     3,     1,     3,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     4,     3,     1,     1,     1,
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
       3,     0,     0,     1,    34,    70,   387,   388,   389,   390,
     391,     0,   346,    68,   122,   346,     0,   247,    68,     0,
       0,     0,     0,    68,    68,     0,     0,     0,     0,   140,
       0,   136,     0,     0,     0,     0,   337,     0,     0,     0,
       0,   246,   246,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   124,     0,     0,     0,   436,   438,     0,
     439,   440,   371,     0,     0,   437,   434,    77,   435,     0,
       0,     0,     4,     0,     5,     9,     0,    10,    11,    12,
      14,   306,    19,    13,    78,    15,    17,    16,    18,    83,
       0,    81,   363,     0,    84,    82,    85,     0,   372,   359,
     360,   309,   307,     0,     0,   364,   365,     0,   308,     0,
     373,   374,   375,   358,   311,   310,   361,   362,     0,    21,
     317,     0,     0,   347,     0,    69,     0,     0,     0,     0,
       0,     0,     0,     0,   363,   372,   307,   364,   365,   346,
     308,   373,   374,     0,     0,     0,     0,   297,     0,    72,
      71,   141,    86,     0,   142,     0,     0,     0,     0,     0,
       0,   297,     0,     0,     0,     0,     0,   249,    28,   422,
     356,     0,   423,     7,   297,   247,   248,    80,    79,   226,
     289,     0,   288,    75,     0,     0,     0,    74,    31,     0,
     312,     0,   297,    32,   318,   238,     0,     0,   288,     0,
       0,   425,   370,   421,   424,   420,    40,    42,     0,     0,
     292,     0,   294,     0,     0,   293,     0,   288,     0,     0,
       6,     0,   125,   214,   213,   143,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   369,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   297,   297,     0,     0,
       0,    22,    23,     0,    24,     0,     0,     0,     0,     0,
       0,     0,    25,    26,     0,   306,   304,     0,   298,   299,
     305,     0,     0,     0,     0,     0,    95,     0,     0,    94,
       0,     0,   101,     0,     0,   111,     0,    27,   191,   137,
     254,     0,   255,   257,     0,   268,     0,     0,   260,     0,
       0,    29,     0,   142,   225,     0,    51,    76,   115,    73,
      30,   297,     0,     0,   236,   233,    48,     0,     0,    88,
      33,    41,    43,   392,     0,     0,   384,     0,   386,     0,
       0,     0,     0,     0,   394,     8,     0,     0,     0,   209,
       0,     0,     0,     0,     0,   345,   417,   416,   419,   427,
     426,   431,   430,   413,   410,   411,   412,   367,   400,   383,
     382,   381,   368,   404,   415,   409,   407,   418,   408,   406,
     398,   403,   405,   414,   397,   401,   402,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   429,   428,   433,   432,
     203,   200,   201,   202,   206,   207,   208,     0,     0,   349,
       0,     0,     0,     0,     0,     0,     0,     0,   393,   346,
     346,    91,   234,     0,     0,     0,   314,     0,   134,     0,
     131,   235,   125,     0,     0,     0,   321,     0,     0,     0,
     327,     0,     0,   102,   110,     0,     0,   313,     0,   192,
       0,   199,   215,     0,   258,     0,   272,     0,   267,   359,
       0,     0,   251,   357,   250,   380,   291,   290,     0,     0,
     315,     0,   240,   359,     0,     0,    44,     0,   366,   385,
     295,   296,     0,     0,   107,   331,     0,   395,     0,     0,
     126,   127,   210,   211,   212,   144,     0,     0,   242,   241,
     243,   245,    52,    59,    60,    61,    56,    58,    66,    67,
      54,    57,    55,    53,    63,    62,    64,    65,   378,   379,
     204,   205,   351,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,   303,   301,
     302,   300,     0,   132,   130,     0,     0,   109,     0,     0,
      93,     0,    92,     0,     0,    99,     0,     0,    97,     0,
       0,     0,   113,   198,   190,     0,   281,   216,   219,   218,
     220,     0,   256,   259,     0,   260,     0,   252,   261,   262,
       0,     0,   114,   116,   316,     0,   237,     0,    49,     0,
      50,     0,     0,     0,     0,   108,     0,    35,     0,   246,
     215,   173,   171,   176,   183,   184,   185,   180,   182,   178,
     181,   179,   177,   187,   186,   152,   155,   153,   154,   165,
     156,   169,   161,   159,   172,   160,   158,   163,   168,   170,
     157,   162,   166,   167,   164,   174,   175,   150,   188,   188,
     244,   350,   372,   372,     0,     0,     0,     0,     0,     0,
       0,     0,    90,    89,   135,   133,   127,    96,     0,     0,
     320,     0,   319,     0,     0,   326,   100,     0,   325,    98,
     112,   336,   193,     0,     0,   260,   217,   231,   253,     0,
       0,     0,   118,     0,     0,    46,    45,     0,   105,   333,
       0,     0,   103,   332,     0,   396,    36,    68,   246,   246,
     120,   246,   128,     0,   151,     0,   191,   146,   147,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   324,     0,
       0,   330,     0,     0,   281,   284,   285,   286,     0,   283,
     287,   359,   227,   195,   194,     0,     0,     0,   270,   359,
     119,   117,     0,     0,   106,     0,   104,   129,   231,   150,
     188,   188,     0,     0,     0,     0,     0,     0,     0,   121,
       0,     0,     0,     0,   260,   273,     0,   228,   230,   229,
     232,   223,   224,   138,     0,    47,     0,     0,     0,   148,
     149,   189,     0,     0,     0,     0,     0,     0,   323,   322,
     329,   328,   197,   196,   275,   276,   278,   359,     0,   393,
     359,   335,   334,   221,   145,   222,     0,   277,   279,   273,
     280
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   207,    73,   474,    75,    76,    77,   208,
     505,    78,    79,    80,   150,    81,   154,   188,    82,    83,
      84,    85,    86,   563,    87,    88,   498,   613,    89,    90,
     368,   629,    91,   459,   460,   134,   163,   482,    93,    94,
     369,   630,   525,   668,   669,   737,   319,   478,   479,   480,
     481,   526,   225,   601,   834,   803,   180,   798,   763,   766,
      95,   196,   344,    96,    97,   166,   167,   323,   324,   492,
     327,   328,   488,   825,   760,   705,   209,   213,   214,   297,
     298,   299,   135,    99,   100,   101,   136,   103,   122,   123,
     438,   439,   104,   137,   138,   107,   140,   109,   141,   142,
     112,   113,   218,   114,   115,   116,   117,   118
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -731
static const yytype_int16 yypact[] =
{
    -731,    68,  2385,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  3536,    -3,    66,  -731,    -3,  7233,    22,    66,  7233,
    3536,    13,    66,   177,   499,  5913,  7233,  7233,    37,  -731,
      66,  -731,    39,  3536,  7233,  7233,  -731,  7233,  7233,   122,
      86,   599,   603,  -731,  6177,  6309,  7233,  6441,  7233,   163,
     148,  3536,    66,  -731,  6177,  7233,  7233,  -731,  -731,  7233,
    -731,  -731,  8421,  7233,  7233,  -731,  7233,  -731,  -731,  2744,
    5649,  6177,  -731,  3404,  -731,  -731,   248,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
      66,  -731,   282,   156,  -731,  -731,  -731,   250,   166,  -731,
    -731,  -731,   181,   186,   202,   218,   231, 12271,  1470,   183,
     262,   268,  -731,  -731,  -731,  -731,  -731,  -731,   142,  -731,
   12271,   265,  3536,  -731,   277,  -731,   276,  7233,  7233,  7233,
    7233,  7233,  6177,  6177,   245,  -731,  -731,  -731,  -731,  9808,
     288,  -731,  -731,    66,   289, 10102,   344,  6177,   294,  -731,
    -731,  -731,  -731,    66,   130,    66,   293,    23,  8853,  8819,
    9019,  6177,  3536,   296,    17,    69,    -2,  -731,  -731,  1003,
     320,  9082,  1003,  -731,  6177,  -731,   336,  -731,  -731,    66,
    -731,   206, 12271,  -731, 10136,  9687,  3536,  -731,  -731,  9082,
   12271,   307,  6177,  -731, 12271,   360,   325,   209,  9916,  9082,
   10221,   252,  1865,  1003,   252,  1003,  -731,  -731,  2876,   -22,
    -731,  7233,  -731,   -48,   -29, 12271,    61, 10265,   -23,   442,
    -731,    66,   347,  -731,  -731,  -731,    35,    39,    39,    39,
    -731,  7233,  7233,  7233,  7233,  6573,  6573,  7233,  7233,  7233,
    7233,  7233,  7233,    54,  8421,  7233,  7233,  7233,  7233,  7233,
    7233,  7233,  7233,  7233,  7233,  7233,  7233,  7233,  7233,  7233,
    7233,  7233,  7233,  7233,  7233,  7233,  7233,  7233,  7233,  7233,
    7233,  7233,  7233,  7233,  7233,  7233,  6177,  6177,  6573,  6573,
    5517,  -731,  -731,  3008,  -731,  9960, 10068, 10340,     7,  6573,
      23,   330,  -731,  -731,  7233,   222,  -731,   324,   355,  -731,
   12271,    66,   337,    66,   427,  6177,  -731,  3668,  6573,  -731,
    3800,  6573,  -731,    23,  3536,   440,   334,  -731,    63,  -731,
    -731,    17,  -731,   367,   339,  -731,  5120,   390,   389,  7233,
      39,  -731,   342,  -731,  -731,  6177,  -731,  -731,  -731,  -731,
    -731,  6177,   345,  5252,   377,  -731,  -731,  7233,  7233,  -731,
    -731,  -731,  -731,  -731,  8711,  5781,  -731,  6045,  -731,  6573,
    2567,   350,  7233,  7233,  -731,  -731,   353,  6177,   356,   315,
      39,   210,   217,   243,   249,  9745,  8638,  8638,   -17,  -731,
     -17,  -731,   -17,  8887,   590,   433,  1546,   320,   252,  -731,
    -731,  -731,  1865, 12445,   -17,  1578,  1578,  8638,  1578,  1578,
    1020,   252, 12445, 12387,  1020,  1003,  1003,   252, 10384, 10452,
   10496, 10564, 10608, 10676, 10720, 10788, 10832, 10900, 10944, 11012,
   11056, 11124, 11168, 11236,   354,   361,  -731,   -17,  -731,   -17,
     132,  -731,  -731,  -731,  -731,  -731,  -731,    66,   -20,  -731,
   12329,   155,  7365,  6573,  7497,  6573,  7233,  6573,   845,    -3,
   11294,  -731,  -731, 11357,  7233,  6177,  -731,  6177,   403,   167,
    -731,  -731,   347,  7233,    79,  7233, 12271,    52,  9140,  7233,
   12271,    47,  8961,  -731,   463, 11424,  3536,  -731,    83,  -731,
      43,  -731,   273,   365,    17,    53,  -731,  6177,  -731,   484,
    7233,  6705,  -731, 12271,  -731,  -731,  -731, 12271,    60,   370,
    -731,  6177,  -731,   279,    66,   284, 11482,   290,  -731,  -731,
    -731,  -731,    -7,  1165,  -731, 12271,  3536, 12271, 11545,  3140,
     387,  -731,  -731,  -731,  -731,  -731,  8554,   292,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  5517,  -731,  6573,  6573,  7233,   475, 11603,
    7233,   480, 11666,   374,  1766,    23,    23,  -731, 12271,  -731,
   12271,  -731,  7233,    66,  -731,   382,  9082,  -731,  9203,  3932,
    -731,  4064,  -731,  9261,  4196,  -731,    23,  4328,  -731,    23,
    3536,  7233,  -731,   358,  -731,    17,   410,   454,  -731,  -731,
    -731,    29,  -731,  -731,  5252,   389,    44, 12271,  -731, 12271,
    4460,  6177,  -731,  -731,  -731,    64,  -731,  7233,  -731,  7233,
    -731,  4592,   381,  4724,   385,  -731,  7233,  -731,  3272,   956,
     273,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,    66,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   411,   391,   391,
    -731,  -731,    84,   173, 11724,  7629,  7761, 11787,  7893,  8025,
    8157,  8289,  -731,  -731, 12271,  -731,  -731,  -731,  3536,  7233,
   12271,  7233, 12271,  3536,  7233, 12271,  -731,  7233, 12271,  -731,
    -731, 12271,  -731,   388,  6837,   100,  -731,   444,  -731,  6573,
    4856,  3536,  -731,    56,  4988, 11845, 12271,  7233,  -731, 12271,
    3536,  7233,  -731, 12271,  3536, 12271,  -731,   366,   495,   495,
    -731,  1390,  -731,    28,  -731,  8554,    63,  -731,  -731,  7233,
    7233,  7233,  7233,  7233,  7233,  7233,  7233,  1176, 11424,  9324,
    9382, 11424,  9445,  9503,   410,    13,  7233,  7233,  5384,  -731,
    -731,   187,  6177,  -731,  -731,  7233,    25,  2131,  -731,   639,
     344,  -731,  7233,  9566,  -731,  9624,  -731,  -731,   444,  -731,
     391,   391,   175, 11908, 11966, 12029, 12087, 12150, 12208,  -731,
    3536,  3536,  3536,  3536,   100,  6969,    74,  -731,  -731, 12271,
   12271,  -731,  -731,  -731,  8289, 12271,  3536,  3536,    80,  -731,
    -731,  -731,  7233,  7233,  7233,  7233,  7233,  7233, 11424, 11424,
   11424, 11424,  -731,  -731,  -731,  -731,  -731,   257,  6573,  8503,
     736, 11424, 11424,  -731,  -731,  -731,  8667,  -731,  -731,  7101,
    -731
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -731,  -731,  -731,    -1,  -608,  1503,  -731,  -731,  1549,     8,
     178,  -731,  -731,  -731,   269,  2043,   -12,   306,  -661,  -557,
     -39,  -731,  -731,    49,  -731,  -731,  -731,  -731,  -731,   375,
      75,  -158,  -731,  -731,   -43,   378,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -194,  -187,  -619,  -731,  -183,   -38,  -731,
    -267,  -731,  -731,   -76,  -731,  -245,   127,  -731,  -229,  -212,
    -731,  -134,  -731,  -731,  -731,  -165,   237,  -731,  -314,  -580,
    -731,  -457,  -339,  -628,  -730,  -182,   -32,    27,  -731,  -143,
    -731,   117,   348,  -203,  -731,  -731,   739,  -731,    -9,  -149,
    -731,    24,  -731,   787,  1005,   -11,  1131,  -731,  1330,  1468,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -263
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -394
static const yytype_int16 yytable[] =
{
     120,    72,   177,   178,   502,   139,   126,   483,   145,   291,
     313,   151,   181,   437,   158,   159,   160,   441,   316,   302,
       5,   731,   197,   169,   170,   708,   171,   172,   605,   325,
     325,   332,   329,   182,   184,   185,   189,   190,     5,   216,
     194,   447,     5,   198,   199,   200,     5,   186,   201,   342,
     738,   202,   203,   204,   325,   205,   355,     5,   182,   215,
     217,   371,   372,   373,   374,   826,  -198,   361,     3,     5,
     325,   584,   732,   121,   157,   357,   579,   121,   709,   389,
     186,   363,   335,   430,   553,   356,   241,  -265,    45,   243,
     320,  -265,  -265,   246,   143,   359,   370,   431,   709,   838,
     432,   288,   330,  -265,   358,   802,  -265,  -265,   828,   353,
     610,   335,   364,   554,   433,   434,   120,   285,   286,   287,
     194,   182,   217,   489,   435,   764,   390,   121,   621,   173,
     290,   604,   604,   424,   425,   436,   300,   611,  -265,   731,
     503,   121,   448,    45,   707,   147,    14,   802,   335,   321,
     300,  -265,  -263,   283,  -265,   283,   604,  -265,  -265,  -265,
     335,   809,   810,   300,   333,   335,   550,   164,   335,   161,
     603,   164,   326,  -265,   777,   595,  -265,   283,   335,   710,
       5,   300,   283,   335,   149,   491,   283,   593,  -352,    43,
     732,   612,  -265,   551,   223,  -198,   360,   212,   499,   714,
     354,   837,   278,   833,    53,   527,   279,   352,   762,   829,
     283,   840,   577,   224,   823,   556,   594,  -352,   174,   279,
     375,   376,   377,   378,   380,   382,   383,   384,   385,   386,
     387,   388,   191,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   300,   300,   427,   429,   440,
     226,   573,  -282,   464,   296,   234,   778,  -353,   450,   593,
     192,   703,   124,   453,   379,   381,   437,   144,   296,  -339,
     441,  -282,   221,   597,   182,  -282,   466,   468,   574,   470,
     472,   296,   227,   475,  -338,   454,  -353,   455,   811,   230,
     335,   228,   235,   335,   330,   276,   236,   277,   493,   296,
    -282,   330,   586,   589,   497,   598,   229,   426,   428,   336,
     300,   231,   346,   528,   599,   520,   506,   506,   449,     5,
     529,  -343,  -274,   149,   215,   600,   215,   330,   513,   515,
      98,   517,   518,   330,  -344,   241,   182,   467,   243,    98,
     471,  -274,   246,   622,   624,  -274,   530,   522,    98,     5,
     616,   768,   531,   149,   -68,   768,   523,  -377,   430,  -377,
      92,    98,   510,  -239,   511,  -341,  -248,   524,   617,    92,
    -274,  -340,   431,   -68,   617,   432,   330,   280,    92,    98,
     282,   489,  -239,   296,   296,  -342,   283,   618,   512,   433,
     434,    92,   292,   620,  -377,   670,  -377,    98,  -248,   435,
    -376,    98,  -376,   294,   301,   305,   243,  -248,   318,    92,
     436,   466,   559,   470,   562,   475,   564,   515,   153,   341,
     565,   566,  -248,   568,   570,   343,   300,    92,   345,   365,
     367,    92,   576,   452,   578,   606,   234,   456,   583,   457,
     461,   463,   496,   306,   309,   312,   476,   477,   296,   615,
      98,   484,   485,   490,   491,   495,   217,   331,   500,   607,
     609,   504,   212,   519,   212,   516,   521,   548,   572,   590,
     217,   335,   558,   235,   561,   340,   549,   236,   602,   675,
      92,   761,     5,   614,   678,   349,   149,   769,   152,   680,
      98,   769,   686,   704,   175,   706,   720,   735,  -266,  -248,
     724,   754,   765,   736,    23,   176,   507,   628,   747,   303,
     685,    29,   238,  -139,    98,   240,   241,   575,   242,   243,
      92,   780,   440,   246,   380,   427,   674,  -266,   781,   677,
     253,  -248,  -139,   782,   733,   702,    98,   257,   258,   259,
    -248,   684,  -266,   835,    92,   822,   808,   494,   690,  -266,
     692,   153,   794,   695,   571,  -248,   698,   671,     0,   713,
     701,     0,   569,     0,   296,     0,    92,     0,  -266,     0,
       0,     0,   827,     0,     0,     0,   451,     0,     0,     0,
     182,   830,     0,     0,   379,   426,   715,  -266,   716,     0,
     719,     0,   723,   234,  -266,   725,     0,     0,   175,   473,
       0,     0,   175,     0,     0,     0,   827,   352,    23,   176,
       0,    98,    23,   176,     0,    29,   827,  -139,     0,    29,
       0,  -139,     0,   -39,     0,     0,     0,   -38,     0,     0,
     235,     0,     0,     0,   236,    98,  -139,     0,    98,     0,
    -139,    92,    98,     0,   690,   692,     0,   695,   698,   719,
     723,     0,     0,  -269,     0,     0,     0,   748,   749,     0,
     750,     0,   751,   752,     0,    92,   753,     0,    92,   177,
     178,     0,    92,   241,     0,   242,   243,     0,   767,   515,
     246,     0,  -269,   515,     0,     0,   773,   253,    98,     0,
     775,     0,     0,     0,   257,   258,   259,  -269,     0,     0,
       0,     0,     0,     0,  -269,     0,   796,     0,   748,   783,
     784,   751,   785,   786,   787,   788,     0,     0,    92,     0,
       0,   102,     0,  -269,     0,   190,   194,   217,     0,     0,
     102,   799,     0,     0,   800,     0,     0,     0,     0,   102,
       0,   805,  -269,     0,     0,     0,     0,     0,     0,  -269,
    -271,     0,   102,   580,   582,     0,     0,   585,   588,   818,
     819,   820,   821,     0,     0,   552,     0,     0,     0,   105,
     102,     0,     0,   723,     0,   831,   832,     0,   105,  -271,
       0,   818,   819,   820,   821,   831,   832,   105,   102,     0,
       0,     0,   102,     0,  -271,     0,     0,   836,   515,     0,
     105,  -271,     0,     0,    98,     0,     0,     0,   723,     0,
       0,   759,     0,     0,     0,     0,     0,     0,   105,     0,
    -271,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       7,     8,     9,    10,    92,     0,   105,   127,     0,  -271,
     105,   102,     0,     0,    98,     0,  -271,    98,     0,     0,
      21,   682,   683,     0,     0,     0,   128,   129,   130,     0,
      28,     0,   687,     0,     0,    31,    32,     0,    34,   797,
      35,    36,   696,     0,    92,   699,     0,    92,    38,     0,
      40,   102,     0,   672,   673,     0,     0,     0,     0,   105,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,   824,     0,     0,   102,     0,    98,     0,    98,
       0,     0,    98,     0,     0,    98,     0,     0,    98,     0,
       0,     0,     0,     0,     0,    59,     0,   102,     0,   105,
       0,     0,     0,     0,     0,     0,   824,    92,    98,    92,
       0,     0,    92,    64,     0,    92,   824,     0,    92,    98,
       0,    98,    14,   105,     0,    17,    98,    70,     0,   133,
       0,     0,     0,    22,     0,    23,   727,     0,    92,     0,
       0,     0,    29,     0,  -139,   105,     0,     0,     0,    92,
       0,    92,     0,     0,     0,     0,    92,   106,     0,     0,
      39,     0,   728,  -139,   729,    43,   106,     0,     0,   771,
       0,     0,   102,     0,     0,   106,   234,     0,    52,     0,
      53,     0,     0,     0,     0,     0,    98,     0,   106,     0,
       0,    98,     0,   234,     0,     0,   102,     0,     0,   102,
       0,     0,     0,   102,     0,     0,   106,     0,     0,    98,
       0,     0,     0,   235,     0,     0,    92,   236,    98,     0,
     105,    92,    98,     0,   106,     0,     0,     0,   106,    67,
     235,     0,     0,     0,   236,     0,     0,   730,     0,    92,
       0,     0,     0,     0,   105,     0,     0,   105,    92,   102,
       0,   105,    92,     0,     0,     0,   241,     0,   242,   243,
       0,     0,     0,   246,     0,     0,     0,     0,     0,   238,
     253,   239,   240,   241,     0,   242,   243,   106,     0,   259,
     246,     0,     0,   108,     0,     0,     0,   253,    98,    98,
      98,    98,   108,     0,   257,   258,   259,   105,     0,     0,
       0,   108,     0,     0,    98,    98,     0,     0,     0,     0,
       0,     0,     0,     0,   108,     0,     0,   106,    92,    92,
      92,    92,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,   108,     0,    92,    92,     0,     0,   234,     0,
       0,   106,    14,     0,     0,    17,     0,     0,     0,     0,
     108,     0,     0,    22,   108,    23,   727,     0,     0,     0,
       0,     0,    29,   106,  -139,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,   236,
      39,     0,   728,  -139,   729,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   121,     0,     0,    52,   237,
      53,     0,     0,   108,     0,   102,     0,     0,   102,     0,
       0,     0,     0,   105,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   106,   257,
     258,   259,     0,   108,     0,     0,     0,     0,     0,    67,
     623,     0,     0,   105,     0,     0,   105,   789,     0,     0,
       0,     0,   106,     0,     0,   106,     0,   108,   102,   106,
     102,     0,     0,   102,     0,     0,   102,     0,     0,   102,
       0,     0,   110,     0,     0,     0,     0,     0,     0,   108,
       0,   110,     0,     0,     0,     0,     0,     0,     0,   102,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,   102,   110,     0,   106,   105,   102,   105,     0,
       0,   105,     0,     0,   105,     0,     0,   105,     0,     0,
       0,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,   110,
       0,     0,     0,   110,     0,     0,    14,     0,   105,    17,
     105,     0,     0,     0,   108,   105,     0,    22,     0,    23,
     727,     0,     0,     0,     0,     0,    29,   102,  -139,     0,
       0,     0,   102,     0,     0,     0,     0,     0,   108,     0,
       0,   108,     0,     0,   219,   108,   728,  -139,   729,    43,
     102,     0,   110,     0,     0,     0,     0,     0,     0,   102,
       0,     0,    52,   102,    53,     0,     0,     0,     0,     0,
     111,     0,     0,     0,     0,   105,     0,     0,     0,   111,
     105,   106,     0,     0,     0,     0,     0,     0,   111,     0,
       0,   108,   110,     0,     0,     0,     0,     0,   105,     0,
       0,   111,     0,     0,     0,    74,     0,   105,     0,     0,
       0,   105,     0,    67,   119,     0,   110,     0,     0,   111,
       0,   106,     0,   146,   106,     0,     0,     0,     0,   102,
     102,   102,   102,     0,     0,     0,   168,   111,   110,     0,
       0,   111,     0,     0,     0,   102,   102,     0,     0,     0,
       0,     0,     0,     0,   193,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   234,
       0,     0,    74,     0,     0,     0,   220,   105,   105,   105,
     105,     0,     0,     0,   106,     0,   106,     0,     0,   106,
     111,     0,   106,   105,   105,   106,   187,   274,   275,  -354,
       0,   234,  -376,     0,  -376,     0,   235,   108,     0,     0,
     236,     0,     0,   110,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   281,   106,     0,   106,     0,
     111,     0,     0,   106,     0,     0,     0,   110,   235,     0,
     110,     0,   236,     0,   110,   238,     0,   108,     0,   241,
     108,   242,   243,     0,   111,     0,   246,     0,     0,     0,
       0,     0,     0,   253,     0,   317,     0,     0,     0,     0,
     257,   258,   259,     0,     0,   284,   111,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,     0,   246,   339,
     110,     0,     0,   106,   252,   253,     0,     0,   106,   256,
       0,     0,   257,   258,   259,     0,   187,   187,   187,   315,
     108,    74,   108,     0,     0,   108,   106,     0,   108,     0,
     187,   108,     0,     0,     0,   106,     0,     0,     0,   106,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,   108,     0,     0,     0,     0,     0,     0,   187,     0,
       0,   111,   108,     0,   108,     0,     0,     0,     0,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,   232,     0,   111,     0,
       0,   233,   111,     0,     0,     0,    74,     0,     0,   234,
       0,     0,     0,     0,     0,   106,   106,   106,   106,     0,
       0,     0,     0,     0,     0,     0,   110,     0,     0,     0,
     339,   106,   106,   339,     0,     0,     0,     0,     0,   108,
       0,     0,     0,     0,   108,     0,   235,     0,   111,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,   187,
       0,     0,   108,     0,     0,     0,   110,     0,     0,   110,
     237,   108,     0,     0,     0,   108,     0,     0,     0,     0,
       0,     0,   187,   514,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   234,     0,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,   681,     0,     0,     0,     0,     0,     0,     0,   110,
       0,   110,     0,     0,   110,     0,     0,   110,     0,     0,
     110,   108,   108,   108,   108,   235,     0,     0,     0,   236,
       0,     0,     0,     0,     0,     0,     0,   108,   108,     0,
     110,     0,     0,     0,   111,     0,     0,     0,     0,     0,
       0,   110,     0,   110,     0,     0,     0,     0,   110,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,     0,     0,     0,   246,     0,     0,     0,   592,
       0,   252,   253,     0,   111,     0,   256,   111,     0,   257,
     258,   259,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   187,   187,   110,   625,
     187,   187,    74,   110,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,     0,     0,     0,     0,     0,   111,     0,   111,
     110,     0,   111,     0,   110,   111,   125,     0,   111,     0,
       0,   125,     0,     0,     0,   148,   125,   125,     0,     0,
       0,     0,     0,   162,     0,   165,     0,     0,   111,     0,
       0,     0,   339,     0,   339,     0,     0,   339,     0,   111,
     339,   111,     0,   700,     0,   195,   111,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   712,   187,   187,     0,     0,     0,     0,
     110,   110,   110,   110,   718,   187,   722,   315,     0,     0,
       0,    74,   315,   222,     0,   187,   110,   110,   187,     0,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,   111,     0,     0,     0,
       0,   111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,     0,   195,     0,   111,     0,
     295,   235,   111,     0,     0,   236,   195,     0,   304,     0,
       0,     0,     0,     0,   295,     0,     0,   322,     0,     0,
       0,     0,     0,     0,   770,   237,     0,   295,     0,   125,
       0,     0,   334,   774,     0,     0,     0,   776,     0,     0,
     238,     0,   239,   240,   241,   295,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,     0,   257,   258,   259,   111,   111,
     111,   111,   187,     0,   366,     0,   804,     0,     0,   165,
     165,   165,   165,     0,   111,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   315,   315,
       0,   315,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   801,     0,     0,     0,   295,
     295,     0,   315,     0,   315,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,     0,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   801,     0,     0,
       0,     0,     0,     0,   322,     0,     0,     0,     0,     0,
       0,     0,     0,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   295,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -246,    18,    19,     0,    20,
      21,     0,    22,   165,    23,    24,    25,    26,    27,     0,
      28,    29,     0,  -139,    30,    31,    32,    33,    34,   -37,
      35,    36,     0,    37,     0,     0,     0,  -246,    38,    39,
      40,    41,  -139,    42,    43,     0,  -246,    44,    45,     0,
      46,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,  -246,     0,     0,    55,     0,    56,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
     295,    63,     0,    64,     0,    65,    66,     0,    67,     0,
       0,    68,     0,     0,     0,    69,     0,    70,     0,    71,
       0,     0,     0,   596,     0,     0,     0,   322,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   195,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,   667,
       5,     6,     7,     8,     9,    10,     0,  -393,     0,    11,
      12,    13,  -393,    14,    15,    16,    17,  -246,    18,    19,
    -393,    20,    21,  -393,    22,     0,    23,    24,    25,    26,
      27,     0,    28,    29,     0,  -139,    30,    31,    32,    33,
      34,   -37,    35,    36,     0,    37,   458,     0,     0,  -246,
      38,     0,    40,    41,  -139,    42,    43,  -393,  -246,    44,
      45,  -393,    46,    47,    48,    49,    50,    51,   322,    52,
       0,    53,    54,  -246,     0,     0,    55,     0,    56,     0,
       0,  -393,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -393,    59,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,     0,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,    64,  -393,  -393,  -393,   734,
      67,  -393,  -393,  -393,     0,     0,     0,    69,  -393,    70,
       0,    71,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     0,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -246,    18,    19,     0,    20,    21,
     125,    22,     0,    23,    24,    25,    26,    27,   779,    28,
      29,     0,  -139,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -246,    38,    39,    40,
      41,  -139,    42,    43,     0,  -246,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -246,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,   179,    67,     0,     0,
      68,     0,     0,     0,    69,   206,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -246,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -139,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -246,    38,
      39,    40,    41,  -139,    42,    43,     0,  -246,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -246,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,     0,    69,   351,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -246,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,     0,    28,    29,     0,  -139,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -246,    38,    39,    40,    41,  -139,    42,    43,     0,  -246,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -246,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,     0,    69,   206,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -246,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -139,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -246,    38,    39,    40,    41,  -139,    42,    43,
       0,  -246,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -246,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,     0,
      69,   627,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -246,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,     0,    28,    29,     0,
    -139,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -246,    38,    39,    40,    41,  -139,
      42,    43,     0,  -246,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -246,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,     0,    69,   726,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -246,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,     0,    28,
      29,     0,  -139,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -246,    38,   219,    40,
      41,  -139,    42,    43,     0,  -246,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -246,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -246,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
       0,    28,    29,     0,  -139,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -246,    38,
       0,    40,    41,  -139,    42,    43,     0,  -246,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -246,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -246,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   465,     0,    28,    29,     0,  -139,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -246,    38,     0,    40,    41,  -139,    42,    43,     0,  -246,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -246,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -246,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,   469,     0,    28,    29,     0,  -139,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -246,    38,     0,    40,    41,  -139,    42,    43,
       0,  -246,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -246,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -246,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   689,     0,    28,    29,     0,
    -139,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -246,    38,     0,    40,    41,  -139,
      42,    43,     0,  -246,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -246,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -246,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   691,     0,    28,
      29,     0,  -139,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -246,    38,     0,    40,
      41,  -139,    42,    43,     0,  -246,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -246,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,     0,    69,     0,    70,     4,    71,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -246,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,   694,
       0,    28,    29,     0,  -139,    30,    31,    32,    33,    34,
     -37,    35,    36,     0,    37,     0,     0,     0,  -246,    38,
       0,    40,    41,  -139,    42,    43,     0,  -246,    44,    45,
       0,    46,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,  -246,     0,     0,    55,     0,    56,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,    64,     0,    65,    66,     0,    67,
       0,     0,    68,     0,     0,     0,    69,     0,    70,     4,
      71,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -246,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,   697,     0,    28,    29,     0,  -139,    30,    31,    32,
      33,    34,   -37,    35,    36,     0,    37,     0,     0,     0,
    -246,    38,     0,    40,    41,  -139,    42,    43,     0,  -246,
      44,    45,     0,    46,    47,    48,    49,    50,    51,     0,
      52,     0,    53,    54,  -246,     0,     0,    55,     0,    56,
       0,     0,    57,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,    59,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,     0,    63,     0,    64,     0,    65,    66,
       0,    67,     0,     0,    68,     0,     0,     0,    69,     0,
      70,     4,    71,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -246,    18,    19,     0,   711,    21,     0,    22,     0,    23,
      24,    25,    26,    27,     0,    28,    29,     0,  -139,    30,
      31,    32,    33,    34,   -37,    35,    36,     0,    37,     0,
       0,     0,  -246,    38,     0,    40,    41,  -139,    42,    43,
       0,  -246,    44,    45,     0,    46,    47,    48,    49,    50,
      51,     0,    52,     0,    53,    54,  -246,     0,     0,    55,
       0,    56,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
      59,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,    64,     0,
      65,    66,     0,    67,     0,     0,    68,     0,     0,     0,
      69,     0,    70,     4,    71,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -246,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,   717,     0,    28,    29,     0,
    -139,    30,    31,    32,    33,    34,   -37,    35,    36,     0,
      37,     0,     0,     0,  -246,    38,     0,    40,    41,  -139,
      42,    43,     0,  -246,    44,    45,     0,    46,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,  -246,     0,
       0,    55,     0,    56,     0,     0,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,    59,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
      64,     0,    65,    66,     0,    67,     0,     0,    68,     0,
       0,     0,    69,     0,    70,     4,    71,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -246,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,   721,     0,    28,
      29,     0,  -139,    30,    31,    32,    33,    34,   -37,    35,
      36,     0,    37,     0,     0,     0,  -246,    38,     0,    40,
      41,  -139,    42,    43,     0,  -246,    44,    45,     0,    46,
      47,    48,    49,    50,    51,     0,    52,     0,    53,    54,
    -246,     0,     0,    55,     0,    56,     0,     0,    57,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,    64,     0,    65,    66,     0,    67,     0,     0,
      68,     0,     0,     0,    69,     0,    70,   486,    71,     5,
       6,     7,     8,     9,    10,     0,  -393,     0,   127,     0,
       0,  -393,     0,     0,     0,     0,     0,     0,     0,  -393,
       0,    21,     0,     0,     0,     0,     0,   128,   129,   130,
       0,    28,     0,     0,     0,     0,    31,    32,     0,    34,
       0,    35,    36,     0,     0,     0,     0,     0,     0,    38,
       0,    40,     0,     0,     0,     0,  -393,     0,     0,     0,
    -393,     0,     0,    48,    49,    50,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -264,
    -393,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -393,    59,  -393,  -393,  -393,
       0,  -393,  -393,  -393,     0,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,    64,  -393,  -393,  -393,     0,     0,
    -393,  -393,  -393,     0,     0,     0,   132,     0,    70,   486,
     501,     5,     6,     7,     8,     9,    10,     0,  -393,     0,
     127,     0,     0,  -393,     0,     0,     0,     0,     0,     0,
       0,  -393,     0,    21,     0,     0,     0,     0,     0,   128,
     129,   130,     0,    28,     0,     0,     0,     0,    31,    32,
       0,    34,     0,    35,    36,     0,     0,     0,     0,     0,
       0,    38,     0,    40,     0,     0,     0,     0,  -393,     0,
       0,     0,  -393,     0,     0,    48,    49,    50,   131,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -393,    59,  -393,
    -393,  -393,     0,  -393,  -393,  -393,     0,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,  -393,    64,  -393,  -393,  -393,
       0,     0,  -393,  -393,  -393,     0,     0,     0,   132,     0,
      70,   486,   501,     5,     6,     7,     8,     9,    10,     0,
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
      65,    66,     0,     0,     0,     0,    68,     0,     0,     0,
     132,     0,    70,   486,   487,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   127,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
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
       0,     0,   132,     0,    70,     0,   501,     5,     6,     7,
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
      63,     0,    64,     0,    65,    66,   179,     0,     0,     0,
      68,     0,     0,     0,   132,     0,    70,     0,   133,   795,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     130,   431,    28,     0,   432,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,   433,   434,
      38,     0,    40,     0,     0,     0,     0,     0,   435,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,     0,   132,     0,    70,
       0,   133,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,   210,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,   211,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,   179,     0,     0,     0,    68,     0,     0,     0,   132,
       0,    70,     0,   133,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,   210,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,   179,     0,     0,     0,    68,     0,     0,
       0,   132,     0,    70,   509,   133,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,   155,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,     0,   132,     0,    70,     0,   133,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,   210,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,   179,     0,     0,
       0,    68,     0,     0,     0,   132,     0,    70,     0,   133,
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
       0,   133,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,   183,     0,     0,    68,     0,     0,     0,   132,
       0,    70,     0,   133,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,    21,     0,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,     0,     0,     0,    68,     0,     0,
       0,    69,     0,    70,     0,   133,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,     0,   132,     0,    70,     0,   133,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   128,   129,   130,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,   608,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,     0,   132,     0,    70,     0,   133,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   755,     0,     0,     0,     0,     0,   128,   129,
     130,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   756,    49,    50,   757,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,   179,
       0,     0,     0,    68,     0,     0,     0,   132,     0,    70,
       0,   758,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   130,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,   179,     0,     0,     0,    68,     0,     0,     0,   132,
       0,    70,     0,   758,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   746,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,   179,     0,     0,     0,    68,     0,     0,
       0,   132,     0,    70,     0,   133,     5,     6,     7,     8,
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
       0,     0,     0,   132,     0,    70,     0,   133,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   128,   129,   557,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,     0,   132,     0,    70,     0,   133,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     560,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,     0,   132,     0,    70,
       0,   133,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   740,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,     0,     0,     0,    68,     0,     0,     0,   132,
       0,    70,     0,   133,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   741,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,    64,
       0,    65,    66,     0,     0,     0,     0,    68,     0,     0,
       0,   132,     0,    70,     0,   133,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   743,     0,    28,     0,
       0,     0,     0,    31,    32,     0,    34,     0,    35,    36,
       0,     0,     0,     0,     0,     0,    38,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    59,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    63,
       0,    64,     0,    65,    66,     0,     0,     0,     0,    68,
       0,     0,     0,   132,     0,    70,     0,   133,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   128,   129,   744,     0,
      28,     0,     0,     0,     0,    31,    32,     0,    34,     0,
      35,    36,     0,     0,     0,     0,     0,     0,    38,     0,
      40,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,   131,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,    64,     0,    65,    66,     0,     0,     0,
       0,    68,     0,     0,     0,   132,     0,    70,     0,   133,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   128,   129,
     745,     0,    28,     0,     0,     0,     0,    31,    32,     0,
      34,     0,    35,    36,     0,     0,     0,     0,     0,     0,
      38,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,   131,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    64,     0,    65,    66,     0,
       0,     0,     0,    68,     0,     0,     0,   132,     0,    70,
       0,   133,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     128,   129,   746,     0,    28,     0,     0,     0,     0,    31,
      32,     0,    34,     0,    35,    36,     0,     0,     0,     0,
       0,     0,    38,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,    50,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,    64,     0,    65,
      66,     0,     0,     0,     0,    68,     0,     0,     0,   132,
       0,    70,     0,   133,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   128,   129,   130,     0,    28,     0,     0,     0,
       0,    31,    32,     0,    34,     0,    35,    36,     0,     0,
       0,     0,     0,     0,    38,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    49,
      50,   131,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   127,     0,     0,     0,     0,
      58,    59,    60,    61,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   128,   129,   130,    63,    28,    64,
       0,     0,    66,    31,    32,     0,    34,    68,    35,    36,
       0,   132,     0,    70,     0,   133,    38,     5,    40,     0,
       0,     0,     0,     0,   631,     0,     0,     0,     0,   632,
      48,    49,    50,   131,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    59,     0,     0,     0,  -273,     0,     0,
       0,  -273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,     0,   179,     0,     0,     0,     0,
       0,     0,     0,   132,     0,    70,  -273,   758,     0,   633,
     634,   635,   636,   637,   638,     0,     0,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,     0,     0,   649,
       0,   234,     0,   650,   651,   652,   653,   654,   655,   656,
     657,   658,   659,   660,     0,   661,     0,   232,   662,   663,
     664,   665,   233,   666,     0,     0,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   232,   237,     0,     0,     0,   233,   235,     0,     0,
       0,   236,     0,     0,   234,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   237,   250,   251,   252,   253,     0,   254,   255,   256,
       0,     0,   257,   258,   259,     0,   238,     0,   239,   240,
     241,   235,   242,   243,   244,   236,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
       0,   257,   258,   259,     0,   237,     0,     0,     0,     0,
       0,     0,   839,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   232,
     254,   255,   256,     0,   233,   257,   258,   259,     0,     0,
       0,     0,   234,   310,   508,     0,     0,     0,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,   307,     0,   235,
       0,     0,     0,   236,     0,     0,     0,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
       0,     0,     0,   237,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   235,     0,     0,     0,   236,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   237,   254,   255,
     256,     0,     0,   257,   258,   259,     0,   235,     0,   283,
       0,   236,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   232,   254,   255,   256,     0,   233,   257,   258,   259,
       0,     0,     0,   283,   234,   587,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
       0,   250,   251,   252,   253,     0,   254,     0,   256,     0,
       0,   257,   258,   259,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,   121,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   235,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
     314,   283,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   237,     0,   234,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,     0,   254,   255,
     256,     0,   235,   257,   258,   259,   236,     0,     0,   283,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   581,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     235,   254,   255,   256,   236,     0,   257,   258,   259,     0,
       0,     0,   283,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,   237,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,     0,   235,   257,   258,   259,   236,     0,     0,
     283,   232,     0,     0,   688,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,   237,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   235,   254,   255,   256,   236,     0,   257,   258,   259,
       0,     0,   693,   283,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   237,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   235,   257,   258,   259,   236,     0,
       0,   283,   232,     0,     0,   790,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,   791,   283,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,     0,   283,   232,     0,     0,   792,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,   793,   283,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,   283,   232,     0,     0,   806,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,   807,   283,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,   283,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,   338,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,  -355,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   121,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   232,   254,   255,   256,
       0,   233,   257,   258,   259,     0,     0,     0,     0,   234,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   348,     0,     0,     0,     0,
     232,     0,     0,     0,     0,   233,   235,     0,     0,     0,
     236,     0,     0,   234,   442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   443,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,     0,
     257,   258,   259,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   232,   254,
     255,   256,     0,   233,   257,   258,   259,     0,     0,     0,
       0,   234,   444,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   235,     0,
       0,     0,   236,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   232,     0,     0,     0,
       0,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   235,     0,     0,     0,   236,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   237,   254,   255,   256,
       0,     0,   257,   258,   259,     0,   235,     0,     0,     0,
     236,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     237,   254,   255,   256,     0,   293,   257,   258,   259,     0,
       0,   232,     0,     0,     0,   238,   233,   239,   240,   241,
       0,   242,   243,   244,   234,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   337,
     257,   258,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,   235,     0,     0,     0,   236,     0,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,   235,   242,   243,   244,   236,
     245,   246,   247,   248,   249,   250,   251,   252,   253,     0,
     254,   255,   256,     0,   350,   257,   258,   259,   362,   237,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,   234,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,     0,   257,
     258,   259,     0,     0,   232,     0,     0,     0,     0,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,     0,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   532,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   533,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   534,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   535,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   536,   257,   258,   259,   233,   235,     0,     0,     0,
     236,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
     235,   242,   243,   244,   236,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   537,
     257,   258,   259,   233,   237,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   538,   257,   258,   259,   233,   235,     0,
       0,     0,   236,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,   235,   242,   243,   244,   236,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
     232,   539,   257,   258,   259,   233,   237,     0,     0,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   540,   257,   258,   259,   233,
     235,     0,     0,     0,   236,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,   235,   242,   243,   244,   236,   245,
     246,   247,   248,   249,   250,   251,   252,   253,     0,   254,
     255,   256,   232,   541,   257,   258,   259,   233,   237,     0,
       0,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   542,   257,   258,
     259,   233,   235,     0,     0,     0,   236,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,   235,   242,   243,   244,
     236,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,   232,   543,   257,   258,   259,   233,
     237,     0,     0,     0,     0,     0,     0,   234,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,   232,   544,
     257,   258,   259,   233,   235,     0,     0,     0,   236,     0,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,   235,   242,
     243,   244,   236,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,   232,   545,   257,   258,
     259,   233,   237,     0,     0,     0,     0,     0,     0,   234,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   546,   257,   258,   259,     0,   235,     0,     0,     0,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,   547,
     257,   258,   259,     0,     0,     0,     0,   232,     0,     0,
       0,     0,   233,     0,   121,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,   237,     0,     0,     0,     0,     0,   234,     0,     0,
     591,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     567,   257,   258,   259,   235,     0,     0,     0,   236,     0,
       0,     0,   232,   619,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   235,   254,   255,   256,   236,     0,   257,   258,
     259,     0,     0,     0,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   237,     0,   234,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   238,     0,   239,   240,   241,     0,   242,   243,   244,
       0,   245,   246,   247,   248,   249,   250,   251,   252,   253,
       0,   254,   255,   256,     0,   235,   257,   258,   259,   236,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
       0,     0,     0,     0,     0,     0,   234,   676,   626,   237,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   235,   254,   255,   256,   236,     0,   257,
     258,   259,     0,     0,     0,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   237,     0,   234,
     679,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   239,   240,   241,     0,   242,   243,
     244,     0,   245,   246,   247,   248,   249,   250,   251,   252,
     253,     0,   254,   255,   256,     0,   235,   257,   258,   259,
     236,     0,     0,     0,   232,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,     0,     0,   234,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   235,   254,   255,   256,   236,     0,
     257,   258,   259,     0,     0,   739,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   237,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   238,     0,   239,   240,   241,     0,   242,
     243,   244,     0,   245,   246,   247,   248,   249,   250,   251,
     252,   253,     0,   254,   255,   256,     0,   235,   257,   258,
     259,   236,     0,     0,     0,   232,   772,     0,   742,     0,
     233,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   235,   254,   255,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   237,
       0,   234,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,   245,   246,   247,   248,   249,   250,
     251,   252,   253,     0,   254,   255,   256,     0,   235,   257,
     258,   259,   236,     0,     0,     0,   232,     0,     0,   812,
       0,   233,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   235,   254,   255,   256,
     236,     0,   257,   258,   259,     0,     0,   813,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     237,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   238,     0,   239,   240,   241,
       0,   242,   243,   244,     0,   245,   246,   247,   248,   249,
     250,   251,   252,   253,     0,   254,   255,   256,     0,   235,
     257,   258,   259,   236,     0,     0,     0,   232,     0,     0,
     814,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,   815,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   237,     0,   234,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,   252,   253,     0,   254,   255,   256,     0,
     235,   257,   258,   259,   236,     0,     0,     0,   232,     0,
       0,   816,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,   239,   240,   241,     0,   242,   243,   244,     0,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   235,   254,
     255,   256,   236,     0,   257,   258,   259,     0,     0,   817,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   237,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   238,     0,   239,
     240,   241,     0,   242,   243,   244,     0,   245,   246,   247,
     248,   249,   250,   251,   252,   253,     0,   254,   255,   256,
       0,   235,   257,   258,   259,   236,     0,     0,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,   237,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     238,     0,   239,   240,   241,     0,   242,   243,   244,     0,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   555,
     254,   255,   256,   236,     0,   257,   258,   259,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     234,     0,     0,   237,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,     0,
     239,   240,   241,     0,   242,   243,   244,     0,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   235,   254,   255,
     256,   236,     0,   257,   258,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,     0,
       0,   237,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,     0,   239,   240,
     241,     0,   242,   243,   244,     0,   245,   246,   247,   248,
       0,   250,   251,   252,   253,   235,   254,     0,   256,   236,
       0,   257,   258,   259,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   238,     0,   239,   240,   241,     0,
     242,   243,   244,     0,     0,   246,   247,   248,     0,   250,
     251,   252,   253,     0,     0,     0,   256,     0,     0,   257,
     258,   259
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-731))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,    41,    42,   343,    16,    15,   321,    19,   143,
     159,    23,    44,   280,    25,    26,    27,   280,   161,   153,
       3,   629,    54,    34,    35,   605,    37,    38,   485,     1,
       1,   174,    34,    44,    45,    46,    47,    48,     3,    71,
      51,    34,     3,    54,    55,    56,     3,    24,    59,   192,
     669,    62,    63,    64,     1,    66,   104,     3,    69,    70,
      71,   226,   227,   228,   229,   795,     3,   216,     0,     3,
       1,    24,   629,    80,    25,   104,    24,    80,    34,    25,
      24,   104,   104,    20,   104,   133,   103,    34,    63,   106,
      73,    63,    63,   110,    72,    34,    61,    34,    34,   829,
      37,   133,   104,    34,   133,   766,    78,    78,    34,   131,
      50,   104,   135,   133,    51,    52,   127,   128,   129,   130,
     131,   132,   133,   326,    61,   705,    72,    80,   135,     7,
     139,   103,   103,   276,   277,    72,   147,    77,    85,   747,
     343,    80,   135,    63,   601,   132,    16,   808,   104,   132,
     161,   123,    83,   130,    85,   130,   103,   104,   130,   130,
     104,   780,   781,   174,   176,   104,    34,   132,   104,   132,
     484,   132,   103,   104,   731,   132,   123,   130,   104,   135,
       3,   192,   130,   104,     7,    85,   130,   104,   104,    59,
     747,   131,   123,    61,    38,   132,   135,    70,   341,   135,
     211,   829,    60,   123,    74,   370,    64,   208,   108,   135,
     130,   839,   133,    57,   794,    60,   133,   133,   132,    64,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,    69,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
      20,   104,    85,   305,   147,    23,   733,   104,   289,   104,
     132,   595,    13,   294,   235,   236,   553,    18,   161,   123,
     553,   104,    44,    20,   305,   108,   307,   308,   131,   310,
     311,   174,    52,   314,   123,    83,   133,    85,   133,   123,
     104,    61,    60,   104,   104,   132,    64,   134,   329,   192,
     133,   104,   471,   472,   335,    52,    76,   278,   279,   123,
     341,   129,   123,   123,    61,   367,   347,   348,   289,     3,
     123,   123,    85,     7,   355,    72,   357,   104,   359,   360,
       2,   362,   363,   104,   123,   103,   367,   308,   106,    11,
     311,   104,   110,   512,   513,   108,   123,    52,    20,     3,
     504,   710,   123,     7,    38,   714,    61,   132,    20,   134,
       2,    33,   355,   104,   357,   123,    20,    72,   104,    11,
     133,   123,    34,    57,   104,    37,   104,   132,    20,    51,
     123,   604,   123,   276,   277,   123,   130,   123,   359,    51,
      52,    33,   123,   123,   132,   123,   134,    69,    52,    61,
     132,    73,   134,    79,   130,   132,   106,    61,   132,    51,
      72,   442,   443,   444,   445,   446,   447,   448,    72,   132,
     449,   450,    76,   454,   455,    85,   457,    69,   123,     7,
     103,    73,   463,   123,   465,   487,    23,   133,   469,   104,
     123,    34,   335,   157,   158,   159,    26,   133,   341,   501,
     122,   104,   133,    83,    85,   133,   487,   171,   133,   490,
     491,   104,   355,   130,   357,   135,   130,   133,    85,    26,
     501,   104,   443,    60,   445,   189,   135,    64,   133,    24,
     122,   704,     3,   133,    24,   199,     7,   710,     9,   135,
     162,   714,   130,   103,    19,    61,   135,   106,    34,    20,
     135,   133,    78,   132,    29,    30,   348,   519,   686,   154,
     573,    36,    99,    38,   186,   102,   103,   462,   105,   106,
     162,   735,   553,   110,   555,   556,   557,    63,   735,   560,
     117,    52,    57,   736,   630,   593,   208,   124,   125,   126,
      61,   572,    78,   808,   186,   794,   778,   330,   579,    85,
     581,    72,   754,   584,   457,    76,   587,   553,    -1,   611,
     591,    -1,   455,    -1,   457,    -1,   208,    -1,   104,    -1,
      -1,    -1,   795,    -1,    -1,    -1,   290,    -1,    -1,    -1,
     611,   804,    -1,    -1,   555,   556,   617,   123,   619,    -1,
     621,    -1,   623,    23,   130,   626,    -1,    -1,    19,   313,
      -1,    -1,    19,    -1,    -1,    -1,   829,   628,    29,    30,
      -1,   283,    29,    30,    -1,    36,   839,    38,    -1,    36,
      -1,    38,    -1,    44,    -1,    -1,    -1,    44,    -1,    -1,
      60,    -1,    -1,    -1,    64,   307,    57,    -1,   310,    -1,
      57,   283,   314,    -1,   675,   676,    -1,   678,   679,   680,
     681,    -1,    -1,    34,    -1,    -1,    -1,   688,   689,    -1,
     691,    -1,   693,   694,    -1,   307,   697,    -1,   310,   728,
     729,    -1,   314,   103,    -1,   105,   106,    -1,   709,   710,
     110,    -1,    63,   714,    -1,    -1,   717,   117,   360,    -1,
     721,    -1,    -1,    -1,   124,   125,   126,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,   758,    -1,   739,   740,
     741,   742,   743,   744,   745,   746,    -1,    -1,   360,    -1,
      -1,     2,    -1,   104,    -1,   756,   757,   758,    -1,    -1,
      11,   762,    -1,    -1,   765,    -1,    -1,    -1,    -1,    20,
      -1,   772,   123,    -1,    -1,    -1,    -1,    -1,    -1,   130,
      34,    -1,    33,   467,   468,    -1,    -1,   471,   472,   790,
     791,   792,   793,    -1,    -1,   437,    -1,    -1,    -1,     2,
      51,    -1,    -1,   804,    -1,   806,   807,    -1,    11,    63,
      -1,   812,   813,   814,   815,   816,   817,    20,    69,    -1,
      -1,    -1,    73,    -1,    78,    -1,    -1,   828,   829,    -1,
      33,    85,    -1,    -1,   476,    -1,    -1,    -1,   839,    -1,
      -1,   704,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   476,    -1,    69,    12,    -1,   123,
      73,   122,    -1,    -1,   516,    -1,   130,   519,    -1,    -1,
      25,   565,   566,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,   576,    -1,    -1,    40,    41,    -1,    43,   762,
      45,    46,   586,    -1,   516,   589,    -1,   519,    53,    -1,
      55,   162,    -1,   555,   556,    -1,    -1,    -1,    -1,   122,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,   795,    -1,    -1,   186,    -1,   579,    -1,   581,
      -1,    -1,   584,    -1,    -1,   587,    -1,    -1,   590,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   208,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,   829,   579,   610,   581,
      -1,    -1,   584,   118,    -1,   587,   839,    -1,   590,   621,
      -1,   623,    16,   186,    -1,    19,   628,   132,    -1,   134,
      -1,    -1,    -1,    27,    -1,    29,    30,    -1,   610,    -1,
      -1,    -1,    36,    -1,    38,   208,    -1,    -1,    -1,   621,
      -1,   623,    -1,    -1,    -1,    -1,   628,     2,    -1,    -1,
      54,    -1,    56,    57,    58,    59,    11,    -1,    -1,   713,
      -1,    -1,   283,    -1,    -1,    20,    23,    -1,    72,    -1,
      74,    -1,    -1,    -1,    -1,    -1,   688,    -1,    33,    -1,
      -1,   693,    -1,    23,    -1,    -1,   307,    -1,    -1,   310,
      -1,    -1,    -1,   314,    -1,    -1,    51,    -1,    -1,   711,
      -1,    -1,    -1,    60,    -1,    -1,   688,    64,   720,    -1,
     283,   693,   724,    -1,    69,    -1,    -1,    -1,    73,   123,
      60,    -1,    -1,    -1,    64,    -1,    -1,   131,    -1,   711,
      -1,    -1,    -1,    -1,   307,    -1,    -1,   310,   720,   360,
      -1,   314,   724,    -1,    -1,    -1,   103,    -1,   105,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    99,
     117,   101,   102,   103,    -1,   105,   106,   122,    -1,   126,
     110,    -1,    -1,     2,    -1,    -1,    -1,   117,   790,   791,
     792,   793,    11,    -1,   124,   125,   126,   360,    -1,    -1,
      -1,    20,    -1,    -1,   806,   807,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,   162,   790,   791,
     792,   793,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    51,    -1,   806,   807,    -1,    -1,    23,    -1,
      -1,   186,    16,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    27,    73,    29,    30,    -1,    -1,    -1,
      -1,    -1,    36,   208,    38,   476,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    64,
      54,    -1,    56,    57,    58,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    72,    84,
      74,    -1,    -1,   122,    -1,   516,    -1,    -1,   519,    -1,
      -1,    -1,    -1,   476,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,   283,   124,
     125,   126,    -1,   162,    -1,    -1,    -1,    -1,    -1,   123,
     135,    -1,    -1,   516,    -1,    -1,   519,   131,    -1,    -1,
      -1,    -1,   307,    -1,    -1,   310,    -1,   186,   579,   314,
     581,    -1,    -1,   584,    -1,    -1,   587,    -1,    -1,   590,
      -1,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   610,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     621,    -1,   623,    33,    -1,   360,   579,   628,   581,    -1,
      -1,   584,    -1,    -1,   587,    -1,    -1,   590,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   610,    -1,    69,
      -1,    -1,    -1,    73,    -1,    -1,    16,    -1,   621,    19,
     623,    -1,    -1,    -1,   283,   628,    -1,    27,    -1,    29,
      30,    -1,    -1,    -1,    -1,    -1,    36,   688,    38,    -1,
      -1,    -1,   693,    -1,    -1,    -1,    -1,    -1,   307,    -1,
      -1,   310,    -1,    -1,    54,   314,    56,    57,    58,    59,
     711,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,   720,
      -1,    -1,    72,   724,    74,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,   688,    -1,    -1,    -1,    11,
     693,   476,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,
      -1,   360,   162,    -1,    -1,    -1,    -1,    -1,   711,    -1,
      -1,    33,    -1,    -1,    -1,     2,    -1,   720,    -1,    -1,
      -1,   724,    -1,   123,    11,    -1,   186,    -1,    -1,    51,
      -1,   516,    -1,    20,   519,    -1,    -1,    -1,    -1,   790,
     791,   792,   793,    -1,    -1,    -1,    33,    69,   208,    -1,
      -1,    73,    -1,    -1,    -1,   806,   807,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    23,
      -1,    -1,    69,    -1,    -1,    -1,    73,   790,   791,   792,
     793,    -1,    -1,    -1,   579,    -1,   581,    -1,    -1,   584,
     122,    -1,   587,   806,   807,   590,    47,   127,   128,   129,
      -1,    23,   132,    -1,   134,    -1,    60,   476,    -1,    -1,
      64,    -1,    -1,   283,    -1,   610,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   621,    -1,   623,    -1,
     162,    -1,    -1,   628,    -1,    -1,    -1,   307,    60,    -1,
     310,    -1,    64,    -1,   314,    99,    -1,   516,    -1,   103,
     519,   105,   106,    -1,   186,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,   117,    -1,   162,    -1,    -1,    -1,    -1,
     124,   125,   126,    -1,    -1,   126,   208,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,    -1,   110,   186,
     360,    -1,    -1,   688,   116,   117,    -1,    -1,   693,   121,
      -1,    -1,   124,   125,   126,    -1,   157,   158,   159,   160,
     579,   208,   581,    -1,    -1,   584,   711,    -1,   587,    -1,
     171,   590,    -1,    -1,    -1,   720,    -1,    -1,    -1,   724,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,   610,    -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,
      -1,   283,   621,    -1,   623,    -1,    -1,    -1,    -1,   628,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   307,    10,    -1,   310,    -1,
      -1,    15,   314,    -1,    -1,    -1,   283,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,   790,   791,   792,   793,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   476,    -1,    -1,    -1,
     307,   806,   807,   310,    -1,    -1,    -1,    -1,    -1,   688,
      -1,    -1,    -1,    -1,   693,    -1,    60,    -1,   360,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,    -1,   711,    -1,    -1,    -1,   516,    -1,    -1,   519,
      84,   720,    -1,    -1,    -1,   724,    -1,    -1,    -1,    -1,
      -1,    -1,   313,   360,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    23,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   579,
      -1,   581,    -1,    -1,   584,    -1,    -1,   587,    -1,    -1,
     590,   790,   791,   792,   793,    60,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   806,   807,    -1,
     610,    -1,    -1,    -1,   476,    -1,    -1,    -1,    -1,    -1,
      -1,   621,    -1,   623,    -1,    -1,    -1,    -1,   628,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,   476,
      -1,   116,   117,    -1,   516,    -1,   121,   519,    -1,   124,
     125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   467,   468,   688,   516,
     471,   472,   519,   693,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   711,    -1,    -1,    -1,    -1,    -1,   579,    -1,   581,
     720,    -1,   584,    -1,   724,   587,    13,    -1,   590,    -1,
      -1,    18,    -1,    -1,    -1,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    32,    -1,    -1,   610,    -1,
      -1,    -1,   579,    -1,   581,    -1,    -1,   584,    -1,   621,
     587,   623,    -1,   590,    -1,    52,   628,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   610,   565,   566,    -1,    -1,    -1,    -1,
     790,   791,   792,   793,   621,   576,   623,   578,    -1,    -1,
      -1,   628,   583,    90,    -1,   586,   806,   807,   589,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,   688,    -1,    -1,    -1,
      -1,   693,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   711,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,   720,    -1,
     147,    60,   724,    -1,    -1,    64,   153,    -1,   155,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,   711,    84,    -1,   174,    -1,   176,
      -1,    -1,   179,   720,    -1,    -1,    -1,   724,    -1,    -1,
      99,    -1,   101,   102,   103,   192,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    -1,   124,   125,   126,   790,   791,
     792,   793,   713,    -1,   221,    -1,   135,    -1,    -1,   226,
     227,   228,   229,    -1,   806,   807,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   749,   750,
      -1,   752,   753,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   766,    -1,    -1,    -1,   276,
     277,    -1,   773,    -1,   775,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   301,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   808,    -1,    -1,
      -1,    -1,    -1,    -1,   321,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   330,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   341,     0,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,   370,    29,    30,    31,    32,    33,    -1,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    57,    58,    59,    -1,    61,    62,    63,    -1,
      65,    66,    67,    68,    69,    70,    -1,    72,    -1,    74,
      75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     457,   116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,
      -1,    -1,    -1,   480,    -1,    -1,    -1,   484,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   504,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,   526,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    29,    30,    31,    32,
      33,    -1,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,   573,    -1,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,   595,    72,
      -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
     103,   104,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   646,
     123,   124,   125,   126,    -1,    -1,    -1,   130,   131,   132,
      -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
     727,    27,    -1,    29,    30,    31,    32,    33,   735,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,   122,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,   131,   132,     1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      54,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,     1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,   131,
     132,     1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,   131,   132,     1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,   130,   131,   132,     1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,     1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,     1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,     1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,     1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,     1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,     1,   134,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      -1,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    -1,    -1,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      -1,    65,    66,    67,    68,    69,    70,    -1,    72,    -1,
      74,    75,    76,    -1,    -1,    79,    -1,    81,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,     1,
     134,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    -1,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    -1,    61,
      62,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    -1,    74,    75,    76,    -1,    -1,    79,    -1,    81,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,
     132,     1,   134,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    -1,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      -1,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      -1,    61,    62,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    -1,    74,    75,    76,    -1,    -1,    79,
      -1,    81,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,
     120,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
     130,    -1,   132,     1,   134,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    -1,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      48,    -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,
      58,    59,    -1,    61,    62,    63,    -1,    65,    66,    67,
      68,    69,    70,    -1,    72,    -1,    74,    75,    76,    -1,
      -1,    79,    -1,    81,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,
     118,    -1,   120,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,   130,    -1,   132,     1,   134,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    -1,    -1,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    -1,    65,
      66,    67,    68,    69,    70,    -1,    72,    -1,    74,    75,
      76,    -1,    -1,    79,    -1,    81,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     116,    -1,   118,    -1,   120,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,     1,   134,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    12,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    -1,    -1,   130,    -1,   132,     1,
     134,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,    -1,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
      -1,    -1,   124,   125,   126,    -1,    -1,    -1,   130,    -1,
     132,     1,   134,     3,     4,     5,     6,     7,     8,    -1,
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
     130,    -1,   132,     1,   134,     3,     4,     5,     6,     7,
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
     116,    -1,   118,    -1,   120,   121,   122,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,   135,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    34,    35,    -1,    37,    -1,    -1,    40,    41,    -1,
      43,    -1,    45,    46,    -1,    -1,    -1,    -1,    51,    52,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    67,    68,    69,    70,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,
      -1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,    -1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,   132,   133,   134,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    68,    69,    70,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,    -1,   120,   121,   122,    -1,    -1,
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
      -1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,    -1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    24,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,   132,    -1,   134,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    -1,
      -1,    -1,    -1,    40,    41,    -1,    43,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,   118,    -1,   120,   121,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    43,    -1,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,    -1,
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
      -1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,    -1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,   122,    -1,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,   132,    -1,   134,     3,     4,     5,     6,
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
      -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,
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
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,
      -1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,    -1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
      -1,   120,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,   130,    -1,   132,    -1,   134,     3,     4,     5,     6,
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
      -1,    -1,    -1,   130,    -1,   132,    -1,   134,     3,     4,
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
      -1,    -1,    -1,   116,    -1,   118,    -1,   120,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,   130,    -1,   132,
      -1,   134,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    40,
      41,    -1,    43,    -1,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,   118,    -1,   120,
     121,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,   130,
      -1,   132,    -1,   134,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    -1,    -1,    -1,
      -1,    40,    41,    -1,    43,    -1,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      99,   100,   101,   102,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    33,   116,    35,   118,
      -1,    -1,   121,    40,    41,    -1,    43,   126,    45,    46,
      -1,   130,    -1,   132,    -1,   134,    53,     3,    55,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,   104,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,   132,   133,   134,    -1,    85,
      86,    87,    88,    89,    90,    -1,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,    -1,   105,
      -1,    23,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,    -1,    10,   124,   125,
     126,   127,    15,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    84,    -1,    -1,    -1,    15,    60,    -1,    -1,
      -1,    64,    -1,    -1,    23,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,    84,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,    -1,   124,   125,   126,    -1,    99,    -1,   101,   102,
     103,    60,   105,   106,   107,    64,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      -1,   124,   125,   126,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    10,
     119,   120,   121,    -1,    15,   124,   125,   126,    -1,    -1,
      -1,    -1,    23,    24,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    60,    -1,    -1,    -1,    64,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    84,   119,   120,
     121,    -1,    -1,   124,   125,   126,    -1,    60,    -1,   130,
      -1,    64,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    10,   119,   120,   121,    -1,    15,   124,   125,   126,
      -1,    -1,    -1,   130,    23,    24,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,   119,    -1,   121,    -1,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    60,
     119,   120,   121,    64,    -1,   124,   125,   126,    -1,    -1,
      71,   130,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    84,    -1,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,   119,   120,
     121,    -1,    60,   124,   125,   126,    64,    -1,    -1,   130,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    24,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      60,   119,   120,   121,    64,    -1,   124,   125,   126,    -1,
      -1,    -1,   130,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    -1,    60,   124,   125,   126,    64,    -1,    -1,
     130,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    60,   119,   120,   121,    64,    -1,   124,   125,   126,
      -1,    -1,    71,   130,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    -1,   105,   106,   107,    -1,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,    60,   124,   125,   126,    64,    -1,
      -1,   130,    10,    -1,    -1,    71,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    71,   130,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,    -1,   130,    10,    -1,    -1,    71,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,    71,   130,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,    -1,   130,    10,    -1,    -1,    71,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    71,   130,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,   130,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    10,   119,   120,   121,
      -1,    15,   124,   125,   126,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    24,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    -1,
     124,   125,   126,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    10,   119,
     120,   121,    -1,    15,   124,   125,   126,    -1,    -1,    -1,
      -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    60,    -1,    -1,    -1,    64,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    84,   119,   120,   121,
      -1,    -1,   124,   125,   126,    -1,    60,    -1,    -1,    -1,
      64,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      84,   119,   120,   121,    -1,   123,   124,   125,   126,    -1,
      -1,    10,    -1,    -1,    -1,    99,    15,   101,   102,   103,
      -1,   105,   106,   107,    23,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,   102,   103,    60,   105,   106,   107,    64,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,   126,    83,    84,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    -1,   124,
     125,   126,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,
      -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    60,   105,   106,   107,    64,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,    -1,   124,   125,   126,    15,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    60,    -1,    -1,    -1,
      64,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      60,   105,   106,   107,    64,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    60,    -1,
      -1,    -1,    64,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    60,   105,   106,   107,    64,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      10,   123,   124,   125,   126,    15,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      60,    -1,    -1,    -1,    64,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    60,   105,   106,   107,    64,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    -1,   119,
     120,   121,    10,   123,   124,   125,   126,    15,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    60,    -1,    -1,    -1,    64,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    60,   105,   106,   107,
      64,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    10,   123,   124,   125,   126,    15,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    10,   123,
     124,   125,   126,    15,    60,    -1,    -1,    -1,    64,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    60,   105,
     106,   107,    64,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    10,   123,   124,   125,
     126,    15,    84,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    -1,   119,   120,   121,
      -1,   123,   124,   125,   126,    -1,    60,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,   123,
     124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    80,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
     123,   124,   125,   126,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    60,   119,   120,   121,    64,    -1,   124,   125,
     126,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,    -1,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,   110,   111,   112,   113,   114,   115,   116,   117,
      -1,   119,   120,   121,    -1,    60,   124,   125,   126,    64,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    60,   119,   120,   121,    64,    -1,   124,
     125,   126,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,   106,
     107,    -1,   109,   110,   111,   112,   113,   114,   115,   116,
     117,    -1,   119,   120,   121,    -1,    60,   124,   125,   126,
      64,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    60,   119,   120,   121,    64,    -1,
     124,   125,   126,    -1,    -1,    71,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,   105,
     106,   107,    -1,   109,   110,   111,   112,   113,   114,   115,
     116,   117,    -1,   119,   120,   121,    -1,    60,   124,   125,
     126,    64,    -1,    -1,    -1,    10,    11,    -1,    71,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    60,   119,   120,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    84,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,   119,   120,   121,    -1,    60,   124,
     125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,    71,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,
     102,   103,    -1,   105,   106,   107,    -1,   109,   110,   111,
     112,   113,   114,   115,   116,   117,    60,   119,   120,   121,
      64,    -1,   124,   125,   126,    -1,    -1,    71,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,   110,   111,   112,   113,
     114,   115,   116,   117,    -1,   119,   120,   121,    -1,    60,
     124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,    -1,
      71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    71,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,    -1,   119,   120,   121,    -1,
      60,   124,   125,   126,    64,    -1,    -1,    -1,    10,    -1,
      -1,    71,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
      -1,   101,   102,   103,    -1,   105,   106,   107,    -1,   109,
     110,   111,   112,   113,   114,   115,   116,   117,    60,   119,
     120,   121,    64,    -1,   124,   125,   126,    -1,    -1,    71,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    84,    -1,    23,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,
     101,   102,   103,    -1,   105,   106,   107,    -1,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    60,   119,   120,
     121,    64,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,   101,   102,
     103,    -1,   105,   106,   107,    -1,   109,   110,   111,   112,
      -1,   114,   115,   116,   117,    60,   119,    -1,   121,    64,
      -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,    -1,   101,   102,   103,    -1,
     105,   106,   107,    -1,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,    -1,   124,
     125,   126
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
     132,   134,   142,   143,   144,   145,   146,   147,   150,   151,
     152,   154,   157,   158,   159,   160,   161,   163,   164,   167,
     168,   171,   174,   177,   178,   199,   202,   203,   221,   222,
     223,   224,   225,   226,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   242,   243,   244,   245,   246,   144,
     234,    80,   227,   228,   153,   154,   227,    12,    31,    32,
      33,    70,   130,   134,   174,   221,   225,   232,   233,   234,
     235,   237,   238,    72,   153,   234,   144,   132,   154,     7,
     153,   155,     9,    72,   155,    52,    82,   162,   234,   234,
     234,   132,   154,   175,   132,   154,   204,   205,   144,   234,
     234,   234,   234,     7,   132,    19,    30,   159,   159,   122,
     195,   215,   234,   123,   234,   234,    24,   147,   156,   234,
     234,    69,   132,   144,   234,   154,   200,   215,   234,   234,
     234,   234,   234,   234,   234,   234,   131,   142,   148,   215,
      73,   108,   195,   216,   217,   234,   215,   234,   241,    54,
     144,    44,   154,    38,    57,   191,    20,    52,    61,    76,
     123,   129,    10,    15,    23,    60,    64,    84,    99,   101,
     102,   103,   105,   106,   107,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   119,   120,   121,   124,   125,   126,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,   127,   128,   132,   134,    60,    64,
     132,   144,   123,   130,   147,   234,   234,   234,   215,    34,
     227,   200,   123,   123,    79,   154,   195,   218,   219,   220,
     234,   130,   200,   168,   154,   132,   156,    24,    34,   156,
      24,    34,   156,   228,    71,   147,   218,   144,   132,   185,
      73,   132,   154,   206,   207,     1,   103,   209,   210,    34,
     104,   156,   218,   155,   154,   104,   123,   123,   130,   144,
     156,   132,   218,    85,   201,   123,   123,    28,    49,   156,
     123,   131,   142,   131,   234,   104,   133,   104,   133,    34,
     135,   228,    83,   104,   135,     7,   154,   103,   169,   179,
      61,   204,   204,   204,   204,   234,   234,   234,   234,   162,
     234,   162,   234,   234,   234,   234,   234,   234,   234,    25,
      72,   154,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   218,   218,   162,   234,   162,   234,
      20,    34,    37,    51,    52,    61,    72,   189,   229,   230,
     234,   246,    24,    34,    24,    34,    71,    34,   135,   162,
     234,   156,   123,   234,    83,    85,   133,   104,   154,   172,
     173,   123,   154,    34,   215,    33,   234,   162,   234,    33,
     234,   162,   234,   156,   144,   234,    26,   133,   186,   187,
     188,   189,   176,   207,   104,   133,     1,   134,   211,   222,
      83,    85,   208,   234,   205,   133,   195,   234,   165,   218,
     133,   134,   211,   222,   104,   149,   234,   149,   133,   133,
     216,   216,   162,   234,   144,   234,   135,   234,   234,   130,
     215,   130,    52,    61,    72,   181,   190,   204,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   133,   135,
      34,    61,   221,   104,   133,    60,    60,    33,   162,   234,
      33,   162,   234,   162,   234,   227,   227,   123,   234,   195,
     234,   220,    85,   104,   131,   169,   234,   133,   234,    24,
     156,    24,   156,   234,    24,   156,   228,    24,   156,   228,
      26,    26,   144,   104,   133,   132,   154,    20,    52,    61,
      72,   192,   133,   207,   103,   210,   215,   234,    47,   234,
      50,    77,   131,   166,   133,   215,   200,   104,   123,    11,
     123,   135,   228,   135,   228,   144,    83,   131,   148,   170,
     180,    10,    15,    85,    86,    87,    88,    89,    90,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   105,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   121,   124,   125,   126,   127,   129,   154,   182,   183,
     123,   230,   221,   221,   234,    24,    24,   234,    24,    24,
     135,   135,   156,   156,   234,   173,   130,   156,    71,    33,
     234,    33,   234,    71,    33,   234,   156,    33,   234,   156,
     144,   234,   187,   207,   103,   214,    61,   210,   208,    34,
     135,    24,   144,   215,   135,   234,   234,    33,   144,   234,
     135,    33,   144,   234,   135,   234,   131,    30,    56,    58,
     131,   143,   158,   192,   154,   106,   132,   184,   184,    71,
      33,    33,    71,    33,    33,    33,    33,   170,   234,   234,
     234,   234,   234,   234,   133,    25,    67,    70,   134,   195,
     213,   222,   108,   197,   208,    78,   198,   234,   211,   222,
     144,   156,    11,   234,   144,   234,   144,   158,   210,   154,
     182,   183,   186,   234,   234,   234,   234,   234,   234,   131,
      71,    71,    71,    71,   214,   135,   215,   195,   196,   234,
     234,   147,   157,   194,   135,   234,    71,    71,   198,   184,
     184,   133,    71,    71,    71,    71,    71,    71,   234,   234,
     234,   234,   197,   208,   195,   212,   213,   222,    34,   135,
     222,   234,   234,   123,   193,   194,   234,   212,   213,   135,
     212
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
#line 418 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 423 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 424 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 431 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 436 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 437 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 453 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 454 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 455 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 457 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 458 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 459 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 460 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 461 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 463 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 464 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 465 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 466 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 467 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 472 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 474 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 495 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 496 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 501 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 502 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 507 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 512 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 517 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 521 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 527 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 528 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 529 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 533 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 537 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 538 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 539 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 542 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 544 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 545 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 547 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 548 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 551 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 556 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 566 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 596 "chapel.ypp"
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

  case 87:

/* Line 1806 of yacc.c  */
#line 610 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 611 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 613 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 615 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 627 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pcallexpr),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 651 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 658 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pcallexpr),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 671 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 672 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 673 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 674 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 683 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 684 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 712 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 717 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 722 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 736 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 738 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 740 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 745 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 755 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 764 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 768 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 775 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 776 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 781 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 786 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 791 "chapel.ypp"
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

  case 139:

/* Line 1806 of yacc.c  */
#line 811 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 817 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 824 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 832 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 843 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 849 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 856 "chapel.ypp"
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

  case 146:

/* Line 1806 of yacc.c  */
#line 878 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 882 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 887 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 899 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 900 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 901 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 903 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 905 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 906 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 907 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 908 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 909 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 910 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 911 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 912 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 913 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 914 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 915 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 916 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 920 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 921 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 922 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 923 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 927 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 928 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 929 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 931 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 933 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 937 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 938 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 951 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 962 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 964 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 969 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 977 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 978 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 979 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 980 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 985 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 986 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 992 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 993 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 997 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 998 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 999 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1001 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1002 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1012 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1019 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1023 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1061 "chapel.ypp"
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

  case 238:

/* Line 1806 of yacc.c  */
#line 1077 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1079 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1081 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1086 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1094 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1110 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1119 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1128 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1129 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1130 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1136 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1144 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1146 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1152 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1157 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1159 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1161 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1176 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1177 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1178 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1182 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1184 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1190 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1191 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1212 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1220 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1227 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1231 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1260 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1269 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1284 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1294 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1295 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1296 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1302 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1321 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1323 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1329 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1331 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1336 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1340 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1342 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1344 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1346 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1350 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1352 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1358 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1360 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1367 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1373 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1379 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1385 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1394 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1398 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1414 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1418 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1423 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1428 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pIntentExpr).first, (yyvsp[(1) - (1)].pIntentExpr).second); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1430 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).first);
      (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pIntentExpr).second);
    }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    {
      ArgSymbol* tiMark = tiMarkForIntent((yyvsp[(1) - (2)].pt));
      if (!tiMark) {
        USR_FATAL_CONT((yyvsp[(2) - (2)].pexpr), "%s is not supported in a 'with' clause",
                           intentDescrString((yyvsp[(1) - (2)].pt)));
        tiMark = tiMarkForIntent(INTENT_IN); //dummy, so parser can continue
      }
      (yyval.pIntentExpr).first = new SymExpr(tiMark);
      (yyval.pIntentExpr).second = (yyvsp[(2) - (2)].pexpr);
    }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1448 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1449 "chapel.ypp"
    { (yyval.pIntentExpr).first = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).second = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1455 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1460 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1465 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1481 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1483 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1485 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1487 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1489 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1514 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1515 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1516 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1520 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1521 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1522 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1530 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1531 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1532 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1536 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1537 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1538 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1539 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1540 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1541 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1542 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1544 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1550 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1556 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1557 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1558 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1559 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1560 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1561 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1562 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1563 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1564 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1565 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1566 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1567 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1568 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1569 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1570 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1571 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1572 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1573 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1574 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1575 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1576 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1577 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1581 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1582 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1583 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1590 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1591 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1592 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1593 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1598 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1599 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1600 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1610 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1611 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7964 "bison-chapel.cpp"
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



