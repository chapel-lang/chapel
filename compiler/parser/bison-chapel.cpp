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
#line 32 "chapel.ypp"

  #include <string>
  extern int         captureTokens;
  extern std::string captureString;


/* Line 288 of yacc.c  */
#line 45 "chapel.ypp"

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
#line 65 "chapel.ypp"

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
#line 135 "chapel.ypp"

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
#line 157 "chapel.ypp"

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
#line 239 "bison-chapel.cpp"

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
     TFORWARDING = 288,
     TIF = 289,
     TIN = 290,
     TINDEX = 291,
     TINLINE = 292,
     TINOUT = 293,
     TITER = 294,
     TLABEL = 295,
     TLAMBDA = 296,
     TLET = 297,
     TLOCAL = 298,
     TMINUSMINUS = 299,
     TMODULE = 300,
     TNEW = 301,
     TNIL = 302,
     TNOINIT = 303,
     TON = 304,
     TONLY = 305,
     TOTHERWISE = 306,
     TOUT = 307,
     TPARAM = 308,
     TPLUSPLUS = 309,
     TPRAGMA = 310,
     TPRIMITIVE = 311,
     TPRIVATE = 312,
     TPROC = 313,
     TPUBLIC = 314,
     TRECORD = 315,
     TREDUCE = 316,
     TREF = 317,
     TREQUIRE = 318,
     TRETURN = 319,
     TSCAN = 320,
     TSELECT = 321,
     TSERIAL = 322,
     TSINGLE = 323,
     TSPARSE = 324,
     TSUBDOMAIN = 325,
     TSYNC = 326,
     TTHEN = 327,
     TTHROW = 328,
     TTHROWS = 329,
     TTRY = 330,
     TTRYBANG = 331,
     TTYPE = 332,
     TUNDERSCORE = 333,
     TUNION = 334,
     TUSE = 335,
     TVAR = 336,
     TWHEN = 337,
     TWHERE = 338,
     TWHILE = 339,
     TWITH = 340,
     TYIELD = 341,
     TZIP = 342,
     TALIAS = 343,
     TAND = 344,
     TASSIGN = 345,
     TASSIGNBAND = 346,
     TASSIGNBOR = 347,
     TASSIGNBXOR = 348,
     TASSIGNDIVIDE = 349,
     TASSIGNEXP = 350,
     TASSIGNLAND = 351,
     TASSIGNLOR = 352,
     TASSIGNMINUS = 353,
     TASSIGNMOD = 354,
     TASSIGNMULTIPLY = 355,
     TASSIGNPLUS = 356,
     TASSIGNSL = 357,
     TASSIGNSR = 358,
     TBAND = 359,
     TBNOT = 360,
     TBOR = 361,
     TBXOR = 362,
     TCOLON = 363,
     TCOMMA = 364,
     TDIVIDE = 365,
     TDOT = 366,
     TDOTDOT = 367,
     TDOTDOTDOT = 368,
     TEQUAL = 369,
     TEXP = 370,
     TGREATER = 371,
     TGREATEREQUAL = 372,
     THASH = 373,
     TLESS = 374,
     TLESSEQUAL = 375,
     TMINUS = 376,
     TMOD = 377,
     TNOT = 378,
     TNOTEQUAL = 379,
     TOR = 380,
     TPLUS = 381,
     TQUESTION = 382,
     TSEMI = 383,
     TSHIFTLEFT = 384,
     TSHIFTRIGHT = 385,
     TSTAR = 386,
     TSWAP = 387,
     TASSIGNREDUCE = 388,
     TIO = 389,
     TLCBR = 390,
     TRCBR = 391,
     TLP = 392,
     TRP = 393,
     TLSBR = 394,
     TRSBR = 395,
     TNOELSE = 396,
     TUMINUS = 397,
     TUPLUS = 398
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
#line 188 "chapel.ypp"

  extern int yydebug;

  void yyerror(YYLTYPE*       ignored,
               ParserContext* context,
               const char*    str);



/* Line 340 of yacc.c  */
#line 450 "bison-chapel.cpp"

/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 456 "bison-chapel.cpp"
/* Unqualified %code blocks.  */

/* Line 344 of yacc.c  */
#line 38 "chapel.ypp"

  #include <string>
  int         captureTokens;
  std::string captureString;


/* Line 344 of yacc.c  */
#line 196 "chapel.ypp"

  #include "build.h"
  #include "DoWhileStmt.h"
  #include "driver.h"
  #include "flex-chapel.h"
  #include "ForLoop.h"
  #include "parser.h"
  #include "stmt.h"
  #include "stringutil.h"
  #include "TryStmt.h"
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
#line 532 "bison-chapel.cpp"

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
#define YYLAST   13529

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  144
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  117
/* YYNRULES -- Number of rules.  */
#define YYNRULES  467
/* YYNRULES -- Number of states.  */
#define YYNSTATES  887

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   398

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
     135,   136,   137,   138,   139,   140,   141,   142,   143
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    45,    47,    50,    53,    57,    61,    65,    69,    73,
      77,    80,    84,    88,    91,    94,    98,   100,   106,   113,
     114,   116,   118,   121,   125,   127,   130,   132,   136,   140,
     146,   147,   149,   151,   153,   157,   163,   169,   173,   178,
     183,   188,   193,   198,   203,   208,   213,   218,   223,   228,
     233,   238,   243,   248,   253,   254,   256,   258,   260,   262,
     265,   267,   270,   274,   276,   278,   281,   284,   286,   288,
     290,   292,   294,   296,   300,   306,   312,   315,   318,   324,
     328,   335,   342,   347,   353,   359,   363,   367,   374,   380,
     387,   393,   400,   404,   409,   416,   424,   431,   439,   444,
     450,   455,   460,   464,   471,   477,   480,   483,   487,   493,
     494,   497,   501,   504,   508,   515,   524,   526,   528,   530,
     531,   534,   535,   538,   542,   548,   550,   553,   557,   559,
     563,   564,   565,   574,   575,   577,   580,   583,   584,   585,
     596,   600,   604,   610,   616,   618,   622,   624,   627,   629,
     631,   633,   635,   637,   639,   641,   643,   645,   647,   649,
     651,   653,   655,   657,   659,   661,   663,   665,   667,   669,
     671,   673,   675,   677,   679,   681,   683,   685,   687,   689,
     691,   693,   695,   697,   699,   700,   704,   708,   709,   711,
     715,   720,   725,   732,   739,   740,   742,   744,   746,   748,
     750,   753,   756,   758,   760,   762,   763,   765,   767,   770,
     772,   774,   776,   778,   779,   781,   784,   786,   788,   790,
     791,   793,   795,   797,   799,   801,   804,   806,   807,   809,
     812,   815,   816,   819,   823,   828,   833,   836,   841,   842,
     845,   848,   853,   858,   863,   869,   874,   875,   877,   879,
     881,   885,   889,   894,   900,   902,   904,   908,   910,   913,
     917,   918,   921,   924,   925,   930,   931,   934,   937,   939,
     944,   949,   956,   958,   959,   961,   963,   967,   972,   976,
     981,   988,   989,   992,   995,   998,  1001,  1004,  1007,  1009,
    1011,  1015,  1019,  1021,  1023,  1025,  1029,  1033,  1034,  1036,
    1038,  1042,  1046,  1050,  1054,  1056,  1058,  1060,  1062,  1064,
    1066,  1068,  1070,  1073,  1078,  1083,  1088,  1094,  1097,  1100,
    1107,  1114,  1119,  1129,  1139,  1147,  1154,  1161,  1166,  1176,
    1186,  1194,  1199,  1206,  1213,  1223,  1233,  1240,  1242,  1244,
    1246,  1248,  1250,  1252,  1254,  1256,  1260,  1261,  1263,  1268,
    1270,  1274,  1279,  1281,  1285,  1288,  1292,  1296,  1298,  1302,
    1305,  1310,  1312,  1314,  1316,  1318,  1320,  1322,  1324,  1326,
    1331,  1335,  1339,  1342,  1345,  1347,  1349,  1351,  1353,  1355,
    1357,  1359,  1364,  1369,  1374,  1378,  1382,  1386,  1390,  1395,
    1399,  1404,  1406,  1408,  1410,  1412,  1414,  1418,  1423,  1427,
    1432,  1436,  1441,  1445,  1451,  1455,  1459,  1463,  1467,  1471,
    1475,  1479,  1483,  1487,  1491,  1495,  1499,  1503,  1507,  1511,
    1515,  1519,  1523,  1527,  1531,  1535,  1539,  1543,  1546,  1549,
    1552,  1555,  1558,  1561,  1565,  1569,  1573,  1577,  1581,  1585,
    1589,  1593,  1595,  1597,  1599,  1601,  1603,  1605
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     145,     0,    -1,   146,    -1,    -1,   146,   147,    -1,   149,
      -1,   148,   149,    -1,    55,     7,    -1,   148,    55,     7,
      -1,   150,    -1,   152,    -1,   157,    -1,   158,    -1,   165,
      -1,   159,    -1,   168,    -1,   171,    -1,   169,    -1,   174,
      -1,   172,    -1,   173,    -1,   164,    -1,   238,   128,    -1,
      12,   149,    -1,    13,   239,   149,    -1,    14,   160,   128,
      -1,    17,   239,   152,    -1,    21,   160,   128,    -1,    22,
     248,   128,    -1,    40,   161,   149,    -1,    43,   149,    -1,
      49,   248,   163,    -1,    67,   248,   163,    -1,    67,   163,
      -1,    71,   149,    -1,    86,   248,   128,    -1,     1,    -1,
     151,    45,   161,   135,   136,    -1,   151,    45,   161,   135,
     153,   136,    -1,    -1,    59,    -1,    57,    -1,   135,   136,
      -1,   135,   153,   136,    -1,   147,    -1,   153,   147,    -1,
     248,    -1,   248,    11,   248,    -1,   154,   109,   248,    -1,
     154,   109,   248,    11,   248,    -1,    -1,   154,    -1,   131,
      -1,   154,    -1,    80,   227,   128,    -1,    80,   248,    28,
     156,   128,    -1,    80,   248,    50,   155,   128,    -1,    63,
     227,   128,    -1,   249,    90,   248,   128,    -1,   249,   101,
     248,   128,    -1,   249,    98,   248,   128,    -1,   249,   100,
     248,   128,    -1,   249,    94,   248,   128,    -1,   249,    99,
     248,   128,    -1,   249,    95,   248,   128,    -1,   249,    91,
     248,   128,    -1,   249,    92,   248,   128,    -1,   249,    93,
     248,   128,    -1,   249,   103,   248,   128,    -1,   249,   102,
     248,   128,    -1,   249,   132,   248,   128,    -1,   249,   133,
     248,   128,    -1,   249,    96,   248,   128,    -1,   249,    97,
     248,   128,    -1,    -1,   161,    -1,     3,    -1,   160,    -1,
       7,    -1,    24,   149,    -1,   152,    -1,    64,   128,    -1,
      64,   248,   128,    -1,   128,    -1,   166,    -1,    59,   166,
      -1,    57,   166,    -1,   181,    -1,   211,    -1,   177,    -1,
     167,    -1,   188,    -1,   214,    -1,    33,   248,   128,    -1,
      33,   248,    28,   156,   128,    -1,    33,   248,    50,   155,
     128,    -1,    33,   214,    -1,    30,     9,    -1,    24,   149,
      84,   248,   128,    -1,    84,   248,   163,    -1,    18,   248,
      35,   248,   239,   163,    -1,    18,   248,    35,   170,   239,
     163,    -1,    18,   248,   239,   163,    -1,    31,   248,    35,
     248,   163,    -1,    31,   248,    35,   170,   163,    -1,    31,
     248,   163,    -1,    31,   170,   163,    -1,    31,    53,   161,
      35,   248,   163,    -1,    32,   248,    35,   248,   163,    -1,
      32,   248,    35,   248,   242,   163,    -1,    32,   248,    35,
     170,   163,    -1,    32,   248,    35,   170,   242,   163,    -1,
      32,   248,   163,    -1,    32,   248,   242,   163,    -1,   139,
     227,    35,   248,   140,   149,    -1,   139,   227,    35,   248,
     242,   140,   149,    -1,   139,   227,    35,   170,   140,   149,
      -1,   139,   227,    35,   170,   242,   140,   149,    -1,   139,
     227,   140,   149,    -1,   139,   227,   242,   140,   149,    -1,
      87,   137,   227,   138,    -1,    34,   248,    72,   149,    -1,
      34,   248,   152,    -1,    34,   248,    72,   149,    26,   149,
      -1,    34,   248,   152,    26,   149,    -1,    75,   149,    -1,
      76,   149,    -1,    73,   248,   128,    -1,    66,   248,   135,
     175,   136,    -1,    -1,   175,   176,    -1,    82,   227,   163,
      -1,    51,   149,    -1,    51,    24,   149,    -1,   178,   161,
     179,   135,   180,   136,    -1,    30,   162,   178,   161,   179,
     135,   180,   136,    -1,    16,    -1,    60,    -1,    79,    -1,
      -1,   108,   227,    -1,    -1,   180,   165,    -1,   180,   148,
     165,    -1,    27,   161,   135,   182,   136,    -1,   183,    -1,
     182,   109,    -1,   182,   109,   183,    -1,   161,    -1,   161,
      90,   248,    -1,    -1,    -1,    41,   185,   196,   186,   203,
     222,   210,   206,    -1,    -1,    37,    -1,    29,   162,    -1,
      30,   162,    -1,    -1,    -1,   187,   202,   189,   191,   190,
     203,   222,   204,   210,   205,    -1,   201,   193,   195,    -1,
     201,   194,   195,    -1,   201,   192,   111,   193,   195,    -1,
     201,   192,   111,   194,   195,    -1,   233,    -1,   137,   248,
     138,    -1,   161,    -1,   105,   161,    -1,   104,    -1,   106,
      -1,   107,    -1,   105,    -1,   114,    -1,   124,    -1,   120,
      -1,   117,    -1,   119,    -1,   116,    -1,   126,    -1,   121,
      -1,   131,    -1,   110,    -1,   129,    -1,   130,    -1,   122,
      -1,   115,    -1,   123,    -1,    15,    -1,   118,    -1,    10,
      -1,   132,    -1,   134,    -1,    90,    -1,   101,    -1,    98,
      -1,   100,    -1,    94,    -1,    99,    -1,    95,    -1,    91,
      -1,    92,    -1,    93,    -1,   103,    -1,   102,    -1,    -1,
     137,   197,   138,    -1,   137,   197,   138,    -1,    -1,   198,
      -1,   197,   109,   198,    -1,   199,   161,   226,   220,    -1,
     199,   161,   226,   209,    -1,   199,   137,   219,   138,   226,
     220,    -1,   199,   137,   219,   138,   226,   209,    -1,    -1,
     200,    -1,    35,    -1,    38,    -1,    52,    -1,    20,    -1,
      20,    35,    -1,    20,    62,    -1,    53,    -1,    62,    -1,
      77,    -1,    -1,    53,    -1,    62,    -1,    20,    62,    -1,
      20,    -1,    77,    -1,    58,    -1,    39,    -1,    -1,    20,
      -1,    20,    62,    -1,    62,    -1,    53,    -1,    77,    -1,
      -1,    74,    -1,   128,    -1,   206,    -1,   152,    -1,   164,
      -1,   127,   161,    -1,   127,    -1,    -1,   207,    -1,   113,
     248,    -1,   113,   208,    -1,    -1,    83,   248,    -1,    77,
     212,   128,    -1,    19,    77,   212,   128,    -1,    30,    77,
     212,   128,    -1,   161,   213,    -1,   161,   213,   109,   212,
      -1,    -1,    90,   234,    -1,    90,   223,    -1,   215,    53,
     216,   128,    -1,   215,    20,   216,   128,    -1,   215,    62,
     216,   128,    -1,   215,    20,    62,   216,   128,    -1,   215,
      81,   216,   128,    -1,    -1,    19,    -1,    30,    -1,   217,
      -1,   216,   109,   217,    -1,   161,   222,   220,    -1,   161,
     221,    88,   248,    -1,   137,   219,   138,   222,   220,    -1,
      78,    -1,   161,    -1,   137,   219,   138,    -1,   218,    -1,
     218,   109,    -1,   218,   109,   219,    -1,    -1,    90,    48,
      -1,    90,   248,    -1,    -1,   108,   139,   227,   140,    -1,
      -1,   108,   234,    -1,   108,   223,    -1,     1,    -1,   139,
     227,   140,   234,    -1,   139,   227,   140,   223,    -1,   139,
     227,    35,   248,   140,   234,    -1,     1,    -1,    -1,   234,
      -1,   207,    -1,   139,   140,   224,    -1,   139,   227,   140,
     224,    -1,   139,   140,   225,    -1,   139,   227,   140,   225,
      -1,   139,   227,    35,   248,   140,   224,    -1,    -1,   108,
     234,    -1,   108,   207,    -1,   108,    25,    -1,   108,    68,
      -1,   108,    71,    -1,   108,   225,    -1,   248,    -1,   207,
      -1,   227,   109,   248,    -1,   227,   109,   207,    -1,    78,
      -1,   248,    -1,   207,    -1,   228,   109,   228,    -1,   229,
     109,   228,    -1,    -1,   231,    -1,   232,    -1,   231,   109,
     232,    -1,   161,    90,   207,    -1,   161,    90,   248,    -1,
     161,    88,   248,    -1,   207,    -1,   248,    -1,   161,    -1,
     237,    -1,   249,    -1,   236,    -1,   257,    -1,   256,    -1,
      68,   248,    -1,    36,   137,   230,   138,    -1,    25,   137,
     230,   138,    -1,    70,   137,   230,   138,    -1,    69,    70,
     137,   230,   138,    -1,    12,   248,    -1,    71,   248,    -1,
      31,   248,    35,   248,    24,   248,    -1,    31,   248,    35,
     170,    24,   248,    -1,    31,   248,    24,   248,    -1,    31,
     248,    35,   248,    24,    34,   248,    72,   248,    -1,    31,
     248,    35,   170,    24,    34,   248,    72,   248,    -1,    31,
     248,    24,    34,   248,    72,   248,    -1,    32,   248,    35,
     248,    24,   248,    -1,    32,   248,    35,   170,    24,   248,
      -1,    32,   248,    24,   248,    -1,    32,   248,    35,   248,
      24,    34,   248,    72,   248,    -1,    32,   248,    35,   170,
      24,    34,   248,    72,   248,    -1,    32,   248,    24,    34,
     248,    72,   248,    -1,   139,   227,   140,   248,    -1,   139,
     227,    35,   248,   140,   248,    -1,   139,   227,    35,   170,
     140,   248,    -1,   139,   227,    35,   248,   140,    34,   248,
      72,   248,    -1,   139,   227,    35,   170,   140,    34,   248,
      72,   248,    -1,    34,   248,    72,   248,    26,   248,    -1,
      47,    -1,   237,    -1,   233,    -1,   252,    -1,   251,    -1,
     184,    -1,   246,    -1,   247,    -1,   245,   134,   248,    -1,
      -1,   240,    -1,    85,   137,   241,   138,    -1,   244,    -1,
     241,   109,   244,    -1,    85,   137,   243,   138,    -1,   244,
      -1,   243,   109,   244,    -1,   200,   233,    -1,   248,    61,
     233,    -1,   260,    61,   233,    -1,   249,    -1,   245,   134,
     248,    -1,    46,   248,    -1,    42,   216,    35,   248,    -1,
     254,    -1,   234,    -1,   235,    -1,   258,    -1,   259,    -1,
     184,    -1,   246,    -1,   247,    -1,   137,   113,   248,   138,
      -1,   248,   108,   248,    -1,   248,   112,   248,    -1,   248,
     112,    -1,   112,   248,    -1,   112,    -1,   233,    -1,   251,
      -1,   252,    -1,   253,    -1,   249,    -1,   184,    -1,   250,
     137,   230,   138,    -1,   250,   139,   230,   140,    -1,    56,
     137,   230,   138,    -1,   248,   111,   161,    -1,   248,   111,
      77,    -1,   248,   111,    25,    -1,   137,   228,   138,    -1,
     137,   228,   109,   138,    -1,   137,   229,   138,    -1,   137,
     229,   109,   138,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,   135,   227,   136,    -1,   135,   227,
     109,   136,    -1,   139,   227,   140,    -1,   139,   227,   109,
     140,    -1,   139,   255,   140,    -1,   139,   255,   109,   140,
      -1,   248,    88,   248,    -1,   255,   109,   248,    88,   248,
      -1,   248,   126,   248,    -1,   248,   121,   248,    -1,   248,
     131,   248,    -1,   248,   110,   248,    -1,   248,   129,   248,
      -1,   248,   130,   248,    -1,   248,   122,   248,    -1,   248,
     114,   248,    -1,   248,   124,   248,    -1,   248,   120,   248,
      -1,   248,   117,   248,    -1,   248,   119,   248,    -1,   248,
     116,   248,    -1,   248,   104,   248,    -1,   248,   106,   248,
      -1,   248,   107,   248,    -1,   248,    89,   248,    -1,   248,
     125,   248,    -1,   248,   115,   248,    -1,   248,    15,   248,
      -1,   248,    10,   248,    -1,   248,   118,   248,    -1,   248,
      23,   248,    -1,   126,   248,    -1,   121,   248,    -1,    44,
     248,    -1,    54,   248,    -1,   123,   248,    -1,   105,   248,
      -1,   248,    61,   248,    -1,   248,    61,   170,    -1,   260,
      61,   248,    -1,   260,    61,   170,    -1,   248,    65,   248,
      -1,   248,    65,   170,    -1,   260,    65,   248,    -1,   260,
      65,   170,    -1,   126,    -1,   131,    -1,    89,    -1,   125,
      -1,   104,    -1,   106,    -1,   107,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   430,   430,   435,   436,   442,   443,   448,   449,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   485,   487,   492,
     493,   494,   509,   510,   515,   516,   521,   526,   531,   535,
     541,   546,   550,   555,   559,   560,   561,   565,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   588,   589,   593,   597,   598,   602,
     603,   607,   608,   612,   613,   614,   615,   616,   617,   618,
     619,   623,   624,   628,   629,   630,   631,   635,   650,   651,
     652,   653,   654,   655,   656,   657,   658,   659,   660,   661,
     662,   663,   664,   665,   666,   672,   678,   684,   690,   697,
     707,   711,   712,   713,   714,   718,   719,   723,   726,   731,
     732,   736,   738,   740,   747,   752,   760,   765,   770,   778,
     779,   784,   785,   787,   792,   802,   811,   815,   823,   824,
     829,   834,   828,   859,   865,   872,   880,   891,   897,   890,
     925,   929,   934,   938,   946,   947,   951,   952,   953,   954,
     955,   956,   957,   958,   959,   960,   961,   962,   963,   964,
     965,   966,   967,   968,   969,   970,   971,   972,   973,   974,
     975,   976,   980,   981,   982,   983,   984,   985,   986,   987,
     988,   989,   990,   991,   995,   996,  1000,  1004,  1005,  1006,
    1010,  1012,  1014,  1016,  1021,  1022,  1026,  1027,  1028,  1029,
    1030,  1031,  1032,  1033,  1034,  1038,  1039,  1040,  1041,  1042,
    1043,  1047,  1048,  1052,  1053,  1054,  1055,  1056,  1057,  1061,
    1062,  1065,  1066,  1070,  1071,  1075,  1077,  1082,  1083,  1087,
    1088,  1092,  1093,  1097,  1099,  1101,  1106,  1119,  1136,  1137,
    1139,  1144,  1152,  1160,  1168,  1177,  1187,  1188,  1189,  1193,
    1194,  1202,  1204,  1210,  1215,  1217,  1219,  1224,  1226,  1228,
    1235,  1236,  1237,  1241,  1242,  1247,  1248,  1249,  1250,  1270,
    1274,  1278,  1286,  1290,  1291,  1292,  1296,  1298,  1304,  1306,
    1308,  1313,  1314,  1315,  1316,  1317,  1318,  1319,  1325,  1326,
    1327,  1328,  1332,  1333,  1334,  1338,  1339,  1343,  1344,  1348,
    1349,  1353,  1354,  1355,  1356,  1357,  1361,  1372,  1373,  1374,
    1375,  1376,  1377,  1379,  1381,  1383,  1385,  1387,  1389,  1394,
    1396,  1398,  1400,  1402,  1404,  1406,  1408,  1410,  1412,  1414,
    1416,  1418,  1425,  1431,  1437,  1443,  1452,  1457,  1465,  1466,
    1467,  1468,  1469,  1470,  1471,  1472,  1477,  1478,  1482,  1487,
    1490,  1495,  1500,  1503,  1508,  1512,  1513,  1517,  1518,  1523,
    1528,  1536,  1537,  1538,  1539,  1540,  1541,  1542,  1543,  1544,
    1546,  1548,  1550,  1552,  1554,  1559,  1560,  1561,  1562,  1573,
    1574,  1578,  1579,  1580,  1584,  1585,  1586,  1594,  1595,  1596,
    1597,  1601,  1602,  1603,  1604,  1605,  1606,  1607,  1608,  1609,
    1610,  1614,  1622,  1623,  1627,  1628,  1629,  1630,  1631,  1632,
    1633,  1634,  1635,  1636,  1637,  1638,  1639,  1640,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1648,  1649,  1653,  1654,  1655,
    1656,  1657,  1658,  1662,  1663,  1664,  1665,  1669,  1670,  1671,
    1672,  1677,  1678,  1679,  1680,  1681,  1682,  1683
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
  "TFOR", "TFORALL", "TFORWARDING", "TIF", "TIN", "TINDEX", "TINLINE",
  "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL", "TMINUSMINUS",
  "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TONLY", "TOTHERWISE",
  "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE",
  "TPROC", "TPUBLIC", "TRECORD", "TREDUCE", "TREF", "TREQUIRE", "TRETURN",
  "TSCAN", "TSELECT", "TSERIAL", "TSINGLE", "TSPARSE", "TSUBDOMAIN",
  "TSYNC", "TTHEN", "TTHROW", "TTHROWS", "TTRY", "TTRYBANG", "TTYPE",
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
  "forwarding_stmt", "extern_block_stmt", "loop_stmt", "zippered_iterator",
  "if_stmt", "try_stmt", "throw_stmt", "select_stmt", "when_stmt_ls",
  "when_stmt", "class_decl_stmt", "class_tag", "opt_inherit",
  "class_level_stmt_ls", "enum_decl_stmt", "enum_ls", "enum_item",
  "lambda_decl_expr", "$@1", "$@2", "linkage_spec", "fn_decl_stmt", "$@3",
  "$@4", "fn_decl_stmt_inner", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_or_iter", "opt_ret_tag", "opt_throws_error",
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   144,   145,   146,   146,   147,   147,   148,   148,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   150,   150,   151,
     151,   151,   152,   152,   153,   153,   154,   154,   154,   154,
     155,   155,   156,   156,   157,   157,   157,   158,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   160,   160,   161,   162,   162,   163,
     163,   164,   164,   165,   165,   165,   165,   165,   165,   165,
     165,   166,   166,   167,   167,   167,   167,   168,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     170,   171,   171,   171,   171,   172,   172,   173,   174,   175,
     175,   176,   176,   176,   177,   177,   178,   178,   178,   179,
     179,   180,   180,   180,   181,   182,   182,   182,   183,   183,
     185,   186,   184,   187,   187,   187,   187,   189,   190,   188,
     191,   191,   191,   191,   192,   192,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   195,   195,   196,   197,   197,   197,
     198,   198,   198,   198,   199,   199,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   201,   201,   201,   201,   201,
     201,   202,   202,   203,   203,   203,   203,   203,   203,   204,
     204,   205,   205,   206,   206,   207,   207,   208,   208,   209,
     209,   210,   210,   211,   211,   211,   212,   212,   213,   213,
     213,   214,   214,   214,   214,   214,   215,   215,   215,   216,
     216,   217,   217,   217,   218,   218,   218,   219,   219,   219,
     220,   220,   220,   221,   221,   222,   222,   222,   222,   223,
     223,   223,   223,   224,   224,   224,   225,   225,   225,   225,
     225,   226,   226,   226,   226,   226,   226,   226,   227,   227,
     227,   227,   228,   228,   228,   229,   229,   230,   230,   231,
     231,   232,   232,   232,   232,   232,   233,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   236,   237,   238,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   240,   241,
     241,   242,   243,   243,   244,   244,   244,   245,   245,   246,
     247,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   249,   249,   249,   249,   250,
     250,   251,   251,   251,   252,   252,   252,   253,   253,   253,
     253,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,   254,   255,   255,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   257,   257,   257,
     257,   257,   257,   258,   258,   258,   258,   259,   259,   259,
     259,   260,   260,   260,   260,   260,   260,   260
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     3,     1,     5,     6,     0,
       1,     1,     2,     3,     1,     2,     1,     3,     3,     5,
       0,     1,     1,     1,     3,     5,     5,     3,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     0,     1,     1,     1,     1,     2,
       1,     2,     3,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     2,     5,     3,
       6,     6,     4,     5,     5,     3,     3,     6,     5,     6,
       5,     6,     3,     4,     6,     7,     6,     7,     4,     5,
       4,     4,     3,     6,     5,     2,     2,     3,     5,     0,
       2,     3,     2,     3,     6,     8,     1,     1,     1,     0,
       2,     0,     2,     3,     5,     1,     2,     3,     1,     3,
       0,     0,     8,     0,     1,     2,     2,     0,     0,    10,
       3,     3,     5,     5,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     3,     0,     1,     3,
       4,     4,     6,     6,     0,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     0,     1,     2,     1,     1,     1,     0,
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
       3,     4,     1,     3,     2,     3,     3,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       3,     3,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     4,     3,     3,     3,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    36,    76,   411,   412,   413,   414,
     415,     0,   366,    74,   136,   366,     0,   267,    74,     0,
       0,     0,     0,    74,    74,     0,     0,   266,     0,     0,
     154,     0,   150,     0,     0,     0,     0,   357,     0,     0,
       0,     0,   266,   266,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   138,     0,     0,
       0,   463,   465,     0,   466,   467,   394,     0,     0,   464,
     461,    83,   462,     0,     0,     0,     4,     0,     5,     9,
       0,    10,    11,    12,    14,   326,    21,    13,    84,    90,
      15,    17,    16,    19,    20,    18,    89,     0,    87,   386,
       0,    91,    88,    92,     0,   395,   382,   383,   329,   327,
       0,     0,   387,   388,     0,   328,     0,   396,   397,   398,
     381,   331,   330,   384,   385,     0,    23,   337,     0,     0,
     367,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,   386,   395,   327,   387,   388,   366,   328,   396,   397,
       0,     0,     0,     0,   317,     0,    78,    77,   155,    97,
       0,   156,     0,     0,     0,     0,     0,   267,   268,    96,
       0,     0,   317,     0,     0,     0,     0,     0,   269,    30,
     449,   379,     0,   450,     7,   317,   268,    86,    85,   246,
     309,     0,   308,    81,     0,     0,     0,    80,    33,     0,
     332,     0,   317,    34,   338,     0,   125,   126,   258,     0,
       0,   308,     0,     0,   452,   393,   448,   451,   447,    42,
      44,     0,     0,   312,     0,   314,     0,     0,   313,     0,
     308,     0,     0,     6,     0,   139,   232,   231,   157,     0,
       0,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   317,
     317,     0,     0,     0,    24,    25,     0,    26,     0,     0,
       0,     0,     0,     0,     0,    27,    28,     0,   326,   324,
       0,   318,   319,   325,     0,     0,     0,     0,     0,   106,
       0,     0,   105,     0,     0,     0,   112,     0,     0,    50,
      93,     0,   122,     0,    29,   207,   151,   274,     0,   275,
     277,     0,   288,     0,     0,   280,     0,     0,    31,     0,
     156,   245,     0,    57,    82,   129,    79,    32,   317,     0,
     127,     0,   256,   253,    54,     0,    50,    99,    35,    43,
      45,     0,   416,     0,     0,   407,     0,   409,     0,     0,
       0,     0,     0,     0,   420,     8,     0,     0,     0,   225,
       0,     0,     0,     0,     0,   365,   444,   443,   446,   454,
     453,   458,   457,   440,   437,   438,   439,   390,   427,   406,
     405,   404,   391,   431,   442,   436,   434,   445,   435,   433,
     425,   430,   432,   441,   424,   428,   429,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   456,   455,   460,   459,
     219,   216,   217,   218,   222,   223,   224,     0,     0,   369,
       0,     0,     0,     0,     0,     0,     0,     0,   418,   366,
     366,   102,   254,     0,     0,     0,   334,     0,   148,     0,
     145,   255,   139,     0,     0,     0,   341,     0,     0,     0,
     347,     0,     0,     0,   113,   462,    53,     0,    46,    51,
       0,   121,     0,     0,   333,     0,   208,     0,   215,   233,
       0,   278,     0,   292,     0,   287,   382,     0,     0,   271,
     380,   270,   403,   311,   310,     0,     0,   335,     0,   260,
     382,     0,     0,     0,   417,   389,   408,   315,   410,   316,
       0,     0,   419,   118,   351,     0,   422,   421,     0,     0,
     140,   141,   229,   226,   227,   230,   158,     0,     0,   262,
     261,   263,   265,    58,    65,    66,    67,    62,    64,    72,
      73,    60,    63,    61,    59,    69,    68,    70,    71,   401,
     402,   220,   221,   374,     0,   368,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,   323,
     321,   322,   320,     0,   146,   144,     0,     0,   120,     0,
       0,   104,     0,   103,     0,     0,   110,     0,     0,   108,
       0,     0,   372,     0,    94,     0,    95,     0,     0,   124,
     214,   206,     0,   301,   234,   237,   236,   238,     0,   276,
     279,     0,   280,     0,   272,   281,   282,     0,     0,   128,
     130,   336,     0,   257,    55,    56,     0,     0,     0,     0,
     119,     0,    37,     0,   266,   228,   233,   189,   187,   192,
     199,   200,   201,   196,   198,   194,   197,   195,   193,   203,
     202,   168,   171,   169,   170,   181,   172,   185,   177,   175,
     188,   176,   174,   179,   184,   186,   173,   178,   182,   183,
     180,   190,   191,     0,   166,     0,   204,   204,   164,   264,
     370,   395,   395,     0,     0,     0,     0,     0,     0,     0,
       0,   101,   100,   149,   147,   141,   107,     0,     0,   340,
       0,   339,     0,     0,   346,   111,     0,   345,   109,     0,
     371,    48,    47,   123,   356,   209,     0,     0,   280,   235,
     251,   273,     0,     0,     0,   132,     0,     0,     0,   116,
     353,     0,     0,   114,   352,     0,   423,    38,    74,   266,
     266,   134,   266,   142,     0,   167,     0,     0,   207,   160,
     161,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     344,     0,     0,   350,     0,     0,   373,     0,   301,   304,
     305,   306,     0,   303,   307,   382,   247,   211,   210,     0,
       0,     0,   290,   382,   133,   131,     0,   117,     0,   115,
     143,   239,   165,   166,   204,   204,     0,     0,     0,     0,
       0,     0,     0,   135,     0,     0,     0,     0,    49,   280,
     293,     0,   248,   250,   249,   252,   243,   244,   152,     0,
       0,     0,   240,   251,   162,   163,   205,     0,     0,     0,
       0,     0,     0,   343,   342,   349,   348,   213,   212,   295,
     296,   298,   382,     0,   418,   382,   355,   354,     0,     0,
     297,   299,   241,   159,   242,   293,   300
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   220,    77,   501,    79,    80,    81,   221,
     496,   500,   497,    82,    83,    84,   157,    85,   161,   198,
      86,    87,    88,    89,    90,    91,   594,    92,    93,    94,
      95,   525,   650,    96,    97,   388,   664,    98,   479,   480,
     141,   174,   509,   100,   101,   389,   666,   556,   705,   706,
     707,   779,   336,   505,   506,   507,   457,   557,   238,   638,
     853,   883,   848,   190,   843,   807,   810,   102,   209,   362,
     103,   104,   177,   178,   340,   341,   519,   344,   345,   515,
     870,   804,   748,   222,   226,   227,   310,   311,   312,   142,
     106,   107,   108,   143,   110,   129,   130,   458,   327,   621,
     459,   111,   144,   145,   114,   147,   116,   148,   149,   119,
     120,   231,   121,   122,   123,   124,   125
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -789
static const yytype_int16 yypact[] =
{
    -789,   141,  2468,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  3611,    -6,   187,  -789,    -6,  7861,   147,   187,  7861,
    3611,   108,   187,   186,   423,  6354,  7861,  6491,  7861,   164,
    -789,   187,  -789,    23,  3611,  7861,  7861,  -789,  7861,  7861,
     218,   167,   379,   916,  -789,  6765,  6902,  7861,  7039,  7861,
     161,   174,  3611,  7861,  3611,  3611,   187,  -789,  6765,  7861,
    7861,  -789,  -789,  7861,  -789,  -789,  9231,  7861,  7861,  -789,
    7861,  -789,  -789,  2789,  5943,  6765,  -789,  3474,  -789,  -789,
     305,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,   187,  -789,   283,
     115,  -789,  -789,  -789,   275,   246,  -789,  -789,  -789,   252,
     282,   273,   291,   297, 13336,  2009,   -50,   312,   313,  -789,
    -789,  -789,  -789,  -789,  -789,   278,  -789, 13336,   307,  3611,
    -789,   314,  -789,   298,  7861,  7861,  7861,  7861,  7861,  6765,
    6765,    78,  -789,  -789,  -789,  -789, 10979,   244,  -789,  -789,
     187,   317, 11138,   363,  6765,   324,  -789,  -789,  -789,  -789,
     187,    80,   187,   325,    44,  9936,  9869,  -789,  -789,  -789,
   10931, 10065,  6765,  3611,   328,    15,    77,    26,  -789,  -789,
     517,   338, 10132,   517,  -789,  6765,   431,  -789,  -789,   187,
    -789,    90, 13336,  -789, 11255, 10792,  3611,  -789,  -789, 10132,
   13336,   332,  6765,  -789, 13336, 11299,  -789,  -789,   381,   345,
     178, 11056, 10132, 11333,   279,  1338,   517,   279,   517,  -789,
    -789,  2926,    46,  -789,  7861,  -789,   -36,   -29, 13336,    83,
   11377,   -58,   468,  -789,   187,   369,  -789,  -789,  -789,    19,
      23,    23,    23,  -789,  7861,  7861,  7861,  7861,  7176,  7176,
    7861,  7861,  7861,  7861,  7861,  7861,   289,  9231,  7861,  7861,
    7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,
    7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,
    7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  6765,
    6765,  7176,  7176,  5806,  -789,  -789,  3063,  -789, 11104, 11182,
   11457,    66,  7176,    44,   352,  -789,  -789,  7861,   120,  -789,
     346,   374,  -789, 13336,   187,   358,   187,   455,  6765,  -789,
    3748,  7176,  -789,  3885,  7176,   356,  -789,    44,  7998,  7861,
    -789,  3611,   469,   361,  -789,    95,  -789,  -789,    15,  -789,
     385,   364,  -789,  5118,   408,   411,  7861,    23,  -789,   367,
    -789,  -789,  6765,  -789,  -789,  -789,  -789,  -789,  6765,   371,
    -789,  5255,   401,  -789,  -789,  7998,  7861,  -789,  -789,  -789,
    -789,  6628,  -789,  9745,  6080,  -789,  6217,  -789,  7176,  5392,
    2652,   372,  7861,  5668,  -789,  -789,   376,  6765,   378,   268,
      23,   180,   206,   229,   250, 10854,  1677,  1677,   214,  -789,
     214,  -789,   214,  9281,   689,  1797,   427,   338,   279,  -789,
    -789,  -789,  1338,  1027,   214,   931,   931,  1677,   931,   931,
    1572,   279,  1027,  2517,  1572,   517,   517,   279, 11501, 11535,
   11579, 11652, 11696, 11730, 11774, 11847, 11891, 11925, 11969, 12042,
   12086, 12120, 12164, 12237,   382,   384,  -789,   214,  -789,   214,
     152,  -789,  -789,  -789,  -789,  -789,  -789,   187,   -18,  -789,
   13398,   281,  8135,  7176,  8272,  7176,  7861,  7176,  9446,    -6,
   12299,  -789,  -789, 12366,  7861,  6765,  -789,  6765,   424,    94,
    -789,  -789,   369,  7861,    88,  7861, 13336,    48, 10194,  7861,
   13336,    36,  9998,  5806,  -789,   391,   413,   398, 12428,   413,
     400,   503, 12490,  3611,  -789,    89,  -789,    29,  -789,   274,
     392,    15,    68,  -789,  6765,  -789,   829,  7861,  7313,  -789,
   13336,  -789,  -789,  -789, 13336,     2,   394,  -789,  6765,  -789,
     286,   187,   405,   415,  -789,  -789,  -789,  -789,  -789,  -789,
     -35,  9418,  -789,  -789, 13336,  3611, 13336,  -789, 12552,  3200,
     430,  -789,   474,  -789,  -789,  -789,  -789,  2322,   308,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  5806,  -789,  7176,  7176,  7861,   520,
   12614,  7861,   521, 12676,   407,  9544,    44,    44,  -789, 13336,
    -789, 13336,  -789,  7861,   187,  -789,   416, 10132,  -789, 10256,
    4022,  -789,  4159,  -789, 10323,  4296,  -789,    44,  4433,  -789,
      44,    91,  -789,  7861,  -789,  7861,  -789,  3611,  7861,  -789,
     344,  -789,    15,   442,   490,  -789,  -789,  -789,    28,  -789,
    -789,  5255,   411,    71, 13336,  -789, 13336,  4570,  6765,  -789,
    -789,  -789,    72,  -789,  -789,  -789,  4707,   414,  4844,   419,
    -789,  7861,  -789,  3337,   871,  -789,   274,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,   187,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,
    -789,  -789,  -789,  7861,   444,   449,   425,   425,  -789,  -789,
    -789,   188,   215, 12738,  8409,  8546, 12805,  8683,  8820,  8957,
    9094,  -789,  -789, 13336,  -789,  -789,  -789,  3611,  7861, 13336,
    7861, 13336,  3611,  7861, 13336,  -789,  7861, 13336,  -789,  5806,
    -789, 12872, 13336,  -789, 13336,  -789,   426,  7450,   -14,  -789,
     470,  -789,  7176,  2235,  3611,  -789,     7,  4981,  7861,  -789,
   13336,  3611,  7861,  -789, 13336,  3611, 13336,  -789,   296,   630,
     630,  -789,  1522,  -789,    11,  -789,  9807,  9496,    95,  -789,
    -789,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  7861,  1487,
   12490, 10390, 10457, 12490, 10524, 10591,  -789,  7861,   442,   108,
    7861,  7861,  5530,  -789,  -789,   210,  6765,  -789,  -789,  7861,
     -45,  9621,  -789,   892,   363,  -789, 10658,  -789, 10725,  -789,
    -789,   487,  -789,  -789,   425,   425,   253, 12934, 13001, 13068,
   13135, 13202, 13269,  -789,  3611,  3611,  3611,  3611, 13336,   -14,
    7587,    79,  -789,  -789, 13336, 13336,  -789,  -789,  -789,  9094,
    3611,  3611,  -789,   470,  -789,  -789,  -789,  7861,  7861,  7861,
    7861,  7861,  7861, 12490, 12490, 12490, 12490,  -789,  -789,  -789,
    -789,  -789,   322,  7176,  9309,   912, 12490, 12490,    81,  9683,
    -789,  -789,  -789,  -789,  -789,  7724,  -789
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -789,  -789,  -789,    -1,  -625,  1636,  -789,  -789,   901,    16,
    -159,   201,   205,  -789,  -789,  -789,   119,  1972,   -17,   -99,
    -756,  -594,   -33,  -789,  -789,  -789,   137,  -789,  -789,  -789,
    -789,  -789,  -789,  -789,   418,   102,  -140,  -789,  -789,   -16,
     514,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -789,  -191,
    -188,  -674,  -789,  -185,   -40,  -789,  -332,  -789,  -789,   -72,
    -789,  -789,  -282,  1133,  -789,  -242,  -255,  -789,  -130,  -789,
     576,  -789,  -196,   258,  -789,  -327,  -638,  -789,  -489,  -354,
    -788,  -766,  -192,    13,    39,  -789,   -68,  -789,   132,   350,
    -266,  -789,  -789,   561,  -789,    -2,  -789,  -789,  -208,  -789,
    -453,  -789,   620,   978,   -11,  1209,  -789,  1237,  1366,  -789,
    -789,  -789,  -789,  -789,  -789,  -789,  -291
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -419
static const yytype_int16 yytable[] =
{
     127,    76,   461,   508,   751,   146,   158,   529,   152,   187,
     188,   510,   342,   133,   165,   166,   170,   171,     5,    46,
     304,   381,     5,   642,   180,   181,     5,   182,   183,   342,
     315,   196,     5,   780,   192,   194,   195,   199,   200,   772,
     622,   204,   205,   391,   392,   393,   394,   211,   212,   213,
     325,   383,   214,   647,   847,   215,   216,   217,   191,   218,
     615,   346,   192,   228,   230,   319,   322,   326,   196,   342,
     773,   210,   610,   374,   871,  -285,   518,   516,   342,   128,
     376,   390,   384,   348,   648,  -285,   880,   289,   229,   290,
     296,   584,  -285,   337,  -285,   530,    14,   886,  -214,   806,
     357,   467,   375,  -285,   333,   656,   752,   752,   881,   377,
     808,  -285,  -285,   367,   873,   450,   352,   349,   378,   641,
     585,   325,   847,   127,   298,   299,   300,   204,   192,   230,
     451,   710,   131,   452,   359,   347,   641,   151,   649,  -285,
      44,     3,   296,   313,   303,    46,  -285,   453,   454,   750,
     854,   855,   338,   301,   236,   371,   175,   455,  -285,    57,
     175,   313,   164,  -285,   772,  -283,   632,  -285,   325,   350,
     499,   296,   456,   237,   313,   379,   641,  -285,   820,   296,
     379,   379,   372,   296,   640,   343,  -285,   581,   379,     5,
       5,   313,   379,   156,   558,   773,  -285,   352,   630,   352,
     739,   868,   461,   604,   471,  -285,   468,   499,   474,   882,
     475,   753,   757,   373,   582,  -400,   296,  -400,   353,   874,
     370,   444,   445,   380,   150,   184,   608,   631,   494,   740,
     605,   201,  -214,   395,   396,   397,   398,   400,   402,   403,
     404,   405,   406,   407,   408,   154,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   313,   313,
     447,   449,   460,   617,   620,   821,   796,   352,   552,   347,
     526,   470,     5,   461,   634,   239,   473,  -375,   508,     5,
    -302,   172,   247,   156,   185,   746,   364,   192,   559,   486,
     488,   202,   490,   492,   409,   347,  -268,   498,   498,  -302,
     502,   553,   254,  -302,  -376,   256,  -375,   635,   240,   259,
     554,   484,   657,   659,   560,   520,   636,   241,   347,   291,
     248,   524,   587,   292,   249,   555,   292,   313,  -302,  -268,
     234,   637,   105,  -376,   498,   498,   242,   561,  -268,   347,
     524,   105,   630,   228,   450,   228,   410,   541,   524,   544,
     105,   546,   548,   160,  -359,   516,   192,  -268,   562,   451,
    -358,  -399,   452,  -399,   105,   399,   401,   254,   611,   613,
     256,   856,   616,   619,   259,  -259,   453,   454,   167,   812,
     550,   653,   105,   812,   105,   105,   455,   244,    23,   186,
     243,  -362,  -294,   537,  -259,   539,    30,   347,  -153,  -363,
    -400,   456,  -400,   105,   -41,  -364,     5,   105,   446,   448,
     156,  -294,   159,   296,     5,  -294,   709,  -153,   156,   469,
    -361,  -360,   295,  -268,   293,   305,   508,   307,   461,   256,
     247,   486,   590,   490,   593,   502,   595,   544,   487,   314,
    -294,   491,   318,   599,   601,   335,   313,   596,   597,   358,
     -74,   361,   607,   363,   609,   385,  -268,   387,   614,   105,
     472,   805,   460,   477,   476,  -268,   481,   813,   248,   -74,
     483,   813,   249,   493,   511,   503,   517,   721,   722,   504,
     160,   518,   512,   230,  -268,   522,   644,   646,   726,   527,
     531,   549,   545,   551,   603,   540,    99,   230,   735,   -52,
     579,   738,   623,   105,   580,    99,   624,   643,   626,   627,
     639,   251,   651,   654,    99,   254,   665,   255,   256,   352,
     247,   652,   259,   655,   714,   717,   105,   719,    99,   266,
     747,   725,   749,   809,   761,  -326,   270,   271,   272,   765,
     777,   852,   778,   109,   798,   663,    99,   533,    99,    99,
     532,   105,   109,   460,   872,   400,   447,   713,   248,   316,
     716,   109,   249,   875,   606,   789,   824,    99,   724,   825,
     745,    99,   723,   826,   774,   109,   884,   867,   878,   729,
     589,   731,   592,   169,   734,   521,   839,   737,   872,   602,
       0,     0,   741,   109,   742,   109,   109,   744,     0,   872,
       0,     0,   112,     0,     0,   254,     0,   255,   256,     0,
       0,   112,   259,     0,   109,     0,     0,   192,   109,   266,
     112,     0,     0,    99,     0,   760,   105,   764,   272,   167,
     766,     0,     0,     0,   112,     0,     0,   815,     0,    23,
     186,   756,   370,     0,     0,     0,     0,    30,     0,  -153,
     105,     0,   112,   105,   112,   112,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     0,     0,    99,  -153,     0,
     109,     0,   776,   112,     0,     0,     0,   112,     0,     0,
       0,     0,     0,   729,   731,     0,   734,   737,   760,   764,
      99,     0,   247,     0,     0,     0,   790,   791,     0,   792,
       0,   793,   794,   399,   446,   795,     0,     0,   460,     0,
     105,     0,     0,     0,   109,    99,   187,   188,     0,     0,
       0,   811,   544,     0,     0,     0,   544,   816,     0,   112,
     248,   818,     0,     0,   249,     0,     0,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     790,   827,   828,   793,   829,   830,   831,   832,     0,     0,
       0,     0,   109,     0,     0,     0,   838,     0,     0,   200,
     204,   230,     0,   112,     0,   844,     0,   254,   845,   255,
     256,     0,     0,     0,   259,     0,     0,   583,     0,     0,
      99,   266,     0,     0,     0,   841,   112,     0,   270,   271,
     272,     0,     0,   863,   864,   865,   866,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,    99,   764,   876,
     877,   112,     0,     0,     0,    99,   863,   864,   865,   866,
     876,   877,     0,   105,     0,     0,     0,   109,     0,     0,
       0,     0,   879,   544,  -286,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   764,     0,     0,     0,     0,     0,
       0,   109,     0,     0,   109,     0,     0,    14,     0,     0,
      17,     0,   109,  -286,    99,   105,     0,     0,    22,   105,
      23,   768,     0,  -286,    27,     0,     0,   708,    30,     0,
    -153,     0,  -286,     0,     0,     0,   112,     0,     0,  -286,
       0,     0,     0,     0,     0,     0,    40,  -289,   769,  -153,
     770,    44,     0,     0,     0,   167,   711,   712,  -286,     0,
     112,   109,     0,   112,     0,    23,   186,  -291,    56,   197,
      57,   112,     0,    30,   247,  -153,  -289,  -286,     0,     0,
     105,   -40,   105,     0,  -286,   105,  -289,     0,   105,     0,
       0,     0,     0,     0,  -153,  -289,  -291,   105,     0,     0,
     113,     0,  -289,     0,     0,     0,  -291,     0,     0,   113,
       0,     0,   248,     0,     0,  -291,   249,   105,   113,    71,
     112,  -289,  -291,     0,     0,     0,   105,   771,   105,     0,
       0,     0,   113,   105,     0,     0,     0,    99,     0,     0,
    -289,  -291,     0,     0,     0,     0,     0,  -289,     0,     0,
     113,     0,   113,   113,   297,   251,     0,   252,   253,   254,
    -291,   255,   256,   257,     0,     0,   259,  -291,     0,     0,
     247,   113,   265,   266,     0,   113,     0,   269,     0,    99,
     270,   271,   272,    99,   109,   197,   197,   197,     0,     0,
       0,     0,   332,     0,     0,     0,     0,   105,     0,     0,
       0,     0,   105,   197,     0,     0,     0,     0,   248,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,     0,
     197,     0,     0,     0,   105,     0,   109,   113,     0,     0,
     109,   105,     0,   197,     0,   105,     0,     0,     0,     0,
       0,     0,     0,   112,    99,     0,    99,     0,     0,    99,
       0,   251,    99,   252,   253,   254,     0,   255,   256,   257,
       0,    99,   259,   260,   261,     0,   263,   264,   265,   266,
       0,   113,     0,   269,     0,     0,   270,   271,   272,     0,
       0,    99,     0,     0,     0,   112,     0,     0,     0,   112,
      99,   109,    99,   109,   113,     0,   109,    99,     0,   109,
       0,     0,     0,     0,   105,   105,   105,   105,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   113,
     105,   105,     0,     0,   197,     0,     0,   225,   109,     0,
       0,   115,     0,     0,     0,     0,     0,   109,     0,   109,
     115,     0,     0,     0,   109,     0,     0,     0,   197,   115,
     112,     0,   112,     0,     0,   112,     0,     0,   112,   117,
       0,    99,     0,   115,     0,     0,    99,   112,   117,     0,
       0,     0,     0,     0,     0,     0,     0,   117,     0,     0,
       0,   115,     0,   115,   115,     0,     0,   112,    99,     0,
       0,   117,     0,     0,   113,    99,   112,     0,   112,    99,
       0,     0,   115,   112,     0,     0,   115,   309,   109,   117,
       0,   117,   117,   109,     0,     0,     0,     0,   113,     0,
       0,   113,     0,     0,     0,   309,     0,     0,     0,   113,
     117,     0,     0,     0,   117,   109,     0,     0,   309,     0,
       0,     0,   109,     0,     0,     0,   109,     0,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,   115,     0,
       0,     0,     0,     0,     0,     0,     0,   112,    99,    99,
      99,    99,   112,     0,     0,     0,     0,     0,   113,     0,
       0,   247,     0,     0,    99,    99,   117,     0,   118,     0,
       0,     0,     0,     0,   112,     0,     0,   118,     0,     0,
       0,   112,   115,     0,     0,   112,   118,     0,   197,   197,
       0,     0,   197,   197,     0,   109,   109,   109,   109,   248,
     118,     0,     0,   249,     0,   115,     0,     0,     0,     0,
     117,   109,   109,     0,     0,     0,     0,     0,   118,     0,
     118,   118,   309,   309,     0,     0,     0,     0,     0,     0,
     115,     0,     0,   117,     0,     0,     0,     0,     0,   118,
       0,     0,   251,   118,   252,   253,   254,     0,   255,   256,
       0,     0,     0,   259,   112,   112,   112,   112,   117,   265,
     266,     0,     0,     0,   269,     0,     0,   270,   271,   272,
     112,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   113,     0,     0,     0,   523,     0,     0,     0,     0,
       0,   309,     0,     0,     0,   118,     0,   197,   197,     0,
       0,     0,     0,    14,   523,   115,    17,   225,   197,   225,
     332,     0,   523,     0,    22,   332,    23,   768,   197,     0,
      27,   197,     0,   113,    30,     0,  -153,   113,     0,   115,
       0,     0,   115,   117,     0,     0,     0,     0,    14,   118,
     115,    17,    40,     0,   769,  -153,   770,    44,     0,    22,
       0,    23,   768,     0,     0,    27,     0,   117,     0,    30,
     117,  -153,   118,     0,    56,     0,    57,     0,   117,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,   769,
    -153,   770,    44,     0,     0,     0,     0,   118,   113,   115,
     113,     0,     0,   113,     0,   247,   113,     0,     0,    56,
       0,    57,     0,     0,     0,   113,     0,     0,   600,     0,
     309,     0,     0,     0,     0,    71,     0,   117,     0,     0,
       0,     0,     0,   833,     0,   113,     0,     0,     0,     0,
       0,     0,     0,   248,   113,     0,   113,   249,    78,     0,
       0,   113,     0,     0,     0,     0,     0,   126,     0,     0,
      71,     0,     0,     0,     0,     0,   153,   197,     0,     0,
       0,     0,   118,     0,     0,     0,     0,     0,     0,     0,
     179,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,     0,     0,   118,   259,   203,   118,
     206,   207,   332,   332,   266,   332,   332,   118,     0,     0,
     247,   270,   271,   272,     0,   113,     0,     0,     0,    78,
     113,   846,   115,   233,     0,     0,     0,   332,     0,   332,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,     0,     0,     0,     0,     0,   248,   113,
     117,     0,   249,   113,     0,     0,   118,     0,     0,     0,
       0,     0,     0,     0,   115,     0,     0,     0,   115,     0,
       0,     0,     0,     0,     0,   294,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   846,
       0,   251,   117,   252,   253,   254,   117,   255,   256,   257,
       0,   258,   259,   260,   261,     0,   263,   264,   265,   266,
       0,   267,   268,   269,     0,     0,   270,   271,   272,   334,
       0,     0,   113,   113,   113,   113,     0,     0,     0,   115,
     247,   115,     0,     0,   115,     0,     0,   115,   113,   113,
       0,     0,   356,     0,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   117,     0,   117,
       0,     0,   117,     0,     0,   117,   115,    78,   248,     0,
       0,     0,   249,     0,   117,   115,     0,   115,     0,   118,
       0,     0,   115,     0,     0,     0,     0,     0,     0,     0,
     803,     0,     0,     0,   117,     0,     0,     0,     0,     0,
       0,     0,     0,   117,     0,   117,     0,     0,     0,     0,
     117,   251,     0,     0,   253,   254,     0,   255,   256,     0,
       0,   118,   259,     0,     0,   118,     0,     0,     0,   266,
       0,     0,     0,     0,     0,     0,   270,   271,   272,     0,
       0,     0,    78,     0,     0,     0,   115,     0,     0,   842,
       0,   115,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   356,     0,     0,   356,
       0,     0,     0,   115,   117,     0,     0,     0,     0,   117,
     115,     0,     0,   869,   115,     0,   118,     0,   118,     0,
       0,   118,     0,     0,   118,   132,     0,     0,     0,     0,
     132,   117,     0,   118,   155,   132,   132,     0,   117,     0,
       0,     0,   117,   173,     0,   176,     0,   869,     0,     0,
       0,     0,     0,   118,     0,     0,   543,     0,   869,     0,
       0,     0,   118,     0,   118,     0,     0,     0,   208,   118,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   115,   115,   115,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   115,
     115,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,   117,   117,   117,   117,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   117,   117,     0,
       0,     0,     0,   118,     0,     0,     0,     0,   118,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,     0,     0,     0,     0,     0,     0,     0,
     118,     0,   208,     0,     0,     0,   308,   118,     0,     0,
       0,   118,   208,     0,   317,     0,     0,     0,     0,   629,
       0,   287,   288,  -377,   308,     0,  -399,   339,  -399,     0,
       0,     0,     0,     0,     0,     0,     0,   308,   132,     0,
       0,   351,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   308,     0,     0,     0,     0,     0,
       0,   660,     0,     0,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     118,   118,   118,   118,     0,     0,   386,     0,     0,     0,
       0,   176,   176,   176,   176,     0,   118,   118,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
       0,     0,     0,     0,     0,     0,   513,     0,     5,     6,
       7,     8,     9,    10,     0,  -418,   356,   134,   356,     0,
    -418,   356,     0,     0,   356,     0,     0,     0,  -418,     0,
      21,   308,   308,   743,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,   755,     0,     0,   478,     0,   482,    39,
       0,    41,   759,     0,   763,     0,  -418,     0,     0,    78,
    -418,     0,     0,    49,    50,    51,   138,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,  -284,  -418,     5,     0,     0,     0,     0,
     308,     0,   667,     0,     0,     0,     0,   668,     0,  -418,
      63,  -418,  -418,  -418,     0,  -418,  -418,  -418,     0,  -418,
    -418,  -418,  -418,  -418,  -418,  -418,  -418,  -418,    68,  -418,
    -418,  -418,   176,     0,  -418,  -418,  -418,     0,     0,     0,
     139,     0,    74,     0,   528,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     814,     0,     0,     0,     0,     0,     0,   817,     0,     0,
       0,   819,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   670,   671,   672,   673,   674,     0,     0,
     675,   676,   677,   678,   679,   680,   681,   682,   683,   684,
       0,     0,   685,     0,     0,     0,   686,   687,   688,   689,
     690,   691,   692,   693,   694,   695,   696,     0,   697,   308,
       0,   698,   699,   700,   701,     0,   702,     0,     0,   703,
       0,     0,     0,     0,     0,     0,     0,     0,    -2,     4,
       0,     5,     6,     7,     8,     9,    10,     0,     0,   633,
      11,    12,    13,   339,    14,    15,    16,    17,  -266,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,    28,   208,    29,    30,     0,  -153,    31,    32,
      33,    34,    35,   -39,    36,    37,     0,    38,     0,     0,
       0,  -266,    39,    40,    41,    42,  -153,    43,    44,   704,
    -266,    45,    46,     0,    47,    48,    49,    50,    51,    52,
     247,    53,     0,    54,    55,    56,     0,    57,    58,  -266,
       0,     0,    59,     0,    60,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,   478,     0,   248,     0,
      66,     0,   249,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,    71,     0,     0,    72,
       0,     0,     0,    73,   339,    74,   250,    75,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,   252,   253,   254,     0,   255,   256,   257,
       0,   258,   259,   260,   261,     0,   263,   264,   265,   266,
       0,   267,     0,   269,     0,     0,   270,   271,   272,     0,
       0,     0,     0,     4,   775,     5,     6,     7,     8,     9,
      10,     0,  -418,     0,    11,    12,    13,  -418,    14,    15,
      16,    17,  -266,    18,    19,  -418,    20,    21,  -418,    22,
       0,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,  -153,    31,    32,    33,    34,    35,   -39,    36,    37,
       0,    38,     0,     0,     0,  -266,    39,     0,    41,    42,
    -153,    43,    44,  -418,  -266,    45,    46,  -418,    47,    48,
      49,    50,    51,    52,     0,    53,     0,    54,    55,    56,
       0,    57,    58,  -266,     0,     0,    59,     0,    60,     0,
     132,  -418,     0,     0,     0,     0,     0,     0,     0,   823,
       0,     0,     0,     0,     0,     0,  -418,    63,  -418,  -418,
    -418,  -418,  -418,  -418,  -418,     0,  -418,  -418,  -418,  -418,
    -418,  -418,  -418,  -418,  -418,    68,  -418,  -418,  -418,     0,
      71,  -418,  -418,  -418,     0,     0,     0,    73,  -418,    74,
       4,    75,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -266,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,    28,     0,    29,    30,     0,  -153,    31,
      32,    33,    34,    35,   -39,    36,    37,     0,    38,     0,
       0,     0,  -266,    39,    40,    41,    42,  -153,    43,    44,
       0,  -266,    45,    46,     0,    47,    48,    49,    50,    51,
      52,     0,    53,     0,    54,    55,    56,     0,    57,    58,
    -266,     0,     0,    59,     0,    60,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,   189,    71,     0,     0,
      72,     0,     0,     0,    73,   219,    74,     4,    75,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -266,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
      28,     0,    29,    30,     0,  -153,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,     0,     0,  -266,
      39,    40,    41,    42,  -153,    43,    44,     0,  -266,    45,
      46,     0,    47,    48,    49,    50,    51,    52,     0,    53,
       0,    54,    55,    56,     0,    57,    58,  -266,     0,     0,
      59,     0,    60,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,    71,     0,     0,    72,     0,     0,
       0,    73,   369,    74,     4,    75,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -266,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,    28,     0,    29,
      30,     0,  -153,    31,    32,    33,    34,    35,   -39,    36,
      37,     0,    38,     0,     0,     0,  -266,    39,    40,    41,
      42,  -153,    43,    44,     0,  -266,    45,    46,     0,    47,
      48,    49,    50,    51,    52,     0,    53,     0,    54,    55,
      56,     0,    57,    58,  -266,     0,     0,    59,     0,    60,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,    71,     0,     0,    72,     0,     0,     0,    73,   219,
      74,     4,    75,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -266,    18,    19,     0,    20,    21,     0,    22,     0,    23,
      24,    25,    26,    27,    28,     0,    29,    30,     0,  -153,
      31,    32,    33,    34,    35,   -39,    36,    37,     0,    38,
       0,     0,     0,  -266,    39,    40,    41,    42,  -153,    43,
      44,     0,  -266,    45,    46,     0,    47,    48,    49,    50,
      51,    52,     0,    53,     0,    54,    55,    56,     0,    57,
      58,  -266,     0,     0,    59,     0,    60,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,     0,    71,     0,
       0,    72,     0,     0,     0,    73,   662,    74,     4,    75,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -266,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,  -153,    31,    32,    33,
      34,    35,   -39,    36,    37,     0,    38,     0,     0,     0,
    -266,    39,    40,    41,    42,  -153,    43,    44,     0,  -266,
      45,    46,     0,    47,    48,    49,    50,    51,    52,     0,
      53,     0,    54,    55,    56,     0,    57,    58,  -266,     0,
       0,    59,     0,    60,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,     0,    71,     0,     0,    72,     0,
       0,     0,    73,   767,    74,     4,    75,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -266,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,    28,     0,
      29,    30,     0,  -153,    31,    32,    33,    34,    35,   -39,
      36,    37,     0,    38,     0,     0,     0,  -266,    39,   232,
      41,    42,  -153,    43,    44,     0,  -266,    45,    46,     0,
      47,    48,    49,    50,    51,    52,     0,    53,     0,    54,
      55,    56,     0,    57,    58,  -266,     0,     0,    59,     0,
      60,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,     0,    71,     0,     0,    72,     0,     0,     0,    73,
       0,    74,     4,    75,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    11,    12,    13,     0,    14,    15,    16,
      17,  -266,    18,    19,     0,    20,    21,     0,    22,     0,
      23,    24,    25,    26,    27,    28,     0,    29,    30,     0,
    -153,    31,    32,    33,    34,    35,   -39,    36,    37,     0,
      38,     0,     0,     0,  -266,    39,     0,    41,    42,  -153,
      43,    44,     0,  -266,    45,    46,     0,    47,    48,    49,
      50,    51,    52,     0,    53,     0,    54,    55,    56,     0,
      57,    58,  -266,     0,     0,    59,     0,    60,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    68,     0,    69,    70,     0,    71,
       0,     0,    72,     0,     0,     0,    73,     0,    74,     4,
      75,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      11,    12,    13,     0,    14,    15,    16,    17,  -266,    18,
      19,     0,    20,    21,     0,    22,     0,    23,    24,    25,
      26,    27,   485,     0,    29,    30,     0,  -153,    31,    32,
      33,    34,    35,   -39,    36,    37,     0,    38,     0,     0,
       0,  -266,    39,     0,    41,    42,  -153,    43,    44,     0,
    -266,    45,    46,     0,    47,    48,    49,    50,    51,    52,
       0,    53,     0,    54,    55,    56,     0,    57,    58,  -266,
       0,     0,    59,     0,    60,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,    71,     0,     0,    72,
       0,     0,     0,    73,     0,    74,     4,    75,     5,     6,
       7,     8,     9,    10,     0,     0,     0,    11,    12,    13,
       0,    14,    15,    16,    17,  -266,    18,    19,     0,    20,
      21,     0,    22,     0,    23,    24,    25,    26,    27,   489,
       0,    29,    30,     0,  -153,    31,    32,    33,    34,    35,
     -39,    36,    37,     0,    38,     0,     0,     0,  -266,    39,
       0,    41,    42,  -153,    43,    44,     0,  -266,    45,    46,
       0,    47,    48,    49,    50,    51,    52,     0,    53,     0,
      54,    55,    56,     0,    57,    58,  -266,     0,     0,    59,
       0,    60,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,     0,    71,     0,     0,    72,     0,     0,     0,
      73,     0,    74,     4,    75,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    11,    12,    13,     0,    14,    15,
      16,    17,  -266,    18,    19,     0,    20,    21,     0,    22,
       0,    23,    24,    25,    26,    27,   728,     0,    29,    30,
       0,  -153,    31,    32,    33,    34,    35,   -39,    36,    37,
       0,    38,     0,     0,     0,  -266,    39,     0,    41,    42,
    -153,    43,    44,     0,  -266,    45,    46,     0,    47,    48,
      49,    50,    51,    52,     0,    53,     0,    54,    55,    56,
       0,    57,    58,  -266,     0,     0,    59,     0,    60,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,     0,
      71,     0,     0,    72,     0,     0,     0,    73,     0,    74,
       4,    75,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    11,    12,    13,     0,    14,    15,    16,    17,  -266,
      18,    19,     0,    20,    21,     0,    22,     0,    23,    24,
      25,    26,    27,   730,     0,    29,    30,     0,  -153,    31,
      32,    33,    34,    35,   -39,    36,    37,     0,    38,     0,
       0,     0,  -266,    39,     0,    41,    42,  -153,    43,    44,
       0,  -266,    45,    46,     0,    47,    48,    49,    50,    51,
      52,     0,    53,     0,    54,    55,    56,     0,    57,    58,
    -266,     0,     0,    59,     0,    60,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,     0,    71,     0,     0,
      72,     0,     0,     0,    73,     0,    74,     4,    75,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    11,    12,
      13,     0,    14,    15,    16,    17,  -266,    18,    19,     0,
      20,    21,     0,    22,     0,    23,    24,    25,    26,    27,
     733,     0,    29,    30,     0,  -153,    31,    32,    33,    34,
      35,   -39,    36,    37,     0,    38,     0,     0,     0,  -266,
      39,     0,    41,    42,  -153,    43,    44,     0,  -266,    45,
      46,     0,    47,    48,    49,    50,    51,    52,     0,    53,
       0,    54,    55,    56,     0,    57,    58,  -266,     0,     0,
      59,     0,    60,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,    71,     0,     0,    72,     0,     0,
       0,    73,     0,    74,     4,    75,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    11,    12,    13,     0,    14,
      15,    16,    17,  -266,    18,    19,     0,    20,    21,     0,
      22,     0,    23,    24,    25,    26,    27,   736,     0,    29,
      30,     0,  -153,    31,    32,    33,    34,    35,   -39,    36,
      37,     0,    38,     0,     0,     0,  -266,    39,     0,    41,
      42,  -153,    43,    44,     0,  -266,    45,    46,     0,    47,
      48,    49,    50,    51,    52,     0,    53,     0,    54,    55,
      56,     0,    57,    58,  -266,     0,     0,    59,     0,    60,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,    71,     0,     0,    72,     0,     0,     0,    73,     0,
      74,     4,    75,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    11,    12,    13,     0,    14,    15,    16,    17,
    -266,    18,    19,     0,   754,    21,     0,    22,     0,    23,
      24,    25,    26,    27,    28,     0,    29,    30,     0,  -153,
      31,    32,    33,    34,    35,   -39,    36,    37,     0,    38,
       0,     0,     0,  -266,    39,     0,    41,    42,  -153,    43,
      44,     0,  -266,    45,    46,     0,    47,    48,    49,    50,
      51,    52,     0,    53,     0,    54,    55,    56,     0,    57,
      58,  -266,     0,     0,    59,     0,    60,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,     0,    71,     0,
       0,    72,     0,     0,     0,    73,     0,    74,     4,    75,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    11,
      12,    13,     0,    14,    15,    16,    17,  -266,    18,    19,
       0,    20,    21,     0,    22,     0,    23,    24,    25,    26,
      27,   758,     0,    29,    30,     0,  -153,    31,    32,    33,
      34,    35,   -39,    36,    37,     0,    38,     0,     0,     0,
    -266,    39,     0,    41,    42,  -153,    43,    44,     0,  -266,
      45,    46,     0,    47,    48,    49,    50,    51,    52,     0,
      53,     0,    54,    55,    56,     0,    57,    58,  -266,     0,
       0,    59,     0,    60,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,     0,    71,     0,     0,    72,     0,
       0,     0,    73,     0,    74,     4,    75,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    11,    12,    13,     0,
      14,    15,    16,    17,  -266,    18,    19,     0,    20,    21,
       0,    22,     0,    23,    24,    25,    26,    27,   762,     0,
      29,    30,     0,  -153,    31,    32,    33,    34,    35,   -39,
      36,    37,     0,    38,     0,     0,     0,  -266,    39,     0,
      41,    42,  -153,    43,    44,     0,  -266,    45,    46,     0,
      47,    48,    49,    50,    51,    52,     0,    53,     0,    54,
      55,    56,     0,    57,    58,  -266,     0,     0,    59,     0,
      60,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,     0,    71,     0,     0,    72,     0,     0,     0,    73,
       0,    74,   513,    75,     5,     6,     7,     8,     9,    10,
       0,  -418,     0,   134,     0,     0,  -418,     0,     0,     0,
       0,     0,     0,     0,  -418,     0,    21,     0,     0,     0,
       0,     0,   135,   136,     0,   137,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,     0,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,  -418,     0,     0,     0,  -418,     0,     0,    49,
      50,    51,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -418,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -418,    63,  -418,  -418,  -418,
       0,  -418,  -418,  -418,     0,  -418,  -418,  -418,  -418,  -418,
    -418,  -418,  -418,  -418,    68,  -418,  -418,  -418,     0,     0,
    -418,  -418,  -418,     0,     0,     0,   139,     0,    74,   513,
     528,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,     0,     0,     0,    72,
       0,     0,     0,   139,     0,    74,   513,   514,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,     0,     0,     0,     0,    72,     0,     0,     0,
     139,     0,    74,     0,   528,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   135,   136,     0,   137,     0,    29,     0,
       0,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,   189,
       0,     0,     0,    72,     0,     0,     0,   139,     0,    74,
       0,   140,   542,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,   189,     0,     0,
       0,    72,     0,     0,     0,   139,     0,    74,     0,   140,
     840,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,     0,     0,     0,    72,
       0,     0,     0,   139,     0,    74,     0,   140,   547,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,   450,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   135,   136,     0,
     137,   451,    29,     0,   452,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,   453,   454,
      39,     0,    41,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,   456,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,     0,     0,     0,    72,     0,     0,
       0,   139,     0,    74,     0,   140,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   135,   136,     0,   137,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,   224,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
     189,     0,     0,     0,    72,     0,     0,     0,   139,     0,
      74,     0,   140,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,   189,     0,     0,
       0,    72,     0,     0,     0,   139,     0,    74,   536,   140,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,   189,     0,     0,     0,    72,     0,
       0,     0,   139,     0,    74,   538,   140,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   135,   136,     0,   137,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,   162,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   163,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,     0,     0,     0,     0,    72,     0,     0,     0,   139,
       0,    74,     0,   140,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
     167,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,   168,   135,   136,     0,   137,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,     0,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    68,     0,    69,    70,     0,     0,
       0,     0,    72,     0,     0,     0,   139,     0,    74,     0,
     140,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,   189,     0,     0,     0,    72,
       0,     0,     0,   139,   534,    74,     0,   140,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   135,   136,     0,   137,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,   189,     0,     0,     0,    72,     0,     0,     0,
     139,     0,    74,     0,   140,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   135,   136,     0,   137,     0,    29,     0,
       0,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,     0,
     193,     0,     0,    72,     0,     0,     0,   139,     0,    74,
       0,   140,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   196,    21,     0,     0,     0,     0,     0,
     135,   136,     0,   137,     0,    29,     0,     0,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    39,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,     0,     0,     0,     0,
      72,     0,     0,     0,    73,     0,    74,     0,   140,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   135,   136,     0,
     137,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,     0,     0,     0,    72,     0,     0,
       0,   139,     0,    74,     0,   140,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   135,   136,     0,   137,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,   645,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,     0,     0,     0,    72,     0,     0,     0,   139,     0,
      74,     0,   140,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   799,     0,     0,     0,     0,
       0,   135,   136,     0,   137,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   800,    50,
      51,   801,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,   189,     0,     0,
       0,    72,     0,     0,     0,   139,     0,    74,     0,   802,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   135,   136,
       0,   137,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,   189,     0,     0,     0,    72,     0,
       0,     0,   139,     0,    74,     0,   802,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   135,   136,     0,   788,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,   189,     0,     0,     0,    72,     0,     0,     0,   139,
       0,    74,     0,   140,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   135,   136,     0,   137,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,     0,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    62,    63,    64,    65,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,    67,     0,    68,     0,    69,    70,     0,     0,
       0,     0,    72,     0,     0,     0,   139,     0,    74,     0,
     140,     5,     6,     7,     8,     9,    10,     0,     0,     0,
     134,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     0,     0,   135,
     136,     0,   137,     0,    29,     0,     0,     0,     0,    32,
      33,     0,    35,     0,    36,    37,     0,     0,     0,     0,
       0,     0,    39,     0,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,   138,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    63,    64,    65,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,    67,
       0,    68,     0,    69,    70,     0,     0,     0,     0,   495,
       0,     0,     0,   139,     0,    74,     0,   140,     5,     6,
       7,     8,     9,    10,     0,     0,     0,   134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,   135,   136,     0,   588,
       0,    29,     0,     0,     0,     0,    32,    33,     0,    35,
       0,    36,    37,     0,     0,     0,     0,     0,     0,    39,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,    65,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,    67,     0,    68,     0,
      69,    70,     0,     0,     0,     0,    72,     0,     0,     0,
     139,     0,    74,     0,   140,     5,     6,     7,     8,     9,
      10,     0,     0,     0,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,     0,     0,
       0,     0,     0,   135,   136,     0,   591,     0,    29,     0,
       0,     0,     0,    32,    33,     0,    35,     0,    36,    37,
       0,     0,     0,     0,     0,     0,    39,     0,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,   138,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    62,    63,    64,    65,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,    67,     0,    68,     0,    69,    70,     0,
       0,     0,     0,    72,     0,     0,     0,   139,     0,    74,
       0,   140,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,     0,     0,     0,     0,     0,
     135,   136,     0,   782,     0,    29,     0,     0,     0,     0,
      32,    33,     0,    35,     0,    36,    37,     0,     0,     0,
       0,     0,     0,    39,     0,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,    63,    64,    65,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,    68,     0,    69,    70,     0,     0,     0,     0,
      72,     0,     0,     0,   139,     0,    74,     0,   140,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   135,   136,     0,
     783,     0,    29,     0,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     0,
      39,     0,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,    63,    64,    65,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,    69,    70,     0,     0,     0,     0,    72,     0,     0,
       0,   139,     0,    74,     0,   140,     5,     6,     7,     8,
       9,    10,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,     0,
       0,     0,     0,     0,   135,   136,     0,   785,     0,    29,
       0,     0,     0,     0,    32,    33,     0,    35,     0,    36,
      37,     0,     0,     0,     0,     0,     0,    39,     0,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    63,    64,
      65,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,    67,     0,    68,     0,    69,    70,
       0,     0,     0,     0,    72,     0,     0,     0,   139,     0,
      74,     0,   140,     5,     6,     7,     8,     9,    10,     0,
       0,     0,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    21,     0,     0,     0,     0,
       0,   135,   136,     0,   786,     0,    29,     0,     0,     0,
       0,    32,    33,     0,    35,     0,    36,    37,     0,     0,
       0,     0,     0,     0,    39,     0,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,   138,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    62,    63,    64,    65,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,    67,     0,    68,     0,    69,    70,     0,     0,     0,
       0,    72,     0,     0,     0,   139,     0,    74,     0,   140,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,     0,     0,     0,     0,     0,   135,   136,
       0,   787,     0,    29,     0,     0,     0,     0,    32,    33,
       0,    35,     0,    36,    37,     0,     0,     0,     0,     0,
       0,    39,     0,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,   138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    63,    64,    65,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,    69,    70,     0,     0,     0,     0,    72,     0,
       0,     0,   139,     0,    74,     0,   140,     5,     6,     7,
       8,     9,    10,     0,     0,     0,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
       0,     0,     0,     0,     0,   135,   136,     0,   788,     0,
      29,     0,     0,     0,     0,    32,    33,     0,    35,     0,
      36,    37,     0,     0,     0,     0,     0,     0,    39,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,   138,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,    63,
      64,    65,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,    68,     0,    69,
      70,     0,     0,     0,     0,    72,     0,     0,     0,   139,
       0,    74,     0,   140,     5,     6,     7,     8,     9,    10,
       0,     0,     0,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,     0,
       0,     0,   135,   136,     0,   137,     0,    29,     0,     0,
       0,     0,    32,    33,     0,    35,     0,    36,    37,     0,
       0,     0,     0,     0,     0,    39,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,   138,     0,   247,     0,     0,     0,     0,     0,
       0,     0,     5,     6,     7,     8,     9,    10,     0,     0,
       0,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    62,    63,    64,    65,     0,
     135,   136,   248,   137,     0,    29,   249,     0,     0,     0,
      32,    33,    67,    35,    68,    36,    37,    70,     0,     0,
       0,     0,    72,    39,     0,    41,   139,     0,    74,     0,
     140,     0,     0,     0,     0,     0,     0,    49,    50,    51,
     138,     0,     0,     0,     0,   251,     0,   252,   253,   254,
       0,   255,   256,   257,     0,   258,   259,   260,   261,  -293,
     263,   264,   265,   266,     0,   267,     0,   269,     0,     0,
     270,   271,   272,     0,    63,     0,     0,     0,  -293,     0,
       0,     0,  -293,     0,     0,     0,     0,     0,   245,     0,
       0,     0,    68,   246,     0,     0,   189,     0,     0,     0,
       0,   247,     0,     0,   139,     0,    74,  -293,   802,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   134,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,     0,     0,     0,   135,   136,   248,
     137,     0,    29,   249,     0,     0,     0,    32,    33,     0,
      35,     0,    36,    37,     0,     0,     0,     0,     0,     5,
      39,     0,    41,   325,     0,     0,   667,   250,     0,     0,
       0,   668,     0,     0,    49,    50,    51,   138,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,     0,     0,   270,   271,   272,
       0,    63,     0,     0,   245,     0,     0,     0,   658,   246,
       0,     0,     0,     0,     0,     0,     0,   247,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,   140,   669,   670,   671,   672,
     673,   674,     0,     0,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,     0,   248,   685,     0,     0,   249,
     686,   687,   688,   689,   690,   691,   692,   693,   694,   695,
     696,     0,   697,     0,     0,   698,   699,   700,   701,     0,
     702,   245,     0,   250,     0,     0,   246,     0,     0,     0,
       0,     0,     0,     0,   247,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,   267,   268,
     269,     0,     0,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   248,     0,   720,     0,   249,     0,     0,     0,
       0,     0,     0,   245,     0,     0,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,   247,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,   252,   253,   254,
       0,   255,   256,   257,     0,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   248,   267,   268,   269,   249,     0,
     270,   271,   272,     0,     0,   245,     0,     0,     0,     0,
     246,   849,     0,     0,     0,     0,     0,     0,   247,     0,
       0,     0,   250,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,     0,   252,
     253,   254,     0,   255,   256,   257,     0,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   248,   267,   268,   269,
     249,     0,   270,   271,   272,     0,     0,   245,     0,     0,
       0,     0,   246,   885,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,   252,   253,   254,     0,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   248,   267,
     268,   269,   249,     0,   270,   271,   272,     0,     0,   245,
       0,     0,     0,   535,   246,     0,     0,     0,     0,     0,
       0,     0,   247,   323,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,   324,     0,     0,     0,     0,     0,
       0,   251,     0,   252,   253,   254,     0,   255,   256,   257,
       0,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     248,   267,   268,   269,   249,     0,   270,   271,   272,     0,
       0,     0,     0,     0,     0,   822,   245,     0,     0,     0,
       0,   246,     0,     0,   325,     0,     0,     0,   250,   247,
     320,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   321,     0,   251,     0,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,   267,   268,   269,     0,   248,   270,   271,
     272,   249,     0,     0,   296,     0,     0,     0,   245,     0,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
       0,   247,   618,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   248,
     267,   268,   269,   249,     0,   270,   271,   272,     0,     0,
       0,   296,     0,     0,     0,   245,     0,     0,     0,     0,
     246,     0,     0,   325,     0,     0,     0,   250,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,     0,   248,   270,   271,   272,
     249,     0,     0,   296,     0,     0,     0,   331,     0,     0,
       0,     0,   245,     0,     0,     0,     0,   246,     0,     0,
       0,     0,     0,     0,   250,   247,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,   252,   253,   254,     0,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,     0,   248,   270,   271,   272,   249,     0,     0,
     296,     0,     0,     0,   245,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,   247,   612,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   248,   267,   268,   269,   249,
       0,   270,   271,   272,     0,     0,   245,   296,     0,     0,
       0,   246,     0,     0,     0,     0,     0,     0,     0,   247,
       0,     0,     0,   250,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   248,   267,   268,
     269,   249,     0,   270,   271,   272,     0,     0,   727,   296,
       0,     0,     0,   245,     0,     0,     0,     0,   246,     0,
       0,     0,     0,     0,     0,   250,   247,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,     0,
     267,   268,   269,     0,   248,   270,   271,   272,   249,     0,
       0,   296,     0,     0,     0,   732,     0,     0,     0,     0,
     245,     0,     0,     0,     0,   246,     0,     0,     0,     0,
       0,     0,   250,   247,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   251,     0,   252,
     253,   254,     0,   255,   256,   257,     0,   258,   259,   260,
     261,   262,   263,   264,   265,   266,     0,   267,   268,   269,
       0,   248,   270,   271,   272,   249,     0,     0,   296,     0,
       0,     0,   834,     0,     0,     0,     0,   245,     0,     0,
       0,     0,   246,     0,     0,     0,     0,     0,     0,   250,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,   252,   253,   254,     0,
     255,   256,   257,     0,   258,   259,   260,   261,   262,   263,
     264,   265,   266,     0,   267,   268,   269,     0,   248,   270,
     271,   272,   249,     0,     0,   296,     0,     0,     0,   835,
       0,     0,     0,     0,   245,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,   250,   247,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,   252,   253,   254,     0,   255,   256,   257,
       0,   258,   259,   260,   261,   262,   263,   264,   265,   266,
       0,   267,   268,   269,     0,   248,   270,   271,   272,   249,
       0,     0,   296,     0,     0,     0,   836,     0,     0,     0,
       0,   245,     0,     0,     0,     0,   246,     0,     0,     0,
       0,     0,     0,   250,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,   267,   268,
     269,     0,   248,   270,   271,   272,   249,     0,     0,   296,
       0,     0,     0,   837,     0,     0,     0,     0,   245,     0,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
     250,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,   252,   253,   254,
       0,   255,   256,   257,     0,   258,   259,   260,   261,   262,
     263,   264,   265,   266,     0,   267,   268,   269,     0,   248,
     270,   271,   272,   249,     0,     0,   296,     0,     0,     0,
     850,     0,     0,     0,     0,   245,     0,     0,     0,     0,
     246,     0,     0,     0,     0,     0,     0,   250,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,     0,   248,   270,   271,   272,
     249,     0,     0,   296,     0,     0,     0,   851,     0,     0,
       0,     0,   245,     0,     0,     0,     0,   246,     0,     0,
       0,     0,     0,     0,   250,   247,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,   252,   253,   254,     0,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,     0,   248,   270,   271,   272,   249,     0,     0,
     296,     0,     0,     0,   245,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,   247,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   248,   267,   268,   269,   249,
       0,   270,   271,   272,     0,     0,     0,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   245,     0,   250,     0,     0,   246,     0,     0,     0,
       0,     0,     0,     0,   247,     0,     0,     0,   251,   328,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,   267,   268,
     269,   329,     0,   270,   271,   272,     0,     0,  -378,   245,
       0,     0,   248,     0,   246,     0,   249,     0,     0,     0,
       0,     0,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   302,     0,     0,     0,     0,     0,
     250,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,   252,   253,   254,
     248,   255,   256,   257,   249,   258,   259,   260,   261,   262,
     263,   264,   265,   266,     0,   267,   268,   269,     0,   330,
     270,   271,   272,     0,   128,     0,   245,     0,   250,     0,
       0,   246,     0,     0,     0,     0,     0,     0,     0,   247,
       0,     0,     0,   251,   365,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,   267,   268,   269,   366,     0,   270,   271,
     272,     0,     0,     0,   245,     0,     0,   248,     0,   246,
       0,   249,     0,     0,     0,     0,     0,   247,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,     0,     0,     0,     0,   250,     0,     0,   245,     0,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
     251,   247,   252,   253,   254,   248,   255,   256,   257,   249,
     258,   259,   260,   261,   262,   263,   264,   265,   266,     0,
     267,   268,   269,     0,     0,   270,   271,   272,     0,     0,
       0,     0,   245,   250,     0,     0,     0,   246,     0,   248,
       0,     0,     0,   249,     0,   247,   464,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,   465,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   250,   267,   268,
     269,     0,     0,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   251,   248,   252,   253,   254,   249,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,   245,   306,   270,   271,   272,
     246,   250,     0,     0,     0,     0,     0,     0,   247,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,   267,   268,   269,   245,
       0,   270,   271,   272,   246,     0,   248,     0,     0,     0,
     249,     0,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,   250,     0,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,   247,     0,     0,   251,
     248,   252,   253,   254,   249,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,     0,   354,   270,   271,   272,   245,   250,     0,
       0,     0,   246,     0,   248,     0,     0,     0,   249,     0,
     247,     0,     0,   251,     0,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   250,   267,   268,   269,     0,   360,   270,   271,
     272,     0,     0,     0,     0,     0,     0,   251,   248,   252,
     253,   254,   249,   255,   256,   257,     0,   258,   259,   260,
     261,   262,   263,   264,   265,   266,     0,   267,   268,   269,
       0,   368,   270,   271,   272,   382,   250,   245,     0,     0,
       0,     0,   246,     0,     0,     0,     0,     0,     0,     0,
     247,   251,     0,   252,   253,   254,     0,   255,   256,   257,
       0,   258,   259,   260,   261,   262,   263,   264,   265,   266,
       0,   267,   268,   269,     0,     0,   270,   271,   272,     0,
       0,   245,     0,     0,     0,     0,   246,     0,   248,     0,
       0,     0,   249,     0,   247,     0,     0,     0,     0,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,   250,     0,     0,     0,
     246,     0,     0,     0,     0,     0,     0,     0,   247,     0,
       0,   251,   248,   252,   253,   254,   249,   255,   256,   257,
       0,   258,   259,   260,   261,   262,   263,   264,   265,   266,
       0,   267,   268,   269,     0,     0,   270,   271,   272,   245,
     250,     0,     0,     0,   246,     0,   248,     0,     0,     0,
     249,     0,   247,     0,     0,   251,     0,   252,   253,   254,
       0,   255,   256,   257,     0,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   250,   267,   268,   269,     0,   563,
     270,   271,   272,     0,     0,     0,     0,     0,     0,   251,
     248,   252,   253,   254,   249,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,   245,   564,   270,   271,   272,   246,   250,     0,
       0,     0,     0,     0,     0,   247,     0,     0,     0,     0,
       0,     0,     0,   251,     0,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,   267,   268,   269,   245,   565,   270,   271,
     272,   246,     0,   248,     0,     0,     0,   249,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   250,     0,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,   251,   248,   252,   253,
     254,   249,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,   267,   268,   269,     0,
     566,   270,   271,   272,   245,   250,     0,     0,     0,   246,
       0,   248,     0,     0,     0,   249,     0,   247,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   250,
     267,   268,   269,     0,   567,   270,   271,   272,     0,     0,
       0,     0,     0,     0,   251,   248,   252,   253,   254,   249,
     255,   256,   257,     0,   258,   259,   260,   261,   262,   263,
     264,   265,   266,     0,   267,   268,   269,   245,   568,   270,
     271,   272,   246,   250,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,   267,   268,
     269,   245,   569,   270,   271,   272,   246,     0,   248,     0,
       0,     0,   249,     0,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,   250,     0,     0,     0,
     246,     0,     0,     0,     0,     0,     0,     0,   247,     0,
       0,   251,   248,   252,   253,   254,   249,   255,   256,   257,
       0,   258,   259,   260,   261,   262,   263,   264,   265,   266,
       0,   267,   268,   269,     0,   570,   270,   271,   272,   245,
     250,     0,     0,     0,   246,     0,   248,     0,     0,     0,
     249,     0,   247,     0,     0,   251,     0,   252,   253,   254,
       0,   255,   256,   257,     0,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   250,   267,   268,   269,     0,   571,
     270,   271,   272,     0,     0,     0,     0,     0,     0,   251,
     248,   252,   253,   254,   249,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,   245,   572,   270,   271,   272,   246,   250,     0,
       0,     0,     0,     0,     0,   247,     0,     0,     0,     0,
       0,     0,     0,   251,     0,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,   267,   268,   269,   245,   573,   270,   271,
     272,   246,     0,   248,     0,     0,     0,   249,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   250,     0,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,   251,   248,   252,   253,
     254,   249,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,   267,   268,   269,     0,
     574,   270,   271,   272,   245,   250,     0,     0,     0,   246,
       0,   248,     0,     0,     0,   249,     0,   247,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   250,
     267,   268,   269,     0,   575,   270,   271,   272,     0,     0,
       0,     0,     0,     0,   251,   248,   252,   253,   254,   249,
     255,   256,   257,     0,   258,   259,   260,   261,   262,   263,
     264,   265,   266,     0,   267,   268,   269,   245,   576,   270,
     271,   272,   246,   250,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,   267,   268,
     269,     0,   577,   270,   271,   272,     0,     0,   248,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,   246,     0,     0,     0,     0,     0,
       0,     0,   247,     0,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,   252,   253,   254,     0,   255,   256,   257,
       0,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     248,   267,   268,   269,   249,   578,   270,   271,   272,     0,
       0,     0,     0,     0,     0,     0,   245,     0,     0,     0,
       0,   246,     0,     0,   128,     0,     0,     0,   250,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   251,     0,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,   267,   268,   269,     0,   248,   270,   271,
     272,   249,     0,     0,     0,     0,     0,     0,   245,   625,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   248,
     267,   268,   269,   249,   598,   270,   271,   272,     0,     0,
     245,     0,     0,     0,     0,   246,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,   628,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   248,   267,   268,   269,   249,     0,   270,   271,   272,
       0,     0,   245,     0,     0,     0,     0,   246,     0,     0,
       0,     0,     0,     0,     0,   247,     0,     0,     0,   250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,   252,   253,   254,     0,
     255,   256,   257,     0,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   248,   267,   268,   269,   249,     0,   270,
     271,   272,     0,     0,   245,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,   247,   715,     0,
     661,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   248,   267,   268,   269,   249,
       0,   270,   271,   272,     0,     0,   245,     0,     0,     0,
       0,   246,     0,     0,     0,     0,     0,     0,     0,   247,
     718,     0,     0,   250,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   248,   267,   268,
     269,   249,     0,   270,   271,   272,     0,     0,   245,     0,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   248,
     267,   268,   269,   249,     0,   270,   271,   272,     0,     0,
     781,     0,     0,     0,     0,   245,     0,     0,     0,     0,
     246,     0,     0,     0,     0,     0,     0,   250,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,     0,   248,   270,   271,   272,
     249,     0,     0,     0,     0,     0,     0,   784,     0,     0,
       0,     0,   245,   797,     0,     0,     0,   246,     0,     0,
       0,     0,     0,     0,   250,   247,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,   252,   253,   254,     0,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,     0,   248,   270,   271,   272,   249,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,   247,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   248,   267,   268,   269,   249,
       0,   270,   271,   272,     0,     0,   857,     0,     0,     0,
       0,   245,     0,     0,     0,     0,   246,     0,     0,     0,
       0,     0,     0,   250,   247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
     252,   253,   254,     0,   255,   256,   257,     0,   258,   259,
     260,   261,   262,   263,   264,   265,   266,     0,   267,   268,
     269,     0,   248,   270,   271,   272,   249,     0,     0,     0,
       0,     0,     0,   858,     0,     0,     0,     0,   245,     0,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
     250,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,   252,   253,   254,
       0,   255,   256,   257,     0,   258,   259,   260,   261,   262,
     263,   264,   265,   266,     0,   267,   268,   269,     0,   248,
     270,   271,   272,   249,     0,     0,     0,     0,     0,     0,
     859,     0,     0,     0,     0,   245,     0,     0,     0,     0,
     246,     0,     0,     0,     0,     0,     0,   250,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,     0,   248,   270,   271,   272,
     249,     0,     0,     0,     0,     0,     0,   860,     0,     0,
       0,     0,   245,     0,     0,     0,     0,   246,     0,     0,
       0,     0,     0,     0,   250,   247,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,   252,   253,   254,     0,   255,   256,   257,     0,   258,
     259,   260,   261,   262,   263,   264,   265,   266,     0,   267,
     268,   269,     0,   248,   270,   271,   272,   249,     0,     0,
       0,     0,     0,     0,   861,     0,     0,     0,     0,   245,
       0,     0,     0,     0,   246,     0,     0,     0,     0,     0,
       0,   250,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   251,     0,   252,   253,
     254,     0,   255,   256,   257,     0,   258,   259,   260,   261,
     262,   263,   264,   265,   266,     0,   267,   268,   269,     0,
     248,   270,   271,   272,   249,     0,     0,     0,     0,     0,
       0,   862,     0,     0,     0,     0,   245,     0,     0,     0,
       0,   246,     0,     0,     0,     0,     0,     0,   250,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   251,     0,   252,   253,   254,     0,   255,
     256,   257,     0,   258,   259,   260,   261,   262,   263,   264,
     265,   266,     0,   267,   268,   269,     0,   248,   270,   271,
     272,   249,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,   246,     0,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,     0,   252,   253,   254,     0,   255,   256,   257,     0,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   586,
     267,   268,   269,   249,     0,   270,   271,   272,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   251,     0,   252,   253,   254,     0,   255,   256,
     257,     0,   258,   259,   260,   261,   262,   263,   264,   265,
     266,     0,   267,   268,   269,     0,     0,   270,   271,   272
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-789))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,     2,   293,   335,   642,    16,    23,   361,    19,    42,
      43,   338,     1,    15,    25,    26,    27,    28,     3,    64,
     150,   229,     3,   512,    35,    36,     3,    38,    39,     1,
     160,    24,     3,   707,    45,    46,    47,    48,    49,   664,
     493,    52,    53,   239,   240,   241,   242,    58,    59,    60,
      85,   109,    63,    51,   810,    66,    67,    68,    45,    70,
      24,    35,    73,    74,    75,   164,   165,   166,    24,     1,
     664,    58,    24,   109,   840,    64,    90,   343,     1,    85,
     109,    62,   140,   182,    82,    74,   874,   137,    75,   139,
     135,   109,    64,    78,    83,   361,    16,   885,     3,   113,
     199,    35,   138,    35,   172,   140,    35,    35,   874,   138,
     748,    83,    35,   212,    35,    20,   109,   185,    35,   108,
     138,    85,   878,   134,   135,   136,   137,   138,   139,   140,
      35,   584,    13,    38,   202,   109,   108,    18,   136,   128,
      60,     0,   135,   154,   146,    64,   135,    52,    53,   638,
     824,   825,   137,   140,    39,   109,   137,    62,    90,    79,
     137,   172,    25,   135,   789,    88,   137,    90,    85,   186,
     329,   135,    77,    58,   185,   109,   108,   109,   772,   135,
     109,   109,   136,   135,   511,   108,   109,    35,   109,     3,
       3,   202,   109,     7,   390,   789,   128,   109,   109,   109,
     109,   839,   493,   109,   303,   128,   140,   366,    88,   128,
      90,   140,   140,   224,    62,   137,   135,   139,   128,   140,
     221,   289,   290,   140,    77,     7,   138,   138,   327,   138,
     136,    70,   137,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   137,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   491,   492,   774,   739,   109,    20,   109,
     358,   302,     3,   584,    20,    20,   307,   109,   630,     3,
      90,   137,    23,     7,   137,   632,   128,   318,   128,   320,
     321,   137,   323,   324,    25,   109,    20,   328,   329,   109,
     331,    53,   108,   113,   109,   111,   138,    53,    53,   115,
      62,   318,   540,   541,   128,   346,    62,    62,   109,    61,
      61,   352,    61,    65,    65,    77,    65,   358,   138,    53,
      45,    77,     2,   138,   365,   366,    81,   128,    62,   109,
     371,    11,   109,   374,    20,   376,    77,   378,   379,   380,
      20,   382,   383,    77,   128,   641,   387,    81,   128,    35,
     128,   137,    38,   139,    34,   248,   249,   108,   487,   488,
     111,   138,   491,   492,   115,   109,    52,    53,    19,   753,
     387,   531,    52,   757,    54,    55,    62,   134,    29,    30,
     128,   128,    90,   374,   128,   376,    37,   109,    39,   128,
     137,    77,   139,    73,    45,   128,     3,    77,   291,   292,
       7,   109,     9,   135,     3,   113,   128,    58,     7,   302,
     128,   128,   128,    20,   137,   128,   778,    84,   739,   111,
      23,   462,   463,   464,   465,   466,   467,   468,   321,   135,
     138,   324,   137,   474,   475,   137,   477,   469,   470,   137,
      39,    90,   483,   128,   485,     7,    53,   108,   489,   129,
     128,   747,   493,   109,   138,    62,   128,   753,    61,    58,
      35,   757,    65,   137,   109,    26,    88,   596,   597,   138,
      77,    90,   138,   514,    81,   138,   517,   518,   607,   138,
     109,   135,   140,   135,    90,   378,     2,   528,   617,   128,
     138,   620,   109,   173,   140,    11,   128,   514,   128,    26,
     138,   104,   138,   128,    20,   108,    62,   110,   111,   109,
      23,   528,   115,   128,    24,    24,   196,   140,    34,   122,
     108,   135,    62,    83,   140,   111,   129,   130,   131,   140,
     111,    74,   137,     2,   138,   549,    52,   366,    54,    55,
     365,   221,    11,   584,   840,   586,   587,   588,    61,   161,
     591,    20,    65,   849,   482,   725,   777,    73,   604,   777,
     630,    77,   603,   778,   666,    34,   878,   839,   853,   610,
     463,   612,   465,    27,   615,   347,   798,   618,   874,   477,
      -1,    -1,   623,    52,   625,    54,    55,   628,    -1,   885,
      -1,    -1,     2,    -1,    -1,   108,    -1,   110,   111,    -1,
      -1,    11,   115,    -1,    73,    -1,    -1,   648,    77,   122,
      20,    -1,    -1,   129,    -1,   656,   296,   658,   131,    19,
     661,    -1,    -1,    -1,    34,    -1,    -1,   756,    -1,    29,
      30,   648,   663,    -1,    -1,    -1,    -1,    37,    -1,    39,
     320,    -1,    52,   323,    54,    55,    -1,    -1,    -1,    -1,
      -1,   331,    -1,    -1,    -1,    -1,    -1,   173,    58,    -1,
     129,    -1,   703,    73,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,   714,   715,    -1,   717,   718,   719,   720,
     196,    -1,    23,    -1,    -1,    -1,   727,   728,    -1,   730,
      -1,   732,   733,   586,   587,   736,    -1,    -1,   739,    -1,
     380,    -1,    -1,    -1,   173,   221,   769,   770,    -1,    -1,
      -1,   752,   753,    -1,    -1,    -1,   757,   758,    -1,   129,
      61,   762,    -1,    -1,    65,    -1,    -1,   196,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     781,   782,   783,   784,   785,   786,   787,   788,    -1,    -1,
      -1,    -1,   221,    -1,    -1,    -1,   797,    -1,    -1,   800,
     801,   802,    -1,   173,    -1,   806,    -1,   108,   809,   110,
     111,    -1,    -1,    -1,   115,    -1,    -1,   457,    -1,    -1,
     296,   122,    -1,    -1,    -1,   802,   196,    -1,   129,   130,
     131,    -1,    -1,   834,   835,   836,   837,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   320,    -1,    -1,   323,   849,   850,
     851,   221,    -1,    -1,    -1,   331,   857,   858,   859,   860,
     861,   862,    -1,   503,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,   873,   874,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   885,    -1,    -1,    -1,    -1,    -1,
      -1,   320,    -1,    -1,   323,    -1,    -1,    16,    -1,    -1,
      19,    -1,   331,    64,   380,   545,    -1,    -1,    27,   549,
      29,    30,    -1,    74,    33,    -1,    -1,   557,    37,    -1,
      39,    -1,    83,    -1,    -1,    -1,   296,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    35,    57,    58,
      59,    60,    -1,    -1,    -1,    19,   586,   587,   109,    -1,
     320,   380,    -1,   323,    -1,    29,    30,    35,    77,    48,
      79,   331,    -1,    37,    23,    39,    64,   128,    -1,    -1,
     610,    45,   612,    -1,   135,   615,    74,    -1,   618,    -1,
      -1,    -1,    -1,    -1,    58,    83,    64,   627,    -1,    -1,
       2,    -1,    90,    -1,    -1,    -1,    74,    -1,    -1,    11,
      -1,    -1,    61,    -1,    -1,    83,    65,   647,    20,   128,
     380,   109,    90,    -1,    -1,    -1,   656,   136,   658,    -1,
      -1,    -1,    34,   663,    -1,    -1,    -1,   503,    -1,    -1,
     128,   109,    -1,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      52,    -1,    54,    55,   133,   104,    -1,   106,   107,   108,
     128,   110,   111,   112,    -1,    -1,   115,   135,    -1,    -1,
      23,    73,   121,   122,    -1,    77,    -1,   126,    -1,   545,
     129,   130,   131,   549,   503,   164,   165,   166,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,   727,    -1,    -1,
      -1,    -1,   732,   182,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     199,    -1,    -1,    -1,   754,    -1,   545,   129,    -1,    -1,
     549,   761,    -1,   212,    -1,   765,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   503,   610,    -1,   612,    -1,    -1,   615,
      -1,   104,   618,   106,   107,   108,    -1,   110,   111,   112,
      -1,   627,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,   173,    -1,   126,    -1,    -1,   129,   130,   131,    -1,
      -1,   647,    -1,    -1,    -1,   545,    -1,    -1,    -1,   549,
     656,   610,   658,   612,   196,    -1,   615,   663,    -1,   618,
      -1,    -1,    -1,    -1,   834,   835,   836,   837,   627,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   221,
     850,   851,    -1,    -1,   303,    -1,    -1,    74,   647,    -1,
      -1,     2,    -1,    -1,    -1,    -1,    -1,   656,    -1,   658,
      11,    -1,    -1,    -1,   663,    -1,    -1,    -1,   327,    20,
     610,    -1,   612,    -1,    -1,   615,    -1,    -1,   618,     2,
      -1,   727,    -1,    34,    -1,    -1,   732,   627,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,   647,   754,    -1,
      -1,    34,    -1,    -1,   296,   761,   656,    -1,   658,   765,
      -1,    -1,    73,   663,    -1,    -1,    77,   154,   727,    52,
      -1,    54,    55,   732,    -1,    -1,    -1,    -1,   320,    -1,
      -1,   323,    -1,    -1,    -1,   172,    -1,    -1,    -1,   331,
      73,    -1,    -1,    -1,    77,   754,    -1,    -1,   185,    -1,
      -1,    -1,   761,    -1,    -1,    -1,   765,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   727,   834,   835,
     836,   837,   732,    -1,    -1,    -1,    -1,    -1,   380,    -1,
      -1,    23,    -1,    -1,   850,   851,   129,    -1,     2,    -1,
      -1,    -1,    -1,    -1,   754,    -1,    -1,    11,    -1,    -1,
      -1,   761,   173,    -1,    -1,   765,    20,    -1,   487,   488,
      -1,    -1,   491,   492,    -1,   834,   835,   836,   837,    61,
      34,    -1,    -1,    65,    -1,   196,    -1,    -1,    -1,    -1,
     173,   850,   851,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      54,    55,   289,   290,    -1,    -1,    -1,    -1,    -1,    -1,
     221,    -1,    -1,   196,    -1,    -1,    -1,    -1,    -1,    73,
      -1,    -1,   104,    77,   106,   107,   108,    -1,   110,   111,
      -1,    -1,    -1,   115,   834,   835,   836,   837,   221,   121,
     122,    -1,    -1,    -1,   126,    -1,    -1,   129,   130,   131,
     850,   851,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   503,    -1,    -1,    -1,   352,    -1,    -1,    -1,    -1,
      -1,   358,    -1,    -1,    -1,   129,    -1,   596,   597,    -1,
      -1,    -1,    -1,    16,   371,   296,    19,   374,   607,   376,
     609,    -1,   379,    -1,    27,   614,    29,    30,   617,    -1,
      33,   620,    -1,   545,    37,    -1,    39,   549,    -1,   320,
      -1,    -1,   323,   296,    -1,    -1,    -1,    -1,    16,   173,
     331,    19,    55,    -1,    57,    58,    59,    60,    -1,    27,
      -1,    29,    30,    -1,    -1,    33,    -1,   320,    -1,    37,
     323,    39,   196,    -1,    77,    -1,    79,    -1,   331,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    57,
      58,    59,    60,    -1,    -1,    -1,    -1,   221,   610,   380,
     612,    -1,    -1,   615,    -1,    23,   618,    -1,    -1,    77,
      -1,    79,    -1,    -1,    -1,   627,    -1,    -1,   475,    -1,
     477,    -1,    -1,    -1,    -1,   128,    -1,   380,    -1,    -1,
      -1,    -1,    -1,   136,    -1,   647,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,   656,    -1,   658,    65,     2,    -1,
      -1,   663,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    20,   756,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,    -1,    -1,   320,   115,    52,   323,
      54,    55,   791,   792,   122,   794,   795,   331,    -1,    -1,
      23,   129,   130,   131,    -1,   727,    -1,    -1,    -1,    73,
     732,   810,   503,    77,    -1,    -1,    -1,   816,    -1,   818,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   754,    -1,    -1,    -1,    -1,    -1,    61,   761,
     503,    -1,    65,   765,    -1,    -1,   380,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   545,    -1,    -1,    -1,   549,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   878,
      -1,   104,   545,   106,   107,   108,   549,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,    -1,   129,   130,   131,   173,
      -1,    -1,   834,   835,   836,   837,    -1,    -1,    -1,   610,
      23,   612,    -1,    -1,   615,    -1,    -1,   618,   850,   851,
      -1,    -1,   196,    -1,    -1,    -1,   627,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   610,    -1,   612,
      -1,    -1,   615,    -1,    -1,   618,   647,   221,    61,    -1,
      -1,    -1,    65,    -1,   627,   656,    -1,   658,    -1,   503,
      -1,    -1,   663,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     747,    -1,    -1,    -1,   647,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   656,    -1,   658,    -1,    -1,    -1,    -1,
     663,   104,    -1,    -1,   107,   108,    -1,   110,   111,    -1,
      -1,   545,   115,    -1,    -1,   549,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,   296,    -1,    -1,    -1,   727,    -1,    -1,   806,
      -1,   732,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,   323,
      -1,    -1,    -1,   754,   727,    -1,    -1,    -1,    -1,   732,
     761,    -1,    -1,   840,   765,    -1,   610,    -1,   612,    -1,
      -1,   615,    -1,    -1,   618,    13,    -1,    -1,    -1,    -1,
      18,   754,    -1,   627,    22,    23,    24,    -1,   761,    -1,
      -1,    -1,   765,    31,    -1,    33,    -1,   874,    -1,    -1,
      -1,    -1,    -1,   647,    -1,    -1,   380,    -1,   885,    -1,
      -1,    -1,   656,    -1,   658,    -1,    -1,    -1,    56,   663,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   834,   835,   836,   837,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   850,
     851,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,   834,   835,   836,   837,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   850,   851,    -1,
      -1,    -1,    -1,   727,    -1,    -1,    -1,    -1,   732,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     754,    -1,   150,    -1,    -1,    -1,   154,   761,    -1,    -1,
      -1,   765,   160,    -1,   162,    -1,    -1,    -1,    -1,   503,
      -1,   132,   133,   134,   172,    -1,   137,   175,   139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   202,    -1,    -1,    -1,    -1,    -1,
      -1,   545,    -1,    -1,    -1,   549,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     834,   835,   836,   837,    -1,    -1,   234,    -1,    -1,    -1,
      -1,   239,   240,   241,   242,    -1,   850,   851,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,    -1,    10,   610,    12,   612,    -1,
      15,   615,    -1,    -1,   618,    -1,    -1,    -1,    23,    -1,
      25,   289,   290,   627,    -1,    -1,    31,    32,    -1,    34,
      -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,
      -1,    46,    47,   647,    -1,    -1,   314,    -1,   316,    54,
      -1,    56,   656,    -1,   658,    -1,    61,    -1,    -1,   663,
      65,    -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,
     338,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,
      -1,    -1,    -1,    88,    89,     3,    -1,    -1,    -1,    -1,
     358,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,   104,
     105,   106,   107,   108,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   390,    -1,   129,   130,   131,    -1,    -1,    -1,
     135,    -1,   137,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     754,    -1,    -1,    -1,    -1,    -1,    -1,   761,    -1,    -1,
      -1,   765,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
      -1,    -1,   110,    -1,    -1,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,    -1,   126,   477,
      -1,   129,   130,   131,   132,    -1,   134,    -1,    -1,   137,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,   507,
      12,    13,    14,   511,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    34,   531,    36,    37,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,   557,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      23,    73,    -1,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    -1,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,   604,    -1,    61,    -1,
     112,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,   128,    -1,    -1,   131,
      -1,    -1,    -1,   135,   632,   137,    89,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
      -1,   114,   115,   116,   117,    -1,   119,   120,   121,   122,
      -1,   124,    -1,   126,    -1,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,     1,   682,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      -1,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    -1,    -1,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    73,    -1,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,
     768,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   777,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,    -1,
     128,   129,   130,   131,    -1,    -1,    -1,   135,   136,   137,
       1,   139,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    -1,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    -1,    86,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,   125,   126,   127,   128,    -1,    -1,
     131,    -1,    -1,    -1,   135,   136,   137,     1,   139,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    -1,    73,
      -1,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    -1,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,   128,    -1,    -1,   131,    -1,    -1,
      -1,   135,   136,   137,     1,   139,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    34,    -1,    36,
      37,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    -1,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    -1,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,
      -1,   128,    -1,    -1,   131,    -1,    -1,    -1,   135,   136,
     137,     1,   139,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,    -1,    36,    37,    -1,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    -1,    73,    -1,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    -1,    86,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,    -1,   128,    -1,
      -1,   131,    -1,    -1,    -1,   135,   136,   137,     1,   139,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    -1,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    -1,    86,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,   126,    -1,   128,    -1,    -1,   131,    -1,
      -1,    -1,   135,   136,   137,     1,   139,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    34,    -1,
      36,    37,    -1,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    -1,    73,    -1,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,    -1,   128,    -1,    -1,   131,    -1,    -1,    -1,   135,
      -1,   137,     1,   139,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    24,    25,    -1,    27,    -1,
      29,    30,    31,    32,    33,    34,    -1,    36,    37,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    -1,    -1,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    -1,    66,    67,    68,
      69,    70,    71,    -1,    73,    -1,    75,    76,    77,    -1,
      79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,   126,    -1,   128,
      -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,     1,
     139,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    -1,    24,    25,    -1,    27,    -1,    29,    30,    31,
      32,    33,    34,    -1,    36,    37,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    -1,    -1,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    -1,    66,    67,    68,    69,    70,    71,
      -1,    73,    -1,    75,    76,    77,    -1,    79,    80,    81,
      -1,    -1,    84,    -1,    86,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,   128,    -1,    -1,   131,
      -1,    -1,    -1,   135,    -1,   137,     1,   139,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    24,
      25,    -1,    27,    -1,    29,    30,    31,    32,    33,    34,
      -1,    36,    37,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      -1,    66,    67,    68,    69,    70,    71,    -1,    73,    -1,
      75,    76,    77,    -1,    79,    80,    81,    -1,    -1,    84,
      -1,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
     125,   126,    -1,   128,    -1,    -1,   131,    -1,    -1,    -1,
     135,    -1,   137,     1,   139,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,    17,
      18,    19,    20,    21,    22,    -1,    24,    25,    -1,    27,
      -1,    29,    30,    31,    32,    33,    34,    -1,    36,    37,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    -1,    -1,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    -1,    66,    67,
      68,    69,    70,    71,    -1,    73,    -1,    75,    76,    77,
      -1,    79,    80,    81,    -1,    -1,    84,    -1,    86,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,    -1,
     128,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,
       1,   139,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,    25,    -1,    27,    -1,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    -1,
      -1,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    -1,    66,    67,    68,    69,    70,
      71,    -1,    73,    -1,    75,    76,    77,    -1,    79,    80,
      81,    -1,    -1,    84,    -1,    86,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,   125,   126,    -1,   128,    -1,    -1,
     131,    -1,    -1,    -1,   135,    -1,   137,     1,   139,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      24,    25,    -1,    27,    -1,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    -1,    -1,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    -1,    66,    67,    68,    69,    70,    71,    -1,    73,
      -1,    75,    76,    77,    -1,    79,    80,    81,    -1,    -1,
      84,    -1,    86,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,   128,    -1,    -1,   131,    -1,    -1,
      -1,   135,    -1,   137,     1,   139,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    24,    25,    -1,
      27,    -1,    29,    30,    31,    32,    33,    34,    -1,    36,
      37,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    -1,    66,
      67,    68,    69,    70,    71,    -1,    73,    -1,    75,    76,
      77,    -1,    79,    80,    81,    -1,    -1,    84,    -1,    86,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,
      -1,   128,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,
     137,     1,   139,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    -1,    24,    25,    -1,    27,    -1,    29,
      30,    31,    32,    33,    34,    -1,    36,    37,    -1,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      -1,    -1,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    71,    -1,    73,    -1,    75,    76,    77,    -1,    79,
      80,    81,    -1,    -1,    84,    -1,    86,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,    -1,   128,    -1,
      -1,   131,    -1,    -1,    -1,   135,    -1,   137,     1,   139,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    24,    25,    -1,    27,    -1,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    -1,    -1,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    -1,    66,    67,    68,    69,    70,    71,    -1,
      73,    -1,    75,    76,    77,    -1,    79,    80,    81,    -1,
      -1,    84,    -1,    86,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,   126,    -1,   128,    -1,    -1,   131,    -1,
      -1,    -1,   135,    -1,   137,     1,   139,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    24,    25,
      -1,    27,    -1,    29,    30,    31,    32,    33,    34,    -1,
      36,    37,    -1,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    -1,    -1,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    68,    69,    70,    71,    -1,    73,    -1,    75,
      76,    77,    -1,    79,    80,    81,    -1,    -1,    84,    -1,
      86,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,    -1,   128,    -1,    -1,   131,    -1,    -1,    -1,   135,
      -1,   137,     1,   139,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    12,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,
      -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,    -1,
     129,   130,   131,    -1,    -1,    -1,   135,    -1,   137,     1,
     139,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,    -1,    -1,   135,    -1,   137,     1,   139,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,
      -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,
      -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
     135,    -1,   137,    -1,   139,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,   127,
      -1,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,
      -1,   139,   140,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,   127,    -1,    -1,
      -1,   131,    -1,    -1,    -1,   135,    -1,   137,    -1,   139,
     140,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,    -1,    -1,   135,    -1,   137,    -1,   139,   140,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      34,    35,    36,    -1,    38,    -1,    -1,    41,    42,    -1,
      44,    -1,    46,    47,    -1,    -1,    -1,    -1,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,   135,    -1,   137,    -1,   139,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,    36,
      -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,
     127,    -1,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,
     137,    -1,   139,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,   127,    -1,    -1,
      -1,   131,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,
      -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,   126,   127,    -1,    -1,    -1,   131,    -1,
      -1,    -1,   135,    -1,   137,   138,   139,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,
      36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    53,    54,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,   135,
      -1,   137,    -1,   139,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    30,    31,    32,    -1,    34,    -1,    36,    -1,    -1,
      -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,    -1,
     139,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,   127,    -1,    -1,    -1,   131,
      -1,    -1,    -1,   135,   136,   137,    -1,   139,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,
      -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,
      -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
     125,   126,   127,    -1,    -1,    -1,   131,    -1,    -1,    -1,
     135,    -1,   137,    -1,   139,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,    -1,
     128,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,
      -1,   139,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,
      41,    42,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,    -1,    -1,   135,    -1,   137,    -1,   139,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      34,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,
      44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,   135,    -1,   137,    -1,   139,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,    36,
      -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,
     137,    -1,   139,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,   127,    -1,    -1,
      -1,   131,    -1,    -1,    -1,   135,    -1,   137,    -1,   139,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,
      -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,   126,   127,    -1,    -1,    -1,   131,    -1,
      -1,    -1,   135,    -1,   137,    -1,   139,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,
      36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,   127,    -1,    -1,    -1,   131,    -1,    -1,    -1,   135,
      -1,   137,    -1,   139,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,
      -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,   126,    -1,    -1,
      -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,    -1,
     139,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,
      -1,    -1,    -1,   135,    -1,   137,    -1,   139,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,
      -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,
      -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
     125,   126,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,
     135,    -1,   137,    -1,   139,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,   107,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,    -1,
      -1,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,   137,
      -1,   139,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    34,    -1,    36,    -1,    -1,    -1,    -1,
      41,    42,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,    -1,
     131,    -1,    -1,    -1,   135,    -1,   137,    -1,   139,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      34,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,
      44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
      -1,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,    -1,
      -1,   135,    -1,   137,    -1,   139,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,    36,
      -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,    46,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,   126,
      -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,   135,    -1,
     137,    -1,   139,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,   105,   106,   107,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,
      -1,   131,    -1,    -1,    -1,   135,    -1,   137,    -1,   139,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,
      -1,    44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
     123,    -1,   125,   126,    -1,    -1,    -1,    -1,   131,    -1,
      -1,    -1,   135,    -1,   137,    -1,   139,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    34,    -1,
      36,    -1,    -1,    -1,    -1,    41,    42,    -1,    44,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,   105,
     106,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,   125,
     126,    -1,    -1,    -1,    -1,   131,    -1,    -1,    -1,   135,
      -1,   137,    -1,   139,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    34,    -1,    36,    -1,    -1,
      -1,    -1,    41,    42,    -1,    44,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    71,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,   104,   105,   106,   107,    -1,
      31,    32,    61,    34,    -1,    36,    65,    -1,    -1,    -1,
      41,    42,   121,    44,   123,    46,    47,   126,    -1,    -1,
      -1,    -1,   131,    54,    -1,    56,   135,    -1,   137,    -1,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,    90,
     119,   120,   121,   122,    -1,   124,    -1,   126,    -1,    -1,
     129,   130,   131,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,   123,    15,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    23,    -1,    -1,   135,    -1,   137,   138,   139,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    61,
      34,    -1,    36,    65,    -1,    -1,    -1,    41,    42,    -1,
      44,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,     3,
      54,    -1,    56,    85,    -1,    -1,    10,    89,    -1,    -1,
      -1,    15,    -1,    -1,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    -1,    -1,   129,   130,   131,
      -1,   105,    -1,    -1,    10,    -1,    -1,    -1,   140,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,   139,    90,    91,    92,    93,
      94,    95,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,    -1,    61,   110,    -1,    -1,    65,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,    -1,   126,    -1,    -1,   129,   130,   131,   132,    -1,
     134,    10,    -1,    89,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,   124,   125,
     126,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,   140,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    61,   124,   125,   126,    65,    -1,
     129,   130,   131,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,   140,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,    -1,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    61,   124,   125,   126,
      65,    -1,   129,   130,   131,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,   140,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    61,   124,
     125,   126,    65,    -1,   129,   130,   131,    -1,    -1,    10,
      -1,    -1,    -1,   138,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      61,   124,   125,   126,    65,    -1,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,   138,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    85,    -1,    -1,    -1,    89,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,    -1,    61,   129,   130,
     131,    65,    -1,    -1,   135,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    61,
     124,   125,   126,    65,    -1,   129,   130,   131,    -1,    -1,
      -1,   135,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    85,    -1,    -1,    -1,    89,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    -1,    61,   129,   130,   131,
      65,    -1,    -1,   135,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    23,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    -1,    61,   129,   130,   131,    65,    -1,    -1,
     135,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    61,   124,   125,   126,    65,
      -1,   129,   130,   131,    -1,    -1,    10,   135,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    61,   124,   125,
     126,    65,    -1,   129,   130,   131,    -1,    -1,    72,   135,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    -1,
     124,   125,   126,    -1,    61,   129,   130,   131,    65,    -1,
      -1,   135,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
     107,   108,    -1,   110,   111,   112,    -1,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,   124,   125,   126,
      -1,    61,   129,   130,   131,    65,    -1,    -1,   135,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,   124,   125,   126,    -1,    61,   129,
     130,   131,    65,    -1,    -1,   135,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,    61,   129,   130,   131,    65,
      -1,    -1,   135,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,   124,   125,
     126,    -1,    61,   129,   130,   131,    65,    -1,    -1,   135,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,   124,   125,   126,    -1,    61,
     129,   130,   131,    65,    -1,    -1,   135,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    89,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    -1,    61,   129,   130,   131,
      65,    -1,    -1,   135,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    -1,    61,   129,   130,   131,    65,    -1,    -1,
     135,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    61,   124,   125,   126,    65,
      -1,   129,   130,   131,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    10,    -1,    89,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,   104,    28,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,   124,   125,
     126,    50,    -1,   129,   130,   131,    -1,    -1,   134,    10,
      -1,    -1,    61,    -1,    15,    -1,    65,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,
      61,   110,   111,   112,    65,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    85,    -1,    10,    -1,    89,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,   104,    28,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,    50,    -1,   129,   130,
     131,    -1,    -1,    -1,    10,    -1,    -1,    61,    -1,    15,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    23,   106,   107,   108,    61,   110,   111,   112,    65,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    -1,
     124,   125,   126,    -1,    -1,   129,   130,   131,    -1,    -1,
      -1,    -1,    10,    89,    -1,    -1,    -1,    15,    -1,    61,
      -1,    -1,    -1,    65,    -1,    23,    24,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    35,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    89,   124,   125,
     126,    -1,    -1,   129,   130,   131,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    61,   106,   107,   108,    65,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    10,   128,   129,   130,   131,
      15,    89,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,    10,
      -1,   129,   130,   131,    15,    -1,    61,    -1,    -1,    -1,
      65,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    89,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,   104,
      61,   106,   107,   108,    65,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    -1,   128,   129,   130,   131,    10,    89,    -1,
      -1,    -1,    15,    -1,    61,    -1,    -1,    -1,    65,    -1,
      23,    -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    89,   124,   125,   126,    -1,   128,   129,   130,
     131,    -1,    -1,    -1,    -1,    -1,    -1,   104,    61,   106,
     107,   108,    65,   110,   111,   112,    -1,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,   124,   125,   126,
      -1,   128,   129,   130,   131,    88,    89,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,    -1,   129,   130,   131,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    61,    -1,
      -1,    -1,    65,    -1,    23,    -1,    -1,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    89,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,   104,    61,   106,   107,   108,    65,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,    -1,   129,   130,   131,    10,
      89,    -1,    -1,    -1,    15,    -1,    61,    -1,    -1,    -1,
      65,    -1,    23,    -1,    -1,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    89,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      61,   106,   107,   108,    65,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    10,   128,   129,   130,   131,    15,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,    10,   128,   129,   130,
     131,    15,    -1,    61,    -1,    -1,    -1,    65,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    89,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,   104,    61,   106,   107,
     108,    65,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,   131,    10,    89,    -1,    -1,    -1,    15,
      -1,    61,    -1,    -1,    -1,    65,    -1,    23,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    89,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    61,   106,   107,   108,    65,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,   124,   125,   126,    10,   128,   129,
     130,   131,    15,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,   124,   125,
     126,    10,   128,   129,   130,   131,    15,    -1,    61,    -1,
      -1,    -1,    65,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    89,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,
      -1,   104,    61,   106,   107,   108,    65,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,   124,   125,   126,    -1,   128,   129,   130,   131,    10,
      89,    -1,    -1,    -1,    15,    -1,    61,    -1,    -1,    -1,
      65,    -1,    23,    -1,    -1,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    89,   124,   125,   126,    -1,   128,
     129,   130,   131,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      61,   106,   107,   108,    65,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    10,   128,   129,   130,   131,    15,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,    10,   128,   129,   130,
     131,    15,    -1,    61,    -1,    -1,    -1,    65,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    89,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,   104,    61,   106,   107,
     108,    65,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
     128,   129,   130,   131,    10,    89,    -1,    -1,    -1,    15,
      -1,    61,    -1,    -1,    -1,    65,    -1,    23,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    89,
     124,   125,   126,    -1,   128,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    61,   106,   107,   108,    65,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,   124,   125,   126,    10,   128,   129,
     130,   131,    15,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,   124,   125,
     126,    -1,   128,   129,   130,   131,    -1,    -1,    61,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,   108,    -1,   110,   111,   112,
      -1,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      61,   124,   125,   126,    65,   128,   129,   130,   131,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    85,    -1,    -1,    -1,    89,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,    -1,    61,   129,   130,
     131,    65,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    61,
     124,   125,   126,    65,   128,   129,   130,   131,    -1,    -1,
      10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,    26,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    61,   124,   125,   126,    65,    -1,   129,   130,   131,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,    -1,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    61,   124,   125,   126,    65,    -1,   129,
     130,   131,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    -1,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    61,   124,   125,   126,    65,
      -1,   129,   130,   131,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      24,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    61,   124,   125,
     126,    65,    -1,   129,   130,   131,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    61,
     124,   125,   126,    65,    -1,   129,   130,   131,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    89,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    -1,    61,   129,   130,   131,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    -1,    61,   129,   130,   131,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    61,   124,   125,   126,    65,
      -1,   129,   130,   131,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,
     106,   107,   108,    -1,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,   124,   125,
     126,    -1,    61,   129,   130,   131,    65,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,   108,
      -1,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,   124,   125,   126,    -1,    61,
     129,   130,   131,    65,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    89,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    -1,    61,   129,   130,   131,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,   106,   107,   108,    -1,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
     125,   126,    -1,    61,   129,   130,   131,    65,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,   107,
     108,    -1,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,    -1,
      61,   129,   130,   131,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    89,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,
     111,   112,    -1,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,    -1,    61,   129,   130,
     131,    65,    -1,    -1,    -1,    -1,    -1,    -1,    10,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,   107,   108,    -1,   110,   111,   112,    -1,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    61,
     124,   125,   126,    65,    -1,   129,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   104,    -1,   106,   107,   108,    -1,   110,   111,
     112,    -1,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,    -1,    -1,   129,   130,   131
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   145,   146,     0,     1,     3,     4,     5,     6,     7,
       8,    12,    13,    14,    16,    17,    18,    19,    21,    22,
      24,    25,    27,    29,    30,    31,    32,    33,    34,    36,
      37,    40,    41,    42,    43,    44,    46,    47,    49,    54,
      55,    56,    57,    59,    60,    63,    64,    66,    67,    68,
      69,    70,    71,    73,    75,    76,    77,    79,    80,    84,
      86,    89,   104,   105,   106,   107,   112,   121,   123,   125,
     126,   128,   131,   135,   137,   139,   147,   148,   149,   150,
     151,   152,   157,   158,   159,   161,   164,   165,   166,   167,
     168,   169,   171,   172,   173,   174,   177,   178,   181,   184,
     187,   188,   211,   214,   215,   233,   234,   235,   236,   237,
     238,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   256,   257,   258,   259,   260,   149,   248,    85,   239,
     240,   160,   161,   239,    12,    31,    32,    34,    71,   135,
     139,   184,   233,   237,   246,   247,   248,   249,   251,   252,
      77,   160,   248,   149,   137,   161,     7,   160,   162,     9,
      77,   162,    53,    87,   170,   248,   248,    19,    30,   214,
     248,   248,   137,   161,   185,   137,   161,   216,   217,   149,
     248,   248,   248,   248,     7,   137,    30,   166,   166,   127,
     207,   227,   248,   128,   248,   248,    24,   152,   163,   248,
     248,    70,   137,   149,   248,   248,   149,   149,   161,   212,
     227,   248,   248,   248,   248,   248,   248,   248,   248,   136,
     147,   153,   227,    78,   113,   207,   228,   229,   248,   227,
     248,   255,    55,   149,    45,   161,    39,    58,   202,    20,
      53,    62,    81,   128,   134,    10,    15,    23,    61,    65,
      89,   104,   106,   107,   108,   110,   111,   112,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   124,   125,   126,
     129,   130,   131,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   132,   133,   137,
     139,    61,    65,   137,   149,   128,   135,   152,   248,   248,
     248,   227,    35,   239,   212,   128,   128,    84,   161,   207,
     230,   231,   232,   248,   135,   212,   178,   161,   137,   163,
      24,    35,   163,    24,    35,    85,   163,   242,    28,    50,
     128,    72,   152,   230,   149,   137,   196,    78,   137,   161,
     218,   219,     1,   108,   221,   222,    35,   109,   163,   230,
     162,   161,   109,   128,   128,   135,   149,   163,   137,   230,
     128,    90,   213,   128,   128,    28,    50,   163,   128,   136,
     147,   109,   136,   248,   109,   138,   109,   138,    35,   109,
     140,   242,    88,   109,   140,     7,   161,   108,   179,   189,
      62,   216,   216,   216,   216,   248,   248,   248,   248,   170,
     248,   170,   248,   248,   248,   248,   248,   248,   248,    25,
      77,   161,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   230,   230,   170,   248,   170,   248,
      20,    35,    38,    52,    53,    62,    77,   200,   241,   244,
     248,   260,    24,    35,    24,    35,    72,    35,   140,   170,
     248,   163,   128,   248,    88,    90,   138,   109,   161,   182,
     183,   128,   161,    35,   227,    34,   248,   170,   248,    34,
     248,   170,   248,   137,   163,   131,   154,   156,   248,   154,
     155,   149,   248,    26,   138,   197,   198,   199,   200,   186,
     219,   109,   138,     1,   139,   223,   234,    88,    90,   220,
     248,   217,   138,   207,   248,   175,   230,   138,   139,   223,
     234,   109,   156,   155,   136,   138,   138,   228,   138,   228,
     170,   248,   140,   149,   248,   140,   248,   140,   248,   135,
     227,   135,    20,    53,    62,    77,   191,   201,   216,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   138,
     140,    35,    62,   233,   109,   138,    61,    61,    34,   170,
     248,    34,   170,   248,   170,   248,   239,   239,   128,   248,
     207,   248,   232,    90,   109,   136,   179,   248,   138,   248,
      24,   163,    24,   163,   248,    24,   163,   242,    24,   163,
     242,   243,   244,   109,   128,    11,   128,    26,    26,   149,
     109,   138,   137,   161,    20,    53,    62,    77,   203,   138,
     219,   108,   222,   227,   248,    48,   248,    51,    82,   136,
     176,   138,   227,   212,   128,   128,   140,   242,   140,   242,
     149,    88,   136,   153,   180,    62,   190,    10,    15,    90,
      91,    92,    93,    94,    95,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   110,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   126,   129,   130,
     131,   132,   134,   137,   161,   192,   193,   194,   233,   128,
     244,   233,   233,   248,    24,    24,   248,    24,    24,   140,
     140,   163,   163,   248,   183,   135,   163,    72,    34,   248,
      34,   248,    72,    34,   248,   163,    34,   248,   163,   109,
     138,   248,   248,   149,   248,   198,   219,   108,   226,    62,
     222,   220,    35,   140,    24,   149,   227,   140,    34,   149,
     248,   140,    34,   149,   248,   140,   248,   136,    30,    57,
      59,   136,   148,   165,   203,   161,   248,   111,   137,   195,
     195,    72,    34,    34,    72,    34,    34,    34,    34,   180,
     248,   248,   248,   248,   248,   248,   244,    11,   138,    25,
      68,    71,   139,   207,   225,   234,   113,   209,   220,    83,
     210,   248,   223,   234,   149,   163,   248,   149,   248,   149,
     165,   222,   138,   161,   193,   194,   197,   248,   248,   248,
     248,   248,   248,   136,    72,    72,    72,    72,   248,   226,
     140,   227,   207,   208,   248,   248,   152,   164,   206,   140,
      72,    72,    74,   204,   195,   195,   138,    72,    72,    72,
      72,    72,    72,   248,   248,   248,   248,   209,   220,   207,
     224,   225,   234,    35,   140,   234,   248,   248,   210,   248,
     224,   225,   128,   205,   206,   140,   224
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
#line 430 "chapel.ypp"
    { yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 435 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();                                  resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 436 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); context->generatedStmt = (yyvsp[(1) - (2)].pblockstmt); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 443 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 449 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 467 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 468 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 469 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 470 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 471 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 472 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 473 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 474 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 475 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 476 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 477 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 478 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 479 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 480 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 481 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (5)].pch), new BlockStmt(), yyfilename, (yyvsp[(1) - (5)].b), (yylsp[(1) - (5)]).comment))); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 488 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pblockstmt), yyfilename, (yyvsp[(1) - (6)].b), (yylsp[(1) - (6)]).comment))); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 492 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 493 "chapel.ypp"
    { (yyval.b) = false; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 494 "chapel.ypp"
    { (yyval.b) = true; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 509 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 516 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 521 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(1) - (1)].pexpr);
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 526 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr));
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 531 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = (yyvsp[(3) - (3)].pexpr);
                                         (yyvsp[(1) - (3)].ponlylist)->push_back(cur); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 535 "chapel.ypp"
    { OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::DOUBLE;
                                         cur->renamed = new std::pair<Expr*, Expr*>((yyvsp[(3) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr));
                                         (yyvsp[(1) - (5)].ponlylist)->push_back(cur); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 550 "chapel.ypp"
    { (yyval.ponlylist) = new std::vector<OnlyRename*>();
                                         OnlyRename* cur = new OnlyRename();
                                         cur->tag = OnlyRename::SINGLE;
                                         cur->elem = new UnresolvedSymExpr("");
                                         (yyval.ponlylist)->push_back(cur); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 559 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 560 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 561 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 565 "chapel.ypp"
    { (yyval.pblockstmt) = buildRequireStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 569 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 570 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 572 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 573 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 574 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 575 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 576 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 577 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 578 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 579 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 580 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 581 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 582 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), PRIM_REDUCE_ASSIGN); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 583 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 584 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 588 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 593 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 597 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 598 "chapel.ypp"
    { (yyval.pch) = (yyvsp[(1) - (1)].pch); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 602 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 603 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 607 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 612 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 614 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 615 "chapel.ypp"
    { applyPrivateToBlock((yyvsp[(2) - (2)].pblockstmt)); (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (3)].pexpr)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), true); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].ponlylist), false); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pblockstmt) = buildForwardingDeclStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
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

  case 98:

/* Line 1806 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pblockstmt) = DoWhileStmt::build((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pblockstmt) = WhileDoStmt::build((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, false); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 656 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(  (yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pcallexpr), (yyvsp[(5) - (5)].pblockstmt), false,  true); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 657 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, false); }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 658 "chapel.ypp"
    { (yyval.pblockstmt) = ForLoop::buildForLoop(NULL, (yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt), false,  true); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), NULL, (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 661 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 662 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), zipToTuple((yyvsp[(4) - (5)].pcallexpr)), NULL, (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 663 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(5) - (6)].pForallIntents),   (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), NULL, (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 665 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pForallIntents),   (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 667 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 673 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (7)].pexpr), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)));
    }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 679 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), NULL, new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 685 "chapel.ypp"
    {
      if ((yyvsp[(2) - (7)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (7)].pcallexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (7)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (7)].pcallexpr)), (yyvsp[(5) - (7)].pForallIntents),   new BlockStmt((yyvsp[(7) - (7)].pblockstmt)), true);
    }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), NULL, new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 698 "chapel.ypp"
    {
      if ((yyvsp[(2) - (5)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (5)].pcallexpr)), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (5)].pcallexpr)->get(1)->remove(), (yyvsp[(3) - (5)].pForallIntents),   new BlockStmt((yyvsp[(5) - (5)].pblockstmt)));
    }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 707 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ZIP, (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 711 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 712 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 713 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 714 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(false, (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.pblockstmt) = TryStmt::build(true,  (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 723 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_THROW, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 727 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 731 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 732 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 739 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 748 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(2) - (6)].pch), NULL, (yyvsp[(1) - (6)].ptype), (yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt), FLAG_UNKNOWN,
                                             (yylsp[(1) - (6)]).comment));
    }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 753 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(4) - (8)].pch), (yyvsp[(2) - (8)].pch), (yyvsp[(3) - (8)].ptype), (yyvsp[(5) - (8)].pcallexpr), (yyvsp[(7) - (8)].pblockstmt), FLAG_EXTERN,
                                             (yylsp[(3) - (8)]).comment));
    }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 760 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_CLASS);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 765 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_RECORD);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 770 "chapel.ypp"
    {
             (yyval.ptype)                     = new AggregateType(AGGREGATE_UNION);
             (yyloc).comment             = context->latestComment;
             context->latestComment = NULL;
           }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 778 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 779 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 784 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 786 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 788 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 793 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 803 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
      (yyval.penumtype)->doc = context->latestComment;
      context->latestComment = NULL;
    }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 812 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 816 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 823 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 824 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 829 "chapel.ypp"
    {
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 834 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 839 "chapel.ypp"
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

  case 153:

/* Line 1806 of yacc.c  */
#line 859 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 865 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol("");
                  (yyval.pfnsymbol)->addFlag(FLAG_INLINE);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 872 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 880 "chapel.ypp"
    {
                  (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch));
                  (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);

                  (yyloc).comment             = context->latestComment;
                  context->latestComment = NULL;
                }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 891 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString.clear();
    }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 897 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 904 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[(4) - (10)].pfnsymbol);

      fn->copyFlags((yyvsp[(1) - (10)].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if (*(yyvsp[(1) - (10)].pfnsymbol)->name)
        fn->cname = (yyvsp[(1) - (10)].pfnsymbol)->name;

      if ((yyvsp[(2) - (10)].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (10)].pfnsymbol), (yyvsp[(6) - (10)].retTag), (yyvsp[(7) - (10)].pexpr), (yyvsp[(8) - (10)].b), (yyvsp[(9) - (10)].pexpr), (yyvsp[(10) - (10)].pblockstmt), (yylsp[(1) - (10)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 930 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 935 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
    }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 939 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pexpr));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 947 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 956 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 957 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 958 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 962 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 963 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 964 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 965 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 966 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 967 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 968 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 969 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 970 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 971 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 972 "chapel.ypp"
    { (yyval.pch) = "chpl_by"; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 973 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 974 "chapel.ypp"
    { (yyval.pch) = "chpl_align"; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 980 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 984 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 985 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 986 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 987 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 989 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 990 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 991 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 995 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 996 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1000 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1004 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1005 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1006 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1021 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pt) = (yyvsp[(1) - (1)].pt); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1027 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1029 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1031 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1033 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1039 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1041 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF;   }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST;   }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1043 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE;  }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1047 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1048 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1052 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1053 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1054 "chapel.ypp"
    { (yyval.retTag) = RET_CONST_REF; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1055 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1056 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1057 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1061 "chapel.ypp"
    { (yyval.b) = false; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1062 "chapel.ypp"
    { (yyval.b) = true;  }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1065 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1076 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1078 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1082 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1087 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1088 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1092 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1093 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1098 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1102 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1107 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));

      var->addFlag(FLAG_TYPE_VARIABLE);

      var->doc               = context->latestComment;
      context->latestComment = NULL;

      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));

      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 1120 "chapel.ypp"
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

  case 258:

/* Line 1806 of yacc.c  */
#line 1136 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1140 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1145 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_PARAM);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1153 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_CONST);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1161 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1169 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (5)].flag));
      flags.insert(FLAG_CONST);
      flags.insert(FLAG_REF_VAR);
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), flags, (yylsp[(1) - (5)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1178 "chapel.ypp"
    {
      std::set<Flag> flags;
      flags.insert((yyvsp[(1) - (4)].flag));
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), flags, (yylsp[(1) - (4)]).comment);
      context->latestComment = NULL;
    }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 1187 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1188 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1189 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN;  (yyloc).comment = context->latestComment; context->latestComment = NULL; }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1195 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1211 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1216 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1218 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1225 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1227 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1229 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1235 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1236 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1271 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr));
    }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr));
    }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1286 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1297 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1299 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1305 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1307 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1309 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1313 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1314 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1315 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1316 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1317 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1318 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1319 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1325 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1326 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1327 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1328 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1332 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1333 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1334 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1338 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1339 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1343 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1348 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1349 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1353 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1354 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1355 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1356 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1361 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1378 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1380 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 1382 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1384 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1386 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", buildDotExpr((yyvsp[(4) - (5)].pcallexpr)->copy(), "defaultSparseDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1388 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 338:

/* Line 1806 of yacc.c  */
#line 1390 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 339:

/* Line 1806 of yacc.c  */
#line 1395 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 1397 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 1399 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 1401 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 1403 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 1405 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 1407 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1409 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1411 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1413 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1415 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1417 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1419 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1426 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1432 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (6)].pcallexpr)), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1438 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1444 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pcallexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), zipToTuple((yyvsp[(4) - (9)].pcallexpr)), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1453 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1457 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1473 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1477 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1482 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1488 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST);
      addTaskIntent((yyval.pcallexpr), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1491 "chapel.ypp"
    { addTaskIntent((yyvsp[(1) - (3)].pcallexpr), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1495 "chapel.ypp"
    { (yyval.pForallIntents) = (yyvsp[(3) - (4)].pForallIntents); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1501 "chapel.ypp"
    { (yyval.pForallIntents) = new ForallIntents();
      addForallIntent((yyval.pForallIntents), (yyvsp[(1) - (1)].pIntentExpr).iVar, (yyvsp[(1) - (1)].pIntentExpr).tfIntent, (yyvsp[(1) - (1)].pIntentExpr).riExp); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1504 "chapel.ypp"
    { addForallIntent((yyvsp[(1) - (3)].pForallIntents), (yyvsp[(3) - (3)].pIntentExpr).iVar, (yyvsp[(3) - (3)].pIntentExpr).tfIntent, (yyvsp[(3) - (3)].pIntentExpr).riExp); }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1509 "chapel.ypp"
    {
      (yyval.pIntentExpr).tfIntent = (yyvsp[(1) - (2)].pt); (yyval.pIntentExpr).iVar = (yyvsp[(2) - (2)].pexpr); (yyval.pIntentExpr).riExp = NULL;
    }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1512 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1513 "chapel.ypp"
    { (yyval.pIntentExpr).riExp = (yyvsp[(1) - (3)].pexpr), (yyval.pIntentExpr).iVar = (yyvsp[(3) - (3)].pexpr); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1519 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1524 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1529 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1545 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1547 "chapel.ypp"
    { (yyval.pexpr) = createCast((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1549 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_bounded_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1551 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_low_bounded_range", (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1553 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_high_bounded_range", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1555 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_build_unbounded_range"); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1578 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1579 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1580 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1584 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1585 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1586 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1594 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1595 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1596 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1597 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1601 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral((yyvsp[(1) - (1)].pch));    }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1602 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1603 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral((yyvsp[(1) - (1)].pch));   }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1604 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1605 "chapel.ypp"
    { (yyval.pexpr) = buildCStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1606 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1607 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1608 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1609 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr",  (yyvsp[(2) - (4)].pcallexpr)); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1611 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1615 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (4)].pcallexpr));
    }
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1622 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1623 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1627 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1628 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1629 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1630 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1631 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1632 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1633 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1634 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1635 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1636 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1637 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1638 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1639 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1640 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1641 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1642 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1643 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1644 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("||", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1645 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1646 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1647 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl_align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1648 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1649 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1653 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1654 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1655 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1656 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1657 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1658 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 1662 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1663 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1664 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1665 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1669 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1670 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1671 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1672 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), zipToTuple((yyvsp[(3) - (3)].pcallexpr)), true); }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1677 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1678 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1679 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1680 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1681 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1682 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1683 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 8350 "bison-chapel.cpp"
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



